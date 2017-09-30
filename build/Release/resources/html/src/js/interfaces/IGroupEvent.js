/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.interfaces.IGroupEvent = {
        OnGroupChatHistoryReceived: function(errorCode, conversationType, groupId, messages, msgCount, minTimestamp){},
        OnGroupMessageSent: function(errorCode, conversationType, groupId, msgId, msgSeq, msgTime){},
        OnGroupTextMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, msgTime, textFormat, atUsers){},
        OnGroupImageMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime){},
        OnGroupAudioMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, duration, msgTime){},
        OnGroupVideoMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, duration, msgTime){},
        OnGroupMessageRead: function(errorCode, conversationType, userId, readType, msgId, msgSeq){},
        OnGroupMessagePlayed: function(errorCode, userId, msgId, msgSeq){},

        OnGroupCallInvitationReceived: function(channel,conversationType, groupId, hostId, confId, confPwd, msgId, msgSeq, timestamp){},
        OnGroupCallInvitationAccepted: function(channel,conversationType, groupId, confId, userId,msgId, msgSeq, timestamp){},
        OnGroupCallInvitationRejected: function(channel,conversationType, groupId, confId, reason,userId, msgId, msgSeq, timestamp,sendState){},
        OnGroupCallStopped: function(conversationType, groupId, confId, msgId, msgSeq, timestamp){},
        OnGroupCallDurationReceived: function(channel,conversationType, groupId, duration, msgId, msgSeq, timestamp){},

        OnGroupCreated: function(errorCode, operatorId, group){},
        /**
         * event fro group created
         * @param channel
         * @param conversationType
         * @param from
         * @param to
         * @param operatorId
         * @param groupId
         * @param members       member array, each member is {userId: xx, joinTime: xx}
         * @param groupName
         * @param groupAvatar
         * @param msgId
         * @param msgSeq
         * @param msgTime
         * @constructor
         */
        OnGroupCreatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){},
        OnGroupExited: function(errorCode, groupId, userId){},
        /**
         * event for group member exit group
         * @param conversationType
         * @param channel
         * @param from
         * @param to
         * @param operatorId
         * @param memberId
         * @param groupId
         * @param groupName
         * @param groupAvatar
         * @param msgId
         * @param msgSeq
         * @param msgTime
         * @constructor
         */
        OnGroupMemberExitedMessageReceived: function(conversationType, channel, from, to, operatorId, memberId, groupId, groupName, groupAvatar, msgId, msgSeq, msgTime){
        },
        OnGroupEnded: function(errorCode, groupId, operatorId){},
        /**
         * event for group end
         * @param conversationType
         * @param channel
         * @param from
         * @param to
         * @param operatorId
         * * @param msgId
         * @param msgSeq
         * @param msgTime
         * @constructor
         */
        OnGroupEndMessageReceived: function(conversationType, channel, from, to, operatorId, msgId, msgSeq, msgTime){
        },
        OnGroupMemberAdded: function(errorCode, groupId, operatorId, memberIds){},
        /**
         * event for received member added to group from server
         * @param conversationType
         * @param channel
         * @param from
         * @param to
         * @param operatorId
         * @param groupId
         * @param groupName
         * @param groupAvatar
         * @param members       member array, each member is {userId: xx, joinTime: xx}
         * @param msgId
         * @param msgSeq
         * @param msgTime
         * @constructor
         */
        OnGroupMemberAddedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){
        },
        OnGroupMemberKicked: function(errorCode, groupId, operatorId, memberIds){},
        /**
         * event for receive kick member from group
         * @param conversationType
         * @param channel
         * @param from
         * @param to
         * @param operatorId
         * @param groupId
         * @param groupName
         * @param groupAvatar
         * @param members           userId array
         * @param msgId
         * @param msgSeq
         * @param msgTime
         * @constructor
         */
        GroupMemberKickedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){
        },
        OnGroupNameUpdated: function(errorCode, groupId, operatorId, groupName, pinyin){},
        /**
         * event for update group name
         * @param conversationType
         * @param channel
         * @param from
         * @param to
         * @param operatorId
         * @param groupId
         * @param groupName
         * @param namePinYin
         * @param msgId
         * @param msgSeq
         * @param msgTime
         * @constructor
         */
        OnGroupNameUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime){
        },
        OnGroupAvatarUpdated: function(errorCode,groupId, logoURL){},
        /**
         * event for update group avatar
         * @param conversationType
         * @param channel
         * @param from
         * @param to
         * @param operatorId
         * @param groupId
         * @param groupAvatar
         * @param msgId
         * @param msgSeq
         * @param msgTime
         * @constructor
         */
        OnGroupAvatarUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupAvatar, msgId, msgSeq, msgTime){},
        OnDisturbSet: function(errorCode, groupId, isDisturb){},
        /**
         * event for set message disturb statuc
         * @param conversationType
         * @param channel
         * @param from
         * @param to
         * @param groupId
         * @param state             message notification state: 0: do not notify, 1: notify
         * @param msgId
         * @param msgSeq
         * @param msgTime
         * @constructor
         */
        OnGroupDisturbSetMessageReceived: function(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime){},
        OnTopChatSet: function(errorCode, groupId, isTop){},
        /**
         * event for set group on top
         * @param conversationType
         * @param channel
         * @param from
         * @param to
         * @param operatorId
         * @param groupId
         * @param isTop
         * @param msgId
         * @param msgSeq
         * @param msgTime
         * @constructor
         */
        OnGroupOnTopMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, isTop, msgId, msgSeq, msgTime){},
        OnGroupReminderSet: function(errorCode, groupId, isAlert, remindRule, remindTime){},
        OnGroupReminderSetMessageReceived: function(conversationType, channel, from, to, groupId, state, rule, time, msgId, msgSeq, msgTime){},
        OnGroupListReceived: function(errorCode, groupType, groups){},
        OnGroupInfoReceived: function(errorCode, groupId, mountId,group){},
        OnDocListReceived: function(errorCode, groupId, documents){},
        OnDocInfoReceived: function(errorCode, groupId, document){},
        OnDocumentUploaded: function(errorCode, groupId, operatorId, document, msgId){},
        OnDocumentUploadMessageReceived: function(conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username, localPath){},
        OnDocumentShared: function(errorCode, groupId, operatorId, document){},
        OnDocumentShareMessageReceived: function(conversationType, channel, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username){},
        OnDocumentDeleted: function(errorCode, groupId, operatorId, docName, fileId){},
        OnDocumentDeleteMessageReceived: function(conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime){},

        OnCommentListReceived: function(errorCode, groupId, comments){},
        OnCommentCreated: function(errorCode, groupId, comment,msgId){},
        OnCommentCreateMessageReceived:function(conversationType, channel, from, to, msgId, msgSeq, fileId, fileType, fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType){},
        OnCodeSnippetUploaded:function(errorCode, groupId, operatorId, document, msgId){},
        OnCodeSnippetUploadMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, creatorId, creatorName, codeId, codeTitle,
                                                     langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp){},
        OnGroupSummaryVideoMessageReceived:function(conversationType,channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, fileName, startTime, updateTime,type,videoName,fileURL,thumbnails,thumbnailsURL,duration,fileSize,operatorName,operatorAvatar,msgId, msgSeq, msgTime){},
        OnGroupUnreadUserCountEvent:function(errorCode,type,userId,info){},
        OnGroupUnreadUserListEvent:function(errorCode,type,userId,info){},
        OnGroupCloudFileMessageReceived: function(conversationType, channel, sendState, from, to, userId, fileId, fileName,fileSize, fileUrl, fileFormat, description, msgId,msgSeq, msgTime, username,detailContent){},
        OnGroupMemberRoleUpdatedMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, msgTime,roles){},
        OnGroupReportMessageReceived: function(conversationType, channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {},
        OnAtMessageListReceived:function(errorCode, groupId, atMsg){},
        OnAtMessageDeleted:function(errorCode, groupId, atSeq){},
        // added by xue.bai_2 on 2016/04/07
        OnRevocationMessageReceived: function(channel, conversation, conversationType, from, to, msgId, msgSeq, msgTime, seqId){},
        OnRevocationMessage: function(errorCode, groupId, msgId, msgSeq, msgTime, revocationType){}
    }
})(uc);