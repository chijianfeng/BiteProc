#include "StdAfx.h"
#include "DeviceLoader.h"
const int parmlength  = 256;
template<typename T>
CDeviceLoader<T>::CDeviceLoader(void)
{
	m_device = NULL;
	lparma = new char[parmlength];
}

template<typename T>
CDeviceLoader<T>::~CDeviceLoader(void)
{
	delete lparma;
}

template<typename T>
BOOL CDeviceLoader<T>::LoadDevice(T** device)
{
	typedef T*	lpT(void*);
	lpT* lpt;
	if(m_path.GetLength()<=0)
		return FALSE;
	m_Dll = LoadLibrary(m_path);
	if(m_Dll!=NULL)
	{
		lpt = (lpT*)GetProcAddress(m_Dll,"CreateObject");
		if(!lpt)
		{
			DWORD err = GetLastError();
			ASSERT(err);
			FreeLibrary(m_Dll);
			return FALSE;
		}
		else
			*device = lpt(lparma);
	}
	else
	{
		DWORD myerrno = GetLastError();
		CString str;
		str.Format(_T("Can't load the dll,error number:%d"),myerrno);
		//AfxMessageBox(str);
		FreeLibrary(m_Dll);
		return FALSE;
	}
	return TRUE;
}

template<typename T>
BOOL CDeviceLoader<T>::Free()
{
	return FreeLibrary(m_Dll);
}

template<typename T>
void CDeviceLoader<T>::SetDevicePath(CString path)
{
	this->m_path = path;
}
template<typename T>
void CDeviceLoader<T>::Setparma(void *p)
{
	if(p!=NULL)
		memcpy(lparma,p,parmlength);
}