// CharShow.cpp : ʵ���ļ�
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
CString Hit_msg[6][6] = {				//��ʾ���������
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

//ʵ�ֽӿ�

void CCharShow::SendAppCmd(size_t cmd)
{
	/*
	��ֱ�ӷ��͵���Ӧ���Ӵ����У���Ӧ��Ϊ���ڵ��ԣ���ӳ�䵽������̣��ٷ��͵���Ӧ�Ӵ���
	��Ӧ��ʵ��6*6�Ĵ���ĸ�����������ҡ�ͬʱ����������������������������
	*/
	UINT msg = 0;
	switch (cmd)
	{
	case LEFTONELIGHTBITE:					//2��
		msg = 0x31;
		break;
	case LEFTONEHEAVYBITE:					//2��
		msg = 0x32;
		break;
	case RIGHTONELIGHTBITE:					//.��
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
		msg = VK_RETURN;					//�س�
		break;
	case BOTHLONGBITE:
	case LEFTLONGBITE:
	case RIGHTLONGBITE:
		msg = VK_DELETE;					//ɾ��
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
	this->MoveWindow(rect);							//�޸Ĵ��ڴ�С

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
				//�������й켣�����ڶ�������
				::LineDDA(rct.left, rct.top, r.left, r.top, LineDDAproc, (LPARAM)(LPVOID)(&m_WndRange[i][j]));
			}
			if (i == 5)			//������ʾ�Ѵ��ַ�����
			{
				m_pShowChars = new CCharsRect();
				m_pShowChars->Create(IDD_CHARSRECT, this);
				//m_pShowChars->DoModal();
				m_pShowChars->SetWndRect(m_CharFrame[5].rect);
				m_pShowChars->ShowWindow(SW_SHOW);

				SetTimer(TIMER_CHARS, 15, NULL);				//������ʱ������������
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

		if (i < 3)						//�ֳ��������У�ÿ������
			m_CharFrame[i].rect = CRect(rect.left + rect.Width() / 3 * i, rect.top,
			rect.left + rect.Width() / 3 * (i + 1), rect.top + rect.Height() / 2);
		else
			m_CharFrame[i].rect = CRect(rect.left + rect.Width() / 3 * (i % 3), rect.top + rect.Height() / 2,
			rect.left + rect.Width() / 3 * (i % 3 + 1), rect.bottom);
		CRect tmprect;
		CRect rct = m_CharFrame[i].rect;
			for (int j = 0; j < 6; j++)				//����λ�ã���Ϊ�������飬
			{
				if (j < 3)							//��һ��
					tmprect = CRect(rct.left + rct.Width() / 3 * j, rct.top,
					rct.left + rct.Width() / 3 * (j + 1), rct.top + rct.Height() / 2);
				else//�ڶ���λ��
					tmprect = CRect(rct.left + rct.Width() / 3 * (j % 3), rct.top + rct.Height() / 2,
					rct.left + rct.Width() / 3 * (j % 3 + 1), rct.bottom);
				tmprect.InflateRect(-1, -1);
				m_CharFrame[i].pCharWnd[j].SetChars(m_CharFrame[i].str[j]);			//������ʾ���ַ�
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
		//�������ַ����񴰿�
		m_pTaskCharwnd = new CTaskChars(this);
		rect.InflateRect(-8, -8);
		m_pTaskCharwnd->Create(IDD_TASKCHARS, this);
		m_WndTaskRange.m_Curx = rect.left;
		m_WndTaskRange.m_Cury = rect.top;
		m_WndTaskRange.width = rect.Width();
		m_WndTaskRange.height = rect.Height();
		//����·��
		::LineDDA(m_WndTaskRange.m_Curx, m_WndTaskRange.m_Cury,
			m_WndTaskRange.m_Curx + m_WndTaskRange.width,
			m_WndTaskRange.m_Cury + m_WndTaskRange.height, LineDDAproc, (LPARAM)(LPVOID)(&m_WndTaskRange));
		SetTimer(TIMER_FOR_SHOWTASKWND, 10, NULL);	//��������
	}
	m_pTaskCharwnd->SetWndRect(rect);
	CDC *pDC = GetDC();
	CExtWndShadow Shadow;				//��ʾ��ӰЧ��
	Shadow.Paint(*pDC, rect, 8, CExtWndShadow::DEF_BRIGHTNESS_MIN,
		CExtWndShadow::DEF_BRIGHTNESS_MAX, FALSE);
	ReleaseDC(pDC);
}

VOID CALLBACK CCharShow::LineDDAproc(int x, int y, LPARAM lparam)	//�ص�����
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

// CCharShow ��Ϣ�������


int CCharShow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bIsCreate = false;
	// TODO:  ��ʼ�������Ĵ���
	for (int i = 0; i < 6; i++)								//��ʼ���ַ�����
	{
		m_CharFrame[i].rect = CRect(0, 0, 0, 0);
		m_CharFrame[i].pCharWnd = NULL;
		for (int j = 0; j < 6; j++)
		{
			m_CharFrame[i].ID[j] = 0x0FFFF + i + j;			//����ÿ������ID��������Ϣ����
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

	// TODO:  �ͷ��ڴ���Դ

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
	
	if (nIDEvent == TIMER_CHARS)											//��ʾ�ַ����ڶ���
	{
		for (int i = 0; i < 6; i++)												//6*6 ���
		{
			for (int j = 1; j < 6; j++)
			{
				int p = m_WndRange[i][j].ptr;
				if (p < m_WndRange[i][j].Count)
				{
					m_WndRange[i][j].m_Curx = m_WndRange[i][j].X[p];
					m_WndRange[i][j].m_Cury = m_WndRange[i][j].Y[p];
					m_WndRange[i][j].ptr = p + 20;							//ÿ�β���Ϊ20���ٶȽϿ�
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
		for (int i = 0; i < 6; i++)						//ͳ���Ƿ�ÿ���ַ����ڶ��������
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
		if (count == 36)								//ÿ�����ڸ��½�����ֹͣ��ʱ��
		{
			KillTimer(TIMER_CHARS);						//�رն�ʱ��
			m_bIsCreate = true;							//����Ч��
			GetParent()->Invalidate(TRUE);				//ɾ����Ӱ
			//SendMessage(WM_PAINT);
		}
	}
	if (nIDEvent == TIMER_FOR_LARGESHOWCHARS)			//�󴰿ڷ�ʽ��ʾ�ַ�����
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
	if (nIDEvent == TIMER_FOR_SMALLSHOWCHARS)				//С���ڷ�ʽ��ʾ�ַ�����
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
			m_pShowChars->SetWndRect(m_CharFrame[5].rect);				//���´���λ��
			KillTimer(TIMER_FOR_SMALLSHOWCHARS);
		}
	}
	if (nIDEvent == TIMER_FOR_SHOWTASKWND)					//��ʾ���񴰿ڶ���
	{
		int p = m_WndTaskRange.ptr;
		CRect tmprect;
		if (p < m_WndTaskRange.Count)
		{
			tmprect = CRect(m_CharFrame[0].rect.left, m_CharFrame[0].rect.top, m_WndTaskRange.X[p], m_WndTaskRange.Y[p]);
			m_pTaskCharwnd->SetWndRect(tmprect);
			m_WndTaskRange.ptr = p + 50;					//�ƶ�����Ϊ50
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
	if (nIDEvent == TIMER_FOR_POPCHARS)						//ɾ����ĸ
	{
		PopChars();											//����ɾ����ĸ
		if (GetChars().GetLength() <= 0)					//ɾ����ϣ�ֹͣ
		{
			KillTimer(TIMER_FOR_POPCHARS);
		}
		InvalidateRect(&(m_CharFrame[5].rect), FALSE);
	}
	__super::OnTimer(nIDEvent);
}

BOOL CCharShow::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)							//�м��̰���
	{
		//WPARAM code = ((pMsg->lParam & 0x0F0000) >> 16)-1;	//���� for debug
		WPARAM code = pMsg->wParam-0x30;
		if (code >= 1 && code <= 6/*&&pMsg->wParam==229*/)		//��VK����1~6֮�䣬���д���
		{
			if (m_FirstBite == 0)								//�ַ�����Ϸ�ʽ���ж��Ƿ��һ��ҧ
			{
				m_FirstBite = code;									//��Ӧ���ַ���������
				if (m_CharFrame[m_FirstBite-1].pCharWnd == NULL)	//�������ڴ��壬����
					return TRUE;
				if (m_FirstBite == 5)							//��5�鴰��ֿ�������Ϊֻ��Y��Z��ĸ
				{
					for (int i = 0; i < 2; i++)
					{
						::SendMessage(m_CharFrame[m_FirstBite-1].pCharWnd[i].GetSafeHwnd(), MYWM_CHARMSG, 1, -1);	//����ѡ��					
					}
				}
				else
					for (int i = 0; i < 6; i++)
					{
						::SendMessage(m_CharFrame[m_FirstBite-1].pCharWnd[i].GetSafeHwnd(), MYWM_CHARMSG, 1, -1);	//����ѡ��					
					}
			}
			else
			{	//�ڶ���ҧ
				m_SecondBite = code-1;
				if (m_CharFrame[m_FirstBite-1].pCharWnd == NULL)	//�������ڴ��壬����
					return TRUE;
				//ѡ������ָ�
				if (m_FirstBite == 5)
				{
					if (m_SecondBite == 1 || m_SecondBite == 0)
					{
						::SendMessage(m_CharFrame[m_FirstBite - 1].pCharWnd[m_SecondBite].m_hWnd,
							MYWM_CHARMSG, 1, 1);		//��˸����һ��
						m_strBuf += myStr[(m_FirstBite - 1) * 6 + m_SecondBite];
						InvalidateRect(&m_CharFrame[5].rect, FALSE);
						for (int i = 0; i < 2; i++)
						{
							if (m_SecondBite != i)
							::SendMessage(m_CharFrame[m_FirstBite - 1].pCharWnd[i].GetSafeHwnd(),
								MYWM_CHARMSG, 0, -1);	//������ѡ��					
						}
					}
					else
						return TRUE;					//��ָ��������Ч����
				}
				else
				{
					::SendMessage(m_CharFrame[m_FirstBite - 1].pCharWnd[m_SecondBite].m_hWnd, 
						MYWM_CHARMSG, 1, 1);		//��˸����һ��
					m_strBuf += myStr[(m_FirstBite - 1) * 6 + m_SecondBite];		//������ַ���ӵ�������
					InvalidateRect(&m_CharFrame[5].rect, FALSE);
					
					for (int i = 0; i < 6; i++)
					{
						if (m_SecondBite!=i)
						::SendMessage(m_CharFrame[m_FirstBite - 1].pCharWnd[i].GetSafeHwnd(), 
							MYWM_CHARMSG, 0, -1);	//������ѡ��					
					}
				}

				m_FirstBite = 0;
			}
			return TRUE;
		}

		if (pMsg->wParam == VK_RETURN)
		{
			if (m_CharFrame[4].pCharWnd == NULL)	//�������ڴ��壬����
				return TRUE;
			::SendMessage(m_CharFrame[4].pCharWnd[4].m_hWnd,
				MYWM_CHARMSG, 1, 1);		//��˸����һ��
			m_strBuf += L"\r\n";
			//::SendMessage(m_CharFrame[4].pCharWnd[4].m_hWnd,
			//	MYWM_CHARMSG, 0, -1);		//ȡ��ѡ��
			InvalidateRect(&m_CharFrame[5].rect, FALSE);
			return TRUE;
		}

		if (pMsg -> wParam == VK_DELETE)
		{
			if (m_CharFrame[4].pCharWnd == NULL)	//�������ڴ��壬����
				return TRUE;
			::SendMessage(m_CharFrame[4].pCharWnd[5].m_hWnd,
				MYWM_CHARMSG, 1, 1);		//��˸����һ��
			PopChars();
			//::SendMessage(m_CharFrame[4].pCharWnd[5].m_hWnd,
			//	MYWM_CHARMSG, 0, -1);		//ȡ��ѡ��
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
			m_CharFrame[i].pCharWnd[j].SetHintMsg(Hit_msg[i][j]);			//������ʾ��
		}
	}
	Invalidate(TRUE);
	(*m_pOutPut)(L"��ʾ��ʾ��Ϣ");
	return 0;
}

LRESULT CCharShow::OnHindHint(WPARAM wapram, LPARAM lparam)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_CharFrame[i].pCharWnd[j].SetHintMsg(L"");			//������ʾ��
		}
	}
	Invalidate(TRUE);
	(*m_pOutPut)(L"������ʾ��Ϣ");
	return 0;
}

LRESULT CCharShow::OnShowSettingTask(WPARAM wparam, LPARAM lparam)
{
	AfxMessageBox(L"Add Code");
	return 0;
}

//dll ����ӿڣ����ɱ�

extern "C"  _declspec(dllexport) char* GetAppModualMsg()					//��ȡģ����Ϣ
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return " 6*6��׼��ҧ�������ַ�";
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
		CRect(0, 0, 800, 800), parent, 0xFFF2);					//ID ��
	pViewer->ShowWindow(SW_SHOW);
	return b;
}


//��̬��Ӳ˵�������Ӧ�õ���Ҫ������ӣ����Ǳ���Ĳ�
extern"C" _declspec(dllexport) BOOL	InstallExtMenu(CWnd* pChild, HWND Parent, UINT* NextMenuID)
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
	theApp.m_CommandToMessage.InitHashTable(3);		//Ԥ��3����޸�

	NextMenuID = new UINT[3];
	NextMenuID[0] = theApp.ID_MENU_SHOWHINT;		
	NextMenuID[1] = theApp.ID_MENU_HIDEHINT;
	NextMenuID[2] = theApp.ID_MENU_SETTASK;

	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"��ʾ��ʾ��");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_SHOWHINT);	//����ӳ��
	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"������ʾ��");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_HIDEHINT);
	Menu.AppendMenu(MF_STRING, (UINT_PTR)(*NextMenuID), L"��������");
	theApp.m_CommandToMessage.SetAt(*(NextMenuID)++, theApp.ID_MENU_SETTASK);

	HMENU hsubMenu = Menu.Detach();
	ParentMenu.InsertMenuW(ParentMenu.GetMenuItemCount(), MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)hsubMenu,
		_T("�ַ�Ӧ������"));

	ParentMenu.Detach();
	CWnd::FromHandle(Parent)->DrawMenuBar();		//ǿ���ػ�˵�
	return TRUE;
}

extern"C" _declspec(dllexport)BOOL CheckExtMenuItem(UINT id)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	if (id == theApp.ID_MENU_HIDEHINT || 
		id == theApp.ID_MENU_SETTASK||
		id==theApp.ID_MENU_SHOWHINT)		//�ж��Ƿ��ǲ˵���Ϣ
		return TRUE;
	else
		return FALSE;
}