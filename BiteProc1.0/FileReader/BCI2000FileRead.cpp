#include "StdAfx.h"
#include "BCI2000FileRead.h"
#include "Stringoper.h"
HANDLE	BCI2000File_HANDLE;
UINT DataProcess(LPVOID lprarm)				//数据文件读取线程
{
	BCI2000FileRead* preader = (BCI2000FileRead*)lprarm;
	if(preader==NULL)
		return 1;
	DWORD ret;
	while(!preader->m_bExit)
	{
		ret = WaitForSingleObject(BCI2000File_HANDLE,5000);		
		if(ret==WAIT_TIMEOUT)
		{
			preader->EB_Stop();
			preader->EB_CloseDevice();
			AfxMessageBox(_T("Get Data time out!"));
			return 1;
		}
		preader->ExtracetDatatoBuf(preader->m_bufsize==0?MAXLENGTH:preader->m_bufsize);
		Sleep(32);
	}
	return 0;
}
BCI2000FileRead::BCI2000FileRead(void)
{
	perrmsg		= new char[_MAX_LENGTH_];
	m_bufsize	= 0;
	pPath		= new char[_MAX_LENGTH_];
	m_mode		= 0;
	m_samplerate= 0;
	pdeviemsg	= new char[_MAX_LENGTH_];
	m_pDataBuf	= new double[MAXLENGTH];		//data buffer 
	m_Rear = m_Head = 0;
	m_CurGetLength = 0;
	m_pCurov = NULL;
	m_pSelChan = new UCHAR[_MAX_LENGTH_];
	m_nChansize = 0;
	m_SampleInFile = 0;
	m_pThread = NULL;
	m_bExit = false;
}

BCI2000FileRead::BCI2000FileRead(char* p )
{
	//set the data file path and init
	perrmsg		= new char[_MAX_LENGTH_];
	m_bufsize	= 0;
	pPath		= new char[_MAX_LENGTH_];
	strcpy_s(pPath,_MAX_LENGTH_,p);
	m_mode		= 0;
	m_samplerate= 0;
	pdeviemsg	 = new char[_MAX_LENGTH_];
	m_pDataBuf	= new double[MAXLENGTH];		//data buffer 
	m_Rear = m_Head = 0;
	m_CurGetLength = 0;
	m_pCurov = NULL;
	m_pSelChan = new UCHAR[_MAX_LENGTH_];
	m_nChansize = 0;
	m_SampleInFile = 0;
	m_pThread = NULL;
	m_bExit = false;
}
BCI2000FileRead::BCI2000FileRead(const BCI2000FileRead& reader):
m_bufsize(reader.m_bufsize),m_mode(reader.m_mode),
	m_samplerate(reader.m_samplerate)
{
	perrmsg		= new char[_MAX_LENGTH_];
	strcpy_s(perrmsg , _MAX_LENGTH_ , reader.perrmsg);
	pPath		= new char[_MAX_LENGTH_];
	strcpy_s(pPath, _MAX_LENGTH_ , reader.pPath);
	pdeviemsg	=  new char[_MAX_LENGTH_];
	strcpy_s(pdeviemsg , _MAX_LENGTH_ , reader.pdeviemsg);
	m_Head = reader.m_Head;
	m_Rear = reader.m_Rear;
	m_pDataBuf	= new double[MAXLENGTH];		//data buffer 
	memcpy(m_pDataBuf,reader.m_pDataBuf,sizeof(double)*MAXLENGTH);
	m_CurGetLength = reader.m_CurGetLength;
	m_pCurov = reader.m_pCurov;
	m_pSelChan = new UCHAR[_MAX_LENGTH_];
	memcpy(m_pSelChan,reader.m_pSelChan,sizeof(UCHAR)*_MAX_LENGTH_);
	m_nChansize = reader.m_nChansize;
	m_nChansize = 0;
	m_SampleInFile = reader.m_SampleInFile;
}
BCI2000FileRead::~BCI2000FileRead(void)
{
	delete perrmsg;
	delete pPath;
	delete pdeviemsg;
	delete m_pDataBuf;
	delete m_pSelChan;
}
BCI2000FileRead& BCI2000FileRead::operator=(const BCI2000FileRead& reader)
{
	m_bufsize = reader.m_bufsize;
	m_mode = reader.m_mode;
	m_samplerate = reader.m_samplerate;
	perrmsg		= new char[_MAX_LENGTH_];
	strcpy_s(perrmsg , _MAX_LENGTH_ , reader.perrmsg);
	pPath		= new char[_MAX_LENGTH_];
	strcpy_s(pPath, _MAX_LENGTH_ , reader.pPath);
	pdeviemsg	=  new char[_MAX_LENGTH_];
	strcpy_s(pdeviemsg , _MAX_LENGTH_ , reader.pdeviemsg);
	m_Head = reader.m_Head;
	m_Rear = reader.m_Rear;
	m_pDataBuf	= new double[MAXLENGTH];		//data buffer 
	memcpy(m_pDataBuf,reader.m_pDataBuf,sizeof(double)*MAXLENGTH);
	m_CurGetLength = reader.m_CurGetLength;
	m_pCurov = reader.m_pCurov;
	m_pSelChan = new UCHAR[_MAX_LENGTH_];
	memcpy(m_pSelChan,reader.m_pSelChan,sizeof(UCHAR)*_MAX_LENGTH_);
	m_nChansize = reader.m_nChansize;
	m_SampleInFile = reader.m_SampleInFile;
	return *this;
}
BOOL BCI2000FileRead::EB_OpenDevice()				//open the file,if open failed return false.
{	
	if(pPath==NULL)
		return FALSE;
	CFileFind finder;
	Stringoper oper;
	BOOL bfinding = finder.FindFile(oper.chartocstring(pPath));
	while(bfinding)
	{
		bfinding = finder.FindNextFile();
		m_nDataFiles.push_back(finder.GetFileName());
	}
	CString npath = ExtractFilename(oper.chartocstring(pPath),' ');
	if(m_nDataFiles.size()>0)
	{
		Stringoper oper;
		m_File.Open(oper.tostring(npath+m_nDataFiles.at(0)).c_str());
		m_nDataFiles.erase(m_nDataFiles.begin());
	}
	if(!m_File.IsOpen())
	{
		TRACE(_T("Can't open the file"));
		return FALSE;
	}
	m_ChannelNum = m_File.SignalProperties().Channels();
	return TRUE;
}
BOOL BCI2000FileRead::EB_CloseDevice()
{	
	m_bExit = TRUE;
	if(m_File.IsOpen())
		m_File.Open(NULL);
	return TRUE;
}
BOOL BCI2000FileRead::EB_GetData(double *data,ULONG size,OVERLAPPED *ov)
{
	if(NULL==data||ov==NULL||size<=0)
		return FALSE;
	m_bufsize = size;
	m_pCurov = ov;
	int length = (MAXLENGTH+m_Rear-m_Head)%MAXLENGTH;
	if(length<size)
	{
		SetEvent(BCI2000File_HANDLE);
		ResetEvent(ov->hEvent);
		return FALSE;
	}
	else
	{
		for(int sample = 0;sample<size;sample++)
		{
			data[sample] = m_pDataBuf[m_Head++];
			m_Head = m_Head%MAXLENGTH;
		}
		length = (MAXLENGTH+m_Rear-m_Head)%MAXLENGTH;
		if(length<size)
			SetEvent(BCI2000File_HANDLE);
		return TRUE;
	}
}
BOOL BCI2000FileRead::EB_SetBufferSize(WORD bufsize)
{
	return TRUE;
}
BOOL BCI2000FileRead::EB_SetSampleRate(WORD SampleRate)
{
	m_samplerate = SampleRate;
	return TRUE;
}
BOOL BCI2000FileRead::EB_SetMode(int mode)
{
	return TRUE;
}
BOOL BCI2000FileRead::EB_Startup()
{
	//创建线程并启动
	if(m_pThread==NULL)
	{
		m_pThread = new CWinThread(DataProcess,(LPVOID)this);
		m_pThread->CreateThread(CREATE_SUSPENDED);
		m_pThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		BCI2000File_HANDLE = CreateEvent(NULL,TRUE,TRUE,NULL);
	}
	m_pThread->ResumeThread();
	return TRUE;
}
BOOL BCI2000FileRead::EB_Stop()
{
	//暂停线程
	if(m_pThread!=NULL)
		m_pThread->SuspendThread();
	return TRUE;
}
BOOL BCI2000FileRead::EB_GetMode(int *mode)
{
	*mode = 0;
	return TRUE;
}
BOOL BCI2000FileRead::EB_GetLastError(char* errormsg)
{
	if(perrmsg==NULL)
		return FALSE;
	strcpy_s(errormsg,sizeof(char)*_MAX_LENGTH_,perrmsg);
	return TRUE;
}
BOOL BCI2000FileRead::EB_GetInfo(Device_info *device)
{
	if(NULL==pdeviemsg||device==NULL)
		return FALSE;
	strcpy_s(device->info,_MAX_LENGTH_,pdeviemsg);
	return TRUE;
}
BOOL BCI2000FileRead::EB_SetChannel(UCHAR* Channals,int Chansize)
{
	if(Chansize<=0||Channals==NULL)
		return FALSE;
	for (int i=0;i<Chansize;i++)
	{
		m_pSelChan[i] = Channals[i];
	}
	m_nChansize = Chansize;
	return TRUE;
}

BOOL BCI2000FileRead::ExtracetDatatoBuf( int size)
{
	if(size<=0)
		return FALSE;
	//size = min(size,m_File.NumSamples()-m_SampleInFile-1);
	if(size>m_File.NumSamples()-m_SampleInFile-1)
	{
		m_File.Open(NULL);
		if(m_nDataFiles.size()>0)
		{
			Stringoper oper;
			CString npath = ExtractFilename(oper.chartocstring(pPath),' ');
			m_File.Open(oper.tostring(npath+m_nDataFiles.at(0)).c_str());
			m_nDataFiles.erase(m_nDataFiles.begin());
			m_SampleInFile = 0;
		}
	}
	for (int i=0;i<size;i++)
	{
		for (int j=0;j<m_nChansize;j++)
		{
			int chan = (int)m_pSelChan[j];
			m_pDataBuf[m_Rear++] = m_File.RawValue(chan,m_SampleInFile);
			m_Rear = m_Rear%MAXLENGTH;
		}
		m_SampleInFile++;
	}
	if((m_Rear+MAXLENGTH-m_Head)%MAXLENGTH>=m_bufsize)
		ResetEvent(BCI2000File_HANDLE);
	else
		SetEvent(BCI2000File_HANDLE);
	if((m_Rear+MAXLENGTH-m_Head)%MAXLENGTH>=m_bufsize)
	{
		SetEvent(m_pCurov->hEvent);
	}
	return TRUE;
}

CString BCI2000FileRead::ExtractFilename(CString name , char c)
{
	name.TrimLeft();
	name.TrimRight();
	if(name.GetLength()<=0)
		return _T("");
	return name.Left(name.GetLength()-5);
}
//extern interface
extern "C" _declspec(dllexport) BasicDevice* CreateObject(void *p)
{
	return new BCI2000FileRead((char*)p);
}