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
	void	ProcessData(ManagedData& rawd, ManagedData& newd);			//对数据进行滤波
	int	    ProcessData(ManagedData &ldata, ManagedData &hdata, const BpNetdata* pL, int lnum, const BpNetdata* pH, 
						int hnum, Throld& L, Throld& H);				//计算轻重阈值
	int		ExtracSample(double *Lrawd, double *Rrawd, double *Lfiltd, double *Rfiltd, Throld throld,
						int len, BpNetdata *pb, Processdata *prod , int winlen);
	//提取咬牙范围，大小关系
	int		ExtracSample(ManagedData &rawd, ManagedData& newd, BpNetdata* pb, Processdata *prod , Throld , int);
	int		ExtracProcessData(ManagedData &rawd, ManagedData& newd, ManagedData& out, const int sp, const int ep,
							int winlen, Throld t);//获取咬牙段数据最大减最小值，标准差

	int		minpos(BpNetdata &d1, BpNetdata &d2, BpNetdata &d3);
	void	mixBpdata(BpNetdata *Ld, int numL, BpNetdata *Rd, int numR, BpNetdata *LRd, int numLR,
						Matrix &out, Matrix &target);
	void	mysort(double *pd, const int len);
}

#endif