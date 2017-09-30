/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function (uc) {
    uc.listener.nativeEventConfig = {
        IClientEvent: {
            OnLogin: {
                nativeEvent: ucClient.ClientService.LOGIN_EVENT,
                param: ['errorCode', 'authType', 'pbxType', 'capability']
            },
            OnLogout: {
                nativeEvent: ucClient.ClientService.LOGOUT_EVENT,
                param: ['errorCode']
            },
            OnUCASLogin: {
                nativeEvent: ucClient.ClientService.UCAS_CONNECTED_EVENT,
                param: ['errorCode']
            },
            OnUCASLogout: {
                nativeEvent: ucClient.ClientService.UCAS_DISCONNECTED_EVENT,
                param: ['errorCode']
            },
            OnSessionExpired: {
                nativeEvent: ucClient.ClientService.SESSION_EXPIRED_EVENT,
                param: []
            },
            OnPasswordReset: {
                nativeEvent: ucClient.ClientService.FORGET_PASSWORD_EVENT,
                param: ['errorCode']
            },
            OnPasswordModified: {
                nativeEvent: ucClient.ClientService.PASSWORD_MODIFY_EVENT,
                param: ['errorCode']
            },
            OnPasswordModifiedMessageReceived: {
                nativeEvent: ucClient.ClientService.PASSWORD_MODIFY_MESSAGE_RECEIVED_EVENT,
                param: []
            },
            OnPasswordRuleUpdatedMessageReceived: {
                nativeEvent: ucClient.ClientService.PASSWORD_RULE_UPDATE_MESSAGE_RECEIVED_EVENT,
                param: ['type']
            },
            OnPasswordExpiredMessageReceived: {
                nativeEvent: ucClient.ClientService.PASSWORD_EXPIRED_MESSAGE_RECEIVED_EVENT,
                param: ['dueDays']
            },
            OnAccountVerified: {
                nativeEvent: ucClient.ClientService.ACCOUNT_VERIFied_EVENT,
                param: ['errorCode', 'verifyType', 'mobile', 'email', 'exprised']
            },
            OnCodeVerified: {
                nativeEvent: ucClient.ClientService.CODE_VERIFY_EVENT,
                param: ['errorCode','exprised']
            },

            OnConversationListReceived: {
                nativeEvent: ucClient.ClientService.CONVERSATION_LIST_RECEIVED_EVENT,
                param: ['errorCode', 'isLocal', 'conversations', 'totalUnread', 'groupAtCounts']
            },
            OnConversationListComplete: {
                nativeEvent: ucClient.ClientService.CONVERSATION_LIST_COMPLETE_EVENT,
                param: ['conversationSize']
            },

            OnPlayingSoundNotification: {
                nativeEvent: ucClient.System.PLAYINGSOUND_NOTIFICATION,
                param: ['id', 'percent', 'seconds']
            },
            OnNetworkStatusChanged: {
                nativeEvent: ucClient.Net.NETWORK_CHANGED,
                param: ['connectState']
            },
            OnUpdatesStatus:{
                nativeEvent: ucClient.UpdateService.CHECK_UPDATES_EVENT,
                param: ['errorCode','updateFlag','version','notes']
            },
            OnLogUploaded:{
                nativeEvent: ucClient.ClientService.UPLOAD_LOG_EVENT,
                param: ['errorCode']
            },
            OnCodeResent:{
                nativeEvent:  ucClient.ClientService.CODE_RESEND_EVENT,
                param: ['errorCode','exprised']
            },
            OnUpdatesProgress:{
                nativeEvent: ucClient.UpdateService.CHECK_PROGRESS_EVENT,
                param: ['checkProgress']
            },
            OnArchiveLogFinished:{
                nativeEvent:  ucClient.ClientService.ARCHIVELOG_FINISH_EVENT,
                param: ['filePath']
            },
            OnMouseMoveInTray: {
                nativeEvent:  ucClient.ClientService.MOUSE_IN_ICON_EVENT,
                param: []
            },
            OnMouseMoveOutTray: {
                nativeEvent:  ucClient.ClientService.MOUSE_LEAVE_ICON_EVENT ,
                param: []
            },
            OnWindowActiveStatus: {
                nativeEvent:  ucClient.ClientService.MAIN_WINDOW_ACTIVE_EVENT,
                param: []
            },
            OnCloudLogin:{
                nativeEvent: ucClient.Gkclient.LOGIN_EVENT,
                param: ['errorCode']
            },
            OnConversationSetTopReceived: {
                nativeEvent:  ucClient.ClientService.CONVERSATION_TOP_SET_EVENT,
                param: ['errorCode', 'message']
            },
            OnFileSendToBee: {
                nativeEvent: ucClient.ClientService.SEND_FILE_TO_BEE_EVENT,
                param: ['filePath']
            },
            OnUserLoginNotify: {
                nativeEvent: ucClient.ClientService.USER_LOGIN_MSG_NOTIFY,
                param: ['errorCode','message']
            },
            OnUpdateUserNotifySet: {
                nativeEvent: ucClient.ClientService.UPDATE_USER_NOTIFY_SET_EVENT,
                param: ['errorCode','userId']
            },
            OnGetUserNotifyStatus: {
                nativeEvent: ucClient.ClientService.USER_NOTIFY_STATUS_EVENT,
                param: ['errorCode','isNotify']
            }
        },
        IIMEvent: {
            OnChatHistoryReceived: {
                nativeEvent: ucClient.IMService.CHAT_HISTORY_RECEIVED_EVENT,
                param: ['errorCode', 'userId', 'messages', 'msgCount', 'minTimestamp']
            },
            OnMessageSent: {
                nativeEvent: ucClient.IMService.MESSAGE_SENT_EVENT,
                param: ['errorCode', 'userId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnTextMessageReceived: {
                nativeEvent: ucClient.IMService.TEXT_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'textType', 'textContent', 'msgTime', 'msgTime', 'textFormat']
            },
            OnImageMessageReceived: {
                nativeEvent: ucClient.IMService.IMAGE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'fileName', 'fileSize', 'fileURL', 'thumbnails', 'msgTime']
            },
            OnAudioMessageReceived: {
                nativeEvent: ucClient.IMService.AUDIO_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'fileName', 'fileSize', 'fileURL', 'duration', 'msgTime']
            },
            OnVideoMessageReceived: {
                nativeEvent: ucClient.IMService.VIDEO_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'fileName', 'fileSize', 'fileURL', 'thumbnails', 'duration', 'msgTime']
            },
            OnFileReceived: {
                nativeEvent: ucClient.IMService.FILE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'fileName', 'fileSize', 'fileURL', 'msgTime','localPath']
            },
            OnFileShared: {
                nativeEvent: ucClient.IMService.FILE_SHARED_EVENT,
                param: ['jid', 'msgId', 'msgSeq', 'fileName', 'fileSize', 'fileURL', 'fileId']
            },
            OnFileDeleted: {
                nativeEvent: ucClient.IMService.FILE_DELETED_EVENT,
                param: ['jid', 'msgId', 'msgSeq', 'fileName', 'fileId']
            },
            OnMessageRead: {
                nativeEvent: ucClient.IMService.MESSAGE_READ_EVENT,
                param: ['errorCode', 'userId', 'readType', 'msgId', 'msgSeq']
            },
            OnMessageMediaPlayed: {
                nativeEvent: ucClient.IMService.MESSAGE_PLAYED_EVENT,
                param: ['errorCode', 'userId', 'msgId', 'msgSeq']
            },
            OnP2PCallInvitationReceived: {
                nativeEvent: ucClient.IMService.CALL_INVITED_EVENT,
                param: ['channel','userId','fromUserId', 'confId', 'confPwd', 'hostId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnP2PCallInvitationAccepted: {
                nativeEvent: ucClient.IMService.CALL_ACCEPTED_EVENT,
                param: ['channel','userId','fromUserId', 'confId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnP2PCallInvitationRejected: {
                nativeEvent: ucClient.IMService.CALL_REJECTED_EVENT,
                param: ['channel','userId','fromUserId', 'confId','hostId', 'reason', 'msgId', 'msgSeq', 'msgTime','sendState']
            },
            OnP2PCallStopped: {
                nativeEvent: ucClient.IMService.CALL_STOPPED_EVENT,
                param: ['channel', 'userId','fromUserId', 'confId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnP2PCallCancelled: {
                nativeEvent: ucClient.IMService.CALL_CANCELLED_EVENT,
                param: ['channel','userId','fromUserId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnP2PCallNoResponse: {
                nativeEvent: ucClient.IMService.CALL_NOT_RESPONDED_EVENT,
                param: ['channel', 'userId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnP2PCallDurationReceived: {
                nativeEvent: ucClient.IMService.CALL_CONNECTED_EVENT,
                param: ['channel','userId','fromUserId', 'duration', 'msgId', 'msgSeq', 'msgTime','sendState']
            },
            OnCodeReceived: {
                nativeEvent: ucClient.IMService.CODE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'creatorId', 'creatorName',
                    'codeId', 'codeTitle', 'langType', 'codeHead', 'codeDesc', 'codeSize', 'codeUrl', 'codeLine', 'createTime', 'msgTime']
            },
            OnIMSummaryVideoMessageReceived:{
                nativeEvent: ucClient.IMService.RECORD_VIDEO_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'from', 'to', 'operatorId', 'summaryId', 'summaryName', 'conferenceId', 'eventId', 'fileName', 'startTime', 'updateTime','type','videoName','fileURL','thumbnails','thumbnailsURL','duration','fileSize','operatorName','operatorAvatar', 'msgId', 'msgSeq', 'msgTime']
            },
            OnIMUnreadUserCountEvent:{
                nativeEvent: ucClient.IMService.MESSAGE_UNREAD_USER_COUNT_EVENT,
                param: ['errorCode','userId','info']
            },
            OnCloudFileReceived: {
                nativeEvent: ucClient.IMService.CLOUDFILE_CREATE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'fileName', 'fileSize', 'msgTime','detailContent']
            },
            OnExternalMessageReceived: {
                nativeEvent: ucClient.ExternalService.CHAT_HISTORY_RECEIVED_EVENT,
                param:['errorCode', 'conversationType', 'relateID', 'msgCount', 'minTimestamp', 'message']
            },
            OnIMReportMessageReceived: {
                nativeEvent: ucClient.IMService.REPORT_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from','to','eventId','msgId','msgSeq','msgTime', 'reportTitle', 'reportUrl']
            }
        },
        IGroupEvent: {
            OnGroupChatHistoryReceived: {
                nativeEvent: ucClient.GroupService.CHAT_HISTORY_RECEIVED_EVENT,
                param: ['errorCode', 'conversationType', 'groupId', 'messages', 'msgCount', 'minTimestamp']
            },
            OnGroupMessageSent: {
                nativeEvent: ucClient.GroupService.MESSAGE_SENT_EVENT,
                param: ['errorCode', 'conversationType', 'groupId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupTextMessageReceived: {
                nativeEvent: ucClient.GroupService.TEXT_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'textType', 'textContent', 'msgTime', 'textFormat','atUsers']
            },
            OnGroupImageMessageReceived: {
                nativeEvent: ucClient.GroupService.IMAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'fileName', 'fileSize', 'fileURL', 'thumbnails', 'msgTime']
            },
            OnGroupAudioMessageReceived: {
                nativeEvent: ucClient.GroupService.AUDIO_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'fileName', 'fileSize', 'fileURL', 'duration', 'msgTime']
            },
            OnGroupVideoMessageReceived: {
                nativeEvent: ucClient.GroupService.VIDEO_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'fileName', 'fileSize', 'fileURL', 'thumbnails', 'duration', 'msgTime']
            },
            OnGroupMessageRead: {
                nativeEvent: ucClient.GroupService.MESSAGE_READ_EVENT,
                param: ['errorCode', 'conversationType','groupId', 'readType', 'msgId', 'msgSeq']
            },
            OnGroupMessagePlayed: {
                nativeEvent: ucClient.GroupService.MESSAGE_PLAYED_EVENT,
                param: ['errorCode',  'groupId', 'msgId', 'msgSeq']
            },
            OnGroupCallInvitationReceived: {
                nativeEvent: ucClient.GroupService.CALL_INVITED_EVENT,
                param: ['channel','conversationType', 'groupId', 'hostId', 'confId', 'confPwd', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupCallInvitationAccepted: {
                nativeEvent: ucClient.GroupService.CALL_ACCEPTED_EVENT,
                param: ['channel','conversationType', 'groupId',  'confId','userId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupCallInvitationRejected: {
                nativeEvent: ucClient.GroupService.CALL_REJECTED_EVENT,
                param: ['channel','conversationType', 'groupId',  'confId', 'reason','userId', 'msgId', 'msgSeq', 'msgTime','sendState']
            },
            OnGroupCallStopped: {
                nativeEvent: ucClient.GroupService.CALL_STOPPED_EVENT,
                param: ['conversationType', 'groupId',  'confId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupCallDurationReceived: {
                nativeEvent: ucClient.GroupService.CALL_CONNECTED_EVENT,
                param: ['channel','conversationType', 'groupId', 'duration', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupCreated: {
                nativeEvent: ucClient.GroupService.GROUP_CREATED_EVENT,
                param: ['errorCode', 'operatorId', 'group']
            },
            OnGroupCreatedMessageReceived: {
                nativeEvent: ucClient.GroupService.GROUP_CREATE_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'from', 'to', 'adminId', 'id', 'name', 'avatarIndex', 'members', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupExited: {
                nativeEvent: ucClient.GroupService.GROUP_EXITED_EVENT,
                param: ['errorCode',  'groupId', 'userId']
            },
            OnGroupMemberExitedMessageReceived: {
                nativeEvent: ucClient.GroupService.GROUPMEMBER_DEL_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'from', 'to', 'operatorId', 'memberId', 'groupId', 'groupName', 'avatar', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupEnded: {
                nativeEvent: ucClient.GroupService.GROUP_ENDEDED_EVENT,
                param: ['errorCode',  'groupId', 'operatorId']
            },
            OnGroupEndMessageReceived: {
                nativeEvent: ucClient.GroupService.GROUP_CLOSE_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'from', 'to', 'operatorId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupMemberAdded: {
                nativeEvent: ucClient.GroupService.GROUP_MEMBER_ADDED_EVENT,
                param: ['errorCode', 'groupId', 'operatorId', 'memberIds']
            },
            OnGroupMemberAddedMessageReceived: {
                nativeEvent: ucClient.GroupService.GROUPMEMBER_ADD_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'from', 'to', 'operatorId',  'groupId', 'groupName', 'avatar', 'members', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupMemberKicked: {
                nativeEvent: ucClient.GroupService.GROUP_MEMBER_REMOVED_EVENT,
                param: ['errorCode', 'groupId', 'operatorId', 'memberIds']
            },
            GroupMemberKickedMessageReceived: {
                nativeEvent: ucClient.GroupService.GROUPMEMBER_KICK_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'from', 'to', 'operatorId', 'groupId', 'groupName',  'avatar', 'members', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupNameUpdated: {
                nativeEvent: ucClient.GroupService.GROUP_NAME_UPDATED_EVENT,
                param: ['errorCode',  'groupId', 'operatorId', 'groupName', 'pinyin']
            },
            OnGroupNameUpdatedMessageReceived: {
                nativeEvent: ucClient.GroupService.GROUPNAME_UPDATE_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'from', 'to', 'operatorId', 'groupId', 'groupName', 'NamePinYin','msgId', 'msgSeq', 'msgTime']
            },
            OnGroupAvatarUpdated: {
                nativeEvent: ucClient.GroupService.GROUP_LOGO_UPDATED_EVENT,
                param: ['errorCode',  'groupId', 'logoURL']
            },
            OnGroupAvatarUpdatedMessageReceived: {
                nativeEvent: ucClient.GroupService.GROUPAVATAR_UPDATE_MESSAGE_RECEIVED,
                param: ['conversationType', 'channel', 'from', 'to', 'operatorId', 'groupId', 'avatar', 'msgId', 'msgSeq', 'msgTime']
            },
            OnDisturbSet: {
                nativeEvent: ucClient.GroupService.DISTURB_SET_EVENT,
                param: ['errorCode', 'groupId', 'isDisturb']
            },
            OnGroupDisturbSetMessageReceived: {
                nativeEvent: ucClient.GroupService.GROUPDISTURB_SET_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'from', 'to', 'groupId', 'state', 'msgId', 'msgSeq', 'msgTime']
            },
            OnTopChatSet: {
                nativeEvent: ucClient.GroupService.TOP_CHAT_EVENT,
                param: ['errorCode',  'groupId', 'isTop']
            },
            OnGroupOnTopMessageReceived: {
                nativeEvent: ucClient.GroupService.GROUPTOP_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'from', 'to', 'operatorId', 'groupId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupReminderSet: {
                nativeEvent: ucClient.GroupService.REMINDER_SET,
                param: ['errorCode', 'groupId', 'isAlert', 'remindRule', 'remindTime']
            },
            OnGroupReminderSetMessageReceived: {
                nativeEvent: ucClient.GroupService.GROUPREMINDER_SET_MESSAGE_RECEIVED,
                param: ['conversationType', 'channel', 'from', 'to', 'groupId', 'state', 'rule', 'time', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupListReceived: {
                nativeEvent: ucClient.GroupService.GROUP_LIST_RECEIVED,
                param: ['errorCode',  'groupType', 'groups']
            },
            OnGroupInfoReceived: {
                nativeEvent: ucClient.GroupService.GROUP_INFO_RECEIVED,
                param: ['errorCode','groupId','mountId', 'group']
            },
            OnDocListReceived: {
                nativeEvent: ucClient.GroupService.DOC_LIST_RECEIVED,
                param: ['errorCode',  'groupId', 'documents']
            },
            OnDocInfoReceived: {
                nativeEvent: ucClient.GroupService.DOC_INFO_RECEIVED,
                param: ['errorCode', 'groupId', 'document']
            },
            OnDocumentUploaded: {
                nativeEvent: ucClient.GroupService.DOC_UPLOADED_RECEIVED,
                param: ['errorCode', 'groupId', 'operatorId', 'document', 'msgId']
            },
            OnDocumentUploadMessageReceived: {
                nativeEvent: ucClient.GroupService.DOCUMENTUPLOAD_MESSAGE_RECEIVED,
                param: ['conversationType', 'channel', 'sendState', 'from', 'to', 'userId', 'fileId', 'fileName', 'fileSize', 'fileUrl', 'fileFormat', 'description', 'msgId', 'msgSeq', 'msgTime', 'username','fileLocalpath']
            },
            OnDocumentShared: {
                nativeEvent: ucClient.GroupService.DOC_SHARED,
                param: [ 'errorCode', 'groupId', 'operatorId', 'document']
            },
            OnDocumentShareMessageReceived: {
                nativeEvent: ucClient.GroupService.DOCUMENTSHARE_MESSAGE_RECEIVED,
                param: ['conversationType', 'channel', 'from', 'to', 'userId', 'fileId', 'fileName', 'fileSize', 'fileUrl', 'fileFormat', 'description', 'msgId', 'msgSeq', 'msgTime', 'username']
            },
            OnDocumentDeleted: {
                nativeEvent: ucClient.GroupService.DOC_DELETED,
                param: ['errorCode',  'groupId', 'operatorId', 'docName', 'fileId']
            },
            OnDocumentDeleteMessageReceived: {
                nativeEvent: ucClient.GroupService.DOCUMENTDELETE_MESSAGE_RECEIVED,
                param: ['conversationType', 'channel', 'from', 'to', 'operatorId', 'contentId', 'docName', 'msgId', 'msgSeq', 'msgTime']
            },
            OnCommentListReceived: {
                nativeEvent: ucClient.GroupService.COMMENT_LIST_RECEIVED,
                param: ['errorCode', 'groupId', 'comments']
            },
            OnCommentCreated: {
                nativeEvent: ucClient.GroupService.COMMENT_CREATED,
                param: ['errorCode', 'groupId', 'comment','msgId']
            },
            OnCommentCreateMessageReceived: {
                nativeEvent: ucClient.GroupService.COMMENTCREATE_MESSAGE_RECEIVED,
                param: ['conversationType', 'channel', 'from', 'to', 'msgId', 'msgSeq', 'fileId', 'fileType',
                    'fileTitle', 'commentId', 'userName', 'userId', 'content', 'createTime', 'msgTime','cloudType']
            },
            OnCodeSnippetUploadMessageReceived:{
                nativeEvent: ucClient.GroupService.CODESNIPPET_UPLOAD_MESSAGE_RECEIVED,
                param: ['conversationType', 'channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq',
                    'creatorId', 'creatorName', 'codeId', 'codeTitle', 'langType', 'codeHead', 'codeDesc', 'codeSize', 'codeUrl', 'codeLine', 'createTime', 'msgTime']
            },
            OnCodeSnippetUploaded: {
                nativeEvent: ucClient.GroupService.CODESNIPPET_UPLOADED_RECEIVED,
                param: ['errorCode', 'groupId', 'operatorId', 'document', 'msgId']
            },
            OnGroupSummaryVideoMessageReceived:{
                nativeEvent: ucClient.GroupService.RECORD_VIDEO_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType','channel', 'from', 'to', 'operatorId', 'summaryId', 'summaryName', 'conferenceId', 'eventId', 'fileName', 'startTime', 'updateTime','type','videoName','fileURL','thumbnails','thumbnailsURL','duration','fileSize','operatorName','operatorAvatar', 'msgId', 'msgSeq', 'msgTime']
            },
            OnGroupUnreadUserCountEvent:{
                nativeEvent: ucClient.GroupService.MESSAGE_UNREAD_USER_COUNT_EVENT,
                param: ['errorCode','conversationType','groupId','info']
            },
            OnGroupUnreadUserListEvent:{
                nativeEvent: ucClient.GroupService.MESSAGE_UNREAD_USER_LIST_EVENT,
                param: ['errorCode','conversationType','groupId','info']
            },
            OnGroupCloudFileMessageReceived:{
                nativeEvent: ucClient.GroupService.CLOUDFILE_MODIFIED_MESSAGE_RECEIVED,
                param: ['conversationType', 'channel', 'sendState', 'from', 'to', 'fileId', 'fileName', 'fileSize', 'fileUrl', 'fileFormat', 'description', 'msgId', 'msgSeq', 'msgTime', 'username','operation','detailContent']
            },
            OnGroupMemberRoleUpdatedMessageReceived:{
                nativeEvent: ucClient.GroupService.MEMBER_ROLE_UPDATED,
                param: ['conversationType', 'channel', 'sendState', 'from', 'to', 'msgId', 'msgSeq', 'msgTime','roles']
            },
            OnGroupReportMessageReceived:{
                nativeEvent: ucClient.GroupService.REPORT_MESSAGE_RECEIVED_EVENT,
                param: ['conversationType', 'channel', 'sendState', 'from','to','eventId','msgId','msgSeq','msgTime', 'reportTitle', 'reportUrl']
            },
 			OnAtMessageListReceived:{
                nativeEvent: ucClient.GroupService.ATMESSAGE_LIST_EVENT,
                param:['errorCode','groupId','atMsg']
            },
            OnAtMessageDeleted:{
                nativeEvent: ucClient.GroupService.ATMESSAGE_DELETE_EVENT,
                param:['errorCode','conversationType','groupId','atSeq']
            },
            // added by xue.bai_2 on 2016/04/07
            OnRevocationMessage: {
                nativeEvent: ucClient.GroupService.GROUP_SERVICE_REVOCATION_MSG_EVENT,
                param: ['errorCode', 'groupId', 'msgId', 'msgSeq', 'msgTime', 'revocationType']
            },
            OnRevocationMessageReceived:{
                nativeEvent: ucClient.GroupService.GROUP_SERVICE_REVOCATION_MESSAGE_EVENT,
                param: ['channel', 'conversation', 'conversationType', 'from', 'to', 'msgId', 'msgSeq', 'msgTime', 'seqId']
            }
        },
        IContactEvent: {
            OnBasicContactInfoReceived: {
                nativeEvent: ucClient.ContactService.BASIC_CONTACT_INFO_RECEIVED_EVENT,
                param: ['errorCode', 'userIds', 'contacts']
            },
            OnContactListLoaded: {
                nativeEvent: ucClient.ContactService.CONTACT_LOADED_EVENT,
                param: ['errorCode']
            },
            OnContactPresenceChanged: {
                nativeEvent: ucClient.ContactService.CONTACT_PRESENCE_RECEIVED_EVENT,
                param: ['errorCode', 'status']
            },
            OnContactInfoReceived: {
                nativeEvent: ucClient.ContactService.CONTACT_INFO_RECEIVED_EVENT,
                param: ['errorCode', 'userIds', 'contacts']
            },
            OnContactAdded: {
                nativeEvent: ucClient.ContactService.CONTACT_ADDED_EVENT,
                param: ['errorCode', 'userId']
            },
            OnContactDeleted: {
                nativeEvent: ucClient.ContactService.CONTACT_DELETED_EVENT,
                param: ['errorCode', 'userId']
            },
            OnContactUpdated: {
                nativeEvent: ucClient.ContactService.CONTACT_UPDATED_EVENT,
                param: ['contacts']
            },
            OnCurrentUserInfoUpdated: {
                nativeEvent: ucClient.ContactService.USER_INFO_UPDATED_EVENT,
                param: ['errorCode', 'userInfo']
            },
            OnContactTagsUpdated: {
                nativeEvent: ucClient.ContactService.CONTACT_TAGS_UPDATED_EVENT,
                param: ['errorCode', 'addedTags', 'updatedTags', 'deletedTags']
            },
            OnCurrentUserTagsUpdated: {
                nativeEvent: ucClient.ContactService.USER_TAGS_UPDATED_EVENT,
                param: ['errorCode', 'tags']
            },
            OnAdminAddTags: {
                nativeEvent: ucClient.ContactService.SYSTEM_TAGS_ADDED_EVENT,
                param: ['tags']
            },
            OnAdminUpdateTags: {
                nativeEvent: ucClient.ContactService.SYSTEM_TAGS_UPDATED_EVENT,
                param: ['tags']
            },
            OnAdminDeleteTags: {
                nativeEvent: ucClient.ContactService.SYSTEM_TAGS_DELETED_EVENT,
                param: ['tag']
            },
            OnColleagueInfoReceived: {
                nativeEvent: ucClient.ContactService.COLLEAGUE_RECEIVED_EVENT,
                param: ['errorCode', 'userId', 'page', 'colleagueNumber', 'colleagues', 'leaders']
            },
            OnOrgInfoReceived: {
                nativeEvent: ucClient.ContactService.ORG_ADDRESS_BOOK_RECEIVED_EVENT,
                param: ['errorCode', 'department', 'myDepartment','taskId']
            },
            OnOrgAllMembersReceived: {
                nativeEvent: ucClient.ContactService.ORG_MEMBER_RECEIVED_EVENT,
                param: ['errorCode', 'orgId', 'userCount', 'memberIds','copyBack']
            },
            OnDepartmentNameChanged: {
                nativeEvent: ucClient.ContactService.DEPT_UPDATED_EVENT,
                param: ['operatorId', 'departmentId', 'oldDepartmentName', 'newDepartmentName', 'description', 'msgTime']
            },
            OnDepartmentTransferred: {
                nativeEvent: ucClient.ContactService.DEPT_TRANSFERED_EVENT,
                param: ['operatorId', 'userId', 'userName', 'departmentId', 'oldDepartmentName', 'departmentName', 'description', 'msgTime']
            },
            OnPositionChanged: {
                nativeEvent: ucClient.ContactService.POSITION_UPDATED_EVENT,
                param: ['operatorId', 'userId', 'userName', 'newPosition', 'oldPosition', 'departmentName', 'description', 'msgTime']
            },
            OnEmployeeJoined: {
                nativeEvent: ucClient.ContactService.EMPLOYEE_ENTRY_EVENT,
                param: ['operatorId', 'userId', 'departmentId', 'userName', 'position', 'description', 'msgTime']
            },
            OnEmployeeLeft: {
                nativeEvent: ucClient.ContactService.EMPLOYEE_LEAVED_EVENT,
                param: ['operatorId', 'userId', 'userName', 'departmentName', 'description', 'msgTime']
            },
            OnEmployeeRightsChanged: {
                nativeEvent: ucClient.ContactService.EMPLOYEE_RIGHTS_UPDATED_EVENT,
                param: ['operatorId', 'userId', 'userName', 'departmentId', 'departmentName', 'description', 'msgTime']
            },
            OnJoinCompany: {
                nativeEvent: ucClient.ContactService.COMPANY_JOINED_EVENT,
                param: ['operatorId', 'companyId','companyName', 'userId', 'userName', 'description', 'msgTime']
            },
            OnDepartmentParentChanged: {
                nativeEvent: ucClient.ContactService.DEPT_PARENT_CHANGED_EVENT,
                param: ['operatorId', 'departmentId','departmentName', 'oldDepartmentName', 'newDepartmentName', 'description', 'msgTime']
            },
            OnDepartmentDeleted: {
                nativeEvent: ucClient.ContactService.DEPT_DELETED_EVENT,
                param: ['operatorId', 'departmentId', 'departmentName', 'parentDepartmentName', 'description', 'msgTime']
            },
            OnEmployeeJoinConfirmed: {
                nativeEvent: ucClient.ContactService.EMPLOYEE_ENTRY_CONFIRMED_EVENT,
                param: ['operatorId', 'departmentId', 'departmentName', 'userName', 'userId', 'description', 'msgTime']
            },
            OnEmployeeLeaveConfirmed: {
                nativeEvent: ucClient.ContactService.EMPLOYEE_LEAVE_CONFIRMED_EVENT,
                param: ['operatorId', 'departmentId','departmentName', 'userName', 'userId', 'description', 'msgTime']
            },
            OnDepartmentTransferConfirmed: {
                nativeEvent: ucClient.ContactService.DEPT_TRANSFER_CONFIRMED_EVENT,
                param: ['operatorId', 'departmentId','departmentName', 'oldDepartmentName', 'newDepartmentName', 'description', 'msgTime']
            },
            OnUserTagsUpdated:{
                nativeEvent: ucClient.ContactService.USER_TAGS_UPDATED_EVENT,
                param: ['errorCode', 'updateTags']
            },
            OnUserStatusSetted:{
                nativeEvent: ucClient.ContactService.USER_STATUS_SET_EVENT,
                param: ['errorCode', 'userId', 'status']
            },
            OnUserStatusReceived:{
                nativeEvent: ucClient.ContactService.USER_STATUS_RECEIVED_EVENT,
                param: ['errorCode', 'status']
            },
            OnGetUserStatusResponse:{
                nativeEvent: ucClient.ContactService.GET_USER_STATUS_RESPONSE_EVENT,
                param: ['errorCode','status','copyBack']
            }
        },
        IMeetingEvent:{
            OnMeetingCreated:{
                nativeEvent:ucClient.CalendarService.CONFERENCE_CREATED_EVENT,
                param:['errorCode','meeting']
            },
            OnMeetingUpdated:{
                nativeEvent:ucClient.CalendarService.CONFERENCE_UPDATED_EVENT,
                param:['errorCode','eventId','startTime','meeting']
            },
            OnMeetingCanceled:{
                nativeEvent:ucClient.CalendarService.CONFERENCE_CANCELLED_EVENT,
                param:['errorCode','eventId','startTime','authorizerId']
            },
            //OnMeetingStartURLReceived:{
            //    nativeEvent:ucClient.CalendarService.CONFERENCE_STARTURL_RECEIVED_EVENT,
            //    param:['errorCode', 'confId','startURL']
            //},
            OnMeetingStartUrlByConfIdReceived: {
                nativeEvent: ucClient.CalendarService.CONFERENCE_STARTURL_RECEIVED_BYCONFID_EVENT,
                param: ['errorCode', 'startURL', 'confId']
            },
            OnMeetingWebStartUrlReceived:{
                nativeEvent:ucClient.CalendarService.CONFERENCE_WEBSTARTURL_RECEIVED_EVENT,
                param:['errorCode','startURL','confId']
            },
            OnMeetingListReceived:{
                nativeEvent:ucClient.CalendarService.MEETING_LIST_RECEIVED_EVENT,
                param:['errorCode','requestId','startTime','endTime','eventIdFilter','authorizerId','meetings']
            },
            OnMeetingInvitationAccepted:{
                nativeEvent:ucClient.CalendarService.MEETING_INVITATION_ACCEPTED_EVENT,
                param:['errorCode','eventId', 'startTime', 'authorizerId']
            },
            OnMeetingInvitationRejected:{
                nativeEvent:ucClient.CalendarService.MEETING_INVITATION_DENIED_EVENT,
                param:['errorCode','eventId', 'startTime', 'authorizerId']
            },
            OnAccreditUserListReceived:{
                nativeEvent:ucClient.CalendarService.ACCREDIT_USERLIST_RECEIVED_EVENT,
                param:['errorCode','accreditUsers']
            },
            OnAccreditSettingUpdated:{
                nativeEvent:ucClient.CalendarService.ACCREDIT_SETTING_UPDATED_EVENT,
                param:['errorCode','shareUserId','status']
            },
            OnMeetingForwarded:{
                nativeEvent:ucClient.CalendarService.MEETING_FORWARDED_EVENT,
                param:['errorCode','meeting']
            },
            OnUserScheduleListReceived:{
                nativeEvent:ucClient.CalendarService.USER_SCHEDULE_RECEIVED_EVENT,
                param:['errorCode','requestId','schedules']
            },
            OnMinutesCreated:{
                nativeEvent:ucClient.CalendarService.MINUTES_CREATED_EVENT,
                param:['errorCode','eventId','startTime','minutesId','minutesTitle']
            },
            OnMinutesUpdated:{
                nativeEvent:ucClient.CalendarService.MINUTES_UPDATED_EVENT,
                param:['errorCode','eventId','minutesId','minutesTitle']
            },
            OnMinutesDetailsReceived:{
                nativeEvent:ucClient.CalendarService.MINUTES_DETAILS_RECEIVED_EVENT,
                param:['errorCode','eventId','minutesId','creatorId','createTime','content','startTime']
            },

            OnMinutesListReceived:{
                nativeEvent:ucClient.CalendarService.MINUTES_LIST_RECEIVED_EVENT,
                param:['errorCode','eventId','startTime','minutes']
            },
            OnMeetingMessageSent: {
                nativeEvent: ucClient.CalendarService.MESSAGE_SENT_EVENT,
                param: ['errorCode', 'groupId', 'eventId', 'startTime', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingChatHistoryReceived: {
                nativeEvent: ucClient.CalendarService.CHAT_HISTORY_RECEIVED_EVENT,
                param: ['errorCode', 'groupId', 'msgCount', 'minTimestamp']
            },
            OnMeetingTextMessageReceived: {
                nativeEvent: ucClient.CalendarService.TEXT_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'conferenceId', 'eventId', 'startTime', 'textType', 'textContent', 'msgId', 'msgSeq', 'msgTime', 'textFormat']
            },
            OnMeetingImageMessageReceived: {
                nativeEvent: ucClient.CalendarService.IMAGE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'conferenceId',  'eventId', 'startTime',  'fileName', 'fileSize', 'fileURL', 'thumbnails', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingAudioMessageReceived: {
                nativeEvent: ucClient.CalendarService.AUDIO_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'conferenceId',  'eventId', 'startTime',  'fileName', 'fileSize', 'fileURL', 'duration', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingVideoMessageReceived: {
                nativeEvent: ucClient.CalendarService.VIDEO_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from', 'to', 'conferenceId',  'eventId', 'startTime',  'fileName', 'fileSize', 'fileURL', 'thumbnails', 'duration', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingInviteMessageReceived: {
                nativeEvent: ucClient.CalendarService.MEETING_INVITE_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'from', 'to', 'conferenceId', 'eventId',  'title', 'authorizerId', 'hostId', 'isCycle', 'startTime', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingAcceptMessageReceived: {
                nativeEvent: ucClient.CalendarService.MEETING_ACCEPT_MESSAGE_RECEIVED,
                param: ['channel', 'from', 'to', 'conferenceId', 'eventId', 'title', 'authorizerId', 'hostId', 'isCycle', 'startTime', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingRejectMessageReceived: {
                nativeEvent: ucClient.CalendarService.MEETING_DENY_MESSAGE_RECEIVED,
                param: ['channel', 'from', 'to', 'conferenceId', 'eventId', 'title', 'authorizerId', 'hostId', 'isCycle', 'startTime', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingUpdateMessageReceived: {
                nativeEvent: ucClient.CalendarService.MEETING_UPDATE_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'from', 'to', 'conferenceId', 'eventId', 'title', 'authorizerId', 'hostId', 'isCycle', 'startTime', 'updateFlag', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingForwardMessageReceived: {
                nativeEvent: ucClient.CalendarService.MEETING_FORWARD_MESSAGE_RECEIVED,
                param: ['channel', 'from', 'to', 'conferenceId', 'eventId', 'title', 'authorizerId', 'hostId', 'isCycle', 'startTime', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingCancelMessageReceived: {
                nativeEvent: ucClient.CalendarService.MEETING_CANCEL_MESSAGE_RECEIVED,
                param: ['channel', 'from', 'to', 'conferenceId', 'eventId', 'title', 'groupId', 'authorizerId', 'hostId', 'isCycle', 'startTime', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMinutesCreateMessageReceived: {
                nativeEvent: ucClient.CalendarService.MINUTES_CREATE_MESSAGE_RECEIVED,
                param: ['channel', 'from', 'to', 'operatorId', 'minutesId', 'minutesTitle','meetingId', 'eventId', 'confName', 'startTime', 'updateTime', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMinutesUpdateMessageReceived: {
                nativeEvent: ucClient.CalendarService.MINUTES_UPDATE_MESSAGE_RECEIVED,
                param: ['channel', 'from', 'to', 'operatorId',  'minutesId', 'minutesTitle','meetingId', 'eventId', 'confName', 'startTime', 'updateTime', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingAccreditMessageReceived: {
                nativeEvent: ucClient.CalendarService.ACCREDIT_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'from', 'to', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingAccreditCancelledMessageReceived: {
                nativeEvent: ucClient.CalendarService.ACCREDIT_CANCEL_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'from', 'to',  'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingPartJoinMessageReceived: {
                nativeEvent: ucClient.CalendarService.CONFERENCE_PARTJOIN_MESSAGE_RECEIVED,
                param: ['channel', 'from', 'to',  'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingPartCancelMessageReceived: {
                nativeEvent: ucClient.CalendarService.CONFERENCE_PARTCANCEL_MESSAGE_RECEIVED,
                param: ['channel', 'from', 'to',  'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingReminderMessageReceived: {
                nativeEvent: ucClient.CalendarService.MEETING_ALARMED_EVENT,
                param: ['meetings']
            },
            OnMeetingSummaryVideoMessageReceived: {
                nativeEvent: ucClient.CalendarService.RECORD_VIDEO_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'from', 'to', 'operatorId', 'summaryId', 'summaryName', 'conferenceId', 'eventId', 'fileName', 'startTime', 'updateTime','type','videoName','fileURL','thumbnails','thumbnailsURL','duration','fileSize','operatorName','operatorAvatar', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingRecordingDeleted:{
                nativeEvent: ucClient.CalendarService.MEETING_RECORDING_DELETED_EVENT,
                param: ['errorCode','eventId','startTime']
            },
            OnMeetingUnreadUserCountEvent:{
                nativeEvent: ucClient.CalendarService.MESSAGE_UNREAD_USER_COUNT_EVENT,
                param: ['errorCode','conversationType','groupId','info']
            },
            OnBriefMeetingListReceived:{
                nativeEvent: ucClient.CalendarService.MEETING_BRIEF_LIST_RECEIVED_EVENT,
                param: ['errorCode', 'requestId', 'startTime', 'endTime', 'authorizerId', 'meetings']
            },
            OnBriefChangedMeetingListReceived:{
                nativeEvent: ucClient.CalendarService.MEETING_BRIEF_CHANGED_LIST_RECEIVED_EVENT,
                param: ['errorCode', 'requestId', 'startTime', 'endTime', 'authorizerId', 'meetings']
            },
            OnMeetingInfoReceivedById:{
                nativeEvent: ucClient.CalendarService.MEETING_INFO_RECEIVED_BY_ID_EVENT,
                param: ['errorCode', 'eventId', 'authorizerId', 'meetings']
            },
            OnMeetingInfoReceivedByIds:{
                nativeEvent: ucClient.CalendarService.MEETING_INFO_RECEIVED_BY_IDS_EVENT,
                param: ['errorCode', 'requestId', 'authorizerId', 'meetings']
            },
            OnMeetingJoined:{
                nativeEvent: ucClient.CalendarService.MEETING_JOINED_EVENT,
                param: []
            },
            OnMeetingJoinFailed: {
                nativeEvent: ucClient.CalendarService.MEETING_JOIN_FAILED_EVENT,
                param: []
            },
            OnMeetingClosed:{
                nativeEvent: ucClient.CalendarService.MEETING_CLOSED_EVENT,
                param: []
            },
            OnMeetingDocumentUploadMessageReceived:{
                nativeEvent: ucClient.CalendarService.DOCUMENTUPLOAD_MESSAGE_RECEIVED,
                param: ['eventId','conversationType', 'channel', 'sendState', 'from', 'to', 'userId', 'fileId', 'fileName', 'fileSize', 'fileUrl', 'fileFormat', 'description', 'msgId', 'msgSeq', 'msgTime', 'cloudType','fileLocalpath']
            },
            OnMeetingFileUploaded:{
                nativeEvent: ucClient.CalendarService.MEETING_FILE_UPLOADED_EVENT,
                param: ['errorCode', 'eventId', 'operatorId', 'document', 'msgId']
            },
            OnMeetingFileDeleted:{
                nativeEvent: ucClient.CalendarService.MEETING_FILE_DELETEDED_EVENT,
                param: ['errorCode']
            },
            OnMeetingFileDeleteMessageReceived:{
                nativeEvent: ucClient.CalendarService.DOCUMENTDELETE_MESSAGE_RECEIVED,
                param: ['eventId','conversationType', 'channel', 'from', 'to', 'operatorId', 'contentId', 'docName', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMeetingReportMessageReceived: {
                nativeEvent: ucClient.CalendarService.REPORT_MESSAGE_RECEIVED_EVENT,
                param: ['channel', 'sendState', 'from','to','eventId','msgId','msgSeq','msgTime', 'reportTitle', 'reportUrl']
            },
            OnMeetingMessageRevocation: {
                nativeEvent: ucClient.CalendarService.MEETING_SERVICE_REVOCATION_MSG_EVENT,
                param: ['errorCode', 'groupId', 'msgId', 'msgSeq','msgTime','revocationType']
            },
            OnMeetingMessageRevocationReceived: {
                nativeEvent: ucClient.CalendarService.MEETING_SERVICE_REVOCATION_MESSAGE_EVENT,
                param: ['channel', 'conversation', 'conversationType', 'from','to','msgId', 'msgSeq', 'msgTime', 'seqId', 'eventId']
            },
            OnUserBusyFreeStatusInfoReceived: {
                nativeEvent: ucClient.CalendarService.USER_BUSYFREE_INFO_RECEIVED_EVENT,
                param: ['errorCode', 'requestId', 'schedules']
            },
            OnMeetingReadyStatusReceived: {
                nativeEvent: ucClient.CalendarService.MEETING_READIED_EVENT,
                param: []
            }
        },
        IFSEvent: {
            OnFileDownloading:{
                nativeEvent:ucClient.FileTransferService.FILETRANSFER_FILE_DOWNLOADING_EVENT,
                param:['errorCode','taskId','percent']
            },
            OnFileDownloaded:{
                nativeEvent:ucClient.FileTransferService.FILETRANSFER_FILE_DOWNLOADED_EVENT,
                param:['errorCode','taskId','savePath']
            },
            OnFileUploading:{
                nativeEvent:ucClient.FileTransferService.FILETRANSFER_FILE_UPLOADING_EVENT,
                param:['errorCode','taskId','percent']
            },
            OnFileUploaded:{
                nativeEvent:ucClient.FileTransferService.FILETRANSFER_FILE_UPLOADED_EVENT,
                param:['errorCode','taskId','fileName', 'fileSize', 'operatorId', 'fid', 'operatorTime', 'downloadURL', 'convertDownloadURL']
            }
        },
        ISearchEvent: {
            OnServerSearchResultReceived: {
                nativeEvent:ucClient.SearchService.SERVER_SEARCH_RESULT_RECEIVED,
                param:['errorCode', 'requestId', 'contacts', 'groups', 'groupChats', 'cloudGroups', 'conferenceInfos', 'messages','chatMessages','chatMsgNum']
            },
            OnLocalSearchResultReceived: {
                nativeEvent:ucClient.SearchService.LOCAL_SEARCH_RESULT_RECEIVED,
                param:['errorCode', 'requestId', 'contacts', 'groups']
            },
            OnSearchNextResultReceived:
            {
                nativeEvent:ucClient.GroupService.SEARCH_CHAT_HISTORY_RECEIVED_EVENT,
                param:['errorCode', 'conversationType', 'relateId', 'msgCount','minTimestamp','maxTimestamp']
            }
        },
        IOpenApiEvent: {
            OnAppListReceived: {
                nativeEvent:ucClient.OpenApiService.APP_LIST_RECEIVED_EVENT,
                param:['errorCode', 'apps']
            },
            OnGetAppInfoReceived: {
                nativeEvent:ucClient.OpenApiService.APP_INFO_RECEIVED_EVENT,
                param:['errorCode', 'apps']
            },
            OnApiChatHistoryReceived: {
                nativeEvent:ucClient.OpenApiService.CHAT_HISTORY_RECEIVED_EVENT,
                param:['errorCode', 'appId', 'msgCount', 'minTimestamp']
            },
            OnOpenApiTextMessageReceived: {
                nativeEvent:ucClient.OpenApiService.TEXT_MESSAGE_RECEIVED_EVENT,
                param:['channel', 'sendState', 'from', 'to', 'textTitle', 'textContent', 'textType', 'detailContent', 'msgId', 'msgSeq', 'msgTime', 'detailAuth']
            },
            OnOpenApiImageMessageReceived: {
                nativeEvent:ucClient.OpenApiService.IMAGE_RECEIVED_EVENT,
                param:['channel', 'sendState', 'from', 'to', 'imageId', 'imageType', 'imageData', 'msgId', 'msgSeq', 'msgTime']
            },
            OnOpenApiFileMessageReceived: {
                nativeEvent:ucClient.OpenApiService.FILE_RECEIVED_EVENT,
                param:['channel', 'sendState', 'from', 'to', 'fileId', 'fileType', 'fileName', 'fileSize', 'msgId', 'msgSeq', 'msgTime']
            },
            OnOpenApiMessageSent: {
                nativeEvent:ucClient.OpenApiService.MESSAGE_SENT_EVENT,
                param:['errorCode', 'appId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnOAMessageStatusReceived: {
                nativeEvent:ucClient.OpenApiService.OAMESSGAE_STATUS_RECEIVED_EVENT,
                param:['errorCode', 'appId', 'msgSeq', 'status']
            },
            OnOAMessageReceived: {
                nativeEvent:ucClient.OpenApiService.OAMESSGAE_RECEIVED_EVENT,
                param:['channel', 'from', 'to', 'title', 'titleColor', 'titleBkColor', 'titleElements', 'status', 'data', 'detailUrl', 'msgId', 'msgSeq', 'msgTime', 'detailAuth', 'oaExternalData', 'titleStyle','customizedType','customizedData']
            },
            OnOAMessageStatusChanged: {
                nativeEvent:ucClient.OpenApiService.OAMESSGAE_STATUS_CHANGED_RECEIVED_EVENT,
                param:['appId', 'msgSeq', 'status', "from", 'externalData']
            },
            OnPushMessageAck: {
                nativeEvent:ucClient.OpenApiService.PUSH_MESSAGE_ACK_EVENT,
                param:['channel', 'errorCode', 'from', 'to',  'conversation', 'msgId', 'msgSeq', 'msgTime']
            },
            OnMessageRead: {
                nativeEvent:ucClient.OpenApiService.MESSAGE_READ_EVENT,
                param:['errorCode', 'userId', 'readType', 'msgId', 'msgSeq']
            },
            OnAppStatusChangeReceived: {
                nativeEvent:ucClient.OpenApiService.MESSAGE_APP_STATUS_CHANGED_RECEIVED_EVENT,
                param:['channel', 'appId', 'status', 'externalData']
            },
            OnAppUntreadedReceived: {
                nativeEvent:ucClient.OpenApiService.MESSAGE_APP_EVENT_CHANGED_RECEIVED_EVENT,
                param:['channel', 'appId', 'eventCount', 'externalData']
            },
            OnOACustomizedMessageReceived:{
              nativeEvent:ucClient.OpenApiService.CUSTOMIZEDMESSGAE_RECEIVED_EVENT,
              param:['channel','from','data1','data2','data3','data4']
            }
        },
        IMicroblogEvent: {
            OnNewMsgCountGet: {
                nativeEvent: ucClient.MicroblogService.NEWMSGCOUNT_GET_EVENT,
                param:['errorCode', 'countAll', 'relationCount', 'timenewsCount', 'pollCountm', 'caseCount']
            },
            OnAttentionAdd: {
                nativeEvent: ucClient.MicroblogService.ADD_ATTENTION_EVENT,
                param:['errorCode', 'userId', 'relation']
            },
            OnAttentionCancelled: {
                nativeEvent: ucClient.MicroblogService.CANCEL_ATTENTION_EVENT,
                param:['errorCode', 'userId', 'relation']
            }
        },
        IUpdateEvent: {
            OnCheckUpdates: {
                nativeEvent: ucClient.UpdateService.CHECK_UPDATES_EVENT,
                param:['errorCode', 'updateFlag', 'version', 'notes', 'downloadUrl']
            }
        },
        IRemoteAssistanceEvent: {
            OnRemoteAssistanceCreated: {
                nativeEvent: ucClient.CalendarService.REMOTE_ASSISTANCE_CREATED_EVENT,
                param:['errorCode', 'userId', 'conferenceId', 'attendeePassword', 'startURL']
            },
            OnRemoteAssistanceInvitationReceived: {
                nativeEvent: ucClient.CalendarService.REMOTE_ASSISTANCE_INVITE_MSG_RECEIVED_ENVNT,
                param:['channel', 'from', 'conferenceId', 'conferencePwd', 'msgId', 'msgSeq', 'msgTime']
            },
            OnRemoteAssistanceInvitationAccepted: {
                nativeEvent: ucClient.CalendarService.REMOTE_ASSISTANCE_ACCEPT_MSG_RECEIVED_ENVNT,
                param:['channel', 'from', 'conferenceId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnRemoteAssistanceInvitationRejected: {
                nativeEvent: ucClient.CalendarService.REMOTE_ASSISTANCE_REJECT_MSG_RECEIVED_ENVNT,
                param:['channel', 'from', 'conferenceId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnRemoteAssistanceEnded: {
                nativeEvent: ucClient.CalendarService.REMOTE_ASSISTANCE_END_MSG_RECEIVED_ENVNT,
                param:['channel', 'conferenceId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnRemoteAssistanceInvitationSent: {
                nativeEvent: ucClient.CalendarService.REMOTE_ASSISTANCE_INVITE_MSG_SENT_ENVNT,
                param:['errorCode', 'conferenceId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnRemoteAssistanceAcceptMsgSent: {
                nativeEvent: ucClient.CalendarService.REMOTE_ASSISTANCE_ACCEPT_MSG_SENT_ENVNT,
                param:['errorCode', 'conferenceId', 'msgId', 'msgSeq', 'msgTime']
            },
            OnRemoteAssistanceRejectMsgSent: {
                nativeEvent: ucClient.CalendarService.REMOTE_ASSISTANCE_REJECT_MSG_SENT_ENVNT,
                param:['errorCode', 'conferenceId', 'msgId', 'msgSeq', 'msgTime']
            }
        },
        ICallEvent: {
            OnCallCreated: {
                nativeEvent: ucClient.AudioService.CALL_CREATED_EVENT,
                param:['errorCode', 'conferenceId', 'userId', 'callType']
            },
            OnSelfJoinedCall: {
                nativeEvent: ucClient.AudioService.CALL_JOINED_EVENT,
                param:['errorCode', 'conferenceId', 'userId', 'callType']
            },
            OnSelfLeftCall: {
                nativeEvent: ucClient.AudioService.CALL_LEAVED_EVENT,
                param:['errorCode', 'conferenceId', 'userId', 'callType']
            },
            OnUserJoinedCall: {
                nativeEvent: ucClient.AudioService.USER_ADDED_EVENT,
                param:['conferenceId', 'userID', 'isMute', 'audioType', 'phoneNumber']
            },
            OnUserLeftCall: {
                nativeEvent: ucClient.AudioService.USER_REMOVED_EVENT,
                param:['conferenceId', 'userID']
            },
            OnUserCallPropertyChanged: {
                nativeEvent: ucClient.AudioService.USER_PROPERTY_CHANGED_EVENT,
                param:['conferenceId', 'userID', 'isMute', 'audioType', 'phoneNumber', 'propertyType']
            },
            OnCallPhoneFailed: {
                nativeEvent: ucClient.AudioService.CALL_PHONE_FAILED_EVENT,
                param:['errorCode', 'conferenceId', 'phoneNumber']
            },
            OnCallDisconnected: {
                nativeEvent: ucClient.AudioService.CONFERENCE_DISCONNECTED_EVENT,
                param:['errorCode','conferenceId']
            },
            OnCallReconnected: {
                nativeEvent: ucClient.AudioService.CONFERENCE_RECONNECTED_EVENT,
                param:['errorCode', 'conferenceId']
            },
            OnCallQualityChanged:{
                nativeEvent: ucClient.AudioService.VOIP_NETWORK_QUALITY_CHANGED_EVENT,
                param:['conferenceId', 'userId','callType','quality']
            },
            OnCallSpeakingStatusChanged:{
                nativeEvent: ucClient.AudioService.SPEAKING_STATUS_CHANGED_EVENT,
                param:['conferenceId', 'userID','callType','isSpeaking']
            },
            OnCallAllLeft:{
                nativeEvent: ucClient.AudioService.CONFERENCE_ENDED_EVENT,
                param:['errorCode', 'conferenceId','callType']
            },
            OnUserCallStatusReceived:{
                nativeEvent: ucClient.AudioService.USER_CALL_STATUS_GOT_EVENT,
                param:['errorCode', 'id', 'type', 'callStatus']
            },
            OnInviteUsersToCall:{
                nativeEvent: ucClient.AudioService.USERS_ADDED_TO_CALL_EVENT,
                param:['errorCode', 'conferenceId', 'callType', 'userID']
            },
            OnGetHistoryPhoneNumReceived:{
                nativeEvent: ucClient.AudioService.HISTORY_CALL_PHONENUM_GOT_EVENT,
                param:['errorCode', 'phoneNumbers']
            },
            OnGetSpeakerList:{
                nativeEvent: ucClient.AudioService.USER_SPEAKERS_GOT_EVENT,
                param:['errorCode', 'speakers']
            },
            OnGetMicrophoneList:{
                nativeEvent: ucClient.AudioService.USER_MICROPHONES_GOT_EVENT,
                param:['errorCode', 'microphone']
            },
            OnMikeDiagnosisStatus:{
                nativeEvent: ucClient.AudioService.MIKE_LEVEL_GOT_EVENT,
                param:['mikeName', 'mikeLevel']
            },
            OnSpeakerDiagnosisStatus:{
                nativeEvent: ucClient.AudioService.SPEAKER_LEVEL_GOT_EVENT,
                param:['speakerName', 'speakerLevel']
            },
            OnSpeakerDiagnosisStop:{
                nativeEvent: ucClient.AudioService.SPEAKER_PLAY_FINISHED_EVENT,
                param:['speakerName']
            },
            OnAudioServiceReady:{
                nativeEvent: ucClient.AudioService.AUDIO_READY_EVENT,
                param:['audioReadyStatus']
            },
            OnAudioDeviceStatusChanged:{
                nativeEvent: ucClient.AudioService.AUDIO_DEVICE_STATUS_CHANGED_EVENT,
                param:['audioDeviceStatusChanged']
            }
        },
        IBBSEvent:{
            BBSServiceOnResponse:{
                nativeEvent:ucClient.BBSService.BBS_SERVICE_MAIN_EVENT,
                param:['errorCode','requestID','response']
            }
        },
	IPbxEvent: {
            /*  Pbx认证接口 */
            OnPbxLogin:{
                nativeEvent: ucClient.PbxService.PBX_LOGINPBX_EVENT,
                param: ['errorCode']
            },
            /* 获取通话列表 */
            OnPbxListReceived:{
                nativeEvent:ucClient.PbxService.PBX_GETCALLRECORDLST_EVENT,
                param:['errorCode','data']
            },
            /*  通话状变化 */
            OnMakeCallStatus: {
                nativeEvent: ucClient.PbxService.PBX_CALLSTATUSCHANGED_EVENT,
                param:['errorCode','data']
            },
            OnMakeCallStatusReceived: {
                nativeEvent: ucClient.PbxService.PBX_MAKECALL_EVENT,
                param:['errorCode','data']
            },
//            /*  PJSIP状变*/
//            OnPbxPJSIIPStatus: {
//                nativeEvent: ucClient.PbxService.PBX_PJSIPSERVICESTATECHANGED_EVENT,
//                param:['callID', 'callState', 'resultCode','callUUID']
//            },
//            /*  双向呼叫通话状�?��?�知 */
//            OnPbxTwoCallStatus: {
//                nativeEvent: ucClient.PbxService.PBX_TWOWAYCALLSTATUSCHANGED_EVENT,
//                param:['callState', 'callUUID', 'caller','callee']
//            },
            /*  来电通知 */
            OnPbxIncomingNotification: {
                nativeEvent: ucClient.PbxService.PBX_INCOMINGCALL_EVENT,
                param:['errorCode', 'data']
            },
            OnPbxHangUpCallStatus: {
               nativeEvent: ucClient.PbxService.PBX_HANGUPCALL_EVENT,
               param:['errorCode','data']
            },
            /* 获取通话详情 */
            OnPbxCallListDetail: {
               nativeEvent: ucClient.PbxService.PBX_GETCALLDETAILSLST_EVENT,
               param:['errorCode','data']
            },
            OnPbxGetSearchCall: {
                nativeEvent: ucClient.PbxService.PBX_SEARCHCALLRECORD_EVENT,
                param:['errorCode','data']
            }
        },
	    ISharingEvent:{
		    OnSharingError:{
			    nativeEvent: ucClient.SharingService.SHARING_ERROR_EVENT,
			    param:['statusCode']
		    },
		    OnDesktopShared:{
			    nativeEvent: ucClient.SharingService.DESKTOP_SHARED_EVENT ,
			    param:['conferenceId','shareUserId']
		    },
		    OnDesktopShareStoped:{
			    nativeEvent: ucClient.SharingService.DESKTOP_SHARE_STOPED_EVENT,
			    param:['conferenceId','shareUserId']
		    },
		    OnDesktopViewerShareStarted:{
			    nativeEvent: ucClient.SharingService.DESKTOP_VIEWER_STARTED_EVENT,
			    param:['conferenceId']
		    },
		    OnDesktopViewerShareStoped:{
			    nativeEvent: ucClient.SharingService.DESKTOP_VIEWER_STOPPED_EVENT,
			    param:['conferenceId']
        }
	    },
	    IMeetingRoomEvent:{
		    OnMeetingRoomCreated:{
			    nativeEvent: ucClient.CalendarService.MEETINGROOM_CREATED_EVENT,
			    param:['errorCode','meetingRoom']
		    },
		    OnMeetingRoomUpdated:{
			    nativeEvent: ucClient.CalendarService.MEETINGROOM_UPDATED_EVENT,
			    param:['errorCode','meetingRoom']
		    },
		    OnMeetingCreatedInMeetingRoom:{
			    nativeEvent: ucClient.CalendarService.MEETINGROOM_MEETING_CREATED_EVENT,
			    param:['errorCode','roomId','meeting']
		    },
		    OnMeetingRoomListReceived:{
			    nativeEvent: ucClient.CalendarService.MEETINGROOM_LIST_RECEIVED_EVENT,
			    param:['errorCode','orgId', 'lastGetTime', 'rooms']
		    },
		    OnCheckJoinMeetingRoom:{
			    nativeEvent: ucClient.CalendarService.MEETINGROOM_CHECK_JOIN_EVENT,
			    param:['errorCode','roomId', 'hasPassword', 'isValid','mayJoin']
		    },
		    OnCheckMeetingRoomPassword:{
			    nativeEvent: ucClient.CalendarService.MEETINGROOM_CHECK_PASSWORD_EVENT,
			    param:['errorCode','roomId', 'password','value']
		    },
		    OnMeetingRoomDeleted:{
			    nativeEvent: ucClient.CalendarService.MEETINGROOM_DELETED_EVENT,
			    param:['errorCode','roomId']
		    },
		    OnRightsChecked:{
			    nativeEvent: ucClient.CalendarService.MEETINGROOM_RIGHTS_CHECKED_EVENT,
			    param:['errorCode']
		    }
	    }
    }
})(uc);