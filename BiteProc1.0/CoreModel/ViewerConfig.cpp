#include "StdAfx.h"
#include "ViewerConfig.h"
#include "Stringoper.h"

const char COMMENT_CHAR1 = '#';
const char COMMENT_CHAR2 = '%';
const string My_key[] = {
	"Search Dir", "Avaliable Model", "Length", "Width",
	"Max Value", "Min Value", "IsAutoMaxMin"
};
const string My_Comment[] = {
	"%视图所在路径", "%所选择的视图名", "%窗口长度", "%窗口宽度", 
	"%最大值", "%最小值", "%是否自动化最大最小值"
};

CViewerConfig::CViewerConfig()
{
	m_bIsopen = false;
	m_MaxValue = 0;
	m_MinValue = 0;
	m_IsAutoMaxMin = false;
	m_Length = 0;
	m_Width = 0;
}

CViewerConfig::CViewerConfig(CString path)
{
	m_ViewerPath = path;
	m_bIsopen = m_File.Open(m_ViewerPath, CFile::modeReadWrite) == TRUE ? true : false;
}

CViewerConfig::~CViewerConfig()
{
	if (m_bIsopen)
		m_File.Close();
	m_bIsopen = false;
}

bool CViewerConfig::OpenFile(CString path)
{
	m_ViewerPath = path;
	m_bIsopen = m_File.Open(m_ViewerPath, CFile::modeReadWrite) == TRUE ? true : false;
	return m_bIsopen;
}

void CViewerConfig::CloseFile()
{
	if (m_bIsopen)
		m_File.Close();
	m_bIsopen = false;
}

bool CViewerConfig::IsOpen()
{
	if (!m_bIsopen)
		return false;
	return true;
}

bool CViewerConfig::LoadFileMsg()
{
	if (!m_bIsopen)
		return false;
	m_File.SeekToBegin();
	m_Configmap.clear();
	string line, key, value;
	CString tmp;
	while (m_File.ReadString(tmp))
	{
		Stringoper oper;
		line = oper.tostring(tmp);
		if (AnalyseLine(line, key, value))
		{
			m_Configmap[key] = value;
		}
	}
	return true;
}

bool CViewerConfig::SaveFileMsg()
{
	if (!m_bIsopen)
		return false;
	m_File.SeekToBegin();
	m_File.SetLength(0);			
	for (size_t i = 0; i<m_Configmap.size(); i++)
	{
		string s = My_key[i] + " " + "=" + " " +
			m_Configmap[My_key[i]] + "  " + My_Comment[i] + "\n";
		m_File.Write(s.c_str(), s.size());
	}
	return TRUE;
}

CString CViewerConfig::GetViewerPath()
{
	string key = My_key[0];
	Stringoper oper;
	m_ViewerPath = oper.tocstring(m_Configmap[key]);
	return m_ViewerPath;
}

void CViewerConfig::SetViewerPath(CString path)
{
	Stringoper oper;
	string key = My_key[0];
	m_Configmap[key] = oper.tostring(path);
	m_ViewerPath = path;
}

CString CViewerConfig::GetSelDllPath()
{
	string key = My_key[1];
	Stringoper oper;
	m_SelDllPath = oper.tocstring(m_Configmap[key]);
	return m_SelDllPath;
}

void CViewerConfig::SetSelDllPath(CString path)
{
	Stringoper oper;
	string key = My_key[1];
	m_Configmap[key] = oper.tostring(path);
	m_SelDllPath = path;
}

int CViewerConfig::GetWindowLength()
{
	string key = My_key[2];
	Stringoper oper;
	m_Length = oper.stringtoint(m_Configmap[key]);
	return m_Length;
}

void CViewerConfig::SetWindowLength(int length)
{
	Stringoper oper;
	string key = My_key[2];
	m_Configmap[key] = oper.inttostring(length);
	m_Length = length;
}

int CViewerConfig::GetWindowWidth()
{
	string key = My_key[3];
	Stringoper oper;
	m_Width = oper.stringtoint(m_Configmap[key]);
	return m_Width;
}

void CViewerConfig::SetWindowWidth(int width)
{
	Stringoper oper;
	string key = My_key[3];
	m_Configmap[key] = oper.inttostring(width);
	m_Width = width;
}

int CViewerConfig::GetMaxValue()
{
	string key = My_key[4];
	Stringoper oper;
	m_MaxValue = oper.stringtoint(m_Configmap[key]);
	return m_MaxValue;
}

void CViewerConfig::SetMaxValue(int maxvalue)
{
	Stringoper oper;
	string key = My_key[4];
	m_Configmap[key] = oper.inttostring(maxvalue);
	m_MaxValue = maxvalue;
}

int CViewerConfig::GetMinValue()
{
	string key = My_key[5];
	Stringoper oper;
	m_MinValue = oper.stringtoint(m_Configmap[key]);
	return m_MinValue;
}

void CViewerConfig::SetMinValue(int minvalue)
{
	Stringoper oper;
	string key = My_key[5];
	m_Configmap[key] = oper.inttostring(minvalue);
	m_MinValue = minvalue;
}

BOOL CViewerConfig::GetIsAutoMaxMin()
{
	string key = My_key[6];
	Stringoper oper;
	m_IsAutoMaxMin = oper.stringtoint(m_Configmap[key]);
	return m_IsAutoMaxMin;
}

void CViewerConfig::SetIsAutoMaxMin(int autov)
{
	Stringoper oper;
	string key = My_key[6];
	m_Configmap[key] = oper.inttostring(autov);
	m_IsAutoMaxMin = autov;
}

bool CViewerConfig::IsSpace(char c)
{
	if (' ' == c || '\t' == c)
		return true;
	return false;
}

bool CViewerConfig::IsCommentChar(char c)
{
	switch (c){
	case COMMENT_CHAR1:
		return true;
	case COMMENT_CHAR2:
		return true;
	default:
		return false;
	}
}

void CViewerConfig::TrimString(string& str)
{
	if (str.empty())
		return;
	size_t i, start_pos, end_pos;
	for (i = 0; i<str.size(); i++)
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

bool CViewerConfig::AnalyseLine(const string& line, string& key, string& value)
{
	if (line.empty())
		return false;
	int start_pos = 0, end_pos = line.size() - 1, pos = 0;
	if ((pos = line.find(COMMENT_CHAR1)) != -1 || (pos = line.find(COMMENT_CHAR2)) != -1)
	{
		if (0 == pos)
			return false;
		end_pos = pos - 1;
	}
	string new_line = line.substr(start_pos, start_pos + 1 - end_pos);
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