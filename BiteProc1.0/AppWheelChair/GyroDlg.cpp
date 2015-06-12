// GyroDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GyroDlg.h"
#include "afxdialogex.h"
#include "CSeries.h"


// CGyroDlg 对话框
const int _GYRO_MAXVALUE = 10000;
IMPLEMENT_DYNAMIC(CGyroDlg, CDialogEx)

CGyroDlg::CGyroDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CGyroDlg::IDD, pParent)
{
	m_pSignalView = NULL;
}

CGyroDlg::~CGyroDlg()
{
}

void CGyroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_nChart);
}


BEGIN_MESSAGE_MAP(CGyroDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGyroDlg 消息处理程序


BOOL CGyroDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_length = 0;
	m_nMaxLegth = 120;
	m_nDataY.ResizeManageData(96);
	m_nDataX.ResizeManageData(96);
	m_nDataX.SetChannum(1);
	m_nDataY.SetChannum(1);
	return TRUE;
}


void CGyroDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

}

void CGyroDlg::SetData(ManagedData& data)
{
	data.SetChannum(2);
	preocessdata(data);
	CSeries pc0 = (CSeries)m_nChart.Series(0);
	CSeries pc1 = (CSeries)m_nChart.Series(1);
	if (m_nDataX.GetDataSize() >= 96)
	{
		pc0.DeleteRange(0, m_nDataX.GetDataSize());
		pc1.DeleteRange(0, m_nDataY.GetDataSize());
	}
	m_length = 0;
	for (int i = 0; i<m_nDataX.GetDataSize(); i++)
	{
		pc0.AddXY(m_length, m_nDataX[i], NULL, RGB(255, 0, 0));
		pc1.AddXY(m_length++, m_nDataY[i], NULL, RGB(0, 0, 255));
	}
}

void CGyroDlg::preocessdata(ManagedData& data)
{
	ManagedData d1, d2;
	for (int i = 0; i<data.GetDataSize(); i++)
	{
		data[i] = (data[i] + _GYRO_MAXVALUE) / (2 * _GYRO_MAXVALUE) * 100;
	}
	data.GetAnydata(d1, 0);
	data.GetAnydata(d2, 1);
	m_nDataX.AppendData(d1.GetData(), d1.GetDataSize());
	m_nDataY.AppendData(d2.GetData(), d2.GetDataSize());
}