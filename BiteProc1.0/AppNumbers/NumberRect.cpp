// NumberRect.cpp : 实现文件
//

#include "stdafx.h"
#include "NumberRect.h"
#include "afxdialogex.h"
#include "NumberShow.h"
#include <mmsystem.h>

// CNumberRect 对话框
#pragma comment(lib,"winmm.lib")
IMPLEMENT_DYNAMIC(CNumberRect, CDialogEx)
const int MYHEIGHT = 5;
const int MYWIDTH  = 10;

CNumberRect::CNumberRect(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNumberRect::IDD, pParent)
{

}

CNumberRect::~CNumberRect()
{
}

void CNumberRect::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_ESHOWNUMBERS, m_Edit);
	CDialogEx::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CNumberRect, CDialogEx)
	ON_MESSAGE(MYWM_BITEMSG,&CNumberRect::OnMyrecmsg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CNumberRect 消息处理程序

void CNumberRect::SetWndRect(CRect rect)
{
	this->m_Wndrect = rect;
	this->MoveWindow(rect);
	CRect r;
	GetClientRect(r);
	m_Edit.MoveWindow(r);
}

CRect CNumberRect::GetWndRect()
{
	return m_Wndrect;
}

afx_msg LRESULT CNumberRect::OnMyrecmsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == MYWM_BITEMSG)
	{
		CNumberShow* pView = (CNumberShow*)GetParent();
		if (pView == NULL)
			return 1;
		pView->PopNumbers();
		Invalidate(TRUE);
	}
	return 0;
}

BOOL CNumberRect::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CWnd*   pEdit;
	pEdit = GetDlgItem(IDC_ESHOWNUMBERS);
	CRect   rect;
	GetClientRect(&rect);
	NumbersFont.CreateFont(rect.Height() / (MYHEIGHT), rect.Width() / (2 * MYWIDTH), 0, 0, FW_NORMAL, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial"));
	pEdit->SetFont(&NumbersFont);

	return TRUE;
}

void CNumberRect::OnSetFont(CFont* pFont)
{
	CDialogEx::OnSetFont(pFont);
}

void CNumberRect::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CNumberShow *pView = (CNumberShow*)GetParent();
	if (pView == NULL)
		return;
	m_Curstrings = pView->GetNumbers();			//获得缓存区中的字符
	m_Edit.SetWindowTextW(m_Curstrings);
	m_Edit.SetSel(m_Curstrings.GetLength(), m_Curstrings.GetLength(), TRUE);
	m_Edit.SendMessage(WM_VSCROLL, SB_BOTTOM);
	m_Edit.SetFocus();
	if (pView->GetTaskNumbers().GetLength() > 0)
	{
		if (m_Curstrings.GetLength() == 1)
		{
			m_nStartTime = timeGetTime();
		}
		if (m_Curstrings.GetLength() == pView->GetTaskNumbers().GetLength())
		{
			double c = CheckCollect(pView->GetTaskNumbers(), m_Curstrings);
			m_nEndTime = timeGetTime();
			int interval = m_nEndTime - m_nStartTime;
			CString mes;
			mes.Format(_T(" total char:%d \n total time:%d \n mean time:%d \n corect:%lf"),
				m_Curstrings.GetLength(), interval, interval / m_Curstrings.GetLength(), c*100.0);
			AfxMessageBox(mes);
		}
	}
}

void CNumberRect::OnDestroy()
{
	CDialogEx::OnDestroy();
	NumbersFont.DeleteObject();
}

BOOL CNumberRect::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

double CNumberRect::CheckCollect(CString dest, CString str)
{
	int count = 0;
	for (int i = 0; i < dest.GetLength(); i++)
	{
		char c1 = dest.GetAt(i);
		char c2 = str.GetAt(i);
		if (c1 == c2)
			count++;
	}
	return count*1.0 / dest.GetLength();
}