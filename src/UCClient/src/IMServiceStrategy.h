/*
 * IMServiceStrategy.h
 *
 *  Created on: July 7, 2015
 *      Author: gang wang
 */

#ifndef IM_SERVICE_STRATEGY_H_
#define IM_SERVICE_STRATEGY_H_

#include "IIMService.h"
#include "CefClient.h"

namespace ucclient {
	
using namespace uc;

class IMServiceStrategy : public IIMServiceEvent {
public:
	IMServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);

	int32_t GetChatHistory(MessageChannel channel, int64_t userId, int32_t count, int64_t startTime, int64_t endTime);

	int32_t ResendMessage(int32_t msgId);

	int32_t ForwardMessage(UCID& userUid, int32_t msgId);

	int32_t SendTextMessage(UCID& userUid, int8_t textType, std::wstring& textFormat, const std::wstring& textData, OUT int32_t& msgId);

	int32_t SendImageMessage(UCID& userUid, const std::wstring& imageName, int32_t imageSize,
		const std::wstring& imageURL, const std::wstring& imagePath, OUT int32_t& msgId);

	int32_t SendAudioMessage(UCID& userUid, const std::wstring& audioName, int32_t audioSize,
		const std::wstring& audioURL, int32_t audioDuration, OUT int32_t& msgId);

	int32_t SendVideoMessage(UCID& userUid, const std::wstring& videoName, int32_t videoSize,
		const std::wstring& videoURL, const std::wstring& videoPath, int32_t videoDuration, OUT int32_t& msgId);

	int32_t SendReportMessage(UCID userUid, const std::wstring& reportTitle, const std::wstring& reportUrl, OUT int32_t& msgId);

	int32_t SendRecordVideoMessage(UCID& userUid, const std::wstring& videoName, int32_t videoSize, 
		const std::wstring& videoURL, const std::wstring& videoThumb, const std::wstring& videoThumbURL, int32_t videoDuration, int32_t operatorId,
		const std::wstring& operatorName, const std::wstring& operatorAvatar, int32_t& msgId);

	int32_t SendFileMessage(UCID& userId, int32_t operation, const std::wstring& fileName, int32_t flieSize,
		const std::wstring& fileURL, int64_t fileId, OUT int32_t& msgId);

	int32_t SendCloudFileMessage(UCID userUid, const std::wstring& fileName, int64_t fileSize,
		DocumentCloudType cloudType, const std::wstring& detailContent, OUT int32_t& msgId);

	int32_t DeleteFileMessage(UCID& userId, const std::wstring& fileName, int64_t fileId, OUT int32_t& msgId);

	int32_t SendCodeMessage(UCID userUid, int32_t operation, int64_t codeId, const std::wstring& codeTitle, const std::wstring& langType, const std::wstring& codeHead, 
		const std::wstring& codeDesc, int32_t codeSize, const std::wstring& codeUrl, int32_t codeLine, int64_t createTime, OUT int32_t& msgId);

	int32_t SendInviteCallMessage(UCID& userId, const std::wstring& confId, 
		const std::wstring& confPwd, int32_t hostId, OUT int32_t& msgId);

	int32_t SendAcceptCallMessage(UCID& userId, const std::wstring& confId, int32_t hostId, OUT int32_t& msgId);

	int32_t SendRejectCallMessage(UCID& userId, const std::wstring& confId, int32_t hostId, const std::wstring& reason, OUT int32_t& msgId);

	int32_t SendFinishCallMessage(UCID& userId, const std::wstring& confId, int32_t hostId, const std::wstring& reason, OUT int32_t& msgId);

	int32_t SendCancelCallMessage(UCID& userId, const std::string& phoneNum, OUT int32_t& msgId);

	int32_t SendNotRespondCallRecordMessage(UCID& userId, const std::string& phoneNum, OUT int32_t& msgId);

	// int32_t SendRejectCallRecordMessage(UCID& userId, OUT int32_t& msgId);

	int32_t SendConnectedCallRecordMessage(UCID& userId, int32_t druation, const std::string& phoneNum, OUT int32_t& msgId);

	int32_t SendMessageRead(UCID& userId, int32_t msgId, int64_t msgSeq);

	int32_t SendMessagesReadById(int32_t toUserId);

	int32_t SendMessagePlayed(UCID& userId, int32_t msgId, int64_t msgSeq);

	// IIMServiceEvent方法
	virtual void OnChatHistoryReceived(int32_t result, MessageChannel channel, int64_t userId, int32_t msgCount, int64_t min_timestamp);

	virtual void OnMessageSent(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnTextMessageReceived(MessageChannel channel, const TextMessageInfo *message);

	virtual void OnImageMessageReceived(MessageChannel channel, const MediaMessageInfo *message);

	virtual void OnAudioMessageReceived(MessageChannel channel, const MediaMessageInfo *message);

	virtual void OnVideoMessageReceived(MessageChannel channel, const MediaMessageInfo *message);

	virtual void OnReportMessageReceived(MessageChannel channel, const ReportMessageInfo *message);

	virtual void OnRecordVideoReceived(MessageChannel channel, const MediaMessageInfo *message);

	virtual void OnFileMessageReceived(MessageChannel channel, int32_t operation, const FileMessageInfo *message);

	virtual void OnCloudFileCreateMessageReceived(MessageChannel channel, const FileMessageInfo &message);

	virtual void OnFileDeleted(UCID userId, int32_t msgId, int64_t msgSeq, const std::string& fileName, int64_t fileId);

	virtual void OnCodeMessageReceived(MessageChannel channel, int32_t operation, const CodeMessageInfo& message);

	virtual void OnMessageRead(int32_t result, int32_t userId, int32_t readType, int32_t msgId, int64_t msgSeq);

	virtual void OnMessagePlayed(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq);

	virtual void OnMessageUnreadUserCount(int32_t result, int32_t userId, const std::list<MessageReadInfo>& info);
	
	virtual void OnCallInvited(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, const std::string confId,
		const std::string confPwd, int32_t hostId, int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnCallAccepted(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, const std::string confId,
		int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnCallRejected(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime,
		const std::string confId, int32_t hostId, std::string reason);

	virtual void OnCallStopped(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, const std::string confId, int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnCallCanceled(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime, const std::string& phoneNum);

	virtual void OnCallNotResponded(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnCallConnected(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime, int32_t duration, const std::string& phoneNum);

private:
	cefIntegration::CefClient *_cefClient;
	IIMService *_imService;
	IUCEngine *_engine;
};

}

#endif