/*
作者：黄文浩
时间：2015/1/24
描述：实现管理主界面Config Selector下的子界面“咬牙阈值设置”的类BiteThresholdConfig
*/
#pragma once
#include "AlgParamConfig.h"
#include "../Resource.h"
#include <list>
#include <iostream>

using namespace std;
// BiteThresholdConfig 对话框

class BiteThresholdConfig : public CDialogEx
{
	DECLARE_DYNAMIC(BiteThresholdConfig)

public:
	BiteThresholdConfig(CWnd* pParent = NULL);   // 标准构造函数
	BiteThresholdConfig(CString path, CWnd* pParent = NULL);
	virtual ~BiteThresholdConfig();

	void	SetConfigFilePath(CString path);

// 对话框数据
	enum { IDD = IDD_BITETHRESHOLDDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString m_ConfigPath;							   //获取Config.txt的路径
	CString m_biteTrainTime;                           //获取编辑框中biteTrainTime的值
	CString m_biteCount;
	CString m_biteDataPath;
	CString m_biteThresholdPath;
	CString m_biteThroldModulePath;
public:
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);       //重写回车事件
	afx_msg BOOL OnInitDialog();				       //重写OnInitDialog方法
	afx_msg void OnBnClickedBitedatapath();		       //BiteDataPath路径搜索按钮的单击事件
	afx_msg void OnBnClickedBbitethroldpath();         //BitethresholdPath路径搜索按钮的单击事件
	afx_msg void OnBnClickedBbitethroldmodulepath();   //BiteThroldModulePath路径搜索按钮的单击事件
	afx_msg void SaveFile();
};
