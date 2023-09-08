#pragma once


// CBmpWnd

class CBmpWnd : public CWnd
{
	DECLARE_DYNAMIC(CBmpWnd)

public:
	CBmpWnd();
	virtual ~CBmpWnd();

	void DrawImageList(CImageList* pImagelist, CSize const& size, COLORREF bgcl, BOOL bGray);
	BOOL SaveImage(LPCTSTR lpFileName);

protected:
	CDC m_dcMem;
	CBitmap m_bmpMem;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


