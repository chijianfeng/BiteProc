
// CoreModelDoc.h : CCoreModelDoc ��Ľӿ�
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
protected: // �������л�����
	CCoreModelDoc();
	DECLARE_DYNCREATE(CCoreModelDoc)

// ����
public:

	bool	RegDevice(CDeviceConfig& cfg);	//ע���豸���������ݲɼ�
	bool	DeleteDevice();					//�Ƴ���ǰ���豸

	BOOL	StartGetDataThread();			//�����ɼ������߳�
	void	PauseGetDataThead();			//��ͣ�ɼ������߳�
	BOOL	EndGetDataThread();				//�رղɼ������߳�

	bool	RegEEGViewer(CString path, CWnd* pParent);		//ע��EEG �ź���ʾģ��,�ṩDll ��·��
	bool	DeleteEEGViewer();				//�Ƴ���ǰEEG Viewer ģ��
	CBaseEEGViewerWnd*	GetEEGViewer();		//��ȡ��ǰEEG��ʾ��ͼ

	//���ڼ���ѵ��ģ��
	BOOL	RegTrainModule(CString path, CWnd* pParent);
	BOOL	CloseTrainModule();				//�����رմ��ڣ����ͷ�ģ��
	BOOL	DelTrainModule();				//�Ƴ���ǰѵ��ģ��,�ͷ��ڴ�
	CBaseTrainWnd* GetBaseTrainWnd();		//��ȡ��ǰѵ��ģ��

	BOOL	AppendAlgModel(CString path);	//��������ģ�ͣ����漰ѵ������
	void	ClearAlgModel();				//��յ�ǰ����ģ��


	//����Ӧ��ģ��
	BOOL	RegAppModule(CString path, CWnd* pParent, CRect& rect);	//����Ӧ�ó���ģ��
	BOOL	DelAppModule();								//ɾ����ǰӦ��ģ�鴰�壬�ͷ���Դ
	CBaseAppWnd*	GetCurApp();						//��ȡ��ǰAppֵ


// ����
public:

	void    GetNewData(ManagedData& data, ManagedData& color);		//��ȡ��ǰ���µ�����
	bool	GetGyroData(ManagedData& data);
	size_t	GetChanNum();					//��ȡͨ����
	bool    SaveData();                     //save the flage data
	bool    WriteHeader();
	bool    SendFlagState();
	void    SetState();
	void	SetCurUser(CString path);	   //��ǰ�û���·��
	CString	GetCurUser(); 

	BOOL	IsTrain();						//���ص�ǰϵͳ�Ƿ���ѵ��ģʽ

	void	CalculateColor();				//������ɫֵ

	BOOL	StartPrcessThread();						//�������ݴ���ʶ���߳�
	BOOL	EndProcessThread();							//��ֹ�߳�
	BOOL	SuspendProcessThread();						//������ǰ�߳�
	CString CreateSavingFile();
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CCoreModelDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//�豸��ز���
	BasicDevice*		m_pDevice;
	size_t				m_nSampleRate;					//������
	size_t				m_nChannum;						//�豸ͨ����
	size_t				m_nBuffsize;					//�豸���������豸�Ǿ���Ӳ�����ò�����ʾӲ����Ļ��棬�����ļ��豸���������ڴ滺����
	int					m_nMode;						//ģʽ��0��������1���迹��2��У׼

	int					m_CurColorNumber;				//��ǰ���ݶδ������ݸ���
	int					m_PreColorNumber;				//��ǰһ�����ݶδ������ݸ���
	int					m_CurBiteLen;					//��ǰҧ���ۻ�����
	double              m_nMeanValue;
	bool                m_bIsSaving;                    //the flage saving the data
	CFile               m_nFile;						//record the flage data
	//EEG View ģ�顢����
	CBaseEEGViewerWnd*	m_pEEGViewer;					//EEG �ź���ʾģ��

	//ѵ��ģ�����
	CBaseTrainWnd*		m_pTrainWnd;					//ѵ��ģ��ָ��

	//Ӧ�ô���
	CBaseAppWnd*		m_pAppWnd;

	//�㷨ģ�ͣ�
	list<CBaseTrainWnd*>	m_listAlgModel;					//�㷨ģ��			
	//��������
	ManagedData		m_EEGOriginalData;		//����ԭʼ����
	ManagedData		m_EEGNewData;			//������������
	ManagedData		m_ColorData;			//�����Ӧ����ɫֵ
	ManagedData		m_NewColorData;
	ManagedData     m_GyroData;
	//��ǰ�û�����
	CString			m_CurUser;				//��ǰ�û�·��
private:
	static UINT GetData(LPVOID lpvoid);					//��ȡ�����߳�
	static UINT	OnLineProcess(LPVOID lpvoid);			//ʵʱ���ݴ���

	BOOL		m_bExit;								//�߳��˳���־
	HANDLE		m_hDataThread;							//�ɼ������߳�

	HANDLE		m_hProcessThread;						//���ݴ����߳�
	BOOL		m_bProcessExit;							//���ݴ����߳��˳���־
	HANDLE		m_ProcessEvent;							//���ݴ���ͬ���¼�
	BOOL	    StartThread(UINT(*pf)(LPVOID), HANDLE handle, LPVOID lparam);		//�����߳�
	BOOL		EndThread(HANDLE handle);											//��ֹ�߳�

	//���ݲ���
	void	AddOriginalData(double* pd, int size);
	void    RemoveMeanData(double* pd, int size);     //hwh_add
	void	ExtractData(ManagedData& rawdata, ManagedData& filtd);	//��ȡ1�������

	UINT	Analysis(ManagedData& rawd , ManagedData& filtd);		//�����ݽ��з���

	void	Notify();
	double  mean(const double d[], int len);
	void    ShowCmdCode(int cmd);
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	static void	LogOut(CString str);			//����ģ��״̬�����ʾ
#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual void OnCloseDocument();
};

class UpdateRng : public CObject
{
public:
	BOOL m_bUpDateEEG;				//��������EEGViewer
	BOOL m_bUpdataFrame;			//��������CoreModelView
};
