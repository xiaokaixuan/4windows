// ContextBGExt.cpp : CContextBGExt µÄÊµÏÖ

#include "stdafx.h"
#include "ContextBGExt.h"
#include "dllmain.h"

// CContextBGExt

STDMETHODIMP CContextBGExt::Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY)
{
	do
	{
		if (m_hbgBrush) break;
		HBITMAP hBitmap = LoadBitmap(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDB_BG));
		if (!hBitmap) break;
		m_hbgBrush = CreatePatternBrush(hBitmap);
		DeleteObject(hBitmap);
		if (!m_hbgBrush) break;
	} while (0);
	return S_OK;
}

STDMETHODIMP CContextBGExt::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	if (uFlags & CMF_DEFAULTONLY)
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
	}
	do
	{
		if (!m_hbgBrush) break;
		MENUINFO mi = { 0 };
		mi.cbSize = sizeof(mi);
		mi.hbrBack = m_hbgBrush;
		mi.fMask = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND;
		SetMenuInfo(hMenu, &mi);
	} while (0);
	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 1);
}
