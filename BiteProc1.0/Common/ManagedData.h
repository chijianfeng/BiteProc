/*
这个类主要是提供算法处理数据和基本操作。
类内部提供：
1.基本的数据（数据需要转换为uv值），和长度。
2.数据通道数，采样率。

3.数据的操作，比如获取某一通道的值。
*/
#pragma once
#ifndef _MANAGED_DATA_

#define _MANAGED_DATA_
#include <list>
#include <iostream>
using namespace std;
#define MAXHANDLEDATA 1280
#define MAXCHANNUM 20
#endif
class ManagedData
{
public:
	ManagedData(void);
	ManagedData(int size);											//设置缓存大小
	~ManagedData(void);
	ManagedData(const ManagedData& data);
	ManagedData& operator= (const ManagedData& data);

	double&	operator[] (int index);

	void	Zero(size_t len);										//初始化为长度为len的0值数组							
	
	double* GetData()const;											//获取数据
	int		GetDataSize()const;										//获取队列的数据长度
	void	SetChannum(const int channum);
	int		GetChannum() const;
	void	SetSampleRate(long rate);
	long	GetSampleRate()const;
	void	GetAnydata(double *temp,int chanserial,int length);		//获取某一通道的数据。
	void	GetAnydata(ManagedData& d , int chanserial);

	//add other operating function here.
	void	AppendData(ManagedData& m_data);						//在尾部添加数据,主要用在作为缓冲区操作。
	void	AppendData(const double*m_data, const int length);
	void	Remove(const int m_datalen);                            //在头部移除数据。

	void	GetData(ManagedData& data, size_t& len);				//实现从缓冲区获取数据。
	void	ResizeManageData(const int szie);						//重新分配缓存大小
	void	SetData(const double d , const int length);				//在尾部增加length长度的数据，将length的数据设置为d，	

	int		GetBufsize() const;										//获取缓存的大小

	int		CopyData(ManagedData& d , int len , bool isfront)const;						//return the copy number

	void	GetMaxMin(double& maxv , double& minx);
	
	double	MeanValue() const;

protected:
	double *eegdata;												//remember to release.
	int m_Head,m_Tail;												//循环链表实现快速插入，删除。
	int datalength;
	//UCHAR channum;
	int channum;
	long samplerate;
	int	 m_MaxSize;

public:
	void SetData(const double* pd, const int len);			
};
