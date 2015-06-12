// ClasssifyWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClassifyTrain.h"
#include "ClasssifyWnd.h"
#include "afxdialogex.h"
#include "Stringoper.h"
#include "ThreadUtil.h"
#include "NetWorker.h"
#include "DataProcess.h"
#include "ClassifyTrain.h"
#include "FileOper.h"

using namespace DATAPROCESS;
extern CClassifyTrainApp theApp;
// CClasssifyWnd �Ի���
extern HANDLE MianEvent;						//���ڴ������̺߳����߳�

const int TIMER_FOR_TRAIN = 0;

IMPLEMENT_DYNAMIC(CClasssifyWnd, CDialogEx)

CClasssifyWnd::CClasssifyWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClasssifyWnd::IDD, pParent)
{
	m_Error = 0.01;
	m_MaxTrainTime = 1000;
	m_nSelArth = 0;					//default
	m_HindLayer = 20;
	m_pNetWorker = NULL;
}

CClasssifyWnd::~CClasssifyWnd()
{
}

void CClasssifyWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SSTATE, m_strState);
	DDX_Text(pDX, IDC_EDITINSTRUCT, m_strInstruct);

	DDX_Text(pDX, IDC_EDITL2M, m_dL2M);
	DDX_Text(pDX, IDC_EDITM2R, m_dM2R);
	DDX_Text(pDX, IDC_EDITRESULT, m_nResult);

	DDX_Control(pDX, IDC_PROGRESSSTATE, m_ProgressState);
	DDX_Control(pDX, IDC_SLIDERLTOM, m_Left2Middle);
	DDX_Control(pDX, IDC_SLIDERMTOR, m_Middle2Right);
	DDX_Control(pDX, IDOK, m_ButtonOK);
	DDX_Control(pDX, IDC_BUTTONTEST, m_ButtonTest);
}
// implement the interface
void CClasssifyWnd::SetParm(CAlgParamConfig& parm)
{
	m_bIsTrain = parm.GetClassifystate();
	m_bReTrain = parm.IsRetrainClassify();
	m_nWinLen = parm.GetWindowLength();
	m_nWinOverLen = parm.GetOverLength();
	parm.GetClassifyTrain(m_nTraintime);					//��ȡѵ��ʱ��
	m_ModlePath = parm.GetClassifyModel();					//��ȡģ�ͱ����·��
	m_DataSavePath[0] = parm.GetClassifyLData();
	m_DataSavePath[1] = parm.GetClassifyRData();
	m_DataSavePath[2] = parm.GetClassifyBData();
	m_BiteThrold = LoadThrold(parm.GetBiteThroldModel());	//������ֵģ��
	parm.GetLRThrolds(m_dL2M, m_dM2R);
}

void CClasssifyWnd::SetData(ManagedData& d)
{
	if (m_CurState != STATE_START)						//�ж��Ƿ��ǿ�ʼѵ����
		return;
	m_BufData.AppendData(d);							//�������ȷ�����ʱ������
	SetEvent(m_hTrainEvent);							//֪ͨ���̣߳����Լ�����������
}

bool CClasssifyWnd::SaveData()
{
	if (!m_bIsOpen)
		return false;
	if (m_nFile.GetLength() <= 0)						//�ļ��ոտ�ʼд�룬����ͷ�ļ���Ϣ��samplerate and channal number
	{
		string str;
		Stringoper oper;
		str = "Samplerate = " + oper.inttostring(m_BufData.GetSampleRate())+"\r\n";
		m_nFile.Write(str.c_str(), str.size());
		str = "Channum = " + oper.inttostring(m_BufData.GetChannum())+"\r\n";
		m_nFile.Write(str.c_str(), str.size());
		m_nFile.Flush();
	}
	m_nFile.Write(m_BufData.GetData(), m_BufData.GetDataSize()*sizeof(double));
	m_BufData.Remove(m_BufData.GetDataSize()+1);		//��ջ���������
	ResetEvent(m_hTrainEvent);
	//change the process.
	ULONGLONG dwlength = m_nFile.GetLength();
	ULONGLONG length = dwlength / sizeof(double);
	ULONGLONG total = m_BufData.GetSampleRate()*m_CurTrainTime*m_BufData.GetChannum();
	m_nProgress = (int)(length * 100 / total);						//���ȼ���

	if (m_nProgress >= 100)
	{
		m_nProgress = 100;
		m_nFile.Close();										//�ر��ļ�
		m_bIsOpen = false;

		if (m_nFileCount >= 3)									//3�����趼�Ѿ�ѵ�����
		{
			m_bExit = TRUE;
			SendMessage(CLASSIFY_ONEJOBDONE, JOB_ALLDONE, 0);		//�����������
		}														//exit the thread.
		else
			SendMessage(CLASSIFY_ONEJOBDONE, JOB_ONEDONE, 0);				//����֪ͨ���һ������
	}
	this->SendMessage(WM_PAINT);								//update the state

	return true;
}

bool CClasssifyWnd::LoadData()
{
	if (m_bIsOpen)								//ʵ�����ʱ����,�ر��ļ�
	{
		m_nFile.Close();
		m_bIsOpen = false;
	}
	
	for (size_t i = 0; i < m_nTraintime.size();i++)
	{
		m_nProgress = 0;
		if (i == 0)
		{
			m_strState = L"������ҧ����...";
		}
		else if (i == 1)
			m_strState = L"������ҧ����...";
		else if (i == 2)
			m_strState = L"����ͬʱҧ����...";
		//SendMessage(WM_PAINT);
		Invalidate(TRUE);
		MyLoaddata(m_DataSavePath[i], m_TrainDataBuf[i]);
		Sleep(500);
	}
	return true;
}

bool CClasssifyWnd::DoProcess()
{
	Stringoper oper;
	char path[MAX_PATH];
	oper.CStringtoChars(m_ModlePath, path);
	m_strState = L"���ڴ�������...";
	//SendMessage(WM_PAINT);
	Invalidate(TRUE);

	//���ȴ���ģ�Ͳ����ļ�
	if (!CreateModelFile())
	{
		SendMessage(MYWM_THREADMSG, CLASSIFY_CREATEFAILED);
		return false;
	}
	else
		SendMessage(MYWM_THREADMSG, CLASSIFY_CREATEDONE);
	//����ģ�Ͳ����ò���
	m_pNetWorker = new CNetWorker(path);				//����ѵ��ģ��
	m_pNetWorker->SetParament(m_Error, m_MaxTrainTime);
	m_pNetWorker->SetLRThrolds(m_dL2M, m_dM2R);		
	m_pNetWorker->SetWinlen(m_nWinLen);
	m_pNetWorker->SetThreashold(m_BiteThrold);

	//��ʼ��������
	BpNetdata **pNet = new BpNetdata *[3];				//��������&ͬʱҧ������
	int Smaplenum[3];									//ÿ�����ݵ�������
	for (int i = 0; i < 3;i++)
	{
		ManagedData Newdata;
		ProcessData(m_TrainDataBuf[i], Newdata);		//�˲�3~100HZ
		pNet[i] = new BpNetdata[1000];					//������ȡ������
		Processdata prodata[1000];
		//��ȡҧ������
		Smaplenum[i] = ExtracSample(m_TrainDataBuf[i], Newdata, pNet[i], prodata, m_BiteThrold, m_nWinLen);
	}
	//before training the data. mix the sample data.
	Matrix out;
	out.pdata = new double *[3];						//����
	out.colnum = Smaplenum[0] + Smaplenum[1] + Smaplenum[2];
	out.raw = 3;
	Matrix target;
	target.pdata = new double *[1];						//���
	target.raw = 1;
	target.colnum = Smaplenum[0] + Smaplenum[1] + Smaplenum[2];
	//����ռ�
	for (int i = 0; i < 3; i++)
	{
		out.pdata[i] = new double[out.colnum];
	}
	target.pdata[0] = new double[target.colnum];
	//���л�ϣ�����������ѵ�����������
	mixBpdata(pNet[0], Smaplenum[0], pNet[1], Smaplenum[1], pNet[2], Smaplenum[2], out, target);
	for (int i = 0; i < 3; i++)
	{
		delete pNet[i];
	}
	delete pNet;
	
	//�����ݱ��浽��ʱ�ļ���
	CString datapath = GetDirectory() + L"\\Classify_NetData.txt";
	CStdioFile file;
	if (!file.Open(datapath, CFile::modeCreate | CFile::modeWrite))
	{
		for (int i = 0; i < 3; i++)
		{
			delete out.pdata[i];
		}
		delete target.pdata[0];
		delete out.pdata;
		delete target.pdata;
		return false;
	}

	//save the result to the file.
	if (m_pNetWorker->m_BpNet.m_nTrainArithmetic == 2)			//support the FANN data format.
	{
		Streamoper oper;
		string str = "";
		str += oper.SerizlizeStream((int)out.colnum) + "\t";
		str += oper.SerizlizeStream((int)(m_pNetWorker->m_BpNet.m_nInputLayerNumber)) + "\t";
		str += oper.SerizlizeStream((int)(m_pNetWorker->m_BpNet.m_nOutputLayerNumber)) + "\n";
		Stringoper sp;
		file.WriteString(sp.tocstring(str));
	}
	for (int i = 0; i < out.colnum; i++)
	{
		Streamoper oper;
		string str = "";
		for (int j = 0; j < 3; j++)
		{
			str += oper.SerizlizeStream(out.pdata[j][i]) + "\t";
		}
		str += oper.SerizlizeStream(target.pdata[0][i]) + '\n';
		Stringoper sp;
		file.WriteString(sp.tocstring(str));
	}
	file.Close();

	//�ļ���������󣬿�ʼѵ������
	for (int i = 0; i < 3; i++)
	{
		delete out.pdata[i];
	}
	delete target.pdata[0];
	delete out.pdata;
	delete target.pdata;

	SetTimer(TIMER_FOR_TRAIN, 600, NULL);					//0.6����һ��״̬

	Stringoper soper;
	oper.CStringtoChars(datapath, path);

	m_strState = L"����ѵ��ģ��...";
	//SendMessage(WM_PAINT);
	Invalidate();											//����״̬
	bool res = m_pNetWorker->Train(path);					//��ʼѵ��

	//��������浽��ʱ�ļ�����������ɾ��
	oper.CStringtoChars(GetDirectory() + L"\\tmpModel.txt", path);
	m_pNetWorker->SaveTrainModel(path);

	KillTimer(TIMER_FOR_TRAIN);
	SendMessage(CLASSIFY_ONEJOBDONE, JOB_PROCESSDONE);		//������һ������
	return res;
}

bool CClasssifyWnd::ChangeState(CAlgParamConfig& parm, bool s)
{
	parm.SetBiteState(s);
	return true;
}

bool CClasssifyWnd::StartThread(DWORD(WINAPI lp)(LPVOID), HANDLE& handle)
{
	if (NULL == lp)
		return false;
	//m_bExit = false;
	if (handle == NULL)
	{
		handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)*lp, this, 0, NULL);
		if (!handle)
			return false;
		m_nProgress = 0;
		return true;
	}
	return false;
}

bool CClasssifyWnd::EndThread(HANDLE& handle)
{
	m_bExit = TRUE;
	if (handle)
	{
		DWORD dwRet = WaitForSingleObject(handle, 2000);
		if (dwRet == WAIT_OBJECT_0)
		{
			TRACE("Exit successed\n");
		}
		else
		{
			DWORD ret = 0;
			GetExitCodeProcess(handle, &ret);
			TerminateThread(handle, ret);
			TRACE("Exit the thread but may cause some problem\n");
		}
	}
	CloseHandle(handle);
	handle = NULL;
	return true;
}

bool CClasssifyWnd::SaveResult()
{
	if (!m_pNetWorker)
		return false;
	Stringoper oper;
	char path[MAX_PATH];
	oper.CStringtoChars(m_ModlePath, path);

	CString algpath = m_ModlePath.Left(m_ModlePath.ReverseFind('\\')+1);	//���Ƽ��÷�ʽ
	algpath += CONFIGFILE;
	CAlgParamConfig config(algpath);
	if (config.IsOpen())
	{
		config.LoadParam();												//���ز������ڴ�
		config.SetLRThrolds(m_dL2M, m_dM2R);
		config.SaveParam();
		config.CloseParmFile();
	}

	return m_pNetWorker->SaveTrainModel(path);
}

bool CClasssifyWnd::LoadResult()
{
	Stringoper oper;
	char path[MAX_PATH];
	oper.CStringtoChars(m_ModlePath, path);
	if (!m_pAlogrithm[0])
	{
		m_pNetWorker = new CNetWorker();
		bool b =  m_pNetWorker->LoadModel(path);
		if (b)
		{
			//������ز���
			m_pNetWorker->SetParament(m_Error, m_MaxTrainTime);
			m_pNetWorker->SetLRThrolds(m_dL2M, m_dM2R);
			m_pNetWorker->SetWinlen(m_nWinLen);
			m_pNetWorker->SetThreashold(m_BiteThrold);
			m_pNetWorker->m_BpNet.m_bSimulateDataFlag = true;
			m_pAlogrithm[0] = m_pNetWorker;
			//SendMessage(MYWM_THREADMSG, CLASSIFY_MODELOK);
		}
		else
		{
			delete m_pNetWorker;
			m_pNetWorker = NULL;
			//SendMessage(MYWM_THREADMSG, CLASSIFY_MODELCANCLE);
		}
		return b;
	}
	//reload the model.
	return m_pAlogrithm[0]->LoadModel(path);
}

void CClasssifyWnd::SetLogOut(void(*pFunc)(CString))
{
	m_pOutPut = pFunc;
}

BEGIN_MESSAGE_MAP(CClasssifyWnd, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CClasssifyWnd::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTONTEST, &CClasssifyWnd::OnBnClickedButtontest)
	ON_BN_CLICKED(IDCANCEL, &CClasssifyWnd::OnBnClickedCancel)
	ON_MESSAGE(MYWM_THREADMSG, OnReceivMsg)
	ON_WM_TIMER()
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_PARAM, OnParam)
	ON_MESSAGE(CLASSIFY_ONEJOBDONE , OnJobMsg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERLTOM, &CClasssifyWnd::OnNMCustomdrawSliderltom)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERLTOM, &CClasssifyWnd::OnNMReleasedcaptureSliderltom)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERMTOR, &CClasssifyWnd::OnNMCustomdrawSlidermtor)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERMTOR, &CClasssifyWnd::OnNMReleasedcaptureSlidermtor)
END_MESSAGE_MAP()


// CClasssifyWnd ��Ϣ�������


BOOL CClasssifyWnd::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if (!m_bReTrain)
	{
		m_CurState = STATE_NONE;				//��ʼ״̬Ϊ��
		m_bExit = FALSE;
	}
	else
	{
		m_CurState = STATE_RETRAIN;
		m_bExit = TRUE;							//���������ݲɼ�����
	}

	m_nFileCount = 0;
	m_strState = "ѵ������ģ��...";
	m_ButtonOK.SetWindowText(L"��ʼ");
	m_ButtonTest.EnableWindow(FALSE);

	m_Left2Middle.SetRange(0, 10);				//-1~0
	m_Middle2Right.SetRange(0, 10);				//0~1
	m_Left2Middle.SetPos((int)(abs(m_dL2M) * 10));
	m_Middle2Right.SetPos((int)(10 * m_dM2R));
	
	UPDATAVALTOCONTROL;

	//move the window to center
	CRect rect;
	GetParent()->GetClientRect(rect);
	CRect clientrect;
	GetClientRect(clientrect);

	this->MoveWindow(CRect(rect.left + (rect.Width() - clientrect.Width()) / 2,
		rect.top + (rect.Height() - clientrect.Height()) / 2,
		(rect.Width() + clientrect.Width()) / 2,
		(rect.Height() + clientrect.Height()) / 2));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


HBRUSH CClasssifyWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC&&(pWnd->GetDlgCtrlID()!=IDC_SLIDERLTOM
		&&pWnd->GetDlgCtrlID() != IDC_SLIDERMTOR&&IDC_EDITRESULT!=pWnd->GetDlgCtrlID()))
	{
		pDC->SetTextColor(RGB(0, 0, 0));				//���ó��㱳������ɫ   
		pDC->SetBkMode(TRANSPARENT);						//͸��   
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}


BOOL CClasssifyWnd::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	COLORREF bckcolor = RGB(255, 255, 255);				//white.

	pDC->FillSolidRect(rect, bckcolor);

	//return __super::OnEraseBkgnd(pDC);
	return TRUE;
}


void CClasssifyWnd::OnDestroy()
{
	__super::OnDestroy();

	//�˳����ڣ�����������ѵ����Դ

}

void CClasssifyWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_strInstruct.Format(L"���ҧ��ʱ����%d��\r\n�ұ�ҧ��ʱ����%d��\r\nͬʱҧ��ʱ����%d;\r\nҧ��ʱ����������������",
		m_nTraintime.at(0),m_nTraintime.at(1),m_nTraintime.at(2));
	UPDATAVALTOCONTROL;
	m_ProgressState.SetPos(m_nProgress);
}


void CClasssifyWnd::OnBnClickedOk()
{
	switch (m_CurState)
	{
	case CClasssifyWnd::STATE_NONE:
		ProcessNone();
		break;
	case CClasssifyWnd::STATE_START:
		ProcessStart();
		break;
	case CClasssifyWnd::STATE_PAUSE:
		ProcessPause();
		break;
	case CClasssifyWnd::STATE_DONE:
		ProcessDone();
		break;
	case CClasssifyWnd::STATE_RETRAIN:
		ProcessRetrain();
		break;
	case  CClasssifyWnd::STATE_STOP:
		ProcessStop();
		break;
	default:
		break;
	}
}


void CClasssifyWnd::OnBnClickedButtontest()
{
	//֪ͨcoremodule�����в��ԣ�
	CWnd* pParent = this->GetParent();
	if (!pParent)
	{
		m_pOutPut(L"�޷�֪ͨ��ģ�飺�޷���ȡ��ģ����");
	}
	CFileOper foper;
	if (!foper.FileExist(GetDirectory() + L"\\tmpModel.txt"))
	{
		m_pOutPut(L"�޷����в��ԣ��޷��ҵ�����ģ��");
		return;
	}
	if (m_pNetWorker == NULL)
	{
		m_pNetWorker = new CNetWorker();
	}
	char path[MAX_PATH];
	Stringoper oper;
	oper.CStringtoChars(GetDirectory() + L"\\tmpModel.txt", path);
	m_pNetWorker->LoadModel(path);
	m_pOutPut(L"�����翪ʼ����");
	pParent->SendMessage(MYWM_TRAINWMD, TRAINWND_TEST,0);				//��ʾ��ʼ����
}


void CClasssifyWnd::OnBnClickedCancel()
{
	//ȡ������
	if (AfxMessageBox(L"ģ����δ���棬�Ƿ񱣴�?", MB_YESNO) == IDYES)
	{
		if (SaveResult())
			m_pOutPut(L"ģ�ͱ���ɹ�");
		else
		{
			m_pOutPut(L"ģ�ͱ���ʧ��");
		}
	}
	else
	{
		m_pOutPut(L"������ǰģ��...");
		LoadResult();
	}
	GetParent()->SendMessage(MYWM_TRAINWMD, TRAINWND_CLASSIFYCANCLE);
	CFileOper oper;
	oper.DeleteDirectory(GetDirectory() + L"\\tmpModel.txt");
	OnCancel();
}

LRESULT CClasssifyWnd::OnReceivMsg(WPARAM wparam, LPARAM lparam)
{
	Stringoper oper;
	switch (wparam)
	{
	case THREADMSG_PROCESSFINISH:
		m_CurState = STATE_DONE;
		m_ButtonOK.SetWindowTextW(L"���");
		if (m_pOutPut)
			(*m_pOutPut)(L"����ѵ��ģ�飺ģ��ѵ����ɣ�");
		m_pNetWorker->m_BpNet.m_bSimulateDataFlag = true;
		m_pAlogrithm[0] = m_pNetWorker;							//ģ�Ϳ��á���
		break;
	case THREADMSG_LOADFINISH:
		if (m_pOutPut)
			(*m_pOutPut)(L"����ѵ��ģ�飺����������ɣ�");
		break;
	case THREADMSG_SAVEFINISH:
		if (m_pOutPut)
			(*m_pOutPut)(L"����ѵ��ģ�飺����������ɣ�");
		break;
	case CLASSIFY_CREATEDONE:
		if (m_pOutPut)
			(*m_pOutPut)(L"��������ģ������ļ���ɣ�");
		break;
	case CLASSIFY_CREATEFAILED:
		if (m_pOutPut)
			(*m_pOutPut)(L"��������ģ������ļ�ʧ�ܣ�");
		break;
	case CLASSIFY_MODELCANCLE:
		if (m_pOutPut)
			(*m_pOutPut)(L"���ط���ģ��ʧ�ܣ�");
		break;
	case CLASSIFY_MODELOK:
		if (m_pOutPut)
			(*m_pOutPut)(L"���ط���ģ�ͳɹ���");
		break;
	case TRAINWND_PROCESSRESULT:
		ProcessResult(int(lparam));
		break;
	default:
		break;
	}
	return 0;
}

void CClasssifyWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_FOR_TRAIN)
	{
		if (m_pNetWorker)						//��ȡѵ��״̬��������ʾ
		{
			double err;
			int time;
			m_pNetWorker->GetTrainState(err, time);
			m_strState.Format(L"ѵ��������״̬:\r\n     ѵ��������%d\r\n     ϵͳ��%lf", time, err);
			CRect rect;
			CRect ParentRect;
			GetWindowRect(ParentRect);
			GetDlgItem(IDC_SSTATE)->GetWindowRect(rect);
			InvalidateRect(CRect(rect.left-ParentRect.left , rect.top-ParentRect.top ,
				ParentRect.right-rect.right , ParentRect.bottom-rect.bottom));
			SendMessage(WM_PAINT);
		}
	}
	__super::OnTimer(nIDEvent);
}


void CClasssifyWnd::OnNMCustomdrawSliderltom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  ��ʾ������Χ
	int pos = m_Left2Middle.GetPos();
	m_dL2M = pos / 10.0 * (-1);
	UPDATAVALTOCONTROL;
	*pResult = 0;
}


void CClasssifyWnd::OnNMReleasedcaptureSliderltom(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  ��ģ�ͽ��в�������

	*pResult = 0;
}


void CClasssifyWnd::OnNMCustomdrawSlidermtor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  ��ʾ������Χ
	int pos = m_Middle2Right.GetPos();
	m_dM2R = pos / 10.0;
	UPDATAVALTOCONTROL;
	*pResult = 0;
}


void CClasssifyWnd::OnNMReleasedcaptureSlidermtor(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  ��ģ�Ͳ������и���

	*pResult = 0;
}

//private function

Throld CClasssifyWnd::LoadThrold(CString path)
{
	Throld throld = {0.0,0.0};
	CStdioFile file;
	if (!file.Open(path, CFile::modeRead))
	{
		m_pOutPut(L"�޷�������ֵģ��");
		return throld;
	}
	Stringoper oper;
	CString str;
	file.ReadString(str);
	string _key, val;
	char comment = '=';
	oper.AnalyseLine(oper.tostring(str), _key, val, comment);
	throld.stdvalue = oper.stringtoint(val);

	file.ReadString(str);
	oper.AnalyseLine(oper.tostring(str), _key, val, comment);
	throld.voltvalue = oper.stringtoint(val);
	file.Close();
	return throld;
}

void CClasssifyWnd::MyLoaddata(CString path, ManagedData& buf)
{
	CFile file;
	bool isopen = file.Open(path, CFile::modeRead)==TRUE?true:false;
	
	if (!isopen)
	{
		m_pOutPut(L"�޷����ļ�:"+path);
		return;
	}
	file.SeekToBegin();
	ULONGLONG length = file.GetLength() / sizeof(double);
	buf.ResizeManageData((int)length + 1);
	//buf.SetSampleRate(m_nSampleRate);
	//buf.SetChannum(m_nChannum);
	/*���ȶ�ȡ�ļ�ͷ*/
	TRY 
	{
		Stringoper oper;
		char strbuf[512];
		UINT ret = file.Read(strbuf, 512);
		string str = oper.charstostring(strbuf);
		str = str.substr(0,str.find('\n')+1);
		string key, val;
		oper.AnalyseLine(str, key, val, '%');
		buf.SetSampleRate(oper.stringtoint(val));

		file.Seek(str.size() , CFile::begin);			//���ö�ȡλ��
		int	pos = str.size();

		ret = file.Read(strbuf, 512);
		str = oper.charstostring(strbuf);
		str = str.substr(0, str.find('\n')+1);
		oper.AnalyseLine(str, key, val, '%');
		buf.SetChannum(oper.stringtoint(val));

		file.Seek(pos + str.size(), CFile::begin);		//��ȡ��ͷ
	}
	CATCH(CMemoryException, e)
	{
		file.Close();
		m_pOutPut(L"�ļ�ͷ��ȡ����");
		return;
	}
	END_CATCH

	int ret = 0;
	double *pd = new double[1000];
	ULONGLONG total = 0;
	while (ret = file.Read(pd, 1000))					//��ȡ����
	{
		total += ret / sizeof(double);
		m_nProgress = total * 100 / length;
		buf.AppendData(pd, ret / sizeof(double));
		Sleep(1);
		SendMessage(WM_PAINT);
	}
	delete pd;
	file.Close();
}

bool CClasssifyWnd::CreateModelFile()
{
	if (m_ModlePath.GetLength() <= 0)
		return false;
	CFile file;
	if (!file.Open(m_ModlePath, CFile::modeCreate | CFile::modeReadWrite))
		return false;
	file.SetLength(0);	//clear the content.

	Stringoper oper;
	string str = "## ��������������Ŀ: ##\r\n";
	file.Write(str.c_str() , str.size());
	str = "3\r\n";
	file.Write(str.c_str() , str.size());
	str = "## ���������������Ŀ: ##\r\n";
	file.Write(str.c_str(),str.size());
	str = (oper.inttostring(m_HindLayer)) + "\r\n";
	file.Write(str.c_str() , str.size());
	str = "## ��������������Ŀ: ##\r\n";
	file.Write(str.c_str() , str.size());
	str = "1\r\n";
	file.Write(str.c_str() , str.size());
	str = "## ѵ�����������õ��������㷨: ##\r\n";
	file.Write(str.c_str() , str.size());
	str = (oper.inttostring(m_nSelArth)) + "\r\n";
	file.Write(str.c_str() , str.size());
	str = "## ������ʹ�õĺ���������ֵ: ##\r\n";
	file.Write(str.c_str() , str.size());
	str = "2\r\n";
	file.Write(str.c_str() , str.size());

	file.Close();
	return true;
}

void CClasssifyWnd::ProcessRetrain()
{
	m_CurState = STATE_STOP;
	m_ButtonOK.SetWindowTextW(L"��ֹ");
	if (MianEvent == NULL)
	{
		MianEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		SetEvent(MianEvent);
		m_hTrainEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	StartThread(TrainThread, m_hThread);
}

void CClasssifyWnd::ProcessNone()
{
	m_CurState = STATE_START;
	m_ButtonOK.SetWindowTextW(L"��ͣ");
	//�����¼��������߳�
	if (MianEvent == NULL)
	{
		MianEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		SetEvent(MianEvent);
		m_hTrainEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	if (!m_bIsOpen)
	{
		m_bIsOpen = m_nFile.Open(m_DataSavePath[0], CFile::modeCreate | CFile::modeReadWrite) == TRUE ? true : false;
		m_strState = L"���ڲɼ���ҧ����...";
		m_nFileCount = 1;
		m_CurTrainTime = m_nTraintime.at(0);
		Invalidate(TRUE);
	}
	StartThread(TrainThread, m_hThread);
}

void CClasssifyWnd::ProcessStart()
{
	if (m_nProgress < 100)							//��ǰ����δ���
	{	
		m_CurState = STATE_PAUSE;					//����״̬Ϊ��ͣ
		m_ButtonOK.SetWindowTextW(L"��ʼ");
		return;
	}
	//����ǰ�����Ѿ���ɣ�
	if (!m_bIsOpen)
	{
		m_bIsOpen = m_nFile.Open(m_DataSavePath[m_nFileCount], CFile::modeCreate | CFile::modeReadWrite) == TRUE ? true : false;
		if (m_nFileCount == 1)
			m_strState = L"���ڲɼ���ҧ����...";
		if (m_nFileCount == 2)
			m_strState = L"���ڲɼ�ͬʱҧ����...";
		m_CurTrainTime = m_nTraintime.at(m_nFileCount);

		m_nFileCount++;
		m_nProgress = 0;
		m_ButtonOK.SetWindowTextW(L"��ʼ");
		m_CurState = STATE_PAUSE;						//׼��������һ״̬
		Invalidate(TRUE);
	}
}

void CClasssifyWnd::ProcessPause()
{
	m_CurState = STATE_START;					//�޸�״̬
	m_ButtonOK.SetWindowTextW(L"��ͣ");
}

void CClasssifyWnd::ProcessDone()
{
	UPDATACONTROLTOVAL;
	if (!SaveResult())
	{
		m_pOutPut(L"Classify Train ���޷�����ѵ�����");
	}
	GetParent()->SendMessage(MYWM_TRAINWMD, TRAINWND_CLASSIFYDONE);
	CFileOper oper;
	oper.DeleteDirectory(GetDirectory() + L"\\tmpModel.txt");
	OnOK();
}

void CClasssifyWnd::ProcessStop()
{
	//end the Thread.
	if (m_pNetWorker)
	{
		m_pNetWorker->StopTrain(true);					//ֹͣѵ��
	}
	m_pOutPut(L"��ֹ�������ѵ����");					//�������һ��״̬
	m_ButtonOK.SetWindowTextW(L"���");
	m_CurState = STATE_DONE;							//����һ��״̬���б���
	m_ButtonTest.EnableWindow(TRUE);					//�ɽ��в���
}

void CClasssifyWnd::ProcessResult(int res)
{
	if (res < 5)
		m_nResult = -1;
	else if (res < 9)
		m_nResult = 1;
	else if (res < 13)
		m_nResult = 0;
	UPDATAVALTOCONTROL;
}

CString CClasssifyWnd::GetDirectory()
{
	CString path;
	if (m_DataSavePath[0].GetLength() > 0)
	{
		path = m_DataSavePath[0].Left(m_DataSavePath[0].ReverseFind('\\') );
	}
	else if (m_DataSavePath[1].GetLength() > 0)
	{
		path = m_DataSavePath[1].Left(m_DataSavePath[1].ReverseFind('\\') );
	}
	else if (m_DataSavePath[2].GetLength() > 0)
	{
		path = m_DataSavePath[2].Left(m_DataSavePath[2].ReverseFind('\\') );
	}
	else
		path = L"";
	return path;
}

LRESULT CClasssifyWnd::OnParam(WPARAM wparam, LPARAM lparam)
{
	AfxMessageBox(L"add code");
	return 0;
}

LRESULT CClasssifyWnd::OnJobMsg(WPARAM wparam, LPARAM lparam)
{
	if (JOB_ONEDONE==wparam)
		ProcessStart();
	if (JOB_ALLDONE == wparam)
	{
		m_ButtonOK.SetWindowTextW(L"��ֹ");
		m_CurState = STATE_STOP;
	}
	if (JOB_PROCESSDONE == wparam)
	{
		m_pOutPut(L"������ѵ����ɣ�");
		m_CurState = STATE_DONE;
		m_ButtonOK.SetWindowTextW(L"���");
		m_ButtonTest.EnableWindow(TRUE);
	}
	return 0;
}
//extern function���ú��������޸�
extern "C"  _declspec(dllexport) char* GetTrainModualMsg()					//��ȡģ����Ϣ
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return "ѵ��ҧ����ֵ���Լ��ҧ��";
}

extern "C" _declspec(dllexport)CClasssifyWnd*  CreateTrainClass()			//��ȡģ���࣬���ڼ���ģ��
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return new CClasssifyWnd();
}

extern "C" _declspec(dllexport)BOOL  CreateTrainDlg(CDialogEx* pchild, CWnd* pParent)	//�������ڴ�������
{
	HINSTANCE save_hInstance = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);

	//AFX_MANAGE_STATE(AfxGetAppModuleState());
	if (pchild == NULL || pParent == NULL)
		return FALSE;
	BOOL b = pchild->Create(IDD_CLASSSIFYWND, pParent);
	if (!b)
		return FALSE;
	pchild->ShowWindow(SW_SHOW);
	AfxSetResourceHandle(save_hInstance);
	return TRUE;
}

extern "C" _declspec(dllexport) CClasssifyWnd* CreateTrainWnd(CWnd* parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());			//for MFC dll stander
	CClasssifyWnd* pdlg = new CClasssifyWnd(parent);

	BOOL b = pdlg->Create(IDD_CLASSSIFYWND, parent);
	if (!b)
	{
		delete pdlg;
		return NULL;
	}
	pdlg->ShowWindow(SW_SHOW);
	return pdlg;
}

//�����ز˵�
extern "C" _declspec(dllexport) BOOL InstallExtMenu(CWnd* pChild, HWND Parent, UINT* NextMenuID)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	BOOL bReturn = FALSE;
	if (!pChild)					//dll �е�wndΪ�գ��޷����
		return FALSE;
	if (!Parent)
		return FALSE;
	HMENU hMenu = ::GetMenu(Parent);		//��ȡ�����ڵĲ˵�
	if (!hMenu)
		return FALSE;
	CMenu ParentMenu;
	ParentMenu.Attach(hMenu);
	CMenu Menu;
	if (!Menu.CreateMenu())						//�����˵�
		return FALSE;

	theApp.m_CommandToMessage.InitHashTable(1);	//������
	NextMenuID = new UINT[1];
	NextMenuID[0] = theApp.ID_MENU_PARAM;
	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"��������");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_PARAM);	//����ӳ��

	HMENU hsubMenu = Menu.Detach();
	ParentMenu.InsertMenuW(ParentMenu.GetMenuItemCount(), MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)hsubMenu,
		_T("����ѵ��ģ������"));

	ParentMenu.Detach();
	CWnd::FromHandle(Parent)->DrawMenuBar();		//ǿ���ػ�˵�
	return TRUE;
}


extern"C" _declspec(dllexport)BOOL CheckExtMenuItem(UINT id)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	if (id == theApp.ID_MENU_PARAM)				//�ж��Ƿ��ǲ˵���Ϣ
		return TRUE;
	else
		return FALSE;
}




