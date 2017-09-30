/**
 * Created by waouyang on 15/9/15.
 */
(function(uc){
    uc.interfaces.IOpenApiEvent = {
        OnAppListReceived: function(errorCode, apps){},
        OnGetAppInfoReceived: function(errorCode, apps) {},
        OnApiChatHistoryReceived: function(errorCode, appId, msgCount, minTimestamp){},
        OnOpenApiTextMessageReceived: function(channel, sendState, from, to, textTitle, textContent, textType, detailContent, msgId, msgSeq, mstTime){},
        OnOpenApiImageMessageReceived: function(channel, sendState, from, to, imageId, imageType, imageData, msgId, msgSeq, msgTime){},
        OnOpenApiFileMessageReceived: function(channel, sendState, from, to, fileId, fileType, fileName, fileSize, msgId, msgSeq, mstTime){},
        OnOpenApiMessageSent:function(errorCode, appId, msgId, msgSeq, msgTime){},
        OnOAMessageStatusReceived: function(errorCode, appId, msgSeq, status) {},
        OnOAMessageReceived: function(channel, from, to, title, titleColor, titleBkColor, titleElements, status, data, detailUrl, msgId, msgSeq,msgTime, detailAuth, oaExternalData, titleStyle,customizedType,customizedData) {},
        OnOAMessageStatusChanged: function(appId, msgSeq, status, from, externalData) {},
        OnPushMessageAck: function(channel, errorCode, from, to, conversation, msgId, msgSeq, msgTime){},
        OnMessageRead: function (errorCode, userId, readType, msgId, msgSeq) {},
        OnAppStatusChangeReceived: function (channel, appId, status, extraData) {},
        OnAppUntreadedReceived: function (channel, appId, eventCount, extraData) {},
        OnOACustomizedMessageReceived:function(channel,from,data1,data2,data3,data4){}
    }
})(uc);