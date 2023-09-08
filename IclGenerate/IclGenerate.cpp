// IclGenerate.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "IclGenerate.h"
#include "IclGenerateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIclGenerateApp

BEGIN_MESSAGE_MAP(CIclGenerateApp, CWinApp)
	//{{AFX_MSG_MAP(CIclGenerateApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIclGenerateApp construction

CIclGenerateApp::CIclGenerateApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIclGenerateApp object

CIclGenerateApp theApp;

BOOL CIclGenerateApp::ExtractResource(LPCTSTR lpResType, LPCTSTR lpResName, LPCTSTR lpFileName)
{
	BOOL bRet(FALSE);
	HRSRC hRes = FindResource(NULL, lpResName, lpResType);
	DWORD dwSizeRes = SizeofResource(NULL, hRes);
	if (NULL != hRes)
	{
		HGLOBAL hGlobal = LoadResource(NULL, hRes);
		if (NULL != hGlobal)
		{
			LPVOID lpBuff = LockResource(hGlobal);
			HANDLE hFile = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE != hFile)
			{
				DWORD dwSizeWri(0);
				WriteFile(hFile, lpBuff, dwSizeRes, &dwSizeWri, NULL);
				CloseHandle(hFile);
			}
			UnlockResource(hGlobal);
		}
	}
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CIclGenerateApp initialization

BOOL CIclGenerateApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SetDialogBkColor(RGB(244, 246, 248));
	CIclGenerateDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
