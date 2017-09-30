/*
 * UCEngine.cpp
 *
 *  Created on: Feb 11, 2015
 *      Author: ethan
 */

#include "UCEngine.h"
#include <iostream>
#include <sstream>
#include "UCSDK.h"
#include "client_service/ClientService.h"
#include "contact_service/ContactService.h"
#include "filetransfer_service/FileTransferService.h"
#include "message_service/IMService.h"
#include "message_service/GroupService.h"
#include "calendar_service/CalendarService.h"
#include "search_service/SearchService.h"
#include "store_service/StoreService.h"
#include "openapi_service/OpenApiService.h"
#include "microblog_service/MicroblogService.h"
#include "external_service/ExternalService.h"
#include "bbs_service/BBSService.h"
#include "gtest/gtest.h"
#include "LogManager.h"
#include "common/StringConvert.h"
#include "DBManager.h"
#include "common/ThreadMessage.h"
#include "CommonDef.h"
#include "utils/TaskExecutor.h"

#ifdef SHARE_SERVICE
    #include "sharing_service/SharingService.h"
#endif

#ifdef AUDIO_SERVICE
    #include "audio_service/AudioService.h"
#endif

#ifdef VIDEO_SERVICE
    #include "video_service/VideoService.h"
#endif

#ifdef P2P_SERVICE
#include "p2p_service/p2p_service.h"
#endif // P2P_SERVICE

#ifdef PBX_SERVICE
#include "pbx_service/PbxService.h"
#endif // PBX_SERVICE

namespace uc {

using namespace std;

StoreService theStore;

UCEngine::UCEngine(UCEngineConfig &config) : _UCCNetwork(UCC_REQUEST_THREADS) {
	// TODO Auto-generated constructor stub
	_config = config;
}

UCEngine::~UCEngine() {
}



bool UCEngine::Init() {
	// init log module
	// covert logPath to ascii
	std::wstring logpath = StringConvert::FromUTF8Str(_config.logConfig.logPath);
	std::string log = StringConvert::Unicode2Ascii(logpath);
	LogManager::GetInstance().Init(log, _config.logConfig.programName, _config.logConfig.logFileName, !_config.logConfig.logToFile);

	UC_LOG(INFO) << "==========================start bee.exe add version:" << _config.version << "========================== ";

	// init common database
	if (_config.appPath.empty()) {
		UC_LOG(ERROR) << "app path is empty";
		return false;
	}

	string commonDBPath = _config.appPath + "/commondb.sqlite";
	int result = DBManager::GetInstance().InitCommonDB(commonDBPath);
	if (result != DBError::DB_SUCCESS) {
		UC_LOG(ERROR) << "init common db error:" << result;
		return false;
	}

	//
	AttachMainThreadMessageProc(new DefaultMessageProc());

	// 暂时不需要
	TaskExecutor::GetInstance().Init();

	// 初始化 ucc 网络
	if (!_UCCNetwork.Init(this) || !_UCCNetwork.Start()) {
		UC_LOG(ERROR) << "ucc network start error";
		return false;
	}

	// 初始化 ucas 网络
	_connection.Init(this);

	// 创建 services
	InitServices();

	triggerEvent(IUCEngineEvent, OnInit());

	return true;
}

void UCEngine::UnInit() {

	triggerEvent(IUCEngineEvent, OnUnInit());

	_UCCNetwork.Stop();

	_connection.UnInit();

	DetachMainThreadMessageProc();

	// 暂时不需要
	TaskExecutor::GetInstance().Shutdown();
	theStore.Clear();
	theStore.GetGroupListStore()->Clear();

	for (SERVICEMAP::iterator iter = ServiceMap.begin(); iter != ServiceMap.end(); iter++) {
		delete iter->second;
	}
	ServiceMap.clear();

	UC_LOG(INFO) << "==========================exit bee.exe add version:" << _config.version << "========================== ";
}

UCEngineConfig& UCEngine::GetConfig() {
	return _config;
}

IUCService* UCEngine::QueryService(UCServiceType type) {
	IUCService *Service = NULL;

	map<UCServiceType, IUCService*>::iterator iter;

	iter = ServiceMap.find(type);
	if (iter != ServiceMap.end()) {
		Service = iter->second;
	} else {
		Service = CreateService(type);
	}

	return Service;
}

/**
 *
 */
void UCEngine::InitServices() {
	for (uint16_t i = UCClientService; i <= UCPbxService; ++i) {
		CreateService((UCServiceType) i);
	}
}

/**
 *
 * @param type
 * @return
 */
IUCService* UCEngine::CreateService(UCServiceType type) {
	IUCService *Service = NULL;

	switch (type) {
		case UCClientService: {
			Service = new ClientService(this);
			break;
		}
		case UCContactService: {
			Service = new ContactService(this);
			break;
		}
		case UCFileTransferService: {
			Service = new FileTransferService(this);
			break;
		}
		case UCCalendarService: {
			Service = new CalendarService(this);
			break;
		}
		case UCIMService: {
			Service = new IMService(this);
			break;
		}
		case UCGroupService: {
			Service = new GroupService(this);
			break;
		}
#ifdef AUDIO_SERVICE
		case UCAudioService: {
			Service = new AudioService(this);
			break;
		}
#endif
#ifdef VIDEO_SERVICE
		case UCVideoService: {
			Service = new VideoService(this);
			break;
		}
#endif
#ifdef SHARE_SERVICE
		case UCSharingService: {
			Service = new SharingService(this);
			break;
		}
#endif
		case UCSearchService: {
			Service = new SearchService(this);
			break;
		}
#ifdef P2P_SERVICE
		case UCP2PService: {
			Service = new p2p_service(this);
		} break;
#endif // P2P_SERVICE
		case UCOpenApiService: {
			Service = new OpenApiService(this);
			break;
		}
		case UCMicroblogService: {
			Service = new MicroblogService(this);
			break;
		}
		case UCExternalService:{
			Service = new ExternalService(this);
			break;
		}
		case UCBBSService: {
			Service = new CBBSService(this);
			break;
		}
#ifdef PBX_SERVICE
		case UCPbxService: {
			Service = new PbxService(this);
		} break;
#endif // PBX_SERVICE
		default:
			break;
	}

	if (Service != NULL) {
		ServiceMap.insert(pair<UCServiceType, IUCService*>(type, Service));
	}

	return Service;
}

string UCEngine::GetRequestURL(const string &serverURL, const string &methodURL) {
	stringstream sStream;

	sStream << serverURL << methodURL;
	return sStream.str();
}

int32_t UCEngine::SendAsyncRequest(int32_t requestId, const HTTPRequest &request, HTTPRequestEvent *event, int32_t timeOut /* = HTTP_TIMEOUT*/,int32_t timeOut4Request /*= HTTP_RESPONSE_TIMEOUT*/) {
	return _UCCNetwork.SendAsyncRequest(requestId, request, event, timeOut, timeOut4Request);
}

int32_t UCEngine::SendAsyncRequest(const HTTPRequest &request, RequestCallback *callback, int32_t timeOut /*= HTTP_TIMEOUT*/, int32_t timeOut4Request /*= HTTP_RESPONSE_TIMEOUT*/) {
	return _UCCNetwork.SendAsyncRequest(0, request, callback, timeOut, timeOut4Request);
}

bool UCEngine::SendSyncRequest(HTTPRequest &request, HTTPResponse &response, int32_t timeOut /* = HTTP_TIMEOUT*/) {
	return _UCCNetwork.SendSyncRequest(request, response, timeOut);
}

void UCEngine::RegisterQueueNetMgrListener(QueuedNetWorkManagerListener* listener) {
	_UCCNetwork.registerListener(listener);
}

void UCEngine::SendQueuedRequest(ConversationType type, int64_t userId, int32_t msgId, const std::string &body) {
	_UCCNetwork.SendQueuedRequest(type, userId, msgId, body);
}

void UCEngine::ConnectUCAS(const string &url) {
	_connection.Connect(url);
}

void UCEngine::DisconnectUCAS() {
	_connection.Disconnect();
}

void UCEngine::ConnectUCC()
{
	_UCCNetwork.Start();
}

void UCEngine::DisconnectUCC()
{
	_UCCNetwork.Stop();
}

/**
 * TODO: make this method thread safe
 */
static IUCEngine* s_engine = NULL;
IUCEngine* CreateUCEngine(UCEngineConfig &config) {
	Timer::GetInstance().Start();

	if (s_engine == NULL) {
		s_engine = new UCEngine(config);
		s_engine->Init();
	}

	return s_engine;
}

void DestroyUCEngine() {
	Timer::GetInstance().Shutdown();

	if (s_engine != NULL) {
		s_engine->UnInit();
		delete s_engine;
		s_engine = NULL;
	}
}
} /* namespace uc */
