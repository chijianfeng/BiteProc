#pragma once
//����Windows ƽ̨���ļ��������ϣ��漰�Ĳ���������
/*
1.����Ŀ���ļ����ļ���
2.����Ŀ���ļ����������ļ���
*/
#include <iostream>
#include <list>
#include <vector>
#include <afx.h>
using namespace std;
class CFileOper
{
public:
	CFileOper();
	virtual ~CFileOper();

	/*
	ʵ�ֱ���Ŀ���ļ����������ļ���
	���룺path:CString
	�����listPath:list<CString>
	*/
	void FindAll(list<CString>& listPath, CString path);

	void FindAll(vector<CString>& vectorpath, CString path);

	//fformat,, such as: *.dll
	void FindFiles(vector<CString>& vectorpath, CString path, CString fformat);

	BOOL FileExist(CString path);

	BOOL FileExist(string path);

	CString DirectorySelect(HWND hwnd);
	
	/*
	�ļ�����˵������չ������ | �ָ���ͬ�������ļ�����չ��������� ; �ָÿ���ļ����ͼ��� | �ָ���ĩβ�� || ָ����
	*/
	CString PathSelect(CString fileextern);					//open the the format file

	CString	GetExeFilePath();								//��ȡexe����Ŀ¼

	//�ļ�ɾ��
	void DeleteDirectory(CString path);
};

