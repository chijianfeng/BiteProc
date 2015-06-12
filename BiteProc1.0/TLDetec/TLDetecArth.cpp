#include "StdAfx.h"
#include "TLDetecArth.h"
#include "Stringoper.h"
#include "Structdefine.h"
#include "DataProcess.h"
#include <iostream>
#include <list>
using namespace std;

const char PARAM_COMMENT = '%';
CTLDetecArth::CTLDetecArth(void)
{
	m_nPData = NULL;
	m_nPData = new DataSet[_MAX_DATA_LENGTH];
	for (int i=0;i<_MAX_DATA_LENGTH;i++)
	{
		m_nPData[i].len = 0;
		m_nPData[i].pd = NULL;
		m_nPData[i].rlen = 0;
		m_nPData[i].res = NULL;
	}
	m_nSimData.len =  0;
	m_nSimData.pd = NULL;
	m_nSimData.res = NULL;
	m_nSimData.rlen = 0;
	m_nSamplenum = 0;
}

CTLDetecArth::CTLDetecArth(const char* parmpath)
{
	m_nPData = NULL;
	m_nPData = new DataSet[_MAX_DATA_LENGTH];
	for (int i=0;i<_MAX_DATA_LENGTH;i++)
	{
		m_nPData[i].len = 0;
		m_nPData[i].pd = NULL;
		m_nPData[i].rlen = 0;
		m_nPData[i].res = NULL;
	}
	m_nSimData.len =  0;
	m_nSimData.pd = NULL;
	m_nSimData.res = NULL;
	m_nSimData.rlen = 0;
	m_nSamplenum = 0;
	if(parmpath==NULL)
		return;
	Stringoper oper;
	char tmp[1024];
	strcpy_s(tmp , parmpath);
	m_nParamPath = oper.chartocstring(tmp);
}

CTLDetecArth::~CTLDetecArth(void)
{
	Release();
}

bool CTLDetecArth::LoadTrainData(const char* pPath)
{
	if(pPath==NULL)
		return false;
	CFileFind finder;
	Stringoper oper;
	if(!finder.FindFile(oper.tocstring(pPath)))
		return false;
	FILE *file = NULL;
	if((file=fopen(pPath,"r"))==NULL)
	{
		TRACE("Open Source file Failed\n");
		return false;
	}
	unsigned int num_input, num_output, num_data;
	unsigned int line = 1;
	if(fscanf(file, "%u %u %u\n", &num_data, &num_input, &num_output) != 3)
	{
		TRACE("Read the File Head failed ");
		TRACE("Read error in Line:%d\n",line);
		return false;
	}
	line++;

	for (int i=0;i<num_data;i++)
	{
		m_nPData[i].len = num_input;
		m_nPData[i].rlen = num_output;
		m_nPData[i].pd = new double[num_input];
		m_nPData[i].res = new double[num_output];
		for (int j=0;j<num_input;j++)
		{
			if(fscanf(file, "%le" " ",&m_nPData[i].pd[j]) != 1)
			{
				Release();
				TRACE("Read error in Line:%d\n",line);
				return false;
			}
		}
		for (int j=0;j<num_output;j++)
		{
			if(fscanf(file, "%le" " ",&m_nPData[i].res[j]) != 1)
			{
				Release();
				TRACE("Read error in Line:%d\n",line);
				return false;
			}
		}
		line++;
	}
	m_nSamplenum = num_data;
	return true;
}

bool CTLDetecArth::Train(char* path /* = NULL */)
{
	int lep = m_nConfig.Getleftend();
	int rsp = m_nConfig.GetrightStart();
	double	max1  , max2 ,  mind;
	list<double> target;
	target.clear();
	for (int i=0;i<m_nSamplenum;i++)
	{
		target.push_back(m_nPData[i].res[0]);
	}
	target.sort();
	target.unique();
	int *pt = new int[target.size()];
	for (int i=0;i<target.size();i++)
	{
		pt[i] = 128;
	}
	m_nThres = 0;
	for (int i=0;i<m_nSamplenum;i++)
	{
		max1 = maxdata(m_nPData[i].pd , lep);
		max2 = maxdata(m_nPData[i].pd+rsp , m_nPData[i].len-rsp+1);
		mind = min(max1 , max2)*m_nConfig.GetRatio1();
		Pos np;
		Pos oldp;
		oldp.sp = firstpos(m_nPData[i].pd,lep,max1); 
		oldp.ep = firstpos(m_nPData[i].pd+rsp , m_nPData[i].len-rsp+1,max2)+rsp;
		int l = GetMaxLength(m_nPData[i].pd ,oldp , mind , np);
		pt[i] = l;
	}
	for (int i = 0; i < m_nSamplenum;i++)
	{
		m_nThres += pt[i];
	}
	if (m_nSamplenum <= 0)
		m_nThres = 128;
	else
		m_nThres /= m_nSamplenum;
//	delete[] pt;
	return true;
}

void CTLDetecArth::SetTestData(const double* pd , const int len)
{
	if(m_nSimData.pd == NULL)
		m_nSimData.pd = new double[len];
	for (int i=0;i<len;i++)
	{
		m_nSimData.pd[i] = pd[i];
	}
	m_nSimData.len = len;
}

bool CTLDetecArth::Sim(double* res , int& len)
{
	if(m_nSimData.len<=0)
		return false;
	int lep = m_nConfig.Getleftend();
	int rsp = m_nConfig.GetrightStart();
	double	max1  , max2 ,  mind;
	max1 = maxdata(m_nSimData.pd , lep);
	max2 = maxdata(m_nSimData.pd+rsp , m_nSimData.len-rsp+1);
	mind = min(max1 , max2)*m_nConfig.GetRatio1();
	Pos np , np2;
	Pos oldp;
	oldp.sp = firstpos(m_nSimData.pd,lep,max1); 
	oldp.ep = firstpos(m_nSimData.pd+rsp ,m_nSimData.len-rsp+1,max2)+rsp;
	int l = GetMaxLength(m_nSimData.pd ,oldp , mind , np);
	int l2= GetMaxLength(m_nSimData.pd , np , mind*m_nConfig.GetRatio2()/m_nConfig.GetRatio1(),np2);
	if(l<=m_nThres)
	{
		*res = 3;	//long bite.
		len = 1;
	}
	else if(l2<l*m_nConfig.GetRatio3())
	{
		*res = 3;
		len = 1;
	}
	else
	{
		*res = 2;		//two bite
		len = 1;
	}
	return true;
}

bool CTLDetecArth::Sim(ManagedData& data, ManagedData& result)
{
	//首先判断是否是符合两次或者长咬判断条件的
	//若不是，则返回0，否则返回two：2，long：3
	//选取值最大的通道
	if (data.GetChannum() < 2)
		return false;
	ManagedData colord;
	result.GetAnydata(colord, 1);
	int startpos;
	if (IsOneBite(colord , startpos))
	{
		result[0] = 0;						//返回0，不进行判断
		return true;
	}
	//选取需要进行判断的通道
	int selchannel = GetSelChannel(data);
	ManagedData processdata;
	data.GetAnydata(processdata, selchannel);
	//对选取的通道数据进行滤波
	ManagedData newd;
	DATAPROCESS::ProcessData(processdata , newd);
	SetTestData(newd.GetData() + startpos, newd.GetDataSize() - startpos);
	double res;
	int len;
	bool b = Sim(&res, len);
	result[0] = res;
	return b;
}

bool CTLDetecArth::SaveTrainModel(const char* pPath)
{
	if(pPath ==NULL)
		return false;
	CStdioFile file;
	Stringoper oper;
	BOOL bIsopen = file.Open(oper.tocstring(pPath) , CFile::modeWrite|CFile::modeCreate);
	if(bIsopen==false)
		return false;
	file.SeekToBegin();
	file.SetLength(0);
	string s = "Thres = "+oper.inttostring(m_nThres);
	file.Write(s.c_str() , s.size());
	file.Close();
	return true;
}

bool CTLDetecArth::LoadModel(const char* pPath)
{
	if(pPath==NULL)
		return false;
	CStdioFile file;
	Stringoper oper;
	BOOL bIsopen = file.Open(oper.tocstring(pPath) , CFile::modeRead);
	if(!bIsopen)
		return false;
	file.SeekToBegin();
	CString tmp;
	file.ReadString(tmp);
	string line, key, value;
	line = oper.tostring(tmp);
	AnalyseLine(line , key  , value);
	m_nThres = oper.stringtoint(value);
	Config();
	return true;
}

void CTLDetecArth::Config()
{
	Stringoper oper;
	char c[1024];
	oper.CStringtoChars(m_nParamPath , c);
	m_nConfig.LoadArthConfig(c);
}


//private function
bool CTLDetecArth::LoadTestData(const char *pPath)
{
	return false;
}

void CTLDetecArth::SetTrainData(const double* pd , const int len)
{
	return;
}

void CTLDetecArth::Release()
{
	if(m_nPData!=NULL)
	{
		for (int i=0;i<_MAX_DATA_LENGTH;i++)
		{
			if(m_nPData[i].pd!=NULL)
			{
				delete m_nPData[i].pd;
				m_nPData[i].pd = NULL;
			}
			if(m_nPData[i].res!=NULL)
			{
				delete m_nPData[i].res;
				m_nPData[i].res = NULL;
			}
		}
		delete[] m_nPData;
		m_nPData = NULL;
	}
	if(m_nSimData.pd!=NULL)
	{
		delete m_nSimData.pd;
		m_nSimData.pd = NULL;
		m_nSimData.len = 0;
	}
	if(m_nSimData.res!=NULL)
	{
		delete m_nSimData.res;
		m_nSimData.rlen = 0;
		m_nSimData.res = NULL;
	}
}

int CTLDetecArth::GetMaxLength(const double* pd , const Pos p1 , const double thresd , Pos& p2)
{
	int count = 0 , num = 0;
	Pos curpos;
	curpos.sp = p1.sp;
	curpos.ep = p1.sp;
	for (int i = p1.sp;i<p1.ep;i++)
	{
		if(pd[i]<=thresd)
		{
			if(count==0)
				curpos.sp = i;
			count++;
			curpos.ep = i;
		}
		else
		{
			if(count>num)
			{
				num = count;
				p2 = curpos;
			}
			count = 0;
		}
	}
	if(count>num)
	{
		num = count;
		p2 = curpos;
	}
	return num;
}

bool CTLDetecArth::IsSpace(char c)
{
	if(' '==c||'\t'==c)
		return true;
	return false;
}
bool CTLDetecArth::IsCommentChar(char c)
{
	switch(c){
	case PARAM_COMMENT:
		return true;

	default:
		return false;
	}
}

void CTLDetecArth::TrimString(string& str)
{
	if(str.empty())
		return;
	int i , start_pos , end_pos;
	for (i=0;i<str.size();i++)
	{
		if (!IsSpace(str[i]))
		{
			break;
		}
	}
	if(i==str.size())
	{
		str="";
		return;
	}
	start_pos = i;
	for (i = str.size() - 1; i >= 0; --i) {
		if (!IsSpace(str[i])) {
			break;
		}
	}
	end_pos = i;

	str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool CTLDetecArth::AnalyseLine(const string& line , string& key , string& value)
{
	if(line.empty())
		return false;
	int start_pos = 0 , end_pos = line.size()-1 , pos = 0;
	if((pos=line.find(PARAM_COMMENT))!=-1)
	{
		if(0==pos)
			return false;
		end_pos = pos - 1 ;  
	}
	string new_line = line.substr(start_pos, start_pos + 1 - end_pos); 
	if ((pos = new_line.find('=')) == -1)
		return false;  
	key = new_line.substr(0, pos);
	value = new_line.substr(pos + 1, end_pos + 1- (pos + 1));

	TrimString(key);
	if (key.empty()) {
		return false;
	}
	TrimString(value);
	return true;
}

int CTLDetecArth::GetSelChannel(ManagedData& d)
{
	int i = 0;
	int index = 0;
	double maxv = 0;
	for (i = 0; i < d.GetChannum(); i++)
	{
		ManagedData tmp;
		d.GetAnydata(tmp, i);
		double v = stdval(tmp.GetData(), tmp.GetDataSize());
		if (maxv < v)
		{
			index = i;
			maxv = v;
		}
	}
	return index;
}

bool CTLDetecArth::IsOneBite(ManagedData& d, int& startpos)
{
	BpNetdata bpdata[10];
	int sampleblock = 0;			//样本数
	int dis = 0;
	startpos = 0;
	for (int i = 0; i < d.GetDataSize();i++)
	{
		if (d[i]>0)
			dis++;
		if (d[i] <= 0 && dis > 0)
		{
			bpdata[sampleblock].sp = i - dis;
			bpdata[sampleblock].ep = i - 1;
			sampleblock++;
			dis = 0;
		}
	}
	if (sampleblock <= 0 && dis < 64)
		return true;
	else if (dis == 64)          //hwh_add
	{
		sampleblock = 1;
		startpos = 64;
		return false;
	}
	else if (sampleblock==1&&bpdata[sampleblock - 1].ep - bpdata[sampleblock - 1].sp + 1 > ONEBITELENGTH*d.GetSampleRate())
	{
		startpos = bpdata[sampleblock - 1].sp;
		return false;
	}
	else if (sampleblock >= 2 && bpdata[sampleblock - 2].ep > BITELENGTH*d.GetSampleRate())
	{
		startpos = bpdata[sampleblock - 2].sp;
		return false;
	}
	return true;
}

extern "C" _declspec(dllexport) CBaseArth* CreateArth(void* p, void *p1 = NULL)
{
	if(p==NULL)
		return new CTLDetecArth();
	else
	{
		return new CTLDetecArth((char*)p);
	}
}