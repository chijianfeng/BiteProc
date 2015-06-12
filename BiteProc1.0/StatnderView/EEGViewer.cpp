// EEGViewer.cpp : 实现文件
//

#include "stdafx.h"
#include "StatnderView.h"
#include "EEGViewer.h"
#include "SignalView.h"

// CEEGViewer

IMPLEMENT_DYNAMIC(CEEGViewer, CWnd)

CEEGViewer::CEEGViewer()
{
	m_pSignalView = NULL;
	m_Maxv = m_Minv = 0;
}

CEEGViewer::~CEEGViewer()
{
}


BEGIN_MESSAGE_MAP(CEEGViewer, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CEEGViewer::SetEEGData(ManagedData& data, ManagedData& color)
{
	if (!m_pSignalView)
		return;
	if (m_AutoMaxMin)
	{
		if (m_Maxv == 0 && m_Minv == 0)
		{
			double a, b;
			data.GetMaxMin(a, b);
			m_Maxv = (int)a;
			m_Minv = (int)b;
		}
	}
	for (size_t i = 0; i < m_Channum;i++)
	{
		ManagedData d;
		ManagedData c;
		data.GetAnydata(d, (int)i);
		color.GetAnydata(c, (int)i);
		m_pSignalView[i].SetMaxMin(m_Maxv, m_Minv);
		m_pSignalView[i].UpdateData(d, c);
	}
}

void CEEGViewer::SetChannelNum(const size_t num)
{
	m_Channum = num;
}

void CEEGViewer::SetClientSize(const CRect rect)
{
	m_Rect = rect;
}

void CEEGViewer::SetMaxMin(const int Maxv, const int Minv)
{
	m_Maxv = Maxv;
	m_Minv = Minv;
}

void CEEGViewer::GetCurMaxMin(int& Maxv, int& Minv)
{
	Maxv = m_Maxv;
	Minv = m_Minv;
}

void CEEGViewer::AutoMaxMin(bool b)
{
	m_AutoMaxMin = b;
}

void CEEGViewer::UpdateAll()
{
	//add code here
}

void CEEGViewer::OnMove(int x, int y)
{
	__super::OnMove(x, y);

	// TODO:  在此处添加消息处理程序代码
}

// CEEGViewer 消息处理程序
void CEEGViewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (m_pSignalView == NULL)
		return;
	for (size_t i = 0; i < m_Channum;i++)
	{
		m_pSignalView[i].SendMessage(WM_PAINT);
	}
}

int CEEGViewer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  添加Signal view
	if (m_pSignalView)
		return 0;
	m_pSignalView = new CSignalView[m_Channum];
	for (size_t i = 0; i < m_Channum;i++)
	{
		CRect tmprect = CRect(m_Rect.left, m_Rect.top + m_Rect.Height() / m_Channum*i, m_Rect.right,
			m_Rect.top + m_Rect.Height() / m_Channum*(i + 1));
		m_pSignalView[i].Create(NULL,
			NULL,
			WS_CHILD | WS_VISIBLE,
			tmprect,
			this,
			i + 20000);
		m_pSignalView[i].SetMaxMin(m_Maxv, m_Minv);
	}
	return 0;
}


void CEEGViewer::OnDestroy()
{
	//add release code.
	if (!m_pSignalView)
	{
		for (size_t i = 0; i < m_Channum;i++)
		{
			m_pSignalView[i].SendMessage(WM_DESTROY);
			delete m_pSignalView[i];
		}
		delete[] m_pSignalView;
		m_pSignalView = NULL;
	}
	__super::OnDestroy();
}


void CEEGViewer::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if (!m_pSignalView)
		return;
	m_Rect.bottom = cy;
	m_Rect.right = cx;
	for (size_t i = 0; i < m_Channum;i++)
	{
		CRect tmprect = CRect(m_Rect.left, m_Rect.top + m_Rect.Height() / m_Channum*i, m_Rect.right,
			m_Rect.top + m_Rect.Height() / m_Channum*(i + 1));
		m_pSignalView[i].MoveWindow(tmprect);
	}
}


//outside func
//extern function：该函数不可修改
extern "C"  _declspec(dllexport) char* GetEEGViewerModualMsg()					//获取模块信息
{
	return "显示标准的EEG 数据";
}

extern "C" _declspec(dllexport) CEEGViewer* CreateViewer()
{
	CEEGViewer* pViewer = new CEEGViewer();
	return pViewer;
}

extern "C" _declspec(dllexport) BOOL CreateEEGViewerWnd(CWnd* parent , CWnd* pViewer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());			//for MFC dll stander
	if (pViewer == NULL)
		return FALSE;
	BOOL b = pViewer->Create(NULL,
		NULL,
		WS_CHILD | WS_VISIBLE, 
		CRect(0,0,800,800), parent,0xFFF1);					//ID 号
	return b;
}

