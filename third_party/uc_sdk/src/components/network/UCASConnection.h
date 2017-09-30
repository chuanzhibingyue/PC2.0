/*
 * UCASConnection.h
 *
 *  Created on: 2015年7月4日
 *      Author: xuejian
 */

#ifndef UCASCONNECTION_H_
#define UCASCONNECTION_H_

#include <string>
#include "IUCEngine.h"
#include "HTTPHandler.h"
#include <boost/thread.hpp>

namespace uc {

class UCASConnection : public HTTPRequestEvent {
public:
	UCASConnection();
	virtual ~UCASConnection();
public:
	bool Init(IUCEngine *engine);
	bool UnInit();

	bool Connect(const std::string &serverURL);

	bool Disconnect();

	static void ThreadProc(UCASConnection *connection, string &serverURL);

public:
	virtual void OnHeaderReceived(int32_t requestId, HTTPRequest *request, int32_t statusCode, int32_t authType, const HeaderMap &header);
	virtual void OnDataReceived(int32_t requestId, HTTPRequest *request, void *data, int32_t length);
	virtual void OnRequestFinished(int32_t requestId, HTTPRequest *request, int32_t errorCode);

private:
	void ParseUCASData(std::string &data);

	void SetNoopTimer();
	void CancelNoopTimer();
	void OnNoDataReceivedWithLongTime(bool in_main_thread);

	void triggerEventInMainThread(int errorCode);

private:
	IUCEngine *_ucEngine;
	bool _closed;
	boost::thread *_thread;
	std::string _content; // 
	unsigned char *_thriftData;
	unsigned long _thriftDataLen;
	bool _isFirstNoopData;
	int64_t _timerId;
	boost::recursive_mutex _mtx;
};

} /* namespace uc */

#endif /* UCASCONNECTION_H_ */
