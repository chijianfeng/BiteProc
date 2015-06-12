#pragma once

#include "ITrainMsg.h"
#include "CommonUtile.h"
#include "AlgParamConfig.h"
using namespace UTILE;
// CTLTrainWnd 对话框

class CTLTrainWnd : public CDialogEx,implements ITrainMsg
{
	DECLARE_DYNAMIC(CTLTrainWnd)

public:
	CTLTrainWnd(CWnd* pParent = NULL);   // 标准构造函数
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

// 对话框数据
	enum { IDD = IDD_TLTRAINWND };
	enum BiteState										//定义控制状态
	{
		STATE_NONE,		//无效状态
		STATE_START,	//开始状态
		STATE_PAUSE,	//暂停状态
		STATE_DONE,		//完成状态
		STATE_RETRAIN,	//重新训练
	};

private:

	Throld	LoadThrold(CString path);					//加载阈值模型，便于后续算法的处理
	void	MyLoaddata(CString path, ManagedData& buf);	//加载数据

private:
	CProgressCtrl		m_ProgressState;				//显示当前状态进度
	ULONGLONG			m_nProgress;					//当前进度条进度
	CButton				m_ButtonOK;
	BiteState			m_CurState;						//当前所处的状态
	CString				m_strState;						//显示当前状态
	CString				m_strInstruct;					//显示提示说明

	//参数
	vector<int>			m_nTraintime;					//训练时间
	ManagedData			m_TrainDataBuf[3];				//保存训练数据缓存区，分别为：L ，R , B
	int					m_nWinLen;						//窗口长度
	int					m_nWinOverLen;					//窗口重叠长度
	bool				m_bIsTrain;
	bool				m_bReTrain;
	Throld				m_BiteThrold;					//咬牙阈值
	CString				m_strLeftModel;					//保存左侧模型
	CString				m_strBothModel;					//保存两侧模型
	CString				m_strRightModel;				//保存右侧模型
	CString				m_DataSavePath[3];				//采集数据保存的路径
	CString				m_ParamPath[3];					//参数保存的路径


	HANDLE				m_hThread;						//the handle of thread
	CFile				m_nFile;						//保存数据文件
	int					m_CurTrainTime;					//当前步骤训练时长
	ManagedData			m_BufData;						//数据保存临时区域
	bool				m_bIsOpen;						//当前保存的文件是否打开
	size_t				m_nFileCount;					//标记保存的是第几个文件
	CBaseArth*			m_pArth[3];						//模型训练算法
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//提取临时数据段
	bool SavetoFile(ManagedData& rawd, ManagedData& newdL, BpNetdata* pData, int num, CString path);
	//选取咬牙通道
	int	 GetBiteChannel(ManagedData& d);

	CString GetDirectory(CString path);					//获取当前文件路径

	//不同状态的处理过程
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
