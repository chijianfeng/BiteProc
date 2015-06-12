#pragma once
#include "ITrainMsg.h"
#include "Resource.h"
#include "CommonUtile.h"
#include "CalcuteColor.h"

using namespace UTILE;
// CBiteThroldWnd �Ի���ѵ���õ�ҧ���ж���ֵ��

class CBiteThroldWnd : public CDialogEx,implements ITrainMsg
{
	DECLARE_DYNAMIC(CBiteThroldWnd)

public:
	CBiteThroldWnd(CWnd* pParent = NULL);				// ��׼���캯��
	virtual ~CBiteThroldWnd();
	//implement the interface
	void SetParm( CAlgParamConfig& parm);
	void SetData(ManagedData& d);
	bool SaveData();
	bool LoadData();
	bool DoProcess();									//calculate the throld.
	bool StartThread(DWORD(WINAPI lp)(LPVOID) , HANDLE&);			//process thread
	bool EndThread(HANDLE&);
	bool SaveResult();
	bool LoadResult();
	bool ChangeState(CAlgParamConfig& parm, bool s);	//change the parament.

	void SetLogOut(void(*pFunc)(CString));

// �Ի�������
	enum { IDD = IDD_BITETHROLDWND };
	enum BiteState
	{
		BITESTART,BITEPAUSE,BITEDONE		
	};												//ҧ���Ĳ�ͬ״̬

private:
	//train param.
	bool	m_bIsTrain;								//�ж��Ƿ�ѵ��
	CString m_nConfigFile;							//������ֵ�������ļ�·��
	int		m_nTraintime;							//ѵ��ʱ������λS
	int		m_nTrainCount;							//ҧ������
	int		m_Winlen;								//����Ĵ��ڳ���
	int		m_WinOverLen;							//�ص����ڳ���
	
	ManagedData		m_nBuf;							//���ݱ��滺����
	CFile			m_nFile;						//���������ļ�
	CString			m_strFilePath;					//�����ļ������·��
	bool			m_bIsOpen;						//�����ļ��Ƿ�򿪣�true���򿪣�false��δ��
	Throld			m_BiteThreshold;				//ҧ����ֵ�ṹ��


	HANDLE		m_hThread;								//the handle of thread

	CString		m_strState;								// the process state
	CString		m_strInstrcut;							//the instruction of user
	CProgressCtrl	m_proState;							//the progress of process
	int			m_nProgress;							//��������ʾ����
	CSliderCtrl	m_ListRatio;							//the Ratio of Bite throld.
	double		m_Ratio;								//the Throld 
	double		m_TmpThreshold;

	CButton		m_Bctrl;								//���ư�ť

	BiteState	m_CurState;								//��ǰ��ҧ��״̬

	CBaseArth*	m_pBase;								//�㷨ģ��


private:												//�ڲ���������ͬģ���������ͬ��������

	void		ProcessStartState();					//����ʼ״̬
	void		ProcessPauseState();					//������ͣ״̬
	void		ProcessDoneState();						//�������״̬

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();
	afx_msg LRESULT OnReceivMsg(WPARAM wparam, LPARAM lparam);
	afx_msg void OnNMCustomdrawSlidbiteradio(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlidbiteradio(NMHDR *pNMHDR, LRESULT *pResult);
};
