#pragma once
//基于Windows 平台的文件操作集合，涉及的操作包括：
/*
1.遍历目标文件下文件；
2.变量目标文件夹下所有文件。
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
	实现遍历目标文件夹下所有文件；
	输入：path:CString
	输出：listPath:list<CString>
	*/
	void FindAll(list<CString>& listPath, CString path);

	void FindAll(vector<CString>& vectorpath, CString path);

	//fformat,, such as: *.dll
	void FindFiles(vector<CString>& vectorpath, CString path, CString fformat);

	BOOL FileExist(CString path);

	BOOL FileExist(string path);

	CString DirectorySelect(HWND hwnd);
	
	/*
	文件类型说明和扩展名间用 | 分隔，同种类型文件的扩展名间可以用 ; 分割，每种文件类型间用 | 分隔，末尾用 || 指明。
	*/
	CString PathSelect(CString fileextern);					//open the the format file

	CString	GetExeFilePath();								//获取exe工作目录

	//文件删除
	void DeleteDirectory(CString path);
};

