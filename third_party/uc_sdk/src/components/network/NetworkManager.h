/*
 * NetworkManager.h
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "curl.h"
#include "HTTPRequestEvent.h"
#include "HTTPRequestCallback.h"
#include "ThreadPool.h"
#include "HTTPHandler.h"
#include "UCASConnection.h"

namespace uc {

class NetworkManager {
public:
	NetworkManager(uint8_t threads);
	NetworkManager(uint8_t threads, uint32_t handlerNumber);

	virtual ~NetworkManager();

public:

	bool Init(IUCEngine *engine);
	bool UnInit();

	/**
	 * 启动线程准备处理异步请求
	 * @return
	 */
	bool Start();

	/**
	 * 停止线程
	 * @return
	 */
	bool Stop();

	/**
	 * http同步请求
	 * @param request
	 * @param response
	 * @return
	 */
	bool SendSyncRequest(HTTPRequest &request, HTTPResponse &response, int32_t timeOut = HTTP_TIMEOUT);

	/**
	 * http异步请求,以event的方式返回
	 * @param request
	 * @param event
	 * @return 错误时返回:UC_FAILED 否则返回 UC_PENDING 表示异步操作正在进行
	 */
	int32_t SendAsyncRequest(int32_t requestId, const HTTPRequest &request, HTTPRequestEvent *event, int32_t timeOut = HTTP_TIMEOUT, int32_t timeOut4Request = HTTP_RESPONSE_TIMEOUT);

	/**
	 * http异步请求,以callback的方式返回
	 * @param request
	 * @param timeOut 超时时间,单位毫秒
	 * @param timeOut4Request 超时时间，针对发送出去的request没有返回, 单位毫秒
	 * @param callback
	 * @return 错误时返回:UC_FAILED 否则返回 UC_PENDING 表示异步操作正在进行
	 */
	int32_t SendAsyncRequest(int32_t requestId, const HTTPRequest &request, RequestCallback *callback, int32_t timeOut = HTTP_TIMEOUT, int32_t timeOut4Request = HTTP_RESPONSE_TIMEOUT);

	/**
	 * 退出异步请求
	 * @param request 异步请求的id
	 * @return 返回 UC_SUCCEEDED 表示cancel操作成功, UC_PENDING 表示cancel 操作将异步结束
	 */
	int32_t CancelAysncRequest(int32_t requestId);

private:
	/**
	 * 线程函数,处理http请求
	 * @param manager
	 */
	void ThreadProc();
	void DoHTTPRequest(CURLM *multiHandler, deque<HTTPHandler*> &handlers);
	bool RemoveHttpHandler(CURLM *multiHandler, std::map<CURL*, HTTPHandler*> &handlers, int32_t requestId);
	virtual void AfterHttpRequestDone() {}
	virtual void BeforeHandlerSleep() {}

protected:
	void WakeUpHandlers() { _handlerCond.notify_one(); }

protected:
	IUCEngine *_engine;

private:
	std::list<HTTPHandler*> _handlers; // 准备执行的http请求
	std::list<int32_t> _cancelHdandlers; // 准备要执行cancel操作的handle
	boost::recursive_mutex _mtxHandlers;
	boost::recursive_mutex _mtxCancelHandler;
	boost::condition_variable_any _handlerCond;

	ThreadPool _threadPool;
	uint8_t _threadNumber;
	uint32_t _handlerNumber;  // 并发http请求个数;
	bool _stop;
};

} /* namespace uc */
#endif /* NETWORKMANAGER_H_ */
