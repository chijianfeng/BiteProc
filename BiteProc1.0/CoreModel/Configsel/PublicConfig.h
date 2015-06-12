/*
���ߣ����ĺ�
ʱ�䣺2015/1/24
������ʵ�ֹ����ӽ��桰���������������á�����PublicConfig
*/

#pragma once


// PublicConfig �Ի���
#include "AlgParamConfig.h"
#include "../Resource.h"
#include <list>
#include <iostream>
using namespace std;

class PublicConfig : public CDialogEx
{
	DECLARE_DYNAMIC(PublicConfig)

public:
	PublicConfig(CWnd* pParent = NULL);   // ��׼���캯��
	PublicConfig(CString path, CWnd* pParent);
	virtual ~PublicConfig();

	void	SetConfigFilePath(CString path);


// �Ի�������
	enum { IDD = IDD_PUBLICDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_ConfigPath;
	CString m_WindowLength;
	CString m_OverLappedWinLen;
	CString m_LeftThreshold;
	CString m_RightThreshold;
public:
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);    //��д�س��¼�
	afx_msg BOOL OnInitDialog();                    //��дOnInitDialog����
	afx_msg void SaveFile();
};
