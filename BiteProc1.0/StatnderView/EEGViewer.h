#pragma once

#include "IEEGViewer.h"
#include "SignalView.h"
// CEEGViewer

class CEEGViewer : public CWnd,implements IEEGViewer
{
	DECLARE_DYNAMIC(CEEGViewer)

public:
	CEEGViewer();
	virtual ~CEEGViewer();

	//implement the virtual func
	void SetEEGData( ManagedData& data,  ManagedData& color);
	void SetMaxMin(const int Maxv, const int Minv);

	void GetCurMaxMin(int& Maxv, int& Minv);					//���ص�ǰ�����Сֵ

	void AutoMaxMin(bool b);

	void UpdateAll();

	void SetClientSize(const CRect rect);

	void SetChannelNum(const size_t num);

private:

	size_t			m_Channum;							//��ʾ��ͨ����
	CSignalView*	m_pSignalView;						//��ͨ����ʾ
	int				m_Maxv, m_Minv;						//�����Сֵ
	CRect			m_Rect;								//��ʾ��Χ

	bool			m_AutoMaxMin;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnPaint();
};


