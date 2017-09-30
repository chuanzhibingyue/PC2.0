
#include "ClientServiceStrategy.h"
#include "utils/NetworkAdapterUtil.h"
#include "ClientDefs.h"
#include "UCDefs.h"
#include "LogManager.h"
#include <sstream>
#include "ServiceObjectConversion.h"
#include "UCManager.h"
#include "Shlobj.h"
namespace ucclient {

ClientServiceStrategy::ClientServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient)
	: _engine(engine), _cefClient(cefClient), _isFirstLogin(false){
	_clientService = (IClientService*)_engine->QueryService(UCClientService);
}

void ClientServiceStrategy::Login(const wstring &account, const wstring &password, int32_t accountType) {

	CheckLocalMeetingClientVersion();

	std::wstring mac;
	NetworkAdapterUtil::GetEthernetMAC(mac);

	std::string utf8Account = StringConvert::ToUTF8Str(account);
	std::string utf8Password = StringConvert::ToUTF8Str(password);
	std::string utf8Mac = StringConvert::ToUTF8Str(mac);

	utf8Mac += "_1";
	_clientService->Login(UCCLIENT_WIN, utf8Account, utf8Password, "", utf8Mac, "",
		StringConvert::ToUTF8Str(UCManager::GetInstance().GetClientVersion()), "", "", accountType);
}

void ClientServiceStrategy::GetAccessToken(OauthGrantType grantType) {
	_clientService->GetAccessToken(grantType);
}

void ClientServiceStrategy::Relogin() {
	_clientService->Relogin(true);
}

bool ClientServiceStrategy::SetProxyInfo(int32_t auth_type, const wstring& auth_account, const wstring& auth_passwd)
{
	std::string utf8Account = StringConvert::ToUTF8Str(auth_account);
	std::string utf8Password = StringConvert::ToUTF8Str(auth_passwd);
	_cefClient->SetAuthInfo(auth_account, auth_passwd);
	return _clientService->SetProxyInfo(auth_type, utf8Account, utf8Password);
}

void ClientServiceStrategy::Logout() {
	_clientService->Logout();
}

void ClientServiceStrategy::ModifyPassword(const wstring &oldPassword, const wstring &newPassword) {
	std::string utf8OldPwd = StringConvert::ToUTF8Str(oldPassword);
	std::string utf8NewPwd = StringConvert::ToUTF8Str(newPassword);

	_clientService->ModifyPassword(utf8OldPwd, utf8NewPwd);
}

void ClientServiceStrategy::ConnectUCAS() {
	_clientService->ConnectUCAS();
}

void ClientServiceStrategy::DisconnectUCAS() {
	_clientService->DisconnectUCAS();
}

void ClientServiceStrategy::ConnectUCC() {
	_clientService->ConnectUCC();
}

void ClientServiceStrategy::DisconnectUCC() {
	_clientService->DisconnectUCC();
}


void ClientServiceStrategy::GetLocalConversations(int32_t number) {
	_clientService->GetLocalConversations(number);
}

void ClientServiceStrategy::GetNewConversationList(int32_t number) {
	_clientService->GetNewConversationList(number);
}

void ClientServiceStrategy::GetOldConversationList(int64_t endTime, int32_t number) {
	_clientService->GetOldConversationList(endTime, number);
}

int32_t ClientServiceStrategy::GetConversationInfo(ConversationType type, int64_t relateId, ConversationInfo& info){

	return _clientService->GetConversationInfo(type, relateId, info);
}

int32_t ClientServiceStrategy::SetConversationTop(ConversationType type, int64_t relateId, int32_t state){

	return _clientService->SetConversationTop(type, relateId, state);
}

void ClientServiceStrategy::SendHeartBeat() {
	_clientService->SendHeartBeat();
}

std::string ClientServiceStrategy::GetDocumentDataDir(DocDirType type) {
	return _clientService->GetDocumentDataDir(type);
}

void ClientServiceStrategy::DeleteHistoryAccount(const std::wstring& account){
	std::string utf8Account = StringConvert::ToUTF8Str(account);
	_clientService->DeleteHistoryAccount(utf8Account);
}

void ClientServiceStrategy::SetAutoLogin(const std::wstring& account, bool autoLogin){
	std::string utf8Account = StringConvert::ToUTF8Str(account);
	_clientService->SetAutoLogin(utf8Account, autoLogin);
}

void ClientServiceStrategy::RememberPassword(const std::wstring& account, bool remember){
	std::string utf8Account = StringConvert::ToUTF8Str(account);
	_clientService->RememberPassword(utf8Account, remember);
}

int32_t ClientServiceStrategy::LoadHistoryAccounts(std::list<AccountInfo> &accounts){
	return _clientService->LoadHistoryAccounts(accounts);
}

std::wstring ClientServiceStrategy::GetCaptchaURL(const wstring &hashStr) {
	std::string utf8URL = "";
	std::string utf8HashStr = StringConvert::ToUTF8Str(hashStr);

	utf8URL = _clientService->GetCaptchaURL(utf8HashStr);
	return StringConvert::FromUTF8Str(utf8URL);
}

void ClientServiceStrategy::VerifyAccount(int32_t verifyType, const std::wstring &account, const std::wstring &captcha, const std::wstring &hashStr) {
	std::string utf8Account = StringConvert::ToUTF8Str(account);
	std::string utf8Captcha = StringConvert::ToUTF8Str(captcha);
	std::string utf8HashStr = StringConvert::ToUTF8Str(hashStr);

	_clientService->VerifyAccount(verifyType, UCCLIENT_WIN, utf8Account, utf8Captcha, utf8HashStr);
}

void ClientServiceStrategy::VerifyCode(int32_t verifyType, const std::wstring &account, const std::wstring &mobileCode, int32_t resend) {
	std::string utf8Account = StringConvert::ToUTF8Str(account);
	std::string utf8MobileCode = StringConvert::ToUTF8Str(mobileCode);

	_clientService->VerifyCode(verifyType, UCCLIENT_WIN, utf8Account, utf8MobileCode, resend);
}

void ClientServiceStrategy::ResetPassword(const std::wstring &account, const std::wstring &newPwd, const std::wstring &mobileCode) {
	std::string utf8Account = StringConvert::ToUTF8Str(account);
	std::string utf8NewPwd = StringConvert::ToUTF8Str(newPwd);
	std::string utf8MobileCode = StringConvert::ToUTF8Str(mobileCode);

	_clientService->ResetPassword(UCCLIENT_WIN, utf8Account, utf8NewPwd, utf8NewPwd, utf8MobileCode);
}

const UCID& ClientServiceStrategy::GetCurUCID() const {
	return _clientService->GetCurUCID();
}

void ClientServiceStrategy::UploadLog(const std::wstring &logURL, const std::wstring &desc, const std::wstring &version) {
	std::string utf8logURL = StringConvert::ToUTF8Str(logURL);
	std::string utf8Desc = StringConvert::ToUTF8Str(desc);
	std::string utf8Version = StringConvert::ToUTF8Str(version);

	_clientService->UploadLog(utf8logURL, utf8Desc, utf8Version);
}

void ClientServiceStrategy::OnLogin(int32_t errorCode, int32_t authType, int32_t cloudDiskType, const std::string &capability) {

	if (errorCode == 407 && !_isFirstLogin){
		std::wstring proxyAccount;
		std::wstring proxyPassword;
		_cefClient->GetAuthInfo(proxyAccount, proxyPassword);
		if (!proxyAccount.empty() && !proxyPassword.empty()){
			_clientService->SetProxyInfo(authType, StringConvert::ToUTF8Str(proxyAccount), StringConvert::ToUTF8Str(proxyPassword));
			UCManager::GetInstance().GetGkClient()->SetProxyAuthInfo(authType, proxyAccount, proxyPassword);
			_clientService->Relogin(false);
			_isFirstLogin = true;
            return;
		}
	}

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_LOGIN_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", authType: " << authType
        << ", pbxType: " << cloudDiskType << ", capability: \"" << UTF2WJSON(capability) << "\"},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	//update check after login success
	if (errorCode == 0) {
		UCManager::GetInstance().GetUpdateServiceStrategy()->CheckUpdates(0);

		wstring sendFilePath = GetSendFilePath();
		StringConvert::Replace(sendFilePath, L"\\", L"\\\\");
		if (!sendFilePath.empty()) {
			UC_LOG(WARNING) << "Send file by bee, file path:" << sendFilePath;
			std::wstringstream jsScript;
			jsScript << L"var e = new CustomEvent(\"" << SEND_FILE_TO_BEE_EVENT
				<< "\",{detail:" << "{filePath:\"" << sendFilePath
				<< "\"},});" << "document.dispatchEvent(e);";
			_cefClient->ExecuteJSForAllBrowsers(jsScript.str());
			SetSendFilePath(L"");
		}
	}

	UC_LOG(INFO) << "dispatch OnLogin event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnAccessTokenReceived(int32_t errorCode, OauthGrantType grantType, const std::string &accessToken) {
}

void ClientServiceStrategy::OnLogout(int32_t errorCode) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_LOGOUT_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnLogout event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnPasswordModified(int32_t errorCode) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_PASSWORD_MODIFIED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnPasswordReset event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnPasswordModifyMessageReceived() {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_PASSWORD_MODIFY_MESSAGE_RECEIVED_EVENT
		<< "\",{detail:{},});" << "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnPasswordModifyMessageReceived event to JS";
}

void ClientServiceStrategy::OnPasswordRuleUpdateMessageReceived(int32_t type) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_PASSWORD_RULE_UPDATE_MESSAGE_RECEIVED_EVENT
		<< "\",{detail:" << "{type: " << type << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnPasswordRuleUpdateMessageReceived event to JS, type: " << type;
}

void ClientServiceStrategy::OnPasswordExpiredMessageReceived(int32_t dueDays) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_PASSWORD_EXPIRED_MESSAGE_RECEIVED_EVENT
		<< "\",{detail:" << "{dueDays: " << dueDays << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnPasswordExpiredMessageReceived event to JS, dueDays: " << dueDays;
}

void ClientServiceStrategy::OnUCASConnected(int32_t errorCode) {

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_UCAS_CONNECTED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUCASConnected event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnUCASDisconnected(int32_t errorCode) {
	std::wstringstream jsScript;
	if (errorCode == 0)
		return;

	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_UCAS_DISCONNECTED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUCASDisconnected event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnConversationListReceived(int32_t errorCode, const std::list<ConversationInfo*> *conversations,
	bool isLocal, int32_t totalUnRead, std::map<int64_t, int32_t>& groupAtCounts) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_CONVERSATION_LIST_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", isLocal: " << isLocal << ", totalUnread: " << totalUnRead
		<< ", groupAtCounts: [";
	std::map<int64_t, int32_t>::const_iterator iter = groupAtCounts.begin();
	while (iter != groupAtCounts.end()) {
		jsScript << "{" << iter->first << ":" << iter->second << "},";
		iter++;
	}
	jsScript << "], conversations:[";

	if (conversations != NULL) {
		std::list<ConversationInfo*>::const_iterator iter = conversations->begin();
		for (; iter != conversations->end(); ++iter) {

			//关联conversion与group中@人数
			int32_t conversionAtCounts = 0;
			int64_t releatedId = (*iter)->GetRelateId();
			std::map<int64_t, int32_t>::const_iterator iter_map = groupAtCounts.begin();
			while (iter_map != groupAtCounts.end()) {
				if (iter_map->first == releatedId) {
					conversionAtCounts = iter_map->second;
					groupAtCounts.erase(iter_map);
					break;
				}
				iter_map ++;
			}

			jsScript << "{type:" << (*iter)->GetType() << ","
				<< "relateID:" << releatedId << ","
				<< "conversation:" << (*iter)->GetConversation() << ","
				<< "newMsgCount:" << (*iter)->GetNewMsgCount() << ","
				<< "lastMsgTime:" << (*iter)->GetLastMsgTime() << ","
				<< "lastMsgSeq:" << (*iter)->GetLastMsgSeq() << ","
				<< "topIndex:" << (*iter)->GetTopIndex() << ","
				<< "eventId:" << (*iter)->GetEventId() << ","
				<< "conferenceId:" << (*iter)->GetConferenceId() << ","
				<< "authorizerId:" << (*iter)->GetAuthorizerId() << ","
				<< "hidden:" << (*iter)->isHidden() << ","
				<< "atCounts:" << conversionAtCounts << "},";
		}
	}

	jsScript << "]},});" << "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnConversationListReceived event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnConversationListComplete(int32_t conversationSize)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_CONVERSATION_LIST_COMPLETE_EVENT
		<< "\",{detail:" << "{conversationSize: " << conversationSize << "},});" << "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnConversationListComplete event to JS, conversationSize" << conversationSize;
}

void ClientServiceStrategy::OnAccountVerified(int32_t errorCode, int32_t verifyType, const std::string &mobile, const std::string &email, int32_t exprised) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_ACCOUNT_VERIFIED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", " 
		<< "verifyType:" << verifyType << ", "
		<< "mobile: \"" << UTF2WJSON(mobile) << "\", "
		<< "email: \"" << UTF2WJSON(email) << "\", "
		<< "exprised: " << exprised << ","
		<< "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAccountVerified event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnConversationTopSet(int32_t errorCode, ConversationType type, int64_t relateId, int32_t state, int64_t index){
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_CONVERSATION_TOP_SET_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", "
		<< "message:{" 
		<< "conversationType:" << type << ", "
		<< "relateId:" << relateId << ", "
		<< "state:" << state << ", "
		<< "index:" << index << ", }"
		<< "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnConversationTopSet event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnUserLoginNotify(int32_t errorCode, int64_t userId, DeviceType::type deviceType, std::string deviceName){
	std::wstringstream jsScript;

 	jsScript << L"var e = new CustomEvent(\"" << ClIENT_SERVICE_USER_LOGIN_NOTIFY_EVENT
 		<< "\",{detail:" << "{errorCode: " << errorCode << ", "
 		<< "message:{"
 		<< "userId:" << userId << ", "
 		<< "deviceType:" << deviceType << ", "
		<< "deviceName:\"" << UTF2WJSON(deviceName) << "\", }"
		<< "},});"
 		<< "document.dispatchEvent(e);";
 
 	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUserLoginNotify event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnUpdateUserNotifySet(int32_t errorCode, int64_t userId)
{
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << UPDATE_USER_NOTIFY_SET_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", "
		<< "userId:" << userId << ", "
		<< "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUpdateUserNotifySet event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnGetUserNotifyStatus(int32_t errorCode, int16_t isNotify)
{
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << USER_NOTIFY_STATUS_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", "
		<< "isNotify:" << isNotify << ", "
		<< "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGetUserNotifyStatus event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnCodeVerified(int32_t errorCode, int32_t exprised) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_CODE_VERIFIED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ","
		<< "exprised:" << exprised << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCodeVerified event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnResendCodeComplete(int32_t errorCode, int32_t exprised){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << ClIENT_SERVICE_CODE_RESEND_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ","
		<< "exprised:" << exprised << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnResendCodeComplete event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnPasswordReset(int32_t errorCode) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_PASSWORD_RESET_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnPasswordReset event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnLogUploaded(int32_t errorCode) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_UPLOAD_LOG_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnLogUploaded event to JS, errorCode: " << errorCode;
}

void ClientServiceStrategy::OnSessionExpired() {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_SESSION_EXPIRED_EVENT
		<< "\",{detail:" << "{},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnSessionExpired event to JS";
}

wstring Ascii2Unicode(string astrsrc)
{
	int nLength = ::MultiByteToWideChar(CP_ACP, 0, astrsrc.c_str(), -1, NULL, 0);
	if (nLength <= 0) return wstring(L"");
	wchar_t *szbuffer = new wchar_t[nLength + 2];
	::MultiByteToWideChar(CP_ACP, 0, astrsrc.c_str(), -1, szbuffer, nLength);
	wstring strnew = szbuffer;
	delete[] szbuffer;
	return strnew;
}

void ClientServiceStrategy::OnArchiveLogFinish(std::string filePath)
{
	std::wstringstream jsScript;


	jsScript << L"var e = new CustomEvent(\"" << CLIENT_SERVICE_ARCHIVELOG_FINISH_EVENT
		<< "\",{detail:" << "{filePath: \"" << Ascii2Unicode(StringConvert::JsonEscape(filePath))
		<< "\"},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnArchiveLogFinish event to JS";
}

int32_t ClientServiceStrategy::GetConfigInfos(const std::wstring &app, std::map<wstring, wstring> &configs) {
	std::string utf8App = StringConvert::ToUTF8Str(app);
	std::map<string, string> utf8Configs;

	int32_t result = _clientService->GetConfigInfos(utf8App, utf8Configs);

	std::map<string, string>::const_iterator itor;

	itor = utf8Configs.begin();
	while (itor != utf8Configs.end()) {
		configs.insert(pair<wstring, wstring>(StringConvert::FromUTF8Str(itor->first), StringConvert::FromUTF8Str(itor->second)));
		itor++;
	}

	return result;
}

int32_t ClientServiceStrategy::UpdateConfigInfos(const std::wstring &app, std::map<wstring, wstring> &configs) {
	std::string utf8App = StringConvert::ToUTF8Str(app);
	std::map<string, string> utf8configs;
	std::map<wstring, wstring>::const_iterator itor;

	itor = configs.begin();
	while (itor != configs.end()) {
		utf8configs.insert(pair<string, string>(StringConvert::ToUTF8Str(itor->first), StringConvert::ToUTF8Str(itor->second)));
		itor++;
	}

	return _clientService->UpdateConfigInfos(utf8App, utf8configs);
}

int32_t ClientServiceStrategy::GetConfigInfo(const std::wstring &app, const std::wstring &name, std::wstring &value) {
	std::string utf8App = StringConvert::ToUTF8Str(app);
	std::string utf8Name = StringConvert::ToUTF8Str(name);
	std::string utf8Value = "";

	int32_t result = _clientService->GetConfigInfo(utf8App, utf8Name, utf8Value);
	value = StringConvert::FromUTF8Str(utf8Value);
	return result;
}

int32_t ClientServiceStrategy::UpdateConfigInfo(const std::wstring &app, const std::wstring &name, const std::wstring &value) {
	std::string utf8App = StringConvert::ToUTF8Str(app);
	std::string utf8Name = StringConvert::ToUTF8Str(name);
	std::string utf8Value = StringConvert::ToUTF8Str(value);

	return _clientService->UpdateConfigInfo(utf8App, utf8Name, utf8Value);
}

int32_t ClientServiceStrategy::GetValueByKey(const std::wstring &app, const std::wstring &name, std::wstring &value) {
	std::string utf8App = StringConvert::ToUTF8Str(app);
	std::string utf8Name = StringConvert::ToUTF8Str(name);
	std::string utf8Value = "";

	int32_t result = _clientService->GetValueByKey(utf8App, utf8Name, utf8Value);
	value = StringConvert::FromUTF8Str(utf8Value);
	return result;
}

int32_t ClientServiceStrategy::UpdateValueBykey(const std::wstring &app, const std::wstring &name, const std::wstring &value) {
	std::string utf8App = StringConvert::ToUTF8Str(app);
	std::string utf8Name = StringConvert::ToUTF8Str(name);
	std::string utf8Value = StringConvert::ToUTF8Str(value);

	return _clientService->UpdateValueBykey(utf8App, utf8Name, utf8Value);
}

int32_t ClientServiceStrategy::GetPwdExpiredDays() {
	return _clientService->GetPwdExpiredDays();
}

std::wstring ClientServiceStrategy::GetSessionId() {
	return StringConvert::FromUTF8Str(_clientService->GetSessionId());
}

std::wstring ClientServiceStrategy::GetUccServerDomain()
{
	std::string uccDomain;
	_clientService->GetUccServerDomain(uccDomain);
	return StringConvert::FromUTF8Str(uccDomain);
}

void ClientServiceStrategy::DeleteSession(ConversationType type, int64_t relateId)
{
	_clientService->DeleteSession(type, relateId);
}

void ClientServiceStrategy::ArchiveLogFile() {

	_clientService->ArchiveLogFile();
}

void ClientServiceStrategy::GetPasswordRule(OUT int32_t & days, OUT int32_t & memory, OUT int32_t & complex, OUT int32_t & allow_change)
{
	_clientService->GetPasswordRule(days, memory, complex, allow_change);
}

std::wstring ClientServiceStrategy::GetDefaultFileSavePath(std::wstring filename)
{
	std::string utf8filename = StringConvert::ToUTF8Str(filename);
	return _clientService->GetDefaultFileSavePath(utf8filename);
}

int32_t ClientServiceStrategy::GetMinGroupMemberLimit(){
	return _clientService->GetMinGroupMemberLimit();
}

int32_t ClientServiceStrategy::GetMaxGroupMemberLimit(){
	return _clientService->GetMaxGroupMemberLimit();
}

int32_t ClientServiceStrategy::GetGroupCountLimit(){
	return _clientService->GetGroupCountLimit();
}

int ClientServiceStrategy::GetModuleFlag(){
	return _clientService->GetModuleFlag();
}

void ClientServiceStrategy::SetUpdateUserNotify(int16_t isNotify){
	_clientService->SetUpdateUserNotify(isNotify);
}

void ClientServiceStrategy::GetUserNotifyStatus(){
	_clientService->GetUserNotifyStatus();
}

void ClientServiceStrategy::CheckLocalMeetingClientVersion() {
	//判断本地云会议安装包版本
	int localMeetingVersion = 0;
	WCHAR profile_path[MAX_PATH] = { 0 };
	::SHGetSpecialFolderPath(0, profile_path, CSIDL_PROFILE, false);
	WCHAR temp_path[MAX_PATH] = { 0 };
	wcscpy_s(temp_path, MAX_PATH, profile_path);
	wcscat_s(profile_path, MAX_PATH, L"\\Quanshi\\Quanshi Meeting\\6\\60000\\TangClient.exe");
	wcscat_s(temp_path, MAX_PATH, L"\\Quanshi\\Quanshi Meeting\\6\\60001\\TangClient.exe");
	int res0 = _waccess(profile_path, 0);
	int res1 = _waccess(temp_path, 0);
	if (res0 != -1 && res1 != -1){
		localMeetingVersion = 2;
	}else if (res0 != -1) {
		localMeetingVersion = 0;
	}else if (res1 != -1) {
		localMeetingVersion = 1;
	} else {
		localMeetingVersion = -1;
	}

	_clientService->SetLocalMeetingClientVersion(localMeetingVersion);
}

}