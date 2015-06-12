#pragma once

#include "ITrainMsg.h"
#include "CommonUtile.h"
#include "NetWorker.h"
#include "AlgParamConfig.h"
using namespace UTILE;
// CClasssifyWnd �Ի���

class CClasssifyWnd : public CDialogEx,implements ITrainMsg
{
	DECLARE_DYNAMIC(CClasssifyWnd)

public:
	CClasssifyWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClasssifyWnd();

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

	//extend function


// �Ի�������
	enum { IDD = IDD_CLASSSIFYWND };
	enum BiteState
	{
		STATE_NONE,//��Ч״̬
		STATE_START,//��ʼ״̬
		STATE_PAUSE,//��ͣ״̬
		STATE_DONE,//���״̬
		STATE_STOP,//��ֹ״̬
		STATE_RETRAIN,	//����ѵ��
	};

private:
	Throld	LoadThrold(CString path);					//������ֵģ�ͣ����ں����㷨�Ĵ���
	void	MyLoaddata(CString path, ManagedData& buf);	//��������

	bool	CreateModelFile();							//����ģ�Ͳ����ļ�
	void	ProcessResult(int res);						//������

	void	ProcessNone();								//�Բ�ͬ��״̬���д���
	void	ProcessStart();
	void	ProcessPause();
	void	ProcessDone();	
	void	ProcessStop();
	void	ProcessRetrain();

	CString	GetDirectory();								//��ȡ�û�Ŀ¼
private:

	//�ؼ�����
	CString				m_strState;						//��ʾ��ǰ״̬
	CString				m_strInstruct;					//��ʾ��ʾ˵��
	
	CProgressCtrl		m_ProgressState;				//��ʾ��ǰ״̬����
	CSliderCtrl			m_Left2Middle;					//��������
	CSliderCtrl			m_Middle2Right;
	double				m_dL2M;
	double				m_dM2R;
	int					m_nResult;						//���Խ��
	CButton				m_ButtonOK;
	CButton				m_ButtonTest;					//���԰�ť

	//�㷨����
	bool				m_bIsTrain;
	bool				m_bReTrain;
	ManagedData			m_TrainDataBuf[3];				//����ѵ�����ݻ��������ֱ�Ϊ��L��M ��R
	int					m_nWinLen;						//���ڳ���
	int					m_nWinOverLen;					//�����ص�����
	vector<int>			m_nTraintime;					//ѵ��ʱ��
	CString				m_DataSavePath[3];				//�ɼ����ݱ����·��
	CString				m_ModlePath;					//ģ�Ͳ���
	Throld				m_BiteThrold;					//ҧ����ֵ

	//��Ա����
	ManagedData			m_BufData;						//���ݱ�����ʱ����
	BiteState			m_CurState;						//��ǰ������״̬
	bool				m_bIsOpen;						//��ǰ������ļ��Ƿ��
	int					m_nProgress;					//��ǰ����������
	size_t				m_nFileCount;					//��Ǳ�����ǵڼ����ļ�
	HANDLE				m_hThread;						//the handle of thread
	CFile				m_nFile;						//���������ļ�
	int					m_CurTrainTime;					//��ǰ����ѵ��ʱ��

	CNetWorker*			m_pNetWorker;					//�������㷨ģ��
	int					m_nSelArth;						//0-->LM;1-->BP;2-->FANN
	double				m_Error;						//ϵͳ���
	int					m_MaxTrainTime;					//���ѵ������
	int					m_HindLayer;					//��������Ŀ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtontest();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnReceivMsg(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnParam(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnJobMsg(WPARAM wparam, LPARAM lparam);
public:
	afx_msg void OnNMCustomdrawSliderltom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderltom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlidermtor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlidermtor(NMHDR *pNMHDR, LRESULT *pResult);
};
