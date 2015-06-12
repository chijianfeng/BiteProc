#include "StdAfx.h"
#include "FiltFilt.h"
#include <iostream>
using namespace std;
FiltFilt::FiltFilt(void)
{
	a = NULL;
	b = NULL;
	len = 0;
}

FiltFilt::~FiltFilt(void)
{
	if (a!=NULL)
	{
		delete a;
		a = NULL;
	}
	if (b!=NULL)
	{
		delete b;
		b = NULL;
	}
}

void FiltFilt::SetParam(const double pa[],const double pb[],int l)
{
	if (pa==NULL||pb==NULL)
		return;
	this->len = l;
	if (a==NULL)
	a = new double[len];
	if (b==NULL)
	b = new double[len];
	for (int i=0;i<len;i++)
	{
		a[i] = pa[i];
		b[i] = pb[i];
	}
}
void FiltFilt::useFFilt(double *xn,double *yn,int l)
{
	if (xn==NULL||yn==NULL)
		return;
	filtfilt(xn,yn,l,a,b,len);
}
int FiltFilt::rinv(double *a,int n)
{
	int *is,*js,i,j,k,l,u,v;
	double d,p;
	is=(int *)malloc(n*sizeof(int));
	js=(int *)malloc(n*sizeof(int));
	for (k=0; k<=n-1; k++)
	{ d=0.0;
	for (i=k; i<=n-1; i++)
		for (j=k; j<=n-1; j++)
		{ l=i*n+j; p=fabs(a[l]);
	if (p>d) { d=p; is[k]=i; js[k]=j;}
	}
	if (d+1.0==1.0)
	{	
		free(is); free(js); 
		printf("err**not inv\n");
		return(0);
	}
	if (is[k]!=k)
		for (j=0; j<=n-1; j++)
		{ u=k*n+j; v=is[k]*n+j;
	p=a[u]; a[u]=a[v]; a[v]=p;
	}
	if (js[k]!=k)
		for (i=0; i<=n-1; i++)
		{
			u=i*n+k; v=i*n+js[k];
			p=a[u]; a[u]=a[v]; a[v]=p;
		}
		l=k*n+k;
		a[l]=1.0/a[l];
		for (j=0; j<=n-1; j++)
			if (j!=k)
			{ u=k*n+j; a[u]=a[u]*a[l];}
			for (i=0; i<=n-1; i++)
				if (i!=k)
					for (j=0; j<=n-1; j++)
						if (j!=k)
						{ u=i*n+j;
			a[u]=a[u]-a[i*n+k]*a[k*n+j];
			}
			for (i=0; i<=n-1; i++)
				if (i!=k)
				{ u=i*n+k; a[u]=-a[u]*a[l];}
	}
	for (k=n-1; k>=0; k--)
	{ if (js[k]!=k)
	for (j=0; j<=n-1; j++)
	{ u=k*n+j; v=js[k]*n+j;
	p=a[u]; a[u]=a[v]; a[v]=p;
	}
	if (is[k]!=k)
		for (i=0; i<=n-1; i++)
		{ u=i*n+k; v=i*n+is[k];
	p=a[u]; a[u]=a[v]; a[v]=p;
	}
	}
	free(is); 
	free(js);
	return (1);
}
void FiltFilt::filter(const double* x, double* y, int xlen, double* a, double* b, int nfilt, double* zi)
{
	double tmp;
	int i,j;
	//normalization
	if((*a-1.0>EPS) || (*a-1.0<-EPS) )
	{
		tmp=*a;
		for(i=0;i<nfilt;i++)
		{
			b[i]/=tmp;
			a[i]/=tmp;
		}
	}
	memset(y,0,xlen*sizeof(double));//将y清零，以双浮点为单位
	a[0]=0.0;
	for(i=0;i<xlen;i++)
	{
		for(j=0;i>=j&&j<nfilt;j++){
			y[i] += (b[j]*x[i-j]-a[j]*y[i-j]);
		}
		if(zi&&i<nfilt-1) y[i] += zi[i];
	}
	a[0]=1.0;
}
void FiltFilt::trmul(double *a,double *b,double *c,int m,int n,int k)
{
	int i,j,l,u;
	for (i=0; i<=m-1; i++)
		for (j=0; j<=k-1; j++)
		{ u=i*k+j; c[u]=0.0;
	for (l=0; l<=n-1; l++)
		c[u]=c[u]+a[i*n+l]*b[l*k+j];
	}
	return;
}
int FiltFilt::filtfilt(double* x, double* y, int xlen, double* a, double* b, int nfilt)
{
	int nfact;
	int tlen;    //length of tx
	int i;
	double *tx,*tx1,*p,*t,*end;
	double *sp,*tvec,*zi;
	double tmp,tmp1;


	nfact=nfilt-1;    //3*nfact: length of edge transients

	if(xlen<=3*nfact || nfilt<2) return -1;    
	//too short input x or a,b
	//Extrapolate beginning and end of data sequence using a "reflection
	//method". Slopes of original and extrapolated sequences match at
	//the end points.
	//This reduces end effects.
	tlen=6*nfact+xlen;
	tx=(double *)malloc(tlen*sizeof(double));
	tx1=(double *)malloc(tlen*sizeof(double));


	sp=(double *)malloc( sizeof(double) * nfact * nfact );
	tvec=(double *)malloc( sizeof(double) * nfact );
	zi=(double *)malloc( sizeof(double) * nfact );


	if( !tx || !tx1 || !sp || !tvec || !zi ){
		free(tx);
		free(tx1);
		free(sp);
		free(tvec);
		free(zi);
		return 1;
	}

	tmp=x[0];
	for(p=x+3*nfact,t=tx;p>x;--p,++t) *t=2.0*tmp-*p;
	for(end=x+xlen;p<end;++p,++t) *t=*p;
	tmp=x[xlen-1];
	for(end=tx+tlen,p-=2;t<end;--p,++t) *t=2.0*tmp-*p;
	//now tx is ok.


	end = sp + nfact*nfact;
	p=sp;
	while(p<end) *p++ = 0.0L; //clear sp
	sp[0]=1.0+a[1];
	for(i=1;i<nfact;i++){
		sp[i*nfact]=a[i+1];
		sp[i*nfact+i]=1.0L;
		sp[(i-1)*nfact+i]=-1.0L;
	}


	for(i=0;i<nfact;i++){
		tvec[i]=b[i+1]-a[i+1]*b[0];
	}


	if(!rinv(sp,nfact)){
		free(zi);
		zi=NULL;
	}
	else{
		trmul(sp,tvec,zi,nfact,nfact,1);
	}//zi is ok


	free(sp);free(tvec);


	//filtering tx, save it in tx1
	tmp1=tx[0];
	if(zi)
		for( p=zi,end=zi+nfact; p<end;) *(p++) *= tmp1;
	filter(tx,tx1,tlen,a,b,nfilt,zi);


	//reverse tx1
	for( p=tx1,end=tx1+tlen-1; p<end; p++,end--){
		tmp = *p;
		*p = *end;
		*end = tmp;
	}


	//filter again
	tmp1 = (*tx1)/tmp1;
	if(zi)
		for( p=zi,end=zi+nfact; p<end;) *(p++) *= tmp1;
	filter(tx1,tx,tlen,a,b,nfilt,zi);


	//reverse to y
	end = y+xlen;
	p = tx+3*nfact+xlen-1;
	while(y<end){
		*y++ = *p--;
	}


	free(zi);
	free(tx);
	free(tx1);


	return 0;
}