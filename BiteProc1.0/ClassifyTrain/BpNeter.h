#pragma once
//�ṩBP���磬ѵ��������
#include "Matrix.h"

//***************************************************************************
/////////////////////////////////////////////////////////////////////////////
//	Levenberg-Marquart		---->	��һ��ǰ������ʼ��				   //
/////////////////////////////////////////////////////////////////////////////
__declspec (dllimport) void LMForwardCalculateInit( int nInputLayerNumber,
													int nHideLayerNumber,
													int nOutputLayerNumber,
													CMatrix &matrixDemoDataInput,
													CMatrix &matrixInputLayerValue,
													CMatrix &matrixInputToHideWeightValue,
													CMatrix &matrixHideLayerValveValue,
													CMatrix &matrixHideToOutputWeightValue,
													CMatrix &matrixOutputLayerValveValue
													);

/////////////////////////////////////////////////////////////////////////////
//	Levenberg-Marquart		---->		ǰ�����						   //
/////////////////////////////////////////////////////////////////////////////
__declspec(dllimport) void LMForwardCalculate ( int nInputLayerNumber,
												int nHideLayerNumber,
												int nOutputLayerNumber,
												bool bSimulateDataFlag,
												int nComboFunc,
												CMatrix &matrixDemoDataInput,
												CMatrix &matrixInputLayerValue,
												CMatrix &matrixInputToHideWeightValue,
												CMatrix &matrixHideLayerValveValue,
												CMatrix &matrixHideLayerOutput,
												CMatrix &matrixHideToOutputWeightValue,
												CMatrix &matrixOutputLayerOutput,
												CMatrix &matrixOutputLayerValveValue
											   );

/////////////////////////////////////////////////////////////////////////////
//	Levenberg-Marquart		---->		��������						   //
/////////////////////////////////////////////////////////////////////////////
__declspec(dllimport) bool LMDemoDataTrainRepeat (	int nInputLayerNumber,
													int nHideLayerNumber,
													int nOutputLayerNumber,
													bool bSimulateDataFlag,
													int nComboFunc,
													double nSystemErrorOld,
													double nSystemErrorNew,
													double nSystemErrorLevel,
													double nSystemError,
													double nStep,
													UINT nMaxTrainTimes,
													UINT nTrainTimes,
													double& nCurSystemError,
													int&    nCurTrainTimes,
													CMatrix &matrixDemoDataInput,
													CMatrix &matrixInputLayerValue,
													CMatrix &matrixInputToHideWeightValue,
													CMatrix &matrixHideLayerValveValue,
													CMatrix &matrixHideLayerOutput,
													CMatrix &matrixHideToOutputWeightValue,
													CMatrix &matrixOutputLayerOutput,
													CMatrix &matrixOutputLayerValveValue
												 );

/////////////////////////////////////////////////////////////////////////////
//	Back propagation		---->		ǰ�����(Only for Training)		   //
/////////////////////////////////////////////////////////////////////////////
__declspec(dllimport) void BPForwardCalculate ( int nInputLayerNumber,
												int nHideLayerNumber,
												int nOutputLayerNumber,
												bool bSimulateDataFlag,
												int nComboFunc,
												CMatrix &matrixDemoDataInput,
												CMatrix &matrixInputLayerValue,
												CMatrix &matrixInputToHideWeightValue,
												CMatrix &matrixHideLayerValveValue,
												CMatrix &matrixHideLayerOutput,
												CMatrix &matrixHideToOutputWeightValue,
												CMatrix &matrixOutputLayerOutput,
												CMatrix &matrixOutputLayerValveValue,
												CMatrix &cMatrixExHideLayerValveValue,
												CMatrix &cMatrixExOutputLayerValveValue
											   );

/////////////////////////////////////////////////////////////////////////////
//	Back propagation		---->		ǰ�����(Only for Simulating)	   //
/////////////////////////////////////////////////////////////////////////////
__declspec(dllimport) void BPForwardCalculate2( int nInputLayerNumber,
												int nHideLayerNumber,
												int nOutputLayerNumber,
												bool bSimulateDataFlag,
												int nComboFunc,
												CMatrix &matrixDemoDataInput,
												CMatrix &matrixInputLayerValue,
												CMatrix &matrixInputToHideWeightValue,
												CMatrix &matrixHideLayerValveValue,
												CMatrix &matrixHideLayerOutput,
												CMatrix &matrixHideToOutputWeightValue,
												CMatrix &matrixOutputLayerOutput,
												CMatrix &matrixOutputLayerValveValue
											   );

/////////////////////////////////////////////////////////////////////////////
//	Back propagation		---->		��������						   //
/////////////////////////////////////////////////////////////////////////////
__declspec(dllimport) bool BPDemoDataTrainRepeat (	int nInputLayerNumber,
													int nHideLayerNumber,
													int nOutputLayerNumber,
													bool bSimulateDataFlag,
													int nComboFunc,
													double nSystemErrorOld,
													double nSystemErrorNew,
													double nSystemErrorLevel,
													double nSystemError,
													double nStep,
													UINT nMaxTrainTimes,
													UINT nTrainTimes,
													double& nCurSystemError,
													int&    nCurTrainTimes,
													CMatrix &matrixDemoDataInput,
													CMatrix &matrixInputLayerValue,
													CMatrix &matrixInputToHideWeightValue,
													CMatrix &matrixHideLayerValveValue,
													CMatrix &matrixHideLayerOutput,
													CMatrix &matrixHideToOutputWeightValue,
													CMatrix &matrixOutputLayerOutput,
													CMatrix &matrixOutputLayerValveValue
												 );

__declspec(dllimport)  bool FANNTrainRepeat( int nInputLayerNumber,
											 int nHideLayerNumber,
	                                         int nOutputLayerNumber,
	                                         bool bSimulateDataFlag,
	                                         int nHideFunc,
	                                         int nOutFunc,
	                                         int nMaxTrainTime,
	                                         double nSystemError,
	                                         int& nCurTime,
	                                         double& nCurError, 
	                                         char *nDatapath,		/*train data path*/
	                                         char *nDestPath			/*for the net path*/
	                                        );
__declspec(dllimport) void StopTrain(const bool b);

__declspec(dllimport) bool CreateNetWork(char* path);

__declspec(dllimport) bool DelteNetWork();

__declspec(dllimport) bool SimFannNet(float *pd ,const  int num , double* outres);
//***************************************************************************


class BpNeter
{
public:
	BpNeter(void);
	BpNeter(UINT input , UINT Hide , UINT Output , int Arith , int Spirfunc);

	~BpNeter(void);
	bool	SimulateData(CMatrix &matrix);		//���ݷ���
	bool	TrainingDemo();						//ѵ��������
	//����ѵ��������
	bool	SetDataFromBuf(const double* puf , int length);
	bool	SetDataFromFile(CString path);		//��ѵ�������ļ��м���

	//����������Ȩֵ , �Լ�������Ĳ���
	bool	LoadWeightFormFile(CString path);

	//�����������
	bool	SavaDatatoFile(CString path);		//����ѵ������
	bool	SaveWeighttoFile(CString path);		//��������Ȩֵ
	bool	SaveRestoFile(CString path);		//����ѵ�����

	//������������
	bool	SetSimDataFormFile(CString path);
	bool	SetSimDataFormbuf(const double* pbuf , const int length);
	bool	GetSimResult(CMatrix& matrix);

	bool	ChangeTrainState(bool flag);		//��ͣ������ѵ���߳�
	bool	GetTrainState(double& err , int& time);
	void	ChangeTrain(const bool b);
	void	SetTrainParm(const double syserr , int maxnum);

	bool	m_bSimulateDataFlag;				//�������ѵ����־
	//ѵ������
	UINT	m_nInputLayerNumber;
	UINT	m_nHideLayerNumber;
	UINT	m_nOutputLayerNumber;
	double	m_nSystemError;						//��ǰϵͳ���
	UINT	m_nTrainTimes;						//ʵ��ѵ������
	UINT	m_nMaxTrainTimes;					//���ѵ������
	double	m_nSystemErrorLevel;
	double	m_nStep;							//��ʼ����
	/*0---->LM
	  1---->BP
	  2---->FANN       */
	int		m_nTrainArithmetic;	
	/*inspire function
	0----->sigmoid
	1----->tanh
	2----->tansig
	*/
	int		m_nInspireFunc;
	void ForwardCalculateInit();

	// ����ѵ��
	bool DemoDataTrainRepeat();
private:

	bool SaveConstantStringToFile(CString& strFileName,CString& strConstantData);

	HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA);
	HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW);
	// ���ڴ洢������������ݵľ���
	CMatrix		m_matrixDemoDataInput;

	// ��������������ֵ����
	CMatrix		m_matrixInputLayerValue;

	// ����㵽�������Ȩֵ����
	CMatrix		m_matrixInputToHideWeightValue;

	// ������ķ�ֵ����
	CMatrix		m_matrixHideLayerValveValue;

	// �����㵽������Ȩֵ����
	CMatrix		m_matrixHideToOutputWeightValue;

	// �����ķ�ֵ����
	CMatrix		m_matrixOutputLayerValveValue;

	// ����������ֵ�ľ���
	CMatrix		m_matrixHideLayerOutput;

	// ���������ֵ�ľ���
	CMatrix		m_matrixOutputLayerOutput;

	// ��������ʱ����������������
	CMatrix		m_matrixSimuNetwork;

	// ������ŷ������ݽ���ľ���
	CMatrix		m_matrixSimuResult;

	// �����µ��̵߳��߳̾��
	// ϵͳ���

	double		m_SystemErrorNew;
	double		m_SystemErrorOld;

	double		m_nCurSystemError;
	int			m_nCurTrainTimes;

	//just for FANN param.
	CString		m_nFANNDataPath;
	CString		m_nFANNNetPath;
};

