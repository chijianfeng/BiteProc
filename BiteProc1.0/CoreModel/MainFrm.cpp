
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "CoreModel.h"
#include "EEGView.h"
#include "MainFrm.h"
#include "CoreModelDoc.h"
#include "DeviceSelDlg.h"
#include "CoreModelView.h"
#include "./Loader/AppLoader.h"
#include "TrainWndLoader.h"
#include "./Configsel/ConfigSelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_DEVICESETTING_SELDEVICE, &CMainFrame::OnDevicesettingSeldevice)
	ON_MESSAGE(MYWM_MAINFRAMINNER , OnInnerMessage)
	ON_COMMAND(ID_SETTING_ALOGRITHM, &CMainFrame::OnSettingAlogrithm)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT BASED_CODE style[] =
{
	ID_START,
	ID_ZOOMOUT,
	ID_ZOOMIN,
	ID_PAUSE,
	ID_STOP,
	ID_COLORPANE,
	ID_DRAWFLAGE			//for saving the eeg data
	//add other rs here..
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	/*if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);*/

	if (!m_OperToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_WRAPABLE | TBSTYLE_TOOLTIPS,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_OperToolBar.LoadBitmap(IDB_BOPER) || !m_OperToolBar.SetButtons(style, sizeof(style) / sizeof(UINT)))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
	m_OperToolBar.SetButtons(style, 7);
	m_OperToolBar.SetButtonInfo(0, ID_START, TBBS_BUTTON, 0);
	m_OperToolBar.SetButtonInfo(1, ID_ZOOMOUT, TBBS_BUTTON, 1);
	m_OperToolBar.SetButtonInfo(2, ID_ZOOMIN, TBBS_BUTTON, 2);
	m_OperToolBar.SetButtonInfo(3, ID_PAUSE, TBBS_BUTTON, 3);
	m_OperToolBar.SetButtonInfo(4, ID_STOP, TBBS_BUTTON, 4);
	m_OperToolBar.SetButtonInfo(5, ID_COLORPANE, TBBS_BUTTON, 5);
	m_OperToolBar.SetButtonInfo(6, ID_DRAWFLAGE, TBBS_BUTTON, 6);


	// ��ֹ�˵����ڼ���ʱ��ý���
	//CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}



	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);
	

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// �����û�����Ĺ���������: 
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO:  �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_OperToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_OperToolBar);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// �����û�����Ĺ�����ͼ��
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO:  �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);
	CString dir;
	m_UserCfg.GetUserPath(m_UserCfg.GetCurUser(), dir);
	((CCoreModelDoc*)GetActiveDocument())->SetCurUser(dir);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	m_UserCfg.LoadConfig();
	m_strTitle = m_UserCfg.GetCurUser() + L"�û�";
	cs.style &= ~FWS_ADDTOTITLE;

	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// �����������
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// Ϊ�����û������������Զ��尴ť
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}
	//add init here

	LoadUserConfig();
	LoadDevice();
	//add load app& viewer here.

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	//���ִ���
	if (!m_wndSplitter.CreateStatic(this, 2, 1))				//����Ϊ2*1
	{
		m_wndOutput<<("FAILED to Create static spitter")<<m_EndOut;
		return FALSE;
	}
	CRect rct;
	this->GetClientRect(rct);
	ScreenToClient(rct);
	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CCoreModelView), CSize(0, 550), pContext))		//��ʾӦ����ͼ
	{
		m_wndOutput<<("Failed to create first pane\n")<<m_EndOut;
		return FALSE;
	}
	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CEEGView), CSize(0, 0), pContext))				//��ʾ�ź���ͼ	
	{
		m_wndOutput<<("Failed to create second pane\n")<<m_EndOut;
		return FALSE;
	}
	SetActiveView((CView*)m_wndSplitter.GetPane(0, 0));
	CRect rect1, rect2;
	(m_wndSplitter.GetPane(0, 0))->GetClientRect(rect1);
	(m_wndSplitter.GetPane(1, 0))->GetClientRect(rect2);
	m_wndSplitter.SetHeight(rect1.Height() + rect2.Height());
	return CFrameWndEx::OnCreateClient(lpcs, pContext);
	//return TRUE;
}

void CMainFrame::OnDevicesettingSeldevice()
{
	// TODO:  ʵ�ֲɼ�ģ����������Լ�ģ��ѡ���л�
	CDeviceSelDlg dlg(m_UserCfg,this);

	if (dlg.DoModal() == IDOK)
	{
		//change the device
		CCoreModelDoc *pDoc = (CCoreModelDoc*)GetActiveDocument();
		if (pDoc == NULL)
			return ;
		m_wndOutput << "�޸��豸" << m_EndOut;
		pDoc->DeleteDevice();
		LoadDevice();
	}
	else
		m_wndOutput << "δ�޸��豸" << m_EndOut;
}

void CMainFrame::OnSettingAlogrithm()
{
	// TODO:  �㷨�Լ�ѵ��ģ���������
	CConfigSelDlg dlg(m_UserCfg, this);
	//�������õ�ǰѵ��ģ�����
	if (dlg.DoModal() == IDOK)
	{
		CCoreModelDoc *pDoc = (CCoreModelDoc*)GetActiveDocument();
		if (pDoc == NULL)
			return;
		//��ȡ��ǰģ�����Ӧ��
		CBaseTrainWnd* pTrainWnd = pDoc->GetBaseTrainWnd();				//��ȡ��ǰ��ѵ��ģ��
		if (pTrainWnd)
		{
			CString path;
			m_UserCfg.GetUserPath(m_UserCfg.GetCurUser(), path);
			CString algpath = path + _T("\\" + CONFIGFILE);
			CAlgParamConfig config(algpath);
			config.LoadParam();												//���ز������ڴ�
			config.CloseParmFile();
			pTrainWnd->SetParm(config);										//���ò���
			pTrainWnd->SendMessage(WM_PAINT);
		}
		else
			m_wndOutput << "ģ�鲻���ڣ��޷����ò���" << m_EndOut;
	}
	else
		m_wndOutput << "δ����ѵ��ģ��" << m_EndOut;
}

LRESULT CMainFrame::OnInnerMessage(WPARAM wparam, LPARAM lparam)
{
	CCoreModelDoc* pDoc = NULL;
	switch (wparam)
	{
		/*��������*/
	case INNERMAINFRAME_UPDATA:
		pDoc = (CCoreModelDoc*)GetActiveDocument();
		if (pDoc != NULL)
		{
			//�ȼ���ҧ����ɫֵ
			pDoc->CalculateColor();
			//������ʾ����
			m_Rng.m_bUpDateEEG = TRUE;
			if (pDoc->IsTrain())
				m_Rng.m_bUpdataFrame = TRUE;
			else
				m_Rng.m_bUpdataFrame = FALSE;
			pDoc->UpdateAllViews(NULL, 0L, &m_Rng);				//����
		}
		break;
	case  INNERMAINFRAME_CMDCODE:
		m_wndOutput << "ʶ������Ϊ��" << lparam << m_EndOut;
		break;
	default:
		break;
	}
	return 0;
}

//private operation

bool CMainFrame::LoadUserConfig()
{
	if (!m_UserCfg.LoadConfig())
		return false;
	CString dir;
	m_UserCfg.GetUserPath(m_UserCfg.GetCurUser(), dir);
	CString path = dir + _T("\\"+CONFIGFILE);
	CAlgParamConfig config;
	if (!config.OpenParamFile(path))
	{
		m_wndOutput<<("OPen the Arth Config file failed!")<<m_EndOut;
		return false;
	}
	bool b =  config.LoadParam();						//��ȡ����
	config.CloseParmFile();
	return b;
}

bool CMainFrame::LoadDevice()
{
	m_UserCfg.LoadConfig();
	CString dir;
	m_UserCfg.GetUserPath(m_UserCfg.GetCurUser(), dir);
	CString path = dir + _T("\\"+DEVICEFILE);				//�豸���������ļ�
	if (!m_DeviceCfg.OpenFile(path))
	{
		m_wndOutput << "Open Device config file Failed!" << m_EndOut;
		return false;
	}
	m_DeviceCfg.LoadFileMsg();

	//set device parmeter.
	CCoreModelDoc *pDoc = (CCoreModelDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return false;
	CString name;
	name = m_DeviceCfg.GetDevicePath().Right(m_DeviceCfg.GetDevicePath().GetLength() - 
		1 - m_DeviceCfg.GetDevicePath().ReverseFind('\\'));
	if (!pDoc->RegDevice(m_DeviceCfg))
	{
		m_wndOutput << "Load "<< name<<" Device Failed!" << m_EndOut;		//�����豸ʧ��
		m_DeviceCfg.CloseFile();
		return false;
	}
	m_wndOutput << "Load " << name << " Device Success!" << m_EndOut;
	m_DeviceCfg.CloseFile();

	return true;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO����Ӧ�Զ�����Ϣ
	CAppLoader * ploader = CAppLoader::GetInstance();
	CBaseAppWnd* pWnd(0);
	CCoreModelDoc* pDoc = (CCoreModelDoc*)GetActiveDocument();

	CTrainWndLoader* pTrainLoader = CTrainWndLoader::GetInstance();
	CBaseTrainWnd* pTrainWnd(0);
	pTrainWnd = pDoc->GetBaseTrainWnd();

	pWnd = pDoc->GetCurApp();

	if (ploader->CheckMenuID((UINT)wParam))
	{
		return (BOOL)pWnd->SendMessage(wParam, 0, 0);				//������Ϣ
	}

	if (pTrainLoader->CheckMenuID((UINT)wParam)	)					//ѵ��ģ����Ϣ
	{
		if (pTrainWnd)
			return pTrainWnd->SendMessage(wParam, 0, 0);	
	}

	return CFrameWndEx::OnCommand(wParam, lParam);
}


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode == CN_COMMAND)////����ǲ˵�����
	{ // if nID translates to our internal message then enable the menu item
		// otherwise, let OnCmdMsg() handle nID.
		CAppLoader * ploader = CAppLoader::GetInstance();
		if (ploader->CheckMenuID(nID))
			return TRUE;
	}
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

