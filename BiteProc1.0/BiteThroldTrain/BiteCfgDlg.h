#pragma once
#include "resource.h"

// CBiteCfgDlg 对话框

class CBiteCfgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBiteCfgDlg)

public:
	CBiteCfgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBiteCfgDlg();

	void GetCfgParma(int& time, int& number);			//获取参数

// 对话框数据
	enum { IDD = IDD_BITECFGDLG };

protected:

	int		m_BiteTime;									//咬牙时长
	int		m_BiteNumber;								//咬牙次数

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
