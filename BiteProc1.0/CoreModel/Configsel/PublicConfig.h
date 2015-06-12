/*
作者：黄文浩
时间：2015/1/24
描述：实现管理子界面“公共基本参数设置”的类PublicConfig
*/

#pragma once


// PublicConfig 对话框
#include "AlgParamConfig.h"
#include "../Resource.h"
#include <list>
#include <iostream>
using namespace std;

class PublicConfig : public CDialogEx
{
	DECLARE_DYNAMIC(PublicConfig)

public:
	PublicConfig(CWnd* pParent = NULL);   // 标准构造函数
	PublicConfig(CString path, CWnd* pParent);
	virtual ~PublicConfig();

	void	SetConfigFilePath(CString path);


// 对话框数据
	enum { IDD = IDD_PUBLICDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_ConfigPath;
	CString m_WindowLength;
	CString m_OverLappedWinLen;
	CString m_LeftThreshold;
	CString m_RightThreshold;
public:
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);    //重写回车事件
	afx_msg BOOL OnInitDialog();                    //重写OnInitDialog方法
	afx_msg void SaveFile();
};
