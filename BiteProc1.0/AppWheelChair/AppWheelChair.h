// AppWheelChair.h : AppWheelChair DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CAppWheelChairApp
// 有关此类实现的信息，请参阅 AppWheelChair.cpp
//
typedef CMap<UINT, UINT, UINT, UINT> CommandToMessage;			//构建消息映射
class CAppWheelChairApp : public CWinApp
{
public:
	CAppWheelChairApp();
	CommandToMessage	m_CommandToMessage;
	static UINT		ID_MENU_SHOWHINT;							//显示提示字符
	static UINT		ID_MENU_HIDEHINT;							//隐藏提示字符
	static UINT		ID_MENU_SETTASK;							//设置任务
// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
