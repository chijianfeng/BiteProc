#pragma once

#include "UserConfig.h"
#include <list>
#include <iostream>
using namespace std;
// CDeviceSelDlg 对话框

class CDeviceSelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSelDlg)

public:
	CDeviceSelDlg(CWnd* pParent = NULL);   // 标准构造函数
	CDeviceSelDlg(CUserConfig& config , CWnd* pParent);

	virtual ~CDeviceSelDlg();

// 对话框数据
	enum { IDD = IDD_DEVICESELDLG };

private:
	CString			m_strSearchDir;						//模块搜索路径
	CString			m_strChannels;						//通道
	size_t			m_nBufSize;
	CString			m_strDllPath;						//选择设备的路径

	CComboBox		m_comDeviceSel;						//设备选择
	CComboBox		m_comSampleRate;					//采样率选择

	CString			m_strUserPath;						//用户设备路径
	list<CString>	m_listPaths;						//设备s
	
private:
	void			CheckDevice();						//检索设备

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBdevsearchdir();
	afx_msg void OnBnClickedOk();
//	virtual BOOL OnInitDialog();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComdevsel();
private:
	CString m_LParam;                          //hwh_add
public: 
	afx_msg void OnBnClickedBlparam();         //hwh_add
};
