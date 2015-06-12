// CharWnd.cpp : ʵ���ļ�
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

void CCharWnd::OnPaint()	//���ƾ������
{
	Invalidate(TRUE);
	CPaintDC dc(this);
	CWnd::OnPaint();
	if(m_nChars.GetLength()<=0)
		return;
	CRect  rect;
	GetClientRect(rect);
	rect.InflateRect(-1,-1);								//��յ�ǰ��������
	dc.Draw3dRect(rect,m_nBnkColor,m_nBnkColor);
    HDC dcMemory = CreateCompatibleDC( dc.m_hDC );			//�����ڴ������Ļ���
	if (NULL == dcMemory)
	{
		return;
	}
	HBITMAP hMemoryBitmap = CreateCompatibleBitmap( dc.m_hDC, rect.Width(), rect.Height() );//�ڴ滭��
	if(NULL == hMemoryBitmap)
	{
		return ;    
	}
	SelectObject( dcMemory, hMemoryBitmap );				
	HBRUSH     hBrushBackground = CreateSolidBrush(m_nBnkColor);//������ˢ
	FillRect( dcMemory, &rect, hBrushBackground );			//���Ʊ���

	//��������
	HFONT hFont = CreateFont( rect.Height()*2/3,rect.Width()/(1+m_nChars.GetLength())*0.6,0,0,FW_NORMAL,0,0,0,
		DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, _T("Arial") );
	HFONT hitFont = CreateFont( rect.Height()*1/7,rect.Width()/(1+m_HintMsg.GetLength())*0.4,0,0,FW_NORMAL,0,0,0,
		DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, _T("Arial") );
    SelectObject(dcMemory, hFont );      
	SetTextColor(dcMemory,m_nCharsColor);
	SetBkMode( dcMemory, TRANSPARENT );

	//�����ַ���
	DrawTextEx(dcMemory,m_nChars.GetBuffer(0),m_nChars.GetLength(),&rect, DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE, 0);
	m_nChars.ReleaseBuffer();
	SetTextColor(dcMemory ,RGB(0,0,0));
	SelectObject(dcMemory,hitFont);

	//������ʾ�ַ���
	DrawTextEx(dcMemory,m_HintMsg.GetBuffer(),m_HintMsg.GetLength(),&CRect(rect.left , rect.top+rect.Height()*5/6 , rect.right,rect.bottom), 
		DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE, 0);
	m_HintMsg.ReleaseBuffer();

	//��ʾ
	BitBlt(dc.m_hDC,rect.left,rect.top,rect.Width(),rect.Height(),dcMemory,0,0,SRCCOPY);
	DeleteObject( hMemoryBitmap );
	DeleteDC( dcMemory );  
	DeleteObject( hBrushBackground );
	DeleteObject( hFont );
	DeleteObject(hitFont);
}

// CCharWnd ��Ϣ�������



//ͨ����ͬ����Ϣ�������ƴ�����ʾ��ɫ
/*
BNKCOLOR������Ĭ����ɫ
CHKCOLOR������ѡ����ɫ
CHARCOLOR���ַ�����ɫ
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
	if(wParam>0&&lParam>0)						//����Ч��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_nBnkColor = BNKCOLOR;
	m_nCharsColor = CHARCOLOR;
	SendMessage(WM_PAINT,0,0);
	this->KillTimer(1);
	CWnd::OnTimer(nIDEvent);
}
