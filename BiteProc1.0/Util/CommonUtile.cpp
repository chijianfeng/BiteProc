
#include "StdAfx.h"
#include "CommonUtile.h"
double UTILE::mean(const double d[],int len)
{
	if (len<=0) return -1;
	double sumv = 0.0;
	for (int i=0 ; i<len ; i++)
	{
		sumv += d[i];
		if (d[i]>10000000)
			cerr<<"mean err"<<endl;
	}
	return sumv/len;
}

void UTILE::absdata(double d[] , int len)
{
	if(len<=0||d==NULL)
		return;
	for(int i=0;i<len;i++)
	{
		d[i] = abs(d[i]);
	}
}

void UTILE::absdata(const double d[] , double out[] , int len)
{
	if(len<=0||out==NULL)
		return;
	for(int i=0;i<len;i++)
	{
		out[i] = abs(d[i]);
	}
}
double UTILE::stdval(const double d[],int len)
{
	if (len<=0) return -1;
	if (len==1) return 0;
	double meand = mean(d,len);
	double sumv = 0.0;
	for (int i=0;i<len;i++)
	{
		sumv += pow(d[i]-meand,2);
	}
	return sqrt(sumv/(len-1));
}

double UTILE::variance(const double d[] ,int len)
{
	if (len<=0) return -1;
	if (len==1) return 0;
	double meand = mean(d,len);
	double sumv = 0.0;
	for (int i=0;i<len;i++)
	{
		sumv += pow(d[i]-meand,2);
	}
	return sumv/(len-1);
}

double UTILE::maxdata(const double d[],int len)
{
	if (len<=0) return 0;
	double maxv = d[0];
	for (int i=1;i<len;i++)
	{
		maxv = max(maxv,d[i]);
	}
	return maxv;
}
double UTILE::mindata(const double d[],int len)
{
	if (len<=0) return 0;
	double minv = d[0];
	for (int i=1;i<len;i++)
	{
		minv = min(minv,d[i]);
	}
	return minv;
}
const int UTILE::finddata(const int srcdata[],int *dest,int len)
{
	/*
	find the seq 
	*/
	if (len<=0||dest==NULL)
		return 0;
	int count = 0;
	for (int i=0;i<len;i++)
	{
		if (srcdata[i]!=0)
		{
			dest[count] = i;
			count++;
		}
	}
	return count;
}
const int UTILE::largeseq(const double d[],int len,int *dest,const double s)
{
	/*
	this function is used for check the element large than the 
	*/
	if (len<=0||dest==NULL) return 0;
	int count = 0;
	for (int i=0;i<len;i++)
	{
		if ((d[i]-s)>=EPS)
		{
			dest[i] = 1;
			count++;
		}
		else
			dest[i] = 0;
	}
	return count;
}
const int UTILE::largeseq(const int d[],int len,int *dest,const double s)
{
	if (len<=0||dest==NULL) return 0;
	int count = 0;
	for (int i=0;i<len;i++)
	{
		if (d[i]>s)
		{
			dest[i] = 1;
			count++;
		}
		else
			dest[i] = 0;
	}
	return count;
}
const int UTILE::equalseq(const double d[],int len,int *dest,const double s)
{
	if (len<=0||dest==NULL) return 0;
	int count = 0;
	for (int i=0;i<len;i++)
	{
		if (fabs((d[i]-s))<EPS)
		{
			dest[i] = 1;
			count++;
		}
		else
			dest[i] = 0;
	}
	return count;
}
const int UTILE::equalseq(const int d[],int len ,int *dest,const int s)

{
	if (len<=0||dest==NULL) return 0;
	int count = 0;
	for (int i=0;i<len;i++)
	{
		if (d[i]==s)
		{
			dest[i] = 1;
			count++;
		}
		else
			dest[i] = 0;
	}
	return count;
}
const int UTILE::lessseq(const double d[],int len,int *dest,const double s)
{
	if (len<=0||dest==NULL) return 0;
	int count = 0;
	for (int i=0;i<len;i++)
	{
		if ((s-d[i])>=EPS)
		{
			dest[i] = 1;
			count++;
		}
		else
			dest[i] = 0;
	}
	return count;
}
const int UTILE::lessseq(const int d[],int len,int *dest,const int s)
{
	if (len<=0||dest==NULL) return 0;
	int count = 0;
	for (int i=0;i<len;i++)
	{
		if (s>d[i])
		{
			dest[i] = 1;
			count++;
		}
		else
			dest[i] = 0;
	}
	return count;
}
void UTILE::ORseq(int d[],int s[],int len)
{
	if (len<=0)
		return;
	for (int i=0;i<len;i++)
	{
		d[i] = d[i]|s[i];
	}
}

const int UTILE::firstpos(double d[],const int len,double compd)
{
	if(d==NULL) return -1;
	for (int i=0;i<len;i++)
	{
		if(fabs(d[i]-compd)<=EPS)
			return i;
	}
	return -1;
}

//
const int UTILE::findnum(point *p,int len,int seq[],int lens,int dis)
{
	///this function is used for find the location data block ,dis is the min distance.
	if (len<=0) return 0;
	int num = 1;
	p[0].s = seq[0];
	p[0].e = seq[lens-1];
	for (int i=1;i<len;i++)
	{
		if (seq[i-1]+dis>=seq[i])
			continue;
		else
		{
			p[num-1].e = seq[i-1];
			num++;
			p[num-1].s = seq[i];
			p[num-1].e = seq[lens-1];
		}
	}
	return num;
}
void UTILE::calmaxmin(const double src[],double *dest,const int len,const int winlen)
{
	if (len<=0) return;
	for (int i=0;i<len;i++)
	{
		if (i+winlen>=len)
		{
			dest[i] = src[i];
			continue;
		}
		dest[i] = maxdata(src+i,winlen)-mindata(src+i,winlen);
	}
}

UTILE::Processdata  UTILE::processdata(const double dl[],const double dr[],const double filtdl[],double filtdr[],const int len , Throld throld)
{
	Processdata pd;
	pd.lmaxmin = pd.rmaxmin = pd.stdlval = pd.stdrval = 0.0;
	if (len<=0) return pd;
	pd.lmaxmin = maxdata(dl,len)-mindata(dl,len);
	pd.rmaxmin = maxdata(dr,len)-mindata(dr,len);
	pd.stdlval = stdval(filtdl,len);
	pd.stdrval = stdval(filtdr,len);
	if (pd.lmaxmin<throld.voltvalue&&pd.stdlval<=throld.stdvalue)
		pd.stdlval = 0.0;
	if (pd.rmaxmin<throld.voltvalue&&pd.stdrval<=throld.stdvalue)
		pd.stdrval = 0.0;
	return pd;
}

void UTILE::smooth(const int dis,double **data,const int len)
{
	if (dis<=0||data==0||len<=dis) return;
	double *p = *data;
	int *dest = new int [len];
	int l = 170*256;
	int res = largeseq(p,len,dest,0);			//find the data > 0;
	if(!res) 
	{
		delete dest;
		dest = NULL;
		return;
	}
	int *src = new int[len];
	res = finddata(dest,src,len);
	int distance = 0;
	for (int i=1;i<res;i++)
	{
		if (src[i]==src[i-1]+1)
		{
			distance++;
			continue;
		}
		if (distance<dis)
		{
			memset((p+src[i-distance-1]),0,sizeof(double)*(distance+1));	
		}
		distance = 0;
	}
	delete dest;
	dest = 0;
	delete src;
	src = 0;
}

void UTILE::track( point **po, const int count, const double *pd, const int len ,const double volthrold)
{
	if (po==0||pd==0||count<=0||len<=0) return;
	point *p = *po;
	int *dest = new int[len];
	int *seq = new int [len];
	int *tmpseq = new int[len];
	int *tmpseq1 = new int[len];
	int *tmpdest = new int[len];
	for (int i=0;i<count;i++)
	{
		int res = lessseq(pd,len,dest,volthrold);
		if (res<=0) continue;
		res = finddata(dest,seq,len);							//find(data<throld)
		int res1 = lessseq(seq,res,tmpseq,p[i].s);
		int res2 = equalseq(seq,res,tmpseq1,p[i].s);
		ORseq(tmpseq,tmpseq1,res);						//a<=point(ii).start;
		int num = finddata(tmpseq,tmpdest,res);			//find(a<=pin....);
		if (num>0)
		{
			p[i].s = seq[tmpdest[num-1]];
		}
		else
			p[i].s = 0;
		//calculate the end.
		res1 = largeseq(seq,res,tmpseq,p[i].e);
		res2 = equalseq(seq,res,tmpseq1,p[i].e);
		ORseq(tmpseq,tmpseq1,res);
		num = finddata(tmpseq,dest,res);			//find(>=pin....);
		if (num>0)
		{
			p[i].e = seq[dest[0]];
		}
		else
			p[i].e = len-1;
	}
	delete dest;
	dest = 0;
	delete seq;
	seq = 0;
	delete tmpseq;
	tmpseq = 0;
	delete tmpseq1;
	tmpseq1 = 0;
	delete tmpdest;
}

UTILE::Throld UTILE::calthrold(const double rawdata[],const double filtdata[],int len,int winlen,int count,bool accure)
{
	/*
	check the peak not less than the count
	*/
	Throld throld;
	throld.voltvalue = throld.stdvalue = -1;
	if (len<=0||len<winlen||count<=0) return throld;
	double *tmpmaxmin = new double[len];
	double *tmpstd = new double[len];
	for (int i=0;i<len;i++)
	{
		if (i+winlen>=len-1)
		{
			tmpstd[i] = 0;
			tmpmaxmin[i] = 0;
			continue;
		}
		tmpstd[i] = floor(stdval(filtdata+i,winlen));
		tmpmaxmin[i] = floor(maxdata(rawdata+i,winlen+1)-mindata(rawdata+i,winlen+1));
	}
	double stdmean = mean(tmpstd,len);
	double volmean = mean(tmpmaxmin,len);
	double maxstd;
	double maxvol;
	int *seq = new int[len];
	int *dest = new int [len];
	point *pp = new point[count];
	for (int bitnum = 0;bitnum<count;bitnum++)
	{
		memset(seq,len,sizeof(int)*len);
		memset(dest,len,sizeof(int)*len);
		memset(pp,len,sizeof(int)*count);
		maxstd = maxdata(tmpstd,len);
		maxvol = maxdata(tmpmaxmin,len);
		int seqlen = equalseq(tmpmaxmin,len,seq,maxvol);
		int num = finddata(seq,dest,len);
		num = findnum(pp,count,dest,num,12);
		track(&pp,num,tmpmaxmin,len,volmean);
		for (int i=0;i<num;i++)
		{
			//memset(tmpmaxmin+pp[i].s,0.0,(pp[i].e-pp[i].s+1)*sizeof(double));
			//memset(tmpstd+pp[i].s,0.0,(pp[i].e-pp[i].s+1)*sizeof(double));
			for (int j=pp[i].s ; j<=pp[i].e  ; j++)
			{
				tmpmaxmin[j] = 0.0;
				tmpstd[j] = 0.0;
			}
		}
		bitnum = bitnum+num-1;
	}
	throld.stdvalue = (maxstd+stdmean)/2;
	throld.voltvalue = (maxvol+volmean)/2;
	if (accure)
	{
		int seqlen = largeseq(tmpmaxmin,len,seq,throld.voltvalue);
		int num = finddata(seq,dest,len);
		while(num>0)
		{
			if(throld.voltvalue<=volmean)
				break;
			maxstd = throld.stdvalue;
			maxvol = throld.voltvalue;
			throld.stdvalue = floor((throld.stdvalue+volmean)/2);
			throld.voltvalue = floor((throld.voltvalue+volmean)/2);
			seqlen = largeseq(tmpmaxmin,len,seq,throld.voltvalue);
			num = finddata(seq,dest,len);
		}
		throld.stdvalue = maxstd;
		throld.voltvalue = maxvol;
	}
	delete seq;
	delete dest;
	delete pp;
	delete tmpmaxmin;
	delete tmpstd;
	return throld;
}

UTILE::BpNetdata UTILE::extractnetdata(const double *rawdatal,const double *rawdatar , const int length ,Throld throld)
{
	BpNetdata data;
	data.LrationR = data.Lvalue = data.Rvalue = 0.0;
	if (rawdatal==NULL||rawdatar==NULL||length<=0) return data;
	data.Lvalue = mean(rawdatal,length)/throld.voltvalue;
	data.Rvalue = mean(rawdatar,length)/throld.voltvalue;
	if(data.Rvalue!=0)
		data.LrationR = data.Lvalue/data.Rvalue;
	else
		data.LrationR = 1;
	data.sp = data.sp = 0;
	return data;
}

//

void UTILE::removebaseline(double *pd,const int len)
{	
	if (len<=0||pd==NULL) return;
	for (int i=0;i<len-1;i++)
	{
		pd[i] = pd[i+1]-pd[i];
	}
}
const bool UTILE::Isequal(const double *pd,const int len,const int sp,const int ep)
{
	if (pd==NULL||len<=0||sp>ep||ep>=len) return false;
	double meanstd = stdval(pd,len);
	double midstd = stdval(pd+sp,ep-sp+1);
	if (meanstd/2<=midstd)
	{
		return true;
	}
	else
		return false;
}
const UTILE::BITESTATE UTILE::judge(const double *pd,const int rnum,const int cnum,const double maxr,const int winlen)
{
	/*
	pd is a rnum*cnum array.
	*/
	if (pd==NULL||rnum<=0&&cnum<=0&&maxr<=0)
	return (BITESTATE)0;
	int chan = 0;
	double maxv = 0;
	double minv = 0;
	int maxpos = 0;
	int minpos = 0;
	for (int i=0;i<rnum;i++)
	{
		double tmp = maxdata(pd,cnum);
		if (maxv<tmp)
		{
			chan = i;
			maxv = tmp;
		}
	}
	double *p = new double[cnum];
	memcpy(p,pd,cnum*sizeof(double));
	maxpos = firstpos(p,cnum,maxv);
	minv = mindata(p,cnum);
	minpos = firstpos(p,cnum,minv);					
	UTILE::BITESTATE result = BT_ONE;					//as the matlab code :res =1
	//then judge from the amx to the min data.

	for (int leftmove = maxpos-1;leftmove>=0;leftmove--)
	{
		if(leftmove-winlen>=1)
		{
			double maxd = maxdata(p+leftmove-winlen,winlen+1);
			double maxout = maxdata(p,leftmove-winlen-1);
			if(maxd*maxr<=maxout&&maxd*maxr<=maxv)
			{
				result = BT_TWO;
				return result;
			}
		}
	}
	for(int rightmove = maxpos+1;rightmove<cnum;rightmove++)
	{
		if(rightmove+winlen<=cnum-2)
		{
			double maxd = maxdata(p+rightmove,winlen+1);
			double maxout = maxdata(p+winlen+rightmove+1,cnum-winlen-rightmove-1);
			if(maxd*maxr<=maxout&&maxd*maxr<=maxv)
			{
				result = BT_TWO;
				return result;
			}
		}
	}
	for (int leftmove = minpos-1;leftmove>=0;leftmove--)
	{
		if(leftmove-winlen>=1)
		{
			double mind = fabs(mindata(p+leftmove-winlen,winlen+1));
			double minout = fabs(mindata(p,leftmove-winlen-1));
			if(mind*maxr<=minout&&Isequal(p,cnum,0,leftmove-winlen-1)&&mind*maxr<=fabs(minv))
			{
				result = BT_TWO;
				return result;
			}
		}
	}
	for (int rightmove = minpos+1;rightmove<cnum;rightmove++)
	{
		if(rightmove+winlen<=cnum-2)
		{
			double mind = fabs(mindata(p+rightmove,winlen+1));
			double minout = fabs(mindata(p+winlen+rightmove+1,cnum-winlen-rightmove-1));
			if(mind*maxr<=minout&&Isequal(p,cnum,winlen+rightmove+1,cnum-1)&&mind*maxr<=fabs(minv))
			{
				result = BT_TWO;
				return result;
			}
		}
	}
	return result;
}

UTILE::Throld UTILE::MaxThreshold(const Throld& t1 , const Throld& t2)
{
	Throld t; 
	t.stdvalue = max(t1.stdvalue ,t2.stdvalue);
	t.voltvalue = max(t1.voltvalue , t2.voltvalue);
	return t;
}

UTILE::Throld UTILE::MinThreshold(const Throld& t1 , const Throld& t2)
{
	Throld t; 
	t.stdvalue = min(t1.stdvalue ,t2.stdvalue);
	t.voltvalue = min(t1.voltvalue , t2.voltvalue);
	return t;
}

UTILE::Throld UTILE::MeanThreshold(const Throld& t1 , const Throld& t2)
{
	Throld t; 
	t.stdvalue = (t1.stdvalue +t2.stdvalue)/2;
	t.voltvalue = (t1.voltvalue , t2.voltvalue)/2;
	return t;
}
int UTILE::ExtracSample(double *Lrawd,double *Rrawd,double *Lfiltd,double *Rfiltd,UTILE::Throld throld,int len,UTILE::BpNetdata *pb)
{
	//first process data.
	int winlen = 50;
	double *datasetL = new double[len];
	double *datasetR = new double[len];
	double *newdataL = new double[len];
	double *newdataR = new double[len];
	//UTILE::BpNetdata data;
	UTILE::Processdata psd;
	int Sampleblock = 0;
	for (int i=0;i<len;i++)
	{
		if (i+winlen+1>=len)
		{
			newdataL[i] = datasetL[i] = 0.0;
			newdataR[i] = datasetR[i] = 0.0;
			continue;
		}
		psd = processdata(Lrawd+i,Rrawd+i,Lfiltd+i,Rfiltd+i,winlen+1,throld);
		datasetL[i] = psd.lmaxmin;
		datasetR[i] = psd.rmaxmin;
		newdataL[i] = psd.stdlval;
		newdataR[i] = psd.stdrval;
	}
	cout<<endl<<"first step ...process the data finished ...............,"<<endl;

	int smoothlen = 0;
	UTILE::smooth(smoothlen,&newdataL,len);
	UTILE::smooth(smoothlen,&newdataR,len);
	cout<<"smooth the process data....finished"<<endl;
	int distance = 0;
	for (int i=0;i<len;i++)
	{
		if (i+winlen>=len)
			break;
		if (newdataL[i]>0||newdataR[i]>0)
			distance++;
		else if(distance>0)
		{
			pb[Sampleblock] = UTILE::extractnetdata(datasetL+i-distance+1,datasetR+i-distance+1,distance,throld);
			pb[Sampleblock].sp = i-distance+1;
			pb[Sampleblock].ep = i-1;
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

double UTILE::sum(double *pd , int len)
{
	if (pd==NULL)
	{
		return 0;
	}
	double s = 0;
	for (int i=0;i<len ; i++)
	{
		s += pd[i];
	}
	return s;
}

UTILE::Processdata UTILE::MeanProcessData(const Processdata *pd , const int num)
{
	Processdata prod;
	prod.lmaxmin = prod.rmaxmin = prod.stdlval = prod.stdrval = 0;
	if(pd==NULL||num<=0)
		return prod;
	for (int i=0;i<num;i++)
	{
		prod.lmaxmin += pd[i].lmaxmin;
		prod.rmaxmin += pd[i].rmaxmin;
		prod.stdlval += pd[i].stdlval;
		prod.stdrval += pd[i].stdrval;
	}
	prod.lmaxmin = prod.lmaxmin/num;
	prod.rmaxmin = prod.rmaxmin/num;
	prod.stdlval = prod.stdlval/num;
	prod.stdrval = prod.stdrval/num;
	return prod;
}

int	UTILE::MergeBpData(const BpNetdata* d1 , const int num1 , const BpNetdata* d2 , const int num2 , BpNetdata *out)
{
	if(NULL==d1||NULL==d2||NULL==out)
		return 0;
	int num = 0;
	for (int i=0;i<num1;i++)
	{
		out[num].Lvalue = d1[i].Lvalue;
		out[num].Rvalue = d1[i].Rvalue;
		out[num].LrationR = d1[i].LrationR;
		out[num].sp = d1[i].sp;
		out[num].ep = d1[i].ep;
		num++;
	}
	for (int i=0;i<num2;i++)
	{
		out[num].Lvalue = d2[i].Lvalue;
		out[num].Rvalue = d2[i].Rvalue;
		out[num].LrationR = d2[i].LrationR;
		out[num].sp = d2[i].sp;
		out[num].ep = d2[i].ep;
		num++;
	}
	return num;
}

int UTILE::MeanBpdataLen(const BpNetdata* pd , int num)
{
	if(pd==NULL||num<=0)
		return 0;
	int sumv = 0;
	for (int i=0;i<num;i++)
	{
		sumv += pd[i].ep-pd[i].sp+1;
	}
	return sumv/num;
}

int UTILE::ZeroNum(double *pd ,int size)
{
	if(pd==NULL||size<=0)
		return 0;
	int num = 0;
	for (int i=0;i<size;i++)
	{
		if(*(pd+i)==0)
			num++;
	}
	return num;
}

int UTILE::ExtremMaxvalue(const double value[]  , const int len , double* out , int * index)
{
	if(out==NULL||index==NULL)
		return 0;
	int l = 0;
	for (int i=1;i<len;i++)
	{
		out[i-1] = value[i]-value[i-1];
		if(out[i-1]!=0)
			out[i-1] = out[i-1]/abs(out[i-1]);
	}
	out[len-1] = 0;
	int count = 0;
	for (int i=1;i<len-1;i++)
	{
		out[i-1] = out[i]-out[i-1];
		if(out[i-1]==-2)
		{
			index[count++] = i;
		}
	}
	for (int i=0;i<count;i++)
	{
		out[i] = value[index[i]];
	}

	return count;
}

int UTILE::ExtremMinvalue(const double value[] , const int len , double* out , int* index)
{
	if(out==NULL||index==NULL)
		return 0;
	int l = 0;
	for (int i=1;i<len;i++)
	{
		out[i-1] = value[i]-value[i-1];
		if(out[i-1]!=0)
			out[i-1] = out[i-1]/abs(out[i-1]);
	}
	out[len-1] = 0;
	int count = 0;
	for (int i=1;i<len-1;i++)
	{
		out[i-1] = out[i]-out[i-1];
		if(out[i-1]==2)
		{
			index[count++] = i;
		}
	}
	for (int i=0;i<count;i++)
	{
		out[i] = value[index[i]];
	}
	return count;

}

int UTILE::myinterp( double x[] ,  double y[] , const int len , const double orgx[],const int orgxlen , double *out)
{
	if(out==NULL) 
		return 0;
	int count = 1;
	int curpos = 0;
	for (int i=0;i<x[0]-1;i++)
	{
		out[i] = 0;
	}
	curpos=(int)(x[0]-1);
	for (count=1;count<len;count++)
	{
		for (;curpos<orgxlen;curpos++)
		{
			if(orgx[curpos]-1>=x[count-1]-1&&x[count]-1>orgx[curpos]-1)
			{
				out[curpos] = ((y[count]-y[count-1])*(orgx[curpos])+y[count-1]*x[count]-y[count]*x[count-1])/(x[count]-x[count-1]);
			}
			else
				break;
		}
	}
	for (int i=(int)x[len-1];i<orgxlen;i++)
	{
		out[i] = 0;
	}
	return orgxlen;
}

void UTILE::Evelop(const double x[] , const int xlen , double y[] , const int ylen,double *up , int& uplen , double *down , int& downlen)
{
	if(up==NULL||down==NULL)
		return;
	int *pmaxindex = new int[ylen];
	int *pminindex = new int[ylen];
	int maxlen = ExtremMaxvalue(y,ylen,up,pmaxindex);
	int minlen = ExtremMinvalue(y,ylen,down,pminindex);
	double *up_x = new double [maxlen];
	double *down_x = new double[minlen];
	double *uptmp = new double[maxlen];
	double *dowtmp = new double[minlen];
	for (int i=0;i<maxlen;i++)
	{
		up_x[i] = x[pmaxindex[i]];
		uptmp[i] = up[i];
	}
	for (int i=0;i<minlen;i++)
	{
		down_x[i] = x[pminindex[i]];
		dowtmp[i] = down[i];
	}
	int l = xlen;
	uplen = myinterp(up_x , uptmp,maxlen,x,(int)l,up);
	downlen = myinterp(down_x , dowtmp , minlen,x , (int)l , down);
	delete pminindex;
	delete pmaxindex;
	delete up_x;
	delete down_x;
	delete uptmp;
	delete dowtmp;
}

int UTILE::CalCrossPoint(const double pd[] , const int len , const double thres)
{
	if(pd==NULL)
		return 0;
	int num = 0;
	for (int i=0;i<len-1;i++)
	{
		if(CmpData(pd[i],thres)*CmpData(pd[i+1],thres)<0)
			num++;
		else if(CmpData(pd[i],thres)*CmpData(pd[i+1],thres)==0)
		{
			i++;
			num++;
		}
	}
	return num;
}

int UTILE::FindPeeks(const double *pd , const int len , bool b)
{
	if(pd==NULL)
		return 0;
	int count = 0;
	if(b)
	{
		for (int i=1;i<len-1;i++)
		{
			if(pd[i-1]<pd[i]&&pd[i]>pd[i+1])
				count++;
		}
	}
	else
	{
		for (int i=1;i<len-1;i++)
		{
			if(pd[i-1]>pd[i]&&pd[i]<pd[i+1])
				count++;
		}
	}
	return count;
}