#pragma once
#include <list>
#include <iostream>
#include "UserConfig.h"
#include "ViewerConfig.h"
using namespace std;

// CViewerSelDlg 对话框

class CViewerSelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewerSelDlg)

public:
	CViewerSelDlg(CWnd* pParent = NULL);   // 标准构造函数
	CViewerSelDlg(CString userpath, CWnd* pParent = NULL);
	virtual ~CViewerSelDlg();
	CString GetSelDllPath();
	void SetSize(const int len, const int width);
	void GetSize(int& len, int& width);
	void GetMaxMin(int& Maxv, int& Minv);
	BOOL IsAutoMaxMin();
	void CheckModels();
// 对话框数据
	enum { IDD = IDD_VIEWERSELDLG };

private:
	CString             m_EEGViewerPath;            //EEGViewer.txt目录
	CString				m_strSearchDir;				//搜索路径
	list<CString>		m_listModels;				//模块列表
	CString				m_strDllPath;				//选择的Dll路径
	CComboBox			m_comViewers;				//可以视图模块

	CString				m_strMessage;				//当前选择模块的信息

	int					m_Length;					//单通道显示长度
	int					m_Width;					//单通道显示宽度

	int					m_nMaxv;					//最大值
	int					m_nMinv;					//最小值
	BOOL				m_bAuto;					//自动调整大小值

	CString				m_strUserPath;				//用户路径
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBviewsearchdir();
	virtual BOOL OnInitDialog();
	afx_msg void OnViewseldlgCheck();
	afx_msg void OnCbnSelchangeComviewmodel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedChkviewauto();
};
