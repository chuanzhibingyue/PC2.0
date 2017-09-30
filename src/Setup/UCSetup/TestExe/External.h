/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file External.h
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

#pragma once

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include "ExternalDispatch.h"
#include "IExternal.h"
#include <map>
#include <comdef.h>
#include <comdefsp.h>

//using namespace std;

typedef std::map<_bstr_t, IDispatch*> MODULE_ADAPTOR_MAP;
//daming: 2011-08-09 增加按serviceType查询的功能，便于 ServiceList 可以按ID查询
typedef std::map<int, IDispatch*> MODULE_ADAPTOR_BY_TYPE_MAP;
//typedef std::map<IDispatch*, int> MODULE_TYPE_BY_DISPATCH_MAP;

/**
* @class External
* @brief External is the collection of module adapter.it will be used by UI JS code.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
class External: public IExternal, public ExternalDispatch<External>
{
public:
    External();
    ~External();
	 
	/**< Adaptor属性接口函数*/
    IDispatch * WINAPI get_ModuleAdaptor(BSTR bstrAdapter);

    /**< IExternal interface*/
	/**< 向adaptor map中添加接口实例，供native调用*/
	HRESULT virtual PutPluginObject(BSTR name, IDispatch* pDis);
	
	/**< IExternal interface*/
	/**< 向adaptor map中添加接口实例，供native调用*/
	//daming: 2011-08-09 增加serviceType便于 ServiceList 可以按ID查询
	HRESULT virtual PutPluginObjectWithType(int serviceType, BSTR name, IDispatch* pDis);

	/**< 通过名字得到Adaptor实例，供native调用*/
	HRESULT virtual GetPluginObject(BSTR name, IDispatch** pDis);
	
	/**< 删除已添加的adaptor实例*/
	HRESULT virtual RemovePluginObject(BSTR name);

protected:

	//daming: 2011-08-09 
	void DeleteByTypeMap( int nServiceType,BSTR bstrServiceName );

protected:
	MODULE_ADAPTOR_MAP m_adaptorMap;            /**< map of the adapter name and object*/
	//daming: 2011-08-09 增加按serviceType查询的功能，便于 ServiceList 可以按ID查询
	MODULE_ADAPTOR_BY_TYPE_MAP m_adaptorByTypeMap;
	//daming: 2011-08-09 增加按IDispatch*查询服务类型的机制，为删除提供方便
	//MODULE_TYPE_BY_DISPATCH_MAP	m_typeByDispatchMap;
   // CRITICAL_SECTION   m_adaptorMaplock;        /**< shared resource lock*/
};