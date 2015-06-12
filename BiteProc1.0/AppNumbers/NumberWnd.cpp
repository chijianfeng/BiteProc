#include"stdafx.h"
#include"NumberWnd.h"
#include"Resource.h"

IMPLEMENT_DYNAMIC(CNumberWnd,CWnd)

CNumberWnd::CNumberWnd()
{
	m_nBnkColor = BNKCOLOR;
	m_nNumbersColor = NUMBERCOLOR;
	m_HintMsg = _T("");
}

CNumberWnd::CNumberWnd(CRect rect)
{
	this->m_nPostion  =  rect;
	m_nBnkColor       =  BNKCOLOR;
	m_nNumbersColor   =  NUMBERCOLOR;
	m_HintMsg         =  _T("");
}

CNumberWnd::~CNumberWnd()
{

}

void CNumberWnd::SetBackColor(COLORREF color)
{
	this->m_nBnkColor = color;
}

COLORREF CNumberWnd::GetBackColor()
{
	return m_nBnkColor;
}

void CNumberWnd::SetNumbers(CString& str)
{
	m_nNumbers = str;
}

void CNumberWnd::SetNumbers(char c)
{
	m_nNumbers = c;
}

void CNumberWnd::SetHintMsg(CString hit)
{
	m_HintMsg = hit;
}

void CNumberWnd::SetNumbersColor(COLORREF color)
{
	this->m_nNumbersColor = color;
}

BEGIN_MESSAGE_MAP(CNumberWnd,CWnd)
	ON_WM_PAINT()
	ON_MESSAGE(MYWM_NUMBERMSG,&CNumberWnd::OnMyResivemsg)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CNumberWnd::OnPaint()
{
	Invalidate(TRUE);
	CPaintDC dc(this);
	CWnd::OnPaint();
	if (m_nNumbers.GetLength() <= 0)
		return;
	CRect  rect;
	GetClientRect(rect);
	rect.InflateRect(-1, -1);								//清空当前窗口内容
	dc.Draw3dRect(rect, m_nBnkColor, m_nBnkColor);
	HDC dcMemory = CreateCompatibleDC(dc.m_hDC);			//创建内存上下文环境
	if (NULL == dcMemory)
	{
		return;
	}
	HBITMAP hMemoryBitmap = CreateCompatibleBitmap(dc.m_hDC, rect.Width(), rect.Height());//内存画布
	if (NULL == hMemoryBitmap)
	{
		return;
	}
	SelectObject(dcMemory, hMemoryBitmap);
	HBRUSH     hBrushBackground = CreateSolidBrush(m_nBnkColor);//创建画刷
	FillRect(dcMemory, &rect, hBrushBackground);			//绘制背景

	//创建字体
	HFONT hFont = CreateFont(rect.Height() * 2 / 3, rect.Width() / (1 + m_nNumbers.GetLength())*0.6, 0, 0, FW_NORMAL, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial"));
	HFONT hitFont = CreateFont(rect.Height() * 1 / 7, rect.Width() / (1 + m_HintMsg.GetLength())*0.4, 0, 0, FW_NORMAL, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial"));
	SelectObject(dcMemory, hFont);
	SetTextColor(dcMemory, m_nNumbersColor);
	SetBkMode(dcMemory, TRANSPARENT);

	//绘制字符串
	DrawTextEx(dcMemory, m_nNumbers.GetBuffer(0), m_nNumbers.GetLength(), &rect, DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE, 0);
	m_nNumbers.ReleaseBuffer();
	SetTextColor(dcMemory, RGB(0, 0, 0));
	SelectObject(dcMemory, hitFont);

	//绘制提示字符串
	DrawTextEx(dcMemory, m_HintMsg.GetBuffer(), m_HintMsg.GetLength(), &CRect(rect.left, rect.top + rect.Height() * 5 / 6, rect.right, rect.bottom),
		DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE, 0);
	m_HintMsg.ReleaseBuffer();

	//显示
	BitBlt(dc.m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), dcMemory, 0, 0, SRCCOPY);
	DeleteObject(hMemoryBitmap);
	DeleteDC(dcMemory);
	DeleteObject(hBrushBackground);
	DeleteObject(hFont);
	DeleteObject(hitFont);
}

afx_msg LRESULT CNumberWnd::OnMyResivemsg(WPARAM wParam,LPARAM lParam)
{
	if (wParam <= 0 && lParam <= 0)
	{
		m_nBnkColor = BNKCOLOR;
		m_nNumbersColor = NUMBERCOLOR;
	}
	if (wParam > 0 && lParam <= 0)
	{
		m_nBnkColor = CHKCOLOR;
		m_nNumbersColor = NUMBERCOLOR;
	}
	if (wParam > 0 && lParam > 0)
	{
		m_nBnkColor = SLECOLOR;
		m_nNumbersColor = NUMBERCOLOR;
		this->SetTimer(1,lParam*1000/8,NULL);
	}
	SendMessage(WM_PAINT,0,0);
	return 0;
}

void CNumberWnd::OnTimer(UINT_PTR nIDEvent)
{
	m_nBnkColor = BNKCOLOR;
	m_nNumbersColor = NUMBERCOLOR; 
	SendMessage(WM_PAINT,0,0);
	this->KillTimer(1);
	CWnd::OnTimer(nIDEvent);
}