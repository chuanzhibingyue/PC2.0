/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.interfaces.IUIEvent = {
        /**
         * event for init client
         * @param clearData
         * @constructor
         */
        OnInitClient: function(clearData){},
        /**
         * event for whole window resize
         */
        OnWindowResized: function(){},
        /**
         * event for change module, for example message/meeting/contact
         * @param moduleId  id of module
         * @param dataObj  change data
         */
        OnModuleChanged: function(moduleId, formCloudMountId, dataObj){},

        /**
         * event for open chat window
         * @param chatType  chat type of chat window, for example single chat, group chat, project chat
         * @param chatId    chatId to pass to chat window, for single chat, is userId, for group chat is group Id
         * @param openChatWindowSourceModule in which module, open
         * @param meetingData  meeting data if chat type is MeetingChat,
         *                       meetingData: {
         *                          meetingId: 0,
         *                          eventId: 0,
         *                          startTime: 0,
         *                          isCancelled: false|true
         *                        }
         * @param siteId
         */
        OnOpenChatWindow: function(chatType, chatId, openChatWindowSourceModule, meetingData, siteId, needSwitchModule,isFromCall, paramObj){},

        /**
         * event for show contact profile
         * @param userId    userId of contact to show profile
         */
        OnShowContactProfile: function(userId){},
        /**
         * event for show pop up contact profile
         * @param userId
         * @constructor
         */
        OnShowPopupContactProfile: function(userId){},

        /**
         * show meeting detail
         * @param meetingInfo
         * @param isFromMeetingList
         * @constructor
         */
        OnShowMeetingDetail: function(meetingInfo, isFromMeetingList){},
        /**
         * event for show choose contacts page
         * @constructor
         */
        OnOpenChooseContactWindow: function(){},
        /**
         * event for show Avatar Upload & Crop page
         * @constructor
         */
        OnOpenImageCropperWindow: function(){},
        /**
         * event for start search
         * @constructor
         */
        OnStartSearch: function () { },
        /**
         * event for start chat
         * @constructor
         */
        OnStartChat: function(){},
        /**
         * event for create project
         * @constructor
         */
        OnCreateProject: function(){},
        /**
         * event for create meeting
         * @constructor
         */
        OnCreateMeeting: function (uniqueName, data) { },
        /**
         * event for open setting window
         */
        OnOpenSettingWindow: function(){},
        /**
         * event for click the button of submit choose contacts
         */
        OnChooseContactsSubmit: function(/* string */ key, /* object */ contacts){},
        /**
         * event for preview image when user click image in Message Box
         * @param chatId    chat id of the image
         * @param chatType  chat type of the image
         * @param msgSeq    image seq
         * @param imageName image name
         */
        OnPreviewChatImage: function(chatId, chatType, msgSeq, imageName){},
        /**
         * event for preview single image
         * @param imagePath
         */
        OnPreviewImage: function(imagePath){},
        /**
         * event for user avatar downloaded
         * @param userId
         * @param serverAvatarUrl
         * @param localAvatarUrl
         */
        OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){},
        /**
         * event for project avatar downloaded
         * @param groupId
         * @param serverAvatarUrl
         * @param localAvatarUrl
         */
        OnProjectAvatarDownloaded: function(groupId, serverAvatarUrl, localAvatarUrl){},

        /**
         * event for download chat image
         * @param chatId
         * @param chatType
         * @param serverUrl
         * @param localUrl
         * @param msgId
         * @constructor
         */
        OnChatImageDownloaded: function(chatId, chatType, serverUrl, localUrl, msgId){},
        /**
         * event for message sending from MessageInputBox
         * @param messageData   message data in MessageInputBox
         */
        OnMessageSending: function(messageData){},
        /**
         * event for uploading file
         * @param chatId
         * @param chatType
         * @param taskId
         * @param contentType
         */
        OnMessageFileUploading: function(chatId, chatType, taskId, contentType,messageData){},
        /**
         * event for cancel upload file
         * @param chatId
         * @param chatType
         * @param taskId
         * @constructor
         */
        OnMessageFileUploadCancelled: function(chatId, chatType, taskId){},
        /**
         * event for opening the window of group profile
         * @param groupId
         */
        OnShowPopupGroupProfile: function (groupId) { },

        OnJoinMeeting: function(meetingInfo, isFromMeetingList,isCheckMeetingRoomMeeting){},

        OnOpenMeetingAttendeeBusyStatusWindow: function (uniqueName, data) {},
        OnNewMessageCountChanged: function(count){},
        OnOpenMyLinkPage: function () {},

        OnQueryMeetingInCalendar: function (meeting) { },
        OnDateChanged: function () { },
        OnDraftMessageChanged: function(chatId, chatType, moduleId){},
        OnPreviewVideo: function (jsonData) {},
        OnOpenPresetApp: function (appId) {},
        OnGkFileDownloadCallBack: function(data){},
        OnCheckGkFilePermission: function(data,cloudData,type,that){},
        OnCheckGkFileExist: function(data,cloudData,option,Type){},
        OnGkGetFileInfo: function(data,cloudData,Type){},
        OnGkClientConnectionStatus:function(errorCode,mountIdData){},
        OnOpenOAApproveWindow: function () {},
        OnCreateCloud:function(){},
        OnCloudFilePreviewer:function(data){},
        OnCloudChatSet: function (groupId){},
        /**
         * event for call started
         * @param chatId
         * @param chatType
         */
        OnCallStarted: function(chatId, chatType){},
        /**
         * event for call ended
         * @param chatId
         * @param chatType
         */
        OnCallEnded: function(chatId, chatType){},

        OnOpenChooseCallContactsWindow: function (key, callback) {},

	    onOpenAVdetectionWindow: function(){},
        OnMinimizeWindow:function(){},
	    OnCloudFileSelectCallBack: function (data){},
        OnOpenPbxCallChat:function(data){},
        onOpenNoticeReceiverWindow:function(){},
        OnNoticeConversationStatusChanged:function(conversationId){},
        OnNoticeLastMessageReceived:function(conversationId,title,timestamp){},
        OnPbxCallFromContact:function(data){},
        OnPbxCallBee:function(data){},
        OnSaveBeeCallDetailsToPbx:function(data){},
        OnSaveBeeCallStatusToPbx:function(data){},
        OnConversationDataUpdate:function(){},

	    OnCreateMeetingRoom:function(data){},
        OnShowLocation: function (locationObj) {},

	    /**MeetingRoom UI Event**/
	    OnWaitJoinMeetingRoomMeeting: function () {},

	    OnMeetingRoomMeetingJoined: function () {},

	    OnJoinMeetingRoomMeetingFailed: function (errorCode) {},

	    OnJoinMeetingRoomMeeting: function (data) {},
        OnShowJoinAnimation: function () {},
        OnCancelJoinMeeting: function () {}
    }
})(uc);