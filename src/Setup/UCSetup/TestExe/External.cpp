/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file External.cpp
* @brief defined External Object to support browser call native function
* and native call script function.External provides the APIs of inserting,
* removing and getting the adaptor isntance for native implementation.
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

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include "External.h"

/**
* @brief the constructor of external class.
* @param 
* @return
*/
External::External()
{

}

/**
* @brief the dis constructor of external class.
* @param 
* @return
*/
External::~External()
{
    m_adaptorMap.clear();

	//daming: 2011-08-09 
	m_adaptorByTypeMap.clear();
}

/**
* @brief this is the get_property function.UI JS invoke this function
* to get the specific adapter from external.
* @param [in] name. the name of the adapter.
* @return IDispatch* return the pointer of adaptor if successful, otherwise return null.
*/
IDispatch * WINAPI External::get_ModuleAdaptor( BSTR bstrAdapter )
{
    IDispatch* pDisp = NULL;
    MODULE_ADAPTOR_MAP::iterator iter;

#ifdef _DEBUG
    //LOG_INFO(L"Adaptor %s",bstrAdapter);
#endif

	//::EnterCriticalSection(&m_adaptorMaplock);
    if (NULL == bstrAdapter)
    {
        goto exit;
    }

	iter = m_adaptorMap.find( _bstr_t( bstrAdapter ) );
	if ( iter == m_adaptorMap.end() )
	{
		//LOG_ERR(L"Can't find the adaptor:%s instance.",bstrAdapter);
		goto exit;
	}
	if( NULL != (*iter).second )
	{
		(*iter).second->QueryInterface(IID_IDispatch,(void**)&pDisp);
		//if ( NULL == pDisp )
			//LOG_ERR(L"get_ModuleAdaptor. query the specific adapter failed.");
	}

exit:
	//::LeaveCriticalSection(&m_adaptorMaplock);
    return pDisp;
}

/**
* @brief add the specific adapter into external to publish the API to UI JS code.
* @param [in] name. the name of the adapter will be added.
* @param [in] pDis. IDispatch pointer to the adapter object.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT External::PutPluginObject( BSTR bstrName,IDispatch* pDis )
{
	HRESULT     hr = S_OK;
	//MODULE_ADAPTOR_MAP::iterator iter;
	DISPATCH_MAP::iterator citer;

	//CHECK_IF_NULL(bstrName);
	//CHECK_IF_NULL(pDis);

	//Add dispatch instance into the adapter list.添加各服务的Adaptor到列表中
	//::EnterCriticalSection(&m_adaptorMaplock);
// 	iter = m_adaptorMap.find(_bstr_t( bstrName ) );
// 	if (iter != m_adaptorMap.end())
// 	{
// 		LOG_INFO(L"Adaptor:%s is already existing, replace with then new one.",bstrName);
// 		// ToDo SongGuoqiang need to release the disp pointer
// 		m_adaptorMap.erase(iter);
// 	}
	DeleteByTypeMap( 0,bstrName );
    pDis->AddRef();
	m_adaptorMap[ _bstr_t( bstrName ) ] = pDis;

	//::LeaveCriticalSection(&m_adaptorMaplock);

	//Add dispatch entry into entry map.添加自动化实例到map中
	//{ OLESTR(#member), dispid, DISPATCH_PROPERTYGET, vt, 0, { VTS_REF_DISPATCH }, VT_EMPTY, 
	//(void (__stdcall CExternal::*)())&get_##member }
	DispatchEntry<External>* pEntry = new DispatchEntry<External>();
	pEntry->bstrName = ::SysAllocString( bstrName );
	pEntry->wFlags = DISPATCH_PROPERTYGET;
	pEntry->vtReturn = VT_DISPATCH;
	pEntry->nArgs = 1;
	pEntry->vtSingle = VT_BSTR;
	pEntry->vtbstrArgs = ::SysAllocString ( bstrName );
	pEntry->dispid = (long)bstrName;
	pEntry->pfn = (void (__stdcall External::*)())&External::get_ModuleAdaptor;

	//insert DispatchEntry into dispatch entry map.
	//::EnterCriticalSection(&m_dispatchMapLock);
	citer = m_dispatchEntryMap.find(bstrName);
	if ( citer != m_dispatchEntryMap.end() )
	{
		SAFE_DELETE((*citer).second);
		m_dispatchEntryMap.erase(citer);
	}

	m_dispatchEntryMap[ _bstr_t( bstrName ) ] = pEntry;
	//::LeaveCriticalSection(&m_dispatchMapLock);

exit:
// 	if (FAILED(hr))
// 		//log the error.
// 		LOG_ERR(L"Add adapter object with error: 0x%08X", hr);
// 	else
// 		LOG_INFO(L"Add adapter object:%s successfully",bstrName);

	return hr;
}

/**
* @brief add the specific adapter into external to publish the API to UI JS code.
* @param [in] serviceType
* @param [in] name. the name of the adapter will be added.
* @param [in] pDis. IDispatch pointer to the adapter object.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
//daming: 2011-08-09 增加serviceType便于 ServiceList 可以按ID查询
HRESULT External::PutPluginObjectWithType( int serviceType, BSTR bstrName,IDispatch* pDis )
{
    HRESULT     hr = S_OK;
	//MODULE_ADAPTOR_MAP::iterator iter;
	DISPATCH_MAP::iterator citer;

    //CHECK_IF_NULL(bstrName);
    //CHECK_IF_NULL(pDis);

    //Add dispatch instance into the adapter list.添加各服务的Adaptor到列表中
    //::EnterCriticalSection(&m_adaptorMaplock);
	DeleteByTypeMap( serviceType, bstrName );
    pDis->AddRef();
    m_adaptorMap[ _bstr_t( bstrName ) ]  = pDis;

	//daming: 2011-08-09 
	pDis->AddRef();
	m_adaptorByTypeMap[ serviceType ] = pDis;
	//m_typeByDispatchMap[ pDis ] = serviceType;

    //::LeaveCriticalSection(&m_adaptorMaplock);
    
    //Add dispatch entry into entry map.添加自动化实例到map中
    //{ OLESTR(#member), dispid, DISPATCH_PROPERTYGET, vt, 0, { VTS_REF_DISPATCH }, VT_EMPTY, 
    //(void (__stdcall CExternal::*)())&get_##member }
    DispatchEntry<External>* pEntry = new DispatchEntry<External>();
	pEntry->bstrName =  ::SysAllocString ( bstrName );
    pEntry->wFlags = DISPATCH_PROPERTYGET;
    pEntry->vtReturn = VT_DISPATCH;
    pEntry->nArgs = 1;
    pEntry->vtSingle = VT_BSTR;
	pEntry->vtbstrArgs = ::SysAllocString( bstrName );
    pEntry->dispid = (long)bstrName;
    pEntry->pfn = (void (__stdcall External::*)())&External::get_ModuleAdaptor;

    //insert DispatchEntry into dispatch entry map.
    //::EnterCriticalSection(&m_dispatchMapLock);
	citer = m_dispatchEntryMap.find( _bstr_t( bstrName ) );
	if (citer != m_dispatchEntryMap.end())
	{
		SAFE_DELETE((*citer).second);
		m_dispatchEntryMap.erase(citer);
	}

    m_dispatchEntryMap.insert(DISPATCH_PAIR(bstrName, pEntry));
    //::LeaveCriticalSection(&m_dispatchMapLock);

exit:
//     if (FAILED(hr))
//         //log the error.
//         LOG_ERR(L"Add adapter object with error: 0x%08X", hr);
// 	else
// 		LOG_INFO(L"Add adapter object:%s successfully",bstrName);

    return hr;
}

/**
* @brief get the specific adapter from external.
* @param [in] name. the name of the adapter.
* @param [in] pDis. the pointer of adapter pointer.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT External::GetPluginObject( BSTR bstrName, IDispatch ** pDis )
{
    HRESULT     hr = S_OK;
    MODULE_ADAPTOR_MAP::iterator iter;

//    CHECK_IF_NULL(bstrName);

   // ::EnterCriticalSection(&m_adaptorMaplock);
	iter = m_adaptorMap.find( _bstr_t( bstrName ) );
	if (iter != m_adaptorMap.end())
		*pDis = (*iter).second;  
   // ::LeaveCriticalSection(&m_adaptorMaplock);

exit:
    if (FAILED(hr))
    {
        //log the error.
        //LOG_ERR(L"Get adapter object with error: 0x%08X", hr);
    }

    return hr;
}

void External::DeleteByTypeMap( int nServiceType,BSTR bstrServiceName )
{
	if( nServiceType > 0 )
	{
		MODULE_ADAPTOR_BY_TYPE_MAP::iterator iter1 = m_adaptorByTypeMap.find( nServiceType );
		if ( iter1 != m_adaptorByTypeMap.end() )
		{
			// ToDo SongGuoqiang neet to release
			m_adaptorByTypeMap.erase( iter1 );
		}
	}
	if ( NULL != bstrServiceName )
	{
		MODULE_ADAPTOR_MAP::iterator iter3;
		iter3 = m_adaptorMap.find( _bstr_t( bstrServiceName ) );
		if ( iter3 != m_adaptorMap.end() )
		{
			// ToDo SongGuoqiang neet to release
			m_adaptorMap.erase(iter3);
		}
	}	
}

/**
* @brief remove the specific adapter from external.adapter object will recycle
* in the owner module.
* @param [in] name. the name of the adapter will be removed.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT External::RemovePluginObject( BSTR bstrName )
{
    HRESULT     hr = S_OK;
    //MODULE_ADAPTOR_MAP::iterator iter;
	DISPATCH_MAP::iterator citer;

//    CHECK_IF_NULL(bstrName);

   // ::EnterCriticalSection(&m_adaptorMaplock);
	DeleteByTypeMap( 0,bstrName );
// 	iter = m_adaptorMap.find(bstrName);
// 	if (iter != m_adaptorMap.end())
// 	{
// 		//daming: 2011-08-09 从另两个容器中把按serviceType查询的对应表项删除
// 		IDispatch * pDisp = (*iter).second;
// 		DeleteByTypeMap( pDisp );
// 
// 		m_adaptorMap.erase(iter); 
// 	}
   // ::LeaveCriticalSection(&m_adaptorMaplock);

	//::EnterCriticalSection(&m_dispatchMapLock);
	citer = m_dispatchEntryMap.find(bstrName);
	if (citer != m_dispatchEntryMap.end())
	{
		SAFE_DELETE((*citer).second);
		m_dispatchEntryMap.erase(citer);
	}
	//::LeaveCriticalSection(&m_dispatchMapLock);

exit:
//     if (FAILED(hr))
//         //log the error.
//         LOG_ERR(L"Remove adapter object with error: 0x%08X", hr);
// 	else
// 		LOG_INFO(L"Remove adapter object:%s successfully",bstrName);

	return hr;
}