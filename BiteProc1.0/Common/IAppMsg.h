#pragma once

#ifndef _I_APP_MSG_
#define _I_APP_MSG_

#include "Structdefine.h"
#include <iostream>
#include <Windows.h>

using namespace std;

DECLEAR_INTERFACE(IAppMsg)
//add interface here

virtual void SendAppCmd(size_t cmd) = 0;									//��AppӦ�÷�������

virtual void SetWndSize(size_t windth, size_t height) = 0;					//�趨Ӧ��ģ�鴰�ڴ�С

virtual void SetLogOut(void(*pFunc)(CString)) = 0;							//��������Ϣ��ָ��λ��

protected:
	void(*m_pOutPut)(CString) = 0;											//�����Ϣ����
	size_t				m_Width, m_Height;									//���ڵĴ�С

END_DECLEAR_INTERFACE

#endif