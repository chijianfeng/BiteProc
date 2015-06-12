#pragma once

#include "IAppMsg.h"
// CBaseAppWnd

class CBaseAppWnd : public CWnd,implements IAppMsg
{
	DECLARE_DYNAMIC(CBaseAppWnd)

public:
	CBaseAppWnd();
	virtual ~CBaseAppWnd();

protected:
	DECLARE_MESSAGE_MAP()
};


