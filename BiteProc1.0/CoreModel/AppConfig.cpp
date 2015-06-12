#include "stdafx.h"
#include "AppConfig.h"
#include "Stringoper.h"

const char PARAM_COMMENT = '%';

const string Param_key[] = {
	"Width", "Height", "App Module Path",
};

CAppConfig::CAppConfig()
{
	m_bIsopen = FALSE;
}

CAppConfig::CAppConfig(CString path)
{
	m_bIsopen = FALSE;
	m_bIsopen = OpenFile(path);
	//m_Configmap.clear();
}


CAppConfig::~CAppConfig()
{
}

BOOL CAppConfig::IsOpen()
{
	return m_bIsopen;
}

BOOL CAppConfig::OpenFile(CString path)
{
	if (!m_bIsopen)
		m_bIsopen = m_nFile.Open(path, CFile::modeReadWrite);
	m_Configmap.clear();

	LoadFileMsg();

	return m_bIsopen;
}

void CAppConfig::CloseFile()
{
	if (m_bIsopen)
	{
		SaveFileMsg();
		m_nFile.Close();
		//m_Configmap.clear();
		m_bIsopen = FALSE;
	}
}

size_t CAppConfig::GetWidth()
{
	string key = Param_key[0];
	Stringoper oper;
	return oper.stringtoint(m_Configmap[key]);
}

void CAppConfig::SetWidth(size_t w)
{
	string key = Param_key[0];
	Stringoper oper;
	m_Configmap[key] = oper.inttostring(w);
}

size_t CAppConfig::GetHeight()
{
	string key = Param_key[1];
	Stringoper oper;
	return oper.stringtoint(m_Configmap[key]);
}

void CAppConfig::SetHeight(size_t h)
{
	string key = Param_key[1];
	Stringoper oper;
	m_Configmap[key] = oper.inttostring(h);
}

CString CAppConfig::GetModulePath()
{
	string key = Param_key[2];
	Stringoper oper;
	return oper.tocstring(m_Configmap[key]);
}

void CAppConfig::SetModulePath(CString path)
{
	string key = Param_key[2];
	Stringoper oper;
	m_Configmap[key] = oper.tostring(path);
}

//private function

void CAppConfig::LoadFileMsg()
{
	if (!m_bIsopen)
		return;
	m_nFile.SeekToBegin();
	m_Configmap.clear();
	string line, key, value;
	CString tmp;
	while (m_nFile.ReadString(tmp))
	{
		Stringoper oper;
		line = oper.tostring(tmp);
		if (AnalyseLine(line, key, value))
		{
			m_Configmap[key] = value;
		}
	}
}

void CAppConfig::SaveFileMsg()
{
	if (!m_bIsopen)
		return ;
	m_nFile.SeekToBegin();
	m_nFile.SetLength(0);
	for (size_t i = 0; i < 3; i++)
	{
		string s = Param_key[i] + " " + "=" + " " +
			m_Configmap[Param_key[i]] + "\n";
		m_nFile.Write(s.c_str(), s.size());
	}
}

bool CAppConfig::IsSpace(char c)
{
	if (' ' == c || '\t' == c)
		return true;
	return false;
}
bool CAppConfig::IsCommentChar(char c)
{
	switch (c){
	case PARAM_COMMENT:
		return true;

	default:
		return false;
	}
}

void CAppConfig::TrimString(string& str)
{
	if (str.empty())
		return;
	size_t i, start_pos, end_pos;
	for (i = 0; i < str.size(); i++)
	{
		if (!IsSpace(str[i]))
		{
			break;
		}
	}
	if (i == str.size())
	{
		str = "";
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

bool CAppConfig::AnalyseLine(const string& line, string& key, string& value)
{
	if (line.empty())
		return false;
	int start_pos = 0, end_pos = line.size() - 1, pos = 0;
	if ((pos = line.find(PARAM_COMMENT)) != -1)
	{
		if (0 == pos)
			return false;
		end_pos = pos - 1;
	}
	string new_line = line.substr(start_pos, end_pos - start_pos + 1);
	if ((pos = new_line.find('=')) == -1)
		return false;
	key = new_line.substr(0, pos);
	value = new_line.substr(pos + 1, end_pos + 1 - (pos + 1));

	TrimString(key);
	if (key.empty()) {
		return false;
	}
	TrimString(value);
	return true;
}
