/*
���ߣ����ĺ�
ʱ�䣺2015/1/24
������ʵ�ֹ����ӽ��桰��ҧ,��������ҧ�������á�����TLConfig  ��T means Two ��������ҧ L means Long ��ҧ��
*/

#pragma once
#include "AlgParamConfig.h"
#include "../Resource.h"
#include <list>
#include <iostream>
using namespace std;
// TLConfig �Ի���

class TLConfig : public CDialogEx
{
	DECLARE_DYNAMIC(TLConfig)

public:
	TLConfig(CWnd* pParent = NULL);   // ��׼���캯��
	TLConfig(CString path, CWnd* pParent = NULL);
	virtual ~TLConfig();

	void	SetConfigFilePath(CString path);

// �Ի�������
	enum { IDD = IDD_TLDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);     //��д�س��¼�
	afx_msg BOOL OnInitDialog();                     //��дOnInitDialog����
	afx_msg void OnBnClickedBlefttlparmapath();      //LeftTLParamPath·��������ť�ĵ����¼�
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
