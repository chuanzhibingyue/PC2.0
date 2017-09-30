#ifndef GK_CLIENT_H_
#define GK_CLIENT_H_

#include "IClientService.h"
#include "IGroupService.h"
#include "CefClient.h"
#include "../utils/NetworkMonitor.h"
#include "gkclient/GKCallbackEx.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "GkClientMessage.h"
#include "GkClientTask.h"
#include <string>
#include <Windows.h>
#include <memory>
#include "base/timer.h"

namespace ucclient {

using namespace uc;

enum GkClientConnection{
	GKC_SUCCESS = 0,
	GKC_NOT_START = 1,
	GKC_CONNECTING = 2,
	GKC_ERROR = 3,
};

class GkClient : public IClientServiceEvent, 
				public IGroupServiceEvent, 
				public NetworkListener,
				public GKCallbackEx {
public:
	typedef rapidjson::GenericDocument<rapidjson::UTF16<> >									 WDocument;
	typedef rapidjson::GenericValue<rapidjson::UTF16<> >									 WValue;
	typedef rapidjson::GenericStringBuffer<rapidjson::UTF16<> >								 WStringBuffer;
	typedef rapidjson::Writer<WStringBuffer, rapidjson::UTF16<>, rapidjson::UTF16<> >		 WWriter;

	GkClient(cefIntegration::CefClient *cefClient);
	virtual ~GkClient();

	void GkClientLogin(const std::wstring& token);
	void OnGkClientLogin(int errorCode);
	GkClientConnection GetGkClientConnection() { return _gkClientConnection; }
	void SetProxyAuthInfo(int32_t type, const wstring& account, const wstring& password);
	void GkClientLogin();

	// GKCallbackEx
	virtual void SetUserMessage(const wstring& content);

	// IClientServiceEvent
    virtual void OnLogin(int32_t errorCode, int32_t authType, int32_t cloudDiskType, const std::string &capability = "");
	virtual void OnAccessTokenReceived(int32_t errorCode, OauthGrantType grantType, const std::string &accessToken);
	virtual void OnLogout(int32_t errorCode);
	
	virtual void OnPasswordModified(int32_t errorCode) {}
	virtual void OnPasswordModifyMessageReceived() {}
	virtual void OnPasswordRuleUpdateMessageReceived(int32_t type) {}
	virtual void OnPasswordExpiredMessageReceived(int32_t dueDays) {}
	virtual void OnUCASConnected(int32_t errorCode) {}
	virtual void OnUCASDisconnected(int32_t errorCode) {}
	virtual void OnConversationListReceived(int32_t errorCode, const std::list<ConversationInfo*> *conversations, bool isLocal, int32_t totalUnread, std::map<int64_t, int32_t>& groupAtCounts) {}
	virtual void OnConversationTopSet(int32_t errorCode, ConversationType type, int64_t relateId, int32_t state, int64_t index){}
	virtual void OnUserLoginNotify(int32_t errorCode, int64_t userId, DeviceType::type deviceType, std::string deviceName){}
	virtual void OnAccountVerified(int32_t errorCode, int32_t verifyType, const std::string &mobile, const std::string &email, int32_t exprised) {}
	virtual void OnCodeVerified(int32_t errorCode, int32_t exprised) {}
	virtual void OnPasswordReset(int32_t errorCode) {}
	virtual void OnLogUploaded(int32_t errorCode) {}
	virtual void OnSessionExpired() {}
	virtual void OnResendCodeComplete(int32_t errorCode, int32_t exprised){}
	virtual void OnConversationListComplete(int32_t conversationSize){}
	virtual void OnArchiveLogFinish(std::string filePath){}
	virtual void OnUpdateUserNotifySet(int32_t errorCode, int64_t userId){}
	virtual void OnGetUserNotifyStatus(int32_t errorCode, int16_t isNotify){}

	// IGroupServiceEvent
	virtual void OnGroupInfoReceived(int32_t result, int64_t groupId, int32_t mountId, const GroupInfo& info);
	virtual void OnGroupEnded(int32_t result, int64_t groupId, int32_t operatorId){}
	virtual void OnMemberAdded(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<UserInfo>& memberList){}
	virtual void OnMemberRemoved(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<int32_t>& memberId){}
	virtual void OnGroupNameUpdated(int32_t result, int64_t groupId, int32_t operatorId, const std::string& groupName, const std::string& pinYinName){}
	virtual void OnGroupCreated(int32_t result, int32_t operatorId, const GroupInfo& info);
	virtual void OnGroupLogoUpdated(int32_t result, int64_t groupId, const std::string& logoURL){}
	virtual void OnGroupCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const GroupInfo& group);
	virtual void OnAdminSet(int32_t result, int64_t groupId, SetAdminType type, const std::vector<int32_t>& adminList){}
	virtual void OnGroupListReceived(int32_t result, GroupStyle groupType, bool allData, const std::list<GroupInfo>& info){}
	virtual void OnAtMessageListReceived(int32_t result, int64_t groupId, const std::map<int64_t, int64_t>& atMsg){};
	virtual void OnAtMessageDeleted(int32_t result, ConversationType type, int64_t groupId, const std::vector<int64_t>& seqList){};
	
	virtual void OnRecordVideoReceived(MessageChannel channel, const MediaMessageInfo *message){}
	virtual void OnChatHistoryReceived(int32_t result, MessageChannel channel, ConversationType type, int64_t groupId, int32_t msgCount, int64_t min_timestamp){}
	virtual void OnSearchChatHistoryReceived(int32_t result, ConversationType type, int64_t relateId, int32_t msgCount, int64_t min_timestamp, int64_t max_timestamp){}
	virtual void OnMessageSent(int32_t result, ConversationType type, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime){}
	virtual void OnTextMessageReceived(MessageChannel channel, const TextMessageInfo *message){}
	virtual void OnImageMessageReceived(MessageChannel channel, const MediaMessageInfo *message){}
	virtual void OnAudioMessageReceived(MessageChannel channel, const MediaMessageInfo *message){}
	virtual void OnVideoMessageReceived(MessageChannel channel, const MediaMessageInfo *message){}
	virtual void OnReportMessageReceived(MessageChannel channel, const ReportMessageInfo *message){}
	virtual void OnCloudFileModifiedMessageReceived(MessageChannel channel, CloudFileOperation operation, const FileMessageInfo &message){}
	virtual void OnGroupCloseMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId){}
	virtual void OnGroupMemberDelMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
		int32_t memberId, const std::string &avatar, const std::string &groupName, const std::string &namePinYin, int8_t nameFlag){}
	virtual void OnGroupMemberAddMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
		const std::vector<UserInfo> &memberList, const std::string &avatar, const std::string &groupName,
		const std::string &namePinYin, int8_t nameFlag){}
	virtual void OnGroupMemberKickMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
		const std::vector<int32_t> &memberList, const std::string &avatar, const std::string &groupName, const std::string &namePinYin,
		int8_t nameFlag){}
	virtual void OnGroupNameUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
		const std::string &groupName, const std::string &namePinYin, int8_t nameFlag){}
	virtual void OnGroupDisturbSetMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId, int8_t state){}
	virtual void OnGroupTopMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
		int8_t isTop){}
	virtual void OnGroupAvatarUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
		const std::string &avatar, int32_t groupId){}
	virtual void OnGroupStatusInfoUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId,
		int32_t status, const std::string &statusInfo){}
	virtual void OnGroupReminderSetMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId,
		int8_t state, int32_t rule, int32_t time){}
	virtual void OnDocumentUploadMessageReceived(MessageChannel channel, int32_t operation, const MessageInfo &message, const DocumentInfo &doc){}
	virtual void OnCloudFileCreateMessageReceived(MessageChannel channel, const FileMessageInfo &message){}
	virtual void OnDocumentDeleteMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
		int64_t contentId, const std::string &docName){}
	virtual void OnCodeSnippetUploadMessageReceived(MessageChannel channel, int32_t operation, const MessageInfo &message, const DocumentInfo &doc){}
	virtual void OnCommentCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const CommentInfo& commentInfo){}
	virtual void OnMessageRead(int32_t result, int64_t groupId, int32_t readType, int32_t msgId, int64_t msgSeq){}
	virtual void OnMessagePlayed(int32_t result, int64_t groupId, int32_t msgId, int64_t msgSeq){}
	virtual void OnCallInvited(int32_t result, ConversationType type, int64_t groupId, int32_t msgId,
		int64_t msgSeq, const std::string confId, const std::string confPwd, int32_t hostId){}
	virtual void OnCallAccepted(int32_t result, ConversationType type, int64_t groupId, const std::string confId){}
	virtual void OnCallRejected(int32_t result, ConversationType type, int64_t groupId, const std::string confId){}
	virtual void OnCallStopped(int32_t result, ConversationType type, int64_t groupId, const std::string confId){}
	virtual void OnCallConnected(int32_t result, ConversationType type, int64_t groupId, int32_t duration){}
	virtual void OnGroupExited(int32_t result, int64_t groupId, int32_t userId){}
	virtual void OnDisturbSet(int32_t result, int64_t groupId, int32_t isDisturb){}
	virtual void OnTopChatSet(int32_t result, int64_t groupId, int32_t isTop){}
	virtual void OnReminderSet(int32_t result, int64_t groupId, int32_t isAlert, int32_t remindRule, int32_t remindTime){}
	virtual void OnDocListReceived(int32_t result, int64_t groupId, const std::list<DocumentInfo>& info){}
	virtual void OnDocInfoReceived(int32_t result, int64_t groupId, const DocumentInfo& info){}
	virtual void OnDocumentUploaded(int32_t result, int64_t groupId, int32_t operatorId, int32_t operation, const DocumentInfo& info){}
	virtual void OnCodeSnippetUploaded(int32_t result, int64_t groupId, int32_t operatorId, int32_t operation, const DocumentInfo& info){}
	virtual void OnDocumentDeleted(int32_t result, int64_t groupId, int32_t operatorId, const std::string& docName, int64_t fileId){}
	virtual void OnCommentListReceived(int32_t result, int64_t groupId, int64_t fileId, const std::list<CommentInfo>& info){}
	virtual void OnCommentCreated(int32_t result, int64_t groupId, const CommentInfo& info){}
	virtual void OnMessageRead(int32_t result, ConversationType type, int64_t groupId, int32_t readType, int32_t msgId, int64_t msgSeq){}
	virtual void OnMemberRoleUpdated(MessageChannel channel, const RoleUpdatedMessageInfo &info){}
	virtual void OnMessageUnreadUserCount(int32_t result, ConversationType type, int64_t userId, const std::list<MessageReadInfo>& info){}
	virtual void OnMessageUnreadUserList(int32_t result, ConversationType type, int64_t userId, const std::list<MessageReadUserInfo>& info){}
	virtual void OnCallInvited(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime,
		const std::string confId, const std::string confPwd, int32_t hostId){}
	virtual void OnCallAccepted(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId, const std::string confId, int32_t msgId, int64_t msgSeq, int64_t msgTime){}
	virtual void OnCallRejected(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId,
		const std::string confId, int32_t hostId, std::string reason,
		int32_t msgId, int64_t msgSeq, int64_t msgTime){}
	virtual void OnCallStopped(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId, const std::string confId,
		int32_t msgId, int64_t msgSeq, int64_t msgTime){}
	virtual void OnCallConnected(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId, int32_t duration, const std::string& phoneNum,
		int32_t msgId, int64_t msgSeq, int64_t msgTime){}
	virtual void OnCallCanceled(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId,
		int32_t msgId, int64_t msgSeq, int64_t msgTime, const std::string& phoneNum){}

	virtual void OnRevocationMessage(int32_t result, int64_t groupId, ConversationType type, int64_t msgSeq, int32_t msgId, int64_t msgTime){}
	virtual void OnRevocationMesssageReceived(MessageChannel channel, const MessageInfo &message, int64_t seqId, int16_t type){}

	virtual void OnMeetingMessageRevocation(int32_t result, int64_t msgSeq, int32_t msgId, int64_t msgTime, int64_t eventId){}
	virtual void OnMeetingMessageRevocationRecevied(MessageChannel channel, const MessageInfo &message, int64_t eventId, int64_t seqId){}
	// NetworkListener
	virtual void OnNetWorkChanged(ConnectionState state);
private:
	static DWORD WINAPI ThreadProc(LPVOID lpParam);
	void SetUserMessageInternal(GkClientTask &task);
	void SetUserMessageIfGroupExists(GkClientTask &task);

	void ProcessGkMessage(GkClientMessage& message, const wstring& content);
	void ProcessGkFileMessage(GkClientTask &task);

	void NotifyResultToGk(std::shared_ptr<GkClientTask> task);
	void NotifyUserMessageResult(GkClientTask& task);

	void AddGkClientTask(std::shared_ptr<GkClientTask> task);
	void RemoveGkClientTask(std::shared_ptr<GkClientTask> task);

	bool MountIdNotAssociated(int32_t mountId);
	void AddNotAssociatedMountId(int32_t mountId);
	void RemoveNotAssociatedMountId(int32_t mountId);

	CloudFileOperation GetCloudFileOperation(const std::wstring &type);
	GkClientTaskType GetTaskType(WDocument & doc);

	bool IsLogin();
	bool CanLogin(int32_t loginCode, int32_t cloudDiskType);
	bool CanLoginEx(int32_t loginCode);
	void CancelLogin();

private:
	cefIntegration::CefClient *_cefClient;
	std::wstring _token;

	GkClientConnection _gkClientConnection;
	int32_t _curUserId;

	std::list<std::shared_ptr<GkClientTask>> _gkClientTasks;
	std::set<int32_t> _nonAssociatedMountIds;
	CRITICAL_SECTION _cs;

	timer_id _timerId;
};

}

#endif
