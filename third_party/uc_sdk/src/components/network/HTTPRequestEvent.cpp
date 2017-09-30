/*
 * HTTPRequestEvent.cpp
 *
 *  Created on: 2015年5月11日
 *      Author: jianxue
 */

#include "HTTPRequestEvent.h"
#include <iostream>
#include "LogManager.h"

namespace uc {

HTTPRequestEvent::HTTPRequestEvent() {

}

HTTPRequestEvent::~HTTPRequestEvent() {

}

void HTTPRequestEvent::OnHeaderReceived(int32_t requestId, HTTPRequest *request, int32_t statusCode, int32_t authType, const HeaderMap &header) {
	UC_LOG(INFO) << "HTTPRequestEvent::OnReceiveHeader: " << requestId;
}

void HTTPRequestEvent::OnDataSend(int32_t requestId, HTTPRequest *request, char *buffer, int32_t &size) {
	UC_LOG(INFO) << "HTTPRequestEvent::OnDataSend" << requestId;
}

void HTTPRequestEvent::OnDataReceived(int32_t requestId, HTTPRequest *request, void *data, int32_t length) {
	UC_LOG(INFO) << "HTTPRequestEvent::OnReceiveData: " << requestId;
}

void HTTPRequestEvent::OnRequestFinished(int32_t requestId, HTTPRequest *request, int32_t errorCode) {
	UC_LOG(INFO) << "HTTPRequestEvent::OnReceiveFinish: " << requestId;
}

} /* namespace uc */
