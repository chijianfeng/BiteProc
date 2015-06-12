#pragma once
#include <list>
#include <iostream>
#include "UserConfig.h"
#include "ViewerConfig.h"
using namespace std;

// CViewerSelDlg �Ի���

class CViewerSelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewerSelDlg)

public:
	CViewerSelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CViewerSelDlg(CString userpath, CWnd* pParent = NULL);
	virtual ~CViewerSelDlg();
	CString GetSelDllPath();
	void SetSize(const int len, const int width);
	void GetSize(int& len, int& width);
	void GetMaxMin(int& Maxv, int& Minv);
	BOOL IsAutoMaxMin();
	void CheckModels();
// �Ի�������
	enum { IDD = IDD_VIEWERSELDLG };

private:
	CString             m_EEGViewerPath;            //EEGViewer.txtĿ¼
	CString				m_strSearchDir;				//����·��
	list<CString>		m_listModels;				//ģ���б�
	CString				m_strDllPath;				//ѡ���Dll·��
	CComboBox			m_comViewers;				//������ͼģ��

	CString				m_strMessage;				//��ǰѡ��ģ�����Ϣ

	int					m_Length;					//��ͨ����ʾ����
	int					m_Width;					//��ͨ����ʾ���

	int					m_nMaxv;					//���ֵ
	int					m_nMinv;					//��Сֵ
	BOOL				m_bAuto;					//�Զ�������Сֵ

	CString				m_strUserPath;				//�û�·��
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBviewsearchdir();
	virtual BOOL OnInitDialog();
	afx_msg void OnViewseldlgCheck();
	afx_msg void OnCbnSelchangeComviewmodel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedChkviewauto();
};
