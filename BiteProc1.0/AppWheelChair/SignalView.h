#pragma once


// 显示单通道数据，实现特性化操作

#include <vector>
#include "ManagedData.h"

class CSignalView : public CWnd
{
	DECLARE_DYNAMIC(CSignalView)

public:
	CSignalView();
	CSignalView(CRect rect);
	virtual ~CSignalView();
	void	UpdateData(ManagedData& data ,  ManagedData& color);				//只进行具备更新
	void	SetThrold(const size_t throld);				//设置相应的判断阈值
	void	UpdateAll();								//更新全部
	void	SetMaxMin(int maxv ,int minv);

	void	SetState(bool b);

	void	SetShowLength(const int bitelen , const bool flag);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnSignalviewmsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();

private:
	CRect					m_SignalRect;				//signal view rect.
	ManagedData				m_SignalColor;				//record the reference
	ManagedData				m_SignalData;				//保存一个窗口的数据
	size_t					m_Throld;
	//
	ManagedData				m_NewSignalData;
	ManagedData				m_NewSignalColor;
	bool					m_bUpdateLocal;
	int						m_Curpos;	
	int						m_MaxValue;
	int						m_MinValue;
	int						m_MeanValue;
	int						m_Prey;						//record the previous value
	COLORREF				m_Precolor;

	bool					m_bIsSaving;

	bool					m_bDrawlength;
	int						m_nBitelen;
	int						m_nCurlinepos;
	bool					m_bStartDrawline;
private:
	void ReDrawmap(CDC *pDC);
	void DrawMap(CDC *pDC , double *pdata , size_t length);

	int  maxvalue(double *pd , int length);
	int  minvalue(double *pd , int length);

	void DrawLeghtline(CDC *pDC , int sp);				//draw the mark line
	//for change.
	int  numcount;
public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


