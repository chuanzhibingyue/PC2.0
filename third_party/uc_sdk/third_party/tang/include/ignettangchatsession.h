#ifndef IGNetTangChatrSession_H
#define IGNetTangChatrSession_H

#include "ignettangbase.h"

//#define  DISP_CHAT_MESSAGE_TIMESTAMP                    "timeStamp"
//#define  DISP_CHAT_MESSAGE_SENDER_NAME                  "senderName"
//#define  DISP_CHAT_MESSAGE_RECEIVER_NAME                "receiverName"
//#define  DISP_CHAT_MESSAGE_CONTENT                      "content"

class IGNetTangChatMessage:public IGNetTangProperty
{
public:
    virtual CGNetTangVariant getProperty(const char* propName) = 0;
};

class IGNetTangChatSessionSink:public IGNetTangBaseSessionSink
{
public:
//        virtual void onPropertyChanged(const char *propName,
//                const CGNetTangVariant &oldPropValue,
//                const CGNetTangVariant &newPropValue) = 0;
//        virtual void onItemAdded(const char *itemDataType, IGNetTangProperty *pItemInstance) = 0;
    virtual void onChatPropertyChanged(const char *propName,
            const CGNetTangVariant &oldPropValue,
            const CGNetTangVariant &newPropValue) = 0;
    virtual void onChatMessageReceived(IGNetTangChatMessage* pMessage) = 0;
};

class IGNetTangChatSession:public IGNetTangBaseSession
{
public:
    virtual int SendMessage(const char* qstrDestUsers, const char* qstrMessage) =0;
};


#endif // IGNetTangChatrSession_H

