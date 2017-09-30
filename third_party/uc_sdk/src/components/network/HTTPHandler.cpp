/*
 * HTTPHandler.cpp
 *
 *  Created on: 2015年3月6日
 *      Author: jianxue
 */

#include "HTTPHandler.h"

#include <iostream>
#include <sstream>
#include "common/StringConvert.h"
#include "LogManager.h"
//#include "DBManager.h"
#include <regex>
//#include "client_service/ClientService.h"
using namespace std;

namespace uc {

HTTPHandler::HTTPHandler() {
	_id = -1;
	_handler = NULL;
	_header = NULL;
	_request = NULL;
	_response = NULL;
	_requestEvent = NULL;
	_callback = NULL;
	_isSync = true;
	_timeOut = HTTP_TIMEOUT;
	_timeOut4Request = HTTP_RESPONSE_INFINITY; // -1 表示不用设置，默认的无限
	_cur_proxy_auth_type = 0; //0: Basic; 1: Digest; 2: NTLM
}

HTTPHandler::~HTTPHandler() {
	Cleanup();
}


/**
 * 接收到http header数据的返回
 * @param buffer
 * @param size
 * @param nitems
 * @param userdata
 * @return
 */
size_t HTTPHeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata) {

	if (buffer == NULL || userdata == NULL) {
		return CURLE_OK;
	}

	HTTPHandler	*handler = (HTTPHandler*)userdata;

	if (handler == NULL) {
		return CURLE_OK;
	}

	string data(buffer, size*nitems);

	if (handler->GetResponse()->GetStatusCode() == -1) {
		int32_t major = 0;
		int32_t version = 0;
		int32_t statusCode = 0;

		sscanf(data.c_str(), "HTTP/%d.%d %d", &major, &version, &statusCode);
		handler->GetResponse()->SetStatusCode(statusCode);
		handler->GetResponse()->ClearHeadInfo();
		UC_LOG(INFO) << "HTTP/" << major << "." << version << " " << statusCode;
	} else {
		string key;
		string value;

		int32_t Find = data.find(":");
		if (Find != -1) {
			key = data.substr(0, Find);
			value = data.substr(Find+1, data.length()-Find);
			value.erase(0, value.find_first_not_of(" "));
			value.erase(value.find_last_not_of("\r\n")+1);

			handler->GetResponse()->AddHeaderInfo(key, value);
		} else {
			// 说明header数据已经接收完毕
			if (handler->GetRequestEvent() != NULL) {
 				handler->GetRequestEvent()->OnHeaderReceived(handler->GetId(), handler->GetRequest(),
 					handler->GetResponse()->GetStatusCode(), handler->GetCurProxyAuthType(), handler->GetResponse()->GetHeaderInfo());
			}

			if (handler->GetResponse()->GetStatusCode() == 407)
			{
 				if (handler->GetCurProxyAuthType() == 1 || handler->GetCurProxyAuthType() == 2)
 				{
 					handler->GetResponse()->SetStatusCode(-1);
 				}
			}
		}
	}
	return data.length();
}

/**
 * 发送body数据
 * @param buffer 用户需要把发送的数据kaob
 * @param size
 * @param nitems
 * @param userData 用户指针
 * @return
 */
size_t HTTPReadCallback(char *buffer, size_t size, size_t nitems, void *userData) {

	if (buffer == NULL) {
		return CURLE_OK;
	}

	int32_t length = size*nitems;

	if (length <= 0) {
		return CURLE_OK; 
	}

	HTTPHandler *handler = (HTTPHandler*)userData;

	if (handler == NULL) {
		return CURLE_OK;
	}

	if (handler->GetRequestEvent() != NULL) {
		handler->GetRequestEvent()->OnDataSend(handler->GetId(), handler->GetRequest(), buffer, length);
		return length;
	}

	return CURLE_OK;

}

/**
 * 接收到http body数据
 * @param ptr
 * @param size
 * @param nmemb
 * @param userdata
 * @return
 */
size_t HTTPWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	if (ptr == NULL || userdata == NULL) {
		return CURLE_OK;
	}

	HTTPHandler *handler = (HTTPHandler*)userdata;

	if (handler == NULL) {
		return CURLE_OK;
	}

	int32_t length = size*nmemb;

	if (handler->GetRequestEvent() != NULL) {
		handler->GetRequestEvent()->OnDataReceived(handler->GetId(), handler->GetRequest(), ptr, length);
	} else {
		handler->GetResponse()->AppendContent(ptr, length);
	}

	return length;
}

/**
 *
 * @param handle
 * @param type
 * @param data
 * @param size
 * @param userptr
 * @return
 */
int HTTPDebugCallback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {

	if (data == NULL) {
		return CURLE_OK;
	}

	string ValueType = "";
	string Value(data, size);

	switch (type) {
		case CURLINFO_TEXT: {
			ValueType = "TEXT:";
			break;
		}
		case CURLINFO_DATA_IN: {
			ValueType = "DATA_IN:";
			break;
		}
		case CURLINFO_DATA_OUT: {
			ValueType = "DATA_OUT:";
			break;
		}
		case CURLINFO_HEADER_IN: {
			ValueType = "HEADER_IN:";
			break;
		}
		case CURLINFO_HEADER_OUT: {
			ValueType = "HEADER_OUT:";
			break;
		}
		case CURLINFO_SSL_DATA_IN: {
		//	ValueType = "SSL_DATA_IN:";
			break;
		}
		case CURLINFO_SSL_DATA_OUT: {
		//	ValueType = "SSL_DATA_OUT:";
			break;
		}
		case CURLINFO_END: {
			ValueType = "END:";
			break;
		}
		default:
		{
			ValueType = "default:";
			break;
		}
	}

	if (!ValueType.empty()) {
		UC_LOG(INFO) << ValueType << Value;
	}
	return CURLE_OK;
}

bool HTTPHandler::Init(HTTPRequest &request, HTTPResponse &response, int32_t timeOut /*= HTTP_TIMEOUT*/) {

	_request = &request;
	_response = &response;
	_isSync = true;
	_timeOut = timeOut;
	return true;
}

bool HTTPHandler::Init(int32_t id, const HTTPRequest &request, HTTPRequestEvent *event, int32_t timeOut /*= HTTP_TIMEOUT*/, int32_t timeOut4Request /*= HTTP_RESPONSE_TIMEOUT*/) {
	if (event == NULL) {
		return false;
	}

	_id = id;
	_request = new HTTPRequest(request);
	_response = new HTTPResponse();
	_requestEvent = event;
	_isSync = false;
	_timeOut = timeOut;
	_timeOut4Request = timeOut4Request;
	return true;
}

bool HTTPHandler::Init(int32_t id, const HTTPRequest &request, RequestCallback *callback, int32_t timeOut /*= HTTP_TIMEOUT*/, int32_t timeOut4Request /*= HTTP_RESPONSE_TIMEOUT*/) {
	if (callback == NULL) {
		return false;
	}

	_id = id;
	_request = new HTTPRequest(request);
	_response = new HTTPResponse();
	_callback = callback;
	_isSync = false;
	_timeOut = timeOut;
	_timeOut4Request = timeOut4Request;
	return true;
}

int32_t HTTPHandler::GetId() const {
	return _id;
}

CURL* HTTPHandler::GetHandler() const {
	return _handler;
}

HTTPRequest* HTTPHandler::GetRequest() const {
	return _request;
}

HTTPResponse* HTTPHandler::GetResponse() const {
	return _response;
}

HTTPRequestEvent* HTTPHandler::GetRequestEvent() const {
	return _requestEvent;
}

RequestCallback* HTTPHandler::GetCallback() const {
	return _callback;
}

bool HTTPHandler::Setup(const ProxyInfo& proxyInfo, const int& settings, const std::string &certPath) {

	if (_handler != NULL || _request == NULL) {
		return false;
	}

	_handler = curl_easy_init();
	if (_handler == NULL) {
		return false;
	}

	UC_LOG(INFO) << "url:" << _request->GetURL();
	curl_easy_setopt(_handler, CURLOPT_FORBID_REUSE, 1L);
	curl_easy_setopt(_handler, CURLOPT_URL, _request->GetURL().c_str());
	curl_easy_setopt(_handler, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(_handler, CURLOPT_CONNECTTIMEOUT_MS, _timeOut);
	if (_timeOut4Request != HTTP_RESPONSE_INFINITY)
		curl_easy_setopt(_handler, CURLOPT_TIMEOUT_MS, _timeOut4Request);

	// 设置 HTTP Header
	HeaderMap::const_iterator itor;
	string headerInfo;
	stringstream sStream;

	if (_header != NULL) {
		curl_slist_free_all(_header);
		_header = NULL;
	}
	
	// 去掉Expect: 100-continue
	_header = curl_slist_append(_header, "Expect:");

	for (itor = _request->GetHeaderInfo().begin(); itor != _request->GetHeaderInfo().end(); ++itor) {
		sStream.str(std::string());
		sStream.clear();
		sStream << itor->first << ": " << itor->second;
		headerInfo = sStream.str();
		_header = curl_slist_append(_header, headerInfo.c_str());
	}

	if (_header != NULL) {
		curl_easy_setopt(_handler, CURLOPT_HTTPHEADER, _header);
	}

	// 设置body数据
	if (_request->GetMethod() == HTTP_METHOD_POST) {
		// log里过滤password
		//UC_LOG(INFO) << "body:" << _request->GetBody();
		std::string body = _request->GetBody();
		std::regex pattern("(user_account=.*&password=).*(&client_type.*)*");
		std::string replace = "$1******$2";
		UC_LOG(INFO) << "body:" << std::regex_replace(body, pattern, replace);

		curl_easy_setopt(_handler, CURLOPT_POST, 1L);
		if (_request->GetBody().length() > 0) {
			// body有数据时,直接设置,否则从ReadFunction中读取
			curl_easy_setopt(_handler, CURLOPT_POSTFIELDSIZE, _request->GetBody().length());
			curl_easy_setopt(_handler, CURLOPT_POSTFIELDS, _request->GetBody().c_str());
		}
	} else {
		curl_easy_setopt(_handler, CURLOPT_HTTPGET, 1L);
	}

	curl_easy_setopt(_handler, CURLOPT_FOLLOWLOCATION, 1L);

	if ((settings & 2))
	{
		curl_easy_setopt(_handler, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(_handler, CURLOPT_SSL_VERIFYPEER, 0L);
	}
	else
	{
		curl_easy_setopt(_handler, CURLOPT_SSL_VERIFYHOST, 2L);
		curl_easy_setopt(_handler, CURLOPT_SSL_VERIFYPEER, 1L);
		//curl_easy_setopt(_handler, CURLOPT_CAINFO, "D:\\workspace\\uc\\client\\branches\\PC2.0\\PC2.0_for_main_features\\build\\Debug\\oncloud2CA.pem");
		curl_easy_setopt(_handler, CURLOPT_CAINFO, certPath.c_str());
	}

	// 设置回调函数
	curl_easy_setopt(_handler, CURLOPT_HEADERFUNCTION, HTTPHeaderCallback);
	curl_easy_setopt(_handler, CURLOPT_HEADERDATA, this);
	curl_easy_setopt(_handler, CURLOPT_READFUNCTION, HTTPReadCallback);
	curl_easy_setopt(_handler, CURLOPT_READDATA, this);
	curl_easy_setopt(_handler, CURLOPT_WRITEFUNCTION, HTTPWriteCallback);
	curl_easy_setopt(_handler, CURLOPT_WRITEDATA, this);

	if (!proxyInfo.GetProxyUrl().empty())
	{
		if (strncmp("http", proxyInfo.GetProxyUrl().c_str(), 4) == 0)
		{
			curl_easy_setopt(_handler, CURLOPT_PROXY, proxyInfo.GetProxyUrl().c_str());
			curl_easy_setopt(_handler, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
			_cur_proxy_url = proxyInfo.GetProxyUrl().c_str();
			_cur_proxy_auth_type = proxyInfo.GetAuthType();

			if (!proxyInfo.GetProxyAccount().empty() && !proxyInfo.GetProxyPassword().empty())
			{
				std::string account_pwd = proxyInfo.GetProxyAccount() + ":" + proxyInfo.GetProxyPassword();
				if (_cur_proxy_auth_type == 0)
				{
					curl_easy_setopt(_handler, CURLOPT_PROXYAUTH, CURLAUTH_BASIC);
					curl_easy_setopt(_handler, CURLOPT_PROXYUSERPWD, account_pwd.c_str());
				}
				else if (_cur_proxy_auth_type == 1)
				{
					curl_easy_setopt(_handler, CURLOPT_PROXYAUTH, CURLAUTH_DIGEST);
					curl_easy_setopt(_handler, CURLOPT_PROXYUSERPWD, account_pwd.c_str());
				}
				else if (_cur_proxy_auth_type == 2)
				{
					curl_easy_setopt(_handler, CURLOPT_PROXYAUTH, CURLAUTH_NTLM);
					curl_easy_setopt(_handler, CURLOPT_PROXYUSERPWD, account_pwd.c_str());
				}
				else
				{
					UC_LOG(WARNING) << "we do not support this auth so far. auth_type: " << proxyInfo.GetAuthType();
				}
			}

			UC_LOG(INFO) << "set http proxy info url: " << proxyInfo.GetProxyUrl().c_str() << "  authType: " << proxyInfo.GetAuthType()
				<< " account: " << proxyInfo.GetProxyAccount().c_str() << " password: " << proxyInfo.GetProxyPassword().c_str();
		}
		else
		{
			UC_LOG(INFO) << "it is not http proxy. we just handle http proxy. proxy_url: " << proxyInfo.GetProxyUrl().c_str();
		}
	}

	if ((settings & 1 ))
	{
		curl_easy_setopt(_handler, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(_handler, CURLOPT_DEBUGFUNCTION, HTTPDebugCallback);
		curl_easy_setopt(_handler, CURLOPT_DEBUGDATA, NULL);
	}

	return true;
}

std::string HTTPHandler::GetCurProxyUrl() const
{
	return _cur_proxy_url;
}

int32_t HTTPHandler::GetCurProxyAuthType() const
{
	return _cur_proxy_auth_type;
}

void HTTPHandler::CheckProxy()
{
	if (_response && _response->GetStatusCode() != 200)
	{
		if (!_cur_proxy_url.empty())
		{
			//在用户设置代理账号和密码的地方修改DB数据.
			//DBManager::GetInstance().DeleteAuthInfo(_cur_proxy_url);
			_response->SetStatusCode(407);
		}
	}
}

void HTTPHandler::Cleanup() {

	CheckProxy();
	_id = -1;
	if (_handler != NULL) {
		curl_easy_cleanup(_handler);
		_handler = NULL;
	}

	if (_header != NULL) {
		curl_slist_free_all(_header);
		_header = NULL;
	}

	if (_callback != NULL) {
		delete _callback;
		_callback = NULL;
	}

	_requestEvent = NULL;
	if (!_isSync) {

		if (_request != NULL) {
			delete _request;
			_request = NULL;
		}
		if (_response != NULL) {
			delete _response;
			_response = NULL;
		}
	}

	_cur_proxy_url.clear();
	_cur_proxy_auth_type = 0;
	_isSync = false;
}

void HTTPHandler::StartTimer() {
//	time(&_tmCount);

//	_tmCount = GetTickCount();
}

void HTTPHandler::EndTimer() {
	//time_t tmCurrent;
//	DWORD tmCurrent = GetTickCount();


//	UC_LOG(INFO) << "HTTP Request Time: " << (tmCurrent - _tmCount) << "---" << _request->GetURL();
}
} /* namespace uc */
