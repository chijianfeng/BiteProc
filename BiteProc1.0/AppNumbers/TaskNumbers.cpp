// TaskNumbers.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskNumbers.h"
#include "afxdialogex.h"
#include "NumberShow.h"


// CTaskNumbers 对话框

IMPLEMENT_DYNAMIC(CTaskNumbers, CDialogEx)
const int MYHEIGHT = 4;
const int MYWIDTH  = 10;

CTaskNumbers::CTaskNumbers(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskNumbers::IDD, pParent)
	, m_nEditStr(_T(""))
{
	m_Wndrect = CRect(0,0,0,0);
	m_nCurString = _T("");
}

CTaskNumbers::~CTaskNumbers()
{
}

void CTaskNumbers::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ETASKNUMBERS, m_nEditStr);
	DDX_Control(pDX, IDC_ETASKNUMBERS, m_Edit);
}


BEGIN_MESSAGE_MAP(CTaskNumbers, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTaskNumbers 消息处理程序

BOOL CTaskNumbers::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CWnd*   pEdit = NULL;
	pEdit = GetDlgItem(IDC_ETASKNUMBERS);

	if (pEdit != NULL)
	{
		CRect rect;
		GetClientRect(&rect);
		NumbersFont.CreateFont(rect.Height() / (MYHEIGHT + 1), rect.Width() / (2 * MYWIDTH), 0, 0, FW_NORMAL, 0, 0, 0,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial"));
		pEdit->SetFont(&NumbersFont);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTaskNumbers::SetWndRect(CRect rect)
{
	this->m_Wndrect = rect;
	this->MoveWindow(rect);
	CRect r;
	GetClientRect(r);
	m_Edit.MoveWindow(m_Wndrect);
}

CString CTaskNumbers::GetTaskNumbers()
{
	UpdateData(TRUE);
	return m_nEditStr;
}

CRect CTaskNumbers::GetWndRect()
{
	return m_Wndrect;
}

void CTaskNumbers::OnPaint()
{
	CPaintDC dc(this);
	CNumberShow *pView = (CNumberShow*)GetParent();
	if (pView == NULL)
		return;
	m_nCurString = pView->GetTaskNumbers();
	m_Edit.SetWindowTextW(m_nCurString);
}

HBRUSH CTaskNumbers::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->m_hWnd == (GetDlgItem(IDC_ETASKNUMBERS)->m_hWnd))
	{
		pDC->SetTextColor(RGB(128, 128, 128));
	}
	return hbr;
}

void CTaskNumbers::OnDestroy()
{
	CDialogEx::OnDestroy();

	NumbersFont.DeleteObject();
}