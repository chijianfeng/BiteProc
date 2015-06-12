#include "StdAfx.h"
#include "UserConfig.h"
#include "Stringoper.h"

const CString path = _T("..\\User.config");				//系统用户参数文件，默认不可修改
const char PARAM_COMMENT = '%';
string _key[]={
	"Current User","Default",
};
CUserConfig::CUserConfig(void)
{
}


CUserConfig::~CUserConfig(void)
{
}

bool CUserConfig::LoadConfig()
{
	CFileFind finder;
	CStdioFile file;
	if (!finder.FindFile(path))				//create the file
	{
		file.Open(path, CFile::modeCreate | CFile::modeReadWrite);
	}
	else
	{
		if (!file.Open(path, CFile::modeRead))
			return false;
	}
	string line, key, value;
	CString tmp;
	while(file.ReadString(tmp))
	{
		Stringoper oper;
		line = oper.tostring(tmp);
		if(AnalyseLine(line , key  , value))
		{
			m_Config[key] = value;
		}
	}
	return true;
}

bool CUserConfig::SaveResult()
{
	CFileFind finder;
	if(!finder.FindFile(path))
		return false;
	CStdioFile file;
	if(!file.Open(path , CFile::modeWrite))
		return false;
	for (map<string,string>::iterator it = m_Config.begin();it!=m_Config.end();it++)
	{
		string s = it->first+" "+"="+" "+	it->second+"\n";
		file.Write(s.c_str() , s.size());
	}
	return true;
}

CString CUserConfig::GetCurUser()
{
	Stringoper oper;
	string key = _key[0];
	return oper.tocstring(m_Config[key]);
}

void CUserConfig::SetCurUser(CString name)
{
	Stringoper oper;
	string key = _key[0];
	m_Config[key] = oper.tostring(name);
}

bool CUserConfig::IsDefault()
{
	Stringoper oper;
	string key = _key[1];
	return oper.stringtobool(m_Config[key]);
}

void CUserConfig::SetDefault(bool b)
{
	Stringoper oper;
	string key = _key[1];
	m_Config[key] = oper.booltostring(b);
}

void CUserConfig::GetUserlist(map<CString , CString>& user)
{
	Stringoper oper;
	map<string,string>::iterator it = m_Config.begin();
	for (;it!=m_Config.end();it++)
	{
		if(!Equal(_key , 2,it->first))
		{
			CString key = oper.tocstring(it->first);
			user[key] = oper.tocstring(it->second);
		}
	}
}

bool CUserConfig::AddNewUser(CString name , CString path)
{
	Stringoper oper;
	if(name.GetLength()<=0)
		return false;
	string user = oper.tostring(name);
	for (map<string , string>::iterator it=m_Config.begin();it!=m_Config.end();it++)
	{
		if(user.compare(it->first)==0)
			return false;
	}
	m_Config[user] = oper.tostring(path);
	return true;
}

bool CUserConfig::GetUserPath(CString user , CString& path)
{
	if(user.GetLength()<=0)
		return false;
	Stringoper oper;
	for (map<string , string>::iterator it=m_Config.begin();it!=m_Config.end();it++)
	{
		if(it->first.compare(oper.tostring(user))==0)
		{
			path = oper.tocstring(it->second);
			return true;
		}
	}
	return false;
}

//private function
bool CUserConfig::IsSpace(char c)
{
	if(' '==c||'\t'==c)
		return true;
	return false;
}
bool CUserConfig::IsCommentChar(char c)
{
	switch(c){
	case PARAM_COMMENT:
		return true;

	default:
		return false;
	}
}

void CUserConfig::TrimString(string& str)
{
	if(str.empty())
		return;
	size_t i , start_pos , end_pos;
	for (i=0;i<str.size();i++)
	{
		if (!IsSpace(str[i]))
		{
			break;
		}
	}
	if(i==str.size())
	{
		str="";
		return;
	}
	start_pos = i;
	for (i = str.size() - 1; i >= 0; --i) {
		if (!IsSpace(str[i])) {
			break;
		}
	}
	end_pos = i;

	str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool CUserConfig::AnalyseLine(const string& line , string& key , string& value)
{
	if(line.empty())
		return false;
	int start_pos = 0 , end_pos = line.size()-1 , pos = 0;
	if((pos=line.find(PARAM_COMMENT))!=-1)
	{
		if(0==pos)
			return false;
		end_pos = pos - 1 ;  
	}
	string new_line = line.substr(start_pos, start_pos + 1 - end_pos); 
	if ((pos = new_line.find('=')) == -1)
		return false;  
	key = new_line.substr(0, pos);
	value = new_line.substr(pos + 1, end_pos + 1- (pos + 1));

	TrimString(key);
	if (key.empty()) {
		return false;
	}
	TrimString(value);
	return true;
}

bool CUserConfig::Equal(string key[] ,int size, string str)
{
	bool ret = false;
	for (int i=0;i<size;i++)
	{
		if(str.compare(key[i])==0)
		{
			ret = true;
			break;
		}
	}
	return ret;
}
