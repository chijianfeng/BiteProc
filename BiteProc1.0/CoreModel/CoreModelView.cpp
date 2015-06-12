
// CoreModelView.cpp : CCoreModelView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "CoreModel.h"
#endif

#include "CoreModelDoc.h"
#include "CoreModelView.h"
#include "OutputWnd.h"
#include "AppConfig.h"
#include"../AppWheelChair/WheelChairShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCoreModelView

IMPLEMENT_DYNCREATE(CCoreModelView, CView)

BEGIN_MESSAGE_MAP(CCoreModelView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_MESSAGE(MYWM_TRAINWMD , OnRcvTrainMsg)
END_MESSAGE_MAP()

// CCoreModelView ����/����

CCoreModelView::CCoreModelView()
{
	// TODO:  �ڴ˴���ӹ������

}

CCoreModelView::~CCoreModelView()
{
}

BOOL CCoreModelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CCoreModelView ����

void CCoreModelView::OnDraw(CDC* /*pDC*/)
{
	CCoreModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

void CCoreModelView::SendDatatoWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CBaseTrainWnd* pWnd = pDoc->GetBaseTrainWnd();				//��ȡ��ǰ��ѵ������
	if (!pWnd)													//������Ч��������
		return;
	ManagedData data ,color;
	pDoc->GetNewData(data, color);
	pWnd->SetData(data);										//���͸�����
}

void CCoreModelView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCoreModelView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCoreModelView ���

#ifdef _DEBUG
void CCoreModelView::AssertValid() const
{
	//CView::AssertValid();
}

void CCoreModelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCoreModelDoc* CCoreModelView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCoreModelDoc)));
	return (CCoreModelDoc*)m_pDocument;
}
#endif //_DEBUG


// CCoreModelView ��Ϣ�������


void CCoreModelView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ��ʼ�������ڼ���ѵ��ģ���Լ�Ӧ��ģ��
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	LoadBiteTrainWnd();
}

void CCoreModelView::LoadBiteTrainWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CString algpath = pDoc->GetCurUser() + "\\" + CONFIGFILE;	//�����㷨����·��
	CAlgParamConfig config(algpath);
	if (!config.LoadParam())									//���ز������ڴ�
	{
		m_wndOutput << "������ֵѵ������ʧ�ܣ��޷���ȡ�����ļ�" << m_EndOut;
		config.CloseParmFile();
		return;
	}
	else
		config.CloseParmFile();
	//check the dll module path.
	CString dllpath = config.GetBiteModulePath();
	if (dllpath.GetLength() <= 0)
	{
		m_wndOutput << "������ֵѵ������ʧ�ܣ�ģ��·��������" << m_EndOut;
		return;
	}
	if (!config.GetBitestate())
	{
		if (!pDoc->AppendAlgModel(dllpath))						//��������ģ��
		{
			m_wndOutput << "������ֵģ��ʧ��" << m_EndOut;
			return;
		}
		m_wndOutput << "������ֵģ�ͳɹ�" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_BITEDONE);
		return;
	}
	if (!pDoc->RegTrainModule(dllpath, this))
	{
		m_wndOutput << "������ֵѵ������ʧ�ܣ�����ע��ʧ��" << m_EndOut;
		return;
	}
	m_wndOutput << "������ֵѵ������ģ��ɹ�" << m_EndOut;
}

void CCoreModelView::LoadClassifyTrainWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CString algpath = pDoc->GetCurUser() + "\\" + CONFIGFILE;	//�����㷨����·��
	CAlgParamConfig config(algpath);
	if (!config.LoadParam())									//���ز������ڴ�
	{
		m_wndOutput << "���ط���ѵ������ʧ�ܣ��޷���ȡ�����ļ�" << m_EndOut;
		config.CloseParmFile();
		return;
	}
	else
		config.CloseParmFile();
	//check the dll module path.
	CString dllpath = config.GetClassifyModule();
	if (dllpath.GetLength() <= 0)
	{
		m_wndOutput << "���ط���ѵ������ʧ�ܣ�ģ��·��������" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_CLASSIFYCANCLE);
		return;
	}

	if (!config.GetClassifystate()&&!config.IsRetrainClassify())
	{
		if (!pDoc->AppendAlgModel(dllpath))						//��������ģ��
		{
			m_wndOutput << "���ط���ģ��ʧ��" << m_EndOut;
			return;
		}
		m_wndOutput << "���ط���ģ�ͳɹ�" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_CLASSIFYDONE);
		return;
	}
	if (!pDoc->RegTrainModule(dllpath, this))
	{
		m_wndOutput << "���ط���ѵ������ʧ�ܣ�����ע��ʧ��" << m_EndOut;
		return;
	}
	m_wndOutput << "���ط���ѵ������ģ��ɹ�" << m_EndOut;

}

void CCoreModelView::LoadHLTrainWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CString algpath = pDoc->GetCurUser() + "\\" + CONFIGFILE;	//�����㷨����·��
	CAlgParamConfig config(algpath);
	if (!config.LoadParam())									//���ز������ڴ�
	{
		m_wndOutput << "��������ѵ������ʧ�ܣ��޷���ȡ�����ļ�" << m_EndOut;
		config.CloseParmFile();
		return;
	}
	else
		config.CloseParmFile();
	//check the dll module path.
	CString dllpath = config.GetHLModule();
	if (dllpath.GetLength() <= 0)
	{
		m_wndOutput << "��������ѵ������ʧ�ܣ�ģ��·��������" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_HLCANCLE);
		return;
	}

	if (!config.GetHLState()&&!config.GetHLReTrain())
	{
		if (!pDoc->AppendAlgModel(dllpath))						//��������ģ��
		{
			m_wndOutput << "��������ģ��ʧ��" << m_EndOut;
			return;
		}
		m_wndOutput << "��������ģ�ͳɹ�" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_HLDONE);
		return;
	}
	if (!pDoc->RegTrainModule(dllpath, this))
	{
		m_wndOutput << "��������ѵ������ʧ�ܣ�����ע��ʧ��" << m_EndOut;
		return;
	}
	m_wndOutput << "��������ѵ������ģ��ɹ�" << m_EndOut;
}

void CCoreModelView::LoadTLTrainWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CString algpath = pDoc->GetCurUser() + "\\" + CONFIGFILE;	//�����㷨����·��
	CAlgParamConfig config(algpath);
	if (!config.LoadParam())									//���ز������ڴ�
	{
		m_wndOutput << "���س�ҧ������ѵ������ʧ�ܣ��޷���ȡ�����ļ�" << m_EndOut;
		config.CloseParmFile();
		return;
	}
	else
		config.CloseParmFile();
	//check the dll module path.
	CString dllpath = config.GetTLModule();
	if (dllpath.GetLength() <= 0)
	{
		m_wndOutput << "���س�ҧ������ѵ������ʧ�ܣ�ģ��·��������" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_TLCANCLE);
		return;
	}

	if (!config.GetTLTrainState()&&!config.GetTLReTrain())
	{
		if (!pDoc->AppendAlgModel(dllpath))						//��������ģ��
		{
			m_wndOutput << "���س�ҧ������ģ��ʧ��" << m_EndOut;
			return;
		}
		m_wndOutput << "���س�ҧ������ģ�ͳɹ�" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_TLDONE);
		return;
	}
	if (!pDoc->RegTrainModule(dllpath, this))
	{
		m_wndOutput << "���س�ҧ������ѵ������ʧ�ܣ�����ע��ʧ��" << m_EndOut;
		return;
	}
	m_wndOutput << "���س�ҧ������ѵ������ģ��ɹ�" << m_EndOut;
}

void CCoreModelView::LoadAppWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CString Apppath = pDoc->GetCurUser() + "\\" + APPFILE;	//�����㷨����·��
	CAppConfig config(Apppath);
	if (!config.IsOpen())
	{
		m_wndOutput << "����Ӧ�ô���ʧ�ܣ��޷���ȡ�����ļ�" << m_EndOut;
		return;
	}
	config.CloseFile();
	CString dllpath = config.GetModulePath();				//��ȡģ��·��
	if (dllpath.GetLength() <= 0)
	{
		m_wndOutput << "����Ӧ�ô���ʧ�ܣ�ģ��·��������" << m_EndOut;
		return;
	}
	CRect rect;
	rect.left = rect.top = 0;
	rect.right = config.GetWidth();							//��ȡ����
	rect.bottom = config.GetHeight();

	if (!pDoc->RegAppModule(dllpath, this, rect))
	{
		m_wndOutput << "����Ӧ�ô���ʧ�ܣ�����ע��ʧ��" << m_EndOut;
		return;
	}
	m_wndOutput << "����Ӧ�ô���ģ��ɹ�" << m_EndOut;
	//���������߳�
	if (!pDoc->StartPrcessThread())
	{
		m_wndOutput << "�޷����������̣߳��޷�ʵ�����ݴ���" << m_EndOut;
	}
	else
		m_wndOutput << "���������̳߳ɹ�" << m_EndOut;
}

int CCoreModelView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//add init once code here

	return 0;
}

void CCoreModelView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* pHint)
{
	if (pHint)
	{
		if (pHint->IsKindOf(RUNTIME_CLASS(UpdateRng)))
		{
			UpdateRng* rng = (UpdateRng*)pHint;
			if (rng->m_bUpdataFrame)
			{
				//ѵ����Ӧ�����ݸ���
				rng->m_bUpdataFrame = FALSE;
				//
				SendDatatoWnd();
				return;
			}
			if (rng->m_bUpDateEEG)
				return;
		}
	}
	Invalidate(TRUE);
}

LRESULT CCoreModelView::OnRcvTrainMsg(WPARAM wparam, LPARAM lparam)
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return 1;
	switch (wparam)
	{
	case TRAINWND_BITEDONE:
		if (!pDoc->CloseTrainModule())
			m_wndOutput << "�Ƴ�ѵ��ģ��ʧ��" << m_EndOut;
		LoadClassifyTrainWnd();										//������һ��ѵ������
		break;
	case TRAINWND_BITECANCLE:										//����ȡ��ѵ��
		m_wndOutput << "ȡ����ֵģ��ѵ���������Զ�����ԭ��ģ��" << m_EndOut;
		LoadClassifyTrainWnd();
		break;
	case TRAINWND_CLASSIFYDONE:
		if (!pDoc->CloseTrainModule())							//�رյ�ǰģ�ʹ���
			m_wndOutput << "�Ƴ�����ѵ��ģ��ʧ��" << m_EndOut;
		LoadHLTrainWnd();										//������һ��ѵ������
		break;
	case TRAINWND_CLASSIFYCANCLE:
		m_wndOutput << "ȡ������ģ��ѵ���������Զ�����ԭ��ģ��" << m_EndOut;
		LoadHLTrainWnd();
		break;
	case TRAINWND_HLDONE:
		if (!pDoc->CloseTrainModule())							//�رյ�ǰģ�ʹ���
			m_wndOutput << "�Ƴ�����ѵ��ģ��ʧ��" << m_EndOut;
		LoadTLTrainWnd();
		break;
	case TRAINWND_HLCANCLE:
		m_wndOutput << "ȡ������ģ��ѵ���������Զ�����ԭ��ģ��" << m_EndOut;
		LoadTLTrainWnd();
		break;
	case TRAINWND_TLDONE:
		if (!pDoc->CloseTrainModule())							//�رյ�ǰģ�ʹ���
			m_wndOutput << "�Ƴ���ҧ������ѵ��ģ��ʧ��" << m_EndOut;
		LoadAppWnd();
		break;
	case TRAINWND_TLCANCLE:
		m_wndOutput << "ȡ����ҧ������ģ��ѵ���������Զ�����ԭ��ģ��" << m_EndOut;
		LoadAppWnd();
	case  TRAINWND_TEST:
		//��ǰ����ģ�飬�������ģʽ
		if (!pDoc->StartPrcessThread())
			m_wndOutput << "�޷��������ݴ����߳�" << m_EndOut;
		else
			m_wndOutput << "�������ݴ����̳߳ɹ�" << m_EndOut;
		break;
	case TRAINWND_TESTFINISH:
		if(!pDoc->SuspendProcessThread())
			m_wndOutput << "�޷�ֹͣ���ݴ����߳�" << m_EndOut;
		else
			m_wndOutput << "ֹͣ���ݴ����̳߳ɹ�" << m_EndOut;
		break;
	default:
		break;
	}
	return 0;
}


BOOL CCoreModelView::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)						//ģ��VK ����
	{
		CCoreModelDoc* pDoc = GetDocument();
		if (!pDoc)
			return TRUE;
		CBaseAppWnd* pApp = pDoc->GetCurApp();
		if (!pApp)
		{
			m_wndOutput << "Ӧ�ô�����Ч���޷�����" << m_EndOut;
			return TRUE;
		}
		pApp->PreTranslateMessage(pMsg);
		//return TRUE;
	}

	return CView::PreTranslateMessage(pMsg);
}
