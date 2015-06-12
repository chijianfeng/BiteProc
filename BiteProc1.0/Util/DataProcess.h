#pragma once
#ifndef _DATA_PROCESS_
#define _DATA_PROCESS_

#include <iostream>
#include <math.h>
#include "CommonUtile.h"
#include "ManagedData.h"
using namespace std;
using namespace UTILE;
namespace DATAPROCESS
{
	void	ProcessData(ManagedData& rawd, ManagedData& newd);			//�����ݽ����˲�
	int	    ProcessData(ManagedData &ldata, ManagedData &hdata, const BpNetdata* pL, int lnum, const BpNetdata* pH, 
						int hnum, Throld& L, Throld& H);				//����������ֵ
	int		ExtracSample(double *Lrawd, double *Rrawd, double *Lfiltd, double *Rfiltd, Throld throld,
						int len, BpNetdata *pb, Processdata *prod , int winlen);
	//��ȡҧ����Χ����С��ϵ
	int		ExtracSample(ManagedData &rawd, ManagedData& newd, BpNetdata* pb, Processdata *prod , Throld , int);
	int		ExtracProcessData(ManagedData &rawd, ManagedData& newd, ManagedData& out, const int sp, const int ep,
							int winlen, Throld t);//��ȡҧ��������������Сֵ����׼��

	int		minpos(BpNetdata &d1, BpNetdata &d2, BpNetdata &d3);
	void	mixBpdata(BpNetdata *Ld, int numL, BpNetdata *Rd, int numR, BpNetdata *LRd, int numLR,
						Matrix &out, Matrix &target);
	void	mysort(double *pd, const int len);
}

#endif