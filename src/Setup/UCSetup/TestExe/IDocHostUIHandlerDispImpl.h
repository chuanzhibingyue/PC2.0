/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file IDocHostUIHandlerDispImpl.h
* @brief template class for IDocHostUIHandlerDispatch implementation 
* 
* @author Hongwei Hu (hongwei.hu@quanshi.com)
*
* @date April 25 2011
*
* @version 1.0.0
* 
* @warning
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug

******************************************************************************/
#pragma once

/**< include header files.*/
#include <strsafe.h>
#include <comdef.h>
#include <comdefsp.h>

/**
* @class IDocHostUIHandlerDispImpl
* @brief template class of IDocHostUIHandlerDispatch interface implementation
* 
* @author Hongwei Hu
* @version 1.0.0
*/
template<class T> 
class IDocHostUIHandlerDispImpl :
	public IDispatchImpl<IDocHostUIHandlerDispatch>
{
protected:

public:
    IDocHostUIHandlerDispImpl()
    {
    }
    virtual ~IDocHostUIHandlerDispImpl()
    {
    }

    ////////Interface IDocHostUIHandlerDispatch begin/////////////////
    virtual HRESULT STDMETHODCALLTYPE HideUI( void )
    {
        //TODO:
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( /* [in] */ DWORD dwID, 
		/* [in] */ DWORD x, /* [in] */ DWORD y, /* [in] */ IUnknown *pcmdtReserved, 
		/* [in] */ IDispatch *pdispReserved, /* [retval][out] */ HRESULT *dwRetVal )
    {
        //TODO: custom context menu according difference menu type. 
        switch(dwID)
        {
        case CONTEXT_MENU_CONTROL:
            *dwRetVal = S_FALSE;        //use default context menu in control.
            break;
        case CONTEXT_MENU_IMAGE:
        case CONTEXT_MENU_TABLE:
        case CONTEXT_MENU_TEXTSELECT:
        case CONTEXT_MENU_DEFAULT:
        default:
            *dwRetVal    = S_OK;        //disable mshtml default context menu.
            break;
        }

        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE GetHostInfo( /* [out][in] */ DWORD *pdwFlags, /* [out][in] */ DWORD *pdwDoubleClick )
    {
        *pdwFlags = DOCHOSTUIFLAG_NO3DBORDER;
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE ShowUI( /* [in] */ DWORD dwID, /* [in] */ IUnknown *pActiveObject, /* [in] */ IUnknown *pCommandTarget, /* [in] */ IUnknown *pFrame, /* [in] */ IUnknown *pDoc, /* [retval][out] */ HRESULT *dwRetVal )
    {
        //TODO:
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE UpdateUI( void )
    {
        //TODO:
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE EnableModeless( /* [in] */ VARIANT_BOOL fEnable )
    {
        //TODO:
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( /* [in] */ VARIANT_BOOL fActivate )
    {
        //TODO:
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( /* [in] */ VARIANT_BOOL fActivate )
    {
        //TODO:
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE ResizeBorder( /* [in] */ long left, /* [in] */ long top, /* [in] */ long right, /* [in] */ long bottom, /* [in] */ IUnknown *pUIWindow, /* [in] */ VARIANT_BOOL fFrameWindow )
    {
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( /* [in] */ DWORD_PTR hWnd, /* [in] */ DWORD nMessage, /* [in] */ DWORD_PTR wParam, /* [in] */ DWORD_PTR lParam, /* [in] */ BSTR bstrGuidCmdGroup, /* [in] */ DWORD nCmdID, /* [retval][out] */ HRESULT *dwRetVal )
    {
        //TODO:
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath( /* [out] */ BSTR *pbstrKey, /* [in] */ DWORD dw )
    {
        HRESULT hr = S_OK;
		//// ToDo for test
  //       WCHAR* szKey = REG_TANG_OPTION_KEY_PATH;
 
  //       //cbLength is the length of szKey in bytes.
  //       size_t cbLength;
  //       hr = StringCbLengthW(szKey, 1280, &cbLength);
  //       //TODO:add error handling code here.
 
  //       if (pbstrKey)
  //       {
  //           *pbstrKey = (LPOLESTR)CoTaskMemAlloc(cbLength + sizeof(WCHAR));
  //           if (*pbstrKey)
  //           {
  //               hr = StringCbCopyW(*pbstrKey, cbLength + sizeof(WCHAR), szKey);
  //           }
  //       }
  //       else
  //       {
  //           hr = E_INVALIDARG;
  //       }

        return hr;
    }

    virtual HRESULT STDMETHODCALLTYPE GetDropTarget( /* [in] */ IUnknown *pDropTarget, /* [out] */ IUnknown **ppDropTarget )
    {
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE GetExternal( /* [out] */ IDispatch **ppDispatch )
    {
        *ppDispatch = ((T*)this)->m_pExternal;
        return S_OK;
    }

    //Called by MSHTML to give the host an opportunity to modify the URL to be loaded.
    /*
    Parameters
    dwTranslate
    [in] Reserved. Must be set to NULL.
    pchURLIn
    [in] Pointer to an OLECHAR that specifies the current URL for navigation.
    ppchURLOut
    [out] Address of a pointer variable that receives an OLECHAR pointer containing the new URL.
    Return Value
    Returns S_OK if the URL was translated, or S_FALSE if the URL was not translated.
    */
    virtual HRESULT STDMETHODCALLTYPE TranslateUrl( /* [in] */ DWORD dwTranslate, /* [in] */ BSTR bstrURLIn, /* [out] */ BSTR *pbstrURLOut )
    {
        //TODO:
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE FilterDataObject( /* [in] */ IUnknown *pDO, /* [out] */ IUnknown **ppDORet )
    {
        return S_OK;
    }
};