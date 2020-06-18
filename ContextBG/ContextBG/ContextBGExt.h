// ContextBGExt.h : CContextBGExt ������

#pragma once
#include "resource.h"       // ������



#include "ContextBG_i.h"
#include <ShlObj.h>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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
