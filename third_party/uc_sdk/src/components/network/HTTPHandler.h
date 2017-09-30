/*
 * HTTPHandler.h
 *
 *  Created on: 2015年3月6日
 *      Author: jianxue
 */

#ifndef HTTPHANDLER_H_
#define HTTPHANDLER_H_

#include "curl.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "HTTPRequestEvent.h"
#include "HTTPRequestCallback.h"
#include "client_service/ProxyInfo.h"

namespace uc {

class HTTPHandler {
public:
	HTTPHandler();
	virtual ~HTTPHandler();

public:
	/**
	 * 初始化同步请求。
	 * @param request
	 * @param response
	 * @return
	 */
	bool Init(HTTPRequest &request, HTTPResponse &response, int32_t timeOut = HTTP_TIMEOUT);

	/**
	 * 初始化异步请求,通过event回调结果
	 * @param request
	 * @param event
	 * @return
	 */
	bool Init(int32_t id, const HTTPRequest &request, HTTPRequestEvent *event, int32_t timeOut = HTTP_TIMEOUT, int32_t timeOut4Request = HTTP_RESPONSE_TIMEOUT);

	/**
	 * 初始化异步请求,通过callback回调结果
	 * @param request
	 * @param callback
	 * @return
	 */
	bool Init(int32_t id, const HTTPRequest &request, RequestCallback *callback, int32_t timeOut = HTTP_TIMEOUT, int32_t timeOut4Request = HTTP_RESPONSE_TIMEOUT);

	/**
	 * 生成CURL 对象,设置libcurl的各项回调函数
	 * @param enable_log 是否打开log
	 * @return
	 */
	bool Setup(const ProxyInfo& proxyInfo, const int& enable_log, const std::string &certPath);

	/**
	 * 清除各项数据
	 */
	void Cleanup();
	void CheckProxy();
	
	void StartTimer();
	void EndTimer();

	CURL* GetHandler() const;
	HTTPRequest* GetRequest() const;
	HTTPResponse* GetResponse() const;
	HTTPRequestEvent* GetRequestEvent() const;
	RequestCallback* GetCallback() const;
	int32_t GetId() const;
	std::string GetCurProxyUrl() const;
	int32_t GetCurProxyAuthType() const;

private:
	int32_t _id;
	CURL *_handler;
	curl_slist 	*_header;
	HTTPRequest *_request;
	HTTPResponse *_response;
	HTTPRequestEvent *_requestEvent;
	RequestCallback *_callback;
	bool _isSync; // 是否为同步请求
	int32_t _timeOut;
	int32_t _timeOut4Request;
	int32_t _cur_proxy_auth_type;
	std::string _cur_proxy_url;
//	DWORD _tmCount; // 用来统计http请求的时间
};

} /* namespace uc */

#endif /* HTTPHANDLER_H_ */
