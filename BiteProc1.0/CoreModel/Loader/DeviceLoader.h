#pragma once
//�������ڶ�̬����ʱ�����豸
#include <iostream>
#include <cstring>
using namespace std;
template <typename T>
class CDeviceLoader
{
public:
	CDeviceLoader(void);
	~CDeviceLoader(void);
	BOOL LoadDevice(T** device);				//���ڻ�ȡ�ɼ��豸������ȡ�ɹ��򷵻�true������Ϊfalse.
	BOOL Free();								//�ͷ�����豸��Դ�����뱣֤�豸�Ѿ��ر�
	void SetDevicePath(CString path);
	void Setparma(void *p);
private:
	CString		m_path;						    //the path of the device.
	HINSTANCE	m_Dll;
	T*			m_device;
	char*		lparma;
};

