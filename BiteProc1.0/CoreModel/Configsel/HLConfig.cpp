// HLConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "HLConfig.h"
#include "afxdialogex.h"
#include "FileOper.h"
#include "Structdefine.h"
#include "Streamoper.h"
#include "Stringoper.h"

// HLConfig 对话框

IMPLEMENT_DYNAMIC(HLConfig, CDialogEx)

HLConfig::HLConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(HLConfig::IDD, pParent)
	, m_HLTrainTime(_T(""))
	, m_HLModelPath(_T(""))
	, m_HLModulePath(_T(""))
	, m_LeftLightDataPath(_T(""))
	, m_LeftHeavyDataPath(_T(""))
	, m_RightLightDataPath(_T(""))
	, m_RightHeavyDataPath(_T(""))
	, m_BothLightDataPath(_T(""))
	, m_BothHeavyDataPath(_T(""))
{
}

HLConfig::HLConfig(CString path, CWnd*pParent /* = NULL */)
: CDialogEx(HLConfig::IDD, pParent)
, m_HLTrainTime(_T(""))
, m_HLModelPath(_T(""))
, m_HLModulePath(_T(""))
, m_LeftLightDataPath(_T(""))
, m_LeftHeavyDataPath(_T(""))
, m_RightLightDataPath(_T(""))
, m_RightHeavyDataPath(_T(""))
, m_BothLightDataPath(_T(""))
, m_BothHeavyDataPath(_T(""))
{
	m_ConfigPath = path;
}

HLConfig::~HLConfig()
{
}

void HLConfig::SetConfigFilePath(CString path)
{
	m_ConfigPath = path;
}

BOOL HLConfig::OnInitDialog()
{
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	m_Config.LoadParam();

	/*从配置文件Config.txt读取各参数的默认值*/
	vector<int> HLTrainTime;
	m_HLTrainTime = L"";
	m_Config.GetHLTrainTime(HLTrainTime);
	for (size_t i = 0; i < HLTrainTime.size(); i++)
	{
		m_HLTrainTime += oper.tocstring(oper.inttostring(HLTrainTime[i]))+L" ";
	}
	m_HLModelPath = m_Config.GetHLModel();
	m_HLModulePath = m_Config.GetHLModule();
	m_LeftLightDataPath = m_Config.GetLLData();
	m_LeftHeavyDataPath = m_Config.GetLHData();
	m_RightLightDataPath = m_Config.GetRLData();
	m_RightHeavyDataPath = m_Config.GetRHData();
	m_BothLightDataPath = m_Config.GetBLData();
	m_BothHeavyDataPath = m_Config.GetBHData();

	if (m_Config.GetHLState() == 1)
		((CButton*)GetDlgItem(IDC_RHLTrain))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_RHLNoTrain))->SetCheck(1);
	if (m_Config.GetHLReTrain() == 1)
		((CButton*)GetDlgItem(IDC_RHLReTrain))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_RHLNoReTrain))->SetCheck(1);
	m_Config.CloseParmFile();

	UPDATAVALTOCONTROL;
	return true;
}

void HLConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EHLTrainTime, m_HLTrainTime);
	DDX_Text(pDX, IDC_EHLModelPath, m_HLModelPath);
	DDX_Text(pDX, IDC_EHLModulePath, m_HLModulePath);
	DDX_Text(pDX, IDC_ELeftLightDataPath, m_LeftLightDataPath);
	DDX_Text(pDX, IDC_ELeftHeavyDataPath, m_LeftHeavyDataPath);
	DDX_Text(pDX, IDC_ERightLightDataPath, m_RightLightDataPath);
	DDX_Text(pDX, IDC_ERightHeavyDataPath, m_RightHeavyDataPath);
	DDX_Text(pDX, IDC_EBothLightDataPath, m_BothLightDataPath);
	DDX_Text(pDX, IDC_EBothHeavyDataPath, m_BothHeavyDataPath);
}


BEGIN_MESSAGE_MAP(HLConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BHLModelPath, &HLConfig::OnBnClickedBhlmodelpath)
	ON_BN_CLICKED(IDC_BHLModulePath, &HLConfig::OnBnClickedBhlmodulepath)
	ON_BN_CLICKED(IDC_BLeftLightDataPath, &HLConfig::OnBnClickedBleftlightdatapath)
	ON_BN_CLICKED(IDC_BLeftHeavyDataPath, &HLConfig::OnBnClickedBleftheavydatapath)
	ON_BN_CLICKED(IDC_BRightLightDataPath, &HLConfig::OnBnClickedBrightlightdatapath)
	ON_BN_CLICKED(IDC_BRightHeavyDataPath, &HLConfig::OnBnClickedBrightheavydatapath)
	ON_BN_CLICKED(IDC_BBothLightDataPath, &HLConfig::OnBnClickedBbothlightdatapath)
	ON_BN_CLICKED(IDC_BBothHeavyDataPath, &HLConfig::OnBnClickedBbothheavydatapath)
END_MESSAGE_MAP()


// HLConfig 消息处理程序


void HLConfig::OnBnClickedBhlmodelpath()
{
	CFileOper foper;
	m_HLModelPath = foper.PathSelect(L"Data Files|*.txt||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_HLModelPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetHLModel(m_HLModelPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void HLConfig::OnBnClickedBhlmodulepath()
{
	CFileOper foper;
	m_HLModulePath = foper.PathSelect(L"Data Files|*.dll||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	/*若搜索到.dll文件，将该文件的路径值写回配置文件Config.txt*/
	if (m_HLModulePath != "")
	{
		m_Config.LoadParam();
		m_Config.SetHLModule(m_HLModulePath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void HLConfig::OnBnClickedBleftlightdatapath()
{
	CFileOper foper;
	m_LeftLightDataPath = foper.PathSelect(L"Data Files|*.dat||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	/*若搜索到.dat文件，将该文件的路径值写回配置文件Config.txt*/
	if (m_LeftLightDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetLLData(m_LeftLightDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void HLConfig::OnBnClickedBleftheavydatapath()
{
	CFileOper foper;
	m_LeftHeavyDataPath = foper.PathSelect(L"Data Files|*.dat||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_LeftHeavyDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetLHData(m_LeftHeavyDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void HLConfig::OnBnClickedBrightlightdatapath()
{
	CFileOper foper;
	m_RightLightDataPath = foper.PathSelect(L"Data Files|*.dat||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_RightLightDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetRLData(m_RightLightDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void HLConfig::OnBnClickedBrightheavydatapath()
{
	CFileOper foper;
	m_RightHeavyDataPath = foper.PathSelect(L"Data Files|*.dat||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_RightHeavyDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetRLData(m_RightHeavyDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void HLConfig::OnBnClickedBbothlightdatapath()
{
	CFileOper foper;
	m_BothLightDataPath = foper.PathSelect(L"Data Files|*.dat||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_BothLightDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetBLData(m_BothLightDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void HLConfig::OnBnClickedBbothheavydatapath()
{
	CFileOper foper;
	m_BothHeavyDataPath = foper.PathSelect(L"Data Files|*.dat||");
	
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_BothHeavyDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetBHData(m_BothHeavyDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


BOOL HLConfig::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateData(false);
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void HLConfig::SaveFile()
{
	Stringoper soper;
	Streamoper oper;
	vector<int> VTrainTime;
	int TrainTime[10];
	int number;
	GetDlgItemText(IDC_EHLTrainTime, m_HLTrainTime);
	GetDlgItemText(IDC_EHLModelPath, m_HLModelPath);
	CAlgParamConfig m_Config(m_ConfigPath);
	m_Config.LoadParam();
	oper.InitDeseri(soper.tostring(m_HLTrainTime));
	oper.DeserizlizeStream(TrainTime, number, false);

	for (int i = 0; i < number-1; i++)
	{
		if (TrainTime[i] >= 0)
			VTrainTime.push_back(TrainTime[i]);
	}
	m_Config.SetHLTrainTime(VTrainTime);
	m_Config.SetHLModel(m_HLModelPath);
	if (((CButton*)GetDlgItem(IDC_RHLTrain))->GetCheck())
		m_Config.SetHLState(1);
	if (((CButton*)GetDlgItem(IDC_RHLNoTrain))->GetCheck())
		m_Config.SetHLState(0);
	if (((CButton*)GetDlgItem(IDC_RHLReTrain))->GetCheck())
		m_Config.SetHLReTrain(1);
	if (((CButton*)GetDlgItem(IDC_RHLNoReTrain))->GetCheck())
		m_Config.SetHLReTrain(0);

	m_Config.SaveParam();
	vector <int>().swap(VTrainTime);
}