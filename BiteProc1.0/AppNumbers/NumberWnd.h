#pragma once
/*
作者：黄文浩
时间：2015/2/3
描述：数字颜色，背景色等参数的配置类
*/

const COLORREF BNKCOLOR    = RGB(128,128,128);
const COLORREF CHKCOLOR    = RGB(255,255,255);
const COLORREF SLECOLOR    = RGB(0,255,0);
const COLORREF NUMBERCOLOR = RGB(0,0,255);

class CNumberWnd :public CWnd
{
	DECLARE_DYNAMIC(CNumberWnd)

public:
	CNumberWnd();
	CNumberWnd(CRect rect);
	virtual ~CNumberWnd();

	void SetNumbers(CString& str);    //需要显示的数字
	void SetNumbers(char c);
	void SetHintMsg(CString str);     

	void SetBackColor(COLORREF color); //设置背景色
	COLORREF GetBackColor();
	void SetNumbersColor(COLORREF color); //设置数字颜色
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
private:
	CString		m_nNumbers;		
	CString		m_HintMsg;
	COLORREF	m_nBnkColor;
	COLORREF	m_nNumbersColor;
	CRect		m_nPostion;	//the number's wnd position 

protected:
	afx_msg LRESULT OnMyResivemsg(WPARAM wParam, LPARAM lParam);		//字符窗口消息响应函数
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};