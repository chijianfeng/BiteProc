#pragma once
/*
used for load and save the base Arithmetic config file.

*/
#include <iostream>
#include <map>
#include <vector>

using namespace std;
class BaseArthConfig
{
public:
	BaseArthConfig(void);
	virtual ~BaseArthConfig(void);

	 bool	LoadArthConfig(const char *pPath) ;

	 bool	SaveArthConfig(const char* pPath) ;

private:

	bool	IsSpace(char c);
	bool	IsCommentChar(char c);
	void	TrimString(string& str);
	bool	AnalyseLine(const string& line , string& key , string& value);

protected:
	CStdioFile			m_nFile;
	map<string,string>  m_Config;
	bool				m_bIsopen;
	string*				pParam_Key;
};

