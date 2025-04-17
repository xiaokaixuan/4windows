#include "StdAfx.h"
#include "MultiWechat.h"
#include "MultiWechatDlg.h"

BEGIN_MESSAGE_MAP(CMultiWechatApp, CWinApp)
END_MESSAGE_MAP()


CMultiWechatApp::CMultiWechatApp()
{
}

CMultiWechatApp theApp;

BOOL CMultiWechatApp::InitInstance()
{
	AfxEnableControlContainer();
	
	//SetDialogBkColor(RGB(255, 255, 255));

	CMultiWechatDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	return FALSE;
}
