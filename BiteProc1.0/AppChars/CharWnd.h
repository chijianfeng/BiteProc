#pragma once


// CCharWnd 用于显示字母，动画形式的呈现
const COLORREF BNKCOLOR= RGB(128,128,128);
const COLORREF CHKCOLOR = RGB(255,255,255);
const COLORREF SLECOLOR = RGB(0,255,0);
const COLORREF CHARCOLOR = RGB(0,0,255);
class CCharWnd : public CWnd
{
	DECLARE_DYNAMIC(CCharWnd)

public:
	CCharWnd(CRect rect);
	CCharWnd();
	virtual ~CCharWnd();

	//显示的字符or字符串
	void	  SetChars(CString& str);		//需要显示的字符串
	void	  SetChars(char c);				//change the char to CString
	void	  SetHintMsg(CString str);		//提示字符串

	void	  SetBackColor(COLORREF color);//设置背景色
	COLORREF  GetBackColor();
	void      SetCharsColor(COLORREF color);//设置字符串颜色

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();					//绘制
private:
	CString		m_nChars;		//the show CString .
	CString		m_HintMsg;
	COLORREF	m_nBnkColor;
	COLORREF	m_nCharsColor;
	CRect		m_nPostion;	//the char's wnd position and 

protected:
	afx_msg LRESULT OnMyResivemsg(WPARAM wParam, LPARAM lParam);		//字符窗口消息响应函数
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);							//定时器
};


