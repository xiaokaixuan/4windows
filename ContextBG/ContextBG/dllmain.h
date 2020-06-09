// dllmain.h : 模块类的声明。

class CContextBGModule : public ATL::CAtlDllModuleT< CContextBGModule >
{
public :
	DECLARE_LIBID(LIBID_ContextBGLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CONTEXTBG, "{CE243DFC-DF84-417F-B543-CAF10C5FDE0C}")
};

extern class CContextBGModule _AtlModule;
