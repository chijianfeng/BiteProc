#pragma once
#include"BaseArth.h"
#include"CommonUtile.h"
#include"AlgParamConfig.h"
#include<map>
using namespace UTILE;

class CDistinguishHL :
	public CBaseArth
{
public:
	CDistinguishHL();
	CDistinguishHL(int len);
	virtual ~CDistinguishHL();
	bool    IsOneBite(ManagedData& d,int& startpos,double& LValue,double& RValue,double& LRValue);
	bool	LoadTrainData(const char* pPath);						//导入训练数据
	bool	Train(char* path);										//开始训练
	void	SetTestData(const double* pd, const int len);			//传递测试数据
	bool	Sim(double* res, int& len);
	bool	Sim(ManagedData& data, ManagedData& result);		    //仿真
	bool	SaveTrainModel(const char* pPath);						//保存训练模型
	bool	LoadModel(const char* pPath);							//导入模型
	int     judgeLH(int res,double t);
	bool    GetResult(map<string, double> &Result,int Time,Throld t);
	BpNetdata   CalNetData(const double *rawdatal, const double *rawdatar, const int length);
private:
	
				//ManageData进行数据处理
	bool	LoadTestData(const char *pPath);						//导入测试数据：从文件
	void	SetTrainData(const double* pd, const int len);			//写入训练数据
	void	Config();												//模型配置
	double*				m_pDataBuf;
	ManagedData*		m_pManagedata;
	ManagedData			m_FiltData;
	ManagedData			m_BufData;
	ManagedData         m_LeftData;
	ManagedData         m_RightData;
	bool				m_bIsOK;
	int					m_Winlen;
	int                 m_Time;   //计数，0：LeftModel 1: RightModel 2:BothModel
	double              m_LVol;
	double              m_RVol;
	double              m_LRVol;
	Throld				m_Throld;
	map<string, double>	m_nResult;
};