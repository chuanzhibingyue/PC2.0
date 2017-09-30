/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.interfaces.IIMEvent = {
        /**
         * event when receive chat history from SDK
         * @param errorCode     error code
         * @param userId        user id of the chat
         * @param messageList   message array
         * @constructor
         */
        OnChatHistoryReceived: function(errorCode, userId, messageList, msgCount, minTimestamp){},
        /**
         * event when message is sent to server
         * @param errorCode     error code
         * @param userId        user id of the chat
         * @param msgId         message id created in SDK
         * @param msgSeq        message sequence returned from server
         * @param msgTime       message timestamp
         * @constructor
         */
        OnMessageSent: function(errorCode, userId, msgId, msgSeq, msgTime){},
        OnTextMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, textType, textData, sendTime, timestamp, textFormat){},
        OnImageMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp){},
        OnAudioMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, audioName, audioSize, audioURL, audioDuration, timestamp){},
        OnVideoMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, videoName, videoSize, videoURL, videoThumb, duration, timestamp){},
        OnFileReceived: function(channel, sendState, from, to, msgId, msgSeq,fileName, fileSize, fileURL, timestamp,localPath){},
        OnCodeReceived: function(channel, sendState, from, to, msgId, msgSeq, creatorId, creatorName, codeId, codeTitle,
                                 langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp){},
        OnFileShared: function(jid, msgId, msgSeq, fileName, fileSize, fileURL, fileId){},
        OnFileDeleted: function(jid, msgId, msgSeq, fileName, fileId){},
        OnMessageRead: function(errorCode, userId, readType, msgId, msgSeq){},
        OnMessageMediaPlayed: function(errorCode, userId, msgId, msgSeq){},
        OnIMUnreadUserCountEvent:function(errorCode,userId,info){},

        //Call related
        OnP2PCallInvitationReceived: function(channel, userId,fromUserId, confId, confPwd, hostId, msgId, msgSeq, timestamp){},
        OnP2PCallInvitationAccepted: function(channel, userId,fromUserId, confId, msgId, msgSeq, timestamp){},
        OnP2PCallInvitationRejected: function(channel, userId,fromUserId, confId, hostId, reason, msgId, msgSeq, timestamp,sendState){},
        OnP2PCallStopped: function(channel, userId, confId, msgId, msgSeq, timestamp){},
        OnP2PCallCancelled: function(channel, userId,fromUserId, msgId, msgSeq, timestamp){},
        OnP2PCallNoResponse: function(channel, userId, msgId, msgSeq, timestamp){},
        OnP2PCallDurationReceived: function(channel, userId, fromUserId,duration, msgId, msgSeq, timestamp,sendState){},

        OnIMSummaryVideoMessageReceived:function(channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, fileName, startTime, updateTime,type,videoName,fileURL,thumbnails,thumbnailsURL,duration,fileSize,operatorName,operatorAvatar,msgId, msgSeq, msgTime){},
        OnCloudFileReceived: function(channel, sendState, from, to, msgId, msgSeq,fileName, fileSize, timestamp,detailContent){},
        /**
         * event for received external message.
         * @param errorCode
         * @param conversationType
         * @param relateID
         * @param msgCount
         * @param minTimestamp
         * @param messages
         */
        OnExternalMessageReceived: function(errorCode, conversationType, relateID, msgCount, minTimestamp, messages) {},

        OnIMReportMessageReceived: function (channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {}
    }
})(uc);