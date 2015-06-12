#include"stdafx.h"
#include"NumberShow.h"
#include"AppNumbers.h"
#include"ExtWndShadow.h"

extern  CAppNumbersApp theApp;

CString myStr[36] = 
{
	_T("1"), _T("2"), _T("3"), _T("4"), _T("5"), 
	_T("6"), _T("7"), _T("8"), _T("9"), _T("0"),
	_T("") , _T("") , _T("") , _T("") , _T("") ,
	_T("") , _T("") , _T("") , _T("") , _T("") ,
	_T("") , _T("") , _T("") , _T("") , _T("") ,
	_T("") , _T("") , _T("") , _T("") , _T("") ,
	_T("") , _T("") , _T("") , _T("") , _T("") ,
	_T("")
};

CString Hit_msg[6][6] = {				//提示咬牙对应数字
	_T("左轻咬"), _T("左重咬"), _T("左边两次咬"), _T("左长咬"), 
	_T("右轻咬"), _T("右重咬"),_T("右边两次咬"), _T("右长咬"),
	_T("两边轻咬"), _T("两边重咬"),_T(""), _T(""),
    _T(""), _T(""),_T(""), _T(""), _T(""), _T(""), 
	_T(""), _T(""), _T(""), _T(""), _T(""), _T(""),
	_T(""), _T(""), _T(""), _T(""), _T(""), _T(""),
	_T(""), _T(""), _T(""), _T(""), _T(""), _T("")
};
#define TIMER_FOR_LARGESHOWNUMBERS 1
#define TIMER_FOR_SMALLSHOWNUMBERS 2
#define TIMER_NUMBERS 3
#define TIMER_FOR_SHOWTASKWND 4
#define TIMER_FOR_POPNUMBERS 5
#define TIMER_SHOWTASKWND 6

IMPLEMENT_DYNAMIC(CNumberShow,CWnd)

CNumberShow::CNumberShow()
{

}

CNumberShow::~CNumberShow()
{

}

void CNumberShow::SendAppCmd(size_t cmd)
{
	UINT msg = 0;
	switch (cmd)
	{
	case LEFTONELIGHTBITE:					//1键
		msg = 0x31;
		break;
	case LEFTONEHEAVYBITE:					//2键
		msg = 0x32;
		break;
	case LEFTTWOBITE:					    //3键
		msg = 0x33;
		break;
	case LEFTLONGBITE:
		msg = 0x34;
		break;
	case RIGHTONELIGHTBITE:
		msg = 0x35;
		break;
	case RIGHTONEHEAVYBITE:
		msg = 0x36;
		break;
	case RIGHTTWOBITE:
		msg = 0x37;
		break;
	case RIGHTLONGBITE:
		msg = 0x38;
		break;
	case BOTHONELIGHTBITE:
		msg = 0x39;					
		break;
	case BOTHONEHEAVYBITE:
		msg = 0x30;
		break;
	default:
		break;
	}
	GetParent()->PostMessage(WM_KEYDOWN, msg, 0);

}

void CNumberShow::SetWndSize(size_t width,size_t height)
{
	m_Width = width;
	m_Height = height;
}

void CNumberShow::SetLogOut(void(*pFunc)(CString))
{
	m_pOutPut = pFunc;
}

BEGIN_MESSAGE_MAP(CNumberShow, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_HIDEHINT, OnHindHint)
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_SETTASK, OnShowSettingTask)
	ON_REGISTERED_MESSAGE(theApp.ID_MENU_SHOWHINT, OnShowHint)
END_MESSAGE_MAP()

void CNumberShow::ShowNumbersWndsAnimation()
{
	CRect rect;
	rect.left   = rect.top = 0;
	rect.right  = m_Width;
	rect.bottom = m_Height;
	this->MoveWindow(rect);

	for (int i = 0; i < 6; i++)
	{
		if (i < 3)
			m_NumberFrame[i].rect = CRect(rect.left + rect.Width() / 3 * i, rect.top,
			rect.left + rect.Width() / 3 * (i + 1), rect.top + rect.Height() / 2);
		else
			m_NumberFrame[i].rect = CRect(rect.left + rect.Width() / 3 * (i % 3), rect.top + rect.Height() / 2,
			rect.left + rect.Width() / 3 * (i % 3 + 1), rect.bottom);
		CRect tmprect;
		CRect rct = m_NumberFrame[i].rect;
		if (m_NumberFrame[i].pNumberWnd == NULL)
		{
			m_NumberFrame[i].pNumberWnd = new CNumberWnd[6];
			for (int j = 0; j < 6; j++)
			{
				tmprect = CRect(rct.left, rct.top, rct.left + rct.Width() / 3, rct.top + rct.Height() / 2);
				CRect r;
				if (j < 3)
					r = CRect(rct.left + rct.Width() / 3 * j, rct.top, rct.left + rct.Width() / 3 * (j + 1),
					rct.top + rct.Height() / 2);
				else
					r = CRect(rct.left + rct.Width() / 3 * (j % 3), rct.top + rct.Height() / 2,
					rct.left + rct.Width() / 3 * (j % 3 + 1), rct.bottom);
				tmprect.InflateRect(-1, -1);
				m_NumberFrame[i].pNumberWnd[j].SetNumbers(m_NumberFrame[i].str[j]);
				m_NumberFrame[i].pNumberWnd[j].Create(
					NULL,
					NULL,
					WS_CHILD | WS_VISIBLE,
					tmprect,
					this,
					m_NumberFrame[i].ID[j]);
				m_WndRange[i][j].width = tmprect.Width();
				m_WndRange[i][j].height = tmprect.Height();
				m_WndRange[i][j].m_Curx = rct.left;
				m_WndRange[i][j].m_Cury = rct.top;
				//计算运行轨迹，便于动画呈现
				::LineDDA(rct.left, rct.top, r.left, r.top, LineDDAproc, (LPARAM)(LPVOID)(&m_WndRange[i][j]));
							
			}
			if (i == 5)			//创建显示已打字符窗口
			{
				m_pShowNumbers = new CNumberRect();
				m_pShowNumbers->Create(IDD_NUMBERRECT, this);
				m_pShowNumbers->SetWndRect(m_NumberFrame[5].rect);
				m_pShowNumbers->ShowWindow(SW_SHOW);

				SetTimer(TIMER_NUMBERS, 15, NULL);				//启动定时器，开启动画
			}
		}
		else
		{
			for (int j = 0; j < 6; j++)
			{
				m_NumberFrame[i].pNumberWnd[j].SetNumbers(m_NumberFrame[i].str[j]);
				m_NumberFrame[i].pNumberWnd[j].MoveWindow(m_WndRange[i][j].m_Curx, m_WndRange[i][j].m_Cury
					, m_WndRange[i][j].width, m_WndRange[i][j].height);
			}
			if (i == 5)
				m_pShowNumbers->SetWndRect(m_NumberFrame[5].rect);
		}
	}
}

void CNumberShow::ShowNumbersWnds()
{
	CRect rect;
	rect.left = rect.top = 0;
	rect.right = m_Width;
	rect.bottom = m_Height;

	for (int i = 0; i < 6; i++)
	{

		if (i < 3)						//分成上下两行，每行三组
			m_NumberFrame[i].rect = CRect(rect.left + rect.Width() / 3 * i, rect.top,
			rect.left + rect.Width() / 3 * (i + 1), rect.top + rect.Height() / 2);
		else
			m_NumberFrame[i].rect = CRect(rect.left + rect.Width() / 3 * (i % 3), rect.top + rect.Height() / 2,
			rect.left + rect.Width() / 3 * (i % 3 + 1), rect.bottom);
		CRect tmprect;
		CRect rct = m_NumberFrame[i].rect;
		for (int j = 0; j < 6; j++)				//更新位置，分为上下两组，
		{
			if (j < 3)							//第一组
				tmprect = CRect(rct.left + rct.Width() / 3 * j, rct.top,
				rct.left + rct.Width() / 3 * (j + 1), rct.top + rct.Height() / 2);
			else//第二组位置
				tmprect = CRect(rct.left + rct.Width() / 3 * (j % 3), rct.top + rct.Height() / 2,
				rct.left + rct.Width() / 3 * (j % 3 + 1), rct.bottom);
			tmprect.InflateRect(-1, -1);
			m_NumberFrame[i].pNumberWnd[j].SetNumbers(m_NumberFrame[i].str[j]);			//设置显示的字符
			m_NumberFrame[i].pNumberWnd[j].MoveWindow(tmprect);
		}
	}
	m_pShowNumbers->SetWndRect(m_NumberFrame[5].rect);
}

void CNumberShow::ShowTaskWnd()
{
	CRect rect = CRect(m_NumberFrame[0].rect.left, m_NumberFrame[0].rect.top, m_NumberFrame[1].rect.right, m_NumberFrame[1].rect.bottom);
	if (m_pTaskNumberwnd == NULL)
	{
		//创建打字符任务窗口
		m_pTaskNumberwnd = new CTaskNumbers(this);
		rect.InflateRect(-8, -8);
		m_pTaskNumberwnd->Create(IDD_TaskNumbers, this);
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
	m_pTaskNumberwnd->SetWndRect(rect);
	CDC *pDC = GetDC();
	CExtWndShadow Shadow;				//显示阴影效果
	Shadow.Paint(*pDC, rect, 8, CExtWndShadow::DEF_BRIGHTNESS_MIN,
		CExtWndShadow::DEF_BRIGHTNESS_MAX, FALSE);
	ReleaseDC(pDC);
}

void CNumberShow::UpdateNumbersWnds()
{

}

VOID CALLBACK CNumberShow::LineDDAproc(int x, int y, LPARAM lparam)
{
	CWndMoveRange *pwnd = (CWndMoveRange*)lparam;
	pwnd->X[pwnd->Count] = x;
	pwnd->Y[(pwnd->Count)++] = y;
}

void CNumberShow::PopNumbers()
{
	if (m_strBuf.GetLength() <= 0)
		return;
	m_strBuf = m_strBuf.Left(m_strBuf.GetLength()-1);
	return; 
}

CString CNumberShow::GetNumbers()
{
	return m_strBuf;
}

CString CNumberShow::GetTaskNumbers()
{
	return m_strTaskBuf;
}

int CNumberShow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_bIsCreate = false;
	//TODO:初始化构建代码
	for (int i = 0; i < 6; i++)
	{
		m_NumberFrame[i].rect = CRect(0,0,0,0);
		m_NumberFrame[i].pNumberWnd = NULL;
		for (int j = 0; j < 6; j++)
		{
			m_NumberFrame[i].ID[j] = 0x0FFFF + i + j;
			m_NumberFrame[i].str[j] = myStr[i * 6 + j];
			m_WndRange[i][j].Count = 0;
			m_WndRange[i][j].ptr = 0;
		}
	}
	return 0;
}

void CNumberShow::OnDestroy()
{
	__super::OnDestroy();
	/*释放内存资源*/
	for (int i = 0; i < 6; i++)
	{
		if (m_NumberFrame[i].pNumberWnd != NULL)
		{
			for (int j = 0; j < 6; j++)
			{
				m_NumberFrame[i].pNumberWnd[j].DestroyWindow();
			}
		}
	}
}

void CNumberShow::OnPaint()
{
	CPaintDC dc(this);
	if (!m_bIsCreate)
		ShowNumbersWndsAnimation();
	else
		ShowNumbersWnds();
}

void CNumberShow::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_NUMBERS)											//显示数字窗口动画
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
		for (int i = 0; i < 6; i++)						//统计是否每个数字窗口都更新完毕
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
			KillTimer(TIMER_NUMBERS);						//关闭定时器
			m_bIsCreate = true;							//动画效果
			GetParent()->Invalidate(TRUE);				//删除阴影
			//SendMessage(WM_PAINT);
		}
	}
	if (nIDEvent == TIMER_FOR_LARGESHOWNUMBERS)			//大窗口方式显示数字窗口
	{
		int p = m_WndShowNumbers.ptr;
		CRect rect;
		GetClientRect(rect);
		int disx = m_NowShowNumbersRect.Width() - m_PreShowNumbersRect.Width();
		int disy = m_NowShowNumbersRect.Height() - m_PreShowNumbersRect.Height();
		if (p < m_WndShowNumbers.Count)
		{
			m_WndShowNumbers.m_Curx = m_WndShowNumbers.X[p];
			m_WndShowNumbers.m_Cury = m_WndShowNumbers.Y[p];
			m_WndShowNumbers.width = m_PreShowNumbersRect.Width() + disx*p / m_WndShowNumbers.Count;
			m_WndShowNumbers.height = m_PreShowNumbersRect.Height() + disy*p / m_WndShowNumbers.Count;
			m_pShowNumbers->SetWndRect(CRect(m_WndShowNumbers.m_Curx, m_WndShowNumbers.m_Cury, m_WndShowNumbers.m_Curx + m_WndShowNumbers.width,
				m_WndShowNumbers.height + m_WndShowNumbers.m_Cury));
			m_WndShowNumbers.ptr = p + 80;
		}
		else
		{
			p = m_WndShowNumbers.Count - 1;
			m_WndShowNumbers.m_Curx = m_WndShowNumbers.X[p];
			m_WndShowNumbers.m_Cury = m_WndShowNumbers.Y[p];
			m_WndShowNumbers.width = m_NowShowNumbersRect.Width();
			m_WndShowNumbers.height = m_NowShowNumbersRect.Height();
			m_pShowNumbers->SetWndRect(CRect(m_WndShowNumbers.m_Curx, m_WndShowNumbers.m_Cury, m_WndShowNumbers.m_Curx + m_WndShowNumbers.width,
				m_WndShowNumbers.height + m_WndShowNumbers.m_Cury));
			KillTimer(TIMER_FOR_LARGESHOWNUMBERS);
		}
	}
	if (nIDEvent == TIMER_FOR_SMALLSHOWNUMBERS)				//小窗口方式显示数字窗口
	{
		int p = m_WndShowNumbers.ptr;
		int disx = m_NowShowNumbersRect.Width() - m_PreShowNumbersRect.Width();
		int disy = m_NowShowNumbersRect.Height() - m_PreShowNumbersRect.Height();
		if (p < m_WndShowNumbers.Count)
		{
			m_WndShowNumbers.m_Curx = m_WndShowNumbers.X[p];
			m_WndShowNumbers.m_Cury = m_WndShowNumbers.Y[p];
			m_WndShowNumbers.width = m_PreShowNumbersRect.Width() + disx*p / m_WndShowNumbers.Count;
			m_WndShowNumbers.height = m_PreShowNumbersRect.Height() + disy*p / m_WndShowNumbers.Count;
			m_pShowNumbers->SetWndRect(CRect(m_WndShowNumbers.m_Curx, m_WndShowNumbers.m_Cury, m_WndShowNumbers.m_Curx + m_WndShowNumbers.width,
				m_WndShowNumbers.height + m_WndShowNumbers.m_Cury));
			m_WndShowNumbers.ptr = p + 80;
		}
		else
		{
			p = m_WndShowNumbers.Count - 1;
			m_WndShowNumbers.m_Curx = m_WndShowNumbers.X[p];
			m_WndShowNumbers.m_Cury = m_WndShowNumbers.Y[p];
			m_WndShowNumbers.width = m_NowShowNumbersRect.Width();
			m_WndShowNumbers.height = m_NowShowNumbersRect.Height();
			m_pShowNumbers->SetWndRect(m_NumberFrame[5].rect);				//更新窗口位置
			KillTimer(TIMER_FOR_SMALLSHOWNUMBERS);
		}
	}
	if (nIDEvent == TIMER_FOR_SHOWTASKWND)					//显示任务窗口动画
	{
		int p = m_WndTaskRange.ptr;
		CRect tmprect;
		if (p < m_WndTaskRange.Count)
		{
			tmprect = CRect(m_NumberFrame[0].rect.left, m_NumberFrame[0].rect.top, m_WndTaskRange.X[p], m_WndTaskRange.Y[p]);
			m_pTaskNumberwnd->SetWndRect(tmprect);
			m_WndTaskRange.ptr = p + 50;					//移动步长为50
		}
		else
		{
			p = m_WndTaskRange.Count - 1;
			tmprect = CRect(m_NumberFrame[0].rect.left, m_NumberFrame[0].rect.top, m_WndTaskRange.X[p], m_WndTaskRange.Y[p]);
			m_pTaskNumberwnd->SetWndRect(tmprect);
			KillTimer(TIMER_FOR_SHOWTASKWND);
		}
	}
	if (nIDEvent == TIMER_FOR_SHOWTASKWND)
	{
		ShowTaskWnd();
		KillTimer(TIMER_FOR_SHOWTASKWND);
		PostMessage(WM_KEYDOWN, VK_NUMPAD0, 0);
		//OnRemoveHint();
	}
	if (nIDEvent == TIMER_FOR_POPNUMBERS)						//删除数字
	{
		PopNumbers();											//依次删除数字
		if (GetNumbers().GetLength() <= 0)					//删除完毕，停止
		{
			KillTimer(TIMER_FOR_POPNUMBERS);
		}
		InvalidateRect(&(m_NumberFrame[5].rect), FALSE);
	}
	__super::OnTimer(nIDEvent);
}

BOOL CNumberShow::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)							//有键盘按下
	{
		//WPARAM code = ((pMsg->lParam & 0x0F0000) >> 16) - 2;			//解码
		WPARAM code = pMsg->wParam - 0x31;
		if (code == -1)
		{
			::SendMessage(m_NumberFrame[1].pNumberWnd[3].m_hWnd, MYWM_NUMBERMSG, 1, 1);	//区块选中										
			return TRUE;
		}
		if (pMsg->wParam >= 0x31 && pMsg->wParam <= 0x39)		
		{
			::SendMessage(m_NumberFrame[code/6].pNumberWnd[code%6].m_hWnd, MYWM_NUMBERMSG, 1, 1);	//区块选中										
			return TRUE;
		}
	}
	return __super::PreTranslateMessage(pMsg);
}


LRESULT CNumberShow::OnShowHint(WPARAM wparam, LPARAM lparam)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_NumberFrame[i].pNumberWnd[j].SetHintMsg(Hit_msg[i][j]);			//设置提示符
		}
	}
	Invalidate(TRUE);
	(*m_pOutPut)(L"显示提示信息");
	return 0;
}

LRESULT CNumberShow::OnHindHint(WPARAM wapram, LPARAM lparam)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_NumberFrame[i].pNumberWnd[j].SetHintMsg(L"");			//设置提示符
		}
	}
	Invalidate(TRUE);
	(*m_pOutPut)(L"隐藏提示信息");
	return 0;
}

LRESULT CNumberShow::OnShowSettingTask(WPARAM wparam, LPARAM lparam)
{
	AfxMessageBox(L"Add Code");
	return 0;
}

extern "C" _declspec(dllexport) char* GetAppModualMsg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return "咬牙控制数字";
}

extern "C" _declspec(dllexport) CNumberShow* CreateAppClass()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CNumberShow* pClass = new CNumberShow();
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
		_T("数字应用设置"));

	ParentMenu.Detach();
	CWnd::FromHandle(Parent)->DrawMenuBar();		//强制重绘菜单
	return TRUE;
}

extern"C" _declspec(dllexport)BOOL CheckExtMenuItem(UINT id)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	if (id == theApp.ID_MENU_HIDEHINT ||
		id == theApp.ID_MENU_SETTASK ||
		id == theApp.ID_MENU_SHOWHINT)		//判断是否是菜单消息
		return TRUE;
	else
		return FALSE;
}