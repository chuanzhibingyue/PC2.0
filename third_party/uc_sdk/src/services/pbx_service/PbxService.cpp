#include "PbxService.h"
#include "UCEngine.h"
#include "DBManager.h"
#include "UCErrorCode.h"
#include "boost/regex.hpp"
#include "utils/UCTime.h"

#include "client_service/ClientService.h"

#ifdef PBX_SERVICE
#pragma comment(lib, "EasiioLibrary.lib")
#ifdef _DEBUG
#define toConsole 0
#else
#define toConsole 0
#endif // _DEBUG

namespace uc {
	PbxService* PbxService::_lpPbxSrv = NULL;

	PbxService::PbxService(IUCEngine *engine) {

		_lpPbxSrv = this;
		_engine = engine;
		_engine->registerListener(this);

		_isInit = false;
		_reLogin = false;
		_isConnected = false;
		_loginRet = LR_WAITING;

		_timerId = -1;

		_paramsCount = 0;
		_lpParams = NULL;
		_lpCBInterface = NULL;

		_login = false;

		InitConsoleWindow();

		std::string dirU = _engine->GetConfig().logConfig.logPath;
		std::wstring dirW = StringConvert::FromUTF8Str(dirU);
		_logDirA = StringConvert::Unicode2Ascii(dirW);

		int index = -1;
		index = _logDirA.find_last_of("log");
		int len = _logDirA.length();
		if (std::string::npos != index && index != (_logDirA.length() - 1)) {
			UC_LOG(INFO) << "err logDir U=" << dirU << ", A=" << _logDirA;
			_logDirA = _logDirA.substr(0, index + 1);
		}
		else {
			UC_LOG(INFO) << "logDir U=" << dirU << ", A=" << _logDirA;
		}
		EasiioSetLogDir(_logDirA.c_str());
	}

	PbxService::~PbxService() {

		DestroyPbx();

		_engine->removeListener(this);
		_engine = NULL;
	}
	//////////////////////////////////////////////////////////////////////////

	int32_t PbxService::CheckLogin(int &nRet) {
		UC_LOG(INFO) << "CheckLogin loginRet=" << _loginRet;
		cout << "CheckLogin loginRet=" << _loginRet << endl;

		nRet = _loginRet;

		return 0;
	}
	// VoIP呼叫
	// ucClient.PbxService.makeCall('15840436354', 10012, '15840436354', 2, '1111')
	int32_t PbxService::MakeCall(const std::string &userName, int userID, const std::string &number,
		int callType, const std::string &postValue, __int64 &timeStamp, int32_t &taskID) {
		UC_LOG(INFO) << "MakeCall userName=" << StringConvert::FromUTF8Str(userName).c_str()
			<< ", number=" << number.c_str() << ", callType=" << callType;
		int nRet = ERROR_INPUT_PARAMETER;
		do
		{
			if (number.length() <= 0 || number.length() > 20) {
				break;
			}

			if (callType > VIDEO_CALL) {
				nRet = EC_PARAMETER2;
				break;
			}

			int nTaskCount = _tasks.GetTasksCount();
			if (nTaskCount > 0){
				UC_LOG(INFO) << "Tasks Count=" << nTaskCount;
				nRet = EC_CALLING;
				break;
			}

			nRet = EasiioMakeCall(number.c_str(), callType, postValue.c_str());
			UC_LOG(INFO) << "EasiioMakeCall ret=" << nRet;

			if (REQUEST_SUCCESS != nRet) {
				// SaveCallID(number, -1);
				// shared_ptr<PbxTask> pTask = _tasks.RemoveTask(taskID);
				// nRet = DBManager::GetInstance().InsertPbxCallDetails(*pTask);

				timeStamp = 0;
				taskID = -2;
				nRet = EC_MAKECALL;
				break;
			}

			taskID = CreateCallInfo(CALL_STATE_INIT, callType, userName, userID,
				number, postValue.c_str());

			GetCallKey(number, timeStamp, taskID);

		} while (false);
		UC_LOG(INFO) << "MakeCall taskID=" << taskID << ", ret=" << nRet;

		cout << "VoIP呼叫 userName=" << StringConvert::FromUTF8Str(userName).c_str()
			<< ", number=" << number.c_str() << ", callType=" << callType
			<< ", taskID = " << taskID << ", ret = " << endl;

		return nRet;
	}

	// 来电应答
	int32_t PbxService::AnswerCall(int taskID, int callType) {
		int nRet = ERROR_INPUT_PARAMETER;
		int callID = -1;
		do
		{
			shared_ptr<PbxTask> pTask = _tasks.GetTaskByID(taskID);
			if (NULL == pTask) {
				break;
			}
			callID = pTask->GetCallid();
			if (callID < 0) {
				nRet = EC_MAKING;
				break;
			}

			if (!pTask->_isIncoming) {
				nRet = EC_OPERATE;
				break;
			}

			if (CALL_STATE_INCOMING != pTask->GetStateEx()) {
				nRet = EC_STATE;
				break;
			}

			nRet = EasiioAnswerCall(callID, callType);
		} while (false);
		UC_LOG(INFO) << "AnswerCall taskID=" << taskID << ", callID=" << callID << ", ret=" << nRet;
		cout << "来电应答 taskID=" << taskID << ", callID=" << callID << ", ret=" << nRet << endl;

		return nRet;
	}

	// 拒接来电
	int32_t PbxService::RejectCall(int taskID) {
		int nRet = ERROR_INPUT_PARAMETER;
		int callID = -1;
		do
		{
			shared_ptr<PbxTask> pTask = _tasks.GetTaskByID(taskID);
			if (NULL == pTask) {
				break;
			}
			callID = pTask->GetCallid();
			if (callID < 0) {
				nRet = EC_MAKING;
				break;
			}

			if (!pTask->_isIncoming) {
				nRet = EC_OPERATE;
				break;
			}

			if (CALL_STATE_INCOMING != pTask->GetStateEx()) {
				nRet = EC_STATE;
				break;
			}

			pTask->SetState(CALL_STATE_REJECT, pTask->_isIncoming);

			nRet = EasiioRejectCall(callID);
		} while (false);

		UC_LOG(INFO) << "RejectCall taskID=" << taskID << ", callID=" << callID << ", ret=" << nRet;
		cout << "拒接来电 taskID=" << taskID << ", callID=" << callID << ", ret=" << nRet << endl;

		return nRet;
	}

	// 挂断通话
	// ucClient.PbxService.hangupCall()
	int32_t PbxService::HangupCall(int taskID) {
		int nRet = ERROR_INPUT_PARAMETER;
		int callID = -1;
		do
		{
			shared_ptr<PbxTask> pTask = _tasks.GetTaskByID(taskID);
			if (NULL == pTask) {
				break;
			}

			callID = pTask->GetCallid();
			if (callID < 0) {
				nRet = EC_MAKING;
				break;
			}

			if (CALL_STATE_INCOMING == pTask->GetStateEx()) {
				nRet = EC_OPERATE;
				break;
			}

			pTask->SetState(CALL_STATE_HANGUP, pTask->_isIncoming);

			nRet = EasiioHangupCall(callID);
		} while (false);

		UC_LOG(INFO) << "HangupCall taskID=" << taskID << ", callID=" << callID << ", ret=" << nRet;
		cout << "挂断通话 taskID=" << taskID << ", callID=" << callID << ", ret=" << nRet << endl;

		return nRet;
	}

	// 通话保持
	int32_t PbxService::HoldCall(int taskID) {
		UC_LOG(INFO) << "HoldCall taskID=" << taskID;
		return UpCallHold(taskID, true);
	}
	// 解除通话保持
	int32_t PbxService::UnHoldCall(int taskID) {
		UC_LOG(INFO) << "UnHoldCall taskID=" << taskID;
		return UpCallHold(taskID, false);
	}
	// 通话静音
	int32_t PbxService::MuteCall(int taskID) {
		UC_LOG(INFO) << "MuteCall taskID=" << taskID;
		return UpCallMute(taskID, true);
	}
	// 解除通话静音
	int32_t PbxService::UnMuteCall(int taskID) {
		UC_LOG(INFO) << "UnMuteCall taskID=" << taskID;
		return UpCallMute(taskID, false);
	}
	//////////////////////////////////////////////////////////////////////////
	// 调整通话麦克风音量
	int32_t PbxService::AdjustMicLevel(int taskID, int level) {
		int nRet = ERROR_INPUT_PARAMETER;
		int callID = -1;
		do
		{
			shared_ptr<PbxTask> pTask = _tasks.GetTaskByID(taskID);
			if (NULL == pTask) {
				break;
			}

			callID = pTask->GetCallid();
			if (callID < 0) {
				nRet = EC_MAKING;
				break;
			}

			nRet = EasiioAdjustMicLevel(callID, level);
		} while (false);

		return nRet;
	}

	// 刷新麦克风
	int32_t PbxService::RefreshMic(int &micState){
		int nRet = 0;

		micState = _MicRet(EasiioRefreshMic());
		UC_LOG(INFO) << "RefreshMic micState=" << micState;
		cout << "RefreshMic micState=" << micState << endl;

		return nRet;
	}

	// 发送DTMF
	int32_t PbxService::SendDTMF(int taskID, const std::string &dtmf) {
		int nRet = ERROR_INPUT_PARAMETER;
		int callID = -1;
		do
		{
			shared_ptr<PbxTask> pTask = _tasks.GetTaskByID(taskID);
			if (NULL == pTask) {
				break;
			}
			callID = pTask->GetCallid();
			if (callID < 0) {
				nRet = EC_MAKING;
				break;
			}

			if (dtmf.length() <= 0) {
				nRet = EC_PARAMETER2;
				break;
			}

			nRet = EasiioSendDTMF(callID, dtmf.c_str());
		} while (false);

		return nRet;
	}
	//////////////////////////////////////////////////////////////////////////
	// 获取当前PBX号码
	int32_t PbxService::GetCurrentPBXAccount(std::string &pbxAccount) {
		int nRet = 0;
		char account[11] = { 0 };
		nRet = EasiioGetCurrentPBXAccount(account);
		if (0 == nRet) {
			pbxAccount.assign(account, 11);
		}

		return nRet;
	}

	int32_t PbxService::IncomingCallUserName(int taskID, std::string &userName) {
		return 0;
	}

	// ucClient.PbxService.getCallRecordLst(2)
	int32_t PbxService::GetCallRecordLst(int32_t& count, int64_t timeStamp) {

		PbxCallRecordList lstData;

		int selMask = AC_PbxCallRecord::PMALL;
		int nRet = DBManager::GetInstance().GetPbxCallRecordLst(timeStamp, count, selMask, lstData);
		UC_LOG(INFO) << "GetCallRecordLst count=" << count << ", ret=" << nRet;

		count = lstData.size();

		PbxCallRecordList lstT;
		AC_PbxCallRecord cond;
		selMask = AC_PbxCallRecord::PM_OPERATE_TIME;
		for (AC_PbxCallRecord &dataT : lstData) {
			cond.SetMarker(AC_PbxCallRecord::PMNULL);
			if (0 == dataT.GetUserid()) {
				cond.SetNumber(dataT.GetNumber());
			}
			else {
				cond.SetUserid(dataT.GetUserid());
			}
			lstT.clear();
			DBManager::GetInstance().SelectPbxCallDetails(cond, 0, 0, selMask, lstT);
			dataT.SetCallid(lstT.size());
		}

		triggerEvent(IPbxServiceEvent, OnGetCallRecordLst(nRet, lstData));

		return nRet;
	}
	// ucClient.PbxService.getCallDetailsLst('15840436354',0,0,3)
	int32_t PbxService::GetCallDetailsLst(const std::string &number, int userID, int64_t timeStamp, int count) {
		bool isNumber = true;
		// 匹配任意个数字 
		// 		std::string key = "^[0-9]*$";
		// 		boost::regex reg(key);
		// 		isNumber = boost::regex_match(condition, reg);

		int nRet = ERROR_INPUT_PARAMETER;
		if (isNumber) {
			PbxCallRecordList lstData;
			AC_PbxCallRecord cond;
			if (0 == userID) {
				cond.SetNumber(number);
			}
			else {
				cond.SetUserid(userID);
			}
			int selMask = AC_PbxCallRecord::PMALL;
			nRet = DBManager::GetInstance().SelectPbxCallDetails(cond, timeStamp, count, selMask, lstData);
			triggerEvent(IPbxServiceEvent, OnGetCallDetailsLst(nRet, lstData));
		}

		UC_LOG(INFO) << "GetCallDetailsLst number=" << number << ", userID=" << userID
			<< ", offset=" << timeStamp << ", count=" << count << ", ret=" << nRet;

		return nRet;
	}
	// ucClient.PbxService.searchCallRecord('158',0,3)
	int32_t PbxService::SearchCallRecord(const std::string &keyWord, int64_t timeStamp, int count) {
		PbxCallRecordList lstData;

		int nRet = DBManager::GetInstance().SearchPbxCallRecord(keyWord, timeStamp, count, lstData);

		triggerEvent(IPbxServiceEvent, OnSearchCallRecord(nRet, lstData));

		return nRet;
	}

	int32_t PbxService::SaveCallDetails(int64_t timeStamp, int taskID, const std::string &number,
		int userID, const std::string &name, int incoming, int type, int state, int druation) {
		AC_PbxCallRecord data;
		data.SetOperatetime(timeStamp);
		data.SetCallid(taskID);
		data.SetNumber(number);
		data.SetName(name);
		data.SetState(state, (1 == incoming));
		data.SetUserid(userID);
		data.SetType(type);
		data.SetDruation(druation);

		return DBManager::GetInstance().InsertPbxCallDetails(data);
	}
	//////////////////////////////////////////////////////////////////////////
	int PbxService::_LoginPbx() {
		UC_LOG(INFO) << "Start LoginPbx Server!";
		cout << "Start LoginPbx Server!" << endl;

		_serverUrlValue = _engine->GetConfig().casServer;
		int nPos = _serverUrlValue.find("//");
		if (std::string::npos != nPos) {
			_serverUrlValue = _serverUrlValue.substr(nPos + 2);
		}

		_lpParams[0].param_value = (char*)_serverUrlValue.c_str();
		_lpParams[1].param_value = (char*)_accessTokenValue.c_str();

		int nRet = 0;
		nRet = EasiioLogin(_account.c_str(), _paramsCount, _lpParams);
		_login = true;
		UC_LOG(INFO) << "Easiio Login token=" << _lpParams[1].param_value
			<< ", url=" << _lpParams[0].param_value << ", EasiioLogin Ret=" << nRet;
		_reLogin = (REQUEST_SUCCESS == nRet);
		UC_LOG(INFO) << "reLogin=" << _reLogin << ", isInit=" << _isInit << ", Connected=" << _isConnected;
		if (REQUEST_SUCCESS != nRet) {
			_loginRet = LR_PARAM;
		}
		else {
			_loginRet = LR_TOKEN;
		}

		return nRet;
	}

	void PbxService::_LogoutPbx() {
		UC_LOG(INFO) << "LogoutPbx! login flag=" << _login;
		cout << "LogoutPbx! login flag=" << _login << endl;
		if (!_login) {
			return;
		}
		_login = false;

		EasiioLogout();
	}

	int PbxService::_MicRet(int ret) {

		switch (ret) {
		case -1: ret = EC_MAKECALL; break;
		case -300: ret = EC_MIC_IN; break;
		case -301: ret = EC_MIC_OUT; break;
		case -302: ret = EC_MIC_INOUT; break;
		default:
			break;
		}

		return ret;
	}
	//////////////////////////////////////////////////////////////////////////
	// IUCEngineEvent
	void PbxService::OnUCASConnected(int32_t errorCode) {
		UC_LOG(INFO) << "OnUCASConnected errorCode=" << errorCode;
		cout << "OnUCASConnected errorCode=" << errorCode << endl;
		if (0 != errorCode) {
			return;
		}

		if (_account.length() > 0 && !_isConnected) {
			_isConnected = true;
			UC_LOG(INFO) << "reLogin=" << _reLogin << ", isInit=" << _isInit << ", Connected=" << _isConnected;
			_lpPbxSrv->GetToken();
		}
	}

	void PbxService::OnUCASDisconnected(int32_t errorCode) {
		UC_LOG(INFO) << "OnUCASDisconnected errorCode=" << errorCode;
		cout << "OnUCASDisconnected errorCode=" << errorCode << endl;

		_reLogin = false;

		UC_LOG(INFO) << "reLogin=" << _reLogin << ", isInit=" << _isInit << ", Connected=" << _isConnected;
		if (_isConnected) {
			_isConnected = false;
			_loginRet = LR_WAITING;

			CancelTimer();

			_LogoutPbx();
		}
	}

	void PbxService::OnLoginSucceeded() {
		UC_LOG(INFO) << "On LoginSucceeded";
		cout << "On LoginSucceeded" << endl;
		InitPbx();
	}

	void PbxService::OnLogoutSucceeded() {
		UC_LOG(INFO) << "On LogoutSucceeded=" << _account;
		cout << "On LogoutSucceeded" << endl;
		_account = "";
	}

	void PbxService::ConnectPbx(int32_t userID, const std::string &token, int expiresIn) {

		UC_LOG(INFO) << "ConnectPbx account=" << userID << ", \r\ntokenNew=" << token << ", Connected=" << _isConnected
			<< ", \r\ntokenOld=" << _accessTokenValue.c_str() << "， expiresIn=" << expiresIn;
		cout << "ConnectPbx account=" << userID << ", \r\ntokenNew=" << token << ", Connected=" << _isConnected
			<< ", \r\ntokenOld=" << _accessTokenValue.c_str() << "， expiresIn=" << expiresIn << endl;

		if (_isConnected && 0 == _accessTokenValue.compare(token)) {
			return;
		}

		_isConnected = true;
		UC_LOG(INFO) << "reLogin=" << _reLogin << ", isInit=" << _isInit << ", Connected=" << _isConnected;

		std::stringstream account;
		account << userID;
		_account = account.str();

		_accessTokenValue = token;

		ReLogin(2);
	}

	void PbxService::SetProxyUrl(const std::string& proxyUrl){
		UC_LOG(INFO) << "Set ProxyUrl: proxy url: " << proxyUrl;

		std::string dirU = proxyUrl;
		std::wstring dirW = StringConvert::FromUTF8Str(dirU);
		std::string proxyUrlA = StringConvert::Unicode2Ascii(dirW);

		EasiioSetProxyUrl(proxyUrlA.c_str());
	}

	void PbxService::SetProxyInfo(int32_t auth_type,
		const std::string& auth_account, const std::string& auth_passwd) {

		std::string dirU = auth_account;
		std::wstring dirW = StringConvert::FromUTF8Str(dirU);
		std::string auth_accountA = StringConvert::Unicode2Ascii(dirW);

		dirU = auth_passwd;
		dirW = StringConvert::FromUTF8Str(dirU);
		std::string auth_passwdA = StringConvert::Unicode2Ascii(dirW);
		UC_LOG(INFO) << "Set ProxyInfo: type=" << auth_type
			<< ", account=" << auth_accountA.c_str() << ", password len=" << auth_passwdA.length();

		EasiioSetProxyInfo(auth_type, auth_accountA.c_str(), auth_passwd.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	* 登录结果回调
	*/
	void PbxService::OnLoginResult(int resultCode, const char* resultMsg) {
		UC_LOG(INFO) << "onLoginResult resultCode=" << resultCode << ", resultMsg=" << resultMsg;
		cout << "onLoginResult resultCode=" << resultCode << ", resultMsg=" << resultMsg << endl;

		triggerEventEx(IPbxServiceEvent, *(_lpPbxSrv), OnLoginPbx(resultCode));

		_lpPbxSrv->_loginRet = resultCode;
		if (LOGIN_SUCCESS != resultCode) {
			if (LOGIN_FAILED_LIBRARY_ERROR == resultCode
				|| LOGIN_FAILED_NO_PERMISSION_TO_CREATE_USER == resultCode) {
				_lpPbxSrv->CancelTimer();
			}
			else if (_lpPbxSrv->_reLogin
				&& _lpPbxSrv->_isInit
				&& _lpPbxSrv->_isConnected) {

				UC_LOG(INFO) << "reLoginPbx";
				cout << "reLoginPbx" << endl;

				_lpPbxSrv->ReLogin(3);
			}
			UC_LOG(INFO) << "reLogin=" << _lpPbxSrv->_reLogin << ", isInit="
				<< _lpPbxSrv->_isInit << ", Connected=" << _lpPbxSrv->_isConnected;
		}
		else {
			std::string account;
			_lpPbxSrv->GetCurrentPBXAccount(account);
			UC_LOG(INFO) << "CurrentPBXAccount=" << account;
			cout << "CurrentPBXAccount=" << account << endl;
		}
	}

	/**
	* 通话状态变化通知
	*/
	void PbxService::OnCallStatusChanged(int callID, int callState,
		const char* resultCode, const char* callUUID) {
		UC_LOG(INFO) << "onCallStatusChanged callID=" << callID << ", callState="
			<< callState << ", resultCode=" << resultCode << ", callUUID = " << callUUID;
		cout << "onCallStatusChanged callID=" << callID << ", callState="
			<< callState << ", resultCode=" << resultCode << ", callUUID = " << callUUID << endl;

		if (CALL_STATE_INCOMING == callState) {
			return;
		}

		int32_t taskID = 0;
		__int64 timeStamp = 0;
		_lpPbxSrv->UpCallStatus(callID, callState, callUUID, taskID, timeStamp);

		triggerEventEx(IPbxServiceEvent, *(_lpPbxSrv),
			OnStatusChanged(timeStamp, taskID, callState, resultCode, callUUID));
	}

	/**
	* VoIP呼叫通知
	*/
	void PbxService::OnMakeCallResult(int callID, const char* toNumber) {

		std::string number = toNumber;
		_lpPbxSrv->SaveCallID(number, callID);

		__int64 timeStamp = 0;
		int32_t taskID = 0;
		_lpPbxSrv->GetCallKey(number, timeStamp, taskID);

		int nRet = 0;
		if (callID < 0) {
			_lpPbxSrv->_tasks.RemoveTask(taskID);
			nRet = EC_MAKECALL;
		}

		int micState = 0;
		if (callID < 0) {
			micState = _lpPbxSrv->_MicRet(callID);
		}

		UC_LOG(INFO) << "On MakeCallResult taskID=" << taskID << ", callID=" << callID
			<< ", micState=" << micState << ", toNumber=" << number.c_str();
		cout << "VoIP呼叫通知 taskID=" << taskID << ", callID=" << callID
			<< ", micState=" << micState << ", toNumber=" << number.c_str() << endl;

		triggerEventEx(IPbxServiceEvent, *(_lpPbxSrv),
			OnMakeCall(nRet, timeStamp, taskID, micState, toNumber));
	}

	/**
	* 来电通知
	*/
	void PbxService::OnIncomingCall(int callID, int callType,
		const char* fromNumber, const char* postValue) {

		int userID = 0;
		std::string number = fromNumber;

		_lpPbxSrv->CreateCallInfo(CALL_STATE_INCOMING,
			callType, number, userID, number, postValue);
		_lpPbxSrv->SaveCallID(number, callID);

		__int64 timeStamp = 0;
		int32_t taskID = 0;
		_lpPbxSrv->GetCallKey(number, timeStamp, taskID);

		UC_LOG(INFO) << "On IncomingCall taskID=" << taskID << ", callID=" << callID << ", toNumber=" << number.c_str();
		cout << "来电通知 taskID=" << taskID << ", callID=" << callID << ", toNumber=" << number.c_str() << endl;

		triggerEventEx(IPbxServiceEvent, *(_lpPbxSrv),
			OnCallIncoming(timeStamp, taskID, callType, fromNumber, postValue));
	}

	/**
	* PJSIP状态变化通知
	*/
	void PbxService::OnPJSIPServiceStateChanged(int stateCode) {
		// 网络断开时，更新所有在通话状态的时长。
		UC_LOG(INFO) << "onPJSIPServiceStateChanged stateCode=" << stateCode;
		cout << "onPJSIPServiceStateChanged stateCode=" << stateCode << endl;
		if (PJSIP_SC_OK != stateCode) {
			if (PJSIP_EXTENSION_OAUTH_CHECK_FAILED == stateCode) {
				UC_LOG(INFO) << "reLogin=" << _lpPbxSrv->_reLogin << ", isInit="
					<< _lpPbxSrv->_isInit << ", Connected=" << _lpPbxSrv->_isConnected;
				_lpPbxSrv->_loginRet = LR_OAUTH;
				_lpPbxSrv->GetToken();
			}
			else {
				_lpPbxSrv->_loginRet = LR_PBXSRV;
			}
		}

		triggerEventEx(IPbxServiceEvent, *(_lpPbxSrv), OnServiceStateChanged(stateCode));
	}

	//////////////////////////////////////////////////////////////////////////
	int PbxService::InitPbx() {
		int nRet = 1;
		do
		{
			if (_isInit) {
				nRet = 0;
				break;
			}

			CancelTimer();

			NewData();

			nRet = EasiioInit(_lpCBInterface);
			if (0 != nRet) {
				UC_LOG(INFO) << "EasiioInit ret=" << nRet;
				break;
			}

			ClientService *lpService = (ClientService *)_engine->QueryService(UCClientService);
			const LoginInfo &li = lpService->GetLoginInfo();
			UC_LOG(INFO) << "Key=" << li.GetPbxKey().c_str() << ", Token=" << li.GetPbxToken().c_str();
			cout << "Key=" << li.GetPbxKey().c_str() << ", Token=" << li.GetPbxToken().c_str() << endl;
			nRet = EasiioInitDeveloperKeyAndToken(li.GetPbxKey().c_str(), li.GetPbxToken().c_str());
			if (0 != nRet) {
				UC_LOG(INFO) << "EasiioInitDeveloperKeyAndToken ret=" << nRet;
				break;
			}

			_isInit = true;
			UC_LOG(INFO) << "reLogin=" << _reLogin << ", isInit=" << _isInit << ", Connected=" << _isConnected;
		} while (false);

		if (0 != nRet) {
			DeleteData();
		}

		UC_LOG(INFO) << "InitPbx ret=" << nRet;
		cout << "InitPbx ret=" << nRet << endl;

		return nRet;
	}

	void PbxService::DestroyPbx() {
		// 销毁，第三方应用退出时调用，区别于EasiioLogout
		UC_LOG(INFO) << "DestroyPbx S";
		{
			EasiioDestroy();
		}
		UC_LOG(INFO) << "DestroyPbx E";

		CancelTimer();

		DeleteData();
	}

	int32_t PbxService::CreateCallInfo(int status, int callType, const std::string &userName,
		int userID, const std::string &number, const char* postValue) {

		shared_ptr<PbxTask> pTask = make_shared<PbxTask>(PbxTask::MakeTaskID(), (CALL_STATE_INCOMING == status));
		_tasks.AddTask(pTask);

		pTask->SetOperatetime(Time::GetSysTime());
		pTask->SetCallid(-3);

		pTask->SetName(userName);
		pTask->SetUserid(userID);
		pTask->SetNumber(number);

		pTask->SetType(callType);

		pTask->SetState(status, pTask->_isIncoming);

		if (NULL != postValue) {
			pTask->postValue.assign(postValue, strlen(postValue));
		}

		return pTask->GetTaskID();
	}

	int PbxService::SaveCallID(const std::string &number, int callID) {
		shared_ptr<PbxTask> pTask = _tasks.GetTaskByNumber(number);
		if (NULL == pTask) {
			return EC_FAIL;
		}

		pTask->SetCallid(callID);

		UC_LOG(INFO) << "SaveCallID CallInfo opTime=" << pTask->GetOperatetime()
			<< ", taskID=" << pTask->GetTaskID() << ", callID=" << callID
			<< ", TasksCount=" << _tasks.GetTasksCount();
		cout << "SaveCallID CallInfo opTime=" << pTask->GetOperatetime()
			<< ", taskID=" << pTask->GetTaskID() << ", callID=" << callID
			<< ", TasksCount=" << _tasks.GetTasksCount() << endl;

		return 0;
	}

	bool PbxService::GetCallKey(const std::string &number, __int64 &timeStamp, int32_t &taskID) {

		shared_ptr<PbxTask> pTask = _tasks.GetTaskByNumber(number);
		if (NULL == pTask) {
			return false;
		}

		timeStamp = pTask->GetOperatetime();
		taskID = pTask->GetTaskID();

		return true;
	}

	int PbxService::UpCallHold(int taskID, bool isHold) {

		int nRet = ERROR_INPUT_PARAMETER;
		int callID = -1;
		do
		{
			shared_ptr<PbxTask> pTask = _tasks.GetTaskByID(taskID);
			if (NULL == pTask) {
				break;
			}
			callID = pTask->GetCallid();
			if (callID < 0) {
				nRet = EC_MAKING;
				break;
			}

			nRet = pTask->setHold(isHold);
			if (0 != nRet) {
				break;
			}

			if (isHold) {
				nRet = EasiioHoldCall(callID);
			}
			else {
				nRet = EasiioUnHoldCall(callID);
			}
		} while (false);

		UC_LOG(INFO) << "Up CallHold taskID=" << taskID << ", callID=" << callID
			<< ", isHold=" << isHold << ", ret=" << nRet;

		return nRet;
	}

	int PbxService::UpCallMute(int taskID, bool isMute) {
		int nRet = ERROR_INPUT_PARAMETER;
		int callID = -1;
		do
		{
			shared_ptr<PbxTask> pTask = _tasks.GetTaskByID(taskID);
			if (NULL == pTask) {
				break;
			}
			callID = pTask->GetCallid();
			if (callID < 0) {
				nRet = EC_MAKING;
				break;
			}

			nRet = pTask->setMute(isMute);
			if (0 != nRet) {
				break;
			}

			if (isMute) {
				nRet = EasiioMuteCall(callID);
			}
			else {
				nRet = EasiioUnMuteCall(callID);
			}
		} while (false);

		UC_LOG(INFO) << "Up CallMute taskID=" << taskID << ", callID=" << callID
			<< ", isMute=" << isMute << ", ret=" << nRet;

		return nRet;
	}

	bool PbxService::UpCallStatus(int callID, int status,
		const char* callUUID, int32_t &taskID, __int64 &opTime) {

		shared_ptr<PbxTask> pTask = _tasks.GetTaskByCallID(callID);
		if (NULL == pTask) {
			return false;
		}

		taskID = pTask->GetTaskID();
		opTime = pTask->GetOperatetime();

		if (NULL != callUUID && pTask->callUUID.size() <= 0) {
			pTask->callUUID.assign(callUUID, strlen(callUUID));
		}

		if (CALL_STATE_DISCONNECTED != status) {
			pTask->SetState(status, pTask->_isIncoming);

			if (CALL_STATE_CONFIRMED == status) {
				pTask->Calling();
			}
		}
		else {
			pTask->CallDruation();

			DBManager::GetInstance().InsertPbxCallDetails(*pTask);

			triggerEvent(IPbxServiceEvent, OnCallHangup(*pTask));

			_tasks.RemoveTask(taskID);
		}

		return true;
	}

	void PbxService::NewData() {
		if (NULL != _lpCBInterface) {
			return;
		}

		_lpCBInterface = new EASIIO_CALLBACK_INTERFACE;
		memset(_lpCBInterface, 0, sizeof(EASIIO_CALLBACK_INTERFACE));

		_lpCBInterface->onLoginResult = &OnLoginResult;
		_lpCBInterface->onCallStatusChanged = &OnCallStatusChanged;
		_lpCBInterface->onMakeCallResult = &OnMakeCallResult;
		_lpCBInterface->onIncomingCall = &OnIncomingCall;
		_lpCBInterface->onPJSIPServiceStateChanged = &OnPJSIPServiceStateChanged;

		_lpCBInterface->onMakeTwoWayCallResult = &OnMakeTwoWayCallResult;
		_lpCBInterface->onTwoWayCallStatusChanged = &OnTwoWayCallStatusChanged;
		_lpCBInterface->onCallRecordingReaponse = &OnCallRecordingReaponse;
		_lpCBInterface->onGetAllRecordResponse = &OnGetAllRecordResponse;
		_lpCBInterface->onGetRecordResponse = &OnGetRecordResponse;

		_paramsCount = 2;
		_lpParams = new EasiioLoginParams[_paramsCount];
		memset(_lpParams, 0, sizeof(EasiioLoginParams)* _paramsCount);

		_serverUrlKey = LOGIN_PARAM_SERVER_URL;
		_lpParams[0].param_key = (char *)_serverUrlKey.c_str();

		_accessTokenKey = LOGIN_PARAM_ACCESS_TOKEN;
		_lpParams[1].param_key = (char *)_accessTokenKey.c_str();
	}

	void PbxService::DeleteData() {
		UC_LOG(INFO) << "DeleteData S";

		if (NULL != _lpCBInterface) {
			delete _lpCBInterface;
			_lpCBInterface = NULL;
		}

		if (NULL != _lpParams) {
			delete[] _lpParams;
			_lpParams = NULL;
		}
		UC_LOG(INFO) << "DeleteData E";
	}

	//////////////////////////////////////////////////////////////////////////
	void PbxService::ReLogin(int expiresIn) {
		_LogoutPbx();

		CancelTimer();

		if (expiresIn > 20) {
			expiresIn -= 10;
		}

		Timer::GetInstance().Schedule(
			MakeCallback(this, &PbxService::onTimeReLogin), expiresIn * 1000, &_timerId);
	}

	void PbxService::onTimeReLogin() {
		UC_LOG(INFO) << "start onTimeReLogin";
		cout << "start onTimeReLogin" << endl;

		if (0 == _lpPbxSrv->_LoginPbx()) {
			_lpPbxSrv->_loginRet = LR_RELOGIN;
		}
	}

	void PbxService::CancelTimer() {
		if (_timerId > 0) {
			Timer::GetInstance().Cancel(_timerId);
			_timerId = -1;
		}
	}

	void PbxService::GetToken() {
		UC_LOG(INFO) << "GetToken token=" << _accessTokenKey;
		cout << "GetToken token=" << _accessTokenKey << endl;

		_isConnected = false;
		ClientService *lpService = (ClientService *)_engine->QueryService(UCClientService);
		lpService->GetAccessToken(OGT_PASSWORD);
	}

	//////////////////////////////////////////////////////////////////////////
	// 双向呼叫
	// ucClient.PbxService.makeTwoWayCall('15840436354', '15840052784', 0)
	int32_t PbxService::MakeTwoWayCall(const std::string &toNumber,
		const std::string &fromNumber, int openRecord) {
		int nRet = 0;
		if (fromNumber.length() < 0) {
			return 1;
		}
		if (toNumber.length() < 0) {
			return 1;
		}

		nRet = EasiioMakeTwoWayCall(toNumber.c_str(), fromNumber.c_str(), openRecord);
		return nRet;
	}
	/**
	* 调用双向呼叫结果回调
	*/
	void PbxService::OnMakeTwoWayCallResult(EasiioResponseReason reason,
		const char* caller, const char* callee,
		const char* callUUID, const char* retryId) {
		UC_LOG(INFO) << "onMakeTwoWayCallResult reason=" << reason.reason << ", msg="
			<< reason.msg << ", caller=" << caller << ", callee=" << callee
			<< ", callUUID=" << callUUID << ", retryId" << retryId;
	}

	/**
	* 双向呼叫通话状态通知
	*/
	void PbxService::OnTwoWayCallStatusChanged(int callState, const char* taskID,
		const char* caller, const char* callee, int retrying) {
		UC_LOG(INFO) << "onTwoWayCallStatusChanged taskID=" << taskID << ", callState="
			<< callState << ", caller=" << caller << ", callee = " << callee
			<< ", retrying = " << retrying;
	}
	//////////////////////////////////////////////////////////////////////////
	// 通话中开始或停止录音
	int32_t PbxService::SwitchRecordInCall(int recordSwitch, const std::string &callUUID) {
		int nRet = 1;
		return nRet;
	}

	/**
	* 通话中录音开启或关闭调用回调
	*/
	void PbxService::OnCallRecordingReaponse(EasiioResponseReason reason,
		int switchRecord, const char* callUUID) {
		UC_LOG(INFO) << "onCallRecordingReaponse reason=" << reason.reason << ", msg=" << reason.msg
			<< ", callUUID=" << callUUID << ", switchRecord=" << switchRecord;
	}
	//////////////////////////////////////////////////////////////////////////
	// 获取所有录音列表目录
	int32_t PbxService::GetAllRecord(int recordType) {
		int nRet = 0;
		if (RECORD_TYPE_IN_CALL != recordType) {
			return 1;
		}

		nRet = EasiioGetAllRecord(recordType);

		return nRet;
	}

	// 获取单次通话录音
	int32_t PbxService::GetRecord(int recordType, const std::string &callUUID) {
		int nRet = 0;
		if (RECORD_TYPE_IN_CALL != recordType) {
		}
		nRet = EasiioGetRecord(recordType, callUUID.c_str());
		return nRet;
	}
	/**
	* 获取所有录音回调
	*/
	void PbxService::OnGetAllRecordResponse(EasiioResponseReason reason, int count, EasiioRecord* record) {
		UC_LOG(INFO) << "onGetAllRecordResponse reason=" << reason.reason << ", msg="
			<< reason.msg << ", count=" << record->record_count;
		if (record == NULL || count == 0) {
			UC_LOG(INFO) << "onGetAllRecordResponse record is null or count is 0";
			return;
		}

		EasiioRecord* lpER = record;
		for (int i = 0; i < count; i++) {
			printf("EasiioRecord:i=%d, from_user=%s, call_uuid=%s, record_count=%d, "
				"direction=%s, to_user=%s, answer_time=%s\n",
				i, lpER->from_user, lpER->call_uuid, lpER->record_count,
				lpER->direction, lpER->to_user, lpER->answer_time);
			if (i < count - 1) {
				lpER++;
			}
		}
	}

	/**
	* 获取单次通话录音回调
	*/
	void PbxService::OnGetRecordResponse(EasiioResponseReason reason, const char* callUUID,
		int count, EasiioRecordItem* recordItem) {
		printf("onGetRecordResponse reason=%d, msg=%s, callUUID=%s, count=%d\n",
			reason.reason, reason.msg, callUUID, count);
		if (recordItem == NULL || count == 0) {
			printf("onGetRecordResponse record is null or count is 0\n");
			return;
		}

		EasiioRecordItem* lpER = recordItem;
		for (int i = 0; i < count; i++) {
			printf("EasiioRecordItem:i=%d, insert_time=%s, uri=%s, "
				"record_ms=%.2f, uuid=%s, answer_time=%s\n",
				i, lpER->insert_time, lpER->uri, lpER->record_ms, lpER->uuid, lpER->answer_time);
			if (i < count - 1) {
				lpER++;
			}
		}
	}

#include "io.h"
	void PbxService::InitConsoleWindow() {

		int fh = -2;
#if toConsole
		fh = 0;
#endif // toConsole
		if (fh == 0 && AllocConsole() != 0) {
			fclose(stdout);
			fclose(stderr);
			fh = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 0);
			if (-1 != fh) {
				// 				_dup2(fh, 1);
				// 				_dup2(fh, 2);
				// 				_fdopen(1, "wt");
				// 				_fdopen(2, "wt");
				// 				fflush(stdout);
			}
		}
		std::string ver = "2016.06.16.01，pbx:V1.2.4";
		printf("run %d %s.\r\n", fh, ver.c_str());
		UC_LOG(INFO) << ver.c_str();
	}
}

#endif // PBX_SERVICE