
#include "StdAfx.h"
#include <vector>
#include <algorithm>
#include "DataProcess.h"
#include "FiltFilt.h"
#include "Structdefine.h"

void DATAPROCESS::ProcessData(ManagedData& rawd, ManagedData& newd)
{
	FiltFilt	ff;
	ff.SetParam(a_3_100, b_3_100, 7);								//设置滤波参数
	double** pd = NULL;
	pd = new double*[rawd.GetChannum()];
	
	for (int i = 0; i < rawd.GetChannum(); i++)
	{
		ManagedData d;
		pd[i] = new double[rawd.GetDataSize() / rawd.GetChannum()];		//加载每个通道数据
		rawd.GetAnydata(d, i);
		ff.useFFilt(d.GetData(), pd[i], d.GetDataSize());				//滤波
	}
	newd.SetSampleRate(rawd.GetSampleRate());
	newd.SetChannum(rawd.GetChannum());
	newd.Zero(rawd.GetDataSize());										//初始化为0
	for (int i = 0; i < rawd.GetDataSize() / rawd.GetChannum(); i++)
	{
		for (int j = 0; j < rawd.GetChannum();j++)
		{
			newd[i*rawd.GetChannum() + j] = pd[j][i];
		}
	}
	for (int i = 0; i < rawd.GetChannum(); i++)
	{
		delete pd[i];
	}
	delete pd;
}

int DATAPROCESS::ProcessData(ManagedData &ldata, ManagedData &hdata, const BpNetdata* pL, int lnum, const BpNetdata* pH, int hnum, Throld& L, Throld& H)
{
	if (ldata.GetDataSize() <= 0 || hdata.GetDataSize() <= 0)
		return -1;
	if (pL == NULL || pH == NULL)
		return -1;
	double Lmaxmin[1000];
	double Hmaxmin[1000];	//tmp buffer.
	double* tmp = new double[ldata.GetDataSize() / ldata.GetChannum()];
	ManagedData left, right;
	ldata.GetAnydata(left, 0);
	ldata.GetAnydata(right, 1);
	for (int i = 0; i < lnum; i++)
	{
		double lv = abs(abs(maxdata(left.GetData() + pL[i].sp, pL[i].ep - pL[i].sp + 1)) -
			abs(mindata(left.GetData() + pL[i].sp, pL[i].ep - pL[i].sp + 1)));
		double rv = abs(abs(maxdata(right.GetData() + pL[i].sp, pL[i].ep - pL[i].sp + 1)) -
			abs(mindata(right.GetData() + pL[i].sp, pL[i].ep - pL[i].sp + 1)));
		//absdata(left.GetData()+pL[i].sp,tmp,pL[i].ep-pL[i].sp+1);
		//double lv = mean(tmp , pL[i].ep-pL[i].sp+1);
		//absdata(right.GetData()+pL[i].sp,tmp,pL[i].ep-pL[i].sp+1);
		//double rv = mean(tmp , pL[i].ep-pL[i].sp+1);
		Lmaxmin[i] = max(lv, rv);
	}
	left.Remove(left.GetDataSize() + 1);		//remove all.
	right.Remove(right.GetDataSize() + 1);
	hdata.GetAnydata(left, 0);
	hdata.GetAnydata(right, 1);
	for (int i = 0; i < hnum; i++)
	{
		double lv = abs(abs(maxdata(left.GetData() + pH[i].sp, pH[i].ep - pH[i].sp + 1)) - 
			abs(mindata(left.GetData() + pH[i].sp, pH[i].ep - pH[i].sp + 1)));
		double rv = abs(abs(maxdata(right.GetData() + pH[i].sp, pH[i].ep - pH[i].sp + 1)) - 
			abs(mindata(right.GetData() + pH[i].sp, pH[i].ep - pH[i].sp + 1)));
		//absdata(left.GetData()+pL[i].sp,tmp,pH[i].ep-pH[i].sp+1);
		//double lv = mean(tmp , pL[i].ep-pL[i].sp+1);
		//absdata(right.GetData()+pL[i].sp,tmp,pH[i].ep-pH[i].sp+1);
		//double rv = mean(tmp , pL[i].ep-pL[i].sp+1);
		Hmaxmin[i] = max(lv, rv);
	}
	//sort from the small to large.
	mysort(Lmaxmin, lnum);
	mysort(Hmaxmin, hnum);

	double lmax = Lmaxmin[lnum - 1];		//get the light min data.
	int count;
	for (count = 0; count < hnum; count++)
	{
		if (lmax<Hmaxmin[count])
			break;
	}
	L.stdvalue = 0;
	L.voltvalue = min(lmax, Hmaxmin[0]);
	H.stdvalue = 0;
	H.voltvalue = max(lmax, Hmaxmin[0]);
	delete tmp;
	if (count>0)
		return count + 1;
	else
		return count;
}

void DATAPROCESS::mysort(double *pd, const int len)
{
	if (pd == NULL || len <= 0)
		return;
	vector<double> vc;
	for (int i = 0; i < len; i++)
	{
		vc.push_back(pd[i]);
	}
	sort(vc.begin(), vc.end(), less<double>());
	for (int i = 0; i < len; i++)
	{
		pd[i] = vc.at(i);
	}
}

int DATAPROCESS::ExtracSample(ManagedData &rawd, ManagedData& newd, BpNetdata* pb, Processdata *prod , Throld t , int winlen)
{
	if (pb == NULL || rawd.GetDataSize() != newd.GetDataSize())
		return 0;
	ManagedData rl, rr, nl, nr;
	rl.ResizeManageData(rawd.GetDataSize() / rawd.GetChannum() + 1);
	rr.ResizeManageData(rawd.GetDataSize() / rawd.GetChannum() + 1);
	nl.ResizeManageData(newd.GetDataSize() / newd.GetChannum() + 1);
	nr.ResizeManageData(newd.GetDataSize() / newd.GetChannum() + 1);
	rawd.GetAnydata(rl, 0);
	rawd.GetAnydata(rr, 1);
	newd.GetAnydata(nl, 0);
	newd.GetAnydata(nr, 1);
	return ExtracSample(rl.GetData(), rr.GetData(), nl.GetData(), nr.GetData(), t, rl.GetDataSize(), pb, prod , winlen);
}

int DATAPROCESS::ExtracSample(double *Lrawd, double *Rrawd, double *Lfiltd, double *Rfiltd, Throld throld, 
	int len, BpNetdata *pb, Processdata *prod , int winlen)
{
	//first process data.
	double *datasetL = new double[len];
	double *datasetR = new double[len];
	double *newdataL = new double[len];
	double *newdataR = new double[len];
	//BpNetdata data;
	Processdata psd;
	int Sampleblock = 0;
	for (int i = 0; i < len; i++)
	{
		if (i + winlen + 1 >= len)
		{
			psd = processdata(Lrawd + i, Rrawd + i, Lfiltd + i, Rfiltd + i, len - i - 1, throld);
		}
		else
			psd = processdata(Lrawd + i, Rrawd + i, Lfiltd + i, Rfiltd + i, winlen + 1, throld);
		datasetL[i] = psd.lmaxmin;
		datasetR[i] = psd.rmaxmin;
		newdataL[i] = psd.stdlval;
		newdataR[i] = psd.stdrval;
	}

	int smoothlen = 0;
	smooth(smoothlen, &newdataL, len);
	smooth(smoothlen, &newdataR, len);
	int distance = 0;
	for (int i = 0; i<len; i++)
	{
		if (newdataL[i]>0 || newdataR[i] > 0)
			distance++;
		else if (distance > 0)
		{
			pb[Sampleblock] = extractnetdata(datasetL + i - distance + 1, datasetR + i - distance + 1, distance, throld);
			pb[Sampleblock].sp = i - distance;
			pb[Sampleblock].ep = i;
			prod[Sampleblock].lmaxmin = prod[Sampleblock].rmaxmin = 0;
			prod[Sampleblock].stdlval = mean(newdataL + i - distance + 1, distance);
			prod[Sampleblock].stdrval = mean(datasetR + i - distance + 1, distance);
			Sampleblock++;
			distance = 0;
		}
	}
	delete newdataR;
	delete newdataL;
	delete datasetL;
	delete datasetR;
	return Sampleblock;
}

int DATAPROCESS::ExtracProcessData(ManagedData &rawd, ManagedData& newd, ManagedData& out, 
	const int sp, const int ep , int winlen , Throld t)
{
	ManagedData rl, rr, nl, nr;
	rawd.GetAnydata(rl, 0);
	rawd.GetAnydata(rr, 1);
	newd.GetAnydata(nl, 0);
	newd.GetAnydata(nr, 1);
	int len = rl.GetDataSize();
	Processdata psd;
	double *Lrawd = rl.GetData();
	double *Rrawd = rr.GetData();
	double *Lfiltd = nl.GetData();
	double *Rfiltd = nr.GetData();
	double *pd = new double[(ep - sp + 1) * 4];
	for (int i = sp; i < ep + 1; i++)
	{
		if (i + winlen + 1 >= len)
		{
			continue;
		}
		psd = processdata(Lrawd + i, Rrawd + i, Lfiltd + i, Rfiltd + i, winlen + 1, t);
		pd[(i - sp) * 4] = psd.lmaxmin;
		pd[(i - sp) * 4 + 1] = psd.rmaxmin;
		pd[(i - sp) * 4 + 2] = psd.stdlval;
		pd[(i - sp) * 4 + 3] = psd.stdrval;
	}
	out.SetChannum(4);
	out.SetSampleRate(rawd.GetSampleRate());
	out.SetData(pd, (ep - sp + 1) * 4);
	return ep - sp + 1;
}

int DATAPROCESS::minpos(BpNetdata &d1, BpNetdata &d2, BpNetdata &d3)
{
	int res = 0;
	if (d1.sp <= d2.sp&&d1.sp <= d3.sp)
		return 1;
	if (d1.sp > d2.sp&&d2.sp <= d3.sp)
		return 2;
	if (d1.sp > d3.sp&&d3.sp < d2.sp)
		return 3;
	return res;
}

void DATAPROCESS::mixBpdata(BpNetdata *Ld, int numL, BpNetdata *Rd, int numR, BpNetdata *LRd, int numLR, Matrix &out, Matrix &target)
{
	int len = numLR + numL + numR;
	int count = 0;
	int pL(0), pR(0), pLR(0);
	while (count < len)
	{
		int pos = 0;
		if (pL < numL&&pR < numR&&pLR < numLR)
			pos = DATAPROCESS::minpos(Ld[pL], Rd[pR], LRd[pLR]);
		else
		{
			if (pL < numL)
				pos = 1;
			else if (pR < numR)
				pos = 2;
			else if (pLR < numLR)
				pos = 3;
		}
		if (pos == 1)
		{
			out.pdata[0][count] = Ld[pL].Lvalue;
			out.pdata[1][count] = Ld[pL].Rvalue;
			out.pdata[2][count] = Ld[pL].LrationR;
			target.pdata[0][count] = 0.9;
			pL++;
			count++;
		}
		else if (pos == 2)
		{
			out.pdata[0][count] = Rd[pR].Lvalue;
			out.pdata[1][count] = Rd[pR].Rvalue;
			out.pdata[2][count] = Rd[pR].LrationR;
			target.pdata[0][count] = -0.9;
			pR++;
			count++;

		}
		else if (pos == 3)
		{
			out.pdata[0][count] = LRd[pLR].Lvalue;
			out.pdata[1][count] = LRd[pLR].Rvalue;
			out.pdata[2][count] = LRd[pLR].LrationR;
			target.pdata[0][count] = 0;
			pLR++;
			count++;
		}
	}
}