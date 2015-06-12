#include "stdafx.h"
#include "AppLoader.h"
#include"Stringoper.h"

CAppLoader::CAppLoader()
{
	m_pItemID = NULL;
}


CAppLoader::~CAppLoader()
{
}

CAppLoader* CAppLoader::GetInstance()
{
	static CAppLoader loader;
	return &loader;
}

void CAppLoader::SetWndPath(CString path)
{
	m_ModelPath = path;
}

void CAppLoader::SetMainWnd(HWND h)
{
	m_MainFrameWnd = h;
}

void CAppLoader::SetParent(CWnd* p /* = 0 */)
{
	m_pParent = p;
}

BOOL CAppLoader::CheckSource(CString& msg)
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
	pfunc = (funcT*)GetProcAddress(m_hDll, "GetAppModualMsg");		//查找相应模块

	if (!pfunc)
	{
		DWORD err = GetLastError();
		TRACE(L"can't load the func:%ld\n\n", err);
		Free();
		return FALSE;
	}
	string tmp = pfunc();						//返回信息
	Stringoper oper;
	msg = oper.tocstring(tmp);
	Free();
	return TRUE;
}

BOOL CAppLoader::Free()
{
	return FreeLibrary(m_hDll);
}

BOOL CAppLoader::CreateAppClass(CBaseAppWnd** pWnd)
{
	typedef CBaseAppWnd* (pFunT)();					//指针函数类型定义
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
	pFunc = (pFunT*)GetProcAddress(m_hDll, "CreateAppClass");
	if (!pFunc)													//load object failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the train wnd func , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}
	*pWnd = pFunc();					//创建对象
	if (!*pWnd)
		return FALSE;
	return TRUE;
}

BOOL CAppLoader::CreateAppWnd(CBaseAppWnd *pBase, CWnd* pParent)
{
	if (pBase == NULL)
		return FALSE;
	m_pParent = pParent;
	return CreateAppWnd(pBase);
}

BOOL CAppLoader::CreateAppWnd(CBaseAppWnd* pBase)
{
	if (!pBase)
		return FALSE;

	if (m_hDll == NULL || m_hDll == INVALID_HANDLE_VALUE)
		return FALSE;
	typedef BOOL(pFunT)(CWnd* pdlg, CWnd* pParent);				//指针函数类型定义
	pFunT* pFunc;

	pFunc = (pFunT*)GetProcAddress(m_hDll, "CreateAppWindow");
	if (!pFunc)													//load object failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the train wnd func , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}
	if (!pFunc(m_pParent, pBase))								//创建窗体
	{
		Free();
		return FALSE;
	}
	//添加菜单，如果模块包含菜单项的话
	typedef BOOL 	MenuFunT(CWnd* pChild, HWND Parent, UINT* NextMenuID);
	MenuFunT* Menufunc;
	Menufunc = (MenuFunT*)GetProcAddress(m_hDll, "InstallExtMenu");
	if (!Menufunc)
	{
		return TRUE;
	}
	return Menufunc(pBase, m_MainFrameWnd, m_pItemID);		//加载菜单
}

BOOL CAppLoader::CheckMenuID(CBaseAppWnd* pWnd, UINT id)
{
	if (!m_hDll||!pWnd)
		return FALSE;
	typedef BOOL 	funcT(UINT ID);
	funcT*  GetMenuItem = (funcT*)GetProcAddress(m_hDll, "CheckExtMenuItem");

	BOOL bSend = GetMenuItem(id);		//检测是否是菜单命令
	if (bSend)
	{
		pWnd->SendMessage(id, 0, 0);	//发送消息
	}
	return TRUE;
}

BOOL CAppLoader::CheckMenuID(UINT id)
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



