/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.interfaces.IGroupService = {
		getChatHistory: function(chatType, groupId, count, startTime, endTime, callback) {},
		sendTextMessage: function(chatType, groupJid, textType, textFont, textData, atUsers, callback) {},
		sendImageMessage: function(chatType, groupJid, imageName, imageSize, imageURL, imageData, callback) {},
		sendAudioMessage: function(chatType, groupJid, audioName, audioSize, audioURL, audioDuration, callback) {}, 
		sendVideoMessage: function(chatType, groupJid, videoName, videoSize, videoURL, videoData, videoDuration, callback) {},

		sendCallInvitation: function(chatType, groupJid, confId, confPwd, hostId, callback) {},
		acceptCallInvitation: function(chatType, groupJid, confId, hostId, callback) {},
		rejectCallInvitation: function(chatType, groupJid, confId, hostId, callback) {},
		finishCall: function(chatType, groupJid, confId, callback) {},
		cancelCall: function(chatType, groupJid, callback) {},
		//sendNotRespondCallMessage: function(chatType, groupJid, callback) {},
		sendRejectCallRecordMessage: function(chatType, groupJid, callback) {}, 
		sendCallDurationMessage: function(chatType, groupJid, duration, callback) {},

		sendMessageRead: function(chatType, groupJid, msgId, msgSeq, callback) {},
		sendMessagesReadById: function(chatType,groupId, callback) {},
		sendMessagePlayed: function(chatType, groupJid, msgId, msgSeq, callback) {},
		resendMessage: function (msgId, callback){},
		forwardMessage: function (jid, msgId, callback){},
		getGroupList: function(groupType, page, count, lastGroupTime, includingQuitMembers, callback) {},
		/**
		 * get contact info, if group is in local, will return directly, otherwise will return via OnGroupInfoReceived
		 * @param groupId	group id
		 */
		getGroupInfo: function(groupId) {},
        getGroupInfoByMountId:function(MountId) {},
		createGroup: function(groupType, groupName, userInfos, nameFlag, callback) {},
		endGroup: function(groupId, callback) {}, 
		exitGroup: function(groupId, callback) {},
		addMember: function(groupId, userInfos, callback) {}, 
		removeMember: function(groupId, members, callback) {}, 
		setGroupName: function(groupId, groupName, callback) {},
		getDocumentList: function(groupId, page, count, callback) {}, 
		uploadDocument: function(groupId, operation, fileName, fileSize, fileURL, uploadTime, callback) {},
        uploadCodeSnippet: function(groupId, operation, codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, callback){},
		deleteDocument: function(groupId, fileId, callback) {}, 
		getDocumentInfo: function(groupId, fileId, callback) {},
		setGroupLogo: function(groupId, logoURL, callback) {},
		setTopChat: function(groupId, state, callback) {}, 
		setDoNotDisturb: function(groupId, state, callback) {}, 
		setReminder: function(groupId, state, alertRule, alertTime, callback) {},
		createComment: function(groupId, fileId, fileType, fileTitle, content, callback) {},
		getCommentList: function(groupId, fileId, count, startTime, endTime, callback) {},
		updateGroupAvatar: function(groupId, localAvatarUrl, callback){},
        sendGroupVideoMessage:function(chatType, groupJid,videoName,videoSize,videoUrl,videoAvatar,thumbnailsURL,timeLength,operatorId,operatorName,operatorAvatar,callback){},
        getUnreadUserList:function(chatType,groupJid,messageList,callback){},
        uploadCloudDocument: function(groupId, operation, fileName, fileSize, cloudType, detailContent, callback){},
        sendCloudFileMessage: function(chatType, groupJid, fileName, fileSize, cloudType, detailContent, callback){},
		sendReportMessage: function(ids, reportTitle, reportUrl, callback) {},
		getAtMessageList: function(groupId, page, count, callback){},
		deleteAtMessage:function(chatType, groupId, seqList, callback){},
		// added by xue.bai_2 on 2016/04/08
		revocationMessage: function(toUserId, type, seq, callback){}
	}
})(uc);