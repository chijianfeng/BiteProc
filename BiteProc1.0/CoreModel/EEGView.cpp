// EEGView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OutputWnd.h"
#include "CoreModel.h"
#include "EEGView.h"
#include "CoreModelDoc.h"
#include "ViewerSelDlg.h"
#include "ViewerConfig.h"


// CEEGView

IMPLEMENT_DYNCREATE(CEEGView, CScrollView)

CEEGView::CEEGView()
{
	SetScrollSizes(MM_TEXT, CSize(0, 0));
}

CEEGView::~CEEGView()
{
}


BEGIN_MESSAGE_MAP(CEEGView, CScrollView)
	ON_WM_PAINT()
	ON_COMMAND(ID_SETTING_EEGVIEWER, &CEEGView::OnSettingEegviewer)
	ON_COMMAND(ID_START, &CEEGView::OnStart)
	ON_WM_SIZE()
	ON_COMMAND(ID_ZOOMIN, &CEEGView::OnZoomin)
	ON_COMMAND(ID_ZOOMOUT, &CEEGView::OnZoomout)
	ON_COMMAND(ID_STOP, &CEEGView::OnStop)
	ON_COMMAND(ID_PAUSE, &CEEGView::OnPause)
	ON_COMMAND(ID_DRAWFLAGE,&CEEGView::OnSaveData)
END_MESSAGE_MAP()


// CEEGView ��ͼ

void CEEGView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CRect rect;
	GetClientRect(rect);

	CSize sizeTotal;

	CCoreModelDoc* pDoc = (CCoreModelDoc*)GetDocument();

	CString path = pDoc->GetCurUser()+_T("\\")+_T(VIEWFILE);
	CViewerConfig viewconfig(path);
	if (!viewconfig.IsOpen())			//δ���ļ�
	{
		m_wndOutput << "δ����EEG Viewer" << m_EndOut;
		sizeTotal.cx = 100;
		sizeTotal.cy = 100;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}
	else
	{
		if (!viewconfig.LoadFileMsg())
			m_wndOutput << "�޷���ȡ�ļ���" << path << " ����" << m_EndOut;
		viewconfig.CloseFile();

		CString name = L"";
		if (viewconfig.GetSelDllPath().ReverseFind('\\') == -1)
		{
			name = viewconfig.GetSelDllPath();
		}
		else
		{
			name = viewconfig.GetSelDllPath().Right(viewconfig.GetSelDllPath().GetLength() - 1 -
				viewconfig.GetSelDllPath().ReverseFind('\\'));	//���dll����
		}

		int len, width;
		len = viewconfig.GetWindowLength();
		width = viewconfig.GetWindowWidth();
		// TODO:  �������ͼ�ĺϼƴ�С
		sizeTotal.cx = len;
		sizeTotal.cy = width*pDoc->GetChanNum();
		SetScrollSizes(MM_TEXT, sizeTotal);
		if (!pDoc->RegEEGViewer(viewconfig.GetSelDllPath(), this))
			m_wndOutput << "����EEG Viewer:" << name << "ʧ��" << m_EndOut;
		else
		{
			m_wndOutput << "����EEG Viewer:" << name << "�ɹ�" << m_EndOut;
			CBaseEEGViewerWnd* pEEGView = pDoc->GetEEGViewer();
			int maxv, minv;
			if (viewconfig.GetIsAutoMaxMin())
				pEEGView->AutoMaxMin(true);										//�Զ�������ʾ����
			else
			{
				pEEGView->AutoMaxMin(false);
				maxv = viewconfig.GetMaxValue();
				minv = viewconfig.GetMinValue();
				pEEGView->SetMaxMin(maxv, minv);
			}
		}
	}

	CBaseEEGViewerWnd* pEEGView = pDoc->GetEEGViewer();
	if (!pEEGView)
	{
		m_wndOutput << "EEG ViewerΪ�գ���Ч" << m_EndOut;
		return;
	}

	rect.right = sizeTotal.cx;
	rect.bottom = sizeTotal.cy;
	pEEGView->MoveWindow(rect);
}

void CEEGView::OnDraw(CDC* pDC)
{
	CCoreModelDoc* pDoc = (CCoreModelDoc*)GetDocument();
	if (!pDoc)
		return;
	CBaseEEGViewerWnd* pEEGView = pDoc->GetEEGViewer();
	// TODO:  �ڴ���ӻ��ƴ���
	ManagedData data, color;
	pDoc->GetNewData(data , color);								//update the EEG view
	if (pEEGView)
	{
		pEEGView->SetEEGData(data, color);
		pEEGView->SendMessage(WM_PAINT);
	}
}

void CEEGView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* pHint)
{
	if (pHint)
	{
		if (pHint->IsKindOf(RUNTIME_CLASS(UpdateRng)))
		{
			UpdateRng* rng = (UpdateRng*)pHint;
			if (rng->m_bUpDateEEG)
			{
				//���¾ֲ�
				rng->m_bUpDateEEG = FALSE;
				CDC *pDC = GetDC();
				OnDraw(pDC);
				ReleaseDC(pDC);
				return;
			}
			if (rng->m_bUpdataFrame)
				return;
		}
	}
	Invalidate(TRUE);
}
// CEEGView ���

#ifdef _DEBUG
void CEEGView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CEEGView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEEGView ��Ϣ�������


void CEEGView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CScrollView::OnPaint()
}



void CEEGView::OnSettingEegviewer()
{
	// TODO: ���ء��޸�EEG Viewer ģ��
	CCoreModelDoc* pDoc = (CCoreModelDoc*)GetDocument();
	CViewerSelDlg dlg(pDoc->GetCurUser() , this);
	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetSelDllPath();
		CString name = path.Right(path.GetLength() - 1 - path.ReverseFind('\\'));

		//��ɾ��ԭ��ģ��
		if (pDoc->GetEEGViewer())
		{
			pDoc->DeleteEEGViewer();
			m_wndOutput << "ɾ��ԭEEG��ͼ" << m_EndOut;
		}
		
		if (!pDoc->RegEEGViewer(path, this))
			m_wndOutput << "����EEG Viewer:" << name << "ʧ��" << m_EndOut;
		else
		{
			m_wndOutput << "����EEG Viewer:" << name << "�ɹ�" << m_EndOut;
			CBaseEEGViewerWnd* pEEGView = pDoc->GetEEGViewer();
			int maxv, minv;
			if (dlg.IsAutoMaxMin())
				pEEGView->AutoMaxMin(true);										//�Զ�������ʾ����
			else
			{
				pEEGView->AutoMaxMin(false);
				dlg.GetMaxMin(maxv, minv);
				pEEGView->SetMaxMin(maxv, minv);
			}
			CRect rect;//������ͼ��С
			int len, width;
			dlg.GetSize(len, width);
			rect.left = rect.top = 0;
			rect.right = len;
			rect.bottom = width*pDoc->GetChanNum();
			pEEGView->MoveWindow(rect);
		}
	}
	else
		m_wndOutput << "δ����EEG Viewer" << m_EndOut;
}

void CEEGView::OnStart()//��ʼ�ɼ�
{
	CCoreModelDoc* pDoc = (CCoreModelDoc*)GetDocument();
	if (!pDoc->StartGetDataThread())
	{
		m_wndOutput << "���ݲɼ��߳�����ʧ��" << m_EndOut;
	}
}

void CEEGView::OnZoomin()
{
	CCoreModelDoc* pDoc = (CCoreModelDoc*)GetDocument();
	CBaseEEGViewerWnd* pEEGViewer = pDoc->GetEEGViewer();
	if (!pEEGViewer)
	{
		m_wndOutput << "EEG��ͼ������,�޷���С" << m_EndOut;
	}
	else
	{
		int maxv, minv;
		pEEGViewer->GetCurMaxMin(maxv, minv);
		maxv = (int)(maxv*1.5);
		minv = (int)(minv*1.5);
		pEEGViewer->SetMaxMin(maxv, minv);
	}
}


void CEEGView::OnZoomout()
{
	CCoreModelDoc* pDoc = (CCoreModelDoc*)GetDocument();
	CBaseEEGViewerWnd* pEEGViewer = pDoc->GetEEGViewer();
	if (!pEEGViewer)
	{
		m_wndOutput << "EEG��ͼ������,�޷��Ŵ�" << m_EndOut;
	}
	else
	{
		int maxv, minv;
		pEEGViewer->GetCurMaxMin(maxv, minv);
		maxv = (int)(maxv*0.66);
		minv = (int)(minv*0.66);
		pEEGViewer->SetMaxMin(maxv, minv);
	}
}


void CEEGView::OnStop()
{
	CCoreModelDoc* pDoc = (CCoreModelDoc*)GetDocument();
	if (!pDoc->EndGetDataThread())
	{
		m_wndOutput << "�����ɼ��̣߳�����" << m_EndOut;
	}
}


void CEEGView::OnPause()
{
	CCoreModelDoc* pDoc = (CCoreModelDoc*)GetDocument();
	pDoc->PauseGetDataThead();
}

void CEEGView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	CCoreModelDoc* pDoc = (CCoreModelDoc*)GetDocument();
	CRect rect;
	GetClientRect(&rect);

}

void CEEGView::OnSaveData()
{
	CCoreModelDoc *pDoc = (CCoreModelDoc*)GetDocument();
	CBaseEEGViewerWnd* pEEGViewer = pDoc->GetEEGViewer();

	if (!pDoc->SaveData())
	{
		m_wndOutput << "������ļ������ڣ��޷���������" << m_EndOut;
		return;
	}

	if (pEEGViewer == NULL)
	{
		m_wndOutput << "EEG��ͼ������,�޷���������" << m_EndOut;
		return;
	}
	//pEEGViewer->SetState(pDoc->SendFlagState());
}