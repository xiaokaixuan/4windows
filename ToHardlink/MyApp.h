// MyApp.h: interface for the CMyApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYAPP_H__063550FA_4EE1_4AF0_BA5C_FD65AE0B6E37__INCLUDED_)
#define AFX_MYAPP_H__063550FA_4EE1_4AF0_BA5C_FD65AE0B6E37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyApp : public CWinApp  
{
public:
	CMyApp();
	virtual ~CMyApp();
public:
	VOID SetVerbose(BOOL bVerbose = TRUE);

public:
	VOID FindFiles(LPCTSTR pszDirName);
	VOID CreateLink(LPCTSTR pszTarget, LPCTSTR pszLink) const;
	VOID CheckMd5Mklink(LPCTSTR pszMD5, LPCTSTR pszPath);

	VOID ProcessFiles();

public:
	static DWORD WINAPI _SumMD5AndProccess(LPVOID lpParameter);

private:
	CMapStringToString m_mapMd5Names;
	CStringArray m_arrFileNames;
	CCriticalSection m_crtSection;
	BOOL m_bVerbose;
};

extern CMyApp theApp;

#endif // !defined(AFX_MYAPP_H__063550FA_4EE1_4AF0_BA5C_FD65AE0B6E37__INCLUDED_)
