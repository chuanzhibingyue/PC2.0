#ifndef IM_SERVICE_MOCK_CLIENT_H_
#define IM_SERVICE_MOCK_CLIENT_H_

#include "IIMService.h"
#include "UCMockBase.h"

using namespace uc;

class IMServiceMockClient : public IIMServiceEvent, public UCMockBase{

public:
	IMServiceMockClient();
	~IMServiceMockClient();

	//interface;
	void Init(IUCEngine *ucEngine); 
	void UnInit();
	void GetChatHistory(int32_t userId, int32_t count, int32_t startTime, int32_t endTime);
	void ResendMessage(int32_t msgId);
	void SendTextMessage(UCID userUid, int8_t textType, std::string& textFromat, const std::string& textData, OUT int32_t& msgId);
	void SendImage(UCID userUid, const std::string& imageName, int32_t imageSize,
		const std::string& imageURL, const std::string& imageData, OUT int32_t& msgId);
	void SendAudioMessage(UCID userUid, const std::string& audioName, int32_t audioSize,
		const std::string& audioURL, int32_t audioDuration, OUT int32_t& msgId);
	void SendVideoMessage(UCID userUid, const std::string& videoName, int32_t videoSize,
		const std::string& videoURL, const std::string& videoData, int32_t videoDuration, OUT int32_t& msgId);
	void SendFileMessage(UCID userUid, const std::string& fileName, int32_t flieSize,
		const std::string& fileURL, int64_t fileId, OUT int32_t& msgId);
	void ShareFileMessage(UCID userUid, const std::string& fileName, int32_t flieSize,
		const std::string& fileURL, int64_t fileId, OUT int32_t& msgId);
	void DeleteFileMessage(UCID userUid, const std::string& fileName, int64_t fileId, OUT int32_t& msgId);
	void SendInviteCallMessage(UCID userUid, const std::string& confId,
		const std::string& confPwd, int32_t hostId, OUT int32_t& msgId);
	void SendAcceptCallMessage(UCID userUid, const std::string& confId, OUT int32_t& msgId);
	void SendRejectCallMessage(UCID userUid, const std::string& confId, OUT int32_t& msgId);
	void SendFinishCallMessage(UCID userUid, const std::string& confId, OUT int32_t& msgId);
	void SendCancelCallMessage(UCID userUid, OUT int32_t& msgId);
	void SendNotRespondCallRecordMessage(UCID userUid, OUT int32_t& msgId);
	void SendConnectedCallRecordMessage(UCID userUid, int32_t druation, OUT int32_t& msgId);
	void SendMessageRead(UCID userUid, int32_t msgId, int64_t msgSeq);
	void SendMessagesRead(int32_t userId, std::vector<int32_t> chatId);
	void SendMessagePlayed(UCID userUid, int32_t msgId, int64_t msgSeq);
	
	// Event;			     
	virtual void OnChatHistoryReceived(int32_t result, int64_t userId, const std::list<MessageInfo*>& msgList);
	virtual void OnMessageSent(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime);
	virtual void OnTextMessageReceived(const TextMessageInfo *message);
	virtual void OnImageMessageReceived(const MediaMessageInfo *message);
	virtual void OnAudioMessageReceived(const MediaMessageInfo *message);
	virtual void OnVideoMessageReceived(const MediaMessageInfo *message);
	virtual void OnFileMessageReceived(const FileMessageInfo *message);
	virtual void OnFileShared(UCID userUid, int32_t msgId, int64_t msgSeq, const std::string& fileName, int32_t fileSize,
		const std::string& fileURL, int64_t fileId);
	virtual void OnFileDeleted(UCID userUid, int32_t msgId, int64_t msgSeq, const std::string& fileName, int64_t fileId);
	virtual void OnMessageRead(int32_t result, int32_t userId, int32_t readType, int32_t msgId, int64_t msgSeq);
	virtual void OnMessagePlayed(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq);
	virtual void OnCallInvited(int32_t result, int32_t userId, const std::string confId, const std::string confPwd, int32_t hostId);
	virtual void OnCallAccepted(int32_t result, int32_t userId, const std::string confId);
	virtual void OnCallRejected(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, const std::string confId);
	virtual void OnCallStopped(int32_t result, int32_t userId, const std::string confId);
	virtual void OnCallCanceled(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq);
	virtual void OnCallNotResponded(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq);
	virtual void OnCallConnected(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, int32_t duration);


private:
	IIMService *_imService;

};



#endif //GROUP_SERVICE_MOCK_CLIENT_H_
