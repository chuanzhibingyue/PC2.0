#include "QueuedNetWorkManager.h"
#include "common/ZipWrapper.h"
#include "client_service/ClientService.h"
#include "UCEngine.h"
#include "CommonDef.h"
#include "UCErrorCode.h"
#include <boost/foreach.hpp>

namespace uc {

QueuedNetWorkManager::QueuedNetWorkManager(uint8_t threads) : NetworkManager(threads){
}

void QueuedNetWorkManager::SendQueuedRequest(ConversationType type, int64_t userId, int32_t msgId, 
	const std::string & body) {
	MessageId id;
	id.type = type;
	id.userId = userId;

	std::unique_lock<std::recursive_mutex> lock(_mtx);

	_msgIds[msgId] = id;
	_msgQueue.EnqueueMessage(msgId, body);
	lock.unlock();

	WakeUpHandlers();
}

void QueuedNetWorkManager::BeforeHandlerSleep() {
	SendRequest();
}

void QueuedNetWorkManager::AfterHttpRequestDone() {
	SendRequest();
}

void QueuedNetWorkManager::SendRequest() {
	std::vector<int32_t> dequeuedMsg;

	std::unique_lock<std::recursive_mutex> lock(_mtx);
	std::string body = _msgQueue.DequeueMessage(dequeuedMsg);
	lock.unlock();

	if (!dequeuedMsg.empty() && !body.empty()) {
		SendQueuedRequest(body, dequeuedMsg);
	}
}

void QueuedNetWorkManager::SendQueuedRequest(const std::string &body, std::vector<int32_t> &dequeuedMsg) {
	unsigned long zipDataLen = body.size();
	unsigned char *zipData = new unsigned char[zipDataLen];
	int result = ZipWrapper::Compress((unsigned char *)body.c_str(), body.size(), zipData, &zipDataLen);
	if (0 != result) {
		UC_LOG(ERROR) << "gzip compress error: " << result;

		delete[]zipData;
		zipData = NULL;

		TriggerFailResponse(dequeuedMsg, result);
		ClearMsgId(dequeuedMsg);
		return;
	}

	stringstream sStream;
	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);

	assert(clientService != NULL);
	sStream << UCEngine::GetRequestURL(clientService->GetLoginInfo().GetUccServerDomain(), MESSAGE_SEND_METHOD) <<
		"?session_id=" << clientService->GetLoginInfo().GetSessionId() <<
		"&user_id=" << clientService->GetLoginInfo().GetUserId();

	HTTPRequest	request;
	request.Init(sStream.str(), HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");
	request.AddHeaderInfo("Content-Encoding", "gzip");
	request.SetBody(string((char*)zipData, zipDataLen));

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &QueuedNetWorkManager::OnSendMessageResponse, dequeuedMsg));

	delete[]zipData;
	zipData = NULL;
}

int32_t QueuedNetWorkManager::OnSendMessageResponse(int32_t errorCode, HTTPResponse *response, std::vector<int32_t> dequeuedMsg) {
	UC_LOG(INFO) << "QueuedNetWorkManager::OnSendMessageResponse, errorCode:" << errorCode 
		<< " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	std::vector<MessageResponse> messageResponses;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		GetResponse(response->GetContent(), result, messageResponses);
	}
	else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (result != UC_SUCCEEDED) {
		TriggerFailResponse(dequeuedMsg, result);
		ClearMsgId(dequeuedMsg);
	}
	else {
		std::unique_lock<std::recursive_mutex> lock(_mtx);

		for (std::size_t i = 0; i < messageResponses.size(); i++) {
			MsgMap::iterator iter = _msgIds.find(messageResponses[i].msgId);
			if (iter != _msgIds.end()) {
				messageResponses[i].type = iter->second.type;
				messageResponses[i].userId = iter->second.userId;
			}
		}
		
		lock.unlock();

		triggerEvent(QueuedNetWorkManagerListener, OnSendMessageResponse(messageResponses));
		ClearMsgId(dequeuedMsg);
	}

	return 0;
}

void QueuedNetWorkManager::GetResponse(const std::string &content, int32_t& result, 
	std::vector<MessageResponse> &messageResponses) {

	wstringstream wsStream;
	wsStream.str(StringConvert::FromUTF8Str(content).c_str());
	boost::property_tree::wptree jsonRoot;
	try{
		boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
		result = jsonRoot.get<int32_t>(L"code");
		if (UC_SUCCEEDED == result) {
			boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"success_data");
			BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
				messageResponses.push_back(GetMessageResponse(v));
			}

			jsonChild = jsonRoot.get_child(L"failed_data");
			BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
				messageResponses.push_back(GetMessageResponse(v));
			}
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse json from OnSendMessageResponse , exception: " << e.what();
		result = JSON_PARSE_ERROR;
	}
}

MessageResponse QueuedNetWorkManager::GetMessageResponse(boost::property_tree::wptree::value_type& v) {
	MessageResponse response;

	response.code = v.second.get<int32_t>(L"code");
	response.msgId = v.second.get<int32_t>(L"msg_id");
	response.msgSeq = v.second.get<int64_t>(L"seq");
	response.sendTime = v.second.get<int64_t>(L"timestamp");

	return response;
}


void QueuedNetWorkManager::TriggerFailResponse(const std::vector<int32_t> &dequeuedMsg, int32_t res) {
	std::unique_lock<std::recursive_mutex> lock(_mtx);

	std::vector<MessageResponse> messageResponses;
	for (std::size_t i = 0; i < dequeuedMsg.size(); i++) {
		MsgMap::iterator iter = _msgIds.find(dequeuedMsg[i]);
		if (iter != _msgIds.end()) {
			MessageResponse response;
			response.type = iter->second.type;
			response.userId = iter->second.userId;
			response.code = res;
			response.msgId = dequeuedMsg[i];

			messageResponses.push_back(response);
		}
	}

	lock.unlock();

	triggerEvent(QueuedNetWorkManagerListener, OnSendMessageResponse(messageResponses));
}

void QueuedNetWorkManager::ClearMsgId(const std::vector<int32_t>& dequeuedMsg) {
	std::unique_lock<std::recursive_mutex> lock(_mtx);

	std::vector<MessageResponse> messageResponses;
	for (std::size_t i = 0; i < dequeuedMsg.size(); i++) {
		_msgIds.erase(dequeuedMsg[i]);
	}

	lock.unlock();
}

}