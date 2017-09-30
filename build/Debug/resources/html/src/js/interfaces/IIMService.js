/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function(uc){
    uc.interfaces.IIMService = {
        getChatHistory: function(userId, count, startTime, endTime, callback){},
        resendMessage: function (msgId, callback){},
        sendTextMessage: function(jid, textType, textFont, textData, callback){},
        sendImageMessage: function(jid, imageName, imageSize, imageURL, imageData, callback){},
        sendAudioMessage: function(jid, audioName, audioSize, audioURL, audioDuration, callback){}, 
        sendVideoMessage: function(jid, videoName, videoSize, videoURL, videoData, videoDuration, callback){}, 
        sendFileMessage: function(jid, operation, fileName, fileSize, fileURL, fileId, callback){},
        shareFileMessage: function(jid, fileName, fileSize, fileURL, fileId, callback){}, 
        deleteFileMessage: function(jid, fileName, fileId, callback){},

        sendCallInvitation: function(jid, confId, confPwd, hostId, callback){},
        acceptCallInvitation: function(jid, confId, hostId, callback){},
        rejectCallInvitation: function(jid, confId, hostId, reason, callback){},
        finishCall: function(jid, confId, callback){},
        cancelCall: function(jid,telephoneNumber, callback){},
        sendNotRespondCallMessage: function(jid, callback){},
        sendCallDurationMessage: function(jid, druation, callback){},
        sendMessageRead: function(jid, msgId, msgSeq, callback){},
        sendMessagesReadById: function(userId, callback){},
        sendMessagePlayed: function(jid, msgId, msgSeq, callback){},
        sendCodeMessage: function(jid, operation, codeId, codeTitle, langType, codeHead, codeDesc, codeSize, codeURL, codeLine, createTime, callback){},
        sendImRecordVideoMessage:function(user,videoName,videoSize,videoUrl,videoAvatar,thumbnailsURL,timeLength,operatorId,operatorName,operatorAvatar,callback){},
	    sendCloudFileMessage: function(jid, fileName, fileSize, cloudType, detailContent, callback){},
        /**
         * get external history message.
         * @param messageType
         * @param relateID
         * @param msgCount
         * @param startTime
         * @param endTime
         * @param callback
         */
        getExternalMessageHistory: function(messageType, relateID, msgCount, startTime, endTime, callback) {},

        /**
         * show external history message or not.
         */
        isShowExternalMessage: function(account) {},

        sendReportMessage: function(ids, reportTitle, reportUrl, callback) {}
    }
})(uc);


