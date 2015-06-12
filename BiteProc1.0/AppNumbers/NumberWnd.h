#pragma once
/*
���ߣ����ĺ�
ʱ�䣺2015/2/3
������������ɫ������ɫ�Ȳ�����������
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

	void SetNumbers(CString& str);    //��Ҫ��ʾ������
	void SetNumbers(char c);
	void SetHintMsg(CString str);     

	void SetBackColor(COLORREF color); //���ñ���ɫ
	COLORREF GetBackColor();
	void SetNumbersColor(COLORREF color); //����������ɫ
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
	afx_msg LRESULT OnMyResivemsg(WPARAM wParam, LPARAM lParam);		//�ַ�������Ϣ��Ӧ����
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};