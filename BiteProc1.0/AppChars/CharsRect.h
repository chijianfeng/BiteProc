#pragma once


// CCharsRect �Ի���

class CCharsRect : public CDialogEx
{
	DECLARE_DYNAMIC(CCharsRect)

public:
	CCharsRect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCharsRect();

	void	SetWndRect(CRect rect);		//change the window size;
	CRect	GetWndRect();
	CFont	 CharsFont;
// �Ի�������
	enum { IDD = IDD_CHARSRECT };

private:

	CString		m_Curstrings;
	CRect		m_Wndrect;
	CEdit		m_Edit;

	DWORD		m_nStarttime;
	DWORD		m_nEndTime;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMyrecmsg(WPARAM wParam, LPARAM lParam);

	double	CheckCollect(CString dest, CString str);
public:
	virtual BOOL OnInitDialog();
	virtual void OnSetFont(CFont* pFont);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
