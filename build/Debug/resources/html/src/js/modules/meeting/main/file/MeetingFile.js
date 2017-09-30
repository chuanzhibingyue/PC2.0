/**
 * Created by waouyang on 16/4/27.
 */
(function (uc) {
    var self = null;
    uc.modules.meeting.MeetingFile = {
        meetingFileList: null,
        meetingInfo: null,
        init: function () {
            this.meetingFileList = new uc.modules.chat.FileList();//new uc.modules.chat.FileList();
        },

        showMeetingFileList: function (meetingInfo, fileListPanel) {
            this.meetingInfo = meetingInfo;

            meetingInfo.userId  = meetingInfo.groupId;
            meetingInfo.siteId =  uc.IContactService.getCurrentUserInfo().siteId;
            meetingInfo.resId  = 100;
            meetingInfo.chatType = uc.constants.Chat.ChatType.MeetingChat;

            this.meetingFileList.init(meetingInfo.groupId, fileListPanel, uc.constants.Conversation.ConversationType.MeetingMsg, true, meetingInfo);
        },

        uploadMeetingFile: function () {
            if (this.meetingFileList && this.meetingFileList.isActivity) {
                this.meetingFileList.openUploadWindow();
            }
        },

        setUploadMsgId: function (taskId, msgId) {
            if (this.meetingFileList) {
                this.meetingFileList.setUploadMsgId(taskId, msgId);
            }
        },

        onFileTransfer: function (errorCode, taskId, percent, action) {
            if (this.meetingFileList && this.meetingFileList.uploadingItemMap) {
                var fileInfos = this.meetingFileList.uploadingItemMap.get('' + taskId), fileInfo;
                if (fileInfos) {
                    fileInfo = fileInfos.fileMessageData;
                }

                if (fileInfo && fileInfo.chatType == uc.constants.Chat.ChatType.MeetingChat &&
                    fileInfo.chatObjectId == this.meetingInfo.groupId) {
                    switch (action) {
                        case 1:
                            this.meetingFileList.OnFileDownloading(taskId, percent);
                            break;
                        case 2:
                            this.meetingFileList.fileListUploadProgress(taskId, percent,errorCode);
                            break;
                    }

                }
            }
        },

        OnMessageFileUploadCancelled: function (chatId, chatType, taskId) {
            if (this.meetingFileList && chatId == this.meetingInfo.groupId && chatType == uc.constants.Chat.ChatType.MeetingChat) {
                this.meetingFileList.clearUploadingItemMap(taskId);
            }
        },

        OnMinutesListReceived: function (errorCode, eventId, startTime, minutes) {
            if (this.meetingFileList && this.meetingFileList.isActivity) {
                this.meetingFileList.OnMinutesListReceived(errorCode, eventId, minutes);
            }
        },

        OnFileUploading: function (errorCode, taskId, percent) {
            this.onFileTransfer(errorCode, taskId, percent, uc.constants.Meeting.FileTransfer.Uploading);
        },

        OnFileDownloading: function (errorCode, taskId, percent) {
            this.onFileTransfer(errorCode, taskId, percent, uc.constants.Meeting.FileTransfer.Downloading);
        },

        OnFileDownloaded: function (errorCode, taskId, savePath) {
            if (this.meetingFileList && this.meetingFileList.isActivity) {
                this.meetingFileList.OnFileDownloaded(errorCode, taskId, savePath);
            }
        },

        OnFileUploaded: function (errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL) {

            var fileInfo = {
                name: fileName,
                size: fileSize,
                url: downloadURL,
                thumbnailURL: convertDownloadURL,
                operatorId: operatorId,
                operatorTime: operatorTime,
                fid: fid
            };

            if (this.meetingFileList && errorCode == 0 && this.meetingFileList.isActivity) {
                this.meetingFileList.onDocListReceived(errorCode, fileInfo);
            }
        },

        OnMeetingFileUploaded: function (errorCode, eventId, operatorId, document, msgId) {
            if (this.meetingFileList &&  this.meetingFileList.isActivity) {
                this.meetingFileList.fileListUploadComplete(eventId, operatorId, document, msgId);
            }
        },
        OnMeetingDocumentUploadMessageReceived: function (eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp,localPath) {
            if (this.meetingFileList && this.meetingFileList.isActivity) {
                this.meetingFileList.fileListUploadMessageReceived(eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp);
            }
        },

        OnDocListReceived: function (errorCode, groupId, documents) {
            if (this.meetingFileList && errorCode == 0 && this.meetingFileList.isActivity) {
                this.meetingFileList.onDocListReceived(errorCode, documents);
            }
        },

        refresh: function () {

        }
    }
})(uc);