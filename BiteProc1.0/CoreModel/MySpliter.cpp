// MySpliter.cpp : 实现文件
//

#include "stdafx.h"
#include "CoreModel.h"
#include "MySpliter.h"


// CMySpliter

IMPLEMENT_DYNAMIC(CMySpliter, CSplitterWnd)

CMySpliter::CMySpliter()
{
	m_Min = 11;
	m_Height = 250;
}

CMySpliter::~CMySpliter()
{
}

void CMySpliter::SetHeight(int h)
{
	this->m_Height = h;
	m_Min = 11;
}

BEGIN_MESSAGE_MAP(CMySpliter, CSplitterWnd)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CMySpliter 消息处理程序




void CMySpliter::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int height , m;
	GetRowInfo(0,height,m);
	if(height ==0||height>m_Height)
	{
		SetRowInfo(0,m_Height , m_Min);
	}
	else
	{
		m_Height = height;
		m_Min = m;
		SetRowInfo(0,800,m_Min);
	}
	RecalcLayout();
//	CSplitterWnd::OnLButtonDblClk(nFlags, point);
}
