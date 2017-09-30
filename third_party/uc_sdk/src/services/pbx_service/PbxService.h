#ifndef PBXSERVICE_H_
#define PBXSERVICE_H_
#pragma once

#include "IPbxService.h"

#include "EasiioLibrary.h"
#include "base/Timer.h"

#include "PbxErrorCode.h"
#include "TasksMgr.h"

#ifdef PBX_SERVICE
namespace uc {
	class PbxService : public IPbxService, IUCEngineEvent
	{
	public:
		PbxService(IUCEngine *engine);
		virtual ~PbxService();

		static PbxService* _lpPbxSrv;

	public:

	protected:
		enum CALL_STATE {
			CALL_STATE_REJECT = CALL_STATE_DISCONNECTED + 1, // 拒接来电
			CALL_STATE_HANGUP, // 主动挂断通话
		};

		enum LOGIN_RET {
			LR_SUCCESS,
			LR_PBXSRV = LOGIN_FAILED_LIBRARY_ERROR + 1, // pbx服务器返回错误
			LR_RELOGIN, // 重连
			LR_OAUTH, // 账号验证失败
			LR_PARAM, // 参数错误
			LR_TOKEN, // 获取token
			LR_WAITING, // 
			LR_NOTOPEN, // 
		};

	protected:
		// IPbxService
		virtual void ConnectPbx(int32_t userID, const std::string &token, int expiresIn);
		virtual void SetProxyUrl(const std::string& proxyUrl);
		virtual void SetProxyInfo(int32_t auth_type, const std::string& auth_account, const std::string& auth_passwd);

		virtual int32_t CheckLogin(int &nRet);
		// VoIP呼叫
		virtual int32_t MakeCall(const std::string &userName, int userID, const std::string &number,
			int callType, const std::string &postValue, __int64 &timeStamp, int32_t &taskID);
		// 来电应答
		virtual int32_t AnswerCall(int taskID, int callType);
		// 拒接来电
		virtual int32_t RejectCall(int taskID);
		// 挂断通话
		virtual int32_t HangupCall(int taskID);
		// 通话保持
		virtual int32_t HoldCall(int taskID);
		// 解除通话保持
		virtual int32_t UnHoldCall(int taskID);
		// 通话静音
		virtual int32_t MuteCall(int taskID);
		// 解除通话静音
		virtual int32_t UnMuteCall(int taskID);

		// 调整通话麦克风音量
		virtual int32_t AdjustMicLevel(int taskID, int level);
		// 刷新麦克风
		virtual int32_t RefreshMic(int &micState);

		// 发送DTMF
		virtual int32_t SendDTMF(int taskID, const std::string &dtmf);
		// 获取当前PBX号码
		virtual int32_t GetCurrentPBXAccount(std::string &pbxAccount);
		//////////////////////////////////////////////////////////////////////////
		virtual int32_t IncomingCallUserName(int taskID, std::string &userName);

		virtual int32_t GetCallRecordLst(int32_t& count, int64_t timeStamp);

		virtual int32_t GetCallDetailsLst(const std::string &number, int userID, int64_t timeStamp, int count);

		virtual int32_t SearchCallRecord(const std::string &keyWord, int64_t timeStamp, int count);

		virtual int32_t SaveCallDetails(int64_t timeStamp, int taskID, const std::string &number,
			int userID, const std::string &name, int incoming, int type, int state, int druation);
		//////////////////////////////////////////////////////////////////////////

	protected:
		int _LoginPbx();
		void _LogoutPbx();

		int _MicRet(int ret);

	protected:
		// IUCEngineEvent
		// ucas连接的事件
		virtual void OnUCASConnected(int32_t errorCode);
		// ucas断开连接的回调事件
		virtual void OnUCASDisconnected(int32_t errorCode);
		// 登陆成功的回调事件
		virtual void OnLoginSucceeded();
		// 登出成功的回调事件
		virtual void OnLogoutSucceeded();
		//////////////////////////////////////////////////////////////////////////

	private:
		// EASIIO_CALLBACK_INTERFACE
		static void OnLoginResult(int resultCode, const char* resultMsg); // 登录结果

		static void OnCallStatusChanged(int callID, int callState,
			const char* resultCode, const char* callUUID); // 通话状态变化通知

		static void OnMakeCallResult(int callID, const char* toNumber); // VoIP呼叫通知

		static void OnIncomingCall(int callID, int callType,
			const char* fromNumber, const char* postValue); // 来电通知

		static void OnPJSIPServiceStateChanged(int stateCode); // PJSIP状态变化通知

	private:
		int InitPbx();
		void DestroyPbx();

		int32_t CreateCallInfo(int status, int callType, const std::string &userName,
			int userID, const std::string &number, const char* postValue);
		int SaveCallID(const std::string &number, int callID);
		bool GetCallKey(const std::string &number, __int64 &timeStamp, int32_t &taskID);

		int UpCallHold(int callID, bool isHold);
		int UpCallMute(int callID, bool isMute);

		bool UpCallStatus(int callID, int status, const char* callUUID, 
			int32_t &taskID, __int64 &opTime);

		void NewData();
		void DeleteData();

		void ReLogin(int expiresIn);
		void onTimeReLogin();
		void CancelTimer();

		void GetToken();

		void InitConsoleWindow();

	private:
		bool _isInit;
		bool _reLogin;
		bool _isConnected;
		int _loginRet;

		int _paramsCount;
		EasiioLoginParams* _lpParams;
		EASIIO_CALLBACK_INTERFACE *_lpCBInterface;

		PbxTasks _tasks;

		timer_id _timerId;

		std::string _account;

		std::string _serverUrlKey;
		std::string _serverUrlValue;

		std::string _accessTokenKey;
		std::string _accessTokenValue;

		std::string _logDirA;

		bool _login;

	protected:
		// 双向呼叫
		virtual int32_t MakeTwoWayCall(const std::string &toNumber,
			const std::string &fromNumber, int openRecord);
		static void OnMakeTwoWayCallResult(EasiioResponseReason reason, const char* caller,
			const char* callee, const char* callUUID, const char* retryId); // 双向呼叫结果
		static void OnTwoWayCallStatusChanged(int callState, const char* callUUID,
			const char* caller, const char* callee, int retrying); // 双向呼叫通话状态通知

		// 通话中开始或停止录音
		virtual int32_t SwitchRecordInCall(int recordSwitch, const std::string &callUUID);
		static void OnCallRecordingReaponse(EasiioResponseReason reason,
			int switchRecord, const char* callUUID); // 通话中开启、停止录音调用结果

		// 获取所有录音列表目录
		virtual int32_t GetAllRecord(int recordType);
		// 获取单次通话录音
		virtual int32_t GetRecord(int recordType, const std::string &callUUID);
		static void OnGetAllRecordResponse(EasiioResponseReason reason,
			int count, EasiioRecord *record); // 获取所有录音回调结果 
		static void OnGetRecordResponse(EasiioResponseReason reason,
			const char* callUUID, int count, EasiioRecordItem *recordItem); // 获取单次通话录音回调结果
	};
}
#endif // PBX_SERVICE

#endif /* PBXSERVICE_H_ */