#pragma once

#include "IAppMsg.h"
#include "CharWnd.h"
#include "CharsRect.h"
#include"TaskChars.h"
/*�ַ�����ʾ������*/
// CCharShow
class CWndMoveRange							//�ַ����嶯���ƶ�
{
public:
	int X[2048];							//��¼�ƶ��켣
	int Y[2048];
	int Count;
	int m_Curx;								//��ǰλ��
	int m_Cury;
	int width;								//���
	int height;
	int ptr;
};
struct Char_Frame{
	CRect rect;							//��С
	CString str[6];						//����6���ַ�
	CCharWnd *pCharWnd;
	UINT	ID[6];						//�ַ�����ID
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

	void PopChars();			//�Ƴ�һ���ַ�
		
	CString GetChars();			//��ȡ��ǰ����ַ�

	CString GetTaskChars();		//��ȡָ��Ҫ����ַ�

protected:

	Char_Frame			m_CharFrame[6];							//6���ַ�����
	UINT				m_FirstBite, m_SecondBite;				//��ǵ�һ�κ͵ڶ���ҧ��
	CWndMoveRange		m_WndRange[6][6];						//the move range and the current position
	CWndMoveRange		m_WndShowChars;
	CWndMoveRange		m_WndTaskRange;

	CCharsRect*			m_pShowChars;							//��ʾ����ַ�
	CTaskChars*			m_pTaskCharwnd;							//��ʾ���񴰿�

	CString				m_strBuf;								//�Ѵ���ַ�������
	CString				m_strTaskBuf;							//�ַ�����
			
	CRect				m_PreShowCharsRect;						//��¼����仯��С
	CRect				m_NowShowCharsRect;

	bool				m_bIsCreate;

private:

	void	ShowCharWndsAnimation();							//������ʽ���г���

	void	ShowCharWnds();										//��ʾ�ַ�����

	void	UpdataCharWnds();									//�����ַ�����

	void	ShowTaskWnd();

	static VOID CALLBACK LineDDAproc(int x, int y, LPARAM lparam);	//���㴰���ƶ�����

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//�˵���Ϣ
	afx_msg LRESULT OnHindHint(WPARAM wapram , LPARAM lparam);
	afx_msg LRESULT OnShowHint(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnShowSettingTask(WPARAM wparam, LPARAM lparam);
};


