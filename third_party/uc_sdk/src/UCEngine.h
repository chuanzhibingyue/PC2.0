/*
 * UCEngine.h
 *
 *  Created on: Feb 11, 2015
 *      Author: ethan
 */

#ifndef UCENGINE_H_
#define UCENGINE_H_

#include <map>
#include "common/event.h"
#include "IUCEngine.h"
#include "network/QueuedNetWorkManager.h"

namespace uc {

using namespace std;

class UCEngine : public IUCEngine {
public:
	UCEngine(UCEngineConfig &config);
	virtual ~UCEngine();

public:
	virtual IUCService* QueryService(UCServiceType type);
	virtual bool Init();
	virtual void UnInit();
	virtual UCEngineConfig& GetConfig();

	// 对NetworkManager里面http请求的封装
	virtual int32_t SendAsyncRequest(int32_t requestId, const HTTPRequest &request, HTTPRequestEvent *event, int32_t timeOut = HTTP_TIMEOUT, int32_t timeOut4Request = HTTP_RESPONSE_TIMEOUT);
	virtual int32_t SendAsyncRequest(const HTTPRequest &request, RequestCallback *callback, int32_t timeOut = HTTP_TIMEOUT, int32_t timeOut4Request = HTTP_RESPONSE_TIMEOUT);
	virtual bool SendSyncRequest(HTTPRequest &request, HTTPResponse &response, int32_t timeOut = HTTP_TIMEOUT);

	void RegisterQueueNetMgrListener(QueuedNetWorkManagerListener* listener);
	void SendQueuedRequest(ConversationType type, int64_t userId, int32_t msgId, const std::string & body);

	void ConnectUCAS(const string &url);
	void DisconnectUCAS();

	void ConnectUCC();
	void DisconnectUCC();
public:
	static string GetRequestURL(const string &serverURL, const string &methodURL);

private:
	void InitServices();
	IUCService* CreateService(UCServiceType type);

private:
	typedef std::map<UCServiceType, IUCService*> SERVICEMAP;
	SERVICEMAP ServiceMap;
	QueuedNetWorkManager _UCCNetwork;
	UCASConnection _connection;
	UCEngineConfig _config;
};

} /* namespace uc */

#endif /* UCENGINE_H_ */
