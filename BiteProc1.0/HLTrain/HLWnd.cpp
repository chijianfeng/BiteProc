// HLWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HLTrain.h"
#include "HLWnd.h"
#include "FiltFilt.h"
#include "afxdialogex.h"
#include "Structdefine.h"
#include "Stringoper.h"
#include "ThreadUtil.h"
#include "FileOper.h"
#include "DataProcess.h"
using namespace DATAPROCESS;

extern CHLTrainApp theApp;
extern HANDLE MianEvent;						//���ڴ������̺߳����߳�
const int TIMER_FOR_TRAIN = 0;
const string HL_key[] = 
{
	"L_Vol", "L_Std", "L_Light_Vol", "L_Heavy_Vol", 
	"R_Vol", "R_Std", "R_Light_Vol", "R_Heavy_Vol",
	"LR_Vol", "LR_Std", "LR_Light_Vol", "LR_Heavy_Vol",
};
const double a[] = { 1.0000, -1.1689, -0.8881, 0.7217, 0.7770, -0.2404, -0.1978 };
const double b[] = { 0.4459, 0, -1.3377, 0, 1.3377, 0, -0.4459 };

// CHLWnd �Ի���

IMPLEMENT_DYNAMIC(CHLWnd, CDialogEx)

CHLWnd::CHLWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHLWnd::IDD, pParent)
	, m_strState(_T(""))
	, m_strInstruct(_T(""))
{
	m_nHLWeight[0] = 10;
	m_nHLWeight[1] = 10;
	m_nHLWeight[2] = 10;
}

CHLWnd::~CHLWnd()
{
}

void CHLWnd::SetParm(CAlgParamConfig &parm)
{
    m_IsTrain = parm.GetHLState();
	m_ReTrain = parm.GetHLReTrain();
	m_WinLen = parm.GetWindowLength();
	m_WinOverLen = parm.GetOverLength();
	parm.GetHLTrainTime(m_TrainTime);
	m_DataSavePath[0] = parm.GetLLData();
	m_DataSavePath[1] = parm.GetLHData();
	m_DataSavePath[2] = parm.GetRLData();
	m_DataSavePath[3] = parm.GetRHData();
	m_DataSavePath[4] = parm.GetBLData();
	m_DataSavePath[5] = parm.GetBHData();
	m_ModelPath = parm.GetHLModel();
	m_ModulePath = parm.GetHLModule();
	m_BiteThreshold = LoadThrold(parm.GetBiteThroldModel());
}

void CHLWnd::SetData(ManagedData &d)
{
	if (m_CurState != STATE_START)						//�ж��Ƿ��ǿ�ʼѵ����
		return;
	m_BufData.AppendData(d);							//�������ȷ�����ʱ������
	m_nSampleRate = m_BufData.GetSampleRate();
	m_nChannum = m_BufData.GetChannum();
	SetEvent(m_hTrainEvent);							//֪ͨ���̣߳����Լ�����������
}

bool CHLWnd::SaveData()
{
	if (!m_bIsOpen)
		return false;
	if (m_nFile.GetLength() <= 0)						//�ļ��ոտ�ʼд�룬����ͷ�ļ���Ϣ��samplerate and channal number
	{
		string str;
		Stringoper oper;
		str = "Samplerate = " + oper.inttostring(m_BufData.GetSampleRate()) + "\r\n";
		m_nFile.Write(str.c_str(), str.size());
		str = "Channum = " + oper.inttostring(m_BufData.GetChannum()) + "\r\n";
		m_nFile.Write(str.c_str(), str.size());
		m_nFile.Flush();
	}
	m_nFile.Write(m_BufData.GetData(), m_BufData.GetDataSize()*sizeof(double));
	m_BufData.Remove(m_BufData.GetDataSize() + 1);		//��ջ���������
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

		if (m_nFileCount >= 6)									//6�����趼�Ѿ�ѵ�����
		{
			m_bExit = TRUE;
			SendMessage(HL_ONEJOBDONE, JOB_ALLDONE, 0);		//�����������
		}														//exit the thread.
		else
			SendMessage(HL_ONEJOBDONE, JOB_ONEDONE, 0);				//����֪ͨ���һ������
	}
	this->SendMessage(WM_PAINT);								//update the state

	return true;
}

bool CHLWnd::LoadData()
{
	if (m_bIsOpen)								//ʵ�����ʱ����,�ر��ļ�
	{
		m_nFile.Close();
		m_bIsOpen = false;
	}

	for (size_t i = 0; i < 6; i++)
	{
		m_nProgress = 0;
		if (i == 0)
			m_strState = L"��������ҧ����...";
		else if (i == 1)
			m_strState = L"��������ҧ����...";
		else if (i == 2)
			m_strState = L"��������ҧ����...";
		else if (i == 3) 
			m_strState = L"��������ҧ����...";
		else if (i == 4)
			m_strState = L"����ͬʱ��ҧ����...";
		else if (i == 5)
			m_strState = L"����ͬʱ��ҧ����...";
		//SendMessage(WM_PAINT);
		Invalidate(TRUE);
		MyLoadData(m_DataSavePath[i], m_TrainDataBuf[i]);
		Sleep(500);
	}
	return true;
}

void CHLWnd::MyLoadData(CString path, ManagedData& buf)
{
	CFile file;
	bool isopen = file.Open(path, CFile::modeRead) == TRUE ? true : false;

	if (!isopen)
	{
		m_pOutPut(L"�޷����ļ�:" + path);
		return;
	}
	file.SeekToBegin();
	ULONGLONG length = file.GetLength() / sizeof(double);
	buf.ResizeManageData((int)length + 1);

	/*���ȶ�ȡ�ļ�ͷ*/
	TRY
	{
		Stringoper oper;
		char strbuf[512];
		UINT ret = file.Read(strbuf, 512);
		string str = oper.charstostring(strbuf);
		str = str.substr(0, str.find('\n') + 1);
		string key, val;
		oper.AnalyseLine(str, key, val, '%');
		buf.SetSampleRate(oper.stringtoint(val));

		file.Seek(str.size(), CFile::begin);			//���ö�ȡλ��
		int	pos = str.size();

		ret = file.Read(strbuf, 512);
		str = oper.charstostring(strbuf);
		str = str.substr(0, str.find('\n') + 1);
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

bool  CHLWnd::MeanSample(BpNetdata* bp, int len, Throld& t)
{
	if (bp == NULL || len <= 0)
		return false;
	t.stdvalue = 0;
	double l = 0;
	double r = 0;
	for (int i = 0; i<len; i++)
	{
		l += bp[i].Lvalue;
		r += bp[i].Rvalue;
	}
	l = l / len;
	r = r / len;
	t.voltvalue = max(l, r);
	return true;
}

int	 CHLWnd::ExtracSample(ManagedData &rawd, ManagedData& newd, BpNetdata* pb, Processdata *prod)
{
	if (pb == NULL || rawd.GetDataSize() != newd.GetDataSize())
		return 0;
	ManagedData rl, rr, nl, nr;
	rawd.GetAnydata(rl, 0);
	rawd.GetAnydata(rr, 1);
	newd.GetAnydata(nl, 0);
	newd.GetAnydata(nr, 1);
	return ExtracSample(rl.GetData(), rr.GetData(), nl.GetData(), nr.GetData(), m_BiteThreshold, rl.GetDataSize(), pb, prod);
}

int CHLWnd::ExtracSample(double *Lrawd, double *Rrawd, double *Lfiltd, double *Rfiltd, Throld throld, int len, BpNetdata *pb, Processdata *prod)
{
	int winlen = m_WinLen;
	double *datasetL = new double[len];
	double *datasetR = new double[len];
	double *newdataL = new double[len];
	double *newdataR = new double[len];
	Processdata psd;
	int Sampleblock = 0;
	for (int i = 0; i<len; i++)
	{
		if (i + winlen + 1 >= len)
		{
			psd = processdata(Lrawd + i, Rrawd + i, Lfiltd + i, Rfiltd + i, len - i - 1, throld);
		}
		else
			psd = processdata(Lrawd + i, Rrawd + i, Lfiltd + i, Rfiltd + i, winlen + 1, throld);
		datasetL[i] = psd.lmaxmin;
		datasetR[i] = psd.rmaxmin;
		newdataL[i] = psd.stdlval;
		newdataR[i] = psd.stdrval;
	}

	int smoothlen = 0;
	smooth(smoothlen, &newdataL, len);
	smooth(smoothlen, &newdataR, len);
	int distance = 0;
	for (int i = 0; i<len; i++)
	{
		if (newdataL[i]>0 || newdataR[i]>0)
			distance++;
		else if (distance>0)
		{
			pb[Sampleblock] = extractnetdata(datasetL + i - distance + 1, datasetR + i - distance + 1, distance, throld);
			pb[Sampleblock].sp = i - distance+1;
			pb[Sampleblock].ep = i-1;
			prod[Sampleblock].lmaxmin = prod[Sampleblock].rmaxmin = 0;
			prod[Sampleblock].stdlval = mean(newdataL + i - distance + 1, distance);
			prod[Sampleblock].stdrval = mean(datasetR + i - distance + 1, distance);
			Sampleblock++;
			distance = 0;
		}
	}
	delete newdataR;
	delete newdataL;
	delete datasetL;
	delete datasetR;
	return Sampleblock;
}

void CHLWnd::ProcessData(ManagedData& rawd, ManagedData& newd)		//filt the data.
{
	FiltFilt	ff;
	ff.SetParam(a, b, 7);
	double**	pd = NULL;
	pd = new double*[2];
	ManagedData dl, dr;

	pd[0] = new double[rawd.GetDataSize() / 2];
	rawd.GetAnydata(dl, 0);
	ff.useFFilt(dl.GetData(), pd[0], dl.GetDataSize());

	pd[1] = new double[rawd.GetDataSize() / 2];
	rawd.GetAnydata(dr, 1);
	ff.useFFilt(dr.GetData(), pd[1], dr.GetDataSize());

	newd.ResizeManageData(rawd.GetDataSize());
	int length = rawd.GetDataSize() / 2;
	double *p = new double[rawd.GetDataSize()];
	for (int i = 0; i<rawd.GetDataSize(); i++)
	{
		if (i % 2 == 0)
			p[i] = pd[0][i / 2];
		else
			p[i] = pd[1][(i - 1) / 2];
	}
	newd.SetData(p, rawd.GetDataSize());
	newd.SetSampleRate(rawd.GetSampleRate());
	newd.SetChannum(rawd.GetChannum());
	for (int i = 0; i<m_BufData.GetChannum(); i++)
	{
		delete pd[i];
	}
	delete pd;
	delete p;
}

Throld CHLWnd::LoadThrold(CString path)
{
	Throld throld = { 0.0, 0.0 };
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

bool CHLWnd::DoProcess()
{
	Stringoper oper;
	char path[MAX_PATH];
	oper.CStringtoChars(m_ModelPath,path);
	m_strState = L"���ڴ�������...";
	Invalidate(TRUE);

	for (int i = 0; i < 3; i++)
	{
		m_DisHL[i] = new CDistinguishHL();
	}

	//��ʼ��������
	BpNetdata **pNet = new BpNetdata *[3];
	int Samplenum[3];
	for (int i = 0; i< 3; i++)
	{
		ManagedData newdL, newdH;
		
		ProcessData(m_TrainDataBuf[i*2], newdL);
		ProcessData(m_TrainDataBuf[i*2+1], newdH);

		BpNetdata* pL = new BpNetdata[1000];
		BpNetdata* pH = new BpNetdata[1000];
		Processdata prodL[1000];
		Processdata prodR[1000];
		int numL = ExtracSample(m_TrainDataBuf[i*2], newdL, pL, prodL);
		int numH = ExtracSample(m_TrainDataBuf[i*2+1], newdH, pH, prodR);

		Throld L, H;
		MeanSample(pL, numL, L);
		MeanSample(pH, numH, H);
		//Processdata pdL = MeanProcessData(prodL, numL);
		//Processdata pdH = MeanProcessData(prodR, numH);
		
		double Ratio;
		switch (i)
		{
		case 0:
			Ratio = m_nHLWeight[0];
			break;
		case 1:
			Ratio = m_nHLWeight[1];
			break;
		case 2:
			Ratio = m_nHLWeight[2];
			break;
		default:
			break;
		}
		m_nResult[HL_key[i * 4]] = max(H.voltvalue,L.voltvalue)*Ratio;
		m_nResult[HL_key[i * 4 + 1]] = (L.stdvalue + H.stdvalue) / 2;
		m_nResult[HL_key[i * 4 + 2]] = L.voltvalue;						//min value;
		m_nResult[HL_key[i * 4 + 3]] = H.voltvalue;						//max value;

		//pNet[i] = new BpNetdata[numH + numL];
		//Samplenum[i] = numL + numH;
		//MergeBpData(pL, numL, pH, numH, pNet[i]);
		delete pL;
		delete pH;
	}

	for (int i = 0; i < 3; i++)
	{
		delete pNet[i];
	}
	delete pNet;
	
	m_strState = L"����ѵ��ģ��";
	m_bExit = TRUE;
	KillTimer(TIMER_FOR_TRAIN);
	SendMessage(HL_ONEJOBDONE, JOB_PROCESSDONE);
	return true;
}

bool CHLWnd::StartThread(DWORD(WINAPI lp)(LPVOID), HANDLE& handle)
{
	if (NULL == lp)
		return false;
	m_bExit = false;
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

bool CHLWnd::EndThread(HANDLE& handle)
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

bool CHLWnd::SaveResult()
{
	Stringoper oper;
	CStdioFile file;
	char path[MAX_PATH];
	
	oper.CStringtoChars(m_ModelPath, path);
	if (m_DisHL[0])
	{
		m_DisHL[0]->GetResult(m_nResult, 0, m_BiteThreshold);
		m_DisHL[0]->SaveTrainModel(path);
	}
	if (m_DisHL[1])
	{
		m_DisHL[1]->GetResult(m_nResult, 1, m_BiteThreshold);
		m_DisHL[1]->SaveTrainModel(path);
	}
	if (m_DisHL[2])
	{
		m_DisHL[2]->GetResult(m_nResult, 2, m_BiteThreshold);
		m_DisHL[2]->SaveTrainModel(path);
	}
	return true;
}

bool CHLWnd::LoadResult()
{
	Stringoper oper;
	char path[MAX_PATH];
	oper.CStringtoChars(m_ModelPath, path);
	//�������ģ��
	if (!m_pAlogrithm[0])
	{
		m_DisHL[0] = new CDistinguishHL();
		m_DisHL[0]->GetResult(m_nResult, 0, m_BiteThreshold);
		bool b = m_DisHL[0]->LoadModel(path);
		if (b)
		{
			m_pAlogrithm[0] = m_DisHL[0];
		}
		else
		{
			m_pAlogrithm[0] = NULL;
			delete m_pAlogrithm[0];
		}
	}
	else
	{
		m_DisHL[0]->GetResult(m_nResult, 0, m_BiteThreshold);
		m_pAlogrithm[0] = m_DisHL[0];
		m_pAlogrithm[0]->LoadModel(path);
	}
	if (!m_pAlogrithm[1])
	{
		m_DisHL[1] = new CDistinguishHL();
		m_DisHL[1]->GetResult(m_nResult, 1, m_BiteThreshold);
		bool b = m_DisHL[1]->LoadModel(path);
		if (b)
		{
			m_pAlogrithm[1] = m_DisHL[1];
		}
		else
		{
			m_pAlogrithm[1] = NULL;
			delete m_pAlogrithm[1];
		}
	}
	else
	{
		m_DisHL[1]->GetResult(m_nResult, 1, m_BiteThreshold);
		m_pAlogrithm[1] = m_DisHL[1];
		m_pAlogrithm[1]->LoadModel(path);
	}
	if (!m_pAlogrithm[2])
	{
		m_DisHL[2] = new CDistinguishHL();
		m_DisHL[2]->GetResult(m_nResult, 2,m_BiteThreshold);
		bool b = m_DisHL[2]->LoadModel(path);
		if (b)
		{
			m_pAlogrithm[2] = m_DisHL[2];
		}
		else
		{
			m_pAlogrithm[2] = NULL;
			delete m_pAlogrithm[2];
		}
	}
	else
	{
		m_DisHL[2]->GetResult(m_nResult, 2, m_BiteThreshold);
		m_pAlogrithm[2] = m_DisHL[2];
		m_pAlogrithm[2]->LoadModel(path);
	}
	return true;
}

bool CHLWnd::ChangeState(CAlgParamConfig& parm, bool s)
{
	parm.SetBiteState(s);
	return true;
}

void CHLWnd::SetLogOut(void(*pFunc)(CString))
{
	m_pOutPut = pFunc;
}

void   CHLWnd::ProcessNone()
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
		m_strState = L"���ڲɼ�����ҧ����...";
		m_nFileCount = 1;
		m_CurTrainTime = m_TrainTime.at(0);
		Invalidate(TRUE);
	}
	StartThread(TrainThread, m_hThread);
}

void	CHLWnd::ProcessStart()
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
			m_strState = L"���ڲɼ�����ҧ����...";
		if (m_nFileCount == 2)
			m_strState = L"���ڲɼ�����ҧ����...";
		if (m_nFileCount == 3)
			m_strState = L"���ڲɼ�����ҧ����...";
		if (m_nFileCount == 4)
			m_strState = L"���ڲɼ�ͬʱ��ҧ����...";
		if (m_nFileCount == 5)
			m_strState = L"���ڲɼ�ͬʱ��ҧ����...";
		m_CurTrainTime = m_TrainTime.at(m_nFileCount);

		m_nFileCount++;
		m_nProgress = 0;
		m_ButtonOK.SetWindowTextW(L"��ʼ");
		m_CurState = STATE_PAUSE;						//׼��������һ״̬
		Invalidate(TRUE);
	}
}

void	CHLWnd::ProcessPause()
{
	m_CurState = STATE_START;					//�޸�״̬
	m_ButtonOK.SetWindowTextW(L"��ͣ");
}

void	CHLWnd::ProcessDone()
{
	UPDATACONTROLTOVAL;
	if (!SaveResult())
	{
		m_pOutPut(L"HeavyLight Train ���޷�����ѵ�����");
	}
	GetParent()->SendMessage(MYWM_TRAINWMD, TRAINWND_HLDONE);
	OnOK();
}

void	CHLWnd::ProcessStop()
{
	m_CurState = STATE_STOP;
	m_ButtonOK.SetWindowTextW(L"��ֹ");
	m_ButtonTest.EnableWindow(TRUE);					//�ɽ��в���
}

void CHLWnd::ProcessRetrain()
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

void CHLWnd::ProcessResult(int res)
{
	if (res == 1 || res == 5 || res == 9)
		m_testResult = 1;    //OneBite(Light)
	else if (res == 2 || res == 6 || res == 10)
		m_testResult = 2;    //OneBite(Heavy)
	else
		m_testResult = 0;    //No_OneBite (TWOBITE OR LONGBITE)
}

void CHLWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SSTATE, m_strState);
	DDX_Text(pDX, IDC_EDITINSTRUCT, m_strInstruct);
	DDX_Control(pDX, IDC_PROGRESSBITETRAIN, m_ProgressState);
	DDX_Control(pDX, ID_OK, m_ButtonOK);
	DDX_Control(pDX, ID_TEST, m_ButtonTest);
	DDX_Control(pDX, IDC_SLIDERLV, m_SliderLVol);
	DDX_Control(pDX, IDC_SLIDERRV, m_SliderRVol);
	DDX_Control(pDX, IDC_SLIDERLRV, m_SliderLRVol);
	DDX_Text(pDX, IDC_ELVOL, m_nHLWeight[0]);
	DDX_Text(pDX, IDC_ERVOL, m_nHLWeight[1]);
	DDX_Text(pDX, IDC_ELRVOL, m_nHLWeight[2]);
	DDX_Text(pDX, IDC_EDIT_TESTRESULT, m_testResult);
}


BEGIN_MESSAGE_MAP(CHLWnd, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_OK, &CHLWnd::OnBnClickedOk)
	ON_BN_CLICKED(ID_TEST, &CHLWnd::OnBnClickedTest)
	ON_BN_CLICKED(ID_CANCEL, &CHLWnd::OnBnClickedCancel)
	ON_MESSAGE(MYWM_THREADMSG, OnReceivMsg)
	ON_WM_TIMER()
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_PARAM, OnParam)
	ON_MESSAGE(HL_ONEJOBDONE, OnJobMsg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERLV, &CHLWnd::OnNMCustomdrawSliderLVol)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERLV, &CHLWnd::OnNMCustomdrawSliderLVol)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERRV, &CHLWnd::OnNMCustomdrawSliderRVol)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERRV, &CHLWnd::OnNMCustomdrawSliderRVol)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERLRV, &CHLWnd::OnNMCustomdrawSliderLRVol)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERLRV, &CHLWnd::OnNMCustomdrawSliderLRVol)
END_MESSAGE_MAP()


// CHLWnd ��Ϣ�������
BOOL CHLWnd::OnInitDialog()
{

	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_CurState = STATE_NONE;				//��ʼ״̬Ϊ��
	m_nFileCount = 0;
	m_strState = "ѵ������ģ��...";
	m_ButtonOK.SetWindowText(L"��ʼ");
	m_ButtonTest.EnableWindow(FALSE);

	m_SliderLVol.SetRange(0,20);
	m_SliderRVol.SetRange(0,20);
	m_SliderLRVol.SetRange(0,20);
	m_SliderLVol.SetPos(m_nHLWeight[0]);
	m_SliderRVol.SetPos(m_nHLWeight[1]);
	m_SliderLRVol.SetPos(m_nHLWeight[2]);
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

	return TRUE;
}

void CHLWnd::OnBnClickedOk()
{
	switch (m_CurState)
	{
	case CHLWnd::STATE_NONE:
		ProcessNone();
		break;
	case CHLWnd::STATE_START:
		ProcessStart();
		break;
	case CHLWnd::STATE_PAUSE:
		ProcessPause();
		break;
	case CHLWnd::STATE_DONE:
		ProcessDone();
		break;
	case CHLWnd::STATE_RETRAIN:
		ProcessRetrain();
		break;
	case  CHLWnd::STATE_STOP:
		ProcessStop();
		break;
	default:
		break;
	}
}

void CHLWnd::OnBnClickedTest()
{
	CWnd* pParent = this->GetParent();
	if (!pParent)
	{
		m_pOutPut(L"�޷�֪ͨ��ģ�飺�޷���ȡ��ģ����");
	}
	CFileOper foper;
	if (!foper.FileExist(m_ModelPath))
	{
		m_pOutPut(L"�޷����в��ԣ��޷��ҵ�����ģ��");
		return;
	}
	Stringoper oper;
	char path[MAX_PATH];
	oper.CStringtoChars(m_ModelPath,path);
	m_DisHL[0]->LoadModel(path);
	m_DisHL[1]->LoadModel(path);
	m_DisHL[2]->LoadModel(path);
	m_pOutPut(L"������ֵ��ʼ����");
	pParent->SendMessage(MYWM_TRAINWMD, TRAINWND_TEST, 0);				//��ʾ��ʼ����
}

void CHLWnd::OnBnClickedCancel()
{
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
	GetParent()->SendMessage(MYWM_TRAINWMD, TRAINWND_HLCANCLE);
	OnCancel();
}

void CHLWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_strInstruct.Format(L"�����ҧʱ����%d��\r\n�����ҧʱ����%d��\r\n�ұ���ҧʱ����%d��\r\n�ұ���ҧʱ����%d��\r\nͬʱ��ҧʱ����%d��\r\nͬʱ��ҧʱ����%d��\r\n",
		m_TrainTime.at(0), m_TrainTime.at(1), m_TrainTime.at(2),m_TrainTime.at(3),m_TrainTime.at(4),m_TrainTime.at(5));
	UPDATAVALTOCONTROL;
	m_ProgressState.SetPos(m_nProgress);
}

void CHLWnd::OnDestroy()
{
	__super::OnDestroy();
}

void CHLWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_FOR_TRAIN)
	{
		if (!m_DisHL)
		{
			SendMessage(WM_PAINT);
		}
	}
	__super::OnTimer(nIDEvent);
}

CString CHLWnd::GetDirectroy()
{
	CString path;
	if (m_DataSavePath[0].GetLength() > 0)
	{
		path = m_DataSavePath[0].Left(m_DataSavePath[0].ReverseFind('\\'));
	}
	else if (m_DataSavePath[1].GetLength() > 0)
	{
		path = m_DataSavePath[1].Left(m_DataSavePath[1].ReverseFind('\\'));
	}
	else if (m_DataSavePath[2].GetLength() > 0)
	{
		path = m_DataSavePath[2].Left(m_DataSavePath[2].ReverseFind('\\'));
	}
	else if (m_DataSavePath[3].GetLength() > 0)
	{
		path = m_DataSavePath[3].Left(m_DataSavePath[3].ReverseFind('\\'));
	}
	else if (m_DataSavePath[4].GetLength() > 0)
	{
		path = m_DataSavePath[4].Left(m_DataSavePath[4].ReverseFind('\\'));
	}
	else if (m_DataSavePath[5].GetLength() > 0)
	{
		path = m_DataSavePath[5].Left(m_DataSavePath[5].ReverseFind('\\'));
	}
	else
		path = L"";
	return path;
}

HBRUSH CHLWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC && (pWnd->GetDlgCtrlID() != IDC_SLIDERLV &&pWnd->GetDlgCtrlID() != IDC_SLIDERRV && pWnd->GetDlgCtrlID() != IDC_SLIDERLRV))
	{
		pDC->SetTextColor(RGB(0, 0, 0));				//���ó��㱳������ɫ   
		pDC->SetBkMode(TRANSPARENT);						//͸��   
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}

BOOL CHLWnd::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	COLORREF bckcolor = RGB(255, 255, 255);				//white.
	pDC->FillSolidRect(rect, bckcolor);
	return TRUE;
}

LRESULT CHLWnd::OnParam(WPARAM wparam, LPARAM lparam)
{
	AfxMessageBox(L"add code");
	return 0;
}

LRESULT CHLWnd::OnJobMsg(WPARAM wparam, LPARAM lparam)
{
	if (JOB_ONEDONE == wparam)
		ProcessStart();
	if (JOB_ALLDONE == wparam)
	{
		m_ButtonOK.SetWindowTextW(L"��ֹ");
		m_CurState = STATE_STOP;
	}
	if (JOB_PROCESSDONE == wparam)
	{
		m_pOutPut(L"������ֵѵ�����!");
		m_CurState = STATE_DONE;
		m_ButtonOK.SetWindowTextW(L"���");
		m_ButtonTest.EnableWindow(TRUE);
	}
	return 0;
}

LRESULT CHLWnd::OnReceivMsg(WPARAM wparam, LPARAM lparam)
{
	Stringoper oper;
	switch (wparam)
	{
	case THREADMSG_PROCESSFINISH:
		m_CurState = STATE_DONE;
		m_ButtonOK.SetWindowTextW(L"���");
		if (m_pOutPut)
			(*m_pOutPut)(L"����ѵ��ģ�飺ģ��ѵ����ɣ�");
		m_pAlogrithm[0] = m_DisHL[0];							//ģ�Ϳ��á���
		m_pAlogrithm[1] = m_DisHL[1];
		m_pAlogrithm[2] = m_DisHL[2];
		break;
	case THREADMSG_LOADFINISH:
		if (m_pOutPut)
			(*m_pOutPut)(L"����ѵ��ģ�飺����������ɣ�");
		break;
	case THREADMSG_SAVEFINISH:
		if (m_pOutPut)
			(*m_pOutPut)(L"����ѵ��ģ�飺����������ɣ�");
		break;
	case HL_CREATEDONE:
		if (m_pOutPut)
			(*m_pOutPut)(L"��������ģ������ļ���ɣ�");
		break;
	case HL_CREATEFAILED:
		if (m_pOutPut)
			(*m_pOutPut)(L"��������ģ������ļ�ʧ�ܣ�");
		break;
	case HL_MODELCANCEL:
		if (m_pOutPut)
			(*m_pOutPut)(L"��������ģ��ʧ�ܣ�");
		break;
	case HL_MODELOK:
		if (m_pOutPut)
			(*m_pOutPut)(L"��������ģ�ͳɹ���");
		break;
	case TRAINWND_PROCESSRESULT:
		ProcessResult(int(lparam));
		break;
	default:
		break;
	}
	return 0;
}

void CHLWnd::OnNMCustomdrawSliderLVol(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int pos = m_SliderLVol.GetPos();
	m_nHLWeight[0] = pos / 10.0;
	UPDATAVALTOCONTROL;
	*pResult = 0;
}

void CHLWnd::OnNMReleasedcaptureSliderLVol(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CHLWnd::OnNMCustomdrawSliderRVol(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int pos = m_SliderRVol.GetPos();
	m_nHLWeight[1] = pos / 10.0;
	UPDATAVALTOCONTROL;
	*pResult = 0;
}

void CHLWnd::OnNMReleasedcaptureSliderRVol(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CHLWnd::OnNMCustomdrawSliderLRVol(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int pos = m_SliderLRVol.GetPos();
	m_nHLWeight[2] = pos / 10.0;
	UPDATAVALTOCONTROL;
	*pResult = 0;
}

void CHLWnd::OnNMReleasedcaptureSliderLRVol(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

//extern function���ú��������޸�
extern "C" _declspec(dllexport) char* GetTrainModualMsg()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return "ѵ����ҧ����ҧ������ҧ��������ֵ";
}

extern "C" _declspec(dllexport) CHLWnd*  CreateTrainClass()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return new CHLWnd();
}

extern "C" _declspec(dllexport)BOOL CreateTrainDlg(CDialogEx* pChild,CWnd* pParent)
{
	HINSTANCE save_hInstance = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);
	if (pChild == NULL || pParent == NULL)
		return FALSE;
	BOOL b = pChild->Create(IDD_HLWND,pParent);
	if (!b)
		return FALSE;
	pChild->ShowWindow(SW_SHOW);
	AfxSetResourceHandle(save_hInstance);
	return TRUE;
}

extern "C" _declspec(dllexport) CHLWnd* CreateTrainWnd(CWnd* pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CHLWnd* pdlg = new CHLWnd();
	BOOL b = pdlg->Create(IDD_HLWND,pParent);
	if (!b)
	{
		delete pdlg;
		return NULL;
	}
	pdlg->ShowWindow(SW_SHOW);
	return pdlg;
}

// �����ز˵�
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

