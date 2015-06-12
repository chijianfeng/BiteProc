/*
���ߣ����ĺ�
ʱ�䣺2015/1/24
������ʵ�ֹ����ӽ��桰��,��,ͬʱҧ�������á�����LeftRightBothConfig
*/
#pragma once

#include "AlgParamConfig.h"
#include "../Resource.h"
#include <list>
#include <iostream>
using namespace std;
// LeftRightBothConfig �Ի���

class LeftRightBothConfig : public CDialogEx
{
	DECLARE_DYNAMIC(LeftRightBothConfig)

public:
	LeftRightBothConfig(CWnd* pParent = NULL);   // ��׼���캯��
	LeftRightBothConfig(CString path, CWnd* pParent = NULL);
	virtual ~LeftRightBothConfig();

	void	SetConfigFilePath(CString path);

// �Ի�������
	enum { IDD = IDD_LEFTRIGHTBOTHDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);        //��д�س��¼�
	afx_msg BOOL OnInitDialog();                        //��дOnInitDialog����
	afx_msg void OnBnClickedBmodelpath();
	afx_msg void OnBnClickedBleftdatapath();
	afx_msg void OnBnClickedBrightdatapath();
	afx_msg void OnBnClickedBbothdatapath();
	afx_msg void OnBnClickedBmodulepath();
	afx_msg void SaveFile();
};
