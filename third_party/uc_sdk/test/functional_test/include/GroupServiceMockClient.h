#ifndef GROUP_SERVICE_MOCK_CLIENT_H_
#define GROUP_SERVICE_MOCK_CLIENT_H_

#include "IGroupService.h"
#include "UCMockBase.h"

using namespace uc;

class GroupServiceMockClient : public IGroupServiceEvent, public UCMockBase{

public:
	GroupServiceMockClient();
	~GroupServiceMockClient();
	
	//interface;
	void Init(IUCEngine *ucEngine); 
	void UnInit();
	void GetGroupList(GroupStyle groupType);
	void GetGroupInfo(int64_t groupid);
	void CreateGroup(GroupStyle groupType, const std::string& groupName, std::vector<UserInfo>& userInfo,
		int32_t nameFlag);
	void EndGroup(int64_t gourpId);
	void ExitGroup(int64_t gourpId);
	void AddMember(int64_t groupId, std::vector<UserInfo>& memberList);
	void RemoveMember(int64_t groupId, const std::vector<int32_t>& memberList);
	void SetGroupName(int64_t groupId, const std::string& groupName);
	void SetGroupLogo(int64_t groupId, const std::string& logoURL);
	void SetTopChat(int64_t groupId, int32_t state);
	void SetReminder(int64_t groupId, int32_t state, int32_t alertRule, int32_t alertTime);
	void SetDoNotDisturb(int64_t groupId, int32_t state);
	void GetDocumentList(int64_t groupId);
	void GetDocumentInfo(int64_t groupId, int64_t fileId);
	void UploadDocument(int64_t groupId, const std::string& fileName, int32_t flieSize,
		const std::string& fileURL, int64_t uploadTime);
	void ShareDocument(int64_t groupId, const std::string& fileName, int32_t flieSize,
		const std::string& fileURL, int64_t uploadTime);
	void DeleteDocument(int64_t groupId, int64_t fileId);
	void GetChatHistory(ConversationType type, int64_t groupId, int32_t count, int32_t startTime, int32_t endTime);
	void ResendMessage(int32_t msgId);
	void SendText(ConversationType type, UCID groupUid, int8_t textType, std::string& textFromat, std::string& textData, OUT int32_t& msgId);
	void SendImage(ConversationType type, UCID groupUid, std::string& imageName, int32_t imageSize,
		std::string& imageURL, std::string& imageData, OUT int32_t& msgId);
	void SendAudio(ConversationType type, UCID groupUid, std::string& audioName, int32_t audioSize,
		std::string& audioURL, int32_t audioDuration, OUT int32_t& msgId);
	void SendVideo(ConversationType type, UCID groupUid, std::string& videoName, int32_t videoSize,
		std::string& videoURL, std::string& videoData, int32_t videoDuration, OUT int32_t& msgId);
	void SendInvateCall(ConversationType type, UCID groupUid, const std::string& confId,
		const std::string& confPwd, int32_t hostId, OUT int32_t& msgId);
	void SendAppectCall(ConversationType type, UCID groupUid, const std::string& confId, OUT int32_t& msgId);
	void SendRejectCall(ConversationType type, UCID groupUid, const std::string& confId, OUT int32_t& msgId);
	void SendFinishCall(ConversationType type, UCID groupUid, const std::string& confId, OUT int32_t& msgId);
	void SendConnectCallRecord(ConversationType type, UCID groupUid, int32_t druation, OUT int32_t& msgId);
	void SendMessageRead(ConversationType type, UCID groupUid, int32_t msgId, int64_t msgSeq);
	void SendMessagesRead(int64_t groupId, std::vector<int32_t> chatId);
	void SendMessagesPalyed(ConversationType type, UCID groupUid, int32_t msgId, int64_t msgSeq);


	
	// Event;
	virtual void OnChatHistoryReceived(int32_t result, int64_t groupId, const std::list<MessageInfo*>& msgList);
	virtual void OnMessageSent(int32_t result, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime);
	virtual void OnTextMessageReceived(const TextMessageInfo *message);
	virtual void OnImageMessageReceived(const MediaMessageInfo *message);
	virtual void OnAudioMessageReceived(const MediaMessageInfo *message);
	virtual void OnVideoMessageReceived(const MediaMessageInfo *message);
	virtual void OnMessageRead(int32_t result, int64_t groupId, int32_t readType, int32_t msgId, int64_t msgSeq);
	virtual void OnMessagePlayed(int32_t result, int64_t groupId, int32_t msgId, int64_t msgSeq);
	virtual void OnCallInvited(int32_t result, ConversationType type, int64_t groupId, int32_t msgId, int64_t msgSeq, const std::string confId, const std::string confPwd, int32_t hostId);
	virtual void OnCallAccepted(int32_t result, ConversationType type, int64_t groupId, const std::string confId);
	virtual void OnCallRejected(int32_t result, ConversationType type, int64_t groupId, const std::string confId);
	virtual void OnCallStopped(int32_t result, ConversationType type, int64_t groupId, const std::string confId);
	virtual void OnCallConnected(int32_t result, ConversationType type, int64_t groupId, int32_t duration);
	virtual void OnGroupCreated(int32_t result, int32_t operatorId, const GroupInfo *info);
	virtual void OnGroupExited(int32_t result, int64_t groupId, int32_t userId);
	virtual void OnGroupEnded(int32_t result, int64_t groupId, int32_t operatorId);
	virtual void OnMemberAdded(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<int32_t>& memberList);
	virtual void OnMemberRemoved(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<int32_t>& memberList);
	virtual void OnGroupNameUpdated(int32_t result, int64_t groupId, int32_t operatorId, 
		const std::string& groupName, const std::string& pinYinName);
	virtual void OnGroupLogoUpdated(int32_t result, int64_t groupId, const std::string& logoURL);
	virtual void OnDisturbSetted(int32_t result, int64_t groupId, int32_t isDisturb);
	virtual void OnTopChatSetted(int32_t result, int64_t groupId, int32_t isTop);
	virtual void OnReminderSetted(int32_t result, int64_t groupId, int32_t isAlert, int32_t remindRule, int32_t remindTime);
	virtual void OnGroupListReceived(int32_t result, GroupStyle groupType, const std::list<GroupInfo*>& groupList);
	virtual void OnGroupInfoReceived(int32_t result, int64_t groupId, const GroupInfo *info);
	virtual void OnDocListReceived(int32_t result, int64_t groupId, const std::list<DocumentInfo*>& info);
	virtual void OnDocInfoReceived(int32_t result, int64_t groupId, const DocumentInfo *info);
	virtual void OnDocumentUploaded(int32_t result, int64_t groupId, int32_t operatorId, const DocumentInfo *info);
	virtual void OnDocumentShared(int32_t result, int64_t groupId, int32_t operatorId, const DocumentInfo *info);
	virtual void OnDocumentDeleted(int32_t result, int64_t groupId, int32_t operatorId, 
		const std::string& docName, int64_t fileId);

private:
	IGroupService *_groupService;
	


};



#endif //GROUP_SERVICE_MOCK_CLIENT_H_
