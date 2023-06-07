// MyApp.cpp: implementation of the CMyApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToHardlink.h"
#include "MyApp.h"
#include <shlwapi.h>
#include "md5.h"
#include "ThreadPool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMyApp theApp;

CMyApp::CMyApp()
: m_bVerbose(FALSE)
{
	
}

CMyApp::~CMyApp()
{	
}

VOID CMyApp::SetVerbose(BOOL bVerbose/* = TRUE*/)
{
	m_bVerbose = bVerbose;
}

VOID CMyApp::FindFiles(LPCTSTR pszDirName)
{
	TCHAR szDirName[4096] = { 0 };
	lstrcpyn(szDirName, pszDirName, 4096);
	PathAppend(szDirName, _T("*.*"));
	CFileFind finder;
	for (BOOL bResult = finder.FindFile(szDirName); bResult;)
	{
		bResult = finder.FindNextFile();
		CString strPath = finder.GetFilePath();
		if (m_bVerbose) _ftprintf(stdout, _T("%s\n"), strPath);
		if (finder.IsDots()) continue;
		else if (finder.IsDirectory()) FindFiles(strPath);
		else m_arrFileNames.Add(strPath);
	}
	finder.Close();
}

VOID CMyApp::CheckMd5Mklink(LPCTSTR pszMD5, LPCTSTR pszPath)
{
	if (m_bVerbose) _ftprintf(stdout, _T("%s\n"), pszPath);
	CString strPath;
	BOOL bMklink = FALSE;
	m_crtSection.Lock();
	if (m_mapMd5Names.Lookup(pszMD5, strPath))
	{
		bMklink = TRUE;
	}
	else
	{
		m_mapMd5Names.SetAt(pszMD5, pszPath);
	}
	m_crtSection.Unlock();
	if (bMklink) CreateLink(pszPath, strPath);
}

VOID CMyApp::CreateLink(LPCTSTR pszLink, LPCTSTR pszTarget) const
{
	do {
		TCHAR szLinkPath[4096] = { 0 };
		lstrcpyn(szLinkPath, pszLink, 4096);
		lstrcat(szLinkPath, _T(".hardlink_swap"));
		if (!MoveFile(pszLink, szLinkPath))
		{
			_ftprintf(stderr, _T("[WARN]CreateLink -> MoveFile FAIL:\n  %s\n  <<===>> %s\n"), pszLink, szLinkPath);
			break;
		}
		if (!CreateHardLink(pszLink, pszTarget, NULL))
		{
			MoveFile(szLinkPath, pszLink);
			_ftprintf(stderr, _T("[WARN]CreateLink -> CreateHardLink FAIL:\n  %s\n  <<===>> %s\n"), pszLink, pszTarget);
			break;
		}
		DWORD dwAttr = GetFileAttributes(szLinkPath);
		if (dwAttr & FILE_ATTRIBUTE_READONLY)
		{
			dwAttr &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(szLinkPath, dwAttr);
		}
		if (!DeleteFile(szLinkPath))
		{
			_ftprintf(stderr, _T("[WARN]CreateLink -> DeleteFile FAIL:\n  %s\n"), szLinkPath);
		}
		_ftprintf(stdout, _T("[INFO]CreateLink OK:\n  %s\n  <<===>> %s\n"), pszLink, pszTarget);
	} while (0);
}

DWORD WINAPI CMyApp::_SumMD5AndProccess(LPVOID lpParameter)
{
	LPCTSTR pszFileName = (LPCTSTR)lpParameter;
	TCHAR szMD5[33] = { 0 };
	DWORD dwSize = 0;
	try
	{
		HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hFile)
		{
			HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
			if (hMap)
			{
				dwSize = GetFileSize(hFile, NULL);
				LPVOID lpBuff = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
				MD5 md5(lpBuff, dwSize);
#if defined(_UNICODE) || defined(UNICODE)
				mbstowcs(szMD5, md5.toString().c_str(), 32);
#else
				lstrcpyn(szMD5, md5.toString().c_str(), 33);
#endif // defined(_UNICODE) || defined(UNICODE)
				UnmapViewOfFile(lpBuff);
				CloseHandle(hMap);
			}
			CloseHandle(hFile);
			if (dwSize > 0)
			{
				CString strKey;
				strKey.Format(_T("%s|%u"), szMD5, dwSize);
				theApp.CheckMd5Mklink(strKey, pszFileName);
			}
		}
		else
		{
			_ftprintf(stderr, _T("[WARN]Sum MD5 FAIL:\n  %s\n"), pszFileName);
		}
	}
	catch (...)
	{
		_ftprintf(stderr, _T("[WARN]Sum MD5 EXCEPTION:\n  %s\n"), pszFileName);
	}
	return 0;
}

VOID CMyApp::ProcessFiles()
{
	SYSTEM_INFO si = { 0 };
	GetSystemInfo(&si);
	CThreadPool *pThreadPool = new CThreadPool(8);
	for (INT_PTR i(0), nCount(m_arrFileNames.GetSize()); i < nCount; ++i)
	{
		pThreadPool->Run(_SumMD5AndProccess, (LPVOID)(LPCTSTR)m_arrFileNames.GetAt(i));
	}
	pThreadPool->Wait(INFINITE);
	delete pThreadPool;
}

