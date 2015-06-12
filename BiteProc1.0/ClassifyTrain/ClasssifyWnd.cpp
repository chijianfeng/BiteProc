// ClasssifyWnd.cpp : 实现文件
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
// CClasssifyWnd 对话框
extern HANDLE MianEvent;						//用于处理主线程和子线程

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
	parm.GetClassifyTrain(m_nTraintime);					//获取训练时间
	m_ModlePath = parm.GetClassifyModel();					//获取模型保存的路径
	m_DataSavePath[0] = parm.GetClassifyLData();
	m_DataSavePath[1] = parm.GetClassifyRData();
	m_DataSavePath[2] = parm.GetClassifyBData();
	m_BiteThrold = LoadThrold(parm.GetBiteThroldModel());	//加载阈值模型
	parm.GetLRThrolds(m_dL2M, m_dM2R);
}

void CClasssifyWnd::SetData(ManagedData& d)
{
	if (m_CurState != STATE_START)						//判断是否是开始训练了
		return;
	m_BufData.AppendData(d);							//将数据先放入临时缓存区
	SetEvent(m_hTrainEvent);							//通知主线程，可以继续接收数据
}

bool CClasssifyWnd::SaveData()
{
	if (!m_bIsOpen)
		return false;
	if (m_nFile.GetLength() <= 0)						//文件刚刚开始写入，保存头文件信息：samplerate and channal number
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
	m_BufData.Remove(m_BufData.GetDataSize()+1);		//清空缓存区数据
	ResetEvent(m_hTrainEvent);
	//change the process.
	ULONGLONG dwlength = m_nFile.GetLength();
	ULONGLONG length = dwlength / sizeof(double);
	ULONGLONG total = m_BufData.GetSampleRate()*m_CurTrainTime*m_BufData.GetChannum();
	m_nProgress = (int)(length * 100 / total);						//进度计算

	if (m_nProgress >= 100)
	{
		m_nProgress = 100;
		m_nFile.Close();										//关闭文件
		m_bIsOpen = false;

		if (m_nFileCount >= 3)									//3个步骤都已经训练完成
		{
			m_bExit = TRUE;
			SendMessage(CLASSIFY_ONEJOBDONE, JOB_ALLDONE, 0);		//所有任务完成
		}														//exit the thread.
		else
			SendMessage(CLASSIFY_ONEJOBDONE, JOB_ONEDONE, 0);				//仅仅通知完成一个任务
	}
	this->SendMessage(WM_PAINT);								//update the state

	return true;
}

bool CClasssifyWnd::LoadData()
{
	if (m_bIsOpen)								//实现完成时工作,关闭文件
	{
		m_nFile.Close();
		m_bIsOpen = false;
	}
	
	for (size_t i = 0; i < m_nTraintime.size();i++)
	{
		m_nProgress = 0;
		if (i == 0)
		{
			m_strState = L"加载左咬数据...";
		}
		else if (i == 1)
			m_strState = L"加载右咬数据...";
		else if (i == 2)
			m_strState = L"加载同时咬数据...";
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
	m_strState = L"正在处理数据...";
	//SendMessage(WM_PAINT);
	Invalidate(TRUE);

	//首先创建模型参数文件
	if (!CreateModelFile())
	{
		SendMessage(MYWM_THREADMSG, CLASSIFY_CREATEFAILED);
		return false;
	}
	else
		SendMessage(MYWM_THREADMSG, CLASSIFY_CREATEDONE);
	//创建模型并设置参数
	m_pNetWorker = new CNetWorker(path);				//构造训练模型
	m_pNetWorker->SetParament(m_Error, m_MaxTrainTime);
	m_pNetWorker->SetLRThrolds(m_dL2M, m_dM2R);		
	m_pNetWorker->SetWinlen(m_nWinLen);
	m_pNetWorker->SetThreashold(m_BiteThrold);

	//开始处理数据
	BpNetdata **pNet = new BpNetdata *[3];				//保存左、右&同时咬牙数据
	int Smaplenum[3];									//每组数据的样本数
	for (int i = 0; i < 3;i++)
	{
		ManagedData Newdata;
		ProcessData(m_TrainDataBuf[i], Newdata);		//滤波3~100HZ
		pNet[i] = new BpNetdata[1000];					//保存提取的数据
		Processdata prodata[1000];
		//提取咬牙数据
		Smaplenum[i] = ExtracSample(m_TrainDataBuf[i], Newdata, pNet[i], prodata, m_BiteThrold, m_nWinLen);
	}
	//before training the data. mix the sample data.
	Matrix out;
	out.pdata = new double *[3];						//输入
	out.colnum = Smaplenum[0] + Smaplenum[1] + Smaplenum[2];
	out.raw = 3;
	Matrix target;
	target.pdata = new double *[1];						//输出
	target.raw = 1;
	target.colnum = Smaplenum[0] + Smaplenum[1] + Smaplenum[2];
	//分配空间
	for (int i = 0; i < 3; i++)
	{
		out.pdata[i] = new double[out.colnum];
	}
	target.pdata[0] = new double[target.colnum];
	//进行混合，便于神经网络训练样本随机性
	mixBpdata(pNet[0], Smaplenum[0], pNet[1], Smaplenum[1], pNet[2], Smaplenum[2], out, target);
	for (int i = 0; i < 3; i++)
	{
		delete pNet[i];
	}
	delete pNet;
	
	//将数据保存到临时文件中
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

	//文件保存结束后，开始训练。。
	for (int i = 0; i < 3; i++)
	{
		delete out.pdata[i];
	}
	delete target.pdata[0];
	delete out.pdata;
	delete target.pdata;

	SetTimer(TIMER_FOR_TRAIN, 600, NULL);					//0.6秒检查一次状态

	Stringoper soper;
	oper.CStringtoChars(datapath, path);

	m_strState = L"正在训练模型...";
	//SendMessage(WM_PAINT);
	Invalidate();											//更新状态
	bool res = m_pNetWorker->Train(path);					//开始训练

	//将结果保存到临时文件，后续进行删除
	oper.CStringtoChars(GetDirectory() + L"\\tmpModel.txt", path);
	m_pNetWorker->SaveTrainModel(path);

	KillTimer(TIMER_FOR_TRAIN);
	SendMessage(CLASSIFY_ONEJOBDONE, JOB_PROCESSDONE);		//进行下一个步骤
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

	CString algpath = m_ModlePath.Left(m_ModlePath.ReverseFind('\\')+1);	//不推荐该方式
	algpath += CONFIGFILE;
	CAlgParamConfig config(algpath);
	if (config.IsOpen())
	{
		config.LoadParam();												//加载参数至内存
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
			//设置相关参数
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


// CClasssifyWnd 消息处理程序


BOOL CClasssifyWnd::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (!m_bReTrain)
	{
		m_CurState = STATE_NONE;				//开始状态为无
		m_bExit = FALSE;
	}
	else
	{
		m_CurState = STATE_RETRAIN;
		m_bExit = TRUE;							//不进行数据采集操作
	}

	m_nFileCount = 0;
	m_strState = "训练分类模型...";
	m_ButtonOK.SetWindowText(L"开始");
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
	// 异常:  OCX 属性页应返回 FALSE
}


HBRUSH CClasssifyWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC&&(pWnd->GetDlgCtrlID()!=IDC_SLIDERLTOM
		&&pWnd->GetDlgCtrlID() != IDC_SLIDERMTOR&&IDC_EDITRESULT!=pWnd->GetDlgCtrlID()))
	{
		pDC->SetTextColor(RGB(0, 0, 0));				//设置成你背景的颜色   
		pDC->SetBkMode(TRANSPARENT);						//透明   
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

	//退出窗口，清除窗口相关训练资源

}

void CClasssifyWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_strInstruct.Format(L"左边咬牙时长：%d；\r\n右边咬牙时长：%d；\r\n同时咬牙时长：%d;\r\n咬牙时尽量包含有轻有重",
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
	//通知coremodule，进行测试，
	CWnd* pParent = this->GetParent();
	if (!pParent)
	{
		m_pOutPut(L"无法通知主模块：无法获取主模块句柄");
	}
	CFileOper foper;
	if (!foper.FileExist(GetDirectory() + L"\\tmpModel.txt"))
	{
		m_pOutPut(L"无法进行测试，无法找到最新模型");
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
	m_pOutPut(L"神经网络开始测试");
	pParent->SendMessage(MYWM_TRAINWMD, TRAINWND_TEST,0);				//表示开始测试
}


void CClasssifyWnd::OnBnClickedCancel()
{
	//取消操作
	if (AfxMessageBox(L"模型尚未保存，是否保存?", MB_YESNO) == IDYES)
	{
		if (SaveResult())
			m_pOutPut(L"模型保存成功");
		else
		{
			m_pOutPut(L"模型保存失败");
		}
	}
	else
	{
		m_pOutPut(L"加载先前模型...");
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
		m_ButtonOK.SetWindowTextW(L"完成");
		if (m_pOutPut)
			(*m_pOutPut)(L"分类训练模块：模型训练完成！");
		m_pNetWorker->m_BpNet.m_bSimulateDataFlag = true;
		m_pAlogrithm[0] = m_pNetWorker;							//模型可用。。
		break;
	case THREADMSG_LOADFINISH:
		if (m_pOutPut)
			(*m_pOutPut)(L"分类训练模块：导入数据完成！");
		break;
	case THREADMSG_SAVEFINISH:
		if (m_pOutPut)
			(*m_pOutPut)(L"分类训练模块：保存数据完成！");
		break;
	case CLASSIFY_CREATEDONE:
		if (m_pOutPut)
			(*m_pOutPut)(L"创建分类模块参数文件完成！");
		break;
	case CLASSIFY_CREATEFAILED:
		if (m_pOutPut)
			(*m_pOutPut)(L"创建分类模块参数文件失败！");
		break;
	case CLASSIFY_MODELCANCLE:
		if (m_pOutPut)
			(*m_pOutPut)(L"加载分类模型失败！");
		break;
	case CLASSIFY_MODELOK:
		if (m_pOutPut)
			(*m_pOutPut)(L"加载分类模型成功！");
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
		if (m_pNetWorker)						//获取训练状态并进行显示
		{
			double err;
			int time;
			m_pNetWorker->GetTrainState(err, time);
			m_strState.Format(L"训练神经网络状态:\r\n     训练次数：%d\r\n     系统误差：%lf", time, err);
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
	// TODO:  显示调整范围
	int pos = m_Left2Middle.GetPos();
	m_dL2M = pos / 10.0 * (-1);
	UPDATAVALTOCONTROL;
	*pResult = 0;
}


void CClasssifyWnd::OnNMReleasedcaptureSliderltom(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  对模型进行参数更新

	*pResult = 0;
}


void CClasssifyWnd::OnNMCustomdrawSlidermtor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  显示调整范围
	int pos = m_Middle2Right.GetPos();
	m_dM2R = pos / 10.0;
	UPDATAVALTOCONTROL;
	*pResult = 0;
}


void CClasssifyWnd::OnNMReleasedcaptureSlidermtor(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  对模型参数进行更新

	*pResult = 0;
}

//private function

Throld CClasssifyWnd::LoadThrold(CString path)
{
	Throld throld = {0.0,0.0};
	CStdioFile file;
	if (!file.Open(path, CFile::modeRead))
	{
		m_pOutPut(L"无法加载阈值模型");
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
		m_pOutPut(L"无法打开文件:"+path);
		return;
	}
	file.SeekToBegin();
	ULONGLONG length = file.GetLength() / sizeof(double);
	buf.ResizeManageData((int)length + 1);
	//buf.SetSampleRate(m_nSampleRate);
	//buf.SetChannum(m_nChannum);
	/*首先读取文件头*/
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

		file.Seek(str.size() , CFile::begin);			//设置读取位置
		int	pos = str.size();

		ret = file.Read(strbuf, 512);
		str = oper.charstostring(strbuf);
		str = str.substr(0, str.find('\n')+1);
		oper.AnalyseLine(str, key, val, '%');
		buf.SetChannum(oper.stringtoint(val));

		file.Seek(pos + str.size(), CFile::begin);		//读取完头
	}
	CATCH(CMemoryException, e)
	{
		file.Close();
		m_pOutPut(L"文件头读取错误");
		return;
	}
	END_CATCH

	int ret = 0;
	double *pd = new double[1000];
	ULONGLONG total = 0;
	while (ret = file.Read(pd, 1000))					//读取数据
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
	string str = "## 样本的输入层的数目: ##\r\n";
	file.Write(str.c_str() , str.size());
	str = "3\r\n";
	file.Write(str.c_str() , str.size());
	str = "## 样本的隐含层的数目: ##\r\n";
	file.Write(str.c_str(),str.size());
	str = (oper.inttostring(m_HindLayer)) + "\r\n";
	file.Write(str.c_str() , str.size());
	str = "## 样本的输出层的数目: ##\r\n";
	file.Write(str.c_str() , str.size());
	str = "1\r\n";
	file.Write(str.c_str() , str.size());
	str = "## 训练网络所采用的神经网络算法: ##\r\n";
	file.Write(str.c_str() , str.size());
	str = (oper.inttostring(m_nSelArth)) + "\r\n";
	file.Write(str.c_str() , str.size());
	str = "## 网络中使用的函数的索引值: ##\r\n";
	file.Write(str.c_str() , str.size());
	str = "2\r\n";
	file.Write(str.c_str() , str.size());

	file.Close();
	return true;
}

void CClasssifyWnd::ProcessRetrain()
{
	m_CurState = STATE_STOP;
	m_ButtonOK.SetWindowTextW(L"中止");
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
	m_ButtonOK.SetWindowTextW(L"暂停");
	//创建事件，启动线程
	if (MianEvent == NULL)
	{
		MianEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		SetEvent(MianEvent);
		m_hTrainEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	if (!m_bIsOpen)
	{
		m_bIsOpen = m_nFile.Open(m_DataSavePath[0], CFile::modeCreate | CFile::modeReadWrite) == TRUE ? true : false;
		m_strState = L"正在采集左咬数据...";
		m_nFileCount = 1;
		m_CurTrainTime = m_nTraintime.at(0);
		Invalidate(TRUE);
	}
	StartThread(TrainThread, m_hThread);
}

void CClasssifyWnd::ProcessStart()
{
	if (m_nProgress < 100)							//当前步骤未完成
	{	
		m_CurState = STATE_PAUSE;					//设置状态为暂停
		m_ButtonOK.SetWindowTextW(L"开始");
		return;
	}
	//否则当前步骤已经完成；
	if (!m_bIsOpen)
	{
		m_bIsOpen = m_nFile.Open(m_DataSavePath[m_nFileCount], CFile::modeCreate | CFile::modeReadWrite) == TRUE ? true : false;
		if (m_nFileCount == 1)
			m_strState = L"正在采集右咬数据...";
		if (m_nFileCount == 2)
			m_strState = L"正在采集同时咬数据...";
		m_CurTrainTime = m_nTraintime.at(m_nFileCount);

		m_nFileCount++;
		m_nProgress = 0;
		m_ButtonOK.SetWindowTextW(L"开始");
		m_CurState = STATE_PAUSE;						//准备进入下一状态
		Invalidate(TRUE);
	}
}

void CClasssifyWnd::ProcessPause()
{
	m_CurState = STATE_START;					//修改状态
	m_ButtonOK.SetWindowTextW(L"暂停");
}

void CClasssifyWnd::ProcessDone()
{
	UPDATACONTROLTOVAL;
	if (!SaveResult())
	{
		m_pOutPut(L"Classify Train ：无法保存训练结果");
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
		m_pNetWorker->StopTrain(true);					//停止训练
	}
	m_pOutPut(L"终止神经网络的训练！");					//仅仅添加一条状态
	m_ButtonOK.SetWindowTextW(L"完成");
	m_CurState = STATE_DONE;							//到下一个状态进行保存
	m_ButtonTest.EnableWindow(TRUE);					//可进行测试
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
		m_ButtonOK.SetWindowTextW(L"中止");
		m_CurState = STATE_STOP;
	}
	if (JOB_PROCESSDONE == wparam)
	{
		m_pOutPut(L"神经网络训练完成！");
		m_CurState = STATE_DONE;
		m_ButtonOK.SetWindowTextW(L"完成");
		m_ButtonTest.EnableWindow(TRUE);
	}
	return 0;
}
//extern function：该函数不可修改
extern "C"  _declspec(dllexport) char* GetTrainModualMsg()					//获取模块信息
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return "训练咬牙阈值，以检测咬牙";
}

extern "C" _declspec(dllexport)CClasssifyWnd*  CreateTrainClass()			//获取模块类，用于加载模型
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return new CClasssifyWnd();
}

extern "C" _declspec(dllexport)BOOL  CreateTrainDlg(CDialogEx* pchild, CWnd* pParent)	//仅仅用于创建窗体
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

//添加相关菜单
extern "C" _declspec(dllexport) BOOL InstallExtMenu(CWnd* pChild, HWND Parent, UINT* NextMenuID)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	BOOL bReturn = FALSE;
	if (!pChild)					//dll 中的wnd为空，无法添加
		return FALSE;
	if (!Parent)
		return FALSE;
	HMENU hMenu = ::GetMenu(Parent);		//获取父窗口的菜单
	if (!hMenu)
		return FALSE;
	CMenu ParentMenu;
	ParentMenu.Attach(hMenu);
	CMenu Menu;
	if (!Menu.CreateMenu())						//创建菜单
		return FALSE;

	theApp.m_CommandToMessage.InitHashTable(1);	//可增加
	NextMenuID = new UINT[1];
	NextMenuID[0] = theApp.ID_MENU_PARAM;
	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"参数设置");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_PARAM);	//设置映射

	HMENU hsubMenu = Menu.Detach();
	ParentMenu.InsertMenuW(ParentMenu.GetMenuItemCount(), MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)hsubMenu,
		_T("分类训练模块设置"));

	ParentMenu.Detach();
	CWnd::FromHandle(Parent)->DrawMenuBar();		//强制重绘菜单
	return TRUE;
}


extern"C" _declspec(dllexport)BOOL CheckExtMenuItem(UINT id)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	if (id == theApp.ID_MENU_PARAM)				//判断是否是菜单消息
		return TRUE;
	else
		return FALSE;
}




