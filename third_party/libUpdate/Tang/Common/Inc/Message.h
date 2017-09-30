/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file Message.h
* @brief Definite a base class for all the messages includes internal and 
*        external messages in Tang Client.
* 
* @author Xiaogang Yu (xiaogang.yu@gnetis.com)
*
* @date Janu. 12 2010
*
* @version 1.0.0
* 
* @warning null
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug 

******************************************************************************/

#ifndef GNET_TANG_CLIENT_MESSAGE_H
#define GNET_TANG_CLIENT_MESSAGE_H

#define TANG_CLIENT_MESSAGE_TYPE_INTERNAL       (0)     /**< the internal message type in Tang Client. */
#define TANG_CLIENT_MESSAGE_TYPE_EXTERNAL       (1)     /**< the external message type in Tang Client. */

/*
* @class Message
* @brief Definite a base class for all the messages includes internal and 
*        external messages in Tang Client.
* 
* All the messages should inherit from this class in Tang Client.
* 
* @author Xiaogang Yu
* @version 1.0.0
*/
class Message
{
public:
    virtual ~Message(){}

    UINT32          msgType;   /**< The message type, only two type, external message or internal message. */
};

#endif //GNET_TANG_CLIENT_MESSAGE_H
