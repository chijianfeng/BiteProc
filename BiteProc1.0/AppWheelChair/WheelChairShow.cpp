#include"stdafx.h"
#include"AppWheelChair.h"
#include"ExtWndShadow.h"
#include"WheelChairShow.h"
#include"resource.h"

extern  CAppWheelChairApp theApp;


IMPLEMENT_DYNAMIC(CWheelChairShow, CWnd)

CWheelChairShow::CWheelChairShow()
{
	m_pAppWnd = NULL;
	m_pGyroDlg = NULL;
}

CWheelChairShow::~CWheelChairShow()
{

}

void CWheelChairShow::SendAppCmd(size_t cmd)
{

}

void CWheelChairShow::SetWndSize(size_t width, size_t height)
{
	m_Width = width;
	m_Height = height;
}

void CWheelChairShow::SetLogOut(void(*pFunc)(CString))
{
	m_pOutPut = pFunc;
}


BEGIN_MESSAGE_MAP(CWheelChairShow, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_HIDEHINT, OnHindHint)
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_SETTASK, OnShowSettingTask)
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_SHOWHINT, OnShowHint)
END_MESSAGE_MAP()




int CWheelChairShow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CWheelChairShow::OnDestroy()
{
	__super::OnDestroy();
	
}

void CWheelChairShow::OnPaint()
{
	CPaintDC dc(this);
	CRect rect,r,r1,r2;
	if (m_pAppWnd == NULL)
	{
		m_pAppWnd = new CWheelChair();

		if (!m_pAppWnd->Create(IDD_DIALOG1, this))
		{
			m_pOutPut(_T("Error creating IDD_DIALOG1"));
		}
		m_pAppWnd->ShowWindow(SW_SHOW);
		
	}
	
	if (m_pGyroDlg == NULL)
	{
		m_pGyroDlg = new CGyroDlg();
		if (!m_pGyroDlg->Create(IDD_GYRODLG, this))
		{
			m_pOutPut(_T("Error creating GyroDlg"));
		}
		m_pGyroDlg->ShowWindow(SW_NORMAL);
		m_pGyroDlg->GetWindowRect(r2);
		m_pGyroDlg->MoveWindow(r2.left+650,r2.top,r2.right+650,r2.bottom);
	}
}


void CWheelChairShow::OnTimer(UINT_PTR nIDEvent)
{
	__super::OnTimer(nIDEvent);
}

BOOL CWheelChairShow::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}


LRESULT CWheelChairShow::OnShowHint(WPARAM wparam, LPARAM lparam)
{
	Invalidate(TRUE);
	(*m_pOutPut)(L"��ʾ��ʾ��Ϣ");
	return 0;
}

LRESULT CWheelChairShow::OnHindHint(WPARAM wapram, LPARAM lparam)
{
	Invalidate(TRUE);
	(*m_pOutPut)(L"������ʾ��Ϣ");
	return 0;
}

LRESULT CWheelChairShow::OnShowSettingTask(WPARAM wparam, LPARAM lparam)
{
	AfxMessageBox(L"Add Code");
	return 0;
}

extern "C" _declspec(dllexport) char* GetAppModualMsg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return "ҧ����������";
}

extern "C" _declspec(dllexport) CWheelChairShow* CreateAppClass()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWheelChairShow* pClass = new CWheelChairShow();
	return pClass;
}

extern "C" _declspec(dllexport) BOOL CreateAppWindow(CWnd* parent, CWnd* pViewer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());			//for MFC dll stander
	if (pViewer == NULL)
		return FALSE;
	BOOL b = pViewer->Create(NULL,
		NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 1300, 1300), parent, 0xFFF2);					//ID ��
	pViewer->ShowWindow(SW_SHOW);
	return b;
}

//��̬��Ӳ˵�������Ӧ�õ���Ҫ������ӣ����Ǳ���Ĳ�
extern"C" _declspec(dllexport) BOOL	InstallExtMenu(CWnd* pChild, HWND Parent, UINT* NextMenuID)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	BOOL bReturn = FALSE;
	if (!pChild)					//dll �е�wndΪ�գ��޷����
		return FALSE;
	if (!Parent)
		return FALSE;
	HMENU hMenu = ::GetMenu(Parent);		//��ȡ�����ڵĲ˵�
	if (!hMenu)
		return FALSE;
	CMenu ParentMenu;
	ParentMenu.Attach(hMenu);
	CMenu Menu;
	if (!Menu.CreateMenu())						//�����˵�
		return FALSE;
	theApp.m_CommandToMessage.InitHashTable(3);		//Ԥ��3����޸�

	NextMenuID = new UINT[3];
	NextMenuID[0] = theApp.ID_MENU_SHOWHINT;
	NextMenuID[1] = theApp.ID_MENU_HIDEHINT;
	NextMenuID[2] = theApp.ID_MENU_SETTASK;

	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"��ʾ��ʾ��");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_SHOWHINT);	//����ӳ��
	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"������ʾ��");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_HIDEHINT);
	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"��������");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_SETTASK);

	HMENU hsubMenu = Menu.Detach();
	ParentMenu.InsertMenuW(ParentMenu.GetMenuItemCount(), MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)hsubMenu,
		_T("����Ӧ������"));

	ParentMenu.Detach();
	CWnd::FromHandle(Parent)->DrawMenuBar();		//ǿ���ػ�˵�
	return TRUE;
}

extern"C" _declspec(dllexport)BOOL CheckExtMenuItem(UINT id)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	if (id == theApp.ID_MENU_HIDEHINT ||
		id == theApp.ID_MENU_SETTASK ||
		id == theApp.ID_MENU_SHOWHINT)		//�ж��Ƿ��ǲ˵���Ϣ
		return TRUE;
	else
		return FALSE;
}