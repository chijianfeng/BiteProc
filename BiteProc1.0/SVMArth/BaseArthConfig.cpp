#include "StdAfx.h"
#include "BaseArthConfig.h"
#include "Stringoper.h"

const char PARAM_COMMENT = '%';
BaseArthConfig::BaseArthConfig(void)
{
	m_bIsopen = false;
	m_Config.clear();
}


BaseArthConfig::~BaseArthConfig(void)
{
}

bool BaseArthConfig::LoadArthConfig(const char *pPath)
{
	if(pPath==NULL)
		return false;
	if(!m_bIsopen)
	{
		Stringoper oper;
		m_bIsopen = m_nFile.Open(oper.tocstring(pPath) , CFile::modeRead);
		m_Config.clear();
	}
	if(!m_bIsopen)
		return false;
	m_nFile.SeekToBegin();
	string line, key, value;
	CString tmp;
	while(m_nFile.ReadString(tmp))
	{
		Stringoper oper;
		line = oper.tostring(tmp);
		if(AnalyseLine(line , key  , value))
		{
			m_Config[key] = value;
		}
	}
	m_nFile.Close();
	m_bIsopen = false;
	return true;
}

bool BaseArthConfig::SaveArthConfig(const char* pPath)
{
	if (pPath==NULL)
	{
		return false;
	}
	if(m_bIsopen)
		return false;
	Stringoper oper;
	m_bIsopen = m_nFile.Open(oper.tocstring(pPath) , CFile::modeWrite);
	if(!m_bIsopen)
		return false;
	m_nFile.SeekToBegin();
	m_nFile.SetLength(0);
	for (int i=0 ; i<m_Config.size();i++)
	{
		string s = pParam_Key[i]+" "+"="+" "+
			m_Config[pParam_Key[i]]+"\n";
		m_nFile.Write(s.c_str() , s.size());
	}
	m_nFile.Close();
	m_bIsopen = false;
	return true;
}

//private function
bool BaseArthConfig::IsSpace(char c)
{
	if(' '==c||'\t'==c)
		return true;
	return false;
}
bool BaseArthConfig::IsCommentChar(char c)
{
	switch(c){
	case PARAM_COMMENT:
		return true;

	default:
		return false;
	}
}

void BaseArthConfig::TrimString(string& str)
{
	if(str.empty())
		return;
	int i , start_pos , end_pos;
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

bool BaseArthConfig::AnalyseLine(const string& line , string& key , string& value)
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

