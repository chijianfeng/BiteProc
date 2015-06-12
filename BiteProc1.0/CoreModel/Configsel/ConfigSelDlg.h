/*
����:���ĺ�
ʱ��:2015/1/24
����:����������ConfigSelector����
	 ʹ����Tab�ؼ�,����������������ӽ���
	 "ҧ����ֵ����","��,��,ͬʱҧ��������"��"����ҧ��������","��ҧ,��������ҧ��������","����������������"
	 ��Ӧ�������(BiteThresholdConfig,LeftRightBothConfig,HLConfig,TLConfig,PublicConfig) ������
Repaire: chi
*/

// ConfigSelDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "BiteThresholdConfig.h"
#include "LeftRightBothConfig.h"
#include "HLConfig.h"
#include "TLConfig.h"
#include "PublicConfig.h"
#include "FileOper.h"
#include "../UserConfig.h"

// CConfigSelDlg �Ի���
class CConfigSelDlg : public CDialogEx
{

public:
	CConfigSelDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CConfigSelDlg(CUserConfig& config, CWnd* pParent);

	enum { IDD = IDD_CONFIGSEL_DIALOG };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl m_tab;
	BiteThresholdConfig m_dialog1;      
	LeftRightBothConfig m_dialog2;
	HLConfig m_dialog3;
	TLConfig m_dialog4;
	PublicConfig m_dialog5;

	CString			m_strUserPath;				//�û�Ŀ¼

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
