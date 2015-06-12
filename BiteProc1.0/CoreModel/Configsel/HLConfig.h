/*
作者：黄文浩
时间：2015/1/24
描述：实现管理子界面"轻重咬参数设置"的类HLConfig  （H means Heavy 重咬 L means Light 轻咬）
*/
#pragma once
#include "AlgParamConfig.h"
#include "../Resource.h"
#include <list>
#include <iostream>
using namespace std;
// HLConfig 对话框

class HLConfig : public CDialogEx
{
	DECLARE_DYNAMIC(HLConfig)

public:
	HLConfig(CWnd* pParent = NULL);   // 标准构造函数
	HLConfig(CString path, CWnd*pParent = NULL);
	virtual ~HLConfig();

	void	SetConfigFilePath(CString path);

// 对话框数据
	enum { IDD = IDD_HLDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_ConfigPath;
	CString m_HLTrainTime;
	CString m_HLModelPath;
	CString m_HLModulePath;
	CString m_LeftLightDataPath;
	CString m_LeftHeavyDataPath;
	CString m_RightLightDataPath;
	CString m_RightHeavyDataPath;
	CString m_BothLightDataPath;
	CString m_BothHeavyDataPath;
public:
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);        //重写回车事件
	afx_msg BOOL OnInitDialog();                        //重写OnInitDialog事件
	afx_msg void OnBnClickedBhlmodelpath();             //HLModelPath路径搜索按钮的单击事件
	afx_msg void OnBnClickedBhlmodulepath();
	afx_msg void OnBnClickedBleftlightdatapath();
	afx_msg void OnBnClickedBleftheavydatapath();
	afx_msg void OnBnClickedBrightlightdatapath();
	afx_msg void OnBnClickedBrightheavydatapath();
	afx_msg void OnBnClickedBbothlightdatapath();
	afx_msg void OnBnClickedBbothheavydatapath();
	afx_msg void SaveFile();
};
