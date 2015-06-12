/*
���ߣ����ĺ�
ʱ�䣺2015/1/25
������ʵ����ͼ��ز����Ķ�ȡ������
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

	bool	OpenFile(CString path);		     //�������ļ�
	void	CloseFile();				     //�ر��ļ�
	bool	IsOpen();
	bool	LoadFileMsg();             
	bool	SaveFileMsg();				     //�����ļ���Ϣ
	CString GetViewerPath();			     //��ȡ��ͼ����·��
	void    SetViewerPath(CString path);     //������ͼ·��
	CString GetSelDllPath();				 //��ȡѡ�е�Dll ·��
	void    SetSelDllPath(CString path);     //����ѡ�е�Dll ·��
	int     GetWindowLength();               //��ȡ���ڳ���
	void    SetWindowLength(int len);        //���ô��ڳ���
	int     GetWindowWidth();				 //��ȡ���ڿ��
	void    SetWindowWidth(int width);       //���ô��ڿ��
	int     GetMaxValue();					 //��ȡ���ֵ
	void    SetMaxValue(int maxvalue);		 //�������ֵ
	int     GetMinValue();					 //��ȡ��Сֵ
	void	SetMinValue(int minvalue);		 //������Сֵ
	BOOL    GetIsAutoMaxMin();				 //��ȡ�Ƿ��Զ������С��ֵ
	void    SetIsAutoMaxMin(int autov);		 //�����Ƿ��Զ������С��ֵ

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