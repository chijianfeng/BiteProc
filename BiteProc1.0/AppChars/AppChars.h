// AppChars.h : AppChars DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAppCharsApp
// �йش���ʵ�ֵ���Ϣ������� AppChars.cpp
//
typedef CMap<UINT, UINT, UINT, UINT> CommandToMessage;			//������Ϣӳ��
class CAppCharsApp : public CWinApp
{
public:
	CAppCharsApp();
	CommandToMessage	m_CommandToMessage;
	static UINT		ID_MENU_SHOWHINT;							//��ʾ��ʾ�ַ�
	static UINT		ID_MENU_HIDEHINT;							//������ʾ�ַ�
	static UINT		ID_MENU_SETTASK;							//��������
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
