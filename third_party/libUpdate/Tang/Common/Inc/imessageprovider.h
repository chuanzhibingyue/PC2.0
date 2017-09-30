/******************************************************************************
* Copyright (c) 2009~2010 All Rights Resverved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file IMessageProvider.h
* @brief Definite a interface class for MessageDispatcher in Tang Client.
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

#ifndef GNET_TANG_CLIENT_IMESSAGE_PROVIDER_H
#define GNET_TANG_CLIENT_IMESSAGE_PROVIDER_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <Message.h>
#include <IMessageConsumer.h>

/**
* @class Message
* @brief This class is a interface class for MessageDispatcher.
* 
* MessageDispatcher must inherit from this class.
* 
* @author Xiaogang Yu
* @version 1.0.0
*/
class IMessageProvider
{
public:

    /**
    * @brief It is a interface, add the consumer to consumer queue.
    * @param [in] conferenceID: the id of conference that business in.
    * @param [in] groupID: the id of channel group that business use.
    * @param [in] pConsumer: the pointer of IMessageConsumer.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    * @see IMessageConsumer.
    */
    virtual 
    HRESULT 
    RegisterConsumer(
        __in UINT32 conferenceID, 
        __in UINT32 groupID, 
        __in IMessageConsumer *pConsumer
        ) = 0;
    
    /**
    * @brief It is a interface, delete the consumer from consumer queue.
    * @param [in] pConsumer: the pointer of IMessageConsumer.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    * @see IMessageConsumer.
    */
    virtual 
    HRESULT 
    UnRegisterConsumer(
        __in IMessageConsumer *pConsumer
        ) = 0;

    /**
    * @brief It is a interface, push the message to the message queue.
    * @param [in] conferenceID: the id of conference that business in.
    * @param [in] groupID: the id of channel group that business use.
    * @param [in] pMsg*: the pointer of Message.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    * @see Message.
    */
    virtual 
    HRESULT 
    PushMessage(
        __in UINT32 conferenceID, 
        __in UINT32 groupID, 
        __in Message *pMsg
        ) = 0;
	virtual 
	HRESULT 
	SendMessage
		(
		__in UINT32 conferenceID, 
		__in UINT32 groupID, 
		__in Message *pMsg
		) = 0;
};

#endif //GNET_TANG_CLIENT_IMESSAGE_PROVIDER_H
