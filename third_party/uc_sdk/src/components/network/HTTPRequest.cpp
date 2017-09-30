/*
 * HTTPRequest.cpp
 *
 *  Created on: 2015年2月12日
 *      Author: jianxue
 */

#include "base/HTTPRequest.h"
#include "common/StringConvert.h"
#include <sstream>

namespace uc {

HTTPRequest::HTTPRequest() {
	// TODO Auto-generated constructor stub
	_method = HTTP_METHOD_GET;
	_url 	= "";
	_sessionId = "";
}

HTTPRequest::HTTPRequest(const HTTPRequest &request) {
	_method = request.GetMethod();
	_url = request.GetURL();
	_body = request.GetBody();
	_sessionId = request.GetSessionId();

	HeaderMap::const_iterator itor;

	for (itor = request.GetHeaderInfo().begin(); itor != request.GetHeaderInfo().end(); ++itor) {
		_headerInfo.insert(pair<string, string>(itor->first, itor->second));
	}
}

HTTPRequest::~HTTPRequest() {
	// TODO Auto-generated destructor stub
}

/**
 *
 * @param Method
 * @param URL
 */
void HTTPRequest::Init(string url, string method /*= HTTP_METHOD_GET*/) {
	_method = method;
	_url = url;
}

/**
 *
 * @param Name
 * @param Value
 */
void HTTPRequest::AddHeaderInfo(const string &name, const string &value) {
	_headerInfo.insert(pair<string, string>(name, value));
}

/**
 * body数据
 * @param body
 */
void HTTPRequest::SetBody(string body) {
	_body = body;
}

/**
 *
 * @param Name
 * @param Value
 */
void HTTPRequest::AddHeaderInfo(string name, uint32_t value) {
	stringstream sstream;

	sstream << value;

	AddHeaderInfo(name, sstream.str());
}

/**
 *
 * @return
 */
const string& HTTPRequest::GetMethod() const {
	return _method;
}

/**
 *
 * @return
 */
const string& HTTPRequest::GetURL() const {
	return _url;
}

/**
 *
 * @return
 */
const HeaderMap& HTTPRequest::GetHeaderInfo() const {
	return _headerInfo;
}

const string& HTTPRequest::GetBody() const {
	return _body;
}

void HTTPRequest::SetSessionId(const std::string & sessionId)
{
	_sessionId = sessionId;
}

const std::string & HTTPRequest::GetSessionId() const
{
	return _sessionId;
}

} /* namespace uc */


