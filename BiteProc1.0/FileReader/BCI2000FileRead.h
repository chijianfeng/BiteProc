#pragma once
#include "basicdevice.h"
#include "./BCI2000File/BCI2000FileReader.h"
#include <cstringt.h>
#define _MAX_LENGTH_ 256
const	int		MAXLENGTH = 1024*16;
class BCI2000FileRead :
	public BasicDevice
{
public:
	BCI2000FileRead();
	BCI2000FileRead(char* );
	BCI2000FileRead(const BCI2000FileRead& reader);
	BCI2000FileRead& operator=(const BCI2000FileRead& reader);
	~BCI2000FileRead(void);
	BOOL EB_OpenDevice(int usbportnumber){return TRUE;}  //the int is the usb number.
	BOOL EB_OpenDevice() ;
	BOOL EB_CloseDevice();                     //0 is failed.1 is successes.
	BOOL EB_GetData(double *data,ULONG size,OVERLAPPED *ov);
    BOOL EB_SetBufferSize(WORD bufsize);        
	BOOL EB_SetSampleRate(WORD SampleRate) ;
	BOOL EB_Startup() ;                              //after opening the device the device 
	//isn't starting to collect the data.unless by starting up. 
	BOOL EB_Stop();	
	BOOL EB_SetChannel(UCHAR* Channals,int Chansize);
	//the function is used for filter in the device.
	BOOL EB_GetFilter(FILT* filt) {return TRUE;}
	BOOL EB_SetBandPass(UCHAR channal,int index){return TRUE;}
	//setting MODE
	BOOL EB_SetMode(int mode);
	BOOL EB_GetMode(int *mode);
	BOOL EB_SetScale(int offset[],int factor[]){return TRUE;}    //set the weight.
	BOOL EB_GetScale(int *offset,int *factor){return TRUE;}
	//Error controlling..
	BOOL EB_GetLastError(char* errormsg);
	//info..
	BOOL EB_GetInfo(Device_info *device) ;
	BOOL EB_GetImpedance(UCHAR channels,double* impedance){return TRUE;}//get the any channel's impedance.

	//inner function
	BOOL ExtracetDatatoBuf( int size);

	bool    m_bExit;	
	WORD	m_bufsize;
private:

	CString	ExtractFilename(CString name , char c);

	char*	perrmsg;			//show the error message

	char*	pPath;				//the data file path
	WORD	m_samplerate;
	int		m_mode;				
	char*	pdeviemsg;
	BCI2000FileReader	m_File;
	int		m_ChannelNum;

	//data buffer
	double* m_pDataBuf;			//实现循环插入于与删除
	int		m_Rear , m_Head;

	OVERLAPPED* m_pCurov;
	int			m_CurGetLength;

	//for file parm
	UCHAR*	m_pSelChan;
	int		m_nChansize;
	size_t	m_SampleInFile;
	CWinThread  *m_pThread;

	vector<CString> m_nDataFiles;
};

