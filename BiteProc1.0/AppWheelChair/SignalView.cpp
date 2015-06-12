// SignalView.cpp : 实现文件
//

#include "stdafx.h"
#include "SignalView.h"
#include "CommonUtile.h"
// CSignalView

IMPLEMENT_DYNAMIC(CSignalView, CWnd)

CSignalView::CSignalView()
{
	m_Throld = 0;
	m_bUpdateLocal = false;
	m_NewSignalData.SetChannum(1);
	m_SignalColor.SetChannum(1);
	m_SignalData.SetChannum(1);
	m_NewSignalColor.SetChannum(1);
	m_Curpos  = 0;
	m_Prey = 0;
	m_Precolor = 0;
	m_MinValue = m_MaxValue = 0;
	numcount = 0;
	m_MeanValue = 0;
	m_bIsSaving = true;
	m_bStartDrawline = false;
	m_bDrawlength = true;
}

CSignalView::CSignalView(CRect rect)
{
	m_bUpdateLocal = false;
	m_Throld = 0;
	m_SignalColor.ResizeManageData(rect.Width());
	m_SignalData.ResizeManageData(rect.Width());
	m_SignalRect = rect;
	m_NewSignalData.SetChannum(1);
	m_SignalColor.SetChannum(1);
	m_SignalData.SetChannum(1);
	m_NewSignalColor.SetChannum(1);
	m_Curpos  = 0;
	m_Prey = 0;
	m_Precolor = 0;
	m_MinValue = m_MaxValue = 0;
	numcount = 0;
	m_MeanValue = 0;
	m_bIsSaving = true;
	m_bDrawlength = true;
	m_nCurlinepos = -10000;
}
CSignalView::~CSignalView()
{

}


BEGIN_MESSAGE_MAP(CSignalView, CWnd)
	ON_WM_PAINT()
//	ON_WM_ERASEBKGND()
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSignalView 消息处理程序




afx_msg LRESULT CSignalView::OnSignalviewmsg(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


void CSignalView::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	GetClientRect(&m_SignalRect);

	if(!m_bUpdateLocal)		
	{
		CDC *pDC = GetDC();
		CDC MemDC;
		MemDC.CreateCompatibleDC(NULL);
		CBitmap Memmap;
		Memmap.CreateCompatibleBitmap(pDC , m_SignalRect.Width(),m_SignalRect.Height());
		CBitmap *pOldMap = MemDC.SelectObject(&Memmap);
		ReDrawmap(&MemDC);
		pDC->BitBlt(0,0,m_SignalRect.Width(),m_SignalRect.Height(),&MemDC,0,0,SRCCOPY);
		ReleaseDC(pDC);
		MemDC.SelectObject(pOldMap);
		Memmap.DeleteObject();
		MemDC.DeleteDC();
		m_bUpdateLocal = false;
	}
	else
	{
		CDC *pDC = GetDC();
		if (pDC != NULL)
		{
			DrawMap(pDC, m_NewSignalData.GetData(), m_NewSignalData.GetDataSize());
			ReleaseDC(pDC);
		}
		else
			ReleaseDC(pDC);
		m_bUpdateLocal = TRUE;
	}
	CWnd::OnPaint();
}

void CSignalView::UpdateAll()
{
	m_bUpdateLocal  = false;
//	this->SendMessage(WM_PAINT);
}

void CSignalView::UpdateData(ManagedData& data, ManagedData& color)
{
	m_NewSignalData = data;
	m_NewSignalColor = color;
	m_SignalData.AppendData(data);
	m_SignalColor.AppendData(color.GetData(),color.GetDataSize());
	m_bUpdateLocal = true;
	//OnPaint();
}

void CSignalView::SetState(bool b)
{
	m_bIsSaving = b;
}

void CSignalView::SetMaxMin(int maxv ,int minv)
{
	m_MaxValue = maxv;
	m_MinValue = minv;
}

void CSignalView::SetThrold(const size_t throld)
{
	this->m_Throld = throld;
}

void CSignalView::SetShowLength(const int bitelen , const bool flag)
{
	m_bDrawlength = flag;
	m_nBitelen = bitelen;
}

void CSignalView::ReDrawmap(CDC *pDC)
{
	if(pDC==NULL)
		return;
	double *pdata = m_SignalData.GetData();
	double *pcolor = m_SignalColor.GetData();
	size_t length = m_SignalData.GetDataSize();
	int base = m_SignalRect.Height()/2;
	pDC->FillSolidRect(0,0,m_SignalRect.Width()  ,m_SignalRect.Height(),RGB(255,255,255));
	COLORREF precolor  = (int)pcolor[0];
	COLORREF curcolor = precolor;
	CPen drawpen;
	int prey = 0;
	if(pdata[0]<=0)
	{
		prey = m_MinValue==0?(pdata[0]*(-1.0)+base):(pdata[0]/m_MinValue*base+base);
	}
	else
		prey = m_MaxValue==0?(base-pdata[0]):(base-pdata[0]/m_MaxValue*base);
	drawpen.CreatePen(PS_SOLID , 1 , precolor);
	for (int i=1;i<length;i++)
	{
		if(precolor!=curcolor)
		{
			drawpen.DeleteObject();
			drawpen.CreatePen(PS_SOLID ,1,curcolor);
			precolor = curcolor;
		}
		pDC->SelectObject(drawpen);
		int cury = 0;
		if(m_MaxValue==0&&m_MinValue==0)
		{
			cury = (int)(pdata[i]*(-1.0)+base);
		}
		else
		{
			cury = (pdata[i]-m_MinValue)*m_SignalRect.Height()/(m_MaxValue-m_MinValue)+base-m_MeanValue;
		}
		pDC->MoveTo(i-1,prey);
		pDC->LineTo(i,cury);
		prey = cury;
		curcolor = (int)pcolor[i];
	}
	drawpen.DeleteObject();
}
void CSignalView::DrawMap(CDC *pDC , double *pdata , size_t length)
{
	if(NULL==pDC||pdata==NULL||length<=0)
		return;
	CPen drawpen;
	drawpen.CreatePen(PS_SOLID , 1 , m_Precolor);
/*
	int maxd = maxvalue(pdata , length);
	int mind = minvalue(pdata , length);
	m_MaxValue = m_MaxValue>maxd?m_MaxValue:maxd;
	m_MinValue = m_MinValue<mind?m_MinValue:mind;*/
	CPen *pold = pDC->SelectObject(&drawpen);
	//erase the previous map;
	COLORREF previouscolor = RGB(255,255,255);
	if(m_bIsSaving)
	{
		previouscolor = RGB(84,255,159);
	}
	int cx = min(m_Curpos+length,m_SignalRect.Width());
	pDC->FillSolidRect(m_Curpos,0,length , m_SignalRect.Height(),previouscolor);
	cx = m_Curpos+length-m_SignalRect.Width();
	if(cx>0)
	{
		pDC->FillSolidRect(0,0,cx , m_SignalRect.Height(),previouscolor);
	}
	/*if(m_bDrawlength&&m_bStartDrawline)
	{
		DrawLeghtline(pDC , m_nCurlinepos);
	}*/
	int base = m_SignalRect.Height()/2;
	int mind1 = UTILE::mean(pdata , length);
	for (int i=0;i<length;i++)
	{
		int curcolor = *(m_NewSignalColor.GetData()+i);
		if(m_Precolor!=curcolor)
		{
			m_Precolor = curcolor;
			drawpen.DeleteObject();
			drawpen.CreatePen(PS_SOLID , 1, curcolor);
			pDC->SelectObject(&drawpen);
		}
		if(curcolor>0&&(m_Curpos-m_nCurlinepos+m_SignalRect.Width())%m_SignalRect.Width()>m_nBitelen)
		{
			m_bStartDrawline = true;
			m_nCurlinepos = m_Curpos;
		}
		else if((m_Curpos-m_nCurlinepos+m_SignalRect.Width())%m_SignalRect.Width()<=m_nBitelen)
		{
			m_bStartDrawline = true;
		}
		else
		{
			m_bStartDrawline = false;
		}
		int cury = 0;
		if(m_MaxValue==0&&m_MinValue==0)
		{
			cury = (int)(pdata[i]*(-1.0)+base);
		}
		else
		{
			cury = (pdata[i]-0.5*(m_MaxValue+m_MinValue))/(m_MaxValue-m_MinValue)*m_SignalRect.Height()+base;
			//cury = (m_SignalRect.Height()*0.5-(pdata[i]-mind1)*(m_SignalRect.Height()*3)/m_MaxValue);
		}
		pDC->MoveTo(m_Curpos , m_Prey);
		pDC->LineTo(m_Curpos+1 , cury);
		m_Prey = cury;
		m_Curpos = (m_Curpos++)%m_SignalRect.Width();
	}
	drawpen.DeleteObject();
	drawpen.CreatePen(PS_SOLID , 2 , RGB(32,128,128));
	pDC->SelectObject(&drawpen);
	pDC->MoveTo(m_Curpos+8,0);
	pDC->LineTo(m_Curpos+8,m_SignalRect.Height());
	pDC->SelectObject(pold);
	drawpen.DeleteObject();
}

void CSignalView::DrawLeghtline(CDC *pDC , int startpos)
{
	if(pDC==NULL||startpos<0)
		return;
	CPoint startp = CPoint(startpos , m_SignalRect.Height()*0.1);
	CPoint endp   = CPoint(startpos+m_nBitelen , m_SignalRect.Height()*0.1);
	COLORREF color = RGB(0,0,200);
	int width = 5;
	if(endp.x<=m_SignalRect.Width())
	{
		pDC->FillSolidRect(startp.x , startp.y , endp.x-startp.x,width , color);
	}
	else
	{
		pDC->FillSolidRect(startp.x , startp.y , m_SignalRect.Width()-startp.x,width , color);
		pDC->FillSolidRect(0 ,        startp.y , endp.x-m_SignalRect.Width(),  width , color);
	}
}

int CSignalView::maxvalue(double *pd , int length)
{
	ASSERT(pd!=NULL||length>=0);
	int maxv = 0;
	for (int i = 0;i<length;i++)
	{
		if(maxv<pd[i])
			maxv = (int)pd[i];
	}
	return maxv;
}

int CSignalView::minvalue(double *pd , int length)
{
	ASSERT(pd!=NULL||length>=0);
	int minv = 0;
	for (int i = 0;i<length;i++)
	{
		if(minv>pd[i])
			minv = (int)pd[i];
	}
	return minv;
}

//BOOL CSignalView::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	pDC->FillSolidRect(0,0,m_SignalRect.Width(),m_SignalRect.Height(),RGB(255,255,255));
//	return true;
//}


int CSignalView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetClientRect(&m_SignalRect);
	m_SignalData.ResizeManageData(m_SignalRect.Width());
	m_SignalColor.ResizeManageData(m_SignalRect.Width());
	Invalidate(TRUE);
	return 0;
}


void CSignalView::OnDestroy()
{
	CWnd::OnDestroy();
}


void CSignalView::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	CWnd::OnLButtonDown(nFlags, point);
}


void CSignalView::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	CWnd::OnLButtonUp(nFlags, point);
}


void CSignalView::OnMouseMove(UINT nFlags, CPoint point)
{
	
	CWnd::OnMouseMove(nFlags, point);
}


BOOL CSignalView::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	COLORREF bckcolor = RGB(245, 245, 245);				//white.

	pDC->FillSolidRect(rect, bckcolor);
	return TRUE;

	//return CWnd::OnEraseBkgnd(pDC);
}
