/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.modules.conversation.ConversationInfo = function(){
        this.conversationId = 0;
        this.conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
        this.conversation = 0;
        this.title = '';
        this.message = '';
        this.draftMesage = '';
        this.sendingMessageStack = [];
        this.meetingId = 0;
        this.eventId = 0;
        this.meetingStartTime = 0;
        this.accreditUserId = 0;
        this.isGNet = 0;
        this.isMeetingCancelled = 0;
        this.isCancelled = 0;
        this.newMsgCount = 0;
        this.chatMsgCount = 0;
        this.timestamp = 0;
        this.msgSeq = 0;
        this.msgId = 0;
        this.hidden = false;
        this.avatarIdArray = [];
        this.avatarUrlArray = [];
        this.selected = false;
        this.fakeTime = 0;
        this.isFakeTime = false; //If true, timestamp is only for conversation list order display,
                                  // and will not show the time in ConversationView
        this.isInCall = false;
        this.disturbStatus = false;
        this.topIndex = 0;//conversation topIndex : -1 notTopindex  0 ~ n*1 isTopindex
    };
    jQuery.extend(uc.modules.conversation.ConversationInfo.prototype, {
        init: function(options){
            this.conversationId = options.conversationId;
            this.conversationType = options.conversationType ? options.conversationType : this.conversationType;
            this.title = options.title ? options.title : this.title;
            this.conversation = options.conversation ? options.conversation : this.conversation;
            this.message = options.message ? options.message : this.message;
            this.draftMesage = options.draftMesage ? options.draftMesage : this.draftMesage;
            this.sendingMessageStack = options.sendingMessageStack ? options.sendingMessageStack : this.sendingMessageStack;
            this.meetingId = options.meetingId ? options.meetingId : this.meetingId;
            this.eventId = options.eventId ? options.eventId : this.eventId;
            this.meetingStartTime = options.meetingStartTime ? options.meetingStartTime : this.meetingStartTime;
            this.accreditUserId = options.accreditUserId ? options.accreditUserId : this.accreditUserId;
            this.isGNet = options.isGNet ? options.isGNet : this.isGNet;
            this.isMeetingCancelled = options.isMeetingCancelled ? options.isMeetingCancelled : this.isMeetingCancelled;
            this.isCancelled = options.isCancelled ? options.isCancelled : this.isCancelled;
            this.newMsgCount = options.newMsgCount ? options.newMsgCount : this.newMsgCount;
            this.chatMsgCount = options.chatMsgCount ? options.chatMsgCount : this.chatMsgCount;
            this.timestamp = options.timestamp ? options.timestamp : this.timestamp;
            this.msgSeq = options.msgSeq ? options.msgSeq : this.msgSeq;
            this.msgId = options.msgId ? options.msgId : this.msgId;
            this.hidden = options.hidden ? options.hidden : this.hidden;
            this.avatarIdArray = options.avatarIdArray ? options.avatarIdArray : this.avatarIdArray;
            this.avatarUrlArray = options.avatarUrlArray ? options.avatarUrlArray : this.avatarUrlArray;
            this.fakeTime = options.fakeTime ? options.fakeTime : this.fakeTime;
            this.isFakeTime = options.isFakeTime ? options.isFakeTime : this.isFakeTime;
            this.isInCall = options.isInCall ? options.isInCall : this.isInCall;
            this.meetingMsgType = options.meetingMsgType || uc.constants.Meeting.MsgType.detail;
            this.disturbStatus = options.disturbStatus || this.disturbStatus;
            this.atCounts = options.atCounts || this.atCounts;
            this.topIndex = options.topIndex || this.topIndex;
        },
        setSelected: function(selected){
            this.selected = selected;
        },
        update: function(options){
            this.conversation = options.conversation ? options.conversation : this.conversation;
            this.newMsgCount = options.newMsgCount ? options.newMsgCount : this.newMsgCount;
            this.chatMsgCount = options.chatMsgCount ? options.chatMsgCount : this.chatMsgCount;
            this.hidden = options.hidden ? options.hidden : this.hidden;
            this.timestamp = options.timestamp > this.timestamp ? options.timestamp : this.timestamp;
            this.msgSeq = options.msgSeq > this.msgSeq ? options.msgSeq : this.msgSeq;
            this.msgId = options.msgId > this.msgId ? options.msgId : this.msgId;
            this.atCounts = options.atCounts || this.atCounts;
            this.topIndex = options.topIndex || this.topIndex;
        },
        updateData: function(updateData){
            this.message = updateData.message ? updateData.message : this.message;
            this.title = updateData.title ? updateData.title : this.title;
            this.meetingId = updateData.meetingId ? updateData.meetingId : this.meetingId;
            this.eventId = updateData.eventId ? updateData.eventId : this.eventId;
            this.meetingStartTime = updateData.meetingStartTime ? updateData.meetingStartTime : this.meetingStartTime;
            this.accreditUserId = updateData.accreditUserId ? updateData.accreditUserId : this.accreditUserId;
            this.isGNet = updateData.isGNet ? updateData.isGNet : this.isGNet;
            this.isMeetingCancelled = updateData.isMeetingCancelled ? updateData.isMeetingCancelled : this.isMeetingCancelled;
            this.timestamp = updateData.timestamp ? updateData.timestamp : this.timestamp;
            this.isFakeTime = !!updateData.isFakeTime;
            this.fakeTime = updateData.fakeTime ? updateData.fakeTime : this.fakeTime;
            this.newMsgCount = updateData.newMsgCount == undefined ? this.newMsgCount : this.newMsgCount + updateData.newMsgCount;
            this.chatMsgCount = updateData.chatMsgCount == undefined ? this.chatMsgCount : this.chatMsgCount + updateData.chatMsgCount;
            this.meetingMsgType = updateData.meetingMsgType || uc.constants.Meeting.MsgType.detail;
        }
    })
})(uc);