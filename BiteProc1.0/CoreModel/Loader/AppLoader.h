#pragma once

#include "BaseAppWnd.h"

class CAppLoader
{
public:
	static CAppLoader* GetInstance();			//����ģʽ����ȡΨһ����

	void	SetWndPath(CString path);			//App �ļ���·��

	void	SetMainWnd(HWND h);

	void	SetParent(CWnd* p = 0);				//App���ø�����

	BOOL	CheckSource(CString& msg);			//����ģ���Ƿ���һ��Ӧ�ó���ģ�飬���Ƿ���TRUE ,���򷵻�FALSE

	BOOL	Free();								//ж��ģ��

	BOOL	CreateAppClass(CBaseAppWnd** pWnd);	//����������ͼ�࣬������������

	BOOL	CreateAppWnd(CBaseAppWnd *pBase, CWnd* pParent);	//���ڴ�������,&�˵�������в˵��Ļ�

	BOOL	CreateAppWnd(CBaseAppWnd* pBase);	//�������壬&�˵�������в˵��Ļ�

	BOOL	CheckMenuID(CBaseAppWnd* pwnd,UINT id);				//���˵���ID��������Ϣ��

	BOOL	CheckMenuID(UINT id);				//�������ID

	virtual ~CAppLoader();

private:
	CAppLoader();

	CString		m_ModelPath;
	CWnd*		m_pParent;
	
	HWND		m_MainFrameWnd;							//�����ھ��

	HINSTANCE	m_hDll;									//ģ����

	UINT*		m_pItemID;								//�˵�id

	UINT		m_ItemNumber;							//item��
};

