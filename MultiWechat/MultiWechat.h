#if !defined(AFX_XUNLEITOOL_H__3C79EEAA_B520_4D9B_B0BE_38CCB760C32E__INCLUDED_)
#define AFX_XUNLEITOOL_H__3C79EEAA_B520_4D9B_B0BE_38CCB760C32E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CMultiWechatApp : public CWinApp
{
public:
	CMultiWechatApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_XUNLEITOOL_H__3C79EEAA_B520_4D9B_B0BE_38CCB760C32E__INCLUDED_)
