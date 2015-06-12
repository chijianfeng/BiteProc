
#ifndef STREAM_OPER_
#define STREAM_OPER_
#include <sstream>
#include <iostream>
using namespace std;
#pragma once

#endif

class Streamoper
{
public:
	Streamoper(void);
	~Streamoper(void);

	string SerizlizeStream(double *data,int Length);
	string SerizlizeStream(double date);
	string SerizlizeStream(int data);
	string SerizlizeStream(string str);
	string SerizlizeStream(char c);
	string SerizlizeStream(int *data,int Length);

	void DeserizlizeStream(double *data,int& length);
	void DeserizlizeStream(double &data);
	void DeserizlizeStream(int &data);
	void DeserizlizeStream(char &c);
	void DeserizlizeStream(string &s);
	void DeserizlizeStream(int *data, const int length);
	void DeserizlizeStream(int *data , int& length , bool b);
	
	void InitDeseri(string str);
protected:
	stringstream stream;
};

