/*
作者：黄文浩
时间：2015/1/24
描述：实现管理子界面“左,右,同时咬参数设置”的类LeftRightBothConfig
*/
#pragma once

#include "AlgParamConfig.h"
#include "../Resource.h"
#include <list>
#include <iostream>
using namespace std;
// LeftRightBothConfig 对话框

class LeftRightBothConfig : public CDialogEx
{
	DECLARE_DYNAMIC(LeftRightBothConfig)

public:
	LeftRightBothConfig(CWnd* pParent = NULL);   // 标准构造函数
	LeftRightBothConfig(CString path, CWnd* pParent = NULL);
	virtual ~LeftRightBothConfig();

	void	SetConfigFilePath(CString path);

// 对话框数据
	enum { IDD = IDD_LEFTRIGHTBOTHDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_ConfigPath;
	CString m_ModelTrainTime;
	CString m_ModelPath;
	CString m_LeftDataPath;
	CString m_RightDataPath;
	CString m_BothDataPath;
	CString m_ModulePath;
public:
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);        //重写回车事件
	afx_msg BOOL OnInitDialog();                        //重写OnInitDialog方法
	afx_msg void OnBnClickedBmodelpath();
	afx_msg void OnBnClickedBleftdatapath();
	afx_msg void OnBnClickedBrightdatapath();
	afx_msg void OnBnClickedBbothdatapath();
	afx_msg void OnBnClickedBmodulepath();
	afx_msg void SaveFile();
};
