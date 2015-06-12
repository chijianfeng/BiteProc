#pragma once
/************************************************************************/
/*emotive privades two different data source the eeg and the composer data
but  , this program is only get the eeg data.                           */
/************************************************************************/
#include "basicdevice.h"
#include "edkErrorCode.h"
#include "edk.h"
#include "EmoStateDLL.h"

class EmotivRead :
	public BasicDevice
{
public:
	EmotivRead(void);
	EmotivRead(char* p);
	~EmotivRead(void);

	 BOOL EB_OpenDevice(int usbportnumber) ;  //the int is the usb number.
	 BOOL EB_OpenDevice() ;
	 BOOL EB_CloseDevice();                     //0 is failed.1 is successes.
	 BOOL EB_GetData(double *data,ULONG size,OVERLAPPED *ov);
	 BOOL EB_SetBufferSize(WORD bufsize);        
	 BOOL EB_SetSampleRate(WORD SampleRate) ;
	 BOOL EB_Startup();                              //after opening the device the device 
	//isn't starting to collect the data.unless by starting up. 
	 BOOL EB_Stop() ;	
	 BOOL EB_SetChannel(UCHAR* Channals,int Chansize) ;
	//the function is used for filter in the device.
	 BOOL EB_GetFilter(FILT* filt) ;
	 BOOL EB_SetBandPass(UCHAR channal,int index);
	//setting MODE
	 BOOL EB_SetMode(int mode);
	 BOOL EB_GetMode(int *mode);
	 BOOL EB_SetScale(int offset[],int factor[]);    //set the weight.
	 BOOL EB_GetScale(int *offset,int *factor);
	//Error controling..
	 BOOL EB_GetLastError(char* errormsg) ;
	//info..
	 BOOL EB_GetInfo(Device_info *device);
	 BOOL EB_GetImpedance(UCHAR channels,double* impedance); //get the any channel's impedance.


	 bool			m_bExit;	
	 unsigned int	m_nUserID;
	 DataHandle		m_hData;							//data handle.
	 EmoEngineEventHandle m_neEvent;
	 EmoStateHandle  m_neState;
	 EE_DataChannel_t	m_nChans[256];
	 BOOL    SaveDatatoBuf(double *pd ,  int size);

	 unsigned int GetChangnum();
private: 

	CWinThread*		m_pThread;							//collect thread.
	OVERLAPPED*		m_pCurov;							//sync with the main thread.

	int				m_nErrorCode;						//connect state.
	int				m_nState;

	int				m_nSampleteRate;
	
	int				m_TotalChans;

	//data buffer
	double*			m_pDataBuf;							//实现循环插入于与删除
	int				m_Rear , m_Head;
	int				m_CurGetLength;						//the current data buffer length.
	int				m_Cursize;
	float			m_nDeviceBufsize;

	//inner function;
	BOOL	StartThread();
	void	EndThread();

};

