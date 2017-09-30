/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function (uc) {
    uc.listener.nativeServiceConfig = {
        IClientService: {
            login: {
                context: ucClient.ClientService,
                method: 'login'
            },
            logout: {
                context: ucClient.ClientService,
                method: 'logout'
            },
            reLogin: {
                context: ucClient.ClientService,
                method: 'relogin'
            },
            loginUCAS: {
                context: ucClient.ClientService,
                method: 'connectUCAS'
            },
            logoutUCAS: {
                context: ucClient.ClientService,
                method: 'disconnectUCAS'
            },
            quit: {
                context: ucClient.App,
                method: 'quit'
            },
            getLocalConversationList: {
                context: ucClient.ClientService,
                method: 'getLocalConversations'
            },
            syncConversationList: {
                context: ucClient.ClientService,
                method: 'getNewConversationList'
            },
            getHistoryConversationList: {
                context: ucClient.ClientService,
                method: 'getOldConversationList'
            },
            getDataDirectory: {
                context: ucClient.ClientService,
                method: 'getDocumentDataDirSync'
            },
            getHistoryLoginAccounts:{
                context: ucClient.ClientService,
                method: 'loadHistoryAccounts'
            },
            deleteHistoryAccount:{
                context: ucClient.ClientService,
                method: 'deleteHistoryAccount'
            },
            rememberPassword:{
                context: ucClient.ClientService,
                method: 'rememberPassword'
            },
            setAutoLogin:{
                context: ucClient.ClientService,
                method: 'setAutoLogin'
            },
            modifyPassword: {
                context: ucClient.ClientService,
                method: 'modifyPassword'
            },
            getCaptchaURL: {
                context: ucClient.ClientService,
                method: 'getCaptchaURL'
            },
            verifyAccount: {
                context: ucClient.ClientService,
                method: 'verifyAccount'
            },
            verifyCode: {
                context: ucClient.ClientService,
                method: 'verifyCode'
            },
            resetPassword: {
                context: ucClient.ClientService,
                method: 'resetPassword'
            },
            getCurrentUCID: {
                context: ucClient.ClientService,
                method: 'getCurUCID'
            },
            setProxyAuthInfo: {
                context: ucClient.ClientService,
                method: 'setProxyAuthInfo'
            },
            updateClient: {
                context: ucClient.ClientService,
                method: 'updateClient'
            },
            getConfigInfo: {
                context: ucClient.ClientService,
                method: 'getConfigInfo'
            },
            getConfigInfos: {
                context: ucClient.ClientService,
                method: 'getConfigInfos'
            },
            updateConfigInfo: {
                context: ucClient.ClientService,
                method: 'updateConfigInfo'
            },
            updateConfigInfos: {
                context: ucClient.ClientService,
                method: 'updateConfigInfos'
            },
            getPwdExpiredDays: {
                context: ucClient.ClientService,
                method: 'getPwdExpiredDays'
            },
            enableAutoStart: {
                context: ucClient.App,
                method: 'enableAutoStart'
            },
            disableAutoStart: {
                context: ucClient.App,
                method: 'disableAutoStart'
            },
            checkUpdates: {
                context: ucClient.UpdateService,
                method: 'checkUpdates'
            },
            update: {
                context: ucClient.UpdateService,
                method: 'update'
            },
            archiveLogFile: {
                context: ucClient.ClientService,
                method: 'archiveLogFile'
            },
            uploadLog:{
                context: ucClient.ClientService,
                method: 'uploadLog'
            },
            getClientVersion:{
                context: ucClient.App,
                method: 'getClientVersion'
            },
            getPasswordRule: {
                context: ucClient.ClientService,
                method: 'getPasswordRule'
            },
            getSessionId: {
                context: ucClient.ClientService ,
                method: 'getSessionId'
            },
            isAutoStart:{
                context: ucClient.App,
                method: 'isAutoStart'
            },
            getGkClientConnection:{
                context: ucClient.Gkclient,
                method: 'getGkClientConnection'
            },
            getGkClientReLogin:{
                context:ucClient.Gkclient,
                method: 'relogin'
             },
            getTrayPos: {
            	context: ucClient.ClientService,
                method: 'getTrayPos'
            },
            getWindowStatus: {
            	context: ucClient.ClientService,
                method: 'getWindowStatue'
            },
	        getUccServerDomain:{
                context: ucClient.ClientService,
                method: 'getUccServerDomain'
            },
            getDefaultFileSavePath:{
                context: ucClient.ClientService,
                method: 'getDefaultFileSavePath'
            },
            getMaxGroupMemberLimit:{
                context: ucClient.ClientService,
                method: 'getMaxGroupMemberLimit'
            },
            getMinGroupMemberLimit:{
                context: ucClient.ClientService,
                method: 'getMinGroupMemberLimit'
            },
	        getWindowHwnd:{
		        context: ucClient.ClientService,
		        method: 'getWindowHwnd'
	        },
            setConversationTop :{
		        context: ucClient.ClientService,
		        method: 'setConversationTop'
	        },
            getConversationInfoSync :{
		        context: ucClient.ClientService,
		        method: 'getConversationInfoSync'
	        },
            setUpdateUserNotify:{
                context: ucClient.ClientService,
                method: 'setUpdateUserNotify'
            },
            getUserNotifyStatus:{
                context: ucClient.ClientService,
                method: 'getUserNotifyStatus'
            },
            getWindowPosition: {
                context: ucClient.ClientService,
                method: 'getWindowPos'
            }
        },
        IIMService: {
            getChatHistory: {
                context: ucClient.IMService,
                method: 'getChatHistory'
            },
            resendMessage: {
                context: ucClient.IMService,
                method: 'resendMessage'
            },
            sendTextMessage: {
                context: ucClient.IMService,
                method: 'sendTextMessage'
            },
            sendImageMessage: {
                context: ucClient.IMService,
                method: 'sendImageMessage'
            },
            sendAudioMessage: {
                context: ucClient.IMService,
                method: 'sendAudioMessage'
            },
            sendVideoMessage: {
                context: ucClient.IMService,
                method: 'sendVideoMessage'
            },
            sendFileMessage: {
                context: ucClient.IMService,
                method: 'sendFileMessage'
            },
            shareFileMessage: {
                context: ucClient.IMService,
                method: 'shareFileMessage'
            },
            deleteFileMessage: {
                context: ucClient.IMService,
                method: 'deleteFileMessage'
            },
            sendCallInvitation: {
                context: ucClient.IMService,
                method: 'sendInviteCallMessage'
            },
            acceptCallInvitation: {
                context: ucClient.IMService,
                method: 'sendAcceptCallMessage'
            },
            rejectCallInvitation: {
                context: ucClient.IMService,
                method: 'sendRejectCallMessage'
            },
            finishCall: {
                context: ucClient.IMService,
                method: 'sendFinishCallMessage'
            },
            cancelCall: {
                context: ucClient.IMService,
                method: 'sendCancelCallMessage'
            },
            sendNotRespondCallMessage: {
                context: ucClient.IMService,
                method: 'sendNotRespondCallRecordMessage'
            },
            sendCallDurationMessage: {
                context: ucClient.IMService,
                method: 'sendConnectedCallRecordMessage'
            },
            sendMessageRead: {
                context: ucClient.IMService,
                method: 'sendMessageRead'
            },
            sendMessagesReadById: {
                context: ucClient.IMService,
                method: 'sendMessagesReadById'
            },
            sendMessagePlayed: {
                context: ucClient.IMService,
                method: 'sendMessagePlayed'
            },
            sendCodeMessage: {
                context: ucClient.IMService,
                method: 'sendCodeMessage'
            },
            sendCloudFileMessage:{
                context: ucClient.IMService,
                method: 'sendCloudFileMessage'
            },
	        sendImRecordVideoMessage:{
                context: ucClient.IMService,
                method: 'sendRecordVideoMessage'
            },
            getExternalMessageHistory: {
                context: ucClient.ExternalService,
                method: 'getChatHistory'
            },
            isShowExternalMessage: {
                context: ucClient.ExternalService,
                method: 'showExternalMessage'
            },
            sendReportMessage: {
                context: ucClient.IMService,
                method: 'sendReportMessage'
            }
        },
        IGroupService: {
            getChatHistory: {
                context: ucClient.GroupService,
                method: 'getChatHistory'
            },
            sendTextMessage: {
                context: ucClient.GroupService,
                method: 'sendTextMessage'
            },
            sendImageMessage: {
                context: ucClient.GroupService,
                method: 'sendImageMessage'
            },
            sendAudioMessage: {
                context: ucClient.GroupService,
                method: 'sendAudioMessage'
            },
            sendVideoMessage: {
                context: ucClient.GroupService,
                method: 'sendVideoMessage'
            },
            sendCallInvitation: {
                context: ucClient.GroupService,
                method: 'sendInviteCallMessage'
            },
            acceptCallInvitation: {
                context: ucClient.GroupService,
                method: 'sendAcceptCallMessage'
            },
            rejectCallInvitation: {
                context: ucClient.GroupService,
                method: 'sendRejectCallMessage'
            },
            finishCall: {
                context: ucClient.GroupService,
                method: 'sendFinishCallMessage'
            },
            cancelCall: {
                context: ucClient.GroupService,
                method: 'sendCancelCallMessage'
            },
            sendNotRespondMessage: {
                context: ucClient.GroupService,
                method: 'sendNotRespondCallRecordMessage'
            },
            sendRejectCallRecordMessage: {
                context: ucClient.GroupService,
                method: 'sendRejectCallRecordMessage'
            },
            sendCallDurationMessage: {
                context: ucClient.GroupService,
                method: 'sendConnectedCallRecordMessage'
            },
            sendMessageRead: {
                context: ucClient.GroupService,
                method: 'sendMessageRead'
            },
            sendMessagesReadById: {
                context: ucClient.GroupService,
                method: 'sendMessagesReadById'
            },
            sendMessagePlayed: {
                context: ucClient.GroupService,
                method: 'sendMessagePlayed'
            },
            resendMessage: {
                context: ucClient.GroupService,
                method: 'resendMessage'
            },
            forwardMessage: {
                context: ucClient.GroupService,
                method: 'forwardMessage'
            },
            getGroupList: {
                context: ucClient.GroupService,
                method: 'getGroupList'
            },
            getGroupInfo: {
                context: ucClient.GroupService,
                method: 'getGroupInfoSync'
            },
            getGroupInfoByMountId: {
                context: ucClient.GroupService,
                method: 'getGroupInfoByMountIdSync'
            },
            createGroup: {
                context: ucClient.GroupService,
                method: 'createGroup'
            },
            endGroup: {
                context: ucClient.GroupService,
                method: 'endGroup'
            },
            exitGroup: {
                context: ucClient.GroupService,
                method: 'exitGroup'
            },
            addMember: {
                context: ucClient.GroupService,
                method: 'addMember'
            },
            removeMember: {
                context: ucClient.GroupService,
                method: 'removeMember'
            },
            setGroupName: {
                context: ucClient.GroupService,
                method: 'setGroupName'
            },
            getDocumentList: {
                context: ucClient.GroupService,
                method: 'getDocumentList'
            },
            uploadDocument: {
                context: ucClient.GroupService,
                method: 'uploadDocument'
            },
            uploadCodeSnippet: {
                context: ucClient.GroupService,
                method: 'uploadCodeSnippet'
            },
            deleteDocument: {
                context: ucClient.GroupService,
                method: 'deleteDocument'
            },
            getDocumentInfo: {
                context: ucClient.GroupService,
                method: 'getDocumentInfo'
            },
            setGroupLogo: {
                context: ucClient.GroupService,
                method: 'setGroupLogo'
            },
            setTopChat: {
                context: ucClient.GroupService,
                method: 'setTopChat'
            },
            setDoNotDisturb: {
                context: ucClient.GroupService,
                method: 'setDoNotDisturb'
            },
            setReminder: {
                context: ucClient.GroupService,
                method: 'setReminder'
            },
            createComment: {
                context: ucClient.GroupService,
                method: 'createComment'
            },
            getCommentList: {
                context: ucClient.GroupService,
                method: 'getCommentList'
            },
            updateGroupAvatar: {
                context: ucClient.GroupService,
                method: 'updateGroupLogo'
            },
            sendGroupVideoMessage:{
                context: ucClient.GroupService,
                method: 'sendRecordVideoMessage'
            },
	        getUnreadUserList:{
                context: ucClient.GroupService,
                method: 'getUnreadUserList'
            },
	        uploadCloudDocument: {
                context: ucClient.GroupService,
                method: 'uploadCloudDocument'
            },
            sendCloudFileMessage: {
                context: ucClient.GroupService,
                method: 'sendCloudFileMessage'
            },
            sendReportMessage: {
                context: ucClient.GroupService,
                method: 'sendReportMessage'
            },
            getAtMessageList: {
                context: ucClient.GroupService,
                method: 'getAtMessageList'
            },
            deleteAtMessage:{
                context: ucClient.GroupService,
                method: 'deleteAtMessage'
            },
            revocationMessage: {
                context: ucClient.GroupService,
                method: 'revocationMessage'
            }
        },
        IContactService: {
            syncContactList: {
                context: ucClient.ContactService,
                method: 'getUpdatedContacts'
            },
            getBasicContactInfo: {
                context: ucClient.ContactService,
                method: 'getBasicContactInfoSync'
            },
            getContactInfo: {
                context: ucClient.ContactService,
                method: 'getContactInfoSync'
            },
            getContactList: {
                context: ucClient.ContactService,
                method: 'getContactList'
            },
            setContactLocalAvatarUrl: {
                context: ucClient.ContactService,
                method: 'updateContactAvatar'
            },
            addContact: {
                context: ucClient.ContactService,
                method: 'addCommonContact'
            },
            deleteContact: {
                context: ucClient.ContactService,
                method: 'deleteCommonContact'
            },
            searchLocalContact: {
                context: ucClient.ContactService,
                method: 'searchLocalContact'
            },
            updateCurrentUserTags: {
                context: ucClient.ContactService,
                method: 'updateUserTags'
            },
            getCurrentUserInfo: {
                context: ucClient.ContactService,
                method: 'getCurrentUserInfoSync'
            },
            updateUserInfo: {
                context: ucClient.ContactService,
                method: 'updateUserInfo'
            },
            getColleagues: {
                context: ucClient.ContactService,
                method: 'getColleagues'
            },
            getOrgInfo: {
                context: ucClient.ContactService,
                method: 'getOrgAddressBook'
            },
            getOrgAllMembers: {
                context: ucClient.ContactService,
                method: 'getOrgMember'
            },
            getCustomTags:{
                context: ucClient.ContactService,
                method: 'getUserTags'
            },
            updateCustomTags:{
                context: ucClient.ContactService,
                method: 'updateUserTags'
            },
            setUserStatus:{
                context: ucClient.ContactService,
                method: 'setUserStatus'
            },
            subscribeUserStatus:{
                context: ucClient.ContactService,
                method: 'subscribeUserStatus'
            },
            unsubscribeUserStatus: {
                context: ucClient.ContactService,
                method: 'unsubscribeUserStatus'
            },
            unsubscribeAllUserStauts: {
                context: ucClient.ContactService,
                method: 'unsubscribeAllUserStatus'
            },
	        getUserStatus:{
		        context: ucClient.ContactService,
		        method: 'getUserStatus'
	        },
            getCurrentUserFullInfo:{
                context: ucClient.ContactService,
                method: 'getCurrentUserFullInfo'
            }

        },
        IMeetingService: {
            createMeeting:{
                context: ucClient.CalendarService,
                method: 'createMeeting'
            },
            updateMeeting:{
                context: ucClient.CalendarService,
                method: 'updateMeeting'
            },
            cancelMeeting:{
                context: ucClient.CalendarService,
                method: 'cancelMeeting'
            },
            getMeetingStartURLSync:{
                context: ucClient.CalendarService,
                method: 'getMeetingStartURLSync'
            },
            getMeetingStartUrlByConfId: {
                context: ucClient.CalendarService,
                method: 'getConferenceStartUrlByConfId'
            },
            getMeetingWebStartUrl:{
                context: ucClient.CalendarService,
                method: 'getMeetingWebStartURL'
            },
            startMeetingClient:{
                context: ucClient.CalendarService,
                method: 'startMeetingClient'
            },
            checkMeetingClient:{
                context: ucClient.CalendarService,
                method: 'checkMeetingClient'
            },
            getMeetingInfoById:{
            	context: ucClient.CalendarService,
                method: 'getMeetingInfoById'
            },
            getMeetingInfoByIdSync: {
                context: ucClient.CalendarService,
                method: 'getMeetingInfoByIdSync'
            },
            getMeetingInfoByIds: {
                context: ucClient.CalendarService,
                method: 'getMeetingInfoByIds'
            },
            getMeetingList:{
                context: ucClient.CalendarService,
                method: 'getMeetingList'
            },
			getMeetingListSync:{
				context: ucClient.CalendarService,
                method: 'getMeetingListSync'
			},
            acceptMeetingInvitation:{
                context: ucClient.CalendarService,
                method: 'acceptMeetingInvitation'
            },
            rejectMeetingInvitation:{
                context: ucClient.CalendarService,
                method: 'denyMeetingInvitation'
            },
            getAccreditUserList:{
                context: ucClient.CalendarService,
                method: 'getAccreditUserList'
            },
            updateAccreditSetting:{
                context: ucClient.CalendarService,
                method: 'updateAccreditSetting'
            },
            forwardMeeting:{
                context: ucClient.CalendarService,
                method: 'forwardMeeting'
            },
            getUserScheduleList:{
                context: ucClient.CalendarService,
                method: 'getUserSchedule'
            },
            createMinutes:{
                context: ucClient.CalendarService,
                method: 'createMinutes'
            },
            updateMinutes:{
                context: ucClient.CalendarService,
                method: 'updateMinutes'
            },
            getMinutesDetails:{
                context: ucClient.CalendarService,
                method: 'getMinutesDetails'
            },
            getMinutesList:{
                context: ucClient.CalendarService,
                method: 'getMinutesList'
            },
            resendMessage: {
                context: ucClient.CalendarService,
                method: 'resendMessage'
            },
            forwardMessage: {
                context: ucClient.CalendarService,
                method: 'forwardMessage'
            },
            getChatHistory: {
                context: ucClient.CalendarService,
                method: 'getChatHistory'
            },
            sendTextMessage: {
                context: ucClient.CalendarService,
                method: 'sendTextMessage'
            },
            sendImageMessage: {
                context: ucClient.CalendarService,
                method: 'sendImageMessage'
            },
            sendAudioMessage: {
                context: ucClient.CalendarService,
                method: 'sendAudioMessage'
            },
            sendVideoMessage: {
                context: ucClient.CalendarService,
                method: 'sendVideoMessage'
            },
            setConferenceAlarm: {
                context: ucClient.CalendarService,
                method: 'setMeetingAlarm'
            },
            getMeetingClientLaunchStatus: {
                context: ucClient.Util,
                method: 'getMeetingClientStatus'
            },
            deleteMeetingRecording:{
                context: ucClient.CalendarService,
                method: 'deleteMeetingRecording'
            },
            shareMeetingRecording:{
                context: ucClient.MicroblogService,
                method: 'shareMeetingRecording'
            },
			getBriefMeetingList:{
				context: ucClient.CalendarService,
                method: 'getBriefMeetingList'
			},
			getBriefChangedMeetingList:{
				context: ucClient.CalendarService,
                method: 'getBriefChangedMeetingList'
			},
            isInMeeting:{
				context: ucClient.CalendarService,
                method: 'isInMeeting'
			},
            killMeetingClient:{
				context: ucClient.CalendarService,
                method: 'killMeetingClient'
			},
            cancelJoinMeeting:{
				context: ucClient.CalendarService,
                method: 'cancelJoinMeeting'
			},
            sendMeetingClientPosition:{
				context: ucClient.CalendarService,
                method: 'sendPosition2MeetingClient'
			},
	        uploadMeetingFile:{
		        context: ucClient.CalendarService,
		        method: 'uploadMeetingFile'
	        },
	        deleteMeetingFile:{
		        context: ucClient.CalendarService,
		        method: 'deleteMeetingFile'
	        },
            uploadCloudFile:{
		        context: ucClient.CalendarService,
		        method: 'uploadCloudFile'
	        },
            revocationMeetingMessage:{
                context: ucClient.CalendarService,
                method: 'revocationMeetingMessage'
            },
            getUserBusyFreeStatusInfo: {
                context: ucClient.CalendarService,
                method: 'getUserBusyFreeInfo'
            }
        },
        IFSService: {
            downloadFile: {
                context: ucClient.FileTransferService,
                method: 'downloadFile'
            },
            uploadFile: {
                context: ucClient.FileTransferService,
                method: 'uploadFile'
            },
            cancelDownloadFile: {
                context: ucClient.FileTransferService,
                method: 'cancelDownloadFile'
            },
            cancelUploadFile: {
                context: ucClient.FileTransferService,
                method: 'cancelUploadFile'
            },
            isFileDownloading: {
                context: ucClient.FileTransferService,
                method: 'isFileDownloading'
            },
            isFileUploading: {
                context: ucClient.FileTransferService,
                method: 'isFileUploading'
            },
            getLocalFilePath: {
                context: ucClient.FileTransferService,
                method: 'getLocalFilePathSync'
            },
            updateFileLocalPath: {
                context: ucClient.FileTransferService,
                method: 'updateLocalFilePath'
            },
            getDownloadUrlFromFid: {
                context: ucClient.FileTransferService,
                method: 'getDownloadUrlFromFid'
            },
            resumeTask: {
                context: ucClient.FileTransferService,
                method: 'resumeTask'
            }
        },
        ISearchService: {
            searchServer: {
                context: ucClient.SearchService,
                method: 'searchServer'
            },
            searchLocal: {
                context: ucClient.SearchService,
                method: 'searchLocal'
            },
            searchHistoryMsgByKeyWord: {
                context: ucClient.SearchService,
                method: 'searchMessage'
            },
            searchHistoryByNexeResult: {
                context: ucClient.GroupService,
                method: 'getSearchChatHistory'
            }
        },
        IOpenApiService: {
            getAppList: {
                context: ucClient.OpenApiService,
                method: 'getAppList'
            },
            getAppInfo: {
                context: ucClient.OpenApiService,
                method: 'getAppInfo'
            },
            getChatHistory: {
                context: ucClient.OpenApiService,
                method: 'getChatHistory'
            },
            resendMessage: {
                context: ucClient.OpenApiService,
                method: 'resendMessage'
            },
            sendTextMessage: {
                context: ucClient.OpenApiService,
                method: 'sendTextMessage'
            },
            sendFileMessage: {
                context: ucClient.OpenApiService,
                method: 'sendFileMessage'
            },
            sendImageMessage: {
                context: ucClient.OpenApiService,
                method: 'sendImageMessage'
            },
            getOAMessageStatus : {
                context: ucClient.OpenApiService ,
                method: 'getOAMessageStatus'
            },
            sendRequest : {
                context: ucClient.OpenApiService ,
                method: 'sendRequest'
            },
            sendMessagesReadById: {
                context: ucClient.OpenApiService,
                method: 'sendMessagesReadById'
            },
            sendMessageRead: {
                context: ucClient.OpenApiService,
                method: 'sendMessageRead'
            }
        },
        IMicroblogService: {
            addAttention: {
                context: ucClient.MicroblogService,
                method: 'addAttention'
            },
            cancelAttention: {
                context: ucClient.MicroblogService,
                method: 'cancelAttention'
            },
            getHomePageURL: {
                context: ucClient.MicroblogService,
                method: 'getHomePageURL'
            },
            getNewMsgCount: {
                context: ucClient.MicroblogService,
                method: 'getNewMsgCount'
            },
            getRelation: {
                context: ucClient.MicroblogService,
                method: 'getRelation'
            },
            getRelationSync: {
            	context: ucClient.MicroblogService,
                method: 'getRelationSync'
            },
            getUserhomePageURL: {
            	context: ucClient.MicroblogService,
                method: 'getUserhomePageURL'
            }
        },
        IUpdateService: {
            checkUpdates: {
                context: ucClient.UpdateService,
                method: 'checkUpdates'
            },
            update: {
                context: ucClient.UpdateService,
                method: 'update'
            }
        },
        IRemoteAssistanceService: {
            createRemoteAssistance: {
                context: ucClient.CalendarService,
                method: 'createRemoteAssistance'
            },
            sendRemoteAssistanceInvitation: {
                context: ucClient.CalendarService,
                method: 'sendRemoteAssistanceInviteMessage'
            },
            acceptRemoteAssistanceInvitation: {
                context: ucClient.CalendarService,
                method: 'sendRemoteAssistanceAcceptMessage'
            },
            rejectRemoteAssistanceInvitation: {
                context: ucClient.CalendarService,
                method: 'sendRemoteAssistanceRejectMessage'
            }
        },
        ICallService: {
            createCallById: {
                context: ucClient.AudioService,
                method: 'createCallById'
            },
            createCallByUserList: {
                context: ucClient.AudioService,
                method: 'createCallByUserList'
            },
            joinCall: {
                context: ucClient.AudioService,
                method: 'joinCall'
            },
            leaveCall:{
                context: ucClient.AudioService,
                method: 'leaveCall'
            },
            switchToPhone: {
                context: ucClient.AudioService,
                method: 'switchToPhone'
            },
            switchToVOIP: {
                context: ucClient.AudioService,
                method: 'switchToVOIP'
            },
            muteSelf: {
                context: ucClient.AudioService,
                method: 'muteSelf'
            },
            unMuteSelf: {
                context: ucClient.AudioService,
                method: 'unMuteSelf'
            },
            getUserCallStatus:{
                context: ucClient.AudioService,
                method: 'getUserCallStatus'
            },
            inviteUsersToSingleCall:{
                context: ucClient.AudioService,
                method: 'inviteUsersToSingleCall'
            },
            inviteUsersToGroupCall:{
                context: ucClient.AudioService,
                method: 'inviteUsersToGroupCall'
            },
			getHistoryPhoneNum:{
				context: ucClient.AudioService,
                method: 'getHistoryPhoneNum'
			},
            getSpeakerList:{
                context: ucClient.AudioService,
                method: 'getSpeakerList'
            },
            getMicrophoneList:{
                context: ucClient.AudioService,
                method: 'getMicrophoneList'
            },
            startSpeakerDiagnosis:{
                context: ucClient.AudioService,
                method: 'startSpeakerDiagnosis'
            },
            startMikeDiagnosis:{
                context: ucClient.AudioService,
                method: 'startMikeDiagnosis'
            },
            stopSpeakerDiagnosis:{
                context: ucClient.AudioService,
                method: 'stopSpeakerDiagnosis'
            },
            stopMikeDiagnosis:{
                context: ucClient.AudioService,
                method: 'stopMikeDiagnosis'
            },
            reconnectConference:{
                context: ucClient.AudioService,
                method: 'reconnectConference'
            },
            hangupPhone:{
                context: ucClient.AudioService,
                method: 'hangupPhone'
            },
	        getSpeakerNum:{
		        context: ucClient.AudioService,
		        method: 'getSpeakerNum'
	        },
	        getMicrophoneNum:{
		        context: ucClient.AudioService,
		        method: 'getMicrophoneNum'
	        },
	        getLastAudioInvite:{
		        context: ucClient.AudioService,
		        method: 'getLastAudioInvite'
	        },
	        getCameraList:{
		        context: ucClient.AudioService,
		        method: 'getCameraList'
	        },
	        cameraDiagnosis:{
		        context: ucClient.AudioService,
		        method: 'cameraDiagnosis'
	        },
	        stopCameraDiagnosis:{
		        context: ucClient.AudioService,
		        method: 'stopCameraDiagnosis'
	        }
        },
        IBBSService:{
            BBSRequest:{
                context:ucClient.BBSService,
                method:"BBSRequest"
            }
        },
        IPbxService: {
            getMakeCall: {
                context: ucClient.PbxService,
                method: 'makeCall'
            },
            getCallRecordLst: {
                context: ucClient.PbxService,
                method: 'getCallRecordLst'
            },
            getCallDetailsLst : {
                context: ucClient.PbxService,
                method: 'getCallDetailsLst'
            },
            answerCall : {
                context: ucClient.PbxService,
                method: 'answerCall'
            },
            makeTwoWayCall: {
                context: ucClient.PbxService,
                method: 'makeTwoWayCall'
            },
            rejectCall: {
                context: ucClient.PbxService,
                method: 'rejectCall'
            },
            hangupCall: {
                context: ucClient.PbxService,
                method: 'hangupCall'
            },
            holdCall: {
                context: ucClient.PbxService,
                method: 'holdCall'
            },
            unHoldCall: {
                context: ucClient.PbxService,
                method: 'unHoldCall'
            },
            muteCall: {
                context: ucClient.PbxService,
                method: 'muteCall'
            },
            unMuteCall: {
                context: ucClient.PbxService,
                method: 'unMuteCall'
            },
            adjustMicLevel: {
                context: ucClient.PbxService,
                method: 'adjustMicLevel'
            },
            sendDTMF: {
                context: ucClient.PbxService,
                method: 'sendDTMF'
            },
            switchRecordInCall: {
                context: ucClient.PbxService,
                method: 'switchRecordInCall'
            },
            getCurrentPBXAccount: {
                context: ucClient.PbxService,
                method: 'getCurrentPBXAccount'
            },
            getSearchCallRecord : {
                context: ucClient.PbxService,
                method: 'searchCallRecord'
            },
            checkPbxLogin:{
                context: ucClient.PbxService,
                method: 'checkLogin'
            },
            refreshMic :{
                context: ucClient.PbxService,
                method: 'refreshMic'
            },
            saveCallDetails:{
                context: ucClient.PbxService,
                method: 'saveCallDetails'
            }
        },
	    ISharingService:{
		    startDesktopShare:{
			    context: ucClient.SharingService,
			    method: 'startDesktopShare'
		    },
		    stopDesktopShare:{
			    context: ucClient.SharingService,
			    method: 'stopDesktopShare'
		    },
		    startDesktopView:{
			    context: ucClient.SharingService,
			    method: 'startDesktopView'
		    },
		    stopDesktopView:{
			    context: ucClient.SharingService,
			    method: 'stopDesktopView'
		    }
	    },
	    IMeetingRoomService:{
		    createMeetingRoom: {
			    context: ucClient.CalendarService,
			    method: 'createMeetingRoom'
		    },
		    updateMeetingRoom: {
			    context: ucClient.CalendarService,
			    method: 'updateMeetingRoom'
		    },
		    createMeetingInMeetingRoom: {
			    context: ucClient.CalendarService,
			    method: 'createMeetingInMeetingRoom'
		    },
		    getMeetingRoomList: {
			    context: ucClient.CalendarService,
			    method: 'getMeetingRoomList'
		    },
		    checkJoinMeetingRoom: {
			    context: ucClient.CalendarService,
			    method: 'checkJoinMeetingRoom'
		    },
		    checkMeetingRoomPassword: {
			    context: ucClient.CalendarService,
			    method: 'checkMeetingRoomPassword'
		    },
		    deleteMeetingRoom: {
			    context: ucClient.CalendarService,
			    method: 'deleteMeetingRoom'
		    },
			checkRightsOfCreateRoom: {
				context: ucClient.CalendarService,
				method: 'checkRightsOfCreateRoom'
			}
	    }
    }
})(uc);