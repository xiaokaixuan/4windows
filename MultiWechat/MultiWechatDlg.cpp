#include "StdAfx.h"
#include "MultiWechat.h"
#include "MultiWechatDlg.h"
#include "extended.h"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

CMultiWechatDlg::CMultiWechatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiWechatDlg::IDD, pParent)
	, m_nNumber(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiWechatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, m_edtInfo);
	DDX_Control(pDX, IDC_STATIC_HTTP, m_sttHttp);
}

BEGIN_MESSAGE_MAP(CMultiWechatDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CMultiWechatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	SetTimer(IDT_FINDER, 500, NULL);

	return TRUE;
}

void CMultiWechatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CMultiWechatDlg::OnOK() 
{
}

void CMultiWechatDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == IDT_FINDER)
	{
	 	KillTimer(nIDEvent);
		this->_FindWechat();
	}
	CDialog::OnTimer(nIDEvent);
}

void CMultiWechatDlg::_FindWechat()
{
	HKEY hKey(0);
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Tencent\\WeChat"), 
		0, 0x0200 | KEY_READ, &hKey))
	{
		DWORD dwSize(2048), dwType(0);
		LPTSTR lpValue = m_strWechat.GetBuffer(dwSize);
		RegQueryValueEx(hKey, _T("InstallPath"), 0, &dwType, LPBYTE(lpValue), &dwSize);
		PathAppend(lpValue, _T("WeChat.exe"));
		m_strWechat.ReleaseBuffer();
		RegCloseKey(hKey);
	}
	if (m_strWechat.IsEmpty() || !PathFileExists(m_strWechat))
	{
		if (IDYES == MessageBox(_T("警告：未找到WeChat.exe，是否手动查找？"), 
			_T("微信(Wechat)多开器 v1.0"), MB_YESNO | MB_ICONQUESTION))
		{
			CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
				_T("WeChat.exe|WeChat.exe||"), this);
			dlg.m_ofn.lStructSize = 88;
			if (IDOK == dlg.DoModal())
			{
				m_strWechat = dlg.GetPathName();
			}
			else EndDialog(IDCANCEL);
		}
		else EndDialog(IDCANCEL);
	}
	CString strTip;
	strTip.Format(_T("%d、微信已安装：%s\r\n"), m_nNumber++, m_strWechat.GetString());
	m_edtInfo.SetSel(m_edtInfo.GetWindowTextLength(), m_edtInfo.GetWindowTextLength());
	m_edtInfo.ReplaceSel(strTip);
	m_edtInfo.SetSel(m_edtInfo.GetWindowTextLength(), m_edtInfo.GetWindowTextLength());
}

void CMultiWechatDlg::_Process()
{
	int nReturn = PatchWeChat();
	CString strTip;
	if (0 == nReturn)
	{
		strTip.Format(_T("%d、已存在微信进程，正在破解进程互斥...\r\n"), m_nNumber++);
		m_edtInfo.SetSel(m_edtInfo.GetWindowTextLength(), m_edtInfo.GetWindowTextLength());
		m_edtInfo.ReplaceSel(strTip);
	}
	strTip.Format(_T("%d、正在启动微信(Wechat)中...\r\n"), m_nNumber++);
	m_edtInfo.SetSel(m_edtInfo.GetWindowTextLength(), m_edtInfo.GetWindowTextLength());
	m_edtInfo.ReplaceSel(strTip);
	if ((INT)ShellExecute(GetSafeHwnd(), _T("Open"), m_strWechat, NULL, NULL, SW_SHOW) > 32)
	{
		strTip.Format(_T("%d、微信(Wechat)成功启动！！！\r\n"), m_nNumber++);
		m_edtInfo.SetSel(m_edtInfo.GetWindowTextLength(), m_edtInfo.GetWindowTextLength());
		m_edtInfo.ReplaceSel(strTip);
	}
}

void CMultiWechatDlg::OnButtonStart()
{
	CWaitCursor wait;
	this->_Process();
}

HBRUSH CMultiWechatDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 设置对话框的背景颜色
	//if (nCtlColor == CTLCOLOR_DLG)
	{
		// 例如，设置为浅灰色
		pDC->SetBkColor(RGB(255, 255, 255));
		// 创建并返回一个浅灰色画刷
		return (HBRUSH)GetStockObject(WHITE_BRUSH); // 或者 CreateSolidBrush(RGB(240, 240, 240));
	}
	return hbr;
}
