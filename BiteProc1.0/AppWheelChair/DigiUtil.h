//Download by http://www.NewXing.com
// DigiUtil.h: interface for the CDigiClock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIGIUTIL_H__4866B082_D237_11D3_949E_00104B6C2FFE__INCLUDED_)
#define AFX_DIGIUTIL_H__4866B082_D237_11D3_949E_00104B6C2FFE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Digistring.h"

class CDigiClock : public CDigistring  
{
// Construction
public:
	CDigiClock();
// Attributes
public:
	enum CClockStyle { XDC_SECOND, XDC_NOSECOND };
// Operations
public:
	CClockStyle GetStyle() const { return m_style;}
	CClockStyle SetStyle(CClockStyle style);
	BOOL GetAlarm() const { return m_bAlarm;}
	BOOL SetAlarm(BOOL bAlarm = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigiClock)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CDigiClock)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
private:
	BOOL		m_bAlarm;				// Alarm mode
	CClockStyle m_style;				// clock style
	UINT		m_nTimer;				// active timer
	UINT		m_nCount;				// timer counts

	DECLARE_MESSAGE_MAP()
};

class CDigiScroll: public CDigistring 
{ 
public: 
	void DoScroll(); 
	CDigiScroll(); 
	
	void ScrollText(LPCSTR lpszFormat); 
	void SetTextLength(int iMaxLength); 
	void SetSpeed(long lSpeed); 
	
	// Overrides 
	// ClassWizard generated virtual function overrides 
	//{{AFX_VIRTUAL(CDigiScroll) 
protected: 
	virtual void PreSubclassWindow(); 
	//}}AFX_VIRTUAL 
	
	// Generated message map functions 
protected: 
	//{{AFX_MSG(CDigiScroll) 
	afx_msg void OnTimer(UINT nIDEvent); 
	afx_msg void OnDestroy();
	//}}AFX_MSG 
	
private: 
	CString m_strTextToScroll; 
	long m_lSpeed; 
	int m_iTextLength; 
	
	DECLARE_MESSAGE_MAP() 
}; 



#endif // !defined(AFX_DIGIUTIL_H__4866B082_D237_11D3_949E_00104B6C2FFE__INCLUDED_)
