/**
 * Created by waouyang on 16/4/26.
 */
(function (uc) {
    var _this = null;
    uc.modules.meeting.MeetingDetailManager = {
        meetingDetail: null,

        init: function () {
            _this = this;
            if(!this.meetingDetailInitialized){
                _this.meetingDetail = uc.modules.meeting.MeetingDetail;
                _this.meetingDetail.init();
                this.meetingDetailInitialized = true;
            }
        },

        initMeetingList: function () {

        },

        joinMeeting: function (conferenceId, eventId, startTime, authorizerId) {
           
        },

        updateMeetingInfo: function (meetingInfo) {
            uc.util.LogUtil.info('MeetingDetailManager', 'updateMeetingInfo', 'meetingInfo', meetingInfo);
            if (meetingInfo && this.meetingDetail) {
                this.meetingDetail.updateMeetingInfo(meetingInfo);
            }
        },

        cancelMeeting: function (eventId) {
            uc.util.LogUtil.info('MeetingDetailManager', 'cancelMeeting', 'eventId', eventId);
            if (eventId && this.meetingDetail) {
                this.meetingDetail.cancelMeeting(eventId);
            }
        },

        OnInitClient: function (clearData) {
            this.init();
        },

        OnDateChanged: function () {
            this.meetingDetail.refresh();
        },

        showMeetingChat: function (meetingInfo) {
            this.meetingDetail.showMeetingChat();
        },

        OnShowMeetingDetail: function (meetingInfo, isFromMeetingList) {
            this.meetingDetail.showMeetingDetail(meetingInfo, isFromMeetingList);
        },

        OnMeetingCanceled: function (/*int*/errorCode, eventId, startTime, authorizerId) {
            if (errorCode != uc.ErrorCode.Success) {
                return;
            }
        },

        OnWaitJoinMeeting: function () {
            if (this.meetingDetail) {
                this.meetingDetail.OnWaitJoinMeeting();
            }
        },

        OnMeetingJoined: function () {
            if (this.meetingDetail) {
                this.meetingDetail.OnMeetingJoined();
            }
        },

        OnJoinMeetingFailed: function (errorCode) {
            if (this.meetingDetail) {
                this.meetingDetail.OnJoinMeetingFailed(errorCode);
            }
        },

        OnMeetingJoinFailed: function () {
            if (this.meetingDetail) {
                this.meetingDetail.OnMeetingJoined();
            }
        },

        OnCancelJoinMeeting: function () {
            if (this.meetingDetail) {
                this.meetingDetail.OnMeetingJoined();
            }
        },

        OnMeetingUpdateMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS && accreditUserId != uc.modules.meeting.MeetingListManager.authorizerId) {
                return;
            }
            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, meetingStartTime, 0, accreditUserId);

            if (meetings && meetings.length) {
                this.updateMeetingInfo(meetings[0]);
            }
        },

        OnMeetingCancelMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS && accreditUserId != uc.modules.meeting.MeetingListManager.authorizerId) {
                return;
            }

            this.cancelMeeting(eventId);
        },

        OnMeetingRejectMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }

            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, meetingStartTime, 0, accreditUserId);
            var userInfo = uc.IContactService.getCurrentUserInfo();

            if (meetings && meetings.length) {
                var meeting = meetings[0];

                if (accreditUserId == uc.constants.Meeting.AuthorizeMySelf && accreditUserId == uc.modules.meeting.MeetingListManager.authorizerId) {
                    if (userInfo.userId != meeting.hostId) {
                        this.cancelMeeting(eventId);
                    } else {
                        this.updateMeetingInfo(meeting);
                    }
                }
                else {
                    if (uc.modules.meeting.MeetingListManager.authorizerId == uc.constants.Meeting.AuthorizeMySelf) {
                        if (userInfo.userId != meeting.hostId) {
                            this.cancelMeeting(eventId);
                        }
                    }
                    else {
                        if (uc.modules.meeting.MeetingListManager.authorizerId != meeting.hostId) {
                            this.cancelMeeting(eventId);
                        }
                    }
                }
            }
        },

        OnMeetingInvitationRejected:function(errorCode, eventId, startTime, authorizerId) {
            if (errorCode !== uc.ErrorCode.Success) {
                return;
            }

            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, meetingStartTime, 0, authorizerId);
            var userInfo = uc.IContactService.getCurrentUserInfo();

            if (meetings && meetings.length) {
                var meeting = meetings[0];

                if (authorizerId == uc.constants.Meeting.AuthorizeMySelf && authorizerId == uc.modules.meeting.MeetingListManager.authorizerId) {
                    if (userInfo.userId != meeting.hostId) {
                        this.cancelMeeting(eventId);
                    } else {
                        this.updateMeetingInfo(meeting);
                    }
                }
                else {
                    if (authorizerId != meeting.hostId) {
                        this.cancelMeeting(eventId);
                    }
                }
            }
        },

        OnMeetingAcceptMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }

            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, meetingStartTime, 0, uc.modules.meeting.MeetingListManager.authorizerId);

            if (meetings && meetings.length) {
                this.updateMeetingInfo(meetings[0]);
            }
        },

        OnMeetingInvitationAccepted: function (errorCode, eventId, startTime, authorizerId) {
            if (errorCode == uc.ErrorCode.Success && authorizerId == uc.modules.meeting.MeetingListManager.authorizerId) {
                var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, startTime, 0, authorizerId);

                if (meetings && meetings.length) {
                    this.updateMeetingInfo(meetings[0]);
                }
            }
        },

	    //检查会议室是否可用事件
	    OnCheckJoinMeetingRoom: function (errorCode, roomId, hasPassword, isValid, mayJoin) {
		    if (this.meetingDetail && uc.modules.main.Main.moduleId == uc.constants.UI.ModuleId.Meeting ) {
			    this.meetingDetail.OnCheckJoinMeetingRoom(errorCode, roomId, hasPassword, isValid, mayJoin);
		    }
	    }
    };
})(uc);