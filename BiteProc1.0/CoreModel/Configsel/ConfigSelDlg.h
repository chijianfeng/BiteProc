/*
作者:黄文浩
时间:2015/1/24
描述:管理主界面ConfigSelector的类
	 使用了Tab控件,主界面下有有五个子界面
	 "咬牙阈值设置","左,右,同时咬参数设置"，"轻重咬参数设置","长咬,连续两次咬参数设置","公共基本参数设置"
	 对应用五个类(BiteThresholdConfig,LeftRightBothConfig,HLConfig,TLConfig,PublicConfig) 来管理
Repaire: chi
*/

// ConfigSelDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "BiteThresholdConfig.h"
#include "LeftRightBothConfig.h"
#include "HLConfig.h"
#include "TLConfig.h"
#include "PublicConfig.h"
#include "FileOper.h"
#include "../UserConfig.h"

// CConfigSelDlg 对话框
class CConfigSelDlg : public CDialogEx
{

public:
	CConfigSelDlg(CWnd* pParent = NULL);	// 标准构造函数
	CConfigSelDlg(CUserConfig& config, CWnd* pParent);

	enum { IDD = IDD_CONFIGSEL_DIALOG };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl m_tab;
	BiteThresholdConfig m_dialog1;      
	LeftRightBothConfig m_dialog2;
	HLConfig m_dialog3;
	TLConfig m_dialog4;
	PublicConfig m_dialog5;

	CString			m_strUserPath;				//用户目录

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
