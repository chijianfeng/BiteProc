#include "stdafx.h"
#include "CalcuteColor.h"
#include "Stringoper.h"
#include "FiltFilt.h"
#include "Structdefine.h"

CCalcuteColor::CCalcuteColor()
{
	m_pData = new double*[MAXCHANNUM];
	for (int i = 0; i < MAXCHANNUM; i++)
	{
		m_pData[i] = new double[MAXHANDLEDATA];
	}
	m_pDataBuf = new double[MAXHANDLEDATA + 1];
	m_pManagedata = new ManagedData[MAXCHANNUM];
	m_bIsOK = false;
}

CCalcuteColor::CCalcuteColor(int len)
{
	m_pData = new double*[MAXCHANNUM];
	for (int i = 0; i < MAXCHANNUM; i++)
	{
		m_pData[i] = new double[MAXHANDLEDATA];
	}
	m_pDataBuf = new double[MAXHANDLEDATA + 1];
	m_pManagedata = new ManagedData[MAXCHANNUM];
	m_Winlen = len;
	m_bIsOK = false;
}


CCalcuteColor::~CCalcuteColor()
{
	for (int i = 0; i < MAXCHANNUM; i++)
	{
		delete[] m_pData[i];
	}
	delete m_pData;
	delete m_pDataBuf;
	delete m_pManagedata;
}

bool CCalcuteColor::Sim(ManagedData& data, ManagedData& result)
{
	if (!m_bIsOK)
		return false;
	//首先进行滤波
	FiltData(data, m_FiltData);
	ProcessData(data, m_FiltData, result);
	return true;
}

bool CCalcuteColor::LoadModel(const char* pPath)
{
	if (!pPath)
		return false;
	Stringoper oper;
	CString path;
	char tmppath[MAX_PATH];
	strcpy_s(tmppath, pPath);
	path  = oper.chartocstring(tmppath);
	CStdioFile file;
	if (!file.Open(path, CFile::modeRead))
		return false;
	
	CString str;
	file.ReadString(str);
	string _key, val;
	char comment = '=';
	oper.AnalyseLine(oper.tostring(str), _key, val, comment);
	m_BiteThreshold.stdvalue = oper.stringtoint(val);

	file.ReadString(str);
	oper.AnalyseLine(oper.tostring(str), _key, val, comment);
	m_BiteThreshold.voltvalue = oper.stringtoint(val);

	file.Close();

	m_bIsOK = true;
	return true;
}

void CCalcuteColor::FiltData(ManagedData& rawdata, ManagedData& filtData)
{
	if (rawdata.GetChannum() <= 0)
		return;
	if (rawdata.GetChannum() > MAXCHANNUM)
		return;
	FiltFilt	ff;
	ff.SetParam(a_3_100, b_3_100, 7);

	for (int i = 0; i < rawdata.GetChannum();i++)								//对每个通道提取并滤波
	{
		rawdata.GetAnydata(m_pManagedata[i] , i);
		ff.useFFilt(m_pManagedata[i].GetData(), m_pData[i], m_pManagedata[i].GetDataSize());
	}
	filtData.ResizeManageData(rawdata.GetDataSize());
	for (int i = 0; i < rawdata.GetDataSize() / rawdata.GetChannum();i++)		//合并各个通道
	{
		for (int j = 0; j < rawdata.GetChannum();j++)
		{
			m_pDataBuf[i*rawdata.GetChannum() + j] = m_pData[j][i];
		}
	}
	filtData.AppendData(m_pDataBuf, rawdata.GetDataSize());						//设置值
	filtData.SetChannum(rawdata.GetChannum());
	filtData.SetSampleRate(rawdata.GetSampleRate());
}

void CCalcuteColor::ProcessData(ManagedData& rawdata, ManagedData& filtdata , ManagedData& colordata)
{
	int winlen = m_Winlen;					//分析窗口长
	if (winlen <= 0)
		return;
	m_ColorData.SetData(0.0, rawdata.GetDataSize());
	Processdata psd;
	//选取其中两个通道:0,and 1
	rawdata.GetAnydata(m_pManagedata[0], 0);
	rawdata.GetAnydata(m_pManagedata[1], 1);
	filtdata.GetAnydata(m_pManagedata[3], 0);
	filtdata.GetAnydata(m_pManagedata[4], 1);

	COLORREF Curbite = NBite;
	int len = m_pManagedata[0].GetDataSize();
	for (int i = 0; i < len;i++)
	{
		if (i + winlen + 1 >= len)							//若分析窗口小于winlen，则标记为Curbite
		{
			m_ColorData[i * 2] = (double)Curbite;
			m_ColorData[i * 2 + 1] = (double)Curbite;
			continue;
		}
		/*对这段数据进行最大值减最小值以及标准差计算
		若计算结果小于阈值（包括标准差小于阈值），则将这段数据中的标准差置为0
		*/
		psd = processdata(m_pManagedata[0].GetData() + i, m_pManagedata[1].GetData() + i, m_pManagedata[2].GetData() + i, 
			m_pManagedata[3].GetData() + i, winlen + 1, m_BiteThreshold);
		if (psd.lmaxmin >= m_BiteThreshold.voltvalue || psd.rmaxmin >= m_BiteThreshold.voltvalue)			//大于阈值则判断为咬牙
		{
			m_ColorData[i * 2] = (double)Bite;
			m_ColorData[i * 2 + 1] = (double)Bite;
			Curbite = Bite;
		}
		else
		{
			m_ColorData[i * 2] = (double)NBite;
			m_ColorData[i * 2 + 1] = (double)NBite;
			Curbite = NBite;
		}
	}
	int cnl = colordata.GetDataSize();			//更新color缓冲区颜色值
	int crl = m_ColorData.GetDataSize();
	for (int k = cnl - 1; k >= 0; k--)
	{
		colordata[k] = m_ColorData[crl - cnl + k];
	}
}

//private function
bool CCalcuteColor::LoadTrainData(const char* pPath)
{
	return false;
}

void CCalcuteColor::SetTrainData(const double* pd, const int len)
{

}

bool CCalcuteColor::Train(char* path)
{
	return false;
}

bool CCalcuteColor::LoadTestData(const char *pPath)
{
	return false;
}

void CCalcuteColor::SetTestData(const double* pd, const int len)
{
	return;
}

bool CCalcuteColor::Sim(double* res, int& len)
{
	return false;
}

bool CCalcuteColor::SaveTrainModel(const char* pPath)
{
	return false;
}

void CCalcuteColor::Config()
{

}




