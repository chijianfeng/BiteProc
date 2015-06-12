// TLTrainWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "TLTrain.h"
#include "TLTrainWnd.h"
#include "afxdialogex.h"
#include "Stringoper.h"
#include "ArthLoad.cpp"
#include "DataProcess.h"
#include "ThreadUtil.h"

using namespace DATAPROCESS;
// CTLTrainWnd 对话框
extern CTLTrainApp theApp;
extern HANDLE MianEvent;						//用于处理主线程和子线程
IMPLEMENT_DYNAMIC(CTLTrainWnd, CDialogEx)

CTLTrainWnd::CTLTrainWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTLTrainWnd::IDD, pParent)
{

}

CTLTrainWnd::~CTLTrainWnd()
{
}

void CTLTrainWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PGSTLTRAIN, m_ProgressState);
	DDX_Text(pDX, IDC_STATICSTATE, m_strState);
	DDX_Text(pDX, IDC_ETLINSTRUCT, m_strInstruct);
	DDX_Control(pDX, IDOK, m_ButtonOK);
}

void CTLTrainWnd::SetParm(CAlgParamConfig& parm)
{
	m_bIsTrain = parm.GetTLTrainState();
	m_bReTrain = parm.GetTLReTrain();
	m_nWinLen = parm.GetWindowLength();
	m_nWinOverLen = parm.GetOverLength();

	parm.GetTLTrainTime(m_nTraintime);						//训练时长
	m_strLeftModel = parm.GetTLLeftModel();					//获取保存模型的路径
	m_strBothModel = parm.GetTLBothModel();
	m_strRightModel = parm.GetTLRightModel();
	m_DataSavePath[0] = parm.GetTLLeftData();				//获取采集数据保存的路径
	m_DataSavePath[1] = parm.GetTLRightData();
	m_DataSavePath[2] = parm.GetTLBothData();
	m_ParamPath[0] = parm.GetTLLeftParamPath();				//获取参数
	m_ParamPath[1] = parm.GetTLRightParamPath();
	m_ParamPath[2] = parm.GetTLBothParamPath();
	m_BiteThrold = LoadThrold(parm.GetBiteThroldModel());	//加载阈值模型
}

void CTLTrainWnd::SetData(ManagedData& d)
{
	if (m_CurState != STATE_START)						//判断是否是开始训练了
		return;
	m_BufData.AppendData(d);							//将数据先放入临时缓存区
	SetEvent(m_hTrainEvent);							//通知主线程，可以继续接收数据
}

bool CTLTrainWnd::SaveData()
{
	if (!m_bIsOpen)
		return false;
	if (m_nFile.GetLength() <= 0)						//文件刚刚开始写入，保存头文件信息：samplerate and channal number
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
	m_BufData.Remove(m_BufData.GetDataSize() + 1);		//清空缓存区数据
	ResetEvent(m_hTrainEvent);
	//change the process.
	ULONGLONG dwlength = m_nFile.GetLength();
	ULONGLONG length = dwlength / sizeof(double);
	ULONGLONG total = m_BufData.GetSampleRate()*m_CurTrainTime*m_BufData.GetChannum();
	m_nProgress = (int)(length * 100 / total);						//进度计算
	if (m_nProgress >= 100)
	{
		m_nProgress = 100;
		m_nFile.Close();											//关闭文件
		m_bIsOpen = false;

		if (m_nFileCount >= 3)										//3个步骤都已经训练完成
		{
			m_bExit = TRUE;
			SendMessage(CLASSIFY_ONEJOBDONE, JOB_ALLDONE, 0);		//所有任务完成
		}															//exit the thread.
		else
			SendMessage(CLASSIFY_ONEJOBDONE, JOB_ONEDONE, 0);		//仅仅通知完成一个任务
	}
	this->SendMessage(WM_PAINT);									//update the state
	return true;
}

bool CTLTrainWnd::LoadData()
{
	if (m_bIsOpen)								//实现完成时工作,关闭文件
	{
		m_nFile.Close();
		m_bIsOpen = false;
	}
	for (size_t i = 0; i < m_nTraintime.size(); i++)
	{
		m_nProgress = 0;
		if (i == 0)
		{
			m_strState = L"加载左咬两次数据...";
		}
		else if (i == 1)
			m_strState = L"加载右咬两次数据...";
		else if (i == 2)
			m_strState = L"加载同时咬两次数据...";
		//SendMessage(WM_PAINT);
		Invalidate(TRUE);
		MyLoaddata(m_DataSavePath[i], m_TrainDataBuf[i]);
		Sleep(500);
	}
	return true;
}

bool CTLTrainWnd::DoProcess()//数据处理
{
	Stringoper oper;
	m_strState = L"正在处理数据...";
	Invalidate(TRUE);

	CArthLoad<CBaseArth> loader;			//加载算法模块
	
	loader.SetArthPath(L"./TLDetec.dll");	//默认算法模块路径
	for (int i = 0; i < 3;i++)				//顺序分别为左侧、右侧、同时
	{
		char tmp[1024];
		CString parampath = GetDirectory(m_DataSavePath[i]) + L"\\" + m_ParamPath[i];
		oper.CStringtoChars(parampath, tmp);

		loader.SetParam(tmp);				//设置参数路径
		if (!loader.LoadArth(&m_pArth[i]))
		{
			m_pOutPut(L"Failed to load the TL Arth "+i);
			continue;
		}
		m_pArth[i]->Config();				//配置

		//进行数据准备，提取咬牙相关数据段
		ManagedData filtd;
		ProcessData(m_TrainDataBuf[i],filtd);	//滤波
		BpNetdata netdata[1000];
		Processdata prodata[1000];
		int samplenum = ExtracSample(m_TrainDataBuf[i], filtd, netdata, prodata, m_BiteThrold, m_nWinLen);
		CString trainfile;
		trainfile.Format(_T("Train_DataInput_%d_.txt"), i);
		trainfile = GetDirectory(m_DataSavePath[i]) + L"\\" + trainfile;
		if (!SavetoFile(m_TrainDataBuf[i], filtd, netdata, samplenum, trainfile))
		{
			m_pOutPut(L"长咬与两次保存数据失败");
			continue;
		}
		if (!m_pArth[i]->LoadTrainData(oper.tostring(trainfile).c_str()))
		{
			delete m_pArth[i];
			m_pOutPut(L"长咬与两次：加载训练数据出错，" + i);
			continue;
		}
		m_pArth[i]->Train();			//训练模型
	}
	SendMessage(CLASSIFY_ONEJOBDONE, JOB_PROCESSDONE, 0);
	return true;
}

bool CTLTrainWnd::ChangeState(CAlgParamConfig& parm, bool s)
{
	parm.SetBiteState(s);
	return true;
}

bool CTLTrainWnd::StartThread(DWORD(WINAPI lp)(LPVOID), HANDLE& handle)
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

bool CTLTrainWnd::EndThread(HANDLE& handle)
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

bool CTLTrainWnd::SaveResult()			//保存训练模型
{
	Stringoper oper;
	char tmp[1024];
	
	oper.CStringtoChars(m_strLeftModel, tmp);
	if (m_pArth[0])
		m_pArth[0]->SaveTrainModel(tmp);
	oper.CStringtoChars(m_strRightModel, tmp);
	if (m_pArth[1])
		m_pArth[1]->SaveTrainModel(tmp);
	oper.CStringtoChars(m_strBothModel, tmp);
	if (m_pArth[2])
		m_pArth[2]->SaveTrainModel(tmp);


	return true;
}

bool CTLTrainWnd::LoadResult()
{
	Stringoper oper;
	char path[MAX_PATH];
	//加载左侧模型
	oper.CStringtoChars(m_strLeftModel, path);
	CArthLoad<CBaseArth> loader;				//加载算法模块
	loader.SetArthPath(L"./TLDetec.dll");		//默认算法模块路径
	if (!m_pAlogrithm[0])
	{
		char tmp[1024];
		CString parampath = GetDirectory(m_DataSavePath[0]) + L"\\" + m_ParamPath[0];
		oper.CStringtoChars(parampath, tmp);	//参数文件
		loader.SetParam(tmp);
		if (!loader.LoadArth(&m_pAlogrithm[0]))
		{
			m_pOutPut(L"Failed to load the TL Arth: 0");
			m_pAlogrithm[0] = NULL;
		}
		else
		{
			m_pAlogrithm[0]->Config();					//配置
			m_pAlogrithm[0]->LoadModel(path);			//加载模型
		}
	}
	else
		m_pAlogrithm[0]->LoadModel(path);				//重新加载模型

	//加载右侧模型
	oper.CStringtoChars(m_strRightModel, path);
	if (!m_pAlogrithm[1])
	{
		char tmp[1024];
		CString parampath = GetDirectory(m_DataSavePath[1]) + L"\\" + m_ParamPath[1];
		oper.CStringtoChars(parampath, tmp);	//参数文件
		loader.SetParam(tmp);
		if (!loader.LoadArth(&m_pAlogrithm[1]))
		{
			m_pOutPut(L"Failed to load the TL Arth: 1");
			m_pAlogrithm[1] = NULL;
		}
		else
		{	
			m_pAlogrithm[1]->Config();					//配置
			m_pAlogrithm[1]->LoadModel(path);			//加载模型
		}
	}
	else
		m_pAlogrithm[1]->LoadModel(path);				//重新加载模型

	//加载两侧模型
	oper.CStringtoChars(m_strBothModel, path);
	if (!m_pAlogrithm[2])
	{
		char tmp[1024];
		CString parampath = GetDirectory(m_DataSavePath[2]) + L"\\" + m_ParamPath[2];
		oper.CStringtoChars(parampath, tmp);	//参数文件
		loader.SetParam(tmp);
		if (!loader.LoadArth(&m_pAlogrithm[2]))
		{
			m_pOutPut(L"Failed to load the TL Arth: 2");
			m_pAlogrithm[2] = NULL;
		}
		else
		{
			m_pAlogrithm[2]->Config();					//配置
			m_pAlogrithm[2]->LoadModel(path);			//加载模型
		}
	}
	else
		m_pAlogrithm[2]->LoadModel(path);				//重新加载模型
	return true;
}

void CTLTrainWnd::SetLogOut(void(*pFunc)(CString))
{
	m_pOutPut = pFunc;
}

//private function
void CTLTrainWnd::MyLoaddata(CString path, ManagedData& buf)
{
	CFile file;
	bool isopen = file.Open(path, CFile::modeRead) == TRUE ? true : false;

	if (!isopen)
	{
		m_pOutPut(L"无法打开文件:" + path);
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
		str = str.substr(0, str.find('\n') + 1);
		string key, val;
		oper.AnalyseLine(str, key, val, '%');
		buf.SetSampleRate(oper.stringtoint(val));

		file.Seek(str.size(), CFile::begin);			//设置读取位置
		int	pos = str.size();

		ret = file.Read(strbuf, 512);
		str = oper.charstostring(strbuf);
		str = str.substr(0, str.find('\n') + 1);
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

Throld CTLTrainWnd::LoadThrold(CString path)
{
	Throld throld = { 0.0, 0.0 };
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

BEGIN_MESSAGE_MAP(CTLTrainWnd, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTLTrainWnd::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTLTrainWnd::OnBnClickedCancel)
	ON_MESSAGE(CLASSIFY_ONEJOBDONE, OnJobMsg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTLTrainWnd 消息处理程序


void CTLTrainWnd::OnBnClickedOk()
{
	switch (m_CurState)
	{
	case CTLTrainWnd::STATE_NONE:
		ProcessNone();
		break;
	case CTLTrainWnd::STATE_START:
		ProcessStart();
		break;
	case CTLTrainWnd::STATE_PAUSE:
		ProcessPause();
		break;
	case CTLTrainWnd::STATE_DONE:
		ProcessDone();
		break;
	case CTLTrainWnd::STATE_RETRAIN:
		ProcessRetrain();
		break;
	default:
		break;
	}
}


void CTLTrainWnd::OnBnClickedCancel()
{
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
	GetParent()->SendMessage(MYWM_TRAINWMD, TRAINWND_TLCANCLE);
	OnCancel();
}


BOOL CTLTrainWnd::OnInitDialog()
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
	for (int i = 0; i < 3;i++)		//模型指针
	{
		m_pArth[i] = 0;
	}
	m_strState = L"训练长咬、两次咬模型";
	m_strInstruct = L"只采集两次咬数据，两次咬不需要过快";

	m_ButtonOK.SetWindowTextW(L"开始");

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


void CTLTrainWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	UPDATAVALTOCONTROL;
	m_ProgressState.SetPos((int)m_nProgress);
}


BOOL CTLTrainWnd::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	COLORREF bckcolor = RGB(255, 255, 255);				//white.

	pDC->FillSolidRect(rect, bckcolor);

	//return __super::OnEraseBkgnd(pDC);
	return TRUE;
}


LRESULT CTLTrainWnd::OnJobMsg(WPARAM wparam, LPARAM lparam)
{
	if (wparam == JOB_ONEDONE)
	{
		if (!m_bIsOpen)
		{
			m_bIsOpen = m_nFile.Open(m_DataSavePath[m_nFileCount], CFile::modeCreate
				| CFile::modeReadWrite) == TRUE ? true : false;
			if (m_nFileCount == 1)
				m_strState = L"正在采集右咬两次数据...";
			if (m_nFileCount == 2)
				m_strState = L"正在采集同时咬两次数据...";
			m_CurTrainTime = m_nTraintime.at(m_nFileCount);

			m_nFileCount++;
			m_nProgress = 0;
			m_ButtonOK.SetWindowTextW(L"开始");
			m_CurState = STATE_PAUSE;						//准备进入下一状态
			Invalidate(TRUE);
		}
	}
	if (wparam == JOB_ALLDONE)
	{
		m_CurState = STATE_PAUSE;
	}
	if (wparam == JOB_PROCESSDONE)
	{
		m_pOutPut(L"长咬与两次咬模型训练完成");
		m_strState = L"长咬与两次咬模型训练完成";
		m_CurState = STATE_DONE;
		m_ButtonOK.SetWindowTextW(L"完成");
		Invalidate(TRUE);
	}
	return 0;
}

HBRUSH CTLTrainWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC && (pWnd->GetDlgCtrlID() != IDC_ETLSIMRESULT&&pWnd->GetDlgCtrlID() != IDC_ETLINSTRUCT))
	{
		pDC->SetTextColor(RGB(0, 0, 0));				//设置成你背景的颜色   
		pDC->SetBkMode(TRANSPARENT);						//透明   
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}

bool CTLTrainWnd::SavetoFile(ManagedData& rawd, ManagedData& newdL,BpNetdata* pData , int num, CString path)
{
	if (num <= 0)
		return false;
	CStdioFile file;
	if (!file.Open(path, CFile::modeCreate | CFile::modeWrite))
		return false;
	Matrix Inputdata;
	Inputdata.pdata = new double*[num];
	Inputdata.colnum = 0;
	Inputdata.raw = (int)(BITELENGTH*rawd.GetSampleRate());
	int channel = GetBiteChannel(rawd);
	int cs = rawd.GetChannum();
	int Curpos = 0;
	for (int i = 0; i < num; i++)
	{
		if (pData[i].sp + Inputdata.raw >= rawd.GetDataSize() / cs)
			break;
		if (Curpos <= pData[i].sp)
		{
			Inputdata.pdata[Inputdata.colnum] = new double[Inputdata.raw];
			for (int k = 0; k < Inputdata.raw; k++)
			{
				Inputdata.pdata[Inputdata.colnum][k] = newdL[(pData[i].sp + k)*cs + channel];
			}
			Inputdata.colnum++;
			Curpos = pData[i].sp + Inputdata.raw;
		}
	}
	//提取完数据后，将数据保存到文件中
	Streamoper oper;
	string str = "";
	str += oper.SerizlizeStream((int)Inputdata.colnum) + "\t";
	str += oper.SerizlizeStream((int)Inputdata.raw) + "\t";
	str += oper.SerizlizeStream((int)1) + "\n";
	Stringoper sp;
	file.WriteString(sp.tocstring(str));

	//写入数据
	for (int i = 0; i < Inputdata.colnum; i++)
	{
		Streamoper oper;
		string str = "";
		for (int j = 0; j < Inputdata.raw; j++)
		{
			str += oper.SerizlizeStream(Inputdata.pdata[i][j]) + " ";
		}
		str += oper.SerizlizeStream(0.9) + '\n';
		Stringoper sp;
		file.WriteString(sp.tocstring(str));
	}
	file.Close();

	//释放内存
	for (int i = 0; i < Inputdata.colnum; i++)
	{
		delete Inputdata.pdata[i];
	}
	delete[] Inputdata.pdata;
	return true;
}

int CTLTrainWnd::GetBiteChannel(ManagedData& d)
{
	int i = 0;
	int index = 0;
	double maxv = 0;
	for (i = 0; i < d.GetChannum(); i++)
	{
		ManagedData tmp;
		d.GetAnydata(tmp, i);
		double v = stdval(tmp.GetData(), tmp.GetDataSize());
		if (maxv < v)
		{
			index = i;
			maxv = v;
		}
	}
	return index;
}

CString CTLTrainWnd::GetDirectory(CString path)
{
	if (path.GetLength() <= 0)
		return L"";
	CString dir(L"");
	dir = path.Left(path.ReverseFind('\\'));
	return dir;
}

void CTLTrainWnd::ProcessNone()
{
	m_CurState = STATE_START;
	m_ButtonOK.SetWindowTextW(L"暂停");
	if (MianEvent == NULL)
	{
		MianEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		SetEvent(MianEvent);
		m_hTrainEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	if (!m_bIsOpen)
	{
		m_bIsOpen = m_nFile.Open(m_DataSavePath[0], CFile::modeCreate | CFile::modeReadWrite) == TRUE ? true : false;
		m_strState = L"正在采集左咬两次数据...";
		m_nFileCount = 1;
		m_CurTrainTime = m_nTraintime.at(0);
		Invalidate(TRUE);
	}
	StartThread(TrainThread, m_hThread);
}

void CTLTrainWnd::ProcessStart()
{
	if (m_nProgress < 100)							//当前步骤未完成
	{
		m_CurState = STATE_PAUSE;					//设置状态为暂停
		m_ButtonOK.SetWindowTextW(L"开始");
		return;
	}
}

void CTLTrainWnd::ProcessPause()
{
	m_CurState = STATE_START;					//修改状态
	m_ButtonOK.SetWindowTextW(L"暂停");
}

void CTLTrainWnd::ProcessDone()
{
	UPDATACONTROLTOVAL;
	if (!SaveResult())
	{
		m_pOutPut(L"TL Train ：无法保存训练结果");
	}
	m_pAlogrithm[0] = m_pArth[0];     //hwh_add
	m_pAlogrithm[1] = m_pArth[1];
	m_pAlogrithm[2] = m_pArth[2];
	GetParent()->SendMessage(MYWM_TRAINWMD, TRAINWND_TLDONE);
	OnOK();
}

void CTLTrainWnd::ProcessRetrain()
{
	m_CurState = STATE_PAUSE;
	m_ButtonOK.SetWindowTextW(L"暂停");
	if (MianEvent == NULL)
	{
		MianEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		SetEvent(MianEvent);
		m_hTrainEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	StartThread(TrainThread, m_hThread);
}

extern "C"  _declspec(dllexport) char* GetTrainModualMsg()					//获取模块信息
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return "训练咬牙阈值，以检测咬牙";
}

extern "C" _declspec(dllexport)CTLTrainWnd*  CreateTrainClass()				//获取模块类，用于加载模型
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return new CTLTrainWnd();
}

extern "C" _declspec(dllexport)BOOL  CreateTrainDlg(CDialogEx* pchild, CWnd* pParent)	//仅仅用于创建窗体
{
	HINSTANCE save_hInstance = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);

	//AFX_MANAGE_STATE(AfxGetAppModuleState());
	if (pchild == NULL || pParent == NULL)
		return FALSE;
	BOOL b = pchild->Create(IDD_TLTRAINWND, pParent);
	if (!b)
		return FALSE;
	pchild->ShowWindow(SW_SHOW);
	AfxSetResourceHandle(save_hInstance);
	return TRUE;
}

extern "C" _declspec(dllexport) CTLTrainWnd* CreateTrainWnd(CWnd* parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());			//for MFC dll stander
	CTLTrainWnd* pdlg = new CTLTrainWnd(parent);

	BOOL b = pdlg->Create(IDD_TLTRAINWND, parent);
	if (!b)
	{
		delete pdlg;
		return NULL;
	}
	pdlg->ShowWindow(SW_SHOW);
	return pdlg;
}





