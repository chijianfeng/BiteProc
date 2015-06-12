#pragma once
//提供BP网络，训练及仿真
#include "Matrix.h"

//***************************************************************************
/////////////////////////////////////////////////////////////////////////////
//	Levenberg-Marquart		---->	第一次前向计算初始化				   //
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
//	Levenberg-Marquart		---->		前向计算						   //
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
//	Levenberg-Marquart		---->		反馈计算						   //
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
//	Back propagation		---->		前向计算(Only for Training)		   //
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
//	Back propagation		---->		前向计算(Only for Simulating)	   //
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
//	Back propagation		---->		反馈计算						   //
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
	bool	SimulateData(CMatrix &matrix);		//数据仿真
	bool	TrainingDemo();						//训练神经网络
	//设置训练的数据
	bool	SetDataFromBuf(const double* puf , int length);
	bool	SetDataFromFile(CString path);		//从训练数据文件中加载

	//加载神经网络权值 , 以及神经网络的参数
	bool	LoadWeightFormFile(CString path);

	//保存相关数据
	bool	SavaDatatoFile(CString path);		//保存训练数据
	bool	SaveWeighttoFile(CString path);		//保存网络权值
	bool	SaveRestoFile(CString path);		//保存训练结果

	//仿真数据设置
	bool	SetSimDataFormFile(CString path);
	bool	SetSimDataFormbuf(const double* pbuf , const int length);
	bool	GetSimResult(CMatrix& matrix);

	bool	ChangeTrainState(bool flag);		//暂停或其他训练线程
	bool	GetTrainState(double& err , int& time);
	void	ChangeTrain(const bool b);
	void	SetTrainParm(const double syserr , int maxnum);

	bool	m_bSimulateDataFlag;				//仿真或是训练标志
	//训练参数
	UINT	m_nInputLayerNumber;
	UINT	m_nHideLayerNumber;
	UINT	m_nOutputLayerNumber;
	double	m_nSystemError;						//当前系统误差
	UINT	m_nTrainTimes;						//实际训练次数
	UINT	m_nMaxTrainTimes;					//最大训练次数
	double	m_nSystemErrorLevel;
	double	m_nStep;							//初始步长
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

	// 反复训练
	bool DemoDataTrainRepeat();
private:

	bool SaveConstantStringToFile(CString& strFileName,CString& strConstantData);

	HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA);
	HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW);
	// 用于存储输入的样本数据的矩阵
	CMatrix		m_matrixDemoDataInput;

	// 样本的输入层的数值矩阵
	CMatrix		m_matrixInputLayerValue;

	// 输入层到隐含层的权值矩阵
	CMatrix		m_matrixInputToHideWeightValue;

	// 隐含层的阀值矩阵
	CMatrix		m_matrixHideLayerValveValue;

	// 隐含层到输出层的权值矩阵
	CMatrix		m_matrixHideToOutputWeightValue;

	// 输出层的阀值矩阵
	CMatrix		m_matrixOutputLayerValveValue;

	// 隐含层的输出值的矩阵
	CMatrix		m_matrixHideLayerOutput;

	// 输出层的输出值的矩阵
	CMatrix		m_matrixOutputLayerOutput;

	// 用来仿真时输入的网络参数矩阵
	CMatrix		m_matrixSimuNetwork;

	// 用来存放仿真数据结果的矩阵
	CMatrix		m_matrixSimuResult;

	// 创建新的线程的线程句柄
	// 系统误差

	double		m_SystemErrorNew;
	double		m_SystemErrorOld;

	double		m_nCurSystemError;
	int			m_nCurTrainTimes;

	//just for FANN param.
	CString		m_nFANNDataPath;
	CString		m_nFANNNetPath;
};

