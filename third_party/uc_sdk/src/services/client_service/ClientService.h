/*
 * ClientService.h
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#ifndef CLIENTSERVICE_H_
#define CLIENTSERVICE_H_

#include "IClientService.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "LoginInfo.h"
#include "HTTPResponse.h"
#include "store_service/StoreService.h"
#include "Timer.h"
#include "PasswordRule.h"
#include "ProxyInfo.h"

#if defined(WIN32) || defined(WIN64)
#include "IBee2MeetingPlugin.h"
#endif

namespace uc {

#if defined(WIN32) || defined(WIN64)
class Bee2MeetingPluginSink : public IBee2MeetingPluginSink {
	virtual void NotifyUpdateResult(int Result);
};
#endif
    
class ClientService: public IClientService, IUCEngineEvent {
public:
	ClientService(IUCEngine *engine);
	virtual ~ClientService();

public:
	virtual int32_t Login(uint16_t clientType, const string &account, const string &password, const string &clientId,
			const string &macAddress, const string &pinNumber, const string &version,
			const string &storeType, const string &phoneNumber, const int32_t & accountType);
	virtual int32_t Relogin(bool fromReConnect);
	virtual void GetAccessToken(OauthGrantType grantType);
	virtual void Logout();
	virtual bool SetProxyUrl(const std::string& proxyUrl);
	virtual bool SetProxyInfo(int32_t auth_type, const std::string& auth_account, const std::string& auth_passwd);
	virtual bool GetProxyInfo(std::string& auth_account, std::string& auth_passwd, int &auth_type);
	virtual void ModifyPassword(const string &oldPassword, const string &newPassword);
	virtual void ConnectUCAS();
	virtual void DisconnectUCAS();
	virtual void ConnectUCC();
	virtual void DisconnectUCC();
	virtual void GetLocalConversations(int32_t number);
	virtual void GetNewConversationList(int32_t number);
	virtual void GetOldConversationList(int64_t endTime, int32_t number);
	virtual int32_t GetConversationInfo(ConversationType type, int64_t relateId, OUT ConversationInfo& info);
	virtual int32_t SetConversationTop(ConversationType type, int64_t relateId, int32_t state);
	virtual int32_t GetLoginUserId();
	virtual void SendHeartBeat();
	virtual int32_t LoadHistoryAccounts(std::list<AccountInfo> &accounts);
	virtual std::string GetDocumentDataDir(DocDirType type);
	virtual int32_t DeleteHistoryAccount(const std::string& account);
	virtual int32_t RememberPassword(const std::string& account, bool remember);
	virtual int32_t SetAutoLogin(const std::string& account, bool autoLogin);
	virtual std::string GetCaptchaURL(const string &hashStr);
	virtual void VerifyAccount(int32_t verifyType, uint16_t clientType, const std::string &account, const std::string &captcha, const std::string &hashStr);
	virtual void VerifyCode(int32_t verifyType, uint16_t clientType, const std::string &account, const std::string &mobileCode, int32_t resend);
	virtual void ResetPassword(uint16_t clientType, const std::string &account, const std::string &newPwd,
				const std::string &confirmPwd, const std::string &mobileCode);
	virtual const UCID& GetCurUCID() const;
	virtual int32_t GetConfigInfos(const std::string &app, std::map<string, string> &configs);
	virtual int32_t UpdateConfigInfos(const std::string &app, std::map<string, string> &configs);
	virtual int32_t GetConfigInfo(const std::string &app, const std::string &name, std::string &value);
	virtual int32_t UpdateConfigInfo(const std::string &app, const std::string &name, const std::string &value);
	virtual int32_t GetValueByKey(const std::string &app, const std::string &name, std::string &value);
	virtual int32_t UpdateValueBykey(const std::string &app, const std::string &name, const std::string &value);
	virtual void UploadLog(const std::string &logURL, const std::string &desc, const std::string &version);
	virtual int32_t GetPwdExpiredDays();
	virtual std::string GetSessionId();
	virtual void ArchiveLogFile();
	virtual void GetUpgradeInfo(std::string &upgradeServer, std::string &packageInfo, std::string &packageDownload);
	virtual void GetPasswordRule(OUT int32_t & days, OUT int32_t & memory, OUT int32_t & complex, OUT int32_t &allow_change);
	virtual void EnableCurlSettings(int value);
	virtual void GetUccServerDomain(std::string &uccServerDomain);
	virtual std::wstring GetDefaultFileSavePath(std::string filename);
	virtual int32_t GetMinGroupMemberLimit();
	virtual int32_t GetMaxGroupMemberLimit();
	virtual int32_t GetGroupCountLimit();
	virtual int GetModuleFlag();

	virtual void SetUpdateUserNotify(int16_t isNotify);
	virtual void GetUserNotifyStatus();

	// IUCEngine Event
	virtual int32_t OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state);
	virtual void OnUCASConnected(int32_t errorCode);
	virtual void OnUCASDisconnected(int32_t errorCode);
	//设置和获取本地云会议版本-1--未安装云会议， 0--安装60000版本， 1--安装了60001版本， 2--同时安装了60000和60001版本
	virtual void SetLocalMeetingClientVersion(int version) {
		_localMeetingClientVersion = version;
	}
	virtual int32_t GetLocalMeetingClientVersion() {
		return _localMeetingClientVersion;
	}

	virtual void DeleteSession(ConversationType type, int64_t relateId);

public:
	const LoginInfo& GetLoginInfo() const;

	const int GetCurlSettings() const;

	const ProxyInfo& GetProxyInfo() const;

protected:
	void Lookup();
	int32_t OnLookupComplete(int32_t errorCode, HTTPResponse *response);

	void Login();
	int32_t OnLoginComplete(int32_t errorCode, HTTPResponse *response);
	int32_t OnAccessTokenReceived(int32_t errorCode, HTTPResponse *response, OauthGrantType grantType);
	int32_t OnLogoutComplete(int32_t errorCode, HTTPResponse *response);
	int32_t OnResetPasswordComplete(int32_t errorCode, HTTPResponse *response);
	int32_t OnModifyPasswordComplete(int32_t errorCode, HTTPResponse *response);

	void ListConversation(int64_t startTime, int64_t endTime, int32_t number, int32_t topState);
	int32_t OnListConversationComplete(int32_t errorCode, HTTPResponse *response, int32_t number);
	int32_t OnVerifyAccountComplete(int32_t errorCode, HTTPResponse *response, int32_t verifyType);
	int32_t OnVerifyCodeComplete(int32_t errorCode, HTTPResponse *response, int32_t resend);
	int32_t OnSetTopConversationComplete(int32_t errorCode, HTTPResponse *response, ConversationType type, int64_t relateId, int32_t state);
	int32_t OnSendHeartBeatComplete(int32_t errorCode, HTTPResponse *response);

	int32_t OnUploadLogComplete(int32_t errorCode, HTTPResponse *response, std::string logFile);
	int32_t OnUpdateUserNotifyComplete(int32_t errorCode, HTTPResponse *response);
	int32_t OnGetUserNotifyStatusComplete(int32_t errorCode, HTTPResponse *response);

	void SessionTopStateNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, int32_t state);
	void UserLoginNotity(UCMsg& ucMsg);

	bool InitUserDB();
	void InitPresence(); // 连接上UCCAS后，发送online 给UCC
	int32_t OnSendMessageResponse(int32_t errorCode, HTTPResponse *response);

	bool GetUserStatus(int32_t userIds);

	std::string GetFileName(std::string filepath, std::string filename, std::string tempfilename, int32_t namecount = 1);
	bool FindFile(std::string filepath,std::string filename);
	int32_t UpdateMeetingClient();
private:
	void _ArchiveLogFile();
    void MakeCapabilityData(CapabilityInfo &cpbltInfo, std::string &capability);
	void _RefreshToken(int expiresIn);
	void _CancelTimer();

private:
	LoginInfo _loginInfo;
	int64_t _lastSendTime; // 保存
	int64_t _lastRecvTime;
	int64_t _heartbeat_timerId;
	PasswordRule _passwordRule;
	ProxyInfo   _proxyInfo;
	bool	_isLoginning; 

	int _libcurlSettings;

	int32_t _maxGroupMemberLimit;  //大讨论组成员个数上限;
	int32_t _groupCountLimit;         //讨论组个数上限
	int32_t _minGroupMemberLimit;     //小讨论组成员个数上限
	int _moduleFlag;
	int _localMeetingClientVersion;		//-1--未安装云会议， 0--安装60000版本， 1--安装了60001版本， 2--同时安装了60000和60001版本
    timer_id _timerId;

	bool _isFromReConnect; // 用来标记是从login界面登录还是从ReConnect登录
#if defined(WIN32) || defined(WIN64)
	Bee2MeetingPluginSink _bee2MeetingPluginSink;
#endif
};
} /* namespace uc */
#endif /* CLIENTSERVICE_H_ */
