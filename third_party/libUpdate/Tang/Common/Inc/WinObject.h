/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file WindObject.h
* @brief implementation of the WindowControl class.
* 
* @author Hongwei Hu (hongwei.hu@gnetis.com)
*
* @date April. 16 2010
*
* @version 1.0.0
* 
* @warning 
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug CR0001      Hongwei Hu     Janu. 26 2010   create version 1.0.0\n

******************************************************************************/

#pragma once

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include "Commonlib.h"

/**
* @class WindowControl
* @brief WindowControl designed to provide a windows object to desktop and av modules.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
class ATL_NO_VTABLE WindowControl:public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<ITangWinControl>,
    public CComControl<WindowControl>,
    public IOleInPlaceObjectWindowlessImpl<WindowControl>,
    public IOleInPlaceActiveObjectImpl<WindowControl>,
    public IQuickActivateImpl<WindowControl>,
    public IOleObjectImpl<WindowControl>,
    public IOleControlImpl<WindowControl>,
    public IViewObjectExImpl<WindowControl>
{
    BEGIN_COM_MAP(WindowControl)
        COM_INTERFACE_ENTRY(ITangWinControl)
        COM_INTERFACE_ENTRY(IClassFactory)
        COM_INTERFACE_ENTRY_IMPL(IOleInPlaceObjectWindowless)
        COM_INTERFACE_ENTRY_IMPL_IID(IID_IOleInPlaceObject, IOleInPlaceObjectWindowless)
        COM_INTERFACE_ENTRY_IMPL_IID(IID_IOleWindow, IOleInPlaceObjectWindowless)
        COM_INTERFACE_ENTRY_IMPL(IOleInPlaceActiveObject)
        COM_INTERFACE_ENTRY_IMPL(IQuickActivate)
        COM_INTERFACE_ENTRY_IMPL(IOleControl)
        COM_INTERFACE_ENTRY_IMPL(IOleObject)
        COM_INTERFACE_ENTRY_IMPL(IViewObjectEx)
    END_COM_MAP()

    BEGIN_PROPERTY_MAP(WindowControl)
        //TODO: will provide property to html
    END_PROPERTY_MAP()

    BEGIN_MSG_MAP(WindowControl)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        ALT_MSG_MAP(1) // 处理edit容器的消息
    END_MSG_MAP()

public:

    /**
    * @brief default constructor of WindowControl class.
    * @param [in] void.
    * @no return value.
    */
    WindowControl();
    ~WindowControl();

    /**
    * @brief get the class id of window object.
    * @param [in] void.
    * @return window object class id.
    */
    static CLSID& WINAPI GetObjectCLSID();

    /**
    * @brief WM_CREATE message handler.do the window creation work within.
    * @param uMsg [in]. specifies the message.
    *
    * @param wParam [in]. specifies additional message information. 
    * The contents of this parameter depend on the value of the uMsg parameter.
    *
    * @param lParam [in]. specifies additional message information. 
    * The contents of this parameter depend on the value of the uMsg parameter.
    *
    * @param bHandled [in]. used to judge whether this message is handled.
    * @return the standard return values E_OUTOFMEMORY, E_UNEXPECTED, E_FAIL, and S_OK.
    */
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
    * @brief WM_DESTROY message handler.do the destroy work within.
    * @param uMsg [in]. specifies the message.
    *
    * @param wParam [in]. specifies additional message information. 
    * The contents of this parameter depend on the value of the uMsg parameter.
    *
    * @param lParam [in]. specifies additional message information. 
    * The contents of this parameter depend on the value of the uMsg parameter.
    *
    * @param bHandled [in]. used to judge whether this message is handled.
    * @return the standard return values E_OUTOFMEMORY, E_UNEXPECTED, E_FAIL, and S_OK.
    */
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
    * @brief interface of IClassFactory.Creates an uninitialized object.
    *
    * @param pUnkOuter [in]. If the object is being created as part of an aggregate, 
    * specify a pointer to the controlling IUnknown interface of the aggregate. Otherwise, 
    * this parameter must be NULL. 
    *
    * @param riid [in]. A reference to the identifier of the interface to be used to communicate
    * with the newly created object. If pUnkOuter is NULL, this parameter is generally
    * the IID of the initializing interface; if pUnkOuter is non-NULL, riid must be IID_IUnknown.
    *
    * @param ppvObject [out]. The address of pointer variable that receives the interface pointer
    * requested in riid. Upon successful return, *ppvObject contains the requested interface pointer.
    * If the object does not support the interface specified in riid, the implementation must set *ppvObject to NULL.
    *
    * @return the standard return values E_INVALIDARG, E_OUTOFMEMORY, and E_UNEXPECTED, as well as the following values.
    */
    STDMETHOD(CreateInstance)( 
        /* [unique][in] */ IUnknown *pUnkOuter,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    /**
    * @brief Interface of IClassFactory. Locks an object application open in memory. 
    * This enables instances to be created more quickly.
    * @param fLock [in]. If TRUE, increments the lock count; if FALSE, decrements the lock count. 
    * @return the standard return values E_OUTOFMEMORY, E_UNEXPECTED, E_FAIL, and S_OK.
    */
    STDMETHOD(LockServer)( /* [in] */ BOOL fLock);

    /*STDMETHOD(GetWindow)(HWND* phwnd);*/

	STDMETHOD(GetControlHwnd)(HWND* phwnd);

    STDMETHOD(DetachWindow)();
    STDMETHOD(AttachWindow)(HWND hwnd);
    STDMETHOD(CreateViewWindow)();

	STDMETHOD(SetAssociateElement)(IUnknown *pUnkown);

	STDMETHOD(ReleaseAssociateElement)();
    STDMETHOD (GetD4)(IUnknown **pUnkown);
    STDMETHOD (GetE3)(IUnknown **pUnkown);

	/**
	* @brief interface of ITangWinControl, this method can set the height of control.
	* @param [in] lHeight. the height value of window control.
	* @return HRESULT return S_OK if successful, otherwise return error code.
	*/
	STDMETHOD(put_Height)(long lHeight);

	/**
	* @brief interface of ITangWinControl, this method can set the width of control.
	* @param [in] uWidth. the width value of window control.
	* @return HRESULT return S_OK if successful, otherwise return error code.
	*/
	STDMETHOD(put_Width)(long lWidth);

	/**
	* @brief function method of ComContorlBase.we overwrite here to active the window object 
	* and invoke CreateControlWindow function to creat self window.detail see:
	* http://msdn.microsoft.com/zh-cn/vstudio/93z5y5f9(VS.80).aspx
	* @param [in] iVerb. the verb.
	* @param [in] rect. Pointer to the position of the in-place control.
	* @return HRESULT return S_OK if successful, otherwise return error code.
	*/
	HRESULT InPlaceActivate(LONG iVerb, const RECT* /*prcPosRect*/);

	/**
	* @brief disable UI activate.
	* @param [in] dwAspect render aspect.
	* @param [out] pdwStatus flags value of the specified render aspect.
	* @return HRESULT return S_OK.
	*/
	STDMETHOD(GetMiscStatus)(DWORD dwAspect,DWORD *pdwStatus);

	/**
	* @brief draw UI.
	* @param [in] di draw info.
	* @return HRESULT return S_OK.
	*/
	HRESULT OnDraw(ATL_DRAWINFO& di);

	/**
	* @brief register a window class for the window creation.
	* @param [in] null.
	* @return HRESULT return a class atom that uniquely identifies the class being registered if successful, otherwise return 0.
	*/
	ATOM RegisterMyWinClass();

	/**
	* @brief custom message handler proc.replace default proc with it to handler windows message.
	* @param [in] hWnd. window hwnd which messgae will loop to.
	* @param [in] message. the message type.
	* @param [in] wParam. wParam.
	* @param [in] lParam. lParame.
	* @return HRESULT return S_OK if successful, otherwise return error code.
	*/
	static LRESULT CALLBACK MyWndProc(HWND, UINT, WPARAM, LPARAM);
	HRESULT GetIHTML_D4_E3();

private:
    CWindow m_winCtrl;
	UINT32 m_uHeight;
	UINT32 m_uWidth;
    ULONG m_cRef;

public:
	IUnknown * m_pUkn;
	IHTMLDocument4 *m_pHD4;
	IHTMLElement3 *m_pHE3;
    
	BOOL m_bIsTrue;

    static HWND m_hwndTopParent;
};
