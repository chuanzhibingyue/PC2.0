/**
 * Created by waouyang on 15/9/15.
 */
(function(uc){
    uc.interfaces.IOpenApiService = {
        getAppList: function(callback){},
        getAppInfo: function(appIds, callback) {},
        getChatHistory: function(appId, count, startTime, endTime, callback){},
        resendMessage: function(msgId, callback){},
        sendTextMessage: function(appJid, textTitle, textContent, textType, detailContent, callback){},
        sendFileMessage: function(appJid, fileId, fileType, fileName, fileSize, callback){},
        sendImageMessage: function(appJid, imageId, imageType, imageData, callback){},
        getOAMessageStatus: function(OAMessageInfos, callback) {},
        sendRequest: function(method, url, head, callback) {},
        sendMessagesReadById: function(appId, callback){},
        sendMessageRead: function (siteObj, msgid, msgSeq, callback) {}
    }
})(uc);