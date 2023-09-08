// Image2Icon.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Image2Icon.h"
#include "SkinH.h"
#include "Image2IconDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImage2IconApp

BEGIN_MESSAGE_MAP(CImage2IconApp, CWinApp)
	//{{AFX_MSG_MAP(CImage2IconApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImage2IconApp construction

CImage2IconApp::CImage2IconApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CImage2IconApp object

CImage2IconApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CImage2IconApp initialization

BOOL CImage2IconApp::InitInstance()
{
	setlocale(LC_ALL,".936");
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
	SkinH_Init(AfxGetInstanceHandle());
	SkinH_AttachResEx(_T("pf"), _T("SHE"), NULL, 0, 0, 0);

	using namespace Gdiplus;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR pgdiToken = NULL;
	GdiplusStartup(&pgdiToken,&gdiplusStartupInput,NULL);

	SetDialogBkColor(RGB(220, 230, 244));
	CImage2IconDlg dlg;
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

	GdiplusShutdown(pgdiToken);
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CImage2IconApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	SkinH_Free();
	return CWinApp::ExitInstance();
}

BOOL CImage2IconApp::ExtractResource(LPCTSTR lpResType, LPCTSTR lpResName, LPCTSTR lpFileName)
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
			HANDLE hFile = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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

HANDLE CImage2IconApp::Call(LPCTSTR lpFile, LPCTSTR lpParameters, int nShow, BOOL bWait)
{
	HANDLE Handle(NULL);
	SHELLEXECUTEINFO sif = {0};
	sif.cbSize = sizeof(sif);
	sif.fMask = SEE_MASK_NOCLOSEPROCESS;
	sif.lpFile = lpFile;
	sif.nShow = nShow;
	sif.lpVerb = _T("Open");
	sif.lpParameters = lpParameters;
	if (ShellExecuteEx(&sif))
	{
		if (bWait)
		{
			::WaitForSingleObject(sif.hProcess, INFINITE);
			::CloseHandle(sif.hProcess);
		}
		else
		{
			Handle = sif.hProcess;
		}
	}
	return Handle;
}