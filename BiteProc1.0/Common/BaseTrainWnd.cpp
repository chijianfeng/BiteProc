// BaseTrainWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "BaseTrainWnd.h"
#include "afxdialogex.h"


// CBaseTrainWnd 对话框

IMPLEMENT_DYNAMIC(CBaseTrainWnd, CDialogEx)

CBaseTrainWnd::CBaseTrainWnd(CWnd* pParent /*=NULL*/)
	//: CDialogEx(CBaseTrainWnd::IDD, pParent)
{
	m_pAlogrithm[0] = m_pAlogrithm[1] = m_pAlogrithm[2] = 0;
}

CBaseTrainWnd::~CBaseTrainWnd()
{
}

void CBaseTrainWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseTrainWnd, CDialogEx)
END_MESSAGE_MAP()
