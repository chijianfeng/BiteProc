#include "StdAfx.h"
#include "BaseTrainWnd.h"
#include "ThreadUtil.h"

HANDLE MianEvent = 0;						//用于处理主线程和子线程

DWORD WINAPI SaveFileThread(LPVOID lparam)
{
//	CTrainWnd *pWnd =(CTrainWnd *) CWnd::FromHandle( reinterpret_cast< HWND >( lparam ) );
	CBaseTrainWnd *pWnd =(CBaseTrainWnd *)lparam;
	if (pWnd==NULL)
	{
		return 1;
	}
	while(!pWnd->m_bExit)
	{
		DWORD ret = WaitForSingleObject(pWnd->m_hTrainEvent , INFINITE);
		pWnd->SaveData();
	}
	SetEvent(MianEvent);
	pWnd->SendMessage(MYWM_THREADMSG, THREADMSG_SAVEFINISH);
	return 0;
}

DWORD WINAPI LoadFileThread(LPVOID lparam)
{
	//CTrainWnd *pWnd =(CTrainWnd *) CWnd::FromHandle( reinterpret_cast< HWND >( lparam ) );
	CBaseTrainWnd *pWnd =(CBaseTrainWnd *)lparam;
	if (pWnd==NULL)
	{
		return 1;
	}
	pWnd->LoadData();
	SetEvent(MianEvent);
	pWnd->SendMessage(MYWM_THREADMSG, THREADMSG_LOADFINISH);
	return 0;
}

DWORD WINAPI ProcessThread(LPVOID lparam)
{
//	CTrainWnd *pWnd =(CTrainWnd *) CWnd::FromHandle( reinterpret_cast< HWND >( lparam ) );
	CBaseTrainWnd *pWnd =(CBaseTrainWnd *)lparam;
	if (pWnd==NULL)
	{
		return 1;
	}
	pWnd->DoProcess();
	pWnd->SendMessage(MYWM_THREADMSG, THREADMSG_PROCESSFINISH);
	SetEvent(MianEvent);
	return 0;
}

DWORD WINAPI TrainThread(LPVOID lparam)
{
	HANDLE subthread = 0;
	CBaseTrainWnd *pWnd = (CBaseTrainWnd *)lparam;
	DWORD ret = WaitForSingleObject(MianEvent , INFINITE);	
	if(pWnd==NULL)
	{
		return 1;
	}
	pWnd->StartThread(SaveFileThread , subthread);
	ResetEvent(MianEvent);
	ret = WaitForSingleObject(MianEvent , INFINITE);
	pWnd->EndThread(subthread);
	Sleep(100);
	pWnd->StartThread(LoadFileThread , subthread);
	ResetEvent(MianEvent);
	ret = WaitForSingleObject(MianEvent , INFINITE);
	pWnd->EndThread(subthread);
	Sleep(100);
	pWnd->StartThread(ProcessThread , subthread);
	ResetEvent(MianEvent);
	ret = WaitForSingleObject(MianEvent , INFINITE);
	pWnd->EndThread(subthread);
	pWnd->SendMessage(MYWM_THREADMSG, THREADMSG_ALLFINISH);
	return 0;
}

