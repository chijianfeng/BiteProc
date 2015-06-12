#pragma once


// CCharWnd ������ʾ��ĸ��������ʽ�ĳ���
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

	//��ʾ���ַ�or�ַ���
	void	  SetChars(CString& str);		//��Ҫ��ʾ���ַ���
	void	  SetChars(char c);				//change the char to CString
	void	  SetHintMsg(CString str);		//��ʾ�ַ���

	void	  SetBackColor(COLORREF color);//���ñ���ɫ
	COLORREF  GetBackColor();
	void      SetCharsColor(COLORREF color);//�����ַ�����ɫ

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();					//����
private:
	CString		m_nChars;		//the show CString .
	CString		m_HintMsg;
	COLORREF	m_nBnkColor;
	COLORREF	m_nCharsColor;
	CRect		m_nPostion;	//the char's wnd position and 

protected:
	afx_msg LRESULT OnMyResivemsg(WPARAM wParam, LPARAM lParam);		//�ַ�������Ϣ��Ӧ����
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);							//��ʱ��
};


