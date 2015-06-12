#pragma once
#include "BaseTrainWnd.h"
class CTrainWndLoader
{
public:
	
	static CTrainWndLoader* GetInstance();				//����ģʽ�������ͷźͼ���

	void	SetWndPath(CString path);					//����·��

	void	SetMainWnd(HWND h);							//���������ڣ�MainFram����HWND

	BOOL	LoadTrainWnd(CBaseTrainWnd** pBase);		//ģ����أ�����ģ��and ����

	BOOL	LoadTrainWnd(CBaseTrainWnd** pBase, CWnd* pParent);

	BOOL	CreateTrainClass(CBaseTrainWnd** pBase);	//����ģ���࣬���ڼ���ģ��

	BOOL	CreateTrainWnd(CBaseTrainWnd*pBase, CWnd* pParent);	//���ڴ�������

	BOOL	CheckSource(CString& msg);					//����ģ���Ƿ���һ��ѵ��ģ�飬���Ƿ���TRUE ,���򷵻�FALSE

	BOOL	Free();										//ж��ģ��

	void	SetParent(CWnd* p = 0);						//���ø�����

	BOOL	CheckMenuID(UINT id);						//�������ID

	virtual ~CTrainWndLoader();

private:
	CTrainWndLoader();

	CString		m_ModelPath;
	CWnd*		m_pParent;
	HINSTANCE	m_hDll;									//ģ����

	HWND		m_MainFrameWnd;							//�����ھ��

	UINT*		m_pItemID;								//�˵�id
};
