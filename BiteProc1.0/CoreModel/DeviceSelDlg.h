#pragma once

#include "UserConfig.h"
#include <list>
#include <iostream>
using namespace std;
// CDeviceSelDlg �Ի���

class CDeviceSelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSelDlg)

public:
	CDeviceSelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CDeviceSelDlg(CUserConfig& config , CWnd* pParent);

	virtual ~CDeviceSelDlg();

// �Ի�������
	enum { IDD = IDD_DEVICESELDLG };

private:
	CString			m_strSearchDir;						//ģ������·��
	CString			m_strChannels;						//ͨ��
	size_t			m_nBufSize;
	CString			m_strDllPath;						//ѡ���豸��·��

	CComboBox		m_comDeviceSel;						//�豸ѡ��
	CComboBox		m_comSampleRate;					//������ѡ��

	CString			m_strUserPath;						//�û��豸·��
	list<CString>	m_listPaths;						//�豸s
	
private:
	void			CheckDevice();						//�����豸

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
