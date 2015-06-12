#pragma once
#include"IAppMsg.h"
#include"WheelChair.h"
#include"WconfigDlg.h"
#include"GyroDlg.h"
#include"../CoreModel/CoreModelDoc.h"

class CWheelChairShow :public CWnd, implements IAppMsg
{
	DECLARE_DYNAMIC(CWheelChairShow)
public:
	CWheelChairShow();
	virtual  ~CWheelChairShow();
	CCoreModelDoc* GetDocument() const;
	void     SendAppCmd(size_t cmd);
	void     SetWndSize(size_t width, size_t height);
	void     SetLogOut(void(*pFunc)(CString));
protected:
	CWheelChair* m_pAppWnd;
	CGyroDlg* m_pGyroDlg;
	CDocument* m_pDocument;
private:
	
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//²Ëµ¥ÏûÏ¢
	afx_msg LRESULT OnHindHint(WPARAM wapram, LPARAM lparam);
	afx_msg LRESULT OnShowHint(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnShowSettingTask(WPARAM wparam, LPARAM lparam);
};
