//Download by http://www.NewXing.com
// DigiUtil.cpp: implementation of the CDigiClock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DigiUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDigiClock::CDigiClock()
{
	m_bAlarm  = FALSE;
	m_style	  = XDC_SECOND;
	m_strText = _T("--:--:--");
	m_nCount  = 0;
}

BEGIN_MESSAGE_MAP(CDigiClock, CDigistring)
	//{{AFX_MSG_MAP(CDigiClock)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDigiClock message handlers

void CDigiClock::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nTimer = SetTimer(1, 1000, NULL);
	ASSERT(m_nTimer);
	
	CDigistring::PreSubclassWindow();
}

void CDigiClock::OnDestroy() 
{
	CDigistring::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(m_nTimer);	
}

void CDigiClock::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		m_nCount++;
		CTime time = CTime::GetCurrentTime();
		int nh = time.GetHour();
		int nm = time.GetMinute();
		int ns = time.GetSecond();

		switch(m_style) {
			case XDC_SECOND:
			{
				if (m_bAlarm && m_nCount%2)
					m_strText = _T("  :  :  ");
				else
					m_strText.Format(_T("%02d:%02d:%02d"), nh, nm, ns);
			}
			break;
			case XDC_NOSECOND:
			default:
				if (m_bAlarm && m_nCount%2)
					m_strText = _T("  :  ");
				else
					m_strText.Format(_T("%02d:%02d:"), nh, nm);
				break;
		};
		if (m_bAlarm)
			MessageBeep(MB_OK);
		m_Modified = TRUE;
		Invalidate();
	}

	CDigistring::OnTimer(nIDEvent);
}

BOOL CDigiClock::SetAlarm(BOOL bAlarm /*= TRUE*/)
{
	BOOL temp = m_bAlarm;
	m_bAlarm = bAlarm;
	return temp;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDigiScroll::CDigiScroll() 
{ 
	m_iTextLength = 12; 
	m_lSpeed = 200l; 
	m_strTextToScroll = "This is an example scrolling display "; 
} 

BEGIN_MESSAGE_MAP(CDigiScroll, CDigistring) 
//{{AFX_MSG_MAP(CDigiScroll) 
ON_WM_TIMER() 
//}}AFX_MSG_MAP 
END_MESSAGE_MAP() 


///////////////////////////////////////////////////////////////////////////// 
// CDigiClock message handlers 

void CDigiScroll::PreSubclassWindow() 
{ 
	// TODO: Add your specialized code here and/or call the base class 
	// 
	// Note: Set the Timer here Pls. 
	// Dont set timer in OnCreate(), you cant recieve WM_CREATE 
	// when control construted in Dialog template. Say: OnCreate not called. 
	// 
	
	CDigistring::PreSubclassWindow(); 
} 

void CDigiScroll::ScrollText(LPCSTR lpszFormat) 
{ 
	m_strTextToScroll = lpszFormat;
	OnTimer(1); 
} 

void CDigiScroll::SetSpeed(long lSpeed) 
{ 
	m_lSpeed = lSpeed; 
} 

void CDigiScroll::SetTextLength(int iMaxLength) 
{ 
	m_iTextLength = iMaxLength; 
} 

void CDigiScroll::OnDestroy() 
{
	CDigistring::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(1);	
}

void CDigiScroll::OnTimer(UINT nIDEvent) 
{ 
	// Take the string, only display the bit of the string that we can see 
	CString strTemp; 
	CString strNewString; 
	CString strScrollString; 
	int iLengthOfString = m_strTextToScroll.GetLength(); 
	
	strTemp = m_strTextToScroll[0]; 
	strNewString = m_strTextToScroll.Mid(1); 
	strNewString += strTemp; 
	
	strScrollString = strNewString.Mid(0,m_iTextLength); 
	
	m_strTextToScroll = strNewString; 
	
	SetText(strScrollString.GetBuffer(0)); 
} 

void CDigiScroll::DoScroll() 
{ 
	SetTimer(1, m_lSpeed, NULL); 
} 

