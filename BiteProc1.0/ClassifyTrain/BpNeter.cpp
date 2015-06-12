#include "StdAfx.h"
#include "BpNeter.h"
#include "Stringoper.h"

#ifdef DEBUG
#pragma comment(lib ,"..\\Debug\\MatrixBase.lib")
#pragma comment(lib,"..\\Debug\\Arithmetic.lib")
#else
#pragma comment(lib ,"..\\Release\\MatrixBase.lib")
#pragma comment(lib,"..\\Release\\Arithmetic.lib")
#endif // DEBUG



// Define the max system error

const double	MAX_SYSTEM_ERROR	=	1000;

// 最大调整值
const double	MAX_ADJUST_VALUE	=	10e6;

// 系统误差
double	SYSTEM_ERROR_LEVEL	=	0.01;

// the max loop times
unsigned int LOOP_MAX_TIMES	=	100000;

// 步长初始值
const double	INIT_STEP	=	0.25;
BpNeter::BpNeter(void)
{
	m_nInputLayerNumber = 0;
	m_nHideLayerNumber	= 0;
	m_nOutputLayerNumber= 0;
	m_nTrainArithmetic	= -1;
	m_nInspireFunc		= -1;
	m_nSystemError		= 0.0;
	m_nCurSystemError	= 0.0;
	m_nCurTrainTimes	= 0;
	m_nSystemErrorLevel = SYSTEM_ERROR_LEVEL;
	m_nMaxTrainTimes	= LOOP_MAX_TIMES;
	m_nStep				= INIT_STEP;
	m_nTrainTimes		= 0;
	m_bSimulateDataFlag = false;

	m_SystemErrorNew	= MAX_SYSTEM_ERROR;
	m_SystemErrorOld	= MAX_SYSTEM_ERROR;

	m_nFANNDataPath = _T("~FANNtmpData__.txt");
	m_nFANNNetPath = _T("~FANNtmpNet__.txt");

}
BpNeter::BpNeter(UINT input , UINT Hide , UINT Output , int Arith , int Spirfunc):
    m_nInputLayerNumber(input),
	m_nHideLayerNumber(Hide),
	m_nOutputLayerNumber(Output),
	m_nTrainArithmetic(Arith),
	m_nInspireFunc(Spirfunc),
	m_nCurSystemError(0.0),
	m_nCurTrainTimes(0)
{
	m_nSystemError		= 0.0;
	m_nSystemErrorLevel = SYSTEM_ERROR_LEVEL;
	m_nMaxTrainTimes	= LOOP_MAX_TIMES;
	m_nStep				= INIT_STEP;
	m_bSimulateDataFlag = false;
	m_nTrainTimes		= 0;
	m_SystemErrorNew	= MAX_SYSTEM_ERROR;
	m_SystemErrorOld	= MAX_SYSTEM_ERROR;

	m_nFANNDataPath = _T("~FANNtmpData__.txt");
	m_nFANNNetPath = _T("~FANNtmpNet__.txt");
}

BpNeter::~BpNeter(void)
{
	m_matrixDemoDataInput.InitializeZero ();

	m_matrixInputLayerValue.InitializeZero ();

	m_matrixInputToHideWeightValue.InitializeZero ();
	m_matrixHideLayerValveValue.InitializeZero ();
	m_matrixHideLayerOutput.InitializeZero ();

	m_matrixHideToOutputWeightValue.InitializeZero ();
	m_matrixOutputLayerValveValue.InitializeZero ();	

	m_matrixOutputLayerOutput.InitializeZero ();

	if(m_nTrainTimes==2)
	{
		DelteNetWork();
	}
}

bool	BpNeter::SetDataFromBuf(const double* puf , int length)
{
	if(m_nInputLayerNumber<=0)
		return false;
	unsigned int row = length/m_nInputLayerNumber;
	m_matrixDemoDataInput.SetMatrixRowAndCol(row , m_nInputLayerNumber);
	return m_matrixDemoDataInput.LoadDataFromBuf(puf , length);
}

bool	BpNeter::SetDataFromFile(CString path)
{
	if(path.IsEmpty())
		return false;
	if(m_nTrainArithmetic!=2)
		return m_matrixDemoDataInput.LoadDataFromFile(path);
	else 
	{
		m_nFANNDataPath = path;
		return true;
	}
}

bool	BpNeter::LoadWeightFormFile(CString path)
{
	if(path.GetLength()<=0)
		return false;
	if(m_nTrainArithmetic==2)
	{
		Stringoper oper;
		char tmp[512];
		oper.CStringtoChars(path , tmp);
		return CreateNetWork(tmp);
	}
	else
	return m_matrixSimuNetwork.LoadDataFromFileSpecial(path ,m_matrixInputToHideWeightValue
		,m_matrixHideLayerValveValue , m_matrixHideToOutputWeightValue , m_matrixOutputLayerValveValue , m_nInputLayerNumber
		,m_nHideLayerNumber , m_nOutputLayerNumber , m_nTrainArithmetic , m_nInspireFunc);
}

bool	BpNeter::SimulateData(CMatrix &outmatrix)
{
	if(m_nTrainArithmetic==2)		//only for fann.
	{
		CMatrix tmp(m_matrixDemoDataInput.GetMatrixRowNumber(),1);
		double pd = 0;
		float *pdata = new float[m_matrixDemoDataInput.GetMatrixColNumber()];
		for(int i=0;i<m_matrixDemoDataInput.GetMatrixRowNumber();i++)
		{
			for (int j=0;j<m_matrixDemoDataInput.GetMatrixColNumber();j++)
			{
				pdata[j] =(float)m_matrixDemoDataInput.m_pTMatrix[i][j];
			}
			if(!SimFannNet(pdata , m_matrixDemoDataInput.GetMatrixColNumber(),&pd))
			{
				delete pdata;
				return false;
			}
			tmp.m_pTMatrix[i][0] = pd;
		}
		outmatrix.CopyMatrix(tmp.Transpose());
		m_matrixDemoDataInput.InitializeZero ();
		return true;
	}
	if(m_matrixDemoDataInput.GetMatrixColNumber()!=m_nInputLayerNumber)
	{
		TRACE("sim failed\n");
		return false;
	}
	switch(m_nTrainArithmetic)
	{
	case 0: // LM Arithmetic Selected
		LMForwardCalculate( m_nInputLayerNumber,
			m_nHideLayerNumber,
			m_nOutputLayerNumber,
			m_bSimulateDataFlag,
			m_nInspireFunc,
			m_matrixDemoDataInput,
			m_matrixInputLayerValue,
			m_matrixInputToHideWeightValue,
			m_matrixHideLayerValveValue,
			m_matrixHideLayerOutput,
			m_matrixHideToOutputWeightValue,
			m_matrixOutputLayerOutput,
			m_matrixOutputLayerValveValue
			);

		break;

	case 1: // BP Arithmetic Selected
		BPForwardCalculate2(m_nInputLayerNumber,
			m_nHideLayerNumber,
			m_nOutputLayerNumber,
			m_bSimulateDataFlag,
			m_nInspireFunc,
			m_matrixDemoDataInput,
			m_matrixInputLayerValue,
			m_matrixInputToHideWeightValue,
			m_matrixHideLayerValveValue,
			m_matrixHideLayerOutput,
			m_matrixHideToOutputWeightValue,
			m_matrixOutputLayerOutput,
			m_matrixOutputLayerValveValue
			);

		break;
	default: // No Arithmetic Selected
		return false;
	}

	outmatrix.CopyMatrix(m_matrixOutputLayerOutput.Transpose());
	
	m_matrixDemoDataInput.InitializeZero ();
	return true;
}

bool	BpNeter::TrainingDemo()
{
	m_bSimulateDataFlag = false;
	if(m_nInputLayerNumber<=0)
	{
		TRACE("Input layer is illeagle\n");
		return false;
	}
	if(m_nHideLayerNumber<=0)
	{
		TRACE("Hiden layer is illeagle\n");
		return false;
	}
	if(m_nOutputLayerNumber<=0)
	{
		TRACE("Output layer is illeagle\n");
		return false;
	}
	if(m_nTrainArithmetic==-1)
	{
		TRACE("Invalid Arithmetic\n");
		return false;
	}
	if(m_nInspireFunc==-1)
	{
		TRACE("Invalid Inspire function\n");
		return false;
	}
	if(m_nTrainArithmetic!=2)
	ForwardCalculateInit();	
	bool	bSuccess = DemoDataTrainRepeat();

	return bSuccess;
}

bool	BpNeter::SaveWeighttoFile(CString path)				//record the train result.
{
	if(path.IsEmpty())
		return false;
	if(m_nTrainArithmetic==2)		//support FANN File Format.
	{
		CFileFind finder;
		if(finder.FindFile(m_nFANNNetPath))
		{
			if(!CopyFile(m_nFANNNetPath,path , FALSE))
			{
				return false;
			}
			if(!MoveFile(m_nFANNNetPath , path))
			{
				return false;
			}
		}
		return true;
	}
	CString strDemoInputLayerNumber = _T("## 样本的输入层的数目: ##\r\n");
	SaveConstantStringToFile(path, strDemoInputLayerNumber);

	CString strInputLayerNumber;
	strInputLayerNumber.Format (_T("%u"), m_nInputLayerNumber);
	SaveConstantStringToFile(path, strInputLayerNumber);

	CString strDemoHideLayerNumber = _T("\r\n## 样本的隐含层的数目: ##\r\n");
	SaveConstantStringToFile(path, strDemoHideLayerNumber);

	CString strHideLayerNumber;
	strHideLayerNumber.Format (_T("%u"), m_nHideLayerNumber);
	SaveConstantStringToFile(path, strHideLayerNumber);

	CString strDemoOutputLayerNumber = _T("\r\n## 样本的输出层的数目: ##\r\n");
	SaveConstantStringToFile(path, strDemoOutputLayerNumber);

	CString strOutputLayerNumber;
	strOutputLayerNumber.Format (_T("%u"), m_nOutputLayerNumber);
	SaveConstantStringToFile(path, strOutputLayerNumber);

	CString strSelectAlgorithm = _T("\r\n## 训练网络所采用的神经网络算法: ##\r\n");
	SaveConstantStringToFile(path, strSelectAlgorithm);

	CString strComboArithmetic;
	strComboArithmetic.Format (_T("%d"), m_nTrainArithmetic);
	SaveConstantStringToFile(path, strComboArithmetic);

	CString strSelectFunction = _T("\r\n## 网络中使用的函数的索引值: ##\r\n");
	SaveConstantStringToFile(path, strSelectFunction);

	CString strComboFunc;
	strComboFunc.Format (_T("%d"), m_nInspireFunc);
	SaveConstantStringToFile(path, strComboFunc);


	CString strSeparator = _T("\r\n## 输入层到隐含层的权值矩阵 ##\r\n");
	SaveConstantStringToFile(path, strSeparator);

	// 存储模拟过的输入到隐含层的权值矩阵
	m_matrixInputToHideWeightValue.SaveDataToFile (path);

	strSeparator = _T("\r\n## 隐含层的阀值矩阵 ##\r\n");
	SaveConstantStringToFile(path,strSeparator);

	// 存储模拟过的隐含层的阀值矩阵
	m_matrixHideLayerValveValue.SaveDataToFile (path);

	strSeparator = _T("\r\n## 隐含层到输出层的权值矩阵 ##\r\n");
	SaveConstantStringToFile(path,strSeparator);

	// 存储模拟过的隐含层到输出层的权值矩阵
	m_matrixHideToOutputWeightValue.SaveDataToFile (path);

	strSeparator = _T("\r\n## 输出层的阀值矩阵 ##\r\n");
	SaveConstantStringToFile(path,strSeparator);

	// 存储模拟过的输出层的阀值矩阵
	m_matrixOutputLayerValveValue.SaveDataToFile (path);
	return true;
}

bool	BpNeter::SavaDatatoFile(CString path)				//save the train data to file.
{
	//add code here
	return true;
}

bool	BpNeter::SaveRestoFile(CString path)				//save sim result to file.
{
	//add code here
	return true;
}

bool	BpNeter::SetSimDataFormbuf(const double* pbuf , const int length)
{
	return SetDataFromBuf(pbuf , length);	
}

bool	BpNeter::SetSimDataFormFile(CString path)
{
	//add code here
	return true;
}

bool	BpNeter::GetSimResult(CMatrix& matrix)					//获得仿真结果
{
	if(m_matrixSimuResult.GetMatrixColNumber()>0&&m_matrixSimuResult.GetMatrixRowNumber()>0)
	{
		matrix = m_matrixSimuResult;
		return true;
	}
	return false;
}

void	BpNeter::ForwardCalculateInit()
{
	LMForwardCalculateInit( m_nInputLayerNumber,
		m_nHideLayerNumber,
		m_nOutputLayerNumber,
		m_matrixDemoDataInput,
		m_matrixInputLayerValue,
		m_matrixInputToHideWeightValue,
		m_matrixHideLayerValveValue,
		m_matrixHideToOutputWeightValue,
		m_matrixOutputLayerValveValue
		);	
}

bool	BpNeter::DemoDataTrainRepeat()
{
	bool	bReturn = false;
	char p1[512];
	char p2[512];
	Stringoper oper;
	switch( m_nTrainArithmetic)
	{
	case 0: // LM Arithmetic Selected
		bReturn = LMDemoDataTrainRepeat (	m_nInputLayerNumber,
			m_nHideLayerNumber,
			m_nOutputLayerNumber,
			m_bSimulateDataFlag,
			m_nInspireFunc,
			m_SystemErrorOld,
			m_SystemErrorNew,
			m_nSystemErrorLevel,
			m_nSystemError,
			m_nStep,
			m_nMaxTrainTimes,
			m_nTrainTimes,
			m_nCurSystemError,
			m_nCurTrainTimes,
			m_matrixDemoDataInput,
			m_matrixInputLayerValue,
			m_matrixInputToHideWeightValue,
			m_matrixHideLayerValveValue,
			m_matrixHideLayerOutput,
			m_matrixHideToOutputWeightValue,
			m_matrixOutputLayerOutput,
			m_matrixOutputLayerValveValue
			);

		break;

	case 1: // BP Arithmetic Selected
		bReturn = BPDemoDataTrainRepeat (	m_nInputLayerNumber,
			m_nHideLayerNumber,
			m_nOutputLayerNumber,
			m_bSimulateDataFlag,
			m_nInspireFunc,
			m_SystemErrorOld,
			m_SystemErrorNew,
			m_nSystemErrorLevel,
			m_nSystemError,
			m_nStep,
			m_nMaxTrainTimes,
			m_nTrainTimes,
			m_nCurSystemError,
			m_nCurTrainTimes,
			m_matrixDemoDataInput,
			m_matrixInputLayerValue,
			m_matrixInputToHideWeightValue,
			m_matrixHideLayerValveValue,
			m_matrixHideLayerOutput,
			m_matrixHideToOutputWeightValue,
			m_matrixOutputLayerOutput,
			m_matrixOutputLayerValveValue
			);

		break;
	case 2:
		oper.CStringtoChars(m_nFANNDataPath , p1);
		oper.CStringtoChars(m_nFANNNetPath , p2);
		bReturn = FANNTrainRepeat(m_nInputLayerNumber,
								m_nHideLayerNumber,
								m_nOutputLayerNumber,
								m_bSimulateDataFlag,
								m_nInspireFunc,
								m_nInspireFunc,
								m_nMaxTrainTimes,
								m_nSystemError,
								m_nCurTrainTimes,
								m_nCurSystemError,
								p1,
								p2
								);
		break;
	default: // No Arithmetic Selected
		return bReturn;

	}

	return bReturn;
}

void	BpNeter::ChangeTrain(const bool b)
{
	StopTrain(b);
}

void	BpNeter::SetTrainParm(const double syserr , int maxnum)
{
	m_nSystemErrorLevel = syserr;
	m_nMaxTrainTimes = maxnum;
}

bool	BpNeter::ChangeTrainState(bool flag)		//false is stop and the true is start.
{
	//add code here
	return true;
}

bool	BpNeter::GetTrainState(double& err , int& time)
{
	if(m_bSimulateDataFlag)
		return false;
	ChangeTrainState(false);
	err = m_nCurSystemError;
	time = m_nCurTrainTimes;
	ChangeTrainState(true);
	return true;
}

bool BpNeter::SaveConstantStringToFile(CString& strFileName,CString& strConstantData)
{
	CFile dataFile;

	if(!dataFile.Open (strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		dataFile.Close ();
		return FALSE;
	}

	dataFile.SeekToEnd ();

	// Write data into the file

	Stringoper oper;
	string str = oper.tostring(strConstantData);
	
	dataFile.Write(str.c_str(), str.length()*sizeof(char));
	
	dataFile.Close ();

	return TRUE;
}

HRESULT __fastcall BpNeter::UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA)
{

	ULONG cbAnsi, cCharacters;
	DWORD dwError;

	// If input is null then just return the same.
	if (pszW == NULL)
	{
		*ppszA = NULL;
		return NOERROR;
	}

	cCharacters = wcslen(pszW)+1;
	// Determine number of bytes to be allocated for ANSI string. An
	// ANSI string can have at most 2 bytes per character (for Double
	// Byte Character Strings.)
	cbAnsi = cCharacters*2;

	// Use of the OLE allocator is not required because the resultant
	// ANSI  string will never be passed to another COM component. You
	// can use your own allocator.
	*ppszA = (LPSTR) CoTaskMemAlloc(cbAnsi);
	if (NULL == *ppszA)
		return E_OUTOFMEMORY;

	// Convert to ANSI.
	if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,
		cbAnsi, NULL, NULL))
	{
		dwError = GetLastError();
		CoTaskMemFree(*ppszA);
		*ppszA = NULL;
		return HRESULT_FROM_WIN32(dwError);
	}
	return NOERROR;
}
HRESULT __fastcall BpNeter::AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW)
{

	ULONG cCharacters;
	DWORD dwError;

	// If input is null then just return the same.
	if (NULL == pszA)
	{
		*ppszW = NULL;
		return NOERROR;
	}

	// Determine number of wide characters to be allocated for the
	// Unicode string.
	cCharacters =  strlen(pszA)+1;

	// Use of the OLE allocator is required if the resultant Unicode
	// string will be passed to another COM component and if that
	// component will free it. Otherwise you can use your own allocator.
	*ppszW = (LPOLESTR) CoTaskMemAlloc(cCharacters*2);
	if (NULL == *ppszW)
		return E_OUTOFMEMORY;

	// Covert to Unicode.
	if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
		*ppszW, cCharacters))
	{
		dwError = GetLastError();
		CoTaskMemFree(*ppszW);
		*ppszW = NULL;
		return HRESULT_FROM_WIN32(dwError);
	}

	return NOERROR;
}