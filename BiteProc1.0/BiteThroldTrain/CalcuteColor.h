#pragma once
#include "BaseArth.h"
#include "CommonUtile.h"
#include "AlgParamConfig.h"
using namespace UTILE;
//������ɫֵ��ֻʵ�ֲ��ֽӿ�

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
	void FiltData(ManagedData& rawdata, ManagedData& filtData);		//�˲�

	void ProcessData(ManagedData& rawdata, ManagedData& filtdata, ManagedData& colordata);	//������ɫֵ

	double**		m_pData;							//���ݻ�����
	double*			m_pDataBuf;							//��ͨ�����ݻ�����

	ManagedData*	m_pManagedata;

	Throld			m_BiteThreshold;					//ҧ����ֵ
	int				m_Winlen;							//������ز���

	ManagedData		m_ColorData;						//��ɫ������

	ManagedData		m_FiltData;

	bool			m_bIsOK;							//�Ƿ��Ѿ�������ģ��
	//ʵ�ֽӿں���
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

