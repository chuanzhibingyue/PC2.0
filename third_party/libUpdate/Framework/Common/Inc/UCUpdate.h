

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Fri Sep 11 13:55:07 2015
 */
/* Compiler settings for ..\..\Common\Inc\UCUpdate.idl:
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


#ifndef __UCUpdate_h__
#define __UCUpdate_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITangWnd_FWD_DEFINED__
#define __ITangWnd_FWD_DEFINED__
typedef interface ITangWnd ITangWnd;
#endif 	/* __ITangWnd_FWD_DEFINED__ */


#ifndef __ITangUtil_FWD_DEFINED__
#define __ITangUtil_FWD_DEFINED__
typedef interface ITangUtil ITangUtil;
#endif 	/* __ITangUtil_FWD_DEFINED__ */


#ifndef __ITangSession_FWD_DEFINED__
#define __ITangSession_FWD_DEFINED__
typedef interface ITangSession ITangSession;
#endif 	/* __ITangSession_FWD_DEFINED__ */


#ifndef __ITangFile_FWD_DEFINED__
#define __ITangFile_FWD_DEFINED__
typedef interface ITangFile ITangFile;
#endif 	/* __ITangFile_FWD_DEFINED__ */


#ifndef __ITangDirectory_FWD_DEFINED__
#define __ITangDirectory_FWD_DEFINED__
typedef interface ITangDirectory ITangDirectory;
#endif 	/* __ITangDirectory_FWD_DEFINED__ */


#ifndef __IListEntry_FWD_DEFINED__
#define __IListEntry_FWD_DEFINED__
typedef interface IListEntry IListEntry;
#endif 	/* __IListEntry_FWD_DEFINED__ */


#ifndef __IUpdate_FWD_DEFINED__
#define __IUpdate_FWD_DEFINED__
typedef interface IUpdate IUpdate;
#endif 	/* __IUpdate_FWD_DEFINED__ */


#ifndef __IEventCenter_FWD_DEFINED__
#define __IEventCenter_FWD_DEFINED__
typedef interface IEventCenter IEventCenter;
#endif 	/* __IEventCenter_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __UCUpdateLib_LIBRARY_DEFINED__
#define __UCUpdateLib_LIBRARY_DEFINED__

/* library UCUpdateLib */
/* [uuid] */ 


EXTERN_C const IID LIBID_UCUpdateLib;

#ifndef __ITangWnd_INTERFACE_DEFINED__
#define __ITangWnd_INTERFACE_DEFINED__

/* interface ITangWnd */
/* [object][uuid] */ 


EXTERN_C const IID IID_ITangWnd;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d9a5a3cb-3f52-4267-8c10-084f292be341")
    ITangWnd : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE animate( 
            /* [in] */ DWORD flags,
            /* [in] */ DWORD duration) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE show( 
            /* [defaultvalue][in] */ int uFlags = 1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE doModal( 
            /* [retval][out] */ int *__MIDL__ITangWnd0000) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setTitle( 
            /* [in] */ BSTR Title) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setAOT( 
            /* [in] */ BOOL b) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE minimize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE maximize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE close( 
            /* [in] */ int retVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setSize( 
            /* [in] */ DWORD width,
            /* [in] */ DWORD height) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setMinSize( 
            /* [in] */ DWORD width,
            /* [in] */ DWORD height) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setMaxSize( 
            /* [in] */ DWORD width,
            /* [in] */ DWORD height) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setPos( 
            /* [in] */ int left,
            /* [in] */ int top) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setIcon( 
            /* [in] */ BSTR IconPath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE hide( 
            /* [in] */ BOOL b) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE rightToLeft( 
            /* [in] */ BOOL b) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE resizeable( 
            /* [in] */ BOOL b) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE showMinBox( 
            /* [in] */ BOOL b) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE showMaxBox( 
            /* [in] */ BOOL b) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE showTitlebar( 
            /* [in] */ BOOL b) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE translucent( 
            /* [in] */ DWORD percent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE focus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setForegroundWindow( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE isActive( 
            /* [retval][out] */ BOOL *__MIDL__ITangWnd0001) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE pushFocus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE popFocus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setMinHandler( 
            /* [in] */ BSTR __MIDL__ITangWnd0002) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setLostFocusHandler( 
            /* [in] */ BSTR __MIDL__ITangWnd0003) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setCloseHandler( 
            /* [in] */ BSTR __MIDL__ITangWnd0004) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setRestoreHandler( 
            /* [in] */ BSTR __MIDL__ITangWnd0005) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setMaxHandler( 
            /* [in] */ BSTR __MIDL__ITangWnd0006) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setWindowMoveHandler( 
            /* [in] */ BSTR __MIDL__ITangWnd0007) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setSizeChangedHandler( 
            /* [in] */ BSTR __MIDL__ITangWnd0008) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE registerHotKey( 
            /* [in] */ UINT __MIDL__ITangWnd0009,
            /* [in] */ UINT __MIDL__ITangWnd0010,
            /* [in] */ int __MIDL__ITangWnd0011,
            /* [in] */ BSTR __MIDL__ITangWnd0012) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE unregisterHotKey( 
            /* [in] */ int __MIDL__ITangWnd0013) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE restore( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE fullScreen( 
            /* [in] */ BOOL b) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE modifyStyleEx( 
            /* [in] */ DWORD dwRemove,
            /* [in] */ DWORD dwAdd,
            /* [defaultvalue][in] */ UINT nFlags = 0) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_left( 
            /* [retval][out] */ VARIANT *retVal) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_top( 
            /* [retval][out] */ VARIANT *retVal) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_width( 
            /* [retval][out] */ VARIANT *retVal) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_height( 
            /* [retval][out] */ VARIANT *retVal) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_isMinimized( 
            /* [retval][out] */ BOOL *__MIDL__ITangWnd0014) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_isMaximized( 
            /* [retval][out] */ BOOL *__MIDL__ITangWnd0015) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_isHidden( 
            /* [retval][out] */ BOOL *__MIDL__ITangWnd0016) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Hwnd( 
            /* [retval][out] */ DWORD *pHwnd) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_subWindow( 
            /* [in] */ BSTR name,
            /* [retval][out] */ VARIANT *pDisp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE fireEvent( 
            /* [in] */ BSTR handler,
            /* [in] */ VARIANT *parameters,
            /* [in] */ DWORD nParams,
            /* [retval][out] */ VARIANT *varRet) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE messageBox( 
            /* [in] */ BOOL modal,
            /* [in] */ BSTR text,
            /* [in] */ BSTR caption,
            /* [in] */ DWORD type,
            /* [retval][out] */ DWORD *retval) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE openFileDialog( 
            /* [in] */ BSTR bstrDefExt,
            /* [in] */ BSTR bstrDefFileName,
            /* [in] */ BSTR bstrFilter,
            /* [in] */ BSTR bstrCurrentDir,
            /* [retval][out] */ BSTR *bstrFileName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE selectFolderDialog( 
            /* [in] */ BSTR strDialogTitle,
            /* [retval][out] */ BSTR *pstrDir) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE saveFileDialog( 
            /* [in] */ BSTR bstrDefExt,
            /* [in] */ BSTR bstrDefFileName,
            /* [in] */ BSTR bstrFilter,
            /* [in] */ BSTR bstrCurrentDir,
            /* [retval][out] */ BSTR *bstrFileName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE createWindow( 
            /* [in] */ BOOL bModal,
            /* [in] */ BSTR name,
            /* [in] */ BSTR bstrUrl,
            /* [defaultvalue][in] */ BOOL bNeedMsg,
            /* [defaultvalue][in] */ BOOL bShadow,
            /* [defaultvalue][in] */ BOOL bShowIconInTask,
            /* [defaultvalue][in] */ BSTR strPicPath,
            /* [defaultvalue][in] */ UINT32 uColor,
            /* [retval][out] */ VARIANT *pDisp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setWinTitleRect( 
            /* [defaultvalue][in] */ UINT px = 0,
            /* [defaultvalue][in] */ UINT py = 0,
            /* [defaultvalue][in] */ UINT x = 0,
            /* [defaultvalue][in] */ UINT y = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE navigate( 
            /* [in] */ BSTR relativePath,
            /* [defaultvalue][in] */ BOOL bLocalPage = TRUE) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setAnimationArea( 
            /* [in] */ IUnknown *pViewArea,
            /* [in] */ BSTR renderProperty) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE removeAnimationArea( 
            /* [in] */ IUnknown *pViewArea) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE restoreWndRegular( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE showRoundCorner( 
            /* [in] */ BOOL bShow) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setRoundCornerSize( 
            /* [in] */ int cornerSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setTrapezoidSize( 
            /* [in] */ int iDvalue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE shadowLess( 
            /* [in] */ BOOL bShadowLess) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE behind( 
            /* [in] */ DWORD hwnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE enableWindow( 
            /* [in] */ BOOL bEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsEnabledWindow( 
            /* [retval][out] */ BOOL *__MIDL__ITangWnd0017) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setAdsorption( 
            /* [in] */ BOOL bIsAdsorption) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setMinWinHeight( 
            /* [in] */ UINT uMinWinHeight) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setAllowReSize( 
            /* [in] */ BOOL bAllowReSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE letMeCrash( 
            /* [in] */ UINT32 uCrashType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE flashTaskWindow( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnableSubWindowList( 
            /* [in] */ BOOL bEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAssociatedWithMobile( 
            /* [in] */ UINT32 hParentWnd,
            /* [in] */ UINT32 hSonWnd,
            /* [in] */ int xVal,
            /* [in] */ int yVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetMobileDirection( 
            /* [defaultvalue][in] */ BOOL IsEnabled = FALSE) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetLeaveElapse( 
            /* [in] */ UINT32 uElapse) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HasFullScreenSwitch( 
            /* [in] */ BOOL bHas) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FlashTaskWindowEx( 
            /* [in] */ UINT nFlags,
            /* [defaultvalue][in] */ UINT nCount = 0,
            /* [defaultvalue][in] */ UINT nTimeout = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCursorPosition( 
            /* [retval][out] */ BSTR *bstrPos) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITangWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITangWnd * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITangWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITangWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITangWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITangWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITangWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITangWnd * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *animate )( 
            ITangWnd * This,
            /* [in] */ DWORD flags,
            /* [in] */ DWORD duration);
        
        HRESULT ( STDMETHODCALLTYPE *show )( 
            ITangWnd * This,
            /* [defaultvalue][in] */ int uFlags);
        
        HRESULT ( STDMETHODCALLTYPE *doModal )( 
            ITangWnd * This,
            /* [retval][out] */ int *__MIDL__ITangWnd0000);
        
        HRESULT ( STDMETHODCALLTYPE *setTitle )( 
            ITangWnd * This,
            /* [in] */ BSTR Title);
        
        HRESULT ( STDMETHODCALLTYPE *setAOT )( 
            ITangWnd * This,
            /* [in] */ BOOL b);
        
        HRESULT ( STDMETHODCALLTYPE *minimize )( 
            ITangWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *maximize )( 
            ITangWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *close )( 
            ITangWnd * This,
            /* [in] */ int retVal);
        
        HRESULT ( STDMETHODCALLTYPE *setSize )( 
            ITangWnd * This,
            /* [in] */ DWORD width,
            /* [in] */ DWORD height);
        
        HRESULT ( STDMETHODCALLTYPE *setMinSize )( 
            ITangWnd * This,
            /* [in] */ DWORD width,
            /* [in] */ DWORD height);
        
        HRESULT ( STDMETHODCALLTYPE *setMaxSize )( 
            ITangWnd * This,
            /* [in] */ DWORD width,
            /* [in] */ DWORD height);
        
        HRESULT ( STDMETHODCALLTYPE *setPos )( 
            ITangWnd * This,
            /* [in] */ int left,
            /* [in] */ int top);
        
        HRESULT ( STDMETHODCALLTYPE *setIcon )( 
            ITangWnd * This,
            /* [in] */ BSTR IconPath);
        
        HRESULT ( STDMETHODCALLTYPE *hide )( 
            ITangWnd * This,
            /* [in] */ BOOL b);
        
        HRESULT ( STDMETHODCALLTYPE *rightToLeft )( 
            ITangWnd * This,
            /* [in] */ BOOL b);
        
        HRESULT ( STDMETHODCALLTYPE *resizeable )( 
            ITangWnd * This,
            /* [in] */ BOOL b);
        
        HRESULT ( STDMETHODCALLTYPE *showMinBox )( 
            ITangWnd * This,
            /* [in] */ BOOL b);
        
        HRESULT ( STDMETHODCALLTYPE *showMaxBox )( 
            ITangWnd * This,
            /* [in] */ BOOL b);
        
        HRESULT ( STDMETHODCALLTYPE *showTitlebar )( 
            ITangWnd * This,
            /* [in] */ BOOL b);
        
        HRESULT ( STDMETHODCALLTYPE *translucent )( 
            ITangWnd * This,
            /* [in] */ DWORD percent);
        
        HRESULT ( STDMETHODCALLTYPE *focus )( 
            ITangWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *setForegroundWindow )( 
            ITangWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *isActive )( 
            ITangWnd * This,
            /* [retval][out] */ BOOL *__MIDL__ITangWnd0001);
        
        HRESULT ( STDMETHODCALLTYPE *pushFocus )( 
            ITangWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *popFocus )( 
            ITangWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *setMinHandler )( 
            ITangWnd * This,
            /* [in] */ BSTR __MIDL__ITangWnd0002);
        
        HRESULT ( STDMETHODCALLTYPE *setLostFocusHandler )( 
            ITangWnd * This,
            /* [in] */ BSTR __MIDL__ITangWnd0003);
        
        HRESULT ( STDMETHODCALLTYPE *setCloseHandler )( 
            ITangWnd * This,
            /* [in] */ BSTR __MIDL__ITangWnd0004);
        
        HRESULT ( STDMETHODCALLTYPE *setRestoreHandler )( 
            ITangWnd * This,
            /* [in] */ BSTR __MIDL__ITangWnd0005);
        
        HRESULT ( STDMETHODCALLTYPE *setMaxHandler )( 
            ITangWnd * This,
            /* [in] */ BSTR __MIDL__ITangWnd0006);
        
        HRESULT ( STDMETHODCALLTYPE *setWindowMoveHandler )( 
            ITangWnd * This,
            /* [in] */ BSTR __MIDL__ITangWnd0007);
        
        HRESULT ( STDMETHODCALLTYPE *setSizeChangedHandler )( 
            ITangWnd * This,
            /* [in] */ BSTR __MIDL__ITangWnd0008);
        
        HRESULT ( STDMETHODCALLTYPE *registerHotKey )( 
            ITangWnd * This,
            /* [in] */ UINT __MIDL__ITangWnd0009,
            /* [in] */ UINT __MIDL__ITangWnd0010,
            /* [in] */ int __MIDL__ITangWnd0011,
            /* [in] */ BSTR __MIDL__ITangWnd0012);
        
        HRESULT ( STDMETHODCALLTYPE *unregisterHotKey )( 
            ITangWnd * This,
            /* [in] */ int __MIDL__ITangWnd0013);
        
        HRESULT ( STDMETHODCALLTYPE *restore )( 
            ITangWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *fullScreen )( 
            ITangWnd * This,
            /* [in] */ BOOL b);
        
        HRESULT ( STDMETHODCALLTYPE *modifyStyleEx )( 
            ITangWnd * This,
            /* [in] */ DWORD dwRemove,
            /* [in] */ DWORD dwAdd,
            /* [defaultvalue][in] */ UINT nFlags);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_left )( 
            ITangWnd * This,
            /* [retval][out] */ VARIANT *retVal);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_top )( 
            ITangWnd * This,
            /* [retval][out] */ VARIANT *retVal);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_width )( 
            ITangWnd * This,
            /* [retval][out] */ VARIANT *retVal);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_height )( 
            ITangWnd * This,
            /* [retval][out] */ VARIANT *retVal);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_isMinimized )( 
            ITangWnd * This,
            /* [retval][out] */ BOOL *__MIDL__ITangWnd0014);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_isMaximized )( 
            ITangWnd * This,
            /* [retval][out] */ BOOL *__MIDL__ITangWnd0015);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_isHidden )( 
            ITangWnd * This,
            /* [retval][out] */ BOOL *__MIDL__ITangWnd0016);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hwnd )( 
            ITangWnd * This,
            /* [retval][out] */ DWORD *pHwnd);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_subWindow )( 
            ITangWnd * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ VARIANT *pDisp);
        
        HRESULT ( STDMETHODCALLTYPE *fireEvent )( 
            ITangWnd * This,
            /* [in] */ BSTR handler,
            /* [in] */ VARIANT *parameters,
            /* [in] */ DWORD nParams,
            /* [retval][out] */ VARIANT *varRet);
        
        HRESULT ( STDMETHODCALLTYPE *messageBox )( 
            ITangWnd * This,
            /* [in] */ BOOL modal,
            /* [in] */ BSTR text,
            /* [in] */ BSTR caption,
            /* [in] */ DWORD type,
            /* [retval][out] */ DWORD *retval);
        
        HRESULT ( STDMETHODCALLTYPE *openFileDialog )( 
            ITangWnd * This,
            /* [in] */ BSTR bstrDefExt,
            /* [in] */ BSTR bstrDefFileName,
            /* [in] */ BSTR bstrFilter,
            /* [in] */ BSTR bstrCurrentDir,
            /* [retval][out] */ BSTR *bstrFileName);
        
        HRESULT ( STDMETHODCALLTYPE *selectFolderDialog )( 
            ITangWnd * This,
            /* [in] */ BSTR strDialogTitle,
            /* [retval][out] */ BSTR *pstrDir);
        
        HRESULT ( STDMETHODCALLTYPE *saveFileDialog )( 
            ITangWnd * This,
            /* [in] */ BSTR bstrDefExt,
            /* [in] */ BSTR bstrDefFileName,
            /* [in] */ BSTR bstrFilter,
            /* [in] */ BSTR bstrCurrentDir,
            /* [retval][out] */ BSTR *bstrFileName);
        
        HRESULT ( STDMETHODCALLTYPE *createWindow )( 
            ITangWnd * This,
            /* [in] */ BOOL bModal,
            /* [in] */ BSTR name,
            /* [in] */ BSTR bstrUrl,
            /* [defaultvalue][in] */ BOOL bNeedMsg,
            /* [defaultvalue][in] */ BOOL bShadow,
            /* [defaultvalue][in] */ BOOL bShowIconInTask,
            /* [defaultvalue][in] */ BSTR strPicPath,
            /* [defaultvalue][in] */ UINT32 uColor,
            /* [retval][out] */ VARIANT *pDisp);
        
        HRESULT ( STDMETHODCALLTYPE *setWinTitleRect )( 
            ITangWnd * This,
            /* [defaultvalue][in] */ UINT px,
            /* [defaultvalue][in] */ UINT py,
            /* [defaultvalue][in] */ UINT x,
            /* [defaultvalue][in] */ UINT y);
        
        HRESULT ( STDMETHODCALLTYPE *navigate )( 
            ITangWnd * This,
            /* [in] */ BSTR relativePath,
            /* [defaultvalue][in] */ BOOL bLocalPage);
        
        HRESULT ( STDMETHODCALLTYPE *setAnimationArea )( 
            ITangWnd * This,
            /* [in] */ IUnknown *pViewArea,
            /* [in] */ BSTR renderProperty);
        
        HRESULT ( STDMETHODCALLTYPE *removeAnimationArea )( 
            ITangWnd * This,
            /* [in] */ IUnknown *pViewArea);
        
        HRESULT ( STDMETHODCALLTYPE *restoreWndRegular )( 
            ITangWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *showRoundCorner )( 
            ITangWnd * This,
            /* [in] */ BOOL bShow);
        
        HRESULT ( STDMETHODCALLTYPE *setRoundCornerSize )( 
            ITangWnd * This,
            /* [in] */ int cornerSize);
        
        HRESULT ( STDMETHODCALLTYPE *setTrapezoidSize )( 
            ITangWnd * This,
            /* [in] */ int iDvalue);
        
        HRESULT ( STDMETHODCALLTYPE *shadowLess )( 
            ITangWnd * This,
            /* [in] */ BOOL bShadowLess);
        
        HRESULT ( STDMETHODCALLTYPE *behind )( 
            ITangWnd * This,
            /* [in] */ DWORD hwnd);
        
        HRESULT ( STDMETHODCALLTYPE *enableWindow )( 
            ITangWnd * This,
            /* [in] */ BOOL bEnable);
        
        HRESULT ( STDMETHODCALLTYPE *IsEnabledWindow )( 
            ITangWnd * This,
            /* [retval][out] */ BOOL *__MIDL__ITangWnd0017);
        
        HRESULT ( STDMETHODCALLTYPE *setAdsorption )( 
            ITangWnd * This,
            /* [in] */ BOOL bIsAdsorption);
        
        HRESULT ( STDMETHODCALLTYPE *setMinWinHeight )( 
            ITangWnd * This,
            /* [in] */ UINT uMinWinHeight);
        
        HRESULT ( STDMETHODCALLTYPE *setAllowReSize )( 
            ITangWnd * This,
            /* [in] */ BOOL bAllowReSize);
        
        HRESULT ( STDMETHODCALLTYPE *letMeCrash )( 
            ITangWnd * This,
            /* [in] */ UINT32 uCrashType);
        
        HRESULT ( STDMETHODCALLTYPE *flashTaskWindow )( 
            ITangWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *EnableSubWindowList )( 
            ITangWnd * This,
            /* [in] */ BOOL bEnable);
        
        HRESULT ( STDMETHODCALLTYPE *SetAssociatedWithMobile )( 
            ITangWnd * This,
            /* [in] */ UINT32 hParentWnd,
            /* [in] */ UINT32 hSonWnd,
            /* [in] */ int xVal,
            /* [in] */ int yVal);
        
        HRESULT ( STDMETHODCALLTYPE *SetMobileDirection )( 
            ITangWnd * This,
            /* [defaultvalue][in] */ BOOL IsEnabled);
        
        HRESULT ( STDMETHODCALLTYPE *SetLeaveElapse )( 
            ITangWnd * This,
            /* [in] */ UINT32 uElapse);
        
        HRESULT ( STDMETHODCALLTYPE *HasFullScreenSwitch )( 
            ITangWnd * This,
            /* [in] */ BOOL bHas);
        
        HRESULT ( STDMETHODCALLTYPE *FlashTaskWindowEx )( 
            ITangWnd * This,
            /* [in] */ UINT nFlags,
            /* [defaultvalue][in] */ UINT nCount,
            /* [defaultvalue][in] */ UINT nTimeout);
        
        HRESULT ( STDMETHODCALLTYPE *GetCursorPosition )( 
            ITangWnd * This,
            /* [retval][out] */ BSTR *bstrPos);
        
        END_INTERFACE
    } ITangWndVtbl;

    interface ITangWnd
    {
        CONST_VTBL struct ITangWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITangWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITangWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITangWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITangWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITangWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITangWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITangWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITangWnd_animate(This,flags,duration)	\
    ( (This)->lpVtbl -> animate(This,flags,duration) ) 

#define ITangWnd_show(This,uFlags)	\
    ( (This)->lpVtbl -> show(This,uFlags) ) 

#define ITangWnd_doModal(This,__MIDL__ITangWnd0000)	\
    ( (This)->lpVtbl -> doModal(This,__MIDL__ITangWnd0000) ) 

#define ITangWnd_setTitle(This,Title)	\
    ( (This)->lpVtbl -> setTitle(This,Title) ) 

#define ITangWnd_setAOT(This,b)	\
    ( (This)->lpVtbl -> setAOT(This,b) ) 

#define ITangWnd_minimize(This)	\
    ( (This)->lpVtbl -> minimize(This) ) 

#define ITangWnd_maximize(This)	\
    ( (This)->lpVtbl -> maximize(This) ) 

#define ITangWnd_close(This,retVal)	\
    ( (This)->lpVtbl -> close(This,retVal) ) 

#define ITangWnd_setSize(This,width,height)	\
    ( (This)->lpVtbl -> setSize(This,width,height) ) 

#define ITangWnd_setMinSize(This,width,height)	\
    ( (This)->lpVtbl -> setMinSize(This,width,height) ) 

#define ITangWnd_setMaxSize(This,width,height)	\
    ( (This)->lpVtbl -> setMaxSize(This,width,height) ) 

#define ITangWnd_setPos(This,left,top)	\
    ( (This)->lpVtbl -> setPos(This,left,top) ) 

#define ITangWnd_setIcon(This,IconPath)	\
    ( (This)->lpVtbl -> setIcon(This,IconPath) ) 

#define ITangWnd_hide(This,b)	\
    ( (This)->lpVtbl -> hide(This,b) ) 

#define ITangWnd_rightToLeft(This,b)	\
    ( (This)->lpVtbl -> rightToLeft(This,b) ) 

#define ITangWnd_resizeable(This,b)	\
    ( (This)->lpVtbl -> resizeable(This,b) ) 

#define ITangWnd_showMinBox(This,b)	\
    ( (This)->lpVtbl -> showMinBox(This,b) ) 

#define ITangWnd_showMaxBox(This,b)	\
    ( (This)->lpVtbl -> showMaxBox(This,b) ) 

#define ITangWnd_showTitlebar(This,b)	\
    ( (This)->lpVtbl -> showTitlebar(This,b) ) 

#define ITangWnd_translucent(This,percent)	\
    ( (This)->lpVtbl -> translucent(This,percent) ) 

#define ITangWnd_focus(This)	\
    ( (This)->lpVtbl -> focus(This) ) 

#define ITangWnd_setForegroundWindow(This)	\
    ( (This)->lpVtbl -> setForegroundWindow(This) ) 

#define ITangWnd_isActive(This,__MIDL__ITangWnd0001)	\
    ( (This)->lpVtbl -> isActive(This,__MIDL__ITangWnd0001) ) 

#define ITangWnd_pushFocus(This)	\
    ( (This)->lpVtbl -> pushFocus(This) ) 

#define ITangWnd_popFocus(This)	\
    ( (This)->lpVtbl -> popFocus(This) ) 

#define ITangWnd_setMinHandler(This,__MIDL__ITangWnd0002)	\
    ( (This)->lpVtbl -> setMinHandler(This,__MIDL__ITangWnd0002) ) 

#define ITangWnd_setLostFocusHandler(This,__MIDL__ITangWnd0003)	\
    ( (This)->lpVtbl -> setLostFocusHandler(This,__MIDL__ITangWnd0003) ) 

#define ITangWnd_setCloseHandler(This,__MIDL__ITangWnd0004)	\
    ( (This)->lpVtbl -> setCloseHandler(This,__MIDL__ITangWnd0004) ) 

#define ITangWnd_setRestoreHandler(This,__MIDL__ITangWnd0005)	\
    ( (This)->lpVtbl -> setRestoreHandler(This,__MIDL__ITangWnd0005) ) 

#define ITangWnd_setMaxHandler(This,__MIDL__ITangWnd0006)	\
    ( (This)->lpVtbl -> setMaxHandler(This,__MIDL__ITangWnd0006) ) 

#define ITangWnd_setWindowMoveHandler(This,__MIDL__ITangWnd0007)	\
    ( (This)->lpVtbl -> setWindowMoveHandler(This,__MIDL__ITangWnd0007) ) 

#define ITangWnd_setSizeChangedHandler(This,__MIDL__ITangWnd0008)	\
    ( (This)->lpVtbl -> setSizeChangedHandler(This,__MIDL__ITangWnd0008) ) 

#define ITangWnd_registerHotKey(This,__MIDL__ITangWnd0009,__MIDL__ITangWnd0010,__MIDL__ITangWnd0011,__MIDL__ITangWnd0012)	\
    ( (This)->lpVtbl -> registerHotKey(This,__MIDL__ITangWnd0009,__MIDL__ITangWnd0010,__MIDL__ITangWnd0011,__MIDL__ITangWnd0012) ) 

#define ITangWnd_unregisterHotKey(This,__MIDL__ITangWnd0013)	\
    ( (This)->lpVtbl -> unregisterHotKey(This,__MIDL__ITangWnd0013) ) 

#define ITangWnd_restore(This)	\
    ( (This)->lpVtbl -> restore(This) ) 

#define ITangWnd_fullScreen(This,b)	\
    ( (This)->lpVtbl -> fullScreen(This,b) ) 

#define ITangWnd_modifyStyleEx(This,dwRemove,dwAdd,nFlags)	\
    ( (This)->lpVtbl -> modifyStyleEx(This,dwRemove,dwAdd,nFlags) ) 

#define ITangWnd_get_left(This,retVal)	\
    ( (This)->lpVtbl -> get_left(This,retVal) ) 

#define ITangWnd_get_top(This,retVal)	\
    ( (This)->lpVtbl -> get_top(This,retVal) ) 

#define ITangWnd_get_width(This,retVal)	\
    ( (This)->lpVtbl -> get_width(This,retVal) ) 

#define ITangWnd_get_height(This,retVal)	\
    ( (This)->lpVtbl -> get_height(This,retVal) ) 

#define ITangWnd_get_isMinimized(This,__MIDL__ITangWnd0014)	\
    ( (This)->lpVtbl -> get_isMinimized(This,__MIDL__ITangWnd0014) ) 

#define ITangWnd_get_isMaximized(This,__MIDL__ITangWnd0015)	\
    ( (This)->lpVtbl -> get_isMaximized(This,__MIDL__ITangWnd0015) ) 

#define ITangWnd_get_isHidden(This,__MIDL__ITangWnd0016)	\
    ( (This)->lpVtbl -> get_isHidden(This,__MIDL__ITangWnd0016) ) 

#define ITangWnd_get_Hwnd(This,pHwnd)	\
    ( (This)->lpVtbl -> get_Hwnd(This,pHwnd) ) 

#define ITangWnd_get_subWindow(This,name,pDisp)	\
    ( (This)->lpVtbl -> get_subWindow(This,name,pDisp) ) 

#define ITangWnd_fireEvent(This,handler,parameters,nParams,varRet)	\
    ( (This)->lpVtbl -> fireEvent(This,handler,parameters,nParams,varRet) ) 

#define ITangWnd_messageBox(This,modal,text,caption,type,retval)	\
    ( (This)->lpVtbl -> messageBox(This,modal,text,caption,type,retval) ) 

#define ITangWnd_openFileDialog(This,bstrDefExt,bstrDefFileName,bstrFilter,bstrCurrentDir,bstrFileName)	\
    ( (This)->lpVtbl -> openFileDialog(This,bstrDefExt,bstrDefFileName,bstrFilter,bstrCurrentDir,bstrFileName) ) 

#define ITangWnd_selectFolderDialog(This,strDialogTitle,pstrDir)	\
    ( (This)->lpVtbl -> selectFolderDialog(This,strDialogTitle,pstrDir) ) 

#define ITangWnd_saveFileDialog(This,bstrDefExt,bstrDefFileName,bstrFilter,bstrCurrentDir,bstrFileName)	\
    ( (This)->lpVtbl -> saveFileDialog(This,bstrDefExt,bstrDefFileName,bstrFilter,bstrCurrentDir,bstrFileName) ) 

#define ITangWnd_createWindow(This,bModal,name,bstrUrl,bNeedMsg,bShadow,bShowIconInTask,strPicPath,uColor,pDisp)	\
    ( (This)->lpVtbl -> createWindow(This,bModal,name,bstrUrl,bNeedMsg,bShadow,bShowIconInTask,strPicPath,uColor,pDisp) ) 

#define ITangWnd_setWinTitleRect(This,px,py,x,y)	\
    ( (This)->lpVtbl -> setWinTitleRect(This,px,py,x,y) ) 

#define ITangWnd_navigate(This,relativePath,bLocalPage)	\
    ( (This)->lpVtbl -> navigate(This,relativePath,bLocalPage) ) 

#define ITangWnd_setAnimationArea(This,pViewArea,renderProperty)	\
    ( (This)->lpVtbl -> setAnimationArea(This,pViewArea,renderProperty) ) 

#define ITangWnd_removeAnimationArea(This,pViewArea)	\
    ( (This)->lpVtbl -> removeAnimationArea(This,pViewArea) ) 

#define ITangWnd_restoreWndRegular(This)	\
    ( (This)->lpVtbl -> restoreWndRegular(This) ) 

#define ITangWnd_showRoundCorner(This,bShow)	\
    ( (This)->lpVtbl -> showRoundCorner(This,bShow) ) 

#define ITangWnd_setRoundCornerSize(This,cornerSize)	\
    ( (This)->lpVtbl -> setRoundCornerSize(This,cornerSize) ) 

#define ITangWnd_setTrapezoidSize(This,iDvalue)	\
    ( (This)->lpVtbl -> setTrapezoidSize(This,iDvalue) ) 

#define ITangWnd_shadowLess(This,bShadowLess)	\
    ( (This)->lpVtbl -> shadowLess(This,bShadowLess) ) 

#define ITangWnd_behind(This,hwnd)	\
    ( (This)->lpVtbl -> behind(This,hwnd) ) 

#define ITangWnd_enableWindow(This,bEnable)	\
    ( (This)->lpVtbl -> enableWindow(This,bEnable) ) 

#define ITangWnd_IsEnabledWindow(This,__MIDL__ITangWnd0017)	\
    ( (This)->lpVtbl -> IsEnabledWindow(This,__MIDL__ITangWnd0017) ) 

#define ITangWnd_setAdsorption(This,bIsAdsorption)	\
    ( (This)->lpVtbl -> setAdsorption(This,bIsAdsorption) ) 

#define ITangWnd_setMinWinHeight(This,uMinWinHeight)	\
    ( (This)->lpVtbl -> setMinWinHeight(This,uMinWinHeight) ) 

#define ITangWnd_setAllowReSize(This,bAllowReSize)	\
    ( (This)->lpVtbl -> setAllowReSize(This,bAllowReSize) ) 

#define ITangWnd_letMeCrash(This,uCrashType)	\
    ( (This)->lpVtbl -> letMeCrash(This,uCrashType) ) 

#define ITangWnd_flashTaskWindow(This)	\
    ( (This)->lpVtbl -> flashTaskWindow(This) ) 

#define ITangWnd_EnableSubWindowList(This,bEnable)	\
    ( (This)->lpVtbl -> EnableSubWindowList(This,bEnable) ) 

#define ITangWnd_SetAssociatedWithMobile(This,hParentWnd,hSonWnd,xVal,yVal)	\
    ( (This)->lpVtbl -> SetAssociatedWithMobile(This,hParentWnd,hSonWnd,xVal,yVal) ) 

#define ITangWnd_SetMobileDirection(This,IsEnabled)	\
    ( (This)->lpVtbl -> SetMobileDirection(This,IsEnabled) ) 

#define ITangWnd_SetLeaveElapse(This,uElapse)	\
    ( (This)->lpVtbl -> SetLeaveElapse(This,uElapse) ) 

#define ITangWnd_HasFullScreenSwitch(This,bHas)	\
    ( (This)->lpVtbl -> HasFullScreenSwitch(This,bHas) ) 

#define ITangWnd_FlashTaskWindowEx(This,nFlags,nCount,nTimeout)	\
    ( (This)->lpVtbl -> FlashTaskWindowEx(This,nFlags,nCount,nTimeout) ) 

#define ITangWnd_GetCursorPosition(This,bstrPos)	\
    ( (This)->lpVtbl -> GetCursorPosition(This,bstrPos) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITangWnd_INTERFACE_DEFINED__ */


#ifndef __ITangUtil_INTERFACE_DEFINED__
#define __ITangUtil_INTERFACE_DEFINED__

/* interface ITangUtil */
/* [object][uuid] */ 


EXTERN_C const IID IID_ITangUtil;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a4323344-318c-49d8-b2c3-ddeaa60cd36f")
    ITangUtil : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE JoinConference( 
            /* [in] */ UINT32 runMode,
            /* [in] */ BSTR bstrProtocal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE File( 
            /* [in] */ BSTR path,
            /* [defaultvalue][in] */ BOOL bIsWritable,
            /* [retval][out] */ VARIANT *pDisp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FileExists( 
            /* [in] */ BSTR path,
            /* [retval][out] */ BOOL *bExists) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FileCanbeAccess( 
            /* [in] */ BSTR path,
            /* [defaultvalue][in] */ BOOL bIsWritable,
            /* [retval][out] */ BOOL *bAccess) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSpecialFolderPath( 
            /* [in] */ UINT32 iRequest,
            /* [retval][out] */ BSTR *strPath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlaySound( 
            /* [in] */ BSTR soundFile) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCmdLine( 
            /* [retval][out] */ BSTR *strCmdLine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetModulePath( 
            /* [retval][out] */ BSTR *strModulepath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RestartClient( void) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Directory( 
            /* [retval][out] */ VARIANT *pDisp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteLog( 
            /* [in] */ BSTR strAppName,
            /* [in] */ BSTR strFileName,
            /* [in] */ BSTR strLogInfo,
            /* [in] */ int uLogLevel,
            /* [defaultvalue][in] */ int uLine = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsSupportRecordInAndOutSound( 
            /* [retval][out] */ UINT32 *pSupport) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDiskFreeSpaceInfo( 
            /* [in] */ BSTR strDisk,
            /* [retval][out] */ UINT32 *pFree) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LoadConfig( 
            /* [in] */ BSTR strConfigFilePath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetConfigValue( 
            /* [in] */ BSTR strNodePath,
            /* [retval][out] */ BSTR *strConfigValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetConfigValue( 
            /* [in] */ BSTR strNodePath,
            /* [in] */ BSTR strNodeValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetConfigAttriValue( 
            /* [in] */ BSTR strNodePath,
            /* [in] */ BSTR strAttriName,
            /* [retval][out] */ BSTR *strAttriValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetConfigAttriValue( 
            /* [in] */ BSTR strNodePath,
            /* [in] */ BSTR strAttriName,
            /* [in] */ BSTR strAttriValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPhysicalProcessorsNumber( 
            /* [retval][out] */ UINT32 *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLogicalProcessorsNumber( 
            /* [retval][out] */ UINT32 *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTaskBarPos( 
            /* [retval][out] */ UINT32 *pPos) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTaskBarAutoHide( 
            /* [retval][out] */ BOOL *pHide) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLocalMac( 
            /* [retval][out] */ BSTR *pstrLocalMac) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenURL( 
            /* [in] */ BSTR strUrl) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFreeMemory( 
            /* [retval][out] */ UINT32 *pNumber) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsWLanConnected( 
            /* [retval][out] */ BOOL *bConnected) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSysLangID( 
            /* [retval][out] */ UINT32 *pID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMonitorCount( 
            /* [retval][out] */ UINT32 *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMonitorResolution( 
            /* [in] */ UINT32 pIndex,
            /* [retval][out] */ BSTR *pstrMR) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AutoStartClient( 
            /* [in] */ int nOption,
            /* [defaultvalue][retval][out] */ int *pRes = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenPathFile( 
            /* [in] */ BSTR bstrOperation,
            /* [in] */ BSTR bstrEXE,
            /* [in] */ BSTR bstrParameters,
            /* [defaultvalue][in] */ BOOL bUseDefaultEXE = FALSE,
            /* [defaultvalue][retval][out] */ int *pRes = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReNameFile( 
            /* [in] */ BSTR bstrOldFullName,
            /* [in] */ BSTR bstrNewFullName,
            /* [in] */ BOOL bIsForceReName,
            /* [defaultvalue][retval][out] */ int *pRes = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteDirectory( 
            /* [in] */ BSTR bstrDirectory,
            /* [defaultvalue][retval][out] */ int *pRes = 0) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITangUtilVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITangUtil * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITangUtil * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITangUtil * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITangUtil * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITangUtil * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITangUtil * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITangUtil * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *JoinConference )( 
            ITangUtil * This,
            /* [in] */ UINT32 runMode,
            /* [in] */ BSTR bstrProtocal);
        
        HRESULT ( STDMETHODCALLTYPE *File )( 
            ITangUtil * This,
            /* [in] */ BSTR path,
            /* [defaultvalue][in] */ BOOL bIsWritable,
            /* [retval][out] */ VARIANT *pDisp);
        
        HRESULT ( STDMETHODCALLTYPE *FileExists )( 
            ITangUtil * This,
            /* [in] */ BSTR path,
            /* [retval][out] */ BOOL *bExists);
        
        HRESULT ( STDMETHODCALLTYPE *FileCanbeAccess )( 
            ITangUtil * This,
            /* [in] */ BSTR path,
            /* [defaultvalue][in] */ BOOL bIsWritable,
            /* [retval][out] */ BOOL *bAccess);
        
        HRESULT ( STDMETHODCALLTYPE *GetSpecialFolderPath )( 
            ITangUtil * This,
            /* [in] */ UINT32 iRequest,
            /* [retval][out] */ BSTR *strPath);
        
        HRESULT ( STDMETHODCALLTYPE *PlaySound )( 
            ITangUtil * This,
            /* [in] */ BSTR soundFile);
        
        HRESULT ( STDMETHODCALLTYPE *GetCmdLine )( 
            ITangUtil * This,
            /* [retval][out] */ BSTR *strCmdLine);
        
        HRESULT ( STDMETHODCALLTYPE *GetModulePath )( 
            ITangUtil * This,
            /* [retval][out] */ BSTR *strModulepath);
        
        HRESULT ( STDMETHODCALLTYPE *RestartClient )( 
            ITangUtil * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Directory )( 
            ITangUtil * This,
            /* [retval][out] */ VARIANT *pDisp);
        
        HRESULT ( STDMETHODCALLTYPE *WriteLog )( 
            ITangUtil * This,
            /* [in] */ BSTR strAppName,
            /* [in] */ BSTR strFileName,
            /* [in] */ BSTR strLogInfo,
            /* [in] */ int uLogLevel,
            /* [defaultvalue][in] */ int uLine);
        
        HRESULT ( STDMETHODCALLTYPE *IsSupportRecordInAndOutSound )( 
            ITangUtil * This,
            /* [retval][out] */ UINT32 *pSupport);
        
        HRESULT ( STDMETHODCALLTYPE *GetDiskFreeSpaceInfo )( 
            ITangUtil * This,
            /* [in] */ BSTR strDisk,
            /* [retval][out] */ UINT32 *pFree);
        
        HRESULT ( STDMETHODCALLTYPE *LoadConfig )( 
            ITangUtil * This,
            /* [in] */ BSTR strConfigFilePath);
        
        HRESULT ( STDMETHODCALLTYPE *GetConfigValue )( 
            ITangUtil * This,
            /* [in] */ BSTR strNodePath,
            /* [retval][out] */ BSTR *strConfigValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetConfigValue )( 
            ITangUtil * This,
            /* [in] */ BSTR strNodePath,
            /* [in] */ BSTR strNodeValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetConfigAttriValue )( 
            ITangUtil * This,
            /* [in] */ BSTR strNodePath,
            /* [in] */ BSTR strAttriName,
            /* [retval][out] */ BSTR *strAttriValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetConfigAttriValue )( 
            ITangUtil * This,
            /* [in] */ BSTR strNodePath,
            /* [in] */ BSTR strAttriName,
            /* [in] */ BSTR strAttriValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetPhysicalProcessorsNumber )( 
            ITangUtil * This,
            /* [retval][out] */ UINT32 *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetLogicalProcessorsNumber )( 
            ITangUtil * This,
            /* [retval][out] */ UINT32 *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetTaskBarPos )( 
            ITangUtil * This,
            /* [retval][out] */ UINT32 *pPos);
        
        HRESULT ( STDMETHODCALLTYPE *GetTaskBarAutoHide )( 
            ITangUtil * This,
            /* [retval][out] */ BOOL *pHide);
        
        HRESULT ( STDMETHODCALLTYPE *GetLocalMac )( 
            ITangUtil * This,
            /* [retval][out] */ BSTR *pstrLocalMac);
        
        HRESULT ( STDMETHODCALLTYPE *OpenURL )( 
            ITangUtil * This,
            /* [in] */ BSTR strUrl);
        
        HRESULT ( STDMETHODCALLTYPE *GetFreeMemory )( 
            ITangUtil * This,
            /* [retval][out] */ UINT32 *pNumber);
        
        HRESULT ( STDMETHODCALLTYPE *IsWLanConnected )( 
            ITangUtil * This,
            /* [retval][out] */ BOOL *bConnected);
        
        HRESULT ( STDMETHODCALLTYPE *GetSysLangID )( 
            ITangUtil * This,
            /* [retval][out] */ UINT32 *pID);
        
        HRESULT ( STDMETHODCALLTYPE *GetMonitorCount )( 
            ITangUtil * This,
            /* [retval][out] */ UINT32 *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetMonitorResolution )( 
            ITangUtil * This,
            /* [in] */ UINT32 pIndex,
            /* [retval][out] */ BSTR *pstrMR);
        
        HRESULT ( STDMETHODCALLTYPE *AutoStartClient )( 
            ITangUtil * This,
            /* [in] */ int nOption,
            /* [defaultvalue][retval][out] */ int *pRes);
        
        HRESULT ( STDMETHODCALLTYPE *OpenPathFile )( 
            ITangUtil * This,
            /* [in] */ BSTR bstrOperation,
            /* [in] */ BSTR bstrEXE,
            /* [in] */ BSTR bstrParameters,
            /* [defaultvalue][in] */ BOOL bUseDefaultEXE,
            /* [defaultvalue][retval][out] */ int *pRes);
        
        HRESULT ( STDMETHODCALLTYPE *ReNameFile )( 
            ITangUtil * This,
            /* [in] */ BSTR bstrOldFullName,
            /* [in] */ BSTR bstrNewFullName,
            /* [in] */ BOOL bIsForceReName,
            /* [defaultvalue][retval][out] */ int *pRes);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteDirectory )( 
            ITangUtil * This,
            /* [in] */ BSTR bstrDirectory,
            /* [defaultvalue][retval][out] */ int *pRes);
        
        END_INTERFACE
    } ITangUtilVtbl;

    interface ITangUtil
    {
        CONST_VTBL struct ITangUtilVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITangUtil_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITangUtil_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITangUtil_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITangUtil_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITangUtil_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITangUtil_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITangUtil_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITangUtil_JoinConference(This,runMode,bstrProtocal)	\
    ( (This)->lpVtbl -> JoinConference(This,runMode,bstrProtocal) ) 

#define ITangUtil_File(This,path,bIsWritable,pDisp)	\
    ( (This)->lpVtbl -> File(This,path,bIsWritable,pDisp) ) 

#define ITangUtil_FileExists(This,path,bExists)	\
    ( (This)->lpVtbl -> FileExists(This,path,bExists) ) 

#define ITangUtil_FileCanbeAccess(This,path,bIsWritable,bAccess)	\
    ( (This)->lpVtbl -> FileCanbeAccess(This,path,bIsWritable,bAccess) ) 

#define ITangUtil_GetSpecialFolderPath(This,iRequest,strPath)	\
    ( (This)->lpVtbl -> GetSpecialFolderPath(This,iRequest,strPath) ) 

#define ITangUtil_PlaySound(This,soundFile)	\
    ( (This)->lpVtbl -> PlaySound(This,soundFile) ) 

#define ITangUtil_GetCmdLine(This,strCmdLine)	\
    ( (This)->lpVtbl -> GetCmdLine(This,strCmdLine) ) 

#define ITangUtil_GetModulePath(This,strModulepath)	\
    ( (This)->lpVtbl -> GetModulePath(This,strModulepath) ) 

#define ITangUtil_RestartClient(This)	\
    ( (This)->lpVtbl -> RestartClient(This) ) 

#define ITangUtil_get_Directory(This,pDisp)	\
    ( (This)->lpVtbl -> get_Directory(This,pDisp) ) 

#define ITangUtil_WriteLog(This,strAppName,strFileName,strLogInfo,uLogLevel,uLine)	\
    ( (This)->lpVtbl -> WriteLog(This,strAppName,strFileName,strLogInfo,uLogLevel,uLine) ) 

#define ITangUtil_IsSupportRecordInAndOutSound(This,pSupport)	\
    ( (This)->lpVtbl -> IsSupportRecordInAndOutSound(This,pSupport) ) 

#define ITangUtil_GetDiskFreeSpaceInfo(This,strDisk,pFree)	\
    ( (This)->lpVtbl -> GetDiskFreeSpaceInfo(This,strDisk,pFree) ) 

#define ITangUtil_LoadConfig(This,strConfigFilePath)	\
    ( (This)->lpVtbl -> LoadConfig(This,strConfigFilePath) ) 

#define ITangUtil_GetConfigValue(This,strNodePath,strConfigValue)	\
    ( (This)->lpVtbl -> GetConfigValue(This,strNodePath,strConfigValue) ) 

#define ITangUtil_SetConfigValue(This,strNodePath,strNodeValue)	\
    ( (This)->lpVtbl -> SetConfigValue(This,strNodePath,strNodeValue) ) 

#define ITangUtil_GetConfigAttriValue(This,strNodePath,strAttriName,strAttriValue)	\
    ( (This)->lpVtbl -> GetConfigAttriValue(This,strNodePath,strAttriName,strAttriValue) ) 

#define ITangUtil_SetConfigAttriValue(This,strNodePath,strAttriName,strAttriValue)	\
    ( (This)->lpVtbl -> SetConfigAttriValue(This,strNodePath,strAttriName,strAttriValue) ) 

#define ITangUtil_GetPhysicalProcessorsNumber(This,pCount)	\
    ( (This)->lpVtbl -> GetPhysicalProcessorsNumber(This,pCount) ) 

#define ITangUtil_GetLogicalProcessorsNumber(This,pCount)	\
    ( (This)->lpVtbl -> GetLogicalProcessorsNumber(This,pCount) ) 

#define ITangUtil_GetTaskBarPos(This,pPos)	\
    ( (This)->lpVtbl -> GetTaskBarPos(This,pPos) ) 

#define ITangUtil_GetTaskBarAutoHide(This,pHide)	\
    ( (This)->lpVtbl -> GetTaskBarAutoHide(This,pHide) ) 

#define ITangUtil_GetLocalMac(This,pstrLocalMac)	\
    ( (This)->lpVtbl -> GetLocalMac(This,pstrLocalMac) ) 

#define ITangUtil_OpenURL(This,strUrl)	\
    ( (This)->lpVtbl -> OpenURL(This,strUrl) ) 

#define ITangUtil_GetFreeMemory(This,pNumber)	\
    ( (This)->lpVtbl -> GetFreeMemory(This,pNumber) ) 

#define ITangUtil_IsWLanConnected(This,bConnected)	\
    ( (This)->lpVtbl -> IsWLanConnected(This,bConnected) ) 

#define ITangUtil_GetSysLangID(This,pID)	\
    ( (This)->lpVtbl -> GetSysLangID(This,pID) ) 

#define ITangUtil_GetMonitorCount(This,pCount)	\
    ( (This)->lpVtbl -> GetMonitorCount(This,pCount) ) 

#define ITangUtil_GetMonitorResolution(This,pIndex,pstrMR)	\
    ( (This)->lpVtbl -> GetMonitorResolution(This,pIndex,pstrMR) ) 

#define ITangUtil_AutoStartClient(This,nOption,pRes)	\
    ( (This)->lpVtbl -> AutoStartClient(This,nOption,pRes) ) 

#define ITangUtil_OpenPathFile(This,bstrOperation,bstrEXE,bstrParameters,bUseDefaultEXE,pRes)	\
    ( (This)->lpVtbl -> OpenPathFile(This,bstrOperation,bstrEXE,bstrParameters,bUseDefaultEXE,pRes) ) 

#define ITangUtil_ReNameFile(This,bstrOldFullName,bstrNewFullName,bIsForceReName,pRes)	\
    ( (This)->lpVtbl -> ReNameFile(This,bstrOldFullName,bstrNewFullName,bIsForceReName,pRes) ) 

#define ITangUtil_DeleteDirectory(This,bstrDirectory,pRes)	\
    ( (This)->lpVtbl -> DeleteDirectory(This,bstrDirectory,pRes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITangUtil_INTERFACE_DEFINED__ */


#ifndef __ITangSession_INTERFACE_DEFINED__
#define __ITangSession_INTERFACE_DEFINED__

/* interface ITangSession */
/* [object][uuid] */ 


EXTERN_C const IID IID_ITangSession;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("59cc764f-bb11-4dcf-8f91-ed80e56699bf")
    ITangSession : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddItem( 
            /* [in] */ BSTR bstrKey,
            /* [in] */ VARIANT var,
            /* [defaultvalue][retval][out] */ int *pRes = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveItem( 
            /* [in] */ BSTR bstrKey,
            /* [defaultvalue][retval][out] */ int *pRes = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UpdateItem( 
            /* [in] */ BSTR bstrKey,
            /* [in] */ VARIANT var,
            /* [defaultvalue][retval][out] */ BOOL *pIsExist = 0) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_item( 
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ VARIANT *pRetItem) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_count( 
            /* [retval][out] */ UINT32 *pRetCount) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITangSessionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITangSession * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITangSession * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITangSession * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITangSession * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITangSession * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITangSession * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITangSession * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *AddItem )( 
            ITangSession * This,
            /* [in] */ BSTR bstrKey,
            /* [in] */ VARIANT var,
            /* [defaultvalue][retval][out] */ int *pRes);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveItem )( 
            ITangSession * This,
            /* [in] */ BSTR bstrKey,
            /* [defaultvalue][retval][out] */ int *pRes);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateItem )( 
            ITangSession * This,
            /* [in] */ BSTR bstrKey,
            /* [in] */ VARIANT var,
            /* [defaultvalue][retval][out] */ BOOL *pIsExist);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_item )( 
            ITangSession * This,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ VARIANT *pRetItem);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_count )( 
            ITangSession * This,
            /* [retval][out] */ UINT32 *pRetCount);
        
        END_INTERFACE
    } ITangSessionVtbl;

    interface ITangSession
    {
        CONST_VTBL struct ITangSessionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITangSession_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITangSession_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITangSession_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITangSession_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITangSession_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITangSession_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITangSession_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITangSession_AddItem(This,bstrKey,var,pRes)	\
    ( (This)->lpVtbl -> AddItem(This,bstrKey,var,pRes) ) 

#define ITangSession_RemoveItem(This,bstrKey,pRes)	\
    ( (This)->lpVtbl -> RemoveItem(This,bstrKey,pRes) ) 

#define ITangSession_UpdateItem(This,bstrKey,var,pIsExist)	\
    ( (This)->lpVtbl -> UpdateItem(This,bstrKey,var,pIsExist) ) 

#define ITangSession_get_item(This,bstrKey,pRetItem)	\
    ( (This)->lpVtbl -> get_item(This,bstrKey,pRetItem) ) 

#define ITangSession_get_count(This,pRetCount)	\
    ( (This)->lpVtbl -> get_count(This,pRetCount) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITangSession_INTERFACE_DEFINED__ */


#ifndef __ITangFile_INTERFACE_DEFINED__
#define __ITangFile_INTERFACE_DEFINED__

/* interface ITangFile */
/* [object][uuid] */ 


EXTERN_C const IID IID_ITangFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5cb0b6c2-37a8-455a-8c98-3a0ebd0e1089")
    ITangFile : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ BSTR strFileName,
            /* [in] */ unsigned int dwAccess,
            /* [in] */ unsigned int dwShare,
            /* [in] */ unsigned int dwDisposition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ int fpLow,
            /* [in] */ int fpHigh,
            /* [in] */ unsigned int dwMethod) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEOF( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [out] */ BYTE *pBuffer,
            /* [in] */ unsigned int nRead) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ BYTE *pBuffer,
            /* [in] */ unsigned int nWrite) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadLine( 
            /* [retval][out] */ BSTR *strLine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadLastLine( 
            /* [retval][out] */ BSTR *strLine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadAll( 
            /* [retval][out] */ BSTR *strLine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteLine( 
            /* [in] */ BSTR strLine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadBase64( 
            /* [retval][out] */ BSTR *strBase64) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteBase64( 
            /* [in] */ BSTR strBase64) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ BSTR strTo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Copy( 
            /* [in] */ BSTR strTo,
            /* [defaultvalue][in] */ BOOL bOverWrite,
            /* [retval][out] */ ITangFile **retCopy) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Delete( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EmptyFile( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Flush( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AtEnd( 
            /* [retval][out] */ BOOL *bAtEnd) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Size( 
            /* [retval][out] */ unsigned int *nSize) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FileName( 
            /* [retval][out] */ BSTR *strFileName) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SHA1( 
            /* [retval][out] */ BSTR *strSHA1) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LastWriteTime( 
            /* [retval][out] */ VARIANT *vLastWriteTime) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITangFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITangFile * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITangFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITangFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITangFile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITangFile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITangFile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITangFile * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *Create )( 
            ITangFile * This,
            /* [in] */ BSTR strFileName,
            /* [in] */ unsigned int dwAccess,
            /* [in] */ unsigned int dwShare,
            /* [in] */ unsigned int dwDisposition);
        
        HRESULT ( STDMETHODCALLTYPE *Seek )( 
            ITangFile * This,
            /* [in] */ int fpLow,
            /* [in] */ int fpHigh,
            /* [in] */ unsigned int dwMethod);
        
        HRESULT ( STDMETHODCALLTYPE *SetEOF )( 
            ITangFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *Read )( 
            ITangFile * This,
            /* [out] */ BYTE *pBuffer,
            /* [in] */ unsigned int nRead);
        
        HRESULT ( STDMETHODCALLTYPE *Write )( 
            ITangFile * This,
            /* [in] */ BYTE *pBuffer,
            /* [in] */ unsigned int nWrite);
        
        HRESULT ( STDMETHODCALLTYPE *ReadLine )( 
            ITangFile * This,
            /* [retval][out] */ BSTR *strLine);
        
        HRESULT ( STDMETHODCALLTYPE *ReadLastLine )( 
            ITangFile * This,
            /* [retval][out] */ BSTR *strLine);
        
        HRESULT ( STDMETHODCALLTYPE *ReadAll )( 
            ITangFile * This,
            /* [retval][out] */ BSTR *strLine);
        
        HRESULT ( STDMETHODCALLTYPE *WriteLine )( 
            ITangFile * This,
            /* [in] */ BSTR strLine);
        
        HRESULT ( STDMETHODCALLTYPE *ReadBase64 )( 
            ITangFile * This,
            /* [retval][out] */ BSTR *strBase64);
        
        HRESULT ( STDMETHODCALLTYPE *WriteBase64 )( 
            ITangFile * This,
            /* [in] */ BSTR strBase64);
        
        HRESULT ( STDMETHODCALLTYPE *Move )( 
            ITangFile * This,
            /* [in] */ BSTR strTo);
        
        HRESULT ( STDMETHODCALLTYPE *Copy )( 
            ITangFile * This,
            /* [in] */ BSTR strTo,
            /* [defaultvalue][in] */ BOOL bOverWrite,
            /* [retval][out] */ ITangFile **retCopy);
        
        HRESULT ( STDMETHODCALLTYPE *Delete )( 
            ITangFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *EmptyFile )( 
            ITangFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *Flush )( 
            ITangFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            ITangFile * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AtEnd )( 
            ITangFile * This,
            /* [retval][out] */ BOOL *bAtEnd);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Size )( 
            ITangFile * This,
            /* [retval][out] */ unsigned int *nSize);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileName )( 
            ITangFile * This,
            /* [retval][out] */ BSTR *strFileName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_SHA1 )( 
            ITangFile * This,
            /* [retval][out] */ BSTR *strSHA1);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastWriteTime )( 
            ITangFile * This,
            /* [retval][out] */ VARIANT *vLastWriteTime);
        
        END_INTERFACE
    } ITangFileVtbl;

    interface ITangFile
    {
        CONST_VTBL struct ITangFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITangFile_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITangFile_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITangFile_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITangFile_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITangFile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITangFile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITangFile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITangFile_Create(This,strFileName,dwAccess,dwShare,dwDisposition)	\
    ( (This)->lpVtbl -> Create(This,strFileName,dwAccess,dwShare,dwDisposition) ) 

#define ITangFile_Seek(This,fpLow,fpHigh,dwMethod)	\
    ( (This)->lpVtbl -> Seek(This,fpLow,fpHigh,dwMethod) ) 

#define ITangFile_SetEOF(This)	\
    ( (This)->lpVtbl -> SetEOF(This) ) 

#define ITangFile_Read(This,pBuffer,nRead)	\
    ( (This)->lpVtbl -> Read(This,pBuffer,nRead) ) 

#define ITangFile_Write(This,pBuffer,nWrite)	\
    ( (This)->lpVtbl -> Write(This,pBuffer,nWrite) ) 

#define ITangFile_ReadLine(This,strLine)	\
    ( (This)->lpVtbl -> ReadLine(This,strLine) ) 

#define ITangFile_ReadLastLine(This,strLine)	\
    ( (This)->lpVtbl -> ReadLastLine(This,strLine) ) 

#define ITangFile_ReadAll(This,strLine)	\
    ( (This)->lpVtbl -> ReadAll(This,strLine) ) 

#define ITangFile_WriteLine(This,strLine)	\
    ( (This)->lpVtbl -> WriteLine(This,strLine) ) 

#define ITangFile_ReadBase64(This,strBase64)	\
    ( (This)->lpVtbl -> ReadBase64(This,strBase64) ) 

#define ITangFile_WriteBase64(This,strBase64)	\
    ( (This)->lpVtbl -> WriteBase64(This,strBase64) ) 

#define ITangFile_Move(This,strTo)	\
    ( (This)->lpVtbl -> Move(This,strTo) ) 

#define ITangFile_Copy(This,strTo,bOverWrite,retCopy)	\
    ( (This)->lpVtbl -> Copy(This,strTo,bOverWrite,retCopy) ) 

#define ITangFile_Delete(This)	\
    ( (This)->lpVtbl -> Delete(This) ) 

#define ITangFile_EmptyFile(This)	\
    ( (This)->lpVtbl -> EmptyFile(This) ) 

#define ITangFile_Flush(This)	\
    ( (This)->lpVtbl -> Flush(This) ) 

#define ITangFile_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define ITangFile_get_AtEnd(This,bAtEnd)	\
    ( (This)->lpVtbl -> get_AtEnd(This,bAtEnd) ) 

#define ITangFile_get_Size(This,nSize)	\
    ( (This)->lpVtbl -> get_Size(This,nSize) ) 

#define ITangFile_get_FileName(This,strFileName)	\
    ( (This)->lpVtbl -> get_FileName(This,strFileName) ) 

#define ITangFile_get_SHA1(This,strSHA1)	\
    ( (This)->lpVtbl -> get_SHA1(This,strSHA1) ) 

#define ITangFile_get_LastWriteTime(This,vLastWriteTime)	\
    ( (This)->lpVtbl -> get_LastWriteTime(This,vLastWriteTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITangFile_INTERFACE_DEFINED__ */


#ifndef __ITangDirectory_INTERFACE_DEFINED__
#define __ITangDirectory_INTERFACE_DEFINED__

/* interface ITangDirectory */
/* [object][uuid] */ 


EXTERN_C const IID IID_ITangDirectory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9c807340-c361-4a2f-a716-16272af4d681")
    ITangDirectory : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ BSTR path) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Exists( 
            /* [in] */ BSTR path,
            /* [retval][out] */ BOOL *bExists) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ListDirs( 
            /* [in] */ BSTR path,
            /* [retval][out] */ VARIANT *list) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ListFiles( 
            /* [in] */ BSTR path,
            /* [retval][out] */ VARIANT *list) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Delete( 
            /* [in] */ BSTR path) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITangDirectoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITangDirectory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITangDirectory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITangDirectory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITangDirectory * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITangDirectory * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITangDirectory * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITangDirectory * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *Create )( 
            ITangDirectory * This,
            /* [in] */ BSTR path);
        
        HRESULT ( STDMETHODCALLTYPE *Exists )( 
            ITangDirectory * This,
            /* [in] */ BSTR path,
            /* [retval][out] */ BOOL *bExists);
        
        HRESULT ( STDMETHODCALLTYPE *ListDirs )( 
            ITangDirectory * This,
            /* [in] */ BSTR path,
            /* [retval][out] */ VARIANT *list);
        
        HRESULT ( STDMETHODCALLTYPE *ListFiles )( 
            ITangDirectory * This,
            /* [in] */ BSTR path,
            /* [retval][out] */ VARIANT *list);
        
        HRESULT ( STDMETHODCALLTYPE *Delete )( 
            ITangDirectory * This,
            /* [in] */ BSTR path);
        
        END_INTERFACE
    } ITangDirectoryVtbl;

    interface ITangDirectory
    {
        CONST_VTBL struct ITangDirectoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITangDirectory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITangDirectory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITangDirectory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITangDirectory_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITangDirectory_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITangDirectory_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITangDirectory_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITangDirectory_Create(This,path)	\
    ( (This)->lpVtbl -> Create(This,path) ) 

#define ITangDirectory_Exists(This,path,bExists)	\
    ( (This)->lpVtbl -> Exists(This,path,bExists) ) 

#define ITangDirectory_ListDirs(This,path,list)	\
    ( (This)->lpVtbl -> ListDirs(This,path,list) ) 

#define ITangDirectory_ListFiles(This,path,list)	\
    ( (This)->lpVtbl -> ListFiles(This,path,list) ) 

#define ITangDirectory_Delete(This,path)	\
    ( (This)->lpVtbl -> Delete(This,path) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITangDirectory_INTERFACE_DEFINED__ */


#ifndef __IListEntry_INTERFACE_DEFINED__
#define __IListEntry_INTERFACE_DEFINED__

/* interface IListEntry */
/* [object][uuid] */ 


EXTERN_C const IID IID_IListEntry;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c7e947d6-d422-4190-ac47-bd51b60ed3bd")
    IListEntry : public IDispatch
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsArchive( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsCompressed( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsDirectory( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsEncrypted( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsHidden( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsNormal( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsOffline( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsReadOnly( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HasReparsePoint( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsSparseFile( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsSystem( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsTemporary( 
            /* [retval][out] */ BOOL *b) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_CreationTime( 
            /* [retval][out] */ BSTR *str) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LastAccessTime( 
            /* [retval][out] */ BSTR *str) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LastWriteTime( 
            /* [retval][out] */ BSTR *str) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FileSizeHigh( 
            /* [retval][out] */ DWORD *dw) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FileSizeLow( 
            /* [retval][out] */ DWORD *dw) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ReparseTag( 
            /* [retval][out] */ DWORD *dw) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Reserved1( 
            /* [retval][out] */ DWORD *dw) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *str) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AlternateName( 
            /* [retval][out] */ BSTR *str) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IListEntryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IListEntry * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IListEntry * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IListEntry * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IListEntry * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IListEntry * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IListEntry * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IListEntry * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsArchive )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsCompressed )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsDirectory )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsEncrypted )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsHidden )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsNormal )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsOffline )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsReadOnly )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HasReparsePoint )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsSparseFile )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsSystem )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsTemporary )( 
            IListEntry * This,
            /* [retval][out] */ BOOL *b);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_CreationTime )( 
            IListEntry * This,
            /* [retval][out] */ BSTR *str);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastAccessTime )( 
            IListEntry * This,
            /* [retval][out] */ BSTR *str);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastWriteTime )( 
            IListEntry * This,
            /* [retval][out] */ BSTR *str);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileSizeHigh )( 
            IListEntry * This,
            /* [retval][out] */ DWORD *dw);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileSizeLow )( 
            IListEntry * This,
            /* [retval][out] */ DWORD *dw);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ReparseTag )( 
            IListEntry * This,
            /* [retval][out] */ DWORD *dw);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Reserved1 )( 
            IListEntry * This,
            /* [retval][out] */ DWORD *dw);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IListEntry * This,
            /* [retval][out] */ BSTR *str);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AlternateName )( 
            IListEntry * This,
            /* [retval][out] */ BSTR *str);
        
        END_INTERFACE
    } IListEntryVtbl;

    interface IListEntry
    {
        CONST_VTBL struct IListEntryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IListEntry_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IListEntry_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IListEntry_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IListEntry_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IListEntry_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IListEntry_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IListEntry_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IListEntry_get_IsArchive(This,b)	\
    ( (This)->lpVtbl -> get_IsArchive(This,b) ) 

#define IListEntry_get_IsCompressed(This,b)	\
    ( (This)->lpVtbl -> get_IsCompressed(This,b) ) 

#define IListEntry_get_IsDirectory(This,b)	\
    ( (This)->lpVtbl -> get_IsDirectory(This,b) ) 

#define IListEntry_get_IsEncrypted(This,b)	\
    ( (This)->lpVtbl -> get_IsEncrypted(This,b) ) 

#define IListEntry_get_IsHidden(This,b)	\
    ( (This)->lpVtbl -> get_IsHidden(This,b) ) 

#define IListEntry_get_IsNormal(This,b)	\
    ( (This)->lpVtbl -> get_IsNormal(This,b) ) 

#define IListEntry_get_IsOffline(This,b)	\
    ( (This)->lpVtbl -> get_IsOffline(This,b) ) 

#define IListEntry_get_IsReadOnly(This,b)	\
    ( (This)->lpVtbl -> get_IsReadOnly(This,b) ) 

#define IListEntry_get_HasReparsePoint(This,b)	\
    ( (This)->lpVtbl -> get_HasReparsePoint(This,b) ) 

#define IListEntry_get_IsSparseFile(This,b)	\
    ( (This)->lpVtbl -> get_IsSparseFile(This,b) ) 

#define IListEntry_get_IsSystem(This,b)	\
    ( (This)->lpVtbl -> get_IsSystem(This,b) ) 

#define IListEntry_get_IsTemporary(This,b)	\
    ( (This)->lpVtbl -> get_IsTemporary(This,b) ) 

#define IListEntry_get_CreationTime(This,str)	\
    ( (This)->lpVtbl -> get_CreationTime(This,str) ) 

#define IListEntry_get_LastAccessTime(This,str)	\
    ( (This)->lpVtbl -> get_LastAccessTime(This,str) ) 

#define IListEntry_get_LastWriteTime(This,str)	\
    ( (This)->lpVtbl -> get_LastWriteTime(This,str) ) 

#define IListEntry_get_FileSizeHigh(This,dw)	\
    ( (This)->lpVtbl -> get_FileSizeHigh(This,dw) ) 

#define IListEntry_get_FileSizeLow(This,dw)	\
    ( (This)->lpVtbl -> get_FileSizeLow(This,dw) ) 

#define IListEntry_get_ReparseTag(This,dw)	\
    ( (This)->lpVtbl -> get_ReparseTag(This,dw) ) 

#define IListEntry_get_Reserved1(This,dw)	\
    ( (This)->lpVtbl -> get_Reserved1(This,dw) ) 

#define IListEntry_get_Name(This,str)	\
    ( (This)->lpVtbl -> get_Name(This,str) ) 

#define IListEntry_get_AlternateName(This,str)	\
    ( (This)->lpVtbl -> get_AlternateName(This,str) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IListEntry_INTERFACE_DEFINED__ */


#ifndef __IUpdate_INTERFACE_DEFINED__
#define __IUpdate_INTERFACE_DEFINED__

/* interface IUpdate */
/* [object][uuid] */ 


EXTERN_C const IID IID_IUpdate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("44bf47ea-f60d-41c1-8d4b-40e1fa491e75")
    IUpdate : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetXmlDocument( 
            /* [in] */ IUnknown *pXmlDoc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveXmlDocument( 
            /* [in] */ IUnknown *pXmlDoc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckUpdates( 
            /* [in] */ UINT uAppId,
            /* [in] */ UINT uSiteId,
            /* [in] */ UINT uSkinId,
            /* [in] */ BSTR szServerUrl,
            /* [in] */ UINT uHttpPort,
            /* [in] */ UINT uTimeout,
            /* [in] */ BSTR szCmdLine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Download( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelDownload( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Install( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Restart( 
            /* [in] */ BSTR szTangCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveFile( 
            /* [in] */ BSTR szFullFileName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveFolder( 
            /* [in] */ BSTR szFullFolderName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UpdateUrlProtocol( 
            /* [in] */ BSTR szProtocol,
            /* [in] */ BSTR szFullAppName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveUrlProtocol( 
            /* [in] */ BSTR szProtocol) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UpdateDesktopLink( 
            /* [in] */ BSTR szName,
            /* [in] */ BSTR szFullAppName,
            /* [in] */ BSTR szCmdArgs,
            /* [in] */ BSTR szIco) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveDesktopLink( 
            /* [in] */ BSTR szName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UpdateStartMenu( 
            /* [in] */ BSTR szName,
            /* [in] */ BSTR szFullAppName,
            /* [in] */ BSTR szCmdArgs,
            /* [in] */ BSTR szIco) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveStartMenu( 
            /* [in] */ BSTR szName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadCookie( 
            /* [in] */ BSTR szUrl,
            /* [in] */ BSTR szName,
            /* [retval][out] */ BSTR *szContent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UpdateCookie( 
            /* [in] */ BSTR szUrl,
            /* [in] */ BSTR szName,
            /* [in] */ BSTR szContent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveCookie( 
            /* [in] */ BSTR szUrl,
            /* [in] */ BSTR szName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSiteId( 
            /* [in] */ UINT nSiteId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSiteId( 
            /* [retval][out] */ UINT *nSiteId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetVersion( 
            /* [in] */ BSTR bstrVersion) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [retval][out] */ BSTR *pbstrVersion) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetLanguage( 
            /* [in] */ BSTR bstrLanguage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLanguage( 
            /* [retval][out] */ BSTR *bstrLanguage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInstallPath( 
            /* [retval][out] */ BSTR *bstrPath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUpdateVersion( 
            /* [retval][out] */ BSTR *pbstrVersion) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDownloadDomain( 
            /* [retval][out] */ BSTR *pbstrDomain) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUpdateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUpdate * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUpdate * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUpdate * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUpdate * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUpdate * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUpdate * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUpdate * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *SetXmlDocument )( 
            IUpdate * This,
            /* [in] */ IUnknown *pXmlDoc);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveXmlDocument )( 
            IUpdate * This,
            /* [in] */ IUnknown *pXmlDoc);
        
        HRESULT ( STDMETHODCALLTYPE *CheckUpdates )( 
            IUpdate * This,
            /* [in] */ UINT uAppId,
            /* [in] */ UINT uSiteId,
            /* [in] */ UINT uSkinId,
            /* [in] */ BSTR szServerUrl,
            /* [in] */ UINT uHttpPort,
            /* [in] */ UINT uTimeout,
            /* [in] */ BSTR szCmdLine);
        
        HRESULT ( STDMETHODCALLTYPE *Download )( 
            IUpdate * This);
        
        HRESULT ( STDMETHODCALLTYPE *CancelDownload )( 
            IUpdate * This);
        
        HRESULT ( STDMETHODCALLTYPE *Install )( 
            IUpdate * This);
        
        HRESULT ( STDMETHODCALLTYPE *Restart )( 
            IUpdate * This,
            /* [in] */ BSTR szTangCmd);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveFile )( 
            IUpdate * This,
            /* [in] */ BSTR szFullFileName);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveFolder )( 
            IUpdate * This,
            /* [in] */ BSTR szFullFolderName);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateUrlProtocol )( 
            IUpdate * This,
            /* [in] */ BSTR szProtocol,
            /* [in] */ BSTR szFullAppName);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveUrlProtocol )( 
            IUpdate * This,
            /* [in] */ BSTR szProtocol);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateDesktopLink )( 
            IUpdate * This,
            /* [in] */ BSTR szName,
            /* [in] */ BSTR szFullAppName,
            /* [in] */ BSTR szCmdArgs,
            /* [in] */ BSTR szIco);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveDesktopLink )( 
            IUpdate * This,
            /* [in] */ BSTR szName);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateStartMenu )( 
            IUpdate * This,
            /* [in] */ BSTR szName,
            /* [in] */ BSTR szFullAppName,
            /* [in] */ BSTR szCmdArgs,
            /* [in] */ BSTR szIco);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveStartMenu )( 
            IUpdate * This,
            /* [in] */ BSTR szName);
        
        HRESULT ( STDMETHODCALLTYPE *ReadCookie )( 
            IUpdate * This,
            /* [in] */ BSTR szUrl,
            /* [in] */ BSTR szName,
            /* [retval][out] */ BSTR *szContent);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateCookie )( 
            IUpdate * This,
            /* [in] */ BSTR szUrl,
            /* [in] */ BSTR szName,
            /* [in] */ BSTR szContent);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveCookie )( 
            IUpdate * This,
            /* [in] */ BSTR szUrl,
            /* [in] */ BSTR szName);
        
        HRESULT ( STDMETHODCALLTYPE *SetSiteId )( 
            IUpdate * This,
            /* [in] */ UINT nSiteId);
        
        HRESULT ( STDMETHODCALLTYPE *GetSiteId )( 
            IUpdate * This,
            /* [retval][out] */ UINT *nSiteId);
        
        HRESULT ( STDMETHODCALLTYPE *SetVersion )( 
            IUpdate * This,
            /* [in] */ BSTR bstrVersion);
        
        HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IUpdate * This,
            /* [retval][out] */ BSTR *pbstrVersion);
        
        HRESULT ( STDMETHODCALLTYPE *SetLanguage )( 
            IUpdate * This,
            /* [in] */ BSTR bstrLanguage);
        
        HRESULT ( STDMETHODCALLTYPE *GetLanguage )( 
            IUpdate * This,
            /* [retval][out] */ BSTR *bstrLanguage);
        
        HRESULT ( STDMETHODCALLTYPE *GetInstallPath )( 
            IUpdate * This,
            /* [retval][out] */ BSTR *bstrPath);
        
        HRESULT ( STDMETHODCALLTYPE *GetUpdateVersion )( 
            IUpdate * This,
            /* [retval][out] */ BSTR *pbstrVersion);
        
        HRESULT ( STDMETHODCALLTYPE *GetDownloadDomain )( 
            IUpdate * This,
            /* [retval][out] */ BSTR *pbstrDomain);
        
        END_INTERFACE
    } IUpdateVtbl;

    interface IUpdate
    {
        CONST_VTBL struct IUpdateVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUpdate_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUpdate_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUpdate_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUpdate_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUpdate_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUpdate_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUpdate_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUpdate_SetXmlDocument(This,pXmlDoc)	\
    ( (This)->lpVtbl -> SetXmlDocument(This,pXmlDoc) ) 

#define IUpdate_RemoveXmlDocument(This,pXmlDoc)	\
    ( (This)->lpVtbl -> RemoveXmlDocument(This,pXmlDoc) ) 

#define IUpdate_CheckUpdates(This,uAppId,uSiteId,uSkinId,szServerUrl,uHttpPort,uTimeout,szCmdLine)	\
    ( (This)->lpVtbl -> CheckUpdates(This,uAppId,uSiteId,uSkinId,szServerUrl,uHttpPort,uTimeout,szCmdLine) ) 

#define IUpdate_Download(This)	\
    ( (This)->lpVtbl -> Download(This) ) 

#define IUpdate_CancelDownload(This)	\
    ( (This)->lpVtbl -> CancelDownload(This) ) 

#define IUpdate_Install(This)	\
    ( (This)->lpVtbl -> Install(This) ) 

#define IUpdate_Restart(This,szTangCmd)	\
    ( (This)->lpVtbl -> Restart(This,szTangCmd) ) 

#define IUpdate_RemoveFile(This,szFullFileName)	\
    ( (This)->lpVtbl -> RemoveFile(This,szFullFileName) ) 

#define IUpdate_RemoveFolder(This,szFullFolderName)	\
    ( (This)->lpVtbl -> RemoveFolder(This,szFullFolderName) ) 

#define IUpdate_UpdateUrlProtocol(This,szProtocol,szFullAppName)	\
    ( (This)->lpVtbl -> UpdateUrlProtocol(This,szProtocol,szFullAppName) ) 

#define IUpdate_RemoveUrlProtocol(This,szProtocol)	\
    ( (This)->lpVtbl -> RemoveUrlProtocol(This,szProtocol) ) 

#define IUpdate_UpdateDesktopLink(This,szName,szFullAppName,szCmdArgs,szIco)	\
    ( (This)->lpVtbl -> UpdateDesktopLink(This,szName,szFullAppName,szCmdArgs,szIco) ) 

#define IUpdate_RemoveDesktopLink(This,szName)	\
    ( (This)->lpVtbl -> RemoveDesktopLink(This,szName) ) 

#define IUpdate_UpdateStartMenu(This,szName,szFullAppName,szCmdArgs,szIco)	\
    ( (This)->lpVtbl -> UpdateStartMenu(This,szName,szFullAppName,szCmdArgs,szIco) ) 

#define IUpdate_RemoveStartMenu(This,szName)	\
    ( (This)->lpVtbl -> RemoveStartMenu(This,szName) ) 

#define IUpdate_ReadCookie(This,szUrl,szName,szContent)	\
    ( (This)->lpVtbl -> ReadCookie(This,szUrl,szName,szContent) ) 

#define IUpdate_UpdateCookie(This,szUrl,szName,szContent)	\
    ( (This)->lpVtbl -> UpdateCookie(This,szUrl,szName,szContent) ) 

#define IUpdate_RemoveCookie(This,szUrl,szName)	\
    ( (This)->lpVtbl -> RemoveCookie(This,szUrl,szName) ) 

#define IUpdate_SetSiteId(This,nSiteId)	\
    ( (This)->lpVtbl -> SetSiteId(This,nSiteId) ) 

#define IUpdate_GetSiteId(This,nSiteId)	\
    ( (This)->lpVtbl -> GetSiteId(This,nSiteId) ) 

#define IUpdate_SetVersion(This,bstrVersion)	\
    ( (This)->lpVtbl -> SetVersion(This,bstrVersion) ) 

#define IUpdate_GetVersion(This,pbstrVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,pbstrVersion) ) 

#define IUpdate_SetLanguage(This,bstrLanguage)	\
    ( (This)->lpVtbl -> SetLanguage(This,bstrLanguage) ) 

#define IUpdate_GetLanguage(This,bstrLanguage)	\
    ( (This)->lpVtbl -> GetLanguage(This,bstrLanguage) ) 

#define IUpdate_GetInstallPath(This,bstrPath)	\
    ( (This)->lpVtbl -> GetInstallPath(This,bstrPath) ) 

#define IUpdate_GetUpdateVersion(This,pbstrVersion)	\
    ( (This)->lpVtbl -> GetUpdateVersion(This,pbstrVersion) ) 

#define IUpdate_GetDownloadDomain(This,pbstrDomain)	\
    ( (This)->lpVtbl -> GetDownloadDomain(This,pbstrDomain) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUpdate_INTERFACE_DEFINED__ */


#ifndef __IEventCenter_INTERFACE_DEFINED__
#define __IEventCenter_INTERFACE_DEFINED__

/* interface IEventCenter */
/* [object][uuid] */ 


EXTERN_C const IID IID_IEventCenter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e299a08a-2af2-4f38-867c-0f49f5f655d3")
    IEventCenter : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RegUserEvent( 
            /* [in] */ BSTR eventName,
            /* [in] */ VARIANT *callbackFunc,
            /* [in] */ VARIANT varDocument,
            /* [retval][out] */ VARIANT_BOOL *pbVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnRegUserEvent( 
            /* [in] */ VARIANT varDocument) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnRegUserEventEx( 
            /* [in] */ BSTR eventName,
            /* [in] */ VARIANT varDocument) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FireUserEvent( 
            /* [in] */ BSTR eventName,
            /* [defaultvalue][in] */ VARIANT *params,
            /* [retval][out] */ VARIANT *pVarResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventCenterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEventCenter * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEventCenter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEventCenter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEventCenter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEventCenter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEventCenter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEventCenter * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *RegUserEvent )( 
            IEventCenter * This,
            /* [in] */ BSTR eventName,
            /* [in] */ VARIANT *callbackFunc,
            /* [in] */ VARIANT varDocument,
            /* [retval][out] */ VARIANT_BOOL *pbVal);
        
        HRESULT ( STDMETHODCALLTYPE *UnRegUserEvent )( 
            IEventCenter * This,
            /* [in] */ VARIANT varDocument);
        
        HRESULT ( STDMETHODCALLTYPE *UnRegUserEventEx )( 
            IEventCenter * This,
            /* [in] */ BSTR eventName,
            /* [in] */ VARIANT varDocument);
        
        HRESULT ( STDMETHODCALLTYPE *FireUserEvent )( 
            IEventCenter * This,
            /* [in] */ BSTR eventName,
            /* [defaultvalue][in] */ VARIANT *params,
            /* [retval][out] */ VARIANT *pVarResult);
        
        END_INTERFACE
    } IEventCenterVtbl;

    interface IEventCenter
    {
        CONST_VTBL struct IEventCenterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventCenter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEventCenter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEventCenter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEventCenter_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEventCenter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEventCenter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEventCenter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IEventCenter_RegUserEvent(This,eventName,callbackFunc,varDocument,pbVal)	\
    ( (This)->lpVtbl -> RegUserEvent(This,eventName,callbackFunc,varDocument,pbVal) ) 

#define IEventCenter_UnRegUserEvent(This,varDocument)	\
    ( (This)->lpVtbl -> UnRegUserEvent(This,varDocument) ) 

#define IEventCenter_UnRegUserEventEx(This,eventName,varDocument)	\
    ( (This)->lpVtbl -> UnRegUserEventEx(This,eventName,varDocument) ) 

#define IEventCenter_FireUserEvent(This,eventName,params,pVarResult)	\
    ( (This)->lpVtbl -> FireUserEvent(This,eventName,params,pVarResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEventCenter_INTERFACE_DEFINED__ */

#endif /* __UCUpdateLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


