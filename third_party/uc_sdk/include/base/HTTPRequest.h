/*
 * HTTPRequest.h
 *
 *  Created on: 2015年2月12日
 *      Author: jianxue
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#include <map>
#include <string>
#include "UCSDK.h"

using namespace std;

namespace uc {

typedef map<string, string>	HeaderMap;

class UC_API HTTPRequest {
public:
	HTTPRequest();
	HTTPRequest(const HTTPRequest &request);
	virtual ~HTTPRequest();

public:
	/**
	 * 初始化http 请求,设置请求的url和Get或者Post方法
	 * @param url
	 * @param method
	 */
	void Init(string url, string method = HTTP_METHOD_GET);
	/**
	 * 设置http请求的头信息
	 * @param name
	 * @param value
	 */
	void AddHeaderInfo(const string &name, const string &value);
	void AddHeaderInfo(string name, uint32_t value);

	void SetBody(string body);

	const string& GetMethod() const;
	const string& GetURL() const;
	const string& GetBody() const;
	const HeaderMap& GetHeaderInfo() const;

	void SetSessionId(const std::string & sessionId);
	const std::string & GetSessionId() const;
private:
	HTTPRequest& operator=(const HTTPRequest &request);

private:
	string _method;
	string _url;
	string _body;
	HeaderMap _headerInfo;

	std::string _sessionId;
};

} /* namespace uc */

#endif /* HTTPREQUEST_H_ */
