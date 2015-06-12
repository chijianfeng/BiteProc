#pragma once

#include "ITrainMsg.h"
#include "CommonUtile.h"
#include "AlgParamConfig.h"
using namespace UTILE;
// CTLTrainWnd �Ի���

class CTLTrainWnd : public CDialogEx,implements ITrainMsg
{
	DECLARE_DYNAMIC(CTLTrainWnd)

public:
	CTLTrainWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTLTrainWnd();

	//implement the interface
	void SetParm(CAlgParamConfig& parm);
	void SetData(ManagedData& d);
	bool SaveData();
	bool LoadData();
	bool DoProcess();										//calculate the throld.
	bool StartThread(DWORD(WINAPI lp)(LPVOID), HANDLE&);	//process thread
	bool EndThread(HANDLE&);
	bool SaveResult();
	bool LoadResult();
	bool ChangeState(CAlgParamConfig& parm, bool s);		//change the parament.

	void SetLogOut(void(*pFunc)(CString));

// �Ի�������
	enum { IDD = IDD_TLTRAINWND };
	enum BiteState										//�������״̬
	{
		STATE_NONE,		//��Ч״̬
		STATE_START,	//��ʼ״̬
		STATE_PAUSE,	//��ͣ״̬
		STATE_DONE,		//���״̬
		STATE_RETRAIN,	//����ѵ��
	};

private:

	Throld	LoadThrold(CString path);					//������ֵģ�ͣ����ں����㷨�Ĵ���
	void	MyLoaddata(CString path, ManagedData& buf);	//��������

private:
	CProgressCtrl		m_ProgressState;				//��ʾ��ǰ״̬����
	ULONGLONG			m_nProgress;					//��ǰ����������
	CButton				m_ButtonOK;
	BiteState			m_CurState;						//��ǰ������״̬
	CString				m_strState;						//��ʾ��ǰ״̬
	CString				m_strInstruct;					//��ʾ��ʾ˵��

	//����
	vector<int>			m_nTraintime;					//ѵ��ʱ��
	ManagedData			m_TrainDataBuf[3];				//����ѵ�����ݻ��������ֱ�Ϊ��L ��R , B
	int					m_nWinLen;						//���ڳ���
	int					m_nWinOverLen;					//�����ص�����
	bool				m_bIsTrain;
	bool				m_bReTrain;
	Throld				m_BiteThrold;					//ҧ����ֵ
	CString				m_strLeftModel;					//�������ģ��
	CString				m_strBothModel;					//��������ģ��
	CString				m_strRightModel;				//�����Ҳ�ģ��
	CString				m_DataSavePath[3];				//�ɼ����ݱ����·��
	CString				m_ParamPath[3];					//���������·��


	HANDLE				m_hThread;						//the handle of thread
	CFile				m_nFile;						//���������ļ�
	int					m_CurTrainTime;					//��ǰ����ѵ��ʱ��
	ManagedData			m_BufData;						//���ݱ�����ʱ����
	bool				m_bIsOpen;						//��ǰ������ļ��Ƿ��
	size_t				m_nFileCount;					//��Ǳ�����ǵڼ����ļ�
	CBaseArth*			m_pArth[3];						//ģ��ѵ���㷨
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	//��ȡ��ʱ���ݶ�
	bool SavetoFile(ManagedData& rawd, ManagedData& newdL, BpNetdata* pData, int num, CString path);
	//ѡȡҧ��ͨ��
	int	 GetBiteChannel(ManagedData& d);

	CString GetDirectory(CString path);					//��ȡ��ǰ�ļ�·��

	//��ͬ״̬�Ĵ������
	void ProcessNone();
	void ProcessStart();
	void ProcessPause();
	void ProcessDone();
	void ProcessRetrain();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnJobMsg(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};
