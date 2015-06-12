#pragma once
#include "basicdevice.h"
#include "gUSBamp.h"
#include <map>
class CGtecDevice :
	public BasicDevice
{
public:
	CGtecDevice(void);
	CGtecDevice(void*);
	~CGtecDevice(void);

	BOOL EB_OpenDevice(int usbportnumber);			 //the int is the usb number.
	BOOL EB_OpenDevice() ;
	BOOL EB_CloseDevice();							 //0 is failed.1 is successes.
	BOOL EB_GetData(double *data,ULONG size,OVERLAPPED *ov);
	BOOL EB_SetBufferSize(WORD bufsize);        
	BOOL EB_SetSampleRate(WORD SampleRate) ;
	BOOL EB_Startup() ;                              //after opening the device the device 
	//isn't starting to collect the data.unless by starting up. 
	BOOL EB_Stop();	
	BOOL EB_SetChannel(UCHAR* Channals,int Chansize);
	//the function is used for filter in the device.
	BOOL EB_GetFilter(FILT* filt);
	BOOL EB_SetBandPass(UCHAR channal,int index);
	//setting MODE
	BOOL EB_SetMode(int mode);
	BOOL EB_GetMode(int *mode);
	BOOL EB_SetScale(int offset[],int factor[]);	  //set the weight.
	BOOL EB_GetScale(int *offset,int *factor);
	//Error controlling..
	BOOL EB_GetLastError(char* errormsg);
	//info..
	BOOL EB_GetInfo(Device_info *device) ;
	BOOL EB_GetImpedance(UCHAR channels,double* impedance);	//get the any channel's impedance.

	//
	BOOL SaveDatatoBuf(double *pd ,  int size);

	bool    m_bExit;	
	WORD	m_bufsize;
	HANDLE			m_hDevice;
private:

	CWinThread*		m_pThread;							//collect thread.
	OVERLAPPED*		m_pCurov;							//sync with the main thread.
	int				m_CurGetLength;						//the current data buffer length.
	int				m_Cursize;
	//device param.
	
	
	
	//data buffer
	double* m_pDataBuf;			//实现循环插入于与删除
	int		m_Rear , m_Head;

	//set default value.
	WORD m_Samplerete;
	int	 m_TotalChans;
	int	 m_Bufsize;
	int  m_Mode;
	int  m_SampleBlocksize;
	UCHAR m_nChans[256];	
	int  GetIndexofFilt();
	int  GetIndexofNotch();
	map<int,int> Samratemap;
	float m_filterhighpass, m_filterlowpass, m_notchhighpass, m_notchlowpass;
	int   m_filtermodelorder, m_filtertype, m_notchmodelorder, m_notchtype;

	//inner function;
	BOOL	StartThread();
	void	EndThread();

};

