#include "stdafx.h"
#include "ManagedData.h"
#include <assert.h>
#include <algorithm>

ManagedData::ManagedData(void):
m_MaxSize(MAXHANDLEDATA)
{ 
	eegdata = new double[MAXHANDLEDATA];
	datalength = 0;
	samplerate = 0;
	channum = 0;
	m_Head = 0;		//指向第一个元素位置
	m_Tail = 0;		//指向最后一个元素的后一个位置
}

ManagedData::ManagedData(int size):
m_MaxSize(size),
	datalength(0),
	samplerate(0),
	channum(0),
	m_Head(0),m_Tail(0)
{
	if(m_MaxSize<=0)
		m_MaxSize = MAXHANDLEDATA;
	eegdata = new double[size];
}
ManagedData::~ManagedData(void)
{
	delete[] eegdata;
	eegdata = NULL;
	
}
ManagedData::ManagedData(const ManagedData& data):
datalength(data.datalength),
	samplerate(data.samplerate),
	channum(data.channum),
	m_MaxSize(data.m_MaxSize),
	m_Head(data.m_Head),
	m_Tail(data.m_Tail)
{
	eegdata = new double[m_MaxSize];
	memcpy(eegdata,data.eegdata,m_MaxSize*sizeof(double));
}
ManagedData& ManagedData::operator=(const ManagedData& data)
{
	datalength	=	data.datalength;
	samplerate	=	data.samplerate;
	channum		=	data.channum;
	m_MaxSize	=	data.m_MaxSize;
	m_Head		=	data.m_Head;
	m_Tail		=	data.m_Tail;
	if(eegdata!=NULL)
		delete[] eegdata;
	eegdata = new double[m_MaxSize];										//for deep copy
	memcpy(eegdata,data.eegdata,m_MaxSize*sizeof(double));
	return *this;
}

double& ManagedData::operator[](int index)
{
	assert(index<=GetDataSize());
	return *(eegdata+(m_Head+index)%m_MaxSize);
}

void ManagedData::Zero(size_t len)
{
	if (len <= 0)
		return;
	if (MAXHANDLEDATA < len)
		ResizeManageData(len);
	memset(eegdata, 0, sizeof(double)*len);
	m_Head = 0;
	m_Tail = len;
	datalength = len;
}

double* ManagedData::GetData()const
{
	return eegdata + m_Head;
}

int ManagedData::GetDataSize()const
{
	return datalength;
}
void ManagedData::SetChannum(int channum)
{
	this->channum = channum;
}
int ManagedData::GetChannum() const 
{
	return channum;
}
void ManagedData::SetSampleRate(long rate)
{
	this->samplerate = rate;
}
long ManagedData::GetSampleRate() const
{
	return samplerate;
}

void ManagedData::GetAnydata(double *temp,int chanserial,int length)    
{
	if (chanserial>=channum||length>datalength/channum)
	{
		return;
	}
	for(int i=0;i<length;i++)
		temp[i] = eegdata[(m_Head+chanserial+i*channum)%m_MaxSize];
}

void ManagedData::GetAnydata(ManagedData& d , int chanserial)
{
	if(chanserial<0||GetChannum()<=0)
		return;
	int size = GetDataSize()/GetChannum();
	double *pd = new double[size];
	for (int i=0;i<size;i++)
	{
		pd[i] = eegdata[(m_Head+chanserial+i*channum)%m_MaxSize];
	}
	d.SetData(pd,size);
	d.SetChannum(1);
	d.SetSampleRate(GetSampleRate());
	delete[] pd;
}

void ManagedData::AppendData(ManagedData& m_data)
{
	//ASSERT(m_data->GetChannum()==GetChannum());        //if not equal then failed.
	if(m_data.GetChannum()!=GetChannum()&&GetChannum()>0)
		return ;
	int m_len = GetDataSize()+m_data.GetDataSize();
	int len = 0,i = 0;
	if(m_len>m_MaxSize)
	{
		int len = m_len%m_MaxSize;
		m_Head = (m_Head+len)%m_MaxSize;
		datalength = m_MaxSize;
	}
	else
		this->datalength = datalength+m_data.GetDataSize();
	for(;i<m_data.GetDataSize();i++)
	{		
		*(eegdata+m_Tail) =  (m_data)[i];
		m_Tail = (++m_Tail)%m_MaxSize;
	}
	SetChannum(m_data.GetChannum());
	SetSampleRate(m_data.GetSampleRate());
}
void ManagedData::AppendData(const double*m_data, const int length)
{
	//assert(m_data!=NULL);
	if(m_data==NULL)
		return ;
	int m_len = length+datalength;
	int len = 0,i = 0;
	if(m_len>m_MaxSize)
	{
		int len = m_len%m_MaxSize;
		m_Head = (m_Head+len)%m_MaxSize;
		datalength = m_MaxSize;
	}
	else
		datalength+=length;
	for(;i<length;i++)
	{	
		*(eegdata+m_Tail) =  *(m_data++);
		m_Tail = (++m_Tail)%m_MaxSize;
	}
}
void ManagedData::Remove(const int m_datalen)
{
	if(m_datalen>datalength)				  //remove all.
	{
		datalength = 0;
		m_Head = m_Tail = 0;
		return;
	}          
	m_Head = (m_Head+m_datalen)%m_MaxSize;
	datalength-=m_datalen;
}

void ManagedData::GetData(ManagedData& data, size_t& size)
{
	int s = (int)size;
	int l = min(s, datalength);
	data.ResizeManageData(l+1);
	for (int i = 0; i<l; i++)
	{
		data.eegdata[data.m_Tail++] = eegdata[(m_Head++) % m_MaxSize];
	}
	datalength -= l;
	size = (size_t)l;
	data.samplerate = samplerate;
	data.channum = channum;
	data.datalength = l;
}

void ManagedData::ResizeManageData(const int szie)
{
	if(szie<=0)
		return;
	m_MaxSize = szie;
	m_Head = m_Tail = 0;
	delete[] eegdata;
	eegdata = new double[szie];
}

int ManagedData::GetBufsize() const
{
	return m_MaxSize;
}

int	ManagedData::CopyData(ManagedData& d , int len , bool isfront) const
{
	//if isfornt is true copy the data from head , or form the tail.
	int l = min(len , datalength);
	double *pd = new double[l];
	if(isfront)
	{
		for (int i=0;i<l;i++)
		{
			pd[i] = eegdata[(m_Head+i)%m_MaxSize];
		}
	}
	else
	{
		for (int i=l-1;i>=0;i--)
		{
			pd[i] = eegdata[(m_MaxSize+m_Tail+i-l)%m_MaxSize];
		}
	}
	d.SetData(pd , l);
	d.SetSampleRate(GetSampleRate());
	d.SetChannum(GetChannum());
	delete[] pd;
	return l;
}

void ManagedData::GetMaxMin(double& maxv , double& minv)
{
	if(datalength==0)
	{
		maxv = minv = 0;
		return;
	}
	maxv = minv = 0;
	for (int i=0;i<datalength;i++)
	{
		if(eegdata[(m_Head+i)%m_MaxSize]>maxv)
			maxv = eegdata[(m_Head+i)%m_MaxSize];
		if(eegdata[(m_Head+i)%m_MaxSize]<minv)
			minv = eegdata[(m_Head+i)%m_MaxSize];
	}
}

double ManagedData::MeanValue() const
{
	double sumv = 0.0;
	for (int i=0;i<datalength;i++)
	{
		sumv += eegdata[(i+m_Head)%m_MaxSize];
	}
	if(datalength>0)
		sumv = sumv/datalength;
	return sumv;
}

void ManagedData::SetData(const double d, const int size)
{
	int m_len = size + datalength;
	if (m_len > m_MaxSize)
	{
		int len = m_len%m_MaxSize;
		m_Head = (m_Head + len) % m_MaxSize;
		datalength = m_MaxSize;
	}
	else
		datalength += size;
	for (int i = 0; i < size; i++)
	{
		*(eegdata + m_Tail) = d;
		m_Tail = (++m_Tail) % m_MaxSize;
	}
}

//private function.
void ManagedData::SetData(const double* data, const int length)
{
	if (length > m_MaxSize)
	{
		int len = length%m_MaxSize;
		int count = length / m_MaxSize;
		m_Head = m_Tail = len;
		for (int i = 0; i < m_MaxSize; i++)
		{
			*(eegdata + m_Tail) = data[(count - 1)*m_MaxSize + len + i];
			m_Tail = (m_Tail + 1) % m_MaxSize;//mark
		}
		datalength = m_MaxSize;
	}
	else
	{
		for (int i = 0; i < length; i++)
		{
			*(eegdata + m_Tail) = data[i];
			m_Tail = (m_Tail + 1) % m_MaxSize;
		}
		datalength = length;
	}
}