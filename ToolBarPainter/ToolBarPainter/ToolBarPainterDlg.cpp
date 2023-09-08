
// ToolBarPainterDlg.cpp : implementation file
//
#include "stdafx.h"
#include "ToolBarPainter.h"
#include "ToolBarPainterDlg.h"
#include "afxdialogex.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolBarPainterDlg dialog

static int sg_arrSizes[7] = {16, 24, 32, 48, 72, 128, 256};
static COLORREF sg_arrCls[5] = {RGB(255,255,255), RGB(0,0,0), RGB(255,0,0), RGB(0,255,0), RGB(0,0,255)};

CToolBarPainterDlg::CToolBarPainterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CToolBarPainterDlg::IDD, pParent)
	, m_cmbIndex(2)
	, m_cmbClIndx(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_imgHot.Create(32, 32, ILC_COLOR24 | ILC_MASK, 6, 1);
	m_imgGray.Create(32, 32, ILC_COLOR24 | ILC_MASK, 6, 1);
	CBitmap bmpHot,bmpGray;
	bmpHot.LoadBitmap(IDB_BITMAP_HOT);
	bmpGray.LoadBitmap(IDB_BITMAP_GRAY);
	m_imgHot.Add(&bmpHot, RGB(255, 255, 255));
	m_imgGray.Add(&bmpGray, RGB(255, 255, 255));

	m_pImgIcons = new CImageList;
	m_pImgIcons->Create(sg_arrSizes[m_cmbIndex], sg_arrSizes[m_cmbIndex], ILC_COLOR24 | ILC_MASK, 0, 1);
}

CToolBarPainterDlg::~CToolBarPainterDlg()
{
	if (NULL != m_pImgIcons)
	{
		delete m_pImgIcons;
		m_pImgIcons = NULL;
	}
}

void CToolBarPainterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_SIZE, m_cmbIndex);
	DDX_CBIndex(pDX, IDC_COMBO_COLOR, m_cmbClIndx);
	DDX_Control(pDX, IDC_LIST_ICONS, m_lscIcons);
}

BEGIN_MESSAGE_MAP(CToolBarPainterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ADD, &CToolBarPainterDlg::OnUpdateButtonAdd)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ABOUT, &CToolBarPainterDlg::OnUpdateButtonAbout)
	ON_COMMAND(ID_BUTTON_ABOUT, &CToolBarPainterDlg::OnButtonAbout)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, &CToolBarPainterDlg::OnSelchangeComboSize)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR, &CToolBarPainterDlg::OnSelchangeComboColor)
	ON_COMMAND(ID_BUTTON_ADD, &CToolBarPainterDlg::OnButtonAdd)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LEFT, &CToolBarPainterDlg::OnUpdateButtonLeft)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CLEAR, &CToolBarPainterDlg::OnUpdateButtonClear)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DEL, &CToolBarPainterDlg::OnUpdateButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CToolBarPainterDlg::OnClickedButtonSave)
	ON_COMMAND(ID_POP_SAVENORMALBAR, &CToolBarPainterDlg::OnPopSavenormalbar)
	ON_COMMAND(ID_POP_SAVEGRAYBAR, &CToolBarPainterDlg::OnPopSavegraybar)
	ON_COMMAND(ID_POP_SAVEALLBARS, &CToolBarPainterDlg::OnPopSaveallbars)
	ON_COMMAND(ID_BUTTON_CLEAR, &CToolBarPainterDlg::OnButtonClear)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RIGHT, &CToolBarPainterDlg::OnUpdateButtonRight)
	ON_COMMAND(ID_BUTTON_DEL, &CToolBarPainterDlg::OnButtonDel)
	ON_COMMAND(ID_BUTTON_LEFT, &CToolBarPainterDlg::OnButtonLeft)
	ON_COMMAND(ID_BUTTON_RIGHT, &CToolBarPainterDlg::OnButtonRight)
END_MESSAGE_MAP()

// CToolBarPainterDlg message handlers

BOOL CToolBarPainterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_lscIcons.SetImageList(m_pImgIcons, LVSIL_NORMAL);
	m_lscIcons.SetIconSpacing(32, 32);

	m_bwHot.SubclassDlgItem(IDC_CUSTOM_HOT, this);
	m_bwGray.SubclassDlgItem(IDC_CUSTOM_GRAY, this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CToolBarPainterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

HCURSOR CToolBarPainterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CToolBarPainterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_tbMain.Create(this, CBRS_GRIPPER | WS_CHILD |  WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS);
	m_tbMain.LoadToolBar(IDR_TOOLBAR_MAIN);
	m_tbMain.GetToolBarCtrl().SetImageList(&m_imgHot);
	m_tbMain.GetToolBarCtrl().SetDisabledImageList(&m_imgGray);

	m_sttMain.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(), this, AFX_IDW_STATUS_BAR);
	int arrWid[] = {500, -1};
	m_sttMain.SetParts(_countof(arrWid), arrWid);
	m_sttMain.SetText(_T("ToolBar Painter 2013"), 0, 0);
	m_sttMain.SetText(_T("作者：凯旋 (qq: 513793572)"), 1, 0);

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	return 0;
}

BOOL CToolBarPainterDlg::ContinueModal()
{
	if (m_tbMain.IsWindowVisible()) 
	{ 
		m_tbMain.OnUpdateCmdUI((CFrameWnd*)this, TRUE); 
	}

	CCmdUI cmdUI; 
	cmdUI.m_pParentMenu = GetMenu();
	if (NULL != cmdUI.m_pParentMenu) 
	{ 
		for (int ui(0); ui < cmdUI.m_pParentMenu->GetMenuItemCount() 
			; ++ui) 
		{ 
			cmdUI.m_pMenu = cmdUI.m_pParentMenu->GetSubMenu(ui); 
			cmdUI.m_nIndexMax = cmdUI.m_pMenu->GetMenuItemCount();
			for (UINT uIndex(0); uIndex < cmdUI.m_nIndexMax; ++uIndex)
			{ 
				cmdUI.m_nID = cmdUI.m_pMenu->GetMenuItemID(uIndex); 
				cmdUI.m_nIndex = uIndex; 
				cmdUI.DoUpdate(this, TRUE);
			} 
		}
	}
	return CDialogEx::ContinueModal();
}

void CToolBarPainterDlg::OnUpdateButtonAdd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

void CToolBarPainterDlg::OnUpdateButtonAbout(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

BOOL CToolBarPainterDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE)
	{
		CString strTip;
		int nIndex = m_tbMain.GetToolBarCtrl().GetHotItem();
		switch(nIndex)
		{
		case 0:
			strTip = _T("添加");
			break;
		case 1:
			strTip = _T("清空");
			break;
		case 2:
			strTip = _T("删除");
			break;
		case 3:
			strTip = _T("左移");
			break;
		case 4:
			strTip = _T("右移");
			break;
		case 5:
			strTip = _T("关于");
			break;
		default:
			strTip = _T("ToolBar Painter 2013");
			break;
		}
		m_sttMain.SetText(strTip, 0, 0);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CToolBarPainterDlg::OnButtonAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CToolBarPainterDlg::OnSelchangeComboSize()
{
	int nOld = m_cmbIndex;
	UpdateData(TRUE);
	if (m_cmbIndex != nOld)
	{
		if (m_pImgIcons->GetImageCount() == 0)
		{
			m_pImgIcons = new CImageList;
			m_pImgIcons->Create(sg_arrSizes[m_cmbIndex], sg_arrSizes[m_cmbIndex], ILC_COLOR24 | ILC_MASK, 0, 1);
			m_lscIcons.SetImageList(m_pImgIcons, LVSIL_NORMAL);
			m_lscIcons.SetIconSpacing(sg_arrSizes[m_cmbIndex], sg_arrSizes[m_cmbIndex]);
		}
		else
		{
			m_cmbIndex = nOld;
			UpdateData(FALSE);
		}
	}
}

void CToolBarPainterDlg::OnSelchangeComboColor()
{
	int nOld = m_cmbClIndx;
	UpdateData(TRUE);
	if (m_cmbClIndx != nOld)
	{
		if (m_pImgIcons->GetImageCount() != 0)
		{
			UpdateCtrls();
		}
	}
}

void CToolBarPainterDlg::UpdateCtrls(void)
{
	m_lscIcons.DeleteAllItems();
	for (int nIndex(0); nIndex < m_pImgIcons->GetImageCount(); ++nIndex)
	{
		m_lscIcons.InsertItem(nIndex, NULL, nIndex);
	}
	m_bwHot.DrawImageList(m_pImgIcons, CSize(sg_arrSizes[m_cmbIndex], sg_arrSizes[m_cmbIndex])
		, sg_arrCls[m_cmbClIndx], FALSE);
	m_bwGray.DrawImageList(m_pImgIcons, CSize(sg_arrSizes[m_cmbIndex], sg_arrSizes[m_cmbIndex])
		, sg_arrCls[m_cmbClIndx], TRUE);
}

void CToolBarPainterDlg::OnButtonAdd()
{
	CFileDialog dlg(TRUE, _T(".ico"), NULL
		, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, _T("图标文件(*.ico)|*.ico|所有文件(*.*)|*.*||"), this);
	TCHAR szPath[MAX_PATH * 100]  = {0};
	dlg.m_ofn.lpstrFile = szPath;
	dlg.m_ofn.nMaxFile = MAX_PATH * 100;

	if (IDOK == dlg.DoModal())
	{
		POSITION pos = dlg.GetStartPosition();
		while (NULL != pos)
		{
			CString strFile = dlg.GetNextPathName(pos);
			HICON hIcon = (HICON)::LoadImage(NULL, strFile, IMAGE_ICON
				, sg_arrSizes[m_cmbIndex], sg_arrSizes[m_cmbIndex], LR_LOADFROMFILE | LR_SHARED);
			if (NULL != hIcon)
			{
				m_pImgIcons->Add(hIcon);
			}
		}
		UpdateCtrls();
	}
}

void CToolBarPainterDlg::OnUpdateButtonLeft(CCmdUI *pCmdUI)
{
	POSITION pos = m_lscIcons.GetFirstSelectedItemPosition();
	pCmdUI->Enable(NULL != pos);
}

void CToolBarPainterDlg::OnUpdateButtonClear(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pImgIcons->GetImageCount() != 0);
}

void CToolBarPainterDlg::OnUpdateButtonDel(CCmdUI *pCmdUI)
{
	POSITION pos = m_lscIcons.GetFirstSelectedItemPosition();
	pCmdUI->Enable(NULL != pos);
}

void CToolBarPainterDlg::OnClickedButtonSave()
{
	CRect rc;
	GetDlgItem(IDC_BUTTON_SAVE)->GetWindowRect(&rc);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_POP);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN, rc.left, rc.top, this);
}

void CToolBarPainterDlg::OnPopSavenormalbar()
{
	CFileDialog dlg(FALSE, _T(".bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, _T("BMP Files(*.bmp)|*.bmp|PNG Files(*.png)|*.png|JPG Files(*.jpg)|*.jpg||"), this);
	if (IDOK == dlg.DoModal())
	{
		CString strFile = dlg.GetPathName();
		if (m_bwHot.SaveImage(strFile))
		{
			CString strTip;
			strTip.Format(_T("%s 保存成功!"), PathFindFileName(strFile));
			MessageBox(strTip, _T("温馨提示"), MB_OK);
		}
	}
}

void CToolBarPainterDlg::OnPopSavegraybar()
{
	CFileDialog dlg(FALSE, _T(".bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, _T("BMP Files(*.bmp)|*.bmp|PNG Files(*.png)|*.png|JPG Files(*.jpg)|*.jpg||"), this);
	if (IDOK == dlg.DoModal())
	{
		CString strFile = dlg.GetPathName();
		if (m_bwGray.SaveImage(strFile))
		{
			CString strTip;
			strTip.Format(_T("%s 保存成功!"), PathFindFileName(strFile));
			MessageBox(strTip, _T("温馨提示"), MB_OK);
		}
	}
}

void CToolBarPainterDlg::OnPopSaveallbars()
{
	int nCnt(0);
	TCHAR szPath[MAX_PATH] = {0};
	BROWSEINFO bsi = {0};
	bsi.hwndOwner = GetSafeHwnd();
	bsi.pszDisplayName = szPath;
	bsi.ulFlags = BIF_USENEWUI;
	bsi.lpszTitle = _T("Select Folder");
	LPITEMIDLIST liil = SHBrowseForFolder(&bsi);
	if (liil != NULL)
	{
		TCHAR szHot[MAX_PATH] = {0};
		TCHAR szGray[MAX_PATH] = {0};
		if (SHGetPathFromIDList(liil, szHot))
		{
			PathAppend(szHot, _T("Normal.bmp"));
			if (m_bwHot.SaveImage(szHot))
			{
				nCnt += 1;
			}
		}
		if (SHGetPathFromIDList(liil, szGray))
		{
			PathAppend(szGray, _T("Gray.bmp"));
			if (m_bwGray.SaveImage(szGray))
			{
				nCnt += 1;
			}
		}
		CString strTip;
		strTip.Format(_T("成功保存 %d 个文件！"), nCnt);
		MessageBox(strTip, _T("温馨提示"), MB_OK);
	}
}

void CToolBarPainterDlg::OnButtonClear()
{
	if (NULL != m_pImgIcons && m_pImgIcons->GetImageCount() != 0)
	{
		m_pImgIcons->DeleteImageList();
		m_pImgIcons->Create(sg_arrSizes[m_cmbIndex], sg_arrSizes[m_cmbIndex], ILC_COLOR24 | ILC_MASK, 0, 1);
		m_lscIcons.SetImageList(m_pImgIcons, LVSIL_NORMAL);
		UpdateCtrls();
	}
}

void CToolBarPainterDlg::OnUpdateButtonRight(CCmdUI *pCmdUI)
{
	POSITION pos = m_lscIcons.GetFirstSelectedItemPosition();
	pCmdUI->Enable(NULL != pos);
}

void CToolBarPainterDlg::OnButtonDel()
{
	POSITION pos = m_lscIcons.GetFirstSelectedItemPosition();
	int nItem = m_lscIcons.GetNextSelectedItem(pos);
	if (-1 != nItem)
	{
		m_pImgIcons->Remove(nItem);
		UpdateCtrls();
	}
}

void CToolBarPainterDlg::OnButtonLeft()
{
	POSITION pos = m_lscIcons.GetFirstSelectedItemPosition();
	int nItem = m_lscIcons.GetNextSelectedItem(pos);
	if (0 != nItem && nItem != -1)
	{
		HICON hIcon0 = m_pImgIcons->ExtractIcon(nItem - 1);
		HICON hIcon1 = m_pImgIcons->ExtractIcon(nItem);
		m_pImgIcons->Replace(nItem - 1, hIcon1);
		m_pImgIcons->Replace(nItem, hIcon0);
		UpdateCtrls();
		m_lscIcons.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CToolBarPainterDlg::OnButtonRight()
{
	POSITION pos = m_lscIcons.GetFirstSelectedItemPosition();
	int nItem = m_lscIcons.GetNextSelectedItem(pos);
	int nCnt = m_pImgIcons->GetImageCount();
	if (nCnt - 1 != nItem && nItem != -1)
	{
		HICON hIcon0 = m_pImgIcons->ExtractIcon(nItem);
		HICON hIcon1 = m_pImgIcons->ExtractIcon(nItem + 1);
		m_pImgIcons->Replace(nItem, hIcon1);
		m_pImgIcons->Replace(nItem + 1, hIcon0);
		UpdateCtrls();
		m_lscIcons.SetItemState(nItem + 1, LVIS_SELECTED, LVIS_SELECTED);
	}
}
