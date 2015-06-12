// WconfigDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "Core_module.h"
#include "WconfigDlg.h"
#include "afxdialogex.h"
#include "Resource.h"

// CWconfigDlg 对话框

IMPLEMENT_DYNAMIC(CWconfigDlg, CDialogEx)

CWconfigDlg::CWconfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWconfigDlg::IDD, pParent)
{
	m_nMaxDownSpeed = -90;
	m_nMaxLeftSpeed = -40;
	m_nMaxUpSpeed = 90;
	m_nMaxRightSpeed = 40;
}

CWconfigDlg::~CWconfigDlg()
{
}

void CWconfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX , IDC_EUPSPEED , m_nMaxUpSpeed);
	DDX_Text(pDX , IDC_EDOWNSPEED , m_nMaxDownSpeed);
	DDX_Text(pDX , IDC_ELEFTSPEED , m_nMaxLeftSpeed);
	DDX_Text(pDX , IDC_ERIGHTSPEED , m_nMaxRightSpeed);
}


int CWconfigDlg::GetLeftMaxSpeed()
{
	return m_nMaxLeftSpeed;
}

int CWconfigDlg::GetRightMaxSpeed()
{
	return m_nMaxRightSpeed;
}

int CWconfigDlg::GetUpMaxSpeed()
{
	return m_nMaxUpSpeed;
}

int CWconfigDlg::GetDownMaxSpeed()
{
	return m_nMaxDownSpeed;
}

BEGIN_MESSAGE_MAP(CWconfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWconfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


void CWconfigDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}
