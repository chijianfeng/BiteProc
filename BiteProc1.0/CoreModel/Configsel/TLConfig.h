/*
作者：黄文浩
时间：2015/1/24
描述：实现管理子界面“长咬,连续两次咬参数设置”的类TLConfig  （T means Two 连续两次咬 L means Long 长咬）
*/

#pragma once
#include "AlgParamConfig.h"
#include "../Resource.h"
#include <list>
#include <iostream>
using namespace std;
// TLConfig 对话框

class TLConfig : public CDialogEx
{
	DECLARE_DYNAMIC(TLConfig)

public:
	TLConfig(CWnd* pParent = NULL);   // 标准构造函数
	TLConfig(CString path, CWnd* pParent = NULL);
	virtual ~TLConfig();

	void	SetConfigFilePath(CString path);

// 对话框数据
	enum { IDD = IDD_TLDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_ConfigPath;
	CString m_TwoBiteTrainTime;
	CString m_LeftTLParamPath;
	CString m_LeftTLDataPath;
	CString m_BothTLParamPath;
	CString m_BothTLDataPath;
	CString m_RightTLParamPath;
	CString m_RightTLDataPath;
	CString m_LeftModelPath;
	CString m_BothModelPath;
	CString m_TLModulePath;
	CString m_RightModelPath;

public:
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);     //重写回车事件
	afx_msg BOOL OnInitDialog();                     //重写OnInitDialog方法
	afx_msg void OnBnClickedBlefttlparmapath();      //LeftTLParamPath路径搜索按钮的单击事件
	afx_msg void OnBnClickedBlefttldatapath();
	afx_msg void OnBnClickedBbothtlparampath();
	afx_msg void OnBnClickedBbothtldatapath();
	afx_msg void OnBnClickedBrighttlparampath();
	afx_msg void OnBnClickedBrighttldatapath();
	afx_msg void OnBnClickedBleftmodelpath();
	afx_msg void OnBnClickedBbothmodelpath();
	afx_msg void OnBnClickedBtlmodulepath();
	afx_msg void OnBnClickedBrightmodelpath();
	afx_msg void SaveFile();                       
};
