
// CoreModelView.h : CCoreModelView 类的接口
//

#pragma once


class CCoreModelView : public CView
{
protected: // 仅从序列化创建
	CCoreModelView();
	DECLARE_DYNCREATE(CCoreModelView)

// 特性
public:
	CCoreModelDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	bool		m_bInitlize;		//是否已被初始化

// 实现
public:
	virtual ~CCoreModelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	void LoadBiteTrainWnd();					//导入阈值训练模块
	void LoadClassifyTrainWnd();				//导入左、右同时训练模块
	void LoadHLTrainWnd();						//导入轻重训练模块
	void LoadTLTrainWnd();						//导入两次、长要训练模块
	void LoadAppWnd();							//加载应用模块

	void SendDatatoWnd();						//将数据发送给训练窗口

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	afx_msg LRESULT OnRcvTrainMsg(WPARAM wparam, LPARAM lparam);		//接收训练窗口的消息
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // CoreModelView.cpp 中的调试版本
inline CCoreModelDoc* CCoreModelView::GetDocument() const
   { return reinterpret_cast<CCoreModelDoc*>(m_pDocument); }
#endif

