// BmpWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarPainter.h"
#include "BmpWnd.h"


// CBmpWnd

IMPLEMENT_DYNAMIC(CBmpWnd, CWnd)

CBmpWnd::CBmpWnd()
{
	WNDCLASS wndclass = {0};
	wndclass.style = CS_DBLCLKS;
	wndclass.lpfnWndProc = ::DefWindowProc;
	wndclass.hInstance = AfxGetInstanceHandle();
	wndclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndclass.hbrBackground = ::GetSysColorBrush(COLOR_BTNFACE);
	wndclass.lpszClassName=_T("BmpWnd");
	AfxRegisterClass(&wndclass);
}

CBmpWnd::~CBmpWnd()
{

}


BEGIN_MESSAGE_MAP(CBmpWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CBmpWnd message handlers

void CBmpWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (NULL != m_bmpMem.GetSafeHandle() && NULL != m_dcMem.GetSafeHdc())
	{
		BITMAP bmp;
		m_bmpMem.GetBitmap(&bmp);
		dc.BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &m_dcMem, 0, 0, SRCCOPY);
	}
}

void CBmpWnd::DrawImageList(CImageList* pImagelist, CSize const& size, COLORREF bgcl, BOOL bGray)
{
	CClientDC dc(this);
	if (NULL == dc.GetSafeHdc())
	{
		return;
	}
	if (NULL == pImagelist)
	{
		return;
	}
	if (NULL != m_bmpMem.GetSafeHandle())
	{
		m_bmpMem.DeleteObject();
	}
	if (NULL != m_dcMem.GetSafeHdc())
	{
		m_dcMem.DeleteDC();
	}
	int nCnt = pImagelist->GetImageCount();
	m_bmpMem.CreateCompatibleBitmap(&dc, size.cx * nCnt, size.cy);
	m_dcMem.CreateCompatibleDC(&dc);
	m_dcMem.SelectObject(&m_bmpMem);
	m_dcMem.FillSolidRect(0, 0, size.cx * nCnt, size.cy, bgcl);

	for (int i(0); i < nCnt; ++i)
	{
		pImagelist->Draw(&m_dcMem, i, CPoint(size.cx * i, 0), ILD_TRANSPARENT);
	}
	if (bGray)
	{
		for (int x(0); x < size.cx * nCnt; ++x)
		{
			for (int y(0); y < size.cy; ++y)
			{
				COLORREF cl = m_dcMem.GetPixel(x, y);
				if (cl != bgcl)
				{
					BYTE r = GetRValue(cl);
					BYTE g = GetGValue(cl);
					BYTE b = GetBValue(cl);
					BYTE t = (30*r + 59*g + 11*b)/100;
					cl = RGB(t, t, t);
					m_dcMem.SetPixel(x, y, cl);
				}
			}
		}
	}
	Invalidate();
}

BOOL CBmpWnd::SaveImage(LPCTSTR lpFileName)
{
	BOOL bRet(FALSE);
	HBITMAP hBitmap = (HBITMAP)m_bmpMem.GetSafeHandle();
	if (NULL != hBitmap)
	{
		CImage image;
		image.Attach(hBitmap);
		if (S_OK == image.Save(lpFileName))
		{
			bRet = TRUE;
		}
	}
	return bRet;
}

