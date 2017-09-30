/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file     IWizard.h
* @brief    Defined a interface class that provides the APIs for others.
* 
* @author   huajian.yin
*
* @date     2014.03.15
*
* @version  1.0.0
* 
* @warning 
*
* Revision  History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug

******************************************************************************/

#ifndef GNET_TANG_CLIENT_IWIZARD_H
#define GNET_TANG_CLIENT_IWIZARD_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include "IExternal.h"
#include "IMessageProvider.h"
#include "IConfiguration.h"

/**
* @class IWizard
* @brief This class is the interface class that provides the APIs of the AVWizard.         
* 
* AVWizard module should include this class to using the APIs of the 
* AVWizard it belongs to.
* 
* @author huajian.yin
* @version 1.0.0
*/
class IWizard
{
public:
    virtual HRESULT Init(
        IMessageProvider* pMsgDispatcher,
        IExternal* pExternal,
        IDispatch* pIEInstance,
        IConfiguration * pConfiguration) = 0;
    virtual HRESULT Stop() = 0;

};
#endif  //GNET_TANG_CLIENT_IWIZARD_H