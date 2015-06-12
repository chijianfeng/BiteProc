
// CoreModelView.h : CCoreModelView ��Ľӿ�
//

#pragma once


class CCoreModelView : public CView
{
protected: // �������л�����
	CCoreModelView();
	DECLARE_DYNCREATE(CCoreModelView)

// ����
public:
	CCoreModelDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	bool		m_bInitlize;		//�Ƿ��ѱ���ʼ��

// ʵ��
public:
	virtual ~CCoreModelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	void LoadBiteTrainWnd();					//������ֵѵ��ģ��
	void LoadClassifyTrainWnd();				//��������ͬʱѵ��ģ��
	void LoadHLTrainWnd();						//��������ѵ��ģ��
	void LoadTLTrainWnd();						//�������Ρ���Ҫѵ��ģ��
	void LoadAppWnd();							//����Ӧ��ģ��

	void SendDatatoWnd();						//�����ݷ��͸�ѵ������

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	afx_msg LRESULT OnRcvTrainMsg(WPARAM wparam, LPARAM lparam);		//����ѵ�����ڵ���Ϣ
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // CoreModelView.cpp �еĵ��԰汾
inline CCoreModelDoc* CCoreModelView::GetDocument() const
   { return reinterpret_cast<CCoreModelDoc*>(m_pDocument); }
#endif

