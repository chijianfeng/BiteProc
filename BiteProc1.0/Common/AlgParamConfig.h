#pragma once
//*实现对算法相关参数文件的加载，
//以及对是否进行训练进行，及训练参数进行确定。
/*
author：chi
time：2015-1-18
*/
#include <iostream>
#include <map>
#include <vector>
#include<afx.h>

using namespace std;
class CAlgParamConfig
{
public:
	CAlgParamConfig(void);
	CAlgParamConfig(CString path);
	~CAlgParamConfig(void);
	bool	OpenParamFile(CString path);
	bool	LoadParam();
	bool	SaveParam();
	bool	CloseParmFile();
	bool	IsOpen();
	//bite threld param
	void	SetBiteState(const bool s);						//是否进行训练
	bool	GetBitestate();
	void	SetBiteThorldModel(const CString fpath);
	CString	GetBiteThroldModel();							//若参数文件不存在，返回为空
	void	SetBiteTrain(const int t ,const int count);		//设置咬牙时长，以及在该时长下的咬牙次数
	void	GetBiteTrain(int& t , int& count);
	void	SetBiteDataPath(CString path);					//咬牙数据文件路径
	CString GetBiteDataPath();								
	void	SetBiteModulePath(CString path);				//设置dll路径
	CString	GetBiteModulePath();							//返回咬牙阈值模块dll的路径

	//左、右、同时分类模型 
	void	SetClassifyState(const bool s);					//是否进行左、右、同时模型分类训练
	bool	GetClassifystate();
	void	SetClassifyModel(const CString fpath);			//模型保存参数路径
	CString	GetClassifyModel();								//若参数文件不存在，返回为空
	void	SetClassifyTain(const vector<int> t);			//不同咬牙方式方式训练时长，单位s
	void	GetClassifyTrain(vector<int>& t);
	void	ReTrainClassify(bool s);						//是否重新训练
	bool	IsRetrainClassify();
	void	SetClassifyLData(CString path);					//左咬数据
	CString	GetClassifyLData();
	void	SetClassifyBData(CString path);					//同时咬数据
	CString GetClassifyBData();
	void	SetClassifyRData(CString path);					//右咬数据
	CString GetClassifyRData();
	void	SetClassifyModule(CString path);				//获取模块dll地址
	CString GetClassifyModule();

	

	//heavy or light bite param.
	void	SetHLState(const bool s);						//是否训练轻重阈值
	bool	GetHLState();
	void	SetHLModel(const CString fpath);				//轻重模型路径
	CString	GetHLModel();
	void	SetHLTrainTime(const vector<int> t);
	void	GetHLTrainTime(vector<int>& t);
	bool	GetHLReTrain();
	void	SetHLReTrain(bool b);							//重新计算数据，不采集，前提是得有数据
	void	SetHLModule(CString path);						//轻重模块路径
	CString	GetHLModule();									//获取模块dll

	void	SetLLData(CString path);						//左轻数据
	CString	GetLLData();
	void	SetLHData(CString path);						//左重数据
	CString GetLHData();
	void	SetBLData(CString path);						//同时轻数据
	CString GetBLData();
	void	SetBHData(CString path);						//同时重数据
	CString GetBHData();
	void	SetRLData(CString path);						//右轻数据
	CString	GetRLData();									
	void	SetRHData(CString data);						//右重数据
	CString	GetRHData();
	

	//长咬与两次咬模型
	void	SetTLTrainState(bool b);						//两次咬和长咬是否进行训练
	bool	GetTLTrainState();								//
	void	SetTLTrainTime(const vector<int> t);			//咬两次时间,(左、右、同时)
	void	GetTLTrainTime(vector<int>& t);
	void	SetTLLeftParamPath(const CString str);			//左侧参数
	CString GetTLLeftParamPath();
	void	SetTLBothParamPath(const CString str);			//同时参数路径
	CString	GetTLBothParamPath();
	void	SetTLRightParamPath(const CString path);		//右侧参数
	CString GetTLRightParamPath();

	void	SetTLLeftData(CString path);					//左侧数据值，咬两次
	CString	GetTLLeftData();
	void	SetTLBothData(CString path);					//两侧同时数据
	CString	GetTLBothData();
	void	SetTLRightData(CString path);					//右侧数据
	CString	GetTLRightData();

	void	SetTLLeftModel(CString path);					//左侧模型
	CString	GetTLLeftModel();
	void	SetTLBothModel(CString path);					//同时模型
	CString	GetTLBothModel();
	void	SetTLRightModel(CString path);					//右侧模型
	CString	GetTLRightModel();

	void	SetTLModule(CString path);						//两次咬模块dll
	CString	GetTLModule();									

	void	SetTLReTrain(bool b);							//重新训练
	bool	GetTLReTrain();

	//for basic  param.
	void	SetWindowLength(int l);
	int		GetWindowLength();
	void	SetOverLength(int l);
	int		GetOverLength();
	void	GetLRThrolds(double& l , double& r);			//左右分界值
	void	SetLRThrolds(double l , double r);

private:

	bool	IsSpace(char c);
	bool	IsCommentChar(char c);
	void	TrimString(string& str);
	bool	AnalyseLine(const string& line , string& key , string& value);

private:
	CStdioFile			m_nFile;
	map<string,string>  m_Config;
	BOOL				m_bIsopen;
};

