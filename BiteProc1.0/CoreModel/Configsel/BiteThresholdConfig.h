/*
���ߣ����ĺ�
ʱ�䣺2015/1/24
������ʵ�ֹ���������Config Selector�µ��ӽ��桰ҧ����ֵ���á�����BiteThresholdConfig
*/
#pragma once
#include "AlgParamConfig.h"
#include "../Resource.h"
#include <list>
#include <iostream>

using namespace std;
// BiteThresholdConfig �Ի���

class BiteThresholdConfig : public CDialogEx
{
	DECLARE_DYNAMIC(BiteThresholdConfig)

public:
	BiteThresholdConfig(CWnd* pParent = NULL);   // ��׼���캯��
	BiteThresholdConfig(CString path, CWnd* pParent = NULL);
	virtual ~BiteThresholdConfig();

	void	SetConfigFilePath(CString path);

// �Ի�������
	enum { IDD = IDD_BITETHRESHOLDDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CString m_ConfigPath;							   //��ȡConfig.txt��·��
	CString m_biteTrainTime;                           //��ȡ�༭����biteTrainTime��ֵ
	CString m_biteCount;
	CString m_biteDataPath;
	CString m_biteThresholdPath;
	CString m_biteThroldModulePath;
public:
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);       //��д�س��¼�
	afx_msg BOOL OnInitDialog();				       //��дOnInitDialog����
	afx_msg void OnBnClickedBitedatapath();		       //BiteDataPath·��������ť�ĵ����¼�
	afx_msg void OnBnClickedBbitethroldpath();         //BitethresholdPath·��������ť�ĵ����¼�
	afx_msg void OnBnClickedBbitethroldmodulepath();   //BiteThroldModulePath·��������ť�ĵ����¼�
	afx_msg void SaveFile();
};
