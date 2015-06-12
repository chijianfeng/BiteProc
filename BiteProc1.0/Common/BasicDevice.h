#pragma once
#pragma once
#include "Structdefine.h"

class BasicDevice
{
public:
    BasicDevice(void);
    BasicDevice(void*);
    virtual ~BasicDevice(void);
    virtual BOOL EB_OpenDevice(int usbportnumber) = 0;  //the int is the usb number.
    virtual BOOL EB_OpenDevice() = 0;
    virtual BOOL EB_CloseDevice() = 0;                     //0 is failed.1 is successes.
    virtual BOOL EB_GetData(double *data, ULONG size, OVERLAPPED *ov) = 0;
    virtual BOOL EB_SetBufferSize(WORD bufsize) = 0;
    virtual BOOL EB_SetSampleRate(WORD SampleRate) = 0;
    virtual BOOL EB_Startup() = 0;                              //after opening the device the device 
    //isn't starting to collect the data.unless by starting up. 
    virtual BOOL EB_Stop() = 0;
    virtual BOOL EB_SetChannel(UCHAR* Channals, int Chansize) = 0;
    //the function is used for filter in the device.
    virtual BOOL EB_GetFilter(FILT* filt) = 0;
    virtual BOOL EB_SetBandPass(UCHAR channal, int index) = 0;
    //setting MODE
    virtual BOOL EB_SetMode(int mode) = 0;
    virtual BOOL EB_GetMode(int *mode) = 0;
    virtual BOOL EB_SetScale(int offset[], int factor[]) = 0;    //set the weight.
    virtual BOOL EB_GetScale(int *offset, int *factor) = 0;
    //Error controling..
    virtual BOOL EB_GetLastError(char* errormsg) = 0;
    //info..
    virtual BOOL EB_GetInfo(Device_info *device) = 0;
    virtual BOOL EB_GetImpedance(UCHAR channels, double* impedance) = 0; //get the any channel's impedance.
};
