#include "BBSService.h"
#include "client_service/ClientService.h"
#include "UCErrorCode.h"
#include "CommonDef.h"
#include "UCEngine.h"
#include "HTTPResponseAssist.h"
#include "jsonhelper.h"
#include "filetransfer_service/Task.h"
#include "Event.h"

#define SESSION_ID_KEY "session_id"
#define USER_ID_KEY "user_id"

using namespace rapidjson;
namespace uc {
	CBBSService::CBBSService(IUCEngine *engine)
		:m_ucEngine(engine)
	{
	}

	CBBSService::~CBBSService()
	{
	}
	/****
	requestData = {"controller":"broad","action":"list.json","url":{"_sid":"fbe2f8","uid":"1234211"},"body":{"id":441,"board_name":"tgestboard"}}
	}
	****/
	int32_t CBBSService::BBSRequest(const std::string& requestData, OUT int32_t& taskId)
	{
		Document doc;
		if (doc.Parse<0>(requestData.c_str()).HasParseError())
		{
			UC_LOG(ERROR) << "CBBSService::BBSRequest error, parse error for json string:" << requestData;
			return JSON_PARSE_ERROR;
		}
		std::string strController = "";
		if (doc.HasMember("controller"))
		{
			strController = doc["controller"].GetString();
		}
		else
		{
			UC_LOG(ERROR) << "CBBSService::BBSRequest error, input json string have no controller parameter";
			return ERROR_INPUT_PARAMETER;
		}
		std::string strAction = "";
		if (doc.HasMember("action"))
		{
			strAction = doc["action"].GetString();
		}
		else
		{
			UC_LOG(ERROR) << "CBBSService::BBSRequest error, input json string have no action parameter";
			return ERROR_INPUT_PARAMETER;
		}
		std::string urlParmeter = "";
		if (doc.HasMember("url"))
		{
			urlParmeter = ConvertJSON2Form(doc["url"], "");
		}
		
		bool isPostRequest = false;
		if (doc.HasMember("method"))
		{
			isPostRequest = (doc["method"].GetInt() == 1);
		}

		std::string bodyParmeter = "";
		if (doc.HasMember("body"))
		{
			bodyParmeter = ConvertJSON2Form(doc["body"], "");
		}
		ClientService *clientService = (ClientService*)m_ucEngine->QueryService(UCClientService);
		const LoginInfo	&loginInfo = clientService->GetLoginInfo();

		std::string flag = ":443";
		string doMain = loginInfo.GetUccServerDomain();//"https://bbs.quanshi.com:443"
		int pos = doMain.find(flag);
		if (pos > -1)
		{
			doMain.replace(pos, flag.size(), "");
		}
		

		std::string url =  UCEngine::GetRequestURL(doMain, BBS_MAIN_PATH);
		stringstream urlstream;
		urlstream << url << "/" << strController << "/" << strAction << "?" << SESSION_ID_KEY << "=" << loginInfo.GetSessionId() << "&" << USER_ID_KEY << "=" << loginInfo.GetUserId();
		if (urlParmeter.length() > 0)
		{
			urlstream << "&" << urlParmeter;
		}

		if (taskId == 0)
		{
			taskId = Task::MakeTaskId();
		}
		std::string strMethod(isPostRequest ? HTTP_METHOD_POST : HTTP_METHOD_GET);
		
		HTTPRequest request;
		request.Init(urlstream.str(), strMethod);
		if (bodyParmeter.length() > 0)
		{
			request.SetBody(bodyParmeter);
		}
	
		m_ucEngine->SendAsyncRequest(request, MakeRequestCallback(this, &CBBSService::OnResponse, taskId));
		std::string bodyString((bodyParmeter.length() > 0) ? request.GetBody() : "NULL");
		UC_LOG(INFO) << "CBBSService::BBSRequest had sent async request taskID: " << taskId << " request url: {" << request.GetURL() << " } body: {" << bodyString << "}";
		return UC_SUCCEEDED;
	}

	std::string CBBSService::ConvertJSON2Form(rapidjson::Value &jsonObject, std::string keyName = "")
	{
		std::string strMemberValue;
		if (jsonObject.IsArray())
		{
			int index = 0;
			stringstream urlstream;
			if (jsonObject.Empty())
			{
				strMemberValue = StringConvert::URLEncode(keyName) + "=";
				return strMemberValue;
			}
			for (rapidjson::Value::ConstValueIterator itr = jsonObject.Begin(); itr != jsonObject.End(); ++itr)
			{
				std::string strIndex;
				strIndex = uc::StringConvert::Convert2String(index);
				string preName = keyName + "[" + strIndex + "]";
				std::string strMemberValue;
				if (itr->IsArray())
				{
					strMemberValue = ConvertJSON2Form(jsonObject[index], preName);
				}
				else if (itr->IsObject())
				{
					strMemberValue = ConvertJSON2Form(jsonObject[index], preName);
				}
				else
				{
					std::string strValue = jsonhelper::getStrValue(jsonObject[index], "");
					strMemberValue = StringConvert::URLEncode(preName) + "=" + StringConvert::URLEncode(strValue);
				}
				if (index > 0)
				{
					urlstream << "&";
				}
				urlstream << strMemberValue;
				index++;
			}
			strMemberValue = urlstream.str();
		}
		else if (jsonObject.IsObject())
		{
			int index = 0;
			stringstream urlstream;
			for (rapidjson::Value::ConstMemberIterator itr = jsonObject.MemberBegin(); itr != jsonObject.MemberEnd(); ++itr)
			{
				std::string strMemberValue;
				std::string key = itr->name.GetString(); 
				std::string prekey = "";
				if (keyName.length() > 0)
				{
					prekey = keyName + "[" + key + "]";
				}
				else
				{
					prekey = key;
				}
				if (itr->value.IsArray() )
				{
					strMemberValue = ConvertJSON2Form(jsonObject[itr->name], prekey);
				}
				else if (itr->value.IsObject())
				{
					strMemberValue = ConvertJSON2Form(jsonObject[itr->name], prekey);
				}
				else
				{
					std::string strValue = jsonhelper::getStrValue(itr->value, "");
					strMemberValue = StringConvert::URLEncode(prekey) + "=" + StringConvert::URLEncode(strValue);
					
				}
				if (index > 0)
				{
					urlstream << "&";
				}
				urlstream << strMemberValue;
				index++;
			}
			if (index <= 0)
			{
				strMemberValue = StringConvert::URLEncode(keyName) + "=";
			}
			else 
			{
				strMemberValue = urlstream.str();
			}
		}
		else
		{
			strMemberValue = jsonhelper::getStrValue(jsonObject, "");
		}
		return strMemberValue;
	}

	int32_t CBBSService::OnResponse(int32_t errorCode, HTTPResponse *response, int32_t taskId)
	{
		UC_LOG(INFO) << "CBBSService::OnResponse, " << "taskId: " << taskId <<", errorCode: " << errorCode << ", content: " << (response == NULL ? "" : response->GetContent());
		int32_t result = UC_SUCCEEDED;
		if (errorCode != UC_SUCCEEDED) {
			result = errorCode;
		} 
		else if (response == NULL)
		{
			result = UC_SERVER_ERROR;
		}
		else if (response->GetStatusCode() != 200) {
			result = response->GetStatusCode();
		}

		if (result != UC_SUCCEEDED) {
			triggerEvent(IBBSServiceEvent, OnResponse(taskId ,result , ""));
			return -1;
		}
		triggerEvent(IBBSServiceEvent, OnResponse(taskId, result, response->GetContent()));
		return UC_SUCCEEDED;
	}

	//TODO:: Add Cancel Task
}