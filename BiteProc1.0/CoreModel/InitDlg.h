#pragma once

#include "UserConfig.h"
#include "resource.h"
// CInitDlg �Ի���

class CInitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInitDlg)

public:
	CInitDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInitDlg();

// �Ի�������
	enum { IDD = IDD_INITDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	void		CopyFiles(CString spath[] ,int size , CString dpath );

	CString		GetFileName(CString path);				//��ȡ�ļ�����
private:

	CString			m_Username;
	CComboBox		m_ComboUser;
	CButton			m_Checkmsg;

	CUserConfig		m_UserConfig;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombouserlist();
	afx_msg void OnBnClickedCheckuser();
};
