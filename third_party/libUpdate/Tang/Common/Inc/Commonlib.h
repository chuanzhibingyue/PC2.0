

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Mon Sep 14 18:24:29 2015
 */
/* Compiler settings for .\Commonlib.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

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


#ifndef __Commonlib_h__
#define __Commonlib_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITangWinControl_FWD_DEFINED__
#define __ITangWinControl_FWD_DEFINED__
typedef interface ITangWinControl ITangWinControl;
#endif 	/* __ITangWinControl_FWD_DEFINED__ */


#ifndef __ITangXmlEventArg_FWD_DEFINED__
#define __ITangXmlEventArg_FWD_DEFINED__
typedef interface ITangXmlEventArg ITangXmlEventArg;
#endif 	/* __ITangXmlEventArg_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __TangLib_LIBRARY_DEFINED__
#define __TangLib_LIBRARY_DEFINED__

/* library TangLib */
/* [uuid] */ 


EXTERN_C const IID LIBID_TangLib;

#ifndef __ITangWinControl_INTERFACE_DEFINED__
#define __ITangWinControl_INTERFACE_DEFINED__

/* interface ITangWinControl */
/* [object][uuid] */ 


EXTERN_C const IID IID_ITangWinControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6FDAB5B4-8FBB-466c-B201-C9126096FAB1")
    ITangWinControl : public IClassFactory
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetControlHwnd( 
            /* [retval][out] */ HWND *phwnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DetachWindow( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AttachWindow( 
            /* [in] */ HWND hwnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateViewWindow( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAssociateElement( 
            /* [in] */ IUnknown *pUnkown) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseAssociateElement( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetD4( 
            /* [retval][out] */ IUnknown **pUnkown) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetE3( 
            /* [retval][out] */ IUnknown **pUnkown) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ long lHeight) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ long lWidth) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITangWinControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITangWinControl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITangWinControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITangWinControl * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *CreateInstance )( 
            ITangWinControl * This,
            /* [unique][in] */ IUnknown *pUnkOuter,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *LockServer )( 
            ITangWinControl * This,
            /* [in] */ BOOL fLock);
        
        HRESULT ( STDMETHODCALLTYPE *GetControlHwnd )( 
            ITangWinControl * This,
            /* [retval][out] */ HWND *phwnd);
        
        HRESULT ( STDMETHODCALLTYPE *DetachWindow )( 
            ITangWinControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *AttachWindow )( 
            ITangWinControl * This,
            /* [in] */ HWND hwnd);
        
        HRESULT ( STDMETHODCALLTYPE *CreateViewWindow )( 
            ITangWinControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetAssociateElement )( 
            ITangWinControl * This,
            /* [in] */ IUnknown *pUnkown);
        
        HRESULT ( STDMETHODCALLTYPE *ReleaseAssociateElement )( 
            ITangWinControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetD4 )( 
            ITangWinControl * This,
            /* [retval][out] */ IUnknown **pUnkown);
        
        HRESULT ( STDMETHODCALLTYPE *GetE3 )( 
            ITangWinControl * This,
            /* [retval][out] */ IUnknown **pUnkown);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            ITangWinControl * This,
            /* [in] */ long lHeight);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            ITangWinControl * This,
            /* [in] */ long lWidth);
        
        END_INTERFACE
    } ITangWinControlVtbl;

    interface ITangWinControl
    {
        CONST_VTBL struct ITangWinControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITangWinControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITangWinControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITangWinControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITangWinControl_CreateInstance(This,pUnkOuter,riid,ppvObject)	\
    ( (This)->lpVtbl -> CreateInstance(This,pUnkOuter,riid,ppvObject) ) 

#define ITangWinControl_LockServer(This,fLock)	\
    ( (This)->lpVtbl -> LockServer(This,fLock) ) 


#define ITangWinControl_GetControlHwnd(This,phwnd)	\
    ( (This)->lpVtbl -> GetControlHwnd(This,phwnd) ) 

#define ITangWinControl_DetachWindow(This)	\
    ( (This)->lpVtbl -> DetachWindow(This) ) 

#define ITangWinControl_AttachWindow(This,hwnd)	\
    ( (This)->lpVtbl -> AttachWindow(This,hwnd) ) 

#define ITangWinControl_CreateViewWindow(This)	\
    ( (This)->lpVtbl -> CreateViewWindow(This) ) 

#define ITangWinControl_SetAssociateElement(This,pUnkown)	\
    ( (This)->lpVtbl -> SetAssociateElement(This,pUnkown) ) 

#define ITangWinControl_ReleaseAssociateElement(This)	\
    ( (This)->lpVtbl -> ReleaseAssociateElement(This) ) 

#define ITangWinControl_GetD4(This,pUnkown)	\
    ( (This)->lpVtbl -> GetD4(This,pUnkown) ) 

#define ITangWinControl_GetE3(This,pUnkown)	\
    ( (This)->lpVtbl -> GetE3(This,pUnkown) ) 

#define ITangWinControl_put_Height(This,lHeight)	\
    ( (This)->lpVtbl -> put_Height(This,lHeight) ) 

#define ITangWinControl_put_Width(This,lWidth)	\
    ( (This)->lpVtbl -> put_Width(This,lWidth) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITangWinControl_INTERFACE_DEFINED__ */


#ifndef __ITangXmlEventArg_INTERFACE_DEFINED__
#define __ITangXmlEventArg_INTERFACE_DEFINED__

/* interface ITangXmlEventArg */
/* [object][uuid] */ 


EXTERN_C const IID IID_ITangXmlEventArg;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2F314C18-93D3-4b16-A77C-C1E292199811")
    ITangXmlEventArg : public IDispatch
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_xpath( 
            /* [retval][out] */ BSTR *ppStrPath) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_srcElement( 
            /* [retval][out] */ IUnknown **pElement) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_eventType( 
            /* [retval][out] */ UINT32 *uEventType) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITangXmlEventArgVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITangXmlEventArg * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITangXmlEventArg * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITangXmlEventArg * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITangXmlEventArg * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITangXmlEventArg * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITangXmlEventArg * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITangXmlEventArg * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_xpath )( 
            ITangXmlEventArg * This,
            /* [retval][out] */ BSTR *ppStrPath);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_srcElement )( 
            ITangXmlEventArg * This,
            /* [retval][out] */ IUnknown **pElement);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_eventType )( 
            ITangXmlEventArg * This,
            /* [retval][out] */ UINT32 *uEventType);
        
        END_INTERFACE
    } ITangXmlEventArgVtbl;

    interface ITangXmlEventArg
    {
        CONST_VTBL struct ITangXmlEventArgVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITangXmlEventArg_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITangXmlEventArg_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITangXmlEventArg_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITangXmlEventArg_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITangXmlEventArg_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITangXmlEventArg_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITangXmlEventArg_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITangXmlEventArg_get_xpath(This,ppStrPath)	\
    ( (This)->lpVtbl -> get_xpath(This,ppStrPath) ) 

#define ITangXmlEventArg_get_srcElement(This,pElement)	\
    ( (This)->lpVtbl -> get_srcElement(This,pElement) ) 

#define ITangXmlEventArg_get_eventType(This,uEventType)	\
    ( (This)->lpVtbl -> get_eventType(This,uEventType) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITangXmlEventArg_INTERFACE_DEFINED__ */

#endif /* __TangLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


