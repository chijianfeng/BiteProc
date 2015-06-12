#pragma once
#include "mscomm1.h"
#include <iostream>
#include <list>
#include <vector>
#include "afxwin.h"
#include "afxcmn.h"
#using <mscorlib.dll>
#using "../Debug/SerialPort.dll"
#include <vcclr.h>
#include "DigiUtil.h"
#include "resource.h"

using namespace System;
using namespace SerialPort;

using namespace std;
#pragma managed
// CWheelChair 对话框
enum DIRECTION {NONE , UP , BACK , LEFT , RIGHT};

const int INITLEFTSPEED  = -20;
const int INITRIGHTSPEED =  20;
const int INITUPSPEED    =  20;
const int INITBACKSPEED  = -20;

const int SPEEDUP        =  20;
const int SPEEDOWN       = -20;

class CWheelChair : public CDialogEx
{
	DECLARE_DYNAMIC(CWheelChair)

public:
	CWheelChair(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWheelChair();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private: 
	gcroot<SerialPort::Serialport^> pPort;
	void	OpenPort();									//open the port.
	vector<string> splitEx(const string& src, string separate_character);
	
	CString Hex2CString(byte data[] , int len);

	HANDLE m_PorcessThread;

	void Stop();									//stop the wheelchair.

	void SlowDown(int speed);						//slow down the speed to zero.

	void SpeedUp();
	void Slow();

	CString			m_nMsg;
public:

	void		ClosePort();						  //close the port.
	afx_msg void OnBnClickedBstart();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnUpDateMsg(WPARAM wParam , LPARAM lParam);
private:
	CScrollBar m_nRun;
	CScrollBar m_nleftRight;
	CButton m_nBmove;
	CButton m_nBzyyt;
	CButton m_nBkbyt;

	DIRECTION	m_nCurDir;
	DIRECTION	m_nPreDir;
	BOOL	m_bSrtarting;
	int		m_nUpMaxSpeed;
	int		m_nDownMaxSpeed;
	int		m_nLeftMaxSpeed;
	int		m_nRightMaxSpeed;

	CDigistring	m_nLeftRightdigtal;
	CDigistring	m_nUpDowndigtal;

public:
	afx_msg void OnNMThemeChangedRun(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMThemeChangedLR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBfinish();
	afx_msg void OnBnClickedBwcmove();
	afx_msg void OnBnClickedBzyyt();
	afx_msg void OnBnClickedBkbyt();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnItemchangedListinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnReceiveCmd(WPARAM wParam , LPARAM lParam);
	CListCtrl m_nInfoList;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg long OnHotKey(WPARAM wParam , LPARAM lParam);
	afx_msg void OnBnClickedBset();
};



