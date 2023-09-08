// IclGenerate.h : main header file for the ICLGENERATE application
//

#if !defined(AFX_ICLGENERATE_H__0EDD9327_AF9C_4DAB_A709_E6963B87A742__INCLUDED_)
#define AFX_ICLGENERATE_H__0EDD9327_AF9C_4DAB_A709_E6963B87A742__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIclGenerateApp:
// See IclGenerate.cpp for the implementation of this class
//

class CIclGenerateApp : public CWinApp
{
public:
	CIclGenerateApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIclGenerateApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
	BOOL ExtractResource(LPCTSTR lpResType, LPCTSTR lpResName, LPCTSTR lpFileName);

// Implementation

	//{{AFX_MSG(CIclGenerateApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CIclGenerateApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICLGENERATE_H__0EDD9327_AF9C_4DAB_A709_E6963B87A742__INCLUDED_)
