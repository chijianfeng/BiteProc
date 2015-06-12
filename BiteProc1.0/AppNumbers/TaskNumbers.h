#pragma once
/*
作者：黄文浩
时间：2015/2/3
描述：输出框的Dialog类
*/
#include "afxdialogex.h"
#include "Resource.h"

// CTaskNumbers 对话框

class CTaskNumbers : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskNumbers)

public:
	CTaskNumbers(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTaskNumbers();

	void     SetWndRect(CRect rect);
	CRect    GetWndRect();
	CString  GetTaskNumbers();
// 对话框数据
	enum { IDD = IDD_TaskNumbers };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
