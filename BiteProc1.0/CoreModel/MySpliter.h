#pragma once


// CMySpliter

class CMySpliter : public CSplitterWnd
{
	DECLARE_DYNAMIC(CMySpliter)

public:
	CMySpliter();
	virtual ~CMySpliter();

	void SetHeight(int h);
private:
	int		m_Height;
	int		m_Min;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


