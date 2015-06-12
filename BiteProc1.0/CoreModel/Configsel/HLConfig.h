/*
���ߣ����ĺ�
ʱ�䣺2015/1/24
������ʵ�ֹ����ӽ���"����ҧ��������"����HLConfig  ��H means Heavy ��ҧ L means Light ��ҧ��
*/
#pragma once
#include "AlgParamConfig.h"
#include "../Resource.h"
#include <list>
#include <iostream>
using namespace std;
// HLConfig �Ի���

class HLConfig : public CDialogEx
{
	DECLARE_DYNAMIC(HLConfig)

public:
	HLConfig(CWnd* pParent = NULL);   // ��׼���캯��
	HLConfig(CString path, CWnd*pParent = NULL);
	virtual ~HLConfig();

	void	SetConfigFilePath(CString path);

// �Ի�������
	enum { IDD = IDD_HLDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);        //��д�س��¼�
	afx_msg BOOL OnInitDialog();                        //��дOnInitDialog�¼�
	afx_msg void OnBnClickedBhlmodelpath();             //HLModelPath·��������ť�ĵ����¼�
	afx_msg void OnBnClickedBhlmodulepath();
	afx_msg void OnBnClickedBleftlightdatapath();
	afx_msg void OnBnClickedBleftheavydatapath();
	afx_msg void OnBnClickedBrightlightdatapath();
	afx_msg void OnBnClickedBrightheavydatapath();
	afx_msg void OnBnClickedBbothlightdatapath();
	afx_msg void OnBnClickedBbothheavydatapath();
	afx_msg void SaveFile();
};
