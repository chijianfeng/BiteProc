#pragma once
#include "Structdefine.h"
#include "ITrainMsg.h"
#include <afxdialogex.h>
// CBaseTrainWnd 仅仅作为接口，不可实例化

class CBaseTrainWnd : public CDialogEx,implements ITrainMsg				//构建接口
{
	DECLARE_DYNAMIC(CBaseTrainWnd)

public:
	CBaseTrainWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaseTrainWnd();

// 对话框数据
	//enum { IDD = IDD_BASETRAINWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
