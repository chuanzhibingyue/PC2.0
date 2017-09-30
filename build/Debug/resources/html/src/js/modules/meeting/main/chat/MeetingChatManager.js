/**
 * Created by waouyang on 16/4/26.
 */
(function (uc) {
    var _this = null;
    uc.modules.meeting.MeetingChatManager = {
        meetingChat: null,

        init: function (parentNode, meetingInfo) {
            //this.meetingChat = new uc.modules.meeting.MeetingChat();
            this.meetingChat = uc.modules.chat.ChatManager.newMeetingChat(uc.constants.UI.ModuleId.Meeting);
            this.meetingChat.init(
                meetingInfo.groupId, meetingInfo.conferenceId,
                meetingInfo.eventId, meetingInfo.startTime,
                parentNode
            );
            this.meetingChatInitialized = true;
        },

        showMeetingChat: function (parentNode, meetingInfo) {
            this.init(parentNode, meetingInfo);
            this.meetingChatShown = true;
        },

        hideMeetingChat: function(){
            this.meetingChatShown = false;
            if (this.meetingChat) {
                this.meetingChat.clear();
                this.meetingChat = null;
            }
        },

        OnFileUploading: function (errorCode, taskId, percent) {
            var moduleId = uc.modules.meeting.MeetingMainManager.moduleId;
            if (this.meetingChatInitialized && moduleId == uc.constants.UI.ModuleId.Meeting) {
                uc.modules.chat.ChatManager.noticeFileUploading(errorCode, taskId, percent);
            }
        },

        OnMeetingChatHistoryReceived: function(errorCode, groupId, msgCount, minTimestamp){
        },

        OnMeetingTextMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime, textFormat){
        },

        OnMeetingImageMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime){
        },

        OnMeetingAudioMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq,msgTime){
        },

        OnMeetingVideoMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, msgTime){
        }

    };
})(uc);