// LeftRightBothConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "LeftRightBothConfig.h"
#include "afxdialogex.h"
#include "FileOper.h"
#include "Structdefine.h"
#include "Streamoper.h"
#include "Stringoper.h"

// LeftRightBothConfig 对话框

IMPLEMENT_DYNAMIC(LeftRightBothConfig, CDialogEx)

LeftRightBothConfig::LeftRightBothConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(LeftRightBothConfig::IDD, pParent)
	, m_ModelTrainTime(_T(""))
	, m_ModelPath(_T(""))
	, m_LeftDataPath(_T(""))
	, m_RightDataPath(_T(""))
	, m_BothDataPath(_T(""))
	, m_ModulePath(_T(""))
{
}

LeftRightBothConfig::LeftRightBothConfig(CString path, CWnd* pParent /* = NULL */)
: CDialogEx(LeftRightBothConfig::IDD, pParent)
, m_ModelTrainTime(_T(""))
, m_ModelPath(_T(""))
, m_LeftDataPath(_T(""))
, m_RightDataPath(_T(""))
, m_BothDataPath(_T(""))
, m_ModulePath(_T(""))
{
	m_ConfigPath = path;
}

LeftRightBothConfig::~LeftRightBothConfig()
{
}

void LeftRightBothConfig::SetConfigFilePath(CString path)
{
	m_ConfigPath = path;
}

BOOL LeftRightBothConfig::OnInitDialog()
{
	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	m_Config.LoadParam();

	/*从配置文件Config.txt读取各参数的默认值*/
	vector<int> TrainTime;
	m_ModelTrainTime = L"";
	m_Config.GetClassifyTrain(TrainTime);
	for (size_t i = 0; i < TrainTime.size(); i++)
	{
		m_ModelTrainTime += oper.tocstring(oper.inttostring(TrainTime[i])) + L" ";
	}
	m_ModelPath = m_Config.GetClassifyModel();
	m_LeftDataPath = m_Config.GetClassifyLData();
	m_RightDataPath = m_Config.GetClassifyRData();
	m_BothDataPath = m_Config.GetClassifyBData();
	m_ModulePath = m_Config.GetClassifyModule();
	
	if (m_Config.GetClassifystate() == 1)
		((CButton*)GetDlgItem(IDC_RModelTrain))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_RModelNotTrain))->SetCheck(1);
	if (m_Config.IsRetrainClassify() == 1)
		((CButton*)GetDlgItem(IDC_RModelReTrain))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_RModelNoReTrain))->SetCheck(1);
	
	m_Config.CloseParmFile();
	UPDATAVALTOCONTROL;
	return true;
}

void LeftRightBothConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EModelTrainTime, m_ModelTrainTime);
	DDX_Text(pDX, IDC_EModelPath, m_ModelPath);
	DDX_Text(pDX, IDC_ELeftDataPath, m_LeftDataPath);
	DDX_Text(pDX, IDC_ERightDataPath, m_RightDataPath);
	DDX_Text(pDX, IDC_EBothDataPath, m_BothDataPath);
	DDX_Text(pDX, IDC_EModulePath, m_ModulePath);
}


BEGIN_MESSAGE_MAP(LeftRightBothConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BModelPath, &LeftRightBothConfig::OnBnClickedBmodelpath)
	ON_BN_CLICKED(IDC_BLeftDataPath, &LeftRightBothConfig::OnBnClickedBleftdatapath)
	ON_BN_CLICKED(IDC_BRightDataPath, &LeftRightBothConfig::OnBnClickedBrightdatapath)
	ON_BN_CLICKED(IDC_BBothDataPath, &LeftRightBothConfig::OnBnClickedBbothdatapath)
	ON_BN_CLICKED(IDC_BModulePath, &LeftRightBothConfig::OnBnClickedBmodulepath)
END_MESSAGE_MAP()


// LeftRightBothConfig 消息处理程序


void LeftRightBothConfig::OnBnClickedBmodelpath()
{
	CFileOper foper;
	m_ModelPath = foper.PathSelect(L"Data Files|*.txt||");

	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_ModelPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetClassifyModel(m_ModelPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void LeftRightBothConfig::OnBnClickedBleftdatapath()
{
	CFileOper foper;
	m_LeftDataPath = foper.PathSelect(L"Data Files|*.dat||");

	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	/*若搜索到.dat文件，将该文件的路径值写回配置文件Config.txt*/
	if (m_LeftDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetClassifyLData(m_LeftDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void LeftRightBothConfig::OnBnClickedBrightdatapath()
{
	CFileOper foper;
	m_RightDataPath = foper.PathSelect(L"Data Files|*.dat||");

	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_RightDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetClassifyRData(m_RightDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void LeftRightBothConfig::OnBnClickedBbothdatapath()
{
	CFileOper foper;
	m_BothDataPath = foper.PathSelect(L"Data Files|*.dat||");

	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_BothDataPath != "")
	{
		m_Config.LoadParam();
		m_Config.SetClassifyBData(m_BothDataPath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}


void LeftRightBothConfig::OnBnClickedBmodulepath()
{
	CFileOper foper;
	m_ModulePath = foper.PathSelect(L"Data Files|*.dll||");

	Stringoper oper;
	CAlgParamConfig m_Config(m_ConfigPath);
	
	if (m_ModulePath != "")
	{
		m_Config.LoadParam();
		m_Config.SetClassifyModule(m_ModulePath);
		m_Config.SaveParam();
		UPDATAVALTOCONTROL;
	}
	
}

BOOL LeftRightBothConfig::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateData(false);
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void LeftRightBothConfig::SaveFile()
{
	Stringoper soper;
	Streamoper oper;
	vector<int> VTrainTime;
	int TrainTime[10];
	int number;

	GetDlgItemText(IDC_EModelTrainTime, m_ModelTrainTime);
	CAlgParamConfig m_Config(m_ConfigPath);
	m_Config.LoadParam();
	oper.InitDeseri(soper.tostring(m_ModelTrainTime));
	oper.DeserizlizeStream(TrainTime, number, false);

	for (int i = 0; i < number; i++)
	{
		if (TrainTime[i] >= 0)
			VTrainTime.push_back(TrainTime[i]);
	}
	m_Config.SetClassifyTain(VTrainTime);
	if (((CButton*)GetDlgItem(IDC_RModelTrain))->GetCheck())
		m_Config.SetClassifyState(1);
	if (((CButton*)GetDlgItem(IDC_RModelNotTrain))->GetCheck())
		m_Config.SetClassifyState(0);
	if (((CButton*)GetDlgItem(IDC_RModelReTrain))->GetCheck())
		m_Config.ReTrainClassify(1);
	if (((CButton*)GetDlgItem(IDC_RModelNoReTrain))->GetCheck())
		m_Config.ReTrainClassify(0);
	m_Config.SaveParam();
	vector <int>().swap(VTrainTime);
}