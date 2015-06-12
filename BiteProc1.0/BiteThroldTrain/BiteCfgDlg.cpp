// BiteCfgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BiteCfgDlg.h"
#include "afxdialogex.h"
#include "Structdefine.h"

// CBiteCfgDlg �Ի���

IMPLEMENT_DYNAMIC(CBiteCfgDlg, CDialogEx)

CBiteCfgDlg::CBiteCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBiteCfgDlg::IDD, pParent)
{

}

CBiteCfgDlg::~CBiteCfgDlg()
{
}

void CBiteCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITBITETIME, m_BiteTime);
	DDX_Text(pDX, IDC_EDITBITENUMBER, m_BiteNumber);
}


BEGIN_MESSAGE_MAP(CBiteCfgDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBiteCfgDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBiteCfgDlg ��Ϣ�������


BOOL CBiteCfgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_BiteTime = 20;
	m_BiteNumber = 10;

	UPDATAVALTOCONTROL;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CBiteCfgDlg::GetCfgParma(int& time, int& number)
{
	time = m_BiteTime;
	number = m_BiteNumber;
}


void CBiteCfgDlg::OnBnClickedOk()
{
	UPDATACONTROLTOVAL;
	CDialogEx::OnOK();
}
