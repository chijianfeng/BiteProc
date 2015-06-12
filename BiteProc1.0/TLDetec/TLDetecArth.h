#pragma once
#include "basearth.h"
#include "TLDetecConfig.h"
#include "CommonUtile.h"

using namespace UTILE;

const int _MAX_DATA_LENGTH  = 1000;			//the max legth dataset
class DataSet
{
public:
	int len;
	double *pd;
	int rlen;
	double *res;
};
class Pos
{
public:
	int sp ;
	int ep;
};
class CTLDetecArth :
	public CBaseArth
{
public:
	CTLDetecArth(void);
	CTLDetecArth(const char* parmpath);
	virtual ~CTLDetecArth(void);

	bool	LoadTrainData(const char* pPath);

	bool	Train( char* path = NULL);

	void	SetTestData(const double* pd , const int len);

	/*
	res is result and the len is the acturally return.
	*/
	bool	Sim(double* res ,  int& len);

	bool	Sim(ManagedData& data, ManagedData& result);

	bool	SaveTrainModel(const char* pPath);

	bool	LoadModel(const char* pPath);

	void	Config() ;
private:

	bool	LoadTestData(const char *pPath);
	void	SetTrainData(const double* pd , const int len);
	//get the max successional data length and the p2 is the data position.
	int		GetMaxLength(const double* pd , const Pos p1 , const double thresd ,  Pos& p2);

	void	Release();

	bool	IsSpace(char c);
	bool	IsCommentChar(char c);
	void	TrimString(string& str);
	bool	AnalyseLine(const string& line , string& key , string& value);

	int		GetSelChannel(ManagedData& d);

	//判断是不是一次咬,并返回处理的起始位置
	bool	IsOneBite(ManagedData& d , int& startpos);

	DataSet* m_nPData;

	DataSet	m_nSimData;			//for sim data

	int		m_nSamplenum;
	
	CString	m_nParamPath;
	CTLDetecConfig	m_nConfig;

	int		m_nThres;
};

