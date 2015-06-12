#include "StdAfx.h"
#include "DateReader.h"
#include "Stringoper.h"

HANDLE	DateReader_HANDLE;
const char PARAM_COMMENT = '%';

UINT DataProcess(LPVOID lprarm)				//数据文件读取线程
{
	DateReader *preader = (DateReader*)lprarm;
	if(preader==NULL)
		return 1;
	DWORD ret;
	while(!preader->m_bExit)
	{
		ret = WaitForSingleObject(DateReader_HANDLE,INFINITE);			//just for debug test.
		if(ret==WAIT_FAILED)
		{
			preader->EB_Stop();
			preader->EB_CloseDevice();
			AfxMessageBox(_T("Get Data Failed!"));
			return 1;
		}
		preader->ExtracetDatatoBuf(preader->m_bufsize==0?MAXLENGTH:preader->m_bufsize);
		Sleep(128);
	}
#if _DEBUG
	AfxMessageBox(_T("Read finished"));
#endif
	return 0;
}

DateReader::DateReader(void)
{
	perrmsg = new char[_MAX_LENGTH_];
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
	m_HeadSize = 0;
	m_bFileopen = false;
}

DateReader::DateReader(char* p)
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
	m_HeadSize = 0;
	m_bFileopen = false;
}

DateReader::~DateReader(void)
{
	delete perrmsg;
	delete pPath;
	delete pdeviemsg;
	delete m_pDataBuf;
	delete m_pSelChan;
}

BOOL DateReader::EB_OpenDevice()				//open the file,if open failed return false.
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
	if(m_nDataFiles.size()>0&&!m_bFileopen)
	{
		Stringoper oper;
		m_nFile.Open(npath+m_nDataFiles.at(0) , CFile::modeRead);
		m_nDataFiles.erase(m_nDataFiles.begin());
	}
	else
		return FALSE;
	if(!ReadHeader())
		return FALSE;
	m_bFileopen = true;
	return TRUE;
}

BOOL DateReader::EB_CloseDevice()
{	
	m_bExit = TRUE;
	if(m_bFileopen)
	{
		m_nFile.Close();
		m_bFileopen = false;
	}
	return TRUE;
}

BOOL DateReader::EB_GetData(double *data,ULONG size,OVERLAPPED *ov)
{
	if(NULL==data||ov==NULL||size<=0)
		return FALSE;
	m_bufsize = (WORD)size;
	m_pCurov = ov;
	size_t length = (MAXLENGTH+m_Rear-m_Head)%MAXLENGTH;
	if(length<size)
	{
		SetEvent(DateReader_HANDLE);
		ResetEvent(ov->hEvent);
		return FALSE;
	}
	else
	{
		for(size_t sample = 0;sample<size;sample++)
		{
			data[sample] = m_pDataBuf[m_Head++];
			m_Head = m_Head%MAXLENGTH;
		}
		length = (MAXLENGTH+m_Rear-m_Head)%MAXLENGTH;
		if(length<size)
			SetEvent(DateReader_HANDLE);
		return TRUE;
	}
}

BOOL DateReader::EB_SetBufferSize(WORD bufsize)
{
	return TRUE;
}
BOOL DateReader::EB_SetSampleRate(WORD SampleRate)
{
	m_samplerate = SampleRate;
	return TRUE;
}
BOOL DateReader::EB_SetMode(int mode)
{
	return TRUE;
}
BOOL DateReader::EB_Startup()
{
	//创建线程并启动
	if(m_pThread==NULL)
	{
		m_pThread = new CWinThread(DataProcess,(LPVOID)this);
		m_pThread->CreateThread(CREATE_SUSPENDED);
		m_pThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		DateReader_HANDLE = CreateEvent(NULL,TRUE,TRUE,NULL);
	}
	m_pThread->ResumeThread();
	return TRUE;
}

BOOL DateReader::EB_Stop()
{
	//暂停线程
	if(m_pThread!=NULL)
		m_pThread->SuspendThread();
	return TRUE;
}
BOOL DateReader::EB_GetMode(int *mode)
{
	*mode = 0;
	return TRUE;
}

BOOL DateReader::EB_GetLastError(char* errormsg)
{
	if(perrmsg==NULL)
		return FALSE;
	strcpy_s(errormsg,sizeof(char)*_MAX_LENGTH_,perrmsg);
	return TRUE;
}
BOOL DateReader::EB_GetInfo(Device_info *device)
{
	if(NULL==pdeviemsg||device==NULL)
		return FALSE;
	strcpy_s(device->info,_MAX_LENGTH_,pdeviemsg);
	return TRUE;
}

BOOL DateReader::EB_SetChannel(UCHAR* Channals,int Chansize)
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

BOOL DateReader::ExtracetDatatoBuf( int size)
{
	if(size<=0)
		return FALSE;
	//size = min(size,m_File.NumSamples()-m_SampleInFile-1);
	ULONGLONG length = (m_nFile.GetLength()-m_HeadSize)/sizeof(double);
	if(size>length/m_nChansize-m_SampleInFile-1)
	{
		m_nFile.Close();
		m_bFileopen = false;
		if(m_nDataFiles.size()>0)
		{
			Stringoper oper;
			CString npath = ExtractFilename(oper.chartocstring(pPath),' ');
			m_nFile.Open(npath+m_nDataFiles.at(0)  , CFile::modeRead);
			m_nDataFiles.erase(m_nDataFiles.begin());
			ReadHeader();
			m_SampleInFile = 0;
			m_bFileopen = true;
		}
		else
		{
			m_bExit = true;
			return FALSE;
		}
	}
	double* pd = new double[size*m_nChansize];
	int ret = m_nFile.Read(pd , size*m_nChansize*sizeof(double));
	if(ret==0)
	{
		m_nFile.Close();
		m_bFileopen = false;
		if(m_nDataFiles.size()>0)
		{
			Stringoper oper;
			CString npath = ExtractFilename(oper.chartocstring(pPath),' ');
			m_nFile.Open(npath+m_nDataFiles.at(0)  , CFile::modeRead);
			m_nDataFiles.erase(m_nDataFiles.begin());
			ReadHeader();
			m_SampleInFile = 0;
			m_bFileopen = true;
			return TRUE;
		}
		else
		{
			m_bExit = true;
			return FALSE;
		}
	}
	else if(ret!=0&&ret/sizeof(double)!=size*m_nChansize)
	{
		delete pd;
		SetEvent(DateReader_HANDLE);
		return FALSE;
	}
	for (int i=0;i<size;i++)
	{
		for (int j=0;j<m_nChansize;j++)
		{
			int chan = (int)m_pSelChan[j];
			m_pDataBuf[m_Rear++] = pd[i*m_nChansize+chan-1];
			m_Rear = m_Rear%MAXLENGTH;
		}
		m_SampleInFile++;
	}
	delete pd;
	if((m_Rear+MAXLENGTH-m_Head)%MAXLENGTH>=m_bufsize)
		ResetEvent(DateReader_HANDLE);
	else
		SetEvent(DateReader_HANDLE);
	if((m_Rear+MAXLENGTH-m_Head)%MAXLENGTH>=m_bufsize)
	{
		SetEvent(m_pCurov->hEvent);
	}
	return TRUE;
}

CString DateReader::ExtractFilename(CString name , char c)
{
	name.TrimLeft();
	name.TrimRight();
	if(name.GetLength()<=0)
		return _T("");
	return name.Left(name.GetLength()-5);
}

//private function
bool DateReader::ReadHeader()
{
	m_nFile.SeekToBegin();
	m_HeadSize = 0;
	char tmpbuf[512];
	string str;
	m_nFile.Read(tmpbuf , 512);
	int len = Readline(tmpbuf , 512 , str);
	string key , v;
	Stringoper oper;
	AnalyseLine(str , key , v);
	if(key.compare("SampleRate")==0)
	{
		m_samplerate = oper.stringtoint(v);
	}
	m_nFile.Seek(len , CFile::begin);
	m_HeadSize += len;
	m_nFile.Read(tmpbuf , 512);
	len  = Readline(tmpbuf , 512 , str);
	AnalyseLine(str , key , v);
	if(key.compare("Channum")==0)
	{
		m_nChannum = oper.stringtoint(v);
	}
	m_HeadSize += len;
	m_nFile.SeekToBegin();
	LONGLONG act = m_nFile.Seek(m_HeadSize, CFile::begin);
	return true;
}

bool DateReader::IsSpace(char c)
{
	if(' '==c||'\t'==c)
		return true;
	return false;
}
bool DateReader::IsCommentChar(char c)
{
	switch(c){
	case PARAM_COMMENT:
		return true;

	default:
		return false;
	}
}

void DateReader::TrimString(string& str)
{
	if(str.empty())
		return;
	size_t i , start_pos , end_pos;
	for (i=0;i<str.size();i++)
	{
		if (!IsSpace(str[i]))
		{
			break;
		}
	}
	if(i==str.size())
	{
		str="";
		return;
	}
	start_pos = i;
	for (i = str.size() - 1; i >= 0; --i) {
		if (!IsSpace(str[i])) {
			break;
		}
	}
	end_pos = i;

	str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool DateReader::AnalyseLine(const string& line , string& key , string& value)
{
	if(line.empty())
		return false;
	int start_pos = 0 , end_pos = line.size()-1 , pos = 0;
	if((pos=line.find(PARAM_COMMENT))!=-1)
	{
		if(0==pos)
			return false;
		end_pos = pos - 1 ;  
	}
	string new_line = line.substr(start_pos, start_pos + 1 - end_pos); 
	if ((pos = new_line.find('=')) == -1)
		return false;  
	key = new_line.substr(0, pos);
	value = new_line.substr(pos + 1, end_pos + 1- (pos + 1));

	TrimString(key);
	if (key.empty()) {
		return false;
	}
	TrimString(value);
	return true;
}

int DateReader::Readline(char *p , int len , string& str)
{
	if(p==NULL||len<=0)
		return 0;
	str = "";
	for (int i=0;i<len;i++)
	{
		if(p[i]!='\n')
			str += p[i];
		else
		{
			return i+1;
		}
	}
	return 0;
}
//extern interface
extern "C" _declspec(dllexport) BasicDevice* CreateObject(void *p)
{
	return new DateReader((char*)p);
}