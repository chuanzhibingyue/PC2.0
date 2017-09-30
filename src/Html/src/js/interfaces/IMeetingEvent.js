/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.interfaces.IMeetingEvent = {
        OnMeetingCreated: function(/*int*/errorCode,meetingInfo){},
        OnMeetingUpdated: function(/*int*/errorCode,eventId,startTime,meetingInfo){},
        OnMeetingCanceled: function(/*int*/errorCode,eventId,startTime,authorizerId){},
        //OnMeetingStartURLReceived: function(/*int*/errorCode,confId,startURL){},
        OnMeetingStartUrlByConfIdReceived: function(/*int*/errorCode,startUrl,confId){},
        OnMeetingWebStartUrlReceived:function(errorCode,startUrl,eventId){},
        OnMeetingListReceived: function(/*int*/errorCode,requestId,startTime,endTime,eventIdFilter,authorizerId,meetings){},
        OnMeetingInvitationAccepted:function(errorCode, eventId, startTime, authorizerId){},
        OnMeetingInvitationRejected:function(errorCode,eventId, startTime, authorizerId){},
        OnAccreditUserListReceived:function(errorCode,accreditUsers){},
        OnAccreditSettingUpdated:function(errorCode,shareUserId,status){},
        OnMeetingInfoReceivedById:function(errorCode, eventId,authorizerId, meetings){},
        OnMeetingInfoReceivedByIds:function(errorCode, requestId,authorizerId, meetings){},
        OnMeetingForwarded:function(errorCode,meeting){},
        OnUserScheduleListReceived:function(errorCode,requestId,schedules){},
        OnMinutesCreated:function(errorCode,eventId,startTime,minutesId,minutesTitle){},
        OnMinutesUpdated:function(errorCode,eventId,minutesId,minutesTitle){},
        OnMinutesDetailsReceived:function(errorCode,eventId,minutesId,creatorId,createTime,content,startTime){},
        OnMinutesListReceived:function(errorCode,eventId,startTime,minutes){},
        OnMeetingMessageSent: function(errorCode, groupId, eventId, meetingStartTime, msgId, msgSeq, msgTime){},
        OnMeetingChatHistoryReceived: function(errorCode, groupId, msgCount, minTimestamp){},
        OnMeetingTextMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime, textFormat){},
        OnMeetingImageMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime){},
        OnMeetingAudioMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq,msgTime){},
        OnMeetingVideoMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, msgTime){},
        OnMeetingInviteMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){},
        OnMeetingAcceptMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){},
        OnMeetingRejectMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime,updateFlag, msgId, msgSeq, msgTime){},
        OnMeetingUpdateMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime,updateFlag, msgId, msgSeq, msgTime){},
        OnMeetingForwardMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){},
        OnMeetingCancelMessageReceived: function(channel, from, to, meetingId, eventId, meetingName,groupId, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){},
        OnMinutesCreateMessageReceived: function(channel, from, to, operatorId, minutesId, minutesTitle, meetingId,eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){},
        OnMinutesUpdateMessageReceived: function (channel, from, to, operatorId, minutesId, minutesTitle, meetingId,eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime) { },
        OnMeetingAccreditMessageReceived: function(channel, from, to, msgId, msgSeq, msgTime){},
        OnMeetingAccreditCancelledMessageReceived: function(channel, from, to, msgId, msgSeq, msgTime){},
        OnMeetingReminderMessageReceived: function(meetings) {},
        OnMeetingSummaryVideoMessageReceived:function(channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, fileName, startTime, updateTime,type,videoName,fileURL,thumbnails,thumbnailsURL,duration,fileSize,operatorName,operatorAvatar,msgId, msgSeq, msgTime){},
        OnMeetingRecordingDeleted:function(errorCode,eventId,startTime){},
		OnMeetingUnreadUserCountEvent:function(errorCode,type,userId,info){},
		OnBriefMeetingListReceived: function (errorCode, requestId, startTime, endTime, authorizerId, meetings){},
		OnBriefChangedMeetingListReceived: function (errorCode, actionTime, authorizerId, meetings){},
        OnMeetingJoined: function (){},
        OnMeetingJoinFailed: function(){},
        OnMeetingClosed: function (){},
	    OnMeetingDocumentUploadMessageReceived: function (eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp, cloudType,localPath){},
	    OnMeetingFileUploaded: function(errorCode, eventId, operatorId, document, msgId){},
	    OnMeetingFileDeleted: function(errorCode){},
	    OnMeetingFileDeleteMessageReceived: function(eventId,conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime){},
        OnMeetingReportMessageReceived: function (channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {},
        OnMeetingMessageRevocation: function(errorCode, groupId, msgId, msgSeq, msgTime, revocationType){},
        OnMeetingMessageRevocationReceived: function(channel, conversation, conversationType, from, to, msgId, msgSeq, msgTime, seqId, eventId){},
        /**
         * event when user's busy free status info received
         * @param errorCode 
         * @param requestId
         * @param schedules 
         * @constructor
         */
        OnUserBusyFreeStatusInfoReceived: function (errorCode, requestId, schedules) {},
        OnMeetingReadyStatusReceived: function () {}
    }
})(uc);

