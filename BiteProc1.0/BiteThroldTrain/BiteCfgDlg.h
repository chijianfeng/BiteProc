#pragma once
#include "resource.h"

// CBiteCfgDlg �Ի���

class CBiteCfgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBiteCfgDlg)

public:
	CBiteCfgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBiteCfgDlg();

	void GetCfgParma(int& time, int& number);			//��ȡ����

// �Ի�������
	enum { IDD = IDD_BITECFGDLG };

protected:

	int		m_BiteTime;									//ҧ��ʱ��
	int		m_BiteNumber;								//ҧ������

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
