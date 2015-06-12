#include "StdAfx.h"
#include "GtecDevice.h"
#pragma comment(lib,"gUSBamp.lib")
const int MAXLENGTH = 1024*16;
const int USBPORT	= 11;
HANDLE	m_hEvent;
UINT DataProcess(LPVOID lprarm)
{
	CGtecDevice *pGtec = (CGtecDevice*)lprarm;
	if(pGtec==NULL)
		return 1;
	OVERLAPPED ov;
	ov.hEvent = m_hEvent;
	ov.Offset = 0;
	ov.OffsetHigh = 0;
	DWORD dwBytereceive = 0;
	DWORD dwlength = pGtec->m_bufsize;
	BYTE *pBuffer =  new BYTE[HEADER_SIZE + dwlength];
	float *pfl = NULL;
	float *pf = new float[dwlength/sizeof(float)];	//tmp buffer.
	double* pd = new double[dwlength/sizeof(float)];
	while(!pGtec->m_bExit)
	{
		GT_GetData(pGtec->m_hDevice,pBuffer , HEADER_SIZE+dwlength,&ov);
		DWORD dwOVret = WaitForSingleObject(m_hEvent , 2000);
		if(dwOVret==WAIT_TIMEOUT)
		{
			AfxMessageBox(_T("Timeout occured\n"), MB_ICONWARNING);
			pGtec->m_bExit = true;
			GT_ResetTransfer(pGtec->m_hDevice);
			break;
		}
		GetOverlappedResult(pGtec->m_hDevice,&ov,&dwBytereceive,FALSE);
		dwBytereceive -= HEADER_SIZE;
		for (size_t i=0;i<dwBytereceive/sizeof(float);i++)
		{
			pfl = (float*)(pBuffer+HEADER_SIZE+sizeof(float)*i);
			pf[i] = *pfl;
			pd[i] = pf[i];
		}
		pGtec->SaveDatatoBuf(pd , dwBytereceive/sizeof(float));
	}
	delete pf;
	delete pd;
	return 0;
}
CGtecDevice::CGtecDevice(void)
{
	m_pThread	=	NULL;
	m_bExit		=	false;
	m_bufsize	=	0;
	m_pCurov	=	NULL;
	m_pDataBuf	=	new double[MAXLENGTH];
	m_Rear		=	0;
	m_Head		=	0;

	m_Samplerete	 = 256;
	m_TotalChans	 = 8;
	m_filterhighpass = 0.1f;
	m_filterlowpass  = 60;
	m_notchhighpass  = 48;
	m_notchlowpass   = 52;
	m_filtermodelorder = 8;
	m_filtertype	 = 1;
	m_notchmodelorder = 4;
	m_notchtype      = 1;
	Samratemap.insert(map<int,int>::value_type(32,1));
	Samratemap.insert(map<int,int>::value_type(64,2));
	Samratemap.insert(map<int,int>::value_type(128,4));
	Samratemap.insert(map<int,int>::value_type(256,8));
	Samratemap.insert(map<int,int>::value_type(512,16));
	Samratemap.insert(map<int,int>::value_type(600,32));
	Samratemap.insert(map<int,int>::value_type(1200,64));
	Samratemap.insert(map<int,int>::value_type(2400,128));
	Samratemap.insert(map<int,int>::value_type(4800,256));
	Samratemap.insert(map<int,int>::value_type(9600,512));
	Samratemap.insert(map<int,int>::value_type(19200,512));
	Samratemap.insert(map<int,int>::value_type(38400,512));
	m_SampleBlocksize = 0;
	m_hDevice	=	NULL;
	m_Mode		=	M_NORMAL;
}

CGtecDevice::CGtecDevice(void*)
{
	m_pThread	=	NULL;
	m_bExit		=	false;
	m_bufsize	=	0;
	m_pCurov	=	NULL;
	m_pDataBuf	=	new double[MAXLENGTH];

	m_Samplerete	 = 256;
	m_TotalChans	 = 8;
	m_filterhighpass = 0.1f;
	m_filterlowpass  = 60;
	m_notchhighpass  = 48;
	m_notchlowpass   = 52;
	m_filtermodelorder = 8;
	m_filtertype	 = 1;
	m_notchmodelorder = 4;
	m_notchtype      = 1;
	Samratemap.insert(map<int,int>::value_type(32,1));
	Samratemap.insert(map<int,int>::value_type(64,2));
	Samratemap.insert(map<int,int>::value_type(128,4));
	Samratemap.insert(map<int,int>::value_type(256,8));
	Samratemap.insert(map<int,int>::value_type(512,16));
	Samratemap.insert(map<int,int>::value_type(600,32));
	Samratemap.insert(map<int,int>::value_type(1200,64));
	Samratemap.insert(map<int,int>::value_type(2400,128));
	Samratemap.insert(map<int,int>::value_type(4800,256));
	Samratemap.insert(map<int,int>::value_type(9600,512));
	Samratemap.insert(map<int,int>::value_type(19200,512));
	Samratemap.insert(map<int,int>::value_type(38400,512));
	m_SampleBlocksize = 0;
	m_hDevice	=	NULL;
	m_Mode		=   M_NORMAL;
}

CGtecDevice::~CGtecDevice(void)
{
	if(m_pDataBuf!=NULL)
	{
		delete m_pDataBuf;
		m_pDataBuf = NULL;
	}
}

BOOL CGtecDevice::EB_OpenDevice()
{
	int num;
	for (num = 0;num<USBPORT;num++)
	{
		m_hDevice = GT_OpenDevice(num);
		if(m_hDevice!=NULL)
			break;
	}
	if(num<USBPORT)
	{
		TRACE("OPen GTec Success.at %d\n",num);
		return TRUE;
	}
	else
	{
		TRACE("Can't open Gtec\n");
		return FALSE;
	}
}

BOOL CGtecDevice::EB_OpenDevice(int usbport)
{
	m_hDevice = GT_OpenDevice(usbport);
	if(NULL!=m_hDevice)
	{
		return TRUE;
	}
	else 
		return FALSE;
}

BOOL CGtecDevice::EB_CloseDevice()
{
	if(m_hDevice==NULL)
		return TRUE;
	if(m_bExit)
		return FALSE;
	BOOL ret =	GT_CloseDevice(&m_hDevice);
	m_hDevice = NULL;
	CloseHandle(m_hEvent);
	return ret;
}

BOOL CGtecDevice::EB_Startup()
{
	if(m_hDevice==NULL)
		return FALSE;
	m_hEvent = CreateEvent(NULL ,FALSE , FALSE , NULL);
	if(!GT_SetBufferSize(m_hDevice,m_SampleBlocksize))
		return FALSE;
	if(!GT_SetSampleRate(m_hDevice,m_Samplerete))
		return FALSE;
	if(!GT_SetChannels(m_hDevice , m_nChans , m_TotalChans))
		return FALSE;
	GT_SetMode(m_hDevice,m_Mode);
	if(m_Mode==M_IMPEDANCE)
		return TRUE;
	int findex = GetIndexofFilt();
	int nindex = GetIndexofNotch();
	for (int i=0;i<m_TotalChans;i++)
	{
		GT_SetBandPass(m_hDevice,m_nChans[i],findex);
		GT_SetNotch(m_hDevice , m_nChans[i] , nindex);
	}
	m_bufsize = m_TotalChans*sizeof(float)*m_SampleBlocksize;
	BOOL ret = GT_Start(m_hDevice);
	StartThread();
	return ret;
}

BOOL CGtecDevice::EB_Stop()
{
	//close the device and exit the thread.
	EndThread();
	return GT_Stop(m_hDevice);
	
}

BOOL CGtecDevice::EB_GetData(double *data,ULONG size,OVERLAPPED *ov)
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


BOOL CGtecDevice::SaveDatatoBuf(double *pd ,  int size)
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

BOOL CGtecDevice::EB_SetBufferSize(WORD bufsize)
{
	m_SampleBlocksize = bufsize;
	return TRUE;
}

BOOL CGtecDevice::EB_SetSampleRate(WORD SampleRate)
{
	if(SampleRate<=0)
		return FALSE;
	this->m_Samplerete = SampleRate;
	m_SampleBlocksize = max(m_SampleBlocksize , Samratemap[SampleRate]);
	return TRUE;
}

BOOL CGtecDevice::EB_SetChannel(UCHAR* Channals,int Chansize)
{
	if(Channals==NULL)
		return FALSE;
	m_TotalChans = Chansize;
	for (int i=0;i<Chansize;i++)
	{
		m_nChans[i] = Channals[i];
	}
	return TRUE;
}

BOOL CGtecDevice::EB_GetFilter(FILT* filt)
{
	if(filt!=NULL)
		return FALSE;
	int filtnum;
	GT_GetNumberOfFilter(&filtnum);
	if(filt==NULL)
		filt = new FILT[filtnum];
	if(!GT_GetFilterSpec(filt))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CGtecDevice::EB_SetBandPass(UCHAR channal,int index)
{
	//add code here for more operation.
	return TRUE;
}

BOOL CGtecDevice::EB_SetMode(int mode)
{
	if(mode<M_NORMAL||mode>M_COUNTER)
		return FALSE;
	m_Mode = mode;
	return TRUE;
}

BOOL CGtecDevice::EB_GetMode(int *mode)
{
	UCHAR m;
	BOOL ret = GT_GetMode(m_hDevice , &m);
	*mode = m;
	return ret;
}

BOOL CGtecDevice::EB_SetScale(int offset[],int factor[])
{
	//can add code ..
	return TRUE;
}

BOOL CGtecDevice::EB_GetScale(int *offset,int *factor)
{
	return TRUE;
}

BOOL CGtecDevice::EB_GetLastError(char* errormsg)
{
	if(errormsg==NULL)
	{
		WORD wErrorCode;
		char LastError[512] = "unavailable";
		GT_GetLastError(&wErrorCode, LastError);
		TRACE("The error is:%s\n",LastError);
	}
	else
	{
		WORD wErrorCode;
		GT_GetLastError(&wErrorCode,errormsg);
	}
	return TRUE;
}

BOOL CGtecDevice::EB_GetInfo(Device_info *device)
{
	if(device==NULL)
		return FALSE;
	device->version = GT_GetDriverVersion();
	GT_GetSerial(m_hDevice,device->info,16);
	return TRUE;
}

BOOL CGtecDevice::EB_GetImpedance(UCHAR channels,double* impedance)		//get the number of channels 's impedance.
{
	if(channels>m_TotalChans)
		return FALSE;
	if(!GT_GetImpedance(m_hDevice,channels,impedance))
	{
		return FALSE;
	}
	return TRUE;
}


//inner function
int CGtecDevice::GetIndexofFilt()
{
	int numoffilt = 0;
	int index = -1;
	GT_GetNumberOfFilter(&numoffilt);
	FILT *m_pFilt=NULL;
	m_pFilt = new _FILT[numoffilt];
	if(!GT_GetFilterSpec(m_pFilt))
	{
		TRACE("Can't get the FilterSpace!\n");
		return -1;
	}
	int i = 0;
	for(;i<numoffilt;i++)
	{
		if(fabs(m_pFilt[i].fu-m_filterhighpass)<0.0001&&fabs(m_pFilt[i].fo-m_filterlowpass)<0.0001
			&&m_pFilt[i].fs==m_Samplerete&&m_pFilt[i].order==m_filtermodelorder
			&&m_pFilt[i].type==m_filtertype)
		{
			index = i;
			break;
		}
	}
	delete[] m_pFilt;
	TRACE("the Filter order is: %d\n",index);
	return index;
}
int CGtecDevice::GetIndexofNotch()
{
	int index = -1;
	int numofnotch = 0;
	FILT *m_pFilt = NULL;
	GT_GetNumberOfNotch(&numofnotch);
	m_pFilt = new FILT[numofnotch];
	if(!GT_GetNotchSpec(m_pFilt))
	{
		TRACE("Can't get the Notch Filter\n");
		return -1;
	}
	int i=0;
	for (;i<numofnotch;i++)
	{
		if(m_pFilt[i].fu==m_notchhighpass&&m_pFilt[i].fo==m_notchlowpass
			&&m_pFilt[i].fs==m_Samplerete&&m_pFilt[i].order==m_notchmodelorder
			&&m_pFilt[i].type==m_notchtype)
		{
			index = i;
			break;
		}
	}
	delete[] m_pFilt;
	TRACE("the notch order is: %d\n",index);
	return index;
}
BOOL CGtecDevice::StartThread()
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
void CGtecDevice::EndThread()
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

extern "C" _declspec(dllexport) BasicDevice* CreateObject(void *p)
{
	return new CGtecDevice();
}