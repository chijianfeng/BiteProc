/*
提供一般数据操作功能：
*/
#ifndef _COMMON_UTILE_
#define _COMMON_UTILE_
#include <iostream>
#include <math.h>
using namespace std;
namespace UTILE
{
#define EPS 0.000001					//用于double类型比较大小
enum BITESTATE{							//判断咬牙好坏标志，未用到
	BT_ONE=1,BT_TWO,BT_LONG,
	BT_SMALL=10, BT_GOOD,BT_LARGE,
};
struct Throld							//咬牙结构体
{
	double stdvalue;					//数据标准差
	double voltvalue;					//最大减去最小值
};
/*
this struct is used for record the processdata 
*/
struct Processdata		
{
	double lmaxmin;						//左侧最大减去最小值
	double rmaxmin;						//右侧最大减去最小值
	double stdlval;						//左侧标准差
	double stdrval;						//右侧标准差
};
struct point						
{
	int s;
	int e;
};
struct BpNetdata						
{
	double Lvalue;		//左侧数据均值
	double Rvalue;		//右侧数据均值
	double LrationR;	//Lvalue/Rvalue
	int sp;				//咬牙起始点
	int ep;				//咬牙末尾
};
struct Matrix			//未用
{
	double **pdata;
	int raw;
	int colnum;
};

enum _BITESTATE			//咬牙种类
{
	NONEBITE = 0,
	ONEBITE = 1,
	TWOBITE = 2,
	LONGBITE
};
//basic function

template<typename T>
int CmpData(T d1 , T d2)
{
	if(d1>d2)
		return 1;
	else if(d1==d2)
		return 0;
	else
		return -1;
}

double sum(double *pd , int len);

double mean(const double d[],int len);

void absdata(double d[] , int len);						//数组绝对值

void absdata(const double d[] , double out[] , int len);

double stdval(const double d[],int len);				//标准差

double variance(const double d[] ,int len);				//方差

double maxdata(const double d[],int len);				//求最大值

double mindata(const double d[],int len);				//求最小值

//寻找非0的连续段，返回值为段数
const int finddata(const int seq[],int *dest,int len);

const int largeseq(const double d[],int len,int *dest,const double s);//mark the data which large than s.in 1,or in 0.

const int largeseq(const int d[],int len,int *dest,const double s);

const int equalseq(const double d[],int len,int *dest,const double s);

const int equalseq(const int d[]/*data array*/,int len/*array length*/,int *dest/*dest array*/,const int s/*value*/);

const int lessseq(const double d[],int len,int *dest,const double s);

const int lessseq(const int d[],int len,int *dest,const int s);

void ORseq(int d[],int s[],int len);							//数组序列进行或操作

const int firstpos(double d[],const int len,double compd);		//return the first data equal the cmpd or return -1.


//
const int findnum(point *p,int len,int seq[],int lens,int dis);

void calmaxmin(const double src[],double *dest,const int len,const int winlen);//dismatch the result of matlab

Processdata processdata(const double dl[],const double dr[],const double filtdl[],double filtdr[],const int len, Throld throld);

/*this function is used for remove the small block dataset , which the length is small than dis
*/
void smooth(const int dis,double **data , const int len);

//this function is used for set the data to zero whilc the maxmin value is large than the volthrold.
void track(point **po, const int count, const double *pd, const int len ,const double volthrold);

UTILE::BpNetdata extractnetdata(const double *rawdatal,const double *rawdatar , const int length ,Throld throld);//extract the data and normalize it.

Throld calthrold(const double rawdata[],const double filtdata[],int len,int winlen,int count,bool accure);

//these functions are for test the check two or one bite....

void removebaseline(double *pd,const int len);		//remove the base line shift.

const bool Isequal(const double *pd,const int len,const int sp,const int ep);		//compare is the change vol is almost equal.

const BITESTATE judge(const double *pd,const int rnum,const int cnum,const double maxr,const int winlen);		//check the data is bite one or two..

Throld MaxThreshold(const Throld& t1 , const Throld& t2);		//比较两个阈值

Throld MinThreshold(const Throld& t1 , const Throld& t2);

Throld	MeanThreshold(const Throld& t1 , const Throld& t2);

Processdata MeanProcessData(const Processdata *pd  , const int num);		//对num个Processdata求平均


int ExtracSample(double *Lrawd,double *Rrawd,double *Lfiltd,double *Rfiltd,UTILE::Throld throld,int len,UTILE::BpNetdata *pb);

int	MergeBpData(const BpNetdata* d1 , const int num1 , const BpNetdata* d2 , const int num2 , BpNetdata *out); //merge the d1 and d2 to out data set.

int	MeanBpdataLen(const BpNetdata* pd , int num);

int	ZeroNum(double *pd ,int size);				//将pd置为0

int ExtremMaxvalue(const double value[]  , const int len , double* out , int* index);

int ExtremMinvalue(const double value[]  , const int len , double* out , int* index);

int myinterp( double  x[] ,  double y[] , const int len , const double orgx[] ,const int orgxlen , double *out);

void Evelop(const double x[] , const int xlen , double y[] , const int ylen,double *up , int& uplen , double *down , int& downlen);

/*
calculate the pass point.
*/
int CalCrossPoint(const double pd[] , const int len , const double thres) ;

int FindPeeks(const double *pd , const int len , bool b);		//if b==true return the max value.or return the min value
}
#endif