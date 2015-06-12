#include "stdafx.h"
#include "EEGViewerLoader.h"
#include "Stringoper.h"


CEEGViewerLoader::CEEGViewerLoader()
{
}


CEEGViewerLoader::~CEEGViewerLoader()
{
}

CEEGViewerLoader* CEEGViewerLoader::GetInstance()
{
	static CEEGViewerLoader loader;
	return &loader;
}

void CEEGViewerLoader::SetViewerPath(CString path)
{
	m_strModelPath = path;
}

BOOL CEEGViewerLoader::LoadEEGViewer(CBaseEEGViewerWnd* pBase, CWnd* pParent)
{
	m_pParent = pParent;
	return LoadEEGViewer(pBase);
}

CBaseEEGViewerWnd* CEEGViewerLoader::CreateViewer()
{
	typedef CBaseEEGViewerWnd* (pFunT)();
	pFunT* pFunc;

	if (m_strModelPath.GetLength() <= 0)
		return FALSE;
	m_hDll = LoadLibrary(m_strModelPath);
	if (!m_hDll)												//load failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the dll , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}
	pFunc = (pFunT*)GetProcAddress(m_hDll, "CreateViewer");
	if (!pFunc)													//load object failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the train wnd func , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}

	return pFunc();
}

BOOL CEEGViewerLoader::LoadEEGViewer(CBaseEEGViewerWnd* pBase)
{
	typedef BOOL (pFunT)(CWnd* pParent , CWnd* pView);				//指针函数类型定义
	pFunT* pFunc;

	if (m_strModelPath.GetLength() <= 0)
		return FALSE;
	m_hDll = LoadLibrary(m_strModelPath);
	if (!m_hDll)												//load failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the dll , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}
	pFunc = (pFunT*)GetProcAddress(m_hDll, "CreateEEGViewerWnd");
	if (!pFunc)													//load object failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the train wnd func , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}

	BOOL B =  pFunc(m_pParent,pBase);
	return B;
}

BOOL CEEGViewerLoader::CheckSource(CString& msg)
{
	if (m_strModelPath.GetLength() <= 0)
		return FALSE;
	m_hDll = LoadLibrary(m_strModelPath);

	msg = L"";

	if (!m_hDll)												//load failed
	{
		DWORD erro = GetLastError();
		CString str;
		str.Format(L"Can't Load the dll , erro: %d", erro);
		//AfxMessageBox(str);
		Free();
		return FALSE;
	}
	typedef char*	funcT();					//指针函数类型定义，返回值为char*
	funcT* pfunc;
	pfunc = (funcT*)GetProcAddress(m_hDll, "GetEEGViewerModualMsg");

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

BOOL CEEGViewerLoader::Free()
{
	return FreeLibrary(m_hDll);
}

void CEEGViewerLoader::SetParent(CWnd* p /* = 0 */)
{
	m_pParent = p;
}



