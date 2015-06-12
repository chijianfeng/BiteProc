#pragma once

#include "IEEGViewer.h"
// CBaseEEGViewerWnd

class CBaseEEGViewerWnd : public CWnd,implements IEEGViewer
{
	DECLARE_DYNAMIC(CBaseEEGViewerWnd)

public:
	CBaseEEGViewerWnd();
	virtual ~CBaseEEGViewerWnd();

protected:
	DECLARE_MESSAGE_MAP()
};


