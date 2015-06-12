#pragma once
#include "BaseTrainWnd.h"
class CTrainWndLoader
{
public:
	
	static CTrainWndLoader* GetInstance();				//单例模式，便于释放和加载

	void	SetWndPath(CString path);					//加载路径

	void	SetMainWnd(HWND h);							//设置主窗口（MainFram）的HWND

	BOOL	LoadTrainWnd(CBaseTrainWnd** pBase);		//模块加载，创建模型and 窗体

	BOOL	LoadTrainWnd(CBaseTrainWnd** pBase, CWnd* pParent);

	BOOL	CreateTrainClass(CBaseTrainWnd** pBase);	//创建模块类，用于加载模型

	BOOL	CreateTrainWnd(CBaseTrainWnd*pBase, CWnd* pParent);	//用于创建窗体

	BOOL	CheckSource(CString& msg);					//检测该模块是否是一个训练模块，若是返回TRUE ,否则返回FALSE

	BOOL	Free();										//卸载模块

	void	SetParent(CWnd* p = 0);						//设置父窗口

	BOOL	CheckMenuID(UINT id);						//仅仅检测ID

	virtual ~CTrainWndLoader();

private:
	CTrainWndLoader();

	CString		m_ModelPath;
	CWnd*		m_pParent;
	HINSTANCE	m_hDll;									//模块句柄

	HWND		m_MainFrameWnd;							//主窗口句柄

	UINT*		m_pItemID;								//菜单id
};
