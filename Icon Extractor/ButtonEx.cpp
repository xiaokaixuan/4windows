// ButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "ctrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonEx
static UINT gs_nIdUp(0);
static UINT gs_nIdOver(0);
static BOOL gs_bTrackLeave(FALSE);

CButtonEx::CButtonEx()
{
}

CButtonEx::~CButtonEx()
{
}

BEGIN_MESSAGE_MAP(CButtonEx, CBitmapButton)
	//{{AFX_MSG_MAP(CButtonEx)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonEx message handlers

void CButtonEx::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW | BS_BITMAP);
	CBitmapButton::PreSubclassWindow();
}

void CButtonEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!gs_bTrackLeave)
	{
		TRACKMOUSEEVENT tme = {0};
		tme.cbSize  = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = GetSafeHwnd();
		_TrackMouseEvent(&tme);
		gs_bTrackLeave = TRUE;
		m_bitmap.DeleteObject();
		m_bitmap.LoadBitmap(gs_nIdOver);
		Invalidate();
	}
	CBitmapButton::OnMouseMove(nFlags, point);
}

LRESULT CButtonEx::OnMouseLeave(WPARAM, LPARAM)
{
	gs_bTrackLeave = FALSE;
	m_bitmap.DeleteObject();
	m_bitmap.LoadBitmap(gs_nIdUp);
	Invalidate();
	return 0;
}

BOOL CButtonEx::SetBitmaps(UINT nIdUp, UINT nIdDown, UINT nIdOver, UINT nIdDisable)
{
	gs_nIdOver = nIdOver;
	gs_nIdUp   = nIdUp;
	return CBitmapButton::LoadBitmaps(nIdUp, nIdDown, 0, nIdDisable);
}