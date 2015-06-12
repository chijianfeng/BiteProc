#include "stdafx.h"
#include "PublicConfig.h"
#include "afxdialogex.h"
#include "FileOper.h"
#include "Structdefine.h"
#include "Streamoper.h"
#include "Stringoper.h"

// PublicConfig 对话框

IMPLEMENT_DYNAMIC(PublicConfig, CDialogEx)

PublicConfig::PublicConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(PublicConfig::IDD, pParent)
	, m_LeftThreshold(_T(""))
	, m_RightThreshold(_T(""))
	, m_WindowLength(_T(""))
	, m_OverLappedWinLen(_T(""))
{
}

PublicConfig::PublicConfig(CString path, CWnd* pParent)
: CDialogEx(PublicConfig::IDD, pParent)
, m_LeftThreshold(_T(""))
, m_RightThreshold(_T(""))
, m_WindowLength(_T(""))
, m_OverLappedWinLen(_T(""))
{
	m_ConfigPath = path;
}

PublicConfig::~PublicConfig()
{
}

void PublicConfig::SetConfigFilePath(CString path)
{
	m_ConfigPath = path;
}

BOOL PublicConfig::OnInitDialog()
{
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	m_Config.LoadParam();

	/*从配置文件Config.txt读取各参数的默认值*/
	m_WindowLength = oper.tocstring(oper.inttostring(m_Config.GetWindowLength()));
	m_OverLappedWinLen = oper.tocstring(oper.inttostring(m_Config.GetOverLength()));
	double a, b;
	m_Config.GetLRThrolds(a, b);
	m_LeftThreshold = oper.tocstring(oper.doubletostring(a));
	m_RightThreshold = oper.tocstring(oper.doubletostring(b));
	m_Config.CloseParmFile();
	UPDATAVALTOCONTROL;
	return true;
}

void PublicConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EWindowLength, m_WindowLength);
	DDX_Text(pDX, IDC_EOverLappedWinLen, m_OverLappedWinLen);
	DDX_Text(pDX, IDC_ELeftThreshold, m_LeftThreshold);
	DDX_Text(pDX, IDC_ERightThreshold, m_RightThreshold);
}

BEGIN_MESSAGE_MAP(PublicConfig, CDialogEx)
END_MESSAGE_MAP()


BOOL PublicConfig::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateData(false);
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void PublicConfig::SaveFile()
{
	Stringoper oper;
	double a, b;
	GetDlgItemText(IDC_EWindowLength, m_WindowLength);
	GetDlgItemText(IDC_ELeftThreshold,m_LeftThreshold);
	GetDlgItemText(IDC_ERightThreshold, m_RightThreshold);
	GetDlgItemText(IDC_EOverLappedWinLen, m_OverLappedWinLen);
	CAlgParamConfig m_Config(m_ConfigPath);
	m_Config.LoadParam();
	a = oper.stringtodouble(oper.tostring(m_LeftThreshold));
	b = oper.stringtodouble(oper.tostring(m_RightThreshold));
	m_Config.SetOverLength(oper.stringtoint(oper.tostring(m_OverLappedWinLen)));
	m_Config.SetWindowLength(oper.stringtoint(oper.tostring(m_WindowLength)));
	m_Config.SetLRThrolds(a, b);
	m_Config.SaveParam();
}