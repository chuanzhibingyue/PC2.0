/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file     IConfService.h
* @brief    Defined a interface class that provides the APIs for UCChat.
* 
* @author   Hezhuo
*
* @date     2013.12.25
*
* @version  1.0.0
* 
* @warning 
*
* Revision  History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug

******************************************************************************/

#ifndef GNET_TANG_CLIENT_ICONFSERVICE_H
#define GNET_TANG_CLIENT_ICONFSERVICE_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include "IExternal.h"
#include "IMessageProvider.h"
#include "IConfiguration.h"
#include "PluginManager.h"
#include "IService.h"

/**
* @class IConfService
* @brief This class is the interface class that provides the APIs of the ConfService.         
* 
* business module should include this class to using the APIs of the 
* conference it belongs to.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
class IConfService : public IService
{
public:
    /**
    * @brief Interface for querying the conference id.
    * @return hre return id if successful, otherwise return 0.
    */
    virtual HRESULT Init(
        PluginManager* pPlugMgr,
        IMessageProvider* pMessageDispatcher,
        IExternal* pExternal,
        IDispatch* pIEInstance,
        IConfiguration * pConfiguration) = 0;

    virtual HRESULT  JoinConference(UINT32 runMode, BSTR bstrProtocal) = 0;

};
#endif  //GNET_TANG_CLIENT_ICONFSERVICE_H