#pragma once


// CEEGView ��ͼ,������ʾEEG�������ݣ���������Զ���ģ��

class CEEGView : public CScrollView
{
	DECLARE_DYNCREATE(CEEGView)

protected:
	CEEGView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CEEGView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSettingEegviewer();
	afx_msg void OnStart();				//�������ɼ�
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnZoomin();			//��С
	afx_msg void OnZoomout();			//�Ŵ�
	afx_msg void OnStop();				//ֹͣ
	afx_msg void OnPause();				//��ͣ
	afx_msg void OnSaveData();
};


