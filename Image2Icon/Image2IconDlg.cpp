// Image2IconDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Image2Icon.h"
#include <shlwapi.h>
#include "ctrlEx.h"
#include "Image2IconDlg.h"

#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

private:
	CHyperLink m_link;
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImage2IconDlg dialog
static int gs_arrSize[11] = {16, 24, 32, 48, 64, 72, 96, 128, 256, 512, 1024};
static int gs_arrBpp[5] = {4, 8, 16, 24, 32};

CImage2IconDlg::CImage2IconDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImage2IconDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImage2IconDlg)
	m_bppInd = 4;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImage2IconDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImage2IconDlg)
	DDX_Control(pDX, IDC_LIST_IMAGES, m_lsbImages);
	DDX_Radio(pDX, IDC_RADIO4, m_bppInd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImage2IconDlg, CDialog)
	//{{AFX_MSG_MAP(CImage2IconDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, OnButtonConvert)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImage2IconDlg message handlers

BOOL CImage2IconDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_CHECK16))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK24))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK32))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK48))->SetCheck(1);

	m_lsbImages.SetHorizontalExtent(256);
	ModifyStyleEx(0, WS_EX_ACCEPTFILES);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImage2IconDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImage2IconDlg::OnPaint() 
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
HCURSOR CImage2IconDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CImage2IconDlg::OnButtonConvert() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!m_vPathes.empty())
	{
		AfxGetApp()->BeginWaitCursor();

		TCHAR szExeName[MAX_PATH] = {0};
		TCHAR szPngName[MAX_PATH] = {0};
		TCHAR szIcoName[MAX_PATH] = {0};
		
		GetTempPath(MAX_PATH, szExeName);
		GetTempFileName(szExeName, _T("kx-"), 0, szExeName);
		DeleteFile(szExeName);
		PathRenameExtension(szExeName, _T(".exe"));
		theApp.ExtractResource(_T("EXE"), _T("p2i"), szExeName);
		
		GetTempPath(MAX_PATH, szPngName);
		GetTempFileName(szPngName, _T("kx-"), 0, szPngName);
		DeleteFile(szPngName);
		CreateDirectory(szPngName, NULL);
		SaveToPng(szPngName);
		
		GetModuleFileName(NULL, szIcoName, MAX_PATH);
		PathRemoveFileSpec(szIcoName);
		lstrcat(szIcoName, _T("\\Ico-Output"));
		if (!PathFileExists(szIcoName))
		{
			CreateDirectory(szIcoName, NULL);
		}
		
		CString strParam(_T("-noconfirm "));
		for (int nID = IDC_CHECK16; nID <= IDC_CHECK1024; ++nID)
		{
			if (((CButton*)GetDlgItem(nID))->GetCheck() == 1)
			{
				CString strTemp;
				strTemp.Format(_T("-s %d %dbpp "), gs_arrSize[nID - IDC_CHECK16], gs_arrBpp[m_bppInd]);
				strParam += strTemp;
			}
		}
		CString strDir;
		strDir.Format(_T("-i \"%s\" -o \"%s\""), szPngName, szIcoName);
		strParam += strDir;
		theApp.Call(szExeName, strParam, SW_HIDE, TRUE);
		ShellExecute(GetSafeHwnd(), _T("Open"), szIcoName, NULL, NULL, SW_SHOW);
		
		SHFILEOPSTRUCT sfo = {0};
		sfo.hwnd = GetSafeHwnd();
		sfo.wFunc = FO_DELETE;
		sfo.fFlags = FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION;
		sfo.pFrom = szPngName;
		SHFileOperation(&sfo);
		DeleteFile(szExeName);

		AfxGetApp()->EndWaitCursor();
	}
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	
	ImageCodecInfo* pImageCodecInfo = NULL;
	
	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure
	
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure
	
	GetImageEncoders(num, size, pImageCodecInfo);
	
	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}
	
	free(pImageCodecInfo);
	return -1;  // Failure
}

void CImage2IconDlg::SaveToPng(LPCTSTR lpPath)
{
	using namespace Gdiplus;
	CLSID encode;
	GetEncoderClsid(L"image/png", &encode);
	for (std::vector<CString>::iterator i = m_vPathes.begin()
		; i != m_vPathes.end(); ++i)
	{
		if (!PathMatchSpec(*i, _T("*.png")))
		{
			TCHAR szDest[MAX_PATH] = {0};
			lstrcat(szDest, lpPath);
			PathAppend(szDest, PathFindFileName(*i));
			PathRenameExtension(szDest, _T(".png"));
			WCHAR szSrc[MAX_PATH] = {0};
			WCHAR szDst[MAX_PATH] = {0};
			mbstowcs(szSrc, *i, MAX_PATH);
			mbstowcs(szDst, szDest, MAX_PATH);

			Bitmap bmp(szSrc);
			bmp.Save(szDst, &encode, NULL);
		}
		else
		{
			TCHAR szDest[MAX_PATH] = {0};
			lstrcat(szDest, lpPath);
			PathAppend(szDest, PathFindFileName(*i));
			CopyFile(*i, szDest, FALSE);
		}
	}
}

void CImage2IconDlg::OnButtonImport() 
{
	// TODO: Add your control notification handler code here
	CFileDialog openDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT
		, _T("Í¼Æ¬ÎÄ¼þ(*.bmp;*.png;*.jpg;*.jpeg;*.gif)|*.bmp;*.png;*.jpg;*.jpeg;*.gif||"), this);
	TCHAR szPath[MAX_PATH*100] = {0};
	openDlg.m_ofn.lStructSize = 88;
	openDlg.m_ofn.lpstrFile = szPath;
	openDlg.m_ofn.nMaxFile = MAX_PATH*100;

	if (IDOK == openDlg.DoModal())
	{
		POSITION pos = openDlg.GetStartPosition();
		while (NULL != pos)
		{
			m_vPathes.push_back(openDlg.GetNextPathName(pos));
		}
		UpdateList();
	}
}

void CImage2IconDlg::UpdateList()
{
	m_lsbImages.ResetContent();
	for (std::vector<CString>::iterator i = m_vPathes.begin()
		; i != m_vPathes.end(); ++i)
	{
		LPCTSTR pName = PathFindFileName(*i);
		m_lsbImages.AddString(pName);
	}
}

void CImage2IconDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	m_vPathes.clear();
	UpdateList();
}

HBRUSH CImage2IconDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_LIST_IMAGES)
	{
		pDC->SetTextColor(RGB(0,0,128));
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CImage2IconDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFile[MAX_PATH] = {0};
	UINT nCnt = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (int i(0); i < nCnt; ++i)
	{
		ZeroMemory(szFile, sizeof(szFile));
		DragQueryFile(hDropInfo, i, szFile, MAX_PATH);
		if (PathMatchSpec(szFile, _T("*.bmp")) ||
			PathMatchSpec(szFile, _T("*.png")) ||
			PathMatchSpec(szFile, _T("*.gif")) ||
			PathMatchSpec(szFile, _T("*.jpg")) ||
			PathMatchSpec(szFile, _T("*.jpeg")))
		{
			m_vPathes.push_back(szFile);
		}
	}
	DragFinish(hDropInfo);
	UpdateList();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_link.SubclassDlgItem(IDC_STATIC_LINK, this);
	m_link.SetLinkAddress(_T("tencent://Message/?Uin=513793572"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
