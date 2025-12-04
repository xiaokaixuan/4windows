// CStaticEx.cpp : implementation file
//

#include "stdafx.h"
#include "ctrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticEx
static HBITMAP gs_hbmpUp(NULL);
static HBITMAP gs_hbmpDown(NULL);
static HBITMAP gs_hbmpOver(NULL);

static HICON   gs_hicoUp(NULL);
static HICON   gs_hicoDown(NULL);
static HICON   gs_hicoOver(NULL);
static BOOL    gs_bTrackLeave(FALSE);

CStaticEx::CStaticEx()

{
}

CStaticEx::~CStaticEx()
{
}


BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
	//{{AFX_MSG_MAP(CStaticEx)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticEx message handlers
void CStaticEx::SetBitmaps(UINT nIdUp, UINT nIdDown, UINT nIdOver)
{
	gs_hbmpOver = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIdOver));
	gs_hbmpUp   = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIdUp));
	gs_hbmpDown = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIdDown));
	ModifyStyle(0, SS_BITMAP);
	CStatic::SetBitmap(gs_hbmpUp);
}
void CStaticEx::SetIcons(UINT nIdUp, UINT nIdDown, UINT nIdOver)
{
	gs_hicoOver = AfxGetApp()->LoadIcon(nIdOver);
	gs_hicoUp   = AfxGetApp()->LoadIcon(nIdUp);
	gs_hicoDown = AfxGetApp()->LoadIcon(nIdDown);
	ModifyStyle(0, SS_ICON);
	CStatic::SetIcon(gs_hicoUp);
}

void CStaticEx::OnMouseMove(UINT nFlags, CPoint point)
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

		if ((GetStyle() & SS_BITMAP) == SS_BITMAP)
		{
			if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
			{
				CStatic::SetBitmap(gs_hbmpDown);
			}
			else
			{
				CStatic::SetBitmap(gs_hbmpOver);
			}
		}
		else if ((GetStyle() & SS_ICON) == SS_ICON)
		{
			if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
			{
				CStatic::SetIcon(gs_hicoDown);
			}
			else
			{
				CStatic::SetIcon(gs_hicoOver);
			}
		}
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CStaticEx::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, SS_NOTIFY);
	CStatic::PreSubclassWindow();
}

LRESULT CStaticEx::OnMouseLeave(WPARAM, LPARAM)
{
	gs_bTrackLeave = FALSE;
	if ((GetStyle() & SS_BITMAP) == SS_BITMAP)
	{
		CStatic::SetBitmap(gs_hbmpUp);
	}
	else if ((GetStyle() & SS_ICON) == SS_ICON)
	{
		CStatic::SetIcon(gs_hicoUp);
	}
	return 0;
}

void CStaticEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ((GetStyle() & SS_BITMAP) == SS_BITMAP)
	{
		CStatic::SetBitmap(gs_hbmpDown);
	}
	else if ((GetStyle() & SS_ICON) == SS_ICON)
	{
		CStatic::SetIcon(gs_hicoDown);
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CStaticEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if ((GetStyle() & SS_BITMAP) == SS_BITMAP)
	{
		CStatic::SetBitmap(gs_hbmpOver);
	}
	else if ((GetStyle() & SS_ICON) == SS_ICON)
	{
		CStatic::SetIcon(gs_hicoOver);
	}
	CStatic::OnLButtonUp(nFlags, point);
}
