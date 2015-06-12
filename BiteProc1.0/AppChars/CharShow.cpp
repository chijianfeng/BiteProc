// CharShow.cpp : 实现文件
//

#include "stdafx.h"
#include "AppChars.h"
#include "CharShow.h"
#include "ExtWndShadow.h"

extern CAppCharsApp theApp;
// CCharShow

CString myStr[36] = {
	_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("G"), _T("H"),
	_T("I"), _T("J"), _T("K"), _T("L"), _T("M"), _T("N"), _T("O"), _T("P"),
	_T("Q"), _T("R"), _T("S"), _T("T"), _T("U"), _T("V"), _T("W"), _T("X"),
	_T("Y"), _T("Z"), _T(""), _T("_"), _T("<-")/*Enter*/, _T("Bck"),
	_T(""), _T(""), _T(""), _T(""), _T(""), _T(""),
};
CString Hit_msg[6][6] = {				//提示符命令组合
	_T("1 1"), _T("1 2"), _T("1 3"), _T("1 4"), _T("1 5"), _T("1 6"),
	_T("2 1"), _T("2 2"), _T("2 3"), _T("2 4"), _T("2 5"), _T("2 6"),
	_T("3 1"), _T("3 2"), _T("3 3"), _T("3 4"), _T("3 5"), _T("3 6"),
	_T("4 1"), _T("4 2"), _T("4 3"), _T("4 4"), _T("4 5"), _T("4 6"),
	_T("5 1"), _T("5 2"), _T("5 3"), _T("5 4"), _T("5 5"), _T("5 6"),
	_T("6 1"), _T("6 2"), _T("6 3"), _T("6 4"), _T("6 5"), _T("6 6"),
};

#define TIMER_FOR_LARGESHOWCHARS 1
#define TIMER_FOR_SMALLSHOWCHARS 2
#define TIMER_CHARS 3
#define  TIMER_FOR_SHOWTASKWND  4
#define TIMER_FOR_POPCHARS 5
#define TIMER_SHOWTASKWND 6

IMPLEMENT_DYNAMIC(CCharShow, CWnd)

CCharShow::CCharShow()
{

}

CCharShow::~CCharShow()
{
}

//实现接口

void CCharShow::SendAppCmd(size_t cmd)
{
	/*
	可直接发送到对应的子窗口中，此应用为便于调试，先映射到虚拟键盘，再发送到对应子窗口
	该应用实现6*6的打字母规则：利用左、右、同时的轻重命令，其他命令将进行其他操作
	*/
	UINT msg = 0;
	switch (cmd)
	{
	case LEFTONELIGHTBITE:					//2键
		msg = 0x31;
		break;
	case LEFTONEHEAVYBITE:					//2键
		msg = 0x32;
		break;
	case RIGHTONELIGHTBITE:					//.。
		msg = 0x33;
		break;
	case RIGHTONEHEAVYBITE:
		msg = 0x34;
		break;
	case BOTHONELIGHTBITE:
		msg = 0x35;
		break;
	case BOTHONEHEAVYBITE:
		msg = 0x36;
		break;
	case BOTHTWOBITE:
	case LEFTTWOBITE:
	case RIGHTTWOBITE:
		msg = VK_RETURN;					//回车
		break;
	case BOTHLONGBITE:
	case LEFTLONGBITE:
	case RIGHTLONGBITE:
		msg = VK_DELETE;					//删除
		break;
	default:
		break;
	}
	GetParent()->PostMessage(WM_KEYDOWN, msg, 0);
}

void CCharShow::SetWndSize(size_t windth, size_t height)
{
	m_Width = windth;
	m_Height = height;
}

void CCharShow::SetLogOut(void(*pFunc)(CString))
{
	m_pOutPut = pFunc;
}

BEGIN_MESSAGE_MAP(CCharShow, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_HIDEHINT, OnHindHint)
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_SETTASK,OnShowSettingTask)
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_SHOWHINT , OnShowHint)
END_MESSAGE_MAP()

//private function

void CCharShow::ShowCharWndsAnimation()
{
	CRect rect;
	rect.left = rect.top = 0;
	rect.right = m_Width;
	rect.bottom = m_Height;
	this->MoveWindow(rect);							//修改窗口大小

	for (int i = 0; i < 6; i++)
	{
		if (i < 3)
			m_CharFrame[i].rect = CRect(rect.left + rect.Width() / 3 * i, rect.top,
			rect.left + rect.Width() / 3 * (i + 1), rect.top + rect.Height() / 2);
		else
			m_CharFrame[i].rect = CRect(rect.left + rect.Width() / 3 * (i % 3), rect.top + rect.Height() / 2,
			rect.left + rect.Width() / 3 * (i % 3 + 1), rect.bottom);
		CRect tmprect;
		CRect rct = m_CharFrame[i].rect;
		if (m_CharFrame[i].pCharWnd == NULL)
		{
			m_CharFrame[i].pCharWnd = new CCharWnd[6];
			for (int j = 0; j < 6; j++)
			{
				tmprect = CRect(rct.left, rct.top, rct.left + rct.Width() / 3, rct.top + rct.Height() / 2);
				//tmprect = CRect(0, 0, m_CharFrame[0].rect.Width() / 3, m_CharFrame[0].rect.Height() / 2);
				CRect r;
				if (j < 3)
					r = CRect(rct.left + rct.Width() / 3 * j, rct.top, rct.left + rct.Width() / 3 * (j + 1), 
					rct.top + rct.Height() / 2);
				else
					r = CRect(rct.left + rct.Width() / 3 * (j % 3), rct.top + rct.Height() / 2,
					rct.left + rct.Width() / 3 * (j % 3 + 1), rct.bottom);
				tmprect.InflateRect(-1, -1);
				m_CharFrame[i].pCharWnd[j].SetChars(m_CharFrame[i].str[j]);
				m_CharFrame[i].pCharWnd[j].SetHintMsg(Hit_msg[i][j]);
				m_CharFrame[i].pCharWnd[j].Create(
					NULL,
					NULL,
					WS_CHILD | WS_VISIBLE,
					tmprect,
					this,
					m_CharFrame[i].ID[j]);
				m_WndRange[i][j].width = tmprect.Width();
				m_WndRange[i][j].height = tmprect.Height();
				m_WndRange[i][j].m_Curx = rct.left;
				m_WndRange[i][j].m_Cury = rct.top;
				//计算运行轨迹，便于动画呈现
				::LineDDA(rct.left, rct.top, r.left, r.top, LineDDAproc, (LPARAM)(LPVOID)(&m_WndRange[i][j]));
			}
			if (i == 5)			//创建显示已打字符窗口
			{
				m_pShowChars = new CCharsRect();
				m_pShowChars->Create(IDD_CHARSRECT, this);
				//m_pShowChars->DoModal();
				m_pShowChars->SetWndRect(m_CharFrame[5].rect);
				m_pShowChars->ShowWindow(SW_SHOW);

				SetTimer(TIMER_CHARS, 15, NULL);				//启动定时器，开启动画
			}
		}
		else
		{
			for (int j = 0; j < 6; j++)
			{
				m_CharFrame[i].pCharWnd[j].SetChars(m_CharFrame[i].str[j]);
				m_CharFrame[i].pCharWnd[j].MoveWindow(m_WndRange[i][j].m_Curx, m_WndRange[i][j].m_Cury
					, m_WndRange[i][j].width, m_WndRange[i][j].height);
			}
			if (i == 5)
				m_pShowChars->SetWndRect(m_CharFrame[5].rect);
		}
	}

}

void CCharShow::ShowCharWnds()
{
	CRect rect;
	rect.left = rect.top = 0;
	rect.right = m_Width;
	rect.bottom = m_Height;

	for (int i = 0; i < 6; i++)
	{

		if (i < 3)						//分成上下两行，每行三组
			m_CharFrame[i].rect = CRect(rect.left + rect.Width() / 3 * i, rect.top,
			rect.left + rect.Width() / 3 * (i + 1), rect.top + rect.Height() / 2);
		else
			m_CharFrame[i].rect = CRect(rect.left + rect.Width() / 3 * (i % 3), rect.top + rect.Height() / 2,
			rect.left + rect.Width() / 3 * (i % 3 + 1), rect.bottom);
		CRect tmprect;
		CRect rct = m_CharFrame[i].rect;
			for (int j = 0; j < 6; j++)				//更新位置，分为上下两组，
			{
				if (j < 3)							//第一组
					tmprect = CRect(rct.left + rct.Width() / 3 * j, rct.top,
					rct.left + rct.Width() / 3 * (j + 1), rct.top + rct.Height() / 2);
				else//第二组位置
					tmprect = CRect(rct.left + rct.Width() / 3 * (j % 3), rct.top + rct.Height() / 2,
					rct.left + rct.Width() / 3 * (j % 3 + 1), rct.bottom);
				tmprect.InflateRect(-1, -1);
				m_CharFrame[i].pCharWnd[j].SetChars(m_CharFrame[i].str[j]);			//设置显示的字符
				m_CharFrame[i].pCharWnd[j].MoveWindow(tmprect);
			}
	}
	m_pShowChars->SetWndRect(m_CharFrame[5].rect);
}

void CCharShow::UpdataCharWnds()
{

}

void CCharShow::ShowTaskWnd()
{
	CRect rect = CRect(m_CharFrame[0].rect.left, m_CharFrame[0].rect.top, m_CharFrame[1].rect.right, m_CharFrame[1].rect.bottom);
	if (m_pTaskCharwnd == NULL)
	{
		//创建打字符任务窗口
		m_pTaskCharwnd = new CTaskChars(this);
		rect.InflateRect(-8, -8);
		m_pTaskCharwnd->Create(IDD_TASKCHARS, this);
		m_WndTaskRange.m_Curx = rect.left;
		m_WndTaskRange.m_Cury = rect.top;
		m_WndTaskRange.width = rect.Width();
		m_WndTaskRange.height = rect.Height();
		//运行路径
		::LineDDA(m_WndTaskRange.m_Curx, m_WndTaskRange.m_Cury,
			m_WndTaskRange.m_Curx + m_WndTaskRange.width,
			m_WndTaskRange.m_Cury + m_WndTaskRange.height, LineDDAproc, (LPARAM)(LPVOID)(&m_WndTaskRange));
		SetTimer(TIMER_FOR_SHOWTASKWND, 10, NULL);	//动画呈现
	}
	m_pTaskCharwnd->SetWndRect(rect);
	CDC *pDC = GetDC();
	CExtWndShadow Shadow;				//显示阴影效果
	Shadow.Paint(*pDC, rect, 8, CExtWndShadow::DEF_BRIGHTNESS_MIN,
		CExtWndShadow::DEF_BRIGHTNESS_MAX, FALSE);
	ReleaseDC(pDC);
}

VOID CALLBACK CCharShow::LineDDAproc(int x, int y, LPARAM lparam)	//回调函数
{
	//add code here for store the pos.
	CWndMoveRange *pwnd = (CWndMoveRange*)lparam;
	pwnd->X[pwnd->Count] = x;
	pwnd->Y[(pwnd->Count)++] = y;
}

//extend function
void CCharShow::PopChars()
{
	if (m_strBuf.GetLength() <= 0)
		return ;
	m_strBuf = m_strBuf.Left(m_strBuf.GetLength() - 1);
	return ;
}

CString CCharShow::GetChars()
{
	return m_strBuf;
}

CString CCharShow::GetTaskChars()
{
	return m_strTaskBuf;
}

// CCharShow 消息处理程序


int CCharShow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bIsCreate = false;
	// TODO:  初始化构建的代码
	for (int i = 0; i < 6; i++)								//初始化字符窗体
	{
		m_CharFrame[i].rect = CRect(0, 0, 0, 0);
		m_CharFrame[i].pCharWnd = NULL;
		for (int j = 0; j < 6; j++)
		{
			m_CharFrame[i].ID[j] = 0x0FFFF + i + j;			//设置每个窗体ID，便于消息控制
			m_CharFrame[i].str[j] = myStr[i * 6 + j];
			m_WndRange[i][j].Count = 0;
			m_WndRange[i][j].ptr = 0;
		}

	}

	m_FirstBite = m_SecondBite = 0;



	return 0;
}


void CCharShow::OnDestroy()
{
	__super::OnDestroy();

	// TODO:  释放内存资源

	for (int i = 0; i < 6; i++)
	{
		if (m_CharFrame[i].pCharWnd != NULL)
		{
			for (int j = 0; j < 6; j++)
			{
				m_CharFrame[i].pCharWnd[j].DestroyWindow();
			}
			//delete m_CharFrame[i].pCharWnd;
		}
	}

}

void CCharShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (!m_bIsCreate)
		ShowCharWndsAnimation();
	else
	{
		ShowCharWnds();
	}
}



void CCharShow::OnTimer(UINT_PTR nIDEvent)
{
	
	if (nIDEvent == TIMER_CHARS)											//显示字符窗口动画
	{
		for (int i = 0; i < 6; i++)												//6*6 组合
		{
			for (int j = 1; j < 6; j++)
			{
				int p = m_WndRange[i][j].ptr;
				if (p < m_WndRange[i][j].Count)
				{
					m_WndRange[i][j].m_Curx = m_WndRange[i][j].X[p];
					m_WndRange[i][j].m_Cury = m_WndRange[i][j].Y[p];
					m_WndRange[i][j].ptr = p + 20;							//每次步长为20，速度较快
				}
				else
				{
					p = m_WndRange[i][j].Count - 1;
					m_WndRange[i][j].m_Curx = m_WndRange[i][j].X[p];
					m_WndRange[i][j].m_Cury = m_WndRange[i][j].Y[p];
				}
			}
		}
		int count = 0;
		for (int i = 0; i < 6; i++)						//统计是否每个字符窗口都更新完毕
		{
			for (int j = 0; j < 6; j++)
			{
				int p = m_WndRange[i][j].ptr;
				if (p >= m_WndRange[i][j].Count)
				{
					count++;
				}
			}
		}
		SendMessage(WM_PAINT);
		if (count == 36)								//每个窗口更新结束，停止定时器
		{
			KillTimer(TIMER_CHARS);						//关闭定时器
			m_bIsCreate = true;							//动画效果
			GetParent()->Invalidate(TRUE);				//删除阴影
			//SendMessage(WM_PAINT);
		}
	}
	if (nIDEvent == TIMER_FOR_LARGESHOWCHARS)			//大窗口方式显示字符窗口
	{
		int p = m_WndShowChars.ptr;
		CRect rect;
		GetClientRect(rect);
		int disx = m_NowShowCharsRect.Width() - m_PreShowCharsRect.Width();
		int disy = m_NowShowCharsRect.Height() - m_PreShowCharsRect.Height();
		if (p < m_WndShowChars.Count)
		{
			m_WndShowChars.m_Curx = m_WndShowChars.X[p];
			m_WndShowChars.m_Cury = m_WndShowChars.Y[p];
			m_WndShowChars.width = m_PreShowCharsRect.Width() + disx*p / m_WndShowChars.Count;
			m_WndShowChars.height = m_PreShowCharsRect.Height() + disy*p / m_WndShowChars.Count;
			m_pShowChars->SetWndRect(CRect(m_WndShowChars.m_Curx, m_WndShowChars.m_Cury, m_WndShowChars.m_Curx + m_WndShowChars.width,
				m_WndShowChars.height + m_WndShowChars.m_Cury));
			m_WndShowChars.ptr = p + 80;
		}
		else
		{
			p = m_WndShowChars.Count - 1;
			m_WndShowChars.m_Curx = m_WndShowChars.X[p];
			m_WndShowChars.m_Cury = m_WndShowChars.Y[p];
			m_WndShowChars.width = m_NowShowCharsRect.Width();
			m_WndShowChars.height = m_NowShowCharsRect.Height();
			m_pShowChars->SetWndRect(CRect(m_WndShowChars.m_Curx, m_WndShowChars.m_Cury, m_WndShowChars.m_Curx + m_WndShowChars.width,
				m_WndShowChars.height + m_WndShowChars.m_Cury));
			KillTimer(TIMER_FOR_LARGESHOWCHARS);
		}
	}
	if (nIDEvent == TIMER_FOR_SMALLSHOWCHARS)				//小窗口方式显示字符窗口
	{
		int p = m_WndShowChars.ptr;
		int disx = m_NowShowCharsRect.Width() - m_PreShowCharsRect.Width();
		int disy = m_NowShowCharsRect.Height() - m_PreShowCharsRect.Height();
		if (p < m_WndShowChars.Count)
		{
			m_WndShowChars.m_Curx = m_WndShowChars.X[p];
			m_WndShowChars.m_Cury = m_WndShowChars.Y[p];
			m_WndShowChars.width = m_PreShowCharsRect.Width() + disx*p / m_WndShowChars.Count;
			m_WndShowChars.height = m_PreShowCharsRect.Height() + disy*p / m_WndShowChars.Count;
			m_pShowChars->SetWndRect(CRect(m_WndShowChars.m_Curx, m_WndShowChars.m_Cury, m_WndShowChars.m_Curx + m_WndShowChars.width,
				m_WndShowChars.height + m_WndShowChars.m_Cury));
			m_WndShowChars.ptr = p + 80;
		}
		else
		{
			p = m_WndShowChars.Count - 1;
			m_WndShowChars.m_Curx = m_WndShowChars.X[p];
			m_WndShowChars.m_Cury = m_WndShowChars.Y[p];
			m_WndShowChars.width = m_NowShowCharsRect.Width();
			m_WndShowChars.height = m_NowShowCharsRect.Height();
			m_pShowChars->SetWndRect(m_CharFrame[5].rect);				//更新窗口位置
			KillTimer(TIMER_FOR_SMALLSHOWCHARS);
		}
	}
	if (nIDEvent == TIMER_FOR_SHOWTASKWND)					//显示任务窗口动画
	{
		int p = m_WndTaskRange.ptr;
		CRect tmprect;
		if (p < m_WndTaskRange.Count)
		{
			tmprect = CRect(m_CharFrame[0].rect.left, m_CharFrame[0].rect.top, m_WndTaskRange.X[p], m_WndTaskRange.Y[p]);
			m_pTaskCharwnd->SetWndRect(tmprect);
			m_WndTaskRange.ptr = p + 50;					//移动步长为50
		}
		else
		{
			p = m_WndTaskRange.Count - 1;
			tmprect = CRect(m_CharFrame[0].rect.left, m_CharFrame[0].rect.top, m_WndTaskRange.X[p], m_WndTaskRange.Y[p]);
			m_pTaskCharwnd->SetWndRect(tmprect);
			KillTimer(TIMER_FOR_SHOWTASKWND);
		}
	}
	if (nIDEvent == TIMER_SHOWTASKWND)
	{
		ShowTaskWnd();
		KillTimer(TIMER_SHOWTASKWND);
		PostMessage(WM_KEYDOWN, VK_NUMPAD0, 0);
		//OnRemoveHint();
	}
	if (nIDEvent == TIMER_FOR_POPCHARS)						//删除字母
	{
		PopChars();											//依次删除字母
		if (GetChars().GetLength() <= 0)					//删除完毕，停止
		{
			KillTimer(TIMER_FOR_POPCHARS);
		}
		InvalidateRect(&(m_CharFrame[5].rect), FALSE);
	}
	__super::OnTimer(nIDEvent);
}

BOOL CCharShow::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)							//有键盘按下
	{
		//WPARAM code = ((pMsg->lParam & 0x0F0000) >> 16)-1;	//解码 for debug
		WPARAM code = pMsg->wParam-0x30;
		if (code >= 1 && code <= 6/*&&pMsg->wParam==229*/)		//若VK键在1~6之间，进行处理
		{
			if (m_FirstBite == 0)								//字符是组合方式，判断是否第一次咬
			{
				m_FirstBite = code;									//对应到字符窗体数组
				if (m_CharFrame[m_FirstBite-1].pCharWnd == NULL)	//若不存在窗体，返回
					return TRUE;
				if (m_FirstBite == 5)							//第5组窗体分开处理，因为只有Y和Z字母
				{
					for (int i = 0; i < 2; i++)
					{
						::SendMessage(m_CharFrame[m_FirstBite-1].pCharWnd[i].GetSafeHwnd(), MYWM_CHARMSG, 1, -1);	//区块选中					
					}
				}
				else
					for (int i = 0; i < 6; i++)
					{
						::SendMessage(m_CharFrame[m_FirstBite-1].pCharWnd[i].GetSafeHwnd(), MYWM_CHARMSG, 1, -1);	//区块选中					
					}
			}
			else
			{	//第二次咬
				m_SecondBite = code-1;
				if (m_CharFrame[m_FirstBite-1].pCharWnd == NULL)	//若不存在窗体，返回
					return TRUE;
				//选中区域恢复
				if (m_FirstBite == 5)
				{
					if (m_SecondBite == 1 || m_SecondBite == 0)
					{
						::SendMessage(m_CharFrame[m_FirstBite - 1].pCharWnd[m_SecondBite].m_hWnd,
							MYWM_CHARMSG, 1, 1);		//闪烁持续一秒
						m_strBuf += myStr[(m_FirstBite - 1) * 6 + m_SecondBite];
						InvalidateRect(&m_CharFrame[5].rect, FALSE);
						for (int i = 0; i < 2; i++)
						{
							if (m_SecondBite != i)
							::SendMessage(m_CharFrame[m_FirstBite - 1].pCharWnd[i].GetSafeHwnd(),
								MYWM_CHARMSG, 0, -1);	//区块解除选中					
						}
					}
					else
						return TRUE;					//非指定区域，无效返回
				}
				else
				{
					::SendMessage(m_CharFrame[m_FirstBite - 1].pCharWnd[m_SecondBite].m_hWnd, 
						MYWM_CHARMSG, 1, 1);		//闪烁持续一秒
					m_strBuf += myStr[(m_FirstBite - 1) * 6 + m_SecondBite];		//将打出字符添加到缓存区
					InvalidateRect(&m_CharFrame[5].rect, FALSE);
					
					for (int i = 0; i < 6; i++)
					{
						if (m_SecondBite!=i)
						::SendMessage(m_CharFrame[m_FirstBite - 1].pCharWnd[i].GetSafeHwnd(), 
							MYWM_CHARMSG, 0, -1);	//区块解除选中					
					}
				}

				m_FirstBite = 0;
			}
			return TRUE;
		}

		if (pMsg->wParam == VK_RETURN)
		{
			if (m_CharFrame[4].pCharWnd == NULL)	//若不存在窗体，返回
				return TRUE;
			::SendMessage(m_CharFrame[4].pCharWnd[4].m_hWnd,
				MYWM_CHARMSG, 1, 1);		//闪烁持续一秒
			m_strBuf += L"\r\n";
			//::SendMessage(m_CharFrame[4].pCharWnd[4].m_hWnd,
			//	MYWM_CHARMSG, 0, -1);		//取消选中
			InvalidateRect(&m_CharFrame[5].rect, FALSE);
			return TRUE;
		}

		if (pMsg -> wParam == VK_DELETE)
		{
			if (m_CharFrame[4].pCharWnd == NULL)	//若不存在窗体，返回
				return TRUE;
			::SendMessage(m_CharFrame[4].pCharWnd[5].m_hWnd,
				MYWM_CHARMSG, 1, 1);		//闪烁持续一秒
			PopChars();
			//::SendMessage(m_CharFrame[4].pCharWnd[5].m_hWnd,
			//	MYWM_CHARMSG, 0, -1);		//取消选中
			InvalidateRect(&m_CharFrame[5].rect, FALSE);
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

LRESULT CCharShow::OnShowHint(WPARAM wparam, LPARAM lparam)
{
	for (int i = 0; i < 6;i++)
	{
		for (int j = 0; j < 6;j++)
		{
			m_CharFrame[i].pCharWnd[j].SetHintMsg(Hit_msg[i][j]);			//设置提示符
		}
	}
	Invalidate(TRUE);
	(*m_pOutPut)(L"显示提示信息");
	return 0;
}

LRESULT CCharShow::OnHindHint(WPARAM wapram, LPARAM lparam)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_CharFrame[i].pCharWnd[j].SetHintMsg(L"");			//设置提示符
		}
	}
	Invalidate(TRUE);
	(*m_pOutPut)(L"隐藏提示信息");
	return 0;
}

LRESULT CCharShow::OnShowSettingTask(WPARAM wparam, LPARAM lparam)
{
	AfxMessageBox(L"Add Code");
	return 0;
}

//dll 对外接口，不可变

extern "C"  _declspec(dllexport) char* GetAppModualMsg()					//获取模块信息
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return " 6*6标准，咬牙控制字符";
}

extern "C" _declspec(dllexport) CCharShow* CreateAppClass()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CCharShow* pClass = new CCharShow();
	return pClass;
}

extern "C" _declspec(dllexport) BOOL CreateAppWindow(CWnd* parent, CWnd* pViewer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());			//for MFC dll stander
	if (pViewer == NULL)
		return FALSE;
	BOOL b = pViewer->Create(NULL,
		NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 800, 800), parent, 0xFFF2);					//ID 号
	pViewer->ShowWindow(SW_SHOW);
	return b;
}


//动态添加菜单，根据应用的需要进行添加，不是必须的操
extern"C" _declspec(dllexport) BOOL	InstallExtMenu(CWnd* pChild, HWND Parent, UINT* NextMenuID)
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
	theApp.m_CommandToMessage.InitHashTable(3);		//预定3项，可修改

	NextMenuID = new UINT[3];
	NextMenuID[0] = theApp.ID_MENU_SHOWHINT;		
	NextMenuID[1] = theApp.ID_MENU_HIDEHINT;
	NextMenuID[2] = theApp.ID_MENU_SETTASK;

	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"显示提示符");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_SHOWHINT);	//设置映射
	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"隐藏提示符");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_HIDEHINT);
	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"设置任务");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_SETTASK);

	HMENU hsubMenu = Menu.Detach();
	ParentMenu.InsertMenuW(ParentMenu.GetMenuItemCount(), MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)hsubMenu,
		_T("字符应用设置"));

	ParentMenu.Detach();
	CWnd::FromHandle(Parent)->DrawMenuBar();		//强制重绘菜单
	return TRUE;
}

extern"C" _declspec(dllexport)BOOL CheckExtMenuItem(UINT id)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	if (id == theApp.ID_MENU_HIDEHINT || 
		id == theApp.ID_MENU_SETTASK||
		id==theApp.ID_MENU_SHOWHINT)		//判断是否是菜单消息
		return TRUE;
	else
		return FALSE;
}