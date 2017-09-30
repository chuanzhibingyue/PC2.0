#ifndef UC_DEFS_H_
#define UC_DEFS_H_

#include <windows.h>

// UCClient错误码
#define UCCLIENT_SUCCESS				0
#define UCCLIENT_ERROR					200
#define UCCLIENT_ARGUMENT_ERROR			201
#define UCCLIENT_MODULE_NOTFOUND		202
#define UCCLIENT_METHOD_NOTFOUND		203
#define GKCLIENT_JSON_PARSE_ERROR		204
#define CURRENT_USER_INFO_NOT_FOUND		205

// 服务器错误码
#define CLOUD_DISCUSSION_ALREADY_EXISTS				10374
#define CLOUD_DISCUSSION_NOT_EXISTS					10372

#define UCCLIENT_APP_NAME               L"全时蜜蜂"
#define UCCLIENT_CLASS_NAME             L"QUANSHI UC CLIENT CLASS"

extern DWORD  mainThreadId;
#define ON_MAIN_THREAD()				((mainThreadId == ::GetCurrentThreadId()) ? true : false)

// js事件名
//client service
#define CLIENT_SERVICE_LOGIN_EVENT									L"ClientServiceOnLogin"
#define CLIENT_SERVICE_LOGOUT_EVENT									L"ClientServiceOnLogout"
#define CLIENT_SERVICE_PASSWORD_MODIFIED_EVENT						L"ClientServiceOnPasswordModified"
#define CLIENT_SERVICE_PASSWORD_MODIFY_MESSAGE_RECEIVED_EVENT		L"ClientServiceOnPasswordModifyMessageReceived"
#define CLIENT_SERVICE_PASSWORD_RULE_UPDATE_MESSAGE_RECEIVED_EVENT	L"ClientServiceOnPasswordRuleUpdateMessageReceived"
#define CLIENT_SERVICE_PASSWORD_EXPIRED_MESSAGE_RECEIVED_EVENT		L"ClientServiceOnPasswordExpiredMessageReceived"
#define CLIENT_SERVICE_UCAS_CONNECTED_EVENT							L"ClientServiceOnUCASConnected"
#define CLIENT_SERVICE_UCAS_DISCONNECTED_EVENT						L"ClientServiceOnUCASDisconnected"
#define CLIENT_SERVICE_CONVERSATION_LIST_RECEIVED_EVENT				L"ClientServiceOnConversationListReceived"
#define CLIENT_SERVICE_CONVERSATION_LIST_COMPLETE_EVENT			    L"ClientServiceOnConversationListComplete"
#define CLIENT_SERVICE_CONVERSATION_TOP_SET_EVENT					L"ClientServiceOnConversationTopSet"
#define ClIENT_SERVICE_USER_LOGIN_NOTIFY_EVENT                      L"UserLoginMessageNotify"
#define CLIENT_SERVICE_ACCOUNT_VERIFIED_EVENT						L"ClientServiceOnAccountVerified"
#define CLIENT_SERVICE_CODE_VERIFIED_EVENT							L"ClientServiceOnCodeVerified"
#define ClIENT_SERVICE_CODE_RESEND_EVENT							L"ClientServiceOnCodeResent"
#define CLIENT_SERVICE_PASSWORD_RESET_EVENT							L"ClientServiceOnPasswordReset"
#define CLIENT_SERVICE_UPLOAD_LOG_EVENT								L"ClientServiceOnLogUploaded"
#define CLIENT_SERVICE_SESSION_EXPIRED_EVENT						L"ClientServiceOnSessionExpired"
#define CLIENT_SERVICE_ARCHIVELOG_FINISH_EVENT					L"ClientServiceOnArchiveLogFinish"
#define UPDATE_USER_NOTIFY_SET_EVENT                                L"UpdateUserNotifySet"
#define USER_NOTIFY_STATUS_EVENT                                    L"UserNotifyStatusGet"
// contact service
#define CONTACT_SERVICE_CONTACT_LOADED_EVENT						L"ContactServiceOnContactLoaded"
#define CONTACT_SERVICE_USER_STATUS_RECEIVED_EVENT			   	    L"ContactServiceOnUserStatusReceived"
#define CONTACT_SERVICE_GET_USER_STATUS_RESPONSE_EVENT		L"ContactServiceOnGetUserStatusResponse"
#define CONTACT_SERVICE_USER_STATUS_SET_EVENT				        L"ContactServiceOnUserStatusSet"
#define CONTACT_SERVICE_CONTACT_INFO_RECEIVED_EVENT					L"ContactServiceOnContactInfoReceived"
#define CONTACT_SERVICE_BASIC_CONTACT_INFO_RECEIVED_EVENT			L"ContactServiceOnBasicContactInfoReceived"
#define CONTACT_SERVICE_CONTACT_ADDED_EVENT							L"ContactServiceOnContactAdded"
#define CONTACT_SERVICE_CONTACT_DELETED_EVENT						L"ContactServiceOnContactDeleted"
#define CONTACT_SERVICE_CONTACT_UPDATED_EVENT						L"ContactServiceOnContactUpdated"
#define CONTACT_SERVICE_USER_INFO_UPDATED_EVENT						L"ContactServiceOnUserInfoUpdated"
#define CONTACT_SERVICE_USER_TAGS_RECEIVED_EVENT					L"ContactServiceOnUserTagsReceived"		
#define CONTACT_SERVICE_USER_TAGS_UPDATED_EVENT						L"ContactServiceOnUserTagsUpdated"
#define CONTACT_SERVICE_SYSTEM_TAGS_ADDED_EVENT						L"ContactServiceOnSystemTagsAdded"
#define CONTACT_SERVICE_SYSTEM_TAGS_UPDATED_EVENT					L"ContactServiceOnSystemTagsUpdated"
#define CONTACT_SERVICE_SYSTEM_TAGS_DELETED_EVENT					L"ContactServiceOnSystemTagsDeleted"
#define CONTACT_SERVICE_COLLEAGUE_RECEIVED_EVENT					L"ContactServiceOnColleagueReceived"
#define CONTACT_SERVICE_ORG_ADDRESS_BOOK_RECEIVED_EVENT				L"ContactServiceOnOrgAddressBookReceived"
#define CONTACT_SERVICE_ORG_MEMBER_RECEIVED_EVENT					L"ContactServiceOnOrgMembersReceived"

#define CONTACT_SERVICE_DEPT_UPDATED_EVENT							L"ContactServiceOnDeptUpdated"
#define CONTACT_SERVICE_DEPT_TRANSFERED_EVENT						L"ContactServiceOnDeptTransfered"
#define CONTACT_SERVICE_POSITION_UPDATED_EVENT						L"ContactServiceOnPositionUpdated"
#define CONTACT_SERVICE_EMPLOYEE_ENTRY_EVENT						L"ContactServiceOnEmployeeEntry"
#define CONTACT_SERVICE_EMPLOYEE_LEAVED_EVENT						L"ContactServiceOnEmployeeLeaved"
#define CONTACT_SERVICE_EMPLOYEE_RIGHTS_UPDATED_EVENT				L"ContactServiceOnEmployeeRightsUpdated"
#define CONTACT_SERVICE_DEPT_PARENT_CHANGED_EVENT					L"ContactServiceOnDeptParentChanged"
#define CONTACT_SERVICE_COMPANY_JOINED_EVENT						L"ContactServiceOnCompanyJoined"  
#define CONTACT_SERVICE_DEPT_DELETED_EVENT							L"ContactServiceOnDeptDeleted"  
#define CONTACT_SERVICE_EMPLOYEE_ENTRY_CONFIRMED_EVENT				L"ContactServiceOnEmployeeEntryConfirmed"  
#define CONTACT_SERVICE_EMPLOYEE_LEAVE_CONFIRMED_EVENT				L"ContactServiceOnEmployeeLeaveConfirmed"  
#define CONTACT_SERVICE_DEPT_TRANSFER_CONFIRMED_EVENT				L"ContactServiceOnDeptTransferConfirmed"  

//im service
#define IM_SERVICE_CHAT_HISTORY_RECEIVED_EVENT						L"IMServiceOnChatHistoryReceived"
#define IM_SERVICE_MESSAGE_SENT_EVENT								L"IMServiceOnMessageSent"
#define IM_SERVICE_TEXT_MESSAGE_RECEIVED_EVENT						L"IMServiceOnTextMessageReceived"
#define IM_SERVICE_IMAGE_RECEIVED_EVENT								L"IMServiceOnImageReceived"
#define IM_SERVICE_AUDIO_MESSAGE_RECEIVED_EVENT						L"IMServiceOnAudioMessageReceived"
#define IM_SERVICE_VIDEO_MESSAGE_RECEIVED_EVENT						L"IMServiceOnVideoMessageReceived"
#define IM_SERVICE_REPORT_MESSAGE_RECEIVED_EVENT					L"IMServiceOnReportMessageReceived"
#define IM_SERVICE_RECORD_VIDEO_MESSAGE_RECEIVED_EVENT				L"IMServiceOnRecordVideoMessageReceived"
#define IM_SERVICE_FILE_RECEIVED_EVENT								L"IMServiceOnFileReceived"
#define IM_SERVICE_CLOUDFILE_CREATE_RECEIVED_EVENT					L"IMServiceOnCloudFileCreateMessageReceived"
#define IM_SERVICE_CODE_RECEIVED_EVENT								L"IMServiceOnCodeReceived"
#define IM_SERVICE_FILE_DELETED_EVENT								L"IMServiceOnFileDeleted"
#define IM_SERVICE_MESSAGE_READ_EVENT								L"IMServiceOnMessageRead"
#define IM_SERVICE_MESSAGE_PLAYED_EVENT								L"IMServiceOnMessagePlayed"
#define IM_SERVICE_MESSAGE_UNREAD_USER_COUNT_EVENT		            L"IMServiceOnMessageUnreadUserCountReceived"
#define IM_SERVICE_CALL_INVITED_EVENT								L"IMServiceOnCallInvited"
#define IM_SERVICE_CALL_ACCEPTED_EVENT								L"IMServiceOnCallAccepted"
#define IM_SERVICE_CALL_REJECTED_EVENT								L"IMServiceOnCallRejected"
#define IM_SERVICE_CALL_STOPPED_EVENT								L"IMServiceOnCallStopped"
#define IM_SERVICE_CALL_CANCELLED_EVENT								L"IMServiceOnCallCancelled"
#define IM_SERVICE_CALL_NOT_RESPONDED_EVENT							L"IMServiceOnCallNotResponded"
#define IM_SERVICE_CALL_CONNECTED_EVENT								L"IMServiceOnCallConnected"

// group service
#define GROUP_SERVICE_CHAT_HISTORY_RECEIVED_EVENT					L"GroupServiceOnChatHistoryReceived"
#define GROUP_SERVICE_SEARCH_CHAT_HISTORY_RECEIVED_EVENT			L"GroupServiceOnSearchChatHistoryReceived"
#define GROUP_SERVICE_MESSAGE_SENT_EVENT							L"GroupServiceOnMessageSent"
#define GROUP_SERVICE_TEXT_MESSAGE_RECEIVED_EVENT					L"GroupServiceOnTextMessageReceived"
#define GROUP_SERVICE_IMAGE_RECEIVED_EVENT							L"GroupServiceOnImageReceived"
#define GROUP_SERVICE_AUDIO_MESSAGE_RECEIVED_EVENT					L"GroupServiceOnAudioMessageReceived"
#define GROUP_SERVICE_VIDEO_MESSAGE_RECEIVED_EVENT					L"GroupServiceOnVideoMessageReceived"
#define GROUP_SERVICE_REPORT_MESSAGE_RECEIVED_EVENT					L"GroupServiceOnReportMessageReceived"
#define GROUP_SERVICE_CLOUDFILE_MODIFIED_RECEIVED_EVENT				L"GroupServiceOnCloudFileModifiedMessageReceived"
#define GROUP_SERVICE_RECORD_VIDEO_MESSAGE_RECEIVED_EVENT			L"GroupServiceOnRecordVideoMessageReceived"
#define GROUP_SERVICE_MESSAGE_READ_EVENT							L"GroupServiceOnMessageRead"
#define GROUP_SERVICE_MESSAGE_PLAYED_EVENT							L"GroupServiceOnMessagePlayed"
#define GROUP_SERVICE_MESSAGE_UNREAD_USER_COUNT_EVENT			    L"GroupServiceOnMessageUnreadUserCountReceived"
#define GROUP_SERVICE_MESSAGE_UNREAD_USER_LIST_EVENT			    L"GroupServiceOnMessageUnreadUserListReceived"
#define GROUP_SERVICE_CALL_INVITED_EVENT							L"GroupServiceOnCallInvited"
#define GROUP_SERVICE_CALL_ACCEPTED_EVENT							L"GroupServiceOnCallAccepted"
#define GROUP_SERVICE_CALL_REJECTED_EVENT							L"GroupServiceOnCallRejected"
#define GROUP_SERVICE_CALL_STOPPED_EVENT							L"GroupServiceOnCallStopped"
#define GROUP_SERVICE_CALL_CONNECTED_EVENT							L"GroupServiceOnCallConnected"
#define GROUP_SERVICE_CALL_CANCELLED_EVENT							L"GroupServiceOnCallCancelled"
#define GROUP_SERVICE_GROUP_CREATED_EVENT							L"GroupServiceOnGroupCreated"
#define GROUP_SERVICE_GROUP_EXITED_EVENT							L"GroupServiceOnGroupExited"
#define GROUP_SERVICE_GROUP_ENDEDED_EVENT							L"GroupServiceOnGroupEnded"
#define GROUP_SERVICE_GROUP_MEMBER_ADDED_EVENT						L"GroupServiceOnMemberAdded"
#define GROUP_SERVICE_GROUP_MEMBER_REMOVED_EVENT					L"GroupServiceOnMemberRemoved"
#define GROUP_SERVICE_GROUP_NAME_UPDATED_EVENT						L"GroupServiceOnGroupNameUpdated"
#define GROUP_SERVICE_GROUP_LOGO_UPDATED_EVENT						L"GroupServiceOnGroupLogoUpdated"
#define GROUP_SERVICE_DISTURB_SET_EVENT								L"GroupServiceOnDisturbSet"
#define GROUP_SERVICE_TOP_CHAT_EVENT								L"GroupServiceOnTopChatSet"
#define GROUP_SERVICE_REMINDER_SET									L"GroupServiceOnReminderSet"
#define GROUP_SERVICE_GROUP_LIST_RECEIVED							L"GroupServiceOnGroupListReceived"
#define GROUP_SERVICE_GROUP_INFO_RECEIVED							L"GroupServiceOnGroupInfoReceived"
#define GROUP_SERVICE_DOC_LIST_RECEIVED								L"GroupServiceOnDocListReceived"
#define GROUP_SERVICE_DOC_INFO_RECEIVED								L"GroupServiceOnDocInfoReceived"
#define GROUP_SERVICE_DOC_UPLOADED_RECEIVED							L"GroupServiceOnDocumentUploaded"
#define GROUP_SERVICE_CODESNIPPET_UPLOADED_RECEIVED					L"GroupServiceOnCodeSnippetUploaded"
#define GROUP_SERVICE_DOC_DELETED									L"GroupServiceOnDocumentDeleted"
#define GROUP_SERVICE_COMMENT_LIST_RECEIVED							L"GroupServiceOnCommentListReceived"
#define GROUP_SERVICE_COMMENT_CREATED							    L"GroupServiceOnCommentCreated"
#define GROUP_SERVICE_GROUP_CREATE_MESSAGE_RECEIVED_EVENT			L"GroupServiceOnGroupCreateMessageReceived"
#define GROUP_SERVICE_GROUP_CLOSE_MESSAGE_RECEIVED_EVENT			L"GroupServiceOnGroupCloseMessageReceived"
#define GROUP_SERVICE_GROUPMEMBER_DEL_MESSAGE_RECEIVED_EVENT		L"GroupServiceOnGroupMemberDelMessageReceived"
#define GROUP_SERVICE_GROUPMEMBER_ADD_MESSAGE_RECEIVED_EVENT		L"GroupServiceOnGroupMemberAddMessageReceived"
#define GROUP_SERVICE_GROUPMEMBER_KICK_MESSAGE_RECEIVED_EVENT		L"GroupServiceOnGroupMemberKickMessageReceived"
#define GROUP_SERVICE_GROUPNAME_UPDATE_MESSAGE_RECEIVED_EVENT		L"GroupServiceOnGroupNameUpdateMessageReceived"
#define GROUP_SERVICE_GROUPDISTURB_SET_MESSAGE_RECEIVED_EVENT		L"GroupServiceOnGroupDisturbSetMessageReceived"
#define GROUP_SERVICE_GROUPTOP_MESSAGE_RECEIVED_EVENT				L"GroupServiceOnGroupTopMessageReceived"
#define GROUP_SERVICE_GROUPAVATAR_UPDATE_MESSAGE_RECEIVED			L"GroupServiceOnGroupAvatarUpdateMessageReceived"
#define GROUP_SERVICE_GROUPSTATUSINFO_UPDATE_MESSAGE_RECEIVED		L"GroupServiceOnGroupStatusInfoUpdateMessageReceived"
#define GROUP_SERVICE_GROUPREMINDER_SET_MESSAGE_RECEIVED			L"GroupServiceOnGroupReminderSetMessageReceived"
#define GROUP_SERVICE_DOCUMENTUPLOAD_MESSAGE_RECEIVED				L"GroupServiceOnDocumentUploadMessageReceived"
#define GROUP_SERVICE_CLOUDFILE_CREATE_MESSAGE_RECEIVED				L"GroupServiceOnCloudFileCreateMessageReceived"
#define GROUP_SERVICE_DOCUMENTDELETE_MESSAGE_RECEIVED				L"GroupServiceOnDocumentDeleteMessageReceived"
#define GROUP_SERVICE_CODESNIPPET_UPLOAD_MESSAGE_RECEIVED			L"GroupServiceOnCodeSnippetUploadMessageReceived"
#define GROUP_SERVICE_COMMENTCREATE_MESSAGE_RECEIVED				L"GroupServiceOnCommentCreateMessageReceived"
#define GROUP_SERVICE_MEMBER_ROLE_UPDATED_EVENT						L"GroupServiceOnMemberRoleUpdated"
#define GROUP_SERVICE_AT_MESSAGE_LIST_RECEIVED						L"GroupServiceOnAtMessageListReceived"
#define GROUP_SERVICE_AT_MESSAGE_DELETED							L"GroupServiceOnAtMessageDeleted"

//所有消息撤回均触发此事件
#define GROUP_SERVICE_MESSAGE_REVOCATION_EVENT						L"GroupServiceOnRevocationMesssageReceived"
#define GROUP_SERVICE_MSG_REVOCATION_EVENT                          L"GroupServiceOnRevocationMessage"


//Calendar Service
#define CALENDAR_SERVICE_MEETING_CREATED_EVENT						L"CalendarServiceOnMeetingCreated"
#define CALENDAR_SERVICE_MEETING_UPDATED_EVENT						L"CalendarServiceOnMeetingUpdated"
#define CALENDAR_SERVICE_MEETING_CANCELED_EVENT						L"CalendarServiceOnMeetingCanceled"
#define CALENDAR_SERVICE_MEETING_STARTURL_RECEIVED_EVENT			L"CalendarServiceOnMeetingStartURLReceived"
#define CALENDAR_SERVICE_MEETING_WEBSTARTURL_RECEIVED_EVENT			L"CalendarServiceOnMeetingWebStartURLReceived"
#define CALENDAR_SERVICE_ACCREDIT_SETTING_UPDATED_EVENT				L"CalendarServiceOnAccreditSettingUpdated"
#define CALENDAR_SERVICE_ACCREDIT_USERLIST_RECEIVED_EVENT			L"CalendarServiceOnAccreditUserListReceived"
#define CALENDAR_SERVICE_MEETING_LIST_RECEIVED_EVENT				L"CalendarServiceOnMeetingListReceived"
#define CALENDAR_SERVICE_BRIEF_MEETING_LIST_RECEIVED_EVENT			L"CalendarServiceOnBriefMeetingListReceived"
#define CALENDAR_SERVICE_BRIEF_CHANGED_MEETING_LIST_RECEIVED_EVENT	L"CalendarServiceOnBriefChangedMeetingListReceived"
#define CALENDAR_SERVICE_MEETING_INFO_RECEIVED_BY_ID_EVENT			L"CalendarServiceOnMeetingInfoReceivedById"
#define CALENDAR_SERVICE_MEETING_INFO_RECEIVED_BY_IDS_EVENT			L"CalendarServiceOnMeetingInfoReceivedByIds"
#define CALENDAR_SERVICE_MEETING_INVITATION_ACCEPTED_EVENT			L"CalendarServiceOnMeetingInvitationAccepted"
#define CALENDAR_SERVICE_MEETING_INVITATION_DENIED_EVENT			L"CalendarServiceOnMeetingInvitationDenied"
#define CALENDAR_SERVICE_MEETING_FORWARDED_EVENT					L"CalendarServiceOnMeetingForwarded"
#define CALENDAR_SERVICE_USER_SCHEDULE_RECEIVED_EVENT				L"CalendarServiceOnUserScheduleReceived"
#define CALENDAR_SERVICE_USER_BUSYFREE_INFO_RECEIVED_EVENT			L"CalendarServiceOnUserBusyFreeInfoReceived"
#define CALENDAR_SERVICE_MINUTES_CREATED_EVENT						L"CalendarServiceOnMinutesCreated"
#define CALENDAR_SERVICE_MINUTES_UPDATED_EVENT						L"CalendarServiceOnMinutesUpdated"
#define CALENDAR_SERVICE_MINUTES_DETAILS_RECEIVED_EVENT				L"CalendarServiceOnMinutesDetailsReceived"
#define CALENDAR_SERVICE_MINUTES_LIST_RECEIVED_EVENT				L"CalendarServiceOnMinutesListReceived"
#define CALENDAR_SERVICE_MEETING_RECORDING_DELETED_EVENT			L"CalendarServiceOnMeetingRecordingDeleted"
#define CALENDAR_SERVICE_MEETING_FILE_UPLOADED_EVENT				L"CalendarServiceOnMeetingFileUploaded"
#define CALENDAR_SERVICE_MEETING_FILE_DELETED_EVENT					L"CalendarServiceOnMeetingFileDeleted"
#define CALENDAR_SERVICE_MESSAGE_SENT_EVENT							L"CalendarServiceOnMessageSent"
#define CALENDAR_SERVICE_TEXT_MESSAGE_RECEIVED_EVENT				L"CalendarServiceOnTextMessageReceived"
#define CALENDAR_SERVICE_IMAGE_MESSAGE_RECEIVED_EVENT				L"CalendarServiceOnImageMessageReceived"
#define CALENDAR_SERVICE_AUDIO_MESSAGE_RECEIVED_EVENT				L"CalendarServiceOnAudioMessageReceived"
#define CALENDAR_SERVICE_VIDEO_MESSAGE_RECEIVED_EVENT				L"CalendarServiceOnVideoMessageReceived"
#define CALENDAR_SERVICE_REPORT_MESSAGE_RECEIVED_EVENT				L"CalendarServiceOnReportMessageReceived"
#define CALENDAR_SERVICE_RECORD_VIDEO_MESSAGE_RECEIVED_EVENT		L"CalendarServiceOnRecordVideoMessageReceived"
#define CALENDAR_SERVICE_MESSAGE_UNREAD_USER_COUNT_EVENT		    L"CalendarServiceOnMessageUnreadUserCountReceived"
#define CALENDAR_SERVICE_MEETING_INVITE_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnMeetingInviteMessageReceived"
#define CALENDAR_SERVICE_MEETING_UPDATE_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnMeetingUpdateMessageReceived"
#define CALENDAR_SERVICE_MEETING_FORWARD_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnMeetingForwardMessageReceived"
#define CALENDAR_SERVICE_MEETING_ACCEPT_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnMeetingAcceptMessageReceived"
#define CALENDAR_SERVICE_MEETING_DENY_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnMeetingDenyMessageReceived"
#define CALENDAR_SERVICE_MEETING_CANCEL_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnMeetingCancelMessageReceived"
#define CALENDAR_SERVICE_MINUTES_CREATE_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnMinutesCreateMessageReceived"
#define CALENDAR_SERVICE_MINUTES_UPDATE_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnMinutesUpdateMessageReceived"
#define CALENDAR_SERVICE_ACCREDIT_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnAccreditMessageReceived"
#define CALENDAR_SERVICE_ACCREDIT_CANCEL_MESSAGE_RECEIVED_EVENT		L"CalendarServiceOnAccreditCancelMessageReceived"
#define CALENDAR_SERVICE_MEETING_ALARMED_EVENT						L"CalendarServiceOnMeetingAlarmed"
#define CALENDAR_SERVICE_PARTJOIN_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnPartJoinMessageReceived"
#define CALENDAR_SERVICE_PARTCANCEL_MESSAGE_RECEIVED_EVENT			L"CalendarServiceOnPartCancelMessageReceived"
#define CALENDAR_SERVICE_REMOTE_ASSISTANCE_CREATED_EVENT			L"CalendarServiceOnRemoteAssistanceCreated"
#define CALENDAR_SERVICE_REMOTE_ASSISTANCE_INVITE_MSG_SENT_ENVNT	L"CalendarServiceOnRemoteAssistanceInviteMsgSent"
#define CALENDAR_SERVICE_REMOTE_ASSISTANCE_ACCEPT_MSG_SENT_ENVNT	L"CalendarServiceOnRemoteAssistanceAcceptMsgSent"
#define CALENDAR_SERVICE_REMOTE_ASSISTANCE_REJECT_MSG_SENT_ENVNT	L"CalendarServiceOnRemoteAssistanceRejectMsgSent"
#define CALENDAR_SERVICE_REMOTE_ASSISTANCE_INVITE_MSG_RECEIVED_ENVNT	L"CalendarServiceOnRemoteAssistanceInviteMsgReceived"
#define CALENDAR_SERVICE_REMOTE_ASSISTANCE_ACCEPT_MSG_RECEIVED_ENVNT	L"CalendarServiceOnRemoteAssistanceAcceptMsgReceived"
#define CALENDAR_SERVICE_REMOTE_ASSISTANCE_REJECT_MSG_RECEIVED_ENVNT	L"CalendarServiceOnRemoteAssistanceRejectMsgReceived"
#define CALENDAR_SERVICE_REMOTE_ASSISTANCE_END_MSG_RECEIVED_ENVNT		L"CalendarServiceOnRemoteAssistanceEndMsgReceived"
#define CALENDAR_SERVICE_NOTIFY_MEETING_READIED_ENVNT					L"CalendarServiceNotifyMeetingReadied"
#define CALENDAR_SERVICE_NOTIFY_MEETING_JOINED_ENVNT					L"CalendarServiceNotifyMeetingJoined"
#define CALENDAR_SERVICE_NOTIFY_MEETING_JOIN_FAILED_ENVNT				L"CalendarServiceNotifyMeetingJoinFailed"
#define CALENDAR_SERVICE_NOTIFY_MEETING_ClOSED_ENVNT					L"CalendarServiceNotifyMeetingClosed"
#define CALENDAR_SERVICE_DOCUMENTUPLOAD_MESSAGE_RECEIVED				L"CalendarServiceOnDocumentUploadMessageReceived"
#define CALENDAR_SERVICE_DOCUMENTDELETE_MESSAGE_RECEIVED				L"CalendarServiceOnDocumentDeleteMessageReceived"
#define CALENDAR_SERVICE_MEETINGROOM_CREATED_EVENT						L"CalendarServiceOnMeetingRoomCreated"
#define CALENDAR_SERVICE_MEETINGROOM_UPDATED_EVENT						L"CalendarServiceOnMeetingRoomUpdated"
#define CALENDAR_SERVICE_MEETINGROOM_MEETING_CREATED_EVENT				L"CalendarServiceOnMeetingCreatedInMeetingRoom"
#define CALENDAR_SERVICE_MEETINGROOM_LIST_RECEIVED_EVENT				L"CalendarServiceOnMeetingRoomListReceived"
#define CALENDAR_SERVICE_CHECK_JOIN_MEETINGROOM_EVENT					L"CalendarServiceOnCheckJoinMeetingRoom"
#define CALENDAR_SERVICE_CHECK_MEETINGROOM_PASSWORD_EVENT				L"CalendarServiceOnCheckMeetingRoomPassword"
#define CALENDAR_SERVICE_MEETINGROOM_DELETED_EVENT						L"CalendarServiceOnMeetingRoomDeleted"
#define CALENDAR_SERVICE_RIGHTS_CHECKED_EVENT							L"CalendarServiceOnRightsChecked"
#define CALENDAR_SERVICE_REVOCATION_MESSAGE_EVENT						L"CalendarServiceOnRevocationMesssageReceived"
#define CALENDAR_SERVICE_REVOCATION_MSG_EVENT							L"CalendarServiceOnRevocationMessage"
// FileTransfer Service 
#define FILETRANSFER_SERVICE_ONFILEUPLOADING_EVENT					"FileTransferServiceOnFileUploading"
#define FILETRANSFER_SERVICE_ONFILEUPLOADED_EVENT					"FileTransferServiceOnFileUploaded"
#define FILETRANSFER_SERVICE_ONFILEDOWNLOADING_EVENT				"FileTransferServiceOnFileDownloading"
#define FILETRANSFER_SERVICE_ONFILEDOWNLOADED_EVENT					"FileTransferServiceOnFileDownloaded"
#define FILETRANSFER_SERVICE_ONFILEDELETED_EVENT					"FileTransferServiceOnFileDeleted"

// PBX Service 
// 账号登陆通知
#define PBX_SERVICE_LOGINPBX_EVENT									"PbxServiceOnLoginPbx"
// 通话状态变化通知
#define PBX_SERVICE_STATUSCHANGED_EVENT								"PbxServiceOnCallStatusChanged"
// VoIP呼叫通知
#define PBX_SERVICE_MAKECALL_EVENT									"PbxServiceOnMakeCall"
// 来电通知
#define PBX_SERVICE_INCOMINGCALL_EVENT								"PbxServiceOnIncomingCall"
// 挂断通知
#define PBX_SERVICE_HANGUPCALL_EVENT								"PbxServiceOnHangupCall"
// 获取通话记录列表
#define PBX_SERVICE_GETCALLRECORDLST_EVENT							"PbxServiceOnGetCallRecordLst"
// 获取通话详情列表
#define PBX_SERVICE_GETCALLDETAILSLST_EVENT							"PbxServiceOnGetCallDetailsLst"
// 通话记录匹配搜索
#define PBX_SERVICE_SEARCHCALLRECORD_EVENT							"PbxServiceOnSearchCallRecord"

// PJSIP状态变化
#define PBX_SERVICE_SERVICESTATECHANGED_EVENT						"PbxServiceOnPJSIPServiceStateChanged"

// CatchScreen Service
#define CATCHSCREEN_SERVICE_ONSCREENCAPTURED_EVENT					L"CatchScreenServiceOnScreenCaptured"

#define SEND_FILE_TO_BEE_EVENT			L"SendFileToBeeEvent"

//Audio Service
#define AUDIO_SERVICE_ON_CALL_CREATED_EVENT							"AudioServiceOnCallCreated"
#define AUDIO_SERVICE_ON_CALL_JOINED_EVENT							"AudioServiceOnCallJoined"
#define AUDIO_SERVICE_ON_CALL_LEAVED_EVENT							"AudioServiceOnCallLeaved"
//#define AUDIO_SERVICE_ON_CONFERENCE_JOINED_EVENT					"AudioServiceOnConferenceJoined"
//#define AUDIO_SERVICE_ON_AUDIO_STARTED_EVENT						"AudioServiceOnAudioStarted"
//#define AUDIO_SERVICE_ON_CONFERENCE_LEFTED_EVENT					"AudioServiceOnConferenceLefted"
#define AUDIO_SERVICE_ON_CONFERENCE_ENDED_EVENT						"AudioServiceOnConferenceEnded"
#define AUDIO_SERVICE_ON_CONFERENCE_DISCONNECTED_EVENT				"AudioServiceOnConferenceDisconnected"
#define AUDIO_SERVICE_ON_CONFERENCE_RECONNECTED_EVENT				"AudioServiceOnConfReconnected"
#define AUDIO_SERVICE_ON_USER_ADDED_EVENT							"AudioServiceOnUserAdded"
#define AUDIO_SERVICE_ON_USER_REMOVED_EVENT							"AudioServiceOnUserRemoved"
#define AUDIO_SERVICE_ON_USER_PROPERTY_CHANGED_EVENT				"AudioServiceOnUserPropertyChanged"
#define AUDIO_SERVICE_ON_CALL_PHONE_FAILED_EVENT					"AudioServiceOnCallPhoneFailed"
#define AUDIO_SERVICE_ON_VOIP_NETWORK_QUALITY_CHANGED_EVENT			"AudioServiceOnVoipNetworkQualityChanged"
#define AUDIO_SERVICE_ON_MICROPHONE_STATUS_CHANGED_EVENT            "AudioServiceOnMicrophoneStatusChanged"
#define AUDIO_SERVICE_ON_SPEAKER_STATUS_CHANGED_EVENT				"AudioServiceOnSpeakerStatusChanged"
#define AUDIO_SERVICE_ON_SPEAKING_STATUS_CHANGED_EVENT              "AudioServiceOnSpeakingStatusChanged"
#define AUDIO_SERVICE_ON_USERS_ADDED_TO_CALL_EVENT					"AudioServiceOnUsersAddedToCall"
#define AUDIO_SERVICE_ON_USER_CALL_STATUS_GOT_EVENT					"AudioServiceOnUserCallStatusGot"
#define AUDIO_SERVICE_ON_HISTORY_PHONENUM_GOT_EVENT					"AudioServiceOnHistoryPhoneNumGot"
#define AUDIO_SERVICE_ON_SPEAKER_LEVEL_GOT_EVENT					"AudioServiceOnSpeakerLevelGot"
#define AUDIO_SERVICE_ON_MIKE_LEVEL_GOT_EVENT						"AudioServiceOnMikeLevelGot"
#define AUDIO_SERVICE_ON_SPEAKER_PLAY_FINISHED_EVENT				"AudioServiceOnSpeakerPlayFinished"
#define AUDIO_SERVICE_ON_INVITE_CALL_SEND_EVENT						"AudioServiceOnInviteCallSend"
#define AUDIO_SERVICE_ON_AUDIO_SERVICE_READY_EVENT					"AudioServiceOnAudioServiceReady"
#define AUDIO_SERVICE_ON_AUDIO_DEVICE_STATUS_CHANGED_EVENT			"AudioDeviceStatusChanged"
#define AUDIO_SERVICE_ON_LAST_AUDIO_INVITE_GOT_EVENT				"AudioServiceOnLastAudioInviteGot"

//sharing error handle
#define SHARING_SERVICE_ON_ERROR_EVENT								"SharingServiceOnErrorHandle"
#define SHARING_SERVICE_ON_DESKTOP_SHARED_EVENT						"SharingServiceOnDesktopShared"
#define SHARING_SERVICE_ON_DESKTOP_SHARE_STOPED_EVENT				"SharingServiceOnDesktopShareStoped"
#define SHARING_SERVICE_ON_DESKTOP_VIEWER_STARTED_EVENT				"SharingServiceOnDesktopViewerStarted"
#define SHARING_SERVICE_ON_DESKTOP_VIEWER_STOPPED_EVENT				"SharingServiceOnDesktopViewerStopped"


// Search Service
#define SERVER_SEARCH_RESULT_RECEIVED								L"SearchServiceOnServerSearched"
#define LOCAL_SEARCH_RESULT_RECEIVED								L"SearchServiceOnLocalSearched"

// Network Monitor
#define NETWORK_CHANGED												L"NetworkChanged"

//System Service
#define PLAYINGSOUND_NOTIFICATION									L"PlayingSoundNotification"

//OpenApi Service
#define OPENAPI_SERVICE_APP_LIST_RECEIVED							L"OpenApiServiceOnAppListReceived"
#define OPENAPI_SERVICE_APP_INFO_RECEIVED							L"OpenApiServiceOnAppInfoReceived"
#define OPENAPI_SERVICE_CHAT_HISTORY_RECEIVED_EVENT				    L"OpenApiServiceOnChatHistoryReceived"
#define OPENAPI_SERVICE_MESSAGE_SENT_EVENT						    L"OpenApiServiceOnMessageSent"
#define OPENAPI_SERVICE_PUSH_MESSAGE_ACK_EVENT						L"OpenApiServiceOnPushMessageACK"
#define OPENAPI_SERVICE_TEXT_MESSAGE_RECEIVED_EVENT				    L"OpenApiServiceOnTextMessageReceived"
#define OPENAPI_SERVICE_IMAGE_RECEIVED_EVENT					    L"OpenApiServiceOnImageReceived"
#define OPENAPI_SERVICE_FILE_RECEIVED_EVENT						    L"OpenApiServiceOnFileReceived"
#define OPENAPI_SERVICE_MESSAGE_READ_EVENT                          L"OpenApiServiceOnMessageRead"
#define OPENAPI_SERVICE_MESSAGE_APP_STATUS_RECEIVED_EVENT           L"OpenApiServiceOnMessageAppStatusChangedReceived"
#define OPENAPI_SERVICE_MESSAGE_APP_EVENT_RECEIVED_EVENT            L"OpenApiServiceOnMessageAppEventChangedReceived"
#define OPENAPI_SERVICE_MESSAGE_ACK_STATUS_RECEIVED_EVENT           L"OpenApiServiceOnMessageAckStatusReceived"
#define OPENAPI_SERVICE_OA_MESSAGE_STATUS_RECEIVED_EVENT            L"OpenApiServiceOnOAMessageStatusReceived"
#define OPENAPI_SERVICE_OA_MESSAGE_STATUS_UPDATED_EVENT             L"OpenApiServiceOnOAMessageStatusUpdated"
#define OPENAPI_SERVICE_OAMESSGAE_STATUS_CHANGED_RECEIVED_EVENT     L"OpenApiServiceOnOAMessageStatusChangedReceived"
#define OPENAPI_SERVICE_OA_MESSAGE_RECEIVED_EVENT                   L"OpenApiServiceOnOAMessageReceived"
#define OPENAPI_SERVICE_CUSTOMIZED_MESSAGE_RECEIVED_EVENT           L"OpenApiServiceOnCustomizedReceived"

// microblog service
#define MICROBLOG_SERVICE_GETNEWMSGCOUNT_EVENT						L"MicroblogServiceOnNewMsgCountGet"
#define MICROBLOG_SERVICE_ADDATTENTION_EVENT						L"MicroblogServiceOnAttentionAdd"
#define MICROBLOG_SERVICE_CANCELATTENTION_EVENT						L"MicroblogServiceOnAttentionCancelled"
#define MICROBLOG_SERVICE_RECORDINGSHARED_EVENT						L"MicroblogServiceOnMeetingRecordingShared"
//update service
#define CHECK_UPDATES_EVENT											L"UpdateServiceOnUpdatesChecked"
#define CHECK_PROGRESS_EVENT										L"UpdateServiceOnProgressChecked"

//external service
#define EXTERNAL_SERVICE_CHAT_HISTORY_RECEIVED_EVENT                L"ExternalServiceOnChatHistoryReceived"


// gkclient
#define GKCLIENT_LOGIN_EVENT										L"GkClientOnLogin"

#define BBS_SERVICE_MAIN_EVENT										L"BBSServiceOnResponse"


#endif