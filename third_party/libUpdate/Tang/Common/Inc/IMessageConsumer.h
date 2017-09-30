/******************************************************************************
* Copyright (c) 2009~2010 All Rights Resverved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file IMessageConsumer.h
* @brief Definite a interface class consume message for other business 
*        in Tang Client.
* 
* @author Xiaogang Yu (xiaogang.yu@gnetis.com)
*
* @date Janu. 12 2010
*
* @version 1.0.0
* 
* @warning 
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug CR0001      Xiaogang Yu     Janu. 12 2010   create version 1.0.0\n

******************************************************************************/

#ifndef GNET_TANG_CLIENT_IMESSAGE_CONSUMER_H
#define GNET_TANG_CLIENT_IMESSAGE_CONSUMER_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <Message.h>

/**
* @class IMessageConsumer
* @brief This class is interface class consumer message for other business in 
*        Tang Client.
* 
* Other message consumer should inherit from this class in some business in Tang
* Client. In these child classes deal with message.
* 
* @author Xiaogang Yu
* @version 1.0.0
*/
class IMessageConsumer
{
public:
    /**
     * @brief It is a interface, deal with the message.
     * @param [in] pMsg: the message will been deal with.
     * @return HRESULT return S_OK if successful, otherwise return error code.
     * @pre in this fuction can't UnRegiesterConsumer itself.
     * @see Message.
     */
    virtual 
    HRESULT 
    ConsumeMessage(
        __in const Message *pMsg
        ) = 0;
};

#endif //GNET_TANG_CLIENT_IMESSAGE_CONSUMER_H
