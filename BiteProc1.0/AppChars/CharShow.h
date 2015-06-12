#pragma once

#include "IAppMsg.h"
#include "CharWnd.h"
#include "CharsRect.h"
#include"TaskChars.h"
/*字符：显示、操作*/
// CCharShow
class CWndMoveRange							//字符窗体动画移动
{
public:
	int X[2048];							//记录移动轨迹
	int Y[2048];
	int Count;
	int m_Curx;								//当前位置
	int m_Cury;
	int width;								//宽度
	int height;
	int ptr;
};
struct Char_Frame{
	CRect rect;							//大小
	CString str[6];						//包含6个字符
	CCharWnd *pCharWnd;
	UINT	ID[6];						//字符窗体ID
};

class CCharShow : public CWnd,implements IAppMsg
{
	DECLARE_DYNAMIC(CCharShow)

public:
	CCharShow();
	virtual ~CCharShow();

	//implement the interface
	void SendAppCmd(size_t cmd);

	void SetWndSize(size_t windth, size_t height);

	void SetLogOut(void(*pFunc)(CString));

	//extend function

	void PopChars();			//移除一个字符
		
	CString GetChars();			//获取当前打的字符

	CString GetTaskChars();		//获取指定要打的字符

protected:

	Char_Frame			m_CharFrame[6];							//6组字符窗体
	UINT				m_FirstBite, m_SecondBite;				//标记第一次和第二次咬牙
	CWndMoveRange		m_WndRange[6][6];						//the move range and the current position
	CWndMoveRange		m_WndShowChars;
	CWndMoveRange		m_WndTaskRange;

	CCharsRect*			m_pShowChars;							//显示打出字符
	CTaskChars*			m_pTaskCharwnd;							//显示任务窗口

	CString				m_strBuf;								//已打的字符缓存区
	CString				m_strTaskBuf;							//字符任务
			
	CRect				m_PreShowCharsRect;						//记录窗体变化大小
	CRect				m_NowShowCharsRect;

	bool				m_bIsCreate;

private:

	void	ShowCharWndsAnimation();							//动画方式进行呈现

	void	ShowCharWnds();										//显示字符窗口

	void	UpdataCharWnds();									//更新字符窗体

	void	ShowTaskWnd();

	static VOID CALLBACK LineDDAproc(int x, int y, LPARAM lparam);	//计算窗体移动过程

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//菜单消息
	afx_msg LRESULT OnHindHint(WPARAM wapram , LPARAM lparam);
	afx_msg LRESULT OnShowHint(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnShowSettingTask(WPARAM wparam, LPARAM lparam);
};


