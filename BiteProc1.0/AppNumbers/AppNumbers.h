/*
���ߣ����ĺ�
ʱ�䣺2015/2/3
*/

// AppNumbers.h : AppNumbers DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAppNumbersApp
// �йش���ʵ�ֵ���Ϣ������� AppNumbers.cpp
//
typedef CMap<UINT, UINT, UINT, UINT> CommandToMessage;			//������Ϣӳ��
class CAppNumbersApp : public CWinApp
{
public:
	CAppNumbersApp();
	CommandToMessage	m_CommandToMessage;
	static UINT		ID_MENU_SHOWHINT;							//��ʾ��ʾ�ַ�
	static UINT		ID_MENU_HIDEHINT;							//������ʾ�ַ�
	static UINT		ID_MENU_SETTASK;							//��������
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
