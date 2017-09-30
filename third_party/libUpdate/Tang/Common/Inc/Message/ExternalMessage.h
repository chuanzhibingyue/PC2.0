/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file ExternalMessage.h
* @brief Definite a virtual interface class for every external message class in Tang Client.
* 
* @author Xiaogang Yu (xiaogang.yu@gnetis.com)
*
* @date Feb. 2 2010
*
* @version 1.0.0
* 
* @warning null
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug 

******************************************************************************/

#ifndef GNET_TANG_CLIENT_EXTERNALMESSAGE_H
#define GNET_TANG_CLIENT_EXTERNALMESSAGE_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <windows.h>
#include <Message.h>

/*---------------------------------DEFINITION--------------------------------*/
#ifndef __cplusplus
#error "ExternalMessage.h" need c++ compiler
#endif 

/**< <b>This is a macro for making a DWORD data with two WORD data.</b>
     <b>highWord definite the high 16 bits data.</b> 
     <b>lowWord definite the low 16 bits data.</b> */
#define MAKE_DWORD(highWord, lowWord) ( (((DWORD)highWord) << 16) + (lowWord) ) 

/**
* @defgroup The definition of the protocol type in Tang communication protocol.
* @brief These macros are used in different business module, such as, ChannelMgr,
*        ConfMgr, WhiteBoard Service, DocSharing Service and so on. 
* @{ 
*/  
#define TANG_PROTOCOL_MESSAGE_TYPE_CHANNEL                  (0x0000)
#define TANG_PROTOCOL_MESSAGE_TYPE_CONFERENCE               (0x0001)
#define TANG_PROTOCOL_MESSAGE_TYPE_WHITEBOARD_SERVICE       (0x0307)
#define TANG_PROTOCOL_MESSAGE_TYPE_AUDIO_SERVICE            (0x0102)
#define TANG_PROTOCOL_MESSAGE_TYPE_VIDEO_SERVICE            (0x0103)
#define TANG_PROTOCOL_MESSAGE_TYPE_DOCSHARING_SERVICE       (0x0308)
#define TANG_PROTOCOL_MESSAGE_TYPE_DESKTOPSHARING_SERVICE   (0x0309)
#define TANG_PROTOCOL_MESSAGE_TYPE_CHAT_SERVICE             (0x0204)
#define TANG_PROTOCOL_MESSAGE_TYPE_QA_SERVICE               (0x0205)
#define TANG_PROTOCOL_MESSAGE_TYPE_SURVEY_SERVICE           (0x0206)

#define CHANNEL_AUTH_REQ_MSG_ID                     (0x0001)            /**<认证请求的消息ID */
#define CHANNEL_AUTH_ACK_MSG_ID                     (0x0002)            /**<认证响应的消息类型 */
#define CHANNEL_DESTROY_MSG_REQ_ID                  (0x0003)            /**<通道的拆除消息*/
#define CHANNEL_DESTROY_MSG_ACK_ID                  (0x0004)            /**<通道的拆除消息*/
#define CHANNEL_INVALID_CHANNEL_MSG_ID              (0x000A)
#define CHANNEL_UNREACHABLE_CHANNEL_MSG_ID          (0x000B)
#define CHANNEL_RECV_RESET_MSG_ID                   (0x1008)
#define CHANNEL_MULTICAST_DETECT_REQ_ID             (0x1001)            /**<组播探测请求消息*/
#define CHANNEL_MULTICAST_DETECT_RES_ID             (0x1002)            /**<组播探测应答消息*/
#define CHANNEL_MULTICAST_INFO_REPORT_ID            (0x1003)            /**<组播组信息汇报消息*/
#define CHANNEL_MULTICAST_INFO_UPDATE_ID            (0x1006)            /**<组播组信息更新消息*/
#define CHANNEL_MULTICAST_INFO_UPDATE_RES_ID        (0x1007)            /**<组播组更新应答消息*/

#define CHANNEL_NETWORK_INFO_MSG_ID                 (0x0010)            /**<客户端组网信息汇报的消息 */
#define CHANNEL_P2P_INFO_MSG_ID                     (0x0011)            /**<客户端P2P信息汇报的消息 */
#define CHANNEL_NETWORK_INFO_MSG_ID                 (0x0010)            /**<客户端组网信息汇报的消息 */
#define CHANNEL_P2P_INFO_MSG_ID                     (0x0011)            /**<客户端P2P信息汇报的消息 */
#define CHANNEL_HEARTBEAT_REQ_MSG_ID                (0x0005)            /**<心跳请求的消息ID */
#define CHANNEL_HEARTBEAT_RSP_MSG_ID                (0x0006)            /**<心跳响应的消息ID */
#define CHANNEL_HEARTBEAT_DEFAULT_ID                (0x0000000C)        /**<暂时未用 */
#define CHANNEL_SENDMESSAGE_INFO_ID                 (0x0007)            /**<向上层汇报消息发送情况的消息ID */
/** @} */

/**< This macro definite the error for the size of message body is not matched. */
#define TANG_MESSAGE_BODY_SIZE_NOT_MATCH                    (0XA0000009)

/*
* @class ExternalMessage
* @brief Definite a virtual interface class for every external message class in Tang Client.
* 
* All the external messages in Tang Client must inherit from this class directly
* or indirectly. It supply some common interfaces for external messages. Every 
* child class must implement the interface GetBodyLength().
* 
* @author Xiaogang Yu
* @version 1.0.0
*/
#pragma pack(1)

class ExternalMessage : public Message
{
public:
    /**
    * @brief Serialize the class data to buffer.
    * @param [in][out] ppBuffer: the buffer stores the class data.
    * @param [in][out] cbBuffer: the length of the buffer.
    * @return return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT 
    Serialize(
        __inout BYTE** ppBuffer, 
        __inout size_t& cbBuffer
        ) = 0;

    /**
    * @brief Serialize the body data to buffer in message class.
    * @param [in] pBuf: input the pointer of buffer will store the body data.
    * @param [in] cbBuf: the length of the buffer inputted.
    * @return return the size of body data.
    */
    virtual size_t
    SerializeBody(
        __in LPBYTE pBuf, 
        __in size_t cbBuf
        ) = 0;

    /**
    * @brief DeSerialize buffer to the class data.
    * @param [in] pMsg: input the pointer of ExternalMessage.
    * @return return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT 
    DeSerialize(
        __in ExternalMessage *pMsg
        ) = 0;

    /**
    * @brief DeSerialize the body data of class message from buffer.
    * @param [in] pBuf: input the pointer of buffer stored the body data.
    * @param [in] cbBuf: the length of the buffer inputted.
    * @return return the size of body data.
    */
    virtual size_t
    DeSerializeBody(
        __in LPBYTE pBuf,
        __in size_t cbBuf
        ) = 0;

    /**
    * @brief Get the message type.
    * @param [in] void 
    * @return return the message type.
    */
    virtual UINT16 
    GetMessageType(void) = 0;

    /**
    * @brief Get the message id.
    * @param [in] void 
    * @return return the message id.
    */
    virtual UINT16 
    GetMessageID(void) = 0;

    /**
    * @brief Get the dispatch id.
    * @param [in] void.
    * @return return the dispatch id.
    */
    virtual UINT32 
    GetDispatchID(void) = 0;
    
    /**
    * @brief Get the length of class, this function must been 
    *        implemented by every child class which data length 
    *        is unstable, such as include pointer,vector,list and so on.
    * @param [in] void 
    * @return return length of class.
    */
    virtual UINT32 
    GetBodyLength(void) = 0;

    
    BOOL GetSentFlag()
    {
        return isSent;
    }

    void SetSentFlag(BOOL sent)
    {
        isSent = sent;
    }

    ExternalMessage()
    {
        this->pBuffer = NULL;
        this->cbBuffer = 0;
        this->isSent = FALSE;
    }

    virtual ~ExternalMessage()
    {
    };

public:
    LPBYTE      pBuffer;        /**< The buffer pointer. */
    size_t      cbBuffer;       /**< The size of buffer. */

private:
    BOOL                isSent;
};
#pragma pack()

#endif //GNET_TANG_CLIENT_EXTERNALMESSAGE_H
