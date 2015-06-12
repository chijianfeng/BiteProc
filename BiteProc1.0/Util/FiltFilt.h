/*
this program is used for Filt the data. this function is used the param b,and a
*/
#pragma once
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#define EPS 0.000001
class FiltFilt
{
public:
	FiltFilt(void);
	virtual ~FiltFilt(void);
	void SetParam(const double pa[],const double pb[],int len);			//…Ë÷√≤Œ ˝
	void useFFilt(double *xn,double *yn,int len);
private:
	//private function
	void trmul(double *a,double *b,double *c,int m,int n,int k);
	void filter(const double* x, double* y, int xlen, double* a, double* b, int nfilt, double* zi);
	int rinv(double *a,int n);
	int filtfilt(double* x, double* y, int xlen, double* a, double* b, int nfilt);
private:
	double *b;
	double *a;
	int len;
};

