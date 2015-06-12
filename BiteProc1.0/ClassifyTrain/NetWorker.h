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
	CNetWorker(const char* pathmodel);				//���������ò���
	virtual ~CNetWorker();

	bool LoadTrainData(const char* pPath);

	bool Train(char* path /* = NULL */);

	bool Sim(double* res, int& len);

	bool Sim(ManagedData& data, ManagedData& color);

	bool SaveTrainModel(const char* pPath);

	bool LoadModel(const char* pPath);

	void SetTestData(const double* pd, const int len);

	//external function
	void GetTrainState(double& err, int& traintime);	//��ȡѵ��״̬

	void SetModelPath(const char* pPath);

	void SetParament(double err, int time);

	void StopTrain(const bool b);						//��ֹѵ��

	void SetLRThrolds(double left, double right);		//�����жϲ���

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

	double		m_dLeft;				//С�ڸ�ֵΪ���
	double		m_dRight;				//���ڸ�ֵΪ�ұ�

	int			m_Winlen;
	Throld		m_Throld;

	double		m_Error;				//ϵͳ���
	int			m_Traintime;			//���ѵ������

	ManagedData		m_LeftData;
	ManagedData		m_RightData;
};