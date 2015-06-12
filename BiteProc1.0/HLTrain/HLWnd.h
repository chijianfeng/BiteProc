#pragma once
#include "ITrainMsg.h"
#include "resource.h"
#include "DistinguishHL.h"
#include "CommonUtile.h"
#include "afxwin.h"
#include "afxcmn.h"
using namespace UTILE;
// CHLWnd �Ի���


class CHLWnd : public CDialogEx,implements ITrainMsg
{
	DECLARE_DYNAMIC(CHLWnd)

public:
	CHLWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual	~CHLWnd();

	//implement the interface
	void	SetParm(CAlgParamConfig &parm);
	void	SetData(ManagedData &d);
	bool	SaveData();
	bool	LoadData();
	bool	DoProcess();										
	bool	StartThread(DWORD(WINAPI lp)(LPVOID), HANDLE& handle);	//process thread
	bool	EndThread(HANDLE& handle);
	bool	SaveResult();
	bool	LoadResult();
	bool	ChangeState(CAlgParamConfig& parm, bool s);		//change the parament.
	void	SetLogOut(void(*pFunc)(CString));


// �Ի�������
	enum { IDD = IDD_HLWND };
	enum BiteState
	{
		STATE_NONE,		//��Ч״̬
		STATE_START,	//��ʼ״̬
		STATE_PAUSE,	//��ͣ״̬
		STATE_DONE,		//���״̬
		STATE_STOP,	    //��ֹ״̬
		STATE_RETRAIN   //��ѵ״̬
	};

private:
	void		MyLoadData(CString path,ManagedData& buf);
	bool		MeanSample(BpNetdata* bp, int len, Throld& t);
	int			ExtracSample(ManagedData &rawd, ManagedData& newd, BpNetdata* pb, Processdata *prod);
	int			ExtracSample(double *Lrawd, double *Rrawd, double *Lfiltd, double *Rfiltd, Throld throld, int len, BpNetdata *pb, Processdata *prod);
	bool		CreateModelFile();
	void		ProcessData(ManagedData& rawd, ManagedData& newd);
	void		ProcessNone();
	void		ProcessStart();
	void		ProcessPause();
	void		ProcessDone();
	void		ProcessStop();
	void        ProcessRetrain();
	void		ProcessResult(int res);
	CString		GetDirectroy();
	Throld		LoadThrold(CString path);
private:
	//�㷨����
	bool				m_IsTrain;
	bool				m_ReTrain;
	int					m_WinLen;
	int					m_WinOverLen;
	vector<int>			m_TrainTime;
	ManagedData			m_TrainDataBuf[12];              //����ѵ�����ݻ��������ֱ�Ϊ:LL,LH,RL,RH,BL,BH
	CString				m_DataSavePath[12];			    //����ѵ�������ļ�·��:LL,LH,RL,RH,BL,BH
	CString				m_ModelPath;
	CString				m_ModulePath;
	map<string, double>	m_nResult;
	Throld				m_BiteThreshold;
	int					m_nChannum;
	int					m_nSampleRate;
	double				m_left;
	double				m_right;
	double				m_both;
	double              m_nHLWeight[3];                  
	int                 m_testResult;                  //���Խ��
	//�ؼ�����
	CString				m_strState;
	CString				m_strInstruct;
	CProgressCtrl		m_ProgressState;
	CSliderCtrl			m_SliderLVol;
	CSliderCtrl			m_SliderRVol;
	CSliderCtrl			m_SliderLRVol;
	CButton				m_ButtonOK;
	CButton				m_ButtonTest;

	//��Ա����
	BiteState			m_CurState;						//��ǰ������״̬
	ManagedData			m_BufData;						//���ݱ�����ʱ����
	bool				m_bIsOpen;						//��ǰ������ļ��Ƿ��
	int					m_nProgress;					//��ǰ����������
	CDistinguishHL*		m_DisHL[3];						//���������㷨ģ��
	size_t				m_nFileCount;					//��Ǳ�����ǵڼ����ļ�
	HANDLE				m_hThread;						//the handle of thread
	CFile				m_nFile;						//���������ļ�
	int					m_CurTrainTime;					//��ǰ����ѵ��ʱ��
protected:
	virtual		void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
	virtual		BOOL	OnInitDialog();
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg		void	OnPaint();
	afx_msg		void	OnDestroy();
	afx_msg		void	OnBnClickedOk();
	afx_msg		void	OnBnClickedTest();
	afx_msg		void	OnBnClickedCancel();
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);
	afx_msg		LRESULT OnReceivMsg(WPARAM wparam, LPARAM lparam);
	afx_msg		LRESULT OnParam(WPARAM wparam, LPARAM lparam);
	afx_msg		LRESULT OnJobMsg(WPARAM wparam, LPARAM lparam);
public:
	afx_msg void OnNMCustomdrawSliderLVol(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderLVol(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRVol(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderRVol(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderLRVol(NMHDR *pNMHDR,LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderLRVol(NMHDR *pNMHDR,LRESULT *pResult);
};
