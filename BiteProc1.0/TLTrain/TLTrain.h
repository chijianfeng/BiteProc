// TLTrain.h : TLTrain DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTLTrainApp
// �йش���ʵ�ֵ���Ϣ������� TLTrain.cpp
//

class CTLTrainApp : public CWinApp
{
public:
	CTLTrainApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
