
// CoreModelDoc.h : CCoreModelDoc 类的接口
//


#pragma once
#include "AlgParamConfig.h"
#include "BasicDevice.h"
#include "DeviceConfig.h"
#include "ManagedData.h"
#include "BaseEEGViewerWnd.h"
#include "BaseTrainWnd.h"
#include "BaseAppWnd.h"

class CCoreModelDoc : public CDocument
{
protected: // 仅从序列化创建
	CCoreModelDoc();
	DECLARE_DYNCREATE(CCoreModelDoc)

// 特性
public:

	bool	RegDevice(CDeviceConfig& cfg);	//注册设备，用于数据采集
	bool	DeleteDevice();					//移除当前的设备

	BOOL	StartGetDataThread();			//启动采集数据线程
	void	PauseGetDataThead();			//暂停采集数据线程
	BOOL	EndGetDataThread();				//关闭采集数据线程

	bool	RegEEGViewer(CString path, CWnd* pParent);		//注册EEG 信号显示模块,提供Dll 的路径
	bool	DeleteEEGViewer();				//移除当前EEG Viewer 模块
	CBaseEEGViewerWnd*	GetEEGViewer();		//获取当前EEG显示视图

	//用于加载训练模块
	BOOL	RegTrainModule(CString path, CWnd* pParent);
	BOOL	CloseTrainModule();				//仅仅关闭窗口，不释放模型
	BOOL	DelTrainModule();				//移除当前训练模块,释放内存
	CBaseTrainWnd* GetBaseTrainWnd();		//获取当前训练模块

	BOOL	AppendAlgModel(CString path);	//仅仅加载模型，不涉及训练窗口
	void	ClearAlgModel();				//清空当前所有模型


	//加载应用模块
	BOOL	RegAppModule(CString path, CWnd* pParent, CRect& rect);	//加载应用程序模块
	BOOL	DelAppModule();								//删除当前应用模块窗体，释放资源
	CBaseAppWnd*	GetCurApp();						//获取当前App值


// 操作
public:

	void    GetNewData(ManagedData& data, ManagedData& color);		//获取当前最新的数据
	bool	GetGyroData(ManagedData& data);
	size_t	GetChanNum();					//获取通道数
	bool    SaveData();                     //save the flage data
	bool    WriteHeader();
	bool    SendFlagState();
	void    SetState();
	void	SetCurUser(CString path);	   //当前用户的路径
	CString	GetCurUser(); 

	BOOL	IsTrain();						//返回当前系统是否处于训练模式

	void	CalculateColor();				//计算颜色值

	BOOL	StartPrcessThread();						//启动数据处理，识别线程
	BOOL	EndProcessThread();							//终止线程
	BOOL	SuspendProcessThread();						//阻塞当前线程
	CString CreateSavingFile();
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CCoreModelDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//设备相关参数
	BasicDevice*		m_pDevice;
	size_t				m_nSampleRate;					//采样率
	size_t				m_nChannum;						//设备通道数
	size_t				m_nBuffsize;					//设备冲区，若设备是具体硬件，该参数表示硬件层的缓存，若是文件设备，参数是内存缓存区
	int					m_nMode;						//模式：0：正常，1：阻抗，2：校准

	int					m_CurColorNumber;				//当前数据段打标记数据个数
	int					m_PreColorNumber;				//先前一个数据段打标记数据个数
	int					m_CurBiteLen;					//当前咬牙累积长度
	double              m_nMeanValue;
	bool                m_bIsSaving;                    //the flage saving the data
	CFile               m_nFile;						//record the flage data
	//EEG View 模块、参数
	CBaseEEGViewerWnd*	m_pEEGViewer;					//EEG 信号显示模块

	//训练模块参数
	CBaseTrainWnd*		m_pTrainWnd;					//训练模块指针

	//应用窗体
	CBaseAppWnd*		m_pAppWnd;

	//算法模型：
	list<CBaseTrainWnd*>	m_listAlgModel;					//算法模型			
	//缓存数据
	ManagedData		m_EEGOriginalData;		//保存原始数据
	ManagedData		m_EEGNewData;			//保存最新数据
	ManagedData		m_ColorData;			//保存对应的颜色值
	ManagedData		m_NewColorData;
	ManagedData     m_GyroData;
	//当前用户数据
	CString			m_CurUser;				//当前用户路径
private:
	static UINT GetData(LPVOID lpvoid);					//获取数据线程
	static UINT	OnLineProcess(LPVOID lpvoid);			//实时数据处理

	BOOL		m_bExit;								//线程退出标志
	HANDLE		m_hDataThread;							//采集数据线程

	HANDLE		m_hProcessThread;						//数据处理线程
	BOOL		m_bProcessExit;							//数据处理线程退出标志
	HANDLE		m_ProcessEvent;							//数据处理同步事件
	BOOL	    StartThread(UINT(*pf)(LPVOID), HANDLE handle, LPVOID lparam);		//启动线程
	BOOL		EndThread(HANDLE handle);											//终止线程

	//数据操作
	void	AddOriginalData(double* pd, int size);
	void    RemoveMeanData(double* pd, int size);     //hwh_add
	void	ExtractData(ManagedData& rawdata, ManagedData& filtd);	//提取1秒的数据

	UINT	Analysis(ManagedData& rawd , ManagedData& filtd);		//对数据进行分析

	void	Notify();
	double  mean(const double d[], int len);
	void    ShowCmdCode(int cmd);
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	static void	LogOut(CString str);			//用于模块状态输出显示
#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual void OnCloseDocument();
};

class UpdateRng : public CObject
{
public:
	BOOL m_bUpDateEEG;				//仅仅更新EEGViewer
	BOOL m_bUpdataFrame;			//仅仅更新CoreModelView
};
