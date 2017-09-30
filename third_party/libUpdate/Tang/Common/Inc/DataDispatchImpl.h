/******************************************************************************
* Copyright (c) 2010~2011 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file datadispatchimpl.h
* @brief in this header file, we have dfined all dispatch object implementation 
* template classes and some dispatch object containers.Thease will be used
* to provide services data to java script in all modules.
* 
* @author Hongwei Hu (hongwei.hu@quanshi.com)
*
* @date July. 26 2010
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
#include <atlbase.h>
#include <atlcom.h>
#include <OAIdl.h>
#include <string>
#include <Debug.h>
#include <map>
#include <vector>
#include <exdisp.h>
#include <mshtml.h>
#include <Lock.h>
#include "GlobalDefine.h"

/*! \addtogroup BaseDataStructure
* The module of some base data structure which will be inherited by the services' data;
*  @{
*/


/**< type define for dispatch data module.*/
typedef std::wstring KEY;
typedef VARIANT VALUE;
typedef BSTR JSEVENTHANDLER;

/**< define the base value of custom dispatch id.*/
#define DISPID_PROPERTY_BASE        (0x3000)
#define DISPID_EVENT_BASE           (0x4000)
#define DISPID_METOHD_BASE          (0x5000)

/**< definition of data object(include tang array object) common property.*/
#define DISPID_PROPERTY_PARENT      (0x63)
#define DISP_PROPERTY_KEY_PARENT    L"parent"

#define DISPID_DATA_EVENT_PROPERTYCHANGE    (0x67)
#define DISP_DATA_EVENT_KEY_PROPERTYCHANGE  L"onPropertyChanged"

/**< definition dispatch id of tang array object.*/
#define DISPID_ARRAY_EVENT_ADD      (0x64)
#define DISPID_ARRAY_EVENT_REMOVE   (0x65)
#define DISPID_ARRAY_EVENT_UPDATE   (0x66)
#define DISPID_ARRAY_METHOD_AT      (0x01)
#define DISPID_ARRAY_METHOD_SORT    (0x02)
#define DISPID_ARRAY_METHOD_FIND    (0x03)
#define DISPID_ARRAY_METHOD_COUNT   (0x04)
#define DISPID_ARRAY_PROPERTY_SIZE  (0x30)

/**< definition dispatch key of tang array object.*/
/**< The event means an element is added to the tang array*/
#define DISP_ARRAY_EVENT_KEY_ADD         L"onAdd"

/**< The event means an element is removed from the tang array*/
#define DISP_ARRAY_EVENT_KEY_REMOVE      L"onRemove"

/**< The event means an element is updated in the tang array*/
#define DISP_ARRAY_EVENT_KEY_UPDATE      L"onUpdate"

/**< The method of the tang array object*/
#define DISP_ARRAY_METHOD_KEY_AT         L"at"
#define DISP_ARRAY_METHOD_KEY_SORT       L"sort"
#define DISP_ARRAY_METHOD_KEY_FIND       L"find"
#define DISP_ARRAY_METHOD_KEY_COUNT      L"findCount"
#define DISP_ARRAY_PROPERTY_KEY_SIZE     L"size"

#define TYPE_LIBRARY_EXTENSION_STR       L".tlb"

/**< define the property of the conference or service is ready for UI to initialize, 0 not ready, 1 ready, 2 error*/
#define DISP_IS_READY           L"serviceStatus"
#define PROP_IS_READY_FALSE 0
#define PROP_IS_READY_OK 1
#define PROP_IS_READY_ERROR 2

/**< auto property/method/event micro for dispatch data object.*/
#define BEGIN_ACTION_MAP() \
public: \
	void __GetPropertyMap() {\
	void* fn_add = NULL; \
	DISPID dispid = 0;\
	VARIANT var;\
	VariantInit(&var);
#define ADD_PROPERTY_DISPID_NAME(id,name) \
	m_actionIDsMap[(name)] = id; \
	m_actionNamesMap[id] = name; 
#define ADD_PROPERTY_DISPID_NAME_EX(id,name,type) \
	m_actionIDsMap[(name)] = id; \
	m_actionNamesMap[id] = name;	\
	var.vt = type;					\
	if (VT_I4 == type) var.lVal = 0;	\
	if (VT_UINT == type) var.uintVal = 0;	\
	if (VT_BOOL == type) var.boolVal = FALSE;	\
	if (VT_BSTR == type) var.bstrVal = NULL;	\
	if(VT_DISPATCH == type) var.pdispVal = NULL;\
	SetPropertyValue(name, var);	
#define ADD_PROPERTY_DISPID_EVENT(id,name) \
{\
	wchar_t buffer[256] = {0}; \
	wsprintf(buffer,L"on%sChanged",(name)); \
	m_actionIDsMap[buffer] = id; \
	m_actionNamesMap[id] = buffer; \
}
#define ADD_ACTION_METHOD(name) \
	_asm { lea eax, name };  \
	_asm { mov fn_add,eax }; \
	dispid = (DISPID)fn_add; \
	m_actionIDsMap[(L#name)] = dispid; \
	m_actionNamesMap[dispid] = (L#name);
#define END_ACTION_MAP() \
}\

/**
* @class TangDataEvent
* @brief TangDataEvent class define dispatch data event notify to Java script through
* HTML window instance.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
class TangDataEvent
{
public:
	/*
	* Constructor
	*/
	TangDataEvent()
		:m_pIEInstance( NULL )
	{
	}

	/*
	* Destructor
	*/
	virtual ~TangDataEvent()
	{
		//ref count--
		if (m_pIEInstance)
		{
			m_pIEInstance->Release();
			m_pIEInstance = NULL;
		}
	}

public:
	/*
	* Using IE instance to initialize
	*/
	HRESULT Init(IDispatch* pIEInstance)
	{
		HRESULT hr = S_OK;
		ARG_CHECK( pIEInstance );
		if ( NULL == m_pIEInstance )
		{
			//init tang data event with IE instance.
			//ref count++
			hr = pIEInstance->QueryInterface(IID_IWebBrowser,(void **)&m_pIEInstance);
			HR_CHECK( hr );
			CHECK_IF_NULL( m_pIEInstance );
		}
exit:
		if (FAILED(hr))
		{
			LOG_ERR(L"TangDataEvent::Init exit with error 0x%08X", hr);
		}
		return hr;
	}

	/*
	* Notify dispatch data event to Java script.
	*/
	HRESULT Fire(BSTR handler, VARIANT* parameters,DWORD nParams,VARIANT* varRet)
	{
		HRESULT hr = S_OK;
		IHTMLDocument2* pHTDoc = NULL;
		IHTMLWindow2* pHTWnd = NULL;
		CHECK_IF_NULL( m_pIEInstance );
		hr = m_pIEInstance->get_Document((IDispatch**) &pHTDoc);
		HR_CHECK( hr );
		CHECK_IF_NULL( pHTDoc );  
		hr = pHTDoc->get_parentWindow(&pHTWnd);	
		HR_CHECK( hr );
		CHECK_IF_NULL( pHTWnd );  
		{
			UINT errArg = 0;
			DISPID dispid = 0;
			hr = pHTWnd->GetIDsOfNames(IID_NULL, &handler, 1,
				LOCALE_SYSTEM_DEFAULT, &dispid);
			HR_CHECK( hr );
			{
				DISPPARAMS dParams = { parameters, 0, nParams, 0 };
				hr = pHTWnd->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
					DISPATCH_METHOD, &dParams, varRet, 0, &errArg);
				HR_CHECK( hr );
			}
		}
exit:
		if ( NULL != pHTWnd )
		{
			pHTWnd->Release();
			pHTWnd = NULL;
		}
		if ( NULL != pHTDoc )
		{
			pHTDoc->Release();
			pHTDoc = NULL;
		}
		if (FAILED(hr))
		{
			LOG_ERR(L"TangDataEvent::Fire exit with error 0x%08X", hr);
		}
		return hr;
	}


	/*
	* Notify dispatch data event to Java script.
	*/
	HRESULT Fire(IDispatch * handler, VARIANT* parameters,DWORD nParams,VARIANT* varRet)
	{
		UINT errArg = 0;
		//no need to specific method name. dispid=0 for anonymous JavaScript function.
		DISPID dispid = 0;
		HRESULT hr = S_OK;
		VariantInit( varRet );
		DISPPARAMS dParams = { parameters, 0, nParams, 0 };
		hr = handler->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
			DISPATCH_METHOD, &dParams, varRet, 0, &errArg);
		if (FAILED(hr))
		{
			LOG_ERR(L"Invoke anonymous JS function with error:0x%008X",hr);
		}

		return hr;
	}

private:
	IWebBrowser2* m_pIEInstance;
};

/**
* @class DataDispatchImpl
* @brief DataDispatchImpl is a template class to dispatch the data to Java script.
* This template implements an IUnknown and an IDispatch interface.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
template<class T> class DataDispatchImpl :public IDispatch
{
protected:
	/*
	* The internal COM reference counter, used by the IUnknown implementation.
	*/
	long			        m_COMReferenceCounter;
	/*
	*
	*/
	bool					m_CannotSelfDelete;
	/*
	*
	*/
	bool					m_CannotPropertyPut;

public:
	/*
	*
	*/
	std::map<KEY, VALUE>    m_properties;
	/*
	* event include on{Key}Changed
	*/
	std::map<KEY, JSEVENTHANDLER>   m_events;
	/*
	* event include event object
	*/
	std::map<KEY, IDispatch*>   m_eventObjects;
	/*
	*
	*/
	std::map<DISPID, KEY>           m_actionNamesMap;
	/*
	*
	*/
	std::map<KEY, DISPID>           m_actionIDsMap;
	/*
	*
	*/
	TangDataEvent                   m_dataEvent;
	/*
	*
	*/
	IDispatch*                      m_parentObj;

public:
	/*
	* Constructor
	*/
	DataDispatchImpl()
		:m_CannotSelfDelete( false ),
		m_CannotPropertyPut( false ),
		m_COMReferenceCounter( 1 ),
		m_parentObj( NULL )
	{
		m_events.clear();
		m_eventObjects.clear();

		//build dispatch property map.
		m_actionNamesMap[DISPID_PROPERTY_PARENT] = std::wstring(DISP_PROPERTY_KEY_PARENT);
		m_actionIDsMap[std::wstring(DISP_PROPERTY_KEY_PARENT)] = DISPID_PROPERTY_PARENT;

		//add common onPropertyChanged event into event map.
		m_actionNamesMap[DISPID_DATA_EVENT_PROPERTYCHANGE] = std::wstring(DISP_DATA_EVENT_KEY_PROPERTYCHANGE);
		m_actionIDsMap[std::wstring(DISP_DATA_EVENT_KEY_PROPERTYCHANGE)] = DISPID_DATA_EVENT_PROPERTYCHANGE;

		//add custom property/method/event into map.
		((T*)this)->__GetPropertyMap();
	}

	/*
	* Destructor
	*/
	virtual ~DataDispatchImpl()
	{
		//recycle event handler bstr resource.
		std::map<KEY, JSEVENTHANDLER>::iterator iter;
		iter = m_events.begin();
		while (iter != m_events.end())
		{
			SYSFREESTRING(iter->second);
			iter = m_events.erase(iter);
		}

		//release event objects.
		std::map<KEY, IDispatch*>::iterator it;
		it = m_eventObjects.begin();
		while(it != m_eventObjects.end())
		{
			it->second->Release();
			it = m_eventObjects.erase(it);
		}

		//TODO: SYSFREEBSTR std::map<KEY, VALUE>    m_properties;
		std::map<KEY,VALUE>::iterator itp = m_properties.begin();
		while( itp != m_properties.end() )
		{
			VariantClear( &(itp->second) );
			itp = m_properties.erase(itp);
		}
		OutputDebugString(L"DataDispatchImpl destruction.");
		LOG_INFO(L"DataDispatchImpl destruction.");
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
			riid == IID_IDispatch)
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

		return S_OK;
	}

	/*
	* IDispatch::GetIDsOfNames() implementation
	*/
	STDMETHODIMP GetIDsOfNames(REFIID riid,
		LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
	{
		DWORD j;
		std::map<std::wstring, DISPID>::iterator it;
		for(j=0; j<cNames;j++) 
		{
			it = m_actionIDsMap.find(rgszNames[j]);
			if (it == m_actionIDsMap.end())
				return ERROR_INVALID_ACCESS;
			rgDispId[j]=it->second;
		}
		return S_OK;
	}

	/*
	* IDispatch::Invoke() implementation
	*/
	STDMETHODIMP Invoke(
		DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, 
		DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, 
		UINT* puArgErr)
	{
		HRESULT hr = S_OK;

		std::map<DISPID, KEY>::iterator it = m_actionNamesMap.find(dispidMember);
		if (it == m_actionNamesMap.end()) return ERROR_ACCESS_DENIED;
		const KEY & strFN = it->second;
		switch( wFlags ) 
		{
		case DISPATCH_PROPERTYGET:
			if ( NULL != pVarResult )
			{
				pVarResult->vt = VT_DISPATCH;
				pVarResult->pdispVal = NULL;
			}
			if ( dispidMember == DISPID_PROPERTY_PARENT ) //property get parent object.
			{
				if ( NULL != m_parentObj && NULL != pVarResult )
				{
					hr = m_parentObj->QueryInterface(IID_IDispatch,
						(void**)pVarResult->pdispVal);
					HR_CHECK(hr);
				}
			}
			else if ( !strFN.empty() )   //property get named variables
			{
				hr = GetPropertyValue(strFN,*pVarResult);
				HR_CHECK( hr );
			}
			break; 
		case DISPATCH_PROPERTYPUT:
			if( NULL != pDispParams )
			{
				//put event handler.
				if ( (!strFN.empty()) && dispidMember>=100
					&& pDispParams->rgvarg->vt == VT_BSTR)
				{
					//to avoid user set an event more than once.
					std::map<KEY, JSEVENTHANDLER>::iterator iter;
					iter = m_events.find(strFN);
					if ( iter != m_events.end() )
					{
						SYSFREESTRING(iter->second);
						iter->second = NULL;
					}

					//put the event handler.
					m_events[strFN] = ::SysAllocString(pDispParams->rgvarg->bstrVal);
				}
				else if ( !m_CannotPropertyPut )  // property put of named variables
				{
					hr = SetPropertyValue(strFN, (*pDispParams->rgvarg));
					goto exit;
				}
			}
			break;
		case (DISPATCH_PROPERTYPUT|DISPATCH_PROPERTYPUTREF): //property put dispatch reference.
			if( NULL != pDispParams )
			{
				//put event object.
				if ( (!strFN.empty()) && dispidMember>=100
					&&  pDispParams->rgvarg->vt == VT_DISPATCH)
				{
					//to avoid user set an event more than once.
					std::map<KEY, IDispatch*>::iterator iter;
					iter = m_eventObjects.find(strFN);
					if ( iter != m_eventObjects.end() )
					{
						//release the counter of event object.
						iter->second->Release();
						iter->second = NULL;
					}

					//put the event object.
					IDispatch* pEventDispatch  = NULL;
					hr = pDispParams->rgvarg->pdispVal->QueryInterface(IID_IDispatch,(void**)&pEventDispatch);
					HR_CHECK( hr );
					CHECK_IF_NULL( pEventDispatch );
					m_eventObjects[strFN] =pEventDispatch;
				}
			}
			break;
			
		case DISPATCH_METHOD:
		default:
			T* pT = static_cast<T*>(this);
			void* pFN = (void*)dispidMember;
			if ( NULL != pFN 
				&& NULL != pT
				&& NULL != pDispParams )
			{
				DWORD i=-1,result=0;
				DWORD param = 0;
				for(i=0; i<pDispParams->cArgs; i++)
				{
					switch (pDispParams->rgvarg[i].vt)
					{
					case VT_EMPTY:
					case VT_VOID:  /* not support */
					case VT_ILLEGAL: /* not support */
					case VT_USERDEFINED:
					case VT_RECORD:
					case VT_RESERVED:
					case VT_BSTR_BLOB:
					case VT_BLOB:
					case VT_BLOB_OBJECT:
					case VT_NULL:  continue; /* No parameter */
					case VT_I1:
					case VT_UI1:
						param = (DWORD)pDispParams->rgvarg[i].bVal;
						break;
					case VT_I2:
					case VT_UI2:
					case VT_DECIMAL:
					case VT_BOOL:
						param = (DWORD)pDispParams->rgvarg[i].uiVal;
						break;
					case VT_I4:
					case VT_UI4:
					case VT_HRESULT:
					case VT_INT:
					case VT_UINT:
					case VT_R4:
					case VT_ERROR: /* SCODE, long type */
						param = (DWORD)pDispParams->rgvarg[i].lVal;
						break;
					case VT_I8:
					case VT_R8:
					case VT_UI8:
					case VT_CY:
					case VT_DATE:
					case VT_FILETIME:
#ifndef IS_64
						param = (DWORD)pDispParams->rgvarg[i].cyVal.Hi;
						_asm push param;
#endif
						param = (DWORD)pDispParams->rgvarg[i].cyVal.Lo;
						break;
					case VT_UNKNOWN: /* IUnknow* */
					case VT_DISPATCH:/* IDispatch* */
					case VT_VARIANT: /* VARIANT* */
					case VT_PTR: /* void* */
					case VT_BSTR: /* OLE char* */
					case VT_LPSTR:
					case VT_LPWSTR:
					case VT_CARRAY:
					case VT_SAFEARRAY:
					case VT_INT_PTR:
					case VT_UINT_PTR:
					case VT_CF:
					case VT_CLSID:
					case VT_STREAM:
					case VT_STORAGE:
					case VT_STREAMED_OBJECT:
					case VT_STORED_OBJECT:
					case VT_VERSIONED_STREAM:
						param = (DWORD)pDispParams->rgvarg[i].byref;
						break;
					}
					_asm push param; 
				}

				_asm push this;
				_asm call pFN;
				_asm mov result, eax;
				if( pVarResult ) 
				{ 
					pVarResult->vt = VT_INT;
					pVarResult->intVal = result;
				}
			}
			break;
		}

exit:
		if (FAILED(hr))
		{
			LOG_ERR(L"DataDispatchImpl::Invoke exit with error 0x%08X", hr);
		}
		return hr;
	}

	/*
	* Method to disable self delete when ref equal zero.
	*/
	void DisableSelfDelete()
	{
		m_CannotSelfDelete = true;
	}

	/*
	* Method to disable property put in Java script.
	*/
	void DisablePropertyPut()
	{
		m_CannotPropertyPut = true;
	}

	/*
	* Method to do final release job before self delete.
	*/
	virtual void OnFinalRelease()
	{

	}

	/*
	* Method to set specific property value.
	*/
	virtual HRESULT SetPropertyValue(const KEY & vn, const VALUE & v)
	{
		HRESULT hr = S_OK;

		//clear old memory if m_properties[vn] is not empty.
		std::map<KEY, VALUE>::iterator it = m_properties.find( vn );
		if ( it != m_properties.end() )
		{
			VariantClear( &(it->second) );
		}
		VariantInit( &(m_properties[vn]) );
		VALUE * pTemp = (VALUE*)(&v);
		hr = VariantCopy( &(m_properties[vn]),pTemp );
		HR_CHECK(hr);
exit:
		if (FAILED(hr))
		{
			LOG_ERR(L"DataDispatchImpl::SetPropertyValue exit with error 0x%08X", hr);
		}
		return hr;
	}

	/*
	* Method to get specific property value.
	*/
	virtual HRESULT GetPropertyValue(const KEY & vn, VALUE& v)
	{
		HRESULT hr = S_OK;
		std::map<KEY, VALUE>::iterator it = m_properties.find( vn );
		if( it == m_properties.end()) return ERROR_ACCESS_DENIED;
		VariantInit( &v );
		hr = VariantCopy( &v,&(it->second) );
		HR_CHECK(hr);
exit:
		if (FAILED(hr))
		{
			LOG_ERR(L"DataDispatchImpl::SetPropertyValue exit with error 0x%08X", hr);
		}
		return hr;
	}

	/*
	* Initialize event notify object in this function.
	*/
	HRESULT virtual InitEventSink(IDispatch* pIEInstance)
	{
		return m_dataEvent.Init(pIEInstance);
	}

	/*
	* Update property value and fire relational event.
	*/
	HRESULT UpdatePropertyValue(const KEY & vn, const VALUE & v)
	{
		HRESULT hr = S_OK;
		wchar_t onEventName[256] = {0};
		VARIANT varOld;
		std::map<KEY, JSEVENTHANDLER>::iterator iter;
		VALUE * pTempV = (VALUE*)(&v);
		VariantInit(  &varOld );

		std::map<KEY, VALUE>::iterator it = m_properties.find( vn );
		if ( it != m_properties.end() )
		{
			//get the old value as event argument.
			hr = VariantCopy( &varOld,&(it->second) );
			HR_CHECK(hr);
			hr = VariantClear( &(it->second) );
			HR_CHECK(hr);
		}

		//update property to new value.
		VariantInit( &(m_properties[vn]) );
		hr = VariantCopy( &(m_properties[vn]),pTempV) ;
		HR_CHECK(hr);

		//if single property event had registered, fire the event.
		wsprintf(onEventName,L"on%sChanged",vn.c_str());
		iter = m_events.find(std::wstring(onEventName));
		if (iter != m_events.end())
		{
#ifndef ITEM_EVENT_ARGS_NUM
#define ITEM_EVENT_ARGS_NUM	(3)
#endif
			VARIANT varParam[ITEM_EVENT_ARGS_NUM] = {0};
			for (int i = 0; i < ITEM_EVENT_ARGS_NUM; i++)
			{
				VariantInit(&varParam[i]);
			}
			varParam[0].vt = VT_DISPATCH;           //第三个参数 data
			hr = this->QueryInterface(IID_IDispatch,(void**)&varParam[0].pdispVal);
			HR_CHECK(hr);

			hr = VariantCopy(&varParam[1],pTempV);		//第二个参数 new value
			HR_CHECK(hr);

			hr = VariantCopy(&varParam[2],&varOld);		//第一个参数 old value
			HR_CHECK(hr);

			//fire dispatch data event to Java script.
			hr = m_dataEvent.Fire(
				iter->second,
				varParam,
				ITEM_EVENT_ARGS_NUM,
				NULL);
 			VariantClear(&varOld);
// 			for ( size_t i= 0; i< ITEM_EVENT_ARGS_NUM; i++ )
// 			{
// 				VariantClear( &(varParam[i]) );
// 			}
			goto exit;
		}
		else
		{
			//search event object in event object map.
			std::map<KEY, IDispatch*>::iterator it;
			it = m_eventObjects.find(std::wstring(onEventName));
			if ( it != m_eventObjects.end() 
				&& it->second != NULL )
			{
#ifndef ITEM_EVENT_ARGS_NUM
#define ITEM_EVENT_ARGS_NUM	(3)
#endif
				VARIANT varParam[ITEM_EVENT_ARGS_NUM] = {0};
				for (int i = 0; i < ITEM_EVENT_ARGS_NUM; i++)
				{
					VariantInit(&varParam[i]);
				}
				varParam[0].vt = VT_DISPATCH;           //第三个参数 data
				hr = this->QueryInterface(IID_IDispatch,(void**)&varParam[0].pdispVal);
				HR_CHECK(hr);

				hr = VariantCopy(&varParam[1],pTempV);		//第二个参数 new value
				HR_CHECK(hr);

				hr = VariantCopy(&varParam[2],&varOld);		//第一个参数 old value
				HR_CHECK(hr);
				{
					//fire the event object.
					UINT errArg = 0;
					DISPID dispid = 0;
					VARIANT varRet;
					VariantInit(&varRet);
					//no need to specific method name. dispid=0 for anonymous JavaScript function.
					DISPPARAMS dParams = { varParam, 0, ITEM_EVENT_ARGS_NUM, 0 };
					hr = it->second->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
						DISPATCH_METHOD, &dParams, &varRet, 0, &errArg);
					/*for ( size_t i= 0; i< ITEM_EVENT_ARGS_NUM; i++ )
					{
						VariantClear( &(varParam[i]) );
					}*/
					goto exit;
				}
			}
		}
		//if single property event not fired, fire onpropertychanged event.
		iter = m_events.begin();
		iter = m_events.find(std::wstring(L"onPropertyChanged"));
		if ( iter != m_events.end() )
		{
#ifndef PROPERTY_EVENT_ARGS_NUM
#define PROPERTY_EVENT_ARGS_NUM	(4)
#endif
			VARIANT varParam[PROPERTY_EVENT_ARGS_NUM] = {0};
			for (int i = 0; i < ITEM_EVENT_ARGS_NUM; i++)
			{
				VariantInit(&varParam[i]);
			}

			varParam[0].vt = VT_DISPATCH;           //第四个参数 data
			hr = this->QueryInterface(IID_IDispatch,(void**)&varParam[0].pdispVal);		
			HR_CHECK(hr);

			hr = VariantCopy(&varParam[1],pTempV);		//第三个参数 new value
			HR_CHECK(hr);

			hr = VariantCopy(&varParam[2],&varOld);  //第二个参数 old value
			HR_CHECK(hr);

			varParam[3].vt = VT_BSTR;               //第一个参数 property name
			varParam[3].bstrVal = SysAllocString(vn.c_str());

			//fire dispatch data event to Java script.
			hr = m_dataEvent.Fire(
				iter->second,
				varParam,
				PROPERTY_EVENT_ARGS_NUM,
				NULL);
 			VariantClear(&varOld);
// 			for ( size_t i= 0; i< PROPERTY_EVENT_ARGS_NUM; i++ )
// 			{
// 				VariantClear( &(varParam[i]) );
// 			}
		}
exit:
		if (FAILED(hr))
		{
			LOG_ERR(L"DataDispatchImpl::UpdatePropertyValue exit with error 0x%08X", hr);
		}
		return hr;
	}

	/*
	* Compare property value within dispatch data object.
	*/
	BOOL CompareProperty(const KEY & key,const VALUE & propertyData)
	{
		std::map<KEY, VALUE>::iterator it;
		it = m_properties.find(key);
		if ( it != m_properties.end() )         //compare the value of key.
		{
			//found this key compare the data type first.
			if ( propertyData.vt != it->second.vt )
			{
				//LOG_INFO(L"CompareProperty:Data type(vt) is different.");
				switch(propertyData.vt)
				{
				case VT_R8:
					if (it->second.vt == VT_UINT
						|| it->second.vt == VT_UI4)
					{
						UINT32 cvalue = (UINT32)propertyData.dblVal;
						if ( cvalue == it->second.uintVal )
						{
							return true;
						}
					}
					else if (it->second.vt == VT_I4
						|| it->second.vt == VT_INT)
					{
						int cvalue = (int)propertyData.dblVal;
						if ( cvalue == it->second.lVal )
						{
							return true;
						}
					}
				case VT_I4:
					if (it->second.vt == VT_UINT
						|| it->second.vt == VT_UI4)
					{
						UINT32 cvalue = (UINT32)propertyData.lVal;
						if (cvalue == it->second.uintVal)
						{
							return true;
						}
					}
				case VT_BSTR:
					//TODO: convert the bstr to vt type then compare.
					return false;
				default:
					return false;
				}

				return false;
			}

			//then compare the value.
			LONGLONG compareValue = propertyData.llVal;
			switch(it->second.vt)
			{
			case VT_BOOL:
				if (!(it->second.boolVal) == !(propertyData.boolVal))
					return true;
				return false;
			case VT_INT:
			case VT_I4:
				if (it->second.lVal == (int)compareValue)
					return true;
				return false;
			case VT_UI4:
			case VT_UINT:
				if (it->second.uintVal == (UINT32)compareValue)
					return true;
				return false;
			case VT_UI8:
				return false;
			case VT_R4:
			case VT_I8:
			case VT_R8:
			case VT_CY:
			case VT_DATE:
				if ( (propertyData.dblVal + 0.001 > it->second.dblVal) && 
					(propertyData.dblVal - 0.001 < it->second.dblVal) )
					return true;
				return false;
			case VT_UNKNOWN:
				if ( propertyData.punkVal == it->second.punkVal )
					return true;
				return false;
			case VT_DISPATCH:
				if ( propertyData.pdispVal == it->second.pdispVal )
					return true;
				return false;
			case VT_BSTR:
				if ( std::wstring( propertyData.bstrVal ) == std::wstring(it->second.bstrVal) )
					return true;
				return false;
			case VT_ERROR:
			case VT_EMPTY:
			default:
				return false;
			}
		}

		LOG_ERR(L"DataDispatchImpl: compare property, dispatch data doesn't have this property.");
		return false;
	}
};

/**
* @class DataDispatchServiceInstanceImpl
* @brief DataDispatchServiceInstanceImpl is a template class to dispatch the data to Java script.
* This template implements an IUnknown and an IDispatch interface.
* 
* @author yueming.zou
* @version 1.0.0
*/
template<class T>
class DataDispatchServiceInstanceImpl : public DataDispatchImpl< T >
{
public:
	typedef DataDispatchImpl< T >	BASE;

	DataDispatchServiceInstanceImpl() 
		: BASE(), 
		m_parent( NULL ), 
		m_groupId( 0 ) 
	{

	}

	void Init( IDispatch * parent, UINT32 groupId ) 
	{
		m_parent = parent;
		m_groupId = groupId;
	}

	//daming: 2011-08-10 先调本类的GetIdsOfNames，如果失败，则调用父类的。
	STDMETHODIMP GetIDsOfNames(
		REFIID riid,
		LPOLESTR* rgszNames, 
		UINT cNames, 
		LCID lcid, 
		DISPID* rgDispId)
	{
		HRESULT hr = S_OK;
// 		ARG_CHECK( rgszNames );
// 		ARG_CHECK( rgDispId );
		hr = BASE::GetIDsOfNames( riid, rgszNames, cNames, lcid, rgDispId );
		if( hr != S_OK )
		{
			//daming: 2011-08-11 一定不要用FAILED(hr)，我发现出错时，用FAILED不算错。。。。晕。。。。

			hr = m_parent->GetIDsOfNames( riid, rgszNames, cNames, lcid, rgDispId );
			HR_CHECK( hr );
		}
exit:
		if (FAILED(hr))
		{
			LOG_ERR(L"DataDispatchServiceInstanceImpl::GetIDsOfNames exit with error 0x%08X", hr);
		}
		return hr;
	}

	//daming: 2011-08-10 先调本类的Invoke，如果失败，则调用父类的。
	STDMETHODIMP Invoke(
		DISPID dispidMember, 
		REFIID riid, 
		LCID lcid, 
		WORD wFlags, 
		DISPPARAMS* pDispParams, 
		VARIANT* pVarResult, 
		EXCEPINFO* pExcepInfo, 
		UINT* puArgErr)
	{
		HRESULT hr = S_OK;
		hr = BASE::Invoke( dispidMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr );
		//daming: 2011-08-11 一定不要用FAILED(hr)，我发现出错时，用FAILED不算错。。。。晕。。。。
		if ( S_OK != hr ) {
			//daming: 2011-08-10 调服务类的功能
			if ( DISPATCH_METHOD == wFlags && pDispParams->cArgs > 0 ) {

				// 是方法，要将GroupId加到第一个参数
				std::vector< VARIANT > & vs = mParams;
				vs.resize( pDispParams->cArgs + 1 );

				// 第一步，把原来的参数拷贝过来
				memcpy( & vs[ 0 ], pDispParams->rgvarg, sizeof( VARIANT ) * ( pDispParams->cArgs - 1 ) );

				// 第二步，把this拷贝到最后
				memcpy( & vs[ pDispParams->cArgs ], & pDispParams->rgvarg[ pDispParams->cArgs - 1 ], sizeof( VARIANT ) );

				// 第三步，把groupId拷贝到倒数第二个元素，做为函数的第一个参数
				memset( & vs[ pDispParams->cArgs - 1 ], 0, sizeof( VARIANT ) );
				vs[ pDispParams->cArgs - 1 ].vt = VT_UINT;
				vs[ pDispParams->cArgs - 1 ].uintVal = m_groupId;

				// 第四步，换参数栈
				++ pDispParams->cArgs;
				pDispParams->rgvarg = & vs[ 0 ];

				// 第五步，把大象装冰箱里
				hr = m_parent->Invoke( dispidMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr );
			} else {
				// 不是方法，直接调
				hr = m_parent->Invoke( dispidMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr );
			}
		}
		return hr;
	}

protected:
	IDispatch * m_parent;
	UINT32 m_groupId;
	std::vector< VARIANT > mParams;
private:
	// disable
	DataDispatchServiceInstanceImpl(const DataDispatchServiceInstanceImpl &);
	const DataDispatchServiceInstanceImpl & operator = (const DataDispatchServiceInstanceImpl &);
};


/**
* @class TangDataArray
* @brief TangDataArray is a template class to dispatch the data list to Java script.
* this dispatch list support complex and simple type data.
* This template implements an IUnknown and an IDispatch interface.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
template <class _TYPE, VARTYPE vt>
class TangDataArray: public IDispatch
{
protected:
	/*
	* The internal COM reference counter, used by the IUnknown implementation.
	*/
	long			       m_COMReferenceCounter;
	/*
	*
	*/
	bool					m_CannotSelfDelete;
	/*
	*
	*/
	bool					m_CannotPropertyPut;
	/*
	*
	*/
	std::vector<_TYPE>      m_lstData;
	/*
	* event include onAdded, onRemove,onUpdate for data array.
	*/
	std::map<KEY, JSEVENTHANDLER>   m_events;
	/*
	* event include event object
	*/
	std::map<KEY, IDispatch*>   m_eventObjects;
	/*
	*
	*/
	std::map<DISPID, KEY>           m_actionNamesMap;
	/*
	*
	*/
	std::map<KEY, DISPID>           m_actionIDsMap;
	/*
	*
	*/
	TangDataEvent                   m_dataEvent;
	/*
	*
	*/
	IDispatch*                      m_parentObj;
	/*
	*
	*/
	//CRITICAL_SECTION                m_lock;

public:
	/*
	* Constructor
	*/
	TangDataArray()
		:m_COMReferenceCounter(1),
		m_CannotPropertyPut(false),
		m_CannotSelfDelete(false),
		m_parentObj(NULL)
	{
		//::InitializeCriticalSection(&m_lock);
		m_eventObjects.clear();

		//build dispatch event map.
		m_actionNamesMap[DISPID_ARRAY_EVENT_ADD] = std::wstring(DISP_ARRAY_EVENT_KEY_ADD);
		m_actionIDsMap[std::wstring(DISP_ARRAY_EVENT_KEY_ADD)] = DISPID_ARRAY_EVENT_ADD;
		m_actionNamesMap[DISPID_ARRAY_EVENT_REMOVE] = std::wstring(DISP_ARRAY_EVENT_KEY_REMOVE);
		m_actionIDsMap[std::wstring(DISP_ARRAY_EVENT_KEY_REMOVE)] = DISPID_ARRAY_EVENT_REMOVE;
		m_actionNamesMap[DISPID_ARRAY_EVENT_UPDATE] = std::wstring(DISP_ARRAY_EVENT_KEY_UPDATE);
		m_actionIDsMap[std::wstring(DISP_ARRAY_EVENT_KEY_UPDATE)] = DISPID_ARRAY_EVENT_UPDATE;

		//build dispatch method map.
		m_actionNamesMap[DISPID_ARRAY_METHOD_AT] = std::wstring(DISP_ARRAY_METHOD_KEY_AT);
		m_actionIDsMap[std::wstring(DISP_ARRAY_METHOD_KEY_AT)] = DISPID_ARRAY_METHOD_AT;
		//m_actionNamesMap[DISPID_ARRAY_METHOD_SORT] = std::wstring(DISP_ARRAY_METHOD_KEY_SORT);
		//m_actionIDsMap[std::wstring(DISP_ARRAY_METHOD_KEY_SORT)] = DISPID_ARRAY_METHOD_SORT;
		m_actionNamesMap[DISPID_ARRAY_METHOD_FIND] = std::wstring(DISP_ARRAY_METHOD_KEY_FIND);
		m_actionIDsMap[std::wstring(DISP_ARRAY_METHOD_KEY_FIND)] = DISPID_ARRAY_METHOD_FIND;
		m_actionNamesMap[DISPID_ARRAY_METHOD_COUNT] = std::wstring(DISP_ARRAY_METHOD_KEY_COUNT);
		m_actionIDsMap[std::wstring(DISP_ARRAY_METHOD_KEY_COUNT)] = DISPID_ARRAY_METHOD_COUNT;

		//build dispatch property map.
		m_actionNamesMap[DISPID_PROPERTY_PARENT] = std::wstring(DISP_PROPERTY_KEY_PARENT);
		m_actionIDsMap[std::wstring(DISP_PROPERTY_KEY_PARENT)] = DISPID_PROPERTY_PARENT;
		m_actionNamesMap[DISPID_ARRAY_PROPERTY_SIZE] = std::wstring(DISP_ARRAY_PROPERTY_KEY_SIZE);
		m_actionIDsMap[std::wstring(DISP_ARRAY_PROPERTY_KEY_SIZE)] = DISPID_ARRAY_PROPERTY_SIZE;
	}

	/*
	* Destructor
	*/
	//daming: 2011-08-09 ！！突然发现，它是基类，应该用virtual destructor吧？
	virtual ~TangDataArray()
	{
		//::DeleteCriticalSection(&m_lock);
		if ( m_parentObj )
		{
			m_parentObj->Release();
			m_parentObj = NULL;
		}
		//release event objects.
		std::map<KEY, IDispatch*>::iterator it = m_eventObjects.begin();
		while( it != m_eventObjects.end() )
		{
			if (NULL != it->second)
			{
				it->second->Release();
			}
			it++;
		}

		std::map<KEY, JSEVENTHANDLER>::iterator iter = m_events.begin();
		while (iter != m_events.end())
		{
			SYSFREESTRING(iter->second);
			iter++;
		}
		OutputDebugString(L"TangDataArray destruction.");
	}

	/*
	* Virtual method to get list data by index.
	*/
	virtual HRESULT at(int index, VALUE* v)
	{
		index;
		v;
		return E_NOTIMPL;
	}

	/*
	* Virtual method to append data node into dispatch list, then fire OnAdd event.
	*/
	virtual HRESULT push_back(_TYPE v)
	{
		HRESULT hr = S_OK;
		std::map<KEY, JSEVENTHANDLER>::iterator iter;
		//AutoLock lock(&m_lock);

		//push back the new item.
		m_lstData.push_back(v);

		//if add event registered, fire the add event.
		iter = m_events.find(std::wstring(DISP_ARRAY_EVENT_KEY_ADD));
		if ( iter != m_events.end() )
		{
#ifndef ADD_EVENT_ARGS_NUM
#define ADD_EVENT_ARGS_NUM	(2)
#endif
			VARIANT varParam[ADD_EVENT_ARGS_NUM] = {0};
			hr = GetEventParam(v,varParam);
			HR_CHECK( hr );
			hr = m_dataEvent.Fire(
				iter->second,
				varParam,
				ADD_EVENT_ARGS_NUM,
				NULL);
			/*for ( size_t i= 0; i< ADD_EVENT_ARGS_NUM; i++ )
			{
				VariantClear( &(varParam[i]) );
			}
			HR_CHECK( hr );*/
		}
		else
		{
			//search event object in event object map.
			std::map<KEY, IDispatch*>::iterator it;
			it = m_eventObjects.find(std::wstring(DISP_ARRAY_EVENT_KEY_ADD));
			if (it != m_eventObjects.end() && it->second != NULL)
			{
#ifndef ADD_EVENT_ARGS_NUM
#define ADD_EVENT_ARGS_NUM	(2)
#endif
				VARIANT varParam[ADD_EVENT_ARGS_NUM] = {0};
				//fire the event object.
				UINT errArg = 0;
				DISPID dispid = 0;
				VARIANT varRet;
				VariantInit( &varRet );
				hr = GetEventParam(v,varParam);
				HR_CHECK( hr );
				//no need to specific method name. dispid=0 for anonymous JavaScript function.
				{
					DISPPARAMS dParams = { varParam, 0, ADD_EVENT_ARGS_NUM, 0 };
					hr = it->second->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
						DISPATCH_METHOD, &dParams, &varRet, 0, &errArg);
					/*for ( size_t i= 0; i< ADD_EVENT_ARGS_NUM; i++ )
					{
						VariantClear( &(varParam[i]) );
					}*/
					HR_CHECK( hr );
				}
			}
		}
exit:
		if (FAILED(hr))
		{
			LOG_ERR(L"TangDataArray::push_back with error: 0x%08X",hr);
		}

		return hr;
	}

	/*
	* Virtual method to erase data node from dispatch list by index.
	*   then fire onRemove event.
	*/
	virtual HRESULT erase(int index)
	{
		HRESULT hr = S_OK;
		std::map<KEY, JSEVENTHANDLER>::iterator iter;
		std::vector<_TYPE>::iterator it;
		//AutoLock lock(&m_lock);
		if (index >= 0 && index < (int)m_lstData.size())
		{
			_TYPE data;
			it = m_lstData.begin() + index;
			//erase the data.
			data = (*it);
			m_lstData.erase(it);
			//if remove event registered, fire the remove event.
			iter = m_events.find(std::wstring(DISP_ARRAY_EVENT_KEY_REMOVE));
			if (iter != m_events.end())
			{
#ifndef REMOVE_EVENT_ARGS_NUM
#define REMOVE_EVENT_ARGS_NUM	(2)
#endif
				VARIANT varParam[REMOVE_EVENT_ARGS_NUM] = {0};
				hr = GetEventParam(data,varParam);
				HR_CHECK(hr);
				hr = m_dataEvent.Fire(
					iter->second,
					varParam,
					REMOVE_EVENT_ARGS_NUM,
					NULL);
				/*for ( size_t i= 0; i< REMOVE_EVENT_ARGS_NUM; i++ )
				{
					VariantClear( &(varParam[i]) );
				}
				HR_CHECK(hr);*/
			}
			else
			{
				//search event object in event object map.
				std::map<KEY, IDispatch*>::iterator it;
				it = m_eventObjects.find(std::wstring(DISP_ARRAY_EVENT_KEY_REMOVE));
				if (it != m_eventObjects.end() && it->second != NULL)
				{
#ifndef REMOVE_EVENT_ARGS_NUM
#define REMOVE_EVENT_ARGS_NUM	(2)
#endif
					VARIANT varParam[REMOVE_EVENT_ARGS_NUM] = {0};
					hr = GetEventParam(data,varParam);
					HR_CHECK( hr );
					//fire the event object.
					//no need to specific method name. dispid=0 for anonymous JavaScript function.
					{
						UINT errArg = 0;
						DISPID dispid = 0;
						VARIANT varRet;
						VariantInit( &varRet );
						DISPPARAMS dParams = { varParam, 0, REMOVE_EVENT_ARGS_NUM, 0 };
						hr = it->second->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
							DISPATCH_METHOD, &dParams, &varRet, 0, &errArg);
						/*for ( size_t i= 0; i< REMOVE_EVENT_ARGS_NUM; i++ )
						{
							VariantClear( &(varParam[i]) );
						}*/
						HR_CHECK( hr );
					}
				}
			}
			if ( data )
			{
				if ( vt == VT_DISPATCH )
				{
					IDispatch * pDisp = (IDispatch * )data;
					pDisp->Release();
				}
				else if ( vt == VT_BSTR )
				{
					BSTR bstrData = (BSTR)data;
					::SysFreeString( bstrData );
				}
			}
		}
		else
		{
			hr = ERROR_ACCESS_DENIED;
		}
exit:
		if (FAILED(hr))
		{
			LOG_ERR(L"TangDataArray: erase exit with error: 0x%08X", hr);
		}
		return hr;
	}

	/*
	* Virtual method to erase data node from dispatch list by property and property value.
	*  and fire onRemove event.This methd is provide to complex dispatch object,
	*/
	virtual HRESULT erase(const KEY & key,const VALUE & keyData)
	{
		HRESULT hr = S_OK;
		_TYPE pData = NULL;

		//find the data.
		int index = find(key,keyData,&pData);
		if (pData == NULL || index == -1)   //not found
		{
			//return if find failed.
			return E_FAIL;
		}

		//fire remove event and do erase.
		return erase(index);
	}

	/*
	* Virtual method to erase data node from dispatch list by data value.
	*  and fire onRemove event.This metohd is provide to simple dispatch object,
	*/
	virtual HRESULT erase(const VALUE & keyData)
	{
		HRESULT hr = S_OK;

		//find the data.
		int index = find(keyData);
		if (index == -1)   //not found
		{
			//return if find failed.
			return E_FAIL;
		}

		//fire remove event and do erase.
		return erase(index);
	}

	/*
	* Virtual method to find out the data node from dispatch list which contains the specific
	* property and equal the value.
	* This method is provide to complex dispatch object,
	*/
	virtual int find(const KEY & key,const VALUE & keyData, _TYPE* ppRet)
	{
		int uRetIndex = -1;
		key;
		keyData;
		ppRet;
		return uRetIndex;                           //return the index of found data.
	}

	/*
	* Virtual method to find out the data node from dispatch list which equal the value.
	* This method is provide to simple dispatch object,
	*/
	virtual int find(const VALUE & keyData)
	{
		int uRetIndex = -1;
		keyData;
		return uRetIndex;                           //return the index of found data.
	}

	/*
	* Virtual method to get the count of the data which equal the value.
	* This method is provide to simple dispatch object,
	*/
	virtual int findCount(const VALUE & data)
	{
		int uRetCount = 0;
		data;
		return uRetCount;                           //return the count of found data.
	}

	/*
	* Virtual method to get the count of data list which contains the specific
	* property and equal the value.
	* This method is provide to complex dispatch object,
	*/
	virtual int findCount(const KEY & key,const VALUE & keyData)
	{
		int uRetCount = 0;
		key;
		keyData;
		return uRetCount;                           //return the count of found data.
	}

	/*
	* Method to get the size of dispatch list.
	*/
	virtual size_t size()
	{
		return m_lstData.size();
	}

	/*
	* Method to clear the dispatch list.
	*/
	virtual void clear()
	{
		m_lstData.clear();
	}

	/*
	* Method to disable object delete itself.
	*/
	void DisableSelfDelete()
	{
		m_CannotSelfDelete = true;
	}

	/*
	* Method to disable dispatch object to do property put.
	*/
	void DisablePropertyPut()
	{
		m_CannotPropertyPut = true;
	}

	/*
	* Final release method.
	*/
	virtual void OnFinalRelease()
	{
	}
	/*
	*
	*/
	virtual HRESULT GetEventParam(_TYPE v, VARIANT* param)
	{
		param;
		return E_NOTIMPL;
	}

	/*
	* Method to initialize dispatch list event object.
	*/
	HRESULT InitEventSink(IDispatch* pIEInstance)
	{
		return m_dataEvent.Init(pIEInstance);
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

		return S_OK;
	}

	/*
	* IDispatch::GetIDsOfNames() implementation
	*/
	STDMETHODIMP GetIDsOfNames(REFIID riid,
		LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
	{
		DWORD j = 0;
		std::map<std::wstring, DISPID>::iterator it;
		for( ; j<cNames; j++ )
		{
			it = m_actionIDsMap.find(rgszNames[j]);
			if (it == m_actionIDsMap.end())
				return ERROR_ACCESS_DENIED;
			rgDispId[j]=it->second;
		}
		return S_OK;
	}

	/*
	* IDispatch::Invoke() implementation
	*/
	STDMETHODIMP Invoke(
		DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, 
		DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, 
		UINT* puArgErr)
	{
		HRESULT hr = S_OK;
		std::map<DISPID, KEY>::iterator it = m_actionNamesMap.find(dispidMember);
		if (it == m_actionNamesMap.end()) return ERROR_ACCESS_DENIED;
		const KEY & strFN = it->second;
		switch( wFlags ) 
		{
		case DISPATCH_PROPERTYGET:
			// property get of named variables
			switch( dispidMember )
			{
			case DISPID_ARRAY_PROPERTY_SIZE:
				pVarResult->vt = VT_I4;
				pVarResult->lVal = size();
				break;
			case DISPID_PROPERTY_PARENT:
				if( NULL != pVarResult )
				{
					pVarResult->vt = VT_DISPATCH;
					pVarResult->pdispVal = NULL;
					if ( m_parentObj  )
					{
						pVarResult->vt = VT_DISPATCH;
						hr = m_parentObj->QueryInterface(IID_IDispatch,
							(void**)pVarResult->pdispVal);
						HR_CHECK( hr );
					}
				}
				break;
			default:
				break;
			}
			break; 
		case DISPATCH_PROPERTYPUT:
			//  put event handler.
			if (dispidMember>=100 && strFN.length() > 0 
				&& NULL != pDispParams
				&& pDispParams->rgvarg->vt == VT_BSTR)
			{
				std::map<KEY, JSEVENTHANDLER>::iterator itT = m_events.find( strFN );
				if (  itT != m_events.end() && itT->second != NULL )
				{
					::SysFreeString( itT->second );
					itT->second = NULL;
				}
				BSTR bstrValue = NULL;
				bstrValue= ::SysAllocString( pDispParams->rgvarg->bstrVal );
				ALLOC_CHECK( bstrValue );
				m_events[strFN] = bstrValue;
			}
			else if ( !m_CannotPropertyPut )      //property put of named variables
			{
				//TODO: property put.
			}
			break; 
		case (DISPATCH_PROPERTYPUT|DISPATCH_PROPERTYPUTREF): //property put dispatch reference.
			{
				//put event object.
				if (strFN.length() > 0 && dispidMember>=100
					&& NULL != pDispParams
					&& pDispParams->rgvarg->vt == VT_DISPATCH)
				{
					//to avoid user set an event more than once.
					std::map<KEY, IDispatch*>::iterator iter;
					iter = m_eventObjects.find(strFN);
					if (iter != m_eventObjects.end())
					{
						//release the counter of event object.
						iter->second->Release();
						m_eventObjects.erase(iter);
					}
					//put the event object.
					IDispatch* pEventDispatch  = NULL;
					hr = pDispParams->rgvarg->pdispVal->QueryInterface(IID_IDispatch,(void**)&pEventDispatch);
					HR_CHECK( hr );
					CHECK_IF_NULL( pEventDispatch );
					m_eventObjects[strFN] =pEventDispatch;
				}
				break;
			}
		case DISPATCH_METHOD:
		default:
			CHECK_IF_NULL( pDispParams );
			if ( dispidMember == DISPID_ARRAY_METHOD_AT )
			{
				int index = pDispParams->rgvarg->intVal;
				if ( index >= 0 && pDispParams->cArgs == 1 )
				{
					hr = this->at(index,pVarResult);
					HR_CHECK( hr );
				}
				else if( NULL != pVarResult )
				{
					hr = ERROR_ACCESS_DENIED;
					pVarResult->vt = VT_INT;
					pVarResult->intVal = hr;
				}
			}
			else if ( dispidMember == DISPID_ARRAY_METHOD_FIND )
			{
				int index = -1;
				if (pDispParams->cArgs == 1)    //call simple dispatch list find(value) method.
				{
					index = this->find((*pDispParams->rgvarg));
				}
				else if ( pDispParams->cArgs == 2 )
				{
					if (pDispParams->rgvarg[1].vt == VT_BSTR
						&& pDispParams->rgvarg[1].bstrVal)
					{
						//call complex dispatch list find(value) method.
						_TYPE pData = NULL;
						index = this->find(
							pDispParams->rgvarg[1].bstrVal,
							(*pDispParams->rgvarg),
							&pData);
						if ( NULL != pData )
						{
							if( VT_DISPATCH == vt)
							{
								IDispatch * pTemp = (IDispatch *)pData;
								pTemp->Release();
							}
							else if ( VT_BSTR == vt )
							{
								BSTR bstrTemp = (BSTR)pData;
								::SysFreeString( bstrTemp );
							}
						}
					}
				}
				if ( pVarResult )
				{
					//return the index of data in this list.
					hr = this->at(index,pVarResult);
					HR_CHECK( hr );
				}
			}
			else if( dispidMember == DISPID_ARRAY_METHOD_COUNT )
			{
				int count = 0;
				if (pDispParams->cArgs == 1)    //call simple dispatch list findCount(value) method.
				{
					count = this->findCount((*pDispParams->rgvarg));
				}
				else if ( pDispParams->cArgs == 2 )
				{
					if (pDispParams->rgvarg[1].vt == VT_BSTR
						&& pDispParams->rgvarg[1].bstrVal)
					{
						//call complex dispatch list findCount(value) method.
						count = this->findCount(
							pDispParams->rgvarg[1].bstrVal,
							(*pDispParams->rgvarg));
					}
				}
				if (pVarResult)
				{
					//return the count of data in this list.
					pVarResult->vt = VT_I4;
					pVarResult->lVal = count;
				}
			}
			break;
		}
exit:
		if (FAILED(hr))
		{
			LOG_ERR(L"TangDataArray::Invoke  with error: 0x%08X",hr);
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
            riid == IID_IDispatch)
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
		if( --m_COMReferenceCounter <=0 )
		{
			if(!m_CannotSelfDelete)
			{
				OnFinalRelease();
				delete this;
			}
			return 0;
		}
		else
		{
			return m_COMReferenceCounter;
		}
    }
};

/**
* @class TangComplexDataArray
* @brief TangComplexDataArray is a template class to dispatch the complex data list to Java script.
* @detail It inherits from TangDataArray,
* this dispatch list support complex data,
* This template implements an IUnknown and an IDispatch interface.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
template<class _TYPE>
class TangComplexDataArray : public TangDataArray<_TYPE,VT_DISPATCH>
{
public:
    
    /**
    * @brief	<b>Implement the virtual method to get list data by index.</b> 
    *
    *			<b>Detail: Send a text message to some users that indecate by the list.</b>  
    *
    * @param[in] BSTR bstDestUsers : A user ID list like "123;456;789;", or "0" 
    *                                "0" represents that the message will be sent to all users.
    *                                "1001;1002" represents that the message will be sent to 1001 and 1002.
    * @param[in] BSTR bstrMessage : A text message which is encoded in the UNICODE;
    *
    * @return HRESULT : Return 0 if successful;
    *
    * @par Example:
    * @code
    *  
    * @endcode
    * @pre Need the ChatPrivilegeSend privilege to send the message;
    * @post NONE;
    * @see
    **************************************************************************/
	virtual HRESULT at(int index, VALUE* pV)
    {
        HRESULT hr = S_OK;
		_TYPE itemRef = NULL;
		ARG_CHECK( pV );
        pV->vt = VT_DISPATCH;
		pV->pdispVal = NULL;
		if( index >= 0  && index < (int)m_lstData.size() )
		{
			itemRef = m_lstData.at(index);
			CHECK_IF_NULL( itemRef );
			
			hr = itemRef->QueryInterface(IID_IDispatch,(void **)&pV->pdispVal);
			HR_CHECK(hr);
			CHECK_IF_NULL(pV->pdispVal);
		}
		else
		{
			hr = ERROR_ACCESS_DENIED;
		}
exit:
		if ( FAILED(hr) )
		{
			
			LOG_ERR( L"TangComplexDataArray::at exit with error: 0x%08X.",hr );
		}
        return hr;
    }
    
    /**
    * @brief	<b>IImplement the virtual method to find the complex data.Return the index of the data.</b> 
    *
    *			<b>Detail: Send a text message to some users that indecate by the list.</b>  
    *
    * @param[in] BSTR bstDestUsers : A user ID list like "123;456;789;", or "0" 
    *                                "0" represents that the message will be sent to all users.
    *                                "1001;1002" represents that the message will be sent to 1001 and 1002.
    * @param[in] BSTR bstrMessage : A text message which is encoded in the UNICODE;
    *
    * @return HRESULT : Return 0 if successful;
    *
    * @par Example:
    * @code
    *  
    * @endcode
    * @pre Need the ChatPrivilegeSend privilege to send the message;
    * @post NONE;
    * @see
    **************************************************************************/
    virtual int find(const KEY & key, const VALUE & keyData, _TYPE* ppRet)
    {
        BOOL bFound = false;
        int uRetIndex = -1;
        //AutoLock lock(&m_lock);

        std::vector<_TYPE>::iterator iter;
        for (iter = m_lstData.begin();
            iter != m_lstData.end();
            iter++)
        {
            uRetIndex ++;
            bFound = false;

            //found this key then compare the data.
            bFound = (*iter)->CompareProperty(key, keyData);

            if (bFound && ppRet)
            {
                //found, return with the data pointer.
                *ppRet = m_lstData.at(uRetIndex);
                IDispatch* pDisp = ( *ppRet );
				if( NULL != pDisp )
 				{
 					pDisp->AddRef();
 				}
                break;
            }
        }

        uRetIndex = bFound ? uRetIndex : -1;        //-1 stand for not be found.
        return uRetIndex;                           //return the index of found data.
    }

    /**
    * @brief	<b>Implement the virtual method to get the count of a complex data.Return the count of the data.</b> 
    *
    *			<b>Detail: Send a text message to some users that indecate by the list.</b>  
    *
    * @param[in] BSTR bstDestUsers : A user ID list like "123;456;789;", or "0" 
    *                                "0" represents that the message will be sent to all users.
    *                                "1001;1002" represents that the message will be sent to 1001 and 1002.
    * @param[in] BSTR bstrMessage : A text message which is encoded in the UNICODE;
    *
    * @return HRESULT : Return 0 if successful;
    *
    * @par Example:
    * @code
    *  
    * @endcode
    * @pre Need the ChatPrivilegeSend privilege to send the message;
    * @post NONE;
    * @see
    **************************************************************************/
    virtual int findCount(const KEY & key,const VALUE & keyData)
    {
        int uRetCount = 0;
        std::vector<_TYPE>::iterator iter;
        BOOL bFound = false;
        //AutoLock lock(&m_lock);

        for (iter = m_lstData.begin();
            iter != m_lstData.end();
            iter++)
        {
            bFound = false;

            //found this key then compare the data.
            bFound = (*iter)->CompareProperty(key, keyData);

            if (bFound)
            {
                //found, count++.
                uRetCount++;
            }
        }

        return uRetCount;
    }

    /*
    *
    */
    virtual HRESULT GetEventParam(_TYPE v, VARIANT* pParams)
    {
        HRESULT hr = S_OK;
		ARG_CHECK( pParams );
		if ( m_parentObj )
		{
			pParams[0].vt = VT_DISPATCH;
			pParams[0].pdispVal = NULL;
			hr = m_parentObj->QueryInterface(IID_IDispatch,(void**)&pParams[0].pdispVal);  //参数2 parent.
			HR_CHECK( hr );
			CHECK_IF_NULL( pParams[0].pdispVal );
		}
		else
		{
			VariantInit( &(pParams[0]) );
		}
		if ( v )
		{
			pParams[1].vt = VT_DISPATCH;
			pParams[1].pdispVal = NULL;
			hr = v->QueryInterface(IID_IDispatch,(void**)&pParams[1].pdispVal);  //参数1 data object.
			HR_CHECK( hr );
			CHECK_IF_NULL( pParams[1].pdispVal );
		}
		else
		{
			VariantInit( &(pParams[1]) );
		}
exit:
		if ( FAILED(hr) )
		{
			LOG_ERR( L"TangComplexDataArray::GetEventParam exit with error: 0x%08X.",hr );
		}
        return hr;
    }
};

/**
* @class TangSimpleDataArray
* @brief TangSimpleDataArray is a template class to dispatch the complex data
* list to Java script.It inherits from TangDataArray.
* this dispatch list support complex data.
* This template implements an IUnknown and an IDispatch interface.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
template <class _TYPE,VARTYPE vt>
class TangSimpleDataArray : public TangDataArray<_TYPE,vt>
{
public:
    /*
    * Implement the virtual method to get list data by index.
    */
    virtual HRESULT at(int index, VALUE* v)
    {
        HRESULT hr = S_OK;
        //AutoLock lock(&m_lock);

        if (index >= 0 && index < (int)m_lstData.size())
        {
            //TODO:
            CComVariant var;
            var = m_lstData[index];
            switch(vt&var.vt)
            {
            case VT_UINT:
                v->vt = VT_UINT;
                v->uintVal = var.uintVal;
                break;
            case VT_I4:
                v->vt = VT_I4;
                v->lVal = var.lVal;
                break;
            case VT_BSTR:
                v->vt = VT_BSTR;
                v->bstrVal = SysAllocString(var.bstrVal);
                break;
            default:
                break;
            }
        }

        return hr;
    }

    /*
    * Implement the virtual method to find the simple data.Return the index of the data.
    */
    virtual int find(const VALUE & keyData)
    {
        int uRetIndex = -1;
        BOOL bFound = false;
        //AutoLock lock(&m_lock);

        std::vector<_TYPE>::iterator iter;
        for (iter = m_lstData.begin();iter != m_lstData.end();
            iter++)
        {
            uRetIndex ++;
            bFound = false;

            CComVariant var = (*iter);
            switch(vt&var.vt)
            {
            case VT_UINT:
                if (keyData.uintVal == var.uintVal)
                    bFound = true;
                break;
            case VT_I4:
                if (keyData.lVal == var.lVal)
                    bFound = true;
                break;
            case VT_BSTR:
                if (std::wstring(keyData.bstrVal)
                    == std::wstring(var.bstrVal))
                    bFound = true;
                break;
            default:
                break;
            }

            if (bFound)
            {
                break;
            }
        }

        uRetIndex = bFound ? uRetIndex : -1;        //-1 stand for not be found.
        return uRetIndex;                           //return the index of found data.
    }

    /*
    * Implement the virtual method to get the count of a simple data.Return the count of the data.
    */
    virtual int findCount(const VALUE & data)
    {
        int uRetCount = 0;
        std::vector<_TYPE>::iterator iter;
        BOOL bFound = false;
        //AutoLock lock(&m_lock);

        for (iter = m_lstData.begin();iter != m_lstData.end();
            iter++)
        {
            bFound = false;
            CComVariant var = (*iter);
            switch(vt&var.vt)
            {
            case VT_UINT:
                if (data.uintVal == var.uintVal)
                    bFound = true;
                break;
            case VT_I4:
                if (data.lVal == var.lVal)
                    bFound = true;
                break;
            case VT_BSTR:
                if (std::wstring(data.bstrVal)
                    == std::wstring(var.bstrVal))
                    bFound = true;
                break;
            default:
                break;
            }

            if (bFound)
            {
                uRetCount ++;
            }
        }

        return uRetCount;
    }

    /*
    *
    */
    virtual HRESULT GetEventParam(_TYPE v, VARIANT* varParam)
    {
        HRESULT hr = S_OK;

        if (varParam == NULL)
        {
            return E_INVALIDARG;
        }

        if (m_parentObj)
        {
            varParam[0].vt = VT_DISPATCH;
            hr = m_parentObj->QueryInterface(IID_IDispatch,(void**)&varParam[0].pdispVal);  //参数2 parent.
            if (FAILED(hr) || varParam[0].pdispVal == NULL)
            {
                return hr;
            }
        }
        else
        {
            varParam[0].vt = VT_EMPTY;
        }

        CComVariant var;
        varParam[1].vt = vt;
        switch (vt)
        {
        case VT_I4:
            varParam[1].lVal = v;
            break;
        case VT_UI4:
            varParam[1].uiVal = v;
            break;
        case VT_INT:
            varParam[1].intVal = v;
            break;
        case VT_UINT:
            varParam[1].uintVal = v;
            break;
        case VT_BSTR: /* OLE char* */
            var = v;
            varParam[1].bstrVal = SysAllocString(var.bstrVal);
            break;
        case VT_EMPTY:
        default:        
            break;
        }
        
        return hr;
    }

    /*
    * Convert the simple data list to a standard list.
    */
    void toStdVector(std::vector<_TYPE>& lstData)
    {
        //AutoLock lock(&m_lock);

        lstData.clear();
        lstData = m_lstData;
    }

    /*
    * Fill the simple data list with a standard list.
    */
    void fromStdVector(std::vector<_TYPE>& lstData)
    {
        //AutoLock lock(&m_lock);

        m_lstData.clear();
        m_lstData = lstData;
    }
};

/**
* @class TangDispatchDataArray
* @brief TangDispatchDataArray is a template class to dispatch the simple IDispatch data
* list to Java script.It inherits from TangDataArray.
* this dispatch list support complex data.
* This template implements an IUnknown and an IDispatch interface.
* 
* @author yueming.zou
* @version 1.0.0
*/
class TangDispatchDataArray : public TangDataArray< IDispatch *, VT_DISPATCH >
{
public:

	typedef IDispatch *	_TYPE;

	TangDispatchDataArray() : TangDataArray< IDispatch *, VT_DISPATCH >() {}

    /*
    *
    */
	virtual HRESULT GetEventParam(_TYPE v, VARIANT* pParams)
	{
		HRESULT hr = S_OK;
		ARG_CHECK( pParams );        
		if ( m_parentObj )
		{
			pParams[0].vt = VT_DISPATCH;
			pParams[0].pdispVal = NULL;
			hr = m_parentObj->QueryInterface(IID_IDispatch,(void**)&pParams[0].pdispVal);  //参数2 parent.
			HR_CHECK(hr);
			CHECK_IF_NULL( pParams[0].pdispVal  );
		}
		else
		{
			VariantInit( &(pParams[0]) );
		}
		if ( v )
		{
			pParams[1].vt = VT_DISPATCH;
			pParams[1].pdispVal = NULL;
			hr = v->QueryInterface(IID_IDispatch,(void**)&pParams[1].pdispVal);  //参数1 data object.
			HR_CHECK( hr );
			CHECK_IF_NULL( pParams[1].pdispVal );
		}
		else
		{
			VariantInit( &(pParams[1]) );
		}
exit:
		if ( FAILED(hr) )
		{
			LOG_INFO(L"TangDispatchDataArray::GetEventParam exit with error: 0x%08X",hr);
		}
        return hr;
    }
};

/**
* @class DataWithActionDisptchImpl
* @brief DataWithActionDisptchImpl is a template class to dispatch the data with native APIs to Java script.
* This template implements an IUnknown and an IDispatch interface.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
template<class T,class I_TYPE> class DataWithActionDisptchImpl :
    public I_TYPE
{
    /*
    *   class members for action dispatch.
    */
protected:
    /*
    * The internal COM reference counter, used by the IUnknown implementation.
    */
	long			        m_COMReferenceCounter;
    /*
    *
    */
    bool					m_CannotSelfDelete;
    /*
    *
    */
    bool					m_CannotPropertyPut;
    /*
    * This object's TypeInfo, used by the IDispatch implementation.
    */
    ITypeInfo*				m_TypeInfo;
    /*
    *
    */
    TCHAR                   m_fileName[MAX_PATH];
    /*
    *
    */
    bool					m_DonotLogParam;

    /*
    *   class members for data dispatch.
    */
public:
    /*
    *
    */
    std::map<KEY, VALUE>            m_properties;
    /*
    * event include on{Key}Changed
    */
    std::map<KEY, JSEVENTHANDLER>   m_events;
    /*
    * event include event object
    */
    std::map<KEY, IDispatch*>   m_eventObjects;
    /*
    *
    */
    std::map<DISPID, KEY>           m_actionNamesMap;
    /*
    *
    */
    std::map<KEY, DISPID>           m_actionIDsMap;
    /*
    *
    */
    TangDataEvent                   m_dataEvent;
    /*
    *
    */
    IDispatch*                      m_parentObj;

public:
    /*
    * Constructor
    */
	DataWithActionDisptchImpl(
		BSTR strModuleName, 
		BOOL bUseTypeLibFile = TRUE, 
		IDispatch *pIEInstance = NULL)
		:m_CannotSelfDelete( false ),
		m_DonotLogParam( false ),
		m_CannotPropertyPut( false ),
		m_COMReferenceCounter( 1 ),
		m_parentObj( NULL ),
		m_TypeInfo( NULL )
	{
        m_events.clear();
        m_eventObjects.clear();
        std::wstring wstrPathName;

        //build dispatch property map.
        m_actionNamesMap[DISPID_PROPERTY_PARENT] = std::wstring(DISP_PROPERTY_KEY_PARENT);
        m_actionIDsMap[std::wstring(DISP_PROPERTY_KEY_PARENT)] = DISPID_PROPERTY_PARENT;

        //add common onPropertyChanged event into event map.
        m_actionNamesMap[DISPID_DATA_EVENT_PROPERTYCHANGE] = std::wstring(DISP_DATA_EVENT_KEY_PROPERTYCHANGE);
        m_actionIDsMap[std::wstring(DISP_DATA_EVENT_KEY_PROPERTYCHANGE)] = DISPID_DATA_EVENT_PROPERTYCHANGE;

        //add custom property/method/event into map.
        ((T*)this)->__GetPropertyMap();

        //load action type lib.
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
                LOG_ERR(
                    L"NativeActionDispImpl: get module handle with error: 0x%08X",
                    HRESULT_FROM_WIN32(::GetLastError()));
            }
            else
            {
                ::GetModuleFileName(hMudle, m_fileName, MAX_PATH);
            }
        }

        HRESULT hr = Init();
        if (FAILED(hr))
        {
            LOG_ERR(L"DataWithActionDisptchImpl: Init with error: 0x%08X",hr);
        }
    }

	DataWithActionDisptchImpl()
		:m_CannotSelfDelete( false ),
		m_DonotLogParam( false ),
		m_CannotPropertyPut( false ),
		m_COMReferenceCounter( 1 ),
		m_parentObj( NULL ),
		m_TypeInfo( NULL )
	{
        //build dispatch property map.
        m_actionNamesMap[DISPID_PROPERTY_PARENT] = std::wstring(DISP_PROPERTY_KEY_PARENT);
        m_actionIDsMap[std::wstring(DISP_PROPERTY_KEY_PARENT)] = DISPID_PROPERTY_PARENT;

        //add common onPropertyChanged event into event map.
        m_actionNamesMap[DISPID_DATA_EVENT_PROPERTYCHANGE] = std::wstring(DISP_DATA_EVENT_KEY_PROPERTYCHANGE);
        m_actionIDsMap[std::wstring(DISP_DATA_EVENT_KEY_PROPERTYCHANGE)] = DISPID_DATA_EVENT_PROPERTYCHANGE;

        //add custom property/method/event into map.
        ((T*)this)->__GetPropertyMap();

        //load action type lib.
        ::GetModuleFileName(NULL, m_fileName, MAX_PATH);
        HRESULT hr = Init();
        if (FAILED(hr))
        {
            LOG_ERR(L"NativeActionDispImpl: Init with error: 0x%08X",hr);
        }
    }

    /*
    * Destructor
    */
    virtual ~DataWithActionDisptchImpl()
    {
        //recycle event handler bstr resource.
        std::map<KEY, JSEVENTHANDLER>::iterator iter;
        iter = m_events.begin();
        while (iter != m_events.end())
        {
            SYSFREESTRING(iter->second);
            iter = m_events.erase(iter);
        }

        //release event objects
        std::map<KEY, IDispatch*>::iterator it;
        it = m_eventObjects.begin();
        while(it != m_eventObjects.end())
        {
            it->second->Release();
            it = m_eventObjects.erase(it);
        }

        //release type lib.
        if (m_TypeInfo != NULL)
        {
            m_TypeInfo->Release();
        }
		//SYSFREEBSTR std::map<KEY, VALUE>    m_properties;
		std::map<KEY,VALUE>::iterator itp = m_properties.begin();
		while( itp != m_properties.end() )
		{
			VariantClear( &(itp->second) );
			itp = m_properties.erase(itp);
		}
        OutputDebugString(L"DataWithActionDisptchImpl destruction.");
		LOG_INFO(L"DataWithActionDisptchImpl destruction.");
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
                hr = typeLib->GetTypeInfoOfGuid(__uuidof(I_TYPE),&m_TypeInfo);
                if (FAILED(hr))
                {
                    LOG_ERR(
                        L"DataWithActionDisptchImpl: Get type info \
                         of guid with error: 0x%08X",
                         hr);
                }
                typeLib->Release();
            }
        }
        else
        {
            LOG_ERR(
                L"DataWithActionDisptchImpl: Load type library with error: 0x%08X",
                hr);
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
            riid == __uuidof(I_TYPE))
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
		if( --m_COMReferenceCounter <= 0 )
		{
			if(!m_CannotSelfDelete)
			{
				OnFinalRelease();
				delete this;
			}
			return 0;
		}
		else
		{
			return m_COMReferenceCounter;
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
		HRESULT hr = S_OK;
		DWORD j = 0;
		std::map<std::wstring, DISPID>::iterator it;
		for( ; j<cNames;j++ ) 
		{
			it = m_actionIDsMap.find(rgszNames[j]);
			if (it == m_actionIDsMap.end())
			{
				//if not existing in action id map. look for the type lib.
// 				LOG_INFO(L"Dispatch invoke native function:%s.",*rgszNames);
				hr =  DispGetIDsOfNames(m_TypeInfo, rgszNames, cNames, rgDispId);
				goto exit;
			}
			else
			{
				rgDispId[j]=it->second;
			}
		}
exit:
		if ( FAILED( hr ) )
		{
			LOG_ERR(L"DataWithActionDisptchImpl: GetIDsOfNames exit with error: 0x%08X",	hr);
		}
		return hr;
    }

    /*
    * IDispatch::Invoke() implementation
    */
    STDMETHODIMP Invoke(
        DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, 
        DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, 
        UINT* puArgErr)
    {
        HRESULT hr = S_OK;

		switch( wFlags )
		{
		case DISPATCH_PROPERTYGET:
			{
				if ( NULL != pVarResult )
				{
					pVarResult->vt = VT_DISPATCH;
					pVarResult->pdispVal = NULL;
				}
				//find the dispatch id key from key map.
				std::map<DISPID, KEY>::iterator it = m_actionNamesMap.find(dispidMember);
				if( it == m_actionNamesMap.end())
				{
					return ERROR_ACCESS_DENIED;
				}

				LOG_INFO(L"Get property,the property is %s.",it->second.c_str());
				if ( dispidMember == DISPID_PROPERTY_PARENT ) //property get parent object.
				{
					if ( m_parentObj && pVarResult )
					{
						hr = m_parentObj->QueryInterface(IID_IDispatch,
							(void**)pVarResult->pdispVal);
						HR_CHECK( hr );
					}
				}
				else if ( !it->second.empty() && pVarResult )   //property get named variables
				{
					hr = GetPropertyValue(it->second,*pVarResult);
					HR_CHECK(hr);
				}
				break; 
			}
		case DISPATCH_PROPERTYPUT:
			{
				//find the dispatch id key from key map.
				std::map<DISPID, KEY>::iterator it = m_actionNamesMap.find(dispidMember);
				if( it == m_actionNamesMap.end() ) 
					return ERROR_ACCESS_DENIED;
				const KEY & fn = it->second;
				//put event handler.
				if ( (!(fn.empty())) && dispidMember>=100
					&& NULL != pDispParams
					&& pDispParams->rgvarg->vt == VT_BSTR )
				{
					//to avoid user set an event more than once.
					std::map<KEY, JSEVENTHANDLER>::iterator iter;
					iter = m_events.find(fn);
					if ( iter != m_events.end() )
					{
						SYSFREESTRING(iter->second);
						iter->second = NULL;
					}

					//put the event handler.
					m_events[fn] = ::SysAllocString(pDispParams->rgvarg->bstrVal);

				}
				else if ( !m_CannotPropertyPut && NULL != pDispParams )  // property put of named variables
				{
					hr = SetPropertyValue(fn, (*pDispParams->rgvarg));
					HR_CHECK( hr );
				}

				break;
			}
		case (DISPATCH_PROPERTYPUT|DISPATCH_PROPERTYPUTREF): //property put dispatch reference.
			{
				//find the dispatch id key from key map.
				std::map<DISPID, KEY>::iterator it = m_actionNamesMap.find(dispidMember);
				if( it == m_actionNamesMap.end() ) 
					return ERROR_ACCESS_DENIED;
				const KEY & fn = it->second;
				//put event object.
				if ( (!(fn.empty())) && dispidMember>=100
					&& NULL != pDispParams
					&& pDispParams->rgvarg->vt == VT_DISPATCH )
				{
					//to avoid user set an event more than once.
					std::map<KEY, IDispatch*>::iterator iter;
					iter = m_eventObjects.find(fn);
					if (iter != m_eventObjects.end())
					{
						//release the counter of event object.
						iter->second->Release();
						iter->second = NULL;
					}
					//put the event object.
					IDispatch* pEventDispatch  = NULL;
					hr = pDispParams->rgvarg->pdispVal->QueryInterface(IID_IDispatch,(void**)&pEventDispatch);
					HR_CHECK( hr );
					CHECK_IF_NULL( pEventDispatch );
					m_eventObjects[fn] = pEventDispatch;
				}
				break;
			}
		case DISPATCH_METHOD:
		default:
			{
				//Log parameters for all APIs.
				if ( !m_DonotLogParam 
					&& NULL != pDispParams )
				{
					LogInvokeParameters(pDispParams);
				}

				hr = DispInvoke(this, m_TypeInfo, dispidMember, wFlags,
					pDispParams, pVarResult, pExcepInfo, puArgErr);
 				if ( NULL != pVarResult && pVarResult->vt == VT_EMPTY )
 				{
 					pVarResult->vt =  VT_INT;
 					pVarResult->intVal =HResult_To_App_Layer(hr);
 				}
				break;
			}
		}
exit:
		if ( FAILED(hr) )
		{
			LOG_ERR(L"DataWithActionDisptchImpl::Invoke exit with error: 0x%08X",hr);
		}
		return hr;
    }

    /*
    *
    */
    void LogInvokeParameters(DISPPARAMS* pDispParams)
    {
#if _DEBUG
        for (int i=(pDispParams->cArgs-1); i>=0; i--)
        {
            int uParamNum = pDispParams->cArgs-i;
            switch (pDispParams->rgvarg[i].vt)
            {
            case VT_BOOL:
                LOG_DEBUG(L"Param %d VT:VT_BOOL Value:%d",uParamNum,pDispParams->rgvarg[i].boolVal);
                break;
            case VT_INT:
            case VT_I4:
                LOG_DEBUG(L"Param %d VT:VT_INT Value:%d",uParamNum,pDispParams->rgvarg[i].lVal);
                break;
            case VT_UI4:
            case VT_UINT:
                LOG_DEBUG(L"Param %d VT:VT_UINT Value:%u",uParamNum,pDispParams->rgvarg[i].uintVal);
                break;
            case VT_UI8:
                LOG_DEBUG(L"Param %d VT:VT_UI8 Value:%u",uParamNum,pDispParams->rgvarg[i].ullVal);
                break;
            case VT_R4:
            case VT_I8:
            case VT_R8:
            case VT_CY:
            case VT_DATE:
                LOG_DEBUG(L"Param %d VT:VT_I8 Value:%d",uParamNum,pDispParams->rgvarg[i].llVal);
                break;
            case VT_UNKNOWN:
                LOG_DEBUG(L"Param %d VT:VT_UNKNOWN Value:%d",uParamNum,pDispParams->rgvarg[i].punkVal);
                break;
            case VT_DISPATCH:
                LOG_DEBUG(L"Param %d VT:VT_DISPATCH Value:%d",uParamNum,pDispParams->rgvarg[i].pdispVal);
                break;
            case VT_BSTR:
                LOG_DEBUG(L"Param %d VT:VT_BSTR Value:%s",uParamNum,pDispParams->rgvarg[i].bstrVal);
                break;
            case VT_EMPTY:
                LOG_DEBUG(L"Param %d VT:VT_EMPTY",uParamNum);
                break;
            default:
                break;
            }
        }
#endif
    }

    /*
    * Provide API to disable log parameters for invoked API.
    */
    void DonotLogParameters()
    {
        m_DonotLogParam = true;
    }

    /*
    * Method to disable self delete when ref equal zero.
    */
    void DisableSelfDelete()
    {
        m_CannotSelfDelete = true;
    }

    /*
    * Method to disable property put in Java script.
    */
    void DisablePropertyPut()
    {
        m_CannotPropertyPut = true;
    }

    /*
    * Method to do final release job before self delete.
    */
    virtual void OnFinalRelease()
    {
    }

    /*
    * Method to set specific property value.
    */
    HRESULT SetPropertyValue(const KEY & vn,const VALUE & v)
    {
		HRESULT hr = S_OK;
		//clear old memory if m_properties[vn] is not empty.
		std::map<KEY, VALUE>::iterator it = m_properties.find( vn );
		VALUE * pTempV = (VALUE*)(&v);
		if ( it != m_properties.end() )
		{
			VariantClear( &(it->second) );
		}
		VariantInit( &(m_properties[vn]) );
		hr = VariantCopy( &(m_properties[vn]),pTempV );
		HR_CHECK( hr );
exit:
		if ( FAILED(hr) )
		{
			LOG_ERR(L"DataWithActionDisptchImpl::SetPropertyValue exit with error: 0x%08X",hr);
		}
		return hr;
    }

    /*
    * Method to get specific property value.
    */
    HRESULT GetPropertyValue(const KEY & vn, VALUE& v)
    {
		HRESULT hr = 0;
		std::map<KEY, VALUE>::iterator it = m_properties.find( vn );
		if ( it == m_properties.end() )
		{
			return E_FAIL;
		}
		VariantInit( &v );
		hr = VariantCopy( &v,&(it->second));
		return hr;
    }

    /*
    * Initialize event notify object in this function.
    */
    HRESULT InitEventSink(IDispatch* pIEInstance)
    {
        return m_dataEvent.Init(pIEInstance);
    }

    /*
    * Update property value and fire relational event.
    */
    HRESULT UpdatePropertyValue(const KEY & vn,const VALUE & v)
    {
        HRESULT hr = S_OK;
		wchar_t szOnEventName[256] = {0};
		VARIANT varOld;
		std::map<KEY, JSEVENTHANDLER>::iterator iter;
        VALUE * pTempV = (VALUE*)(&v);
		VariantInit( &varOld );

		std::map<KEY, VALUE>::iterator it = m_properties.find( vn );
		if ( it != m_properties.end() )
		{
			//get the old value as event argument.
			hr = VariantCopy( &varOld,&(it->second) );
			HR_CHECK( hr );
			//release the old property.
			VariantClear( &(it->second) );
		}
		//update property to new value.
		VariantInit( &(m_properties[vn]) );
		hr = VariantCopy( &(m_properties[vn]),pTempV );
		HR_CHECK(hr);

		//if single property event had registered, fire the event.
		wsprintf(szOnEventName,L"on%sChanged",vn.c_str());
		iter = m_events.find(std::wstring(szOnEventName));
		if (iter != m_events.end())
		{
#ifndef ITEM_EVENT_ARGS_NUM
#define ITEM_EVENT_ARGS_NUM	(3)
#endif
			VARIANT varParam[ITEM_EVENT_ARGS_NUM] = {0};
			for (int i = 0; i < ITEM_EVENT_ARGS_NUM; i++)
			{
				VariantInit(&varParam[i]);
			}
			varParam[0].vt = VT_DISPATCH;           //第三个参数 data
			hr = this->QueryInterface(IID_IDispatch,(void**)&varParam[0].pdispVal);
			HR_CHECK(hr);

			hr = VariantCopy(&varParam[1],pTempV);		//第二个参数 new value
			HR_CHECK(hr);

			hr = VariantCopy(&varParam[2],&varOld);		//第一个参数 old value
			HR_CHECK(hr);

			//fire dispatch data event to Java script.
			hr = m_dataEvent.Fire(
				iter->second,
				varParam,
				ITEM_EVENT_ARGS_NUM,
				NULL);
 			VariantClear(&varOld);
// 			for ( size_t i= 0; i< ITEM_EVENT_ARGS_NUM; i++ )
// 			{
// 				VariantClear( &(varParam[i]) );
// 			}
        }
        else
        {
            //search event object in event object map.
             std::map<KEY, IDispatch*>::iterator it;
			it = m_eventObjects.find(std::wstring(szOnEventName));
             if (it != m_eventObjects.end() && it->second != NULL)
             {
 #ifndef ITEM_EVENT_ARGS_NUM
 #define ITEM_EVENT_ARGS_NUM	(3)
 #endif
				VARIANT varParam[ITEM_EVENT_ARGS_NUM] = {0};
				for (int i = 0; i < ITEM_EVENT_ARGS_NUM; i++)
				{
					VariantInit(&varParam[i]);
				}
				varParam[0].vt = VT_DISPATCH;           //第三个参数 data
				hr = this->QueryInterface(IID_IDispatch,(void**)&varParam[0].pdispVal);
				HR_CHECK(hr);

				hr = VariantCopy(&varParam[1],pTempV);		//第二个参数 new value
				HR_CHECK(hr);

				hr = VariantCopy(&varParam[2],&varOld);		//第一个参数 old value
				HR_CHECK(hr);

				//fire the event object.

				//no need to specific method name. dispid=0 for anonymous JavaScript function.
				{
					UINT errArg = 0;
					DISPID dispid = 0;
					VARIANT varRet;
					DISPPARAMS dParams = { varParam, 0, ITEM_EVENT_ARGS_NUM, 0 };
					VariantInit( &varRet );
					hr = it->second->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
						DISPATCH_METHOD, &dParams, &varRet, 0, &errArg);
					/*for ( size_t i= 0; i< ITEM_EVENT_ARGS_NUM; i++ )
					{
						VariantClear( &(varParam[i]) );
					}*/
					HR_CHECK( hr );
				}
             }

            //
            //TOD: Is we need to fire the onpropertychanged event;
            //
        }

        //if single property event not fired, fire onpropertychanged event.
        iter = m_events.begin();
        iter = m_events.find(std::wstring(L"onPropertyChanged"));
        if (iter != m_events.end())
        {
#ifndef PROPERTY_EVENT_ARGS_NUM
#define PROPERTY_EVENT_ARGS_NUM	(4)
#endif
			VARIANT varParam[PROPERTY_EVENT_ARGS_NUM] = {0};
			for (int i = 0; i < ITEM_EVENT_ARGS_NUM; i++)
			{
				VariantInit(&varParam[i]);
			}

			varParam[0].vt = VT_DISPATCH;           //第四个参数 data
			varParam[0].pdispVal = NULL;
			hr = this->QueryInterface(IID_IDispatch,(void**)&varParam[0].pdispVal);		
			HR_CHECK( hr );
			CHECK_IF_NULL( varParam[0].pdispVal );
			hr = VariantCopy(&varParam[1],pTempV);		//第三个参数 new value
			HR_CHECK(hr);
			hr = VariantCopy(&varParam[2],&varOld);  //第二个参数 old value
			HR_CHECK(hr);
			varParam[3].vt = VT_BSTR;               //第一个参数 property name
			varParam[3].bstrVal = NULL;
			varParam[3].bstrVal = SysAllocString(vn.c_str());
			ALLOC_CHECK( varParam[3].bstrVal );
			//fire dispatch data event to Java script.
			hr = m_dataEvent.Fire(
				iter->second,
				varParam,
				PROPERTY_EVENT_ARGS_NUM,
				NULL);
 			VariantClear(&varOld);
// 			for ( size_t i= 0; i< PROPERTY_EVENT_ARGS_NUM; i++ )
// 			{
// 				VariantClear( &(varParam[i]) );
// 			}
            HR_CHECK(hr);
        }
    exit:
        if (FAILED(hr))
        {
			LOG_ERR(L"DataWithActionDisptchImpl::UpdatePropertyValue exit with error: 0x%08X",hr);
        }
        else
        {
            switch(v.vt)
            {
            case VT_UINT:
                LOG_INFO(L"Fired the property update event to [%s] and newValue is [%u]", szOnEventName, v.uintVal);
                break;
            case VT_INT:
                LOG_INFO(L"Fired the property update event to [%s] and newValue is [%u]", szOnEventName, v.intVal);
                break;
            case VT_BOOL:
                LOG_INFO(L"Fired the property update event to [%s] and newValue is [%u]", szOnEventName, v.boolVal);
                break;
            case VT_BSTR:
                LOG_INFO(L"Fired the property update event to [%s] and newValue is [%s]", szOnEventName, v.bstrVal);
                break;

            }
        }
        return hr;
    }

    /*
    * Compare property value within dispatch data object.
    */
    BOOL CompareProperty(const KEY & key, const VALUE & propertyData)
    {
        std::map<KEY, VALUE>::iterator it;
        it = m_properties.find(key);
        if (it != m_properties.end())         //compare the value of key.
        {
            //found this key compare the data type first.
            if (propertyData.vt != it->second.vt)
            {
                LOG_INFO(L"CompareProperty:Data type(vt) is different.");
                switch(propertyData.vt)
                {
                case VT_R8:
                    if (it->second.vt == VT_UINT
                        || it->second.vt == VT_UI4)
                    {
                        UINT32 cvalue = (UINT32)propertyData.dblVal;
                        if (cvalue == it->second.uintVal)
                        {
                            return true;
                        }
                    }else if (it->second.vt == VT_I4
                        || it->second.vt == VT_INT)
                    {
                        int cvalue = (int)propertyData.dblVal;
                        if (cvalue == it->second.lVal)
                        {
                            return true;
                        }
                    }
                case VT_I4:
                    if (it->second.vt == VT_UINT
                        || it->second.vt == VT_UI4)
                    {
                        UINT32 cvalue = (UINT32)propertyData.lVal;
                        if (cvalue == it->second.uintVal)
                        {
                            return true;
                        }
                    }
                case VT_BSTR:
                    //TODO: convert the bstr to vt type then compare.
                    return false;
                default:
                    return false;
                }

                return false;
            }

            //then compare the value.
            LONGLONG compareValue = propertyData.llVal;
            switch(it->second.vt)
            {
            case VT_BOOL:
                if (!(it->second.boolVal) == !(propertyData.boolVal))
                    return true;
                return false;
            case VT_INT:
            case VT_I4:
                if (it->second.lVal == (int)compareValue)
                    return true;
                return false;
            case VT_UI4:
            case VT_UINT:
                if (it->second.uintVal == (UINT32)compareValue)
                    return true;
                return false;
            case VT_UI8:
                return false;
            case VT_R4:
            case VT_I8:
            case VT_R8:
            case VT_CY:
            case VT_DATE:
                if ((propertyData.dblVal + 0.001 > it->second.dblVal) && 
                    (propertyData.dblVal - 0.001 < it->second.dblVal))
                    return true;
                return false;
            case VT_UNKNOWN:
                if (propertyData.punkVal == it->second.punkVal)
                    return true;
                return false;
            case VT_DISPATCH:
                if (propertyData.pdispVal == it->second.pdispVal)
                    return true;
                return false;
            case VT_BSTR:
                if (std::wstring(propertyData.bstrVal)
                    == std::wstring(it->second.bstrVal))
                    return true;
                return false;
            case VT_ERROR:
            case VT_EMPTY:
            default:
                return false;
            }
        }

        LOG_ERR(L"DataDispatchImpl: compare property, dispatch data doesn't have this property.");
        return false;
    }
};


/**
* @class TangDataMap
* @brief TangDataMap is a template class to dispatch the data map to Java script.
* this dispatch map support complex and simple type data.
* This template implements an IUnknown and an IDispatch interface.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
//template <class _KEY_TYPE,class _DATA_TYPE, VARTYPE vt>
//class TangDataMap: public IDispatch
//{
//protected:
//    /*
//    * The internal COM reference counter, used by the IUnknown implementation.
//    */
//    unsigned long			m_COMReferenceCounter;
//    /*
//    *
//    */
//    bool					m_CannotSelfDelete;
//    /*
//    *
//    */
//    bool					m_CannotPropertyPut;
//    /*
//    *
//    */
//    std::map<_KEY_TYPE,_DATA_TYPE>      m_mapData;
//    /*
//    * event include onAdded, onRemove,onUpdate for data array.
//    */
//    std::map<KEY, JSEVENTHANDLER>   m_events;
//    /*
//    *
//    */
//    std::map<DISPID, KEY>           m_actionNamesMap;
//    /*
//    *
//    */
//    std::map<KEY, DISPID>           m_actionIDsMap;
//    /*
//    *
//    */
//    TangDataEvent                   m_dataEvent;
//    /*
//    *
//    */
//    IDispatch*                      m_parentObj;
//    /*
//    *
//    */
//    CRITICAL_SECTION                m_lock;
//
//public:
//    /*
//    * Constructor
//    */
//    TangDataMap()
//    {
//        ::InitializeCriticalSection(&m_lock);
//        m_COMReferenceCounter = 1;
//        m_CannotPropertyPut = false;
//        m_CannotSelfDelete = false;
//        m_parentObj = NULL;
//
//        //build dispatch event map.
//        m_actionNamesMap[DISPID_ARRAY_EVENT_ADD] = std::wstring(DISP_ARRAY_EVENT_KEY_ADD);
//        m_actionIDsMap[std::wstring(DISP_ARRAY_EVENT_KEY_ADD)] = DISPID_ARRAY_EVENT_ADD;
//        m_actionNamesMap[DISPID_ARRAY_EVENT_REMOVE] = std::wstring(DISP_ARRAY_EVENT_KEY_REMOVE);
//        m_actionIDsMap[std::wstring(DISP_ARRAY_EVENT_KEY_REMOVE)] = DISPID_ARRAY_EVENT_REMOVE;
//        m_actionNamesMap[DISPID_ARRAY_EVENT_UPDATE] = std::wstring(DISP_ARRAY_EVENT_KEY_UPDATE);
//        m_actionIDsMap[std::wstring(DISP_ARRAY_EVENT_KEY_UPDATE)] = DISPID_ARRAY_EVENT_UPDATE;
//
//        //build dispatch method map.
//        m_actionNamesMap[DISPID_ARRAY_METHOD_AT] = std::wstring(DISP_ARRAY_METHOD_KEY_AT);
//        m_actionIDsMap[std::wstring(DISP_ARRAY_METHOD_KEY_AT)] = DISPID_ARRAY_METHOD_AT;
//        m_actionNamesMap[DISPID_ARRAY_METHOD_SORT] = std::wstring(DISP_ARRAY_METHOD_KEY_SORT);
//        m_actionIDsMap[std::wstring(DISP_ARRAY_METHOD_KEY_SORT)] = DISPID_ARRAY_METHOD_SORT;
//        m_actionNamesMap[DISPID_ARRAY_METHOD_FIND] = std::wstring(DISP_ARRAY_METHOD_KEY_FIND);
//        m_actionIDsMap[std::wstring(DISP_ARRAY_METHOD_KEY_FIND)] = DISPID_ARRAY_METHOD_FIND;
//        m_actionNamesMap[DISPID_ARRAY_METHOD_COUNT] = std::wstring(DISP_ARRAY_METHOD_KEY_COUNT);
//        m_actionIDsMap[std::wstring(DISP_ARRAY_METHOD_KEY_COUNT)] = DISPID_ARRAY_METHOD_COUNT;
//
//        //build dispatch property map.
//        m_actionNamesMap[DISPID_PROPERTY_PARENT] = std::wstring(DISP_PROPERTY_KEY_PARENT);
//        m_actionIDsMap[std::wstring(DISP_PROPERTY_KEY_PARENT)] = DISPID_PROPERTY_PARENT;
//        m_actionNamesMap[DISPID_ARRAY_PROPERTY_SIZE] = std::wstring(DISP_ARRAY_PROPERTY_KEY_SIZE);
//        m_actionIDsMap[std::wstring(DISP_ARRAY_PROPERTY_KEY_SIZE)] = DISPID_ARRAY_PROPERTY_SIZE;
//    }
//
//    /*
//    * Destructor
//    */
//    ~TangDataMap()
//    {
//        ::DeleteCriticalSection(&m_lock);
//
//        if (vt == VT_BSTR)
//        {
//            //TODO: free bstr data.
//        }
//
//        if (m_parentObj)
//        {
//            m_parentObj->Release();
//            m_parentObj = NULL;
//        }
//
//        std::map<KEY, JSEVENTHANDLER>::iterator iter = m_events.begin();
//        while (iter != m_events.end())
//        {
//            SYSFREESTRING(iter->second);
//            iter++;
//        }
//
//        OutputDebugString(L"TangDataArray destruction.");
//    }
//
//    /*
//    * Virtual method to get list data by index.
//    */
//    virtual HRESULT at(int index, VALUE* v)
//    {
//        index;
//        v;
//        return E_NOTIMPL;
//    }
//
//    /*
//    * Virtual method to append data node into dispatch list, then fire OnAdd event.
//    */
//    virtual HRESULT push_back(_TYPE v)
//    {
//        HRESULT hr = S_OK;
//        size_t endPos = 0;
//        std::map<KEY, JSEVENTHANDLER>::iterator iter;
//        //AutoLock lock(&m_lock);
//
//        //push back the new item.
//        m_mapData.insert().push_back(v);
//
//        //if add event registered, fire the add event.
//        endPos = m_lstData.size() - 1;
//        iter = m_events.find(std::wstring(DISP_ARRAY_EVENT_KEY_ADD));
//        if (iter != m_events.end())
//        {
//#ifndef ADD_EVENT_ARGS_NUM
//#define ADD_EVENT_ARGS_NUM	(2)
//#endif
//            VARIANT varParam[ADD_EVENT_ARGS_NUM];
//            hr = GetEventParam(v,varParam);
//            if (FAILED(hr))
//            {
//                LOG_ERR(L"TangDataArray: push_back get event \
//                         parameter with error: 0x%08X", hr);
//                return hr;
//            }
//
//            hr = m_dataEvent.Fire(
//                iter->second,
//                varParam,
//                ADD_EVENT_ARGS_NUM,
//                NULL);
//            if (FAILED(hr))
//            {
//                LOG_ERR(L"TangDataArray: push_back fire event \
//                         with error: 0x%08X", hr);
//                return hr;
//            }
//        }
//
//        return hr;
//    }
//
//    /*
//    * Virtual method to erase data node from dispatch list by index.
//    *   then fire onRemove event.
//    */
//    virtual HRESULT erase(int index)
//    {
//        HRESULT hr = S_OK;
//        std::map<KEY, JSEVENTHANDLER>::iterator iter;
//        std::vector<_TYPE>::iterator it;
//        //AutoLock lock(&m_lock);
//
//        if (index >= 0 && index < (int)m_lstData.size())
//        {
//            _TYPE data;
//            it = m_lstData.begin() + index;
//            if (it == m_lstData.end())
//                return E_INVALIDARG;
//
//            //erase the data.
//            data = (*it);
//            m_lstData.erase(it);
//
//            //if remove event registered, fire the remove event.
//            iter = m_events.find(std::wstring(DISP_ARRAY_EVENT_KEY_REMOVE));
//            if (iter != m_events.end())
//            {
//#ifndef REMOVE_EVENT_ARGS_NUM
//#define REMOVE_EVENT_ARGS_NUM	(2)
//#endif
//                VARIANT varParam[REMOVE_EVENT_ARGS_NUM];
//                hr = GetEventParam(data,varParam);
//                if (FAILED(hr))
//                {
//                    LOG_ERR(L"TangDataArray: erase with index get event \
//                             parameter with error: 0x%08X", hr);
//                    return hr;
//                }
//
//                hr = m_dataEvent.Fire(
//                    iter->second,
//                    varParam,
//                    REMOVE_EVENT_ARGS_NUM,
//                    NULL);
//                if (FAILED(hr))
//                {
//                    LOG_ERR(L"TangDataArray: erase with index fire event \
//                             with error: 0x%08X", hr);
//                    return hr;
//                }
//            }
//        }
//        else
//        {
//            hr = E_INVALIDARG;
//        }
//
//        return hr;
//    }
//
//    /*
//    * Virtual method to erase data node from dispatch list by property and property value.
//    *  and fire onRemove event.This methd is provide to complex dispatch object,
//    */
//    virtual HRESULT erase(KEY key, VALUE keyData)
//    {
//        HRESULT hr = S_OK;
//        _TYPE pData = NULL;
//
//        //find the data.
//        int index = find(key,keyData,&pData);
//        if (pData == NULL || index == -1)   //not found
//        {
//            //return if find failed.
//            return E_FAIL;
//        }
//
//        //fire remove event and do erase.
//        return erase(index);
//    }
//
//    /*
//    * Virtual method to erase data node from dispatch list by data value.
//    *  and fire onRemove event.This metohd is provide to simple dispatch object,
//    */
//    virtual HRESULT erase(VALUE keyData)
//    {
//        HRESULT hr = S_OK;
//
//        //find the data.
//        int index = find(keyData);
//        if (index == -1)   //not found
//        {
//            //return if find failed.
//            return E_FAIL;
//        }
//
//        //fire remove event and do erase.
//        return erase(index);
//    }
//
//    /*
//    * Virtual method to find out the data node from dispatch list which contains the specific
//    * property and equal the value.
//    * This method is provide to complex dispatch object,
//    */
//    virtual int find(KEY key, VALUE keyData, _TYPE* ppRet)
//    {
//        int uRetIndex = -1;
//        key;
//        keyData;
//        ppRet;
//        return uRetIndex;                           //return the index of found data.
//    }
//
//    /*
//    * Virtual method to find out the data node from dispatch list which equal the value.
//    * This method is provide to simple dispatch object,
//    */
//    virtual int find(VALUE keyData)
//    {
//        int uRetIndex = -1;
//        keyData;
//        return uRetIndex;                           //return the index of found data.
//    }
//
//    /*
//    * Virtual method to get the count of the data which equal the value.
//    * This method is provide to simple dispatch object,
//    */
//    virtual int findCount(VALUE data)
//    {
//        int uRetCount = 0;
//        data;
//        return uRetCount;                           //return the count of found data.
//    }
//
//    /*
//    * Virtual method to get the count of data list which contains the specific
//    * property and equal the value.
//    * This method is provide to complex dispatch object,
//    */
//    virtual int findCount(KEY key, VALUE keyData)
//    {
//        int uRetCount = 0;
//        key;
//        keyData;
//        return uRetCount;                           //return the count of found data.
//    }
//
//    /*
//    * Method to get the size of dispatch list.
//    */
//    virtual size_t size()
//    {
//        return m_lstData.size();
//    }
//
//    /*
//    * Method to clear the dispatch list.
//    */
//    virtual void clear()
//    {
//        m_lstData.clear();
//    }
//
//    /*
//    * Method to disable object delete itself.
//    */
//    void DisableSelfDelete()
//    {
//        m_CannotSelfDelete = true;
//    }
//
//    /*
//    * Method to disable dispatch object to do property put.
//    */
//    void DisablePropertyPut()
//    {
//        m_CannotPropertyPut = true;
//    }
//
//    /*
//    * Method to disable dispatch object to do property put.
//    */
//    virtual void OnFinalRelease()
//    {
//    }
//    /*
//    *
//    */
//    virtual HRESULT GetEventParam(_TYPE v, VARIANT* param)
//    {
//        param;
//        return E_NOTIMPL;
//    }
//
//    /*
//    * Method to initialize dispatch list event object.
//    */
//    HRESULT InitEventSink(IDispatch* pIEInstance)
//    {
//        return m_dataEvent.Init(pIEInstance);
//    }
//
//    /*
//    * IDispatch::GetTypeInfoCount() implementation
//    */
//    STDMETHODIMP GetTypeInfoCount(UINT* pctinfo)
//    {
//        if(pctinfo == NULL)
//        {
//            return E_POINTER;
//        }
//
//        *pctinfo = 1;
//        return S_OK;
//    }
//
//    /*
//    * IDispatch::GetTypeInfo() implementation
//    */
//    STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
//    {
//        if(ppTInfo == NULL)
//        {
//            return E_POINTER;
//        }
//
//        return S_OK;
//    }
//
//    /*
//    * IDispatch::GetIDsOfNames() implementation
//    */
//    STDMETHODIMP GetIDsOfNames(REFIID riid,
//        LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
//    {
//        DWORD j;
//        std::map<std::wstring, DISPID>::iterator it;
//        for(j=0; j<cNames;j++)
//        {
//            it = m_actionIDsMap.find(rgszNames[j]);
//            if (it == m_actionIDsMap.end())
//                return ERROR_INVALID_ACCESS;
//            rgDispId[j]=it->second;
//        }
//        return S_OK;
//    }
//
//    /*
//    * IDispatch::Invoke() implementation
//    */
//    STDMETHODIMP Invoke(
//        DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, 
//        DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, 
//        UINT* puArgErr)
//    {
//        HRESULT hr = S_OK;
//
//        std::map<DISPID, KEY>::iterator it = m_actionNamesMap.find(dispidMember);
//        if (it == m_actionNamesMap.end()) return ERROR_ACCESS_DENIED;
//        KEY fn = it->second;
//        switch(wFlags) {
//case DISPATCH_PROPERTYGET:
//    // property get of named variables
//    switch(dispidMember)
//    {
//    case DISPID_ARRAY_PROPERTY_SIZE:
//        pVarResult->vt = VT_I4;
//        pVarResult->lVal = size();
//        break;
//    case DISPID_PROPERTY_PARENT:
//        if (m_parentObj)
//        {
//            pVarResult->vt = VT_DISPATCH;
//            hr = m_parentObj->QueryInterface(IID_IDispatch,
//                (void**)pVarResult->pdispVal);
//            if (FAILED(hr))
//            {
//                return hr;
//            }
//        }
//        else
//        {
//            LOG_INFO(L"TangDataArray parent object is null.");
//            pVarResult->vt = VT_EMPTY;
//        }
//        break;
//    default:
//        break;
//    }
//    break; 
//case DISPATCH_PROPERTYPUT:
//    //  put event handler.
//    if (dispidMember>=100 && fn.length() > 0 
//        && pDispParams->rgvarg->vt == VT_BSTR)
//    {
//        m_events.insert(std::make_pair(fn,SysAllocString(pDispParams->rgvarg->bstrVal)));
//    }
//    else if (!m_CannotPropertyPut)      //property put of named variables
//    {
//        //TODO: property put.
//    }
//    break; 
//case DISPATCH_METHOD:
//default:
//    if (dispidMember == DISPID_ARRAY_METHOD_AT)
//    {
//        int index = pDispParams->rgvarg->intVal;
//        if (index >= 0 && pDispParams->cArgs == 1)
//        {
//            return this->at(index,pVarResult);
//        }
//
//        hr = E_INVALIDARG;
//    }else if (dispidMember == DISPID_ARRAY_METHOD_FIND)
//    {
//        int index = -1;
//        if (pDispParams->cArgs == 1)    //call simple dispatch list find(value) method.
//        {
//            index = this->find((*pDispParams->rgvarg));
//        }else if (pDispParams->cArgs == 2)
//        {
//            if (pDispParams->rgvarg[1].vt == VT_BSTR
//                && pDispParams->rgvarg[1].bstrVal)
//            {
//                //call complex dispatch list find(value) method.
//                _TYPE pData = NULL;
//                index = this->find(
//                    pDispParams->rgvarg[1].bstrVal,
//                    (*pDispParams->rgvarg),
//                    &pData);
//            }
//        }
//        if (pVarResult)
//        {
//            //return the index of data in this list.
//            this->at(index,pVarResult);
//        }
//    }else if(dispidMember == DISPID_ARRAY_METHOD_COUNT)
//    {
//        int count = 0;
//        if (pDispParams->cArgs == 1)    //call simple dispatch list findCount(value) method.
//        {
//            count = this->findCount((*pDispParams->rgvarg));
//        }else if (pDispParams->cArgs == 2)
//        {
//            if (pDispParams->rgvarg[1].vt == VT_BSTR
//                && pDispParams->rgvarg[1].bstrVal)
//            {
//                //call complex dispatch list findCount(value) method.
//                count = this->findCount(
//                    pDispParams->rgvarg[1].bstrVal,
//                    (*pDispParams->rgvarg));
//            }
//        }
//
//        if (pVarResult)
//        {
//            //return the count of data in this list.
//            pVarResult->vt = VT_I4;
//            pVarResult->lVal = count;
//        }
//    }
//
//    break;
//        }
//
//        return hr;
//    }
//
//    /*
//    * IUnknown::QueryInterface() implementation
//    */
//    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
//    {
//        if(ppvObject == NULL)
//        {
//            return E_POINTER;
//        }
//        else if(riid == IID_IUnknown || 
//            riid == IID_IDispatch)
//        {
//            *ppvObject = (void*) this;
//            AddRef();
//            return S_OK;
//        }
//        else
//        {
//            return E_NOINTERFACE;
//        }
//    }
//
//    /*
//    * IUnknown::AddRef() implementation
//    */
//    STDMETHODIMP_(ULONG) AddRef()
//    {
//        return ++m_COMReferenceCounter;
//    }
//
//    /*
//    * IUnknown::Release() implementation
//    */
//    STDMETHODIMP_(ULONG) Release()
//    {
//        if(m_COMReferenceCounter> 1)
//        {
//            return --m_COMReferenceCounter;
//        }
//        else
//        {
//            if(!m_CannotSelfDelete)
//            {
//                OnFinalRelease();
//                delete this;
//            }
//            return 0;
//        }
//    }
//};