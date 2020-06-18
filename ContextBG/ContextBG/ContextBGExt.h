// ContextBGExt.h : CContextBGExt 的声明

#pragma once
#include "resource.h"       // 主符号



#include "ContextBG_i.h"
#include <ShlObj.h>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CContextBGExt

class ATL_NO_VTABLE CContextBGExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CContextBGExt, &CLSID_ContextBGExt>,
	public IContextBGExt,
	public IShellExtInit,
	public IContextMenu
{
public:
	CContextBGExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CONTEXTBGEXT)

DECLARE_NOT_AGGREGATABLE(CContextBGExt)

BEGIN_COM_MAP(CContextBGExt)
	COM_INTERFACE_ENTRY(IContextBGExt)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

private: 
	static HBRUSH sm_hbgBrush;
public:
	// IShellExtInit
	STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	// IContextMenu
	STDMETHODIMP GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT) { return S_OK; }
	STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO) { return S_OK; }
	STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);
};

OBJECT_ENTRY_AUTO(__uuidof(ContextBGExt), CContextBGExt)
