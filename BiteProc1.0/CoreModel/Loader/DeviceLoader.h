#pragma once
//该类用于动态运行时加载设备
#include <iostream>
#include <cstring>
using namespace std;
template <typename T>
class CDeviceLoader
{
public:
	CDeviceLoader(void);
	~CDeviceLoader(void);
	BOOL LoadDevice(T** device);				//用于获取采集设备，若获取成功则返回true，否则为false.
	BOOL Free();								//释放相关设备资源，必须保证设备已经关闭
	void SetDevicePath(CString path);
	void Setparma(void *p);
private:
	CString		m_path;						    //the path of the device.
	HINSTANCE	m_Dll;
	T*			m_device;
	char*		lparma;
};

