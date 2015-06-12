#include "StdAfx.h"
#include "AlgParamConfig.h"
#include "Stringoper.h"

const char PARAM_COMMENT = '%';
const string Param_key[] = {/*4个一组*/
	/*1*/"Bite IsTrain", "Bite threshold path", "Bite Train time", "Bite count",
	/*2*/"Bite Data Path", "Bite Throld Module Path", "Classify IsTrain", "Classify Model Path",
	/*3*/"Classify Train Time", "Classify ReTrain", "Classify Left Data Path", "Classify Both Data Path",
	/*4*/"Classify Right Data Path", "Classify Module Path", "HL IsTrain", "HL Train time",
	/*5*/"HL ReTrain", "HL Model Path", "HL Module Path", "HL Left Light Data Path",
	/*6*/"HL Left Heavy Data Path", "HL Right Light Data Path", "HL Right Heavy Data Path", "HL Both Light Data Path",
	/*7*/"HL Both Heavy Data Path", "TL IsTrain", "Two Bite Train Time", "TL Left param Path",
	/*8*/"TL Both Param path", "TL Right Param Path", "TL Left Data Path", "TL Both Data Path",
	/*9*/"TL Right Data Path", "TL Left Model Path", "TL Both Model Path", "TL Right Model Path",
	/*10*/"TL Module Path", "TL ReTrain", "Window Length", "OverLapped Window Length",
	/*11*/"Left Throld", "Right Throld",
};
const size_t NUMBER = 42;
#ifndef BLOCK
#define BLOCK
#endif // !BLOCK

CAlgParamConfig::CAlgParamConfig(void)
{
	m_bIsopen = false;
	m_Config.clear();
}

CAlgParamConfig::CAlgParamConfig(CString path)
{
	m_bIsopen = m_nFile.Open(path, CFile::modeReadWrite); 
	m_Config.clear();
}

CAlgParamConfig::~CAlgParamConfig(void)
{
}
bool CAlgParamConfig::IsOpen()
{
	return m_bIsopen==TRUE?true:false;
}
bool CAlgParamConfig::OpenParamFile(CString path)
{
	if(!m_bIsopen)
		m_bIsopen = m_nFile.Open(path , CFile::modeReadWrite);
	m_Config.clear();
	return m_bIsopen==TRUE?true:false;
}

bool CAlgParamConfig::LoadParam()
{
	if(!m_bIsopen)
		return false;
	m_nFile.SeekToBegin();
	string line, key, value;
	CString tmp;
	while(m_nFile.ReadString(tmp))
	{
		Stringoper oper;
		line = oper.tostring(tmp);
		if(AnalyseLine(line , key  , value))
		{
			m_Config[key] = value;
		}
	}
	return true;
}

bool CAlgParamConfig::SaveParam()
{
	if(!m_bIsopen)
		return false;
	m_nFile.SeekToBegin();
	m_nFile.SetLength(0);
	for (size_t i=0 ; i<NUMBER;i++)
	{
		string s = Param_key[i]+" "+"="+" "+
			m_Config[Param_key[i]]+"\n";
		m_nFile.Write(s.c_str() , s.size());
	}
	return true;
}
bool CAlgParamConfig::CloseParmFile()
{
	m_nFile.Close();
	m_bIsopen = false;
	return true;
}

#ifdef BLOCK

void CAlgParamConfig::SetBiteState(const bool s)
{
	string key = Param_key[0];
	Stringoper oper ; 
	m_Config[key] = oper.booltostring(s);
}

bool CAlgParamConfig::GetBitestate()
{
	string key = Param_key[0];
	Stringoper oper;
	return oper.stringtobool(m_Config[key]);
}

void CAlgParamConfig::SetBiteThorldModel(const CString fpath)
{
	string key = Param_key[1];
	Stringoper oper;
	m_Config[key]  = oper.tostring(fpath);
}

CString CAlgParamConfig::GetBiteThroldModel()
{
	string key = Param_key[1];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetBiteTrain(const int t ,const int count)
{
	string key = Param_key[2];
	Stringoper oper;
	m_Config[key] = oper.inttostring(t);
	key = Param_key[3];
	m_Config[key] = oper.inttostring(count);
}

void CAlgParamConfig::GetBiteTrain(int& t , int& count)
{
	string key = Param_key[2];
	Stringoper oper ; 
	t = oper.stringtoint(m_Config[key]);
	key = Param_key[3];
	count = oper.stringtoint(m_Config[key]);
}

void CAlgParamConfig::SetBiteDataPath(CString path)
{
	string key = Param_key[4];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetBiteDataPath()
{
	string key = Param_key[4];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetBiteModulePath(CString path)
{
	string key = Param_key[5];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetBiteModulePath()
{
	string key = Param_key[5];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

#endif

#ifdef BLOCK
void CAlgParamConfig::SetClassifyState(const bool s)
{
	string key = Param_key[6];
	Stringoper oper;
	m_Config[key] = oper.booltostring(s);
}

bool CAlgParamConfig::GetClassifystate()
{
	string key = Param_key[6];
	Stringoper oper;
	return oper.stringtobool(m_Config[key]);
}

void CAlgParamConfig::SetClassifyModel(const CString path)
{
	string key = Param_key[7];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetClassifyModel()
{
	string key = Param_key[7];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetClassifyTain(const vector<int> t)
{
	string key = Param_key[8];
	Streamoper sp;
	string str = "";
	for (size_t i = 0; i < t.size(); i++)
	{
		str += sp.SerizlizeStream(t.at(i));
	}
	m_Config[key] = str;
}

void  CAlgParamConfig::GetClassifyTrain(vector<int>& t)
{
	string key = Param_key[8];
	string str = m_Config[key];
	Streamoper sp;
	sp.InitDeseri(str);
	int tmp[100];
	int length;
	sp.DeserizlizeStream(tmp, length, true);							//字符串转数组
	for (int i = 0; i < length; i++)
	{
		t.push_back(tmp[i]);
	}

}

void CAlgParamConfig::ReTrainClassify(bool s)
{
	string key = Param_key[9];
	Stringoper oper;
	m_Config[key] = oper.booltostring(s);
}

bool CAlgParamConfig::IsRetrainClassify()
{
	string key = Param_key[9];
	Stringoper oper;
	return oper.stringtobool(m_Config[key]);
}

void CAlgParamConfig::SetClassifyLData(CString path)
{
	string key = Param_key[10];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}
CString CAlgParamConfig::GetClassifyLData()
{
	string key = Param_key[10];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetClassifyBData(CString path)
{
	string key = Param_key[11];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}
CString CAlgParamConfig::GetClassifyBData()
{
	string key = Param_key[11];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}
void CAlgParamConfig::SetClassifyRData(CString path)
{
	string key = Param_key[12];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}
CString CAlgParamConfig::GetClassifyRData()
{
	string key = Param_key[12];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetClassifyModule(CString path)
{
	string key = Param_key[13];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetClassifyModule()
{
	string key = Param_key[13];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}
#endif

#ifdef BLOCK

void CAlgParamConfig::SetHLState(const bool s)
{
	string key = Param_key[14];
	Stringoper oper;
	m_Config[key] = oper.booltostring(s);
}

bool CAlgParamConfig::GetHLState()
{
	string key = Param_key[14];
	Stringoper oper;
	return oper.stringtobool(m_Config[key]);
}

void CAlgParamConfig::SetHLTrainTime(const vector<int> t)
{
	string key = Param_key[15];
	Streamoper sp;
	string str = "";
	for (size_t i = 0; i < t.size(); i++)
	{
		str += sp.SerizlizeStream(t.at(i));
	}
	m_Config[key] = str;
}

void CAlgParamConfig::GetHLTrainTime(vector<int>& t)
{
	string key = Param_key[15];
	string str = m_Config[key];
	Streamoper sp;
	sp.InitDeseri(str);
	int tmp[100];
	int length;
	sp.DeserizlizeStream(tmp, length, true);							//字符串转数组
	for (int i = 0; i < length; i++)
	{
		t.push_back(tmp[i]);
	}
}

void CAlgParamConfig::SetHLReTrain(bool b)
{
	string key = Param_key[16];
	Stringoper oper;
	m_Config[key] = oper.booltostring(b);
}

bool CAlgParamConfig::GetHLReTrain()
{
	string key = Param_key[16];
	Stringoper oper;
	return oper.stringtobool(m_Config[key]);
}

void CAlgParamConfig::SetHLModel(const CString path)
{
	string key = Param_key[17];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetHLModel()
{
	string key = Param_key[17];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetHLModule(CString path)
{
	string key = Param_key[18];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetHLModule()
{
	string key = Param_key[18];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetLLData(CString path)
{
	string key = Param_key[19];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetLLData()
{
	string key = Param_key[19];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetLHData(CString path)
{
	string key = Param_key[20];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetLHData()
{
	string key = Param_key[20];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetRLData(CString path)
{
	string key = Param_key[21];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetRLData()
{
	string key = Param_key[21];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetRHData(CString path)
{
	string key = Param_key[22];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetRHData()
{
	string key = Param_key[22];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetBLData(CString path)
{
	string key = Param_key[23];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetBLData()
{
	string key = Param_key[23];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetBHData(CString path)
{
	string key = Param_key[24];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetBHData()
{
	string key = Param_key[24];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

#endif

#ifdef BLOCK
void  CAlgParamConfig::SetTLTrainState(bool b)
{
	string key = Param_key[25];
	Stringoper oper;
	m_Config[key] = oper.booltostring(b);
}
bool CAlgParamConfig::GetTLTrainState()
{
	string key = Param_key[25];
	Stringoper oper;
	return oper.stringtobool(m_Config[key]);
}

void CAlgParamConfig::SetTLTrainTime(const vector<int> t)
{
	string key = Param_key[26];
	Streamoper sp;
	string str = "";
	for (size_t i = 0; i < t.size(); i++)
	{
		str += sp.SerizlizeStream(t.at(i));
	}
	m_Config[key] = str;
}

void CAlgParamConfig::GetTLTrainTime(vector<int>& t)
{
	string key = Param_key[26];
	string str = m_Config[key];
	Streamoper sp;
	sp.InitDeseri(str);
	int tmp[100];
	int length;
	sp.DeserizlizeStream(tmp, length, true);							//字符串转数组
	for (int i = 0; i < length; i++)
	{
		t.push_back(tmp[i]);
	}
}

void CAlgParamConfig::SetTLLeftParamPath(const CString path)
{
	string key = Param_key[27];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetTLLeftParamPath()
{
	string key = Param_key[27];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetTLBothParamPath( CString path)
{
	string key = Param_key[28];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetTLBothParamPath()
{
	string key = Param_key[28];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetTLRightParamPath(const CString path)
{
	string key = Param_key[29];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetTLRightParamPath()
{
	string key = Param_key[29];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetTLLeftData(CString path)
{
	string key = Param_key[30];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetTLLeftData()
{
	string key = Param_key[30];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetTLBothData(CString path)
{
	string key = Param_key[31];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetTLBothData()
{
	string key = Param_key[31];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetTLRightData(CString path)
{
	string key = Param_key[32];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetTLRightData()
{
	string key = Param_key[32];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetTLLeftModel(CString path)
{
	string key = Param_key[33];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetTLLeftModel()
{
	string key = Param_key[33];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetTLBothModel(CString path)
{
	string key = Param_key[34];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetTLBothModel()
{
	string key = Param_key[34];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetTLRightModel(CString path)
{
	string key = Param_key[35];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetTLRightModel()
{
	string key = Param_key[35];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetTLModule(CString path)
{
	string key = Param_key[36];
	Stringoper oper;
	m_Config[key] = oper.tostring(path);
}

CString CAlgParamConfig::GetTLModule()
{
	string key = Param_key[36];
	Stringoper oper;
	return oper.tocstring(m_Config[key]);
}

void CAlgParamConfig::SetTLReTrain(bool b)
{
	string key = Param_key[37];
	Stringoper oper;
	m_Config[key] = oper.booltostring(b);
}

bool CAlgParamConfig::GetTLReTrain()
{
	string key = Param_key[37];
	Stringoper oper;
	return oper.stringtobool(m_Config[key]);
}



#endif

void CAlgParamConfig::SetWindowLength(int l)
{
	string key = Param_key[38];
	Stringoper oper;
	m_Config[key] = oper.inttostring(l);
}

int CAlgParamConfig::GetWindowLength()
{
	string key = Param_key[38];
	Stringoper oper;
	return oper.stringtoint(m_Config[key]);
}

void CAlgParamConfig::SetOverLength(int l)
{
	string key = Param_key[39];
	Stringoper oper;
	m_Config[key] = oper.inttostring(l);
}
int CAlgParamConfig::GetOverLength()
{
	string key = Param_key[39];
	Stringoper oper;
	return oper.stringtoint(m_Config[key]);
}
void CAlgParamConfig::GetLRThrolds(double& l, double& r)
{
	string key = Param_key[40];
	Stringoper oper;
	l = oper.stringtodouble(m_Config[key]);
	key = Param_key[41];
	r = oper.stringtodouble(m_Config[key]);
}

void CAlgParamConfig::SetLRThrolds(double l, double r)
{
	string key = Param_key[40];
	Stringoper oper;
	m_Config[key] = oper.doubletostring(l);
	key = Param_key[41];
	m_Config[key] = oper.doubletostring(r);
}


//private function
bool CAlgParamConfig::IsSpace(char c)
{
	if(' '==c||'\t'==c)
		return true;
	return false;
}
bool CAlgParamConfig::IsCommentChar(char c)
{
	switch(c){
	case PARAM_COMMENT:
		return true;

	default:
		return false;
	}
}

void CAlgParamConfig::TrimString(string& str)
{
	if(str.empty())
		return;
	size_t i , start_pos , end_pos;
	for (i=0;i<str.size();i++)
	{
		if (!IsSpace(str[i]))
		{
			break;
		}
	}
	if(i==str.size())
	{
		str="";
		return;
	}
	start_pos = i;
	for (i = str.size() - 1; i >= 0; --i) {
		if (!IsSpace(str[i])) {
			break;
		}
	}
	end_pos = i;

	str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool CAlgParamConfig::AnalyseLine(const string& line , string& key , string& value)
{
	if(line.empty())
		return false;
	int start_pos = 0 , end_pos = line.size()-1 , pos = 0;
	if((pos=line.find(PARAM_COMMENT))!=-1)
	{
		if(0==pos)
			return false;
		end_pos = pos - 1 ;  
	}
	string new_line = line.substr(start_pos, end_pos  - start_pos+1); 
	if ((pos = new_line.find('=')) == -1)
		return false;  
	key = new_line.substr(0, pos);
	value = new_line.substr(pos + 1, end_pos + 1- (pos + 1));

	TrimString(key);
	if (key.empty()) {
		return false;
	}
	TrimString(value);
	return true;
}

