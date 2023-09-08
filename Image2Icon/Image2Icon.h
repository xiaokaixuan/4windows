// Image2Icon.h : main header file for the IMAGE2ICON application
//

#if !defined(AFX_IMAGE2ICON_H__4F0F9069_F2C1_4E67_98A8_E6EFB916EF57__INCLUDED_)
#define AFX_IMAGE2ICON_H__4F0F9069_F2C1_4E67_98A8_E6EFB916EF57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImage2IconApp:
// See Image2Icon.cpp for the implementation of this class
//

class CImage2IconApp : public CWinApp
{
public:
	BOOL ExtractResource(LPCTSTR lpResType, LPCTSTR lpResName, LPCTSTR lpFileName);
	HANDLE Call(LPCTSTR lpFile, LPCTSTR lpParameters, int nShow, BOOL bWait);
	CImage2IconApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage2IconApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImage2IconApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CImage2IconApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE2ICON_H__4F0F9069_F2C1_4E67_98A8_E6EFB916EF57__INCLUDED_)
