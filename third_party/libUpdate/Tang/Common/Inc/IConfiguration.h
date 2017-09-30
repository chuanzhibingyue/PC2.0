/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file IConfiguration.h
* @brief Defined a interface class for using xml operation interface in Tang Client
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

#ifndef GNET_TANG_CLIENT_ICONFIGURATION_H
#define GNET_TANG_CLIENT_ICONFIGURATION_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <ComDef.h>
#include <list>
using namespace std;

/**
* configuration list type definition.
*/
class IConfiguration;
typedef std::list<IConfiguration*> ConfigurationList;

/**
* @class IConfiguration
* @brief This class is the interface class that provides all the APIs for xml operation
*   in Tang Client.
* 
* Conference manager should passing the IConfiguration instance to All the business module which
* will fill their adapter into External.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
class IConfiguration
{
public:

    /**
    * @brief Interface to get specific sub item value.
    * @param [in] lpstrConfigItemName: node path string.
    * @param [in out] pbstrValue: result of queried node value.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT GetSubItemValue(
        __in LPCWSTR lpstrConfigItemName,
        __inout BSTR* pbstrValue) = 0;

    /**
    * @brief Interface to get current configuration item attribute value.
    * @param [in] lpAttributeName: node attribute name string.
    * @param [in out] pbstrAttrValue: result of attribute value.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT GetAttributeValue(
        __in LPCWSTR lpAttributeName,
        __inout BSTR* pbstrAttrValue) = 0;

    /**
    * @brief Interface to get sub configuration item instance.
    * @param [in] lpstrItemFullName: configuration item full path string.
    * @param [in out] pSubItem: result of sub configuration item.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT GetSubItem(
        __in LPCWSTR lpstrItemFullName,
        __inout IConfiguration** pSubItem) = 0;
    
    /**
    * @brief Interface to get sub configuration items list.
    * @param [in] lpstrItemFullName: configuration item full path string.
    * @param [in out] subItemList: list of sub items.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT GetSubItemList(
        __in LPCWSTR lpstrItemFullName,
        __inout ConfigurationList& subItemList) = 0;

    /**
    * @brief Interface to get current item value.
    * @param [in out] pbstrValue: result of queried node value.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT GetConfigValue(
        __inout BSTR* pbstrValue) = 0;
};
#endif //GNET_TANG_CLIENT_ICONFIGURATION_H
