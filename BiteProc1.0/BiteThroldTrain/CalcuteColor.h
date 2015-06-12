#pragma once
#include "BaseArth.h"
#include "CommonUtile.h"
#include "AlgParamConfig.h"
using namespace UTILE;
//计算颜色值，只实现部分接口

class CCalcuteColor :
	public CBaseArth
{
public:
	CCalcuteColor();
	CCalcuteColor(int len);
	virtual ~CCalcuteColor();

	bool Sim(ManagedData& data, ManagedData& result);

	bool LoadModel(const char* pPath);

private:
	void FiltData(ManagedData& rawdata, ManagedData& filtData);		//滤波

	void ProcessData(ManagedData& rawdata, ManagedData& filtdata, ManagedData& colordata);	//计算颜色值

	double**		m_pData;							//数据缓存区
	double*			m_pDataBuf;							//单通道数据缓存区

	ManagedData*	m_pManagedata;

	Throld			m_BiteThreshold;					//咬牙阈值
	int				m_Winlen;							//配置相关参数

	ManagedData		m_ColorData;						//颜色缓存区

	ManagedData		m_FiltData;

	bool			m_bIsOK;							//是否已经加载了模型
	//实现接口函数
private:
	bool LoadTrainData(const char* pPath);

	void SetTrainData(const double* pd, const int len);

	bool Train(char* path /* = NULL */);

	bool LoadTestData(const char *pPath);

	void SetTestData(const double* pd, const int len);

	bool Sim(double* res, int& len);

	bool SaveTrainModel(const char* pPath);

	void Config();

};

