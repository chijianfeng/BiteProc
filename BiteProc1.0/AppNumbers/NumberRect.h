#pragma once
/*
���ߣ����ĺ�
ʱ�䣺2015/2/3
���������ֱ��Dialog��
*/
#include "afxwin.h"
#include "Resource.h"

// CNumberRect �Ի���

class CNumberRect : public CDialogEx
{
	DECLARE_DYNAMIC(CNumberRect)

public:
	CNumberRect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNumberRect();
	
	void	SetWndRect(CRect rect);		//change the window size;
	CRect	GetWndRect();
	CFont	NumbersFont;
// �Ի�������
	enum { IDD = IDD_NUMBERRECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMyrecmsg(WPARAM wParam,LPARAM lParam);
	double CheckCollect(CString dest,CString str);
private:
	CEdit    m_Edit;
	CRect    m_Wndrect;
	CString  m_Curstrings;

	DWORD    m_nStartTime;
	DWORD    m_nEndTime;
public:
	virtual BOOL OnInitDialog();
	virtual void OnSetFont(CFont* pFont);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
