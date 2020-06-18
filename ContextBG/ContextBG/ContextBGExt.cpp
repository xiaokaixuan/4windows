// ContextBGExt.cpp : CContextBGExt µÄÊµÏÖ

#include "stdafx.h"
#include "ContextBGExt.h"
#include "dllmain.h"

// CContextBGExt

HBRUSH CContextBGExt::sm_hbgBrush = NULL;

STDMETHODIMP CContextBGExt::Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY)
{
	do
	{
		if (sm_hbgBrush) break;
		HBITMAP hBitmap = LoadBitmap(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDB_BG));
		if (!hBitmap) break;
		sm_hbgBrush = CreatePatternBrush(hBitmap);
		DeleteObject(hBitmap);
		if (!sm_hbgBrush) break;
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
		if (!sm_hbgBrush) break;
		MENUINFO mi = { 0 };
		mi.cbSize = sizeof(mi);
		mi.hbrBack = sm_hbgBrush;
		mi.fMask = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND;
		SetMenuInfo(hMenu, &mi);
	} while (0);
	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 1);
}
