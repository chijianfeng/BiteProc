#pragma once
//实现设备相关参数的读取与配置

#include <iostream>
#include <map>
#include <string>
using namespace std;


class CDeviceConfig
{
public:
	CDeviceConfig();
	CDeviceConfig(CString path);

	~CDeviceConfig(void);

	bool	OpenFile(CString path);		//打开配置文件
	void	CloseFile();				//关闭文件
	bool	IsOpen();
	bool	LoadFileMsg();
	bool	SaveFileMsg();				//保存文件信息
	CString GetDevicePath();			//获取设备所在路径
	void	SetDevicePath(CString path);
	string	GetParam();					//数据文件路径
	void	SetParam(string s);
	int		GetChannum();
	void	SetChannum(int num);
	int		GetSamplerate();
	void	SetSamplerate(int s);
	BOOL	GetChans(UCHAR *chans , int num);
	BOOL	SetChans(const UCHAR *chans , int num);
	void	SetBuffersize(int size);
	int		GetBuffersize();
	void	SetMode(int mode);
	int		GetMode();

private:
	bool	IsSpace(char c);
	bool	IsCommentChar(char c);
	void	TrimString(string& str);
	bool	AnalyseLine(const string& line , string& key , string& value);

private:
	CString		m_nPath;
	CStdioFile	m_nFile;
	UCHAR		m_Chans[256];
	int			m_nChannum;
	int			m_nSamplerate;
	string		m_nLparam;				//保存相关附件信息

	bool		m_bIsopen;
	map<string , string>		m_Configmap;
};

