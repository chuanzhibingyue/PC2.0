#pragma once
#include <atlbase.h>
#include <atlcom.h>
#include <OAIdl.h>
#include <string>
#include "GlobalDefine.h"
#include <comdef.h>
#include <comdefsp.h>

#define TYPE_LIBRARY_EXTENSION_STR  L".tlb"

/*
 * This template implements an IUnknown and an IDispatch interface. 
 */
template<class T> class NativeActionDispImpl :
	public T
{
protected:
	/*
	 * The internal COM reference counter, used by the IUnknown implementation.
	 */
	long			m_COMReferenceCounter;
	/*
	 * This object's TypeInfo, used by the IDispatch implementation.
	 */
	ITypeInfo*				m_TypeInfo;
	/*
	 *
	 */
	bool					m_CannotSelfDelete;
    /*
    *
    */
    bool					m_DonotLogParam;
    /*
    *
    */
    TCHAR                   m_fileName[MAX_PATH];

public:
	/*
	 * Constructor
	 */
	NativeActionDispImpl(BSTR strModuleName, BOOL bUseTypeLibFile = TRUE)
	{
		m_CannotSelfDelete = false;
        m_DonotLogParam = false;
		m_COMReferenceCounter = 1;
        m_TypeInfo = NULL;
        std::wstring wstrPathName;

        wstrPathName = strModuleName;
        memset(m_fileName,0, MAX_PATH);
        if (bUseTypeLibFile)
        {
            wstrPathName.append(TYPE_LIBRARY_EXTENSION_STR);
            wcscpy_s(m_fileName,wstrPathName.length()*sizeof(TCHAR),wstrPathName.c_str());
        }
        else
        {
            size_t iLocation = wstrPathName.rfind('\\');
            if (iLocation != -1)
            {
                wstrPathName = wstrPathName.substr(iLocation + 1,wstrPathName.length());
            }

            HMODULE hMudle = ::GetModuleHandle(wstrPathName.c_str());
            if (!(hMudle) || INVALID_HANDLE_VALUE == hMudle)
            {
                
            }
            else
            {
                ::GetModuleFileName(hMudle, m_fileName, MAX_PATH);
            }
        }
		
        HRESULT hr = Init();
        if (FAILED(hr))
        {
            //G_ERR(L"NativeActionDispImpl: Init with error: 0x%08X",hr);
        }
	}

    NativeActionDispImpl()
    {
        m_CannotSelfDelete = false;
        m_COMReferenceCounter = 1;
        m_TypeInfo = NULL;

        ::GetModuleFileName(NULL, m_fileName, MAX_PATH);
        HRESULT hr = Init();
        if (FAILED(hr))
        {
           // LOG_ERR(L"NativeActionDispImpl: Init with error: 0x%08X",hr);
        }
    }

	/*
	 * Destructor
	 */
	virtual ~NativeActionDispImpl()
	{
        if (m_TypeInfo != NULL)
        {
            m_TypeInfo->Release();
        }
	}

    /*
    *   initialize dispatcher.
    */
    STDMETHODIMP Init()
    {
        ITypeLib *typeLib;
        HRESULT hr = ::LoadTypeLibEx(m_fileName, REGKIND_NONE, &typeLib);
        if (SUCCEEDED(hr))
        {
            if (typeLib)
            {
                hr = typeLib->GetTypeInfoOfGuid(__uuidof(T),&m_TypeInfo);
                if (FAILED(hr))
                {
                   
                }
                typeLib->Release();
            }
        }
        else
        {
           
        }

        return hr;
    }

	/*
	 * IUnknown::QueryInterface() implementation
	 */
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
	{
		if(ppvObject == NULL)
		{
			return E_POINTER;
		}
		else if(riid == IID_IUnknown || 
			riid == IID_IDispatch || 
			riid == __uuidof(T))
		{
			*ppvObject = (void*) this;
			AddRef();
			return S_OK;
		}
		else
		{
			return E_NOINTERFACE;
		}
	}

	/*
	 * IUnknown::AddRef() implementation
	 */
	STDMETHODIMP_(ULONG) AddRef()
	{
		return ++m_COMReferenceCounter;
	}

	/*
	 * IUnknown::Release() implementation
	 */
	STDMETHODIMP_(ULONG) Release()
	{
		if(m_COMReferenceCounter> 1)
		{
			return --m_COMReferenceCounter;
		}
		else
		{
			if(!m_CannotSelfDelete)
			{
				OnFinalRelease();
                delete this;
			}
			return 0;
		}
	}

	/*
	 * IDispatch::GetTypeInfoCount() implementation
	 */
	STDMETHODIMP GetTypeInfoCount(UINT* pctinfo)
	{
		if(pctinfo == NULL)
		{
			return E_POINTER;
		}
		*pctinfo = 1;
		return S_OK;
	}

	/*
	 * IDispatch::GetTypeInfo() implementation
	 */
	STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
	{
		if(ppTInfo == NULL)
		{
			return E_POINTER;
		}

        if (m_TypeInfo == NULL)
        {
            return E_POINTER;
        }
		
        m_TypeInfo->AddRef();
        *ppTInfo = m_TypeInfo;
		return S_OK;
	}

	/*
	 * IDispatch::GetIDsOfNames() implementation
	 */
	STDMETHODIMP GetIDsOfNames(REFIID riid,
		LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
	{
        if (rgszNames)
        {
            //LOG_INFO(L"Dispatch invoke native function:%s.",*rgszNames);
        }

		return DispGetIDsOfNames(m_TypeInfo, rgszNames, cNames, rgDispId);
	}

	/*
	 * IDispatch::Invoke() implementation
	 */
	STDMETHODIMP Invoke(
		DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, 
		DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, 
		UINT* puArgErr)
	{
        //Log parameters for all APIs.
        if (!m_DonotLogParam)
        {
            LogInvokeParameters(pDispParams);
        }

		HRESULT hr = DispInvoke(this, m_TypeInfo, dispidMember, wFlags,
			pDispParams, pVarResult, pExcepInfo, puArgErr);
 		if ( NULL != pVarResult && pVarResult->vt == VT_EMPTY )
 		{
 			pVarResult->vt =  VT_INT;
			pVarResult->intVal = HResult_To_App_Layer(hr);
 		}
 		return hr;
//  		if (pVarResult != NULL && pVarResult->vt == NULL)
//  			pVarResult->vt =  VT_INT;
//  		else
//  		{
//  			if (FAILED(hr))
//  			{
//                  //HRESULT_FROM_WIN32 and log the error info.
//  				DWORD error = HRESULT_FROM_WIN32(::GetLastError());
//                  LOG_ERR(
//                      L"NativeActionDispImpl: Dispatch invoke with error:0x%08X get last error:0x%08X",
//                      hr,
//                      error);
//  
//                  VARIANTARG* pVarResult = (VARIANTARG*)_alloca(sizeof(VARIANTARG));
//                  if (pVarResult != NULL)
//                  {
//                      pVarResult->vt = VT_INT;
//                      pVarResult->ulVal = hr;
//                  }
//  			}
//  		}
// 
// 		return S_OK;
	}

    /*
    *
    */
    void LogInvokeParameters(DISPPARAMS* pDispParams)
    {
        for (int i=(pDispParams->cArgs-1); i>=0; i--)
        {
            int uParamNum = pDispParams->cArgs-i;
            switch (pDispParams->rgvarg[i].vt)
            {
            case VT_BOOL:
               // LOG_DEBUG(L"Param %d VT:VT_BOOL Value:%d",uParamNum,pDispParams->rgvarg[i].boolVal);
                break;
            case VT_INT:
            case VT_I4:
               // LOG_DEBUG(L"Param %d VT:VT_INT Value:%d",uParamNum,pDispParams->rgvarg[i].lVal);
                break;
            case VT_UI4:
            case VT_UINT:
               // LOG_DEBUG(L"Param %d VT:VT_UINT Value:%u",uParamNum,pDispParams->rgvarg[i].uintVal);
                break;
            case VT_UI8:
              //  LOG_DEBUG(L"Param %d VT:VT_UI8 Value:%u",uParamNum,pDispParams->rgvarg[i].ullVal);
                break;
            case VT_R4:
            case VT_I8:
            case VT_R8:
            case VT_CY:
            case VT_DATE:
              //  LOG_DEBUG(L"Param %d VT:VT_I8 Value:%d",uParamNum,pDispParams->rgvarg[i].llVal);
                break;
            case VT_UNKNOWN:
               // LOG_DEBUG(L"Param %d VT:VT_UNKNOWN Value:%d",uParamNum,pDispParams->rgvarg[i].punkVal);
                break;
            case VT_DISPATCH:
              //  LOG_DEBUG(L"Param %d VT:VT_DISPATCH Value:%d",uParamNum,pDispParams->rgvarg[i].pdispVal);
                break;
            case VT_BSTR:
              //  LOG_DEBUG(L"Param %d VT:VT_BSTR Value:%s",uParamNum,pDispParams->rgvarg[i].bstrVal);
                break;
            case VT_EMPTY:
               // LOG_DEBUG(L"Param %d VT:VT_EMPTY",uParamNum);
                break;
            default:
                break;
            }
        }
    }

    /*
    * Provide API to disable log parameters for invoked API.
    */
    //************************************
    // Method:    DonotLogParameters
    // FullName:  NativeActionDispImpl<T>::DonotLogParameters
    // Access:    public 
    // Returns:   void
    // Qualifier:
    //************************************
    void DonotLogParameters()
    {
        m_DonotLogParam = true;
    }

	/*
	 *
	 */
	void DisableSelfDelete()
	{
		m_CannotSelfDelete = true;
	}

	virtual void OnFinalRelease()
	{
	}

};
