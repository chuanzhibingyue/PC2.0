/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function (uc) {
    uc.interfaces.IMeetingService = {
        createMeeting: function (/*JSON*/participants, /*String*/title, /*String*/location,
        /*String*/summary, /*String*/timeZone, hostName, /*Int*/startTime,
        /*Int*/duration,/*Int*/ authorizerId, /*object*/ cycleRole,
        /*Int*/isGNet, /*boolean*/isCycle, /*boolean*/isWholeDay, /*Int*/language, /*Int*/emailLanguage, /*Function*/callback) { },

        updateMeeting: function (/*Int*/eventId, ori_start,
         /*JSON*/participants, /*String*/title, /*String*/location,
         /*String*/summary, /*String*/timeZone, /*Int*/startTime,
         /*Int*/duration,/*Int*/ authorizerId, /*object*/ cycleRole,
         /*Int*/isGNet, /*boolean*/isCycle, /*boolean*/isWholeDay, /*Int*/language, /*Int*/emailLanguage, /*Function*/callback) { },

        cancelMeeting: function (/*Int*/eventId, /*Int*/startTime, /*Int*/authorizerId, language, emailLanguage,/*Function*/callback) { },

        getMeetingStartURLSync: function (/*Int*/confId, /*Int*/password) { },

        //getMeetingStartUrlByConfId: function (/*Int*/confId, /*Int*/password, /*Function*/callback) { },

        getMeetingWebStartUrl: function (/*Int*/eventId, /*String*/userAccount,
        /*String*/password, /*String*/userName, /*Function*/callback) { },
        
		getMeetingInfoById: function (eventId, startTime, endTime, authorizerId, callback){},
		
        getMeetingInfoByIdSync: function (eventId, startTime, endTime, accreditId) { },

        getMeetingInfoByIds: function (eventIds, startTime, endTime, accreditId) { },

        getMeetingList: function (startTime, endTime, limit, authorizerId, eventIdFilter, bfilterAsc, refreshflag, callback) { },
		
		getMeetingListSync: function (startTime, endTime, limit, authorizerId, eventIdFilter, bfilterAsc, refreshflag) { },

        acceptMeetingInvitation: function (eventId, startTime, authorizerId, callback) { },

        rejectMeetingInvitation: function (eventId, startTime, authorizerId, callback) { },

        getAccreditUserList: function (callback) { },

        updateAccreditSetting: function (shareUserId, type, callback) { },

        forwardMeeting: function (eventId, /*JSON*/participants, startTime, /*Int*/shareUserId, language, emailLanguage, /*Function*/callback) { },

        getUserScheduleList: function (users, startTime, endTime, interval, eventIdFilter, callback) { },

        createMinutes: function(eventId, startTime, title, minutes, callback) { },

        updateMinutes: function (eventId,minutesId, title, minutes, callback) { },

        getMinutesDetails: function (eventId,minutesId, callback) { },

        getMinutesList: function (eventId, startTime, callback) { },

        /** Message **/
        resendMessage: function (msgId, callback) { },

        forwardMessage: function (jid, msgId, callback) { },

        getChatHistory: function (groupId, count, startTime, endTime, callback) { },

        sendTextMessage: function (eventId, /*conferenceId,*/ startTime, groupJid, textType, textFormat, textData, callback) { },

        sendImageMessage: function (eventId, /*conferenceId,*/  startTime, groupJid, imageName, imageSize, imageURL, imageData, callback) { },

        sendAudioMessage: function (eventId, /*conferenceId,*/  startTime, groupJid, audioName, audioSize, audioURL, audioDuration, callback) { },

        sendVideoMessage: function (eventId, /*conferenceId,*/  startTime, groupJid, videoName, videoSize, videoURL, videoData, videoDuration, callback) { },

        startMeetingClient: function (meetingStartUrl, callback) { },

        checkMeetingClient: function () { },
        /**
         * get meeting client status when join meeting
         * @return  MeetingClient: {visible: 0/1}       0: meeting client is not visible, 1: meeting client is visible
         */
        getMeetingClientLaunchStatus: function(){},
        deleteMeetingRecording:function(eventId, confId, startTime, callback){ },
        shareMeetingRecording:function(videoURL,videoThumbUrl, videoName,videoSize,videoDuration, summary,callback){},
		getBriefMeetingList: function (startTime, endTime, authorizerId, callback) {},
		getBriefChangedMeetingList: function (startTime, authorizerId, callback) {},
        isInMeeting: function (callback) {},
        killMeetingClient: function () {},
        cancelJoinMeeting: function (callback) {},
        /**
         * sendMeetingClientPosition
         * @param  x = topLeft.x  y = topLeft.y
         */
        sendMeetingClientPosition: function (x, y, callback) {},
	    uploadMeetingFile:function(eventId, operation, fileName, fileSize, fileURL, callback) {},
	    deleteMeetingFile:function(minutesId, callback){},
        uploadCloudFile: function (eventId, operation, mountId, fileName, fileSize, cloudType, hashCode, detailContent, callback) {},
        /**
         * back out meeting message
         * @param groupId
         * @param eventId
         * @param seq
         * @param callback
         */
        revocationMeetingMessage: function(groupId, eventId, seq, callback){},

        /**
         * get user's busy free status
         * @param userIds       user id array
         * @param startTime     start time, unix time
         * @param endTime       end time, unix time
         * @param authorizerId  authorizer id
         * @param callback      
         */
        getUserBusyFreeStatusInfo: function (userIds, startTime, endTime, authorizerId, callback) {}
    }
})(uc);