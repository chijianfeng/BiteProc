#pragma once
#include "basicdevice.h"
#include <vector>
#include <cstringt.h>
#define _MAX_LENGTH_ 256
const	int		MAXLENGTH = 1024*16;

class DateReader :
	public BasicDevice
{
public:
	DateReader(void);
	DateReader(char* p);
	virtual ~DateReader(void);

	//
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

	////inner function
	bool    m_bExit;	
	WORD	m_bufsize;

	BOOL ExtracetDatatoBuf( int size);
private: 

	CString	ExtractFilename(CString name , char c);
	bool	ReadHeader();
	bool	IsSpace(char c);
	bool	IsCommentChar(char c);
	void	TrimString(string& str);
	bool	AnalyseLine(const string& line , string& key , string& value);
	int		Readline(char *p , int len , string& str);

	char*	perrmsg;			//show the error message

	char*	pPath;				//the data file path
	WORD	m_samplerate;
	int		m_mode;				
	char*	pdeviemsg;
	CFile	m_nFile;
	int		m_nChannum;
	bool	m_bFileopen;

	ULONGLONG m_HeadSize;

	//data buffer
	double* m_pDataBuf;			//实现循环插入于与删除
	int		m_Rear , m_Head;

	OVERLAPPED* m_pCurov;
	int			m_CurGetLength;


	UCHAR*	m_pSelChan;
	int		m_nChansize;
	size_t	m_SampleInFile;
	CWinThread  *m_pThread;

	vector<CString> m_nDataFiles;
};

