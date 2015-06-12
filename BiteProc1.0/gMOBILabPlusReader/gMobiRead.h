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
	��ȡ�豸֧�ֵĲ����ʣ���Ϊ�ļ����򷵻��ļ�������Ϣ���������
	���ļ�û��������ã�����FALSE��
	��Ϊ����Ӳ���豸����Emotiv 128Ϊ��ߣ�������֧��64,32
	*/
	//BOOL EB_GetSampleRateRange(size_t* pd, size_t& len) ;
 BOOL EB_Startup() ;                              //after opening the device the device 
	//isn't starting to collect the data.unless by starting up. 
	BOOL EB_Stop() ;
	BOOL EB_SetChannel( UCHAR* Channals, int Chansize);

	//<********************************************************>
	/*
	��ȡ�豸�ṩ��ͨ��
	�磺F3 1
	F4 2....
	����豸���ļ�,�����ļ�ͷ���Ƿ������Ӧ�����ļ����ͨ����Ϣ
	�����������򷵻�FALSE
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
	�����豸��֧��ģʽ���ã���GetModeType ����false
	����GetModeType���ؿ����õ�ֵ
	0 ��ʾ����ģʽ
	1 ��ʾ�迹ģʽ
	2 ��ʾУ׼ģʽ
	*/
	// BOOL EB_GetModeType(size_t* pd, size_t& length) ;				//get the device mode

	//*********************************************************>
	/*
	�Ƿ�֧���迹��EEG����ͬʱ�ɼ���
	���֧�֣�����TRUE
	���򷵻� FALSE
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
	HANDLE hDevice;        //�豸���
	__CFG _devConfig;
	__DEVICESTATE  devState;
	BOOL    m_bExit;
	int		m_TotalChans;
	int	     m_Bufsize;
private:
	WORD    m_samplerate;
	
	_AIN    _analogCh;
	_DIO    _digitalCh;
	HANDLE	m_pThread;   //�߳̾��
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

