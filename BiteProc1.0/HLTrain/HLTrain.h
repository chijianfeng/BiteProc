// HLTrain.h : HLTrain DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHLTrainApp
// �йش���ʵ�ֵ���Ϣ������� HLTrain.cpp
//
typedef CMap<UINT, UINT, UINT, UINT> CommandToMessage;			//������Ϣӳ��
class CHLTrainApp : public CWinApp
{
public:
	CHLTrainApp();

// ��д
public:
	virtual BOOL		InitInstance();
	static  UINT		ID_MENU_PARAM;								//��������
	CommandToMessage	m_CommandToMessage;
	DECLARE_MESSAGE_MAP()
};
