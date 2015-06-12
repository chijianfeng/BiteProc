#include "stdafx.h"
#include "DistinguishHL.h"
#include "Stringoper.h"
#include "FiltFilt.h"
#include "Structdefine.h"

const double a[] = { 1.0000, -1.1689, -0.8881, 0.7217, 0.7770, -0.2404, -0.1978 };
const double b[] = { 0.4459,       0, -1.3377,      0, 1.3377,       0, -0.4459 };
const string HL_key[] =
{
	"L_Vol", "L_Std", "L_Light_Vol", "L_Heavy_Vol",
	"R_Vol", "R_Std", "R_Light_Vol", "R_Heavy_Vol",
	"LR_Vol","LR_Std", "LR_Light_Vol", "LR_Heavy_Vol"
};

CDistinguishHL::CDistinguishHL()
{

}

CDistinguishHL::CDistinguishHL(int len)
{
}

CDistinguishHL::~CDistinguishHL()
{
}

bool CDistinguishHL::Train(char* path)
{
	return false;
}

int  CDistinguishHL::judgeLH(int res,double t)
{
	double d;
	int key;
	switch (res + 1)
	{
	case 0:
		key = 0;
		break;
	case 1:
		key = 4;
		break;
	case 2:
		key = 8;
		break;
	default:
		break;
	}
	d = m_nResult[HL_key[key]];
	if (t <= d)					//OneBite(light)
		res = 0;
	else						//OneBite(heavy)
		res = 1;
	return res;
}

bool CDistinguishHL::Sim(double* res, int& len)
{
	return true;
}

bool  CDistinguishHL::Sim(ManagedData& data, ManagedData& result)
{
	ManagedData DataOne;
	int distance = 0;
	int sampleblock = 0;
	int classify = (int)result[0];
	result.GetAnydata(DataOne, 1);
	ASSERT(data.GetChannum() >= 2);
	m_LeftData.Remove(m_LeftData.GetDataSize()+1);
	m_RightData.Remove(m_RightData.GetDataSize()+1);
	data.GetAnydata(m_LeftData, 0);
	data.GetAnydata(m_RightData, 1);
	int startpos;
	double L;
	double R;
	double LR;
	if (!IsOneBite(DataOne, startpos,L,R,LR))
	{
		result[0] = 0;
		return true;
	}
	result[0] = judgeLH(classify, max(L,R));
	return true;
}

bool CDistinguishHL::IsOneBite(ManagedData& d, int& startpos,double& LValue,double& RValue,double& LRValue)
{
	BpNetdata bpdata[10];
	int sampleblock = 0;			//Ñù±¾Êý
	int dis = 0;
	startpos = 0;
	for (int i = 0; i < d.GetDataSize(); i++)
	{
		if (d[i]>0)
			dis++;
		if (d[i] <= 0 && dis > 0)
		{
			bpdata[sampleblock] = CalNetData(m_LeftData.GetData() + i - dis,
				m_RightData.GetData() + i - dis, dis);
			bpdata[sampleblock].sp = i - dis;
			bpdata[sampleblock].ep = i - 1;
			sampleblock++;
			dis = 0;
		}
	}
	if (sampleblock <= 0 && dis < 64)
		return false;
	else if (dis == 64)
	{
		startpos = 64;
		sampleblock = 1;
		return false;
	}
	else if (sampleblock == 1 && bpdata[sampleblock - 1].ep - bpdata[sampleblock - 1].sp + 1 > ONEBITELENGTH*d.GetSampleRate())
	{
		startpos = bpdata[sampleblock - 1].sp;
		return false;
	}
	else if (sampleblock >= 2 && bpdata[sampleblock - 2].ep > BITELENGTH*d.GetSampleRate())
	{
		startpos = bpdata[sampleblock - 2].sp;
		return false;
	}
	LValue = bpdata[sampleblock - 1].Lvalue;
	RValue = bpdata[sampleblock - 1].Rvalue;
	LRValue = bpdata[sampleblock - 1].LrationR;
	return true;
}

bool   CDistinguishHL::GetResult(map<string, double> &Result,int Time,Throld t)
{
	m_nResult = Result;
	m_Time = Time;
	m_Throld = t;
	return true;
}

bool  CDistinguishHL::SaveTrainModel(const char* pPath)
{
	CStdioFile file;
	Stringoper oper;
	CString ModelPath;
	char path[MAX_PATH];
	string key;
	strcpy_s(path, pPath);
	ModelPath = oper.chartocstring(path);
	if (m_Time == 0)
	{
		if (!file.Open(ModelPath, CFile::modeReadWrite | CFile::modeCreate))
			return false;
		file.SeekToBegin();
	}
	else
	{
		if (!file.Open(ModelPath, CFile::modeReadWrite))
			return false;
		file.SeekToEnd();
	}
	for (int i = 0; i < 4; i++)
	{
		double value = m_nResult[HL_key[m_Time * 4 + i]];
		string s = HL_key[m_Time * 4 + i] +" = "+ oper.doubletostring(value)+"\n";
		file.Write(s.c_str(),s.size());
	}
	file.Close();
	return true;
}

bool  CDistinguishHL::LoadModel(const char* pPath)
{
	CStdioFile file;
	Stringoper oper;
	CString ModelPath;
	char path[MAX_PATH];

	strcpy_s(path, pPath);
	ModelPath = oper.chartocstring(path);
	
	if (!file.Open(ModelPath, CFile::modeReadWrite))
		return false;
	file.SeekToBegin();
	
	string line, key, value;
	CString tmp;
	while (file.ReadString(tmp))
	{
		line = oper.tostring(tmp);
		if (oper.AnalyseLine(line, key, value, '#'))
		{
			m_nResult[key] = oper.stringtodouble(value);
		}
	}
	file.Close();
	return true;
}

void CDistinguishHL::SetTestData(const double* pd, const int len)
{
}

bool CDistinguishHL::LoadTestData(const char *pPath)
{
	return false;
}

void CDistinguishHL::SetTrainData(const double* pd, const int len)
{
	return;
}

bool CDistinguishHL::LoadTrainData(const char* path)
{
	return false;
}

void CDistinguishHL::Config()
{

}

BpNetdata CDistinguishHL::CalNetData(const double *rawdatal, const double *rawdatar, const int length)
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

