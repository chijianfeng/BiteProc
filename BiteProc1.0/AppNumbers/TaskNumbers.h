#pragma once
/*
���ߣ����ĺ�
ʱ�䣺2015/2/3
������������Dialog��
*/
#include "afxdialogex.h"
#include "Resource.h"

// CTaskNumbers �Ի���

class CTaskNumbers : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskNumbers)

public:
	CTaskNumbers(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTaskNumbers();

	void     SetWndRect(CRect rect);
	CRect    GetWndRect();
	CString  GetTaskNumbers();
// �Ի�������
	enum { IDD = IDD_TaskNumbers };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString   m_nEditStr;
	CString   m_nCurString;
	CEdit     m_Edit;
	CRect     m_Wndrect;
	CFont     NumbersFont;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
