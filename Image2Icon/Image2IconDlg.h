// Image2IconDlg.h : header file
//

#if !defined(AFX_IMAGE2ICONDLG_H__83128780_5EC2_4535_A332_8DCB02538FB0__INCLUDED_)
#define AFX_IMAGE2ICONDLG_H__83128780_5EC2_4535_A332_8DCB02538FB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CImage2IconDlg dialog
#include <vector>

class CImage2IconDlg : public CDialog
{
// Construction
public:
	void UpdateList(void);
	void SaveToPng(LPCTSTR lpPath);
	CImage2IconDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImage2IconDlg)
	enum { IDD = IDD_IMAGE2ICON_DIALOG };
	CListBox	m_lsbImages;
	int		m_bppInd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage2IconDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	std::vector<CString> m_vPathes;

	// Generated message map functions
	//{{AFX_MSG(CImage2IconDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonConvert();
	afx_msg void OnButtonImport();
	afx_msg void OnButtonClear();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE2ICONDLG_H__83128780_5EC2_4535_A332_8DCB02538FB0__INCLUDED_)
