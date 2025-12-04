// Icon ExtractorDlg.h : header file
//

#if !defined(AFX_ICONEXTRACTORDLG_H__B9DE4B40_D9DA_4350_88EE_ABCE1B37C4B9__INCLUDED_)
#define AFX_ICONEXTRACTORDLG_H__B9DE4B40_D9DA_4350_88EE_ABCE1B37C4B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIconExtractorDlg dialog
#include "ctrlEx.h"
#include <vector>

class CIconExtractorDlg : public CDialog
{
// Construction
public:
	CIconExtractorDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CIconExtractorDlg();
// Dialog Data
	//{{AFX_DATA(CIconExtractorDlg)
	enum { IDD = IDD_ICONEXTRACTOR_DIALOG };
	CListCtrl	m_lscIcons;
	CString	m_strFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconExtractorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void _ParseFile(void);
	void _Clear(void);
	static BOOL CALLBACK EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG lParam);

protected:
	void _Resize(void);
	void _UpdateList();
	HICON m_hIcon;

	CStatusBarCtrl m_stbBar;
	CButtonEx	   m_btnBrowse;

	CImageList *m_pImgList;
	std::vector<LPTSTR> m_vIcoRes;

	class CAutoFont *m_pFont;

	CMenu m_popMenu;

	// Generated message map functions
	//{{AFX_MSG(CIconExtractorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	afx_msg void OnButtonBrowse();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRclickListIcons(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemRefresh();
	afx_msg void OnMenuitemSelectall();
	afx_msg void OnMenuitemExtract();
	afx_msg void OnMenuitemAbout();
	//}}AFX_MSG
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONEXTRACTORDLG_H__B9DE4B40_D9DA_4350_88EE_ABCE1B37C4B9__INCLUDED_)
