#pragma once
#include "BaseArth.h"
#include "BpNeter.h"
#include "CommonUtile.h"
using namespace UTILE;

class CNetWorker :
	public CBaseArth
{
public:
	CNetWorker();
	CNetWorker(const char* pathmodel);				//神经网络配置参数
	virtual ~CNetWorker();

	bool LoadTrainData(const char* pPath);

	bool Train(char* path /* = NULL */);

	bool Sim(double* res, int& len);

	bool Sim(ManagedData& data, ManagedData& color);

	bool SaveTrainModel(const char* pPath);

	bool LoadModel(const char* pPath);

	void SetTestData(const double* pd, const int len);

	//external function
	void GetTrainState(double& err, int& traintime);	//获取训练状态

	void SetModelPath(const char* pPath);

	void SetParament(double err, int time);

	void StopTrain(const bool b);						//终止训练

	void SetLRThrolds(double left, double right);		//设置判断参数

	void SetWinlen(int len);

	void SetThreashold(Throld t);
private:
	void SetTrainData(const double* pd, const int len);

	bool LoadTestData(const char *pPath);

	void Config();

	BpNetdata	CalNetData(const double *rawdatal, const double *rawdatar, const int length);

public:
	CString		m_strModelPath;
	BpNeter		m_BpNet;

	double		m_dLeft;				//小于该值为左边
	double		m_dRight;				//大于该值为右边

	int			m_Winlen;
	Throld		m_Throld;

	double		m_Error;				//系统误差
	int			m_Traintime;			//最大训练次数

	ManagedData		m_LeftData;
	ManagedData		m_RightData;
};