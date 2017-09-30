#include "BBSServiceStragegy.h"
#include "UCDefs.h"

namespace ucclient {
	BBSServiceStrategy::BBSServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient)
		:m_engine(engine), m_cefClient(cefClient)
	{
		m_bbsService = (IBBSService *)m_engine->QueryService(UCBBSService);
	}

	BBSServiceStrategy::~BBSServiceStrategy()
	{

	}

	int32_t BBSServiceStrategy::BBSRequest(const std::wstring& requestData, OUT int32_t& taskId)
	{
		std::string utf8RequestData = StringConvert::ToUTF8Str(requestData);
		return m_bbsService->BBSRequest(utf8RequestData, taskId);
	}

	void BBSServiceStrategy::OnResponse(int32_t taskID, int32_t errorcode, const std::string& response)
	{
		std::wstring wsResponse = StringConvert::FromUTF8Str(response.length() > 0 ? response.c_str() : "\"\"");
		std::wstringstream jsScript;
		jsScript << L"var e = new CustomEvent(\"" << BBS_SERVICE_MAIN_EVENT
			<< "\",{detail:" << "{errorCode: " << errorcode << ", requestID: " << taskID << ", response: " << wsResponse << "},});"
			<< "document.dispatchEvent(e);";

		m_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

		UC_LOG(INFO) << "dispatch BBS::OnResponse event to JS, errorCode: " << errorcode << ", taskID = " << taskID;
	}
}