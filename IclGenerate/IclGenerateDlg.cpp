// IclGenerateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IclGenerate.h"
#include "IclGenerateDlg.h"
#include "ExAPI.h"
#include "AboutDlg.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIclGenerateDlg dialog

CIclGenerateDlg::CIclGenerateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIclGenerateDlg::IDD, pParent)
	, m_pImgList(new CImageList)
{
	//{{AFX_DATA_INIT(CIclGenerateDlg)
	m_strPath = _T("");
	m_bSub = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_popMenu.LoadMenu(IDR_POPMENU);
}
CIclGenerateDlg::~CIclGenerateDlg()
{
	if (NULL != m_pImgList)
	{
		delete m_pImgList;
		m_pImgList = NULL;
	}
	for (std::vector<CString*>::iterator i = m_vpStrs.begin()
		; i != m_vpStrs.end(); ++i)
	{
		delete *i;
	}
	m_vpStrs.clear();
}

void CIclGenerateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIclGenerateDlg)
	DDX_Control(pDX, IDC_CHECK_SUB, m_btnSub);
	DDX_Control(pDX, IDC_LIST_ICON, m_lscIcon);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
	DDX_Check(pDX, IDC_CHECK_SUB, m_bSub);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIclGenerateDlg, CDialog)
	//{{AFX_MSG_MAP(CIclGenerateDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_CHECK_SUB, OnCheckSub)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ICON, OnRclickListIcon)
	ON_COMMAND(ID_MENUITEM_REFIND, OnMenuitemRefind)
	ON_COMMAND(ID_MENUITEM_ALLSELECT, OnMenuitemAllselect)
	ON_COMMAND(ID_MENUITEM_EXPORT, OnMenuitemExport)
	ON_COMMAND(ID_MENUITEM_SAVEAS, OnMenuitemSaveas)
	ON_COMMAND(ID_MENUITEM_ABOUT, OnMenuitemAbout)
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIclGenerateDlg message handlers

BOOL CIclGenerateDlg::OnInitDialog()
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

	m_pImgList->Create(32, 32, ILC_COLOR32 | ILC_MASK, 0, 1);
	m_lscIcon.SetImageList(m_pImgList, LVSIL_NORMAL);

	_Resize();

	StartFind();
	UpdateList();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIclGenerateDlg::OnPaint() 
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
HCURSOR CIclGenerateDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIclGenerateDlg::OnOK() 
{
	// TODO: Add extra validation here
}

BOOL CIclGenerateDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

int CIclGenerateDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_sttCtrl.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM,
		CRect(0,0,0,0), this, IDC_STATUS_BAR);
	int arrParts[] = {300, -1};
	m_sttCtrl.SetParts(sizeof(arrParts)/sizeof(arrParts[0]), arrParts);

	CString strNum;
	strNum.LoadString(IDS_ICONNUM);
	m_sttCtrl.SetText(strNum, 0, 0);

	CString strAuthor;
	strAuthor.LoadString(IDS_AUTHOR);
	m_sttCtrl.SetText(strAuthor, 0, 1);

	DragAcceptFiles(TRUE);
	
	return 0;
}

HBRUSH CIclGenerateDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CIclGenerateDlg::_Resize()
{
	if (IsWindow(m_sttCtrl.GetSafeHwnd()))
	{
		CRect sttRc, cRc;
		m_sttCtrl.GetWindowRect(&sttRc);
		ScreenToClient(&sttRc);
		GetClientRect(&cRc);
		sttRc.right = cRc.right;

		sttRc.OffsetRect(0, cRc.bottom - sttRc.bottom);
		m_sttCtrl.MoveWindow(&sttRc);

		int arrParts[] = {cRc.Width()/3*2, -1};
		m_sttCtrl.SetParts(sizeof(arrParts)/sizeof(arrParts[0]), arrParts);

		if (IsWindow(m_lscIcon.GetSafeHwnd()))
		{
			CRect lscRc, cRc;
			m_lscIcon.GetWindowRect(&lscRc);
			ScreenToClient(&lscRc);
			
			GetClientRect(&cRc);
			lscRc.left = cRc.left;
			lscRc.right = cRc.right;
			
			lscRc.bottom = sttRc.top - 10;
			m_lscIcon.MoveWindow(&lscRc);
			
			UpdateList();
		}
	}
	if (IsWindow(m_btnSub.GetSafeHwnd()))
	{
		CRect subRc, cRc;
		m_btnSub.GetWindowRect(&subRc);
		ScreenToClient(&subRc);
		GetClientRect(&cRc);
		subRc.OffsetRect(cRc.right - 10 - subRc.right, 0);
		m_btnSub.MoveWindow(&subRc);

		if (IsWindow(m_btnBrowse.GetSafeHwnd()))
		{
			CRect browseRc;
			m_btnBrowse.GetWindowRect(&browseRc);
			ScreenToClient(&browseRc);
			browseRc.OffsetRect(subRc.left - 10 - browseRc.right, 0);
			m_btnBrowse.MoveWindow(&browseRc);

			if (IsWindow(GetDlgItem(IDC_EDIT_PATH)->GetSafeHwnd()))
			{
				CRect edtRc;
				GetDlgItem(IDC_EDIT_PATH)->GetWindowRect(&edtRc);
				ScreenToClient(&edtRc);
				edtRc.right = browseRc.left - 10;
				GetDlgItem(IDC_EDIT_PATH)->MoveWindow(&edtRc);
			}
 		}

		
	}
	
}

void CIclGenerateDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	_Resize();
}

void CIclGenerateDlg::_FindIcon(LPCTSTR lpPath)
{
	if (lstrlen(lpPath) > 0)
	{
		TCHAR szFind[MAX_PATH] = {0};
		lstrcpy(szFind, lpPath);
		PathAppend(szFind, _T("*.*"));
		
		CFileFind finder;
		BOOL bFind = finder.FindFile(szFind, 0);
		while (bFind)
		{
			bFind = finder.FindNextFile();
			if (!finder.IsDots() && finder.IsDirectory() && m_bSub)
			{
				_FindIcon(finder.GetFilePath());
			}
			else if (PathMatchSpec(finder.GetFilePath(), _T("*.ico")))
			{
				HICON hIcon= (HICON)::LoadImage(NULL, finder.GetFilePath(), IMAGE_ICON
					, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE | LR_SHARED);
				if (hIcon != NULL)
				{
					m_pImgList->Add(hIcon);
					m_vpStrs.push_back(new CString(finder.GetFilePath()));
				}
			}
		}
	}
}

void CIclGenerateDlg::_Restore()
{
	if (NULL != m_pImgList)
	{
		m_pImgList->DeleteImageList();
		delete m_pImgList;
		m_pImgList = new CImageList;
		m_pImgList->Create(32, 32, ILC_COLOR32 | ILC_MASK, 0, 1);
	}
	else
	{
		m_pImgList = new CImageList;
		m_pImgList->Create(32, 32, ILC_COLOR32 | ILC_MASK, 0, 1);
	}
	m_lscIcon.SetImageList(m_pImgList, LVSIL_NORMAL);
	for (std::vector<CString*>::iterator i = m_vpStrs.begin()
		; i != m_vpStrs.end(); ++i)
	{
		delete *i;
	}
	m_vpStrs.clear();
}

void CIclGenerateDlg::StartFind()
{
	AfxGetApp()->BeginWaitCursor();

	_Restore();
	_FindIcon(m_strPath);

	AfxGetApp()->EndWaitCursor();
}

void CIclGenerateDlg::UpdateList()
{
	m_lscIcon.DeleteAllItems();

	int nCnt = m_pImgList->GetImageCount();
	CString str;
	str.LoadString(IDS_ICONNUM);
	CString strStt;
	strStt.Format(_T("%s%d"), str, nCnt);
	m_sttCtrl.SetText(strStt, 0, 0);

	for (int i(0); i < nCnt; ++i)
	{
		CString str;
		str.Format(_T("%d"), i);
		m_lscIcon.InsertItem(i, str, i);
		m_lscIcon.SetItemData(i, (DWORD)m_vpStrs[i]);
	}
}

void CIclGenerateDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	TCHAR szPath[MAX_PATH] = {0};
	BROWSEINFO bsi = {0};
	bsi.hwndOwner = GetSafeHwnd();
	bsi.pszDisplayName = szPath;

	LPITEMIDLIST liil = SHBrowseForFolder(&bsi);
	if (NULL != liil)
	{
		if (SHGetPathFromIDList(liil, szPath))
		{
			m_strPath = szPath;
			UpdateData(FALSE);

			StartFind();
			UpdateList();
		}
	}
}

void CIclGenerateDlg::OnCheckSub()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	StartFind();
	UpdateList();
}

void CIclGenerateDlg::OnRclickListIcon(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	POINT pt = {0};
	::GetCursorPos(&pt);
	m_popMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTALIGN, pt.x, pt.y, this);
}

void CIclGenerateDlg::OnMenuitemRefind()
{
	// TODO: Add your command handler code here
	UpdateData(TRUE);
	StartFind();
	UpdateList();
}

void CIclGenerateDlg::OnMenuitemAllselect() 
{
	// TODO: Add your command handler code here
	int nCnt = m_lscIcon.GetItemCount();
	for (int i(0); i < nCnt; ++i)
	{
		m_lscIcon.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CIclGenerateDlg::OnMenuitemExport()
{
	// TODO: Add your command handler code here
	if (m_lscIcon.GetSelectedCount() > 0)
	{
		CFileDialog dlg(FALSE, _T(".icl"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
			, _T("图标库文件（*.icl）|*.icl|动态链接库（*.dll）|*.dll|可执行文件（*.exe）|*.exe||")
			, this);
		dlg.m_ofn.lStructSize = 88;

		if (IDOK == dlg.DoModal())
		{
			::AfxGetApp()->BeginWaitCursor();

			CString strFile = dlg.GetPathName();
			theApp.ExtractResource(_T("Template"), _T("Normal"), strFile);
			
			int nExp(0);
			POSITION pos = m_lscIcon.GetFirstSelectedItemPosition();
			while (NULL != pos)
			{
				int nItem = m_lscIcon.GetNextSelectedItem(pos);
				CString *pStr = (CString*)m_lscIcon.GetItemData(nItem);
				if (nExp < 2500)
				{
					if (kxInsertIcon(strFile, *pStr, MAKEINTRESOURCE(nItem + 1)))
					{
						nExp += 1;
					}
				}
				else
				{
					break;
				}
			}

			::AfxGetApp()->EndWaitCursor();

			CString strMsg;
			strMsg.Format(_T("成功导出 %d 个图标！"), nExp);
			MessageBox(strMsg, _T("温馨提醒"), MB_OK | MB_ICONINFORMATION);
		}
	}
}

void CIclGenerateDlg::OnMenuitemSaveas()
{
	// TODO: Add your command handler code here
	POSITION pos = m_lscIcon.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = m_lscIcon.GetNextSelectedItem(pos);
		CString *pStr = (CString*)m_lscIcon.GetItemData(nItem);
		CString strName = m_lscIcon.GetItemText(nItem, 0);
		CFileDialog dlg(FALSE, _T(".ico"), strName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
			, _T("图标文件（*.ico）|*.ico||")
			, this);
		dlg.m_ofn.lStructSize = 88;
		if (IDOK == dlg.DoModal())
		{
			CopyFile(*pStr, dlg.GetPathName(), FALSE);
		}
	}
}

void CIclGenerateDlg::OnDropFiles(HDROP hDrop)
{
	TCHAR szPath[MAX_PATH] = {0};
	if (DragQueryFile(hDrop, 0, szPath, MAX_PATH))
	{
		if (PathIsDirectory(szPath))
		{
			m_strPath = szPath;
			UpdateData(FALSE);
			StartFind();
			UpdateList();
		}
	}
}

void CIclGenerateDlg::OnMenuitemAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}
