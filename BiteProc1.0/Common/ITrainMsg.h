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
virtual void SetParm( CAlgParamConfig& parm) = 0;				//设置算法参数，即config.txt文件中参数
virtual	void SetData(ManagedData& d) = 0;
virtual bool SaveData() = 0;									//save the data to file.
virtual bool LoadData() = 0;									//load the data to file.
virtual bool DoProcess() = 0;									//数据处理..分析得出模型
virtual bool StartThread(DWORD(WINAPI lp)(LPVOID) , HANDLE&) = 0;			//start the thread.
virtual bool EndThread(HANDLE&)  = 0;
virtual	bool SaveResult() = 0;									//保存训练结果
virtual bool LoadResult() = 0;
virtual	bool ChangeState(CAlgParamConfig& parm, bool s) = 0;	//修改算法参数

virtual void SetLogOut(void (*pFunc)(CString)) = 0;				//用于设置状态消息记录，m_WndOutput,可不设置

HANDLE		m_hTrainEvent;										//used for thread sync

CBaseArth*	m_pAlogrithm[3];									//指向算法模型

BOOL		m_bExit;											//线程退出标志

void(*m_pOutPut)(CString) = 0;									//输出操作

END_DECLEAR_INTERFACE


#endif