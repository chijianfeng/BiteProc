#pragma once
/*
���ߣ����ĺ�
ʱ�䣺2015/2/3
������ʵ�����ֱ�0-9����ʾ�����
*/
#include"IAppMsg.h"
#include"NumberRect.h"
#include"NumberWnd.h"
#include"TaskNumbers.h"

class CWndMoveRange            //���ִ��嶯���ƶ�
{
public:
	int X[2048];               //��¼�ƶ��켣
	int Y[2048];
	int Count;
	int m_Curx;                //��ǰλ��
	int m_Cury;
	int height;
	int width;         
	int ptr;
};

struct Number_Frame
{
	CRect           rect;
	CString         str[6];
	CNumberWnd      *pNumberWnd;
	UINT            ID[6];
};

class CNumberShow :public CWnd,implements IAppMsg
{
	DECLARE_DYNAMIC(CNumberShow)
public:
	CNumberShow();
	virtual  ~CNumberShow();
	void     SendAppCmd(size_t cmd);
	void     SetWndSize(size_t windth,size_t height);
	void     SetLogOut(void(*pFunc)(CString));
	void     PopNumbers();
	CString  GetNumbers();
	CString  GetTaskNumbers();
protected:
	Number_Frame   m_NumberFrame[6];
	CWndMoveRange  m_WndRange[6][6];
	CWndMoveRange  m_WndShowNumbers;
	CWndMoveRange  m_WndTaskRange;

	CNumberRect*   m_pShowNumbers;
	CTaskNumbers*  m_pTaskNumberwnd;

	CString      m_strBuf;
	CString      m_strTaskBuf;

	CRect        m_PreShowNumbersRect;
	CRect        m_NowShowNumbersRect;
	bool         m_bIsCreate;

private:
	void         ShowNumbersWndsAnimation();
	void         ShowNumbersWnds();
	void         UpdateNumbersWnds();
	void         ShowTaskWnd();
	static VOID CALLBACK LineDDAproc(int x,int y,LPARAM lparam);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//�˵���Ϣ
	afx_msg LRESULT OnHindHint(WPARAM wapram, LPARAM lparam);
	afx_msg LRESULT OnShowHint(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnShowSettingTask(WPARAM wparam, LPARAM lparam);
};