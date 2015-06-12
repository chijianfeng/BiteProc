// InitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CoreModel.h"
#include "InitDlg.h"
#include "afxdialogex.h"
#include "AlgParamConfig.h"
#include "Structdefine.h"

// CInitDlg 对话框

IMPLEMENT_DYNAMIC(CInitDlg, CDialogEx)

CString Param_files[]={
	_T(CONFIGFILE), _T(DEVICEFILE), _T(VIEWFILE), _T(APPFILE),
	_T(TLLEFTPARAM), _T(TLRIGHTPARAM), _T(TLBOTHPARAM),
};

CInitDlg::CInitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInitDlg::IDD, pParent)
{

}

CInitDlg::~CInitDlg()
{
}

void CInitDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX , IDC_COMBOUSERLIST ,m_ComboUser);
	DDX_Control(pDX , IDC_CHECKUSER , m_Checkmsg);
	DDX_Text(pDX ,IDC_EUSERNAME , m_Username);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInitDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInitDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInitDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBOUSERLIST, &CInitDlg::OnCbnSelchangeCombouserlist)
	ON_BN_CLICKED(IDC_CHECKUSER, &CInitDlg::OnBnClickedCheckuser)
END_MESSAGE_MAP()


// CInitDlg 消息处理程序


void CInitDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CString path = _T("");
	map<CString,CString> userlist;
	m_UserConfig.GetUserlist(userlist);
	for(map<CString,CString>::iterator it=userlist.begin();it!=userlist.end();it++)
	{
		if(m_Username.Compare(it->first)==0)
		{
			path = it->second;
		}
	}
	if(path.GetLength()<=0&&!m_UserConfig.AddNewUser(m_Username , _T("..\\Files\\")+m_Username))
	{
		AfxMessageBox(_T("用户创建失败"));
		return;
	}
	m_UserConfig.GetUserPath(m_Username,path);
	m_UserConfig.SetCurUser(m_Username);
	m_UserConfig.SetDefault(m_Checkmsg.GetState());
	if(!PathIsDirectory(path))
	{
		if(!CreateDirectory(path , NULL))
		{
			AfxMessageBox(_T("文件创建失败！"));
			return;
		}
		//copy files.
		CopyFiles(Param_files,7,path+_T("\\"));
	}
	//change the Config file and create the file
	CString dir = path + _T("\\" + CONFIGFILE);					//配置文件路径
	CAlgParamConfig Configfile(dir);
	Configfile.LoadParam();

	path = path + L"\\";
	//chang the file dir
	if (GetFileName(Configfile.GetBiteThroldModel()).GetLength() > 0)		//设置模型保存路径
		Configfile.SetBiteThorldModel(path + GetFileName(Configfile.GetBiteThroldModel()));
	if (GetFileName(Configfile.GetBiteDataPath()).GetLength() > 0)
		Configfile.SetBiteDataPath(path + GetFileName(Configfile.GetBiteDataPath()));	//设置数据路径
	CString name = GetFileName(Configfile.GetClassifyModel());
	if (name.GetLength() > 0)
		Configfile.SetClassifyModel(path + name);							//设置分离模型
	name = GetFileName(Configfile.GetClassifyLData());
	if (name.GetLength() > 0)
		Configfile.SetClassifyLData(path + name);
	name = GetFileName(Configfile.GetClassifyBData());
	if (name.GetLength() > 0)
		Configfile.SetClassifyBData(path + name);
	name = GetFileName(Configfile.GetClassifyRData());
	if (name.GetLength() > 0)
		Configfile.SetClassifyRData(path + name);
	//change the HL file param
	name = GetFileName(Configfile.GetHLModel());
	if (name.GetLength() > 0)
		Configfile.SetHLModel( path+name);
	name = GetFileName(Configfile.GetLLData());
	if (name.GetLength() > 0)
		Configfile.SetLLData(path + name);
	name = GetFileName(Configfile.GetLHData());
	if (name.GetLength() > 0)
		Configfile.SetLHData(path + name);
	name = GetFileName(Configfile.GetBLData());
	if (name.GetLength() > 0)
		Configfile.SetBLData(path + name);
	name = GetFileName(Configfile.GetBHData());
	if (name.GetLength() > 0)
		Configfile.SetBHData(path + name);
	name = GetFileName(Configfile.GetRLData());
	if (name.GetLength() > 0)
		Configfile.SetRLData(path + name);
	name = GetFileName(Configfile.GetRHData());
	if (name.GetLength() > 0)
		Configfile.SetRHData(path + name);

	//TL param
	name = GetFileName(Configfile.GetTLLeftModel());
	if (name.GetLength() > 0)
		Configfile.SetTLLeftModel(path + name);
	name = GetFileName(Configfile.GetTLBothModel());
	if (name.GetLength()>0)
		Configfile.SetTLBothModel(path + name);
	name = GetFileName(Configfile.GetTLRightModel());
	if (name.GetLength() > 0)
		Configfile.SetTLRightModel(path + name);
	name = GetFileName(Configfile.GetTLLeftData());
	if (name.GetLength() > 0)
		Configfile.SetTLLeftData(path + name);
	name = GetFileName(Configfile.GetTLBothData());
	if (name.GetLength() > 0)
		Configfile.SetTLBothData(path + name);
	name = GetFileName(Configfile.GetTLRightData());
	if (name.GetLength() > 0)
		Configfile.SetTLRightData(path + name);

	Configfile.SaveParam();
	Configfile.CloseParmFile();

	m_UserConfig.SaveResult();
	CDialogEx::OnOK();
}


void CInitDlg::OnBnClickedCancel()
{

	CDialogEx::OnCancel();
}


BOOL CInitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//load the user name.
	if(!m_UserConfig.LoadConfig())
		return FALSE;
	m_Username = m_UserConfig.GetCurUser();

	map<CString,CString> userlist;
	m_UserConfig.GetUserlist(userlist);
	for (map<CString,CString>::iterator it=userlist.begin();it!=userlist.end();it++)
	{
		m_ComboUser.AddString(it->first);
	}
	if(m_UserConfig.IsDefault())
	{
		m_Checkmsg.SetCheck(BST_CHECKED);
	}
	else
	{
		m_Checkmsg.SetCheck(BST_UNCHECKED);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CInitDlg::OnCbnSelchangeCombouserlist()
{
	int pos = m_ComboUser.GetCurSel();
	m_ComboUser.SetCurSel(pos);
	m_ComboUser.GetWindowText(m_Username);
	m_UserConfig.SetCurUser(m_Username);
	UpdateData(FALSE);
}


void CInitDlg::OnBnClickedCheckuser()
{
	m_UserConfig.SetDefault((bool)m_Checkmsg.GetCheck());
}

void CInitDlg::CopyFiles(CString spath[] ,int size , CString dpath )
{
	CString dir = _T("..\\Files\\");
	CFileFind finder;
	for (int i=0;i<size;i++)
	{
		if(!finder.FindFile(dir+spath[i]))
			continue;
		CopyFile(dir+spath[i],dpath+spath[i],TRUE);
	}
}

CString CInitDlg::GetFileName(CString path)
{
	if (path.GetLength() <= 0)
		return L"";
	if (path.ReverseFind('\\') == -1)
		return path;
	return path.Right(path.GetLength() - path.ReverseFind('\\')-1);
}