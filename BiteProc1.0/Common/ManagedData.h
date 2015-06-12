/*
�������Ҫ���ṩ�㷨�������ݺͻ���������
���ڲ��ṩ��
1.���������ݣ�������Ҫת��Ϊuvֵ�����ͳ��ȡ�
2.����ͨ�����������ʡ�

3.���ݵĲ����������ȡĳһͨ����ֵ��
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
	ManagedData(int size);											//���û����С
	~ManagedData(void);
	ManagedData(const ManagedData& data);
	ManagedData& operator= (const ManagedData& data);

	double&	operator[] (int index);

	void	Zero(size_t len);										//��ʼ��Ϊ����Ϊlen��0ֵ����							
	
	double* GetData()const;											//��ȡ����
	int		GetDataSize()const;										//��ȡ���е����ݳ���
	void	SetChannum(const int channum);
	int		GetChannum() const;
	void	SetSampleRate(long rate);
	long	GetSampleRate()const;
	void	GetAnydata(double *temp,int chanserial,int length);		//��ȡĳһͨ�������ݡ�
	void	GetAnydata(ManagedData& d , int chanserial);

	//add other operating function here.
	void	AppendData(ManagedData& m_data);						//��β���������,��Ҫ������Ϊ������������
	void	AppendData(const double*m_data, const int length);
	void	Remove(const int m_datalen);                            //��ͷ���Ƴ����ݡ�

	void	GetData(ManagedData& data, size_t& len);				//ʵ�ִӻ�������ȡ���ݡ�
	void	ResizeManageData(const int szie);						//���·��仺���С
	void	SetData(const double d , const int length);				//��β������length���ȵ����ݣ���length����������Ϊd��	

	int		GetBufsize() const;										//��ȡ����Ĵ�С

	int		CopyData(ManagedData& d , int len , bool isfront)const;						//return the copy number

	void	GetMaxMin(double& maxv , double& minx);
	
	double	MeanValue() const;

protected:
	double *eegdata;												//remember to release.
	int m_Head,m_Tail;												//ѭ������ʵ�ֿ��ٲ��룬ɾ����
	int datalength;
	//UCHAR channum;
	int channum;
	long samplerate;
	int	 m_MaxSize;

public:
	void SetData(const double* pd, const int len);			
};
