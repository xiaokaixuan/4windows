// Icon ExtractorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Icon Extractor.h"
#include "AutoFont.h"
#include <shlwapi.h>
#include "AboutDlg.h"
#include "ExAPI.h"
#include "Icon ExtractorDlg.h"

#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconExtractorDlg dialog

CIconExtractorDlg::CIconExtractorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIconExtractorDlg::IDD, pParent)
	, m_pFont(NULL)
	, m_pImgList(NULL)
{
	//{{AFX_DATA_INIT(CIconExtractorDlg)
	m_strFile = _T("C:\\Windows\\system32\\shell32.dll");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_popMenu.LoadMenu(IDR_POPMENU);
}

CIconExtractorDlg::~CIconExtractorDlg()
{
	if (NULL != m_pImgList)
	{
		delete m_pImgList;
		m_pImgList = NULL;
	}
	if (NULL != m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}

void CIconExtractorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIconExtractorDlg)
	DDX_Control(pDX, IDC_LIST_ICONS, m_lscIcons);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIconExtractorDlg, CDialog)
	//{{AFX_MSG_MAP(CIconExtractorDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ICONS, OnRclickListIcons)
	ON_COMMAND(ID_MENUITEM_REFRESH, OnMenuitemRefresh)
	ON_COMMAND(ID_MENUITEM_SELECTALL, OnMenuitemSelectall)
	ON_COMMAND(ID_MENUITEM_EXTRACT, OnMenuitemExtract)
	ON_COMMAND(ID_MENUITEM_ABOUT, OnMenuitemAbout)
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconExtractorDlg message handlers

BOOL CIconExtractorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_btnBrowse.SubclassDlgItem(IDC_BUTTON_BROWSE, this);
	m_btnBrowse.SetBitmaps(IDB_BITMAP_UP, IDB_BITMAP_DOWN, IDB_BITMAP_OVER, 0);
	m_btnBrowse.SizeToContent();

	m_pImgList = new CImageList;
	m_pImgList->Create(32, 32, ILC_COLOR32 | ILC_MASK, 0, 1);
	m_lscIcons.SetImageList(m_pImgList, LVSIL_NORMAL);
	_Resize();

	_ParseFile();
	_UpdateList();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIconExtractorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIconExtractorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CIconExtractorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	DragAcceptFiles(TRUE);

	m_stbBar.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 0);
	int strPartDim[2]= {300, -1};
	m_stbBar.SetParts(2, strPartDim);

	CString strText, strStatus;
	strText.LoadString(IDS_ICONUM);
	strStatus.Format(_T("%s%d"), strText, 0);
	m_stbBar.SetText(strStatus, 0, 0);
	CString strAuthor;
	strAuthor.LoadString(IDS_AUTHOR);
	m_stbBar.SetText(strAuthor, 1, 0);

	m_pFont = new CAutoFont(*m_stbBar.GetFont());
	return 0;
}

void CIconExtractorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	_Resize();	
}

void CIconExtractorDlg::OnOK() 
{
	// TODO: Add extra validation here
}

BOOL CIconExtractorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
    {
        return TRUE;
    } 
	return CDialog::PreTranslateMessage(pMsg);
}

void CIconExtractorDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, _T("动态链接库（*.dll）|*.dll|可执行文件（*.exe）|*.exe|图标库文件（*.icl）|*.icl|所有支持文件|*.dll;*.exe;*.icl||"), this);
	dlg.m_ofn.lStructSize = 88;
	if (IDOK == dlg.DoModal())
	{
		m_strFile = dlg.GetPathName();
		UpdateData(FALSE);
		_Clear();
		_ParseFile();
		_UpdateList();
	}
}

void CIconExtractorDlg::_Clear()
{
	if (NULL != m_pImgList)
	{
		m_pImgList->DeleteImageList();
		delete m_pImgList;
		m_pImgList = new CImageList;
		m_pImgList->Create(32, 32, ILC_COLOR32 | ILC_MASK, 0, 1);
		m_lscIcons.SetImageList(m_pImgList, LVSIL_NORMAL);
	}
	m_vIcoRes.clear();
}


BOOL CALLBACK CIconExtractorDlg::EnumResNameProc(
												 HMODULE hModule,   // module handle
												 LPCTSTR lpszType,  // resource type
												 LPTSTR lpszName,   // resource name
												 LONG lParam    // application-defined parameter
												 )
{
	CIconExtractorDlg *pthis = (CIconExtractorDlg*)lParam;
	pthis->m_pImgList->Add(::LoadIcon(hModule, lpszName));
	pthis->m_vIcoRes.push_back(lpszName);
	return TRUE;
}

void CIconExtractorDlg::_ParseFile()
{
	AfxGetApp()->BeginWaitCursor();

	HMODULE hFile = LoadLibrary(m_strFile);
	EnumResourceNames(hFile, RT_GROUP_ICON,EnumResNameProc,(LONG)this);

	AfxGetApp()->EndWaitCursor();
}

void CIconExtractorDlg::_UpdateList()
{
	m_lscIcons.DeleteAllItems();
	int nCnt = m_pImgList->GetImageCount();
	CString strText, strStatus;
	strText.LoadString(IDS_ICONUM);
	strStatus.Format(_T("%s%d"), strText, nCnt);
	m_stbBar.SetText(strStatus, 0, 0);

	for (int i(0); i < nCnt; ++i)
	{
		CString str;
		str.Format(_T("%d"), i);
		m_lscIcons.InsertItem(i, str, i);
		m_lscIcons.SetItemData(i, (DWORD)m_vIcoRes[i]);
	}

}

HBRUSH CIconExtractorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_PATH)
	{
		if (NULL != m_pFont)
		{
			pDC->SelectObject(m_pFont);
			pDC->SetBkMode(TRANSPARENT);
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CIconExtractorDlg::OnRclickListIcons(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	POINT pt = {0};
	GetCursorPos(&pt);
	m_popMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
}

void CIconExtractorDlg::OnMenuitemRefresh() 
{
	// TODO: Add your command handler code here
	_UpdateList();
}

void CIconExtractorDlg::OnMenuitemSelectall() 
{
	// TODO: Add your command handler code here
	int nCnt = m_lscIcons.GetItemCount();
	for (int i(0); i < nCnt; ++i)
	{
		m_lscIcons.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CIconExtractorDlg::OnMenuitemExtract() 
{
	// TODO: Add your command handler code here
	int nSucc(0);

	int nCnt = m_lscIcons.GetSelectedCount();
	if (1 == nCnt)
	{
		POSITION pos = m_lscIcons.GetFirstSelectedItemPosition();
		int nItem = m_lscIcons.GetNextSelectedItem(pos);
		CString strName;
		strName = m_lscIcons.GetItemText(nItem, 0);
		strName += _T(".ico");

		CFileDialog dlg(FALSE, _T(".ico"), strName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("图标（*.ico）|*.ico||"), this);
		dlg.m_ofn.lStructSize = 88;
		if (IDOK == dlg.DoModal())
		{
			LPTSTR pszName = (LPTSTR)m_lscIcons.GetItemData(nItem);
			if (kxSaveIcon(m_strFile, pszName, dlg.GetPathName()))
			{
				nSucc += 1;
			}
		}
	}
	else if (nCnt > 1)
	{
		TCHAR szPath[MAX_PATH] = {0};
		BROWSEINFO bsi = {0};
		bsi.hwndOwner = GetSafeHwnd();
		bsi.pszDisplayName = szPath;
		bsi.ulFlags = 0x00000040;

		LPITEMIDLIST lpiil = SHBrowseForFolder(&bsi);
		if (NULL != lpiil)
		{
			SHGetPathFromIDList(lpiil, szPath);
			POSITION pos = m_lscIcons.GetFirstSelectedItemPosition();
			int nItem(-1);

			while (NULL != pos)
			{
				nItem = m_lscIcons.GetNextSelectedItem(pos);

				CString strName = m_lscIcons.GetItemText(nItem, 0);
				strName += _T(".ico");
				TCHAR szFilePath[MAX_PATH] = {0};
				lstrcpy(szFilePath, szPath);
				PathAppend(szFilePath, strName);

				LPTSTR pszName = (LPTSTR)m_lscIcons.GetItemData(nItem);
				if (kxSaveIcon(m_strFile, pszName, szFilePath))
				{
					nSucc += 1;
				}
			}
		}
	}
	if (nSucc > 0)
	{
		CString strMsg;
		strMsg.Format(_T("成功提取 %d 个图标！"), nSucc);
		MessageBox(strMsg, _T("温馨提示"), MB_OK | MB_ICONINFORMATION);
	}
	
}

void CIconExtractorDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFileName[MAX_PATH] = {0};
	DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
	if (PathMatchSpec(szFileName, _T("*.exe")) ||
		PathMatchSpec(szFileName, _T("*.dll")) ||
		PathMatchSpec(szFileName, _T("*.icl")))
	{
		m_strFile = szFileName;
		UpdateData(FALSE);
		_Clear();
		_ParseFile();
		_UpdateList();
	}
}
void CIconExtractorDlg::OnMenuitemAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CIconExtractorDlg::_Resize()
{
	if (IsWindow(m_stbBar.GetSafeHwnd()))
	{
		CRect cRc, rcStatus;
		GetClientRect(&cRc);
		m_stbBar.GetWindowRect(&rcStatus);
		ScreenToClient(&rcStatus);
		rcStatus.right = cRc.right;
		
		rcStatus.OffsetRect(CSize(0, cRc.bottom - rcStatus.bottom));
		m_stbBar.MoveWindow(&rcStatus);
		
		int strPartDim[2]= {cRc.Width()/3*2, -1};
		m_stbBar.SetParts(2, strPartDim);
		
		if (IsWindow(m_lscIcons.GetSafeHwnd()))
		{
			CRect rcLsc;
			m_lscIcons.GetWindowRect(&rcLsc);
			ScreenToClient(&rcLsc);
			rcLsc.left = 0;
			rcLsc.right = cRc.right;
			rcLsc.bottom = rcStatus.top - 10;
			m_lscIcons.MoveWindow(&rcLsc);
			_UpdateList();
		}
	}
	if (IsWindow(GetDlgItem(IDC_BUTTON_BROWSE)->GetSafeHwnd()) &&
		IsWindow(GetDlgItem(IDC_EDIT_PATH)->GetSafeHwnd()))
	{
		CRect rcBtn, rcEdt;
		CRect cRc;
		GetClientRect(&cRc);
		GetDlgItem(IDC_BUTTON_BROWSE)->GetWindowRect(&rcBtn);
		GetDlgItem(IDC_EDIT_PATH)->GetWindowRect(&rcEdt);
		ScreenToClient(&rcBtn);
		ScreenToClient(&rcEdt);
		rcBtn.OffsetRect(CSize(cRc.right - 10 - rcBtn.right, 0));
		rcEdt.right = rcBtn.left - 10;
		GetDlgItem(IDC_BUTTON_BROWSE)->MoveWindow(&rcBtn);
		GetDlgItem(IDC_EDIT_PATH)->MoveWindow(&rcEdt);
	}
}
