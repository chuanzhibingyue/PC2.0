/**
 * Created by xujiansong on 16/3/16.
 */
(function (uc) {
    uc.modules.historymsg.HistoryEventListener = function () {

    };
    jQuery.extend(uc.modules.historymsg.HistoryEventListener.prototype,
        uc.interfaces.IClientEvent,
        uc.interfaces.IUIEvent,
        uc.interfaces.IIMEvent,
        uc.interfaces.IGroupEvent,
        uc.interfaces.IMeetingEvent,
        uc.interfaces.ISearchEvent,
        uc.interfaces.IFSEvent,{
            //IM events
            OnChatHistoryReceived: function (errorCode, userId, messages, msgCount, minTimestamp) {
              	var conversationType =null;
                uc.modules.historymsg.HistoryManager.getChatHistoryReceived(errorCode, conversationType,userId, messages, msgCount, minTimestamp);
            },
            OnTextMessageReceived: function (channel, sendState, from, to, msgId, msgSeq, textType, textData, sendTime, timestamp, textFormat) {
               	var conversationType =null;
                uc.modules.historymsg.HistoryManager.getTextMessageReceived(conversationType,channel, sendState, from, to, msgId, msgSeq, textType, textData, sendTime, timestamp, textFormat);
            },
            OnImageMessageReceived: function (channel, sendState, from, to, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp) {
              	var conversationType =null;
                uc.modules.historymsg.HistoryManager.getImageMessageReceived(conversationType,channel, sendState, from, to, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp);
            },
            OnFileReceived: function (channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, timestamp,localPath) {
            	var conversationType =null;
            	var operatorId =null;
            	var fileId =null;
            	var fileFormat =null;
            	var desc =null;
            	var username =null;
              uc.modules.historymsg.HistoryManager.getFiledMessageReceived(conversationType,channel, sendState, from, to, operatorId,fileId,fileName,fileSize,fileURL,fileFormat,desc,msgId,msgSeq, timestamp, username,localPath);
            },
            
            
            //group events 
            OnGroupChatHistoryReceived: function (errorCode, conversationType, groupId, messages, msgCount, minTimestamp) {
                uc.modules.historymsg.HistoryManager.getChatHistoryReceived(errorCode, conversationType, groupId, messages, msgCount, minTimestamp);
            },
            OnGroupTextMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, msgTime, textFormat, atUsers) {
               //uc.constants.Message.MessageChannel.History==channel
                uc.modules.historymsg.HistoryManager.getTextMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, msgTime, textFormat, atUsers);
            },
            OnGroupImageMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime) {
                uc.modules.historymsg.HistoryManager.getImageMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime);
            },
            OnDocumentUploadMessageReceived: function(conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username,localPath){
                uc.modules.historymsg.HistoryManager.getFiledMessageReceived(conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username,localPath);
            },
             
             //serachHistroyMsgByKeyWord 
            OnServerSearchResultReceived:function(errorCode, searchId, contacts, projects, groups, cloudGroups, meetings, messages,chatMessages,chatMsgNum)
            {
            	//chatMsgNum searchAllCountBykeyword
	               uc.modules.historymsg.HistoryManager.GetHistoryListByKeyWord(errorCode,searchId,chatMessages,chatMsgNum);
            },
            //show searchHistroy  NextResult  
            OnSearchNextResultReceived:function(errorCode, conversationType,groupId, msgCount,minmsgtime,maxmsgTime)
            {
            	  uc.modules.historymsg.HistoryManager.GetHistoryByNexeResult(errorCode, conversationType,groupId, msgCount,minmsgtime,maxmsgTime);
            },
            //file
            OnFileDownloading: function (errorCode, taskId, percent) {
                var fromDomObj = "."+uc.modules.historymsg.Page;
                uc.modules.chat.ChatManager.noticeFileDownloading(errorCode, taskId, percent,fromDomObj);
            },
            OnFileDownloaded: function (errorCode, taskId, savePath) {
                var fromDomObj = "."+uc.modules.historymsg.Page;
                uc.modules.chat.ChatManager.noticeFileDownloaded(errorCode, taskId, savePath,fromDomObj);
            },
            //metting  add by xjs 2016-3-26
            OnMeetingChatHistoryReceived: function(errorCode, groupId, msgCount, minTimestamp){
                var conversationType =null;
                var messages =null;
                uc.modules.historymsg.HistoryManager.getChatHistoryReceived(errorCode, conversationType, groupId, messages, msgCount, minTimestamp);
            },
            OnMeetingTextMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime, textFormat) {
                if (!uc.modules.historymsg.HistoryManager.isQueryMeetingChat(eventId)) {
                    return;
                }
                var atUsers = '';
                var conversationType = null;
                uc.modules.historymsg.HistoryManager.getTextMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, msgTime, textFormat, atUsers);
            },
            OnMeetingImageMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime) {
                if (!uc.modules.historymsg.HistoryManager.isQueryMeetingChat(eventId)) {
                    return;
                }
                var conversationType = null;
                uc.modules.historymsg.HistoryManager.getImageMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime);
            },
            OnMeetingDocumentUploadMessageReceived: function (eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp, cloudType, localPath) {
                if(!uc.modules.historymsg.HistoryManager.isQueryMeetingChat(eventId))
                {
                    return;
                }
                var username =null;
                uc.modules.historymsg.HistoryManager.getFiledMessageReceived(conversationType, channel, sendState, from, to, operatorId, fileId, fileName,fileSize,  downloadUrl, fileFormat,desc,msgId,msgSeq, timestamp, username,localPath);
            },
            //CloudFile Received  add by xjs 2016-5-30
            OnGroupCloudFileMessageReceived: function(conversationType, channel, sendState, from, to, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp, username,operation,detailContent) {
                if (!detailContent) return;
            }

        })
})(uc);