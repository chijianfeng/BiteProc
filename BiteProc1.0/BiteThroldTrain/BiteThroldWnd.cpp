// BiteThroldWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BiteThroldWnd.h"
#include "afxdialogex.h"
#include "BiteCfgDlg.h"
#include "FiltFilt.h"
#include "Stringoper.h"
#include "ThreadUtil.h"

// CBiteThroldWnd �Ի���
extern HANDLE MianEvent;						//�����߳�

IMPLEMENT_DYNAMIC(CBiteThroldWnd, CDialogEx)

CBiteThroldWnd::CBiteThroldWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBiteThroldWnd::IDD, pParent)
{

}

CBiteThroldWnd::~CBiteThroldWnd()
{
	if (!m_pAlogrithm[0])
		delete m_pAlogrithm[0];
}

void CBiteThroldWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PGSBITETRAIN, m_proState);
	DDX_Control(pDX, IDC_SLIDBITERADIO, m_ListRatio);
	DDX_Control(pDX, IDOK, m_Bctrl);

	DDX_Text(pDX, IDC_BITESTATE, m_strState);
	DDX_Text(pDX, IDC_EDITBITETHOLDVALUE, m_Ratio);
	DDX_Text(pDX, IDC_EBITEINSTRCTION, m_strInstrcut);
}
//implement the interface
void CBiteThroldWnd::SetParm(CAlgParamConfig& parm)
{
	m_Winlen = parm.GetWindowLength();
	m_bIsTrain = parm.GetBitestate();
	parm.GetBiteTrain(m_nTraintime, m_nTrainCount);
	m_WinOverLen = parm.GetOverLength();
	m_nConfigFile = parm.GetBiteThroldModel();				//�������ģ�͵�·��
	m_strFilePath = parm.GetBiteDataPath();
}

void CBiteThroldWnd::SetData(ManagedData& d)
{
	if (m_CurState == BITESTART||m_CurState ==BITEDONE)
		return;
	m_nBuf.AppendData(d);
	SetEvent(m_hTrainEvent);							//֪ͨ���̣߳����Լ�����������
}

void CBiteThroldWnd::SetLogOut(void(*pFunc)(CString))
{
	m_pOutPut = pFunc;
}

bool CBiteThroldWnd::SaveData()
{
	if (!m_bIsOpen)
		return false;
	m_strState = L"����������...";
	m_nFile.Write(m_nBuf.GetData(), m_nBuf.GetDataSize()*sizeof(double));
	m_nBuf.Remove(m_nBuf.GetDataSize());
	ResetEvent(m_hTrainEvent);
	//change the process.
	ULONGLONG dwlength = m_nFile.GetLength();
	ULONGLONG length = dwlength / sizeof(double);
	ULONGLONG total = m_nBuf.GetSampleRate()*m_nTraintime*m_nBuf.GetChannum();
	m_nProgress = (int)(length * 100 / total);					//���ȼ���

	if (m_nProgress >= 100)
	{
		
		m_bExit = TRUE;										//exit the thread.
		m_nProgress = 100;
	}
	this->SendMessage(WM_PAINT);								//update the state
	
	return true;
}

bool CBiteThroldWnd::LoadData()
{
	if (!m_bIsOpen)
		return false;
	m_nFile.SeekToBegin();

	ULONGLONG dwlength = m_nFile.GetLength() / sizeof(double);
	ULONGLONG length = dwlength;
	m_nBuf.ResizeManageData((int)length + 1);				//���ݵ����ڴ�
	m_strState = _T("��������...");

	SendMessage(WM_PAINT);									//����

	int ret = 0;
	double *pd = new double[1000];							//������ʱ������
	int total = 0;

	while (ret = m_nFile.Read(pd, 1000))
	{
		
		total += ret / sizeof(double);
		m_nProgress = (int)(total * 100 / length);
		m_nBuf.AppendData(pd, ret / sizeof(double));		//���뻺����
		Sleep(10);
		SendMessage(WM_PAINT);
	}

	delete pd;
	m_bExit = TRUE;
	return true;
}

bool CBiteThroldWnd::DoProcess()
{
	if (m_nBuf.GetDataSize() <= 0 || m_nTrainCount <= 0)
		return false;
	FiltFilt ff;
	m_nProgress = 0;
	m_strState = L"���ڴ�������...";

	SendMessage(WM_PAINT);
	ff.SetParam(a_3_100, b_3_100, 7);								//�����˲�����
	double** pd = NULL;
	pd = new double*[m_nBuf.GetChannum()];

	for (int i = 0; i < m_nBuf.GetChannum(); i++)
	{
		ManagedData d;
		pd[i] = new double[m_nBuf.GetDataSize() / m_nBuf.GetChannum()];							//����ÿ��ͨ������
		m_nBuf.GetAnydata(d, i);
		ff.useFFilt(d.GetData(), pd[i], d.GetDataSize());
		//����ÿ��ͨ����ֵ
		Throld throld = calthrold(d.GetData(), pd[i], d.GetDataSize(), m_Winlen, m_nTrainCount, false);		
		m_BiteThreshold = MaxThreshold(m_BiteThreshold, throld);
		m_nProgress = (i + 1) * 100 / m_nBuf.GetChannum();
		m_TmpThreshold = m_BiteThreshold.voltvalue;												//���ڵ���
		Sleep(100);
		SendMessage(WM_PAINT);
	}

	//�ͷ���ʱ�ڴ�ռ�
	for (int i = 0; i < m_nBuf.GetChannum(); i++)
	{
		delete pd[i];
	}
	delete pd;
	m_bExit = TRUE;
	return true;
}

bool CBiteThroldWnd::ChangeState(CAlgParamConfig& parm, bool s)
{
	parm.SetBiteState(s);
	return true;
}

bool CBiteThroldWnd::StartThread(DWORD(WINAPI lp)(LPVOID),HANDLE& handle)
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

bool CBiteThroldWnd::EndThread(HANDLE& handle)
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

bool CBiteThroldWnd::SaveResult()
{
	CString path = m_nConfigFile;
	if (path.GetLength() <= 0)
		return false;
	CStdioFile file;
	if (!file.Open(path, CFile::modeCreate | CFile::modeReadWrite))
		return false;
	Stringoper oper;
	file.WriteString(L"std="+oper.tocstring(oper.doubletostring(m_BiteThreshold.stdvalue) + '\n'));
	file.WriteString(L"vol=" + oper.tocstring(oper.doubletostring(m_BiteThreshold.voltvalue) + '\n'));
	file.Close();
	return true;
}

bool CBiteThroldWnd::LoadResult()
{
	CString path = m_nConfigFile;						//�����ļ�����
	if (path.GetLength() <= 0)
		return false;
	CStdioFile file;
	if (!file.Open(path, CFile::modeRead))
		return false;
	Stringoper oper;
	CString str;
	file.ReadString(str);
	string _key, val;
	char comment = '=';
	oper.AnalyseLine(oper.tostring(str), _key, val, comment);
	m_BiteThreshold.stdvalue = oper.stringtoint(val);

	file.ReadString(str);
	oper.AnalyseLine(oper.tostring(str), _key, val, comment);
	m_BiteThreshold.voltvalue = oper.stringtoint(val);
	file.Close();

	char szPath[MAX_PATH];
	if (!m_pAlogrithm[0])
	{
		m_pBase = new CCalcuteColor(m_Winlen);				//�����㷨ģ��
		oper.CStringtoChars(m_nConfigFile, szPath);
		if (!m_pBase->LoadModel(szPath))					//����
		{
			(*m_pOutPut)(L"�޷�������ֵģ�ͣ��޷�����ҧ����");
			delete m_pBase;
			m_pBase = NULL;
		}
		else
		{
			(*m_pOutPut)(L"������ֵģ����ɣ�");
			m_pAlogrithm[0] = m_pBase;
		}
		m_pAlogrithm[0] = m_pBase;
	}

	return true;
}

BEGIN_MESSAGE_MAP(CBiteThroldWnd, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK, &CBiteThroldWnd::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBiteThroldWnd::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_MESSAGE(MYWM_THREADMSG , OnReceivMsg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDBITERADIO, &CBiteThroldWnd::OnNMCustomdrawSlidbiteradio)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDBITERADIO, &CBiteThroldWnd::OnNMReleasedcaptureSlidbiteradio)
END_MESSAGE_MAP()


// CBiteThroldWnd ��Ϣ�������


BOOL CBiteThroldWnd::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  ��ʼ��
	m_ListRatio.SetRange(0, 20);				//���õ�����Χ
	m_proState.SetRange(0, 100);				//���÷�Χ
	m_proState.SetPos(0);
	m_strState = "��ʼѵ��...";
	/*CBiteCfgDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCfgParma(m_nTraintime, m_nTrainCount);
	}*/
	m_Ratio = 0.0;
	m_strInstrcut.Format(L"����ҧ����ֵѵ����\r\n��%d���ڣ�ҧ��%d��,\r\n������Ȼƽ��״̬ҧ����",
		m_nTraintime, m_nTrainCount);
	m_Bctrl.SetWindowTextW(L"��ʼ");
	m_CurState = BITESTART;					//���Ϊ��ʼ״̬
	UPDATAVALTOCONTROL;

	//move the window to center
	CRect rect;
	GetParent()->GetClientRect(rect);
	CRect clientrect;
	GetClientRect(clientrect);

	this->MoveWindow(CRect(rect.left+(rect.Width()-clientrect.Width())/2,
							rect.top+(rect.Height()-clientrect.Height())/2,
							(rect.Width()+clientrect.Width())/2,
							(rect.Height()+clientrect.Height())/2));

	return TRUE;  
}


HBRUSH CBiteThroldWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_STATIC&&pWnd->GetDlgCtrlID()!=IDC_SLIDBITERADIO)
	{
		pDC->SetTextColor(RGB(0, 0, 0));				//���ó��㱳������ɫ   
		pDC->SetBkMode(TRANSPARENT);						//͸��   
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}


void CBiteThroldWnd::OnDestroy()
{
	// TODO:  end thread and release the resource
	EndThread(m_hThread);
	m_hThread = INVALID_HANDLE_VALUE;

	OnOK();

	__super::OnDestroy();
}


BOOL CBiteThroldWnd::OnEraseBkgnd(CDC* pDC)
{
	
	CRect rect;
	GetClientRect(&rect);
	COLORREF bckcolor = RGB(255, 255, 255);				//white.

	pDC->FillSolidRect(rect, bckcolor);

	//return __super::OnEraseBkgnd(pDC);
	return TRUE;
}


void CBiteThroldWnd::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	switch (m_CurState)
	{
	case CBiteThroldWnd::BITESTART:					//��ǰ״̬Ϊ��ʼ
		ProcessStartState();
		break;
	case CBiteThroldWnd::BITEPAUSE:					//��ǰ״̬Ϊ��ͣ
		ProcessPauseState();
		break;
	case CBiteThroldWnd::BITEDONE:
		ProcessDoneState();
		break;
	default:
		break;
	}
}


void CBiteThroldWnd::OnBnClickedCancel()
{
	//try to load the model
	char szPath[MAX_PATH];
	if (!m_pAlogrithm)
	{
		Stringoper oper;
		m_pBase = new CCalcuteColor(m_Winlen);
		oper.CStringtoChars(m_nConfigFile, szPath);
		if (!m_pBase->LoadModel(szPath))
		{
			(*m_pOutPut)(L"�޷�������ֵģ�ͣ��޷�����ҧ����ֵ��");
			delete m_pBase;
			m_pBase = NULL;
		}
		else
		{
			(*m_pOutPut)(L"������ǰ��ֵģ����ɣ�");
			m_pAlogrithm[0] = m_pBase;
		}
	}
	GetParent()->SendMessage(MYWM_TRAINWMD, TRAINWND_BITECANCLE);

	OnCancel();
}

//private func
void CBiteThroldWnd::ProcessStartState()
{
	m_Bctrl.SetWindowTextW(L"��ͣ");
	m_CurState = BITEPAUSE;

	//�����¼��������߳�
	if (MianEvent == NULL)
	{
		MianEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		SetEvent(MianEvent);
		m_hTrainEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	if (!m_bIsOpen)
	{
		m_bIsOpen = m_nFile.Open(m_strFilePath, CFile::modeCreate | CFile::modeReadWrite)==TRUE?true:false;
	}
	StartThread(TrainThread, m_hThread);
}

void CBiteThroldWnd::ProcessPauseState()
{
	m_Bctrl.SetWindowTextW(L"��ʼ");
	m_CurState = BITESTART;
}

void CBiteThroldWnd::ProcessDoneState()
{
	UPDATACONTROLTOVAL;
	if (m_bIsOpen)								//ʵ�����ʱ����
	{
		m_nFile.Close();
		m_bIsOpen = false;
	}
	m_Ratio = m_BiteThreshold.voltvalue;
	//save the model.
	SaveResult();

	GetParent()->SendMessage(MYWM_TRAINWMD, TRAINWND_BITEDONE);		//֪ͨ�����ڣ����ѵ��
}

void CBiteThroldWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//add code here.
	/*CStatic* pState = (CStatic*)GetDlgItem(IDC_EDITBITESTATE);
	pState->SetWindowTextW(m_strState);

	CStatic* pInstruct = (CStatic*)GetDlgItem(IDC_EBITEINSTRCTION);
	pInstruct->SetWindowTextW(m_strInstrcut);*/

	m_strInstrcut.Format(L"����ҧ����ֵѵ����\r\n��%d���ڣ�ҧ��%d��,\r\n������Ȼƽ��״̬ҧ����",
		m_nTraintime, m_nTrainCount);
	UPDATAVALTOCONTROL;
	m_proState.SetPos(m_nProgress);				//���½�����...
}


void CBiteThroldWnd::OnNMCustomdrawSlidbiteradio(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	CSliderCtrl   *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDBITERADIO);
	 int pos = pSlidCtrl->GetPos();//ȡ�õ�ǰλ��ֵ 

	 m_Ratio = pos / 10.0*m_TmpThreshold;
	 m_BiteThreshold.voltvalue = m_Ratio;

	 //save to file

	 UPDATAVALTOCONTROL;
	*pResult = 0;
}

void CBiteThroldWnd::OnNMReleasedcaptureSlidbiteradio(NMHDR *pNMHDR, LRESULT *pResult)
{
	Stringoper oper;
	char szPath[MAX_PATH];
	//�������
	SaveResult();
	//�������ò���ģ��
	oper.CStringtoChars(m_nConfigFile, szPath);
	if (m_pBase)
		m_pBase->LoadModel(szPath);
	*pResult = 0;
}


LRESULT CBiteThroldWnd::OnReceivMsg(WPARAM wparam, LPARAM lparam)
{
	Stringoper oper;
	char szPath[MAX_PATH];
	switch (wparam)
	{
	case THREADMSG_PROCESSFINISH:
		m_CurState = BITEDONE;
		m_Bctrl.SetWindowTextW(L"���");
		if (m_pOutPut)
			(*m_pOutPut)(L"��ֵѵ��ģ�飺������ɣ�");
		//����ģ��
		SaveResult();
		//�������ݴ����㷨��
		m_pBase = new CCalcuteColor(m_Winlen);
		oper.CStringtoChars(m_nConfigFile, szPath);
		if (!m_pBase->LoadModel(szPath))
		{
			(*m_pOutPut)(L"�޷�������ֵģ�ͣ��޷�����ҧ����");
			delete m_pBase;
			m_pBase = NULL;
		}
		else
		{
			(*m_pOutPut)(L"������ֵģ����ɣ�");
			m_pAlogrithm[0] = m_pBase;
		}
		break;
	case THREADMSG_LOADFINISH:
		if (m_pOutPut)
			(*m_pOutPut)(L"��ֵѵ��ģ�飺����������ɣ�");
		break;
	case THREADMSG_SAVEFINISH:
		if (m_pOutPut)
			(*m_pOutPut)(L"��ֵѵ��ģ�飺����������ɣ�");
		break;
	default:
		break;
	}
	return 0;
}

//extern function���ú��������޸�
extern "C"  _declspec(dllexport) char* GetTrainModualMsg()					//��ȡģ����Ϣ
{
	return "ѵ��ҧ����ֵ���Լ��ҧ��";
}

extern "C" _declspec(dllexport)CBiteThroldWnd* CreateTrainClass()			//��ȡģ���࣬���ڼ���ģ��
{
	return new CBiteThroldWnd();											
}

extern "C" _declspec(dllexport)BOOL CreateTrainDlg(CDialogEx* pchild, CWnd* pParent)	//�������ڴ�������
{
	if (pchild == NULL || pParent == NULL)
		return FALSE;
	BOOL b = pchild->Create(IDD_BITETHROLDWND, pParent);
	if (!b)
		return FALSE;
	pchild->ShowWindow(SW_SHOW);
	return TRUE;
}

extern "C" _declspec(dllexport) CBiteThroldWnd* CreateTrainWnd(CWnd* parent)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());			//for MFC dll stander
	CBiteThroldWnd* pdlg = new CBiteThroldWnd();

	BOOL b = pdlg->Create(IDD_BITETHROLDWND, parent);
	if (!b)
	{
		delete pdlg;
		return NULL;
	}
	pdlg->ShowWindow(SW_SHOW);
	return pdlg;
}

