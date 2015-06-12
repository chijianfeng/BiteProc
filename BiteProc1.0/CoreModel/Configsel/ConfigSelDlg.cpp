
// ConfigSelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConfigSelDlg.h"
#include "afxdialogex.h"
#include "Structdefine.h"
#include "Streamoper.h"
#include "Stringoper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CConfigSelDlg::CConfigSelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigSelDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CConfigSelDlg::CConfigSelDlg(CUserConfig& config, CWnd* pParent) : CDialogEx(CConfigSelDlg::IDD, pParent)
{
	config.GetUserPath(config.GetCurUser(), m_strUserPath);
	CString str = L"\\";
	str += _T(CONFIGFILE);
	m_strUserPath += str;							//�����ļ�
}

void CConfigSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CConfigSelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CConfigSelDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDOK, &CConfigSelDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigSelDlg ��Ϣ�������

BOOL CConfigSelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect tabRect;
	m_tab.InsertItem(0,_T("ҧ����ֵ����"));
	m_tab.InsertItem(1,_T("��,��,ͬʱҧ��������"));
	m_tab.InsertItem(2,_T("����ҧ��������"));
	m_tab.InsertItem(3,_T("��ҧ,��������ҧ��������"));
	m_tab.InsertItem(4,_T("����������������"));
	m_dialog1.SetConfigFilePath(m_strUserPath);
	m_dialog2.SetConfigFilePath(m_strUserPath);
	m_dialog3.SetConfigFilePath(m_strUserPath);
	m_dialog4.SetConfigFilePath(m_strUserPath);
	m_dialog5.SetConfigFilePath(m_strUserPath);

	m_dialog1.Create(IDD_BITETHRESHOLDDIALOG, &m_tab);
	m_dialog2.Create(IDD_LEFTRIGHTBOTHDIALOG, &m_tab);
	m_dialog3.Create(IDD_HLDIALOG, &m_tab);
	m_dialog4.Create(IDD_TLDIALOG, &m_tab);
	m_dialog5.Create(IDD_PUBLICDIALOG, &m_tab);
	m_tab.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	m_dialog1.SetWindowPos(NULL,tabRect.left,tabRect.top,
		tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
	m_dialog2.SetWindowPos(NULL, tabRect.left, tabRect.top,
		tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_dialog3.SetWindowPos(NULL, tabRect.left, tabRect.top,
		tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_dialog4.SetWindowPos(NULL, tabRect.left, tabRect.top,
		tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_dialog5.SetWindowPos(NULL, tabRect.left, tabRect.top,
		tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

	UPDATAVALTOCONTROL;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CConfigSelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CConfigSelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CConfigSelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CConfigSelDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CRect tabRect;

	m_tab.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 24;
	tabRect.bottom -= 1;

	switch (m_tab.GetCurSel())
	{
	case 0:
		m_dialog1.SetWindowPos(NULL,tabRect.left,tabRect.top,
			tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
		m_dialog2.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog3.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog4.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog5.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 1:
		m_dialog1.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog2.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_dialog3.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog4.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog5.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 2:
		m_dialog1.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog2.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog3.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_dialog4.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog5.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 3:
		m_dialog1.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog2.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog3.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog4.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_dialog5.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 4:
		m_dialog1.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog2.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog3.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog4.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_dialog5.SetWindowPos(NULL, tabRect.left, tabRect.top,
			tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}
}

void CConfigSelDlg::OnBnClickedOk()
{
	UPDATACONTROLTOVAL;
	m_dialog1.SaveFile();
	m_dialog2.SaveFile();
	m_dialog3.SaveFile();
	m_dialog4.SaveFile();
	m_dialog5.SaveFile();
	CDialogEx::OnOK();
}

