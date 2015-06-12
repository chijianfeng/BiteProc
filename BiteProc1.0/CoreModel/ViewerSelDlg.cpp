// ViewerSelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CoreModel.h"
#include "ViewerSelDlg.h"
#include "afxdialogex.h"
#include "FileOper.h"
#include "Structdefine.h"
#include "./Loader/EEGViewerLoader.h"
#include "Stringoper.h"

// CViewerSelDlg 对话框

IMPLEMENT_DYNAMIC(CViewerSelDlg, CDialogEx)

CViewerSelDlg::CViewerSelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewerSelDlg::IDD, pParent)
	, m_Length(0)
{
}

CViewerSelDlg::CViewerSelDlg(CString userpath, CWnd* pParent /* = NULL */)
: CDialogEx(CViewerSelDlg::IDD, pParent)
, m_Length(0)
{
	CString str = L"\\";
	str += _T(VIEWFILE);
	m_strUserPath = userpath + str;							//配置文件
}

CViewerSelDlg::~CViewerSelDlg()
{
}

void CViewerSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMVIEWMODEL, m_comViewers);
	DDX_Text(pDX, IDC_EVIEWMESSAGE, m_strMessage);
	DDX_Text(pDX, IDC_EVIEWSEARCHDIR, m_strSearchDir);
	DDX_Text(pDX, IDC_EDITVIEWLENGTH, m_Length);
	DDX_Text(pDX, IDC_EDITVIEWWIDTH, m_Width);
	DDX_Text(pDX, IDC_EVIEWMAX, m_nMaxv);
	DDX_Text(pDX, IDC_EVIEWMIN, m_nMinv);
	DDX_Check(pDX, IDC_CHKVIEWAUTO, m_bAuto);
	
}


CString CViewerSelDlg::GetSelDllPath()
{
	return m_strDllPath;
}

void CViewerSelDlg::SetSize(const int len, const int width)
{
	m_Width = width;
	m_Length = len;
}

void CViewerSelDlg::GetSize(int& len, int& width)
{
	len = m_Length;
	width = m_Width;
}

void CViewerSelDlg::GetMaxMin(int& Maxv, int& Minv)
{
	Maxv = m_nMaxv;
	Minv = m_nMinv;
}

BOOL CViewerSelDlg::IsAutoMaxMin()
{
	return m_bAuto;
}

BEGIN_MESSAGE_MAP(CViewerSelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BVIEWSEARCHDIR, &CViewerSelDlg::OnBnClickedBviewsearchdir)
	ON_COMMAND(VIEWSELDLG_CHECK, &CViewerSelDlg::OnViewseldlgCheck)
	ON_CBN_SELCHANGE(IDC_COMVIEWMODEL, &CViewerSelDlg::OnCbnSelchangeComviewmodel)
	ON_BN_CLICKED(IDOK, &CViewerSelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHKVIEWAUTO, &CViewerSelDlg::OnBnClickedChkviewauto)
END_MESSAGE_MAP()


// CViewerSelDlg 消息处理程序


void CViewerSelDlg::OnBnClickedBviewsearchdir()
{
	// TODO : 模型搜索路径
	CFileOper oper;
	m_strSearchDir = oper.DirectorySelect(this->GetSafeHwnd());
	CViewerConfig m_config(m_EEGViewerPath);
	if (m_strSearchDir != "")
	{
		m_config.LoadFileMsg();
		m_config.SetViewerPath(m_strSearchDir);
		UPDATAVALTOCONTROL;
	}
	
	PostMessage(WM_COMMAND, VIEWSELDLG_CHECK);
}


BOOL CViewerSelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString dir;
	m_EEGViewerPath = m_strUserPath;
	CViewerConfig m_config(m_EEGViewerPath);
	m_config.LoadFileMsg();
	m_config.CloseFile();
	/*从EEGViewer.txt中读取各参数默认值*/
	m_strSearchDir = m_config.GetViewerPath();
	m_strDllPath = m_config.GetSelDllPath();
	m_Length = m_config.GetWindowLength();
	m_Width = m_config.GetWindowWidth();
	m_nMaxv = m_config.GetMaxValue();
	m_nMinv = m_config.GetMinValue();
	m_bAuto = m_config.GetIsAutoMaxMin();
	if (m_bAuto)
	{
		GetDlgItem(IDC_EVIEWMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EVIEWMAX)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EVIEWMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EVIEWMAX)->EnableWindow(TRUE);
	}
	UPDATAVALTOCONTROL;
	PostMessage(WM_COMMAND , VIEWSELDLG_CHECK);
	return TRUE;
}



void CViewerSelDlg::CheckModels()
{
	CEEGViewerLoader *ploader = CEEGViewerLoader::GetInstance();
	CFileOper oper;
	vector<CString> tmplist;
	oper.FindFiles(tmplist, m_strSearchDir, L"*.dll");		//找到所有dll

	m_comViewers.Clear();
	for each (auto var in tmplist)
	{
		ploader->SetViewerPath(var);
		CString msg;
		if (ploader->CheckSource(msg))
		{
			m_listModels.push_back(var);
			CString name = var.Right(var.GetLength() - var.ReverseFind('\\') - 1);
			m_comViewers.AddString(name);
		}
	}
	if (m_comViewers.GetCount() > 0)
	{
		m_comViewers.SetCurSel(0);
		OnCbnSelchangeComviewmodel();
	}
}

void CViewerSelDlg::OnCbnSelchangeComviewmodel()
{
	CString name;
	m_comViewers.GetLBText(m_comViewers.GetCurSel(), name);

	for each (auto path in m_listModels)
	{
		CString obj = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
		if (Equalwstr(obj, name))
		{
			m_strDllPath = path;
			break;
		}
	}
	CEEGViewerLoader *ploader = CEEGViewerLoader::GetInstance();
	ploader->SetViewerPath(m_strDllPath);
	ploader->CheckSource(m_strMessage);
	UPDATAVALTOCONTROL;
}

void CViewerSelDlg::OnViewseldlgCheck()
{
	CheckModels();
}

void CViewerSelDlg::OnBnClickedOk()
{
	UPDATACONTROLTOVAL;
	Stringoper oper;
	CString length;
	CString width;
	CString maxv;
	CString minv;
	CString path;
	GetDlgItemText(IDC_EDITVIEWLENGTH, length);
	GetDlgItemText(IDC_EDITVIEWWIDTH, width);
	GetDlgItemText(IDC_EVIEWMAX, maxv);
	GetDlgItemText(IDC_EVIEWMIN, minv);
	GetDlgItemText(IDC_CHKVIEWAUTO,oper.tocstring(oper.inttostring(m_bAuto)));
	
	m_nMaxv = oper.stringtoint(oper.tostring(maxv));
	m_nMinv = oper.stringtoint(oper.tostring(minv));
	m_Length = oper.stringtoint(oper.tostring(length));
	m_Width = oper.stringtoint(oper.tostring(width));
	CViewerConfig m_config(m_EEGViewerPath);
	m_config.LoadFileMsg();
	m_config.SetSelDllPath(m_strDllPath);
	m_config.SetViewerPath(m_strSearchDir);
	m_config.SetMaxValue(m_nMaxv);
	m_config.SetMinValue(m_nMinv);
	m_config.SetWindowLength(m_Length);
	m_config.SetWindowWidth(m_Width);
	m_config.SetIsAutoMaxMin(m_bAuto);
	m_config.SaveFileMsg();
	m_config.CloseFile();
	CDialogEx::OnOK();
}


void CViewerSelDlg::OnBnClickedChkviewauto()
{
	m_bAuto = !m_bAuto;
	if (m_bAuto)
	{
		GetDlgItem(IDC_EVIEWMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EVIEWMAX)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EVIEWMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EVIEWMAX)->EnableWindow(TRUE);
	}
}




