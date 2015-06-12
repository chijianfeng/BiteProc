#pragma once

#include "SignalView.h"
#include "tchart1.h"
#include "ManagedData.h"
#include "resource.h"

// CGyroDlg �Ի���

class CGyroDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGyroDlg)

public:
	CGyroDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGyroDlg();

	void	SetData(ManagedData& data);
// �Ի�������
	enum { IDD = IDD_GYRODLG };

private:
	CSignalView*	m_pSignalView;
	int				m_length;
	int				m_nMaxLegth;
	ManagedData		m_nDataX , m_nDataY;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CTchart1 m_nChart;

private:
	void preocessdata(ManagedData& data);
};



