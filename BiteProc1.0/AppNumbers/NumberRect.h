#pragma once
/*
作者：黄文浩
时间：2015/2/3
描述：数字表的Dialog类
*/
#include "afxwin.h"
#include "Resource.h"

// CNumberRect 对话框

class CNumberRect : public CDialogEx
{
	DECLARE_DYNAMIC(CNumberRect)

public:
	CNumberRect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNumberRect();
	
	void	SetWndRect(CRect rect);		//change the window size;
	CRect	GetWndRect();
	CFont	NumbersFont;
// 对话框数据
	enum { IDD = IDD_NUMBERRECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
