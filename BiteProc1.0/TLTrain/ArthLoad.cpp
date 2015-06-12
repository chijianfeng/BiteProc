#include "StdAfx.h"
#include "ArthLoad.h"

const int parmlength  = 256;

template<typename T>
CArthLoad<T>::CArthLoad(void)
{
	m_pArth = NULL;
	lparma = new char[parmlength];
	lparam2 = new char[parmlength];
}

template<typename T>
CArthLoad<T>::~CArthLoad(void)
{
	delete lparma;
	if (!lparam2)
		delete[]lparam2;
}

template<typename T>
BOOL CArthLoad<T>::LoadArth(T** pArth)
{
	typedef T*	lpT(void*,void*);
	lpT* lpt;
	if(m_nPath.GetLength()<=0)
		return FALSE;
	m_nDll = LoadLibrary(m_nPath);
	if(m_nDll!=NULL)
	{
		lpt = (lpT*)GetProcAddress(m_nDll,"CreateArth");
		if(!lpt)
		{
			DWORD err = GetLastError();
			ASSERT(err);
			FreeLibrary(m_nDll);
			return FALSE;
		}
		else
		{
			*pArth = lpt(lparma , lparam2);
		}
	}
	else
	{
		DWORD myerrno = GetLastError();
		CString str;
		str.Format(_T("Can't load the dll,error number:%d"),myerrno);
		AfxMessageBox(str);
		FreeLibrary(m_nDll);
		return FALSE;
	}
	return TRUE;
}

template<typename T>
BOOL CArthLoad<T>::Free()
{
	m_pArth = NULL;
	return FreeLibrary(m_nDll);
}

template<typename T>
void CArthLoad<T>::SetArthPath(CString path)
{
	this->m_nPath = path;
}
template<typename T>
void CArthLoad<T>::SetParam(void *p)
{
	if(p!=NULL)
		memcpy(lparma,p,parmlength);
}

template<typename T>
void CArthLoad<T>::SetParam2(void* p)
{
	if (p != NULL)
		memcpy(lparam2, p, parmlength);
}