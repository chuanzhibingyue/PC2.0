

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri May 20 14:28:55 2016
 */
/* Compiler settings for CopyLocation.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
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

#ifndef __CopyLocation_h__
#define __CopyLocation_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICopyLocationShl_FWD_DEFINED__
#define __ICopyLocationShl_FWD_DEFINED__
typedef interface ICopyLocationShl ICopyLocationShl;

#endif 	/* __ICopyLocationShl_FWD_DEFINED__ */


#ifndef __CopyLocationShl_FWD_DEFINED__
#define __CopyLocationShl_FWD_DEFINED__

#ifdef __cplusplus
typedef class CopyLocationShl CopyLocationShl;
#else
typedef struct CopyLocationShl CopyLocationShl;
#endif /* __cplusplus */

#endif 	/* __CopyLocationShl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICopyLocationShl_INTERFACE_DEFINED__
#define __ICopyLocationShl_INTERFACE_DEFINED__

/* interface ICopyLocationShl */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICopyLocationShl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A7847D3D-09F3-11D4-8F6D-0080AD87DD41")
    ICopyLocationShl : public IDispatch
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ICopyLocationShlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICopyLocationShl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICopyLocationShl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICopyLocationShl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICopyLocationShl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICopyLocationShl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICopyLocationShl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICopyLocationShl * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } ICopyLocationShlVtbl;

    interface ICopyLocationShl
    {
        CONST_VTBL struct ICopyLocationShlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICopyLocationShl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICopyLocationShl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICopyLocationShl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICopyLocationShl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICopyLocationShl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICopyLocationShl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICopyLocationShl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICopyLocationShl_INTERFACE_DEFINED__ */



#ifndef __COPYLOCATIONLib_LIBRARY_DEFINED__
#define __COPYLOCATIONLib_LIBRARY_DEFINED__

/* library COPYLOCATIONLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_COPYLOCATIONLib;

EXTERN_C const CLSID CLSID_CopyLocationShl;

#ifdef __cplusplus

class DECLSPEC_UUID("A7847D3E-09F3-11D4-8F6D-0080AD87DD41")
CopyLocationShl;
#endif
#endif /* __COPYLOCATIONLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


