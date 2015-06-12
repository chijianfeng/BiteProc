#pragma once
#include <afxwin.h>
#include <iostream>
#include <sstream>
#include "Streamoper.h"
using namespace std;

class Stringoper
{
public:
	Stringoper(void);
	~Stringoper(void);
	CString IptoStr(DWORD ipword);
	DWORD	Strtoip(CString ip);
	DWORD	ChangetoIP(UCHAR a[]);
	string	tostring(CString str);
	void	Changestr(string input,string &output,char oldvalue,char newvalue);
	CString chartocstring(char* c);
	string	inttostring(int id);
	string	doubletostring(double data);
	double	stringtodouble(string str);
	string	chartostring(char c);
	CString tocstring(string str);
	int		stringtoint(string str);
	string	charstostring(char *c);
	string	Getsubstr(string str,string substr);
	bool	Isequal(char* c1,char* c2);
	void	CStringtoChars(CString str,char c[]);
	string	booltostring(bool b);
	bool	stringtobool(string s);

	bool	AnalyseLine(const string& line, string& key, string& value , char comment);		//字符串分析
protected:
	bool	IsSpace(char c);								//判断字符串中是否有空格
	bool	IsCommentChar(char c);							//是否是注释符
	void	TrimString(string& str);						//去掉首位空格

private:
	char	m_Comment;
};
