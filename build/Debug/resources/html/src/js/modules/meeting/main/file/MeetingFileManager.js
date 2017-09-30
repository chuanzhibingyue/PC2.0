/**
 * Created by waouyang on 16/5/4.
 */
(function (uc) {
    var _this = null;
    uc.modules.meeting.MeetingFileManager = {
        init: function () {
            uc.modules.meeting.MeetingFile.init();
            this.meetingListInitialized = true;
        },

        showMeetingFileList: function (parentNode, fileListPanel, meetingInfo) {
            if (!this.meetingListInitialized) {
                this.init(parentNode);
            }
            uc.modules.meeting.MeetingFile.showMeetingFileList(meetingInfo, fileListPanel);
            this.meetingFileListShown = true;
        },

        OnBasicContactInfoReceived: function (errorCode, userIds, contacts) {
            if (errorCode == uc.ErrorCode.Success && this.meetingFileListShown) {
                //uc.modules.meeting.MeetingFile.OnBasicContactInfoReceived(contacts);
            }
        },
        
        OnUserAvatarDownloaded: function (userId, serverAvatarUrl, localAvatarUrl) {
            if (this.meetingFileListShown) {
                //uc.modules.meeting.MeetingFile.OnUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
            }
        },

        hideMeetingFileList: function () {
            this.meetingFileListShown = false;
        },

        initMeetingList: function () {

        },

        uploadMeetingFile: function () {
            if (this.meetingListInitialized) {
                uc.modules.meeting.MeetingFile.uploadMeetingFile();
            }
        },

        setUploadMsgId: function (taskId, msgId) {
            if (this.meetingListInitialized) {
                uc.modules.meeting.MeetingFile.setUploadMsgId(taskId, msgId);
            }
        },

        OnInitClient: function (clearData) {
            //this.init();
        },

        OnDateChanged: function () {

        },

        OnMeetingCanceled: function (/*int*/errorCode, eventId, startTime, authorizerId) {

        },

        OnMinutesListReceived: function (errorCode, eventId, startTime, minutes) {
            if (this.meetingListInitialized) {
                uc.modules.meeting.MeetingFile.OnMinutesListReceived(errorCode, eventId, startTime, minutes);
            }
        },

        OnFileDownloading: function (errorCode, taskId, percent) {
            var moduleId = uc.modules.meeting.MeetingMainManager.moduleId;
            if (this.meetingListInitialized && moduleId == uc.constants.UI.ModuleId.Meeting) {
                uc.modules.meeting.MeetingFile.OnFileDownloading(errorCode, taskId, percent);
            }
        },

        OnFileDownloaded: function (errorCode, taskId, savePath) {
            var moduleId = uc.modules.meeting.MeetingMainManager.moduleId;
            if (this.meetingListInitialized && moduleId == uc.constants.UI.ModuleId.Meeting) {
                if (uc.modules.chat.ChatManager.fileDownLoadInfo["" + taskId] == null) {
                    return;
                }
                var msgId = uc.modules.chat.ChatManager.fileDownLoadInfo["" + taskId].msgId;
                uc.modules.meeting.MeetingFile.OnFileDownloaded(errorCode, msgId, savePath);
            }
        },

        OnFileUploading: function (errorCode, taskId, percent) {
            var moduleId = uc.modules.meeting.MeetingMainManager.moduleId;
            if (this.meetingListInitialized && moduleId == uc.constants.UI.ModuleId.Meeting) {
                uc.modules.meeting.MeetingFile.OnFileUploading(errorCode, taskId, percent);
            }
        },

        OnFileUploaded: function (errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL) {
            var moduleId = uc.modules.meeting.MeetingMainManager.moduleId;
            if (this.meetingListInitialized && moduleId == uc.constants.UI.ModuleId.Meeting) {
                //uc.modules.meeting.MeetingFile.OnFileUploaded(errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL);
            }
        },

        OnMessageFileUploadCancelled: function (chatId, chatType, taskId) {
            uc.modules.meeting.MeetingFile.OnMessageFileUploadCancelled(chatId, chatType, taskId);
        },

        OnMeetingFileUploaded: function (errorCode, eventId, operatorId, document, msgId) {
            var moduleId = uc.modules.meeting.MeetingMainManager.moduleId;
            if (this.meetingListInitialized && moduleId == uc.constants.UI.ModuleId.Meeting) {
                uc.modules.meeting.MeetingFile.OnMeetingFileUploaded(errorCode, eventId, operatorId, document, msgId);
            }
        },

        OnMeetingDocumentUploadMessageReceived: function (eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp,localPath) {
            var moduleId = uc.modules.meeting.MeetingMainManager.moduleId;
            if (this.meetingListInitialized && moduleId == uc.constants.UI.ModuleId.Meeting) {
                uc.modules.meeting.MeetingFile.OnMeetingDocumentUploadMessageReceived(eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp,localPath);
            }
        },

        OnDocListReceived: function (errorCode, groupId, documents) {
            var moduleId = uc.modules.meeting.MeetingMainManager.moduleId;
            if (this.meetingListInitialized && moduleId == uc.constants.UI.ModuleId.Meeting) {
                uc.modules.meeting.MeetingFile.OnDocListReceived(errorCode, groupId, documents);
            }
        }
    };
})(uc);