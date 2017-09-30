/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file IExternal.h
* @brief Defined a interface class for other module
*         to using external in Tang Client
* 
* @author Hongwei Hu (hongwei.hu@gnetis.com)
*
* @date Janu. 25 2010
*
* @version 1.0.0
* 
* @warning
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug

******************************************************************************/

#ifndef GNET_TANG_CLIENT_IEXTERNAL_H
#define GNET_TANG_CLIENT_IEXTERNAL_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <ComDef.h>

/**
* @class IDisptchObjectContainer
* @brief This class is the interface class that provides all the APIs for other module
*         to using external in Tang Client.
* 
* Conference manager should passing the IExtenal instance to All the business module which
* will fill their adapter into External.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
class IExternal
{
public:
    /**
    * @brief Interface to insert the adapter into external.
    * @param [in] name: name of the adapter.
    * @param [in] pDis: pointer of IDispatch that identified the adapter.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT 
        PutPluginObject(
        __in BSTR name, 
        __in IDispatch * pDis
        ) = 0;

    /**
    * @brief Interface to insert the adapter into external.
	* @param [in] serviceType: type of the adapter
    * @param [in] name: name of the adapter.
    * @param [in] pDis: pointer of IDispatch that identified the adapter.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
	//daming: add serviceType param
    virtual HRESULT 
        PutPluginObjectWithType(
		__in int serviceType,
        __in BSTR name, 
        __in IDispatch * pDis
        ) = 0;

    /**
    * @brief Interface to query an adapter by the name.
    * @param [in] name: name of the adapter.
    * @param [in] [out] pDis: pointer of IDispatch that identified the adapter.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    virtual    HRESULT
        GetPluginObject(
        __in BSTR name,
        __inout IDispatch ** pDis
        ) = 0;

    /**
    * @brief interface to remove .
    * @param [in] name: name of the adapter.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    virtual 
        HRESULT
        RemovePluginObject(
        __in BSTR name
        ) = 0;
};

class IDispatchObjectContainer: public IExternal
{

};
#endif //GNET_TANG_CLIENT_IEXTERNAL_H