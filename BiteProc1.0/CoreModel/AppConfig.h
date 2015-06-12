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

	size_t	GetWidth();				//设置窗口宽
	void	SetWidth(size_t w);

	size_t	GetHeight();
	void	SetHeight(size_t h);

	void	SetModulePath(CString path);
	CString	GetModulePath();

private:

	void	LoadFileMsg();			//加载数据		
	void	SaveFileMsg();			//保存数据

	bool	IsSpace(char c);
	bool	IsCommentChar(char c);
	void	TrimString(string& str);
	bool	AnalyseLine(const string& line, string& key, string& value);

private:

	CStdioFile			m_nFile;					//保存的文件

	BOOL		m_bIsopen;
	map<string, string>		m_Configmap;
};

