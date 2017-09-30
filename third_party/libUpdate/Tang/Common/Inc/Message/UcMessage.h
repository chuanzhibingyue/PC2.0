/****************************************************************************************/
/**
* @file        MqMessage.h
* @brief       
*
* @author      hezhuo 
*
* @date        2013.12.30
*
* @version     1.0
*
* @warning  
*
* Revision History 
* @if  CR/PR ID Author       Date          Major Change @endif
* @bug 
****************************************************************************************/
#pragma once

#include "Message/InternalMessage.h"

#pragma pack(1)
class CreateSessionRequestMessage : public InternalMessage
{
public:
    CreateSessionRequestMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_CREATESESSION_REQUEST;
        nSessionID = 0;
        nSessionIDType = 0;
        nLoadType = 0;
    }

    virtual ~CreateSessionRequestMessage(){}

    virtual HRESULT Serialize(__inout BYTE** ppBuffer, __inout size_t& cbBuffer)
    {
        cbBuffer = sizeof(msgType) + sizeof(internalMsgType) + sizeof(nSessionID)
            + sizeof(nSessionIDType) + sizeof(nLoadType) + sizeof(int) + strCreateParameter.length()
            + sizeof(nSourceSessionID) + sizeof(nSourceSessionIDType);

        *ppBuffer = new BYTE[cbBuffer];

        BYTE *pTmp = *ppBuffer;
        //@
        memcpy(pTmp,&msgType,sizeof(msgType));
        pTmp += sizeof(msgType);
        //@
        memcpy(pTmp,&internalMsgType,sizeof(internalMsgType));
        pTmp += sizeof(internalMsgType);
        //@
        memcpy(pTmp,&nSessionID,sizeof(nSessionID));
        pTmp += sizeof(nSessionID);
        //@
        memcpy(pTmp,&nSessionIDType,sizeof(nSessionIDType));
        pTmp += sizeof(nSessionIDType);
        //@
        memcpy(pTmp,&nLoadType,sizeof(nLoadType));
        pTmp += sizeof(nLoadType);
        //@
        int nParameterLen = strCreateParameter.length();
        memcpy(pTmp,&nParameterLen,sizeof(nParameterLen));
        pTmp += sizeof(nParameterLen);
        //@
        memcpy(pTmp,strCreateParameter.c_str(),nParameterLen);
        pTmp += nParameterLen;
        //@
        memcpy(pTmp,&nSourceSessionID,sizeof(nSourceSessionID));
        pTmp += sizeof(nSourceSessionID);
        //@
        memcpy(pTmp,&nSourceSessionIDType,sizeof(nSourceSessionIDType));
        pTmp += sizeof(nSourceSessionIDType);

        return S_OK;
    }

    virtual HRESULT DeSerialize(__in BYTE* ppBuffer, __in size_t& cbBuffer)
    {
        //@
        memcpy(&msgType,ppBuffer,sizeof(msgType));
        ppBuffer += sizeof(msgType);
        //@
        memcpy(&internalMsgType,ppBuffer,sizeof(internalMsgType));
        ppBuffer += sizeof(internalMsgType);
        //@
        memcpy(&nSessionID,ppBuffer,sizeof(nSessionID));
        ppBuffer += sizeof(nSessionID);
        //@
        memcpy(&nSessionIDType,ppBuffer,sizeof(nSessionIDType));
        ppBuffer += sizeof(nSessionIDType);
        //@
        memcpy(&nLoadType,ppBuffer,sizeof(nLoadType));
        ppBuffer += sizeof(nLoadType);
        //@
        int nParameterLen = 0;
        memcpy(&nParameterLen,ppBuffer,sizeof(nParameterLen));
        ppBuffer += sizeof(nParameterLen);
        //@
        strCreateParameter = std::string((char*)ppBuffer, nParameterLen);
        ppBuffer += nParameterLen;
        //@
        memcpy(&nSourceSessionID,ppBuffer,sizeof(nSourceSessionID));
        ppBuffer += sizeof(nSourceSessionID);
        //@
        memcpy(&nSourceSessionIDType,ppBuffer,sizeof(nSourceSessionIDType));
        ppBuffer += sizeof(nSourceSessionIDType);

        return S_OK;
    }

    UINT32                  nSessionID;         /**< 会话ID */
    UINT32                  nSessionIDType;     /**< 会话ID类型，详见SessionIDType定义 */
    UINT32                  nLoadType;          /**< 会话启动类型，详见UCChatLoadType定义 */
    std::string             strCreateParameter; /**< 会话创建参数 */

    UINT32                  nSourceSessionID;   /**< 会话ID */
    UINT32                  nSourceSessionIDType;/**< 会话ID类型，详见SessionIDType定义 */
};

class CreateSessionResponseMessage : public InternalMessage
{
public:
    CreateSessionResponseMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_CREATESESSION_RESPONSE;
        nSessionID = 0;
        nSessionIDType = 0;
        nCreateResult = 0;
        nCreateErrorCode = 0; 
    }

    virtual ~CreateSessionResponseMessage(){}

    virtual HRESULT Serialize(__inout BYTE** ppBuffer, __inout size_t& cbBuffer)
    {
        cbBuffer = sizeof(msgType) + sizeof(internalMsgType) + sizeof(nSessionID)
            + sizeof(nSessionIDType) + sizeof(nCreateResult) + sizeof(nCreateErrorCode);

        *ppBuffer = new BYTE[cbBuffer];

        BYTE *pTmp = *ppBuffer;
        //@
        memcpy(pTmp,&msgType,sizeof(msgType));
        pTmp += sizeof(msgType);
        //@
        memcpy(pTmp,&internalMsgType,sizeof(internalMsgType));
        pTmp += sizeof(internalMsgType);
        //@
        memcpy(pTmp,&nSessionID,sizeof(nSessionID));
        pTmp += sizeof(nSessionID);
        //@
        memcpy(pTmp,&nSessionIDType,sizeof(nSessionIDType));
        pTmp += sizeof(nSessionIDType);
        //@
        memcpy(pTmp,&nCreateResult,sizeof(nCreateResult));
        pTmp += sizeof(nCreateResult);
        //@
        memcpy(pTmp,&nCreateErrorCode,sizeof(nCreateErrorCode));
        pTmp += sizeof(nCreateErrorCode);

        return S_OK;
    }

    virtual HRESULT DeSerialize(__in BYTE* ppBuffer, __in size_t& cbBuffer)
    {
        //@
        memcpy(&msgType,ppBuffer,sizeof(msgType));
        ppBuffer += sizeof(msgType);
        //@
        memcpy(&internalMsgType,ppBuffer,sizeof(internalMsgType));
        ppBuffer += sizeof(internalMsgType);
        //@
        memcpy(&nSessionID,ppBuffer,sizeof(nSessionID));
        ppBuffer += sizeof(nSessionID);
        //@
        memcpy(&nSessionIDType,ppBuffer,sizeof(nSessionIDType));
        ppBuffer += sizeof(nSessionIDType);
        //@
        memcpy(&nCreateResult,ppBuffer,sizeof(nCreateResult));
        ppBuffer += sizeof(nCreateResult);
        //@
        memcpy(&nCreateErrorCode,ppBuffer,sizeof(nCreateErrorCode));
        ppBuffer += sizeof(nCreateErrorCode);

        return S_OK;
    }

    UINT32                  nSessionID;         /**< 会话ID */
    UINT32                  nSessionIDType;     /**< 会话ID类型，详见SessionIDType定义 */
    UINT32                  nCreateResult;      /**< 会话创建结果 1成功 0失败 */
    UINT32                  nCreateErrorCode;   /**< 会话创建错误码 */
};

class CreateSessionResultNotifyMessage : public InternalMessage
{
public:
    CreateSessionResultNotifyMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_CREATESESSION_RESULT_NOTIFY;
        nSessionID = 0;
        nSessionIDType = 0;
        nCreateResult = 0;
        nCreateErrorCode = 0; 
    }
    UINT32                  nSessionID;         /**< 会话ID */
    UINT32                  nSessionIDType;     /**< 会话ID类型，详见SessionIDType定义 */
    UINT32                  nCreateResult;      /**< 会话创建结果 1成功 0失败 */
    UINT32                  nCreateErrorCode;   /**< 会话创建错误码 */
};

class CloseSessionNotifyMessage: public InternalMessage
{
public:
    CloseSessionNotifyMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_CLOSESESSION_NOTIFY;
        nSessionID = 0;
        nSessionIDType = 0;
    }

    virtual ~CloseSessionNotifyMessage(){}

    virtual HRESULT Serialize(__inout BYTE** ppBuffer, __inout size_t& cbBuffer)
    {
        cbBuffer = sizeof(msgType) + sizeof(internalMsgType) + sizeof(nSessionID) + sizeof(nSessionIDType);
        *ppBuffer = new BYTE[cbBuffer];
        BYTE* pTmp = *ppBuffer;
        //@
        memcpy(pTmp,&msgType,sizeof(msgType));
        pTmp += sizeof(msgType);
        //@
        memcpy(pTmp,&internalMsgType,sizeof(internalMsgType));
        pTmp += sizeof(internalMsgType);
        //@
        memcpy(pTmp,&nSessionID,sizeof(nSessionID));
        pTmp += sizeof(nSessionID);
        //@
        memcpy(pTmp,&nSessionIDType,sizeof(nSessionIDType));
        pTmp += sizeof(nSessionIDType);

        return S_OK;
    }

    virtual HRESULT DeSerialize(__in BYTE* ppBuffer,__in size_t& cbBuffer)
    {
        //@
        memcpy(&msgType,ppBuffer,sizeof(msgType));
        ppBuffer += sizeof(msgType);
        //@
        memcpy(&internalMsgType,ppBuffer,sizeof(internalMsgType));
        ppBuffer += sizeof(internalMsgType);
        //@
        memcpy(&nSessionID,ppBuffer,sizeof(nSessionID));
        ppBuffer += sizeof(nSessionID);
        //@
        memcpy(&nSessionIDType,ppBuffer,sizeof(nSessionIDType));
        ppBuffer += sizeof(nSessionIDType);

        return S_OK;
    }

    UINT32                  nSessionID;         /**< 会话ID */
    UINT32                  nSessionIDType;     /**< 会话ID类型，详见SessionIDType定义 */
};

class HideSessionNotifyMessage: public InternalMessage
{
public:
    HideSessionNotifyMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_HIDESESSION_NOTIFY;
        nSessionID = 0;
        nSessionIDType = 0;
        nPosX = 0;
        nPosY = 0;
    }

    virtual ~HideSessionNotifyMessage(){}

    virtual HRESULT Serialize(__inout BYTE** ppBuffer, __inout size_t& cbBuffer)
    {
        cbBuffer = sizeof(msgType) + sizeof(internalMsgType) + sizeof(nSessionID) + sizeof(nSessionIDType) + sizeof(nPosX) + sizeof(nPosY);
        *ppBuffer = new BYTE[cbBuffer];
        BYTE* pTmp = *ppBuffer;
        //@
        memcpy(pTmp,&msgType,sizeof(msgType));
        pTmp += sizeof(msgType);
        //@
        memcpy(pTmp,&internalMsgType,sizeof(internalMsgType));
        pTmp += sizeof(internalMsgType);
        //@
        memcpy(pTmp,&nSessionID,sizeof(nSessionID));
        pTmp += sizeof(nSessionID);
        //@
        memcpy(pTmp,&nSessionIDType,sizeof(nSessionIDType));
        pTmp += sizeof(nSessionIDType);
        //@
        memcpy(pTmp,&nPosX,sizeof(nPosX));
        pTmp += sizeof(nPosX);
        //@
        memcpy(pTmp,&nPosY,sizeof(nPosY));
        pTmp += sizeof(nPosY);

        return S_OK;
    }

    virtual HRESULT DeSerialize(__in BYTE* ppBuffer,__in size_t& cbBuffer)
    {
        //@
        memcpy(&msgType,ppBuffer,sizeof(msgType));
        ppBuffer += sizeof(msgType);
        //@
        memcpy(&internalMsgType,ppBuffer,sizeof(internalMsgType));
        ppBuffer += sizeof(internalMsgType);
        //@
        memcpy(&nSessionID,ppBuffer,sizeof(nSessionID));
        ppBuffer += sizeof(nSessionID);
        //@
        memcpy(&nSessionIDType,ppBuffer,sizeof(nSessionIDType));
        ppBuffer += sizeof(nSessionIDType);
        //@
        memcpy(&nPosX,ppBuffer,sizeof(nPosX));
        ppBuffer += sizeof(nPosX);
        //@
        memcpy(&nPosY,ppBuffer,sizeof(nPosY));
        ppBuffer += sizeof(nPosY);

        return S_OK;
    }

    UINT32                  nSessionID;         /**< 会话ID */
    UINT32                  nSessionIDType;     /**< 会话ID类型，详见SessionIDType定义 */
    UINT32                  nPosX;
    UINT32                  nPosY;
};

class NewJsMsgNotifyMessage: public InternalMessage
{
public:
    NewJsMsgNotifyMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_JS_NEWMSG_NOTIFY;
    }

    virtual ~NewJsMsgNotifyMessage(){}

    //message content.
    std::string strName;
    std::string strMsg;
};

class UpdataSessionDataMessage: public InternalMessage
{
public:
    UpdataSessionDataMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_UPDATE_SESSIONDATA;
    }

    virtual ~UpdataSessionDataMessage(){}

    //message content.
    UINT32 loginUser;
    UINT32 sessionid;
    UINT32 sessionIdType;
    std::wstring wstrParameter;
    UINT32 posX;
    UINT32 posY;
};

class OSLeaveChangeNotify:public InternalMessage
{
public:
    OSLeaveChangeNotify()
    {
        //message content.
        m_bStatus = FALSE;//Leave status：TRUE：leave；FALSE：input
        internalMsgType = INTERNAL_MESSAGE_SYS_OSLEAVE_CHANGE;
    }
public:
    BOOL m_bStatus;
};

class HasFullScreenChangeNotify:public InternalMessage
{
public:
    HasFullScreenChangeNotify()
    {
        //message content.
        m_bStatus = FALSE;//fullscreen status：TRUE：full；FALSE：not full
        internalMsgType = INTERNAL_MESSAGE_SYS_HASFULLSCREEN_CHANGE;
    }
public:
    BOOL m_bStatus;
};

class CatchScreenOKNotify:public InternalMessage
{
public:
    CatchScreenOKNotify()
    {
        //message content.
        internalMsgType = INTERNAL_MESSAGE_CATCHSCREEN_NOTIFY;
    }
public:
    std::string m_strPicPath;
};


class P2PTransCallback:public InternalMessage
{
public:
    P2PTransCallback()
    {
        internalMsgType = INTERNAL_MESSAGE_P2P_TRANS_PROGRESS;

        nTaskID = 0;
        nStatus = true;
        nPercent = 0;
        nRate = 0;
        strfileName = "";
    }

public:
    UINT64 nTaskID;
    UINT8 nStatus;
    UINT32 nPercent;
    UINT32 nRate;
    std::string strfileName;
};

class FsUploadCallback:public InternalMessage
{
public:
    FsUploadCallback()
    {
        internalMsgType = INTERNAL_MESSAGE_UPLOAD_TRANS_PROGRESS;

        nTaskID = 0;
        nResult = 0;
        nPercent = 0;
    }

public:
    UINT32 nTaskID;
    int nResult;
    int nPercent;
    std::string strUrl;
    std::string strServ;
    std::string strfileName;
};

class FsDownloadCallback:public InternalMessage
{
public:
    FsDownloadCallback()
    {
        internalMsgType = INTERNAL_MESSAGE_DOWNLOAD_TRANS_PROGRESS;

        nTaskID = 0;
        nResult = 0;
        nPercent = 0;
    }

public:
    UINT32 nTaskID;
    int nResult;
    int nPercent;
    std::string strUrl;
    std::string strfileName;
};

class AjaxRequestCallBack : public InternalMessage
{
public:
    AjaxRequestCallBack()
    {
        internalMsgType = INTERNAL_MESSAGE_AJAXREQUEST_CALLBACK;

        nRequestID = 0;
        nStatusCode = 0;
        nResponseBodyLength = 0;
        pResponseBody = NULL;
    }

public:
    int nRequestID;
    int nStatusCode;
    int nResponseBodyLength;
    void* pResponseBody;
};

class ZipLogOKNotify:public InternalMessage
{
public:
    ZipLogOKNotify()
    {
        //message content.
        internalMsgType = INTERNAL_MESSAGE_ZIPLOG_NOTIFY;
    }
public:
    std::wstring m_wstrZipPath;
};

class CheckNeedUpdate : public InternalMessage
{
public:
    CheckNeedUpdate()
    {
        internalMsgType = INTERNAL_MESSAGE_NEED_UPDATE;
        m_wstrFile = L"";
    }

    BOOL	m_bNeedUpdate;
    std::wstring m_wstrFile;
};

class MqSendMessageFail: public InternalMessage
{
public:
    MqSendMessageFail(/*std::string msg*/)
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_MQ_SENDMSG_FAIL;
        from = 0x0000;
        to = 0x0000;
        appid = 0x00;
        protocoltype = 0x0;
        messageid = 0x0000;
        code = 0;
        timestamp = 0;
        convers_id = 0;
//         strMsg = msg;
    }

    virtual ~MqSendMessageFail(){}

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
        ){
            cbBuffer = sizeof(msgType) + sizeof(internalMsgType) + sizeof(from)
                + sizeof(to) + sizeof(appid) + sizeof(protocoltype) + sizeof(messageid) + sizeof(code)
                +sizeof(convers_id)+ sizeof(timestamp)/* + strMsg.length() + 1*/;
            *ppBuffer = new BYTE[cbBuffer];
            ::memset(*ppBuffer, 0, cbBuffer);
            BYTE *pTmp = *ppBuffer;
            //@
            memcpy(pTmp,&msgType,sizeof(msgType));
            pTmp += sizeof(msgType);
            //@
            memcpy(pTmp,&internalMsgType,sizeof(internalMsgType));
            pTmp += sizeof(internalMsgType);
            //@
            memcpy(pTmp,&from,sizeof(from));
            pTmp += sizeof(from);
            //@
            memcpy(pTmp,&to,sizeof(to));
            pTmp += sizeof(to);
            //@
            memcpy(pTmp,&appid,sizeof(appid));
            pTmp += sizeof(appid);
            //@
            memcpy(pTmp,&protocoltype,sizeof(protocoltype));
            pTmp += sizeof(protocoltype);
            //@
            memcpy(pTmp,&messageid,sizeof(messageid));
            pTmp += sizeof(messageid);
            //@
            memcpy(pTmp,&code,sizeof(code));
            pTmp += sizeof(code);
            //@
            memcpy(pTmp,&convers_id,sizeof(convers_id));
            pTmp += sizeof(convers_id);
            //@
            memcpy(pTmp,&timestamp,sizeof(timestamp));
//             pTmp += sizeof(timestamp);
//             //@
//             memcpy(pTmp,strMsg.c_str(),strMsg.length());

            return S_OK;
    }
    /**
    * @brief DeSerialize buffer to the class data.
    * @param [in] pMsg: input the pointer of ExternalMessage.
    * @return return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT 
        DeSerialize(
        __in BYTE* ppBuffer,
        __in size_t& cbBuffer
        ){
            //@
            memcpy(&msgType,ppBuffer,sizeof(msgType));
            ppBuffer += sizeof(msgType);
            //@
            memcpy(&internalMsgType,ppBuffer,sizeof(internalMsgType));
            ppBuffer += sizeof(internalMsgType);
            //@
            memcpy(&from,ppBuffer,sizeof(from));
            ppBuffer += sizeof(from);
            //@
            memcpy(&to,ppBuffer,sizeof(to));
            ppBuffer += sizeof(to);
            //@
            memcpy(&appid,ppBuffer,sizeof(appid));
            ppBuffer += sizeof(appid);
            //@
            memcpy(&protocoltype,ppBuffer,sizeof(protocoltype));
            ppBuffer += sizeof(protocoltype);
            //@
            memcpy(&messageid,ppBuffer,sizeof(messageid));
            ppBuffer += sizeof(messageid);
            //@
            memcpy(&code,ppBuffer,sizeof(code));
            ppBuffer += sizeof(code);
            //@
            memcpy(&convers_id,ppBuffer,sizeof(convers_id));
            ppBuffer += sizeof(convers_id);
            //@
            memcpy(&timestamp,ppBuffer,sizeof(timestamp));
//             ppBuffer += sizeof(timestamp);
//             //@
//             strMsg = std::string((char*)ppBuffer);

            return S_OK;
    }

    //message content.

    UINT32 from;
    UINT32 to;
    UINT16 appid;
    UINT8 protocoltype;
    UINT32 messageid;//本地消息id
    UINT32 code;
    UINT32 convers_id;
    UINT64 timestamp;
//     string strMsg;
};

// #define UC_MESSAGE_KEY (L"%s#%s#%d")
class MqNewMsgNotifyMessage: public InternalMessage
{
public:
    MqNewMsgNotifyMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_MQ_NEWMSG_NOTIFY;
        from = 0x0000;
        to = 0x0000;
        appid = 0x00;
        protocoltype = 0x0;
        msgid = 0;
        sessionid = 0;
    }

    virtual ~MqNewMsgNotifyMessage(){}

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
        ){
            cbBuffer = sizeof(msgType) + sizeof(internalMsgType) + sizeof(sessionid) + sizeof(msgid) + sizeof(from)
                + sizeof(to) + sizeof(appid) + sizeof(protocoltype)+ sizeof(int) +strHead.length() + sizeof(int) + strBody.length();
            *ppBuffer = new BYTE[cbBuffer];
            BYTE *pTmp = *ppBuffer;
            //@
            memcpy(pTmp,&msgType,sizeof(msgType));
            pTmp += sizeof(msgType);
            //@
            memcpy(pTmp,&internalMsgType,sizeof(internalMsgType));
            pTmp += sizeof(internalMsgType);
            //@
            memcpy(pTmp,&sessionid,sizeof(sessionid));
            pTmp += sizeof(sessionid);
            //@
            memcpy(pTmp,&msgid,sizeof(msgid));
            pTmp += sizeof(msgid);
            //@
            memcpy(pTmp,&from,sizeof(from));
            pTmp += sizeof(from);
            //@
            memcpy(pTmp,&to,sizeof(to));
            pTmp += sizeof(to);
            //@
            memcpy(pTmp,&appid,sizeof(appid));
            pTmp += sizeof(appid);
            //@
            memcpy(pTmp,&protocoltype,sizeof(protocoltype));
            pTmp += sizeof(protocoltype);
            //@
            int nMsgLen = strHead.length();
            memcpy(pTmp,&nMsgLen,sizeof(nMsgLen));
            pTmp += sizeof(nMsgLen);
            //@
            memcpy(pTmp,strHead.c_str(),nMsgLen);
            pTmp += nMsgLen;

            nMsgLen = strBody.length();
            memcpy(pTmp,&nMsgLen,sizeof(nMsgLen));
            pTmp += sizeof(nMsgLen);
            //@
            memcpy(pTmp,strBody.c_str(),nMsgLen);

            return S_OK;
    }
    /**
    * @brief DeSerialize buffer to the class data.
    * @param [in] pMsg: input the pointer of ExternalMessage.
    * @return return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT 
        DeSerialize(
        __in BYTE* ppBuffer,
        __in size_t& cbBuffer
        ){
            //@
            memcpy(&msgType,ppBuffer,sizeof(msgType));
            ppBuffer += sizeof(msgType);
            //@
            memcpy(&internalMsgType,ppBuffer,sizeof(internalMsgType));
            ppBuffer += sizeof(internalMsgType);
            //@
            memcpy(&sessionid,ppBuffer,sizeof(sessionid));
            ppBuffer += sizeof(sessionid);
            //@
            memcpy(&msgid,ppBuffer,sizeof(msgid));
            ppBuffer += sizeof(msgid);
            //@
            memcpy(&from,ppBuffer,sizeof(from));
            ppBuffer += sizeof(from);
            //@
            memcpy(&to,ppBuffer,sizeof(to));
            ppBuffer += sizeof(to);
            //@
            memcpy(&appid,ppBuffer,sizeof(appid));
            ppBuffer += sizeof(appid);
            //@
            memcpy(&protocoltype,ppBuffer,sizeof(protocoltype));
            ppBuffer += sizeof(protocoltype);
            //@
            int nMsgLen = 0;
            memcpy(&nMsgLen,ppBuffer,sizeof(nMsgLen));
            ppBuffer += sizeof(nMsgLen);
            //@
            strHead = std::string((char*)ppBuffer, nMsgLen);
            ppBuffer += nMsgLen;

            memcpy(&nMsgLen,ppBuffer,sizeof(nMsgLen));
            ppBuffer += sizeof(nMsgLen);
            //@
            strBody = std::string((char*)ppBuffer, nMsgLen);

            return S_OK;
    }

    //message content.

    UINT32 sessionid; // 根据此id分发消息到子进程
    UINT32 msgid;
    UINT32 from;
    UINT32 to;
    UINT16 appid;
    UINT8 protocoltype;
    std::string strHead;
    std::string strBody;
};

class ConnectNotifyMessage : public InternalMessage
{
public:
    ConnectNotifyMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_CONNECT_NOTIFY;
        code = 0;
    }
    UINT32                  code;         /**< 错误码 */
};

class GroupRemindNotifyMessage : public InternalMessage
{
public:
    GroupRemindNotifyMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_GROUP_REMIND_NOTIFY;
    }

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
        ){
            cbBuffer = sizeof(msgType) + sizeof(internalMsgType) + sizeof(int) +strHead.length() + sizeof(int) + strBody.length();
            *ppBuffer = new BYTE[cbBuffer];
            BYTE *pTmp = *ppBuffer;
            //@
            memcpy(pTmp,&msgType,sizeof(msgType));
            pTmp += sizeof(msgType);
            //@
            memcpy(pTmp,&internalMsgType,sizeof(internalMsgType));
            pTmp += sizeof(internalMsgType);
            //@
            int nMsgLen = strHead.length();
            memcpy(pTmp,&nMsgLen,sizeof(nMsgLen));
            pTmp += sizeof(nMsgLen);
            //@
            memcpy(pTmp,strHead.c_str(),nMsgLen);
            pTmp += nMsgLen;

            nMsgLen = strBody.length();
            memcpy(pTmp,&nMsgLen,sizeof(nMsgLen));
            pTmp += sizeof(nMsgLen);
            //@
            memcpy(pTmp,strBody.c_str(),nMsgLen);

            return S_OK;
    }
    /**
    * @brief DeSerialize buffer to the class data.
    * @param [in] pMsg: input the pointer of ExternalMessage.
    * @return return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT 
        DeSerialize(
        __in BYTE* ppBuffer,
        __in size_t& cbBuffer
        ){
            //@
            memcpy(&msgType,ppBuffer,sizeof(msgType));
            ppBuffer += sizeof(msgType);
            //@
            memcpy(&internalMsgType,ppBuffer,sizeof(internalMsgType));
            ppBuffer += sizeof(internalMsgType);
            //@
            int nMsgLen = 0;
            memcpy(&nMsgLen,ppBuffer,sizeof(nMsgLen));
            ppBuffer += sizeof(nMsgLen);
            //@
            strHead = std::string((char*)ppBuffer, nMsgLen);
            ppBuffer += nMsgLen;

            memcpy(&nMsgLen,ppBuffer,sizeof(nMsgLen));
            ppBuffer += sizeof(nMsgLen);
            //@
            strBody = std::string((char*)ppBuffer, nMsgLen);

            return S_OK;
    }

    std::string strHead;
    std::string strBody;
};


class MeetingRemindNotifyMessage : public InternalMessage
{
public:
    MeetingRemindNotifyMessage()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNAL_MESSAGE_MEETING_REMIND_NOTIFY;
    }

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
        ){
            cbBuffer = sizeof(msgType) + sizeof(internalMsgType) + sizeof(int) +strHead.length() + sizeof(int) + strBody.length();
            *ppBuffer = new BYTE[cbBuffer];
            BYTE *pTmp = *ppBuffer;
            //@
            memcpy(pTmp,&msgType,sizeof(msgType));
            pTmp += sizeof(msgType);
            //@
            memcpy(pTmp,&internalMsgType,sizeof(internalMsgType));
            pTmp += sizeof(internalMsgType);
            //@
            int nMsgLen = strHead.length();
            memcpy(pTmp,&nMsgLen,sizeof(nMsgLen));
            pTmp += sizeof(nMsgLen);
            //@
            memcpy(pTmp,strHead.c_str(),nMsgLen);
            pTmp += nMsgLen;

            nMsgLen = strBody.length();
            memcpy(pTmp,&nMsgLen,sizeof(nMsgLen));
            pTmp += sizeof(nMsgLen);
            //@
            memcpy(pTmp,strBody.c_str(),nMsgLen);

            return S_OK;
    }
    /**
    * @brief DeSerialize buffer to the class data.
    * @param [in] pMsg: input the pointer of ExternalMessage.
    * @return return S_OK if successful, otherwise return error code.
    */
    virtual HRESULT 
        DeSerialize(
        __in BYTE* ppBuffer,
        __in size_t& cbBuffer
        ){
            //@
            memcpy(&msgType,ppBuffer,sizeof(msgType));
            ppBuffer += sizeof(msgType);
            //@
            memcpy(&internalMsgType,ppBuffer,sizeof(internalMsgType));
            ppBuffer += sizeof(internalMsgType);
            //@
            int nMsgLen = 0;
            memcpy(&nMsgLen,ppBuffer,sizeof(nMsgLen));
            ppBuffer += sizeof(nMsgLen);
            //@
            strHead = std::string((char*)ppBuffer, nMsgLen);
            ppBuffer += nMsgLen;

            memcpy(&nMsgLen,ppBuffer,sizeof(nMsgLen));
            ppBuffer += sizeof(nMsgLen);
            //@
            strBody = std::string((char*)ppBuffer, nMsgLen);

            return S_OK;
    }

    std::string strHead;
    std::string strBody;
};

class Http_Response_Message : public InternalMessage
{
public:
    Http_Response_Message()
    {
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
        internalMsgType = INTERNLA_MESSAGE_HTTP_RESPONSE;
    }

    int index;
    int status_code;
    std::string strResponse;
};

#pragma pack()