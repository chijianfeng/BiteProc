#pragma once
class CBaseArth
{
public:
	CBaseArth(void);
	virtual ~CBaseArth(void);
	virtual bool	LoadTrainData(const char* pPath) = 0;

	virtual void	SetTrainData(const double* pd , const int len) = 0;

	virtual bool	Train( char* path = NULL) = 0;

	virtual bool	LoadTestData(const char *pPath) = 0;

	virtual	void	SetTestData(const double* pd , const int len) = 0;

	/*
	res is result and the len is the acturally return.
	*/
	virtual bool	Sim(double* res ,  int& len) = 0;

	virtual bool	SaveTrainModel(const char* pPath) = 0;

	virtual bool	LoadModel(const char* pPath) = 0;

	virtual void	Config() = 0;

};

