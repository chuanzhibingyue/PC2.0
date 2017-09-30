/*
 * UCASConnection.cpp
 *
 *  Created on: 2015年7月4日
 *      Author: xuejian
 */

#define BOOST_SPIRIT_THREADSAFE

#include "UCASConnection.h"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include "LogManager.h"
#include "common/StringConvert.h"
#include "UCErrorCode.h"
#include "UCMessage.h"
#include "Messageparser.h"
#include "common/ZipWrapper.h"
#include "base/Timer.h"
#include "common/CallOnMainThread.h"
//#include "IClientService.h"
#include "client_service/ClientService.h"

namespace uc {

#define THRIFTDATA_LEN	655350

UCASConnection::UCASConnection() {
	_ucEngine = NULL;
	_thread = NULL;
	_closed = false;
	_thriftData = NULL;
	_thriftDataLen = THRIFTDATA_LEN;
	_timerId = -1;
	_isFirstNoopData = false;
}

UCASConnection::~UCASConnection() {
	if (_thriftData != NULL) {
		delete []_thriftData;
		_thriftData = NULL;
	}
}

bool UCASConnection::Init(IUCEngine *engine) {
	_ucEngine = engine;
	_thriftData = new unsigned char[THRIFTDATA_LEN];
	memset(_thriftData, 0, THRIFTDATA_LEN);

	return true;
}

bool UCASConnection::UnInit() {
	return Disconnect();
}

bool UCASConnection::Connect(const std::string &serverURL) {
	if (_thread != NULL) {
		return false;
	}

	UC_LOG(INFO) << "UCASConnection::Connect URL:" << serverURL;

	_closed = false;
	_isFirstNoopData = false;
	_thread = new boost::thread(boost::bind(&UCASConnection::ThreadProc, this, serverURL));
	return true;
}

bool UCASConnection::Disconnect() {
	if (_thread == NULL) {
		return true;
	}

	UC_LOG(INFO) << "UCASConnection::Disconnect";

	_closed = true;
	_thread->join();
	delete _thread;
	_thread = NULL;

	return true;
}

void UCASConnection::ThreadProc(UCASConnection *connection, string &serverURL) {
	HTTPRequest request;
	HTTPHandler handler;

	request.Init(serverURL, HTTP_METHOD_GET);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.AddHeaderInfo("Connection", "keep-alive");
	request.AddHeaderInfo("Cache-Control", "no-cache");
	request.AddHeaderInfo("Pragma", "no-cache");

	// init http proxy query for ucas
	std::string domain = connection->_ucEngine->GetConfig().casServer;
	ClientService* clientService = (ClientService*)connection->_ucEngine->QueryService(UCClientService);
	std::string certPath = connection->_ucEngine->GetConfig().modulePath + "\\oncloud2CA.pem";
	if (!handler.Init(0, request, connection, HTTP_TIMEOUT, HTTP_RESPONSE_INFINITY) || !handler.Setup(clientService->GetProxyInfo(), clientService->GetCurlSettings(), certPath)) {
		UC_LOG(INFO) << "UCASConnection setup error:" << serverURL;
		return;
	}

	CURLM *multiHandler = NULL;

	multiHandler = curl_multi_init();
	if (multiHandler == NULL) {
		UC_LOG(ERROR) << "UCASConnection multi_init error";
		return;
	}

	curl_multi_add_handle(multiHandler, handler.GetHandler());

	int32_t running = 0;
	int32_t resultCode = CURLM_OK;

	UC_LOG(INFO) << "multi_perform:" << resultCode;

	do {
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
	} while (running && !connection->_closed);

	handler.Cleanup();
	curl_multi_cleanup(multiHandler);

	int32_t errorCode = UCAS_CLOSED_ABNORMLLY;
	if (connection->_closed) {
		UC_LOG(INFO) << "UCASConnection::ThreadProc OnUCASDisconnected, ucas connection closed";
		errorCode = UC_SUCCEEDED;
	} else {
		UC_LOG(INFO) << "UCASConnection::ThreadProc OnUCASDisconnected, ucas connection Non-normal closed: " << resultCode;
	}

	connection->CancelNoopTimer();
	triggerEventEx(IUCEngineEvent, *(connection->_ucEngine), OnUCASDisconnected(errorCode));
}

void UCASConnection::OnHeaderReceived(int32_t requestId, HTTPRequest *request, int32_t statusCode, int32_t authType, const HeaderMap &header) {
	UC_LOG(INFO) << "UCASConnection::OnHeaderReceived: " << statusCode;
	int32_t errorCode = UC_FAILED;

	if (statusCode == 200) {
		return;
	}

	if (statusCode == 407)
	{
		if (authType == 1 || authType == 2)
		{
			ClientService* clientService = (ClientService*)_ucEngine->QueryService(UCClientService);
			if (!clientService->GetProxyInfo().GetProxyUrl().empty())
			{
				return;
			}
		}
		
		errorCode = statusCode;	
	}

	UC_LOG(INFO) << "UCASConnection::OnHeaderReceived OnUCASConnected";

	SetNoopTimer();
	triggerEventEx(IUCEngineEvent, *_ucEngine, OnUCASConnected(errorCode));
}

void UCASConnection::OnDataReceived(int32_t requestId, HTTPRequest *request, void *data, int32_t length) {
	// json解析
	string	jsonData((char*)data, length);

	UC_LOG(INFO) << "UCASConnection::OnDataReceived: " << length << ", content: " << jsonData;

	_content.append(jsonData);

	string::size_type position = _content.find("\n");
	while (position != _content.npos) {
		std::string chunkedData = _content.substr(0, position);
		ParseUCASData(chunkedData);
		_content = _content.substr(position + 1, _content.length() - position - 1);
		position = _content.find("\n");
	}
}

void UCASConnection::OnRequestFinished(int32_t requestId, HTTPRequest *request, int32_t errorCode) {
	UC_LOG(INFO) << "UCASConnection::OnRequestFinished OnUCASDisconnected, error code: " << errorCode;

	CancelNoopTimer();
	triggerEventEx(IUCEngineEvent, *_ucEngine, OnUCASDisconnected(errorCode));
}

void UCASConnection::ParseUCASData(std::string &data) {
	if (data.empty()) {
		return;
	}

	SetNoopTimer();

	boost::property_tree::wptree ptRoot;
	std::wstring type, cname;
	std::string content;
	int32_t seq = 0;

	try {
		wstringstream jsonStream(StringConvert::FromUTF8Str(data));
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);

		type = ptRoot.get<wstring>(L"type");
		cname = ptRoot.get<wstring>(L"cname");
		seq = ptRoot.get<int32_t>(L"seq");
		content = StringConvert::ToUTF8Str(ptRoot.get<wstring>(L"content", L""));
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "UCASConnection::ParseUCASData, critical exception for json data: " << e.what();
		return;
	}

	if (type == L"data") {
		UCMsg *msg = NULL;
		char *zipData = NULL;
		int32_t zipDataLen = 0;
		std::string strZipData;

		// base64 decode
		if (!StringConvert::Base64Decode(content.c_str(), content.length(), strZipData)) {
			UC_LOG(ERROR) << "Base64Decode error:" << content;
			return;
		}

		zipData = (char*)strZipData.c_str();
		zipDataLen = strZipData.length();

		//TODO:增加预申请内存size方案有待替换;
		_thriftDataLen = THRIFTDATA_LEN;
		memset(_thriftData, 0, _thriftDataLen);

		// gzip decompress
		int result = ZipWrapper::Decompress((unsigned char*)zipData, zipDataLen, _thriftData, &_thriftDataLen);
		if (result != 0) {
			UC_LOG(ERROR) << "Gzip decompress error:" << result << " thriftData:" << _thriftDataLen;
			return;
		}

		// thrift decode
		result = MsgParser::DecodeMessage((char*)_thriftData, _thriftDataLen, msg);
		if (result != 0) {
			UC_LOG(ERROR) << "MsgParser:DecodeMessage error:" << result;
			return;
		}

		string body; // base64编码后的body数据
		if (!StringConvert::Base64Encode((const char*)(_thriftData + (_thriftDataLen - msg->GetBodyLen())), msg->GetBodyLen(), body)) {
			UC_LOG(ERROR) << "Base64Encode msg body error";
			return;
		}

		triggerEventEx(IUCEngineEvent, *_ucEngine, OnMessageReceived(UCAS, *msg, body, STATE_SENDSUCCESS));
		delete msg;

		return; //don't log type at the end of this function. :)
	}
	else if (type == L"noop") {
		if (!_isFirstNoopData){
			_isFirstNoopData = true;
			triggerEventEx(IUCEngineEvent, *_ucEngine, OnUCASConnected(UC_SUCCEEDED));
		}
	}
	else if (type == L"error" || type == L"401" || type == L"429" || type == L"430" || type == L"431") {
		/*
		  401: expired session
		  429: ucas connection number is too many to connect ucas
		  430: cache server is busy and can't connect
		  431: connect cache server and get error
		*/
		if (type == L"401") {
			CallOnMainThread(this, &UCASConnection::triggerEventInMainThread, SESSION_EXPIRED);
		}
		else if (type == L"error" || type == L"430" || type == L"431") {
			CallOnMainThread(this, &UCASConnection::triggerEventInMainThread, UCAS_CLOSED_ABNORMLLY);
		}
		else if (type == L"429") {
			CallOnMainThread(this, &UCASConnection::triggerEventInMainThread, UCAS_TOOMANY_CONNECTION);
		}
	}
	else if (type == L"close") {
	}

	UC_LOG(INFO) << "UCASConnection::ParseUCASData, type:" << type;
}

void UCASConnection::OnNoDataReceivedWithLongTime(bool in_main_thread) {
	if (!in_main_thread) {
		CallOnMainThread(this, &UCASConnection::OnNoDataReceivedWithLongTime, true);
	}
	else {
		UC_LOG(ERROR) << "UCASConnection::OnNoDataReceivedWithLongTime. "
			          << "It can't receive data from ucas for a long time, we suspect the connection is invalid, so kickoff the reconnecting process.";
		Disconnect();
		triggerEventEx(IUCEngineEvent, *_ucEngine, OnUCASDisconnected(UCAS_CLOSED_NODATA));
	}
}

void UCASConnection::SetNoopTimer() {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	Timer::GetInstance().Cancel(_timerId);

	// 2 minutes
	static int noop_interval_microsecond = 2 * 60 * 1000; 
	//one-off timer
	Timer::GetInstance().Schedule(MakeCallback(this, &UCASConnection::OnNoDataReceivedWithLongTime, false), noop_interval_microsecond, &_timerId);
}

void UCASConnection::CancelNoopTimer() {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	Timer::GetInstance().Cancel(_timerId);
}

void UCASConnection::triggerEventInMainThread(int errorCode) {
	if (errorCode == SESSION_EXPIRED) {
		IClientService *service = (IClientService*)_ucEngine->QueryService(UCClientService);
		if (service == NULL)
			return;

		Disconnect();
		triggerEventEx(IClientServiceEvent, *service, OnSessionExpired());
	}
	else {
		triggerEventEx(IUCEngineEvent, *_ucEngine, OnUCASDisconnected(errorCode));
	}
}

} /* namespace uc */
