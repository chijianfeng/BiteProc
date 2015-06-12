#include "StdAfx.h"
#include "SvmConfig.h"
#include "Stringoper.h"

static string Param_key[] = 
{
	"svm_type","kernel_type", 
	"degree" , "gamma" , "coef0" , "cost" , "nu" , "epsilon" , "cachesize",
	"eps" , "shrinking" , "probability_estimates" , "weight" , "weight_lable","nr_fold",
	"ModelPath"
};
CSvmConfig::CSvmConfig(void)
{
	this->pParam_Key = Param_key;
}


CSvmConfig::~CSvmConfig(void)
{
}

bool CSvmConfig::LoadArthConfig(const char *pPath)
{
	//add other code .
	return BaseArthConfig::LoadArthConfig(pPath);
}

bool CSvmConfig::SaveArthConfig(const char* pPath)
{
	return BaseArthConfig::SaveArthConfig(pPath);
}

void CSvmConfig::SetSvmType(int type)
{
	string key = Param_key[0];
	Stringoper oper ; 
	m_Config[key] = oper.inttostring(type);
}

int CSvmConfig::GetSvmType()
{
	string key = Param_key[0];
	Stringoper oper ; 
	return oper.stringtoint(m_Config[key]);
}

void CSvmConfig::SetSvmKernel(int type)
{
	string key = Param_key[1];
	Stringoper oper ; 
	m_Config[key] = oper.inttostring(type);
}

int	 CSvmConfig::GetSvmKernel()
{
	string key = Param_key[1];
	Stringoper oper ; 
	return oper.stringtoint(m_Config[key]);
}

void CSvmConfig::SetSvmDegree(int d)
{
	string key = Param_key[2];
	Stringoper oper ; 
	m_Config[key] = oper.inttostring(d);
}

int CSvmConfig::GetSvmDegree()
{
	string key = Param_key[2];
	Stringoper oper ; 
	return oper.stringtoint(m_Config[key]);
}

void CSvmConfig::SetSvmGamma(double g)
{
	string key = Param_key[3];
	Stringoper oper ; 
	m_Config[key] = oper.doubletostring(g);
}

double CSvmConfig::GetSvmGamma()
{
	string key = Param_key[3];
	Stringoper oper ; 
	return oper.stringtodouble(m_Config[key]);
}

void CSvmConfig::SetSvmCoef(double c)
{
	string key = Param_key[4];
	Stringoper oper ; 
	m_Config[key] = oper.doubletostring(c);
}

double CSvmConfig::GetSvmCoef()
{
	string key = Param_key[4];
	Stringoper oper ; 
	return oper.stringtodouble(m_Config[key]);
}

void CSvmConfig::SetSvmCost(double c)
{
	string key = Param_key[5];
	Stringoper oper ; 
	m_Config[key] = oper.doubletostring(c);
}

double CSvmConfig::GetSvmCost()
{
	string key = Param_key[5];
	Stringoper oper ; 
	return oper.stringtodouble(m_Config[key]);
}

void CSvmConfig::SetSvmNu(double n)
{
	string key = Param_key[6];
	Stringoper oper ; 
	m_Config[key] = oper.doubletostring(n);
}

double	CSvmConfig::GetSvmNu()
{
	string key = Param_key[6];
	Stringoper oper ; 
	return oper.stringtodouble(m_Config[key]);
}

void CSvmConfig::SetSvmEpsilon(double p)
{
	string key = Param_key[7];
	Stringoper oper ; 
	m_Config[key] = oper.doubletostring(p);
}

double CSvmConfig::GetSvmEpsilon()
{
	string key = Param_key[7];
	Stringoper oper ; 
	return oper.stringtodouble(m_Config[key]);
}

void CSvmConfig::SetSvmCachesize(double s)
{
	string key = Param_key[8];
	Stringoper oper ; 
	m_Config[key] = oper.doubletostring(s);
}

double CSvmConfig::GetSvmCachesize()
{
	string key = Param_key[8];
	Stringoper oper ; 
	return oper.stringtodouble(m_Config[key]);
}

void CSvmConfig::SetSvmEps(double e)
{
	string key = Param_key[9];
	Stringoper oper ; 
	m_Config[key] = oper.doubletostring(e);
}

double CSvmConfig::GetSvmEps()
{
	string key = Param_key[9];
	Stringoper oper ; 
	return oper.stringtodouble(m_Config[key]);
}

void CSvmConfig::SetSvmShrinking(int s)
{
	string key = Param_key[10];
	Stringoper oper ; 
	m_Config[key] = oper.inttostring(s);
}

int CSvmConfig::GetSvmShriking()
{
	string key = Param_key[10];
	Stringoper oper ; 
	return oper.stringtoint(m_Config[key]);
}

void CSvmConfig::SetSvmPro_est(int s)
{
	string key = Param_key[11];
	Stringoper oper ; 
	m_Config[key] = oper.inttostring(s);
}

int CSvmConfig::GetSvmPro_est()
{
	string key = Param_key[11];
	Stringoper oper ; 
	return oper.stringtoint(m_Config[key]);
}

void CSvmConfig::SetSvmWeight(const double *w , const int len)
{
	if(w==NULL)
		return;
	Streamoper op1;
	double* pd = new double[len];
	for (int i=0;i<len;i++)
	{
		pd[i] = w[i];
	}
	string tmp = op1.SerizlizeStream(pd , len);
	string key = Param_key[12];
	m_Config[key] = tmp;
	delete pd;
}

int CSvmConfig::GetSvmWeight(double *w)
{
	if(w==NULL)
		return 0;
	string key = Param_key[12];
	string tmp = m_Config[key];
	Streamoper oper;
	oper.InitDeseri(tmp);
	int len = 0;
	oper.DeserizlizeStream(w, len);
	return len;
}

void CSvmConfig::SetSvmWeightLable(const int *w , const int len)
{
	if(w==NULL)
		return;
	Streamoper op1;
	int* pd = new int[len];
	for (int i=0;i<len;i++)
	{
		pd[i] = w[i];
	}
	string tmp = op1.SerizlizeStream(pd , len);
	string key = Param_key[13];
	m_Config[key] = tmp;
	delete pd;
}

int CSvmConfig::GetSvmWeightLable(int *w)
{
	if(w==NULL)
		return 0;
	string key = Param_key[13];
	string tmp = m_Config[key];
	Streamoper oper;
	oper.InitDeseri(tmp);
	int len = 0;
	oper.DeserizlizeStream(w ,len , false );
	return len;
}

void CSvmConfig::SetFlods(int n)
{
	string key = Param_key[14];
	Stringoper oper ; 
	m_Config[key] = oper.inttostring(n);
}

int CSvmConfig::GetFlods()
{
	string key = Param_key[14];
	Stringoper oper ; 
	return oper.stringtoint(m_Config[key]);
}

void CSvmConfig::SetModelPath(const char* path)
{
	if(path==NULL)
		return;
	string key = Param_key[15];
	Stringoper oper ;
	char tmp[1024];
	strcpy_s(tmp , path);
	m_Config[key] = oper.charstostring(tmp);
}

string CSvmConfig::GetModelPath()
{
	string key = Param_key[15];
	Stringoper oper;
	return (m_Config[key]);
}