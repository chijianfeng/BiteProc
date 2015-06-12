// CharWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "CharWnd.h"
#include "resource.h"

// CCharWnd

IMPLEMENT_DYNAMIC(CCharWnd, CWnd)

CCharWnd::CCharWnd()
{
	m_nBnkColor		 = BNKCOLOR;
	m_nCharsColor	 = CHARCOLOR;
	m_HintMsg		 = _T("");
}
CCharWnd::CCharWnd(CRect rect)
{
	this->m_nPostion = rect;
	m_nBnkColor		 = BNKCOLOR;
	m_nCharsColor	 = CHARCOLOR;
	m_HintMsg		 = _T("");
}

CCharWnd::~CCharWnd()
{
}

void CCharWnd::SetBackColor(COLORREF color)
{
	this->m_nBnkColor = color;
}
void CCharWnd::SetChars(CString& str)
{
	m_nChars = str;
}
void CCharWnd::SetChars(char c)
{
	m_nChars = c;
}
void CCharWnd::SetHintMsg(CString hit)
{
	m_HintMsg = hit;
}
COLORREF CCharWnd::GetBackColor()
{
	return m_nBnkColor;
}
void CCharWnd::SetCharsColor(COLORREF color)
{
	this->m_nCharsColor = color;
}

BEGIN_MESSAGE_MAP(CCharWnd, CWnd)
	ON_WM_PAINT()
	ON_MESSAGE(MYWM_CHARMSG, &CCharWnd::OnMyResivemsg)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CCharWnd::OnPaint()	//绘制具体过程
{
	Invalidate(TRUE);
	CPaintDC dc(this);
	CWnd::OnPaint();
	if(m_nChars.GetLength()<=0)
		return;
	CRect  rect;
	GetClientRect(rect);
	rect.InflateRect(-1,-1);								//清空当前窗口内容
	dc.Draw3dRect(rect,m_nBnkColor,m_nBnkColor);
    HDC dcMemory = CreateCompatibleDC( dc.m_hDC );			//创建内存上下文环境
	if (NULL == dcMemory)
	{
		return;
	}
	HBITMAP hMemoryBitmap = CreateCompatibleBitmap( dc.m_hDC, rect.Width(), rect.Height() );//内存画布
	if(NULL == hMemoryBitmap)
	{
		return ;    
	}
	SelectObject( dcMemory, hMemoryBitmap );				
	HBRUSH     hBrushBackground = CreateSolidBrush(m_nBnkColor);//创建画刷
	FillRect( dcMemory, &rect, hBrushBackground );			//绘制背景

	//创建字体
	HFONT hFont = CreateFont( rect.Height()*2/3,rect.Width()/(1+m_nChars.GetLength())*0.6,0,0,FW_NORMAL,0,0,0,
		DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, _T("Arial") );
	HFONT hitFont = CreateFont( rect.Height()*1/7,rect.Width()/(1+m_HintMsg.GetLength())*0.4,0,0,FW_NORMAL,0,0,0,
		DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, _T("Arial") );
    SelectObject(dcMemory, hFont );      
	SetTextColor(dcMemory,m_nCharsColor);
	SetBkMode( dcMemory, TRANSPARENT );

	//绘制字符串
	DrawTextEx(dcMemory,m_nChars.GetBuffer(0),m_nChars.GetLength(),&rect, DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE, 0);
	m_nChars.ReleaseBuffer();
	SetTextColor(dcMemory ,RGB(0,0,0));
	SelectObject(dcMemory,hitFont);

	//绘制提示字符串
	DrawTextEx(dcMemory,m_HintMsg.GetBuffer(),m_HintMsg.GetLength(),&CRect(rect.left , rect.top+rect.Height()*5/6 , rect.right,rect.bottom), 
		DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE, 0);
	m_HintMsg.ReleaseBuffer();

	//显示
	BitBlt(dc.m_hDC,rect.left,rect.top,rect.Width(),rect.Height(),dcMemory,0,0,SRCCOPY);
	DeleteObject( hMemoryBitmap );
	DeleteDC( dcMemory );  
	DeleteObject( hBrushBackground );
	DeleteObject( hFont );
	DeleteObject(hitFont);
}

// CCharWnd 消息处理程序



//通过不同的消息参数控制窗口显示颜色
/*
BNKCOLOR：背景默认颜色
CHKCOLOR：窗口选中颜色
CHARCOLOR：字符串颜色
*/
afx_msg LRESULT CCharWnd::OnMyResivemsg(WPARAM wParam, LPARAM lParam)
{
	//change the backcolor.
	if(wParam<=0&&lParam<=0)
	{
		m_nBnkColor = BNKCOLOR;
		m_nCharsColor = CHARCOLOR;
	}
	if(wParam>0&&lParam<=0)
	{
		m_nBnkColor = CHKCOLOR;
		m_nCharsColor = CHARCOLOR;
	}
	if(wParam>0&&lParam>0)						//动画效果
	{
		m_nBnkColor = SLECOLOR;
		m_nCharsColor = CHARCOLOR;
		this->SetTimer(1,lParam*1000/8,NULL);
	}
	SendMessage(WM_PAINT,0,0);
	return 0;
}


void CCharWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nBnkColor = BNKCOLOR;
	m_nCharsColor = CHARCOLOR;
	SendMessage(WM_PAINT,0,0);
	this->KillTimer(1);
	CWnd::OnTimer(nIDEvent);
}
