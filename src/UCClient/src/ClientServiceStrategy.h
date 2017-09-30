/*
 * ClientServiceStrategy.h
 *
 *  Created on: July 27, 2015
 *      Author: gang wang
 */

#ifndef CLIENT_SERVICE_EVENT_LISTENER_H_
#define CLIENT_SERVICE_EVENT_LISTENER_H_

#include <string>

#include "IClientService.h"
#include "CefClient.h"

namespace ucclient {

using namespace uc;

class ClientServiceStrategy : public IClientServiceEvent {
public:
	ClientServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);

	void Login(const wstring &account, const wstring &password,int32_t accountType);

	void GetAccessToken(OauthGrantType grantType);

	void Relogin();

	void Logout();
	
	void ModifyPassword(const wstring &newPass, const wstring &confirmPass);

	bool SetProxyInfo(int32_t auth_type, const wstring& auth_account, const wstring& auth_passwd);

	void ConnectUCAS();

	void DisconnectUCAS();

	void ConnectUCC();

	void DisconnectUCC();

	void GetLocalConversations(int32_t number);

	void GetNewConversationList(int32_t number);

	void GetOldConversationList(int64_t endTime, int32_t number);

	int32_t GetConversationInfo(ConversationType type, int64_t relateId, ConversationInfo& info);

	int32_t SetConversationTop(ConversationType type, int64_t relateId, int32_t state);

	void SendHeartBeat();

	std::string GetDocumentDataDir(DocDirType type);

	void DeleteHistoryAccount(const std::wstring& account);

	void SetAutoLogin(const std::wstring& account, bool autoLogin);

	void RememberPassword(const std::wstring& account, bool remember);

	int32_t LoadHistoryAccounts(std::list<AccountInfo> &accounts);

	std::wstring GetCaptchaURL(const wstring &hashStr);

	void VerifyAccount(int32_t verifyType, const std::wstring &account, const std::wstring &captcha, const std::wstring &hashStr);

	void VerifyCode(int32_t verifyType, const std::wstring &account, const std::wstring &mobileCode, int32_t resend);

	void ResetPassword(const std::wstring &account, const std::wstring &newPwd, const std::wstring &mobileCode);

	int32_t GetPwdExpiredDays();
	
	const UCID& GetCurUCID() const;
	
	void ArchiveLogFile();

	int32_t GetMinGroupMemberLimit();

	int32_t GetMaxGroupMemberLimit();

	int32_t GetGroupCountLimit();

	void SetSendFilePath(const wstring& filepath) { _sendFilePath = filepath; }

	wstring GetSendFilePath() { return _sendFilePath; }
	
	int GetModuleFlag();

	virtual void SetUpdateUserNotify(int16_t isNotify);

	virtual void GetUserNotifyStatus();

	virtual void GetPasswordRule(OUT int32_t & days, OUT int32_t & memory, OUT int32_t & complex, OUT int32_t & allow_change);

	virtual int32_t GetConfigInfos(const std::wstring &app, std::map<wstring, wstring> &configs);

	virtual int32_t UpdateConfigInfos(const std::wstring &app, std::map<wstring, wstring> &configs);

	virtual int32_t GetConfigInfo(const std::wstring &app, const std::wstring &name, std::wstring &value);

	virtual int32_t UpdateConfigInfo(const std::wstring &app, const std::wstring &name, const std::wstring &value);

	virtual int32_t GetValueByKey(const std::wstring &app, const std::wstring &name, std::wstring &value);

	virtual int32_t UpdateValueBykey(const std::wstring &app, const std::wstring &name, const std::wstring &value);
	
	virtual void UploadLog(const std::wstring &logURL, const std::wstring &desc, const std::wstring &version);
	virtual std::wstring GetSessionId();

	virtual std::wstring GetUccServerDomain();

	virtual void DeleteSession(ConversationType type, int64_t relateId);


    virtual void OnLogin(int32_t errorCode, int32_t authType, int32_t cloudDiskType, const std::string &capability = "");

	virtual void OnAccessTokenReceived(int32_t errorCode, OauthGrantType grantType, const std::string &accessToken);


	virtual void OnLogout(int32_t errorCode);

	virtual void OnPasswordModified(int32_t errorCode);

	virtual void OnPasswordModifyMessageReceived();

	virtual void OnPasswordRuleUpdateMessageReceived(int32_t type);

	virtual void OnPasswordExpiredMessageReceived(int32_t dueDays);

	virtual void OnUCASConnected(int32_t errorCode);

	virtual void OnUCASDisconnected(int32_t errorCode);

	virtual void OnConversationListReceived(int32_t errorCode, const std::list<ConversationInfo*> *conversations, bool isLocal, int32_t totalUnread, std::map<int64_t, int32_t>& groupAtCounts);

	virtual void OnConversationListComplete(int32_t conversationSize);

	virtual void OnConversationTopSet(int32_t errorCode, ConversationType type, int64_t relateId, int32_t state, int64_t index);

	virtual void OnUserLoginNotify(int32_t errorCode, int64_t userId, DeviceType::type deviceType, std::string deviceName);

	virtual void OnUpdateUserNotifySet(int32_t errorCode, int64_t userId);

	virtual void OnGetUserNotifyStatus(int32_t errorCode, int16_t isNotify);

	virtual void OnAccountVerified(int32_t errorCode, int32_t verifyType, const std::string &mobile, const std::string &email, int32_t exprised);

	virtual void OnCodeVerified(int32_t errorCode, int32_t exprised);

	virtual void OnResendCodeComplete(int32_t errorCode, int32_t exprised);

	virtual void OnPasswordReset(int32_t errorCode);

	virtual void OnLogUploaded(int32_t errorCode);

	virtual void OnSessionExpired();

	virtual void OnArchiveLogFinish(std::string filePath);

	virtual std::wstring GetDefaultFileSavePath(std::wstring filename);

protected:
	void CheckLocalMeetingClientVersion();

private:
	cefIntegration::CefClient *_cefClient;
	IClientService *_clientService;
	IUCEngine *_engine;
	bool _isFirstLogin;
	wstring _sendFilePath;
};

}

#endif