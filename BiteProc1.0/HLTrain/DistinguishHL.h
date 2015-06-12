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
	bool	LoadTrainData(const char* pPath);						//����ѵ������
	bool	Train(char* path);										//��ʼѵ��
	void	SetTestData(const double* pd, const int len);			//���ݲ�������
	bool	Sim(double* res, int& len);
	bool	Sim(ManagedData& data, ManagedData& result);		    //����
	bool	SaveTrainModel(const char* pPath);						//����ѵ��ģ��
	bool	LoadModel(const char* pPath);							//����ģ��
	int     judgeLH(int res,double t);
	bool    GetResult(map<string, double> &Result,int Time,Throld t);
	BpNetdata   CalNetData(const double *rawdatal, const double *rawdatar, const int length);
private:
	
				//ManageData�������ݴ���
	bool	LoadTestData(const char *pPath);						//����������ݣ����ļ�
	void	SetTrainData(const double* pd, const int len);			//д��ѵ������
	void	Config();												//ģ������
	double*				m_pDataBuf;
	ManagedData*		m_pManagedata;
	ManagedData			m_FiltData;
	ManagedData			m_BufData;
	ManagedData         m_LeftData;
	ManagedData         m_RightData;
	bool				m_bIsOK;
	int					m_Winlen;
	int                 m_Time;   //������0��LeftModel 1: RightModel 2:BothModel
	double              m_LVol;
	double              m_RVol;
	double              m_LRVol;
	Throld				m_Throld;
	map<string, double>	m_nResult;
};