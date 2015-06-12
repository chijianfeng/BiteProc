#pragma once
#include"Resource.h"
#include<afxdialogex.h>
// CWconfigDlg 对话框

class CWconfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWconfigDlg)

public:
	CWconfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWconfigDlg();


	int GetLeftMaxSpeed();
	int GetUpMaxSpeed();
	int	GetDownMaxSpeed();
	int	GetRightMaxSpeed();
// 对话框数据
	enum { IDD = IDD_WCONFIGDLG };
private:

	int		m_nMaxUpSpeed;
	int		m_nMaxDownSpeed;
	int		m_nMaxLeftSpeed;
	int		m_nMaxRightSpeed;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
