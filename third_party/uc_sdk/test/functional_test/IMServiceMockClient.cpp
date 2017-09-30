#include "IMServiceMockClient.h"


IMServiceMockClient::IMServiceMockClient(){


}

IMServiceMockClient::~IMServiceMockClient(){


}

void IMServiceMockClient::Init(IUCEngine *ucEngine){
	_imService = (IIMService *)ucEngine->QueryService(UCIMService);
	_imService->registerListener(this);
}

void IMServiceMockClient::UnInit(){
	_imService->removeListener(this);
}

void IMServiceMockClient::GetChatHistory(int32_t userId, int32_t count, int32_t startTime, int32_t endTime){
	_imService->GetChatHistory(userId, count, startTime, endTime);
}

void IMServiceMockClient::ResendMessage(int32_t msgId){
	_imService->ResendMessage(msgId);
}

void IMServiceMockClient::SendTextMessage(UCID userUid, int8_t textType, std::string& textFromat, const std::string& textData, OUT int32_t& msgId){
	_imService->SendTextMessage(userUid, textType, textFromat, textData, msgId);
}

void IMServiceMockClient::SendImage(UCID userUid, const std::string& imageName, int32_t imageSize,
	const std::string& imageURL, const std::string& imageData, OUT int32_t& msgId){
	_imService->SendImageMessage(userUid, imageName, imageSize, imageURL, imageData, msgId);
}

void IMServiceMockClient::SendAudioMessage(UCID userUid, const std::string& audioName, int32_t audioSize,
	const std::string& audioURL, int32_t audioDuration, OUT int32_t& msgId){
	_imService->SendAudioMessage(userUid, audioName, audioSize, audioURL, audioDuration, msgId);
}

void IMServiceMockClient::SendVideoMessage(UCID userUid, const std::string& videoName, int32_t videoSize,
	const std::string& videoURL, const std::string& videoData, int32_t videoDuration, OUT int32_t& msgId){
	_imService->SendVideoMessage(userUid, videoName, videoSize, videoURL, videoData, videoDuration, msgId);
}

void IMServiceMockClient::SendFileMessage(UCID userUid, const std::string& fileName, int32_t flieSize,
	const std::string& fileURL, int64_t fileId, OUT int32_t& msgId){
	_imService->SendFileMessage(userUid, fileName, flieSize, fileURL, fileId, msgId);
}

void IMServiceMockClient::ShareFileMessage(UCID userUid, const std::string& fileName, int32_t flieSize,
	const std::string& fileURL, int64_t fileId, OUT int32_t& msgId){
	_imService->ShareFileMessage(userUid, fileName, flieSize, fileURL, fileId, msgId);
}

void IMServiceMockClient::DeleteFileMessage(UCID userUid, const std::string& fileName, int64_t fileId, OUT int32_t& msgId){
	_imService->DeleteFileMessage(userUid, fileName, fileId, msgId);
}

void IMServiceMockClient::SendInviteCallMessage(UCID userUid, const std::string& confId,
	const std::string& confPwd, int32_t hostId, OUT int32_t& msgId){
	_imService->SendInviteCallMessage(userUid, confId, confPwd, hostId, msgId);
}

void IMServiceMockClient::SendAcceptCallMessage(UCID userUid, const std::string& confId, OUT int32_t& msgId){
	_imService->SendAcceptCallMessage(userUid, confId, msgId);
}

void IMServiceMockClient::SendRejectCallMessage(UCID userUid, const std::string& confId, OUT int32_t& msgId){
	_imService->SendRejectCallMessage(userUid, confId, msgId);
}

void IMServiceMockClient::SendFinishCallMessage(UCID userUid, const std::string& confId, OUT int32_t& msgId){
	_imService->SendFinishCallMessage(userUid, confId, msgId);
}

void IMServiceMockClient::SendCancelCallMessage(UCID userUid, OUT int32_t& msgId){
	_imService->SendCancelCallMessage(userUid, msgId);
}

void IMServiceMockClient::SendNotRespondCallRecordMessage(UCID userUid, OUT int32_t& msgId){
	_imService->SendNotRespondCallRecordMessage(userUid, msgId);
}

void IMServiceMockClient::SendConnectedCallRecordMessage(UCID userUid, int32_t druation, OUT int32_t& msgId){
	_imService->SendConnectedCallRecordMessage(userUid, druation, msgId);
}

void IMServiceMockClient::SendMessageRead(UCID userUid, int32_t msgId, int64_t msgSeq){
	_imService->SendMessageRead(userUid, msgId, msgSeq);
}

void IMServiceMockClient::SendMessagesRead(int32_t userId, std::vector<int32_t> chatId){
	_imService->SendMessagesRead(userId, chatId);
}

void IMServiceMockClient::SendMessagePlayed(UCID userUid, int32_t msgId, int64_t msgSeq){
	_imService->SendMessagePlayed(userUid, msgId, msgSeq);
}

//Event
void IMServiceMockClient::OnChatHistoryReceived(int32_t result, int64_t userId, const std::list<MessageInfo*>& msgList){
	NotifyResult(result);
	cout << "OnChatHistoryReceived result: " << result << endl;
}

void IMServiceMockClient::OnMessageSent(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime){
	NotifyResult(result);
	cout << "OnMessageSent result: " << result << endl;
}

void IMServiceMockClient::OnTextMessageReceived(const TextMessageInfo *message){

}

void IMServiceMockClient::OnImageMessageReceived(const MediaMessageInfo *message){

}

void IMServiceMockClient::OnAudioMessageReceived(const MediaMessageInfo *message){

}

void IMServiceMockClient::OnVideoMessageReceived(const MediaMessageInfo *message){

}

void IMServiceMockClient::OnFileMessageReceived(const FileMessageInfo *message){

}

void IMServiceMockClient::OnFileShared(UCID userUid, int32_t msgId, int64_t msgSeq, const std::string& fileName, int32_t fileSize,
	const std::string& fileURL, int64_t fileId){

}

void IMServiceMockClient::OnFileDeleted(UCID userUid, int32_t msgId, int64_t msgSeq, const std::string& fileName, int64_t fileId){

}

void IMServiceMockClient::OnMessageRead(int32_t result, int32_t userId, int32_t readType, int32_t msgId, int64_t msgSeq){
	NotifyResult(result);
	cout << "OnMessageRead result: " << result << endl;
}

void IMServiceMockClient::OnMessagePlayed(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq){
	NotifyResult(result);
	cout << "OnMessagePlayed result: " << result << endl;
}

void IMServiceMockClient::OnCallInvited(int32_t result, int32_t userId, const std::string confId, const std::string confPwd, int32_t hostId){
	NotifyResult(result);
	cout << "OnCallInvited result: " << result << endl;
}

void IMServiceMockClient::OnCallAccepted(int32_t result, int32_t userId, const std::string confId){
	NotifyResult(result);
	cout << "OnCallAccepted result: " << result << endl;
}

void IMServiceMockClient::OnCallRejected(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, const std::string confId){
	NotifyResult(result);
	cout << "OnCallRejected result: " << result << endl;
}

void IMServiceMockClient::OnCallStopped(int32_t result, int32_t userId, const std::string confId){
	NotifyResult(result);
	cout << "OnCallStopped result: " << result << endl;
}

void IMServiceMockClient::OnCallCanceled(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq){
	NotifyResult(result);
	cout << "OnCallCanceled result: " << result << endl;
}

void IMServiceMockClient::OnCallNotResponded(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq){
	NotifyResult(result);
	cout << "OnCallNotResponded result: " << result << endl;
}

void IMServiceMockClient::OnCallConnected(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, int32_t duration){
	NotifyResult(result);
	cout << "OnCallConnected result: " << result << endl;
}
