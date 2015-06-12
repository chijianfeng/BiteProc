#pragma once
#include"Resource.h"
#include<afxdialogex.h>
// CWconfigDlg �Ի���

class CWconfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWconfigDlg)

public:
	CWconfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWconfigDlg();


	int GetLeftMaxSpeed();
	int GetUpMaxSpeed();
	int	GetDownMaxSpeed();
	int	GetRightMaxSpeed();
// �Ի�������
	enum { IDD = IDD_WCONFIGDLG };
private:

	int		m_nMaxUpSpeed;
	int		m_nMaxDownSpeed;
	int		m_nMaxLeftSpeed;
	int		m_nMaxRightSpeed;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
