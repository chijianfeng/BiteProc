// ClassifyTrain.h : ClassifyTrain DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CClassifyTrainApp
// �йش���ʵ�ֵ���Ϣ������� ClassifyTrain.cpp
//
typedef CMap<UINT, UINT, UINT, UINT> CommandToMessage;			//������Ϣӳ��
class CClassifyTrainApp : public CWinApp
{
public:
	CClassifyTrainApp();
	CommandToMessage	m_CommandToMessage;
	
	static UINT		ID_MENU_PARAM;								//��������
	
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
