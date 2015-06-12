
// CoreModelDoc.cpp : CCoreModelDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "CoreModel.h"
#endif

#include "CoreModelDoc.h"
#include <propkey.h>
#include "OutputWnd.h"
#include "./Loader/DeviceLoader.cpp"
#include "./Loader/EEGViewerLoader.h"
#include "./Loader/AppLoader.h"
#include "TrainWndLoader.h"
#include "Stringoper.h"
#include "CommonUtile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCoreModelDoc

IMPLEMENT_DYNCREATE(CCoreModelDoc, CDocument)

BEGIN_MESSAGE_MAP(CCoreModelDoc, CDocument)
END_MESSAGE_MAP()


// CCoreModelDoc ����/����

CCoreModelDoc::CCoreModelDoc()
{
	// TODO:  �ڴ����һ���Թ������
	m_pDevice = NULL;
	m_pEEGViewer = NULL;
	m_pTrainWnd = NULL;
	m_hDataThread = NULL;
	m_pAppWnd = NULL;
	m_hProcessThread = INVALID_HANDLE_VALUE;
	m_ProcessEvent = INVALID_HANDLE_VALUE;
	m_CurBiteLen = 0;
	m_PreColorNumber = m_CurColorNumber = 0;

	m_bIsSaving = false;
	m_nMeanValue = 0;
}

CCoreModelDoc::~CCoreModelDoc()
{
}

BOOL CCoreModelDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CCoreModelDoc ���л�

void CCoreModelDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CCoreModelDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CCoreModelDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CCoreModelDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCoreModelDoc ���

#ifdef _DEBUG
void CCoreModelDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCoreModelDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

bool CCoreModelDoc::RegDevice(CDeviceConfig& cfg)
{
	CDeviceLoader<BasicDevice> loader;						//�豸����
	loader.SetDevicePath(cfg.GetDevicePath());
	loader.Setparma((void*)(cfg.GetParam()).c_str());		//���ò���������������ļ��������ļ�·���������豸��Ϊ��

	if (!loader.LoadDevice(&m_pDevice))
	{
		loader.Free();										//�ͷ���Դ
		m_pDevice = NULL;
		return false;
	}

	//�����豸��صĲ���
	m_nSampleRate = (size_t)cfg.GetSamplerate();
	m_pDevice->EB_SetSampleRate(m_nSampleRate);
	m_nChannum = (size_t)cfg.GetChannum();
	UCHAR *pchan = new UCHAR[cfg.GetChannum()];
	cfg.GetChans(pchan, m_nChannum);
	m_pDevice->EB_SetChannel(pchan, m_nChannum);
	delete pchan;
	m_nBuffsize =(size_t)cfg.GetBuffersize();
	m_pDevice->EB_SetBufferSize(m_nBuffsize);
	m_nMode = cfg.GetMode();
	m_pDevice->EB_SetMode(m_nMode);

	m_wndOutput << "�豸���أ��������" << m_EndOut;
	return true;
}

bool CCoreModelDoc::DeleteDevice()
{
	if (!m_pDevice)					//�豸δ����
		return true;
	m_pDevice->EB_Stop();
	m_pDevice->EB_CloseDevice();

	delete m_pDevice;
	m_pDevice = NULL;
	m_wndOutput << "Close the device" << m_EndOut;
	return true;
}

bool CCoreModelDoc::RegEEGViewer(CString path , CWnd* pParent)
{
	if (path.GetLength() <= 0)
	{
		m_wndOutput << "��ͼģ��·��Ϊ��,�޷�����" << m_EndOut;
		return FALSE;
	}
	CEEGViewerLoader* ploader = CEEGViewerLoader::GetInstance();
	ploader->SetParent(pParent);
	ploader->SetViewerPath(path);
	m_pEEGViewer = ploader->CreateViewer();
	if (!m_pEEGViewer)
		return false;
	//���ò���
	m_pEEGViewer->SetChannelNum(m_nChannum);
	m_pEEGViewer->SetClientSize(CRect(0, 0, 100,100));				//����Ϊ��ʼ�������ô�
	BOOL b = ploader->LoadEEGViewer(m_pEEGViewer);
	if (!b)
		return false;
	return true;
}

bool CCoreModelDoc::DeleteEEGViewer()
{
	if (!m_pEEGViewer)
		return true;
	m_pEEGViewer->SendMessage(WM_DESTROY);
	delete m_pEEGViewer;
	m_pEEGViewer = NULL;
	CEEGViewerLoader* ploader = CEEGViewerLoader::GetInstance();
	ploader->Free();
	return true;
}

CBaseEEGViewerWnd* CCoreModelDoc::GetEEGViewer()
{
	return m_pEEGViewer;
}

BOOL CCoreModelDoc::RegTrainModule(CString path, CWnd* pParent)
{
	CTrainWndLoader* ploader = CTrainWndLoader::GetInstance();			//ѵ��ģ�������
	if (!ploader)
		return FALSE;													//�޷���ȡ������

	ploader->SetMainWnd(AfxGetMainWnd()->GetSafeHwnd());				//����������
	ploader->SetWndPath(path);											//����ģ��·��
	ploader->SetParent(pParent);

	BOOL b = ploader->CreateTrainClass(&m_pTrainWnd);					//����ѵ���࣬�������ò���
	if (!b)
	{
		ploader->Free();
		return FALSE;
	}
	CString algpath = m_CurUser + _T("\\" + CONFIGFILE);
	CAlgParamConfig config(algpath);
	config.LoadParam();												//���ز������ڴ�
	config.CloseParmFile();
	m_pTrainWnd->SetParm(config);
	m_pTrainWnd->SetLogOut(LogOut);

	m_listAlgModel.push_back(m_pTrainWnd);							//��ģ�ͼ��ؽ��㷨������
	//������ʾ������
	return ploader->CreateTrainWnd(m_pTrainWnd, pParent);
}

BOOL CCoreModelDoc::CloseTrainModule()
{
	if (!m_pTrainWnd)
		return TRUE;
	m_pTrainWnd->SendMessage(WM_DESTROY);								//���ٴ���,ֻ���ٴ��ڣ���ɾ��ģ��
	m_pTrainWnd = NULL;													//ֻ��Ϊ�ж�,�����ڴ˴��ͷ���Դ
	return TRUE;
}

BOOL CCoreModelDoc::DelTrainModule()
{
	if (!m_pTrainWnd)
		return TRUE;
	m_pTrainWnd->SendMessage(WM_DESTROY);								//���ٴ���,ֻ���ٴ��ڣ���ɾ��ģ��
	delete m_pTrainWnd;
	m_pTrainWnd = NULL;
	CTrainWndLoader* ploader = CTrainWndLoader::GetInstance();			//ѵ��ģ�������
	if (!ploader)
		ploader->Free();												//�ͷ�ģ��
	return TRUE;
}

BOOL CCoreModelDoc::AppendAlgModel(CString path)
{
	CTrainWndLoader* ploader = CTrainWndLoader::GetInstance();			//ѵ��ģ�������
	if (!ploader)
		return FALSE;													//�޷���ȡ������
	CBaseTrainWnd* pClass = NULL;
	ploader->SetWndPath(path);
	if (!ploader->CreateTrainClass(&pClass))							//����ģ��
	{
		ploader->Free();
		return FALSE;
	}
	CString algpath = m_CurUser + _T("\\" + CONFIGFILE);
	CAlgParamConfig config(algpath);
	config.LoadParam();												//���ز������ڴ�
	config.CloseParmFile();

	pClass->SetParm(config);
	pClass->SetLogOut(LogOut);
	char szPath[MAX_PATH];
	Stringoper oper;
	oper.CStringtoChars(config.GetBiteThroldModel(), szPath);
	pClass->LoadResult();											//����ģ��

	m_listAlgModel.push_back(pClass);								//��ģ�ͷ������
	return TRUE;
}

void  CCoreModelDoc::ClearAlgModel()
{
	for each (auto palg in m_listAlgModel)
	{
		delete palg;													//�ͷ��ڴ�
		m_listAlgModel.pop_front();
	}
}

CBaseTrainWnd* CCoreModelDoc::GetBaseTrainWnd()
{
	return m_pTrainWnd;
}

BOOL CCoreModelDoc::RegAppModule(CString path, CWnd* pParent , CRect& rect)
{
	CAppLoader *ploader = CAppLoader::GetInstance();				//������
	if (!ploader)
		return FALSE;
	ploader->SetWndPath(path);
	ploader->SetMainWnd(AfxGetMainWnd()->GetSafeHwnd());			//����������
	BOOL b = ploader->CreateAppClass(&m_pAppWnd);
	if (!b)															//����Ӧ����ʧ��
	{
		ploader->Free();
		return FALSE;
	}
	m_pAppWnd->SetWndSize(rect.Width(), rect.Height());				//����Ӧ�ô�С
	m_pAppWnd->SetLogOut(LogOut);									//���������Ϣ
	return ploader->CreateAppWnd(m_pAppWnd, pParent);
}

BOOL CCoreModelDoc::DelAppModule()
{
	if (!m_pAppWnd)
		return TRUE;
	m_pAppWnd->SendMessage(WM_DESTROY);					//���ٴ���
	delete m_pAppWnd;
	m_pAppWnd = NULL;

	CAppLoader* ploder = CAppLoader::GetInstance();
	if (ploder)
		ploder->Free();
	return TRUE;
}

CBaseAppWnd* CCoreModelDoc::GetCurApp()
{
	return m_pAppWnd;
}

void CCoreModelDoc::GetNewData(ManagedData& data, ManagedData& color)
{
	data = m_EEGNewData;
	color = m_NewColorData;
}

size_t CCoreModelDoc::GetChanNum()
{
	return m_nChannum;
}

CString CCoreModelDoc::GetCurUser()
{
	return m_CurUser;
}

void CCoreModelDoc::SetCurUser(CString path)
{
	m_CurUser = path;
}
BOOL CCoreModelDoc::StartGetDataThread()
{
	m_wndOutput << "�������ݲɼ��߳�" << m_EndOut;
	return StartThread(GetData, m_hDataThread, this);
}

void CCoreModelDoc::PauseGetDataThead()
{
	if (!m_hDataThread&&m_hDataThread == INVALID_HANDLE_VALUE)
		return;
	SuspendThread(m_hDataThread);
}

BOOL CCoreModelDoc::EndGetDataThread()
{
	return EndThread(m_hDataThread);
}

bool CCoreModelDoc::GetGyroData(ManagedData& data)
{
	if (m_GyroData.GetDataSize() <= 0)
		return false;
	data = m_GyroData;
	return true;
}

BOOL CCoreModelDoc::IsTrain()
{
	if (!m_pTrainWnd)
		return FALSE;
	return TRUE;
}

void CCoreModelDoc::LogOut(CString str)					//���
{
	m_wndOutput << str << m_EndOut;
}

void CCoreModelDoc::CalculateColor()
{
	//add cal color here.
	CBaseTrainWnd* pModel = NULL;
	if (m_listAlgModel.size() > 0)
		pModel = (m_listAlgModel.front());
	else
		return;
	if (pModel->m_pAlogrithm[0] == NULL)
		return;
	pModel->m_pAlogrithm[0]->Sim(m_EEGOriginalData, m_NewColorData);		//����ҧ������ɫ��־
	m_ColorData.AppendData(m_NewColorData);

	if (m_hProcessThread == NULL || m_hProcessThread == INVALID_HANDLE_VALUE)
		return;
	if (m_ProcessEvent == NULL || m_ProcessEvent == INVALID_HANDLE_VALUE)
		return;
	
	/*
	��ǰģʽ����0.5�뷢��һ�������ģʽ
	*/
	if (m_PreColorNumber > 0)											//���ಿ�ֲ����д���
	{
		m_PreColorNumber =  UTILE::ZeroNum(m_NewColorData.GetData(), m_NewColorData.GetDataSize())
			>= m_NewColorData.GetDataSize() / m_nChannum ? 0 : 1;		//�ж��Ƿ���ҧ��
		return;
	}
	if (m_CurColorNumber <= 0)
	{
		m_CurColorNumber = UTILE::ZeroNum(m_NewColorData.GetData(), m_NewColorData.GetDataSize())
			>= m_NewColorData.GetDataSize() / m_nChannum ? 0 : 1;			//�ж��Ƿ���ҧ��
	}
	if (m_CurColorNumber > 0)
	{
		m_CurBiteLen += m_NewColorData.GetDataSize() / m_nChannum;		//�ۻ�����
	}
	if (m_CurBiteLen >= (int)(BITELENGTH*m_nSampleRate))				//����0.5��
	{
		SetEvent(m_ProcessEvent);
		m_CurColorNumber = 0;
		m_CurBiteLen = 0;
		m_PreColorNumber =  UTILE::ZeroNum(m_NewColorData.GetData(), m_NewColorData.GetDataSize())
			>= m_NewColorData.GetDataSize() / m_nChannum ? 0 : 1;		//�ж��Ƿ���ҧ��
	}
}

BOOL CCoreModelDoc::StartPrcessThread()
{
	if (m_hProcessThread != NULL&&m_hProcessThread != INVALID_HANDLE_VALUE)
	{
		ResumeThread(m_hProcessThread);					//�ָ��߳�����
		return TRUE;
	}
	m_bProcessExit = FALSE;
	m_hProcessThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OnLineProcess, this, 0, NULL);
	if (m_hProcessThread == NULL || INVALID_HANDLE_VALUE == m_hProcessThread)//����δ�ɹ�
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CCoreModelDoc::EndProcessThread()
{
	m_bProcessExit = TRUE;				//�˳��߳�
	if (m_hProcessThread == NULL || m_hProcessThread == INVALID_HANDLE_VALUE)
		return TRUE;
	DWORD dwRet = WaitForSingleObject(m_hProcessThread, 2000);				//�ȴ�2��ʱ��
	if (dwRet == WAIT_OBJECT_0)
	{
		m_wndOutput << "�˳����ݷ����̳߳ɹ�" << OutEnd;
	}
	else
	{
		DWORD ret = 0;
		GetExitCodeProcess(m_hProcessThread, &ret);
		TerminateThread(m_hProcessThread, ret);								//terminal the thraed mandatory
		m_wndOutput << "�˳����ݷ����̳߳ɹ� �� �����ܻ�����һЩ����" << OutEnd;
	}
	CloseHandle(m_hProcessThread);
	m_hProcessThread = INVALID_HANDLE_VALUE;
	return TRUE;
}

BOOL CCoreModelDoc::SuspendProcessThread()
{
	if (NULL == m_hProcessThread || m_hProcessThread == INVALID_HANDLE_VALUE)
		return TRUE;
	LONG ret = SuspendThread(m_hProcessThread);
	if (ret == 0xFFFFFFFF)
		return FALSE;
	return TRUE;
}
//private function

void CCoreModelDoc::AddOriginalData(double* pd, int size)
{
	RemoveMeanData(pd,size);               //hwh_add
	m_EEGNewData.Remove(m_EEGNewData.GetDataSize() + 1);				//remove all;
	m_EEGNewData.SetChannum(m_nChannum);
	m_EEGNewData.SetSampleRate(m_nSampleRate);
	
	if (m_bIsSaving)
	{
		m_nFile.Write(pd,size*sizeof(double));
		m_nFile.Flush();
	}
	m_EEGNewData.AppendData(pd, size);									//���µ�ǰ��������
	m_NewColorData.Remove(m_NewColorData.GetDataSize() + 1);
	m_NewColorData.SetData(0.0, size);
	m_NewColorData.SetChannum(m_nChannum);
	m_NewColorData.SetSampleRate(m_nSampleRate);
	
	m_GyroData.Remove(m_GyroData.GetDataSize()+1);
	m_GyroData.SetChannum(m_nChannum);
	m_GyroData.SetSampleRate(m_nSampleRate);
	m_GyroData.SetData(pd,size);
	m_EEGOriginalData.AppendData(m_EEGNewData);							//����������ӵ���������

///	m_ColorData.AppendData(m_EEGNewData);								//������ɫ����
}

BOOL CCoreModelDoc::StartThread(UINT(*pf)(LPVOID), HANDLE handle, LPVOID lparam)
{
	if (!pf)
		return FALSE;
	if (!handle || handle == INVALID_HANDLE_VALUE)
	{
		m_bExit = false;
		handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)*pf, lparam, 0, NULL);
		if (handle == INVALID_HANDLE_VALUE)
			return FALSE;
		return TRUE;
	}
	m_bExit = false;
	ResumeThread(handle);
	return TRUE;
}

BOOL CCoreModelDoc::EndThread(HANDLE handle)
{
	m_bExit = true;
	if (handle == NULL || INVALID_HANDLE_VALUE == handle)
		return FALSE;
	DWORD dwRet = WaitForSingleObject(handle, 2000);
	if (dwRet == WAIT_OBJECT_0)
	{
		m_wndOutput << "  Exit the Thread Succsess!" << OutEnd;
	}
	else
	{
		DWORD ret = 0;
		GetExitCodeProcess(handle, &ret);
		TerminateThread(handle, ret);								//terminal the thraed mandatory
		m_wndOutput << "Exit the thread but may cause some problem" << OutEnd;
	}
	CloseHandle(handle);
	handle = NULL;
	return TRUE;
}

void CCoreModelDoc::Notify()
{
	CWnd* pWnd = AfxGetApp()->m_pMainWnd;
	if (pWnd)
		pWnd->SendMessage(MYWM_MAINFRAMINNER, INNERMAINFRAME_UPDATA, 0);
}

void CCoreModelDoc::OnCloseDocument()
{
	// TODO:�ر��̣߳��ͷ���Դ
	if (!EndGetDataThread())
		m_wndOutput << L"�������ݲɼ��߳�ʧ��" << m_EndOut;
	for each (auto obj in m_listAlgModel)			//�ͷ�ģ��
	{
		delete obj;
	}
	m_listAlgModel.clear();
	CDocument::OnCloseDocument();
}

void CCoreModelDoc::ExtractData(ManagedData& rawdata, ManagedData& filtd)
{
	rawdata.Remove(rawdata.GetDataSize() + 1);		//clear the data first.
	filtd.Remove(filtd.GetDataSize() + 1);

	//extrace one second buffer
	m_EEGOriginalData.CopyData(rawdata, m_nSampleRate*m_nChannum, false);			//��ȡ1������ݣ���ĩβ
	//m_FiltEEGData.CopyData(filtd, m_nSampleRate*m_nChannum, false);
	m_ColorData.CopyData(filtd, m_nSampleRate*m_nChannum, false);
}

void CCoreModelDoc::ShowCmdCode(int cmd)
{
	CWnd* pWnd = AfxGetApp()->m_pMainWnd;
	if (pWnd)
		pWnd->PostMessageW(MYWM_MAINFRAMINNER, INNERMAINFRAME_CMDCODE, cmd);
}

UINT CCoreModelDoc::Analysis(ManagedData& rawd, ManagedData& colord)
{
	int count = 0;
	int sum = 0;
	int classify = -1;
	for each (auto pWnd in m_listAlgModel)
	{
		count++;
		if (count == 1)
			continue;
		else if (count == 2)			//����ģ��
		{
			if (pWnd->m_pAlogrithm[0])
			{
				int res = pWnd->m_pAlogrithm[0]->Sim(rawd, colord);
				/*-1:left , 0:right , 1 : both*/
				if (!res)
					return 0;
				classify = (int)colord[0];		//��Ƿ���L �� R  , B
				sum = (int)classify * 4 + 5;
			}
			else
			{	
				return 0;			//��Чģ�ͣ�������Ч����
			}
		}
		else if (count == 3)			//����ģ��
		{
			//heavy ��1 �� ��Light ��0 and the -1 is invalid value
			if (classify==-1 && pWnd->m_pAlogrithm[0])				//left
			{
				int res = pWnd->m_pAlogrithm[0]->Sim(rawd, colord);
				if (!res)
					return 0;
				sum += (int)colord[0];
			}
			else if (classify == 0 && pWnd->m_pAlogrithm[1])		//right
			{
				int res = pWnd->m_pAlogrithm[1]->Sim(rawd, colord);
				if (!res)
					return 0;
				sum += (int)colord[0];
			}
			else if (classify == 1 && pWnd->m_pAlogrithm[2])		//right
			{
				int res = pWnd->m_pAlogrithm[2]->Sim(rawd, colord);
				if (!res)
					return 0;
				sum += (int)colord[0];
			}
			else
			{
				return 0;
			}
		}
		else if (count == 4)									//��ҧ������ģ��
		{
			if (classify == -1 && pWnd->m_pAlogrithm[0])				//left
			{
				int res = pWnd->m_pAlogrithm[0]->Sim(rawd, colord);
				if (!res)
					return 0;
				sum += (int)colord[0];
			}

			else if (classify == 0 && pWnd->m_pAlogrithm[1])		//right
			{
				int res = pWnd->m_pAlogrithm[1]->Sim(rawd, colord);
				if (!res)
					return 0;
				sum += (int)colord[0];
			}
			
			else if (classify == 1 && pWnd->m_pAlogrithm[2])		//both
			{
				int res = pWnd->m_pAlogrithm[2]->Sim(rawd, colord);
				if (!res)
					return 0;
				sum += (int)colord[0];
			}
			else
				return 0;
		}
	} 
	return sum;
}

UINT CCoreModelDoc::GetData(LPVOID lpvoid)
{
	OVERLAPPED ov;							//�߳�ͬ��
	HANDLE hGetDataEvent;					//ͬ���¼�
	CCoreModelDoc* pDoc = (CCoreModelDoc*)lpvoid;
	if (!pDoc)
		return 1;
	BasicDevice* pDevice = pDoc->m_pDevice;
	if (!pDevice)
		return 1;
	//�����豸��
	if (!pDevice->EB_OpenDevice())
	{
		TRACE(_T("Open the device failed\n"));
		return 1;
	}
	if (!pDevice->EB_Startup())
	{
		TRACE(_T("start the device failed\n"));
		return 1;
	}
	int buffersize = 48;
	buffersize = pDoc->m_nBuffsize*pDoc->m_nChannum;
	double *pdata = new double[buffersize];
	int mode;
	if (pDevice->EB_GetMode(&mode) && mode == M_IMPEDANCE)
	{
		//change the state.
		pDevice->EB_SetMode(M_NORMAL);
	}
	DWORD ret;
	hGetDataEvent = CreateEvent(NULL, true, true, NULL);
	ov.hEvent = hGetDataEvent;

	//��ʼ�ɼ�����
	while (!pDoc->m_bExit)
	{
		ret = WaitForSingleObject(hGetDataEvent, INFINITE);			//just for debug testing.
		if (ret == WAIT_TIMEOUT)
		{
			pDevice->EB_Stop();
			pDevice->EB_CloseDevice();
			pDoc->m_bExit = TRUE;
			AfxMessageBox(_T("Get Data from device Time out"));
			return 1;
		}
		if (pDevice->EB_GetData(pdata, buffersize, &ov))
		{
			//append to doc buffer.
			pDoc->AddOriginalData(pdata, buffersize);
			//AfxGetMainWnd()->SendMessage(MYWM_MAINFRAMINNER,INNERMAINFRAME_UPDATA , 0);	//֪ͨ����
			pDoc->Notify();
			
		}
	}

	pDevice->EB_Stop();
	pDevice->EB_CloseDevice();
	delete pdata;
	return 0;
}


//�����ݽ��д���
UINT CCoreModelDoc::OnLineProcess(LPVOID lpvoid)
{
	CCoreModelDoc* pDoc = (CCoreModelDoc*)lpvoid;
	if (!pDoc)
	{
		m_wndOutput << "�޷��������ݴ����߳�" << m_EndOut;
		return 1;
	}
	pDoc->m_ProcessEvent = CreateEvent(NULL, TRUE ,TRUE, NULL);			//�������ݴ���ͬ���߳�
	ResetEvent(pDoc->m_ProcessEvent);

	while (!pDoc->m_bProcessExit)
	{
		WaitForSingleObject(pDoc->m_ProcessEvent, INFINITE);			//�ȴ�֪ͨ
		ResetEvent(pDoc->m_ProcessEvent);								//ֻ����һ�Σ�Ԥ���ظ�
		ManagedData rawd, colord;
		
		//��ȡ����
		pDoc->ExtractData(rawd, colord);
		//��������
		UINT res = pDoc->Analysis(rawd, colord);
		pDoc->ShowCmdCode(res);
		//��������͸���ǰӦ�û�ѵ��ģ��
		if (res == 0)								//0 Ϊ��Ч����
		{
			continue;
		}
		if (pDoc->m_pTrainWnd)				//ѵ��ģ�����
		{
			pDoc->m_pTrainWnd->SendMessage(MYWM_TRAINWMD, TRAINWND_PROCESSRESULT, (LPARAM)res);
		}
		if (pDoc->m_pAppWnd)				//Ӧ��ģ�����
		{
			pDoc->m_pAppWnd->SendAppCmd(res);
		}
	}
	
	return 0;
}

bool CCoreModelDoc::SendFlagState()
{
	return m_bIsSaving;
}

void CCoreModelDoc::SetState()
{
	if (m_bIsSaving == false)
	{
		m_bIsSaving = true;
	}
	else
		m_bIsSaving = false;
}

bool CCoreModelDoc::WriteHeader()
{
	if (m_nFile.m_hFile == NULL)
		return false;
	m_nFile.SeekToBegin();
	m_nFile.SetLength(0);
	string s = "SampleRate = ";
	Stringoper soper;
	string s2 = soper.inttostring(m_nSampleRate);
	s += (s2 + "\r\n");
	m_nFile.Write(s.c_str(), s.length());

	s = "Channum = ";
	s2 = soper.inttostring(GetChanNum());
	s += (s2 + "\r\n");
	m_nFile.Write(s.c_str(), s.length());


	return true;
}

bool CCoreModelDoc::SaveData()
{
	if (m_bIsSaving)
	{
		m_bIsSaving = false;
		m_nFile.Close();
	}
	else
	{
		CString path = CreateSavingFile();
		if (path.GetLength() <= 0)
			return false;
		m_nFile.Open(path, CFile::modeCreate | CFile::modeReadWrite);
		WriteHeader();
		m_bIsSaving = true;
	}
	return true;
}

CString CCoreModelDoc::CreateSavingFile()
{
	CString str = m_CurUser;
	CString fpath = str + _T("\\Raw_Test_Date");
	if (!PathIsDirectory(fpath))			//create the directory
	{
		CreateDirectory(fpath, NULL);
	}

	//check the file .
	CString filename = _T("RDate");
	CFileFind finder;
	int i = 0;
	CString tmp;
	while (i++<100)
	{
		tmp.Format(_T("%s\\%s%d.dat"), fpath, filename, i);
		if (!finder.FindFile(tmp))
			break;
	}
	if (i <= 100)
		return tmp;
	else
		return _T("");
}

double CCoreModelDoc::mean(const double d[], int len)
{
	if (len <= 0) return -1;
	double sumv = 0.0;
	for (int i = 0; i<len; i++)
	{
		sumv += d[i];
		if (d[i]>10000000)
			cerr << "mean err" << endl;
	}
	return sumv / len;
}

void CCoreModelDoc::RemoveMeanData(double* pd, int size)
{
	if (m_nChannum == 2)
	{
		if (m_nMeanValue == 0)
		{
			m_nMeanValue = mean(pd,size);
		}
		else
		{
			for (int i = 0; i < size; i++)
			{
				pd[i] -= m_nMeanValue;
			}
		}
	}
	else if (m_nChannum == 6)
	{
		double *ptmp = new double[size / m_nChannum * 2];
		for (int i = 0; i < size / m_nChannum; i++)
		{
			pd[i * 2] = pd[i*m_nChannum] - pd[i*m_nChannum + 1];
			pd[i * 2 + 1] = pd[i*m_nChannum + 3] - pd[i*m_nChannum + 2];
			ptmp[i * 2] = pd[i*m_nChannum + 4];
			ptmp[i * 2 + 1] = pd[i*m_nChannum + 5];
		}
		size = size / m_nChannum * 2;
		delete[]ptmp;
	}
	else
	{
		for (int i = 0; i < size / m_nChannum; i++)
		{
			pd[i * 2] = pd[i*m_nChannum] - pd[i*m_nChannum + 1];
			pd[i * 2 + 1] = pd[i*m_nChannum + 3] - pd[i*m_nChannum + 2];
		}
		size = size / m_nChannum * 2;
	}
} 
// CCoreModelDoc ����

