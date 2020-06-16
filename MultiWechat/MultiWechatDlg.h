#if !defined(AFX_XUNLEITOOLDLG_H__D374910D_FEF7_4239_AE17_2D1ADE56D06A__INCLUDED_)
#define AFX_XUNLEITOOLDLG_H__D374910D_FEF7_4239_AE17_2D1ADE56D06A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMultiWechatDlg : public CDialog
{
public:
	CMultiWechatDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_MULTIWECHAT_DIALOG };

	CEdit	m_edtInfo;
	CStatic	m_sttHttp;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	int		m_nNumber;
	CString m_strWechat;

	void _Process();
	void _FindWechat();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonStart();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_XUNLEITOOLDLG_H__D374910D_FEF7_4239_AE17_2D1ADE56D06A__INCLUDED_)
