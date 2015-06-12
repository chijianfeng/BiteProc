#pragma once
#include "BasicDevice.h"
#include "gMOBIlabplus.h"

class gMobiRead :
	public BasicDevice
{
public:
	gMobiRead(void);
	gMobiRead(char* p);
	~gMobiRead(void);
	 BOOL EB_OpenDevice(int usbportnumber) ;  //the int is the usb number.
	 BOOL EB_OpenDevice() ;
	BOOL EB_CloseDevice() ;                     //0 is failed.1 is successes.
	BOOL EB_GetData(double *data, ULONG size, OVERLAPPED *ov);
	 BOOL EB_SetBufferSize( WORD bufsize) ;        //if bufsize = 0,then just check;
    BOOL EB_SetSampleRate( WORD SampleRate) ;
	/********************************************************
	获取设备支持的采样率，若为文件，则返回文件配置信息中相关设置
	若文件没有相关配置，返回FALSE，
	若为具体硬件设备：如Emotiv 128为最高，后续可支持64,32
	*/
	//BOOL EB_GetSampleRateRange(size_t* pd, size_t& len) ;
 BOOL EB_Startup() ;                              //after opening the device the device 
	//isn't starting to collect the data.unless by starting up. 
	BOOL EB_Stop() ;
	BOOL EB_SetChannel( UCHAR* Channals, int Chansize);

	//<********************************************************>
	/*
	获取设备提供的通道
	如：F3 1
	F4 2....
	如果设备是文件,解析文件头，是否包含相应数据文件相关通道信息
	若不包含，则返回FALSE
	*/
	//BOOL EB_GetChannel(char channelmsg[], size_t maxsize) ;		//get the channel basic infomation

	//the function is used for filter in the device.
	BOOL EB_GetFilter(FILT* filt) ;								//if NULL just check
	BOOL EB_SetBandPass(UCHAR channal, int index) ;			//if  index = -1 then check
	//setting MODE
	BOOL EB_SetMode(int mode) ;								//if mode = -1 then check
	BOOL EB_GetMode(int *mode) ;

	//<*********************************************************>
	/*
	若此设备不支持模式设置，则GetModeType 返回false
	否则GetModeType返回可设置的值
	0 表示正常模式
	1 表示阻抗模式
	2 表示校准模式
	*/
	// BOOL EB_GetModeType(size_t* pd, size_t& length) ;				//get the device mode

	//*********************************************************>
	/*
	是否支持阻抗和EEG数据同时采集。
	如果支持，返回TRUE
	否则返回 FALSE
	*/
	//BOOL EB_IsSupptImpandEEG() ;

	BOOL EB_SetScale(int offset[],  int factor[]);			//set the weight.
	BOOL EB_GetScale(int *offset, int *factor) ;
	//Error controling..
	 BOOL EB_GetLastError(char* errormsg) ;
	//info..
   BOOL EB_GetInfo(Device_info *device) ;
   BOOL EB_GetImpedance(UCHAR channels, double* impedance) ;			//get the any channel's impedance.,
	//if the impedance is null just check
   BOOL    SaveDatatoBuf(double *pd, int size);
public:
	HANDLE hDevice;        //设备句柄
	__CFG _devConfig;
	__DEVICESTATE  devState;
	BOOL    m_bExit;
	int		m_TotalChans;
	int	     m_Bufsize;
private:
	WORD    m_samplerate;
	
	_AIN    _analogCh;
	_DIO    _digitalCh;
	HANDLE	m_pThread;   //线程句柄
	//data  buffer
	double   *eegdata;
	int     m_head;
	int    m_tail;
	int			m_Cursize;
	OVERLAPPED*		m_pCurov;
private:
	BOOL	StartThread();
	void	EndThread();
  

};

