#include "stdafx.h"
#include "FileOper.h"
#include "Stringoper.h"


CFileOper::CFileOper()
{
}


CFileOper::~CFileOper()
{
}


void CFileOper::FindAll(list<CString>& listPath, CString path)
{
	if (path.GetLength() <= 0)
		return;

	CFileFind tmpfind;
	BOOL isfind = FALSE;

	isfind = tmpfind.FindFile(path + L"\\*.*");
	CString tmppath;

	while (isfind)
	{
		isfind = tmpfind.FindNextFile();
		if (tmpfind.IsDots())
			continue;
		if (tmpfind.IsDirectory())
		{
			tmppath = L"";
			tmppath = tmpfind.GetFilePath();
			FindAll(listPath , tmppath);
		}
		else
		{
			tmppath = tmpfind.GetFilePath();
			listPath.push_back(tmppath);
		}
	}
	tmpfind.Close();
	return;
}

void CFileOper::FindAll(vector<CString>& vectorPath, CString path)
{
	if (path.GetLength() <= 0)
		return;

	CFileFind tmpfind;
	BOOL isfind = FALSE;

	isfind = tmpfind.FindFile(path + L"\\*.*");
	CString tmppath;

	while (isfind)
	{
		isfind = tmpfind.FindNextFile();
		if (tmpfind.IsDots())
			continue;
		if (tmpfind.IsDirectory())
		{
			tmppath = L"";
			tmppath = tmpfind.GetFilePath();
			FindAll(vectorPath, tmppath);
		}
		else
		{
			tmppath = tmpfind.GetFilePath();
			vectorPath.push_back(tmppath);
		}
	}
	tmpfind.Close();
	return;
}

void CFileOper::FindFiles(vector<CString>& vectorpath, CString path, CString fformat)
{
	if (path.GetLength() <= 0)
		return;

	CFileFind tmpfind;
	BOOL isfind = FALSE;

	isfind = tmpfind.FindFile(path + L"\\"+fformat);
	CString tmppath;

	while (isfind)
	{
		isfind = tmpfind.FindNextFile();
		if (tmpfind.IsDots())
			continue;
		if (tmpfind.IsDirectory())
		{
			tmppath = L"";
			tmppath = tmpfind.GetFilePath();
			FindFiles(vectorpath, tmppath, fformat);
		}
		else
		{
			tmppath = tmpfind.GetFilePath();
			vectorpath.push_back(tmppath);
		}
	}
	tmpfind.Close();
	return;
}
BOOL CFileOper::FileExist(CString path)
{
	if (path.GetLength() <= 0)
		return FALSE;
	CFileFind finder;
	return finder.FindFile(path);
}

BOOL CFileOper::FileExist(string path)
{
	if (path.length() <= 0)
		return FALSE;
	CFileFind finder;
	Stringoper oper;
	return finder.FindFile(oper.tocstring(path));
}

CString CFileOper::DirectorySelect(HWND hwnd)
{
	WCHAR szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = hwnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = L"请选择需要打开的目录：";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		str.Format(L"%s", szPath);
		return str;
	}
	else
		return _T("");

}

CString CFileOper::PathSelect(CString fileextern)
{
	CString FilePathName;
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, fileextern, NULL);
	if (dlgFile.DoModal())
	{
		FilePathName = dlgFile.GetPathName();
	}
	else
		FilePathName = L"";
	return FilePathName;
}

CString CFileOper::GetExeFilePath()
{
	TCHAR  exepath[MAX_PATH];
	CString  strdir, tmpdir;
	memset(exepath, 0, MAX_PATH);     
	GetModuleFileName(NULL, exepath, MAX_PATH);
	tmpdir = exepath;
	strdir = tmpdir.Left(tmpdir.ReverseFind('\\'));    
	return strdir;
}

void CFileOper::DeleteDirectory(CString strDir)
{
	if (strDir.IsEmpty())
	{
		RemoveDirectory(strDir);
		return;
	}
	//首先删除文件及子文件夹  
	CFileFind   ff;
	BOOL bFound = ff.FindFile(strDir + _T(" \\*"),0 );
	while (bFound)
	{
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == _T(".") || ff.GetFileName() == _T(".."))
			continue;
		//去掉文件(夹)只读等属性  
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory())
		{
			//递归删除子文件夹  
			DeleteDirectory(ff.GetFilePath());
			RemoveDirectory(ff.GetFilePath());
		}
		else
		{
			DeleteFile(ff.GetFilePath());   //删除文件  
		}
	}
	ff.Close();
	//然后删除该文件夹  
	RemoveDirectory(strDir);
}