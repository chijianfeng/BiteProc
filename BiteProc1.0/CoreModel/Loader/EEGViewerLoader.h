#pragma once

#include "../BaseEEGViewerWnd.h"
//检测，导入EEG 显示视图dll

class CEEGViewerLoader
{
public:

	static CEEGViewerLoader* GetInstance();			//单例模式，便于释放和加载
	
	void SetViewerPath(CString path);				//DLL 路径

	CBaseEEGViewerWnd* CreateViewer();				//创建对象

	BOOL LoadEEGViewer(CBaseEEGViewerWnd* pBase);		//加载模块

	BOOL LoadEEGViewer(CBaseEEGViewerWnd* pBase, CWnd* pParent);

	BOOL CheckSource(CString& msg);					//检测模块是否是一个EEG viewer 模块

	BOOL	Free();										//卸载模块

	void	SetParent(CWnd* p = 0);						//设置父窗口

	virtual ~CEEGViewerLoader();

private:
	CEEGViewerLoader();

	CString		m_strModelPath;						//模块路径
	CWnd*		m_pParent;							//
	HINSTANCE	m_hDll;									//模块句柄
};

