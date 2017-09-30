#pragma once
#include "IBBSService.h"
#include "network/NetworkManager.h"
#include "rapidjson/document.h"

namespace uc {
class CBBSService :public IBBSService
{
public:
	CBBSService(IUCEngine *engine);
	virtual ~CBBSService();

	int32_t BBSRequest(const std::string& requestData, OUT int32_t& taskId);
	int32_t OnResponse(int32_t errorCode, HTTPResponse *response, int32_t taskId);
protected:
	std::string ConvertJSON2Form(rapidjson::Value &jsonObject, std::string keyName);
private:
	IUCEngine *m_ucEngine;
};
} /* namespace uc */
