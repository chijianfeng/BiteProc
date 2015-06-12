// DeviceSelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CoreModel.h"
#include "DeviceSelDlg.h"
#include "afxdialogex.h"
#include "FileOper.h"
#include "Structdefine.h"
#include "DeviceConfig.h"
#include "./Loader/DeviceLoader.cpp"
#include "BasicDevice.h"
#include "Stringoper.h"

// CDeviceSelDlg 对话框

IMPLEMENT_DYNAMIC(CDeviceSelDlg, CDialogEx)

CDeviceSelDlg::CDeviceSelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceSelDlg::IDD, pParent)
	, m_LParam(_T(""))
{

}
CDeviceSelDlg::CDeviceSelDlg(CUserConfig& config , CWnd* pParent) : CDialogEx(CDeviceSelDlg::IDD, pParent)
{
	config.GetUserPath(config.GetCurUser(),m_strUserPath);
	CString str = L"\\";
	str += _T(DEVICEFILE);
	m_strUserPath += str;							//设备的配置文件
}
CDeviceSelDlg::~CDeviceSelDlg()
{
}

void CDeviceSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDEVCHANNELS, m_strChannels);
	DDX_Text(pDX, IDC_EDEVSEARCHDIR, m_strSearchDir);
	DDX_Text(pDX, IDC_EDEVBUFFERSIZE, m_nBufSize);

	DDX_Control(pDX, IDC_COMDEVSAMPLERATE, m_comSampleRate);
	DDX_Control(pDX, IDC_COMDEVSEL, m_comDeviceSel);
	DDX_Text(pDX, IDC_ELPARAM, m_LParam);      //hwh_add
}


BEGIN_MESSAGE_MAP(CDeviceSelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BDEVSEARCHDIR, &CDeviceSelDlg::OnBnClickedBdevsearchdir)
	ON_BN_CLICKED(IDOK, &CDeviceSelDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMDEVSEL, &CDeviceSelDlg::OnCbnSelchangeComdevsel)
	ON_BN_CLICKED(IDC_BLPARAM, &CDeviceSelDlg::OnBnClickedBlparam)
	ON_BN_CLICKED(IDC_BLPARAM, &CDeviceSelDlg::OnBnClickedBlparam)
END_MESSAGE_MAP()


// CDeviceSelDlg 消息处理程序
BOOL CDeviceSelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_strUserPath.GetLength() <= 0)
		return FALSE;

	CFileOper foper;
	if (!foper.FileExist(m_strUserPath))
		return FALSE;

	m_strSearchDir = foper.GetExeFilePath();
	CheckDevice();

	m_comSampleRate.AddString(L"64");
	m_comSampleRate.AddString(L"128");
	m_comSampleRate.AddString(L"256");
	m_comSampleRate.AddString(L"512");
	m_comSampleRate.AddString(L"1024");
	Stringoper oper;
	CDeviceConfig config(m_strUserPath);
	config.LoadFileMsg();

	m_nBufSize = (size_t)config.GetBuffersize();
	m_comSampleRate.SetCurSel(config.GetSamplerate() / 64 - 1);
	m_LParam = oper.tocstring(config.GetParam());                 //hwh_add
	m_strDllPath = config.GetDevicePath();

	if (config.GetMode() == 0)
		((CButton*)GetDlgItem(IDC_RDEVNORMAL))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_RDEVIMP))->SetCheck(1);

	UCHAR *pchan = new UCHAR[config.GetChannum()];
	config.GetChans(pchan, config.GetChannum());
	m_strChannels = L"";
	for (int i = 0; i < config.GetChannum();i++)
	{
		m_strChannels += oper.tocstring(oper.inttostring((int)pchan[i]))+L" ";
	}
	delete pchan;

	UPDATAVALTOCONTROL;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDeviceSelDlg::OnBnClickedBdevsearchdir()
{
	// TODO:  选择搜索路径
	CFileOper oper;
	m_strSearchDir = oper.DirectorySelect(this->GetSafeHwnd());
	UPDATAVALTOCONTROL;
	CheckDevice();
}

//private operation

void CDeviceSelDlg::CheckDevice()
{
	CDeviceLoader<BasicDevice> loader;
	CFileOper oper;
	vector<CString> tmplist;
	oper.FindFiles(tmplist, m_strSearchDir, L"*.dll");		//找到所有dll

	BasicDevice *pd=NULL;

	m_comDeviceSel.Clear();
	for each (auto obj in tmplist)
	{
		loader.SetDevicePath(obj);
		loader.Setparma("   ");
		if (loader.LoadDevice(&pd)&&pd!=NULL)
		{
			m_listPaths.push_back(obj);
			CString name = obj.Right(obj.GetLength()-obj.ReverseFind('\\')-1);
			m_comDeviceSel.AddString(name);
			
			delete pd;
			loader.Free();
		}
	}
}

void CDeviceSelDlg::OnBnClickedOk()
{
	// TODO: Save the paramnent to file.
	UPDATACONTROLTOVAL;
	Streamoper oper;
	Stringoper soepr;

	CDeviceConfig config(m_strUserPath);
	config.SetParam(soepr.tostring(m_LParam));        //hwh_add
	config.SetBuffersize((int)m_nBufSize); 
	config.SetDevicePath(m_strDllPath);
	if (((CButton*)GetDlgItem(IDC_RDEVNORMAL))->GetCheck())
		config.SetMode(0);
	else
		config.SetMode(1);

	int index = m_comSampleRate.GetCurSel();
	CString str;
	m_comSampleRate.GetLBText(index, str);
	config.SetSamplerate(soepr.stringtoint((soepr.tostring(str))));

	
	m_strChannels.TrimRight();
	oper.InitDeseri(soepr.tostring(m_strChannels));
	int chans[100];
	int number;
	oper.DeserizlizeStream(chans, number, false);

	UCHAR *pchan = new UCHAR[number];
	for (int i = 0; i < number;i++)
	{
		pchan[i] = (UCHAR)chans[i];
	}
	config.SetChans(pchan, number);
	delete pchan;
	
	config.SaveFileMsg();							//保存信息

	CDialogEx::OnOK();
}


void CDeviceSelDlg::OnCbnSelchangeComdevsel()
{
	CString name;
	m_comDeviceSel.GetLBText(m_comDeviceSel.GetCurSel(), name);

	for each (auto path in m_listPaths)
	{
		CString obj = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
		if (Equalwstr(obj, name))
		{
			m_strDllPath = path;
			break;
		}
	}
}


void CDeviceSelDlg::OnBnClickedBlparam()        //hwh_add
{
	Stringoper oper;
	CFileOper foper;
	m_LParam = foper.PathSelect(L"Data Files|*.dat||");

	CDeviceConfig Config(m_strUserPath);

	/*若搜索到.dat文件，将该文件的路径值写回配置文件Config.txt*/
	if (m_LParam != "")
	{
		Config.LoadFileMsg();
		Config.SetParam(oper.tostring(m_LParam));
		UPDATAVALTOCONTROL;
	}
}
