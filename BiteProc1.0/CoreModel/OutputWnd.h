// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

#pragma once
#include <sstream>
#include <iostream>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// COutputList ����

class COutputList : public CListBox
{
// ����
public:
	COutputList();

// ʵ��
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
��д�ɵ���ģʽ��������ʾ������Ϣ
*/
class COutputWnd : public CDockablePane
{
// ����
public:
	static COutputWnd* GetInstance();					

	void UpdateFonts();

	void	AddStateMsg(CString str);			//����״̬��Ϣ

	//������Ϣ���
	COutputWnd& operator<< (const int& value) ;
	COutputWnd& operator<< (const double& value) ;
	COutputWnd& operator<< (const CString& str) ;
	COutputWnd& operator<< (const char* pstr) ;
	COutputWnd& operator<< (const size_t& valuse);
	COutputWnd& operator<<(COutputWnd& (*pFunc)( COutputWnd&)) ;

	friend COutputWnd& OutEnd( COutputWnd& outwnd);

// ����
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputBuild;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;

	stringstream m_ss;

protected:
	COutputWnd();								//����ģʽ

	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// ʵ��
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

extern COutputWnd* _poutwnd;									//�����������ָ��
#define m_wndOutput (*_poutwnd)

extern COutputWnd& (*_Poutend)(COutputWnd& out);				//�������ָ�뺯��
#define m_EndOut	(*_Poutend)

