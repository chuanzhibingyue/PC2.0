/****************************************************************************************
* Copyright (c) 2008~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 
****************************************************************************************/
/**
* @file IService.h
* @brief tang service definition.
*
* @author hongwei.hu (hongwei.hu@gnetis.com)
*
* @date 2010/01/29
*
* @version 1.0
*
* @warning  
*
* Revision History 
* @if  CR/PR ID Author	   Date		  Major Change @endif
* @bug 
****************************************************************************************/
#ifndef GNET_TANG_CLIENT_ISERVICE_H
#define GNET_TANG_CLIENT_ISERVICE_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <GlobalDefine.h>

class IService
{
public :
    virtual HRESULT  Start(
        UINT32 uChannelId,
        UINT32 ugroupId,
        const GlobalPointer* glbPointer) =0;
};

#endif //GNET_TANG_CLIENT_ISERVICE_H