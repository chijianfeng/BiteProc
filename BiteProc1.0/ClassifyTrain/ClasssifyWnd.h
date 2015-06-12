#pragma once

#include "ITrainMsg.h"
#include "CommonUtile.h"
#include "NetWorker.h"
#include "AlgParamConfig.h"
using namespace UTILE;
// CClasssifyWnd 对话框

class CClasssifyWnd : public CDialogEx,implements ITrainMsg
{
	DECLARE_DYNAMIC(CClasssifyWnd)

public:
	CClasssifyWnd(CWnd* pParent = NULL);   // 标准构造函数
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


// 对话框数据
	enum { IDD = IDD_CLASSSIFYWND };
	enum BiteState
	{
		STATE_NONE,//无效状态
		STATE_START,//开始状态
		STATE_PAUSE,//暂停状态
		STATE_DONE,//完成状态
		STATE_STOP,//终止状态
		STATE_RETRAIN,	//重新训练
	};

private:
	Throld	LoadThrold(CString path);					//加载阈值模型，便于后续算法的处理
	void	MyLoaddata(CString path, ManagedData& buf);	//加载数据

	bool	CreateModelFile();							//创建模型参数文件
	void	ProcessResult(int res);						//处理结果

	void	ProcessNone();								//对不同的状态进行处理
	void	ProcessStart();
	void	ProcessPause();
	void	ProcessDone();	
	void	ProcessStop();
	void	ProcessRetrain();

	CString	GetDirectory();								//获取用户目录
private:

	//控件变量
	CString				m_strState;						//显示当前状态
	CString				m_strInstruct;					//显示提示说明
	
	CProgressCtrl		m_ProgressState;				//显示当前状态进度
	CSliderCtrl			m_Left2Middle;					//参数调整
	CSliderCtrl			m_Middle2Right;
	double				m_dL2M;
	double				m_dM2R;
	int					m_nResult;						//测试结果
	CButton				m_ButtonOK;
	CButton				m_ButtonTest;					//测试按钮

	//算法变量
	bool				m_bIsTrain;
	bool				m_bReTrain;
	ManagedData			m_TrainDataBuf[3];				//保存训练数据缓存区，分别为：L，M ，R
	int					m_nWinLen;						//窗口长度
	int					m_nWinOverLen;					//窗口重叠长度
	vector<int>			m_nTraintime;					//训练时间
	CString				m_DataSavePath[3];				//采集数据保存的路径
	CString				m_ModlePath;					//模型参数
	Throld				m_BiteThrold;					//咬牙阈值

	//成员变量
	ManagedData			m_BufData;						//数据保存临时区域
	BiteState			m_CurState;						//当前所处的状态
	bool				m_bIsOpen;						//当前保存的文件是否打开
	int					m_nProgress;					//当前进度条进度
	size_t				m_nFileCount;					//标记保存的是第几个文件
	HANDLE				m_hThread;						//the handle of thread
	CFile				m_nFile;						//保存数据文件
	int					m_CurTrainTime;					//当前步骤训练时长

	CNetWorker*			m_pNetWorker;					//神经网络算法模型
	int					m_nSelArth;						//0-->LM;1-->BP;2-->FANN
	double				m_Error;						//系统误差
	int					m_MaxTrainTime;					//最大训练次数
	int					m_HindLayer;					//隐含层数目

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
