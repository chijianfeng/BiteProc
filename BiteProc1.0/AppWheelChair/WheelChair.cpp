// WheelChair.cpp : 实现文件
//

#include "stdafx.h"
#include "WheelChair.h"
#include "afxdialogex.h"
#include "Stringoper.h"
#include "WconfigDlg.h"
#include "Digistring.h"
#include "resource.h"
// CWheelChair 对话框

IMPLEMENT_DYNAMIC(CWheelChair, CDialogEx)
CWheelChair::CWheelChair(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWheelChair::IDD, pParent)
{
      pPort = gcnew SerialPort::Serialport();
	  m_bSrtarting = FALSE;
	  m_nUpMaxSpeed = m_nRightMaxSpeed = 50;
	  m_nLeftMaxSpeed = m_nDownMaxSpeed = -50;
}

CWheelChair::~CWheelChair()
{
}

void CWheelChair::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_nRun);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_nleftRight);
	DDX_Control(pDX, IDC_BWCMOVE, m_nBmove);
	DDX_Control(pDX, IDC_BZYYT, m_nBzyyt);
	DDX_Control(pDX, IDC_BKBYT, m_nBkbyt);
	DDX_Control(pDX, IDC_LISTINFO, m_nInfoList);
	DDX_Control(pDX , IDC_STATIC_LEFTRIGHT , m_nLeftRightdigtal);
	DDX_Control(pDX , IDC_STATIC_UPDOWN , m_nUpDowndigtal);
}


BEGIN_MESSAGE_MAP(CWheelChair, CDialogEx)
	ON_BN_CLICKED(IDC_BSTART, &CWheelChair::OnBnClickedBstart)
	ON_WM_CREATE()
	//ON_MESSAGE(MY_WM_WCUPDATE, &CWheelChair::OnUpDateMsg)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR2, &CWheelChair::OnNMThemeChangedRun)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR1, &CWheelChair::OnNMThemeChangedLR)
	ON_BN_CLICKED(IDC_BFINISH, &CWheelChair::OnBnClickedBfinish)
	ON_BN_CLICKED(IDC_BWCMOVE, &CWheelChair::OnBnClickedBwcmove)
	ON_BN_CLICKED(IDC_BZYYT, &CWheelChair::OnBnClickedBzyyt)
	ON_BN_CLICKED(IDC_BKBYT, &CWheelChair::OnBnClickedBkbyt)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTINFO, &CWheelChair::OnLvnItemchangedListinfo)
	//ON_MESSAGE(MY_WM_CONTROLCMD , &CWheelChair::OnReceiveCmd)
	ON_WM_TIMER()
	ON_MESSAGE(WM_HOTKEY , OnHotKey)
	ON_BN_CLICKED(IDC_BSET, &CWheelChair::OnBnClickedBset)
END_MESSAGE_MAP()

// CWheelChair 消息处理程序


void CWheelChair::OnBnClickedBstart()
{
	//open the serial port and start the thread.
	OpenPort();
}



void CWheelChair::ClosePort()
{
	
}

//private function

void CWheelChair::OpenPort()
{
	Stop();
}

vector<string> CWheelChair::splitEx(const string& src, string separate_character)   
{   
	vector<string> strs;   

	int separate_characterLen = separate_character.size();//分割字符串的长度,这样就可以支持如“,,”多字符串的分隔符   
	int lastPosition = 0,index = -1;   
	while (-1 != (index = src.find(separate_character,lastPosition)))   
	{   
		strs.push_back(src.substr(lastPosition,index - lastPosition));   
		lastPosition = index + separate_characterLen;   
	}   
	string lastString = src.substr(lastPosition);//截取最后一个分隔符后的内容   
	if (!lastString.empty())   
		strs.push_back(lastString);//如果最后一个分隔符后还有内容就入队   
	return strs;   
}   

int CWheelChair::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

LRESULT CWheelChair::OnUpDateMsg(WPARAM wParam , LPARAM lParam)
{
	m_nInfoList.InsertItem(0 , m_nMsg);
	return 0;
}

void CWheelChair::OnNMThemeChangedRun(NMHDR *pNMHDR, LRESULT *pResult)
{
	int pos = m_nRun.GetScrollPos();
	if(pos>100) pos = 100;
	if(pos<-100) pos = -100;
	*pResult = 0;
}


void CWheelChair::OnNMThemeChangedLR(NMHDR *pNMHDR, LRESULT *pResult)
{
	int pos = m_nleftRight.GetScrollPos();
	if(pos>100) pos = 100;
	if(pos<-100) pos = -100;
	*pResult = 0;
}

void CWheelChair::OnBnClickedBfinish()
{
	GetDlgItem(IDC_BSTART)->EnableWindow(TRUE);
	pPort->POff();
	m_bSrtarting = FALSE;
}


void CWheelChair::OnBnClickedBwcmove()
{
	pPort->SetContrmode(0);
}


void CWheelChair::OnBnClickedBzyyt()
{
	
	pPort->SetContrmode(1);
}


void CWheelChair::OnBnClickedBkbyt()
{
	pPort->SetContrmode(2);
}


BOOL CWheelChair::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_nRun.SetScrollRange(-100,100);
	m_nRun.SetScrollPos(0);
	m_nleftRight.SetScrollRange(-100,100);
	m_nRun.SetScrollPos(0);
	m_nInfoList.InsertColumn(0,_T("状态"),LVCFMT_CENTER,400,-1);
	m_nCurDir = m_nPreDir = NONE;
	//::RegisterHotKey(m_hWnd, ID_HOTKEY_STOP, NULL, VK_SPACE);

	m_nLeftRightdigtal.SetBackColor(BLACK);
	m_nLeftRightdigtal.SetColor(BLACK, LIGHTGREEN);
	m_nLeftRightdigtal.SetText(_T("00"));
	m_nUpDowndigtal.SetBackColor(BLACK);
	m_nUpDowndigtal.SetColor(BLACK, LIGHTGREEN);
	m_nUpDowndigtal.SetText(_T("00"));
	return TRUE; 
}


void CWheelChair::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)//shui ping
{
	int nMin,nMax,nCurpos;
	pScrollBar->GetScrollRange(&nMin,&nMax);  //取得滚动条范围
	nCurpos=pScrollBar->GetScrollPos();          //取得当前值
	switch(nSBCode)                 //处理滚动信息
	{
	case SB_LINELEFT:                 //向左的箭头被按下
		nCurpos=nCurpos-5;break;   
	case SB_LINERIGHT:
		nCurpos++;break;    //向右的箭头被按下
	case SB_THUMBTRACK:         //鼠标拖动
		nCurpos=nPos;break;
	}
	if(nCurpos>nMax)  nCurpos=nMax;
	pScrollBar->SetScrollPos(nCurpos);  //更新信息
	int pos = nCurpos;
	if(pos<m_nLeftMaxSpeed)
		pos = m_nLeftMaxSpeed;
	if(pos>m_nRightMaxSpeed)
		pos = m_nRightMaxSpeed;
	pPort->LeftRight(pos);
	CString str;
	str.Format(_T("%d") , pos);
	m_nLeftRightdigtal.SetText(str);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CWheelChair::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)//cui zhii
{
	int nMin,nMax,nCurpos;
	pScrollBar->GetScrollRange(&nMin,&nMax);  //取得滚动条范围
	nCurpos=pScrollBar->GetScrollPos();          //取得当前值
	switch(nSBCode)                 //处理滚动信息
	{
	case SB_LINELEFT:                 //向左的箭头被按下
		nCurpos=nCurpos-5;break;   
	case SB_LINERIGHT:
		nCurpos++;break;			 //向右的箭头被按下
	case SB_THUMBTRACK:				 //鼠标拖动
		nCurpos=nPos;break;
	}
	if(nCurpos>nMax)  nCurpos=nMax;
	pScrollBar->SetScrollPos(nCurpos);  //更新信息
	int pos = nCurpos;
	if(pos>m_nUpMaxSpeed)
		pos = m_nUpMaxSpeed;
	if(pos<m_nDownMaxSpeed)
		pos = m_nDownMaxSpeed;
	pPort->Run(pos);
	CString str;
	str.Format(_T("%d") , pos);
	m_nUpDowndigtal.SetText(str);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CWheelChair::OnLvnItemchangedListinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

LRESULT CWheelChair::OnReceiveCmd(WPARAM wParam , LPARAM lParam)
{
	m_nMsg = _T("Dir:");
	if(wParam==1||wParam==2)	//left bite one
	{
		m_nCurDir = LEFT;
		m_nMsg += _T("Left");
	}
	if(wParam==5||wParam==6)   //right bite one
	{
		m_nCurDir = RIGHT;
		m_nMsg += _T("Right");
	}
	if(wParam==9||wParam==10)	//both bite one
	{
		m_nCurDir = UP;
		m_nMsg += _T("Up");
	}
	if (wParam==11)
	{
		m_nCurDir = BACK;
		m_nMsg += _T("Back");
	}
	if(wParam==12)				//stop or start
	{
		m_nCurDir = m_nPreDir = NONE;
		m_nMsg = _T("Stop/Start");
		Stop();
	}
	if(!m_bSrtarting)
		return 0;
	if(wParam==4)			//slow the speed
	{
		Slow();
		m_nMsg = _T("Slow");
	}
	if(m_nCurDir!=m_nPreDir&&m_nPreDir!=NONE)
	{
		
		if((m_nCurDir==LEFT&&m_nPreDir!=RIGHT))
		{
			if(m_nleftRight.GetScrollPos()==0)
			{	
				m_nPreDir = NONE;
				m_nMsg = _T("Change direction to Left");
			}
			else if(m_nleftRight.GetScrollPos()<0)
			{
				m_nPreDir = LEFT;
			}
			else
			{
				SlowDown(0);
				m_nMsg = _T("Slow Right to zero");
			}
		}
		else if((m_nCurDir==RIGHT&&m_nPreDir!=LEFT))
		{
			if(m_nleftRight.GetScrollPos()==0)
			{	
				m_nPreDir = NONE;
				m_nMsg = _T("Change direction to Right");
			}
			else if(m_nleftRight.GetScrollPos()>0)
			{
				m_nPreDir = RIGHT;
			}
			else
			{
				SlowDown(0);
				m_nMsg = _T("Slow Left to zero");
			}
		}
		else if(m_nCurDir==UP&&m_nPreDir!=BACK)
		{
			if(m_nRun.GetScrollPos()==0){
				m_nPreDir = NONE;
				m_nMsg = _T("Change direction to Up");
			}
			else if(m_nRun.GetScrollPos()>0)
			{
				m_nPreDir  =UP;
			}
			else
			{
				SlowDown(0);
				m_nMsg = _T("Slow back to zero");
			}
			
		}
		else if(m_nCurDir==BACK&&m_nPreDir!=UP)
		{
			if(m_nRun.GetScrollPos()==0)
			{
				m_nPreDir = NONE;
				m_nMsg = _T("Change direction to back");
			}
			else if(m_nRun.GetScrollPos()<0)
			{
				m_nPreDir  =UP;
			}
			else
			{
				SlowDown(0);
				m_nMsg = _T("Slow Up to zero");
			}
			
		}
		else
		{
				SlowDown(0);
				m_nMsg = _T("Slow to zero");
		}
	}
	if(m_nCurDir==m_nPreDir&&m_nPreDir!=NONE&&wParam!=2)
	{
		SpeedUp();
		m_nMsg = _T("Speed up");
	}
	if(m_nPreDir==NONE)				//init
	{
		switch(m_nCurDir)
		{
		case LEFT:
			OnHScroll(SB_THUMBTRACK , INITLEFTSPEED , &m_nleftRight);
			break;
		case RIGHT:
			OnHScroll(SB_THUMBTRACK , INITRIGHTSPEED , &m_nleftRight);
			break;
		case UP:
			OnVScroll(SB_THUMBTRACK , INITUPSPEED , &m_nRun);
			break;
		case BACK:
			OnVScroll(SB_THUMBTRACK , INITBACKSPEED , &m_nRun);
			break;
		default:
			break;
		}
	}
	m_nPreDir = m_nCurDir;
	//this->SendMessage(MY_WM_WCUPDATE  , 0 , 0);
	return 0;
}

void CWheelChair::Stop()
{
	m_nCurDir = m_nPreDir = NONE;
	OnHScroll(SB_THUMBTRACK , 0 , &m_nleftRight);
	OnVScroll(SB_THUMBTRACK , 0 , &m_nRun);
	if(pPort->IsRun())
	{
		GetDlgItem(IDC_BSTART)->EnableWindow(TRUE);
		pPort->POff();
		m_bSrtarting = FALSE;
	}
	else
	{
		pPort->Start();
		GetDlgItem(IDC_BSTART)->EnableWindow(FALSE);
		m_bSrtarting = TRUE;
	}
}

void CWheelChair::SlowDown(int speed)	//zero
{
	//slow down the speed in second.
	int step = 10;
	int elapse = 100;
	SetTimer(1 , elapse , NULL);
}   

void CWheelChair::SpeedUp()
{
	int speed = 0;
	switch(m_nCurDir)
	{
	case LEFT:
		speed = m_nleftRight.GetScrollPos()-SPEEDUP;
		OnHScroll(SB_THUMBTRACK , speed , &m_nleftRight);
		break;
	case RIGHT:
		speed = m_nleftRight.GetScrollPos()+SPEEDUP;
		OnHScroll(SB_THUMBTRACK , speed , &m_nleftRight);
		break;
	case UP:
		speed = m_nRun.GetScrollPos()+SPEEDUP;
		OnVScroll(SB_THUMBTRACK , speed , &m_nRun);
		break;
	case BACK:
		speed = m_nRun.GetScrollPos()-SPEEDUP;
		OnVScroll(SB_THUMBTRACK , speed , &m_nRun);
		break;
	default:
		break;
	}
}

void CWheelChair::Slow()
{
	int speed = 0;
	if(m_nCurDir==LEFT&&m_nRun.GetScrollPos()!=0)
	{
		if(m_nRun.GetScrollPos()>0)
			m_nCurDir =  UP;
		else
			m_nCurDir = BACK;
	}
	if(m_nCurDir==RIGHT&&m_nRun.GetScrollPos()!=0)
	{
		if(m_nRun.GetScrollPos()>0)
			m_nCurDir =  UP;
		else
			m_nCurDir = BACK;
	}
	switch(m_nCurDir)
	{
	case LEFT:
		speed = m_nleftRight.GetScrollPos()-SPEEDOWN;
		if(speed>0)
			speed = 0;
		OnHScroll(SB_THUMBTRACK , speed , &m_nleftRight);
		break;
	case RIGHT:
		speed = m_nleftRight.GetScrollPos()+SPEEDOWN;
		if(speed<0)
			speed = 0;
		OnHScroll(SB_THUMBTRACK , speed , &m_nleftRight);
		break;
	case UP:
		speed = m_nRun.GetScrollPos()+SPEEDOWN;
		if(speed<0)
			speed  =0;
		OnVScroll(SB_THUMBTRACK , speed , &m_nRun);
		break;
	case BACK:
		speed = m_nRun.GetScrollPos()-SPEEDOWN;
		if(speed>0)
			speed = 0;
		OnVScroll(SB_THUMBTRACK , speed , &m_nRun);
		break;
	default:
		break;
	}
	if(m_nRun.GetScrollPos()==0&&m_nleftRight.GetScrollPos()!=0)
	{
		if(m_nleftRight.GetScrollPos()<0)
			m_nCurDir =  LEFT;
		else
			m_nCurDir = RIGHT;
	}
	if(m_nRun.GetScrollPos()==0&&m_nleftRight.GetScrollPos()==0)
	{
		m_nCurDir  =NONE;
	}
}

void CWheelChair::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1)
	{
		int step = 10;
		switch(m_nCurDir)
		{
		case UP:
			if(m_nRun.GetScrollPos()+step>=0)
			{
				step = m_nRun.GetScrollPos()*(-1);
				if (m_nleftRight.GetScrollPos() == 0)
					m_nPreDir = m_nCurDir = NONE;
				else if (m_nleftRight.GetScrollPos() > 0)
				{
					m_nPreDir = m_nCurDir = RIGHT;
				}
				else
					m_nPreDir = m_nCurDir = LEFT;
				KillTimer(1);
			}
			OnVScroll(SB_THUMBTRACK , m_nRun.GetScrollPos()+step , &m_nRun);
			break;
		case BACK:
			if(m_nRun.GetScrollPos()-step<=0)
			{
				step = m_nRun.GetScrollPos();
				if (m_nleftRight.GetScrollPos()==0)
					m_nPreDir = m_nCurDir = NONE;
				else if (m_nleftRight.GetScrollPos() > 0)
				{
					m_nPreDir = m_nCurDir = RIGHT;
				}
				else
					m_nPreDir = m_nCurDir = LEFT;
				KillTimer(1);
			}
			OnVScroll(SB_THUMBTRACK , m_nRun.GetScrollPos()-step , &m_nRun);
			break;
		case LEFT:
			if(m_nleftRight.GetScrollPos()-step<=0)
			{
				step = m_nleftRight.GetScrollPos();
				if (m_nRun.GetScrollPos() == 0)
					m_nPreDir = m_nCurDir = NONE;
				else if (m_nRun.GetScrollPos() > 0)
				{
					m_nPreDir = m_nCurDir = UP;
				}
				else
					m_nPreDir = m_nCurDir = BACK;
				KillTimer(1);
			}
			OnHScroll(SB_THUMBTRACK , m_nleftRight.GetScrollPos()-step , &m_nleftRight);
			break;
		case RIGHT:
			if(m_nleftRight.GetScrollPos()+step>=0)
			{
				step = m_nleftRight.GetScrollPos()*(-1);
				if (m_nRun.GetScrollPos()==0)
					m_nPreDir = m_nCurDir = NONE;
				else if (m_nRun.GetScrollPos() > 0)
				{
					m_nPreDir = m_nCurDir = UP;
				}
				else
					m_nPreDir = m_nCurDir = BACK;
				KillTimer(1);
			}
			OnHScroll(SB_THUMBTRACK , m_nleftRight.GetScrollPos()+step , &m_nleftRight);
			break;
		default:
			break;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

HRESULT CWheelChair::OnHotKey(WPARAM wParam , LPARAM lParam)
{
	m_nCurDir = m_nPreDir = NONE;
	if(pPort->IsRun())
	{
		GetDlgItem(IDC_BSTART)->EnableWindow(TRUE);
		pPort->POff();
	}
	AfxMessageBox(_T("stop"));
	OnHScroll(SB_THUMBTRACK,0 , &m_nleftRight);
	OnVScroll(SB_THUMBTRACK,0 , &m_nRun);
	return 0;
}


void CWheelChair::OnBnClickedBset()
{
	CWconfigDlg *pDlg = new CWconfigDlg();
	if(pDlg->DoModal()==IDOK)
	{
		m_nDownMaxSpeed = pDlg->GetDownMaxSpeed();
		m_nUpMaxSpeed = pDlg->GetUpMaxSpeed();
		m_nLeftMaxSpeed = pDlg->GetLeftMaxSpeed();
		m_nRightMaxSpeed = pDlg->GetRightMaxSpeed();
	}
	delete pDlg;
}



