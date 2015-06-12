// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

#pragma once
#include <sstream>
#include <iostream>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// COutputList 窗口

class COutputList : public CListBox
{
// 构造
public:
	COutputList();

// 实现
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};


/*
改写成单例模式，便于显示调试信息
*/
class COutputWnd : public CDockablePane
{
// 构造
public:
	static COutputWnd* GetInstance();					

	void UpdateFonts();

	void	AddStateMsg(CString str);			//增加状态信息

	//用于信息输出
	COutputWnd& operator<< (const int& value) ;
	COutputWnd& operator<< (const double& value) ;
	COutputWnd& operator<< (const CString& str) ;
	COutputWnd& operator<< (const char* pstr) ;
	COutputWnd& operator<< (const size_t& valuse);
	COutputWnd& operator<<(COutputWnd& (*pFunc)( COutputWnd&)) ;

	friend COutputWnd& OutEnd( COutputWnd& outwnd);

// 特性
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputBuild;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;

	stringstream m_ss;

protected:
	COutputWnd();								//单例模式

	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// 实现
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

extern COutputWnd* _poutwnd;									//输出操作函数指针
#define m_wndOutput (*_poutwnd)

extern COutputWnd& (*_Poutend)(COutputWnd& out);				//输出结束指针函数
#define m_EndOut	(*_Poutend)

