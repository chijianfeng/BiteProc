// AppNumbers.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "AppNumbers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CAppNumbersApp

BEGIN_MESSAGE_MAP(CAppNumbersApp, CWinApp)
END_MESSAGE_MAP()


// CAppNumbersApp ����

CAppNumbersApp::CAppNumbersApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAppNumbersApp ����

CAppNumbersApp theApp;

UINT CAppNumbersApp::ID_MENU_HIDEHINT = 0x10001;
UINT CAppNumbersApp::ID_MENU_SHOWHINT = 0x10002;
UINT CAppNumbersApp::ID_MENU_SETTASK  = 0x10003;
// CAppNumbersApp ��ʼ��

BOOL CAppNumbersApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
