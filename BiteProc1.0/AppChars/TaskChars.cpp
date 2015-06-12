// TaskChars.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskChars.h"
#include "afxdialogex.h"
#include"CharShow.h"

// CTaskChars 对话框

IMPLEMENT_DYNAMIC(CTaskChars, CDialogEx)
const int MYHEIGHT1 = 4;
const int MYWIDTH1 = 10;
CTaskChars::CTaskChars(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskChars::IDD, pParent)
{
	m_nCurString = _T("");
	m_Wndrect = CRect(0,0,0,0);
	m_nEditStr = _T("");
}

CTaskChars::~CTaskChars()
{
}

void CTaskChars::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX , IDC_ETASKCHARS , m_Edit);
	DDX_Text(pDX , IDC_ETASKCHARS , m_nEditStr);
}


BEGIN_MESSAGE_MAP(CTaskChars, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTaskChars 消息处理程序


BOOL CTaskChars::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CWnd*   pEdit = NULL;
	pEdit = GetDlgItem(IDC_ETASKCHARS);
	
	if(pEdit!=NULL)
	{
		CRect rect;
		GetClientRect(&rect);
		CharsFont.CreateFont(rect.Height()/(MYHEIGHT1+1),rect.Width()/(2*MYWIDTH1),0,0,FW_NORMAL,0,0,0,
			DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, _T("Arial"));
		pEdit->SetFont(&CharsFont);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTaskChars::SetWndRect(CRect rect)
{
	this->m_Wndrect = rect;
	this->MoveWindow(rect);
	CRect r;
	GetClientRect(r);
	m_Edit.MoveWindow(m_Wndrect);
}

CString CTaskChars::GetTaskChars()
{
	UpdateData(TRUE);
	return m_nEditStr;
}

CRect CTaskChars::GetWndRect()
{
	return m_Wndrect;
}

void CTaskChars::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CCharShow *pView = (CCharShow *)GetParent();
	if(pView==NULL)
		return;
	m_nCurString = pView->GetTaskChars();
	m_Edit.SetWindowTextW(m_nCurString);
}


HBRUSH CTaskChars::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->m_hWnd==(GetDlgItem(IDC_ETASKCHARS)->m_hWnd))
	{
		pDC->SetTextColor(RGB(128,128,128));
	}
	return hbr;
}


void CTaskChars::OnDestroy()
{
	CDialogEx::OnDestroy();

	CharsFont.DeleteObject();
}

