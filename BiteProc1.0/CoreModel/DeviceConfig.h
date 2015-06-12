#pragma once
//ʵ���豸��ز����Ķ�ȡ������

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

	bool	OpenFile(CString path);		//�������ļ�
	void	CloseFile();				//�ر��ļ�
	bool	IsOpen();
	bool	LoadFileMsg();
	bool	SaveFileMsg();				//�����ļ���Ϣ
	CString GetDevicePath();			//��ȡ�豸����·��
	void	SetDevicePath(CString path);
	string	GetParam();					//�����ļ�·��
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
	string		m_nLparam;				//������ظ�����Ϣ

	bool		m_bIsopen;
	map<string , string>		m_Configmap;
};

