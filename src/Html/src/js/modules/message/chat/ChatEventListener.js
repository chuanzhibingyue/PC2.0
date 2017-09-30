/**
 * Created by waouyang on 15/8/3.
 */
(function (uc) {
    uc.modules.chat.ChatEventListener = function () {

    };
    jQuery.extend(uc.modules.chat.ChatEventListener.prototype,
        uc.interfaces.IClientEvent,
        uc.interfaces.IUIEvent,
        uc.interfaces.IIMEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IFSEvent,
        uc.interfaces.IGroupEvent,
        uc.interfaces.IMeetingEvent,
        uc.interfaces.IOpenApiEvent,
        uc.interfaces.IRemoteAssistanceEvent,
        uc.interfaces.ICallEvent,{
            OnModuleChanged:function(moduleId){
                uc.modules.chat.ChatManager.setModuleId(moduleId);
            },
            OnPlayingSoundNotification: function(id, percent, seconds){
                uc.modules.chat.ChatManager.OnPlayingSoundNotification(id, percent, seconds);
            },
            OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
                uc.modules.chat.ChatManager.NoticeUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
            },
            OnChatImageDownloaded: function(chatId, chatType, serverUrl, localUrl, msgId){
                uc.modules.chat.ChatManager.OnChatImageDownloaded(chatId, chatType, serverUrl, localUrl, msgId);
            },
            OnOpenChatWindow: function (chatType, chatId, openChatWindowSourceModule, meetingData, siteId,type,isFromCall, paramObj) {
                uc.modules.chat.ChatManager.OnOpenChatWindow(chatType, chatId, openChatWindowSourceModule, meetingData, siteId,isFromCall, paramObj);
            },
            OnStartChat: function (userIds) {
                uc.modules.chat.ChatManager.startChat(userIds);
            },
            OnCreateProject: function () {
                uc.modules.chat.ChatManager.createProject();
            },
            OnCallStarted: function(chatId, chatType){
                uc.modules.chat.ChatManager.OnCallStarted(chatId, chatType);
            },
            OnCallEnded: function(chatId, chatType){
                uc.modules.chat.ChatManager.OnCallEnded(chatId, chatType);
            },
            /** Contact event*/
            OnContactInfoReceived: function (errorCode, userIds, contacts) {
                uc.modules.chat.ChatManager.notifyContactInfoReceived(errorCode, userIds, contacts);
            },
            OnContactUpdated: function (contacts) {
                uc.modules.chat.ChatManager.notifyContactUpdated(contacts);
            },
            OnBasicContactInfoReceived: function (errorCode, userIds, contacts) {
                uc.modules.chat.ChatManager.notifyBasicContactInfoReceived(errorCode, userIds, contacts);
            },
            OnDraftMessageChanged: function(chatId, chatType, moduleId){
                uc.modules.chat.ChatManager.OnDraftMessageChanged(chatId, chatType, moduleId);
            },
            /** IM events */
            OnChatHistoryReceived: function (errorCode, userId, messages, msgCount, minTimestamp) {
                uc.modules.chat.ChatManager.noticeChatHistoryReceived(errorCode, userId, messages, msgCount, minTimestamp);
            },
            OnMessageSent: function (errorCode, userId, msgId, msgSeq) {
                uc.modules.chat.ChatManager.noticeMessageSent(errorCode, userId, msgId, msgSeq);
            },
            OnTextMessageReceived: function (channel, sendState, from, to, msgId, msgSeq, textType, textData, sendTime, timestamp, textFormat) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeTextMessageReceived(channel, sendState, from, to, msgId, msgSeq, textType, textData, sendTime, timestamp, textFormat);
            },
            OnImageMessageReceived: function (channel, sendState, from, to, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeImageMessageReceived(channel, sendState, from, to, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp);
            },
            OnAudioMessageReceived: function (channel, sendState, from, to, msgId, msgSeq, audioName, audioSize, audioURL, audioDuration, timestamp) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeAudioMessageReceived(channel, sendState, from, to, msgId, msgSeq, audioName, audioSize, audioURL, audioDuration, timestamp);
            },
            OnVideoMessageReceived: function (channel, sendState, from, to, msgId, msgSeq, videoName, videoSize, videoURL, videoThumb, duration, timestamp) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeVideoMessageReceived(channel, sendState, from, to, msgId, msgSeq, videoName, videoSize, videoURL, videoThumb, duration, timestamp);
            },
            OnFileReceived: function (channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, timestamp,localPath) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeFileReceived(channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, timestamp,localPath);
            },
            OnFileShared: function (jid, msgId, msgSeq, fileName, fileSize, fileURL, fileId) {
                uc.modules.chat.ChatManager.noticeFileShared(jid, msgId, msgSeq, fileName, fileSize, fileURL, fileId);
            },
            OnFileDeleted: function (jid, msgId, msgSeq, fileName, fileId) {
                uc.modules.chat.ChatManager.noticeOnFileDeleted(jid, msgId, msgSeq, fileName, fileId);
            },
            OnCodeReceived: function(channel, sendState, from, to, msgId, msgSeq, creatorId, creatorName, codeId, codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeCodeReceived(channel, sendState, from, to, msgId, msgSeq, creatorId, creatorName, codeId, codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp);
            },
            OnCloudFileReceived:function(channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, timestamp,detailContent){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeCloudFileReceived(channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, timestamp,detailContent);
            },
            OnP2PCallInvitationReceived: function(channel, userId,fromUserId, confId, confPwd, hostId, msgId, msgSeq, timestamp){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnP2PCallInvitationReceived(channel, userId,fromUserId, confId, confPwd, hostId, msgId, msgSeq, timestamp);
            },
            OnP2PCallInvitationAccepted: function(channel, userId,fromUserId, confId, msgId, msgSeq, timestamp){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnP2PCallInvitationAccepted(channel, userId,fromUserId, confId, msgId, msgSeq, timestamp);
            },
            OnP2PCallInvitationRejected: function(channel, userId,fromUserId, confId, hostId, reason, msgId, msgSeq, timestamp,sendState){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnP2PCallInvitationRejected(channel, userId,fromUserId, confId, hostId, reason, msgId, msgSeq, timestamp,sendState);
            },
            OnP2PCallStopped: function(channel, userId, confId, msgId, msgSeq, timestamp){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnP2PCallStopped(channel, userId, confId, msgId, msgSeq, timestamp);
            },
            OnP2PCallCancelled: function(channel, userId,fromUserId, msgId, msgSeq, timestamp){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnP2PCallCancelled(channel, userId,fromUserId, msgId, msgSeq, timestamp);
            },
            OnP2PCallNoResponse: function(channel, userId, msgId, msgSeq, timestamp){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnP2PCallNoResponse(channel, userId, msgId, msgSeq, timestamp);
            },
            OnP2PCallDurationReceived: function(channel, userId,fromUserId, duration, msgId, msgSeq, timestamp,sendState){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnP2PCallDurationReceived(channel, userId,fromUserId, duration, msgId, msgSeq, timestamp,sendState);
            },
            /** group events */
            OnGroupChatHistoryReceived: function (errorCode, conversationType, groupId, messages, msgCount, minTimestamp) {
                uc.modules.chat.ChatManager.noticeGroupChatHistoryReceived(errorCode, conversationType, groupId, messages, msgCount, minTimestamp);
            },
            OnGroupMessageSent: function (errorCode, conversationType, groupId, msgId, msgSeq) {
                uc.modules.chat.ChatManager.noticeGroupMessageSent(errorCode, conversationType, groupId, msgId, msgSeq);
            },
            OnGroupTextMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, msgTime, textFormat,atUsers) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupTextMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, msgTime, textFormat,atUsers);
            },
            OnGroupImageMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupImageMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime);
            },
            OnGroupAudioMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, duration, msgTime) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupAudioMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, duration, msgTime);
            },
            OnGroupVideoMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, duration, msgTime) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupVideoMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, duration, msgTime);
            },
            OnGroupCreatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupCreatedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime);
            },
            OnGroupMemberExitedMessageReceived: function(conversationType, channel, from, to, operatorId, memberId, groupId, groupName, groupAvatar, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupMemberExitedMessageReceived(conversationType, channel, from, to, operatorId, memberId, groupId, groupName, groupAvatar, msgId, msgSeq, msgTime);
            },
            OnGroupEndMessageReceived: function(conversationType, channel, from, to, operatorId, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupEndMessageReceived(conversationType, channel, from, to, operatorId, msgId, msgSeq, msgTime);
            },
            OnGroupMemberAddedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupMemberAddedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime);
            },
            GroupMemberKickedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupMemberKickedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime);
            },
            OnGroupNameUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupNameUpdatedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime);
            },
            OnGroupAvatarUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupAvatar, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupAvatarUpdatedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupAvatar, msgId, msgSeq, msgTime);
            },
            OnGroupDisturbSetMessageReceived: function(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupDisturbSetMessageReceived(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime);
            },
            OnGroupCallDurationReceived:function(channel, conversationType,groupId, duration, msgId, msgSeq, timestamp){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnGroupCallDurationReceived(channel, conversationType, groupId, duration, msgId, msgSeq, timestamp);
            },
            OnGroupOnTopMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, isTop, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupOnTopMessageReceived(conversationType, channel, from, to, operatorId, groupId, isTop, msgId, msgSeq, msgTime);
            },
            OnDocumentUploadMessageReceived: function(conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username,localPath){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeDocumentUploadMessageReceived(conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username, localPath);
            },
            OnDocumentShareMessageReceived: function(conversationType, channel, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeDocumentShareMessageReceived(conversationType, channel, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username);
            },
            OnDocumentDeleteMessageReceived: function(conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeDocumentDeleteMessageReceived(conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime);
            },
            OnGroupCreated: function (errorCode, operatorId, group) {
                if (group.groupType == uc.constants.Chat.ChatType.CloudChat) return;
                uc.modules.chat.ChatManager.noticeGroupCreated(errorCode, operatorId, group);
            },
            OnGroupExited: function (errorCode, channel, groupId, userId) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupExited(errorCode,  channel, groupId, userId);
            },
            OnGroupEnded: function (errorCode, channel, groupId, operatorId) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupEnded(errorCode,  channel, groupId, operatorId);
            },
            OnGroupMemberAdded: function (errorCode, channel, groupId, operatorId, memberIds) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeMemberAdded(errorCode,  channel, groupId, operatorId, memberIds);
            },
            OnGroupMemberKicked: function (errorCode, channel, groupId, operatorId, memberIds) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeMemberRemoved(errorCode,  channel, groupId, operatorId, memberIds);
            },
            OnGroupNameUpdated: function (errorCode, channel, groupId, operatorId, groupName, pinyin) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupNameUpdated(errorCode,  channel, groupId, operatorId, groupName, pinyin);
            },
            OnGroupAvatarUpdated: function (errorCode, channel, groupId, logoURL) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupLogoUpdated(errorCode,  channel, groupId, logoURL);
            },
            OnGroupInfoReceived: function (errorCode, groupId,mountId, group) {
                uc.modules.chat.ChatManager.noticeGroupInfoReceived(errorCode, groupId, group);
            },
            OnDocListReceived: function (errorCode, groupId, documents) {
                uc.modules.chat.ChatManager.noticeDocListReceived(errorCode, groupId, documents);
            },
            OnDocInfoReceived: function (errorCode, groupId, document) {
                uc.modules.chat.ChatManager.noticeDocInfoReceived(errorCode, groupId, document);
            },
            OnDocumentUploaded: function (errorCode, groupId, operatorId, document, msgId) {
                uc.modules.chat.ChatManager.noticeDocumentUploaded(errorCode, groupId, operatorId, document, msgId);
            },
            OnDocumentShared: function (errorCode, groupId, operatorId, document) {
                uc.modules.chat.ChatManager.noticeDocumentShared(errorCode, groupId, operatorId, document);
            },
            OnDocumentDeleted: function (errorCode, groupId, operatorId, docName, fileId) { },
            OnCommentCreateMessageReceived:function(conversationType, channel, from, to, msgId, msgSeq, fileId, fileType, fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeCommentCreateMessageReceived(conversationType, channel, from, to, msgId, msgSeq, fileId, fileType, fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType);
            },
            OnCodeSnippetUploadMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, creatorId, creatorName, codeId, codeTitle,
                                                         langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeCodeSnippetUploadMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, creatorId, creatorName, codeId, codeTitle,
                        langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp);
            },
            OnCodeSnippetUploaded:function(errorCode, groupId, operatorId, document, msgId){
                uc.modules.chat.ChatManager.noticeCodeSnippetUploaded(errorCode, groupId, operatorId, document, msgId);
            },
            OnGroupCallInvitationRejected: function(channel,conversationType, groupId, confId, reason,userId, msgId, msgSeq, timestamp,sendState){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnGroupCallInvitationRejected(channel,conversationType, groupId, confId, reason,userId, msgId, msgSeq, timestamp,sendState);
            },
            OnGroupCloudFileMessageReceived: function(conversationType, channel, sendState, from, to, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username,operation,detailContent){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel)) {
                    if(!detailContent)return;
                    uc.modules.chat.ChatManager.noticeGroupCloudFileMessageReceived(conversationType, channel, sendState, from, to, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username,operation,detailContent);
                }
            },
            OnGroupMemberRoleUpdatedMessageReceived:function(conversationType, channel, sendState, from, to, msgId, msgSeq, msgTime,roles){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupMemberRoleUpdatedMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, msgTime,roles);
            },
            OnRevocationMessage: function(errorCode, groupId, msgId, msgSeq, msgTime, revocationType){
                uc.modules.chat.ChatManager.OnRevocationMessage(errorCode, groupId, msgId, msgSeq, msgTime, revocationType);
            },
            OnRevocationMessageReceived: function(channel, conversation, conversationType, from, to, msgId, msgSeq, msgTime, seqId){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel)){
                    uc.modules.chat.ChatManager.OnRevocationMessageReceived(channel, conversation, conversationType, from, to, msgId, msgSeq, msgTime, seqId);
                }
            },
            /** meeting */
            OnMeetingMessageRevocation: function(errorCode, groupId, msgId, msgSeq, msgTime, revocationType){
                uc.modules.chat.ChatManager.OnRevocationMessage(errorCode, groupId, msgId, msgSeq, msgTime, revocationType);
            },
            OnMeetingMessageRevocationReceived: function(channel, conversation, conversationType, from, to, msgId, msgSeq, msgTime, seqId, eventId){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel)){
                    uc.modules.chat.ChatManager.OnMeetingMessageRevocationReceived(channel, conversation, conversationType, from, to, msgId, msgSeq, msgTime, seqId, eventId);
                }
            },
            OnMeetingMessageSent: function(errorCode, groupId, eventId, meetingStartTime, msgId, msgSeq, msgTime){
                uc.modules.chat.ChatManager.OnMeetingMessageSent(errorCode, groupId, eventId, meetingStartTime, msgId, msgSeq, msgTime);
            },
            OnMeetingChatHistoryReceived: function(errorCode, groupId, msgCount, minTimestamp){
                uc.modules.chat.ChatManager.OnMeetingChatHistoryReceived(errorCode, groupId, msgCount, minTimestamp);
            },
            OnMeetingTextMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime, textFormat){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingTextMessageReceived(channel, sendState, from, to, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime, textFormat);
            },
            OnMeetingImageMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingImageMessageReceived(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime);
            },
            OnMeetingAudioMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq,msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingAudioMessageReceived(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq,msgTime);
            },
            OnMeetingVideoMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingVideoMessageReceived(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, msgTime);
            },
            OnMeetingInviteMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingInviteMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            },
            OnMeetingInvitationAcceptedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingInvitationAcceptedMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            },
            OnMeetingInvitationRejectedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingInvitationRejectedMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            },
            OnMeetingUpdateMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingUpdateMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime);
            },
            OnMeetingForwardMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))	{
                    uc.modules.chat.ChatManager.OnMeetingForwardMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
                }
            },
            OnMeetingCancelMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent,meetingStartTime, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingCancelledMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent,meetingStartTime, msgId, msgSeq, msgTime);
            },
            OnMinutesCreateMessageReceived: function(channel, from, to, operatorId, minutesId, minutesTitle,meetingId, eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMinutesCreateMessageReceived(channel, from, to, operatorId, minutesId, minutesTitle,meetingId, eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime);
            },
            OnMinutesUpdateMessageReceived: function(channel, from, to, operatorId,  minutesId, minutesTitle,  meetingId,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMinutesUpdateMessageReceived(channel, from, to, operatorId,  minutesId, minutesTitle, meetingId,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime);
            },
            OnMeetingAcceptMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingAcceptMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            },
            OnMeetingRejectMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime,updateFlag, msgId, msgSeq, msgTime) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingRejectMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime,updateFlag, msgId, msgSeq, msgTime);
            },
            OnMeetingAccreditMessageReceived: function(channel, from, to, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingAccreditMessageReceived(channel, from, to, msgId, msgSeq, msgTime);
            },
            OnMeetingAccreditCancelledMessageReceived: function(channel, from, to, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingAccreditCancelledMessageReceived(channel, from, to, msgId, msgSeq, msgTime);
            },

            OnApiChatHistoryReceived: function(errorCode, appId, msgCount, minTimestamp){
                uc.modules.chat.ChatManager.OnApiChatHistoryReceived(errorCode, appId, msgCount, minTimestamp);
            },
            OnOpenApiTextMessageReceived: function(channel, sendState, from, to, textTitle, textContent, textType, detailContent, msgId, msgSeq, mstTime, detailAuth){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnOpenApiTextMessageReceived(channel, sendState, from, to, textTitle, textContent, textType, detailContent, msgId, msgSeq, mstTime, detailAuth);
            },
            OnOpenApiImageMessageReceived: function(channel, sendState, from, to, imageId, imageType, imageData, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnOpenApiImageMessageReceived(channel, sendState, from, to, imageId, imageType, imageData, msgId, msgSeq, msgTime);
            },
            OnOpenApiFileMessageReceived: function(channel, sendState, from, to, fileId, fileType, fileName, fileSize, msgId, msgSeq, mstTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnOpenApiFileMessageReceived(channel, sendState, from, to, fileId, fileType, fileName, fileSize, msgId, msgSeq, mstTime);
            },
            OnOpenApiMessageSent:function(errorCode, appId, msgId, msgSeq, msgTime){
                uc.modules.chat.ChatManager.OnOpenApiMessageSent(errorCode, appId, msgId, msgSeq, msgTime);
            },
            OnOAMessageStatusReceived: function(errorCode, appId, msgSeq, status) {
                uc.modules.chat.ChatManager.OnOAMessageStatusReceived(errorCode, appId, msgSeq, status);
            },
            OnOAMessageReceived: function(channel, from, to, title, titleColor, titleBkColor, titleElements, status, data, detailUrl, msgId, msgSeq,msgTime, detailAuth, oaExternalData, titleStyle) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnOAMessageReceived(channel, from, to, title, titleColor, titleBkColor, titleElements, titleStyle, status, data, detailUrl, msgId, msgSeq,msgTime, detailAuth, oaExternalData);
            },
            OnOAMessageStatusChanged: function(appId, msgSeq, status, from, extraData) {
                uc.modules.chat.ChatManager.OnOAMessageStatusChanged(appId, msgSeq, status, from, extraData);
            },
            OnPushMessageAck: function(channel, errorCode, from, to, conversation, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnPushMessageAck(channel, errorCode, from, to, conversation, msgId, msgSeq, msgTime);
            },
            OnFileDownloading: function (errorCode, taskId, percent) {
                uc.modules.chat.ChatManager.noticeFileDownloading(errorCode, taskId, percent);
            },
            OnFileDownloaded: function (errorCode, taskId, savePath) {
                uc.modules.chat.ChatManager.noticeFileDownloaded(errorCode, taskId, savePath);
            },
            OnFileUploading: function (errorCode, taskId, percent) {
                uc.modules.chat.ChatManager.noticeFileUploading(errorCode, taskId, percent);
            },
            OnFileUploaded: function (errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL) {
                uc.modules.chat.ChatManager.noticeFileUploaded(errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL);

                var fileInfo = {
                    name: fileName,
                    size: fileSize,
                    url: downloadURL,
                    thumbnailURL: convertDownloadURL,
                    operatorId: operatorId,
                    operatorTime: operatorTime,
                    fid: fid
                };

                if (errorCode == 0) {
                    uc.modules.chat.MessageManager.noticeFileUploaded(taskId, fileInfo);
                }
            },
            OnMeetingSummaryVideoMessageReceived:function(channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingSummaryVideoMessageReceived(channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime);
            },
            OnIMSummaryVideoMessageReceived:function(channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnIMSummaryVideoMessageReceived(channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime);
            },
            OnGroupSummaryVideoMessageReceived:function(conversationType,channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnGroupSummaryVideoMessageReceived(conversationType,channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime);
            },
            OnGroupUnreadUserCountEvent:function(errorCode,type,userId,info){
                if(errorCode==0){
                    uc.modules.chat.ChatManager.OnGroupUnreadUserCountEvent(type,userId,info);
                }
            },
            OnIMUnreadUserCountEvent:function(errorCode,userId,info){
                if(errorCode==0){
                    uc.modules.chat.ChatManager.OnIMUnreadUserCountEvent(userId,info);
                }
            },
            OnMeetingUnreadUserCountEvent:function(errorCode,type,userId,info){
                if(errorCode==0){
                    uc.modules.chat.ChatManager.OnMeetingUnreadUserCountEvent(userId,info);
                }
            },
            OnGroupUnreadUserListEvent:function(errorCode,type,userId,info){
                if(errorCode==0){
                    uc.modules.chat.ChatManager.OnGroupUnreadUserListEvent(type,userId,info);
                }
            },
            OnGkFileDownloadCallBack:function(data){
                uc.modules.chat.ChatManager.noticeCloudFileDownloaded(data);
            },
            OnCheckGkFilePermission:function(data,cloudData,Type,that){
                if(Type=="comForward" || Type=="comDownload"){
                    uc.modules.comment.CommentManager.noticeCheckGkFilePermission(data,cloudData,Type,that);
                }else{
                    uc.modules.chat.ChatManager.noticeCheckGkFilePermission(data,cloudData,Type,that);
                }

            },
            OnCheckGkFileExist:function(data,cloudData,option,Type){
                uc.modules.chat.ChatManager.noticeCheckGkFileExist(data,cloudData,option,Type);
            },
            OnCloudLogin:function(errorCode){
                if(errorCode==0){
                    uc.modules.chat.ChatManager.OnCloudLoginReceived(errorCode);
                }
            },
            /************* IRemoteAssistanceEvent **************/
            OnRemoteAssistanceCreated: function(errorCode, userId, meetingId, attendeePassword, meetingStartUrl){

            },
            OnRemoteAssistanceInvitationReceived: function(channel, from, meetingId, attendeePassword, msgId, msgSeq, timestamp){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnRemoteAssistanceInvitationReceived(from, meetingId, attendeePassword, msgId, msgSeq, timestamp);
            },
            OnRemoteAssistanceInvitationAccepted: function(channel, from, meetingId, msgId, msgSeq, timestamp){

            },
            OnRemoteAssistanceInvitationRejected: function(channel, from, meetingId, msgId, msgSeq, timestamp){
            },
            OnRemoteAssistanceEnded: function(channel, meetingId, msgId, msgSeq, timestamp){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnRemoteAssistanceEnded(channel, meetingId, msgId, msgSeq, timestamp);
            },
            OnUserStatusSetted: function(errorCode, userId, status) {
            },
            OnUserStatusReceived: function(userIds, status) {
                uc.modules.chat.ChatManager.OnUserStatusReceived(userIds, status);
            },
            OnExternalMessageReceived: function(errorCode, conversationType, relateID, msgCount, minTimestamp, messages) {
                uc.modules.chat.ChatManager.OnExternalMessageReceived(errorCode, conversationType, relateID, msgCount, minTimestamp, messages);
            },
            OnMeetingDocumentUploadMessageReceived: function(eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp, cloudType,localPath){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeMeetingDocumentUploadMessageReceived(eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp, cloudType,localPath);
            },
            OnCloudFileSelectCallBack:function(eventData){
                var obj = JSON.parse(eventData.data);
                var chatInfo = JSON.parse(eventData.chatInfo);
                for(var i in obj.list){
                    uc.modules.chat.MessageManager.sendCloudFileMessage(chatInfo.options,obj.list[i]);
                }
            },
            OnGetUserStatusResponse:function(errorCode,status,copyBack){
                if(copyBack && parseInt(copyBack) != 0){
                    var copyBackobj = $.parseJSON(copyBack);
	                if(!copyBackobj.userId){
		                return;
	                }
                    var chatId = parseInt(copyBackobj.userId) ? parseInt(copyBackobj.userId) : 0;
                    var chatType = parseInt(copyBackobj.type);
                    uc.modules.chat.ChatManager.OnUserCallStatusReceived(errorCode, chatId, chatType, status);
                }
            } ,
            OnMeetingFileUploaded: function (errorCode, eventId, operatorId, document, msgId) {
                uc.modules.chat.ChatManager.noticeMeetingDocumentUploaded(errorCode, eventId, operatorId, document, msgId);
            },
            OnMeetingFileDeleteMessageReceived: function(eventId,conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime){
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeMeetingDocumentDeleteMessageReceived(eventId,conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime);
            },
            OnMeetingReportMessageReceived: function(channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.OnMeetingReportMessageReceived(channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl);
            },
            OnIMReportMessageReceived: function (channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeIMReportMessageReceived(channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl);
            },
            OnGroupReportMessageReceived: function (conversationType, channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
                if( uc.modules.chat.ChatManager.isMyMsgChannel(channel))
                    uc.modules.chat.ChatManager.noticeGroupReportMessageReceived(conversationType, channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl);
            },
            OnMeetingInfoReceivedById: function(errorCode,eventId, authorizerId, meetingInfoArray){
                uc.modules.chat.ChatManager.OnMeetingInfoReceivedById(errorCode,eventId, authorizerId, meetingInfoArray);
            },
            OnMinutesDetailsReceived:function(errorCode,eventId,minutesId,creatorId,createTime,content,startTime){
                uc.modules.chat.ChatManager.OnMinutesDetailsReceived(errorCode,eventId,minutesId);
            },
            OnAtMessageListReceived:function(errorCode, groupId, atMsg){
                uc.modules.chat.ChatManager.OnAtMessageListReceived(errorCode, groupId, atMsg);
            },
            OnAtMessageDeleted:function(errorCode, conversationType, groupId, atSeq){
                uc.modules.chat.ChatManager.OnAtMessageDeleted(errorCode, conversationType, groupId, atSeq);
            }
        })
})(uc);