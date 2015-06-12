/*
作者：黄文浩
时间：2015/1/25
描述：实现视图相关参数的读取与配置
*/
#pragma once
#include <iostream>
#include <map>
#include <string>
using namespace std;

class CViewerConfig
{
public:
	CViewerConfig();
	CViewerConfig(CString path);
	~CViewerConfig();

	bool	OpenFile(CString path);		     //打开配置文件
	void	CloseFile();				     //关闭文件
	bool	IsOpen();
	bool	LoadFileMsg();             
	bool	SaveFileMsg();				     //保存文件信息
	CString GetViewerPath();			     //获取视图所在路径
	void    SetViewerPath(CString path);     //设置视图路径
	CString GetSelDllPath();				 //获取选中的Dll 路径
	void    SetSelDllPath(CString path);     //设置选中的Dll 路径
	int     GetWindowLength();               //获取窗口长度
	void    SetWindowLength(int len);        //设置窗口长度
	int     GetWindowWidth();				 //获取窗口宽度
	void    SetWindowWidth(int width);       //设置窗口宽度
	int     GetMaxValue();					 //获取最大值
	void    SetMaxValue(int maxvalue);		 //设置最大值
	int     GetMinValue();					 //获取最小值
	void	SetMinValue(int minvalue);		 //设置最小值
	BOOL    GetIsAutoMaxMin();				 //获取是否自动最大最小的值
	void    SetIsAutoMaxMin(int autov);		 //设置是否自动最大最小的值

private:
	bool	IsSpace(char c);
	bool	IsCommentChar(char c);
	void	TrimString(string& str);
	bool	AnalyseLine(const string& line, string& key, string& value);

private:
	CString		m_ViewerPath;
	CString     m_SelDllPath;
	CStdioFile	m_File;
	int			m_Length;
	int			m_Width;
	int         m_MaxValue;
	int			m_MinValue;
	BOOL        m_IsAutoMaxMin;
	bool		m_bIsopen;
	map<string, string>		m_Configmap;
};