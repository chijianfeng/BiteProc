// TLConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "TLConfig.h"
#include "afxdialogex.h"
#include "FileOper.h"
#include "Structdefine.h"
#include "Streamoper.h"
#include "Stringoper.h"

// TLConfig 对话框

IMPLEMENT_DYNAMIC(TLConfig, CDialogEx)

TLConfig::TLConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(TLConfig::IDD, pParent)
	, m_LeftTLParamPath(_T(""))
	, m_LeftTLDataPath(_T(""))
	, m_BothTLParamPath(_T(""))
	, m_BothTLDataPath(_T(""))
	, m_RightTLParamPath(_T(""))
	, m_RightTLDataPath(_T(""))
	, m_LeftModelPath(_T(""))
	, m_BothModelPath(_T(""))
	, m_TLModulePath(_T(""))
	, m_RightModelPath(_T(""))
{
	
}

TLConfig::TLConfig(CString path, CWnd* pParent /* = NULL */)
: CDialogEx(TLConfig::IDD, pParent)
, m_LeftTLParamPath(_T(""))
, m_LeftTLDataPath(_T(""))
, m_BothTLParamPath(_T(""))
, m_BothTLDataPath(_T(""))
, m_RightTLParamPath(_T(""))
, m_RightTLDataPath(_T(""))
, m_LeftModelPath(_T(""))
, m_BothModelPath(_T(""))
, m_TLModulePath(_T(""))
, m_RightModelPath(_T(""))
{
	m_ConfigPath = path;
}

TLConfig::~TLConfig()
{
}

void TLConfig::SetConfigFilePath(CString path)
{
	m_ConfigPath = path;
}

BOOL TLConfig::OnInitDialog()
{
	Stringoper soper;
	Streamoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	m_Config.LoadParam();

	/*从配置文件Config.txt读取各参数的默认值*/
	vector<int> TLTrainTime;
	m_TwoBiteTrainTime = L"";
	m_Config.GetTLTrainTime(TLTrainTime);
	for (size_t i = 0; i < TLTrainTime.size(); i++)
	{
		m_TwoBiteTrainTime += soper.tocstring(soper.inttostring(TLTrainTime[i])) + L" ";
	}
	m_LeftTLParamPath = m_Config.GetTLLeftParamPath();
	m_LeftTLDataPath = m_Config.GetTLLeftData();
	m_BothTLParamPath = m_Config.GetTLBothParamPath();
	m_BothTLDataPath = m_Config.GetTLBothData();
	m_RightTLParamPath = m_Config.GetTLRightParamPath();
	m_RightTLDataPath = m_Config.GetTLRightData();
	m_LeftModelPath = m_Config.GetTLLeftModel();
	m_BothModelPath = m_Config.GetTLBothModel();
	m_RightModelPath = m_Config.GetTLRightModel();
	m_TLModulePath = m_Config.GetTLModule();

	if (m_Config.GetTLTrainState() == 1)
		((CButton*)GetDlgItem(IDC_RTLTrain))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_RTLNoTrain))->SetCheck(1);
	if (m_Config.GetTLReTrain() == 1)
		((CButton*)GetDlgItem(IDC_RTLReTrain))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_RTLNoReTrain))->SetCheck(1);
	
	m_Config.CloseParmFile();
	UPDATAVALTOCONTROL;	
	return true;
}

void TLConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ETwoBiteTrainTime, m_TwoBiteTrainTime);
	DDX_Text(pDX, IDC_ELeftTLParmaPath, m_LeftTLParamPath);
	DDX_Text(pDX, IDC_ELeftTLDataPath, m_LeftTLDataPath);
	DDX_Text(pDX, IDC_EBothTLParamPath, m_BothTLParamPath);
	DDX_Text(pDX, IDC_EBothTLDataPath, m_BothTLDataPath);
	DDX_Text(pDX, IDC_ERightTLParamPath, m_RightTLParamPath);
	DDX_Text(pDX, IDC_ERightTLDataPath, m_RightTLDataPath);
	DDX_Text(pDX, IDC_ELeftModelPath, m_LeftModelPath);
	DDX_Text(pDX, IDC_EBothModelPath, m_BothModelPath);
	DDX_Text(pDX, IDC_ETLModulePath, m_TLModulePath);
	DDX_Text(pDX, IDC_ERightModelPath, m_RightModelPath);
	
}


BEGIN_MESSAGE_MAP(TLConfig, CDialogEx)
    
	ON_BN_CLICKED(IDC_BLeftTLParmaPath, &TLConfig::OnBnClickedBlefttlparmapath)
	ON_BN_CLICKED(IDC_BLeftTLDataPath, &TLConfig::OnBnClickedBlefttldatapath)
	ON_BN_CLICKED(IDC_BBothTLParamPath, &TLConfig::OnBnClickedBbothtlparampath)
	ON_BN_CLICKED(IDC_BBothTLDataPath, &TLConfig::OnBnClickedBbothtldatapath)
	ON_BN_CLICKED(IDC_BRightTLParamPath, &TLConfig::OnBnClickedBrighttlparampath)
	ON_BN_CLICKED(IDC_BRightTLDataPath, &TLConfig::OnBnClickedBrighttldatapath)
	ON_BN_CLICKED(IDC_BLeftModelPath, &TLConfig::OnBnClickedBleftmodelpath)
	ON_BN_CLICKED(IDC_BBothModelPath, &TLConfig::OnBnClickedBbothmodelpath)
	ON_BN_CLICKED(IDC_BTLModulePath, &TLConfig::OnBnClickedBtlmodulepath)
	ON_BN_CLICKED(IDC_BRightModelPath, &TLConfig::OnBnClickedBrightmodelpath)
END_MESSAGE_MAP()


// TLConfig 消息处理程序


void TLConfig::OnBnClickedBlefttlparmapath()
{
	CFileOper foper;
	m_LeftTLParamPath = foper.PathSelect(L"Data Files|*.txt||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	if (m_LeftTLParamPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetTLLeftParamPath(m_LeftTLParamPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void TLConfig::OnBnClickedBlefttldatapath()
{
	CFileOper foper;
	m_LeftTLDataPath = foper.PathSelect(L"Data Files|*.dat||");
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);

	/*若搜索到.dat文件，将该文件的路径值写回配置文件Config.txt*/
	if (m_LeftTLDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetTLLeftData(m_LeftTLDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void TLConfig::OnBnClickedBbothtlparampath()
{
	CFileOper foper;
	m_BothTLParamPath = foper.PathSelect(L"Data Files|*.txt||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	if (m_BothTLParamPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetTLBothParamPath(m_BothTLParamPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void TLConfig::OnBnClickedBbothtldatapath()
{
	CFileOper foper;
	m_BothTLDataPath = foper.PathSelect(L"Data Files|*.dat||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);

	/*若搜索到.dat文件，将该文件的路径值写回配置文件Config.txt*/
	if (m_BothTLDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetTLBothData(m_BothTLDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void TLConfig::OnBnClickedBrighttlparampath()
{
	CFileOper foper;
	m_RightTLParamPath = foper.PathSelect(L"Data Files|*.txt||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	if (m_RightTLParamPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetTLRightParamPath(m_RightTLParamPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void TLConfig::OnBnClickedBrighttldatapath()
{
	CFileOper foper;
	m_RightTLDataPath = foper.PathSelect(L"Data Files|*.dat||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	if (m_RightTLDataPath != "" )
	{
		m_Config.LoadParam();
		m_Config.SetTLRightData(m_RightTLDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void TLConfig::OnBnClickedBleftmodelpath()
{
	CFileOper foper;
	m_LeftModelPath = foper.PathSelect(L"Data Files|*.txt||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	if (m_LeftModelPath != "" )
	{
		m_Config.LoadParam();
		m_Config.SetTLLeftModel(m_LeftModelPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void TLConfig::OnBnClickedBbothmodelpath()
{
	CFileOper foper;
	m_BothModelPath = foper.PathSelect(L"Data Files|*.txt||");

	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	if (m_BothModelPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetTLBothModel(m_BothModelPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void TLConfig::OnBnClickedBtlmodulepath()
{
	CFileOper foper;
	m_TLModulePath = foper.PathSelect(L"Data Files|*.dll||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	if (m_TLModulePath != "")
	{
		m_Config.LoadParam();
		m_Config.SetTLModule(m_TLModulePath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void TLConfig::OnBnClickedBrightmodelpath()
{
	CFileOper foper;
	m_RightModelPath = foper.PathSelect(L"Data Files|*.txt||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	if (m_RightModelPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetTLRightModel(m_RightModelPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


BOOL TLConfig::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateData(false);
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void TLConfig::SaveFile()
{
	Stringoper soper;
	Streamoper oper;
	vector<int> VTrainTime;
	int TrainTime[10];
	int number;
	GetDlgItemText(IDC_ETwoBiteTrainTime, m_TwoBiteTrainTime);
	CAlgParamConfig m_Config(m_ConfigPath);
	m_Config.LoadParam();
	oper.InitDeseri(soper.tostring(m_TwoBiteTrainTime));
	oper.DeserizlizeStream(TrainTime, number, false);

	for (int i = 0; i < number; i++)
	{
		if (TrainTime[i] >= 0)
			VTrainTime.push_back(TrainTime[i]);
	}
	m_Config.SetTLTrainTime(VTrainTime);
	if (((CButton*)GetDlgItem(IDC_RTLReTrain))->GetCheck())
		m_Config.SetTLReTrain(1);
	if (((CButton*)GetDlgItem(IDC_RTLNoReTrain))->GetCheck())
		m_Config.SetTLReTrain(0);
	if (((CButton*)GetDlgItem(IDC_RTLTrain))->GetCheck())
		m_Config.SetTLTrainState(1);
	if (((CButton*)GetDlgItem(IDC_RTLNoTrain))->GetCheck())
		m_Config.SetTLTrainState(0);
	m_Config.SaveParam();
	vector <int>().swap(VTrainTime);
}