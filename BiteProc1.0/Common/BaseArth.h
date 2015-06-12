#pragma once
#include "ManagedData.h"

class CBaseArth
{
public:
	CBaseArth(void);
	virtual ~CBaseArth(void);
	virtual bool	LoadTrainData(const char* pPath) = 0;						//导入训练数据

	virtual void	SetTrainData(const double* pd , const int len) = 0;			//写入训练数据

	virtual bool	Train( char* path = NULL) = 0;								//开始训练

	virtual bool	LoadTestData(const char *pPath) = 0;						//导入测试数据：从文件

	virtual	void	SetTestData(const double* pd , const int len) = 0;			//传递测试数据

	/*
	res is result and the len is the acturally return.
	*/
	virtual bool	Sim(ManagedData& data, ManagedData& result) = 0;			//ManageData进行数据处理

	virtual bool	Sim(double* res ,  int& len) = 0;							//仿真

	virtual bool	SaveTrainModel(const char* pPath) = 0;						//保存训练模型

	virtual bool	LoadModel(const char* pPath) = 0;							//导入模型

	virtual void	Config() = 0;												//模型配置

};

