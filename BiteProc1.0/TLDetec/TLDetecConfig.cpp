#include "StdAfx.h"
#include "TLDetecConfig.h"
#include "Stringoper.h"

static string Param_key[] = 
{
	"left end pos","right start pos",
	"ratio1" , "ratio2" , "ratio3",
	
};

CTLDetecConfig::CTLDetecConfig(void)
{
	this->pParam_Key = Param_key;
}


CTLDetecConfig::~CTLDetecConfig(void)
{
}

bool CTLDetecConfig::LoadArthConfig(const char *pPath)
{
	return BaseArthConfig::LoadArthConfig(pPath);
}

bool CTLDetecConfig::SaveArthConfig(const char* pPath)
{
	return BaseArthConfig::SaveArthConfig(pPath);
}

void CTLDetecConfig::Setleftend(int pos)
{
	string key = Param_key[0];
	Stringoper oper ; 
	m_Config[key] = oper.inttostring(pos);
}

int CTLDetecConfig::Getleftend()
{
	string key = Param_key[0];
	Stringoper oper ; 
	return oper.stringtoint(m_Config[key]);
}

void CTLDetecConfig::Setrightstart(int pos)
{
	string key = Param_key[1];
	Stringoper oper ; 
	m_Config[key] = oper.inttostring(pos);
}

int CTLDetecConfig::GetrightStart()
{
	string key = Param_key[1];
	Stringoper oper ; 
	return oper.stringtoint(m_Config[key]);
}

void CTLDetecConfig::SetRatio1(double d)
{
	string key = Param_key[2];
	Stringoper oper ; 
	m_Config[key] = oper.doubletostring(d);
}

double CTLDetecConfig::GetRatio1()
{
	string key = Param_key[2];
	Stringoper oper ; 
	return oper.stringtodouble(m_Config[key]);
}

void CTLDetecConfig::SetRatio2(double d)
{
	string key = Param_key[3];
	Stringoper oper ; 
	m_Config[key] = oper.doubletostring(d);
}

double CTLDetecConfig::GetRatio2()
{
	string key = Param_key[3];
	Stringoper oper ; 
	return oper.stringtodouble(m_Config[key]);
}

void CTLDetecConfig::SetRatio3(double d)
{
	string key = Param_key[4];
	Stringoper oper ; 
	m_Config[key] = oper.doubletostring(d);
}

double CTLDetecConfig::GetRatio3()
{
	string key = Param_key[4];
	Stringoper oper ; 
	return oper.stringtodouble(m_Config[key]);
}