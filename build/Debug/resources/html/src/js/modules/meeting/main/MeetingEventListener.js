/**
 * Created by senlin.ouyang on 2015/7/23.
 */
(function (uc) {
    uc.modules.meeting.MeetingEventListener = function () {
        this.managerList = ['uc.modules.meeting.MeetingMainManager',
            'uc.modules.meeting.MeetingHeaderManager',
            'uc.modules.meeting.MeetingListManager',
            'uc.modules.meeting.MeetingDetailManager',
            'uc.modules.meeting.MeetingAttendeeManager',
            'uc.modules.meeting.MeetingChatManager',
            'uc.modules.meeting.MeetingFileManager',
            'uc.modules.meeting.JoinAnimationManager'];
    };

    jQuery.extend(uc.modules.meeting.MeetingEventListener.prototype,
        uc.interfaces.IContactEvent,
        uc.interfaces.IUIEvent,
        uc.interfaces.IMeetingEvent,
        uc.interfaces.IGroupEvent,
        uc.interfaces.IIMEvent,
        uc.interfaces.IFSEvent,
        uc.interfaces.IMeetingRoomEvent, {

            OnInitClient: function (clearData) {
                this.executeMethod('OnInitClient', arguments);
            },

            OnModuleChanged: function (moduleId, mountId, dataObj) {
                this.executeMethod('OnModuleChanged', arguments);
            },

            OnDateChanged: function () {
                this.executeMethod('OnDateChanged');
            },

            OnMeetingCreated: function (/*int*/errorCode, conferenceInfo) {
                uc.util.LogUtil.info("MeetingEventListener", "OnMeetingCreated", "data=", {
                    errorCode: errorCode,
                    meetings: conferenceInfo
                });
                if (errorCode == uc.ErrorCode.Success) {
                    this.executeMethod('OnMeetingCreated', arguments);
                }
            },

            OnMeetingUpdated: function (/*int*/errorCode, eventId, startTime, conferenceInfo) {
                uc.util.LogUtil.info("MeetingEventListener", "OnMeetingUpdated", "data=", {
                    errorCode: errorCode,
                    eventId: eventId,
                    startTime: startTime,
                    meetings: conferenceInfo
                });
                this.executeMethod('OnMeetingUpdated', arguments);
            },

            OnMeetingListReceived: function (errorCode, requestId, startTime, endTime, eventIdFilter, authorizerId, meetings) {
                uc.util.LogUtil.info("MeetingEventListener", "OnMeetingListReceived", "data = ", {
                    errorCode: errorCode,
                    requestId: requestId,
                    startTime: startTime,
                    endTime: endTime,
                    eventIdFilter: eventIdFilter,
                    authorizerId: authorizerId,
                    meetings: meetings
                });
                if (errorCode == 0 || (errorCode != 0 && meetings.length > 0)) {
                    this.executeMethod('OnMeetingListReceived', arguments);
                }
            },

            OnMeetingCanceled: function (/*int*/errorCode, eventId, startTime, authorizerId) {
                uc.util.LogUtil.info("MeetingEventListener", "OnMeetingCancelled", "data=", {
                    errorCode: errorCode,
                    startTime: startTime,
                    authorizerId: authorizerId,
                    eventId: eventId
                });
                this.executeMethod('OnMeetingCanceled', arguments);
            },

            OnMeetingWebStartUrlReceived: function (errorCode, startUrl, eventId) {
                uc.util.LogUtil.info("MeetingEventListener", "OnMeetingWebStartUrlReceived", "data=", {
                    errorCode: errorCode,
                    startUrl: startUrl,
                    eventId: eventId
                });
                this.executeMethod('OnMeetingWebStartUrlReceived', arguments);
            },

            OnShowMeetingDetail: function (meetingInfo, isFromMeetingList) {
                this.executeMethod('OnShowMeetingDetail', arguments);
            },

            OnBasicContactInfoReceived: function (errorCode, userIds, contacts) {
                if (errorCode == 0) {
                    this.executeMethod('OnBasicContactInfoReceived', arguments);
                }
            },

            OnMeetingInvitationRejected: function (errorCode, eventId, startTime, authorizerId) {
                uc.util.LogUtil.info("MeetingEventListener", "OnMeetingInvitationRejected", "data=", {
                    errorCode: errorCode,
                    eventId: eventId,
                    startTime: startTime,
                    authorizerId: authorizerId
                });

                if (errorCode == 0) {
                    this.executeMethod('OnMeetingInvitationDenied', arguments);
                }
            },

            OnMeetingInvitationAccepted: function (errorCode, eventId, startTime, authorizerId) {
                uc.util.LogUtil.info("MeetingEventListener", "OnMeetingInvitationAccepted", "data=", {
                    errorCode: errorCode,
                    eventId: eventId,
                    startTime: startTime,
                    authorizerId: authorizerId
                });

                if (errorCode == 0) {
                    this.executeMethod('OnMeetingInvitationAccepted', arguments);
                }
            },

            OnMeetingForwarded: function (errorCode, meeting) {
                uc.util.LogUtil.info("MeetingEventListener", "OnMeetingForwarded", "data=", {
                    errorCode: errorCode,
                    meetings: meeting
                });

                if (errorCode == 0) {
                    this.executeMethod('OnMeetingForwarded', arguments);
                }
            },

            OnCreateMeeting: function (uniqueName, data) {
                this.executeMethod('OnCreateMeeting', arguments);
            },

            OnMinutesListReceived: function (errorCode, eventId, startTime, minutes) {
                if (errorCode == 0) {
                    this.executeMethod('OnMinutesListReceived', arguments);
                }
            },

            OnMinutesCreated: function (errorCode, eventId, startTime, minutesId, minutesTitle) {
                if (errorCode == 0) {
                    this.executeMethod('OnMinutesCreated', arguments);
                }
            },

            OnMinutesUpdated: function (errorCode, eventId, minutesId, minutesTitle) {
                if (errorCode == 0) {
                    this.executeMethod('OnMinutesUpdated', arguments);
                }
            },

            OnMinutesCreateMessageReceived: function (channel, from, to, operatorId, minutesId, minutesTitle, meetingId, eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime) {
                if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                    return;
                }
                this.executeMethod('OnMinutesCreateMessageReceived', arguments);
            },

            OnMinutesUpdateMessageReceived: function (channel, from, to, operatorId, minutesId, minutesTitle, meetingId, eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime) {
                if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                    return;
                }
                this.executeMethod('OnMinutesUpdateMessageReceived', arguments);
            },

            OnAccreditUserListReceived: function (errorCode, accreditUsers) {
                uc.util.LogUtil.info("MeetingEventListener", "OnAccreditUserListReceived", "data=", {
                    errorCode: errorCode,
                    accreditUsers: accreditUsers
                });
                if (errorCode == 0) {
                    this.executeMethod('OnAccreditUserListReceived', arguments);
                }
            },

            OnMeetingAccreditMessageReceived: function (channel, from, to, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingAccreditMessageReceived', arguments);
            },

            OnMeetingAccreditCancelledMessageReceived: function (channel, from, to, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingAccreditCancelledMessageReceived', arguments);
            },

            OnMeetingInviteMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingInviteMessageReceived', arguments);
            },

            OnMeetingAcceptMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingAcceptMessageReceived', arguments);
            },

            OnMeetingRejectMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingRejectMessageReceived', arguments);
            },

            OnMeetingUpdateMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingUpdateMessageReceived', arguments);
            },

            OnMeetingForwardMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingForwardMessageReceived', arguments);
            },

            OnMeetingCancelMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingCancelMessageReceived', arguments);
            },

            OnUserAvatarDownloaded: function (userId, serverAvatarUrl, localAvatarUrl) {
                uc.util.LogUtil.info("MeetingEventListener", "OnUserAvatarDownloaded", "data=", {
                    userId: userId,
                    serverAvatarUrl: serverAvatarUrl,
                    localAvatarUrl: localAvatarUrl
                });
                this.executeMethod('OnUserAvatarDownloaded', arguments);
            },

            OnUserScheduleListReceived: function (errorCode, requestId, schedules) {
                uc.util.LogUtil.info("MeetingEventListener", "OnUserScheduleListReceived", "data=", {
                    errorCode: errorCode,
                    requestId: requestId,
                    schedules: schedules
                });
                if (errorCode == 0) {
                    this.executeMethod('OnUserScheduleListReceived', arguments);
                }
            },

            OnJoinMeeting: function (meetingInfo, isFromMeetingModule, isCheckMeetingRoomMeeting) {
                uc.util.LogUtil.info("MeetingEventListener", "OnJoinMeeting", "data=", {
                    meetingInfo: meetingInfo,
                    isFromMeetingModule: isFromMeetingModule,
                    isCheckMeetingRoomMeeting: isCheckMeetingRoomMeeting
                });
                this.executeMethod('OnJoinMeeting', arguments);
            },

            OnMeetingReminderMessageReceived: function (meetings) {
                uc.util.LogUtil.info("MeetingEventListener", "OnMeetingReminderMessageReceived", "meetings=", meetings);
                this.executeMethod('OnMeetingReminderMessageReceived', arguments);
            },

            OnQueryMeetingInCalendar: function (meeting) {
                uc.util.LogUtil.info("MeetingEventListener", "OnQueryMeetingInCalendar", "meeting=", meeting);
                this.executeMethod('OnQueryMeetingInCalendar', arguments);
            },

            OnOpenMeetingAttendeeBusyStatusWindow: function (uniqueName, data) {
                uc.util.LogUtil.info("MeetingEventListener", "OnOpenMeetingAttendeeBusyStatusWindow", "data=", {
                    uniqueName: uniqueName,
                    data: data
                });
                this.executeMethod('OnOpenMeetingAttendeeBusyStatusWindow', arguments);
            },

            OnMeetingSummaryVideoMessageReceived: function (channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime, type, videoName, videoURL, videoThumb, thumbnailsURL, length, size, operateName, operateAvatar, msgId, msgSeq, msgTime) {
                if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                    return;
                }
                this.executeMethod('OnMeetingSummaryVideoMessageReceived', arguments);
            },

            OnMeetingRecordingDeleted: function (errorCode, eventId, startTime) {
                if (errorCode == 0) {
                    this.executeMethod('OnMeetingRecordingDeleted', arguments);
                }
            },

            OnGroupMessageRead: function (errorCode, conversationType, userId, readType, msgId, msgSeq) {
                this.executeMethod('OnGroupMessageRead', arguments);
            },

            OnBriefMeetingListReceived: function (errorCode, requestId, startTime, endTime, authorizerId, meetings) {
                if (errorCode == 0) {
                    this.executeMethod('OnBriefMeetingListReceived', arguments);
                }
            },

            OnBriefChangedMeetingListReceived: function (errorCode, actionTime, authorizerId, meetings) {
                if (errorCode == 0) {
                    this.executeMethod('OnBriefChangedMeetingListReceived', arguments);
                }
            },

            OnMeetingInfoReceivedById: function (errorCode, eventId, authorizerId, meetings) {
                if (errorCode == 0) {
                    this.executeMethod('OnMeetingInfoReceivedById', arguments);
                }
            },

            OnMeetingInfoReceivedByIds: function (errorCode, requestId, authorizerId, meetings) {
                if (errorCode == 0) {
                    this.executeMethod('OnMeetingInfoReceivedByIds', arguments);
                }
            },

            OnDocListReceived: function (errorCode, groupId, documents) {
                this.executeMethod('OnDocListReceived', arguments);
            },

            OnFileUploading: function (errorCode, taskId, percent) {
                this.executeMethod('OnFileUploading', arguments);
            },

            OnGkFileDownloadCallBack: function (data) {
                this.executeMethod('OnGkFileDownloadCallBack', arguments);
            },

            OnFileDownloading: function (errorCode, taskId, percent) {
                this.executeMethod('OnFileDownloading', arguments);
            },

            OnFileDownloaded: function (errorCode, taskId, savePath) {
                this.executeMethod('OnFileDownloaded', arguments);
            },

            OnFileUploaded: function (errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL) {
                this.executeMethod('OnFileUploaded', arguments);
            },

            OnMeetingFileUploaded: function (errorCode, eventId, operatorId, document, msgId) {
                this.executeMethod('OnMeetingFileUploaded', arguments);
            },

            OnMeetingDocumentUploadMessageReceived: function (eventId, conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp, localPath) {
                this.executeMethod('OnMeetingDocumentUploadMessageReceived', arguments);
            },

            OnMessageFileUploadCancelled: function (chatId, chatType, taskId) {
                this.executeMethod('OnMessageFileUploadCancelled', arguments);
            },

            OnMeetingJoined: function () {
                this.executeMethod('OnMeetingJoined');
            },

            OnMeetingClosed: function () {
                this.executeMethod('OnMeetingClosed');
            },

            OnMeetingFileDeleted: function (errorCode) {
                this.executeMethod('OnMeetingFileDeleted');
            },

            OnMeetingMessageSent: function (errorCode, groupId, eventId, meetingStartTime, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingMessageSent', arguments);
            },

            OnMeetingChatHistoryReceived: function (errorCode, groupId, msgCount, minTimestamp) {
                this.executeMethod('OnMeetingChatHistoryReceived', arguments);
            },

            OnMeetingTextMessageReceived: function (channel, sendState, from, to, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime, textFormat) {
                this.executeMethod('OnMeetingTextMessageReceived', arguments);
            },

            OnMeetingImageMessageReceived: function (channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingImageMessageReceived', arguments);
            },

            OnMeetingAudioMessageReceived: function (channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingAudioMessageReceived', arguments);
            },

            OnMeetingVideoMessageReceived: function (channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, msgTime) {
                this.executeMethod('OnMeetingVideoMessageReceived', arguments);
            },

            OnMeetingReportMessageReceived: function (channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
                this.executeMethod('OnMeetingReportMessageReceived', arguments);
            },

            OnCheckJoinMeetingRoom: function (errorCode, roomId, hasPassword, isValid, mayJoin) {
                this.executeMethod('OnCheckJoinMeetingRoom', arguments);
            },

            OnMeetingJoinFailed: function () {
                this.executeMethod('OnMeetingJoinFailed');
            },

            OnShowJoinAnimation: function () {
                this.executeMethod('OnShowJoinAnimation');
            },

            OnCancelJoinMeeting: function () {
                this.executeMethod('OnCancelJoinMeeting');
            },

            OnMeetingReadyStatusReceived: function () {
                this.executeMethod('OnMeetingReadyStatusReceived');
            },

            executeMethod: function (method, args) {
                var manager;
                this.managerList.map(function (managerStr) {
                    manager = uc.util.EventUtil.getObject(managerStr);
                    if (manager && manager[method] && jQuery.isFunction(manager[method])) {
                        manager[method].apply(manager, args);
                    }
                });
            }
        });
})(uc);
