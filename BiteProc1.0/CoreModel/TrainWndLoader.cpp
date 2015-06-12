#include "stdafx.h"
#include "TrainWndLoader.h"
#include "Stringoper.h"


CTrainWndLoader::CTrainWndLoader()
{
}


CTrainWndLoader::~CTrainWndLoader()
{
}

CTrainWndLoader* CTrainWndLoader::GetInstance()
{
	static CTrainWndLoader loader;
	return &loader;											//返回窗口加载对象
}

void CTrainWndLoader::SetWndPath(CString path)
{
	m_ModelPath = path;
}

BOOL CTrainWndLoader::LoadTrainWnd(CBaseTrainWnd** pBase, CWnd* pParent)
{
	m_pParent = pParent;
	return LoadTrainWnd(pBase);
}

BOOL CTrainWndLoader::LoadTrainWnd(CBaseTrainWnd** pBase)
{
	typedef CBaseTrainWnd* (pFunT)(CWnd* pParent);				//指针函数类型定义
	pFunT* pFunc;

	if (m_ModelPath.GetLength() <= 0)
		return FALSE;
	m_hDll = LoadLibrary(m_ModelPath);

	if (!m_hDll)												//load failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the dll , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}
	pFunc = (pFunT*)GetProcAddress(m_hDll, "CreateTrainWnd");
	if (!pFunc)													//load object failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the train wnd func , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}

	*pBase = pFunc(m_pParent);
	
	//添加菜单，如果模块包含菜单项的话
	typedef BOOL 	MenuFunT(CWnd* pChild, HWND Parent, UINT* NextMenuID);
	MenuFunT* Menufunc;
	Menufunc = (MenuFunT*)GetProcAddress(m_hDll, "InstallExtMenu");
	if (!Menufunc)								//没有菜单资源
	if (!*pBase)
		return FALSE;
	else
		return TRUE;

	return Menufunc(*pBase, m_MainFrameWnd, m_pItemID);		//加载菜单
}

BOOL CTrainWndLoader::CreateTrainWnd(CBaseTrainWnd*pBase, CWnd* pParent)
{
	if (pBase == NULL)
		return FALSE;
	if (m_hDll == NULL || m_hDll == INVALID_HANDLE_VALUE)
		return FALSE;
	typedef BOOL (pFunT)(CDialogEx* pdlg , CWnd* pParent);				//指针函数类型定义
	pFunT* pFunc;

	pFunc = (pFunT*)GetProcAddress(m_hDll, "CreateTrainDlg");
	if (!pFunc)													//load object failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the train wnd func , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}
	BOOL res =  pFunc(pBase, pParent);				//创建训练窗体

	//添加菜单，如果模块包含菜单项的话
	typedef BOOL 	MenuFunT(CWnd* pChild, HWND Parent, UINT* NextMenuID);
	MenuFunT* Menufunc;
	Menufunc = (MenuFunT*)GetProcAddress(m_hDll, "InstallExtMenu");
	if (!Menufunc)
		return res;

	return Menufunc(pBase, m_MainFrameWnd, m_pItemID);		//加载菜单
}

BOOL CTrainWndLoader::CreateTrainClass(CBaseTrainWnd** pBase)
{
	typedef CBaseTrainWnd* (pFunT)();					//指针函数类型定义
	pFunT* pFunc;
	if (m_ModelPath.GetLength() <= 0)
		return FALSE;
	m_hDll = LoadLibrary(m_ModelPath);

	if (!m_hDll)												//load failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the dll , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}
	pFunc = (pFunT*)GetProcAddress(m_hDll, "CreateTrainClass");
	if (!pFunc)													//load object failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the train wnd func , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}
	*pBase = pFunc();
	if (!*pBase)
		return FALSE;
	return TRUE;
}

BOOL CTrainWndLoader::CheckSource(CString& msg)
{
	if (m_ModelPath.GetLength() <= 0)
		return FALSE;
	m_hDll = LoadLibrary(m_ModelPath);

	msg = L"";

	if (!m_hDll)												//load failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the dll , erro: %d", erro);
		AfxMessageBox(str);
		Free();
		return FALSE;
	}
	typedef char*	funcT();					//指针函数类型定义，返回值为char*
	funcT* pfunc;
	pfunc = (funcT*)GetProcAddress(m_hDll, "GetTrainModualMsg");

	if (!pfunc)
	{
		DWORD err = GetLastError();
		TRACE(L"can't load the func:%ld\n\n", err);
		Free();
		return FALSE;
	}
	string tmp = pfunc();
	Stringoper oper;
	msg = oper.tocstring(tmp);
	Free();
	return TRUE;
}

BOOL CTrainWndLoader::CheckMenuID(UINT id)
{
	if (!m_hDll)
		return FALSE;
	typedef BOOL 	funcT(UINT ID);
	funcT*  GetMenuItem = (funcT*)GetProcAddress(m_hDll, "CheckExtMenuItem");
	if (!GetMenuItem)
		return FALSE;

	BOOL bSend = GetMenuItem(id);		//检测是否是菜单命令

	return bSend;
}

BOOL CTrainWndLoader::Free()
{
	return FreeLibrary(m_hDll);
}

void CTrainWndLoader::SetParent(CWnd* p /* = 0 */)
{
	m_pParent = p;
}

void CTrainWndLoader::SetMainWnd(HWND h)
{
	m_MainFrameWnd = h;
}

