#include "StdAfx.h"
#include "Streamoper.h"
#include "Stringoper.h"

Streamoper::Streamoper(void)
{
	stream.clear();
}

Streamoper::~Streamoper(void)
{
	stream.str("");           //release the memory.
}
string Streamoper::SerizlizeStream(double *data, int Length)
{
	stringstream ss(stringstream::in | stringstream::out);
	ss.clear();
	ss.str("");
	for(int i=0;i<Length;i++)
	{
		ss<<data[i]<<" ";
	}
	return ss.str();
}
string Streamoper::SerizlizeStream(double data)
{
	stringstream ss(stringstream::in | stringstream::out);
	ss.clear();
	ss.str("");
	ss<<data<<" ";
	return ss.str();
}
string Streamoper::SerizlizeStream(int data)
{
	stringstream ss(stringstream::in | stringstream::out);
	ss.clear();
	ss.str("");
	ss<< data<<" ";
	return ss.str();
}
string Streamoper::SerizlizeStream(char c)
{
	stringstream ss(stringstream::in | stringstream::out);
	ss.clear();
	ss.str("");
	if(c==' ')
		c='#';
	ss<<c<<" ";
	return ss.str();
}
string Streamoper::SerizlizeStream(string str)
{
	
	Stringoper oper;
	string s;
	oper.Changestr(str,s,' ','#');
	return s+" ";
}
string Streamoper::SerizlizeStream(int *data,int Length)
{
	stringstream ss(stringstream::in | stringstream::out);
	ss.clear();
	ss.str("");
	for(int i=0;i<Length;i++)
	{
		ss<<data[i]<<" ";
	}
	return ss.str();
}

void Streamoper::DeserizlizeStream(int &data)
{
	stream>>data;
}

void Streamoper::DeserizlizeStream(double &data)
{
	stream>>data;
}

void Streamoper::DeserizlizeStream( double *data, int length)
{
	for(int i=0;i<length;i++)
	{
		stream>>data[i];
	}
}
void Streamoper::DeserizlizeStream(char &c)
{
	stream>>c;
	if(c=='#')
		c=' ';
}
void Streamoper::DeserizlizeStream(string &str)
{
	stream>>str;
	string s = str;
	Stringoper oper;
	oper.Changestr(s,str,'#',' ');
}
void Streamoper::DeserizlizeStream(int *data, const int length)
{
	for(int i=0;i<length;i++)
	{
		stream>>data[i];
	}
}
void Streamoper::DeserizlizeStream(int *data , int& length ,bool b)
{
	if(data==NULL)
		return;
	int count = 0;
	while(stream.good())
		stream>>data[count++];
	length = count;
}
void Streamoper::InitDeseri(string str)
{
	stream.clear();
	stream<<str;
}