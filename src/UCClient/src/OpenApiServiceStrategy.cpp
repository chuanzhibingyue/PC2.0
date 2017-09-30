#include "OpenApiServiceStrategy.h"
#include "ServiceObjectConversion.h"
#include "utils/Thumbnails.h"
#include "UCDefs.h"
#include "IClientService.h"

namespace ucclient {

OpenApiServiceStrategy::OpenApiServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient) 
	: _engine(engine), _cefClient(cefClient) {
	_openApiService = (IOpenApiService*)_engine->QueryService(UCOpenApiService);
}
 
int32_t OpenApiServiceStrategy::GetAppList(){
	return _openApiService->GetAppList();
}

int32_t OpenApiServiceStrategy::GetAppInfo(const std::list<int64_t>& appIds){
	return _openApiService->GetAppInfo(appIds);
}

int32_t OpenApiServiceStrategy::GetOAMessageStatus(const std::list<AppMessageInfo> &infos){
	return _openApiService->GetOAMessageStatus(infos);
}

int32_t OpenApiServiceStrategy::UpdateOAMessageStatus(const std::list<AppMessageStatusInfo> &infos){
	return _openApiService->UpdateOAMessageStatus(infos);
}

int32_t OpenApiServiceStrategy::GetMessageAckStatus(const std::list<AppMessageInfo> &infos){
	return _openApiService->GetMessageAckStatus(infos);
}

int32_t OpenApiServiceStrategy::GetChatHistory(int64_t appId, int64_t count, int64_t startTime, int64_t endTime){
	return _openApiService->GetChatHistory(appId, count, startTime, endTime);
}

int32_t OpenApiServiceStrategy::ResendMessage(int32_t msgId){
	return _openApiService->ResendMessage(msgId);
}

int32_t OpenApiServiceStrategy::SendTextMessage(UCID toApp, const std::wstring& textTitle, const std::wstring& textContent, int8_t textType,
	const std::wstring& detailContent, int32_t& msgId) {

	std::string utf8Title = StringConvert::ToUTF8Str(textTitle);
	std::string utf8Content = StringConvert::ToUTF8Str(textContent);
	std::string utf8detail = StringConvert::ToUTF8Str(detailContent);
	return _openApiService->SendTextMessage(toApp, utf8Title, utf8Content, textType, utf8detail, msgId);
}

int32_t OpenApiServiceStrategy::SendImageMessage(UCID toApp, const std::wstring& imageId, int8_t imageType, const std::wstring imagePath, int32_t& msgId) {
		
	std::string utf8ImageId = StringConvert::ToUTF8Str(imageId);
	std::wstring thumbnailPath = L"";
	std::string thunmbnailData = "";
	if (1 == imageType)
	{
		IClientService *service = (IClientService*)_engine->QueryService(UCClientService);
		if (service != NULL) {
			thumbnailPath = StringConvert::FromUTF8Str(service->GetDocumentDataDir(IMAGE_DIR));
		}

		ThumbnailsUtil::CreateThumbnails(imagePath, thumbnailPath, thunmbnailData);
	}

	return _openApiService->SendImageMessage(toApp, utf8ImageId, imageType, thunmbnailData, msgId);
}

int32_t OpenApiServiceStrategy::SendFileMessage(UCID toApp, const std::wstring& fileId, int8_t fileType, const std::wstring fileName,
	int64_t fileSize, int32_t& msgId) {

	std::string utf8FileId = StringConvert::ToUTF8Str(fileId);
	std::string utf8FileName = StringConvert::ToUTF8Str(fileName);

	return _openApiService->SendFileMessage(toApp, utf8FileId, fileType, utf8FileName, fileSize, msgId);
}

int32_t OpenApiServiceStrategy::SendMessageRead(UCID& appUid, int32_t msgId, int64_t msgSeq){
	return _openApiService->SendMessageRead(appUid, msgId, msgSeq);
}

int32_t OpenApiServiceStrategy::SendMessagesRead(int64_t toAppId){
	return _openApiService->SendMessagesRead(toAppId);
}

int32_t OpenApiServiceStrategy::SendRequest(CefRefPtr<CefBrowser> browser, int32_t requestId, 
	RequestMethod method, const std::wstring &url, const std::wstring &body, const std::list<HeadInfo> &heads){

	_requestMap.insert(make_pair(requestId, browser->GetHost()->GetWindowHandle()));

	std::string utf8Url = StringConvert::ToUTF8Str(url);
	std::string utf8Body = StringConvert::ToUTF8Str(body);
	return _openApiService->SendRequest(requestId, method, utf8Url, utf8Body, heads);
}

void OpenApiServiceStrategy::OnChatHistoryReceived(int32_t result, int64_t appId, int32_t msgCount, int64_t min_timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_CHAT_HISTORY_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ", appId: "
		<< appId << ", msgCount: " << msgCount << ", minTimestamp: " << min_timestamp;

	jsScript << "}});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnChatHistoryReceived event to JS";
}


void OpenApiServiceStrategy::OnAppListReceived(int32_t result, const std::list<AppInfo>& appList) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_APP_LIST_RECEIVED
		<< "\",{detail:" << "{errorCode:" << result <<  ", apps:[";

	for (std::list<AppInfo>::const_iterator iter = appList.begin(); iter != appList.end(); ++iter) {
		jsScript << ServiceObjectConversion::AppendAppMessageInfoToJSON(*iter) << ",";
	}

	jsScript << "]}});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAppListReceived event to JS";
}

void OpenApiServiceStrategy::OnAppInfoReceived(int32_t result, const std::list<AppInfo>& appList){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_APP_INFO_RECEIVED
		<< "\",{detail:" << "{errorCode:" << result << ", apps:[";

	for (std::list<AppInfo>::const_iterator iter = appList.begin(); iter != appList.end(); ++iter) {
		jsScript << ServiceObjectConversion::AppendAppMessageInfoToJSON(*iter) << ",";
	}

	jsScript << "]}});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAppInfoReceived event to JS";
}

void OpenApiServiceStrategy::OnOAMessageStatusReceived(int32_t result, const std::list<AppMessageStatusInfo>& infos){

	std::wstringstream jsScript;
	
	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_OA_MESSAGE_STATUS_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ", status:[";

	for (std::list<AppMessageStatusInfo>::const_iterator iter = infos.begin(); iter != infos.end(); ++iter) {
		std::wstringstream ss;

		ss << L"{appId:" << iter->appId 
			<< ", msgSeq:" << iter->msgSeq 
			<< ", status:" << iter->status 
			<< ", externalData:\"" << UTF2WJSON(iter->externalData) << "\", "
			<< "}";

		jsScript << ss.str() << ",";
	}

	jsScript << "]}});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnOAMessageStatusReceived event to JS";
}

void OpenApiServiceStrategy::OnOAMessageStatusUpdated(int32_t result, const std::list<AppMessageStatusInfo>& infos){

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_OA_MESSAGE_STATUS_UPDATED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ", status:[";

	for (std::list<AppMessageStatusInfo>::const_iterator iter = infos.begin(); iter != infos.end(); ++iter) {
		std::wstringstream ss;

		ss << L"{appId:" << iter->appId
			<< ", msgSeq:" << iter->msgSeq
			<< ", status:" << iter->status
			<< ", externalData:\"" << UTF2WJSON(iter->externalData) << "\", "
			<<"}";

		jsScript << ss.str() << ",";
	}

	jsScript << "]}});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnOAMessageStatusUpdated event to JS";
}

void OpenApiServiceStrategy::OnMessageAckStatusReceived(int32_t result, const std::list<AppMessageStatusInfo>& infos){

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_MESSAGE_ACK_STATUS_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ", status:[";

	for (std::list<AppMessageStatusInfo>::const_iterator iter = infos.begin(); iter != infos.end(); ++iter) {
		std::wstringstream ss;

		ss << L"{appId:" << iter->appId << ", msgSeq:" << iter->msgSeq << ", ackSent:" << iter->status << "}";

		jsScript << ss.str() << ",";
	}

	jsScript << "]}});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageAckStatusReceived event to JS";
}

void OpenApiServiceStrategy::OnMessageSent(int32_t result, int64_t appId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	
	std::wstringstream jsScript;
	
	jsScript<<L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_MESSAGE_SENT_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ", appId: " 
		<< appId << ", msgId: " << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageSent event to JS";
}

void OpenApiServiceStrategy::OnResponseReceived(int32_t result, int32_t requestId, const std::string& content){
	
	CefRefPtr<CefProcessMessage> response = CefProcessMessage::Create("InvokeCallback");
	CefRefPtr<CefListValue> responseArgs = response->GetArgumentList();
	responseArgs->SetInt(0, requestId);
	responseArgs->SetInt(1, result);
	responseArgs->SetString(2, content);

	std::map<int32_t, CefWindowHandle>::iterator it = _requestMap.find(requestId);
	if (it == _requestMap.end()){
		return;
	}

	_cefClient->GetBrowser(it->second)->SendProcessMessage(PID_RENDERER, response);
	_requestMap.erase(it);
}

void OpenApiServiceStrategy::OnAppStatusChangedReceived(MessageChannel channel, const MessageInfo &message, int64_t appId,
	int16_t status, const std::string& externalData){
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_MESSAGE_APP_STATUS_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "appId:" << appId << ", "
		<< "status:" << status << ", "
		<< "externalData:\"" << UTF2WJSON(externalData) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAppStatusChangedReceived event to JS";

}

void OpenApiServiceStrategy::OnAppEventChangedReceived(MessageChannel channel, const MessageInfo &message, int64_t appId,
	int32_t eventCount, const std::string& externalData){

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_MESSAGE_APP_EVENT_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "appId:" << appId << ", "
		<< "eventCount:" << eventCount << ", "
		<< "externalData:\"" << UTF2WJSON(externalData) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAppEventChangedReceived event to JS";

}

void OpenApiServiceStrategy::OnPushMessageAck(MessageChannel channel, const MessageInfo &message, int32_t result, int32_t msgId, int64_t msgSeq, int32_t conversation){
	
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_PUSH_MESSAGE_ACK_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "errorCode:" << result << ", "
		<<"msgId:" << msgId << ", "
		<< "msgSeq:" << msgSeq << ", "
		<< "conversation:" <<conversation<< ", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnPushMessageAck event to JS";

}

void OpenApiServiceStrategy::OnOAMessageStatusChangedReceived(MessageChannel channel, const MessageInfo &message, const AppMessageStatusInfo &statusInfo){
	
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_OAMESSGAE_STATUS_CHANGED_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "appId:" << statusInfo.appId << ", "
		<< "msgSeq:" << statusInfo.msgSeq << ", "
		<< "status:" << statusInfo.status << ", "
		<< "externalData:\"" << UTF2WJSON(statusInfo.externalData) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnOAMessageStatusChangedReceived event to JS";
}

void OpenApiServiceStrategy::OnOAMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& title, int16_t status,
	const std::string& titleColor, const std::string& titleBkColor, const std::string& data, const std::string& detailUrl, 
	int8_t detailAuth, const std::string& titleElements, const std::string& titleStyle, const std::string& customizedType
	, const std::string& customizedData){
	
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_OA_MESSAGE_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "title:\"" << UTF2WJSON(title) << "\", "
		<< "oaLastStatus:" << message.GetOALastStatus() << ", "
		<< "oaExternalData:\"" << UTF2WJSON(message.GetOAExternalData()) << "\", "
		<< "status:" << status << ", "
		<< "titleColor:\"" << UTF2WJSON(titleColor) << "\", "
		<< "titleBkColor:\"" << UTF2WJSON(titleBkColor) << "\", "
		<< "data:\"" << UTF2WJSON(data) << "\", "
		<< "detailAuth:" << detailAuth << ", "
		<< "detailUrl:\"" << UTF2WJSON(detailUrl) << "\", "
		<< "titleElements:\"" << UTF2WJSON(titleElements) << "\", "
		<< "titleStyle:\"" << UTF2WJSON(titleStyle) << "\", "
		<< "customizedType:\"" << UTF2WJSON(customizedType) << "\", "
		<< "customizedData:\"" << UTF2WJSON(customizedData) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnOAMessageReceived event to JS";
}

void OpenApiServiceStrategy::OnCustomizedMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& frist,
	const std::string& second, const std::string& third, const std::string& four){
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_CUSTOMIZED_MESSAGE_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "data1:\"" << UTF2WJSON(frist) << "\", "
		<< "data2:\"" << UTF2WJSON(second) << "\", "
		<< "data3:\"" << UTF2WJSON(third) << "\", "
		<< "data4:\"" << UTF2WJSON(four) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCustomizedMessageReceived event to JS";
}


void OpenApiServiceStrategy::OnTextMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& textTitle,
	const std::string& textContent, int8_t detailType, const std::string& detailContent, int8_t detailAuth) {

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_TEXT_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message); 
	jsScript << "appAckSent:" << message.GetAppAckSent() << ", "
		<< "textTitle:\"" << UTF2WJSON(textTitle) << "\", "
		<< "textContent:\"" << UTF2WJSON(textContent) << "\", "
		<< "textType:" << detailType << ", "
		<< "detailAuth:" << detailAuth << ", "
		<< "detailContent:\"" << UTF2WJSON(detailContent) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnTextMessageReceived event to JS";
}

void OpenApiServiceStrategy::OnImageMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string &imageId,
	int8_t imageType, const std::string &imageData) {

	std::wstringstream jsScript;
	
	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_IMAGE_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "appAckSent:" << message.GetAppAckSent() << ", "
		<< "imageId:\"" << UTF2WJSON(imageId) << "\", "
		<< "imageType:" << imageType << ", "
		<< "imageData:\"" << UTF2WJSON(imageData) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnImageReceived event to JS";
}

void OpenApiServiceStrategy::OnFileMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& fileId,
	int8_t fileType, const std::string fileName, int64_t fileSize) {

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_FILE_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "appAckSent:" << message.GetAppAckSent() << ", "
		<< "fileId:\"" << UTF2WJSON(fileId) << "\", "
		<< "fileType:" << fileType << ", "
		<< "fileName:\"" << UTF2WJSON(fileName) << "\", "
		<< "fileSize:" << fileSize;
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnFileMessageReceived event to JS";
}

void OpenApiServiceStrategy::OnMessageRead(int32_t result, int64_t appId, int32_t readType, int32_t msgId, int64_t msgSeq){

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << OPENAPI_SERVICE_MESSAGE_READ_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ", userId: " << appId
		<< ", readType:" << readType << ", msgId: " << msgId
		<< ", msgSeq: " << msgSeq << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageReaded event to JS";
}

}