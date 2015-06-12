#pragma once
//*ʵ�ֶ��㷨��ز����ļ��ļ��أ�
//�Լ����Ƿ����ѵ�����У���ѵ����������ȷ����
/*
author��chi
time��2015-1-18
*/
#include <iostream>
#include <map>
#include <vector>
#include<afx.h>

using namespace std;
class CAlgParamConfig
{
public:
	CAlgParamConfig(void);
	CAlgParamConfig(CString path);
	~CAlgParamConfig(void);
	bool	OpenParamFile(CString path);
	bool	LoadParam();
	bool	SaveParam();
	bool	CloseParmFile();
	bool	IsOpen();
	//bite threld param
	void	SetBiteState(const bool s);						//�Ƿ����ѵ��
	bool	GetBitestate();
	void	SetBiteThorldModel(const CString fpath);
	CString	GetBiteThroldModel();							//�������ļ������ڣ�����Ϊ��
	void	SetBiteTrain(const int t ,const int count);		//����ҧ��ʱ�����Լ��ڸ�ʱ���µ�ҧ������
	void	GetBiteTrain(int& t , int& count);
	void	SetBiteDataPath(CString path);					//ҧ�������ļ�·��
	CString GetBiteDataPath();								
	void	SetBiteModulePath(CString path);				//����dll·��
	CString	GetBiteModulePath();							//����ҧ����ֵģ��dll��·��

	//���ҡ�ͬʱ����ģ�� 
	void	SetClassifyState(const bool s);					//�Ƿ�������ҡ�ͬʱģ�ͷ���ѵ��
	bool	GetClassifystate();
	void	SetClassifyModel(const CString fpath);			//ģ�ͱ������·��
	CString	GetClassifyModel();								//�������ļ������ڣ�����Ϊ��
	void	SetClassifyTain(const vector<int> t);			//��ͬҧ����ʽ��ʽѵ��ʱ������λs
	void	GetClassifyTrain(vector<int>& t);
	void	ReTrainClassify(bool s);						//�Ƿ�����ѵ��
	bool	IsRetrainClassify();
	void	SetClassifyLData(CString path);					//��ҧ����
	CString	GetClassifyLData();
	void	SetClassifyBData(CString path);					//ͬʱҧ����
	CString GetClassifyBData();
	void	SetClassifyRData(CString path);					//��ҧ����
	CString GetClassifyRData();
	void	SetClassifyModule(CString path);				//��ȡģ��dll��ַ
	CString GetClassifyModule();

	

	//heavy or light bite param.
	void	SetHLState(const bool s);						//�Ƿ�ѵ��������ֵ
	bool	GetHLState();
	void	SetHLModel(const CString fpath);				//����ģ��·��
	CString	GetHLModel();
	void	SetHLTrainTime(const vector<int> t);
	void	GetHLTrainTime(vector<int>& t);
	bool	GetHLReTrain();
	void	SetHLReTrain(bool b);							//���¼������ݣ����ɼ���ǰ���ǵ�������
	void	SetHLModule(CString path);						//����ģ��·��
	CString	GetHLModule();									//��ȡģ��dll

	void	SetLLData(CString path);						//��������
	CString	GetLLData();
	void	SetLHData(CString path);						//��������
	CString GetLHData();
	void	SetBLData(CString path);						//ͬʱ������
	CString GetBLData();
	void	SetBHData(CString path);						//ͬʱ������
	CString GetBHData();
	void	SetRLData(CString path);						//��������
	CString	GetRLData();									
	void	SetRHData(CString data);						//��������
	CString	GetRHData();
	

	//��ҧ������ҧģ��
	void	SetTLTrainState(bool b);						//����ҧ�ͳ�ҧ�Ƿ����ѵ��
	bool	GetTLTrainState();								//
	void	SetTLTrainTime(const vector<int> t);			//ҧ����ʱ��,(���ҡ�ͬʱ)
	void	GetTLTrainTime(vector<int>& t);
	void	SetTLLeftParamPath(const CString str);			//������
	CString GetTLLeftParamPath();
	void	SetTLBothParamPath(const CString str);			//ͬʱ����·��
	CString	GetTLBothParamPath();
	void	SetTLRightParamPath(const CString path);		//�Ҳ����
	CString GetTLRightParamPath();

	void	SetTLLeftData(CString path);					//�������ֵ��ҧ����
	CString	GetTLLeftData();
	void	SetTLBothData(CString path);					//����ͬʱ����
	CString	GetTLBothData();
	void	SetTLRightData(CString path);					//�Ҳ�����
	CString	GetTLRightData();

	void	SetTLLeftModel(CString path);					//���ģ��
	CString	GetTLLeftModel();
	void	SetTLBothModel(CString path);					//ͬʱģ��
	CString	GetTLBothModel();
	void	SetTLRightModel(CString path);					//�Ҳ�ģ��
	CString	GetTLRightModel();

	void	SetTLModule(CString path);						//����ҧģ��dll
	CString	GetTLModule();									

	void	SetTLReTrain(bool b);							//����ѵ��
	bool	GetTLReTrain();

	//for basic  param.
	void	SetWindowLength(int l);
	int		GetWindowLength();
	void	SetOverLength(int l);
	int		GetOverLength();
	void	GetLRThrolds(double& l , double& r);			//���ҷֽ�ֵ
	void	SetLRThrolds(double l , double r);

private:

	bool	IsSpace(char c);
	bool	IsCommentChar(char c);
	void	TrimString(string& str);
	bool	AnalyseLine(const string& line , string& key , string& value);

private:
	CStdioFile			m_nFile;
	map<string,string>  m_Config;
	BOOL				m_bIsopen;
};

