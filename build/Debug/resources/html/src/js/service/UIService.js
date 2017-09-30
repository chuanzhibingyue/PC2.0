/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function (uc) {
    var downloadUserAvatarTmp = [];
    var _downloadUserAvatar = function () {
        var tmp;
        for (var i = 0; i < 20; i++) {
            tmp = downloadUserAvatarTmp.shift();
            if (!tmp) {
                return;
            }
			uc.util.LogUtil.info('IUIService', 'downloadUserAvatar', 'userId = ' + tmp[0] + ', avatarUrl = ' + tmp[1], null);
            uc.modules.image.ImageManager.downloadUserAvatar(tmp[0], tmp[1]);
        }
        if (downloadUserAvatarTmp.length > 0)
            setTimeout(_downloadUserAvatar, 1000);
    };
    
    uc.service.UIService = function () {
        this.moduleId = uc.constants.UI.ModuleId.Message;
        this.crossWindowEventName = {};
    };
    jQuery.extend(uc.service.UIService.prototype,
        uc.interfaces.IUIService, {
            initClient: function(clearData){
                uc.util.LogUtil.info('IUIService', 'initClient', 'clearData = ' + clearData, null);
                uc.IUIEvent.OnInitClient(clearData);
            },
            /**
             * change module when click menu bar, for example click message menu button
             * @param moduleId  module id
             * @param dataObj change data
             */
            changeModule: function (moduleId,MountId, dataObj) {
                uc.util.LogUtil.info('IUIService', 'changeModule', 'moduleId =' + moduleId, "MountId="+MountId);
                if (moduleId == this.moduleId && !MountId) {
                    return;
                }
                uc.IUIEvent.OnModuleChanged(moduleId,MountId, dataObj);
                this.moduleId = moduleId;
            },
            /**
             * open a chat window
             * @param chatType  chat type of the chat window
             * @param chatId    chat id passed to chat window
             * @param openChatWindowSourceModule
             * @param meetingData
             * @param siteId
             * @param needSwitchModule
             */
            openChatWindow: function (chatType, chatId, openChatWindowSourceModule, meetingData, siteId, needSwitchModule,isFromCall, paramObj) {
                uc.util.LogUtil.info('IUIService', 'openChatWindow', 'chatId = ' + chatId + ', chatType = ' + chatType + ', source = ' + openChatWindowSourceModule + ', siteId = ' + siteId + ', meetingData = ', meetingData);
                if(needSwitchModule){
                    uc.IUIService.changeModule(uc.constants.UI.ModuleId.Message);
                }
                uc.IUIEvent.OnOpenChatWindow(Number(chatType), Number(chatId), openChatWindowSourceModule, meetingData, Number(siteId), needSwitchModule,isFromCall, paramObj);
            },
            /**
             * show a contact's profile
             * @param userId
             */
            showContactProfile: function (userId) {
                uc.util.LogUtil.info('IUIService', 'showContactProfile', 'userId = ' + userId, null);
                uc.IUIEvent.OnShowContactProfile(userId);
            },
            /**
             * show contact profile in popup window
             * @param userId
             */
            showPopupContactProfile: function(userId){
                uc.util.LogUtil.info('IUIService', 'showPopupContactProfile', 'userId = ' + userId, null);
                uc.IUIEvent.OnShowPopupContactProfile(userId);
            },
            /**
             * show choose contacts in popup window
             */
            openChooseContactWindow: function (/*object*/ option, callback) {
                uc.util.LogUtil.info('IUIService', 'openChooseContactWindow', 'option = ', option);
                if (!option ||!option.key) {
                    return;
                }

                if (callback) {
                    this.bindCrossWindowEvent(option.key, callback);
                }

                uc.IUIEvent.OnOpenChooseContactWindow(option);
            },
            /**
             * show Image Crop page in popup window
             */
            openImageCropperWindow: function (/*string*/ key, callback) {
                uc.util.LogUtil.info('IUIService', 'openImageCropperWindow', 'key = ' + key, null);
                if (!key) {
                    return;
                }

                if (callback) {
                    this.bindCrossWindowEvent(key, callback);
                }

                uc.IUIEvent.OnOpenImageCropperWindow(key);
            },
            /**
             * show meeting detail
             * @param meetingInfo
             * @param isFromMeetingList
             */
            showMeetingDetail: function(meetingInfo, isFromMeetingList){
                uc.util.LogUtil.info('IUIService', 'showMeetingDetail', 'data', {
                    isFromMeetingList: isFromMeetingList,
                    meetingInfo: meetingInfo
                });
            	uc.IUIEvent.OnShowMeetingDetail(meetingInfo, isFromMeetingList);
            },
            /**
             * open search window
             */
            startSearch: function(){
                uc.util.LogUtil.info('IUIService', 'startSearch', 'start Search', null);
                uc.IUIEvent.OnStartSearch();
            },
            /**
            * begin single chat
            */
            startChat: function (userIds) {
                uc.util.LogUtil.info('IUIService', 'startChat', 'start chat', null);
                uc.IUIEvent.OnStartChat(userIds);
            },
            /**
             * create project
             */
            createProject: function(){
                uc.util.LogUtil.info('IUIService', 'createProject', 'create project', null);
                uc.IUIEvent.OnCreateProject();
            },
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
            createMeeting: function (uniqueName, data, callback) {
                uc.util.LogUtil.info('IUIService', 'createMeeting', 'data', {uniqueName: uniqueName, data: data});
                if (!uniqueName) {
                    return;
                }

                uc.util.StorageUtil.setData("MeetingEditor:UniqueName", uniqueName);

                this.bindCrossWindowEvent(uniqueName, function (args) {
                    uc.util.StorageUtil.removeData("MeetingEditor:UniqueName");
                    uc.util.StorageUtil.removeData(uniqueName);

                    if (callback) {
                        callback(args);
                    }
                });


                data = jQuery.extend({
                    shareUserId: this.getAuthorizerId(),
                    isCreate: true,
                    type: uc.constants.Meeting.MeetingCreateType.Default,
                    startTime: uc.util.TimeUtil.getCurrentHalfOrWholeHourUnixTime(uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second),
                    duration: uc.constants.Meeting.DefaultMeetingDuration
                }, data);

                uc.IUIEvent.OnCreateMeeting(uniqueName, data);
            },
            /**
             * open setting window
             */
            openSettingWindow: function(){
                uc.util.LogUtil.info('IUIService', 'openSettingWindow', 'open setting window', null);
                uc.IUIEvent.OnOpenSettingWindow();
            },
            /**
             * submit choose contacts 
             */
            chooseContactsSubmit: function(/* string */ key, /* object */ contacts){
                uc.util.LogUtil.info('IUIService', 'chooseContactsSubmit', 'key = ' + key + ', contacts = ', contacts);
            	uc.IUIEvent.OnChooseContactsSubmit(key, contacts);
            },
            /**
             * preview image when user click image in Message Box
             * @param chatId        chat id of the image
             * @param chatType      chat type of the image
             * @param msgSeq        image seq
             * @param imageName     image name
             */
            previewChatImage: function(chatId, chatType,msgSeq, imageName){
                uc.util.LogUtil.info('IUIService', 'previewChatImage', 'chatId = ' + chatId + ', chatType = ' + chatType + ', msgSeq = ' + msgSeq + ', imageName = ' + imageName, null);
                uc.IUIEvent.OnPreviewChatImage(chatId, chatType, msgSeq, imageName);
            },
            previewImage: function(imagePath){
                uc.util.LogUtil.info('IUIService', 'previewImage', 'imagePath = ' + imagePath, null);
                uc.IUIEvent.OnPreviewImage(imagePath);
            },
            startDownloadUserAvatar: function () {
                _downloadUserAvatar();
            },
            downloadUserAvatar: function(userId, avatarUrl){
                //var loginUserInfo = uc.IClientService.getCurrentUCID();
                //if (uc.modules.conversation.ConversationList.isFirstMainLoaded || 'OnContactInfoReceived' == action || (loginUserInfo && loginUserInfo.userId == userId)) {
                    uc.util.LogUtil.info('IUIService', 'downloadUserAvatar', 'userId = ' + userId + ', avatarUrl = ' + avatarUrl, null);
                    uc.modules.image.ImageManager.downloadUserAvatar(Number(userId), avatarUrl);
               // } else {
               //    downloadUserAvatarTmp.push([Number(userId), avatarUrl]);
               // }
            },
            downloadProjectAvatar: function(groupId, avatarUrl){
                uc.util.LogUtil.info('IUIService', 'downloadProjectAvatar', 'groupId = ' + groupId + ', avatarUrl = ' + avatarUrl, null);
                uc.modules.image.ImageManager.downloadProjectAvatar(Number(groupId), avatarUrl);
            },
            downloadChatImage: function(chatId, chatType, imageServerPath, msgId){
                uc.util.LogUtil.info('IUIService', 'downloadChatImage', 'chatId = ' + chatId + ', chatType = ' + chatType + ', path = ' + imageServerPath + ', msgId = ' + msgId, null);
                uc.modules.image.ImageManager.downloadChatImage(chatId, chatType,imageServerPath,  msgId);
            },

            bindCrossWindowEvent: function(eventName, callback){
                uc.util.LogUtil.info('IUIService', 'bindCrossWindowEvent', 'eventName = ' + eventName, null);
                if(this.crossWindowEventName[eventName]){
                    return;
                }
                this.crossWindowEventName[eventName] = callback;
                uc.util.EventUtil.bindCrossWindowEvent(eventName, callback);
            },
            /**
             * trigger event from one window to another
             * @param eventName event name
             * @param eventData event data, can be JSON or other data format
             */
            triggerCrossWindowEvent: function(eventName, eventData){
                uc.util.LogUtil.info('IUIService', 'triggerCrossWindowEvent', 'eventName = ' + eventName + ', eventData = ', eventData);
                uc.util.EventUtil.triggerCrossWindowEvent(eventName, eventData);
            },
            /**
             * open group profile window
             */
            showPopupGroupProfile: function(groupId){
                uc.util.LogUtil.info('IUIService', 'showPopupGroupProfile', 'groupId = ' + groupId, null);
            	uc.IUIEvent.OnShowPopupGroupProfile(groupId);
            },

            openMeetingAttendeeBusyStatusWindow: function (uniqueName, data, callback) {
                uc.util.LogUtil.info('IUIService', 'openMeetingAttendeeBusyStatusWindow', 'uniqueName = ' + uniqueName + ', data = ', data);
                if (!uniqueName) {
                    return;
                }
                uc.util.StorageUtil.setData("Calendar:UniqueName", uniqueName);
                uc.util.StorageUtil.setData(uniqueName, data);

                this.bindCrossWindowEvent(uniqueName, function (args) {
                    uc.util.StorageUtil.removeData("Calendar:UniqueName");
                    uc.util.StorageUtil.removeData(uniqueName);

                    if (callback) {
                        callback(args);
                    }
                });

                uc.IUIEvent.OnOpenMeetingAttendeeBusyStatusWindow(uniqueName, data);
            },

            openMyLinkPage: function(){
                uc.util.LogUtil.info('IUIService', 'openMyLinkPage', 'Open My link page', null);
                uc.IUIEvent.OnOpenMyLinkPage();
            },
            OpenLinkPage: function(userId){

            },

            queryMeetingInCalendar: function (meeting) {
                uc.util.LogUtil.info('IUIService', 'queryMeetingInCalendar', 'Meeting data = ', meeting);
                uc.IUIEvent.OnQueryMeetingInCalendar(meeting);
            },
            openPresetApp: function (appId) {
                uc.IUIEvent.OnOpenPresetApp(appId);
            },
	        previewVideo: function (jsonData) {
                uc.IUIEvent.OnPreviewVideo(jsonData);
            },
            openChooseCallContactsWindow: function (option, callback) {
                uc.util.LogUtil.info('IUIService', 'openChooseCallContactWindow', 'option = ', option);
                if (!option ||!option.key) {
                    return;
                }

                if (callback) {
                    this.bindCrossWindowEvent(option.key, callback);
                }

               // uc.IUIEvent.OnOpenChooseCallContactsWindow(option);

	            uc.util.StorageUtil.setData("chooseCallContacts", option);
	            var openTime = new Date();

	            var options = {
		            title: '',
		            width: 601,
		            height: 620,
		            refreshIfExist: true,
		            centerScreen: true,
		            modal: true,
		            resizable: false,
		            minWidth: 601,
		            minHeight: 620,
		            caption: uc.constants.UI.DragCaption.ChooseCallContacts
	            };

	            uc.util.WindowUtil.open('page/chooseCallContacts.html', 'chooseCallContacts' + openTime, options);


            },
		    onOpenAVdetectionWindow:function(){
			    uc.IUIEvent.onOpenAVdetectionWindow();
		    },
            minimizeWindow:function(){
                uc.IUIEvent.OnMinimizeWindow();
            },
            openOAApproveWindow: function (/*object*/ option, callback) {
                uc.util.LogUtil.info('IUIService', 'openOAApproveWindow', 'option = ', option);
                if (!option ||!option.key) {
                    return;
                }

                if (callback) {
                    this.bindCrossWindowEvent(option.key, callback);
                }

                uc.IUIEvent.OnOpenOAApproveWindow(option);
            },

            openNoticeReceiverWindow:function(){
              uc.IUIEvent.onOpenNoticeReceiverWindow();
            },

            joinMeeting: function (meetingInfo, isFromMeetingModule,isCheckMeetingRoomMeeting) {
                uc.util.LogUtil.info('IUIService', 'joinMeeting', 'data', {
                    meetingInfo: meetingInfo,
                    isFromMeetingModule: isFromMeetingModule,
	                isCheckMeetingRoomMeeting:isCheckMeetingRoomMeeting
                });

                uc.IUIEvent.OnJoinMeeting(meetingInfo, isFromMeetingModule,isCheckMeetingRoomMeeting);
            },
		    createMeetingRoom: function ( option, callback){
			    uc.util.LogUtil.info('IUIService', 'createMeetingRoom', 'option = ', option);
			    if (!option ||!option.key) {
				    return;
			    }
			    uc.util.StorageUtil.setData("meetingRoom_createOrEditMeeting", option);


			    this.bindCrossWindowEvent(option.key, function(args){
				    uc.util.StorageUtil.removeData("meetingRoom_createOrEditMeeting");
				    if (callback) {
					    callback(args);
				    }
			    });

			    uc.IUIEvent.OnCreateMeetingRoom(option);
		    },
            /**
             * get current selected authorizer id who share meeting to me
             */
            getAuthorizerId: function(){
                var authorizerId = uc.modules.meeting.MeetingListManager.getAuthorizerId();
                return authorizerId ? authorizerId : 0;
            },

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
            showLocation: function (locationObj) {
                uc.util.LogUtil.info('IUIService', 'showGeoLocation', 'locationObj', locationObj);
                uc.util.StorageUtil.setData('GeoLocationData', locationObj);
                if (locationObj) {
                    uc.IUIEvent.OnShowLocation();
                }
            },
		    joinMeetingRoomMeeting: function ( option, callback) {
			    uc.util.LogUtil.info('IUIService', 'joinMeetingRoomMeeting', 'option = ', option);
			    if (!option ||!option.key) {
				    return;
			    }
			    uc.util.StorageUtil.setData(option.key, option);

			    this.bindCrossWindowEvent(option.key, function(args){
				    uc.util.StorageUtil.removeData(option.key);
				    if (callback) {
					    callback(args);
				    }
			    });

			    uc.IUIEvent.OnJoinMeetingRoomMeeting(option);
		    },

            showJoinAnimation: function () {
                uc.IUIEvent.OnShowJoinAnimation();
            },

            cancelJoinMeeting: function () {
                uc.IUIEvent.OnCancelJoinMeeting();
            }
        })
})(uc);