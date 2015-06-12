#pragma once

#ifndef _I_TRAIN_MSG_
#define _I_TRAIN_MSG_

#include "Structdefine.h"
#include <iostream>
#include "ManagedData.h"
#include "AlgParamConfig.h"
#include"BaseArth.h"
#include <Windows.h>

using namespace std;

DECLEAR_INTERFACE(ITrainMsg)

//add interface function here
virtual void SetParm( CAlgParamConfig& parm) = 0;				//�����㷨��������config.txt�ļ��в���
virtual	void SetData(ManagedData& d) = 0;
virtual bool SaveData() = 0;									//save the data to file.
virtual bool LoadData() = 0;									//load the data to file.
virtual bool DoProcess() = 0;									//���ݴ���..�����ó�ģ��
virtual bool StartThread(DWORD(WINAPI lp)(LPVOID) , HANDLE&) = 0;			//start the thread.
virtual bool EndThread(HANDLE&)  = 0;
virtual	bool SaveResult() = 0;									//����ѵ�����
virtual bool LoadResult() = 0;
virtual	bool ChangeState(CAlgParamConfig& parm, bool s) = 0;	//�޸��㷨����

virtual void SetLogOut(void (*pFunc)(CString)) = 0;				//��������״̬��Ϣ��¼��m_WndOutput,�ɲ�����

HANDLE		m_hTrainEvent;										//used for thread sync

CBaseArth*	m_pAlogrithm[3];									//ָ���㷨ģ��

BOOL		m_bExit;											//�߳��˳���־

void(*m_pOutPut)(CString) = 0;									//�������

END_DECLEAR_INTERFACE


#endif