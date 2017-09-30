/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file ExternalDispatch.h
* @brief defined ExternalDispatch template class to support automation attribute.
* External will be inherited from it.
*
* @author Hongwei Hu (hongwei.hu@gnetis.com)
*
* @date Janu. 26 2010
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

#include <map>
#include <atlbase.h>
#include <atlcom.h>
#include "GlobalDefine.h"
#include <comdef.h>
#include <comdefsp.h>

// Parameter types: by value VTs
#define VTS_EMPTY           L"\x00"
#define VTS_REF_DISPATCH    L"\x4009"

/////////////////////////////////Define property map///////////////////////////////////

/**
* @struct DispatchEntry
* @brief adapter property dispatch entry.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
template <class T>
struct DispatchEntry
{
	BSTR bstrName;            /**< adapter name*/
	DISPID dispid;              /**< adapter dispatch id*/
	UINT wFlags;                /**< flags*/               
	VARTYPE vtReturn;           /**< return value*/
	UINT nArgs;                 /**< argument number*/
	BSTR vtbstrArgs;                /**< argument, can be extended as VARIANT type argument to support other type argument*/
	VARTYPE vtSingle;           /**< argument type*/
	void (__stdcall T::*pfn)(); /**< get_adaptor function address*/

	DispatchEntry()
	{
		bstrName = NULL;
		dispid = 0;
		vtReturn = VT_EMPTY;
		nArgs = 0;
		vtbstrArgs = NULL;
		vtSingle = VT_EMPTY;
		pfn = NULL;
	}

	~DispatchEntry()
	{
		if ( NULL != bstrName )
		{
			::SysFreeString(bstrName);
			bstrName = NULL;
		}
		if ( NULL != vtbstrArgs )
		{
			::SysFreeString(vtbstrArgs);
			vtbstrArgs = NULL;
		}
		
	}
};

/**
* @class ExternalDispatch
* @brief ExternalDispatch template class to make the T class can invoked by UI JS code.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
template <class T>
class ExternalDispatch: public IDispatch
{
public:
	typedef std::map<_bstr_t, DispatchEntry<T>*> DISPATCH_MAP;
	typedef std::pair<_bstr_t, DispatchEntry<T>*> DISPATCH_PAIR;

	DISPATCH_MAP m_dispatchEntryMap;
    //CRITICAL_SECTION   m_dispatchMapLock;

public:
	ExternalDispatch()
	{
        //create resource lock.
       // ::InitializeCriticalSection(&m_dispatchMapLock);

        m_dispatchEntryMap.clear();
	}

    ~ExternalDispatch()
    { 
        //recycle the memory.
       // ::EnterCriticalSection(&m_dispatchMapLock);
        DISPATCH_MAP::iterator iter;
        for (iter=m_dispatchEntryMap.begin(); iter!=m_dispatchEntryMap.end(); iter++)
        {
            //recycle the dispatch entry. 
            delete (*iter).second;
            (*iter).second = NULL;
        }

        m_dispatchEntryMap.clear();
        //::LeaveCriticalSection(&m_dispatchMapLock);

        //create resource lock.
        //::DeleteCriticalSection(&m_dispatchMapLock);
    }

	DISPATCH_MAP* GetDispMap()
	{
		return &m_dispatchEntryMap;
	}

	HRESULT WINAPI QueryInterface( REFIID riid,void ** object) 
	{ 
		*object=IsEqualIID(riid, IID_IDispatch)?this:0;
		return *object?S_OK:E_NOINTERFACE;
	}         
	ULONG WINAPI AddRef () { return 0; }
	ULONG WINAPI Release() { return 0; }
	HRESULT WINAPI GetTypeInfoCount( UINT *) { return 0; }
	HRESULT WINAPI GetTypeInfo( UINT, LCID, ITypeInfo **)	{ return 0; }

	/**
	GetIDsOfNames查询指定的函数名的调用ID，就是说如果有一个方法是“HelloWorld”，
	那么它会先调用GetIDsOfNames方法来查询这个方法是否支持，
	如果支持则给出该方法的调用ID（通过修改rgDispId[out]参数），如果不支持则返回E_NOTIMPL
	*/
	HRESULT  WINAPI GetIDsOfNames( REFIID riid, WCHAR** name, UINT cnt ,LCID lcid, DISPID *id) 
	{
		HRESULT ret = 0;
        
        //::EnterCriticalSection(&m_dispatchMapLock);
		for( UINT i=0; i<cnt; i++ ) 
		{
            //seek the dispid of the specific adapter object.
            _bstr_t bstrAdapterName = *name;
			DISPATCH_MAP::iterator citer;
			citer = m_dispatchEntryMap.find(bstrAdapterName);
			if (citer == m_dispatchEntryMap.end())
			{
				//log the error.
				//LOG_ERR(L"ExternalDispatch GetIDsOfNames:%s failed. Can't find the dispatch entry.",bstrAdapterName.GetBSTR());
				ret = E_NOTIMPL;
				continue;
			}

			if ((*citer).second != NULL)
			{
				DispatchEntry<T>* pEntry = (*citer).second;
				DISPID dispid = 1;
				id[i] = pEntry->dispid==DISPID_UNKNOWN ? dispid : pEntry->dispid;
				if( pEntry->pfn ==NULL )
				{
					//LOG_ERR(L"Adaptor access function is null");
					continue;
				}

				//find out the adapter dispatch id.
				ret = S_OK;
				//LOG_INFO(L"ExternalDispatch GetIDsOfNames:%s successfully.",bstrAdapterName.GetBSTR());
				break;
			}
			else
			{
				ret = E_NOTIMPL;
				//LOG_ERR(L"ExternalDispatch GetIDsOfNames:%s failed. adaptor dispatch entry is null.",bstrAdapterName.GetBSTR());
				continue;
			}
		}
        //::LeaveCriticalSection(&m_dispatchMapLock);
		return ret;
	}

	HRESULT WINAPI Invoke( DISPID id, REFIID riid, LCID lcid, WORD flags, DISPPARAMS *arg, VARIANT *ret, EXCEPINFO *excp, UINT *err) 
	{
		HRESULT hr = E_FAIL;
		if(id) 
		{
            DISPID i = 1;
            DISPATCH_MAP::iterator iter;
            
            //::EnterCriticalSection(&m_dispatchMapLock);
			DISPATCH_MAP* pMap = GetDispMap();
			for(iter = pMap->begin(); iter != pMap->end(); iter++)
			{
                if (iter->second != NULL)
                {
                    DISPID dispid = iter->second->dispid==DISPID_UNKNOWN ? i : iter->second->dispid;
                    if( id==dispid )
                    {
                        //TODO: 不支持属性Put，此处处理UI对Adapter设值的情况。
                        /*if( (DISPATCH_PROPERTYPUT==flags) && (DISPID_PROPERTYPUT==(pMap+1)->dispid) ) 
                        pMap++;*/

                        //copy the type of args.
                        VARTYPE* pArgsType = &iter->second->vtSingle;
                        if( pArgsType == NULL )
                            pArgsType = (VARTYPE*)VT_EMPTY;

                        //copy the args number.
                        UINT nArgs = iter->second->nArgs;

						//_alloca函数分配函数栈的内存，调用函数返回时，自动释放。
                        VARIANTARG** ppVarArgs = (VARIANTARG**)_alloca(sizeof(VARIANTARG*)*1);
                        VARIANTARG* pVarArgs = (VARIANTARG*)_alloca(sizeof(VARIANTARG)*1);
                        ppVarArgs[0] = &pVarArgs[0];

						//set the variant vt to VT_EMPTY.
                        ::VariantInit(&pVarArgs[0]);

						//copy the arguments.
						pVarArgs[0].vt = *pArgsType;
                        pVarArgs[0].bstrVal = iter->second->vtbstrArgs;
#ifdef _DEBUG
                        //LOG_INFO(L"Auto inovke adaptor:%s ......",pVarArgs[0].bstrVal);
#endif

                        T *pT = static_cast<T*>(this);
                        CComStdCallThunk<T> thunk;
                        thunk.Init(iter->second->pfn, pT);
                        CComVariant tmpResult;
                        if( ret==NULL )
                            ret = &tmpResult;

                        //call the get_property function to get the adapter.
                        hr = ::DispCallFunc(
                            &thunk,
                            0,
                            CC_STDCALL,
                            iter->second->vtReturn,
                            nArgs,
                            pArgsType,
                            nArgs ? ppVarArgs : NULL,
                            ret);

						//clears the contents of a variant and sets the variant to VT_EMPTY.
						//::VariantClear(&pVarArgs[i]); // Fix the corrupt for the clear the variant from the UI;
                        if(ret) 
                        { 
                            ret->vt = VT_DISPATCH;
                        } 
                        break;
                    }
                }
				i++;
			}//end for
			//::LeaveCriticalSection(&m_dispatchMapLock);
		}

		//return value to script DWORD
		return hr; 
	}
};