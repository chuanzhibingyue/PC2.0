/**
 * Created by senlin.ouyang on 2015/7/29.
 */
(function(uc){
    uc.interfaces.IUIService = {
        /**
         * init client when login success
         * @param clearData true: clear data, false: do not clear data
         */
        initClient: function(clearData){},
        /**
         * change module when click menu bar, for example click message menu button
         * @param moduleId  module id
         * @param mountId
         * @param dataObj change data
         */
        changeModule: function(moduleId, mountId, dataObj){},
        /**
         * open a chat window
         * @param chatType  chat type of the chat window
         * @param chatId    chat id pass to chat window, if is single chat, chatId is userId, if is group chat, chatId is
         *                   groupId, others are the same
         * @param openChatWindowSourceModule  in which module, open chat window operation is from
         * @param meetingData   meeting data if chat type is MeetingChat,
         *                       meetingData: {
         *                          meetingId: 0,
         *                          eventId: 0,
         *                          startTime: 0,
         *                          isCancelled: false|true
         *                        }
         * @param siteId    siteId of user/group/meeting
         * @param needSwitchModule    If it needs switch to Message Module, for example, if from profile page, set it to true
         *                             If it is called from conversation module, set it to false, it it's from other modules
         *                             set it to true
         */
        openChatWindow: function(chatType, chatId, openChatWindowSourceModule, meetingData, siteId, /*Boolean*/needSwitchModule){},
        /**
         * show a contact's profile
         * @param userId
         */
        showContactProfile: function(userId){},
        /**
         * show contact profile in popup window
         * @param userId
         */
        showPopupContactProfile: function(userId){},
        /**
         * show choose contacts in popup window
         * @param   key
         * @param   callback
         */
        openChooseContactWindow: function( key, callback){},
        /**
         * show Avatar Upload & Crop in popup window
         */
        openImageCropperWindow: function(/* string*/ key, callback){},
        /**
         * show meeting detail
         * @param meetingInfo
         * @param isFromMeetingList
         */
        showMeetingDetail: function(meetingInfo, isFromMeetingList){},
        /**
         * open search window
         */
        startSearch: function () { },
        /**
        * begin single chat
        */
        startChat: function (userIds) { },
        /**
         * create project
         */
        createProject: function(){},
        /**
         * create meeting
         * @param uniqueName
         * @param data object
         * @param data.shareUserId: uc.modules.meeting.MeetingListManager.getAuthorizerId()
         * @param data.meetingInfo: create meeting = null，edit meeting = current selected meeting info
         * @param data.type: uc.constants.Meeting.MeetingCreateType.Default || uc.constants.Meeting.MeetingCreateType.RoomMeeting
         * @param data.isCreate：create meeting = true，edit meeting = false
         * @param data.startTime: create meeting： uc.util.TimeUtil.getCurrentHalfOrWholeHourUnixTime(), edit meeting: null
         * @param data.users: [userId] || null
         * @param data.duration: meeting duration, in minutes
         * @param callback
         */
        createMeeting: function (uniqueName, data, callback) { },
        /**
         * open setting window
         */
        openSettingWindow: function(){},
        /**
         * the button of submit choose contacts is clicked
         */
        chooseContactsSubmit: function(/* string */ key, /* array*/ contacts){},
        /**
         * preview image when user click image in Message Box
         * @param chatId    chat id of the image
         * @param chatType  chat type of the image
         * @param msgSeq    image msg seq
         * @param imageName image name
         */
        previewChatImage: function(chatId, chatType, msgSeq, imageName){},
        /**
         * preview single image, for example preview image in MessageInputBox
         * @param imagePath     image path
         */
        previewImage: function(imagePath){},
        startDownloadUserAvatar: function () {},
        /**
         * download user avatar from server
         * @param userId
         * @param avatarUrl
         */
        downloadUserAvatar: function(userId, avatarUrl){},
        /**
         * download project avatar from server
         * @param groupId
         * @param avatarUrl
         */
        downloadProjectAvatar: function(groupId, avatarUrl){},
        /**
         * download image in chat window
         * @param chatId
         * @param chatType
         * @param imageServerPath
         * @param msgId
         */
        downloadChatImage: function(chatId, chatType, imageServerPath, msgId){},
        /**
         * bind cross window event
         * @param eventName
         * @param callback      callback(data) data is passed from original window
         */
        bindCrossWindowEvent: function(eventName, callback){},

        /**
         * trigger event from one window to another
         * @param eventName event name
         * @param eventData event data, can be JSON or other data format
         */
        triggerCrossWindowEvent: function(eventName, eventData){},
        /**
         * open the window of group profile
         * @param groupId
         */
        showPopupGroupProfile: function(groupId){},
        /**
         * join a meeting
         * @param meetingInfo
         * @param isFromMeetingModule
         * @param isCheckMeetingRoomMeeting 是否需要检测当前会议是否会议室会议
         */
        joinMeeting: function(meetingInfo, isFromMeetingModule, isCheckMeetingRoomMeeting){},
        /**
         * open calendar window of user or group
         * @param uniqueName
         * @param data
         * @param callback
         */
        openMeetingAttendeeBusyStatusWindow: function (uniqueName, data, callback) {},

        openMyLinkPage: function(){},

        OpenLinkPage: function(userId){},

        queryMeetingInCalendar: function (meeting) { },

        openPresetApp: function (appId) {},

	    previewVideo: function (jsonData) {},

        openChooseCallContactsWindow: function (key, callback) {},

	    onOpenAVdetectionWindow: function( ){},

        minimizeWindow:function(){},

        openOAApproveWindow: function (options, callback) {} ,
		openNoticeReceiverWindow:function(){},

	    createMeetingRoom:function(options, callback ){},

        /**
         * get current selected authorizer id who share meeting to me
         */
        getAuthorizerId: function(){},

        /*
        * show map by geolocation params
        * @param locationObj
        *   {
        *       "longitude":"120.674247951382767",
        *       "latitude":"31.301719361616350",
        *       "type":1,    //1-外出    2-公司内
        *       "name":"苏州维景国际大酒店",
        *       "location":"金鸡湖大道1355号苏州维景国际大酒店3层"
        *   }
        */
        showLocation: function (locationObj) {},

	    joinMeetingRoomMeeting: function (options, callback ){},
        showJoinAnimation: function () {},
        cancelJoinMeeting: function () {}
    }
})(uc);