#include "ExternalServiceStrategy.h"
#include "ServiceObjectConversion.h"
#include "utils/Thumbnails.h"
#include "UCDefs.h"
#include "IClientService.h"

namespace ucclient {

ExternalServiceStrategy::ExternalServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient)
	: _engine(engine), _cefClient(cefClient) {
	_externalService = (IExternalService*)_engine->QueryService(UCExternalService);
}
 
int32_t ExternalServiceStrategy::ShowExternalMessage(const std::wstring& externalId){

	std::string utfExternalId = StringConvert::ToUTF8Str(externalId);
	return _externalService->ShowExternalMessage(utfExternalId);
}

int32_t ExternalServiceStrategy::GetChatHistory(ConversationType type, const std::wstring& externalId, int32_t count, int64_t startTime, int64_t endTime){

	std::string utfExternalId = StringConvert::ToUTF8Str(externalId);
	return _externalService->GetChatHistory(type, utfExternalId, count, startTime, endTime);
}

void ExternalServiceStrategy::OnChatHistoryReceived(int32_t result, ConversationType type, int64_t relateId, int32_t msgCount, int64_t min_timestamp, const list<ExternalMessage*>& msgList) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << EXTERNAL_SERVICE_CHAT_HISTORY_RECEIVED_EVENT
		<< "\", {\"detail\":" << "{\"errorCode\": " << result << ", \"ConversationType\": " << type << ", \"relateId\": "
		<< relateId << ", \"msgCount\": " << msgCount << ", \"minTimestamp\": " << min_timestamp << ", \"message\":[";
		list<ExternalMessage*>::const_iterator iter = msgList.begin();
		for (; iter != msgList.end(); ++iter){
			if ((*iter)->GetContentType() == EXTERNAL_CONTENT_TYPE_TEXT){
				jsScript << ServiceObjectConversion::AppendTextExternalMessageToJSON(dynamic_cast<TextExternalMessage&>(const_cast<ExternalMessage&>(**iter))) << ",";
			}
		}
			
	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnChatHistoryReceived event to JS";
}


}