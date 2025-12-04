// Icon Extractor.h : main header file for the ICON EXTRACTOR application
//

#if !defined(AFX_ICONEXTRACTOR_H__4F3263F7_0F56_40BA_ACCF_8B5FC958F4BB__INCLUDED_)
#define AFX_ICONEXTRACTOR_H__4F3263F7_0F56_40BA_ACCF_8B5FC958F4BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIconExtractorApp:
// See Icon Extractor.cpp for the implementation of this class
//

class CIconExtractorApp : public CWinApp
{
public:
	CIconExtractorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconExtractorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIconExtractorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONEXTRACTOR_H__4F3263F7_0F56_40BA_ACCF_8B5FC958F4BB__INCLUDED_)
