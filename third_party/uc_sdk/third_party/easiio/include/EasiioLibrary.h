/*
* Copyright (c) 2015-2016 Easiio, Inc. All Rights Reserved.
*  
* 版权：厦门奥极亿通讯科技有限公司
* 描述：该头文件仅用于第三方集成本公司Windows PC版VoIP通讯相关的动态库。
* 网址：http://www.easiio.cn
* 邮箱：gavin.zhuang@easiio.com
*
*/

#ifndef EASIIOLIBRARY_H
#define EASIIOLIBRARY_H

#ifdef EASIIOLIBRARY_EXPORTS

#define DLLOPTION _declspec(dllexport)

#else

#define DLLOPTION _declspec(dllimport)

#endif

#ifdef __cplusplus
extern "C"
{
#endif

	//登录可选参数
	static const char* LOGIN_PARAM_USER_FLAG = "user_flag"; // 第三方集成用户标识
	static const char* LOGIN_PARAM_EASIIO_PSTN = "easiio_pstn"; // 落地权限：1开通，0关闭
	static const char* LOGIN_PARAM_CITY_CALL = "city_call"; // 市话权限：1开通，0关闭
	static const char* LOGIN_PARAM_NATIONAL_CALL = "national_call"; // 国内长途权限：1开通，0关闭
	static const char* LOGIN_PARAM_INTERNATIONAL_CALL = "international_call"; // 国际长途权限：1开通，0关闭
	static const char* LOGIN_PARAM_SERVER_URL = "server_url"; // 第三方服务端地址验证参数
	static const char* LOGIN_PARAM_ACCESS_TOKEN = "access_token";// 第三方服务端token验证参数

	enum{// PJSIP 状态
		PJSIP_SC_TRYING = 100, // Trying: extended search being performed may take a significant time so a forking proxy must send a 100 Trying response
		PJSIP_SC_RINGING = 180, // Ringing
		PJSIP_SC_CALL_BEING_FORWARDED = 181, // Call Is Being Forwarded
		PJSIP_SC_QUEUED = 182, // Queued
		PJSIP_SC_PROGRESS = 183, // Session Progress

		PJSIP_SC_OK = 200, // OK
		PJSIP_SC_ACCEPTED = 202, // accepted: It Indicates that the request has been understood but actually can't be processed

		PJSIP_SC_MULTIPLE_CHOICES = 300, // Multiple Choices
		PJSIP_SC_MOVED_PERMANENTLY = 301, // Moved Permanently
		PJSIP_SC_MOVED_TEMPORARILY = 302, // Moved Temporarily
		PJSIP_SC_USE_PROXY = 305, // Use Proxy
		PJSIP_SC_ALTERNATIVE_SERVICE = 380, // Alternative Service

		PJSIP_SC_BAD_REQUEST = 400, // Bad Request
		PJSIP_SC_UNAUTHORIZED = 401, // Unauthorized (Used only by registrars or user agents. Proxies should use proxy authorization 407
		PJSIP_SC_PAYMENT_REQUIRED = 402, // Payment Required (Reserved for future use)
		PJSIP_SC_FORBIDDEN = 403, // Forbidden"),
		PJSIP_SC_NOT_FOUND = 404, // Not Found (User not found)
		PJSIP_SC_METHOD_NOT_ALLOWED = 405, // Method Not Allowed
		PJSIP_SC_NOT_ACCEPTABLE = 406, // Not Acceptable
		PJSIP_SC_PROXY_AUTHENTICATION_REQUIRED = 407, // Proxy Authentication Required
		PJSIP_SC_REQUEST_TIMEOUT = 408, // Request Timeout (Couldn't find the user in time)
		PJSIP_SC_GONE = 410, // Gone (The user existed once, but is not available here any more.)
		PJSIP_SC_REQUEST_ENTITY_TOO_LARGE = 413, // Request Entity Too Large
		PJSIP_SC_REQUEST_URI_TOO_LONG = 414, // Request-URI Too Long
		PJSIP_SC_UNSUPPORTED_MEDIA_TYPE = 415, // Unsupported Media Type
		PJSIP_SC_UNSUPPORTED_URI_SCHEME = 416, // Unsupported URI Scheme
		PJSIP_SC_BAD_EXTENSION = 420, // Bad Extension (Bad SIP Protocol Extension used, not understood by the server)
		PJSIP_SC_EXTENSION_REQUIRED = 421, // Extension Required
		PJSIP_SC_SESSION_TIMER_TOO_SMALL = 422, // Session Interval Too Small
		PJSIP_SC_INTERVAL_TOO_BRIEF = 423, // Interval Too Brief
		PJSIP_SC_TEMPORARILY_UNAVAILABLE = 480, // Temporarily Unavailable
		PJSIP_SC_CALL_TSX_DOES_NOT_EXIST = 481, // Call/Transaction Does Not Exist
		PJSIP_SC_LOOP_DETECTED = 482, // Loop Detected
		PJSIP_SC_TOO_MANY_HOPS = 483, // Too Many Hops
		PJSIP_SC_ADDRESS_INCOMPLETE = 484, // Address Incomplete
		PJSIP_AC_AMBIGUOUS = 485, // Ambiguous
		PJSIP_SC_BUSY_HERE = 486, // Busy Here
		PJSIP_SC_REQUEST_TERMINATED = 487, // Request Terminated
		PJSIP_SC_NOT_ACCEPTABLE_HERE = 488, // Not Acceptable Here
		PJSIP_SC_BAD_EVENT = 489, // Bad Event
		PJSIP_SC_REQUEST_UPDATED = 490, // Request Updated
		PJSIP_SC_REQUEST_PENDING = 491, // Request Pending
		PJSIP_SC_UNDECIPHERABLE = 493, // Undecipherable (Could not decrypt S/MIME body part)

		PJSIP_SC_INTERNAL_SERVER_ERROR = 500, // Server Internal Error
		PJSIP_SC_NOT_IMPLEMENTED = 501, // Not Implemented: The SIP request method is not implemented here
		PJSIP_SC_BAD_GATEWAY = 502, // Bad Gateway
		PJSIP_SC_SERVICE_UNAVAILABLE = 503, // Service Unavailable
		PJSIP_SC_SERVER_TIMEOUT = 504, // Server Time-out
		PJSIP_SC_VERSION_NOT_SUPPORTED = 505, // Version Not Supported: The server does not support this version of the SIP protocol
		PJSIP_SC_MESSAGE_TOO_LARGE = 513, // Message Too Large
		PJSIP_SC_PRECONDITION_FAILURE = 580, // Precondition Failure

		PJSIP_SC_BUSY_EVERYWHERE = 600, // Busy Everywhere
		PJSIP_SC_DECLINE = 603, // Decline
		PJSIP_SC_DOES_NOT_EXIST_ANYWHERE = 604, // Does Not Exist Anywhere
		PJSIP_SC_NOT_ACCEPTABLE_ANYWHERE = 606, // Not Acceptable

		PJSIP_EXTENSION_OAUTH_CHECK_FAILED = 1000, // 第三方账号验证失败
	};

	enum{ // 登录结果
		LOGIN_SUCCESS = 0, // 登录成功
		LOGIN_FAILED_KEY_OR_TOKEN_IS_ERROR, // Developer key 或者 token是错误的
		LOGIN_FAILED_KEY_OR_TOKEN_IS_EMPTY, // Developer key 或者 token是空的
		LOGIN_FAILED_NO_PERMISSION_TO_CREATE_USER, // 该developer key 创建新用户的权限已经关闭
		LOGIN_FAILED_GET_DOMAIN_FAILED, //获取Domain信息失败
		LOGIN_FAILED_GET_USER_INFO_FAILED, //获取用户信息失败
		LOGIN_FAILED_LOGIN_PBX, // 登录PBX平台失败
		LOGIN_FAILED_GET_SIP_PROVISION, // 获取SIP配置信息失败
		LOGIN_FAILED_LIBRARY_ERROR, // Library内部出错
	};

	enum{ // VoIP通话状态
		CALL_STATE_INCOMING = 0,
		CALL_STATE_INIT,
		CALL_STATE_EARLY,
		CALL_STATE_CONNECTING,
		CALL_STATE_CONFIRMED,
		CALL_STATE_DISCONNECTED
	};

	enum{ //双向呼叫状态
		TWO_WAY_CALL_STATE_CALLING,
		TWO_WAY_CALL_STATE_RINGING,
		TWO_WAY_CALL_STATE_EARLY,
		TWO_WAY_CALL_STATE_ANSWERED,
		TWO_WAY_CALL_STATE_BRIDGED,
		TWO_WAY_CALL_STATE_HANGUP
	};

	enum{ // 消息类型
		MSG_TYPE_TEXT = 0, // 文本消息
		MSG_TYPE_VOICE = 1, // 语音消息
		MSG_TYPE_IMAGE = 2, // 图片消息
		MSG_TYPE_FILE = 3, // 文件消息
		MSG_TYPE_CONF_INVITE = 10, // 会议邀请
		MSG_TYPE_CONF_NOTIFY = 11, // 会议通知
		MSG_TYPE_TWO_WAY_CALL_STATE = 201 // 双向呼叫状态消息
	};

	enum{// 呼叫类型
		VOICE_CALL = 0, // 语音呼叫
		VIDEO_CALL  // 语音视频呼叫
	};

	enum{ // 请求回调结果
		REQUEST_SUCCESS = 0, // 调用成功
		REQUEST_FAILED,      // 调用失败

	};
	enum{
		RECORD_STOP = 0,   // 停止录音
		RECORD_START // 开始录音
	};

	enum{
		RECORD_CLOSE = 0, // 录音关闭
		RECORD_OPEN       // 录音开启
	};

	enum{ // 获取录音类型
		RECORD_TYPE_FROM_CALL_START = 0, // 通话一开始就录音
		RECORD_TYPE_IN_CALL  // 通话中的录音
	};

	typedef struct _EasiioLoginParams{ // 登录可选参数
		char* param_key; // 参数名
		char* param_value; // 参数值
	}EasiioLoginParams;

	// 单个通话录音目录
	typedef struct _EasiioRecord{
		char* from_user;    // 主叫
		char* call_uuid;      // 通话ID
		int record_count;   // 录音数量
		char* direction;    // 呼出或呼入
		char* to_user;      // 被叫
		char* answer_time;  // 应答时间
	}EasiioRecord;

	// 单个录音文件
	typedef struct _EasiioRecordItem{
		char* insert_time;   // 插入时间
		char* uri;           // 录音地址
		char* uuid;          // 录音ID
		double record_ms;    // 录音时长（秒）
		char* answer_time;   // 应答时间
	}EasiioRecordItem;

	//回调错误码及描述
	typedef struct _EasiioResponseReason
	{
		int reason;//原因
		char* msg;//原因描述
	}EasiioResponseReason;

	// 回调
	typedef struct _EASIIO_CALLBACK_INTERFACE{
		void(*onLoginResult)(int resultCode, const char* resultMsg); // 登录结果
		void(*onPJSIPServiceStateChanged)(int stateCode); // PJSIP状态变化通知
		void(*onCallStatusChanged)(int callId, int callState, const char* resultCode, const char* callUUID); // 通话状态变化通知
		void(*onTwoWayCallStatusChanged)(int callState, const char* callUUID, const char* caller, const char* callee, int retrying); // 双向呼叫通话状态通知
		void(*onIncomingCall)(int callId, int callType, const char* fromNumber, const char* postValue); // 来电通知

		// response
		void(*onMakeTwoWayCallResult)(EasiioResponseReason reason, const char* caller, const char* callee, const char* callUUID, const char* retryId); //双向呼叫结果
		void(*onCallRecordingReaponse)(EasiioResponseReason reason, int switchRecord, const char* callUUID); // 通话中开启、停止录音调用结果
		void(*onGetAllRecordResponse)(EasiioResponseReason reason, int count, EasiioRecord *record); //获取所有录音回调结果 
		void(*onGetRecordResponse)(EasiioResponseReason reason, const char* callUUID, int count, EasiioRecordItem *recordItem); //获取单次通话录音回调结果
		void(*onMakeCallResult)(int callId, const char* toNumber);
	}EASIIO_CALLBACK_INTERFACE;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 初始化和配置
	
	/* @function
	************************************************************************************
	函数名   : EasiioInit
	功能     : 初始化。
	参数     : [IN]  CallbackInterface     : 回调函数指针结构体EASIIO_CALLBACK_INTERFACE
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioInit(EASIIO_CALLBACK_INTERFACE *CallbackInterface);

	/* @function
	************************************************************************************
	函数名   : EasiioInitDeveloperKeyAndToken
	功能     : 初始化开发者密钥和信令。
	参数     : [IN]  developerKey     :  开发者密钥
			   [IN]  token            :  项目信令
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioInitDeveloperKeyAndToken(const char* developerKey, const char* token);

	/* @function
	************************************************************************************
	函数名   : EasiioLogin
	功能     : 账号登录。
	参数     : [IN]  account     :  第三方应用账号
			   [IN]  paramCount  :  登录可选参数数量
	           [IN]  params      :  登录可选参数
	返回值   : 是否成功 0：成功； 非0失败
	回调函数 : void(*onLoginResult)(int resultCode, const char* resultMsg); // 登录结果
	************************************************************************************/
	DLLOPTION int EasiioLogin(const char* account, int paramCount, EasiioLoginParams* params);

	/* @function
	************************************************************************************
	函数名   : EasiioLogout
	功能     : 账号登出。
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioLogout();

	/* @function
	************************************************************************************
	函数名   : EasiioMakeCall
	功能     : VoIP呼叫
	参数     : [IN] number    : 呼叫号码
	           [IN] callType  : 呼叫类型
			        VOICE_CALL, // 语音呼叫
			        VIDEO_CALL  // 语音视频呼叫
			   [IN] postValue : 需要传递给被叫方的值，长度最好不超过256，且最好用BASE64加密
	返回值   : 返回Call Id
			       call id : >=0    成功呼叫
							 -1     呼叫失败
							 -300   没有麦克风输入
							 -301   没有麦克风输出
							 -302   没有麦克风输入输出
	回调函数 : void(*onCallStatusChanged)(int callId, int callState, const char* resultCode, const char* callUUID); // 通话状态变化通知
	           void(*onMakeCallResult)(int callId, const char* toNumber);
	************************************************************************************/
	DLLOPTION int EasiioMakeCall(const char* number, int callType, const char* postValue);

	/* @function
	************************************************************************************
	函数名   : EasiioMakeCallWithRecording
	功能     : VoIP呼叫
	参数     : [IN] number    : 呼叫号码
	[IN] callType  : 呼叫类型
	VOICE_CALL, // 语音呼叫
	VIDEO_CALL  // 语音视频呼叫
	[IN] postValue : 需要传递给被叫方的值，长度最好不超过256，且最好用BASE64加密
	返回值   : 返回Call Id
	call id : >=0    成功呼叫
	-1     呼叫失败
	-300   没有麦克风输入
	-301   没有麦克风输出
	-302   没有麦克风输入输出
	回调函数 : void(*onCallStatusChanged)(int callId, int callState, const char* resultCode, const char* callUUID); // 通话状态变化通知
	           void(*onMakeCallResult)(int callId, const char* toNumber);
	************************************************************************************/
	DLLOPTION int EasiioMakeCallWithRecording(const char* number, int callType);

	/* @function
	************************************************************************************
	函数名   : EasiioMakeTwoWayCall
	功能     : 双向呼叫
	参数     : [IN] toNumber    : 被叫号码
	           [IN] fromNumber  : 主叫号码
			   [IN] openRecord  : 开启录音
	返回值   : 是否成功 0：成功； 非0失败
	回调函数 : void(*onMakeTwoWayCallResult)(EasiioResponseReason reason, const char* caller, const char* callee, const char* callId, const char* retryId); //双向呼叫结果
	           void(*onTwoWayCallStatusChanged)(int callState, const char* callId, const char* caller, const char* callee, int retrying); // 双向呼叫通话状态通知
	************************************************************************************/
	DLLOPTION int EasiioMakeTwoWayCall(const char* toNumber, const char* fromNumber, int openRecord);

	/* @function
	************************************************************************************
	函数名   : EasiioAnswerCall
	功能     : 来电应答
	参数     : [IN] callId    : 来电通话ID
			   [IN] callType  : 应答类型
					VOICE_CALL, // 语音应答
					VIDEO_CALL  // 视频应答
	返回值   : 是否成功 0：成功； 非0失败
	回调函数 : void(*onCallStatusChanged)(int callId, int callState, const char* resultCode); // 通话状态变化通知
	************************************************************************************/
	DLLOPTION int EasiioAnswerCall(int callId, int callType);

	/* @function
	************************************************************************************
	函数名   : EasiioRejectCall
	功能     : 拒接来电
	参数     : [IN] callId    : 来电通话ID
	返回值   : 是否成功 0：成功； 非0失败
	回调函数 : void(*onCallStatusChanged)(int callId, int callState, const char* resultCode); // 通话状态变化通知
	************************************************************************************/
	DLLOPTION int EasiioRejectCall(int callId);

	/* @function
	************************************************************************************
	函数名   : EasiioHangupCall
	功能     : 挂断通话
	参数     : [IN] callId    : 通话ID
	返回值   : 是否成功 0：成功； 非0失败
	回调函数 : void(*onCallStatusChanged)(int callId, int callState, const char* resultCode); // 通话状态变化通知
	************************************************************************************/
	DLLOPTION int EasiioHangupCall(int callId);

	/* @function
	************************************************************************************
	函数名   : EasiioHoldCall
	功能     : 通话保持
	参数     : [IN] callId    : 通话ID
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioHoldCall(int callId);

	/* @function
	************************************************************************************
	函数名   : EasiioUnHoldCall
	功能     : 解除通话保持
	参数     : [IN] callId    : 通话ID
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioUnHoldCall(int callId);

	/* @function
	************************************************************************************
	函数名   : EasiioMuteCall
	功能     : 通话静音
	参数     : [IN] callId    : 通话ID
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioMuteCall(int callId);

	/* @function
	************************************************************************************
	函数名   : EasiioUnMuteCall
	功能     : 解除通话静音
	参数     : [IN] callId    : 通话ID
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioUnMuteCall(int callId);

	/* @function
	************************************************************************************
	函数名   : EasiioAdjustMicLevel
	功能     : 调整通话麦克风音量
	参数     : [IN] callId    : 通话ID
	           [IN] level     : 麦克风音量大小 0 - 40
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioAdjustMicLevel(int callId, int level);

	/* @function
	************************************************************************************
	函数名   : EasiioSendDTMF
	功能     : 发送DTMF
	参数     : [IN] callId    : 通话ID
	           [IN] dtmf      : DTMF值
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioSendDTMF(int callId, const char* dtmf);

	/* @function
	************************************************************************************
	函数名   : EasiioSwitchRecordInCall
	功能     : 通话中开始或停止录音
	参数     : [IN] recordSwitch : 开始或停止录音
	                RECORD_START, // 开始录音
		            RECORD_STOP   // 停止录音
	           [IN] callUUID     : 通话的UUID，可从状态变化获取
	返回值   : 是否成功 0：成功； 非0失败
	回调函数 : void(*onCallRecordingReaponse)(EasiioResponseReason reason, int switchRecord, const char* callUUID); // 通话中开启、停止录音调用结果
	************************************************************************************/
	DLLOPTION int EasiioSwitchRecordInCall(int recordSwitch, const char* callUUID);

	/* @function
	************************************************************************************
	函数名   : EasiioGetCurrentPBXAccount
	功能     : 获取当前PBX号码
	参数     : [OUT] pbxAccount    : 当前登录的PBX号码
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioGetCurrentPBXAccount(char* pbxAccount);

	/* @function
	************************************************************************************
	函数名   : EasiioGetAllRecord
	功能     : 获取所有录音列表目录
	参数     : [IN] recordType  : 录音类型
				RECORD_TYPE_FROM_CALL_START = 0, // 通话一开始就录音
				RECORD_TYPE_IN_CALL  // 通话中的录音
	返回值   : 是否成功 0：成功； 非0失败
	回调函数 : void(*onGetAllRecordResponse)(EasiioResponseReason reason, int count, EasiioRecord *record); //获取所有录音回调结果 
	************************************************************************************/
	DLLOPTION int EasiioGetAllRecord(int recordType);

	/* @function
	************************************************************************************
	函数名   : EasiioGetRecord
	功能     : 获取单次通话录音
	参数     : [IN] recordType  : 录音类型
				RECORD_TYPE_FROM_CALL_START = 0, // 通话一开始就录音
				RECORD_TYPE_IN_CALL  // 通话中的录音
			   [IN] callUUID     : 录音ID， 可以从获取所有录音里取出，也可以在通话结束后
			                       使用通话状态回调的callUUID
	返回值   : 是否成功 0：成功； 非0失败
	回调函数 : void(*onGetRecordResponse)(EasiioResponseReason reason, const char* callUUID, int count, EasiioRecordItem *recordItem); //获取单次通话录音回调结果
	************************************************************************************/
	DLLOPTION int EasiioGetRecord(int recordType, const char* callUUID);

	/* @function
	************************************************************************************
	函数名   : EasiioDestroy
	功能     : 销毁，第三方应用退出时调用，区别于EasiioLogout
	返回值   : 是否成功 0：成功； 非0失败
	************************************************************************************/
	DLLOPTION int EasiioDestroy();

	/* @function
	************************************************************************************
	函数名   : EasiioRefreshMic
	功能     : 刷新麦克风
	返回值   : 0      正常
	************************************************************************************/
	DLLOPTION int EasiioRefreshMic();

	/* @function
	************************************************************************************
	函数名   : EasiioSetLogDir
	功能     : 设置日志保存路径
	************************************************************************************/
	DLLOPTION void EasiioSetLogDir(const char* logDir);

	/* @function
	************************************************************************************
	函数名   : EasiioSetProxyUrl
	功能     : 设定代理地址
	************************************************************************************/
	DLLOPTION void EasiioSetProxyUrl(const char* proxuUrl);

	/* @function
	************************************************************************************
	函数名   : EasiioSetProxyInfo
	功能     : 设定代理参数
	************************************************************************************/
	DLLOPTION void EasiioSetProxyInfo(int type, const char* account, const char* password);

#ifdef __cplusplus 
}
#endif

#endif