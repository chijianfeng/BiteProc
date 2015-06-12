// BiteThroldWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "BiteThroldWnd.h"
#include "afxdialogex.h"
#include "BiteCfgDlg.h"
#include "FiltFilt.h"
#include "Stringoper.h"
#include "ThreadUtil.h"

// CBiteThroldWnd 对话框
extern HANDLE MianEvent;						//主控线程

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
	m_nConfigFile = parm.GetBiteThroldModel();				//保存参数模型的路径
	m_strFilePath = parm.GetBiteDataPath();
}

void CBiteThroldWnd::SetData(ManagedData& d)
{
	if (m_CurState == BITESTART||m_CurState ==BITEDONE)
		return;
	m_nBuf.AppendData(d);
	SetEvent(m_hTrainEvent);							//通知主线程，可以继续接收数据
}

void CBiteThroldWnd::SetLogOut(void(*pFunc)(CString))
{
	m_pOutPut = pFunc;
}

bool CBiteThroldWnd::SaveData()
{
	if (!m_bIsOpen)
		return false;
	m_strState = L"保存数据中...";
	m_nFile.Write(m_nBuf.GetData(), m_nBuf.GetDataSize()*sizeof(double));
	m_nBuf.Remove(m_nBuf.GetDataSize());
	ResetEvent(m_hTrainEvent);
	//change the process.
	ULONGLONG dwlength = m_nFile.GetLength();
	ULONGLONG length = dwlength / sizeof(double);
	ULONGLONG total = m_nBuf.GetSampleRate()*m_nTraintime*m_nBuf.GetChannum();
	m_nProgress = (int)(length * 100 / total);					//进度计算

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
	m_nBuf.ResizeManageData((int)length + 1);				//数据导入内存
	m_strState = _T("加载数据...");

	SendMessage(WM_PAINT);									//更新

	int ret = 0;
	double *pd = new double[1000];							//数据临时缓存区
	int total = 0;

	while (ret = m_nFile.Read(pd, 1000))
	{
		
		total += ret / sizeof(double);
		m_nProgress = (int)(total * 100 / length);
		m_nBuf.AppendData(pd, ret / sizeof(double));		//放入缓存区
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
	m_strState = L"正在处理数据...";

	SendMessage(WM_PAINT);
	ff.SetParam(a_3_100, b_3_100, 7);								//设置滤波参数
	double** pd = NULL;
	pd = new double*[m_nBuf.GetChannum()];

	for (int i = 0; i < m_nBuf.GetChannum(); i++)
	{
		ManagedData d;
		pd[i] = new double[m_nBuf.GetDataSize() / m_nBuf.GetChannum()];							//加载每个通道数据
		m_nBuf.GetAnydata(d, i);
		ff.useFFilt(d.GetData(), pd[i], d.GetDataSize());
		//计算每个通道阈值
		Throld throld = calthrold(d.GetData(), pd[i], d.GetDataSize(), m_Winlen, m_nTrainCount, false);		
		m_BiteThreshold = MaxThreshold(m_BiteThreshold, throld);
		m_nProgress = (i + 1) * 100 / m_nBuf.GetChannum();
		m_TmpThreshold = m_BiteThreshold.voltvalue;												//便于调整
		Sleep(100);
		SendMessage(WM_PAINT);
	}

	//释放临时内存空间
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
	CString path = m_nConfigFile;						//加载文件参数
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
		m_pBase = new CCalcuteColor(m_Winlen);				//创建算法模型
		oper.CStringtoChars(m_nConfigFile, szPath);
		if (!m_pBase->LoadModel(szPath))					//加载
		{
			(*m_pOutPut)(L"无法加载阈值模型，无法计算咬牙！");
			delete m_pBase;
			m_pBase = NULL;
		}
		else
		{
			(*m_pOutPut)(L"加载阈值模型完成！");
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


// CBiteThroldWnd 消息处理程序


BOOL CBiteThroldWnd::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  初始化
	m_ListRatio.SetRange(0, 20);				//设置调整范围
	m_proState.SetRange(0, 100);				//设置范围
	m_proState.SetPos(0);
	m_strState = "开始训练...";
	/*CBiteCfgDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCfgParma(m_nTraintime, m_nTrainCount);
	}*/
	m_Ratio = 0.0;
	m_strInstrcut.Format(L"进行咬牙阈值训练，\r\n在%d秒内，咬完%d次,\r\n按照自然平常状态咬牙！",
		m_nTraintime, m_nTrainCount);
	m_Bctrl.SetWindowTextW(L"开始");
	m_CurState = BITESTART;					//标记为开始状态
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
		pDC->SetTextColor(RGB(0, 0, 0));				//设置成你背景的颜色   
		pDC->SetBkMode(TRANSPARENT);						//透明   
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
	// TODO:  在此添加控件通知处理程序代码
	switch (m_CurState)
	{
	case CBiteThroldWnd::BITESTART:					//当前状态为开始
		ProcessStartState();
		break;
	case CBiteThroldWnd::BITEPAUSE:					//当前状态为暂停
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
			(*m_pOutPut)(L"无法加载阈值模型，无法计算咬牙阈值！");
			delete m_pBase;
			m_pBase = NULL;
		}
		else
		{
			(*m_pOutPut)(L"加载先前阈值模型完成！");
			m_pAlogrithm[0] = m_pBase;
		}
	}
	GetParent()->SendMessage(MYWM_TRAINWMD, TRAINWND_BITECANCLE);

	OnCancel();
}

//private func
void CBiteThroldWnd::ProcessStartState()
{
	m_Bctrl.SetWindowTextW(L"暂停");
	m_CurState = BITEPAUSE;

	//创建事件，启动线程
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
	m_Bctrl.SetWindowTextW(L"开始");
	m_CurState = BITESTART;
}

void CBiteThroldWnd::ProcessDoneState()
{
	UPDATACONTROLTOVAL;
	if (m_bIsOpen)								//实现完成时工作
	{
		m_nFile.Close();
		m_bIsOpen = false;
	}
	m_Ratio = m_BiteThreshold.voltvalue;
	//save the model.
	SaveResult();

	GetParent()->SendMessage(MYWM_TRAINWMD, TRAINWND_BITEDONE);		//通知父窗口，完成训练
}

void CBiteThroldWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//add code here.
	/*CStatic* pState = (CStatic*)GetDlgItem(IDC_EDITBITESTATE);
	pState->SetWindowTextW(m_strState);

	CStatic* pInstruct = (CStatic*)GetDlgItem(IDC_EBITEINSTRCTION);
	pInstruct->SetWindowTextW(m_strInstrcut);*/

	m_strInstrcut.Format(L"进行咬牙阈值训练，\r\n在%d秒内，咬完%d次,\r\n按照自然平常状态咬牙！",
		m_nTraintime, m_nTrainCount);
	UPDATAVALTOCONTROL;
	m_proState.SetPos(m_nProgress);				//更新进度条...
}


void CBiteThroldWnd::OnNMCustomdrawSlidbiteradio(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	CSliderCtrl   *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDBITERADIO);
	 int pos = pSlidCtrl->GetPos();//取得当前位置值 

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
	//保存参数
	SaveResult();
	//重新设置参数模型
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
		m_Bctrl.SetWindowTextW(L"完成");
		if (m_pOutPut)
			(*m_pOutPut)(L"阈值训练模块：处理完成！");
		//保存模型
		SaveResult();
		//创建数据处理算法：
		m_pBase = new CCalcuteColor(m_Winlen);
		oper.CStringtoChars(m_nConfigFile, szPath);
		if (!m_pBase->LoadModel(szPath))
		{
			(*m_pOutPut)(L"无法加载阈值模型，无法计算咬牙！");
			delete m_pBase;
			m_pBase = NULL;
		}
		else
		{
			(*m_pOutPut)(L"加载阈值模型完成！");
			m_pAlogrithm[0] = m_pBase;
		}
		break;
	case THREADMSG_LOADFINISH:
		if (m_pOutPut)
			(*m_pOutPut)(L"阈值训练模块：导入数据完成！");
		break;
	case THREADMSG_SAVEFINISH:
		if (m_pOutPut)
			(*m_pOutPut)(L"阈值训练模块：保存数据完成！");
		break;
	default:
		break;
	}
	return 0;
}

//extern function：该函数不可修改
extern "C"  _declspec(dllexport) char* GetTrainModualMsg()					//获取模块信息
{
	return "训练咬牙阈值，以检测咬牙";
}

extern "C" _declspec(dllexport)CBiteThroldWnd* CreateTrainClass()			//获取模块类，用于加载模型
{
	return new CBiteThroldWnd();											
}

extern "C" _declspec(dllexport)BOOL CreateTrainDlg(CDialogEx* pchild, CWnd* pParent)	//仅仅用于创建窗体
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

