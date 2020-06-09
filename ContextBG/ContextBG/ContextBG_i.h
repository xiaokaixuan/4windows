

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Jun 09 16:56:54 2020
 */
/* Compiler settings for ContextBG.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ContextBG_i_h__
#define __ContextBG_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IContextBGExt_FWD_DEFINED__
#define __IContextBGExt_FWD_DEFINED__
typedef interface IContextBGExt IContextBGExt;

#endif 	/* __IContextBGExt_FWD_DEFINED__ */


#ifndef __ContextBGExt_FWD_DEFINED__
#define __ContextBGExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class ContextBGExt ContextBGExt;
#else
typedef struct ContextBGExt ContextBGExt;
#endif /* __cplusplus */

#endif 	/* __ContextBGExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IContextBGExt_INTERFACE_DEFINED__
#define __IContextBGExt_INTERFACE_DEFINED__

/* interface IContextBGExt */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IContextBGExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0F43CB3F-0305-4A89-9FEC-06AA191B6011")
    IContextBGExt : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IContextBGExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IContextBGExt * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IContextBGExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IContextBGExt * This);
        
        END_INTERFACE
    } IContextBGExtVtbl;

    interface IContextBGExt
    {
        CONST_VTBL struct IContextBGExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IContextBGExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IContextBGExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IContextBGExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IContextBGExt_INTERFACE_DEFINED__ */



#ifndef __ContextBGLib_LIBRARY_DEFINED__
#define __ContextBGLib_LIBRARY_DEFINED__

/* library ContextBGLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ContextBGLib;

EXTERN_C const CLSID CLSID_ContextBGExt;

#ifdef __cplusplus

class DECLSPEC_UUID("8C3637E2-5DE0-4F02-A3FE-F216BC63D711")
ContextBGExt;
#endif
#endif /* __ContextBGLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


