#include "stdafx.h"
#include "NetWorker.h"
#include "Stringoper.h"


CNetWorker::CNetWorker()
{
	m_Traintime = 1000;
	m_Error = 0.01;
}

CNetWorker::CNetWorker(const char* pathmodel)
{
	Stringoper oper;
	char path[MAX_PATH];
	strcpy_s(path, pathmodel);
	m_strModelPath = oper.chartocstring(path);
	if (m_strModelPath.GetLength() > 0)
		m_BpNet.LoadWeightFormFile(m_strModelPath);
	m_Traintime = 1000;
	m_Error = 0.01;
}

CNetWorker::~CNetWorker()
{
}

void CNetWorker::SetModelPath(const char* pathmodel)
{
	Stringoper oper;
	char path[MAX_PATH];
	strcpy_s(path, pathmodel);
	m_strModelPath = oper.chartocstring(path);
	if (m_strModelPath.GetLength() > 0)
		m_BpNet.LoadWeightFormFile(m_strModelPath);
}

bool CNetWorker::LoadTrainData(const char* pPath)
{
	if (!pPath)
		return false;
	m_BpNet.ChangeTrain(false);					//设置训练
	char path[MAX_PATH];
	strcpy_s(path, pPath);
	Stringoper oper;
	CString datapath = oper.chartocstring(path);
	return m_BpNet.SetDataFromFile(datapath);			//从文件中加载数据
}

bool CNetWorker::Train(char* path)
{
	/*从文件中加载训练数据*/
	Stringoper oper;
	CString traindatapath = oper.chartocstring(path);
	if (!m_BpNet.SetDataFromFile(traindatapath))
		return false;
	m_BpNet.SetTrainParm(m_Error, m_Traintime);
	m_BpNet.ChangeTrain(false);
	m_BpNet.m_bSimulateDataFlag = false;
	return m_BpNet.TrainingDemo();
}

bool CNetWorker::SaveTrainModel(const char* pPath)
{
	Stringoper oper;
	char path[MAX_PATH];
	strcpy_s(path, pPath);
	CString modelpath = oper.chartocstring(path);

	CFile file;
	file.Open(modelpath, CFile::modeWrite|CFile::modeCreate);		//清空原来文件
	file.SetLength(0);
	file.Close();

	if (m_BpNet.m_nTrainArithmetic == 2)						//FANN 模型，保留附加参数
	{
		int index = modelpath.ReverseFind('.');
		if (index != -1)
		{
			modelpath.Insert(index, _T("_Append_"));
		}
		else
			modelpath += _T("_Append_");
	}
	return m_BpNet.SaveWeighttoFile(modelpath);
}

bool CNetWorker::LoadModel(const char* pPath)
{
	Stringoper oper;
	char path[MAX_PATH];
	strcpy_s(path, pPath);
	CString modelpath = oper.chartocstring(path);
	m_BpNet.m_bSimulateDataFlag = true;
	return m_BpNet.LoadWeightFormFile(modelpath);
}

bool CNetWorker::Sim(double* res, int& len)
{
	CMatrix mat;
	bool b = m_BpNet.SimulateData(mat);
	if (!b)
		return false;
	*res = mat.m_pTMatrix[0][0] * (-1);			//计算结果
	if (*res <= m_dLeft)
		*res = -1;								//left
	else if (*res >= m_dRight)
		*res = 0;								//right
	else
		*res = 1;								//both
	len = 1;
	return b;
}

bool CNetWorker::Sim(ManagedData& data, ManagedData& color)
{
	//仿真结果放入color中
	int distance = 0;
	ManagedData colorone;
	color.GetAnydata(colorone, 1);				//获取任意通道数据

	int sampleblock = 0;
	BpNetdata pd[50];

	ASSERT(data.GetChannum() >= 2);
	m_LeftData.Remove(m_LeftData.GetDataSize()+1);
	m_RightData.Remove(m_RightData.GetDataSize()+1);
	data.GetAnydata(m_LeftData, 0);
	data.GetAnydata(m_RightData, 1);
	for (int i = 0; i < colorone.GetDataSize(); i++)
	{
		if (colorone[i]>0)
		{
			distance++;
		}
		else if (distance > 0)
		{
			pd[sampleblock] = CalNetData(m_LeftData.GetData() + i - distance,
				m_RightData.GetData() + i - distance ,distance);
			pd[sampleblock].sp = i - distance;				//起始位置
			pd[sampleblock].ep = i;							//终止位置
			sampleblock++;									//样本数
			distance = 0;
		}
	}
	if (sampleblock <= 0 && distance < 64)        //hwh_add
		return false;
	else if (distance == 64)
	{
		pd[sampleblock] = CalNetData(m_LeftData.GetData() ,m_RightData.GetData() , distance);
		sampleblock = 1;
	}
	double d[3];
	d[0] = pd[sampleblock - 1].Lvalue;										//选取最后一个咬牙左边通道均值
	d[1] = pd[sampleblock - 1].Rvalue;
	d[2] = pd[sampleblock - 1].LrationR;
	SetTestData(d, 3);
	int res;
	bool ret = Sim(d, res);									//计算结果
	color[0] = d[0];
	return ret;
}


void CNetWorker::SetParament(double err, int time)
{
	m_Error = err;
	m_Traintime = time;
}

void CNetWorker::SetTestData(const double* pd, const int len)
{
	if (!pd || len <= 0)
		return;
	m_BpNet.SetSimDataFormbuf(pd , len);
}

void CNetWorker::GetTrainState(double& err, int& traintime)
{
	m_BpNet.GetTrainState(err, traintime);
}

void CNetWorker::StopTrain(const bool b)
{
	m_BpNet.ChangeTrain(b);				//停止训练
}

void CNetWorker::SetLRThrolds(double left, double right)
{
	m_dLeft = left;
	m_dRight = right;
}

void CNetWorker::SetWinlen(int len)
{
	m_Winlen = len;
}

void CNetWorker::SetThreashold(Throld t)
{
	m_Throld = t;
}
//private function
void CNetWorker::SetTrainData(const double* pd, const int len)
{

}

bool CNetWorker::LoadTestData(const char *pPath)
{
	return false;
}

void CNetWorker::Config(){}

BpNetdata CNetWorker::CalNetData(const double *rawdatal, const double *rawdatar, const int length)
{
	Processdata data;
	data.lmaxmin = maxdata(rawdatal, length) - mindata(rawdatal, length);
	data.rmaxmin = maxdata(rawdatar, length) - mindata(rawdatar, length);

	BpNetdata bp;
	bp.Lvalue = data.lmaxmin / m_Throld.voltvalue;
	bp.Rvalue = data.rmaxmin / m_Throld.voltvalue;
	if (bp.Rvalue == 0)
		bp.LrationR = 1;
	else
		bp.LrationR = bp.Lvalue / bp.Rvalue;
	return bp;
}


