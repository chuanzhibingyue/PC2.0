/*
 * NetworkManager.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#include "NetworkManager.h"
#include "curl.h"
#include "HTTPResponse.h"
#include "HTTPHandler.h"
#include "LogManager.h"
#include "common/StringConvert.h"
#include "UCErrorCode.h"

#include "client_service/ClientService.h"

namespace uc {

NetworkManager::NetworkManager(uint8_t threads) : _threadPool(threads) {
	_stop = false;
	_engine = NULL;
	_threadNumber = threads;
	_handlerNumber = 0;
}

NetworkManager::NetworkManager(uint8_t threads, uint32_t handlerNumber) : _threadPool(threads){
	_stop = false;
	_engine = NULL;
	_threadNumber = threads;
	_handlerNumber = handlerNumber;
}

NetworkManager::~NetworkManager() {
}

bool NetworkManager::Init(IUCEngine *engine) {
	_engine = engine;
	return true;
}

bool NetworkManager::UnInit() {
	return true;
}

bool NetworkManager::Start() {
	if (_threadNumber <= 0) {
		return false;
	}

	UC_LOG(INFO) << "networkmgr start";
	curl_global_init(CURL_GLOBAL_DEFAULT);
	_threadPool.Init();
	for (int32_t index = 0; index < _threadNumber; index++) {
		_threadPool.Schedule(MakeCallback(this, &NetworkManager::ThreadProc));
	}
	_stop = false;
	return true;
}

bool NetworkManager::Stop() {
	UC_LOG(INFO) << "networkmgr stop";
	_stop = true;
	_handlerNumber = 0;
	_handlerCond.notify_all();
	_threadPool.Wait();
	_threadPool.Shutdown();
	curl_global_cleanup();

	_mtxHandlers.lock();
	// 清除缓存未处理的http request
	std::list<HTTPHandler*>::iterator itor = _handlers.begin();
	while (itor != _handlers.end()) {
		HTTPHandler *handler = (HTTPHandler*)(*itor);

		// stop的时候抛事件给UI
		if (handler != NULL) {
			if (handler->GetCallback() != NULL) {
				handler->GetCallback()->OnRequestComplete(UC_FAILED, NULL);
			}

			if (handler->GetRequestEvent() != NULL) {
				handler->GetRequestEvent()->OnRequestFinished(handler->GetId(), handler->GetRequest(), UC_FAILED);
			}

			delete handler;
			handler = NULL;
		}
		itor++;
	}

	_handlers.clear();
	_mtxHandlers.unlock();

	// 清除待删除列表
	_mtxCancelHandler.lock();
	_cancelHdandlers.clear();
	_mtxCancelHandler.unlock();

	return true;
}

/**
 * http同步请求
 * @param request
 * @param response
 * @return
 */
bool NetworkManager::SendSyncRequest(HTTPRequest &request, HTTPResponse &response, int32_t timeOut /*= HTTP_TIMEOUT*/) {

	CURLcode 	returnCode 	= CURLE_OK;
	HTTPHandler httpHandler;

	if (!httpHandler.Init(request, response, timeOut)) {
		UC_LOG(ERROR) << "sync handler init error:" << request.GetURL();
		return false;
	}

	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	std::string certPath = _engine->GetConfig().modulePath + "\\oncloud2CA.pem";
	if (httpHandler.Setup(clientService->GetProxyInfo(), clientService->GetCurlSettings(), certPath)) {
		UC_LOG(ERROR) << "sync setup error:";
		return false;
	}

	returnCode = curl_easy_perform(httpHandler.GetHandler());
	if (returnCode != CURLE_OK) {
		UC_LOG(ERROR) << "easy_perform error:" << curl_easy_strerror(returnCode);
		return false;
	}

	return true;
}

void NetworkManager::ThreadProc() {
	UC_LOG(INFO) << "NetworkManager::ThreadProc begin";

	CURLM	*multiHandler = NULL;

	multiHandler = curl_multi_init();
	if (multiHandler == NULL) {
		UC_LOG(ERROR) << "multi_init error";
		return;
	}

	// init http proxy query
	std::string domain = _engine->GetConfig().casServer;
	//HTTPProxy::GetInstance()->QueryProxyInfo(domain);

	while (!_stop) {

		boost::unique_lock<boost::recursive_mutex> lock(_mtxHandlers);
		if (_handlers.empty()) {
			BeforeHandlerSleep();

			_handlerCond.wait(lock);
		}

		if (_stop) {
			break;
		}
		//
		deque<HTTPHandler*> tempHandlers;
		std::list<HTTPHandler*>::const_iterator itor = _handlers.begin();
		while (itor != _handlers.end()) {
			if (_handlerNumber == 0){
				tempHandlers.push_back((HTTPHandler*)*itor);
				itor++;
			}
			else
			{
				if (_handlerNumber > tempHandlers.size()) {
					tempHandlers.push_back((HTTPHandler*)*itor);
					itor = _handlers.erase(itor);
				}else{
					break;
				}
			}
		}

		if(_handlerNumber == 0){
			_handlers.clear();
		}
		
		lock.unlock();

		DoHTTPRequest(multiHandler, tempHandlers);
		AfterHttpRequestDone();
	}

	curl_multi_cleanup(multiHandler);
	UC_LOG(INFO) << "NetworkManager::ThreadProc end";
}

void NetworkManager::DoHTTPRequest(CURLM *multiHandler, deque<HTTPHandler*> &handlers) {

	if (multiHandler == NULL) {
		return;
	}

	HTTPHandler *httpHandler = NULL;
	std::map<CURL*, HTTPHandler*> mapHandler;

	while (!handlers.empty()) {
		httpHandler = handlers.front();
		handlers.pop_front();

		if (httpHandler == NULL) {
			continue;
		}
		ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
		std::string certPath = _engine->GetConfig().modulePath + "\\oncloud2CA.pem";
		if (!httpHandler->Setup(clientService->GetProxyInfo(), clientService->GetCurlSettings(), certPath)) {
			delete httpHandler;
			continue;
		}

		mapHandler.insert(pair<CURL*, HTTPHandler*>(httpHandler->GetHandler(), httpHandler));

		httpHandler->StartTimer();

		curl_multi_add_handle(multiHandler, httpHandler->GetHandler());
	}

	int32_t running = 0;
	int32_t resultCode = CURLM_OK;

	resultCode = curl_multi_perform(multiHandler, &running);

	UC_LOG(INFO) << "multi_perform:" << resultCode;

	do {
		// 在这里增加判断 是否有 handler 需要结束
		_mtxCancelHandler.lock();
		std::list<int32_t>::iterator itor = _cancelHdandlers.begin();
		std::list<int32_t>::iterator removeItor;

		while (itor != _cancelHdandlers.end()) {
			int32_t requestId = *itor;
			if (RemoveHttpHandler(multiHandler, mapHandler, requestId)) {
				removeItor = itor;
				itor++;
				_cancelHdandlers.erase(removeItor);
			}
			else {
				itor++;
			}
		}
		_mtxCancelHandler.unlock();

		struct timeval timeOut;

		fd_set fdRead;
		fd_set fdWrite;
		fd_set fdExcep;
		int32_t maxFD = -1;

		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExcep);

		timeOut.tv_sec = 1;
		timeOut.tv_usec = 0;

		resultCode = curl_multi_fdset(multiHandler, &fdRead, &fdWrite, &fdExcep, &maxFD);
		if (maxFD == -1) {
#ifdef PLATFORM_WIN
			::Sleep(100);
#else
			select(0, NULL, NULL, NULL, &timeOut);
#endif
			resultCode = 1;
		}
		else {
			resultCode = select(maxFD+1, &fdRead, &fdWrite, &fdExcep, &timeOut);
		}

		switch (resultCode) {
		case -1: {
			running = 0;
			UC_LOG(INFO) << "select return error:";
			break;
		}
		case 0:
		default: {
			resultCode = curl_multi_perform(multiHandler, &running);
			break;
		}
		}

		// 判断有没有request已经完成
		int msgsLeft = 0;
		CURLMsg *urlMsg = NULL;

		do {
			urlMsg = curl_multi_info_read(multiHandler, &msgsLeft);

			if ( urlMsg != NULL && urlMsg->msg == CURLMSG_DONE) {
				// request结束,根据CURL 获取 HTTPHandler对象
				UC_LOG(INFO) << "http request result:" << urlMsg->data.result;
				httpHandler = mapHandler[urlMsg->easy_handle];

				if (httpHandler != NULL) {
					httpHandler->CheckProxy();
					httpHandler->EndTimer();
					int32_t code = 0;

					if (httpHandler->GetCallback() != NULL) {
						code = httpHandler->GetCallback()->OnRequestComplete(urlMsg->data.result, httpHandler->GetResponse());
					}

					if (httpHandler->GetRequestEvent() != NULL) {
						httpHandler->GetRequestEvent()->OnRequestFinished(httpHandler->GetId(), httpHandler->GetRequest(),
								urlMsg->data.result);
					}

					if (code == SESSION_EXPIRED ) {
						IClientService *service = (IClientService*)_engine->QueryService(UCClientService);
						if (service && httpHandler->GetRequest() 
							&& httpHandler->GetRequest()->GetSessionId() == service->GetSessionId())
						{
							triggerEventEx(IClientServiceEvent, *service, OnSessionExpired());
						}
					}

					curl_multi_remove_handle(multiHandler, httpHandler->GetHandler());
					mapHandler.erase(httpHandler->GetHandler());
					httpHandler->Cleanup();
					delete httpHandler;
					httpHandler = NULL;
				}
			}
		} while (urlMsg != NULL);
	} while (running && !_stop);

	// 从map里面清除未完成的http request
	map<CURL*, HTTPHandler*>::iterator itor = mapHandler.begin();
	while (itor != mapHandler.end()) {
		httpHandler = (HTTPHandler*)itor->second;
		if (httpHandler != NULL) {
			if (httpHandler->GetCallback() != NULL) {
				httpHandler->GetCallback()->OnRequestComplete(UC_FAILED, NULL);
			}

			if (httpHandler->GetRequestEvent() != NULL) {
				httpHandler->GetRequestEvent()->OnRequestFinished(httpHandler->GetId(), httpHandler->GetRequest(), UC_FAILED);
			}

			curl_multi_remove_handle(multiHandler, httpHandler->GetHandler());
			httpHandler->Cleanup();
			delete httpHandler;
			itor++;
		}
	}

	mapHandler.clear();

	UC_LOG(INFO) << "DoHTTPRequest finish";
	return;

}

int32_t NetworkManager::SendAsyncRequest(int32_t requestId, const HTTPRequest &request, HTTPRequestEvent *event, int32_t timeOut /*= HTTP_TIMEOUT*/, int32_t timeOut4Request /*= HTTP_RESPONSE_TIMEOUT*/) {

	if (_stop) {
		UC_LOG(ERROR) << "networkmgr is stop,can't send async request";
		return UC_FAILED;
	}

	if (event == NULL) {
		UC_LOG(ERROR) << "async request error, event is NULL";
		return UC_FAILED;
	}

	HTTPHandler *httpHandler = new HTTPHandler();
	
	if (!httpHandler->Init(requestId, request, event,timeOut,timeOut4Request)) {
		UC_LOG(ERROR) << "async init error" << request.GetURL();
		delete httpHandler;
		return UC_FAILED;
	}

	// 获取session_id并且保存在 request里
	IClientService *service = (IClientService*)_engine->QueryService(UCClientService);
	httpHandler->GetRequest()->SetSessionId(service->GetSessionId());

	_mtxHandlers.lock();
	_handlers.push_back(httpHandler);
	_handlerCond.notify_one();
	_mtxHandlers.unlock();
	return UC_PENDING;
}

int32_t NetworkManager::SendAsyncRequest(int32_t requestId, const HTTPRequest &request, RequestCallback *callback, int32_t timeOut /*= HTTP_TIMEOUT*/, int32_t timeOut4Request /*= HTTP_RESPONSE_TIMEOUT*/) {

	if (_stop) {
		UC_LOG(ERROR) << "networkmgr is stop,can't send async request";
		return UC_FAILED;
	}

	if (callback == NULL) {
		UC_LOG(ERROR) << "async request error:request or callback is null";
		return UC_FAILED;
	}

	HTTPHandler *httpHandler = new HTTPHandler();

	if (!httpHandler->Init(requestId, request, callback, timeOut,timeOut4Request)) {
		UC_LOG(ERROR) << "async init error" << request.GetURL();
		delete httpHandler;
		return UC_FAILED;
	}

	// 获取session_id并且保存在 request里
	IClientService *service = (IClientService*)_engine->QueryService(UCClientService);
	httpHandler->GetRequest()->SetSessionId(service->GetSessionId());

	_mtxHandlers.lock();
	_handlers.push_back(httpHandler);
	_handlerCond.notify_one();
	_mtxHandlers.unlock();
	return UC_PENDING;
}

int32_t NetworkManager::CancelAysncRequest(int32_t requestId) {
	// 如果这个 request 还没有执行则可以从list中直接删除
	_mtxHandlers.lock();
	std::list<HTTPHandler*>::iterator itor = _handlers.begin();
	while (itor != _handlers.end()) {
		HTTPHandler *handler = (HTTPHandler*)*itor;

		if (handler != NULL && handler->GetId() == requestId) {
			delete handler;
			_handlers.erase(itor);
			_mtxHandlers.unlock();
			return UC_SUCCEEDED;
		}

		itor++;
	}
	_mtxHandlers.unlock();

	// 将requestid加入到待删除列表中
	_mtxCancelHandler.lock();
	_cancelHdandlers.push_back(requestId);
	_mtxCancelHandler.unlock();
	return UC_PENDING;
}

bool NetworkManager::RemoveHttpHandler(CURLM *multiHandler, std::map<CURL*, HTTPHandler*> &handlers, int32_t requestId) {
	if (multiHandler == NULL) {
		return false;
	}

	std::map<CURL*, HTTPHandler*>::iterator itor = handlers.begin();
	std::map<CURL*, HTTPHandler*>::iterator removeItor;

	while (itor != handlers.end()) {
		HTTPHandler *handler = (HTTPHandler*)(itor->second);
		if (handler != NULL && handler->GetId() == requestId) {
			curl_multi_remove_handle(multiHandler, handler->GetHandler());

			if (handler->GetCallback() != NULL) {
				handler->GetCallback()->OnRequestComplete(UC_CANCEL, handler->GetResponse());
			}

			if (handler->GetRequestEvent() != NULL) {
				handler->GetRequestEvent()->OnRequestFinished(handler->GetId(), handler->GetRequest(), UC_CANCEL);
			}

			removeItor = itor;
			itor++;
			handlers.erase(removeItor);
			handler->Cleanup();
			delete handler;
			handler = NULL;
			return true;
		}
		else {
			itor++;
		}
	}

	return false;
}

} /* namespace uc */
