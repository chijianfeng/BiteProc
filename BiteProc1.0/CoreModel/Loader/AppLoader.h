#pragma once

#include "BaseAppWnd.h"

class CAppLoader
{
public:
	static CAppLoader* GetInstance();			//单例模式，获取唯一对象

	void	SetWndPath(CString path);			//App 的加载路径

	void	SetMainWnd(HWND h);

	void	SetParent(CWnd* p = 0);				//App设置父窗口

	BOOL	CheckSource(CString& msg);			//检测该模块是否是一个应用程序模块，若是返回TRUE ,否则返回FALSE

	BOOL	Free();								//卸载模块

	BOOL	CreateAppClass(CBaseAppWnd** pWnd);	//仅仅加载视图类，并不创建窗口

	BOOL	CreateAppWnd(CBaseAppWnd *pBase, CWnd* pParent);	//用于创建窗体,&菜单，如果有菜单的话

	BOOL	CreateAppWnd(CBaseAppWnd* pBase);	//创建窗体，&菜单，如果有菜单的话

	BOOL	CheckMenuID(CBaseAppWnd* pwnd,UINT id);				//检测菜单的ID并发送消息；

	BOOL	CheckMenuID(UINT id);				//仅仅检测ID

	virtual ~CAppLoader();

private:
	CAppLoader();

	CString		m_ModelPath;
	CWnd*		m_pParent;
	
	HWND		m_MainFrameWnd;							//主窗口句柄

	HINSTANCE	m_hDll;									//模块句柄

	UINT*		m_pItemID;								//菜单id

	UINT		m_ItemNumber;							//item数
};

