#include "StdAfx.h"
#include "DeviceConfig.h"
#include "Stringoper.h"

const int MAXCHANNUM = 100;
const char COMMENT_CHAR1 = '#';
const char COMMENT_CHAR2 = '%';
const string My_key[] = {
	"Device_Path" , "Channel_number", "SampleRete" , 
	"Channels"/*1 5 8 ..*/ , "LParam" /*other message*/, 
	"Buffer_Size",
	"Mode"
};
const string My_Comment[] = {
	"##the DLL path" , "#Channel number" , "#the samplerate of the device",
	"%channels that collection in format of 1 2 ..." ,
	"#if the read data file the Lparam is the data file path , other is NULL##",
	"#recommend large than 8.", "#0 is normal , 1 is impedance",
};
CDeviceConfig::CDeviceConfig(CString path)
{
	m_nPath = path;
	m_bIsopen = m_nFile.Open(path , CFile::modeReadWrite)==TRUE?true:false;
	memset(m_Chans , 0 , sizeof(UCHAR)*MAXCHANNUM);
	m_nChannum = 0;
	m_nSamplerate = 0;
}

CDeviceConfig::CDeviceConfig()
{
	m_bIsopen = false;
	m_nChannum = 0;
	m_nSamplerate = 0;
}

void CDeviceConfig::CloseFile()
{
	if (m_bIsopen)
		m_nFile.Close();
	m_bIsopen = false;
}

CDeviceConfig::~CDeviceConfig(void)
{
	if (m_bIsopen)
		m_nFile.Close();
	m_bIsopen = false;
}

bool CDeviceConfig::OpenFile(CString path)
{
	m_nPath = path;
	m_bIsopen = m_nFile.Open(path, CFile::modeReadWrite)==TRUE?true:false;
	memset(m_Chans, 0, sizeof(UCHAR)*MAXCHANNUM);
	return m_bIsopen;
}

bool CDeviceConfig::IsOpen()
{
	if(!m_bIsopen)
		return false;
	return true;
}
bool CDeviceConfig::LoadFileMsg()
{
	if(!m_bIsopen)
		return false;
	m_nFile.SeekToBegin();
	m_Configmap.clear();
	string line, key, value;
	CString tmp;
	while(m_nFile.ReadString(tmp))
	{
		Stringoper oper;
		line = oper.tostring(tmp);
		if(AnalyseLine(line , key  , value))
		{
			m_Configmap[key] = value;
		}
	}
	return true;
}

bool CDeviceConfig::SaveFileMsg()
{
	if(!m_bIsopen)
		return false;
	m_nFile.SeekToBegin();
	m_nFile.SetLength(0);			//clear the befor content
	for (size_t i=0 ; i<m_Configmap.size();i++)
	{
		string s = My_key[i]+" "+"="+" "+
			m_Configmap[My_key[i]]+"  "+My_Comment[i]+"\n";
		m_nFile.Write(s.c_str() , s.size());
	}
	return TRUE;
}

CString CDeviceConfig::GetDevicePath()
{
	string key = My_key[0];
	Stringoper oper;
	m_nPath = oper.tocstring(m_Configmap[key]);
	return m_nPath;
}

void CDeviceConfig::SetDevicePath(CString path)
{
	Stringoper oper;
	string key = My_key[0];
	m_Configmap[key] = oper.tostring(path);
	m_nPath = path;
}

string CDeviceConfig::GetParam()
{
	string key = My_key[4];
	m_nLparam = m_Configmap[key];
	return m_nLparam;
}

void CDeviceConfig::SetParam(string s)
{
	string key = My_key[4];
	m_Configmap[key] = s;
	m_nLparam = s;
}

int CDeviceConfig::GetChannum()
{
	Stringoper oper;
	string key = My_key[1];
	m_nChannum = oper.stringtoint(m_Configmap[key]);
	return m_nChannum;
}

void CDeviceConfig::SetChannum(int num)
{
	Stringoper oper;
	string key = My_key[1];
	m_Configmap[key] = oper.inttostring(num);
	this->m_nChannum = num;
}

int CDeviceConfig::GetSamplerate()
{
	Stringoper oper;
	string key = My_key[2];
	m_nSamplerate = oper.stringtoint(m_Configmap[key]);
	return m_nSamplerate;
}

void CDeviceConfig::SetSamplerate(int s)
{
	Stringoper oper;
	string key = My_key[2];
	m_Configmap[key] = oper.inttostring(s);
	this->m_nSamplerate = s;
}

BOOL CDeviceConfig::GetChans(UCHAR *chans , int num)
{
	if(chans==NULL||num<=0)
		return FALSE;
	string key = My_key[3];
	string s = m_Configmap[key];
	Streamoper oper;
	int channum = GetChannum();
	double *pd = new double[channum];
	oper.InitDeseri(s);
	oper.DeserizlizeStream(pd,channum);
	for (int i=0;i<num;i++)
	{
		chans[i] = (UCHAR)(int)pd[i];
		m_Chans[i] = chans[i];
	}
	return TRUE;
}

BOOL CDeviceConfig::SetChans(const UCHAR *chans , int num)
{
	if(chans==NULL||num<=0)
		return FALSE;
	double c[256];
	for (int i=0;i<num;i++)
	{
		m_Chans[i] = chans[i];
		c[i] = (double)m_Chans[i];
	}
	Streamoper oper;
	string s = oper.SerizlizeStream(c , num);
	string key = My_key[3];
	m_Configmap[key] = s;
	SetChannum(num);														//update the channel number.
	return TRUE;
}

void CDeviceConfig::SetBuffersize(int size)
{
	string key = My_key[5];
	Stringoper oper;
	string str = oper.inttostring(size);
	m_Configmap[key] = str;
}

int CDeviceConfig::GetBuffersize()
{
	string key = My_key[5];
	Stringoper oper;
	int s = oper.stringtoint(m_Configmap[key]);
	return s;
}

void CDeviceConfig::SetMode(int mode)
{
	string key = My_key[6];
	Stringoper oper;
	string str = oper.inttostring(mode);
	m_Configmap[key] = str;
}

int CDeviceConfig::GetMode()
{
	string key = My_key[6];
	Stringoper oper;
	int s = oper.stringtoint(m_Configmap[key]);
	return s;
}

//private function
bool CDeviceConfig::IsSpace(char c)
{
	if(' '==c||'\t'==c)
		return true;
	return false;
}
bool CDeviceConfig::IsCommentChar(char c)
{
	switch(c){
	case COMMENT_CHAR1:
		return true;
	case COMMENT_CHAR2:
		return true;
	default:
		return false;
	}
}

void CDeviceConfig::TrimString(string& str)
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

bool CDeviceConfig::AnalyseLine(const string& line , string& key , string& value)
{
	if(line.empty())
		return false;
	int start_pos = 0 , end_pos = line.size()-1 , pos = 0;
	if((pos=line.find(COMMENT_CHAR1))!=-1||(pos=line.find(COMMENT_CHAR2))!=-1)
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