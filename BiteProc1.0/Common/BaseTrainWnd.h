#pragma once
#include "Structdefine.h"
#include "ITrainMsg.h"
#include <afxdialogex.h>
// CBaseTrainWnd ������Ϊ�ӿڣ�����ʵ����

class CBaseTrainWnd : public CDialogEx,implements ITrainMsg				//�����ӿ�
{
	DECLARE_DYNAMIC(CBaseTrainWnd)

public:
	CBaseTrainWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBaseTrainWnd();

// �Ի�������
	//enum { IDD = IDD_BASETRAINWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
