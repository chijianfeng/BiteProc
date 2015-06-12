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
	WCHAR szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = hwnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = L"��ѡ����Ҫ�򿪵�Ŀ¼��";
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
	//����ɾ���ļ������ļ���  
	CFileFind   ff;
	BOOL bFound = ff.FindFile(strDir + _T(" \\*"),0 );
	while (bFound)
	{
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == _T(".") || ff.GetFileName() == _T(".."))
			continue;
		//ȥ���ļ�(��)ֻ��������  
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory())
		{
			//�ݹ�ɾ�����ļ���  
			DeleteDirectory(ff.GetFilePath());
			RemoveDirectory(ff.GetFilePath());
		}
		else
		{
			DeleteFile(ff.GetFilePath());   //ɾ���ļ�  
		}
	}
	ff.Close();
	//Ȼ��ɾ�����ļ���  
	RemoveDirectory(strDir);
}