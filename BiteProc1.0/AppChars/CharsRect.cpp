// CharsRect.cpp : 实现文件
//

#include "stdafx.h"
#include "AppChars.h"
#include "CharsRect.h"
#include "afxdialogex.h"
#include "CharShow.h"
#include <Mmsystem.h>
// CCharsRect 对话框
#pragma comment( lib,"winmm.lib" )
IMPLEMENT_DYNAMIC(CCharsRect, CDialogEx)
const int MYHEIGHT = 5;
const int MYWIDTH = 10;

//class CCharShow;

CCharsRect::CCharsRect(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCharsRect::IDD, pParent)
{

}

CCharsRect::~CCharsRect()
{
}

void CCharsRect::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_ESHOWCHARS, m_Edit);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCharsRect, CDialogEx)
	ON_MESSAGE(MYWM_LONGBITEMSG, &CCharsRect::OnMyrecmsg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CCharsRect::SetWndRect(CRect rect)
{
	this->m_Wndrect = rect;
	this->MoveWindow(rect);
	CRect r;
	GetClientRect(r);
	m_Edit.MoveWindow(r);
}

CRect CCharsRect::GetWndRect()
{
	return m_Wndrect;
}

// CCharsRect 消息处理程序

afx_msg LRESULT CCharsRect::OnMyrecmsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam==MYWM_LONGBITEMSG)
	{
		CCharShow *pView = (CCharShow *)GetParent();
		if (pView == NULL)
			return 1;
		pView->PopChars();						//删除一个字符
		Invalidate(TRUE);
	}
	return 0;
}


BOOL CCharsRect::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CWnd*   pEdit;
	pEdit = GetDlgItem(IDC_ESHOWCHARS);
	CRect rect;
	GetClientRect(&rect);
	CharsFont.CreateFont(rect.Height() / (MYHEIGHT), rect.Width() / (2 * MYWIDTH), 0, 0, FW_NORMAL, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial"));
	pEdit->SetFont(&CharsFont);

	return TRUE;
}


void CCharsRect::OnSetFont(CFont* pFont)
{
	CDialogEx::OnSetFont(pFont);
}


void CCharsRect::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CCharShow *pView = (CCharShow*)GetParent();
	if (pView == NULL)
		return;
	m_Curstrings = pView->GetChars();			//获得缓存区中的字符
	m_Edit.SetWindowTextW(m_Curstrings);
	m_Edit.SetSel(m_Curstrings.GetLength(), m_Curstrings.GetLength(), TRUE);
	m_Edit.SendMessage(WM_VSCROLL, SB_BOTTOM);
	m_Edit.SetFocus();
	if (pView->GetTaskChars().GetLength() > 0)
	{
		if (m_Curstrings.GetLength() == 1)
		{
			m_nStarttime = timeGetTime();
		}
		if (m_Curstrings.GetLength() == pView->GetTaskChars().GetLength())
		{
			double c = CheckCollect(pView->GetTaskChars(), m_Curstrings);
			m_nEndTime = timeGetTime();
			int interval = m_nEndTime - m_nStarttime;
			CString mes;
			mes.Format(_T(" total char:%d \n total time:%d \n mean time:%d \n corect:%lf"),
				m_Curstrings.GetLength(), interval, interval / m_Curstrings.GetLength(), c*100.0);
			AfxMessageBox(mes);
		}
	}
}


void CCharsRect::OnDestroy()
{
	CDialogEx::OnDestroy();
	CharsFont.DeleteObject();
}


BOOL CCharsRect::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_KEYDOWN)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

double CCharsRect::CheckCollect(CString dest, CString str)
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



