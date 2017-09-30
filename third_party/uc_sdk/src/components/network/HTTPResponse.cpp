/*
 * HTTPResponse.cpp
 *
 *  Created on: 2015Äê2ÔÂ12ÈÕ
 *      Author: jianxue
 */

#include "HTTPResponse.h"

namespace uc {

HTTPResponse::HTTPResponse() {
	// TODO Auto-generated constructor stub
	_statusCode = -1;
}

HTTPResponse::~HTTPResponse() {
	// TODO Auto-generated destructor stub
}

/**
 *
 * @return
 */
int32_t HTTPResponse::GetStatusCode() const{
	return _statusCode;
}

/**
 *
 * @param StatusCode
 */
void HTTPResponse::SetStatusCode(int32_t statusCode) {
	_statusCode = statusCode;
}

const string& HTTPResponse::GetContent() const{
	return _content;
}

/**
 *
 * @param Content
 * @param Length
 * @return
 */
const string& HTTPResponse::AppendContent(const char *content, int32_t length) {
	_content.append(content, length);
	return _content;
}

/**
 *
 */
void HTTPResponse::ClearContent() {
	_content.clear();
}

void HTTPResponse::ClearHeadInfo(){
	_headerInfo.clear();
}

void HTTPResponse::AddHeaderInfo(const string &name, const string &value) {
	HeaderMap::iterator iter = _headerInfo.find(name);
	if (iter != _headerInfo.end()){
		iter->second.append(";");
		iter->second.append(value);
	}
	else{
		_headerInfo.insert(pair<string, string>(name, value));
	}
	
}

const HeaderMap& HTTPResponse::GetHeaderInfo() const{
	return _headerInfo;
}

string HTTPResponse::GetHeaderValue(const string& key)
{
	string value;
	HeaderMap::iterator iter = _headerInfo.find(key);
	if (iter != _headerInfo.end())
	{
		value = iter->second;
	}
	return value;
}

} /* namespace uc */
