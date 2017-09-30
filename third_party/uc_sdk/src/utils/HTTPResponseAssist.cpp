/*
 * CallRequestAssist.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: hualong.zhang
 */

#include <iostream>
#include "HTTPResponseAssist.h"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include "common/StringConvert.h"
#include <CommonDef.h>
#include "LogManager.h"
#include "UCErrorCode.h"
#include "HTTPResponse.h"

namespace uc {

using namespace std;

HTTPResponseAssist::HTTPResponseAssist() {

}

HTTPResponseAssist::~HTTPResponseAssist() {

}

int32_t HTTPResponseAssist::GuardStatementForCallBackCode(int32_t errorCode, int32_t statusCode) {
	if (0 != errorCode) {
		UC_LOG(ERROR) << "HTTPResponseAssist::GuardStatementForCallBack, response is invalid.";
		return UC_FAILED;
	}

	if (200 != statusCode) {
		UC_LOG(ERROR) << "HTTPResponseAssist::GuardStatementForCallBack, request failed, response status code = "
			<< statusCode;
		return UCC_STATUS_CODE_ERROR;
	}

	return UC_SUCCEEDED;
}

int32_t HTTPResponseAssist::GuardStatementForCallBack(int32_t errorCode, const HTTPResponse *response) {
	int32_t result = GuardStatementForCallBackCode(errorCode, response->GetStatusCode());
	if (UC_SUCCEEDED != result)
	{
		UC_LOG(ERROR) << "GuardStatementForCallBackCode return error:" << result << 
			", response:" << (response != NULL ? response->GetContent() : "");
		return result;
	}
	return _RetrieveErrorCodeFromHttpContent(response->GetContent());
}

int32_t HTTPResponseAssist::GuardStatementForCallBack(const std::string &method, int32_t errorCode, const HTTPResponse *response) {
	UC_LOG(INFO) << method << ": " << errorCode << ", content: " << (response == NULL ? "" : response->GetContent());

	if (errorCode != UC_SUCCEEDED) {
		// 表示调用失败,response不是有效的数据
		return errorCode;
	}

	if (response->GetStatusCode() != 200) {
		// 表示http请求失败
		return response->GetStatusCode();
	}

	return UC_SUCCEEDED;
}

int HTTPResponseAssist::_RetrieveErrorCodeFromHttpContent(const std::string &content) {
	if (!_RetrieveJsonPtreeFromHttpContent(content)) {
		return UC_SERVER_ERROR;
	}
	int32_t contentCode = RetrieveHttpContentCode();
	if (0 != contentCode) {
		UC_LOG(ERROR) << "response code error:" << contentCode << ", content:" << content;
	}
	return contentCode;
}

bool HTTPResponseAssist::_RetrieveJsonPtreeFromHttpContent(const string &content) {
	try {
		_RetrieveJsonPtreeFromHttpContentInternal(_ptRoot, content);
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an error when reading json object from http content, exception: " << e.what();
		return false;
	}
	return true;
}

void HTTPResponseAssist::_RetrieveJsonPtreeFromHttpContentInternal(wptree &ptRoot, const string &content) {
	wstringstream jsonStream;
	jsonStream.str(StringConvert::FromUTF8Str(content));
	json_parser::read_json(jsonStream, ptRoot);
}

int32_t HTTPResponseAssist::RetrieveHttpContentCode() const{
	return _ptRoot.get<int32_t>((L"code"), -1);
}

const wptree &HTTPResponseAssist::ReturnWPTree() const {
	return _ptRoot;
}
}
