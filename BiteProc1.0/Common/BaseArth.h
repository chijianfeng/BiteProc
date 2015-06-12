#pragma once
#include "ManagedData.h"

class CBaseArth
{
public:
	CBaseArth(void);
	virtual ~CBaseArth(void);
	virtual bool	LoadTrainData(const char* pPath) = 0;						//����ѵ������

	virtual void	SetTrainData(const double* pd , const int len) = 0;			//д��ѵ������

	virtual bool	Train( char* path = NULL) = 0;								//��ʼѵ��

	virtual bool	LoadTestData(const char *pPath) = 0;						//����������ݣ����ļ�

	virtual	void	SetTestData(const double* pd , const int len) = 0;			//���ݲ�������

	/*
	res is result and the len is the acturally return.
	*/
	virtual bool	Sim(ManagedData& data, ManagedData& result) = 0;			//ManageData�������ݴ���

	virtual bool	Sim(double* res ,  int& len) = 0;							//����

	virtual bool	SaveTrainModel(const char* pPath) = 0;						//����ѵ��ģ��

	virtual bool	LoadModel(const char* pPath) = 0;							//����ģ��

	virtual void	Config() = 0;												//ģ������

};

