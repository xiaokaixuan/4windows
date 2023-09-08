
// ToolBarPainterDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "BmpWnd.h"


// CToolBarPainterDlg dialog
class CToolBarPainterDlg : public CDialogEx
{
// Construction
public:
	CToolBarPainterDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CToolBarPainterDlg();

// Dialog Data
	enum { IDD = IDD_TOOLBARPAINTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	CImageList m_imgHot;
	CImageList m_imgGray;
	CToolBar m_tbMain;
	CStatusBarCtrl m_sttMain;

	CImageList *m_pImgIcons;
	CListCtrl m_lscIcons;

	CBmpWnd m_bwHot;
	CBmpWnd m_bwGray;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void UpdateCtrls(void);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL ContinueModal();
	afx_msg void OnUpdateButtonAdd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonAbout(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnButtonAbout();
protected:
	int m_cmbIndex;
	int m_cmbClIndx;
public:
	afx_msg void OnSelchangeComboSize();
	afx_msg void OnSelchangeComboColor();
	afx_msg void OnButtonAdd();
	afx_msg void OnUpdateButtonLeft(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonClear(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonDel(CCmdUI *pCmdUI);
	afx_msg void OnClickedButtonSave();
	afx_msg void OnPopSavenormalbar();
	afx_msg void OnPopSavegraybar();
	afx_msg void OnPopSaveallbars();
	afx_msg void OnButtonClear();
	afx_msg void OnUpdateButtonRight(CCmdUI *pCmdUI);
	afx_msg void OnButtonDel();
	afx_msg void OnButtonLeft();
	afx_msg void OnButtonRight();
};
