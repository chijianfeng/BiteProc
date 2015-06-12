#pragma once

#ifndef _I_APP_MSG_
#define _I_APP_MSG_

#include "Structdefine.h"
#include <iostream>
#include <Windows.h>

using namespace std;

DECLEAR_INTERFACE(IAppMsg)
//add interface here

virtual void SendAppCmd(size_t cmd) = 0;									//给App应用发送命令

virtual void SetWndSize(size_t windth, size_t height) = 0;					//设定应用模块窗口大小

virtual void SetLogOut(void(*pFunc)(CString)) = 0;							//输出相关信息到指定位置

protected:
	void(*m_pOutPut)(CString) = 0;											//输出信息操作
	size_t				m_Width, m_Height;									//窗口的大小

END_DECLEAR_INTERFACE

#endif