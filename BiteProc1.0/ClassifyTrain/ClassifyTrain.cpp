// ClassifyTrain.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ClassifyTrain.h"

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

// CClassifyTrainApp

BEGIN_MESSAGE_MAP(CClassifyTrainApp, CWinApp)
END_MESSAGE_MAP()


// CClassifyTrainApp ����

CClassifyTrainApp::CClassifyTrainApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CClassifyTrainApp ����

CClassifyTrainApp theApp;
UINT CClassifyTrainApp::ID_MENU_PARAM = 0x10004;

// CClassifyTrainApp ��ʼ��

BOOL CClassifyTrainApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
