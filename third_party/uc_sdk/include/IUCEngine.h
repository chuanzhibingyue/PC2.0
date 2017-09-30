/*
 * IUCEngine.h
 *
 *  Created on: Feb 11, 2015
 *      Author: ethan
 */

#ifndef IUCSERVICE_H_
#define IUCSERVICE_H_

#include <iostream>
#include "UCSDK.h"
#include "base/HTTPRequest.h"
#include "base/HTTPResponse.h"
#include "base/HTTPRequestEvent.h"
#include "base/HTTPRequestCallback.h"
#include "UCMessage.h"
#include "LogManager.h"
#include "common//event.h"
#include "common/StringConvert.h"
#include "MessageInfo.h"

namespace uc {

interface UC_API IUCService;

interface UC_API IUCEngineEvent {
	IUCEngineEvent() {};
	virtual ~IUCEngineEvent() {};
	virtual void OnEngineStatusChanged() {
		//UC_LOG(INFO) << "IUCEngineEvent::OnEngineStatusChanged";
	};

	/**
	 * 接收到ucas消息时的回调函数
	 * @param channel 消息来源
	 * @param msg 消息对象
	 * @param body 对body->thritft->base64后的数据
	 */
	virtual int32_t OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state) {
// 		UC_LOG(INFO) << "IUCEngineEvent::OnMessageReceived; "<< " channel: " << channel
// 				<< " body: " << body;
		return 0;
	};

	/**
	 * ucas连接的事件
	 * @param errorCode 错误码
	 */
	virtual void OnUCASConnected(int32_t errorCode) {
		//UC_LOG(INFO) << "IUCEngineEvent::OnUCASConnected: " << errorCode;
	};

	/*
	 * ucas断开连接的回调事件
	 * @param errorCode 错误码
	 */
	virtual void OnUCASDisconnected(int32_t errorCode) {
		//UC_LOG(INFO) << "IUCEngineEvent::OnUCASDisconnected: " << errorCode;
	}

	/*
	 * 登陆成功的回调事件
	 */
	virtual void OnLoginSucceeded() {
		//UC_LOG(INFO) << "IUCEngineEvent::OnLoginSucceeded";
	}

	/**
	 * 登出成功的回调事件
	 */
	virtual void OnLogoutSucceeded() {
		//UC_LOG(INFO) << "IUCEngineEvent::OnLogoutSucceeded";
	}

	/*
	 * UCEngine初始化事件
	 */
	virtual void OnInit() {
		UC_LOG(INFO) << "IUCEngineEvent::OnInit: ";
	}

	/*
	 * UCEngine UnInit事件
	 */
	virtual void OnUnInit() {
		UC_LOG(INFO) << "IUCEngineEvent::OnUnInit: ";
	}
};

interface UC_API UCEngineConfig {
	UCEngineConfig() {
		logConfig.logPath = "";
		logConfig.programName = "SDK";
		logConfig.logFileName = "";
		logConfig.logToFile = false;

		appPath = "";
		version = "";
		modulePath = "";
	};
	virtual ~UCEngineConfig() {};
public:
	struct LogConfig {
		string logPath;
		string programName;
		string logFileName;
		bool logToFile;
	} logConfig;

	string casServer;
	string appPath;
	string documentPath;
	string version;
	string modulePath; // 当前目录
};

interface UC_API IUCEngine : public Event<IUCEngineEvent> {
	virtual ~IUCEngine(){};
	virtual bool Init() = 0;
	virtual void UnInit() = 0;
	virtual IUCService* QueryService(UCServiceType type) = 0;
	virtual UCEngineConfig& GetConfig() = 0;

	virtual int32_t SendAsyncRequest(int32_t requestId, const HTTPRequest &request, HTTPRequestEvent *event, int32_t timeOut = HTTP_TIMEOUT, int32_t timeOut4Request = HTTP_RESPONSE_TIMEOUT) = 0;

	virtual int32_t SendAsyncRequest(const HTTPRequest &request, RequestCallback *callback, int32_t timeOut = HTTP_TIMEOUT, int32_t timeOut4Request = HTTP_RESPONSE_TIMEOUT) = 0;

	virtual bool SendSyncRequest(HTTPRequest &request, HTTPResponse &response, int32_t timeOut = HTTP_TIMEOUT) = 0;
};

interface UC_API IUCService {
	virtual ~IUCService(){};

	IUCEngine* GetEngine() {
		return _engine;
	}
protected:
	IUCEngine* _engine;
};

/**
 * singleton method
 */
UC_API IUCEngine* CreateUCEngine(UCEngineConfig &config);
UC_API void DestroyUCEngine();

}

#endif /* IUCENGINE_H_ */
