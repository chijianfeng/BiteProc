#pragma once
#include "ITrainMsg.h"
#include "Resource.h"
#include "CommonUtile.h"
#include "CalcuteColor.h"

using namespace UTILE;
// CBiteThroldWnd 对话框，训练得到咬牙判断阈值。

class CBiteThroldWnd : public CDialogEx,implements ITrainMsg
{
	DECLARE_DYNAMIC(CBiteThroldWnd)

public:
	CBiteThroldWnd(CWnd* pParent = NULL);				// 标准构造函数
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

// 对话框数据
	enum { IDD = IDD_BITETHROLDWND };
	enum BiteState
	{
		BITESTART,BITEPAUSE,BITEDONE		
	};												//咬牙的不同状态

private:
	//train param.
	bool	m_bIsTrain;								//判断是否训练
	CString m_nConfigFile;							//保存阈值参数的文件路径
	int		m_nTraintime;							//训练时长，单位S
	int		m_nTrainCount;							//咬牙次数
	int		m_Winlen;								//处理的窗口长度
	int		m_WinOverLen;							//重叠窗口长度
	
	ManagedData		m_nBuf;							//数据保存缓冲区
	CFile			m_nFile;						//保存数据文件
	CString			m_strFilePath;					//数据文件保存的路径
	bool			m_bIsOpen;						//数据文件是否打开：true：打开，false：未打开
	Throld			m_BiteThreshold;				//咬牙阈值结构体


	HANDLE		m_hThread;								//the handle of thread

	CString		m_strState;								// the process state
	CString		m_strInstrcut;							//the instruction of user
	CProgressCtrl	m_proState;							//the progress of process
	int			m_nProgress;							//进度条显示进度
	CSliderCtrl	m_ListRatio;							//the Ratio of Bite throld.
	double		m_Ratio;								//the Throld 
	double		m_TmpThreshold;

	CButton		m_Bctrl;								//控制按钮

	BiteState	m_CurState;								//当前的咬牙状态

	CBaseArth*	m_pBase;								//算法模型


private:												//内部函数，不同模块因情况不同而有区别

	void		ProcessStartState();					//处理开始状态
	void		ProcessPauseState();					//处理暂停状态
	void		ProcessDoneState();						//处理完成状态

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
