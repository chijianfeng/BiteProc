// ClassifyTrain.h : ClassifyTrain DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CClassifyTrainApp
// 有关此类实现的信息，请参阅 ClassifyTrain.cpp
//
typedef CMap<UINT, UINT, UINT, UINT> CommandToMessage;			//构建消息映射
class CClassifyTrainApp : public CWinApp
{
public:
	CClassifyTrainApp();
	CommandToMessage	m_CommandToMessage;
	
	static UINT		ID_MENU_PARAM;								//参数设置
	
// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
