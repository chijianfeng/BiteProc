#pragma once


// CEEGView 视图,用于显示EEG波形数据，并可添加自定义模块

class CEEGView : public CScrollView
{
	DECLARE_DYNCREATE(CEEGView)

protected:
	CEEGView();           // 动态创建所使用的受保护的构造函数
	virtual ~CEEGView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void OnInitialUpdate();     // 构造后的第一次
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSettingEegviewer();
	afx_msg void OnStart();				//启动，采集
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnZoomin();			//缩小
	afx_msg void OnZoomout();			//放大
	afx_msg void OnStop();				//停止
	afx_msg void OnPause();				//暂停
	afx_msg void OnSaveData();
};


