/*
 * OpenApiServiceStrategy.h
 *
 *  Created on: Sep 11 2015
 *      Author: yongshuai.cheng
 */

#ifndef OEPNAPI_SERVICE_STRATEGY_H_
#define OEPNAPI_SERVICE_STRATEGY_H_

#include "IOpenApiService.h"
#include "CefClient.h"

namespace ucclient {
	
using namespace uc;

class OpenApiServiceStrategy : public IOpenApiServiceEvent {
public:
	OpenApiServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);

	int32_t GetChatHistory(int64_t appId, int64_t count, int64_t startTime, int64_t endTime);

	int32_t GetAppList();

	int32_t GetAppInfo(const std::list<int64_t>& appIds);

	int32_t GetOAMessageStatus(const std::list<AppMessageInfo> &infos);

	int32_t UpdateOAMessageStatus(const std::list<AppMessageStatusInfo> &infos);

	int32_t GetMessageAckStatus(const std::list<AppMessageInfo> &infos);

	int32_t ResendMessage(int32_t msgId);

	int32_t SendTextMessage(UCID toApp, const std::wstring& textTitle, const std::wstring& textContent, int8_t textType,
		const std::wstring& detailContent, OUT int32_t& msgId);

	int32_t SendImageMessage(UCID toApp, const std::wstring& imageId, int8_t imageType, const std::wstring imagePath, OUT int32_t& msgId);

	int32_t SendFileMessage(UCID toApp, const std::wstring& fileId, int8_t fileType, const std::wstring fileName,
		int64_t fileSize, OUT int32_t& msgId);

	int32_t SendMessageRead(UCID& appUid, int32_t msgId, int64_t msgSeq);

	int32_t SendMessagesRead(int64_t toAppId);

	int32_t SendRequest(CefRefPtr<CefBrowser> browser, int32_t requestId, RequestMethod method, const std::wstring &url, const std::wstring &body, const std::list<HeadInfo> &heads);

	// IOpenApiServiceEvent方法
	virtual void OnChatHistoryReceived(int32_t result, int64_t appId, int32_t msgCount, int64_t min_timestamp);

	virtual void OnAppListReceived(int32_t result, const std::list<AppInfo>& appList);
	
	virtual void OnAppInfoReceived(int32_t result, const std::list<AppInfo>& appList);

	virtual void OnOAMessageStatusReceived(int32_t result, const std::list<AppMessageStatusInfo>& infos);

	virtual void OnOAMessageStatusUpdated(int32_t result, const std::list<AppMessageStatusInfo>& infos);

	virtual void OnMessageAckStatusReceived(int32_t result, const std::list<AppMessageStatusInfo>& infos);

	virtual void OnMessageSent(int32_t result, int64_t appId, int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnResponseReceived(int32_t result, int32_t requestId, const std::string& content);

	virtual void OnAppStatusChangedReceived(MessageChannel channel, const MessageInfo &message, int64_t appId, int16_t status, const std::string& externalData);

	virtual void OnAppEventChangedReceived(MessageChannel channel, const MessageInfo &message, int64_t appId, int32_t eventCount, const std::string& externalData);

	virtual void OnPushMessageAck(MessageChannel channel, const MessageInfo &message, int32_t result, int32_t msgId, int64_t msgSeq, int32_t conversation);

	virtual void OnOAMessageStatusChangedReceived(MessageChannel channel, const MessageInfo &message, const AppMessageStatusInfo &statusInfo);

	virtual void OnOAMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& title, int16_t status,
		const std::string& titleColor, const std::string& titleBkColor, const std::string& data, const std::string& detailUrl, 
		int8_t detailAuth, const std::string& titleElements, const std::string& titleStyle, const std::string& customizedType
		, const std::string& customizedData);

	virtual void OnCustomizedMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& frist,
		const std::string& second, const std::string& third, const std::string& four);

	virtual void OnTextMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& textTitle, 
		const std::string& textContent, int8_t detailType, const std::string& detailContent, int8_t detailAuth);

	virtual void OnImageMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string &imageId, 
		int8_t imageType, const std::string &imageData);

	virtual void OnFileMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& fileId, 
		int8_t fileType, const std::string fileName, int64_t fileSize);

	virtual void OnMessageRead(int32_t result, int64_t appId, int32_t readType, int32_t msgId, int64_t msgSeq);

private:
	cefIntegration::CefClient *_cefClient;
	IOpenApiService *_openApiService;
	IUCEngine *_engine;
	std::map<int32_t, CefWindowHandle> _requestMap;
};

}

#endif