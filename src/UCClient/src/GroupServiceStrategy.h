/*
 * GroupServiceStrategy.h
 *
 *  Created on: July 13, 2015
 *      Author: gang wang
 */

#ifndef GROUP_SERVICE_STRATEGY_H_
#define GROUP_SERVICE_STRATEGY_H_

#include "IGroupService.h"
#include "CefClient.h"

namespace ucclient {

using namespace uc;

class GroupServiceStrategy : public IGroupServiceEvent {
public:
	GroupServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);

	int32_t GetChatHistory(MessageChannel channel, ConversationType type, int64_t groupId, int32_t count, int64_t startTime, int64_t endTime);

	int32_t GetSearchChatHistory(ConversationType type, int64_t relateId, int32_t count, int64_t startTime, int64_t endTime, int32_t reverse);

	int32_t ResendMessage(int32_t msgId);

	int32_t ForwardMessage(UCID& userUid, int32_t msgId);

	int32_t SendTextMessage(ConversationType type, UCID groupId, int8_t textType, std::wstring& textFormat, std::wstring& textData, std::vector<UCID>& atUsers, OUT int32_t& msgId);

	int32_t RevocationMessage(int64_t& touserId, ConversationType type, const int64_t& seqId, OUT int32_t& msgId);

	int32_t SendTextMessage(ConversationType type, UCID groupId, int8_t textType, std::wstring& textFormat, std::wstring& textData, OUT int32_t& msgId);

	int32_t SendImageMessage(ConversationType type, UCID groupId, std::wstring& imageName, int32_t imageSize,
		std::wstring& imageURL, std::wstring& imagePath, OUT int32_t& msgId);

	int32_t SendAudioMessage(ConversationType type, UCID groupId, std::wstring& audioName, int32_t audioSize,
	 std::wstring& audioURL, int32_t audioDuration, OUT int32_t& msgId);

	int32_t SendVideoMessage(ConversationType type, UCID groupId, std::wstring& videoName, int32_t videoSize,
		std::wstring& videoURL, std::wstring& videoPath, int32_t videoDuration, OUT int32_t& msgId);

	int32_t SendReportMessage(ConversationType type, UCID groupUid, const std::wstring& reportTitle, const std::wstring& reportUrl, OUT int32_t& msgId);
	
	int32_t SendRecordVideoMessage(ConversationType type, UCID& groupId, const std::wstring& videoName, int32_t videoSize, const std::wstring& videoURL, 
		const std::wstring& videoThumb, const std::wstring& videoThumbURL, int32_t videoDuration, int32_t operatorId, const std::wstring& operatorName, const std::wstring& operatorAvatar, int32_t& msgId);
	
	int32_t SendCloudFileMessage(ConversationType type, CloudFileOperation operation, UCID groupUid, const std::wstring& fileName, const std::wstring& fileNameTo, 
		int64_t fileSize, int8_t count, int8_t isDir, int8_t cloudType, const std::wstring& detailContent, 
		bool isSave, OUT int32_t& msgId);

	int32_t SendInviteCallMessage(ConversationType type, UCID groupId, const std::wstring& confId, 
		const std::wstring& confPwd, int32_t hostId, OUT int32_t& msgId);

	int32_t SendAcceptCallMessage(ConversationType type, UCID groupId, const std::wstring& confId, int32_t hostId, OUT int32_t& msgId);

	int32_t SendRejectCallMessage(ConversationType type, UCID groupId, const std::wstring& confId, int32_t hostId, const std::wstring& reason, OUT int32_t& msgId);

	int32_t SendFinishCallMessage(ConversationType type, UCID groupId, const std::wstring& confId, int32_t hostId, const std::wstring& reason, OUT int32_t& msgId);


	int32_t SendCancelCallMessage(ConversationType type, UCID groupUid, const string& phoneNum, OUT int32_t& msgId);

	//int32_t SendNotRespondCallRecordMessage(ConversationType type, UCID groupId, OUT int32_t& msgId);

	//int32_t SendRejectCallRecordMessage(ConversationType type, UCID groupId, OUT int32_t& msgId);

	
	int32_t SendConnectedCallRecordMessage(ConversationType type, UCID groupId, int32_t druation, const std::string& phoneNum, OUT int32_t& msgId);

    int32_t SendMessageRead(ConversationType type, UCID groupId, int32_t msgId, int64_t msgSeq);

	int32_t SendMessagesReadById(ConversationType type, int64_t groupId);

    int32_t SendMessagePlayed(ConversationType type, UCID groupId, int32_t msgId, int64_t msgSeq);

	int32_t GetUnreadUserList(ConversationType type, int64_t groupId, std::list<int64_t> seq);

	int32_t GetGroupList(GroupStyle groupType, int32_t page, int32_t count, int32_t lastGroupTime, bool allData);

	int32_t GetGroupInfo(int64_t groupId, GroupInfo &group, bool forced = false, int32_t mountId = 0);

	int32_t CreateGroup(GroupStyle groupType, const std::wstring& groupName, const std::wstring& logourl, 
		std::vector<UserInfo>& userInfo, int32_t nameFlag, int32_t memberType = 0, int32_t mountId = 0);

	int32_t EndGroup(int64_t groupId);

	int32_t ExitGroup(int64_t groupId);

	int32_t AddMember(int64_t groupId, const std::vector<UserInfo>& userInfo);

	int32_t SetAdmin(int64_t groupId, SetAdminType type, const std::vector<int32_t>& adminList);

	int32_t RemoveMember(int64_t groupId, const std::vector<int32_t>& members);

	int32_t SetGroupName(int64_t groupId, const std::wstring& groupName);

	int32_t GetDocumentList(int64_t groupId, int32_t page, int32_t count);

	int32_t UploadCloudDocument(int64_t groupId, int32_t operation, int32_t isSendMessage, int32_t mountId, const std::wstring& fileName,
		int64_t fileSize, DocumentCloudType cloudType, const std::wstring& hashCode, const std::wstring& detailContent, OUT int32_t& msgId);

	int32_t BatchUploadCloudDocument(int64_t groupId, int32_t operation, int32_t isSendMessage, int32_t mountId, const std::wstring& fileName,
		int64_t fileSize, DocumentCloudType cloudType, const std::wstring& hashCode, const std::wstring& detailContent);

	int32_t UploadDocument(int64_t groupId, int32_t operation, const std::wstring& fileName, int64_t fileSize,
		const std::wstring& fileURL, OUT int32_t& msgId);

	int32_t UploadCodeSnippet(int64_t groupId, int32_t operation, const std::wstring& codeTitle, const std::wstring& langType, const std::wstring& codeHead, 
		const std::wstring& codeDesc, int32_t codeSize, const std::wstring& codeUrl, int32_t codeLine, OUT int32_t& msgId);

	int32_t DeleteDocument(int64_t groupId, int64_t fileId);

	int32_t GetDocumentInfo(int64_t groupId, int64_t fileId, const std::wstring &cloudFileInfo);

	int32_t GetCommentList(int64_t groupId, int64_t fileId, int32_t count, int64_t startTime, int64_t endTime);

	int32_t GetAtMessageList(int64_t groupId, int32_t page, int32_t count);

	int32_t DeleteAtMessage(ConversationType type, int64_t groupId, std::vector<int64_t> seqList);

	int32_t CreateComment(int64_t groupId, int64_t fileId, int32_t fileType, const std::wstring& fileTitle, 
		const std::wstring content, int32_t &msgId);

	int32_t SetGroupLogo(int64_t groupId, const std::wstring& logoURL);

	int32_t UpdateGroupLogo(int64_t groupId, const std::wstring& localPath);

	int32_t SetTopChat(int64_t groupId, int32_t state);

	int32_t SetDoNotDisturb(int64_t groupId, int32_t state);

	int32_t SetReminder(int64_t groupId, int32_t state, int32_t alertRule, int32_t alertTime);


	virtual void OnChatHistoryReceived(int32_t result, MessageChannel channel, ConversationType type, int64_t groupId, int32_t msgCount, int64_t min_timestamp);

	virtual void OnSearchChatHistoryReceived(int32_t result, ConversationType type, int64_t relateId, int32_t msgCount, int64_t min_timestamp, int64_t max_timestamp);

	virtual void OnMessageSent(int32_t result, ConversationType type, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnTextMessageReceived(MessageChannel channel, const TextMessageInfo *message);

	virtual void OnImageMessageReceived(MessageChannel channel, const MediaMessageInfo *message);

	virtual void OnAudioMessageReceived(MessageChannel channel, const MediaMessageInfo *message);

	virtual void OnVideoMessageReceived(MessageChannel channel, const MediaMessageInfo *message);

	virtual void OnReportMessageReceived(MessageChannel channel, const ReportMessageInfo *message);

	virtual void OnRecordVideoReceived(MessageChannel channel, const MediaMessageInfo *message);

	virtual void OnCloudFileModifiedMessageReceived(MessageChannel channel, CloudFileOperation operation, const FileMessageInfo &message);

	virtual void OnGroupCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const GroupInfo& group);

	virtual void OnGroupCloseMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId);

	virtual void OnGroupMemberDelMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			int32_t memberId, const std::string &avatar, const std::string &groupName, const std::string &namePinYin, int8_t nameFlag);

	virtual void OnGroupMemberAddMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
		const std::vector<UserInfo> &memberList, const std::string &avatar, const std::string &groupName,
			const std::string &namePinYin, int8_t nameFlag);

	virtual void OnGroupMemberKickMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			const std::vector<int32_t> &memberList, const std::string &avatar, const std::string &groupName, const std::string &namePinYin,
			int8_t nameFlag);

	virtual void OnGroupNameUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			const std::string &groupName, const std::string &namePinYin, int8_t nameFlag);

	virtual void OnGroupDisturbSetMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId, int8_t state );

	virtual void OnGroupTopMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			int8_t isTop);

	virtual void OnGroupAvatarUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
			const std::string &avatar, int32_t groupId);

	virtual void OnGroupStatusInfoUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId,
		int32_t status, const std::string &statusInfo);

	virtual void OnGroupReminderSetMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId,
			int8_t state, int32_t rule, int32_t time);

	virtual void OnDocumentUploadMessageReceived(MessageChannel channel, int32_t operation, const MessageInfo &message, const DocumentInfo &doc);

	virtual void OnCloudFileCreateMessageReceived(MessageChannel channel, const FileMessageInfo &message);

	virtual void OnDocumentDeleteMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
			int64_t contentId, const std::string &docName);

	virtual void OnCodeSnippetUploadMessageReceived(MessageChannel channel, int32_t operation, const MessageInfo &message, const DocumentInfo &doc);

	virtual void OnCommentCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const CommentInfo& commentInfo);

	virtual void OnMessageRead(int32_t result, ConversationType type, int64_t groupId, int32_t readType, int32_t msgId, int64_t msgSeq);

	virtual void OnMessagePlayed(int32_t result, int64_t groupId, int32_t msgId, int64_t msgSeq);

	virtual void OnMessageUnreadUserCount(int32_t result, ConversationType type, int64_t groupId, const std::list<MessageReadInfo>& info);

	virtual void OnMessageUnreadUserList(int32_t result, ConversationType type, int64_t groupId, const std::list<MessageReadUserInfo>& info);

	virtual void OnCallInvited(MessageChannel channel, int32_t result, ConversationType type, 
		int32_t userId, int64_t groupId, int32_t msgId,
		int64_t msgSeq, int64_t msgTime, const std::string confId, const std::string confPwd, int32_t hostId);

	virtual void OnCallAccepted(MessageChannel channel, int32_t result, ConversationType type, 
		int32_t userId, int64_t groupId, const std::string confId, int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnCallRejected(MessageChannel channel, int32_t result, ConversationType type, 
		int32_t userId, int64_t groupId, const std::string confId, int32_t hostId, std::string reason,
		int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnCallStopped(MessageChannel channel, int32_t result, ConversationType type, 
		int32_t userId, int64_t groupId, const std::string confId,
		int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnCallConnected(MessageChannel channel, int32_t result, ConversationType type, 
		int32_t userId, int64_t groupId, int32_t duration, const std::string& phoneNum,
		int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnCallCanceled(MessageChannel channel, int32_t result, ConversationType type, 
		int32_t userId, int64_t groupId,
		int32_t msgId, int64_t msgSeq, int64_t msgTime, const std::string& phoneNum);

	virtual void OnGroupCreated(int32_t result, int32_t operatorId, const GroupInfo& info);

	virtual void OnGroupExited(int32_t result, int64_t groupId, int32_t userId);

	virtual void OnGroupEnded(int32_t result, int64_t groupId, int32_t operatorId);

	virtual void OnMemberAdded(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<UserInfo>& memberId);
	virtual void OnAdminSet(int32_t result, int64_t groupId, SetAdminType type, const std::vector<int32_t>& adminList){}

	virtual void OnMemberRemoved(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<int32_t>& memberId);

	virtual void OnGroupNameUpdated(int32_t result, int64_t groupId, int32_t operatorId, const std::string& groupName, const std::string& pinYinName);

	virtual void OnGroupLogoUpdated(int32_t result, int64_t groupId, const std::string& logoURL);

	virtual void OnDisturbSet(int32_t result, int64_t groupId, int32_t isDisturb);

	virtual void OnTopChatSet(int32_t result, int64_t groupId, int32_t isTop);

	virtual void OnReminderSet(int32_t result, int64_t groupId, int32_t isAlert, int32_t remindRule, int32_t remindTime);

	virtual void OnGroupListReceived(int32_t result, GroupStyle groupType, bool allData, const std::list<GroupInfo>& info);

	virtual void OnGroupInfoReceived(int32_t result, int64_t groupId, int32_t mountId, const GroupInfo& info);

	virtual void OnDocListReceived(int32_t result, int64_t groupId, const std::list<DocumentInfo>& info);

	virtual void OnDocInfoReceived(int32_t result, int64_t groupId, const DocumentInfo& info);

	virtual void OnDocumentUploaded(int32_t result, int64_t groupId, int32_t operatorId, int32_t operation, const DocumentInfo& info);

	virtual void OnCodeSnippetUploaded(int32_t result, int64_t groupId, int32_t operatorId, int32_t operation, const DocumentInfo& info);

	virtual void OnDocumentDeleted(int32_t result, int64_t groupId, int32_t operatorId, const std::string& docName, int64_t fileId);

	virtual void OnCommentListReceived(int32_t result, int64_t groupId, int64_t fileId, const std::list<CommentInfo>& info);

	virtual void OnCommentCreated(int32_t result, int64_t groupId, const CommentInfo& info);

	virtual void OnMemberRoleUpdated(MessageChannel channel, const RoleUpdatedMessageInfo &info);

	virtual void OnAtMessageListReceived(int32_t result, int64_t groupId, const std::map<int64_t, int64_t>& atMsg);

	virtual void OnAtMessageDeleted(int32_t result, ConversationType type, int64_t groupId, const std::vector<int64_t>& seqList);

	virtual void OnRevocationMesssageReceived(MessageChannel channel, const MessageInfo &message, int64_t seqId, int16_t type);

	virtual void OnRevocationMessage(int32_t result, int64_t groupId, ConversationType type, int64_t msgSeq, int32_t msgId, int64_t msgTime);

private:
	cefIntegration::CefClient *_cefClient;
	IGroupService *_groupService;
	IUCEngine *_engine;
};

}

#endif