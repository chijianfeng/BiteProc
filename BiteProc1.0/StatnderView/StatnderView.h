// StatnderView.h : StatnderView DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStatnderViewApp
// �йش���ʵ�ֵ���Ϣ������� StatnderView.cpp
//

class CStatnderViewApp : public CWinApp
{
public:
	CStatnderViewApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
