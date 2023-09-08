// IclGenerateDlg.h : header file
//

#if !defined(AFX_ICLGENERATEDLG_H__4CA7C468_8FBE_40E1_88EB_AE5830B68CF0__INCLUDED_)
#define AFX_ICLGENERATEDLG_H__4CA7C468_8FBE_40E1_88EB_AE5830B68CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIclGenerateDlg dialog
#include "ctrlEx.h"
#include <vector>

class CIclGenerateDlg : public CDialog
{
// Construction
public:
	void UpdateList(void);
	void StartFind();
	CIclGenerateDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CIclGenerateDlg();
// Dialog Data
	//{{AFX_DATA(CIclGenerateDlg)
	enum { IDD = IDD_ICLGENERATE_DIALOG };
	CButton	m_btnSub;
	CListCtrl	m_lscIcon;
	CString	m_strPath;
	BOOL	m_bSub;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIclGenerateDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void _Restore(void);
	void _FindIcon(LPCTSTR lpPath);
	void _Resize(void);
	HICON m_hIcon;

	CStatusBarCtrl m_sttCtrl;
	CButtonEx	   m_btnBrowse;

	CImageList *m_pImgList;
	std::vector<CString*> m_vpStrs;

	CMenu m_popMenu;

	// Generated message map functions
	//{{AFX_MSG(CIclGenerateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonBrowse();
	afx_msg void OnCheckSub();
	afx_msg void OnRclickListIcon(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemRefind();
	afx_msg void OnMenuitemAllselect();
	afx_msg void OnMenuitemExport();
	afx_msg void OnMenuitemSaveas();
	afx_msg void OnMenuitemAbout();
	//}}AFX_MSG
	afx_msg void OnDropFiles(HDROP);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICLGENERATEDLG_H__4CA7C468_8FBE_40E1_88EB_AE5830B68CF0__INCLUDED_)
