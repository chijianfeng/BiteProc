#pragma once

/*
该类用于用户配置：
选择、加载用户参数，模型等数据
*/

#include <iostream>
#include <map>
using namespace std;
class CUserConfig
{
public:
	CUserConfig(void);
	~CUserConfig(void);

	bool		LoadConfig();
	bool		SaveResult();
	CString		GetCurUser();
	void		SetCurUser(CString name);
	bool		IsDefault();
	void		SetDefault(bool b);
	
	void		GetUserlist(map<CString , CString>& user);

	bool		AddNewUser(CString name , CString path);
	bool	    GetUserPath(CString user , CString& path);

private:

	bool	Equal(string key[] ,int size, string str);

	bool	IsSpace(char c);
	bool	IsCommentChar(char c);
	void	TrimString(string& str);
	bool	AnalyseLine(const string& line , string& key , string& value);
private:
	map<string,string>     m_Config;
};

