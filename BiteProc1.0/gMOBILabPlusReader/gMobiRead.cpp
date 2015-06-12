#include "stdafx.h"
#include "gMobiRead.h"
#pragma comment(lib, "gMOBIlabplus.lib")
#include <string>
#include <sstream>

const int NUMBER_OF_SCANS = 8;
const int MAXLENGTH = 1024 * 8;

DWORD WINAPI  DataProcess(LPVOID  lp)
{
	gMobiRead*  gmo = (gMobiRead*)lp;
	if (gmo == NULL)
		return 1;
	//initialize overlapped structure with the event used for data transfer between device and PC
	  OVERLAPPED ov;
   	  ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	  ov.Offset = 0;
	  ov.OffsetHigh = 0;
	//initialize receive buffer
	   int receiveBufferSize = min(gmo->m_Bufsize* gmo->m_TotalChans,512);   //not   exceed   1024byte
	  _BUFFER_ST receiveBuffer;
	  receiveBuffer.validPoints = 0;
	  receiveBuffer.size = receiveBufferSize * sizeof(short);
	   receiveBuffer.pBuffer = new short[receiveBufferSize];
	if (!GT_StartAcquisition(gmo->hDevice))
		return 2;
	while (!gmo->m_bExit)
	{
		GT_GetData(gmo->hDevice, &receiveBuffer, &ov);
		if (WaitForSingleObject(ov.hEvent, INFINITE) == WAIT_TIMEOUT)
		{
			//OutputDebugString("----------Error on data transfer: timeout  occured---------\n");
			return 3;
		}
		int num = receiveBufferSize;
		double *data = new double[num];
		for (int j = 0; j < num; j++)
		{
			data[j] = receiveBuffer.pBuffer[j];
		}
		gmo->SaveDatatoBuf(data, num);
		delete[] data;
	}
	//OutputDebugString("--------read  thread  exit-------\n");
	return 0;
}
gMobiRead::gMobiRead(void)
{
	//initialize analog channels 
	_analogCh.ain1 = FALSE;
	_analogCh.ain2 = FALSE;
	_analogCh.ain3 = FALSE;
	_analogCh.ain4 = FALSE;
	_analogCh.ain5 = FALSE;
	_analogCh.ain6= FALSE;
	_analogCh.ain7 = FALSE;
	_analogCh.ain8 = FALSE;
	//initialize digital channels
	_digitalCh.dio1_enable = FALSE;
	_digitalCh.dio2_enable = FALSE;
	_digitalCh.dio3_enable = FALSE;
	_digitalCh.dio4_enable = FALSE;
	_digitalCh.dio5_enable = FALSE;
	_digitalCh.dio6_enable = FALSE;
	_digitalCh.dio7_enable = FALSE;
	_digitalCh.dio8_enable = FALSE;
	_digitalCh.dio4_direction = FALSE;
	_digitalCh.dio5_direction = FALSE;
	_digitalCh.dio6_direction = FALSE;
	_digitalCh.dio7_direction = FALSE;
	m_samplerate = 256;
	m_bExit = FALSE;
	m_pThread =NULL;
	m_head = 0;
	m_tail = 0;
	eegdata = new double[MAXLENGTH];
	m_Bufsize = 8;
}


gMobiRead::~gMobiRead(void)
{
	if (eegdata != NULL)
	{
		delete[] eegdata;
		eegdata = NULL;
	}
}

gMobiRead::gMobiRead(char* p)
{
	//initialize analog channels 
	_analogCh.ain1 = FALSE;
	_analogCh.ain2 = FALSE;
	_analogCh.ain3 = FALSE;
	_analogCh.ain4 = FALSE;
	_analogCh.ain5 = FALSE;
	_analogCh.ain6 = FALSE;
	_analogCh.ain7 = FALSE;
	_analogCh.ain8 = FALSE;
	//initialize digital channels
	_digitalCh.dio1_enable = FALSE;
	_digitalCh.dio2_enable = FALSE;
	_digitalCh.dio3_enable = FALSE;
	_digitalCh.dio4_enable = FALSE;
	_digitalCh.dio5_enable = FALSE;
	_digitalCh.dio6_enable = FALSE;
	_digitalCh.dio7_enable = FALSE;
	_digitalCh.dio8_enable = FALSE;
	_digitalCh.dio4_direction = FALSE;
	_digitalCh.dio5_direction = FALSE;
	_digitalCh.dio6_direction = FALSE;
	_digitalCh.dio7_direction = FALSE;
	m_samplerate = 256;
	m_bExit = FALSE;
	m_pThread = NULL;
	m_head = 0;
	m_tail = 0;
	eegdata = new double[MAXLENGTH];
	m_Bufsize = 8;
}

BOOL gMobiRead::EB_OpenDevice(int usbportnumber)
{
	 stringstream  ss;
	 ss << "\\\\.\\COM" << usbportnumber << '\0';
	string  str = ss.str();
	 const char* msg = str.c_str();
	 char * portIdentifier= const_cast<char*>(msg);
	ss.clear();

     hDevice = GT_OpenDevice(portIdentifier);
	 if (hDevice == NULL)
	 {
		// OutputDebugString("---------handle doesn't  exit---------\n");
			 return FALSE;
	 }
	 else
	 {
		// OutputDebugString("-------open device success-------\n");
		 return TRUE;
	 }

}

BOOL gMobiRead::EB_OpenDevice()
{
	const  int usbportnumber = 9;
	stringstream  ss;
	int   k;
	for ( k = 4; k< usbportnumber; k++)
	{
		ss << "\\\\.\\COM" << k<< '\0';
		string  str = ss.str();
		const char* msg = str.c_str();
		char * portIdentifier = const_cast<char*>(msg);
		ss.clear();
		ss.str("");  //清空流里面的缓冲
		hDevice = GT_OpenDevice(portIdentifier);
		if (hDevice != NULL)
			break;
		else
			continue;
	}
	if (k < usbportnumber)
		return TRUE;
	else
		return FALSE;
	
}

BOOL gMobiRead::EB_CloseDevice()
{
	if (hDevice == NULL)
		return TRUE;
	BOOL ret=	GT_CloseDevice(hDevice);
	hDevice = NULL;
	return  ret;
}

BOOL gMobiRead::EB_GetData(double *data, const ULONG size, OVERLAPPED *ov)
{
	if (NULL == data || ov == NULL || size <= 0)
		return FALSE;
	m_pCurov = ov;
	m_Cursize = size;
	size_t length = (MAXLENGTH + m_tail - m_head) % MAXLENGTH;
	if (length < size)
	{
		ResetEvent(ov->hEvent);
		return FALSE;
	}
	for (size_t sample = 0; sample < size; sample++)
	{
		data[sample] = eegdata[m_head++];
		m_head = m_head%MAXLENGTH;
	}
	return TRUE;
}

BOOL gMobiRead::EB_SetBufferSize(const WORD bufsize)    //if bufsize = 0,then just check;
{
	m_Bufsize = bufsize;
	return TRUE;
}

BOOL gMobiRead::EB_SetSampleRate(const WORD SampleRate)
{
	m_samplerate = SampleRate;
	return TRUE;
}

//BOOL gMobiRead::EB_GetSampleRateRange(size_t* pd, size_t& len)
//{
//	if (pd == NULL)
	//	return FALSE;
	//len = 1;
	//pd[0] = 256;
	
	//return TRUE;
// }

BOOL gMobiRead::EB_Startup()
{
	if (!GT_InitChannels(hDevice, _analogCh, _digitalCh))         //初始化通道
		return FALSE;
	 StartThread();
	return TRUE;
}

BOOL gMobiRead::EB_Stop()
{
	EndThread();
	return TRUE;
}

BOOL gMobiRead::EB_SetChannel( UCHAR* Channals, int Chansize)
{
	if (Channals == NULL)
		return FALSE;
	m_TotalChans = Chansize;
	for (int i = 0; i < Chansize; i++)
	{
		switch (Channals[i])
		{
		case 1:
			_analogCh.ain1 = TRUE;
			break;
		case 2:
			_analogCh.ain2= TRUE;
			break;
		case 3:
			_analogCh.ain3 = TRUE;
			break;
		case 4:
			_analogCh.ain4 = TRUE;
			break;
		case 5:
			_analogCh.ain5 = TRUE;
			break;
		case 6:
			_analogCh.ain6 = TRUE;
			break;
		case 7:
			_analogCh.ain7 = TRUE;
			break;
		case 8:
			_analogCh.ain8 = TRUE;
			break;
        default:
            break;
		}	
	}
	return TRUE;
}
/*
BOOL gMobiRead::EB_GetChannel(char channelmsg[], size_t maxsize)
{
	if (channelmsg == NULL)
		return FALSE;
	stringstream ss;
	string str[] = { "ani1", "ani2", " ani3", "ani4", "ani5", " ani6", " ani7", "ani8"};
	for (int i = 0; i < 8; i++)
	{
		ss << str[i] << " " << i +1<< " ";
	}
	string  tmp = ss.str();
	//	size_t  len = tmp.length();
	strcpy_s(channelmsg, maxsize, tmp.c_str());

	return  TRUE;
} */

BOOL gMobiRead::EB_GetFilter(FILT* filt)
{
	return FALSE;
}

BOOL gMobiRead::EB_SetBandPass(UCHAR channal, int index)
{
	return FALSE;
}

BOOL gMobiRead::EB_SetMode(int mode)
{
	return TRUE;
}

BOOL gMobiRead::EB_GetMode(int *mode)
{
	return TRUE;
}

//BOOL    gMobiRead::EB_GetModeType(size_t* pd, size_t& length)
//{
	
//	return FALSE;
//}

//BOOL gMobiRead::EB_IsSupptImpandEEG()
//{
//	return FALSE;
//}

BOOL gMobiRead::EB_SetScale( int offset[],int factor[])
{
	return FALSE;
}

BOOL gMobiRead::EB_GetScale(int *offset, int *factor)
{
	return FALSE;
}

BOOL gMobiRead::EB_GetLastError(char* errormsg)
{
	UINT err;
	if (!GT_GetLastError(&err))
		return FALSE;
     //	stringstream  ss;
	//ss << "error code is  " << err;
	///string  str = ss.str();
	//  const char*msg = str.c_str();
	//  errormsg = const_cast<char*>(msg);
	//  ss.clear();

	_ERRSTR   ErrorString;
	GT_TranslateErrorCode(&ErrorString, err);
	errormsg = ErrorString.Error;
	  return TRUE;

}

BOOL gMobiRead::EB_GetInfo(Device_info *device)
{
	float  num = GT_GetDriverVersion();
	device->version = num;
	strcpy_s(device->info, 512, "");
	return TRUE;
}

BOOL gMobiRead::EB_GetImpedance(UCHAR channels, double* impedance)
{
	return  FALSE;
}

BOOL gMobiRead::StartThread()
{
	if (m_pThread == NULL)
	{
		m_pThread = CreateThread(NULL, 0, DataProcess, (LPVOID)this, 0, NULL);
		if (m_pThread == NULL)
			return FALSE;
		else
			return TRUE;
	}
	return FALSE;
}

void gMobiRead::EndThread()
{
	if (m_pThread == NULL)
		return  ;
	m_bExit = TRUE;
	DWORD ret = WaitForSingleObject(m_pThread, 5000);
	if (WAIT_OBJECT_0 != ret)
	{
		DWORD   cd=0 ;
		GetExitCodeProcess(m_pThread, &cd);
		TerminateThread(m_pThread, cd);
	}
	CloseHandle(m_pThread);
	m_pThread = NULL;
}

BOOL gMobiRead::SaveDatatoBuf(double *pd, int size)
{
	if (NULL == pd || size <= 0)
		return FALSE;
	int dalen = (m_tail - m_head+ MAXLENGTH) % MAXLENGTH;
	if (dalen + size >= MAXLENGTH)
	{
		m_head = (m_head + dalen + size + 1) % MAXLENGTH;
	}
	for (int i = 0; i < size; i++)
	{
		eegdata[m_tail++] = pd[i];
		m_tail = m_tail%MAXLENGTH;
	}
	if ((m_tail - m_head + MAXLENGTH) % MAXLENGTH >= m_Cursize&&m_pCurov != NULL)
		SetEvent(m_pCurov->hEvent);
	return TRUE;
}

//extern interface
extern "C" _declspec(dllexport) BasicDevice* CreateObject(void *p)
{
	return new gMobiRead((char*)p);
}

extern "C" _declspec(dllexport) char* GetDeviceMsg()
{
	return ("gMOBIlab+");
}