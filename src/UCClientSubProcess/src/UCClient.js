var ucClient;

if (!ucClient) {
	ucClient = {};
}

if (!ucClient.ClientService) {
	ucClient.ClientService = {};
}

if (!ucClient.ContactService) {
	ucClient.ContactService = {};
};

if (!ucClient.IMService) {
	ucClient.IMService = {};
};

if (!ucClient.GroupService) {
	ucClient.GroupService = {};
};

if (!ucClient.CalendarService) {
	ucClient.CalendarService = {};
};

if (!ucClient.FileTransferService) {
	ucClient.FileTransferService = {};
};

if (!ucClient.PbxService) {
    ucClient.PbxService = {};
};

if (!ucClient.CatchScreenService) {
    ucClient.CatchScreenService = {};
}

if (!ucClient.SearchService) {
    ucClient.SearchService = {};
}

if (!ucClient.MicroblogService) {
    ucClient.MicroblogService = {};
}

if (!ucClient.BBSService) {
    ucClient.BBSService = {};
}

if (!ucClient.System) {
    ucClient.System = {};
}

if (!ucClient.Util) {
    ucClient.Util = {};
}

if (!ucClient.AudioService) {
    ucClient.AudioService = {};
}

if (!ucClient.SharingService) {
    ucClient.SharingService = {}
}

if (!ucClient.Log) {
    ucClient.Log = {};
}

if (!ucClient.Net) {
    ucClient.Net = {};
}

if (!ucClient.App) {
    ucClient.App = {};
}

if(!ucClient.OpenApiService){
    ucClient.OpenApiService = {};
}

if (!ucClient.UpdateService) {
    ucClient.UpdateService = {};
}

if (!ucClient.ExternalService) {
    ucClient.ExternalService = {};
}

if (!ucClient.Gkclient) {
    ucClient.Gkclient = {};
}

(function() {
	native function CallNativeMethodAync();
	
    // -------------------------------------------------------------------------------
	// ClientService事件
    //登陆
	ucClient.ClientService.LOGIN_EVENT = 'ClientServiceOnLogin';
	
    //注销
	ucClient.ClientService.LOGOUT_EVENT = 'ClientServiceOnLogout';

    //重设密码
	ucClient.ClientService.PASSWORD_MODIFY_EVENT = 'ClientServiceOnPasswordModified';

    //其他登陆终端修改密码
	ucClient.ClientService.PASSWORD_MODIFY_MESSAGE_RECEIVED_EVENT = 'ClientServiceOnPasswordModifyMessageReceived';

    //密码复杂性规则变化
	ucClient.ClientService.PASSWORD_RULE_UPDATE_MESSAGE_RECEIVED_EVENT = 'ClientServiceOnPasswordRuleUpdateMessageReceived';

    //密码到期
	ucClient.ClientService.PASSWORD_EXPIRED_MESSAGE_RECEIVED_EVENT = 'ClientServiceOnPasswordExpiredMessageReceived';

    //连接UCAS
	ucClient.ClientService.UCAS_CONNECTED_EVENT = 'ClientServiceOnUCASConnected';

    //断开UCAS
	ucClient.ClientService.UCAS_DISCONNECTED_EVENT = 'ClientServiceOnUCASDisconnected';

    //list conversation
	ucClient.ClientService.CONVERSATION_LIST_RECEIVED_EVENT = 'ClientServiceOnConversationListReceived';

	//list conversation complete
	ucClient.ClientService.CONVERSATION_LIST_COMPLETE_EVENT = 'ClientServiceOnConversationListComplete';

	//set conversation top complete
	ucClient.ClientService.CONVERSATION_TOP_SET_EVENT = 'ClientServiceOnConversationTopSet';

    //set update user notify complete
	ucClient.ClientService.UPDATE_USER_NOTIFY_SET_EVENT = 'UpdateUserNotifySet';

	//get user notify status complete
    ucClient.ClientService.USER_NOTIFY_STATUS_EVENT = 'UserNotifyStatusGet'

	//user login notify
	ucClient.ClientService.USER_LOGIN_MSG_NOTIFY = 'UserLoginMessageNotify';
	
	// OnAccountVerified
    ucClient.ClientService.ACCOUNT_VERIFied_EVENT = 'ClientServiceOnAccountVerified';

    // OnCodeVerified
    ucClient.ClientService.CODE_VERIFY_EVENT = 'ClientServiceOnCodeVerified';

	// OnCodeResent
    ucClient.ClientService.CODE_RESEND_EVENT = 'ClientServiceOnCodeResent';

    // OnPasswordReset
    ucClient.ClientService.FORGET_PASSWORD_EVENT = 'ClientServiceOnPasswordReset';

    ucClient.ClientService.UPLOAD_LOG_EVENT = 'ClientServiceOnLogUploaded';
    
    ucClient.ClientService.SESSION_EXPIRED_EVENT = 'ClientServiceOnSessionExpired';

    ucClient.ClientService.ARCHIVELOG_FINISH_EVENT = 'ClientServiceOnArchiveLogFinish';	

    ucClient.ClientService.MOUSE_IN_ICON_EVENT = 'MouseInIconEvent';

    ucClient.ClientService.MOUSE_LEAVE_ICON_EVENT = 'MouseLeaveIconEvent';

    ucClient.ClientService.MAIN_WINDOW_ACTIVE_EVENT = 'MainWndActiveEvent';

    ucClient.ClientService.SEND_FILE_TO_BEE_EVENT = 'SendFileToBeeEvent';
    // -------------------------------------------------------------------------------
	// ContactService事件
    //常用联系人加载
	ucClient.ContactService.CONTACT_LOADED_EVENT = 'ContactServiceOnContactLoaded';

    //用户状态变更
	ucClient.ContactService.USER_STATUS_RECEIVED_EVENT = 'ContactServiceOnUserStatusReceived';

	//获取用户状态回调
	ucClient.ContactService.GET_USER_STATUS_RESPONSE_EVENT = 'ContactServiceOnGetUserStatusResponse';

    //用户状态更新
	ucClient.ContactService.USER_STATUS_SET_EVENT = 'ContactServiceOnUserStatusSet';

    //获取联系人信息
	ucClient.ContactService.CONTACT_INFO_RECEIVED_EVENT = 'ContactServiceOnContactInfoReceived';

    //获取联系人基本信息
	ucClient.ContactService.BASIC_CONTACT_INFO_RECEIVED_EVENT = 'ContactServiceOnBasicContactInfoReceived';

    //常用联系人添加
	ucClient.ContactService.CONTACT_ADDED_EVENT = 'ContactServiceOnContactAdded';

    //常用联系人删除
	ucClient.ContactService.CONTACT_DELETED_EVENT = 'ContactServiceOnContactDeleted';

    //常用联系人信息更新
	ucClient.ContactService.CONTACT_UPDATED_EVENT = 'ContactServiceOnContactUpdated';
	  
    //用户信息更新       
	ucClient.ContactService.USER_INFO_UPDATED_EVENT  = 'ContactServiceOnUserInfoUpdated';
	  
    //获取用户自定义标签
	ucClient.ContactService.USER_TAGS_RECEIVED_EVENT = 'ContactServiceOnUserTagsReceived';
    	  
	//用户自定义标签更新 
	ucClient.ContactService.USER_TAGS_UPDATED_EVENT = 'ContactServiceOnUserTagsUpdated';
	  
    //后台管理员增加标签     
	ucClient.ContactService.SYSTEM_TAGS_ADDED_EVENT = 'ContactServiceOnSystemTagsAdded';
	  
    //后台管理员修改标签       
	ucClient.ContactService.SYSTEM_TAGS_UPDATED_EVENT = 'ContactServiceOnSystemTagsUpdated';
	  
    //后台管理员修改标签       
	ucClient.ContactService.SYSTEM_TAGS_DELETED_EVENT = 'ContactServiceOnSystemTagsDeleted';
	  
    //获取同事列表       
	ucClient.ContactService.COLLEAGUE_RECEIVED_EVENT = 'ContactServiceOnColleagueReceived';
	  
    //获取组织通讯录      
	ucClient.ContactService.ORG_ADDRESS_BOOK_RECEIVED_EVENT = 'ContactServiceOnOrgAddressBookReceived';
	
	// 获取部门所有成员
	ucClient.ContactService.ORG_MEMBER_RECEIVED_EVENT = 'ContactServiceOnOrgMembersReceived';
	
    //部门名称变更       
	ucClient.ContactService.DEPT_UPDATED_EVENT = 'ContactServiceOnDeptUpdated';
	  
    //员工部门调动      
	ucClient.ContactService.DEPT_TRANSFERED_EVENT = 'ContactServiceOnDeptTransfered';
	  
    //员工职位调整      
	ucClient.ContactService.POSITION_UPDATED_EVENT = 'ContactServiceOnPositionUpdated';
	  
    //员工入职      
	ucClient.ContactService.EMPLOYEE_ENTRY_EVENT = 'ContactServiceOnEmployeeEntry';
	  
    //员工离职
    ucClient.ContactService.EMPLOYEE_LEAVED_EVENT = 'ContactServiceOnEmployeeLeaved';
	  
    //员工权限变更     
	ucClient.ContactService.EMPLOYEE_RIGHTS_UPDATED_EVENT = 'ContactServiceOnEmployeeRightsUpdated';
	  
    //部门整体移动    
	ucClient.ContactService.DEPT_PARENT_CHANGED_EVENT = 'ContactServiceOnDeptParentChanged';
	  
    //员工被指定为生态企业员工       
	ucClient.ContactService.COMPANY_JOINED_EVENT = 'ContactServiceOnCompanyJoined';
	  
    //部门删除      
	ucClient.ContactService.DEPT_DELETED_EVENT = 'ContactServiceOnDeptDeleted';
	  
    //管理员确认员工入职       
	ucClient.ContactService.EMPLOYEE_ENTRY_CONFIRMED_EVENT = 'ContactServiceOnEmployeeEntryConfirmed';
	  
    //管理员确认员工离职      
	ucClient.ContactService.EMPLOYEE_LEAVE_CONFIRMED_EVENT = 'ContactServiceOnEmployeeLeaveConfirmed';
	  
    //管理员确认员工调岗(部门变更)       
	ucClient.ContactService.DEPT_TRANSFER_CONFIRMED_EVENT = 'ContactServiceOnDeptTransferConfirmed';
	
    // -------------------------------------------------------------------------------
	// IMService事件	  
    //获取聊天历史列表      
	ucClient.IMService.CHAT_HISTORY_RECEIVED_EVENT = 'IMServiceOnChatHistoryReceived';	
	  
    //消息发送结果      
	ucClient.IMService.MESSAGE_SENT_EVENT = 'IMServiceOnMessageSent';
		  
    //收到文本聊天消息       
	ucClient.IMService.TEXT_MESSAGE_RECEIVED_EVENT = 'IMServiceOnTextMessageReceived';
		  
    //收到图片消息       
	ucClient.IMService.IMAGE_RECEIVED_EVENT = 'IMServiceOnImageReceived';
			  
    //收到音频消息       
	ucClient.IMService.AUDIO_MESSAGE_RECEIVED_EVENT = 'IMServiceOnAudioMessageReceived';
		  
    //收到视频消息       
	ucClient.IMService.VIDEO_MESSAGE_RECEIVED_EVENT = 'IMServiceOnVideoMessageReceived';

	//收到视频消息       
	ucClient.IMService.REPORT_MESSAGE_RECEIVED_EVENT = 'IMServiceOnReportMessageReceived';
	
    //收到会议录制视频消息       
	ucClient.IMService.RECORD_VIDEO_MESSAGE_RECEIVED_EVENT = 'IMServiceOnRecordVideoMessageReceived';
	  
    //接收到文件消息响应       
	ucClient.IMService.FILE_RECEIVED_EVENT = 'IMServiceOnFileReceived';
	
    //接收到上传云盘文件
	ucClient.IMService.CLOUDFILE_CREATE_RECEIVED_EVENT = 'IMServiceOnCloudFileCreateMessageReceived';

    //文件删除       
	ucClient.IMService.FILE_DELETED_EVENT = 'IMServiceOnFileDeleted';

    //接收到代码消息响应       
	ucClient.IMService.CODE_RECEIVED_EVENT = 'IMServiceOnCodeReceived';
		  
    //消息已读       
	ucClient.IMService.MESSAGE_READ_EVENT = 'IMServiceOnMessageRead';
	  
    //消息已播放       
    ucClient.IMService.MESSAGE_PLAYED_EVENT = 'IMServiceOnMessagePlayed';

	//收到消息未读用户个数事件   
    ucClient.IMService.MESSAGE_UNREAD_USER_COUNT_EVENT = "IMServiceOnMessageUnreadUserCountReceived"
		  
    //音频通话邀请       
	ucClient.IMService.CALL_INVITED_EVENT = 'IMServiceOnCallInvited';	
	  
    //音频通话接受       
	ucClient.IMService.CALL_ACCEPTED_EVENT = 'IMServiceOnCallAccepted';	
	  
    //音频通话拒绝      
	ucClient.IMService.CALL_REJECTED_EVENT = 'IMServiceOnCallRejected';	
	  
    //音频通话结束      
	ucClient.IMService.CALL_STOPPED_EVENT = 'IMServiceOnCallStopped';	
	  
    //音频通话取消       
	ucClient.IMService.CALL_CANCELLED_EVENT = 'IMServiceOnCallCancelled';	
	  
    //音频通话未接通       
	ucClient.IMService.CALL_NOT_RESPONDED_EVENT = 'IMServiceOnCallNotResponded';	
	  
    //音频通话接通       
	ucClient.IMService.CALL_CONNECTED_EVENT = 'IMServiceOnCallConnected';
	

    // -------------------------------------------------------------------------------
	// GroupService事件	 
	//获取聊天历史列表	   
	ucClient.GroupService.CHAT_HISTORY_RECEIVED_EVENT = 'GroupServiceOnChatHistoryReceived';	

	//获取搜索聊天历史列表	   
	ucClient.GroupService.SEARCH_CHAT_HISTORY_RECEIVED_EVENT = 'GroupServiceOnSearchChatHistoryReceived';	
	
	//消息发送结果	   
	ucClient.GroupService.MESSAGE_SENT_EVENT = 'GroupServiceOnMessageSent';

	//接收文本聊天消息	   
	ucClient.GroupService.TEXT_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnTextMessageReceived';	
	  
	//接收图片消息	   
	ucClient.GroupService.IMAGE_RECEIVED_EVENT = 'GroupServiceOnImageReceived';
	
	//接收音频消息	   
	ucClient.GroupService.AUDIO_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnAudioMessageReceived';
		  
	//接收视频消息	   
	ucClient.GroupService.VIDEO_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnVideoMessageReceived';

	//接收报告消息	   
	ucClient.GroupService.REPORT_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnReportMessageReceived';
	
    
	//接收会议视频录制消息	   
	ucClient.GroupService.RECORD_VIDEO_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnRecordVideoMessageReceived';

    // 接收group创建的消息
    ucClient.GroupService.GROUP_CREATE_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnGroupCreateMessageReceived';

    // 接收group结束的消息
    ucClient.GroupService.GROUP_CLOSE_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnGroupCloseMessageReceived';
    
    // 接收group成员退出的消息
    ucClient.GroupService.GROUPMEMBER_DEL_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnGroupMemberDelMessageReceived';

    // 接收group成员增加的消息
    ucClient.GroupService.GROUPMEMBER_ADD_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnGroupMemberAddMessageReceived';

    // 接收group踢人的消息
    ucClient.GroupService.GROUPMEMBER_KICK_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnGroupMemberKickMessageReceived';

    // 接收group名称修改的消息
    ucClient.GroupService.GROUPNAME_UPDATE_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnGroupNameUpdateMessageReceived';

    // 接收group免打扰设置的消息
    ucClient.GroupService.GROUPDISTURB_SET_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnGroupDisturbSetMessageReceived';

    // 接收group置顶的消息
    ucClient.GroupService.GROUPTOP_MESSAGE_RECEIVED_EVENT = 'GroupServiceOnGroupTopMessageReceived';

    // 接收group头像修改的消息
    ucClient.GroupService.GROUPAVATAR_UPDATE_MESSAGE_RECEIVED = 'GroupServiceOnGroupAvatarUpdateMessageReceived';

	// 接收group会中状态变化的消息
    ucClient.GroupService.GROUPSTATUSINFO_UPDATE_MESSAGE_RECEIVED = 'GroupServiceOnGroupStatusInfoUpdateMessageReceived';

    // 接收Group 提醒设置的消息
    ucClient.GroupService.GROUPREMINDER_SET_MESSAGE_RECEIVED = 'GroupServiceOnGroupReminderSetMessageReceived';

    // 接收group文档上传/分享的消息
    ucClient.GroupService.DOCUMENTUPLOAD_MESSAGE_RECEIVED = 'GroupServiceOnDocumentUploadMessageReceived';

    // 接收group文档修改的消息
    ucClient.GroupService.CLOUDFILE_MODIFIED_MESSAGE_RECEIVED = 'GroupServiceOnCloudFileModifiedMessageReceived';
    
    // 接收group文档删除的消息
    ucClient.GroupService.DOCUMENTDELETE_MESSAGE_RECEIVED = 'GroupServiceOnDocumentDeleteMessageReceived';

    // 接收group代码上传/分享的消息
    ucClient.GroupService.CODESNIPPET_UPLOAD_MESSAGE_RECEIVED = 'GroupServiceOnCodeSnippetUploadMessageReceived';

    // 接收group评论创建的消息;
    ucClient.GroupService.COMMENTCREATE_MESSAGE_RECEIVED = 'GroupServiceOnCommentCreateMessageReceived';

	//接收消息已读消息	   
	ucClient.GroupService.MESSAGE_READ_EVENT = 'GroupServiceOnMessageRead';
	  
	//接收消息已播放消息	   
	ucClient.GroupService.MESSAGE_PLAYED_EVENT = 'GroupServiceOnMessagePlayed';	
	  
	//收到消息未读用户个数事件   
	ucClient.GroupService.MESSAGE_UNREAD_USER_COUNT_EVENT = "GroupServiceOnMessageUnreadUserCountReceived"
	   
	//收到消息未读用户列表事件  
	ucClient.GroupService.MESSAGE_UNREAD_USER_LIST_EVENT = "GroupServiceOnMessageUnreadUserListReceived"
	
    //音频通话邀请响应	   
	ucClient.GroupService.CALL_INVITED_EVENT = 'GroupServiceOnCallInvited';
		  
	//音频通话接受响应	   
	ucClient.GroupService.CALL_ACCEPTED_EVENT = 'GroupServiceOnCallAccepted';	
	  
	//音频通话拒绝响应事件	   
	ucClient.GroupService.CALL_REJECTED_EVENT = 'GroupServiceOnCallRejected';
		  
	//音频通话结束响应事件	   
	ucClient.GroupService.CALL_STOPPED_EVENT = 'GroupServiceOnCallStopped';	
	  
	//音频通话接通时长响应事件	   
	ucClient.GroupService.CALL_CONNECTED_EVENT = 'GroupServiceOnCallConnected';	

    //取消呼操作
	ucClient.GroupService.CALL_CANCELLED_EVENT = 'GroupServiceOnCallCancelled';
	  
	//创建讨论组事件	   
	ucClient.GroupService.GROUP_CREATED_EVENT = 'GroupServiceOnGroupCreated';
	  
	//退出讨论组事件	   
	ucClient.GroupService.GROUP_EXITED_EVENT = 'GroupServiceOnGroupExited';
	  
	//结束讨论组回调事件	   
	ucClient.GroupService.GROUP_ENDEDED_EVENT = 'GroupServiceOnGroupEnded';	
	  
	//添加讨论组成员事件	   
	ucClient.GroupService.GROUP_MEMBER_ADDED_EVENT = 'GroupServiceOnMemberAdded';
		  
	//移除讨论组成员事件	   
	ucClient.GroupService.GROUP_MEMBER_REMOVED_EVENT = 'GroupServiceOnMemberRemoved';
		  
	//讨论组名称更新事件	   
	ucClient.GroupService.GROUP_NAME_UPDATED_EVENT = 'GroupServiceOnGroupNameUpdated';	
	  
	//讨论组Logo更新事件   
	ucClient.GroupService.GROUP_LOGO_UPDATED_EVENT = 'GroupServiceOnGroupLogoUpdated';
	  
	//讨论组免打扰设置事件	   
	ucClient.GroupService.DISTURB_SET_EVENT = 'GroupServiceOnDisturbSet';	
	  
	//讨论组置顶显示事件	   
	ucClient.GroupService.TOP_CHAT_EVENT = 'GroupServiceOnTopChatSet';
		  
	//讨论组设置提醒事件	   
	ucClient.GroupService.REMINDER_SET = 'GroupServiceOnReminderSet';	
	  
	//获取讨论组列表事件	   
	ucClient.GroupService.GROUP_LIST_RECEIVED = 'GroupServiceOnGroupListReceived';
		  
	//获取讨论组信息事件	   
	ucClient.GroupService.GROUP_INFO_RECEIVED = 'GroupServiceOnGroupInfoReceived';
	  
	//获取讨论组文档列表事件	   
	ucClient.GroupService.DOC_LIST_RECEIVED = 'GroupServiceOnDocListReceived';
	  
	//获取讨论组文档信息事件	   
	ucClient.GroupService.DOC_INFO_RECEIVED = 'GroupServiceOnDocInfoReceived';
		  
	//上传/分享讨论组文档响应事件	   
	ucClient.GroupService.DOC_UPLOADED_RECEIVED = 'GroupServiceOnDocumentUploaded';
	  
	//删除讨论组文档响应事件	   
	ucClient.GroupService.DOC_DELETED = 'GroupServiceOnDocumentDeleted';

    //上传/分享讨论组代码响应事件
    ucClient.GroupService.CODESNIPPET_UPLOADED_RECEIVED = 'GroupServiceOnCodeSnippetUploaded';

    //获取评论列表响应事件	   
	ucClient.GroupService.COMMENT_LIST_RECEIVED = 'GroupServiceOnCommentListReceived';

     //创建评论响应事件	   
	ucClient.GroupService.COMMENT_CREATED = 'GroupServiceOnCommentCreated';
	
	//成员角色变更事件	   
	ucClient.GroupService.MEMBER_ROLE_UPDATED = 'GroupServiceOnMemberRoleUpdated';


    //@消息列表获取事件
	ucClient.GroupService.ATMESSAGE_LIST_EVENT = 'GroupServiceOnAtMessageListReceived';

    //@消息删除事件
	ucClient.GroupService.ATMESSAGE_DELETE_EVENT = 'GroupServiceOnAtMessageDeleted'

	//消息撤回事件
	ucClient.GroupService.GROUP_SERVICE_REVOCATION_MESSAGE_EVENT = 'GroupServiceOnRevocationMesssageReceived';

	//消息撤回发送结果
	ucClient.GroupService.GROUP_SERVICE_REVOCATION_MSG_EVENT = 'GroupServiceOnRevocationMessage';


    // -------------------------------------------------------------------------------
	// CalendarService事件	  
	//会议创建	   
	ucClient.CalendarService.CONFERENCE_CREATED_EVENT = 'CalendarServiceOnMeetingCreated';
	
    //远程协助
	ucClient.CalendarService.REMOTE_ASSISTANCE_CREATED_EVENT = 'CalendarServiceOnRemoteAssistanceCreated';

	//会议更新	   
	ucClient.CalendarService.CONFERENCE_UPDATED_EVENT = 'CalendarServiceOnMeetingUpdated';	
	  
	//会议取消   
	ucClient.CalendarService.CONFERENCE_CANCELLED_EVENT = 'CalendarServiceOnMeetingCanceled';
	
	//获取会议调起串 
	ucClient.CalendarService.CONFERENCE_STARTURL_RECEIVED_EVENT = 'CalendarServiceOnMeetingStartURLReceived';
	
	//通过邮件获取会议调起串
	ucClient.CalendarService.CONFERENCE_WEBSTARTURL_RECEIVED_EVENT = 'CalendarServiceOnMeetingWebStartURLReceived';
	
	//更新授权设置  
	ucClient.CalendarService.ACCREDIT_SETTING_UPDATED_EVENT = 'CalendarServiceOnAccreditSettingUpdated';
	
	//收到授权消息
	ucClient.CalendarService.ACCREDIT_MESSAGE_RECEIVED_EVENT = 'CalendarServiceOnAccreditMessageReceived'

	//收到授权取消的消息
    ucClient.CalendarService.ACCREDIT_CANCEL_MESSAGE_RECEIVED_EVENT = 'CalendarServiceOnAccreditCancelMessageReceived'

	//获取授权用户列表
	ucClient.CalendarService.ACCREDIT_USERLIST_RECEIVED_EVENT = 'CalendarServiceOnAccreditUserListReceived';
	  
	//获取会议列表
	ucClient.CalendarService.MEETING_LIST_RECEIVED_EVENT = 'CalendarServiceOnMeetingListReceived';

	//获取简明会议列表
	ucClient.CalendarService.MEETING_BRIEF_LIST_RECEIVED_EVENT = 'CalendarServiceOnBriefMeetingListReceived';

	//获取简明变更会议列表
	ucClient.CalendarService.MEETING_BRIEF_CHANGED_LIST_RECEIVED_EVENT = 'CalendarServiceOnBriefChangedMeetingListReceived';
	
	//获取会议详情
	ucClient.CalendarService.MEETING_INFO_RECEIVED_BY_ID_EVENT = 'CalendarServiceOnMeetingInfoReceivedById';
	
	//批量获取会议详情
	ucClient.CalendarService.MEETING_INFO_RECEIVED_BY_IDS_EVENT = 'CalendarServiceOnMeetingInfoReceivedByIds';
	  
	//接受会议邀请
	ucClient.CalendarService.MEETING_INVITATION_ACCEPTED_EVENT = 'CalendarServiceOnMeetingInvitationAccepted';
	
	//拒绝会议邀请 
	ucClient.CalendarService.MEETING_INVITATION_DENIED_EVENT = 'CalendarServiceOnMeetingInvitationDenied';
 
	//转发会议邀请
	ucClient.CalendarService.MEETING_FORWARDED_EVENT = 'CalendarServiceOnMeetingForwarded';
	  
	//获取用户日程列表
	ucClient.CalendarService.USER_SCHEDULE_RECEIVED_EVENT = 'CalendarServiceOnUserScheduleReceived';

	//获取用户日程列表
	ucClient.CalendarService.USER_BUSYFREE_INFO_RECEIVED_EVENT = 'CalendarServiceOnUserBusyFreeInfoReceived';
	  
	//创建会议纪要  
	ucClient.CalendarService.MINUTES_CREATED_EVENT = 'CalendarServiceOnMinutesCreated';
	  
	//更新会议纪要  
	ucClient.CalendarService.MINUTES_UPDATED_EVENT = 'CalendarServiceOnMinutesUpdated';
	  
	//获取会议纪要详情  
	ucClient.CalendarService.MINUTES_DETAILS_RECEIVED_EVENT = 'CalendarServiceOnMinutesDetailsReceived';
	  
	//根据会议ID获取会议纪要列表
	ucClient.CalendarService.MINUTES_LIST_RECEIVED_EVENT = 'CalendarServiceOnMinutesListReceived';
	
    //删除会议录制视频事件
	ucClient.CalendarService.MEETING_RECORDING_DELETED_EVENT = 'CalendarServiceOnMeetingRecordingDeleted';

    //会议文档上传事件
	ucClient.CalendarService.MEETING_FILE_UPLOADED_EVENT= 'CalendarServiceOnMeetingFileUploaded';

	//会议文档删除事件
	ucClient.CalendarService.MEETING_FILE_DELETEDED_EVENT= 'CalendarServiceOnMeetingFileDeleted';
    
	//会议提醒事件
	ucClient.CalendarService.MEETING_ALARMED_EVENT = 'CalendarServiceOnMeetingAlarmed';
	  
    //会议室创建事件
    ucClient.CalendarService.MEETINGROOM_CREATED_EVENT = 'CalendarServiceOnMeetingRoomCreated';
    
	//会议室更新事件
    ucClient.CalendarService.MEETINGROOM_UPDATED_EVENT = 'CalendarServiceOnMeetingRoomUpdated';

    //会议室创建会议事件
    ucClient.CalendarService.MEETINGROOM_MEETING_CREATED_EVENT = 'CalendarServiceOnMeetingCreatedInMeetingRoom';

    //会议室列表事件
    ucClient.CalendarService.MEETINGROOM_LIST_RECEIVED_EVENT = 'CalendarServiceOnMeetingRoomListReceived';

    //检查会议室是否可用事件
    ucClient.CalendarService.MEETINGROOM_CHECK_JOIN_EVENT = 'CalendarServiceOnCheckJoinMeetingRoom';

    //检查会议室密码是否有效
    ucClient.CalendarService.MEETINGROOM_CHECK_PASSWORD_EVENT = 'CalendarServiceOnCheckMeetingRoomPassword';

    //删除会议室事件
    ucClient.CalendarService.MEETINGROOM_DELETED_EVENT = 'CalendarServiceOnMeetingRoomDeleted';

	//删除会议室事件
    ucClient.CalendarService.MEETINGROOM_RIGHTS_CHECKED_EVENT = 'CalendarServiceOnRightsChecked';

    //消息发送结果      
	ucClient.CalendarService.MESSAGE_SENT_EVENT = 'CalendarServiceOnMessageSent';

    //云会议入会加载完成通知事件
	ucClient.CalendarService.MEETING_JOINED_EVENT = 'CalendarServiceNotifyMeetingJoined';

	//云会议准备完成通知事件
	ucClient.CalendarService.MEETING_READIED_EVENT = 'CalendarServiceNotifyMeetingReadied';

	//云会议入会加载失败通知事件
	ucClient.CalendarService.MEETING_JOIN_FAILED_EVENT = 'CalendarServiceNotifyMeetingJoinFailed';

    //云会议退出通知事件
	ucClient.CalendarService.MEETING_CLOSED_EVENT = 'CalendarServiceNotifyMeetingClosed';

    //远程协助消息发送结果
	ucClient.CalendarService.REMOTE_ASSISTANCE_INVITE_MSG_SENT_ENVNT = 'CalendarServiceOnRemoteAssistanceInviteMsgSent';
	ucClient.CalendarService.REMOTE_ASSISTANCE_ACCEPT_MSG_SENT_ENVNT = 'CalendarServiceOnRemoteAssistanceAcceptMsgSent';
	ucClient.CalendarService.REMOTE_ASSISTANCE_REJECT_MSG_SENT_ENVNT = 'CalendarServiceOnRemoteAssistanceRejectMsgSent';

    //收到远程协助消息
	ucClient.CalendarService.REMOTE_ASSISTANCE_INVITE_MSG_RECEIVED_ENVNT = 'CalendarServiceOnRemoteAssistanceInviteMsgReceived';
	ucClient.CalendarService.REMOTE_ASSISTANCE_ACCEPT_MSG_RECEIVED_ENVNT = 'CalendarServiceOnRemoteAssistanceAcceptMsgReceived';
	ucClient.CalendarService.REMOTE_ASSISTANCE_REJECT_MSG_RECEIVED_ENVNT = 'CalendarServiceOnRemoteAssistanceRejectMsgReceived';
	ucClient.CalendarService.REMOTE_ASSISTANCE_END_MSG_RECEIVED_ENVNT = 'CalendarServiceOnRemoteAssistanceEndMsgReceived';
   
	//收到文本聊天消息       
	ucClient.CalendarService.TEXT_MESSAGE_RECEIVED_EVENT = 'CalendarServiceOnTextMessageReceived';
		  
    //收到图片消息       
	ucClient.CalendarService.IMAGE_RECEIVED_EVENT = 'CalendarServiceOnImageMessageReceived';
			  
    //收到音频消息       
	ucClient.CalendarService.AUDIO_MESSAGE_RECEIVED_EVENT = 'CalendarServiceOnAudioMessageReceived';
		  
    //收到视频消息       
	ucClient.CalendarService.VIDEO_MESSAGE_RECEIVED_EVENT = 'CalendarServiceOnVideoMessageReceived';

	//收到报告消息       
	ucClient.CalendarService.REPORT_MESSAGE_RECEIVED_EVENT = 'CalendarServiceOnReportMessageReceived';

	//收到会议录像消息
	ucClient.CalendarService.RECORD_VIDEO_MESSAGE_RECEIVED_EVENT = 'CalendarServiceOnRecordVideoMessageReceived';

	//收到消息未读用户个数事件
	ucClient.CalendarService.MESSAGE_UNREAD_USER_COUNT_EVENT = 'CalendarServiceOnMessageUnreadUserCountReceived';

    // 接收会议邀请的消息
    ucClient.CalendarService.MEETING_INVITE_MESSAGE_RECEIVED_EVENT = 'CalendarServiceOnMeetingInviteMessageReceived';

    // 接收到会议更新的消息
    ucClient.CalendarService.MEETING_UPDATE_MESSAGE_RECEIVED_EVENT = 'CalendarServiceOnMeetingUpdateMessageReceived';

    // 接收到会议转发的消息
    ucClient.CalendarService.MEETING_FORWARD_MESSAGE_RECEIVED = 'CalendarServiceOnMeetingForwardMessageReceived';

    // 接收到接受会议的消息
    ucClient.CalendarService.MEETING_ACCEPT_MESSAGE_RECEIVED = 'CalendarServiceOnMeetingAcceptMessageReceived';

    // 接受到拒绝会议的消息
    ucClient.CalendarService.MEETING_DENY_MESSAGE_RECEIVED = 'CalendarServiceOnMeetingDenyMessageReceived';

    // 接受到会议取消的消息
    ucClient.CalendarService.MEETING_CANCEL_MESSAGE_RECEIVED = 'CalendarServiceOnMeetingCancelMessageReceived';

    // 接受到会议纪要创建的消息
    ucClient.CalendarService.MINUTES_CREATE_MESSAGE_RECEIVED = 'CalendarServiceOnMinutesCreateMessageReceived';

    // 接受到会议纪要更新的消息
    ucClient.CalendarService.MINUTES_UPDATE_MESSAGE_RECEIVED = 'CalendarServiceOnMinutesUpdateMessageReceived';

    ucClient.CalendarService.MEETING_PARTJOIN_MESSAGE_RECEIVED = 'CalendarServiceOnPartJoinMessageReceived';

    ucClient.CalendarService.MEETING_PARTCANCEL_MESSAGE_RECEIVED = 'CalendarServiceOnPartCancelMessageReceived';

	// 接收会议讨论组文档上传/分享的消息
    ucClient.CalendarService.DOCUMENTUPLOAD_MESSAGE_RECEIVED = 'CalendarServiceOnDocumentUploadMessageReceived';

	// 删除会议讨论组文档的消息
    ucClient.CalendarService.DOCUMENTDELETE_MESSAGE_RECEIVED = 'CalendarServiceOnDocumentDeleteMessageReceived';
    
	//会议消息撤回事件
    ucClient.CalendarService.MEETING_SERVICE_REVOCATION_MESSAGE_EVENT = 'CalendarServiceOnRevocationMesssageReceived';

	//会议消息撤回发送结果
    ucClient.CalendarService.MEETING_SERVICE_REVOCATION_MSG_EVENT = 'CalendarServiceOnRevocationMessage';

    // -------------------------------------------------------------------------------
    // FileTransfer Service事件    
    //文件上传进度的返回事件    
    ucClient.FileTransferService.FILETRANSFER_FILE_UPLOADING_EVENT = 'FileTransferServiceOnFileUploading';
      
    //文件上传完成事件     
    ucClient.FileTransferService.FILETRANSFER_FILE_UPLOADED_EVENT = 'FileTransferServiceOnFileUploaded';
      
    //文件下载进度的返回事件     
    ucClient.FileTransferService.FILETRANSFER_FILE_DOWNLOADING_EVENT = 'FileTransferServiceOnFileDownloading';
      
    //文件下载完成事件     
    ucClient.FileTransferService.FILETRANSFER_FILE_DOWNLOADED_EVENT = 'FileTransferServiceOnFileDownloaded';
    
    //截屏回调事件     
    ucClient.CatchScreenService.CATCHSCREEN_FINISHED_EVENT = 'CatchScreenServiceOnScreenCaptured';

    
	// -------------------------------------------------------------------------------
	// Pbx Service事件  
	// ------------------------------------------------------------------------------- 
	    // 账号登陆
    ucClient.PbxService.PBX_LOGINPBX_EVENT = 'PbxServiceOnLoginPbx';
	    // 通话状态变化通知
    ucClient.PbxService.PBX_CALLSTATUSCHANGED_EVENT = 'PbxServiceOnCallStatusChanged';
	    // VoIP呼叫通知
    ucClient.PbxService.PBX_MAKECALL_EVENT = 'PbxServiceOnMakeCall';
	    // 来电通知
    ucClient.PbxService.PBX_INCOMINGCALL_EVENT = 'PbxServiceOnIncomingCall';
	    // 挂断通知
    ucClient.PbxService.PBX_HANGUPCALL_EVENT = 'PbxServiceOnHangupCall';
    
	    // 获取通话记录列表
    ucClient.PbxService.PBX_GETCALLRECORDLST_EVENT = 'PbxServiceOnGetCallRecordLst';
	    // 获取通话详情列表
    ucClient.PbxService.PBX_GETCALLDETAILSLST_EVENT = 'PbxServiceOnGetCallDetailsLst';
	    // 通话记录匹配搜索
    ucClient.PbxService.PBX_SEARCHCALLRECORD_EVENT = 'PbxServiceOnSearchCallRecord';

	    // PJSIP状态变化
    ucClient.PbxService.PBX_PJSIPSERVICESTATECHANGED_EVENT = 'PbxServiceOnPJSIPServiceStateChanged';
	// -------------------------------------------------------------------------------

	// -------------------------------------------------------------------------------
	// Audio Service事件  
	// ------------------------------------------------------------------------------- 

	//创建呼回调事件
    ucClient.AudioService.CALL_CREATED_EVENT                  = 'AudioServiceOnCallCreated';

    //加入呼回调事件
    ucClient.AudioService.CALL_JOINED_EVENT                   = 'AudioServiceOnCallJoined';

    //离开呼回调事件
    ucClient.AudioService.CALL_LEAVED_EVENT                   = 'AudioServiceOnCallLeaved';
     
    //加入会议回调事件
    ucClient.AudioService.CONFERENCE_JOINED_EVENT             = 'AudioServiceOnConferenceJoined'

	//开启音频成功回调事件
    ucClient.AudioService.AUDIO_STARTED                       = 'AudioServiceOnAudioStarted'
    
	//退出加入会议回调事件
    ucClient.AudioService.CONFERENCE_LEFTED_EVENT            = 'AudioServiceOnConferenceLefted'
     
    //会议结束回调事件
    ucClient.AudioService.CONFERENCE_ENDED_EVENT		        = 'AudioServiceOnConferenceEnded'
     
    //呼会议断网回调事件   
    ucClient.AudioService.CONFERENCE_DISCONNECTED_EVENT	    = 'AudioServiceOnConferenceDisconnected'
     
    //重连会议回调事件
    ucClient.AudioService.CONFERENCE_RECONNECTED_EVENT		= 'AudioServiceOnConfReconnected'

    //用户入会回调事件
    ucClient.AudioService.USER_ADDED_EVENT					= 'AudioServiceOnUserAdded'

     //用户退出回调事件   
    ucClient.AudioService.USER_REMOVED_EVENT				    = 'AudioServiceOnUserRemoved'
     
    //用户属性更新回调事件
    ucClient.AudioService.USER_PROPERTY_CHANGED_EVENT		= 'AudioServiceOnUserPropertyChanged'

    //拨打电话失败回调事件
    ucClient.AudioService.CALL_PHONE_FAILED_EVENT			= 'AudioServiceOnCallPhoneFailed'

    //VOIP网络质量变更
    ucClient.AudioService.VOIP_NETWORK_QUALITY_CHANGED_EVENT = 'AudioServiceOnVoipNetworkQualityChanged'

    //麦克风状态变化
    ucClient.AudioService.MICROPHONE_STATUS_CHANGED_EVENT = 'AudioServiceOnMicrophoneStatusChanged'

    //扬声器状态变化
    ucClient.AudioService.SPEAKER_STATUS_CHANGED_EVENT = 'AudioServiceOnSpeakerStatusChanged'

    //通话状态变化
    ucClient.AudioService.SPEAKING_STATUS_CHANGED_EVENT = 'AudioServiceOnSpeakingStatusChanged'

	//邀请其他人加入当前呼
    ucClient.AudioService.USERS_ADDED_TO_CALL_EVENT = 'AudioServiceOnUsersAddedToCall'

    //获取用户呼状态
    ucClient.AudioService.USER_CALL_STATUS_GOT_EVENT = 'AudioServiceOnUserCallStatusGot'

    //获取呼历史电话号码
    ucClient.AudioService.HISTORY_CALL_PHONENUM_GOT_EVENT = 'AudioServiceOnHistoryPhoneNumGot'

    //扬声器音频播放结束
    ucClient.AudioService.SPEAKER_PLAY_FINISHED_EVENT = 'AudioServiceOnSpeakerPlayFinished'

    //获取扬声器音量大小
    ucClient.AudioService.SPEAKER_LEVEL_GOT_EVENT = 'AudioServiceOnSpeakerLevelGot'

    //获取麦克风音量大小
    ucClient.AudioService.MIKE_LEVEL_GOT_EVENT = 'AudioServiceOnMikeLevelGot'

    //呼邀请事件
    ucClient.AudioService.INVITE_CALL_SEND_EVENT = 'AudioServiceOnInviteCallSend'

    //音频准备好事件
    ucClient.AudioService.AUDIO_READY_EVENT = 'AudioServiceOnAudioServiceReady'

    //音频设备状态变更
    ucClient.AudioService.AUDIO_DEVICE_STATUS_CHANGED_EVENT = 'AudioDeviceStatusChanged'

    //获取上次呼信息数据
    ucClient.AudioService.LAST_AUDIO_INVITE_GOT_EVENT = 'AudioServiceOnLastAudioInviteGot'

	//桌面共享error通知事件
    ucClient.SharingService.SHARING_ERROR_EVENT = 'SharingServiceOnErrorHandle'

    //桌面共享事件
    ucClient.SharingService.DESKTOP_SHARED_EVENT = 'SharingServiceOnDesktopShared'

	//桌面共享停止事件
    ucClient.SharingService.DESKTOP_SHARE_STOPED_EVENT = 'SharingServiceOnDesktopShareStoped'

    //观看桌面共享开始事件
    ucClient.SharingService.DESKTOP_VIEWER_STARTED_EVENT = 'SharingServiceOnDesktopViewerStarted'

	//观看桌面共享结束事件
    ucClient.SharingService.DESKTOP_VIEWER_STOPPED_EVENT = 'SharingServiceOnDesktopViewerStopped'


    // -------------------------------------------------------------------------------
    // Search Service事件
    ucClient.SearchService.SERVER_SEARCH_RESULT_RECEIVED = 'SearchServiceOnServerSearched';
    ucClient.SearchService.LOCAL_SEARCH_RESULT_RECEIVED  = 'SearchServiceOnLocalSearched';
	
	// -------------------------------------------------------------------------------
	// Network Monitor事件
    ucClient.Net.NETWORK_CHANGED = 'NetworkChanged';

	// -------------------------------------------------------------------------------
	// System Service事件
    ucClient.System.PLAYINGSOUND_NOTIFICATION = 'PlayingSoundNotification';


	// -------------------------------------------------------------------------------
	// OpenApiService事件
    
    //获取应用列表      
    ucClient.OpenApiService.APP_LIST_RECEIVED_EVENT = 'OpenApiServiceOnAppListReceived';
	
	//获取应用信息   
    ucClient.OpenApiService.APP_INFO_RECEIVED_EVENT = 'OpenApiServiceOnAppInfoReceived';

    //获取聊天历史列表      
    ucClient.OpenApiService.CHAT_HISTORY_RECEIVED_EVENT = 'OpenApiServiceOnChatHistoryReceived';	

	//获取OA消息状态列表        
    ucClient.OpenApiService.OAMESSGAE_STATUS_RECEIVED_EVENT = 'OpenApiServiceOnOAMessageStatusReceived';

	//获取应用消息ack状态列表 
    ucClient.OpenApiService.MESSGAE_ACK_STATUS_RECEIVED_EVENT = 'OpenApiServiceOnMessageAckStatusReceived';

	//消息发送结果      
    ucClient.OpenApiService.MESSAGE_SENT_EVENT = 'OpenApiServiceOnMessageSent';
	
	//收到消息PushAck响应    
    ucClient.OpenApiService.PUSH_MESSAGE_ACK_EVENT = 'OpenApiServiceOnPushMessageACK';
	
	//接收到OA消息状态变化响应  
    ucClient.OpenApiService.OAMESSGAE_STATUS_CHANGED_RECEIVED_EVENT = 'OpenApiServiceOnOAMessageStatusChangedReceived';

	//接收到定义消息响应  
    ucClient.OpenApiService.OAMESSGAE_RECEIVED_EVENT = 'OpenApiServiceOnOAMessageReceived';

	//接收到OA消息响应  
    ucClient.OpenApiService.CUSTOMIZEDMESSGAE_RECEIVED_EVENT = 'OpenApiServiceOnCustomizedReceived';

    //接收到修改OA消息状态响应  
    ucClient.OpenApiService.OAMESSGAE_STATUS_UPDATED_EVENT = 'OpenApiServiceOnOAMessageStatusUpdated';
	
	//收到文本聊天消息响应       
    ucClient.OpenApiService.TEXT_MESSAGE_RECEIVED_EVENT = 'OpenApiServiceOnTextMessageReceived';
		  
    //收到图片消息响应      
    ucClient.OpenApiService.IMAGE_RECEIVED_EVENT = 'OpenApiServiceOnImageReceived';
			 	  
	//接收到文件消息响应       
    ucClient.OpenApiService.FILE_RECEIVED_EVENT = 'OpenApiServiceOnFileReceived';

	//接收消息已读消息	   
    ucClient.OpenApiService.MESSAGE_READ_EVENT = 'OpenApiServiceOnMessageRead';
	
	//接收应用状态变化响应事件;	   
    ucClient.OpenApiService.MESSAGE_APP_STATUS_CHANGED_RECEIVED_EVENT = 'OpenApiServiceOnMessageAppStatusChangedReceived';

	//接收应用事件变化响应事件;  
    ucClient.OpenApiService.MESSAGE_APP_EVENT_CHANGED_RECEIVED_EVENT = 'OpenApiServiceOnMessageAppEventChangedReceived';
                                            
	// microblog service事件
    // 
    ucClient.MicroblogService.NEWMSGCOUNT_GET_EVENT = 'MicroblogServiceOnNewMsgCountGet'

    ucClient.MicroblogService.ADD_ATTENTION_EVENT = 'MicroblogServiceOnAttentionAdd'

    ucClient.MicroblogService.CANCEL_ATTENTION_EVENT = 'MicroblogServiceOnAttentionCancelled'

    ucClient.MicroblogService.SHARE_RECORDING_EVENT = 'MicroblogServiceOnMeetingRecordingShared'
	// -------------------------------------------------------------------------------
	// UpdateService事件
    ucClient.UpdateService.CHECK_UPDATES_EVENT = 'UpdateServiceOnUpdatesChecked';
    ucClient.UpdateService.CHECK_PROGRESS_EVENT = 'UpdateServiceOnProgressChecked';

	// -------------------------------------------------------------------------------
	    // ExternalService事件
    ucClient.ExternalService.CHAT_HISTORY_RECEIVED_EVENT = 'ExternalServiceOnChatHistoryReceived';

    // -------------------------------------------------------------------------------
    // Gkclient事件
    ucClient.Gkclient.LOGIN_EVENT = 'GkClientOnLogin';

	//bbs service事件
    ucClient.BBSService.BBS_SERVICE_MAIN_EVENT = 'BBSServiceOnResponse';


    // -------------------------------------------------------------------------------
    // ClientService接口	
    // -------------------------------------------------------------------------------
	/**
	 * 用户登录
	 * @param account 账号
	 * @param password 密码
	 */
	ucClient.ClientService.login = function(account, password,accountType) {
	    return CallNativeMethodAync('ClientService', 'Login', account, password,accountType);
	};
	
	ucClient.ClientService.relogin = function() {
	    return CallNativeMethodAync('ClientService', 'Relogin');
	};

	/**
	 * 用户注销
	 */
	ucClient.ClientService.logout = function() {
		return CallNativeMethodAync('ClientService', 'Logout');
	};
	
	/**
	 * 用户重设密码
	 * @param newPass 新密码
	 * @param confirmPass 新密码确认
	 */
	ucClient.ClientService.modifyPassword = function(newPass, confirmPass) {
		return CallNativeMethodAync('ClientService', 'ModifyPassword', newPass, confirmPass);
	};
	
    /**
	 * 删除历史账户
	 * @param account 用户账户
	 */
    ucClient.ClientService.deleteHistoryAccount = function(account) {
		return CallNativeMethodAync('ClientService', 'DeleteHistoryAccount', account);
	};

    /**
	 * 设置自动登录
	 * @param account 用户账户
     * @param autoLogin 是否自动登录
	 */
    ucClient.ClientService.setAutoLogin = function(account, autoLogin) {
		return CallNativeMethodAync('ClientService', 'SetAutoLogin', account, autoLogin);
	};

    /**
	 * 设置自动登录记住密码
	 * @param account 用户账户
     * @param autoLogin 是否记住密码
	 */
    ucClient.ClientService.rememberPassword = function(account, remember) {
		return CallNativeMethodAync('ClientService', 'RememberPassword', account, remember);
	};

	/**
	 * 连接ucas
	 */
	ucClient.ClientService.connectUCAS = function() {
		return CallNativeMethodAync('ClientService', 'ConnectUCAS');
	};
	
	/**
	 * 断开UCAS连接
	 */
	ucClient.ClientService.disconnectUCAS = function() {
		return CallNativeMethodAync('ClientService', 'DisconnectUCAS');
	};

	/**
     * 连接ucc
     */
	ucClient.ClientService.connectUCC = function() {
	    return CallNativeMethodAync('ClientService', 'ConnectUCC');
	};
	
    /**
     * 断开UCC连接
     */
	ucClient.ClientService.disconnectUCC = function() {
	    return CallNativeMethodAync('ClientService', 'DisconnectUCC');
	};
	
	/**
	 * 获取本地的conversations
	 * @param number
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-conversations
	 */
	ucClient.ClientService.getLocalConversations = function(number) {
		return CallNativeMethodAync('ClientService', 'GetLocalConversations', number);
	};
	
	/**
	 * list conversation
	 * @param number conversation数量
	 */
	ucClient.ClientService.getNewConversationList = function(number) {
		return CallNativeMethodAync('ClientService', 'GetNewConversationList', number);
	};
	
	/**
	 * 获取<endTime的历史conversation列表
	 * @param endTime 请求的开始时间
	 * @param number 数量
	 */
	ucClient.ClientService.getOldConversationList = function(endTime, number) {
		return CallNativeMethodAync('ClientService', 'GetOldConversationList', endTime, number);
	};

	/**
     * 获取conversation信息
     * @param type  会话类型
     * @param relateId 
     */
	ucClient.ClientService.getConversationInfoSync = function(type, relateId) {
	    var request = ucClient.xmlHttpRequest('ClientService', 'GetConversationInfoSync', type, relateId);
	    if (request.status == 200) {
	        return JSON.parse(request.responseText);
	    }
	    else{
	        return undefined;
	    }
	};
	
   /**
    * 设置/取消会话置顶
   	* @param type 会话类型;
	* @param relateId 操作置顶状态的会话ID(userId, groupId,orgId);
	* @param state 操作的置顶状态(1:置顶/0:取消置顶);
    */
	ucClient.ClientService.setConversationTop = function(type, relateId, state, callback) {
	    return CallNativeMethodAync('ClientService', 'SetConversationTop', type, relateId, state, callback);
	};
	

	/**
	 * 往uccserver发送heartbeat包
	 */
	ucClient.ClientService.sendHeartBeat = function() {
		return CallNativeMethodAync('ClientService', 'SendHeartBeat');
	};
	
	ucClient.ClientService.getDocumentDataDirSync = function(type) {
	    var request = ucClient.xmlHttpRequest('ClientService', 'GetDocumentDataDirSync', type);
		if (request.status == 200) {
			return request.responseText;
		}
		else{
			return undefined;
		}
	};

    ucClient.ClientService.loadHistoryAccounts = function() {
        var request = ucClient.xmlHttpRequest('ClientService', 'LoadHistoryAccounts');
		if (request.status == 200) {
            return JSON.parse(request.responseText);
		}
		else {
			return undefined;
		}
	};

    ucClient.ClientService.getCurUCID = function() {
        var request = ucClient.xmlHttpRequest('ClientService', 'GetCurUCID');
		if (request.status == 200) {
            return JSON.parse(request.responseText);
		}
		else{
			return undefined;
		}
    }
    // 获取验证码
    ucClient.ClientService.getCaptchaURL = function(hashStr, callback) {
        return CallNativeMethodAync('ClientService', 'GetCaptchaURL', hashStr, callback);
    };

    /**
     * 用户账号验证
     * @param verifyType 验证方式: 0(手机验证码)/1(邮件验证码)
	 * @param account 账号
	 * @param captcha 验证码)
	 * @param hashStr hash值 
     */
    ucClient.ClientService.verifyAccount = function(verifyType, account, captcha, hashStr) {
        return CallNativeMethodAync('ClientService', 'VerifyAccount', verifyType, account, captcha, hashStr);
    };

    /**
     * 验证码验证
     * @param verifyType 验证方式: 0(手机验证码)/1(邮件验证码)
	 * @param account		账号
	 * @param mobileCode	短信验证码
	 * @param resend 1:重发 0:不重发只验证
     */
    ucClient.ClientService.verifyCode = function(verifyType, account, mobileCode, resend) {
        return CallNativeMethodAync('ClientService', 'VerifyCode', verifyType, account, mobileCode, resend);
    };

    /**
     * 重新设置密码
     */
    ucClient.ClientService.resetPassword = function(account, newPwd, mobileCode) {
        return CallNativeMethodAync('ClientService', 'ResetPassword', account, newPwd, mobileCode);
    };

    
    ucClient.ClientService.updateValueByKey = function(app,name,value){
        return CallNativeMethodAync('ClientService','UpdateValueByKey',app,name,value)
    };

    ucClient.ClientService.updateValueByKeySync = function(app,name,value){
        var request = ucClient.xmlHttpRequest('ClientService','UpdateValueBykeySync',app,name,value)
        if(request.status == 200){
            return;
        }
        else{
            return undefined;
        }
    };

    ucClient.ClientService.getValueByKey = function(app,name,callback){
        return CallNativeMethodAync('ClientService','GetValueByKey',app,name, callback)
    };
    
    ucClient.ClientService.getValueByKeySync = function(app,name){
        var request = ucClient.xmlHttpRequest('ClientService','GetValueByKeySync',app,name)
        if(request.status == 200){
            return request.responseText;
        }
        else{
            return undefined;
        }
    };
    
	/**
    * 设置多设备同时登录通知消息
    * @param isNotity  1:允许通知提示，0：不允许通知提示
    */
    ucClient.ClientService.setUpdateUserNotify = function(isNotify){
        return CallNativeMethodAync('ClientService', 'SetUpdateUserNotify', isNotify);
    }

	/**
    * 获取多设备同时登录通知消息设置状态
    */
    ucClient.ClientService.getUserNotifyStatus = function(){
        return CallNativeMethodAync('ClientService', 'GetUserNotifyStatus');
    }

	/**
     * 根据app&Name获取config值
     */
    ucClient.ClientService.getConfigInfo = function(app, name, callback) {
        return CallNativeMethodAync('ClientService', 'GetConfigInfo', app, name, callback);
    };

	/**
     * 根据app获取config的值
     */
    ucClient.ClientService.getConfigInfos = function(app, callback) {
        return CallNativeMethodAync('ClientService', 'GetConfigInfos', app, callback);
    };

    ucClient.ClientService.updateConfigInfo = function(app, name, value, callback) {
        return CallNativeMethodAync('ClientService', 'UpdateConfigInfo', app, name, value, callback);
    };

    ucClient.ClientService.updateConfigInfos = function(app, values, callback) {
        return CallNativeMethodAync('ClientService', 'UpdateConfigInfos', app, values, callback);
    };

    ucClient.ClientService.setProxyAuthInfo = function(authType, authAccount, authPassword, callback) {
        return CallNativeMethodAync('ClientService', 'SetProxyAuthInfo', authType, authAccount,authPassword, callback);
    };

    ucClient.ClientService.uploadLog = function(logURL, desc, version) {
        return CallNativeMethodAync('ClientService', 'UploadLog', logURL, desc, version);
    };

    ucClient.ClientService.getPwdExpiredDays = function(callback) {
        return CallNativeMethodAync("ClientService", 'GetPwdExpiredDays', callback);
    };

    ucClient.ClientService.getSessionId = function(callback) {
        return CallNativeMethodAync("ClientService", 'GetSessionId', callback);
    };

    ucClient.ClientService.archiveLogFile = function() {
        return CallNativeMethodAync("ClientService", "ArchiveLogFile");
    };

    ucClient.ClientService.getTrayPos = function() {
        var request = ucClient.xmlHttpRequest('Window', 'GetTrayPos');
        if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else {
            return undefined;
        }
    };

    ucClient.ClientService.getWindowStatue = function() {
        var request = ucClient.xmlHttpRequest('Window', 'GetWindowStatue');
        if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else {
            return undefined;
        }
    };

    ucClient.ClientService.isWindowOpen = function(name) {
        var request = ucClient.xmlHttpRequest('Window', 'IsWindowOpen', name);
        if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else {
            return undefined;
        }
    };

    ucClient.ClientService.getWindowPos = function() {
        var request = ucClient.xmlHttpRequest('Window', 'GetWindowPos');
        if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else {
            return undefined;
        }
    };

	/**
     * 获取密码规则
     */
    ucClient.ClientService.getPasswordRule = function(callback){
        return CallNativeMethodAync("ClientService", 'GetPasswordRule', callback);
    }

    ucClient.ClientService.getUccServerDomain = function(callback){
        return CallNativeMethodAync("ClientService", 'GetUccServerDomain', callback);
    }

    ucClient.ClientService.deleteSession = function(type,relateId){
        return CallNativeMethodAync("ClientService", 'DeleteSession',type,relateId);
    }

    // -------------------------------------------------------------------------------	
    // Gkclient接口
    // -------------------------------------------------------------------------------

    ucClient.Gkclient.getGkClientConnection = function(callback){
        return CallNativeMethodAync("GkClient", 'GetGkClientConnection', callback);
    }
    
    ucClient.Gkclient.relogin = function(){
        return CallNativeMethodAync("GkClient", 'Relogin');
    }
    
    // -------------------------------------------------------------------------------	
    // ContactService接口
    // -------------------------------------------------------------------------------
	/**
	 * 更新本地常用联系人信息
	 */
	ucClient.ContactService.getUpdatedContacts = function() {
		return CallNativeMethodAync('ContactService', 'GetUpdatedContacts');
	};
	
	/**
	 * 获取联系人信息
	 */
	ucClient.ContactService.getContactInfo = function(userId, callback) {
		return CallNativeMethodAync('ContactService', 'GetContactInfo', userId, callback);
	};

	ucClient.ContactService.getContactInfoSync = function(userId) {
		if (typeof(userId) == 'number') {
			var request = ucClient.xmlHttpRequest('ContactService', 'GetContactInfoSync', userId);
			if (request.status == 200) {
				return JSON.parse(request.responseText);
			}
			else{
				return undefined;
			}
		}
		else if (Array.isArray(userId)) {	
			return CallNativeMethodAync('ContactService', 'GetContactInfo', userId);
		}
	};
	

    /*	
	ucClient.ContactService.getContactInfoSync = function(userIds) {
		return CallNativeMethodAync('ContactService', 'GetContactInfo', userIds);
	};
	*/
	/**
	 * 获取联系人基本信息
	 * @param userIds 联系人id数组
	 */
	ucClient.ContactService.getBasicContactInfo = function(userIds) {
		return CallNativeMethodAync('ContactService', 'GetBasicContactInfo', userIds);
	};
	
	ucClient.ContactService.getBasicContactInfoSync = function(userIds, callback) {
		if (typeof(userIds) == 'number') {
			var request = ucClient.xmlHttpRequest('ContactService', 'GetBasicContactInfoSync', userIds);
			if (request.status == 200) {
				return JSON.parse(request.responseText);
			}
			else{
				return undefined;
			}
		}
		else if (Array.isArray(userIds)) {	
			return CallNativeMethodAync('ContactService', 'GetBasicContactInfo', userIds);
		}
	};

	/**
	 * 获取本地常用联系人信息列表
	 * @param position 取列表的起始位置
	 * @param len 长度
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-常用联系人列表
	 */
	ucClient.ContactService.getContactList = function(position, len, callback) {
		return CallNativeMethodAync('ContactService', 'GetContactList', position, len, callback);
	};
	
	/**
	 * 更新联系人本地头像地址
	 * @param userId 用户id
	 * @param remoteAvatarUrl 头像服务器url
	 * @param localAvatarPath 头像本地路径
	 */
	ucClient.ContactService.updateContactAvatar = function(userId, remoteAvatarUrl, localAvatarPath) {
		return CallNativeMethodAync('ContactService', 'UpdateContactAvatar', 
			userId, remoteAvatarUrl, localAvatarPath);
	};
	
	/**
	 * 添加常用联系人
	 * @param userId 用户id
	 */
	ucClient.ContactService.addCommonContact = function(userId) {
		return CallNativeMethodAync('ContactService', 'AddCommonContact', userId);
	};
	
	/**
	 * 删除常用联系人
	 * @param userId 用户id
	 */
	ucClient.ContactService.deleteCommonContact = function(userId) {
		return CallNativeMethodAync('ContactService', 'DeleteCommonContact', userId);
	};
	
	/**
	 * 本地搜索联系人
	 * @param type 联系人类型。0-常用联系人，1-当前登陆用户，3-其他
	 * @param keyword 关键字
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-常用联系人列表
	 */
	ucClient.ContactService.searchLocalContact = function(type, keyword, callback) {
		return CallNativeMethodAync('ContactService', 'SearchLocalContact', type, keyword, callback);
	};
	
	/**
	 * 获取当前用户信息
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-用户信息
	 */
	ucClient.ContactService.getCurrentUserInfo = function(callback) {
		return CallNativeMethodAync('ContactService', 'GetCurrentUserInfo', callback);
	};
	ucClient.ContactService.getCurrentUserInfoSync = function() {
		var request = ucClient.xmlHttpRequest('ContactService', 'GetCurrentUserInfoSync');
		if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
	};
	
    /**
    * 获取当前用户全部信息，包含tag信息
    * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-用户信息
    */
	ucClient.ContactService.getCurrentUserFullInfo = function() {
	    var request = ucClient.xmlHttpRequest('ContactService', 'GetCurrentUserFullInfoSync');
	    if (request.status == 200) {
	        return JSON.parse(request.responseText);
	    }
	    else{
	        return undefined;
	    }
	};

	/**
	 * 修改当前用户信息
	 * @param updateField 修改的字段信息
	 */
	ucClient.ContactService.updateUserInfo = function(updateField) {
		return CallNativeMethodAync('ContactService', 'UpdateUserInfo', updateField);
	};

	/**
	 * 修改当前用户的标签
	 * @param updateTags 需修改的标签信息
	 */
	ucClient.ContactService.updateUserTags = function(updateTags) {
		return CallNativeMethodAync('ContactService', 'UpdateUserTags', updateTags);
	};
    
	/**
	 * 获取同事列表
	 * @param userId 用户id
	 */
    ucClient.ContactService.getColleagues = function(userId) {
		return CallNativeMethodAync('ContactService', 'GetColleagues', userId);
	};
	
	/**
	 * 获取组织通讯录
	 * @param orgId 部门id
	 */
    ucClient.ContactService.getOrgAddressBook = function(orgId, callback) {
        return CallNativeMethodAync('ContactService', 'GetOrgAddressBook', orgId, callback);
	};
	
	ucClient.ContactService.getOrgMember = function(orgId, limit, copyback) {
	    return CallNativeMethodAync('ContactService', 'GetOrgMember', orgId, limit, copyback);
	};

	/**
     * 设置用户状态
     * @param userId  用户ID
     * @param status  用户状态
     * @param show    状态提示语
     */
	ucClient.ContactService.setUserStatus = function(userId, status, show) {
	    return CallNativeMethodAync('ContactService', 'SetUserStatus', userId, status, show);
	};

   /**
    * 订阅用户状态
    * @param userIds 用户ID列表
    */
	ucClient.ContactService.subscribeUserStatus = function(userIds) {
	    return CallNativeMethodAync('ContactService', 'SubscribeUserStatus', userIds);
	};

	/**
     * 获取用户状态 （单次）
     * @param userIds 用户ID列表
     * @param copyBack 回调中返回
     */
	ucClient.ContactService.getUserStatus = function(userIds,copyBack, callback){
	    return CallNativeMethodAync('ContactService','GetUserStatus',userIds,copyBack, callback)
	}


   /**
    * 取消订阅用户状态
    * @param userIds 用户ID列表
    */
	ucClient.ContactService.unsubscribeUserStatus = function(userIds) {
	    return CallNativeMethodAync('ContactService', 'UnsubscribeUserStatus', userIds);
	};

   /**
    * 取消订阅全部用户状态
    */
	ucClient.ContactService.unsubscribeAllUserStatus = function() {
	    return CallNativeMethodAync('ContactService', 'UnsubscribeAllUserStatus');
	};

    // -------------------------------------------------------------------------------
	// IMService接口
	// -------------------------------------------------------------------------------
	/**
	 * 获取历史聊天内容接口
     * @param channel 消息来源
	 * @param userId 请求用户ID
	 * @param count 请求聊天内容数量，默认20，不能超过200
	 * @param startTime 请求聊天内容的开始时间
	 * @param endTime 请求聊天内容的结束时间
	 * @return callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.IMService.getChatHistory = function(channel, userId, count, startTime, endTime, callback) {
	    return CallNativeMethodAync('IMService', 'GetChatHistory', channel, userId, count, startTime, endTime, callback);
	};

    /**
	 * 重发消息;
	 * @param msgId 消息id
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.resendMessage = function(msgId, callback) {
		return CallNativeMethodAync('IMService', 'ResendMessage', msgId, callback);
	};

     /**
	 * 转发消息;
     * @param jid 用户jid
	 * @param msgId 消息id
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.forwardMessage = function(jid, msgId, callback) {
		return CallNativeMethodAync('IMService', 'ForwardMessage', jid, msgId, callback);
	};
	
	/**
	 * 发送文本聊天消息
	 * @param jid 用户jid
	 * @param textType 文本类型：纯文本-1，MIME格式-2
     * @param textFormat 文本格式;
	 * @param textData 文本内容
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendTextMessage = function(jid, textType, textFormat, textData, callback) {
		return CallNativeMethodAync('IMService', 'SendTextMessage', jid, textType, textFormat, textData, callback);
	};
	

	/**
	 * 发送图片聊天消息接口
	 * @param jid 用户jid
	 * @param imageName 图片名称
	 * @param imageSize 图片大小
	 * @param imageURL 图片下载路径
	 * @param imagePath	本地图片路径;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendImageMessage = function(jid, imageName, imageSize, imageURL, imagePath, callback) {
	    return CallNativeMethodAync('IMService', 'SendImageMessage', jid, imageName, imageSize, imageURL, imagePath, callback);
	};
	
	/**
	 * 发送音频聊天消息接口
	 * @param jid 用户jid;
	 * @param audioName 音频名称
	 * @param audioSize 音频大小
	 * @param audioURL 音频下载路径
	 * @param audioDuration 音频时长
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	*/
	ucClient.IMService.sendAudioMessage = function(jid, audioName, audioSize, audioURL, audioDuration, callback) {
		return CallNativeMethodAync('IMService', 'SendAudioMessage', jid, audioName, audioSize, audioURL, audioDuration, callback);
	};
	
	/**
	 * 发送视频聊天消息接口
	 * @param jid 用户jid;
	 * @param videoName 视频名称;
	 * @param videoSize 视频大小;
	 * @param videoURL 视频下载路径;
	 * @param videoPath 本地视频路径;
	 * @param videoDuration 视频时长;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendVideoMessage = function(jid, videoName, videoSize, videoURL, videoPath, videoDuration, callback) {
	    return CallNativeMethodAync('IMService', 'SendVideoMessage', jid, videoName, videoSize, videoURL, videoPath, videoDuration, callback);
	};

    /**
	 * 发送报告聊天消息接口
	 * @param jid                用户jid;
	 * @param reportTitle		报告标题;
	 * @param reportUrl          报告URL;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendReportMessage = function(jid, reportTitle, reportUrl, callback) {
	    return CallNativeMethodAync('IMService', 'SendReportMessage', jid, reportTitle, reportUrl, callback);
	};
	
	    /**
	 * 发送会议录制视频聊天消息接口
	 * @param jid 用户jid;
	 * @param videoName 视频名称;
	 * @param videoSize 视频大小;
	 * @param videoURL 视频下载路径;
	 * @param videoThumb 视频缩图数据;
     * @param videoThumbURL 视频缩图URL;
	 * @param videoDuration 视频时长;
     * @param operatorId    录制者ID;
     * @param operatorName  录制者名称;
     * @param operatorAvatar 录制者图像URL;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendRecordVideoMessage = function(jid, videoName, videoSize, videoURL, videoThumb, videoThumbURL, videoDuration, operatorId, operatorName, operatorAvatar, callback) {
	    return CallNativeMethodAync('IMService', 'SendRecordVideoMessage', jid, videoName, videoSize, videoURL, videoThumb, videoThumbURL, videoDuration, operatorId, operatorName, operatorAvatar, callback);
	};

	/**
	 * 发送/分享文件消息接口
	 * @param jid                用户jid;
     * @param operation          0: create; 1: forward
	 * @param fileName           文件名称;
	 * @param fileSize           文件大小;
	 * @param fileURL            文件下载路径;
	 * @param fileId             文档ID;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendFileMessage = function(jid, operation, fileName, fileSize, fileURL, fileId, callback) {
		return CallNativeMethodAync('IMService', 'SendFileMessage', jid, operation, fileName, fileSize, fileURL, fileId, callback);
	};
    
	/**
    	 * 发送云盘文件消息接口;
    	 * @param userUid            接收者ID;
    	 * @param fileName			文件/文件夹名称
   	 * @param fileSize			文件大小（单位：字节）
   	 * @param cloudType			云盘类型（1-够快云库, 2-安贞母婴中心）
   	 * @param detailContent      内容
    	 * @param createTime         operateTime（uccserver操作时间,单位：秒）
 	 * @param fileId				fileId
  	 * @param [out]msgId         消息ID;
  	 * @return                   错误码;
    	 */
	ucClient.IMService.sendCloudFileMessage = function(jid, fileName, fileSize, cloudType, detailContent, callback) {
	    return CallNativeMethodAync('IMService', 'SendCloudFileMessage', jid, fileName, fileSize.toString(), cloudType, detailContent, callback);
	};

	/**
	 * 删除文件消息接口;
	 * @param jid             用户jid;
	 * @param fileName           文件名称;
	 * @param fileId             文档ID;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.deleteFileMessage = function(jid, fileName, fileId, callback) {
		return CallNativeMethodAync('IMService', 'DeleteFileMessage', jid, fileName, fileId, callback);
	};

    /**
	 * 发送/转发代码消息接口
	 * @param jid                用户jid;
     * @param operation          0: create; 1: forward
	 * @param codeId             代码ID;
	 * @param codeTitle          代码标题;
	 * @param langType           语言类型;
	 * @param codeHead           代码内容缩影;
	 * @param codeDesc           代码描述;
	 * @param codeSize           代码大小;
	 * @param codeUrl            代码下载路径;
	 * @param codeLine           代码行数;
	 * @param createTime         代码创建时间;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
    ucClient.IMService.sendCodeMessage = function(jid, operation, codeId, codeTitle, langType, codeHead, codeDesc, codeSize, codeURL, codeLine, createTime, callback) {
		return CallNativeMethodAync('IMService', 'SendCodeMessage', jid, operation, codeId, codeTitle, langType, codeHead, codeDesc, codeSize, codeURL, codeLine, createTime, callback);
	};

	/**
	 * 邀请音频通话消息接口;
	 * @param jid             用户jid;
	 * @param confId             会议ID;
	 * @param confPwd            会议密码;
	 * @param hostId             主持人ID;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendInviteCallMessage = function(jid, confId, confPwd, hostId, callback) {
		return CallNativeMethodAync('IMService', 'SendInviteCallMessage', jid, confId, confPwd, hostId, callback);
	};

	/**
	 * 同意音频通话消息接口;
	 * @param jid             用户jid;
	 * @param confId             会议ID;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendAcceptCallMessage = function(jid, confId, hostId, callback) {
	    return CallNativeMethodAync('IMService', 'SendAcceptCallMessage', jid, confId, hostId, callback);
	};

	/**
	 * 拒绝音频通话消息接口
	 * @param jid 用户jid;
	 * @param confId 会议ID;
     * @param hostId 支持人ID;
     * @param reason 拒绝描述;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendRejectCallMessage = function(jid, confId, hostId, reason, callback) {
	    return CallNativeMethodAync('IMService', 'SendRejectCallMessage', jid, confId, hostId, reason, callback);
	};

	/**
	 * 结束音频通话消息接口;
	 * @param jid 用户jid;
	 * @param confId 会议ID;
     * @param hostId 支持人ID;
     * @param reason 结束描述;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendFinishCallMessage = function(jid, confId, hostId, reason, callback) {
	    return CallNativeMethodAync('IMService', 'SendFinishCallMessage', jid, confId, hostId, reason, callback);
	};

	/**
	 * 取消音频通话消息接口
	 * @param jid  用户jid
     * @param phoneNum  电话号码
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendCancelCallMessage = function(jid, phoneNum, callback) {
	    return CallNativeMethodAync('IMService', 'SendCancelCallMessage', jid, phoneNum, callback);
	};

	/**
	 * 未接通音频通话记录消息接口;
	 * @param jid 用户jid
     * @param phoneNum 电话号码
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendNotRespondCallRecordMessage = function(jid, phoneNum, callback) {
	    return CallNativeMethodAync('IMService', 'SendNotRespondCallRecordMessage', jid, phoneNum, callback);
	};

	/**
	 * 拒绝音频通话记录消息接口
	 * @param jid 用户jid
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	 /*
	ucClient.IMService.sendRejectCallRecordMessage = function(jid, callback) {
		return CallNativeMethodAync('IMService', 'SendRejectCallRecordMessage', jid, callback);
	};
	*/

	/**
	 * 接通音频通话记录消息接口
	 * @param jid 用户jid
	 * @param druation 通话时长
     * @param phoneNum 电话号码
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.IMService.sendConnectedCallRecordMessage = function(jid, druation, phoneNum, callback) {
	    return CallNativeMethodAync('IMService', 'SendConnectedCallRecordMessage', jid, druation, phoneNum, callback);
	};

	/**
	 * 发送消息已读接口
	 * @param jid 用户jid
	 * @param msgId 本地消息ID
	 * @param msgSeq 服务器消息ID
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.IMService.sendMessageRead = function(jid, msgId, msgSeq, callback) {
		return CallNativeMethodAync('IMService', 'SendMessageRead', jid, msgId, msgSeq, callback);
	};

	/**
	 * 发送多条消息已读接口
	 * @param toUserId 用户ID
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.IMService.sendMessagesReadById = function(toUserId, callback) {
	    return CallNativeMethodAync('IMService', 'SendMessagesReadById', toUserId, callback);
	};

	/* 发送消息已播放接口
	 * @param jid 用户jid
	 * @param msgId 本地消息ID
	 * @param msgSeq  服务器消息ID
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.IMService.sendMessagePlayed = function(jid, msgId, msgSeq, callback) {
		return CallNativeMethodAync('IMService', 'SendMessagePlayed', jid, msgId, msgSeq, callback);
	};
	


    // -------------------------------------------------------------------------------
	// GroupService接口
	// -------------------------------------------------------------------------------
	/**
     * 获取历史聊天内容接口;
     * @param type 会话类型
     * @param channel 消息来源
	 * @param groupId 请求目标ID（讨论组ID，组织ID）;
     * @param count 请求聊天内容数量，默认20，不能超过200;
	 * @param startTime 请求聊天内容的开始时间;
	 * @param endTime 请求聊天内容的结束时间;
     * @param callback 操作结束后的回调函数。 callback[0]-错误码
     */
	ucClient.GroupService.getChatHistory = function(channel, type, groupId, count, startTime, endTime, callback) {
	    return CallNativeMethodAync('GroupService', 'GetChatHistory', channel, type, groupId, count, startTime, endTime, callback);
	};

    /**
     * 获取搜索历史聊天内容接口;
     * @param type 会话类型
	 * @param relateId 请求目标ID（用户ID, 讨论组ID）;
     * @param count 请求聊天内容数量，默认20，不能超过200;
	 * @param startTime 请求聊天内容的开始时间;
	 * @param endTime 请求聊天内容的结束时间;
     * @param reverse 消息请求方向 1：从starttime向后取， 0：从endtime向前取;
     * @param callback 操作结束后的回调函数。 callback[0]-错误码
     */
	ucClient.GroupService.getSearchChatHistory = function(type, relateId, count, startTime, endTime, reverse, callback) {
	    return CallNativeMethodAync('GroupService', 'GetSearchChatHistory', type, relateId, count, startTime, endTime, reverse, callback);
	};

     /**
	 * 重发消息;
	 * @param msgId 消息id
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.GroupService.resendMessage = function(msgId, callback) {
		return CallNativeMethodAync('GroupService', 'ResendMessage', msgId, callback);
	};

      /**
	 * 转发消息;
     * @param jid 用户jid
	 * @param msgId 消息id
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.GroupService.forwardMessage = function(jid, msgId, callback) {
		return CallNativeMethodAync('GroupService', 'ForwardMessage', jid, msgId, callback);
	};

	/**
    * 消息撤回接口;
    * @param touserId 外部ID;
    * @param conversationType 会话类型;
    * @param seqId 需要撤消的消息ID;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
    */
	ucClient.GroupService.revocationMessage = function( touserId, sessionType, seqId,callback){
	    return CallNativeMethodAync('GroupService', 'RevocationMessage', touserId, sessionType, seqId,callback);
	}

	/**
     * 发送聊天文本消息接口;
	 * @param conversationType 聊天类型;
	 * @param groupJid 群ID/项目组JID;
     * @param textType 文本类型：纯文本，MIME格式;
     * @param textFormat 文本格式;
	 * @param textData 文本内容;
     * @param atUsers @用户id集合
     * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.GroupService.sendTextMessage = function(conversationType, groupJid, textType, textFormat, textData, atUsers, callback) {
	    return CallNativeMethodAync('GroupService', 'SendTextMessage', conversationType, groupJid, textType, textFormat, textData, atUsers, callback);
	};

	/**
	 * 发送聊天图片消息接口;
	 * @param conversationType 聊天类型;
	 * @param groupJid 群ID/项目组JID;
	 * @param imageName  图片名称;
	 * @param imageSize 图片大小;
	 * @param imageURL 图片下载路径;
	 * @param imagePath	本地图片路径;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.GroupService.sendImageMessage = function(conversationType, groupJid, imageName, imageSize, imageURL, imagePath, callback) {
	    return CallNativeMethodAync('GroupService', 'SendImageMessage', conversationType, groupJid, imageName, imageSize, imageURL, imagePath, callback);
	};

	/**
	 * 发送聊天音频消息接口;
	 * @param conversationType 聊天类型;
	 * @param groupJid  群ID/项目组JID;
	 * @param audioName 音频名称;
	 * @param audioSize 音频大小;
	 * @param audioURL 音频下载路径;
	 * @param audioDuration 音频时长;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.GroupService.sendAudioMessage = function(conversationType, groupJid, audioName, audioSize, audioURL, audioDuration, callback) {
		return CallNativeMethodAync('GroupService', 'SendAudioMessage', conversationType, groupJid, audioName, audioSize, audioURL, audioDuration, callback);
	};

	/**
	 * 发送聊天视频消息接口;
	 * @param conversationType 聊天类型;
	 * @param groupJid 群ID/项目组JID;
	 * @param videoName 视频名称;
	 * @param videoSize 视频大小;
	 * @param videoURL 视频下载路径;
	 * @param videoPath 本地视频路径;
	 * @param videoDuration 视频时长;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.GroupService.sendVideoMessage = function(conversationType, groupJid, videoName, videoSize, videoURL, videoPath, videoDuration, callback) {
	    return CallNativeMethodAync('GroupService', 'SendVideoMessage', conversationType, groupJid, videoName, videoSize, videoURL, videoPath, videoDuration, callback);
	};
   
   /**
    * 发送报告聊天消息接口
    * @param conversationType 聊天类型;
    * @param groupJid 群ID/项目组JID;
    * @param reportTitle		报告标题;
    * @param reportUrl          报告URL;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
    */
	ucClient.GroupService.sendReportMessage = function(conversationType, groupJid, reportTitle, reportUrl, callback) {
	    return CallNativeMethodAync('GroupService', 'SendReportMessage', conversationType, groupJid, reportTitle, reportUrl, callback);
	};

    /**
     * 发送会议录制视频聊天消息接口
	 * @param conversationType 聊天类型;
	 * @param groupJid 群ID/项目组JID;
     * @param videoName 视频名称;
     * @param videoSize 视频大小;
     * @param videoURL 视频下载路径;
     * @param videoThumb 视频缩图数据;
     * @param videoThumbURL 视频缩图URL;
     * @param videoDuration 视频时长;
     * @param operatorId    录制者ID;
     * @param operatorName  录制者名称;
     * @param operatorAvatar 录制者图像URL;
     * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
     */
	ucClient.GroupService.sendRecordVideoMessage = function(conversationType, groupJid, videoName, videoSize, videoURL, videoThumb, videoThumbURL,videoDuration, operatorId, operatorName, operatorAvatar, callback) {
	    return CallNativeMethodAync('GroupService', 'SendRecordVideoMessage', conversationType, groupJid, videoName, videoSize, videoURL, videoThumb, videoThumbURL,videoDuration, operatorId, operatorName, operatorAvatar, callback);
	};

	/**
    * 发送云盘文件消息接口;
    * @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param fileName			文件/文件夹名称
	* @param fileSize			文件大小（单位：字节）
	* @param cloudType			云盘类型（1-够快云库, 2-安贞母婴中心）
	* @param detailContent      内容
	* @param createTime         operateTime（uccserver操作时间,单位：秒）
	* @param fileId				fileId
        * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
    */
	
	ucClient.GroupService.sendCloudFileMessage = function(conversationType, operation, groupJid, fileName, filenameTo, fileSize, count, isDir, 
        cloudType, detailContent, callback) {
	    return CallNativeMethodAync('GroupService', 'SendCloudFileMessage', conversationType, operation, groupJid, fileName, filenameTo, fileSize.toString(), 
            count, isDir, cloudType, detailContent, callback);
	};

	/**
	* 发送音频通话邀请信息;
	* @param conversationType 聊天类型;
	* @param groupJid 群JID/项目组JID;
	* @param confId 会议ID;
	* @param confPwd 会议密码;
	* @param hostId 主持人ID;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	*/
	ucClient.GroupService.sendInviteCallMessage = function(conversationType, groupJid, confId, confPwd, hostId, callback) {
		return CallNativeMethodAync('GroupService', 'SendInviteCallMessage', conversationType, groupJid, confId, confPwd, hostId, callback);
	};

	/**
	* 发送同意音频通话消息接口;
	* @param conversationType 聊天类型;
	* @param groupJid 群ID/项目组JID;
	* @param confId 会议ID;
    * @param hostId 主持人ID;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	*/
	ucClient.GroupService.sendAcceptCallMessage = function(conversationType, groupJid, confId, hostId, callback) {
	    return CallNativeMethodAync('GroupService', 'SendAcceptCallMessage', conversationType, groupJid, confId, hostId, callback);
	};

	/**
	 * 发送拒绝音频通话消息接口;
	 * @param conversationType 聊天类型;
	 * @param groupJid 群JID/项目组JID;
	 * @param confId 会议ID;
     * @param hostId 主持人ID;
     * @param reason 拒绝描述;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	 */
	ucClient.GroupService.sendRejectCallMessage = function(conversationType, groupJid, confId, hostId, reason, callback) {
	    return CallNativeMethodAync('GroupService', 'SendRejectCallMessage', conversationType, groupJid, confId, hostId, reason, callback);
	};

	/**
	 * 发送结束音频通话消息接口;
	 * @param conversationType 聊天类型;
	 * @param groupJid 群JID/项目组JID;
	 * @param confId 会议ID;
     * @param hostId 主持人ID;
     * @param reason 结束描述;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	 */
	ucClient.GroupService.sendFinishCallMessage = function(conversationType, groupJid, confId, hostId, reason, callback) {
	    return CallNativeMethodAync('GroupService', 'SendFinishCallMessage', conversationType, groupJid, confId, hostId, reason, callback);
	};
	
	/**
	* 发送取消音频通话;
	* @param conversationType 聊天类型;
	* @param groupJid 群JID/项目组JID;
    * @param phoneNum 电话号码;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	*/
	ucClient.GroupService.sendCancelCallMessage = function(conversationType, groupJid, phoneNum, callback) {
	    return CallNativeMethodAync('GroupService', 'SendCancelCallMessage', conversationType, groupJid, phoneNum, callback);
	};

	/**
	* 发送未接通音频通话记录消息接口;
	* @param conversationType 聊天类型;
	* @param groupJid 群JID/项目组JID;
    * @param phoneNum 电话号码;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	*/
	ucClient.GroupService.sendNotRespondCallRecordMessage = function(conversationType, groupJid, phoneNum, callback) {
	    return CallNativeMethodAync('GroupService', 'SendNotRespondCallRecordMessage', conversationType, groupJid, phoneNum, callback);
	};

	/**
	* 发送拒绝音频通话记录消息接口;
	* @param conversationType 聊天类型;
	* @param groupJid 群JID/项目组JID;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	*/
	ucClient.GroupService.sendRejectCallRecordMessage = function(conversationType, groupJid, callback) {
		return CallNativeMethodAync('GroupService', 'SendRejectCallRecordMessage', conversationType, groupJid, callback);
	};

	/**
	* 发送接通音频通话记录消息接口;
	* @param conversationType 聊天类型;
	* @param groupJid 群JID/项目组JID;
	* @param druation 通话时长;
    * @param phoneNum 电话号码;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	*/
	ucClient.GroupService.sendConnectedCallRecordMessage = function(conversationType, groupJid, druation, phoneNum, callback) {
	    return CallNativeMethodAync('GroupService', 'SendConnectedCallRecordMessage', conversationType, groupJid, druation, phoneNum, callback);
	};

    /**
	* 发送消息已读接口;
	* @param conversationType 聊天类型;
	* @param groupJid 群JID/项目组JID;
	* @param msgId 本地消息ID;
	* @param msgSeq 服务器消息ID;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.GroupService.sendMessageRead = function(conversationType, groupJid, msgId, msgSeq, callback) {
		return CallNativeMethodAync('GroupService', 'SendMessageRead', conversationType, groupJid, msgId, msgSeq, callback);
	};

    /**
	* 发送多条消息已读接口;
    * @param conversationType 聊天类型;
	* @param groupId 群ID/项目组ID/Meeting Group;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.GroupService.sendMessagesReadById = function(conversationType, groupId, callback) {
	    return CallNativeMethodAync('GroupService', 'SendMessagesReadById', conversationType, groupId, callback);
	};

    /**
	* 发送消息已播放接口;
	* @param conversationType 聊天类型;
	* @param groupJid 群JID/项目组JID;
	* @param msgId 本地消息ID;
	* @param msgSeq 服务器消息ID;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.GroupService.sendMessagePlayed = function(conversationType, groupJid, msgId, msgSeq, callback) {
		return CallNativeMethodAync('GroupService', 'SendMessagePlayed', conversationType, groupJid, msgId, msgSeq, callback);
	};


	/**
    * 获取消息未读用户列表接口;
    * @param conversationType 聊天类型;
    * @param groupId 群ID/项目组ID/会议讨论组ID;
    * @param seqList 服务器消息ID列表;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码
    */
	ucClient.GroupService.getUnreadUserList = function(conversationType, groupId, seqList, callback) {
	    return CallNativeMethodAync('GroupService', 'GetUnreadUserList', conversationType, groupId, seqList, callback);
	};

	/**
	 * 获取讨论组列表;
	 * @param groupType 请求讨论组的类型;
	 * @param page 请求讨论组的页数;
	 * @param count 请求讨论组的数量;
	 * @param lastGroupTime 上一次请求讨论组列表时间;
     * @param allData 是否需要所有讨论组信息：ture 需要; false: 不需要;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.getGroupList = function(groupType, page, count, lastGroupTime, alldata, callback) {
	    return CallNativeMethodAync('GroupService', 'GetGroupList', groupType, page, count, lastGroupTime, alldata, callback);
	};

	/**
	 * 获取讨论组信息;
	 * @param groupId 讨论组ID;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.getGroupInfo = function(groupId, callback) {
		return CallNativeMethodAync('GroupService', 'GetGroupInfo', groupId, callback);
	};
	
    ucClient.GroupService.getGroupInfoSync = function(groupId) {
		var request = ucClient.xmlHttpRequest('GroupService', 'GetGroupInfoSync', groupId);
		if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
	};
	
    ucClient.GroupService.getGroupInfoByMountIdSync = function(mountId) {
        var request = ucClient.xmlHttpRequest('GroupService', 'GetGroupInfoByMountIdSync', mountId);
        if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
    };

	/**
	 * 创建讨论组;
     * @param groupType 讨论组类型;
	 * @param groupName 讨论组名称;
	 * @param userInfos 讨论组人员信息;
	 * @param nameFlag 是否为正式名称:0:否/1:是;
	 * @param memberType 讨论组成员上限类型:0:小讨论组/1:大讨论组;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.createGroup = function(groupType, groupName, userInfos, nameFlag, memberType, callback) {
	    return CallNativeMethodAync('GroupService', 'CreateGroup', groupType, groupName, userInfos, nameFlag, memberType, callback);
	};

	/**
	 * 结束讨论组;
	 * @param groupId 讨论组ID;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.endGroup = function(groupId, callback) {
		return CallNativeMethodAync('GroupService', 'EndGroup', groupId, callback);
	};

	/**
	 * 退出讨论组;
	 * @param groupId 讨论组ID;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.exitGroup = function(groupId, callback) {
		return CallNativeMethodAync('GroupService', 'ExitGroup', groupId, callback);
	};

	/**
	 * 添加讨论组成员;
	 * @param groupId 讨论组ID;
	 * @param userInfos 添加讨论组人员信息;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.addMember = function(groupId, userInfos, callback) {
		return CallNativeMethodAync('GroupService', 'AddMember', groupId, userInfos, callback);
	};

	/**
	 * 删除讨论组成员;
	 * @param groupId 讨论组ID;
	 * @param members 删除讨论组人员集合;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.removeMember = function(groupId, members, callback) {
		return CallNativeMethodAync('GroupService', 'RemoveMember', groupId, members, callback);
	};

	/**
	 * 更新讨论组名称;
	 * @param groupId 讨论组ID;
	 * @param groupName 讨论组名称;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.setGroupName = function(groupId, groupName, callback) {
		return CallNativeMethodAync('GroupService', 'SetGroupName', groupId, groupName, callback);
	};
	
	/**
	 * 获取讨论组文档列表;
	 * @param groupId 讨论组ID;
	 * @param page 请求讨论组文档的页数，默认1;
	 * @param count 请求讨论组文档的数量，默认20;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.getDocumentList = function(groupId, page, count, callback) {
		return CallNativeMethodAync('GroupService', 'GetDocumentList', groupId, page, count, callback);
	};

	/**
	* 上传/分享讨论组文档;
	* @param groupId 群ID/项目组ID;
    * @param operation 0:create; 1:forward
	* @param fileName 文件名称;
	* @param fileSize 文件大小;
	* @param fileURL 文件下载路径;
	* @param uploadTime 文档上传时间;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.GroupService.uploadDocument = function(groupId, operation, fileName, fileSize, fileURL, callback) {
	    return CallNativeMethodAync('GroupService', 'UploadDocument', groupId, operation, fileName, fileSize.toString(), fileURL, callback);
	};

	ucClient.GroupService.uploadCloudDocument = function(groupId, operation, isSendMessage, mountId, fileName, fileSize, 
        cloudType, hashCode, detailContent, callback) {
	    return CallNativeMethodAync('GroupService', 'UploadCloudDocument', groupId, operation, isSendMessage, mountId, 
            fileName, fileSize.toString(), cloudType, hashCode, detailContent, callback);
	};

	/**
	 * 删除讨论组文档;
	 * @param groupId 讨论组ID;
	 * @param fileId 文件ID;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.deleteDocument = function(groupId, fileId, callback) {
		return CallNativeMethodAync('GroupService', 'DeleteDocument', groupId, fileId, callback);
	};

    /**
	* 上传/分享讨论组代码;
	* @param groupId 群ID/项目组ID;
    * @param operation 0:create; 1:forward
	* @param codeTitle code标题;
	* @param langType code类型;
	* @param codeHead codeHead;
	* @param codeDesc code描述;
    * @param codeSize code size;
    * @param codeUrl  code url;
    * @param codeLine code line;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.GroupService.uploadCodeSnippet = function(groupId, operation, codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, callback) {
		return CallNativeMethodAync('GroupService', 'UploadCodeSnippet',groupId, operation, codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, callback);
    }

	/**
	 * 获取讨论组文档信息;
	 * @param groupId 讨论组ID;
	 * @param fileId 文件ID;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.getDocumentInfo = function(groupId, fileId, cloudFileInfo, callback) {
		//暂时这样修改，兼容以前的老代码
	    var cb;
	    if (typeof(callback) == "function") {
	        cb = callback;
	    }
	    else if (typeof(cloudFileInfo) == "function") {
	        cb = cloudFileInfo;
	    }
	    else {
	        cb = undefined;
	    }

	    var cfi ='';
	    if (typeof(cloudFileInfo) == "string") {
	        cfi = cloudFileInfo;
	    }

	    return CallNativeMethodAync('GroupService', 'GetDocumentInfo', groupId, fileId, cfi, cb);
	};

    /**
	 * 获取评论信息列表;
	 * @param groupId   讨论组ID;
	 * @param fileId    文件或代码ID;
	 * @param count     请求文档评论信息的数量;
     * @param startTime 请求评论信息的开始时间;
     * @param endTime   请求评论信息的结束时间;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.getCommentList = function(groupId, fileId, count, startTime, endTime, callback) {
		return CallNativeMethodAync('GroupService', 'GetCommentList', groupId, fileId, count, startTime, endTime, callback);
	};

     /**
	 * 获取@消息列表;
	 * @param groupId   讨论组ID;
	 * @param page      页数
	 * @param count     请求文档评论信息的数量;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.getAtMessageList = function(groupId, page, count, callback) {
	    return CallNativeMethodAync('GroupService', 'GetAtMessageList', groupId, page, count, callback);
	};

	/**
    * 删除@消息;
    * @param groupId   讨论组ID;
    * @param type      会话类型;
    * @param seqList   消息seq列表
    * @param callback 操作结束后的回调函数。 callback[0]-错误码
    */
	ucClient.GroupService.deleteAtMessage = function(type, groupId, seqList, callback) {
	    return CallNativeMethodAync('GroupService', 'DeleteAtMessage', type, groupId, seqList, callback);
	};

     /**
	 * 创建评论信息;
	 * @param groupId 讨论组ID;
	 * @param fileId  文件或代码ID;
	 * @param fileType 文档类型：0:文档/1:代码;
     * @param fileTitle 文档或代码标题;
     * @param content   评论内容;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	 */
	ucClient.GroupService.createComment = function(groupId, fileId, fileType, fileTitle, content, callback) {
		return CallNativeMethodAync('GroupService', 'CreateComment', groupId, fileId, fileType, fileTitle, content, callback);
	};

	/**
	 * 设置讨论组logo;
	 * @param groupId 讨论组ID;
	 * @param logoURL logo路径;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.setGroupLogo = function(groupId, logoURL, callback) {
		return CallNativeMethodAync('GroupService', 'SetGroupLogo', groupId, logoURL, callback);
	};

	/**
     * 更新本地讨论组logo路径;
     * @param groupId 讨论组ID;
     * @param localPath logo本地路径;
     * @param callback 操作结束后的回调函数。 callback[0]-错误码
     */
	ucClient.GroupService.updateGroupLogo = function(groupId, localPath, callback) {
	    return CallNativeMethodAync('GroupService', 'UpdateGroupLogo', groupId, localPath, callback);
	};

	/**
	 * 设置讨论组聊天置顶;
	 * @param groupId 讨论组ID;
	 * @param state 是否设置讨论组置顶:0:取消置顶/1:设置置顶;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.setTopChat = function(groupId, state, callback) {
		return CallNativeMethodAync('GroupService', 'SetTopChat', groupId, state, callback);
	};

	/**
	 * 设置讨论组免打扰;
	 * @param groupId 讨论组ID;
	 * @param state 是否设置讨论组面打扰:0:取消打扰/1:设置打扰;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.setDoNotDisturb = function(groupId, state, callback) {
		return CallNativeMethodAync('GroupService', 'SetDoNotDisturb', groupId, state, callback);
	};
	
	/**
	 * 设置讨论组提醒;
	 * @param groupId 讨论组ID;
	 * @param state 是否设置讨论组提醒:0:取消提醒/1:设置提醒;
	 * @param alertRule 讨论组提醒规则;
	 * @param alertTime 讨论组提醒时间;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.GroupService.setReminder = function(groupId, state, alertRule, alertTime, callback) {
		return CallNativeMethodAync('GroupService', 'SetReminder', groupId, state, alertRule, alertTime, callback);
	};
	


    // -------------------------------------------------------------------------------
	// ----------------------------CalendarService接口--------------------------------
    // -------------------------------------------------------------------------------
	/**
	 * 创建会议
	 * @param attendees 参会人列表
	 * @param title 会议主题
	 * @param location 会议地点
	 * @param summary 会议摘要
	 * @param timeZone 时区
     * @param hostName 主持人名称
	 * @param startTime 会议开始时间，UTC Unix时间戳
	 * @param duration 会议时长
     * @param authorizerId 授权人ID
     * @param cycleRole 周期规则
	 * @param isGNet 是否全时云会议
     * @param isCycle 是否周期会议
     * @param isWholeDay 是否全天会议
     * @param language 会议语言 1--中文，2--英文
	 * @param emailLanguage 邮件通知语言 1--中文，2--英文
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.CalendarService.createMeeting = function(attendees, title, location, summary, timeZone, hostName, extrasInfo,
		startTime, duration, authorizerId, cycleRole, isGNet, isCycle, isWholeDay, language, emailLanguage, callback) {
		return CallNativeMethodAync('CalendarService', 'CreateMeeting', attendees, title, 
			location, summary, timeZone, hostName, extrasInfo, startTime, duration, authorizerId, cycleRole, isGNet, isCycle, isWholeDay, language, emailLanguage, callback);
	};
	
	/**
	 * 变更会议
	 * @param conferenceId 会议ID
	 * @param eventID 事件ID
     * @param oriStart 原会议开始时间
	 * @param attendees 参会人列表
	 * @param title 会议主题
	 * @param location 会议地点
	 * @param summary 会议摘要
	 * @param timeZone 时区
	 * @param startTime 会议开始时间，UTC Unix时间戳
	 * @param duration 会议时长
     * @param authorizerId 授权人ID
     * @param repeatEndDate 周期结束时间
	 * @param isGNet 是否全时云会议
     * @param isCycle 是否周期会议
     * @param isWholeDay 是否全天会议
     * @param language 会议语言 1--中文，2--英文
	 * @param emailLanguage 邮件通知语言 1--中文，2--英文
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码
	 */
	ucClient.CalendarService.updateMeeting = function(eventId, oriStart, attendees, title, location, summary, timeZone, extrasInfo,
		startTime, duration, authorizerId, cycleRole, isGNet, isCycle, isWholeDay, language, emailLanguage, callback) {
	    return CallNativeMethodAync('CalendarService', 'UpdateMeeting', eventId, oriStart, attendees, title, location, summary, timeZone, extrasInfo,
			startTime, duration, authorizerId, cycleRole, isGNet, isCycle, isWholeDay, language, emailLanguage, callback);
	};

	/**
	* 取消会议
	* @param eventID 事件ID
	* @param startTime 会议开始时间
    * @param authorizerId 授权人ID
    * @param language 会议语言 1--中文，2--英文
	* @param emailLanguage 邮件通知语言 1--中文，2--英文
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.cancelMeeting = function(eventId, startTime, authorizerId, language, emailLanguage, callback) {
	    return CallNativeMethodAync('CalendarService', 'CancelMeeting', eventId, startTime, authorizerId, language, emailLanguage, callback);
	};

	/**
	* 获取会议调起串
	* @param confId   会议ID
	* @param password 参会密码
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.getMeetingStartURL = function(confId, password, callback) {
	    return CallNativeMethodAync('CalendarService', 'GetMeetingStartURL', confId, password, callback);
	};
	
	ucClient.CalendarService.getMeetingStartURLSync = function(confId, password) {
	    var request = ucClient.xmlHttpRequest('CalendarService', 'GetMeetingStartURLSync', confId, password);
	    if (request.status == 200) {
	        return JSON.parse(request.responseText);
	    }
	    else{
	        return undefined;
	    }
	};

	/**
	* 判断是否在会中
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.isInMeeting = function(callback) {
	    return CallNativeMethodAync('CalendarService', 'IsInMeeting', callback);
	};

	/**
	* 获取通过web入会的调起串
	* @param confId 会议ID
	* @param mail 邮件账号
	* @param password 参会密码
	* @param displayName 会议监控中显示的名字（必填）
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.getMeetingWebStartURL = function(confId, mail, password, displayName, callback) {
	    return CallNativeMethodAync('CalendarService', 'GetMeetingWebStartURL', confId, mail, password, displayName, callback);
	};

	/**
	* 更新授权设置
	* @param authorizedPersonId 被授权人ID
	* @param status 授权状态：授权1， 取消0
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.updateAccreditSetting = function(authorizedPersonId, status, callback) {
		return CallNativeMethodAync('CalendarService', 'UpdateAccreditSetting', authorizedPersonId, status, callback);
	};

	/**
	* 获取授权用户列表 （包括授权与被授权用户）
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.getAccreditUserList = function(callback) {
		return CallNativeMethodAync('CalendarService', 'GetAccreditUserList', callback);
	};
    ucClient.CalendarService.getAccreditUserListSync = function() {
        var request = ucClient.xmlHttpRequest('CalendarService', 'GetAccreditUserListSync');
        if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
    };

	/**
	* 获取会议列表
	* @param startTime 开始时间，UTC时间戳,以秒为单位
	* @param endTime 结束时间，UTC时间戳,以秒为单位
	* @param limit 请求的条数，默认为200
    * @param authorizerId 授权人ID
	* @param eventIdFilter eventid过滤器，大于0有效
    * @param bfilterAsc 过滤规则，与eventIdFilter联合使用。True：过滤开始时间为搜索开始时间头， 并且eventid<= eventIdFilter的会议。False：过滤开始时间为搜索开始时间尾， 并且eveintid〉=eventIdFilter的会议。默认true
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-requestId
	*/
    ucClient.CalendarService.getMeetingList = function(startTime, endTime, limit, authorizerId, eventIdFilter, bfilterAsc, refreshflag, callback) {
        return CallNativeMethodAync('CalendarService', 'GetMeetingList', startTime, endTime, limit, authorizerId, eventIdFilter, bfilterAsc, refreshflag, callback);
	};

    ucClient.CalendarService.getMeetingListSync = function(startTime, endTime, limit, authorizerId, eventIdFilter, bfilterAsc, refreshflag) {
        var request = ucClient.xmlHttpRequest('CalendarService', 'GetMeetingListSync', startTime, endTime, limit, authorizerId, eventIdFilter, bfilterAsc, refreshflag);
        if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
	};
	
	/**
	* 获取简明会议列表
	* @param startTime 开始时间，UTC时间戳,以秒为单位
	* @param endTime 结束时间，UTC时间戳,以秒为单位
    * @param authorizerId 授权人ID
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-requestId
	*/
    ucClient.CalendarService.getBriefMeetingList = function(startTime, endTime, authorizerId, callback) {
        return CallNativeMethodAync('CalendarService', 'GetBriefMeetingList', startTime, endTime, authorizerId, callback);
    };

	/**
	* 获取变化的简明会议列表
	* @param startTime 上次同步时间
    * @param authorizerId 授权人ID
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-requestId
	*/
    ucClient.CalendarService.getBriefChangedMeetingList = function(startTime, authorizerId, callback) {
        return CallNativeMethodAync('CalendarService', 'GetBriefChangedMeetingList', startTime, authorizerId, callback);
    };

	/**
	* 接受会议邀请
	* @param eventId				事件ID
    * @param startTime              开始时间
	* @param authorizerId			授权人ID
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
    ucClient.CalendarService.acceptMeetingInvitation = function(eventId, startTime, authorizerId, callback) {
        return CallNativeMethodAync('CalendarService', 'AcceptMeetingInvitation', eventId, startTime, authorizerId, callback);
	};

	/**
	* 拒绝会议邀请
	* @param eventId 事件ID
    * @param startTime 开始时间
	* @param authorizerId 授权人ID
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
    ucClient.CalendarService.denyMeetingInvitation = function(eventId, startTime, authorizerId, callback) {
        return CallNativeMethodAync('CalendarService', 'DenyMeetingInvitation', eventId, startTime, authorizerId, callback);
	};
	
	/**
	* 根据会议ID获取单场会议详情
	* @param eventId 事件ID
	* @param startTime 限定范围的起始时间
	* @param startTime 限定范围的终止时间
	* @param authorizerId 授权人ID
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.getMeetingInfoById = function(eventId, startTime, endTime, authorizerId, callback) {
	    return CallNativeMethodAync('CalendarService', 'GetMeetingInfoById', eventId, startTime, endTime, authorizerId, callback);
	};
	
	ucClient.CalendarService.getMeetingInfoByIdSync = function(eventId, startTime, endTime, authorizerId) {
	    var request = ucClient.xmlHttpRequest('CalendarService', 'GetMeetingInfoByIdSync', eventId, startTime, endTime, authorizerId);
	    if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
    };

	/**
	* 根据多个会议ID获取多场会议详情
	* @param eventIds 事件ID列表
    * @param startTime 限定范围的起始时间
	* @param startTime 限定范围的终止时间
	* @param authorizerId 授权人ID
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-requestId
	*/
	ucClient.CalendarService.getMeetingInfoByIds = function(eventIds, startTime, endTime, authorizerId, callback) {
		return CallNativeMethodAync('CalendarService', 'GetMeetingInfoByIds', eventIds, startTime, endTime, authorizerId, callback);
	};

    ucClient.CalendarService.getMeetingInfoByIdsSync = function(eventIds, startTime, endTime, authorizerId) {
        var request = ucClient.xmlHttpRequest('CalendarService', 'GetMeetingInfoByIdsSync', eventIds, startTime, endTime, authorizerId);
        if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
    };

	/**
	* 转发会议
	* @param eventId 事件ID
	* @param attendees 参会人列表
    * @param startTime 会议开始时间
	* @param authorizerId 授权人ID
    * @param language 会议语言 1--中文，2--英文
	* @param emailLanguage 邮件通知语言 1--中文，2--英文
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.forwardMeeting = function(eventId, attendees, startTime, authorizerId, language, emailLanguage, callback) {
	    return CallNativeMethodAync('CalendarService', 'ForwardMeeting', eventId, attendees, startTime, authorizerId, language, emailLanguage, callback);
	};

	/**
	* 创建远程协助
	* @param userId 对端用户ID
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.createRemoteAssistance = function(userId, callback) {
	    return CallNativeMethodAync('CalendarService', 'CreateRemoteAssistance', userId, callback);
	}
	/**
	* 获取用户忙闲日程列表
	* @param userIdList 用户ID列表
	* @param startTime 开始日期，unix时间戳
	* @param endTime 结束日期，unix时间戳
	* @param timeInterval 时间间隔，在5-60之间
    * @param eventIdFilter 事件过滤器
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-requestId
	*/
	ucClient.CalendarService.getUserSchedule = function(userIdList, startTime, endTime, timeInterval, eventIdFilter, callback) {
	    return CallNativeMethodAync('CalendarService', 'GetUserSchedule', userIdList, startTime, endTime, timeInterval, eventIdFilter, callback);
	};

	 /**
	* 获取用户忙闲日程列表(新)
	* @param userIdList 用户ID列表
	* @param startTime 开始日期，unix时间戳
	* @param endTime 结束日期，unix时间戳
	* @param shareUserId 授权人ID
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-requestId
	*/
	ucClient.CalendarService.getUserBusyFreeInfo = function(userIdList, startTime, endTime, shareUserId, callback) {
	    return CallNativeMethodAync('CalendarService', 'GetUserBusyFreeInfo', userIdList, startTime, endTime, shareUserId, callback);
	};

	/**
	* 创建会议纪要
	* @param eventId 事件ID
    * @param startTime 会议开始时间
	* @param minutesId 会议纪要ID
	* @param title 会议纪要标题
	* @param minutes 会议纪要内容
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.createMinutes = function(eventId, startTime, title, minutes, callback) {
	    return CallNativeMethodAync('CalendarService', 'CreateMinutes', eventId, startTime, title, minutes, callback);
	};

	/**
	* 更新会议纪要
	* @param eventId 事件ID
	* @param minutesId 会议纪要ID
	* @param title 会议纪要标题
	* @param minutes 会议纪要内容
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.updateMinutes = function(eventId, minutesId, title, minutes, callback) {
	    return CallNativeMethodAync('CalendarService', 'UpdateMinutes', eventId, minutesId, title, minutes, callback);
	};
	
	/**
	* 获取会议纪要详情
	* @param eventId 事件ID
	* @param minutes 会议纪要ID
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.getMinutesDetails = function(eventId, minutesId, callback) {
	    return CallNativeMethodAync('CalendarService', 'GetMinutesDetails', eventId, minutesId, callback);
	};

	/**
	* 根据事件ID获取会议纪要列表
	* @param eventId 事件ID
    * @param startTime 会议开始时间
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.getMinutesList = function(eventId, startTime, callback) {
	    return CallNativeMethodAync('CalendarService', 'GetMinutesList', eventId, startTime, callback);
	};

	/**
    * 上传/分享会议文档;
    * @param eventId 事件ID
    * @param operation 0:create; 1:forward
    * @param fileName 文件名称;
    * @param fileSize 文件大小;
    * @param fileURL 文件下载路径;
    * @param uploadTime 文档上传时间;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码
    */
	ucClient.CalendarService.uploadMeetingFile = function(eventId, operation, fileName, fileSize, fileURL, createTime, callback) {
	    return CallNativeMethodAync('CalendarService', 'UploadMeetingFile', eventId, operation, fileName, fileSize.toString(), fileURL, createTime, callback);
	};

	ucClient.CalendarService.uploadCloudFile = function(eventId, operation, mountId, fileName, fileSize, 
    cloudType, hashCode, detailContent, callback) {
	    return CallNativeMethodAync('CalendarService', 'UploadCloudFile', eventId, operation, mountId, 
            fileName, fileSize.toString(), cloudType, hashCode, detailContent, callback);
	};

    /**
    * 删除会议文档;
    * @param minutesId 会议纪要ID;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码
    */
	ucClient.CalendarService.deleteMeetingFile = function(minutesId, callback) {
	    return CallNativeMethodAync('CalendarService', 'DeleteMeetingFile', minutesId, callback);
	};

	/**
	* 删除会议录像
	* @param eventId    事件ID
    * @param confId     会议ID
    * @param startTime  会议开始时间
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.deleteMeetingRecording = function(eventId, confId, startTime, callback) {
	    return CallNativeMethodAync('CalendarService', 'DeleteMeetingRecording', eventId, confId, startTime, callback);
	};

	/**
    * 会议消息撤回;
    * @param groupId  会议讨论组ID
    * @param eventId  会议eventID;
    * @param seqId    seqId;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
    */
	ucClient.CalendarService.revocationMeetingMessage = function( groupId,eventId,seqId,callback){
	    return CallNativeMethodAync('CalendarService', 'RevocationMeetingMessage',groupId,eventId,seqId,callback);
	}

	/**
	* 设置会议提醒
	* @param alarmStatus 会议提醒状态：0--关闭，1--开启
    * @param alarmTime  会议提醒时间
	* @param callback 操作结束后的回调函数。 callback[0]-错误码
	*/
	ucClient.CalendarService.setMeetingAlarm = function(alarmStatus, alarmTime, callback) {
	    return CallNativeMethodAync('CalendarService', 'SetMeetingAlarm', alarmStatus, alarmTime, callback);
	};

     /**
	 * 重发消息;
	 * @param msgId 消息id
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.CalendarService.resendMessage = function(msgId, callback) {
		return CallNativeMethodAync('CalendarService', 'ResendMessage', msgId, callback);
	};

      /**
	 * 转发消息;
     * @param jid 用户jid
	 * @param msgId 消息id
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
	 */
	ucClient.CalendarService.forwardMessage = function(jid, msgId, callback) {
		return CallNativeMethodAync('CalendarService', 'ForwardMessage', jid, msgId, callback);
	};

	/**
     * 发送聊天文本消息接口;
	 * @param conferenceId	会议ID;
	 * @param startTime	会议开始时间;
	 * @param groupUid 会议讨论组UID;
     * @param textType 文本类型：纯文本，MIME格式;
     * @param textFormat 文本格式;
	 * @param textData 文本内容;
     * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.CalendarService.sendTextMessage = function(eventId, startTime, groupJid, textType, textFormat,textData, callback) {
		return CallNativeMethodAync('CalendarService', 'SendTextMessage', eventId, startTime, groupJid, textType, textFormat, textData, callback);
	};

	/**
	 * 发送聊天图片消息接口;
	 * @param conferenceId	会议ID;
	 * @param startTime	会议开始时间;
	 * @param groupUid 会议讨论组UID;
	 * @param imageName  图片名称;
	 * @param imageSize 图片大小;
	 * @param imageURL 图片下载路径;
	 * @param imagePath 本地图片路径;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.CalendarService.sendImageMessage = function(eventId, startTime, groupJid, imageName, imageSize, imageURL, imagePath, callback) {
	    return CallNativeMethodAync('CalendarService', 'SendImageMessage', eventId, startTime, groupJid, imageName, imageSize, imageURL, imagePath, callback);
	};

	/**
	 * 发送聊天音频消息接口;
	 * @param conferenceId	会议ID;
	 * @param startTime	会议开始时间;
	 * @param groupUid 会议讨论组UID;
	 * @param audioName 音频名称;
	 * @param audioSize 音频大小;
	 * @param audioURL 音频下载路径;
	 * @param audioDuration 音频时长;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.CalendarService.sendAudioMessage = function(eventId, startTime, groupJid, audioName, audioSize, audioURL, audioDuration, callback) {
		return CallNativeMethodAync('CalendarService', 'SendAudioMessage', eventId, startTime, groupJid, audioName, audioSize, audioURL, audioDuration, callback);
	};

	/**
	 * 发送聊天视频消息接口;
	 * @param conferenceId	会议ID;
	 * @param startTime	会议开始时间;
	 * @param groupUid 会议讨论组UID;
	 * @param videoName 视频名称;
	 * @param videoSize 视频大小;
	 * @param videoURL 视频下载路径;
	 * @param videoPath 本地视频路径;
	 * @param videoDuration 视频时长;
	 * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.CalendarService.sendVideoMessage = function(eventId, startTime, groupJid, videoName, videoSize, videoURL, videoPath, videoDuration, callback) {
	    return CallNativeMethodAync('CalendarService', 'SendVideoMessage', eventId, startTime, groupJid, videoName, videoSize, videoURL, videoPath, videoDuration, callback);
	};

	ucClient.CalendarService.sendRemoteAssistanceInviteMessage = function(uid, confId, confPwd, callback) {
	    return CallNativeMethodAync('CalendarService', 'SendRemoteAssistanceInviteMsg', uid, confId, confPwd, callback);
	};

	ucClient.CalendarService.sendRemoteAssistanceAcceptMessage = function(uid, confId, callback) {
	    return CallNativeMethodAync('CalendarService', 'SendRemoteAssistanceAcceptMsg', uid, confId, callback);
	};

	ucClient.CalendarService.sendRemoteAssistanceRejectMessage = function(uid, confId, callback) {
	    return CallNativeMethodAync('CalendarService', 'SendRemoteAssistanceRejectMsg', uid, confId, callback);
	};

    ucClient.CalendarService.startMeetingClient = function(params, callback) {
        return CallNativeMethodAync('CalendarService', 'StartMeetingClient', params, callback);
    };

    ucClient.CalendarService.checkMeetingClient = function(callback) {
        return CallNativeMethodAync('CalendarService', 'CheckMeetingClient', callback);
    };
    
    ucClient.CalendarService.killMeetingClient = function(callback) {
        return CallNativeMethodAync('CalendarService', 'KillMeetingClient', callback);
    };

    ucClient.CalendarService.cancelJoinMeeting = function(callback) {
        return CallNativeMethodAync('CalendarService', 'CancelJoinMeeting', callback);
    };

    ucClient.CalendarService.sendPosition2MeetingClient = function(posX, posY, callback) {
        return CallNativeMethodAync('CalendarService', 'SendPosition2MeetingClient', posX, posY, callback);
    };
    
    ucClient.CalendarService.createMeetingRoom =function(attendees, title, remarks, password, startTime, endTime,
		roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin, callback) {
        return CallNativeMethodAync('CalendarService', 'CreateMeetingRoom', attendees, title, remarks, password, startTime, endTime,
		roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin, callback);
    };

    ucClient.CalendarService.updateMeetingRoom =function(roomId, attendees, title, remarks, password, startTime, endTime,
        roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin, callback) {
        return CallNativeMethodAync('CalendarService', 'UpdateMeetingRoom', roomId, attendees, title, remarks, password, startTime, endTime,
		roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin, callback);
    };


    ucClient.CalendarService.createMeetingInMeetingRoom =function(attendees, roomId, title, summary, startTime, duration, callback) {
        return CallNativeMethodAync('CalendarService', 'CreateMeetingInMeetingRoom', attendees, roomId, title, summary, startTime, duration, callback);
    };

    ucClient.CalendarService.getMeetingRoomList =function(orgId, callback) {
        return CallNativeMethodAync('CalendarService', 'GetMeetingRoomList', orgId, callback);
    };

    ucClient.CalendarService.checkJoinMeetingRoom =function(roomId, callback) {
        return CallNativeMethodAync('CalendarService', 'CheckJoinMeetingRoom', roomId, callback);
    };

    ucClient.CalendarService.checkMeetingRoomPassword =function(roomId, password, callback) {
        return CallNativeMethodAync('CalendarService', 'CheckMeetingRoomPassword', roomId, password, callback);
    };

    ucClient.CalendarService.deleteMeetingRoom =function(roomId, callback) {
        return CallNativeMethodAync('CalendarService', 'DeleteMeetingRoom', roomId, callback);
    };	

    ucClient.CalendarService.checkRightsOfCreateRoom =function(callback) {
        return CallNativeMethodAync('CalendarService', 'CheckRightsOfCreateRoom', callback);
    };	

	// -------------------------------------------------------------------------------
    // ----------------------------FileTransfer Service接口----------------------------
    // -------------------------------------------------------------------------------
    /**
     * 上传文件
     * @param filePath 文件路径
     * @param callback 返回 taskId
     */
    ucClient.FileTransferService.uploadFile = function(filePath, callback) {
        return CallNativeMethodAync('FileTransferService', 'UploadFile', filePath, callback);
    };

    /**
     * 判断文件是否正在上传
     * @param taskId 任务Id
     * @param callback  返回是否正在上传
     */
    ucClient.FileTransferService.isFileUploading = function(taskId, callback) {
        return CallNativeMethodAync('FileTransferService', 'IsFileUploading', taskId, callback);
    };
	
    /**
     * 取消文件上传
     * @param taskId 任务Id
     */
    ucClient.FileTransferService.cancelUploadFile = function(taskId) {
        return CallNativeMethodAync('FileTransferService', 'CancelUploadFile', taskId);
    };

    /**
     * 下载文件
     * @param filePath 文件路径
     * @param callback 返回 taskId
     */
    ucClient.FileTransferService.downloadFile = function(downloadUrl, savePath, callback) {
        return CallNativeMethodAync('FileTransferService', 'DownloadFile', downloadUrl, savePath, callback);
    };

    /**
     * 判断文件是否正在下载
     * @param taskId 任务Id
     * @param callback  返回是否正在上传
     */
    ucClient.FileTransferService.isFileDownloading = function(taskId, callback) {
        return CallNativeMethodAync('FileTransferService', 'IsFileDownloading', taskId, callback);
    };
	
    /**
     * 取消文件下载
     * @param taskId 任务Id
     */
    ucClient.FileTransferService.cancelDownloadFile = function(taskId) {
        return CallNativeMethodAync('FileTransferService', 'CancelDownloadFile', taskId);
    };

	 /**
     * 通过fid获取downloadUrl
     * @param fid 文件Id
     */
    ucClient.FileTransferService.getDownloadUrlFromFid = function(fid, callback) {
        var request = ucClient.xmlHttpRequest('FileTransferService', 'getDownloadUrlFromFidSync', fid);
        if (request.status == 200) {
            return request.responseText;
        } else {
            return undefined;
        }
    }

    /**
    * 通过fid或downloadUrl删除文件
    * @param fid 文件Id或downloadUrl
    */
    ucClient.FileTransferService.deleteFileFromFS = function(fid, callback) {
        return CallNativeMethodAync('FileTransferService', 'DeleteFileFromFS', fid, callback);
    }

	/**
    * 通过taskId重启任务
    * @param taskId
    */
    ucClient.FileTransferService.resumeTask = function(taskId, callback) {
        return CallNativeMethodAync('FileTransferService', 'ResumeTask', taskId, callback);
    }

    /**
     * 更新文件的本地路径
     * @param fileURL 文件的URL
     * @param filePath 文件的本地路径
     */
    ucClient.FileTransferService.updateLocalFilePath = function(fileURL, filePath) {
        return CallNativeMethodAync('FileTransferService', 'UpdateLocalFilePath', fileURL, filePath);
    }

    ucClient.FileTransferService.getLocalFilePathSync = function(fileURL) {
        var request = ucClient.xmlHttpRequest('FileTransferService', 'getLocalFilePathSync', fileURL);
        if (request.status == 200) {
            return request.responseText;
        } else {
            return undefined;
        }
    }
	
	// -------------------------------------------------------------------------------
	// ----------------------------PBX Service接口----------------------------
	// -------------------------------------------------------------------------------
	    
	    /* @function
        功能     : 查看登陆情况
        参数     : [OUT] ret   : 0 成功， 1 未连接， 2 连接中， 3 和服务器网络断开
		*/
    ucClient.PbxService.checkLogin = function(callback) {
        return CallNativeMethodAync('PbxService', 'checkLogin', callback);
    };
	    /* @function
        功能     : VoIP呼叫
        参数     : [IN] userName   : 用户名
                    [IN] number    : 呼叫号码
					[IN] callType  : 呼叫类型
						VOICE_CALL, // 语音呼叫
						VIDEO_CALL  // 语音视频呼叫
					[IN] postValue : 需要传递给被叫方的值，长度最好不超过256，且最好用BASE64加密
                    [OUT] callID   : 通话ID
		*/
    ucClient.PbxService.makeCall = function(userName, userID, number, callType, postValue, callback) {
        return CallNativeMethodAync('PbxService', 'makeCall', userName, userID, number, callType, postValue, callback);
    };

	    /* @function
        功能     : 来电应答
        参数     : [IN] callID    : 来电通话ID
					[IN] callType  : 应答类型
						VOICE_CALL, // 语音应答
						VIDEO_CALL  // 视频应答
		*/
    ucClient.PbxService.answerCall = function(callID, callType, callback) {
        return CallNativeMethodAync('PbxService', 'answerCall', callID, callType, callback);
    };

	    /* @function
        功能     : 拒接来电
        参数     : [IN] callID    : 来电通话ID
		*/
    ucClient.PbxService.rejectCall = function(callID, callback) {
        return CallNativeMethodAync('PbxService', 'rejectCall', callID, callback);
    };

	    /* @function
        功能     : 挂断通话
        参数     : [IN] callID    : 通话ID
		*/
    ucClient.PbxService.hangupCall = function(callID, callback) {
        return CallNativeMethodAync('PbxService', 'hangupCall', callID, callback);
    };

	    /* @function
        功能     : 通话保持
        参数     : [IN] callID    : 通话ID
		*/
    ucClient.PbxService.holdCall = function(callID, callback) {
        return CallNativeMethodAync('PbxService', 'logoutPbx', callID, callback);
    };

	    /* @function
        功能     : 解除通话保持
        参数     : [IN] callID    : 通话ID
		*/
    ucClient.PbxService.unHoldCall = function(callID, callback) {
        return CallNativeMethodAync('PbxService', 'unHoldCall', callID, callback);
    };

	    /* @function
        功能     : 通话静音
        参数     : [IN] callID    : 通话ID
		*/
    ucClient.PbxService.muteCall = function(callID, callback) {
        return CallNativeMethodAync('PbxService', 'muteCall', callID, callback);
    };

	    /* @function
        功能     : 解除通话静音
        参数     : [IN] callID    : 通话ID
		*/
    ucClient.PbxService.unMuteCall = function(callID, callback) {
        return CallNativeMethodAync('PbxService', 'unMuteCall', callID, callback);
    };

	    /* @function
        功能     : 调整通话麦克风音量
        参数     : [IN] callID    : 通话ID
        [IN] level     : 麦克风音量大小 0 - 40
		*/
    ucClient.PbxService.adjustMicLevel = function(callID, level, callback) {
        return CallNativeMethodAync('PbxService', 'adjustMicLevel', callID, level, callback);
    };

	    /* @function
        功能     : 刷新麦克风
        参数     : [OUT] RET
		*/
    ucClient.PbxService.refreshMic = function(callback) {
        return CallNativeMethodAync('PbxService', 'refreshMic', callback);
    };

	    /* @function // 通话中，按键0-9*#
        功能     : 发送DTMF
        参数     : [IN] callID    : 通话ID
					[IN] dtmf      : DTMF值
		*/
    ucClient.PbxService.sendDTMF = function(callID, dtmf, callback) {
        return CallNativeMethodAync('PbxService', 'sendDTMF', callID, dtmf, callback);
    };

	    /* @function
        功能     : 获取当前PBX号码
        参数     : [OUT] pbxAccount    : 当前登录的PBX号码
		*/
    ucClient.PbxService.getCurrentPBXAccount = function(callback) {
        return CallNativeMethodAync('PbxService', 'getCurrentPBXAccount', callback);
    };
    
	    /* @function
        功能     : 呼入电话用户名
        参数     : [IN] callID    : 通话ID
                    [IN] name      : 呼入电话的用户名，如果未知，可以不调用此接口
		*/
    ucClient.PbxService.incomingCallUserName = function(callID, name, callback) {
        return CallNativeMethodAync('PbxService', 'incomingCallUserName', callID, name, callback);
    };
    
	    /* @function
        功能     : 获取通话记录列表
        参数     : [IN] timeStamp : 时间戳单位s,  0 表示获取最新的记录
                    [IN] count    : 需要获取的条数
		*/
    ucClient.PbxService.getCallRecordLst = function(count, timeStamp, callback) {
        return CallNativeMethodAync('PbxService', 'getCallRecordLst', count, timeStamp, callback);
    };
    
	    /* @function
        功能     : 获取通话详情列表
        参数     : [IN] condition    : 查询条件
                    [IN] timeStamp   : 时间戳单位s,  0 表示获取最新的记录
                    [IN] count       : 需要获取的条数
		*/
    ucClient.PbxService.getCallDetailsLst = function(number, userID, timeStamp, count, callback) {
        return CallNativeMethodAync('PbxService', 'getCallDetailsLst', number, userID, timeStamp, count, callback);
    };
    
	    /* @function
        功能     : 通话记录匹配搜索
        参数     : [IN] keyWord      : 查询条件
                    [IN] timeStamp   : 时间戳单位s,  0 表示获取最新的记录
                    [IN] count       : 需要获取的条数
		*/
    ucClient.PbxService.searchCallRecord = function(keyWord, timeStamp, count, callback) {
        return CallNativeMethodAync('PbxService', 'searchCallRecord', keyWord, timeStamp, count, callback);
    };
    
	    /* @function
        功能     : 保存通话详情
        参数     : [IN] timeStamp  : 时间戳单位s,  必须有
                    [IN] taskid     : 通话标识 必须有
                    [IN] number     : 号码 必须有
                    [IN] userID     : 用户id
                    [IN] incoming   : 呼出为0，呼入为1
                    [IN] type       : 呼叫蜜蜂此值为5
                    [IN] state      : 通话状态
                    [IN] druation   : 通话时长
		*/
    ucClient.PbxService.saveCallDetails = function(timestamp, taskid, number, userid, incoming, type, state, druation, callback) {
        return CallNativeMethodAync('PbxService', 'saveCallDetails', timestamp, taskid, number, userid, incoming, type, state, druation, callback);
    };

	    /* @function
        功能     : 双向呼叫
        参数     : [IN] toNumber    : 被叫号码
					[IN] fromNumber  : 主叫号码
					[IN] openRecord  : 开启录音
		*/
    ucClient.PbxService.makeTwoWayCall = function(toNumber, fromNumber, openRecord, callback) {
        return CallNativeMethodAync('PbxService', 'makeTwoWayCall', toNumber, fromNumber, openRecord, callback);
    };

	    /* @function
        功能     : 通话中开始或停止录音
        参数     : [IN] recordSwitch : 开始或停止录音
						RECORD_START, // 开始录音
						RECORD_STOP   // 停止录音
					[IN] callUUID     : 通话的UUID，可从状态变化获取
		*/
    ucClient.PbxService.switchRecordInCall = function(recordSwitch, callUUID, callback) {
        return CallNativeMethodAync('PbxService', 'switchRecordInCall', recordSwitch, callUUID, callback);
    };

	    /* @function
        功能     : 获取所有录音列表目录
        参数     : [IN] recordType  : 录音类型
						RECORD_TYPE_FROM_CALL_START = 0, // 通话一开始就录音
						RECORD_TYPE_IN_CALL  // 通话中的录音
		*/
    ucClient.PbxService.getAllRecord = function(recordType, callback) {
        return CallNativeMethodAync('PbxService', 'getAllRecord', recordType, callback);
    };

	    /* @function
        功能     : 获取单次通话录音
        参数     : [IN] recordType  : 录音类型
						RECORD_TYPE_FROM_CALL_START = 0, // 通话一开始就录音
						RECORD_TYPE_IN_CALL  // 通话中的录音
					[IN] callUUID     : 录音ID， 可以从获取所有录音里取出，也可以在通话结束后
						使用通话状态回调的callUUID
		*/
    ucClient.PbxService.getRecord = function(recordType, callUUID, callback) {
        return CallNativeMethodAync('PbxService', 'getRecord', recordType, callUUID, callback);
    };
	
	// -------------------------------------------------------------------------------
    // ----------------------------Microblog Service接口------------------------------
	// -------------------------------------------------------------------------------
    
    ucClient.MicroblogService.getRelation = function(userId, callback) {
        return CallNativeMethodAync('MicroblogService', 'GetRelation', userId, callback);
    };

    ucClient.MicroblogService.getRelationSync = function(userId) {
        var request = ucClient.xmlHttpRequest('MicroblogService', 'GetRelationSync', userId);
        if (request.status == 200) {
            return parseInt(request.responseText);
        } else {
            return undefined;
        }
    };
    
    ucClient.MicroblogService.getHomePageURL = function(userId, callback) {
        return CallNativeMethodAync('MicroblogService', 'GetHomePageURL', userId, callback);
    };

    ucClient.MicroblogService.getUserhomePageURL = function(selfId,userId,callback){
        return CallNativeMethodAync('MicroblogService','GetUserHomePageURL',selfId,userId,callback);
    };

    ucClient.MicroblogService.getNewMsgCount = function() {
        return CallNativeMethodAync('MicroblogService', 'GetNewMsgCount');
    };

    ucClient.MicroblogService.addAttention = function(userId) {
        return CallNativeMethodAync('MicroblogService', 'AddAttention', userId);
    };

    ucClient.MicroblogService.cancelAttention = function(userId) {
        return CallNativeMethodAync('MicroblogService', 'CancelAttention', userId);
    };

    ucClient.MicroblogService.shareMeetingRecording = function(urlVideo, urlThumb, fileName, fileSize, voiceTime, comments, callback) {
        return CallNativeMethodAync('MicroblogService', 'ShareMeetingRecording', urlVideo, urlThumb, fileName, fileSize.toString(), voiceTime, comments, callback);
    };

    // -------------------------------------------------------------------------------
    // ----------------------------CatchScreen Service接口----------------------------
    // -------------------------------------------------------------------------------
    /*
     *	截屏功能
     */
    ucClient.CatchScreenService.startCatchScreen = function(path, callback) {
        return CallNativeMethodAync('CatchScreenService', 'StartCatchScreen', path, callback);
     };



    // -------------------------------------------------------------------------------
    //----------------------------System接口------------------------------------------
    // -------------------------------------------------------------------------------
    /**
     *	获取黏贴板中Files
     *  类型有：
     *  CF_TEXT: 文本； CF_HTML: HTML格式; CF_HDROP: 文件； CF_BITMAP: 图片，给的是图片路径
     */
    ucClient.System.getClipboardData = function() {
		var request = ucClient.xmlHttpRequest('System', 'getClipboardData');
		if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
	};

	 /**
      *	往系统剪贴板塞CF_HTML数据
      */
    ucClient.System.setClipboardData = function(filePath) {
        var request = ucClient.xmlHttpRequest('System', 'setClipboardData', filePath);
        if (request.status == 200) {
            return true;
        }
        else{
            return false;
        }
    };
    
    /**
     *	check if Paste item in menu is available
     */
    ucClient.System.hasValidClipboardData = function() {
		var request = ucClient.xmlHttpRequest('System', 'hasValidClipboardData');
		if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
	};

    /**
     * 播放声音
     * @param filepath: 声音文件路径，文件为amr格式
     * @Id: id, 字符串. callback中会包含此id
     * @loop : 是否循环播放
     * @callback: 声音播放完毕后回调函数，参数为id
     * 注： 1：同一时间只能播放一个amr文件；
     *      2：调用playSound，如果前一个playSound没有调用对应的stopSound, 则会先调用前一个的callback给js
     */
    ucClient.System.playSound = function(filePath, id, loop,callback) {
        return CallNativeMethodAync('System', 'playSound', filePath, id, loop,callback);
	};

    /**
     *	停止播放声音
     *@Id: id, 字符串. playSound传入的id
     */
    ucClient.System.stopSound = function(id) {
        return CallNativeMethodAync('System', 'stopSound', id);
	};

    ucClient.System.getCurrentTimeZoneDisplay = function(callback) {
        return CallNativeMethodAync('System', 'GetCurrentTimeZoneDisplay', callback);
    };

	/**
     * 创建缩略图
     * @param path       原图片路径
     * @param width      转换后的宽度
     * @param height     转换后的高度
     */
    ucClient.System.createThumbnailsSync = function(path) {
		var request = ucClient.xmlHttpRequest('System', 'createThumbnailsSync', path);
        if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
    };
    


	// -------------------------------------------------------------------------------
	//----------------------------Util接口------------------------------------------
	// -------------------------------------------------------------------------------
	/**
     *	判断PC3.0 UI是否显示
     */
    ucClient.Util.getMeetingClientStatus = function() {
        var request = ucClient.xmlHttpRequest('Util', 'getMeetingClientStatus');
        if (request.status == 200) {
            return JSON.parse(request.responseText);
        }
        else{
            return undefined;
        }
    };



    // -------------------------------------------------------------------------------
    // ----------------------------Search Service-------------------------------------
    // -------------------------------------------------------------------------------
    /**
     *	服务器搜索
     */
    ucClient.SearchService.searchServer = function(key, page, count, scope, callback) {
        return CallNativeMethodAync('SearchService', 'SearchServer', key, page, count, scope, callback);
    };

	/**
    *	服务器搜索某个会话消息
    */
    ucClient.SearchService.searchMessage = function(type, relateId, searchKey, page, count, callback) {
        return CallNativeMethodAync('SearchService', 'SearchMessage', type, relateId, searchKey, page, count, callback);
    };

    /**
     *	本地搜索
     */
    ucClient.SearchService.searchLocal = function(key, page, count, scope, callback) {
        return CallNativeMethodAync('SearchService', 'SearchLocal', key, page, count, scope, callback);
	};
    

    // -------------------------------------------------------------------------------
    // ----------------------------Log接口---------------------------------------------
    // -------------------------------------------------------------------------------
    ucClient.Log.writeLog = function(fileName, line, logLevel, content) {
        CallNativeMethodAync('Log', 'WriteLog', fileName, line, logLevel, content);
    };



    // --------------------------------------------------------------------------------
    // ----------------------------Call Service接口-------------------------------------
    // --------------------------------------------------------------------------------
	/*
    * 根据指定ID创建呼
    * @param id    用户ID或者讨论组ID
    * @param type  0：一对一call 1：讨论组call
    * @param callback  返回是否正在上传创建结果
    */
    ucClient.AudioService.createCallById = function(id, type, callback) {
        return CallNativeMethodAync('AudioService', 'CreateCallById', id, type, callback);
    };

	/*
    * 根据用户列表创建呼
    * @param groupName   讨论组名称
    * @param userInfo    用户列表
    * @param callback    返回是否正在上传创建结果
    */
    ucClient.AudioService.createCallByUserList = function(groupName, userInfo, callback) {
        return CallNativeMethodAync('AudioService', 'CreateCallByUserList', groupName, userInfo, callback);
    };

    /*
     * 加入呼
     * @param callType   呼类型
     * @param tempConferenceId   临时会议Id
     * @param groupId   讨论组Id
     * @param callback  返回是否正在上传创建结果
     */
    ucClient.AudioService.joinCall = function(callType, tempConferenceId, groupId, callback) {
        return CallNativeMethodAync('AudioService', 'JoinCall', callType, tempConferenceId, groupId, callback);
    };

    /*
     * 离开呼
     * @param tempConferenceId   临时会议Id
     * @param callback  返回是否正在上传创建结果
     */
    ucClient.AudioService.leaveCall = function(tempConferenceId, callback) {
        return CallNativeMethodAync('AudioService', 'LeaveCall', tempConferenceId, callback);
    };

    ucClient.AudioService.reconnectConference = function(callback) {
        return CallNativeMethodAync('AudioService', 'ReconnectConference', callback);
    };

    ucClient.AudioService.switchToPhone = function(phoneNumber, callback) {
        return CallNativeMethodAync('AudioService', 'SwitchToPhone', phoneNumber, callback);
    };

    ucClient.AudioService.switchToVOIP = function(callback) {
        return CallNativeMethodAync('AudioService', 'SwitchToVOIP', callback);
    };


    ucClient.AudioService.muteSelf = function(callback) {
        return CallNativeMethodAync('AudioService', 'MuteSelf', callback);
    };

    ucClient.AudioService.unMuteSelf = function(callback) {
	    return CallNativeMethodAync('AudioService', 'UnMuteSelf', callback);
    };

    ucClient.AudioService.hangupPhone = function(callback) {
        return CallNativeMethodAync('AudioService', 'HangupPhone', callback);
    };

    ucClient.AudioService.inviteUsersToSingleCall = function(callType, groupName, userInfo, conferenceId, callback) {
        return CallNativeMethodAync('AudioService', 'InviteUsersToSingleCall', callType, groupName, userInfo, conferenceId, callback);
    };

    ucClient.AudioService.inviteUsersToGroupCall = function(callType, userInfo, groupId, conferenceId, callback) {
        return CallNativeMethodAync('AudioService', 'InviteUsersToGroupCall', callType, userInfo, groupId, conferenceId, callback);
    };

    ucClient.AudioService.getUserCallStatus = function(userIds, id, type, callback) {
        return CallNativeMethodAync('AudioService', 'GetUserCallStatus', userIds, id, type, callback);
    };

    ucClient.AudioService.getHistoryPhoneNum = function(callback) {
        return CallNativeMethodAync('AudioService', 'GetHistoryPhoneNum', callback);
    };

    ucClient.AudioService.getLastAudioInvite = function(userId, callback) {
        return CallNativeMethodAync('AudioService', 'GetLastAudioInvite', userId, callback);
    };

    ucClient.AudioService.getSpeakerNum = function() {
        var request = ucClient.xmlHttpRequest('AudioService', 'GetSpeakerNum');
        if (request.status == 200) {
            return request.responseText;
        } else {
            return undefined;
        }
    }

    ucClient.AudioService.getMicrophoneNum = function() {
        var request = ucClient.xmlHttpRequest('AudioService', 'GetMicrophoneNum');
        if (request.status == 200) {
            return request.responseText;
        } else {
            return undefined;
        }
    }

    ucClient.AudioService.getSpeakerList = function() {
        var request = ucClient.xmlHttpRequest('AudioService', 'GetSpeakerList');
        if (request.status == 200) {
            return request.responseText;
        } else {
            return undefined;
        }
    };

    ucClient.AudioService.getMicrophoneList = function() {
        var request = ucClient.xmlHttpRequest('AudioService', 'GetMicrophoneList');
        if (request.status == 200) {
            return request.responseText;
        } else {
            return undefined;
        }
    };

    ucClient.AudioService.startSpeakerDiagnosis = function(speaker, callback) {
        return CallNativeMethodAync('AudioService', 'StartSpeakerDiagnosis', speaker, callback);
    };

    ucClient.AudioService.startMikeDiagnosis = function(mike, callback) {
        return CallNativeMethodAync('AudioService', 'StartMikeDiagnosis', mike, callback);
    };

    ucClient.AudioService.stopSpeakerDiagnosis = function(callback) {
        return CallNativeMethodAync('AudioService', 'StopSpeakerDiagnosis', callback);
    };

    ucClient.AudioService.stopMikeDiagnosis = function(callback) {
        return CallNativeMethodAync('AudioService', 'StopMikeDiagnosis', callback);
    };

    ucClient.AudioService.getCameraList = function() {
        var request = ucClient.xmlHttpRequest('AudioService', 'GetCameraList');
        if (request.status == 200) {
            return request.responseText;
        } else {
            return undefined;
        }
    };

    ucClient.AudioService.cameraDiagnosis = function(camera, hwnd) {
        var request = ucClient.xmlHttpRequest('AudioService', 'CameraDiagnosis', camera, hwnd);
        if (request.status == 200) {
            return request.responseText;
        } else {
            return undefined;
        }
    };

    ucClient.AudioService.stopCameraDiagnosis = function() {
        var request = ucClient.xmlHttpRequest('AudioService', 'StopCameraDiagnosis');
        if (request.status == 200) {
            return request.responseText;
        } else {
            return undefined;
        }
    };

    ucClient.SharingService.startDesktopShare = function(callback) {
        return CallNativeMethodAync('SharingService', 'StartDesktopShare', callback);
    };

    ucClient.SharingService.stopDesktopShare = function(callback) {
        return CallNativeMethodAync('SharingService', 'StopDesktopShare', callback);
    };

    ucClient.SharingService.startDesktopView = function(sharingName, callback) {
        return CallNativeMethodAync('SharingService', 'StartDesktopView', sharingName, callback);
    };

    ucClient.SharingService.stopDesktopView = function(callback) {
        return CallNativeMethodAync('SharingService', 'StopDesktopView', callback);
    };

	// --------------------------------------------------------------------------------
	// ----------------------------App接口-------------------------------------
	// -------------------------------------------------------------------------------
	ucClient.App.enableAutoStart = function(callback) {
	    return CallNativeMethodAync('App', 'EnableAutoStart', callback);
	};

	ucClient.App.disableAutoStart = function(callback) {
	    return CallNativeMethodAync('App', 'DisableAutoStart', callback);
	};

	ucClient.App.isAutoStart = function(callback) {
	    return CallNativeMethodAync('App', 'IsAutoStart', callback);
	};
	
	ucClient.App.getClientVersion = function(callback) {
	    return CallNativeMethodAync('App', 'GetClientVersion', callback);
	};


	// --------------------------------------------------------------------------------
	// ----------------------------OpenApiService接口-------------------------------------
	// -------------------------------------------------------------------------------
	/**
    * 获取第三方应用列表接口;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码
    */
	ucClient.OpenApiService.getAppList = function(callback) {
	    return CallNativeMethodAync('OpenApiService', 'GetAppList', callback);
	};

   /**
    * 获取第三方应用信息接口;
    * @param appId 请求目标ID(应用ID);
    * @param callback 操作结束后的回调函数。 callback[0]-错误码
    */
	ucClient.OpenApiService.getAppInfo = function(appId, callback) {
	    return CallNativeMethodAync('OpenApiService', 'GetAppInfo', appId, callback);
	};

    /**
    * 获取历史聊天内容接口;
    * @param appId 请求目标ID(应用ID);
    * @param count 请求聊天内容数量，默认20，不能超过200;
    * @param startTime 请求聊天内容的开始时间;
    * @param endTime 请求聊天内容的结束时间;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码
    */
	ucClient.OpenApiService.getChatHistory = function(appId, count, startTime, endTime, callback) {
	    return CallNativeMethodAync('OpenApiService', 'GetChatHistory', appId, count, startTime, endTime, callback);
	};

   /**
    * 获取OA消息状态接口;
    * @param OAMessageInfos 请求OA消息列表;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码
    */
	ucClient.OpenApiService.getOAMessageStatus = function(OAMessageInfos, callback) {
	    return CallNativeMethodAync('OpenApiService', 'GetOAMessageStatus', OAMessageInfos, callback);
	};

   /**
    * 获取消息ACK状态接口;
    * @param messageInfos 请求消息列表;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码
    */
	ucClient.OpenApiService.getMessageAckStatus = function(messageInfos, callback) {
	    return CallNativeMethodAync('OpenApiService', 'GetMessageAckStatus', messageInfos, callback);
	};

    /**
     * 修改OA消息状态接口;
     * @param OAMessageInfos OA消息状态列表;
     * @param callback 操作结束后的回调函数。 callback[0]-错误码
     */
	ucClient.OpenApiService.updateOAMessageStatus = function(OAMessageStatusInfos, callback) {
	    return CallNativeMethodAync('OpenApiService', 'UpdateOAMessageStatus', OAMessageStatusInfos, callback);
	};

	/**
    * 重发消息;
    * @param msgId 消息id
    * @param callback 操作结束后的回调函数。 callback[0]-错误码，callback[1]-msgId
    */
	ucClient.OpenApiService.resendMessage = function(msgId, callback) {
	    return CallNativeMethodAync('OpenApiService', 'ResendMessage', msgId, callback);
	};

	/**
     * 发送聊天文本消息接口;
     * @param appJid	    应用ID;
     * @param textTitle     文本标题;
     * @param textContent   文本内容;
     * @param textType      文本格式：0x01:文本类型，0x02:URL链接类型;
     * @param detailContent 文本详情信息;
     * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.OpenApiService.sendTextMessage = function(appJid, textTitle, textContent, textType, detailContent, callback) {
	    return CallNativeMethodAync('OpenApiService', 'SendTextMessage', appJid, textTitle, textContent, textType, detailContent, callback);
	};

	/**
     * 发送聊天图片消息接口;
     * @param appJid	    应用ID;
     * @param imageId       图片ID或图片URL
     * @param imageType     图片来源类型：0x01:FS文件ID，0x02:URL链接类型;
     * @param imagePath     本地图片路径;
     * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.OpenApiService.sendImageMessage = function(appJid, imageId, imageType, imagePath, callback) {
	    return CallNativeMethodAync('OpenApiService', 'SendImageMessage', appJid, imageId, imageType, imagePath, callback);
	};

	/**
     * 发送文件图片消息接口;
     * @param appJid	    应用ID;
     * @param fileId	    文件ID或文件URL
     * @param fileType	    文件来源类型：0x01:FS文件ID，0x02:URL链接类型;
     * @param fileName	    文件名称;
     * @param fileSize	    文件大小;
     * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
     */
	ucClient.OpenApiService.sendFileMessage = function(appJid, fileId, fileType, fileName, fileSize, callback) {
	    return CallNativeMethodAync('OpenApiService', 'SendFileMessage', appJid, fileId, fileType, fileName, fileSize.toString(), callback);
	};

	/**
     * 发送消息已读接口
     * @param jid 应用jid
     * @param msgId 本地消息ID
     * @param msgSeq 服务器消息ID
     * @param callback 操作结束后的回调函数。 callback[0]-错误码
     */
	ucClient.OpenApiService.sendMessageRead = function(jid, msgId, msgSeq, callback) {
	    return CallNativeMethodAync('OpenApiService', 'SendMessageRead', jid, msgId, msgSeq, callback);
	};

	/**
     * 发送多条消息已读接口
     * @param toAppId  应用ID
     * @param callback 操作结束后的回调函数。 callback[0]-错误码
     */
	ucClient.OpenApiService.sendMessagesReadById = function(toAppId, callback) {
	    return CallNativeMethodAync('OpenApiService', 'SendMessagesReadById', toAppId, callback);
	};

   /**
    * 发送HttpRequest请求接口;
	* @param method         http请求方法;
	* @param url	        http请求Url;
    * @param body	        Post body
	* @param heads	        http请求头协议列表;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
    */
	ucClient.OpenApiService.sendRequest = function(method, url, body, head, callback) {
	    return CallNativeMethodAync('OpenApiService', 'SendRequest', method, url, body, head, callback);
	};

	/**
    * 发送BBS请求接口;
    * @param method         request parameter json;
      @param callback       操作结束后的回调函数, callback[0]-requestId
    */
	ucClient.BBSService.BBSRequest = function(bbsRequestJson, callback) {
	    return CallNativeMethodAync('BBSService', 'BBSRequest', bbsRequestJson, callback);
	};

	// -------------------------------------------------------------------------------
	// ----------------------------Update Service接口----------------------------
	// -------------------------------------------------------------------------------
	/*
    *	升级检测
    */
	ucClient.UpdateService.checkUpdates = function(callback) {
	    return CallNativeMethodAync('UpdateService', 'CheckUpdates', callback);
	};

	/*
    *	升级操作
    */
	ucClient.UpdateService.update = function() {
	    return CallNativeMethodAync('UpdateService', 'Update');
	};

	/*
    *	获取本地版本号
    */
	ucClient.UpdateService.getLocalVersion = function() {
	    var request = ucClient.xmlHttpRequest('UpdateService', 'GetLocalVersion');
	    if (request.status == 200) {
	        return request.responseText;
	    } else {
	        return undefined;
	    }
	}

    // 获取文件默认保存路径
	ucClient.ClientService.getDefaultFileSavePath = function(filename) {
	    var request = ucClient.xmlHttpRequest('ClientService', 'GetDefaultFileSavePath',filename);
	    if (request.status == 200) {
	        return request.responseText;
	    }
	    else {
	        return undefined;
	    }
	};

	// 获取小讨论组成员个数上限
	ucClient.ClientService.getMinGroupMemberLimit = function() {
	    var request = ucClient.xmlHttpRequest('ClientService', 'GetMinGroupMemberLimit');
	    if (request.status == 200) {
	        return request.responseText;
	    }
	    else {
	        return undefined;
	    }
	};

	// 获取大讨论组成员个数上限
	ucClient.ClientService.getMaxGroupMemberLimit = function() {
	    var request = ucClient.xmlHttpRequest('ClientService', 'GetMaxGroupMemberLimit');
	    if (request.status == 200) {
	        return request.responseText;
	    }
	    else {
	        return undefined;
	    }
	};

	// 获取讨论组个数上限
	ucClient.ClientService.getGroupCountLimit = function() {
	    var request = ucClient.xmlHttpRequest('ClientService', 'GetGroupCountLimit');
	    if (request.status == 200) {
	        return request.responseText;
	    }
	    else {
	        return undefined;
	    }
	};

	// -------------------------------------------------------------------------------
	// ----------------------------External Service接口----------------------------
	// -------------------------------------------------------------------------------

   /**
    * 获取历史聊天内容接口;
    * @param type       聊天类型;
    * @param externalId 请求目标ID(P2P:用户账号/讨论组:外部ID);
    * @param count 请求聊天内容数量，默认20，不能超过200;
    * @param startTime 请求聊天内容的开始时间;
    * @param endTime 请求聊天内容的结束时间;
    * @param callback 操作结束后的回调函数。 callback[0]-错误码
    */
	ucClient.ExternalService.getChatHistory = function(type, externalId, count, startTime, endTime, callback) {
	    return CallNativeMethodAync('ExternalService', 'GetChatHistory', type, externalId, count, startTime, endTime, callback);
	};

   /**
	* 是否显示外部消息;
    * @param externalId 请求目标ID(P2P:用户账号/讨论组:外部ID);
	* @return					 0：不显示/1：显示;
    */
	ucClient.ExternalService.showExternalMessage = function(externalId) {
	    var request = ucClient.xmlHttpRequest('ExternalService', 'ShowExternalMessage',externalId);
	    if (request.status == 200) {
	        return request.responseText;
	    }
	    else {
	        return undefined;
	    }
	};


})();