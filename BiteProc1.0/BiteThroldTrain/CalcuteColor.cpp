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
	//���Ƚ����˲�
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

	for (int i = 0; i < rawdata.GetChannum();i++)								//��ÿ��ͨ����ȡ���˲�
	{
		rawdata.GetAnydata(m_pManagedata[i] , i);
		ff.useFFilt(m_pManagedata[i].GetData(), m_pData[i], m_pManagedata[i].GetDataSize());
	}
	filtData.ResizeManageData(rawdata.GetDataSize());
	for (int i = 0; i < rawdata.GetDataSize() / rawdata.GetChannum();i++)		//�ϲ�����ͨ��
	{
		for (int j = 0; j < rawdata.GetChannum();j++)
		{
			m_pDataBuf[i*rawdata.GetChannum() + j] = m_pData[j][i];
		}
	}
	filtData.AppendData(m_pDataBuf, rawdata.GetDataSize());						//����ֵ
	filtData.SetChannum(rawdata.GetChannum());
	filtData.SetSampleRate(rawdata.GetSampleRate());
}

void CCalcuteColor::ProcessData(ManagedData& rawdata, ManagedData& filtdata , ManagedData& colordata)
{
	int winlen = m_Winlen;					//�������ڳ�
	if (winlen <= 0)
		return;
	m_ColorData.SetData(0.0, rawdata.GetDataSize());
	Processdata psd;
	//ѡȡ��������ͨ��:0,and 1
	rawdata.GetAnydata(m_pManagedata[0], 0);
	rawdata.GetAnydata(m_pManagedata[1], 1);
	filtdata.GetAnydata(m_pManagedata[3], 0);
	filtdata.GetAnydata(m_pManagedata[4], 1);

	COLORREF Curbite = NBite;
	int len = m_pManagedata[0].GetDataSize();
	for (int i = 0; i < len;i++)
	{
		if (i + winlen + 1 >= len)							//����������С��winlen������ΪCurbite
		{
			m_ColorData[i * 2] = (double)Curbite;
			m_ColorData[i * 2 + 1] = (double)Curbite;
			continue;
		}
		/*��������ݽ������ֵ����Сֵ�Լ���׼�����
		��������С����ֵ��������׼��С����ֵ��������������еı�׼����Ϊ0
		*/
		psd = processdata(m_pManagedata[0].GetData() + i, m_pManagedata[1].GetData() + i, m_pManagedata[2].GetData() + i, 
			m_pManagedata[3].GetData() + i, winlen + 1, m_BiteThreshold);
		if (psd.lmaxmin >= m_BiteThreshold.voltvalue || psd.rmaxmin >= m_BiteThreshold.voltvalue)			//������ֵ���ж�Ϊҧ��
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
	int cnl = colordata.GetDataSize();			//����color��������ɫֵ
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




