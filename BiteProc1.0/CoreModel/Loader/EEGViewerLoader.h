#pragma once

#include "../BaseEEGViewerWnd.h"
//��⣬����EEG ��ʾ��ͼdll

class CEEGViewerLoader
{
public:

	static CEEGViewerLoader* GetInstance();			//����ģʽ�������ͷźͼ���
	
	void SetViewerPath(CString path);				//DLL ·��

	CBaseEEGViewerWnd* CreateViewer();				//��������

	BOOL LoadEEGViewer(CBaseEEGViewerWnd* pBase);		//����ģ��

	BOOL LoadEEGViewer(CBaseEEGViewerWnd* pBase, CWnd* pParent);

	BOOL CheckSource(CString& msg);					//���ģ���Ƿ���һ��EEG viewer ģ��

	BOOL	Free();										//ж��ģ��

	void	SetParent(CWnd* p = 0);						//���ø�����

	virtual ~CEEGViewerLoader();

private:
	CEEGViewerLoader();

	CString		m_strModelPath;						//ģ��·��
	CWnd*		m_pParent;							//
	HINSTANCE	m_hDll;									//ģ����
};

