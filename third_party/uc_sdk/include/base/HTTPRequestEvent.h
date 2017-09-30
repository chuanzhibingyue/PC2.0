/*
 * HTTPRequestEvent.h
 *
 *  Created on: 2015年5月11日
 *      Author: jianxue
 */

#ifndef HTTPREQUESTEVENT_H_
#define HTTPREQUESTEVENT_H_

#include "HTTPRequest.h"
#include "HTTPResponse.h"

namespace uc {

class HTTPRequestEvent {
public:
	HTTPRequestEvent();
	virtual ~HTTPRequestEvent();

public:
	/**
	 * 接收完http头信息后触发
	 * @param requestId,
	 * @param request
	 * @param statusCode
	 * @param header
	 */
	virtual void OnHeaderReceived(int32_t requestId, HTTPRequest *request, int32_t statusCode, int32_t authType, const HeaderMap &header);

	/**
	 * 发送body数据时触发
	 * @param requestId
	 * @param request
	 * @param buffer 需要赋值的字符指针
	 * @param size 允许赋值的字符长度
	 * return 最终赋值的字符长度
	 */
	virtual void OnDataSend(int32_t requestId, HTTPRequest *request, char *buffer, int32_t &size);

	/**
	 * 接收到body数据时触发
	 * @param request
	 * @param data
	 * @param length
	 */
	virtual void OnDataReceived(int32_t requestId, HTTPRequest *request, void *data, int32_t length);

	/**
	 * http请求结束时触发
	 * @param request
	 */
	virtual void OnRequestFinished(int32_t requestId, HTTPRequest *request, int32_t errorCode);
};

} /* namespace uc */

#endif /* HTTPREQUESTEVENT_H_ */
