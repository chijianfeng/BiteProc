#include "StdAfx.h"
#include "EmotivRead.h"

#pragma comment(lib , "../lib/edk.lib")
const int MAXLENGTH = 1024*22;				//max data buf size.

UINT DataProcess(LPVOID lprarm)				//数据文件读取线程
{
	EmotivRead *pemo = (EmotivRead*)lprarm;
	if(pemo==NULL)
		return 1;
	int state;
	bool readytocollect = false;
	unsigned int nSamplesTaken=0;
	unsigned int channelCount = pemo->GetChangnum();
	double ** buffer = new double*[channelCount];
	while(!pemo->m_bExit)
	{
		state = EE_EngineGetNextEvent(pemo->m_neEvent);
		if(EDK_OK==state)
		{
			EE_Event_t eventType = EE_EmoEngineEventGetType(pemo->m_neEvent);
			EE_EmoEngineEventGetUserId(pemo->m_neEvent, &pemo->m_nUserID);
			if (eventType == EE_UserAdded) {
				EE_DataAcquisitionEnable(pemo->m_nUserID,true);
				readytocollect = true;
			}
		}
		if(readytocollect)
		{
			EE_DataUpdateHandle(0, pemo->m_hData);
			EE_DataGetNumberOfSample(pemo->m_hData,&nSamplesTaken);

			if(nSamplesTaken==0)
			{
				Sleep(1);
				continue;
			}
			for (size_t i=0; i<channelCount; i++)
				buffer[i] = new double[nSamplesTaken];
			EE_DataGetMultiChannels(pemo->m_hData, pemo->m_nChans, channelCount, buffer, nSamplesTaken);
			double *pt = new double[channelCount*nSamplesTaken];
			for (size_t i=0;i<nSamplesTaken;i++)
			{
				for (size_t j=0;j<channelCount;j++)
				{
					pt[channelCount*i+j] = buffer[j][i];
				}
			}
			pemo->SaveDatatoBuf(pt , channelCount*nSamplesTaken);
			delete pt;
			for (size_t i=0; i<channelCount; i++)		//realse;
				delete buffer[i];
		}
	}
	delete buffer;
	buffer = NULL;
	return 0;
}
EmotivRead::EmotivRead(void)
{
	m_nErrorCode = EDK_UNKNOWN_ERROR;
	m_hData = NULL;
	m_neEvent = NULL;
	m_neState = NULL;
	m_bExit = false;
	m_pDataBuf	=	new double[MAXLENGTH];
	//

	m_nUserID = 0;
	m_nSampleteRate = 128;
	m_nDeviceBufsize = 0;
	m_Rear		=	0;
	m_Head		=	0;
	m_pThread  = NULL;
}

EmotivRead::EmotivRead(char* p)
{
	m_nErrorCode = EDK_UNKNOWN_ERROR;
	m_hData = NULL;
	m_neEvent = NULL;
	m_neState = NULL;
	m_bExit = false;
	m_pDataBuf	=	new double[MAXLENGTH];

	m_nUserID = 0;
	m_nSampleteRate = 128;
	m_nDeviceBufsize = 0;
	m_Rear		=	0;
	m_Head		=	0;
	m_pThread  = NULL;
}

EmotivRead::~EmotivRead(void)
{
	if(m_pDataBuf!=NULL)
	{
		delete m_pDataBuf;
		m_pDataBuf = NULL;
	}
}

BOOL EmotivRead::EB_OpenDevice(int usbportnumber)
{
	m_nErrorCode = EE_EngineConnect();
	if(m_nErrorCode!=EDK_OK)
		return FALSE;
	m_neEvent = EE_EmoEngineEventCreate();
	m_neState = EE_EmoStateCreate();

	m_hData = EE_DataCreate();
	m_nState = EE_EngineGetNextEvent(m_neEvent);

	return TRUE;
}

BOOL EmotivRead::EB_OpenDevice()
{
	return EB_OpenDevice(0);
}

BOOL EmotivRead::EB_CloseDevice()
{
	if(m_hData!=NULL)
	{
		EE_DataFree(m_hData);
	}
	if(EE_EngineDisconnect()!=EDK_OK)
	{
		return FALSE;
	}
	if(m_neEvent!=NULL)
	{
		EE_EmoEngineEventFree(m_neEvent);
	}
	if(m_neState!=NULL)
	{
		EE_EmoStateFree(m_neState);
	}
	return TRUE;
}

BOOL EmotivRead::EB_GetData(double *data,ULONG size,OVERLAPPED *ov)
{
	if(NULL==data||ov==NULL||size<=0)
		return FALSE;
	m_pCurov = ov;
	m_Cursize = size;
	size_t length = (MAXLENGTH+m_Rear-m_Head)%MAXLENGTH;
	if(length<size)
	{
		ResetEvent(ov->hEvent);
		return FALSE;
	}
	for(size_t sample = 0;sample<size;sample++)
	{
		data[sample] = m_pDataBuf[m_Head++];
		m_Head = m_Head%MAXLENGTH;
	}
	return TRUE;
}

BOOL EmotivRead::EB_SetBufferSize(WORD bufsize)
{
	m_nDeviceBufsize = (float)(bufsize*1.0/m_nSampleteRate);
	return TRUE;
}

BOOL EmotivRead::EB_SetSampleRate(WORD SampleRate)
{
	m_nSampleteRate = 128;
	return TRUE;
}

BOOL EmotivRead::EB_Startup()
{
	EE_DataSetBufferSizeInSec(m_nDeviceBufsize);
	StartThread();
	return TRUE;
}

BOOL EmotivRead::EB_Stop()
{
	EndThread();
	return TRUE;
}

BOOL EmotivRead::EB_SetChannel(UCHAR* Channals,int Chansize)
{
	if(Channals==NULL)
		return FALSE;
	m_TotalChans = Chansize;
	for (int i=0;i<Chansize;i++)
	{
		m_nChans[i] = (EE_DataChannel_t)(Channals[i]+ED_AF3-1);			//for 1~14..
	}
	return TRUE;
}

BOOL EmotivRead::EB_GetFilter(FILT* filt)
{
	return FALSE;
}

BOOL EmotivRead::EB_SetBandPass(UCHAR channal,int index)
{
	return FALSE;
}

BOOL EmotivRead::EB_SetMode(int mode)
{
	return FALSE;
}

BOOL EmotivRead::EB_GetMode(int *mode)
{
	return FALSE;
}

BOOL EmotivRead::EB_SetScale(int offset[],int factor[])
{
	return FALSE;
}

BOOL EmotivRead::EB_GetScale(int *offset,int *factor)
{
	return FALSE;
}

BOOL EmotivRead::EB_GetLastError(char* errormsg)
{
	return TRUE;
}

BOOL EmotivRead::EB_GetInfo(Device_info *device)
{
	unsigned long data;
	int state = EE_HardwareGetVersion(m_nUserID , &data);
	if(state!=EDK_OK)
		return FALSE;
	device->version = (float)data;
	return TRUE;
}

BOOL EmotivRead::EB_GetImpedance(UCHAR channels,double* impedance)
{
	return FALSE;
}

BOOL EmotivRead::SaveDatatoBuf(double *pd ,  int size)
{
	if(NULL==pd||size<=0)
		return FALSE;
	int dalen = (m_Rear-m_Head+MAXLENGTH)%MAXLENGTH;
	if(dalen+size>=MAXLENGTH)
	{
		m_Head = (m_Head+dalen+size+1)%MAXLENGTH;
	}
	for (int i=0;i<size;i++)
	{
		m_pDataBuf[m_Rear++] = pd[i];
		m_Rear = m_Rear%MAXLENGTH;
	}
	if((m_Rear-m_Head+MAXLENGTH)%MAXLENGTH>=m_Cursize&&m_pCurov!=NULL)
		SetEvent(m_pCurov->hEvent);
	return TRUE;
}

unsigned int EmotivRead::GetChangnum()
{
	return (unsigned int)m_TotalChans;
}

BOOL EmotivRead::StartThread()
{
	if(m_pThread==NULL)
	{
		m_pThread = new CWinThread(DataProcess,LPVOID(this));
		m_pThread->CreateThread(CREATE_SUSPENDED);
		m_pThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
	}
	m_pThread->ResumeThread();
	return TRUE;
}
void EmotivRead::EndThread()
{
	if(m_pThread==NULL)
		return;
	m_bExit = true;
	DWORD dwRet = WaitForSingleObject(m_pThread->m_hThread , 5000);
	if(dwRet==WAIT_OBJECT_0)
	{
		TRACE("Exit successed\n");
	}
	else
	{
		DWORD ret = 0;
		GetExitCodeProcess(m_pThread->m_hThread  , &ret);
		TerminateThread(m_pThread->m_hThread ,ret);
		TRACE("Exit the thread but may cause some problem\n");
	}
	//	delete m_pThread;
	m_pThread = NULL;
}
//extern interface
extern "C" _declspec(dllexport) BasicDevice* CreateObject(void *p)
{
	return new EmotivRead((char*)p);
}