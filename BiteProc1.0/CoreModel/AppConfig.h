#pragma once

#include <iostream>
#include <map>

using namespace std;

class CAppConfig
{
public:
	CAppConfig();
	CAppConfig(CString path);

	virtual ~CAppConfig();

	//oper

	BOOL IsOpen();
	BOOL OpenFile(CString path);
	void CloseFile();

	//property

	size_t	GetWidth();				//���ô��ڿ�
	void	SetWidth(size_t w);

	size_t	GetHeight();
	void	SetHeight(size_t h);

	void	SetModulePath(CString path);
	CString	GetModulePath();

private:

	void	LoadFileMsg();			//��������		
	void	SaveFileMsg();			//��������

	bool	IsSpace(char c);
	bool	IsCommentChar(char c);
	void	TrimString(string& str);
	bool	AnalyseLine(const string& line, string& key, string& value);

private:

	CStdioFile			m_nFile;					//������ļ�

	BOOL		m_bIsopen;
	map<string, string>		m_Configmap;
};

