#pragma once

#include <iostream>
#include <cstring>
using namespace std;
template <typename T>
class CArthLoad
{
public:
	CArthLoad(void);
	virtual ~CArthLoad(void);
	BOOL LoadArth(T** pArth);
	BOOL Free();
	void SetArthPath(CString path);
	void SetParam(void* p);

	void SetParam2(void* p);				//reverse param.

private:
	CString		m_nPath;						//the path of the Arth.
	HINSTANCE	m_nDll;
	T*			m_pArth;
	char*		lparma;
	char*		lparam2;
};

