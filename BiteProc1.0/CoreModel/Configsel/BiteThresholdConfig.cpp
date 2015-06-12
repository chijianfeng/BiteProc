// BiteThresholdConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "BiteThresholdConfig.h"
#include "afxdialogex.h"
#include "FileOper.h"
#include "Structdefine.h"
#include "Streamoper.h"
#include "Stringoper.h"

// BiteThresholdConfig 对话框

IMPLEMENT_DYNAMIC(BiteThresholdConfig, CDialogEx)

BiteThresholdConfig::BiteThresholdConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(BiteThresholdConfig::IDD, pParent)
	, m_biteThresholdPath(_T(""))
	, m_biteDataPath(_T(""))
	, m_biteThroldModulePath(_T(""))
	, m_biteTrainTime(_T(""))
	, m_biteCount(_T(""))
{
}

BiteThresholdConfig::BiteThresholdConfig(CString path, CWnd* pParent /* = NULL */)
: CDialogEx(BiteThresholdConfig::IDD, pParent)
, m_biteThresholdPath(_T(""))
, m_biteDataPath(_T(""))
, m_biteThroldModulePath(_T(""))
, m_biteTrainTime(_T(""))
, m_biteCount(_T(""))
{
	m_ConfigPath = path;
}

BiteThresholdConfig::~BiteThresholdConfig()
{
}

BOOL BiteThresholdConfig::OnInitDialog()
{
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	m_Config.LoadParam();

	/*从配置文件Config.txt读取各参数的默认值*/
	m_biteThresholdPath = m_Config.GetBiteThroldModel();
	m_biteDataPath = m_Config.GetBiteDataPath();
	m_biteThroldModulePath = m_Config.GetBiteModulePath();

	int time, count;
	m_Config.GetBiteTrain(time, count);
	m_biteTrainTime = oper.tocstring(oper.inttostring(time));
	m_biteCount = oper.tocstring(oper.inttostring(count));


	if (m_Config.GetBitestate() == 1)
		((CButton*)GetDlgItem(IDC_RBiteTrain))->SetCheck(1);

	else
		((CButton*)GetDlgItem(IDC_RBiteNotTrain))->SetCheck(1);

	m_Config.CloseParmFile();
	UPDATAVALTOCONTROL;
	return true;
}

void BiteThresholdConfig::SetConfigFilePath(CString path)
{
	m_ConfigPath = path;
}

void BiteThresholdConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_EBiteTrainTime, m_biteTrainTime);
	DDX_Text(pDX, IDC_EBiteCount,m_biteCount);
	DDX_Text(pDX, IDC_EBiteDataPath, m_biteDataPath);
	DDX_Text(pDX, IDC_EBiteThroldPath, m_biteThresholdPath);
	DDX_Text(pDX, IDC_EThroldMoudelPath, m_biteThroldModulePath);

}


BEGIN_MESSAGE_MAP(BiteThresholdConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BBiteDataPath, &BiteThresholdConfig::OnBnClickedBitedatapath)
	ON_BN_CLICKED(IDC_BBiteThroldPath, &BiteThresholdConfig::OnBnClickedBbitethroldpath)
	ON_BN_CLICKED(IDC_BBiteThroldModulePath, &BiteThresholdConfig::OnBnClickedBbitethroldmodulepath)
END_MESSAGE_MAP()



void BiteThresholdConfig::OnBnClickedBitedatapath()
{
	CFileOper foper;
	m_biteDataPath = foper.PathSelect(L"Data Files|*.dat||");         //搜索目录寻找.dat文件
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	/*若搜索到.dat文件，将该文件的路径值写回配置文件Config.txt*/
	if (m_biteDataPath != "")
	{
		m_Config.LoadParam();       
		m_Config.SetBiteDataPath(m_biteDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}	
	
}


void BiteThresholdConfig::OnBnClickedBbitethroldpath()
{
	CFileOper foper;
	m_biteThresholdPath = foper.PathSelect(L"Data Files|*.txt||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_biteThresholdPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetBiteThorldModel(m_biteThresholdPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void BiteThresholdConfig::OnBnClickedBbitethroldmodulepath()
{
	Stringoper oper;
	CFileOper foper;
	m_biteThroldModulePath = foper.PathSelect(L"Data Files|*.dll||");

	CAlgParamConfig m_Config(m_ConfigPath);
	
	/*若搜索到.dll文件，将该文件的路径值写回配置文件Config.txt*/
	if (m_biteThroldModulePath != "")
	{
		m_Config.LoadParam();
		m_Config.SetBiteModulePath(m_biteThroldModulePath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


/*使回车事件不会结束窗口运行*/
BOOL BiteThresholdConfig::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateData(false);
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void BiteThresholdConfig::SaveFile()
{
	Stringoper oper;
	int time, count;
	GetDlgItemText(IDC_EBiteTrainTime, m_biteTrainTime);
	GetDlgItemText(IDC_EBiteCount, m_biteCount);                        //add
	CAlgParamConfig m_Config(m_ConfigPath);
	m_Config.LoadParam();
	time = oper.stringtoint(oper.tostring(m_biteTrainTime));
	count = oper.stringtoint(oper.tostring(m_biteCount));
	m_Config.SetBiteTrain(time, count);
	if (((CButton*)GetDlgItem(IDC_RBiteTrain))->GetCheck())
		m_Config.SetBiteState(1);
	if (((CButton*)GetDlgItem(IDC_RBiteNotTrain))->GetCheck())
		m_Config.SetBiteState(0);
	m_Config.SaveParam();
}