#pragma once

#include "resource.h"
// CTaskChars �Ի���

class CTaskChars : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskChars)

public:
	CTaskChars(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTaskChars();

	void	SetWndRect(CRect rect);
	CRect	GetWndRect();
	CString GetTaskChars();
// �Ի�������
	enum { IDD = IDD_TASKCHARS };
private:
	CFont		CharsFont;
	CRect		m_Wndrect;
	CEdit		m_Edit;
	CString		m_nEditStr;
	CString		m_nCurString;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
};
