#include "StdAfx.h"
#include "Stringoper.h"

Stringoper::Stringoper(void)
{
}

Stringoper::~Stringoper(void)
{
}
DWORD Stringoper::ChangetoIP(UCHAR a[])
{
	DWORD ip = 0;
	if(a[0]>255||a[3]<0)
		return 0;
	ip+=a[0]<<24;
	if(a[1]>255||a[3]<0)
		return 0;
	ip+=a[1]<<16;
	if(a[2]>255||a[3]<0)
		return 0;
	ip+=a[2]<<8;
	if(a[3]>255||a[3]<0)
		return 0;
	ip+=a[3];
	return ip;
}
CString Stringoper::IptoStr(DWORD ipword)
{
	CString ipname;
	int ip[4]={0};
	ip[0] = ((ipword>>24)&0XFF);
	ip[1] = ((ipword>>16)&0XFF);
	ip[2] = ((ipword>>8)&0XFF);
	ip[3] = ((ipword&0XFF));
	ipname.Format(_T("%u.%u.%u.%u"),ip[0],ip[1],ip[2],ip[3]);
	return ipname;
}
string Stringoper::tostring(CString str)
{
#if _MSC_VER<=1200					//vc6.0 or eariler version
		if (str.IsEmpty())
		{
			return "";
		}
		string s = str.GetBuffer(0);
		str.ReleaseBuffer(0);
		return s;
#endif
	USES_CONVERSION;
	string s = (W2A(str));
	//str.ReleaseBuffer(0);
	return s;
}
void Stringoper::Changestr(string input,string &output,char oldvalue,char newvalue)
{
	if(input.length()==0)
	{
		output = newvalue;
		return;
	}
	for(size_t i=0;i<input.length();i++)
	{
		if(input[i]==oldvalue)
			input = newvalue;
	}
	output = input;
}

CString Stringoper::chartocstring(char* c)
{
	int len = strlen(c);
	WCHAR *m_pchar;
	m_pchar = new WCHAR[2*len+1];
	LONG l = MultiByteToWideChar(CP_ACP,0,
		c,len,m_pchar,2*len+1);
	m_pchar[l] = '\0';
	CString cs;
	cs.Format(_T("%s"),m_pchar);
	delete m_pchar;
	return cs;
}
string Stringoper::inttostring(int id)
{
	Streamoper oper;
	return oper.SerizlizeStream(id);
}
string Stringoper::doubletostring(double data)
{
	Streamoper oper;
	return oper.SerizlizeStream(data);
}
string Stringoper::chartostring(char c)
{
	Streamoper oper;
	return oper.SerizlizeStream(c);
}
CString Stringoper::tocstring(string str)
{
	CString cs;
	cs= str.c_str();
	return cs;
}
DWORD Stringoper::Strtoip(CString ip)
{
	if(ip.IsEmpty())
		return 0;
	string str = tostring(ip);
	int c[4];
	UCHAR data[4];
	stringstream ss;
	ss<<str.c_str();
	char tmp;
	for(int i=0;i<=3;i++)
	{
		ss>>c[i];
		data[i] = (UCHAR)c[i];
		ss>>tmp;
	}
	
	return ChangetoIP(data);
}
int Stringoper::stringtoint(string str)
{
	if(str.length()==0)
		return 0;
	stringstream ss;
	ss<<str;
	int val= 0;
	ss>>val;
	return val;
}
string Stringoper::charstostring(char *c)
{
	string str;
	ASSERT(c!=NULL);
	str = c;
	return str;
}
string Stringoper::Getsubstr(string str,string substr)
{
	//this function used for get the string 
	size_t pos;
	pos = str.find(substr);
	pos = pos+substr.length();
	return str.substr(pos);
}
bool Stringoper::Isequal(char* c1,char* c2)
{
	if(c1==NULL||c2==NULL)
		return FALSE;
	return strcmp(c1,c2)==0?true:false;
}

void Stringoper::CStringtoChars( CString str,char c[] )
{
	if(str.GetLength()<=0||str.GetLength()>=100)
	{
		c[0] = '\0';
		return;
	}
	int length = str.GetLength();
	int len = WideCharToMultiByte(CP_ACP,0,str,length,NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,str,length+1,c,len+1,NULL,NULL);
	c[len+1] = '\0';
}

string Stringoper::booltostring(bool b)
{
	string s;
	if(b)
		s = "1";
	else
		s = "0";
	return s;
}

bool Stringoper::stringtobool(string s)
{
	if (s.size()<=0)
	{
		return false;
	}
	int i = stringtoint(s);
	if(i)
		return true;
	else
		return false;
}

double Stringoper::stringtodouble(string str)
{
	return atof(const_cast<const char *>(str.c_str()));
}

bool Stringoper::AnalyseLine(const string& line, string& key, string& value ,char comment)
{
	m_Comment = comment;
	if (line.empty())											//若字符串空，直接返回
		return false;
	int start_pos = 0, end_pos = line.size() - 1, pos = 0;
	if ((pos = line.find(m_Comment)) != -1)						//查找该行注释符位置
	{
		if (0 == pos)												//整行都是注释，返回
			return false;
		end_pos = pos - 1;										//确定末尾
	}
	string new_line = line.substr(start_pos, start_pos + 1 - end_pos);	//提取非注释内容
	if ((pos = new_line.find('=')) == -1)						//将=两边数据提取出来
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

bool Stringoper::IsSpace(char c)
{
	if (' ' == c || '\t' == c)
		return true;
	return false;
}
bool Stringoper::IsCommentChar(char c)
{
	if (c == m_Comment)
		return true;
	else
		return false;
}

void Stringoper::TrimString(string& str)
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