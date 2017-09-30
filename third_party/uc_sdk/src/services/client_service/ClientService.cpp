/*
 * ClientService.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#include "ClientService.h"

#include <iostream>
#include <sstream>

#if defined(WIN32) || defined(WIN64)
    #include <io.h>
#elif defined(__APPLE__)
    #include "sys/uio.h"
#else
    #include "sys/io.h"
#endif

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/filesystem.hpp>

#include "UCEngine.h"
#include "network/NetworkManager.h"
#include "HTTPRequestCallback.h"
#include "common/StringConvert.h"
#include "contact_service/ContactService.h"
#include "message_service/GroupService.h"
#include "message_service/IMService.h"
#include "message_service/MessageProcessor.h"
#include "LogManager.h"
#include "DBManager.h"
#include "UCErrorCode.h"
#include "CommonDef.h"
#include "ConversationInfoInternal.h"
#include "store_service/StoreService.h"
#include "UCTime.h"
#include "rapidjson/document.h"
#include "HTTPResponseAssist.h"
#include "common/ZipWrapper.h"
#include "utils/UCTime.h"
#include "CrypWrapper.h"
#include "rapidjson/document.h"
#include "jsonhelper.h"
#include "utils/TaskExecutor.h"

#ifdef PBX_SERVICE
#include "pbx_service/PbxService.h"
#endif // PBX_SERVICE

#define FLAG_FORMAL	"bee.quanshi.com"

namespace uc {

using namespace std;

ClientService::ClientService(IUCEngine *engine) {
	_engine = engine;
	_engine->registerListener(this);
	_lastSendTime = 0;
	_lastRecvTime = 0;
	_heartbeat_timerId = 0;
	_isLoginning = false;
	_libcurlSettings = 0;
	_groupCountLimit = 500;
	_minGroupMemberLimit = 200;
	_maxGroupMemberLimit = 500;
	_moduleFlag = 0;
	_localMeetingClientVersion = 0;
	_timerId = -1;
}

ClientService::~ClientService() {
	_engine->removeListener(this);
}

int32_t ClientService::Login(uint16_t clientType, const string &account, const string &password, const string &clientId,
		const string &macAddress, const string &pinNumber, const string &version,
		const string &storeType, const string &phoneNumber, const int32_t & accountType) {

	if (_isLoginning) {
		return UC_LOGINNING;
	}


	boost::property_tree::wptree ptRoot;
	wstringstream wsStream;
	wstring clientInfo = L"";

	ptRoot.put(L"lang", L"zh_Hans");
	ptRoot.put(L"mac", StringConvert::FromUTF8Str(macAddress));
	ptRoot.put(L"client_id", StringConvert::FromUTF8Str(clientId));
	ptRoot.put(L"pin_number", StringConvert::FromUTF8Str(pinNumber));
	ptRoot.put(L"version", StringConvert::FromUTF8Str(version));
	ptRoot.put(L"store_type", StringConvert::FromUTF8Str(storeType));
	ptRoot.put(L"phone_number", StringConvert::FromUTF8Str(phoneNumber));
	ptRoot.put(L"device_name", L"PC");

	boost::property_tree::write_json(wsStream, ptRoot);
	clientInfo = wsStream.str();

	_loginInfo.SetAccount(account);
	_loginInfo.SetPassword(password);
	_loginInfo.SetClientInfo(StringConvert::ToUTF8Str(clientInfo));
	_loginInfo.SetClientType(clientType);
	_loginInfo.SetAccountType(accountType);

	return Relogin(false);
}

int32_t ClientService::Relogin(bool fromReConnect) {
	if (_isLoginning) {
		return UC_LOGINNING;
	}

	_isLoginning = true;
	_isFromReConnect = fromReConnect;

	Lookup();

	return UC_SUCCEEDED;
}

void ClientService::GetAccessToken(OauthGrantType grantType) {
	HTTPRequest		request;
	string			url;
	stringstream	sStream;
	
	if (OGT_PASSWORD == grantType) {
		url = UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), AUTHORIZE_METHOD);
		sStream << "user_account=" << _loginInfo.GetAccount() << "&password=" << StringConvert::URLEncode(CrypWrapper::Encrypt(_loginInfo.GetPassword(), _loginInfo.GetAccount()))
			<< "&grant_type=password" << "&encrypt=" << 1;
	}
	else if (OGT_REFRESH_TOKEN == grantType) {
		url = UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), REFRESH_TOKEN_METHOD);
		sStream << "refresh_token=" << _loginInfo.GetRefreshToken() << "&grant_type=refresh_token";
	}
	else {
		triggerEvent(IClientServiceEvent, OnAccessTokenReceived(GRANTTYPE_NOTSUPPORT, grantType, ""));
		return;
	}
	sStream << "&site_id=" << _loginInfo.GetSiteId();

	UC_LOG(INFO) << "GetAccessToken Account=" << _loginInfo.GetAccount().c_str();

	request.Init(url, HTTP_METHOD_POST);

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnAccessTokenReceived, grantType));
}

int32_t ClientService::OnAccessTokenReceived(int32_t errorCode, HTTPResponse *response, OauthGrantType grantType) {
	UC_LOG(WARNING) << "Get AccessToken response, errorCode: " << errorCode
		<< ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
		<< ", body: " << (response != NULL ? response->GetContent() : "") 
		<< ", grantType: " << grantType;

	if (errorCode != UC_SUCCEEDED || response == NULL || response->GetStatusCode() != 200) {
		triggerEvent(IClientServiceEvent, OnAccessTokenReceived(UCC_STATUS_CODE_ERROR, grantType, ""));
		return 0;
	}

	rapidjson::Document document;
	if (document.Parse<0>(response->GetContent().c_str()).HasParseError()) {
		triggerEvent(IClientServiceEvent, OnAccessTokenReceived(JSON_PARSE_ERROR, grantType, ""));
		return 0;
	}

	int32_t res = document["code"].GetInt();
	if (UC_SUCCEEDED != res) {
		triggerEvent(IClientServiceEvent, OnAccessTokenReceived(res, grantType, ""));
		if (OGT_REFRESH_TOKEN == grantType) {
			UC_LOG(INFO) << "refreshToken faild. errno=" << res;
			cout << "refreshToken faild. errno=" << res << endl;
			GetAccessToken(OGT_PASSWORD);
		}

		return 0;
	}

	rapidjson::Value &data = document["data"];
	if (data.Size() > 0) {
		int expiresIn = jsonhelper::getIntValue(data[0], "expires_in", 0);
		_RefreshToken(expiresIn);
		string accessToken = jsonhelper::getStrValue(data[0], "access_token", "");
		string refreshToken = jsonhelper::getStrValue(data[0], "refresh_token", "");
		if (!accessToken.empty() && !refreshToken.empty()) {
			_loginInfo.SetAccessToken(accessToken);
			_loginInfo.SetRefreshToken(refreshToken);

#ifdef PBX_SERVICE
			if (0 != (0x0002 & _moduleFlag)) {
				IPbxService *lpPbxService = (IPbxService *)_engine->QueryService(UCPbxService);
				if (NULL != lpPbxService) {
					lpPbxService->ConnectPbx(_loginInfo.GetUserId(), accessToken, expiresIn);
				}
			}
			else {
				UC_LOG(INFO) << "PBX Not open ! moduleFlag=" << _moduleFlag;
			}
#endif // PBX_SERVICE

			triggerEvent(IClientServiceEvent, OnAccessTokenReceived(UC_SUCCEEDED, grantType, accessToken));
			return 0;
		}
	}
	
	triggerEvent(IClientServiceEvent, OnAccessTokenReceived(JSON_PARSE_ERROR, grantType, ""));
	return 0;
}

bool ClientService::SetProxyUrl(const std::string& proxyUrl){
	if (!proxyUrl.empty()){
		_proxyInfo.SetProxyUrl(proxyUrl);
		DBManager::GetInstance().GetAuthInfo(_proxyInfo);
#ifdef PBX_SERVICE
		// 查看是否支持PBX
		IPbxService *lpPbxService = (IPbxService *)_engine->QueryService(UCPbxService);
		if (NULL != lpPbxService) {
			lpPbxService->SetProxyUrl(proxyUrl);
		}
#endif // PBX_SERVICE
	}
	
	UC_LOG(INFO) << "SetProxyUrl: proxy url: " << proxyUrl;
	return true;
}

bool ClientService::SetProxyInfo(int32_t auth_type, const std::string& auth_account, const std::string& auth_passwd)
{
	if (_proxyInfo.GetProxyUrl().empty()){
		UC_LOG(INFO) << "proxy url is empty. why application start don't query proxy url";
		return false;
	}
	
	_proxyInfo.SetAuthType(auth_type);
	_proxyInfo.SetProxyAccount(auth_account);
	_proxyInfo.SetProxyPassword(auth_passwd);

#ifdef PBX_SERVICE
	// 查看是否支持PBX
	IPbxService *lpPbxService = (IPbxService *)_engine->QueryService(UCPbxService);
	if (NULL != lpPbxService) {
		lpPbxService->SetProxyInfo(auth_type, auth_account, auth_passwd);
	}
#endif // PBX_SERVICE

	int ret = DBManager::GetInstance().SaveProxyAuthInfo(_proxyInfo);
	if (ret != DBError::DB_SUCCESS)
	{
		UC_LOG(ERROR) << "SaveProxyAuthInfo in DB error. error code: " << ret;
		return false;
	}
	return true;
}

bool ClientService::GetProxyInfo(std::string& auth_account, std::string& auth_passwd, int &auth_type){
	if (_proxyInfo.GetProxyUrl().empty()){
		UC_LOG(INFO) << "proxy url is empty. why application start don't query proxy url";
		return false;
	}

	auth_account = _proxyInfo.GetProxyAccount();
	auth_passwd = _proxyInfo.GetProxyPassword();
	auth_type = _proxyInfo.GetAuthType();

	return true;
}

void ClientService::Logout() {

	HTTPRequest		request;
	string			loginURL;
	string 			bodyData;
	stringstream	sStream;

	loginURL = UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), LOGOUT_METHOD);
	request.Init(loginURL, HTTP_METHOD_POST);

	sStream << "session_id=" << _loginInfo.GetSessionId() << "&user_id=" << _loginInfo.GetUserId();

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnLogoutComplete), HTTP_TIMEOUT, HTTP_RESPONSE_INFINITY);
}

/**
 * logout 返回的结果处理
 * @param errorCode
 * @param response
 */
int32_t ClientService::OnLogoutComplete(int32_t errorCode, HTTPResponse *response) {

	UC_LOG(INFO) << "OnLogoutComplete:" << (response ? response->GetContent() : "NULL");

	theStore.Clear();
	theStore.GetGroupListStore()->Clear();
	
	int32_t code = errorCode;

	if (response) {
		try
		{
			wstringstream jsonStream;
			boost::property_tree::wptree ptRoot;
			jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
			boost::property_tree::json_parser::read_json(jsonStream, ptRoot);

			code = ptRoot.get<int32_t>(L"code");
		}
		catch (boost::property_tree::ptree_error & e)
		{
			code = JSON_PARSE_ERROR;
			UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
		}
	}

	triggerEvent(IClientServiceEvent, OnLogout(code));
	triggerEventEx(IUCEngineEvent, *_engine, OnLogoutSucceeded());
	return 0;
}

/**
 * 登录auth/lookup 返回登录的uccserver url
 * @param account
 * @param password
 */
void ClientService::Lookup() {

 	string			lookupURL;
	HTTPRequest		request;
	string 			bodyData;
	stringstream	sStream;
	string			casServer = CASSERVER_URL;

	casServer = _engine->GetConfig().casServer;
	if (casServer.empty())
		casServer = CASSERVER_URL;

	lookupURL = UCEngine::GetRequestURL(casServer, LOOKUP_METHOD);

	request.Init(lookupURL, HTTP_METHOD_POST);

	sStream << "user_account=" << _loginInfo.GetAccount();

	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnLookupComplete));
}

/**
 * lookup 接口的回调操作
 * @param response
 */
int32_t ClientService::OnLookupComplete(int32_t errorCode, HTTPResponse *response) {
	UC_LOG(INFO) << "OnLookupComplete:" << errorCode << " content" << (response == NULL ? "" : response->GetContent());

	int32_t auth_type = 0;
	if (errorCode == UC_FAILED) {
		// 表示调用失败,response不是有效的数据
		_isLoginning = false;
        triggerEvent(IClientServiceEvent, OnLogin(errorCode, auth_type, 0));
        return -1;
	}
	
	if (response->GetStatusCode() != 200) {
		// 表示http请求失败
		if (response->GetStatusCode() == 407)
		{
			std::string auth =  response->GetHeaderValue("Proxy-Authenticate");
			if ( auth.empty() )
			{
				auth = response->GetHeaderValue("Proxy-Authorization");
			}

			if (auth.find("Basic") != std::string::npos)
			{
				auth_type = 0;
			}
			else if (auth.find("Digest") != std::string::npos)
			{
				auth_type = 1;
			}
			else if (auth.find("NTLM") != std::string::npos)
			{
				auth_type = 2;
			}
			else
			{
				auth_type = 0;
			}
			UC_LOG(INFO) << "Proxy-Authorization get http response auth type: " << auth_type;
		}
		_isLoginning = false;
        triggerEvent(IClientServiceEvent, OnLogin(response->GetStatusCode(), auth_type, 0));		
		return 0;
	}

	// 解析返回的json数据
	int32_t code = JSON_PARSE_ERROR;
	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	try
	{
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);

		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}

	if (code != UC_SUCCEEDED) {
		_isLoginning = false;
        triggerEvent(IClientServiceEvent, OnLogin(code, auth_type, 0));
        return code;
	}

	boost::property_tree::wptree ptData;
	try {
		ptData = ptRoot.get_child(L"data");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
		triggerEvent(IClientServiceEvent, OnLogin(JSON_PARSE_ERROR, auth_type, 0));
		_isLoginning = false;
		return JSON_PARSE_ERROR;
	}
	wstring domain = L"";

	BOOST_FOREACH(boost::property_tree::wptree::value_type &v, ptData) {
		boost::property_tree::wptree ptLookup = v.second;
		domain = ptLookup.get<wstring>(L"uc_cluster_url");
	}

	// 保存 cluter_url 信息
	if (domain == L"") {
		_loginInfo.SetUccServerDomain(UCCSERVER_URL);
	} else {
		_loginInfo.SetUccServerDomain(StringConvert::ToUTF8Str(domain));
	}

	// 启动登录流程
	Login();
	return 0;
}

/**
 *
 */

void ClientService::Login() {

	HTTPRequest		request;
	string			loginURL;
	string 			bodyData;
	stringstream	sStream;

	loginURL = UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), LOGIN_METHOD);
	request.Init(loginURL, HTTP_METHOD_POST);

	sStream << "user_account=" << _loginInfo.GetAccount()
		<< "&password=" << StringConvert::URLEncode(CrypWrapper::Encrypt(_loginInfo.GetPassword(), _loginInfo.GetAccount()))
		<< "&client_type=" << _loginInfo.GetClientType() << "&client_info=" << _loginInfo.GetClientInfo() << "&encrypt=" << 1;

	if (!_isFromReConnect)
		sStream << "&notify=" << 1;

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnLoginComplete));
}

/**
 *
 * @param errorCode
 * @param response
 */
int32_t ClientService::OnLoginComplete(int32_t errorCode, HTTPResponse *response) {
	UC_LOG(INFO) << "OnLoginComplete: " << errorCode << " content:" << (response == NULL ? "" : response->GetContent());

	theStore.Clear();
	theStore.GetGroupListStore()->Clear();
	int32_t auth_type = 0, cloudDiskType = 0;
	if (errorCode == UC_FAILED) {
		// 表示调用失败,response不是有效的数据
		_isLoginning = false;
        triggerEvent(IClientServiceEvent, OnLogin(errorCode, auth_type, 0));
        return -1;
	}

	if (response->GetStatusCode() != 200) {
		// 表示http请求失败
		if (response->GetStatusCode() == 407)
		{
			std::string auth = response->GetHeaderValue("Proxy-Authenticate");
			if (auth.empty())
			{
				auth = response->GetHeaderValue("Proxy-Authorization");
			}

			if (auth.find("Basic") != std::string::npos)
			{
				auth_type = 0;
			}
			else if (auth.find("Digest") != std::string::npos)
			{
				auth_type = 1;
			}
			else if (auth.find("NTLM") != std::string::npos)
			{
				auth_type = 2;
			}
			else
			{
				auth_type = 0;
			}
			UC_LOG(INFO) << "Proxy-Authorization get http response auth type: " << auth_type;
		}
		_isLoginning = false;
        triggerEvent(IClientServiceEvent, OnLogin(response->GetStatusCode(), auth_type, 0));
        return 0;
	}

	// 解析登录返回的json数据
	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	int32_t code = JSON_PARSE_ERROR;
	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());

	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}



	if (code != 0) {
		//强制更新错误码
		if (code == 10152) {
			wstring url = L"";
			try{
				boost::property_tree::wptree ptUpgrade = ptRoot.get_child(L"data");
				url = ptUpgrade.get<wstring>(L"upgradeserver_url", L"");
				_loginInfo.AddServerInfo(UPGRADE_SERVER, StringConvert::ToUTF8Str(url));
				url = ptUpgrade.get<wstring>(L"package_info_url", L"");
				_loginInfo.AddServerInfo(PACKAGEINFO_SERVER, StringConvert::ToUTF8Str(url));
				url = ptUpgrade.get<wstring>(L"package_download_url", L"");
				_loginInfo.AddServerInfo(PACKAGE_DOWNLOAD_SERVER, StringConvert::ToUTF8Str(url));
			}
			catch (boost::property_tree::ptree_error & e)
			{
				UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
			}
		}
		// 登录错误
		_isLoginning = false;
        triggerEvent(IClientServiceEvent, OnLogin(code, auth_type, 0));		
		return code;
	}
    // 登陆成功初始化用户数据库
	_isLoginning = InitUserDB();

	boost::property_tree::wptree ptDataArray;
	boost::property_tree::wptree ptData;

	ptDataArray = ptRoot.get_child(L"data");

	BOOST_FOREACH(boost::property_tree::wptree::value_type &v, ptDataArray) {
		ptData = v.second;
		break;
	}

	_loginInfo.SetUserId(ptData.get<int32_t>(L"user_id", 0));
	_loginInfo.SetLastKickTime(ptData.get<int64_t>(L"last_kick_time", 0));
	_loginInfo.SetSessionId(StringConvert::ToUTF8Str(ptData.get<wstring>(L"session_id", L"")));
	_loginInfo.SetResId(ptData.get<int32_t>(L"res_id", 0));
	_loginInfo.SetSiteId(ptData.get<int32_t>(L"site_id", 0));

	// 获取 profile 的信息
	boost::property_tree::wptree ptProfile;

	ptProfile = ptData.get_child(L"profile");
	ContactService *contactService = (ContactService*)((UCEngine*)_engine)->QueryService(UCContactService);
	assert(contactService != NULL);

	UC_LOG(INFO) << "saveContact result:" << contactService->SaveContact(ptProfile);
	
	ContactInfo user;
	if (contactService->GetCurrentUserInfo(user)){
		_loginInfo.SetAccount(user.GetAccount());
	}

	// 获取profile中 customer_code 信息
	_loginInfo.SetCustomerCode(StringConvert::ToUTF8Str(ptProfile.get<wstring>(L"customer_code", L"")));
	_loginInfo.SetPwdExpiredDays(ptProfile.get<int32_t>(L"pwd_expired_days", 0));
	_loginInfo.SetForceChangePwd(ptProfile.get<int32_t>(L"force_change_pwd", 0));

	// 获取profile中的is_allow_change 信息
	_passwordRule.SetAllowChange(ptProfile.get<int32_t>(L"is_allow_change_pwd", 1));


	// 获取各个服务的信息
	boost::property_tree::wptree ptServer;
	wstring domain = L"";
	wstring cname = L"";
	wstring token = L"";

	// ucas server info
	ptServer = ptData.get_child(L"ucas");
	domain = ptServer.get<wstring>(L"domain", L"");
	cname = ptServer.get<wstring>(L"cname", L"");
	token = ptServer.get<wstring>(L"token", L"");
	_loginInfo.AddServerInfo(UCAS_HTTP_SERVER, StringConvert::ToUTF8Str(domain),
			StringConvert::ToUTF8Str(cname), StringConvert::ToUTF8Str(token));

	// ucas_https server info
	ptServer = ptData.get_child(L"ucas_https");
	domain = ptServer.get<wstring>(L"domain", L"");
	cname = ptServer.get<wstring>(L"cname", L"");
	token = ptServer.get<wstring>(L"token", L"");
	_loginInfo.AddServerInfo(UCAS_HTTPS_SERVER, StringConvert::ToUTF8Str(domain),
			StringConvert::ToUTF8Str(cname), StringConvert::ToUTF8Str(token));

	// fs server info
	ptServer = ptData.get_child(L"fs");
	domain = ptServer.get<wstring>(L"domain", L"");
	_loginInfo.AddServerInfo(FS_SERVER, StringConvert::ToUTF8Str(domain));

	// relay server info
	ptServer = ptData.get_child(L"relayserver");
	domain = ptServer.get<wstring>(L"domain", L"");
	_loginInfo.AddServerInfo(RELAY_SERVER, StringConvert::ToUTF8Str(domain));

	// upgradeserver
	ptServer = ptData.get_child(L"upgradeserver");
	domain = ptServer.get<wstring>(L"upgradeserver_url", L"");
	_loginInfo.AddServerInfo(UPGRADE_SERVER, StringConvert::ToUTF8Str(domain));
	domain = ptServer.get<wstring>(L"package_info_url", L"");
	_loginInfo.AddServerInfo(PACKAGEINFO_SERVER, StringConvert::ToUTF8Str(domain));
	domain = ptServer.get<wstring>(L"package_download_url", L"");
	_loginInfo.AddServerInfo(PACKAGE_DOWNLOAD_SERVER, StringConvert::ToUTF8Str(domain));

	// 获取密码规则
	try
	{
		boost::property_tree::wptree ptPasswordrule;
		ptPasswordrule = ptData.get_child(L"pwd_rules");
		_passwordRule.SetExprisedDays(ptPasswordrule.get<int32_t>(L"pwd_exprised_days", 0));
		_passwordRule.SetMemory(ptPasswordrule.get<int32_t>(L"pwd_memory", 0));
		_passwordRule.SetComplex(ptPasswordrule.get<int32_t>(L"pwd_complex", 0));
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json for getting Password rule in OnLoginComplete , exception: " << e.what();
	}

    // 获取站点设置信息
    CapabilityInfo cpbltInfo;

	// 查看是否支持云盘
	try {
		boost::property_tree::wptree siteConfig = ptData.get_child(L"site_config");
		cloudDiskType = siteConfig.get<int32_t>(L"cloud_disk_type", 0);
        cpbltInfo.remoteAssistant = siteConfig.get<int32_t>(L"remote_assistant", 0);

        if (siteConfig.find(L"visable") != siteConfig.not_found()) {
            boost::property_tree::wptree visableArray = siteConfig.get_child(L"visable");
            BOOST_FOREACH(boost::property_tree::wptree::value_type &v, visableArray) {
                if (0 == v.second.get_value(v.first).compare(APP_CENTER)) {
                    cpbltInfo.appCenter = 1;
                }
                else if (0 == v.second.get_value(v.first).compare(APP_MOMENTS)) {
                    cpbltInfo.appMoments = 1;
                }
                else if (0 == v.second.get_value(v.first).compare(CALENDAR)) {
                    cpbltInfo.calendar = 1;
                }
                else if (0 == v.second.get_value(v.first).compare(CALL)) {
                    cpbltInfo.call = 1;
                }
                else if (0 == v.second.get_value(v.first).compare(CLOUD_DISK)) {
					cpbltInfo.cloudDisk = 1;
					_moduleFlag |= 0x0001;
                }
                else if (0 == v.second.get_value(v.first).compare(CONF)) {
                    cpbltInfo.conf = 1;
                }
                else if (0 == v.second.get_value(v.first).compare(IM)) {
                    cpbltInfo.im = 1;
                }
                else if (0 == v.second.get_value(v.first).compare(REMOTE_ASSISTANT)) {
                    cpbltInfo.remoteAssistant = 1;
                }
                else if (0 == v.second.get_value(v.first).compare(SCAN)) {
                    cpbltInfo.scan = 1;
                }
				else if (0 == v.second.get_value(v.first).compare(BROADCAST_STATION)){
					cpbltInfo.broadcastStation = 1;
				}
				else if (0 == v.second.get_value(v.first).compare(MEETING_ROOM)){
					cpbltInfo.meetingRoom = 1;
				}
				else if (0 == v.second.get_value(v.first).compare(CLOUD_MEETING)){
					cpbltInfo.cloudMeeting = 1;
				}
				else if (0 == v.second.get_value(v.first).compare(CI_PBX)){
					cpbltInfo.pbx = 1;
					_moduleFlag |= 0x0002;
				}
            }
        }
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse json for site_config in OnLoginComplete , exception: " << e.what();
	}
	// 过渡代码
	// 开发环境的token和key
	// ("b864e881ec30da85ecs3c47ied25b58ba7cf", "cUCXiusVww");
	// 正式环境的token和key
	// ("f2ae996aeaa46a9825s7202ia670b5d5ea57", "DyTklowRis");
	int nPos = _engine->GetConfig().casServer.find(FLAG_FORMAL);
	if (std::string::npos != nPos) {
		_loginInfo.SetPbxKey("f2ae996aeaa46a9825s7202ia670b5d5ea57");
		_loginInfo.SetPbxToken("DyTklowRis");
	}
	else {
		_loginInfo.SetPbxKey("b864e881ec30da85ecs3c47ied25b58ba7cf");
		_loginInfo.SetPbxToken("cUCXiusVww");
	}
	// 
	_loginInfo.SetCapabilityInfo(cpbltInfo);// 保存到sdk内存

	// 获取讨论组设置;
	try
	{
		boost::property_tree::wptree ptGroupConfig;
		ptGroupConfig = ptData.get_child(L"group_config");
		boost::property_tree::wptree ptDefaultConfig;
		ptDefaultConfig = ptGroupConfig.get_child(L"default");
		_groupCountLimit = ptDefaultConfig.get<int32_t>(L"group_count", 500);
		_maxGroupMemberLimit = ptDefaultConfig.get<int32_t>(L"member_count", 500);
		_minGroupMemberLimit = ptDefaultConfig.get<int32_t>(L"small_member_count", 200);
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json for getting group config in OnLoginComplete , exception: " << e.what();
	}

	// 获取当前登录用户的信息
	ContactInfo		contact;

	contactService->GetCurrentUserInfo(contact);
	UC_LOG(INFO) << "login user info:" << contact.GetAccount() << "|"
			<< contact.GetDisplayName() << "|"
			<< contact.GetLocalAvatar() << "|"
			<< contact.GetRemoteAvatarUrl();

	contactService->GetCurrentUserInfo(contact);
	_loginInfo.SetDisplayName(contact.GetDisplayName());

	// 将当前用户保存到history account表中
	AccountInfo account;

	account.SetUserAccount(contact.GetAccount());
	account.SetUserName(contact.GetDisplayName());
	account.SetPassword(_loginInfo.GetPassword());
	account.SetAutoLogin(true);
	account.SetSavePassword(true);
	time_t nowTime = time(NULL);
	account.SetLoginTime(nowTime);
	account.SetLastManualLoginTime(nowTime);
	account.SetLocalAvatar(contact.GetLocalAvatar());
	account.SetAccountType(_loginInfo.GetAccountType());
	DBManager::GetInstance().SaveAccount(account);

    _isLoginning = false;
	triggerEventEx(IUCEngineEvent, *_engine, OnLoginSucceeded());

    std::string capability;
    MakeCapabilityData(cpbltInfo, capability);
	triggerEvent(IClientServiceEvent, OnLogin(code, auth_type, cloudDiskType, capability));

	return code;
}

void ClientService::MakeCapabilityData(CapabilityInfo &cpbltInfo, std::string &capability) {
    boost::property_tree::wptree jsonRoot;
	
    jsonRoot.put(L"app_center", cpbltInfo.appCenter);
    jsonRoot.put(L"app_moments", cpbltInfo.appMoments);
    jsonRoot.put(L"calendar", cpbltInfo.calendar);
    jsonRoot.put(L"call", cpbltInfo.call);
    jsonRoot.put(L"cloud_disk_type", cpbltInfo.cloudDisk);
    jsonRoot.put(L"conf", cpbltInfo.conf);
    jsonRoot.put(L"im", cpbltInfo.im);
    jsonRoot.put(L"remote_assistant", cpbltInfo.remoteAssistant);
	jsonRoot.put(L"scan", cpbltInfo.scan);
	jsonRoot.put(L"pbx", cpbltInfo.pbx);
	jsonRoot.put(L"meeting_room", cpbltInfo.meetingRoom);
	jsonRoot.put(L"cloud_meeting", cpbltInfo.cloudMeeting);
	jsonRoot.put(L"broadcast_station", cpbltInfo.broadcastStation);

    wstringstream sData;
    boost::property_tree::json_parser::write_json(sData, jsonRoot);

    stringstream sStream;
    sStream << StringConvert::ToUTF8Str(sData.str());

    capability = sStream.str();
}

void ClientService::ModifyPassword(const string &oldPassword, const string &newPassword) {

	HTTPRequest		request;
	string			loginURL;
	string 			bodyData;
	stringstream	sStream;

	loginURL = UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), USER_MODIFYPWD_METHOD);
	request.Init(loginURL, HTTP_METHOD_POST);

	sStream << "user_id=" << _loginInfo.GetUserId() << "&session_id=" << _loginInfo.GetSessionId()
			<< "&new_password=" << newPassword << "&confirm_password=" << newPassword << "&old_password="
			<< oldPassword;

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnModifyPasswordComplete));
}

int32_t ClientService::OnModifyPasswordComplete(int32_t errorCode, HTTPResponse *response) {

	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnModifyPasswordComplete", errorCode, response);

	if (result != UC_SUCCEEDED) {
		triggerEvent(IClientServiceEvent, OnPasswordModified(result));
		return -1;
	}

	// 解析登录返回的json数据
	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	int32_t code = JSON_PARSE_ERROR;
	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());

	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}

	triggerEvent(IClientServiceEvent, OnPasswordModified(code));
	return code;
}

void ClientService::ConnectUCAS() {
	stringstream	sStream;
	ServerInfo		ucasServer = _loginInfo.GetServerInfo(UCAS_HTTPS_SERVER);

	if (ucasServer.GetDomain().empty()) {
		ucasServer = _loginInfo.GetServerInfo(UCAS_HTTP_SERVER);
	}

	if (ucasServer.GetDomain().empty()) {
		UC_LOG(ERROR) << "get ucas server info error";
		triggerEvent(IClientServiceEvent, OnUCASConnected(UC_FAILED));
		return;
	}

	sStream << ucasServer.GetDomain() << "/stream?" << "cname=" << ucasServer.GetCName()
			<< "&token=" << ucasServer.GetToken() << "&seq=1";

	((UCEngine*)_engine)->DisconnectUCAS();
	((UCEngine*)_engine)->ConnectUCAS(sStream.str());
}

void ClientService::DisconnectUCAS() {
	((UCEngine*)_engine)->DisconnectUCAS();
}

void ClientService::ConnectUCC() {
	((UCEngine*)_engine)->DisconnectUCC();
	((UCEngine*)_engine)->ConnectUCC();
}

void ClientService::DisconnectUCC() {
	((UCEngine*)_engine)->DisconnectUCC();
}

int32_t ClientService::GetLoginUserId() {
	return _loginInfo.GetUserId();
}

void ClientService::GetNewConversationList(int32_t number) {
	int64_t startTime = 0;
	int64_t endTime = 0;
	std::map<int64_t, int32_t> atInfo;
	UC_LOG(INFO) << "GetNewConversationList. startTime: " << startTime << " endTime: " << endTime
		<< " number: " << number;

	std::list< ConversationInfo* > conversations;
	bool done = theStore.GetConvStore()->GetConversationList(GetLoginInfo().GetUserId(), number, startTime, endTime, conversations);
	if ( done )
	{
		triggerEvent(IClientServiceEvent, OnConversationListReceived(UC_SUCCEEDED, &conversations, false, -1, atInfo));
		std::list< ConversationInfo* >::iterator iter = conversations.begin();
		for ( ; iter != conversations.end(); ++iter )
		{
			ConversationInfoInternal* conv_internal = (ConversationInfoInternal*)(*iter);
			if ( NULL == conv_internal )
				continue;

			UCMsg* uc_msg = (UCMsg*)conv_internal->GetMsg();
			triggerEventEx(IUCEngineEvent, *_engine, OnMessageReceived(LIST, *uc_msg, conv_internal->GetBody(), conv_internal->GetSendState()));

			triggerEvent(IClientServiceEvent, OnConversationListComplete(conversations.size()));
		}

		// list结束，需要通知UI,如果本次请求的数据没显示一屏，需要UI主动请求下更早的消息列表
		triggerEvent(IClientServiceEvent, OnConversationListComplete(conversations.size()));
	}
	else
	{
		ListConversation(startTime, endTime, number, 1);
	}

}

void ClientService::GetOldConversationList(int64_t endTime, int32_t number) {

	int64_t startTime = 0;
	std::map<int64_t, int32_t> atInfo;

	UC_LOG(INFO) << "GetOldConversationList. startTime: " << startTime << " endTime: " << endTime
		<< " number: " << number;

	std::list< ConversationInfo* > conversations;
	bool done = theStore.GetConvStore()->GetConversationList(GetLoginInfo().GetUserId(), number, startTime, endTime, conversations);
	if ( done )
	{
		triggerEvent(IClientServiceEvent, OnConversationListReceived(UC_SUCCEEDED, &conversations, false, -1, atInfo));
		std::list< ConversationInfo* >::iterator iter = conversations.begin();
		for ( ; iter != conversations.end(); ++iter )
		{
			ConversationInfoInternal* conv_internal = (ConversationInfoInternal*)(*iter);
			if ( NULL == conv_internal )
				continue;

			UCMsg* uc_msg = (UCMsg*)conv_internal->GetMsg();
			triggerEventEx(IUCEngineEvent, *_engine, OnMessageReceived(LIST, *uc_msg, conv_internal->GetBody(), conv_internal->GetSendState()));

			triggerEvent(IClientServiceEvent, OnConversationListComplete(conversations.size()));
		}

		// list结束，需要通知UI,如果本次请求的数据没显示一屏，需要UI主动请求下更早的消息列表
		triggerEvent(IClientServiceEvent, OnConversationListComplete(conversations.size()));
	}
	else
	{
		ListConversation(startTime, endTime, number, 0);
	}
}

void ClientService::ListConversation(int64_t startTime, int64_t endTime, int32_t number, int32_t topState) {
	HTTPRequest		request;
	stringstream	sStream;
	int64_t lastContactSyncTime = 0;
	int64_t lastGroupSyncTime = 0;

	ContactService *contactService = (ContactService*)_engine->QueryService(UCContactService);
	contactService->GetLastContactSyncTime(&lastContactSyncTime);

	//
	GroupService *groupService = (GroupService*)_engine->QueryService(UCGroupService);
	lastGroupSyncTime = theStore.GetGroupListStore()->GetLastModifyTime();//groupService->GetLastGroupSyncTime();

	sStream << UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), CONVERSATION_LIST_METHOD) <<
			"?session_id=" << _loginInfo.GetSessionId() << "&user_id=" << _loginInfo.GetUserId() <<
			"&count=" << number << "&start_time=" << startTime << "&end_time=" << endTime <<
			"&last_contact_time=" << lastContactSyncTime << "&last_group_time=" << lastGroupSyncTime << "&top=" << topState;

	request.Init(sStream.str(), HTTP_METHOD_GET);

	_engine->SendAsyncRequest(request,MakeRequestCallback(this, &ClientService::OnListConversationComplete, number));
}

int32_t ClientService::OnListConversationComplete(int32_t errorCode, HTTPResponse *response, int32_t number) {

	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnListConversationComplete", errorCode, response);

	std::map<int64_t, int32_t> atInfo;

	if (result != UC_SUCCEEDED) {
		triggerEvent(IClientServiceEvent, OnConversationListReceived(result, NULL, false, -1, atInfo));
		return -1;
	}

	
	// 解析登录返回的json数据
	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	boost::property_tree::wptree ptDataArray;
	boost::property_tree::wptree ptData;
	std::list<ConversationInfo*> conversations;

	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
	int32_t code = JSON_PARSE_ERROR;
	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}

	if (code != UC_SUCCEEDED) {
		triggerEvent(IClientServiceEvent, OnConversationListReceived(code, &conversations, false, -1, atInfo));
		return code;
	}
	try {
		int64_t timeStamp = ptRoot.get<int64_t>(L"timestamp");
		ptDataArray = ptRoot.get_child(L"data");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
		triggerEvent(IClientServiceEvent, OnConversationListReceived(UC_FAILED, &conversations, false, -1, atInfo));
		return UC_FAILED;
	}

	BOOST_FOREACH(boost::property_tree::wptree::value_type &v, ptDataArray) {
		ptData = v.second;
		break;
	}

	boost::property_tree::wptree ptValueArray;
	boost::property_tree::wptree ptValue;

	// 解析 contact info
	ContactService *contactService = (ContactService*)_engine->QueryService(UCContactService);
	try {
		ptValueArray = ptData.get_child(L"contact_info");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
		triggerEvent(IClientServiceEvent, OnConversationListReceived(UC_FAILED, &conversations, false, -1, atInfo));
		return UC_FAILED;
	}

	BOOST_FOREACH(boost::property_tree::wptree::value_type &v, ptValueArray) {
		ptValue = v.second;
		contactService->SaveContact(ptValue);
	}

	// 解析group info
	GroupService *groupService = (GroupService*)_engine->QueryService(UCGroupService);
	try {
		ptValueArray = ptData.get_child(L"group_info");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
		triggerEvent(IClientServiceEvent, OnConversationListReceived(UC_FAILED, &conversations, false, -1, atInfo));
		return UC_FAILED;
	}

	BOOST_FOREACH(boost::property_tree::wptree::value_type &v, ptValueArray) {
		ptValue = v.second;
		
		std::wstringstream ss;
		boost::property_tree::json_parser::write_json(ss, ptValue);
		
		rapidjson::Document document;
		document.Parse<0>(StringConvert::ToUTF8Str(ss.str()).c_str());

		GroupInfo groupInfo;
		groupService->ParseGroupInfo(document, groupInfo);

		theStore.GetGroupListStore()->UpdateGroupInfo(groupInfo);
	}

	// 解析conv_status
	map<int32_t, int32_t> mapConvStatus;

	ptValueArray = ptData.get_child(L"conv_status");
	BOOST_FOREACH(boost::property_tree::wptree::value_type &v, ptValueArray) {
		ptValue = v.second;
		int32_t conversationId = ptValue.get<int32_t>(L"conv_id");
		int32_t newMsgCount = ptValue.get<int32_t>(L"count");

		mapConvStatus.insert(pair<int32_t, int32_t>(conversationId, newMsgCount));
	}

	// 解析 msg list
	ptValueArray = ptData.get_child(L"msg_list");
	IMService *imService = (IMService*)_engine->QueryService(UCIMService);

	BOOST_FOREACH(boost::property_tree::wptree::value_type &v, ptValueArray) {
		ptValue = v.second;
		UCMsg *msg = NULL;
		string body;
		int32_t errcode = UC_FAILED;
		try
		{
			errcode = imService->GetMessageFromJSON(ptValue, msg, body);
		}
		catch (boost::property_tree::ptree_error & e)
		{
			UC_LOG(ERROR) << "encounter an exception when parse msg list json in list conversation process, exception: " << e.what();
			continue;
		}
		
		if (UC_SUCCEEDED == errcode){

			// 过滤从本地删除的conversation
			UCHead	head;
			msg->GetHead(head);
			ConversationType type = MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype());
			int64_t relateId = MessageProcessor::GetRelateId(type, GetLoginInfo().GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
			// Get remove time from DB
			int64_t removeTime = 0;
			DBManager::GetInstance().GetDeleteTime(type, relateId, removeTime);
			int64_t lastMsgTime = head.GetTimestamp();
			if (lastMsgTime <= removeTime)
			{
 				continue;
			}
				// 生成conversation对象
			ConversationInfoInternal *conversation = new ConversationInfoInternal();

			conversation->SetType(MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype()));
			conversation->SetRelateId(MessageProcessor::GetRelateId(conversation->GetType(), GetLoginInfo().GetCurUser().GetUserId(),
					head.GetFrom().GetUserId(), head.GetTo().GetUserId()));
			conversation->SetConversation(head.GetConversation());
			conversation->SetHidden(false);
			conversation->SetLastMsgTime(head.GetTimestamp());
			conversation->SetLastMsgSeq(head.GetSeq());
			conversation->SetMsg(msg);
			conversation->SetBody(body);
			conversation->SetTopIndex(ptValue.get<int32_t>(L"topindex", -1));
			if (conversation->GetType() == CONFERENCE_CHAT ) {
				int64_t eventId = 0;
				int64_t conferenceId = 0;
				int64_t authorizerId = 0;

				MessageProcessor::GetIdsFromUCMsg(*msg, eventId, conferenceId, authorizerId);
				conversation->SetEventId(eventId);
				conversation->SetConferenceId(conferenceId);
				conversation->SetAuthorizerId(authorizerId);
			}

			if (conversation->GetType() == OPENAPI_CHAT) {
				conversation->SetOALastStatus(ptValue.get<int32_t>(L"oa_last_status", -1));
				conversation->SetAppAckSent(ptValue.get<int32_t>(L"app_ack_sent", 1));
				conversation->SetOAExternalData(StringConvert::ToUTF8Str(ptValue.get<std::wstring>(L"oa_external_data", L"")));
			}


			map<int32_t, int32_t>::iterator itor = mapConvStatus.find(conversation->GetConversation());

			if (itor != mapConvStatus.end()) {
				conversation->SetNewMsgCount(itor->second);
			} else {
				conversation->SetNewMsgCount(0);
			}

			conversations.push_back(conversation);
		}
	}

	// 解析totalunread message
	int32_t totalUnRead = ptData.get<int32_t>(L"total_unread_counts", 0);

	//解析groupat_counts 
	boost::property_tree::wptree ptDef;
	boost::property_tree::wptree pAtUserCounts = ptData.get_child(L"groupat_counts", ptDef);
	if (!pAtUserCounts.empty()) {		
		BOOST_FOREACH(boost::property_tree::wptree::value_type v, pAtUserCounts){
			atInfo[StringConvert::WStringToInt64(v.first)] = StringConvert::WStringToInt(v.second.get_value(v.first));
		}
	}

	triggerEvent(IClientServiceEvent, OnConversationListReceived(code, &conversations, false, totalUnRead, atInfo));

	ConversationInfoInternal *conversation = NULL;
	std::list< ConversationInfo* >::iterator iter = conversations.begin();
	for ( ; iter != conversations.end(); ++iter ) {
		conversation = static_cast<ConversationInfoInternal*>(*iter);
		triggerEventEx(IUCEngineEvent, *_engine, OnMessageReceived(LIST, *((UCMsg*)conversation->GetMsg()), conversation->GetBody(), conversation->GetSendState()));
	}

	// Sync data in cache
	theStore.GetConvStore()->SyncConversations( conversations );

	// list结束，需要通知UI,如果本次请求的数据没显示一屏，需要UI主动请求下更早的消息列表
	triggerEvent(IClientServiceEvent, OnConversationListComplete(conversations.size()));

	std::list< ConversationInfo* >::iterator it = conversations.begin();
	for ( ; it != conversations.end(); ++it )
	{
		ConversationInfoInternal* conv_internal = (ConversationInfoInternal*)(*it);
		delete conv_internal;
	}

	return code;
}

int32_t ClientService::SetConversationTop(ConversationType type, int64_t relateId, int32_t state){

	HTTPRequest		request;
	stringstream	sStream;
	string targetURL;

	if (1 == state){
		targetURL = UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), CONVERSATION_SET_TOP_METHOD);
	} else if (0 == state){
		targetURL = UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), CONVERSATION_SET_UNTOP_METHOD);
	}

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << _loginInfo.GetSessionId() << "&user_id=" << _loginInfo.GetUserId() <<
		"&to_user_id=" << relateId;

	if (SINGLE_CHAT == type){
		sStream << "&type=" << 0;
	} else if (PROJECT_CHAT == type || GROUP_CHAT == type || CONFERENCE_CHAT == type || CLOUD_CHAT == type){
		sStream << "&type=" << 1;
	} else if (ORG_CHAT == type){
		sStream << "&type=" << 2;
	} else if (OPENAPI_CHAT == type){
		sStream << "&type=" << 3;
	}

	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnSetTopConversationComplete, type, relateId, state));

	return UC_SUCCEEDED;
}

int32_t ClientService::OnSetTopConversationComplete(int32_t errorCode, HTTPResponse *response, ConversationType type, int64_t relateId, int32_t state){
	UC_LOG(INFO) << "ClientService::OnSetTopConversationComplete relateId= " << relateId << " state= " << state << " errCode= " << errorCode << ", response=" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;

	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		rapidjson::Document document;
		if (!document.Parse<0>(response->GetContent().c_str()).HasParseError()) {
			result = document["code"].GetInt();
		}
		else{
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSetTopConversationComplete , exception: " << document.GetParseError();
			result = JSON_PARSE_ERROR;
		}
	}
	else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (UC_SUCCEEDED != result){
		triggerEvent(IClientServiceEvent, OnConversationTopSet(result, type, relateId, state, -1));
	}

	return result;

}

int32_t ClientService::GetConversationInfo(ConversationType type, int64_t relateId, OUT ConversationInfo & info){

	if (theStore.GetConvStore()->GetConversationInfo(type, relateId, info)){
		return UC_LOCALDATA_SUCCESSED;
	}
	return UC_FAILED;
}

int32_t ClientService::OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state) {
	
	UCHead head;
	msg.GetHead(head);
	switch (msg.GetMsgType()) {
		case UCNotifyPasswordModify: {
		// 密码修改的ucas消息
			PasswordModifyContentMsg *ucMsg = (PasswordModifyContentMsg*)&msg;

			triggerEvent(IClientServiceEvent, OnPasswordModifyMessageReceived());
			break;
		}
		case UCSessionTopSet:{
			SessionTopStateNotify(channel, head, msg, 1);
			break;
		}
		case UCSessionTopUnset:{
			SessionTopStateNotify(channel, head, msg, 0);
			break;
		}
		case UCUserLoginNotify:
			UserLoginNotity(msg);
			break;
		default:{
			break;
		}
	}

	return 0;
}


void ClientService::UserLoginNotity(UCMsg& ucMsg){
	UserLoginNotifyContentMsg *msg = (UserLoginNotifyContentMsg*)&ucMsg;
	if (!msg)
	{
		UC_LOG(ERROR) << "ClientService::UserLoginNotity msg is null";
		return;
	}
									
	triggerEvent(IClientServiceEvent, OnUserLoginNotify(UC_SUCCEEDED, msg->get_user().userID, msg->get_deviceType(), msg->get_deviceName()));
}

void ClientService::SessionTopStateNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, int32_t state){
	
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	SessionTopContentMsg *msg = (SessionTopContentMsg*)&ucMsg;
	if (!msg){
		UC_LOG(ERROR) << "ClientService::SessionTopStateNotify msg is null";
		return;
	}

	MessageInfo	message;
	message.InfoFromUCHead(head);

	ConversationType type = NONE_CHAT;
	if (SessionType::SingleChat == msg->get_sessionType()){
		type = SINGLE_CHAT;
	}
	if (SessionType::GroupChat == msg->get_sessionType()){
		type = GROUP_CHAT;
	}
	else if (SessionType::DiscussionChat == msg->get_sessionType()){
		type = PROJECT_CHAT;
	}
	else if (SessionType::ConferenceType == msg->get_sessionType()){
		type = CONFERENCE_CHAT;
	}
	else if (SessionType::CloudFileChat == msg->get_sessionType()){
		type = CLOUD_CHAT;
	}
	else if (SessionType::OrganizationType == msg->get_sessionType()){
		type = ORG_CHAT;
	}
	else if (SessionType::APIType == msg->get_sessionType()){
		type = OPENAPI_CHAT;
	}
	else if (SessionType::SystemType == msg->get_sessionType()){
		type = WELCOME;
	}

	triggerEvent(IClientServiceEvent, OnConversationTopSet(UC_SUCCEEDED, type, msg->get_to().userID, state, msg->get_topindex()));

	if (channel != UCAS) {
		return;
	}

	theStore.GetConvStore()->UpdataConversationTopIndex(type, msg->get_to().userID, state, msg->get_topindex());
	
}

const LoginInfo& ClientService::GetLoginInfo() const {
	return _loginInfo;
}

const int ClientService::GetCurlSettings() const
{
	return _libcurlSettings;
}

const ProxyInfo& ClientService::GetProxyInfo() const{
	return _proxyInfo;
}

void ClientService::GetLocalConversations(int32_t number) {

	std::map<int64_t, int32_t> atInfo;
	std::list<ConversationInfo*> conversations;
	theStore.GetConvStore()->GetLocalConversations(number, conversations);
	triggerEvent(IClientServiceEvent, OnConversationListReceived(UC_SUCCEEDED, &conversations, true, -1, atInfo));

	std::list<ConversationInfo*>::iterator iter = conversations.begin();
	for ( ; iter != conversations.end(); ++iter )
	{
		ConversationInfoInternal* conv_internal = (ConversationInfoInternal*)(*iter);
		if ( NULL == conv_internal )
			continue;

		UCMsg* uc_msg = (UCMsg*)conv_internal->GetMsg();
		if (uc_msg != NULL) {
			triggerEventEx(IUCEngineEvent, *_engine, OnMessageReceived(LIST, *uc_msg, conv_internal->GetBody(), conv_internal->GetSendState()));
		}
	}
}

int32_t ClientService::OnSendHeartBeatComplete(int32_t errorCode, HTTPResponse *response) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnSendHeartBeatComplete", errorCode, response);
	return 0;
}

int32_t ClientService::LoadHistoryAccounts(std::list<AccountInfo> &accounts) {
	if (DBManager::GetInstance().GetAccountList(accounts) == DBError::DB_SUCCESS) {
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

void ClientService::OnUCASConnected(int32_t errorCode) {
	UC_LOG(INFO) << "ClientService::OnUCASConnected: " << errorCode;

	triggerEvent(IClientServiceEvent, OnUCASConnected(errorCode));
	if (errorCode == UC_SUCCEEDED) {
		// clear store when connecting ucas success
		theStore.Clear();
		InitPresence();
		GetUserStatus(GetCurUCID().GetUserId());
		Timer::GetInstance().Schedule(MakeCallback(this, &ClientService::SendHeartBeat), HEARTBEAT_INTERVAL, HEARTBEAT_INTERVAL, &_heartbeat_timerId);

		//进行云会议升级
		UpdateMeetingClient();
	}
}

void ClientService::OnUCASDisconnected(int32_t errorCode) {
	UC_LOG(INFO) << "ClientService::OnUCASDisconnected: " << errorCode;

	triggerEvent(IClientServiceEvent, OnUCASDisconnected(errorCode));

	Timer::GetInstance().Cancel(_heartbeat_timerId);
	
	_CancelTimer();
}

void ClientService::DeleteSession(ConversationType type, int64_t relateId)
{
	UC_LOG(INFO) << "ClientService::DeleteSession: Type:" << type << " ,relateId:"<< relateId;
	DBManager::GetInstance().DeleteConversation(type, relateId);
}

void ClientService::SendHeartBeat() {
	HTTPRequest		request;
	string			loginURL;
	stringstream	sStream;

	loginURL = UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), HEARTBEAT_METHOD);
	request.Init(loginURL, HTTP_METHOD_POST);

	sStream << "session_id=" << _loginInfo.GetSessionId() << "&user_id=" << _loginInfo.GetUserId();

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnSendHeartBeatComplete));
}

std::string ClientService::GetDocumentDataDir(DocDirType type) {
	UC_LOG(INFO) << "get work directory, type: " << type;
	std::string rootDirectory;
	rootDirectory = _engine->GetConfig().appPath;

	// DB中并没有DocDataDir这个字段。这个写法只是为了扩展。 但是目前js会频繁调用接口造成DB被锁，因此取消该扩展
// 	if (DBManager::GetInstance().GetConfigValue("ClientService", "DocDataDir", rootDirectory) != DBError::DB_SUCCESS || rootDirectory.empty()) {
// 		rootDirectory = _engine->GetConfig().appPath;
// 	}
	
	std::stringstream destDirStream;
	destDirStream << rootDirectory << "\\" << _loginInfo.GetAccount();
	if (type == AUDIO_DIR) {
		destDirStream << "\\Audio";
	}
	else if (type == VIDEO_DIR) {
		destDirStream << "\\Video";
	}
	else if (type == IMAGE_DIR) {
		destDirStream << "\\Image";
	}
	else if (type == FILE_DIR) {
		destDirStream << "\\File";
	}
	else if (type == AVATAR_DIR) {
		destDirStream << "\\Avatar";
	}

	std::string destDir = destDirStream.str();
	boost::filesystem::create_directories(boost::filesystem::path(StringConvert::FromUTF8Str(destDir)));
	return destDir;
}

int32_t ClientService::DeleteHistoryAccount(const std::string& account){
	if (DBManager::GetInstance().DeleteAccount(account) == DBError::DB_SUCCESS) {
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t ClientService::RememberPassword(const std::string& account, bool remember){
	std::string db_account = CrypWrapper::Encrypt(account);
	if (DBManager::GetInstance().RememberPassword(db_account, remember) == DBError::DB_SUCCESS) {
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t ClientService::SetAutoLogin(const std::string& account, bool autoLogin){
	std::string db_account = CrypWrapper::Encrypt(account);
	if (DBManager::GetInstance().SetAutoLogin(db_account, autoLogin) == DBError::DB_SUCCESS) {
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

const UCID& ClientService::GetCurUCID() const {
	return _loginInfo.GetCurUser();
}

std::string ClientService::GetCaptchaURL(const string &hashStr) {
	stringstream	sStream;
	string	casServer = CASSERVER_URL;

	casServer = _engine->GetConfig().casServer;
	if (casServer.empty())
		casServer = CASSERVER_URL;

	string url = UCEngine::GetRequestURL(casServer, CAPTCHA_METHOD);

	sStream << url << "?hashstr=" << hashStr;
	return sStream.str();
}

void ClientService::VerifyAccount(int32_t verifyType, uint16_t clientType, const std::string &account, const std::string &captcha, const std::string &hashStr) {
	if (clientType == UCCLIENT_WIN) {
		if (captcha.empty() || hashStr.empty()) {
			triggerEvent(IClientServiceEvent, OnAccountVerified(UC_FAILED, verifyType, "", "", 0));
			return;
		}
	}
	stringstream sStream;
	HTTPRequest request;
	string	casServer = CASSERVER_URL;

	casServer = _engine->GetConfig().casServer;
	if (casServer.empty())
		casServer = CASSERVER_URL;

	string url = UCEngine::GetRequestURL(casServer, ACCOUNT_VERIFY_METHOD);

	request.Init(url, HTTP_METHOD_POST);

	sStream << "user_account=" << account << "&client_type=" << clientType
		<< "&captcha=" << captcha << "&hash_str=" << hashStr << "&verify_type=" << verifyType;

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnVerifyAccountComplete, verifyType));
}

void ClientService::VerifyCode(int32_t verifyType, uint16_t clientType, const std::string &account, const std::string &mobileCode, int32_t resend) {
	stringstream sStream;
	HTTPRequest request;
	string	casServer = CASSERVER_URL;

	casServer = _engine->GetConfig().casServer;
	if (casServer.empty())
		casServer = CASSERVER_URL;

	string url = UCEngine::GetRequestURL(casServer, CODE_VERIFY_METHOD);

	request.Init(url, HTTP_METHOD_POST);

	sStream << "user_account=" << account << "&client_type=" << clientType
			<< "&mobile_code=" << mobileCode << "&resend=" << resend << "&verify_type=" << verifyType;

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnVerifyCodeComplete, resend));
}

void ClientService::ResetPassword(uint16_t clientType, const std::string &account, const std::string &newPwd, const std::string &confirmPwd, const std::string &mobileCode) {
	stringstream sStream;
	HTTPRequest request;
	string	casServer = CASSERVER_URL;

	casServer = _engine->GetConfig().casServer;
	if (casServer.empty())
		casServer = CASSERVER_URL;

	string url = UCEngine::GetRequestURL(casServer, RESET_PASSWORD_METHOD);

	request.Init(url, HTTP_METHOD_POST);

	sStream << "user_account=" << account << "&client_type=" << clientType
			<< "&new_password=" << newPwd << "&confirm_password=" << confirmPwd << "&mobile_code=" << mobileCode;

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnResetPasswordComplete));
}

int32_t ClientService::OnVerifyAccountComplete(int32_t errorCode, HTTPResponse *response, int32_t verifyType) {
	std::string mobile = "";
	std::string email = "";
	int32_t exprised = 0;
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnVerifyAccountComplete", errorCode, response);

	if (result != UC_SUCCEEDED) {
		triggerEvent(IClientServiceEvent, OnAccountVerified(result, verifyType, mobile, email, exprised));
		return -1;
	}

	// 解析返回的json数据
	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	int32_t code = JSON_PARSE_ERROR;

	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}

	boost::property_tree::wptree ptData;

	if (code == 0) {
		try {
			ptData = ptRoot.get_child(L"data");
		}
		catch (boost::property_tree::ptree_error & e)
		{
			UC_LOG(ERROR) << "encounter an exception when parse json for getting verify accout in OnVerifyAccountComplete , exception: " << e.what();
			triggerEvent(IClientServiceEvent, OnAccountVerified(JSON_PARSE_ERROR, verifyType, mobile, email, exprised));
			return JSON_PARSE_ERROR;
		}

		BOOST_FOREACH(boost::property_tree::wptree::value_type &v, ptData) {
			boost::property_tree::wptree ptMobile = v.second;
			mobile = StringConvert::ToUTF8Str(ptMobile.get<wstring>(L"mobile", L""));
			email = StringConvert::ToUTF8Str(ptMobile.get<wstring>(L"email", L""));

			// 获取密码规则
			boost::property_tree::wptree ptPasswordrule = ptMobile.get_child(L"pwd_rules");
			_passwordRule.SetExprisedDays(ptPasswordrule.get<int32_t>(L"pwd_exprised_days", 0));
			_passwordRule.SetMemory(ptPasswordrule.get<int32_t>(L"pwd_memory", 0));
			_passwordRule.SetComplex(ptPasswordrule.get<int32_t>(L"pwd_complex", 0));
			break;
		}
	}
	if (code == 20136){
		try {
			ptData = ptRoot.get_child(L"data");
		}
		catch (boost::property_tree::ptree_error & e)
		{
			UC_LOG(ERROR) << "encounter an exception when parse json for getting verify accout in OnVerifyAccountComplete , exception: " << e.what();
			triggerEvent(IClientServiceEvent, OnAccountVerified(JSON_PARSE_ERROR, verifyType, mobile, email, exprised));
			return JSON_PARSE_ERROR;
		}

		BOOST_FOREACH(boost::property_tree::wptree::value_type &v, ptData) {
			exprised = v.second.get<int32_t>(L"exprised_time", 0);
			break;
		}
	}

	triggerEvent(IClientServiceEvent, OnAccountVerified(code, verifyType, mobile, email,exprised));
	return code;
}

int32_t ClientService::OnVerifyCodeComplete(int32_t errorCode, HTTPResponse *response, int32_t resend) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnVerifyCodeComplete", errorCode, response);

	int32_t exprised = 0;
	if (result != UC_SUCCEEDED) {
		if (resend == 0)
		{
			triggerEvent(IClientServiceEvent, OnCodeVerified(result, exprised));
		}
		else
		{
			triggerEvent(IClientServiceEvent, OnResendCodeComplete(result, exprised));
		}
		return -1;
	}

	// 解析返回的json数据
	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	int32_t code = JSON_PARSE_ERROR;
	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());

	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e){
		UC_LOG(ERROR) << "encounter an exception when parse json in verify code process, exception: " << e.what();
		code = JSON_PARSE_ERROR;
	}

	boost::property_tree::wptree ptData;
	if (code == 20136){
		try {
			ptData = ptRoot.get_child(L"data");
		}
		catch (boost::property_tree::ptree_error & e)
		{
			UC_LOG(ERROR) << "encounter an exception when parse json for getting verify code in OnVerifyCodeComplete , exception: " << e.what();
			code = JSON_PARSE_ERROR;
		}
		BOOST_FOREACH(boost::property_tree::wptree::value_type& v, ptData){
			exprised = v.second.get<int32_t>(L"exprised_time", 0);
		}
	}
	

	if (resend == 0)
	{
		triggerEvent(IClientServiceEvent, OnCodeVerified(code, exprised));
	}
	else
	{
		triggerEvent(IClientServiceEvent, OnResendCodeComplete(code, exprised));
	}

	return code;
}

int32_t ClientService::OnResetPasswordComplete(int32_t errorCode, HTTPResponse *response) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnResetPasswordComplete", errorCode, response);

	if (result != UC_SUCCEEDED) {
		triggerEvent(IClientServiceEvent, OnPasswordReset(result));
		return -1;
	}

	// 解析返回的json数据
	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	int32_t code = JSON_PARSE_ERROR;
	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}

	triggerEvent(IClientServiceEvent, OnPasswordReset(code));
	return code;
}

int32_t ClientService::GetConfigInfos(const std::string &app, std::map<string, string> &configs) {
	return DBManager::GetInstance().GetConfigValue(app, configs);
}

int32_t ClientService::UpdateConfigInfos(const std::string &app, std::map<string, string> &configs) {
	return DBManager::GetInstance().SaveConfigValue(app, configs);
}

int32_t ClientService::GetConfigInfo(const std::string &app, const std::string &name, std::string &value) {
	return DBManager::GetInstance().GetConfigValue(app, name, value);
}
int32_t ClientService::UpdateConfigInfo(const std::string &app, const std::string &name, const std::string &value) {
	return DBManager::GetInstance().SaveConfigValue(app, name, value);
}

int32_t ClientService::GetValueByKey(const std::string &app, const std::string &name, std::string &value)
{
	return DBManager::GetInstance().GetValueByKey(app, name, value);
}


int32_t ClientService::UpdateValueBykey(const std::string &app, const std::string &name, const std::string &value)
{
	return DBManager::GetInstance().SaveValueByKey(app, name, value);
}

void ClientService::UploadLog(const std::string &logURL, const std::string &desc, const std::string &version) {
	std::string			url = "";
	ClientService		*clientService = (ClientService*)_engine->QueryService(UCClientService);
	std::stringstream	sStream;
	const LoginInfo		&loginInfo = clientService->GetLoginInfo();

	url = UCEngine::GetRequestURL(clientService->GetLoginInfo().GetUccServerDomain(), UPLOAD_LOG_METHOD);

	boost::property_tree::wptree ptRoot;
	wstringstream wsStream;
	wstring json = L"";

	ptRoot.put(L"user_id", loginInfo.GetUserId());
	ptRoot.put(L"user_account", StringConvert::FromUTF8Str(loginInfo.GetAccount()));
	ptRoot.put(L"log_url", StringConvert::FromUTF8Str(logURL));
	ptRoot.put(L"desc", StringConvert::FromUTF8Str(desc));
	ptRoot.put(L"device_type", loginInfo.GetClientType());
	ptRoot.put(L"bug_version", StringConvert::FromUTF8Str(version));

	boost::property_tree::write_json(wsStream, ptRoot);
	json = wsStream.str();

	sStream << "session_id=" << loginInfo.GetSessionId() << "&user_id=" << loginInfo.GetUserId() << "&data=" << StringConvert::URLEncode( StringConvert::ToUTF8Str(json));

	HTTPRequest request;

	request.Init(url, HTTP_METHOD_POST);
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnUploadLogComplete, logURL), HTTP_TIMEOUT, HTTP_RESPONSE_INFINITY);
}

int32_t ClientService::OnUploadLogComplete(int32_t errorCode, HTTPResponse *response, string logFile) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnUploadLogComplete", errorCode, response); 

	if (result != UC_SUCCEEDED) {
		triggerEvent(IClientServiceEvent, OnLogUploaded(result));
		return -1;
	}

	int32_t code = JSON_PARSE_ERROR;

	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}

	triggerEvent(IClientServiceEvent, OnLogUploaded(code));
	return code;
}

int32_t ClientService::GetPwdExpiredDays() {
	return _loginInfo.GetPwdExpiredDays();
}

std::string ClientService::GetSessionId() {
	return _loginInfo.GetSessionId();
}

int32_t ClientService::GetMinGroupMemberLimit(){
	return _minGroupMemberLimit;
}

int32_t ClientService::GetMaxGroupMemberLimit(){
	return _maxGroupMemberLimit;
}

int32_t ClientService::GetGroupCountLimit(){
	return _groupCountLimit;
}

int ClientService::GetModuleFlag(){
	return _moduleFlag;
}

void ClientService::ArchiveLogFile(){
	TaskExecutor::GetInstance().Schedule(MakeCallback(this, &ClientService::_ArchiveLogFile));
}

void ClientService::_ArchiveLogFile() {
	UCEngineConfig config = _engine->GetConfig();
	std::string filePath = "";
	std::string logFile = config.logConfig.logPath;
	std::vector<std::string> files;
	std::stringstream ss;

	// 修改utf8 logPath到多字节logPath
	std::wstring log_path = StringConvert::FromUTF8Str(logFile);

	std::string ascPath = StringConvert::Unicode2Ascii(log_path);

	ss << ascPath << "\\" << Time::GetSysTime() << ".zip";
	filePath = ss.str();

#if defined(WIN32) || defined(WIN64)
	// 遍历文件夹，将目录内的文件都加进来
	_finddata_t file_info;
	string current_path = ascPath + "/*.*"; //可以定义后面的后缀为*.exe，*.txt等来查找特定后缀的文件，*.*是通配符，匹配所有类型,路径连接符最好是左斜杠/，可跨平台  
	//打开文件查找句柄  
	int handle = _findfirst(current_path.c_str(), &file_info);
	//返回值为-1则查找失败  
	if (-1 == handle)
	{
		UC_LOG(ERROR) << "没有找到log目录，返回-1";
		triggerEvent(IClientServiceEvent, OnArchiveLogFinish(""));
		return;
	}

	do
	{
		//判断是否子目录  
		string attribute;
		if (file_info.attrib & _A_SUBDIR) //是目录  
			attribute = "dir";
		else
		{
			attribute = "file";
			//输出文件信息并计数,文件名(带后缀)、文件最后修改时间、文件字节数(文件夹显示0)、文件是否目录  
			//cout << file_info.name << ' ' << file_info.time_write << ' ' << file_info.size << ' ' << attribute << endl; //获得的最后修改时间是time_t格式的长整型，需要用其他方法转成正常时间显示  
			files.push_back(ascPath + "\\" + file_info.name);
		}
	} while (!_findnext(handle, &file_info));  //返回0则遍历完  
	//关闭文件句柄  
	_findclose(handle);

	// 	files.push_back(logFile + "\\client.log");
	// 	files.push_back(logFile + "\\worker.log");
	// 	files.push_back(logFile + "\\cef_log.txt");
	// 	files.push_back(logFile + "\\cef_log_old.txt");

	UC_LOG_FLUSH();
	ZipWrapper::ZipFile(filePath, files);
#endif
    
	triggerEvent(IClientServiceEvent, OnArchiveLogFinish(filePath));
}

void ClientService::GetUpgradeInfo(std::string &upgradeServer, std::string &packageInfo, std::string &packageDownload) {
	upgradeServer = _loginInfo.GetServerInfo(UPGRADE_SERVER).GetDomain();
	packageInfo = _loginInfo.GetServerInfo(PACKAGEINFO_SERVER).GetDomain();
	packageDownload = _loginInfo.GetServerInfo(PACKAGE_DOWNLOAD_SERVER).GetDomain();
}

void ClientService::GetUccServerDomain(std::string &uccServerDomain) {
	uccServerDomain = _loginInfo.GetUccServerDomain();
}

void ClientService::GetPasswordRule(OUT int32_t & days, OUT int32_t & memory, OUT int32_t & complex, OUT int32_t & allow_change)
{
	days = _passwordRule.GetExprisedDays();
	memory = _passwordRule.GetMemory();
	complex = _passwordRule.GetComplex();
	allow_change = _passwordRule.GetAllowChange();
}

void ClientService::EnableCurlSettings(int value)
{
	_libcurlSettings = value;
}

bool ClientService::InitUserDB() {
	if (_engine->GetConfig().appPath.empty()) {
		return false;
	}
	
	// 初始化用户目录
	string userDirectory = ((UCEngine*)_engine)->GetConfig().appPath + "/" + _loginInfo.GetAccount();
	string userDBFile = userDirectory + "/uc_db.sqlite";
	UC_LOG(INFO) << "userDirectory: " << userDirectory;
	UC_LOG(INFO) << "userDBFile: " << userDBFile;
	
	boost::filesystem::path dir(StringConvert::FromUTF8Str(userDirectory));
	if (!boost::filesystem::create_directories(dir)) {
		UC_LOG(INFO) << "create user directory error: " << userDirectory;
	}
	
	boost::filesystem::path dbFile(StringConvert::FromUTF8Str(userDBFile));
	bool addWelcome = false;
	
	if (!boost::filesystem::is_regular_file(dbFile)) {
		// 用户数据数据库文件，不存在则插入一条欢迎的conversation
		// 表示用户是第一次登陆
		addWelcome = true;
	}
	
	int resultCode = DBManager::GetInstance().InitUserDB(userDBFile);
	UC_LOG(INFO) << "init user database: " << resultCode;
	
	if (addWelcome) {
		int64_t msgSeq = Time::GetSysTime();
		int64_t msgTime = Time::GetSysTime();
		
		DBManager::GetInstance().SaveConversationInfo(WELCOME, msgSeq, msgTime);
	}

#ifdef PBX_SERVICE
	userDBFile = userDirectory + "/pbx_db.sqlite";

	resultCode = DBManager::GetInstance().InitPbxDB(userDBFile, "1.0.0.24");
	UC_LOG(INFO) << "Init Pbx database: " << resultCode;
#endif // PBX_SERVICE

	return true;
}

bool ClientService::GetUserStatus(int32_t userId){
	ContactService *contactService = (ContactService*)_engine->QueryService(UCContactService);
	if (contactService != NULL)
	{
		std::list<int32_t> userIds;
		userIds.push_back(userId);
		std::string copyBack("");
		contactService->GetUserStatus(userIds, copyBack);
	}
	return UC_SUCCEEDED;
}

void ClientService::InitPresence(){
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;
	BinMsg data;
	string body;

	boost::property_tree::wptree jsonRoot;
	boost::property_tree::wptree jsonChild;
	boost::property_tree::wptree jsonElement;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		MESSAGE_SEND_METHOD);
	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	UCHead head;
	head.SetVersion(MESSAGE_VERSION);
	head.SetFrom(GetLoginInfo().GetCurUser());
	head.SetTo(GetLoginInfo().GetCurUser());
	head.SetSeq(0);
	head.SetConversation(0);
	head.SetTimestamp(0);
	head.SetControlPri(0);
	head.SetPri(PriType::thrift_type);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetAppId(AppId::AppPresence);
	head.SetProtocoltype(PresenceMessageType::DefaultType);
	head.SetProtocolId(PresenceMessageID::DefaultId);
	head.SetId(0);

	MessagePresenceMsg msg;
	msg.SetHead(head);
	msg.set_pri(0);
	msg.set_show("");
	msg.set_status(PresenceType::Online);
	switch (GetLoginInfo().GetClientType())
	{
	case 1:
		msg.set_deviceType(DeviceType::Iphone);
		break;
	case 2:
		msg.set_deviceType(DeviceType::Android);
		break;
	case 3:
		msg.set_deviceType(DeviceType::PC);
		break;
	case 4:
		msg.set_deviceType(DeviceType::Background);
		break;
	case 5:
		msg.set_deviceType(DeviceType::Ipad);
		break;
	case 6:
		msg.set_deviceType(DeviceType::Web);
	default:
		break;
	}
	
	if (msgProcessor.EncodeMessageData(msg, data, body) != UC_SUCCEEDED) {
		return;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(msg, request)){
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &ClientService::OnSendMessageResponse));
		return;
	}
}

int32_t ClientService::OnSendMessageResponse(int32_t errorCode, HTTPResponse *response){
	UC_LOG(INFO) << "ClientService::OnSendMessageResponse errCode= " << errorCode << ", response=" << (response != NULL ? response->GetContent() : "");
	return UC_SUCCEEDED;
}

void ClientService::SetUpdateUserNotify(int16_t isNotify){
	HTTPRequest		request;
	string			loginURL;
	stringstream	sStream;

	loginURL = UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), USER_SETTING_UPDATE_METHOD);
	request.Init(loginURL, HTTP_METHOD_POST);

	sStream << "session_id=" << _loginInfo.GetSessionId() << "&user_id=" << _loginInfo.GetUserId() 
		<< "&data={\"general\":{\"login_notify\":" << isNotify << "}}";

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnUpdateUserNotifyComplete));

}

int32_t ClientService::OnUpdateUserNotifyComplete(int32_t errorCode, HTTPResponse *response){
	UC_LOG(INFO) << "ClientService::OnUpdateUserNotifyComplete errorCode= " << errorCode 
		<< ", response=" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode())
	{
		rapidjson::Document document;
		if (!document.Parse<0>(response->GetContent().c_str()).HasParseError())
		{
			result = document["code"].GetInt();
		}
		else
		{
			UC_LOG(ERROR) << "encounter an exception when parse json form OnUpdateUserNotifyComplete , exception: " << document.GetParseError();
		}
	}
	else
	{
		result = (errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED));
	}

//	if (UC_SUCCEEDED != result){
		triggerEvent(IClientServiceEvent, OnUpdateUserNotifySet(result,_loginInfo.GetUserId()));
//	}

	return result;
}

void ClientService::GetUserNotifyStatus(){
	HTTPRequest		request;
	string			loginURL;
	stringstream	sStream;

	loginURL = UCEngine::GetRequestURL(_loginInfo.GetUccServerDomain(), USER_SETTING_METHOD);
	request.Init(loginURL, HTTP_METHOD_POST);

	sStream << "session_id=" << _loginInfo.GetSessionId() << "&user_id=" << _loginInfo.GetUserId();

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ClientService::OnGetUserNotifyStatusComplete));
}

int32_t ClientService::OnGetUserNotifyStatusComplete(int32_t errorCode, HTTPResponse *response){
	UC_LOG(INFO) << "ClientService::OnGetUserNotifyStatusComplete errorCode= " << errorCode
		<< ", response=" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	int16_t isNotify = -1;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()){
		rapidjson::Document document;
		if (!document.Parse<0>(response->GetContent().c_str()).HasParseError()){
			result = document["code"].GetInt();
			if (result != 0)
			{
				return result;
			}
			rapidjson::Value &data = document["data"];
			for (rapidjson::SizeType i = 0; i < data.Size(); i++){
				rapidjson::Value &v = data[i];
				if (v.HasMember("general")){
					rapidjson::Value &general = v["general"];
					isNotify = jsonhelper::getIntValue(general, "login_notify", -1);
				}
			}
		}
		else{
			UC_LOG(ERROR) << "encounter an exception when parse json form OnUpdateUserNotifyComplete , exception: " << document.GetParseError();
		}
	}
	else{
		result = (errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED));
	}

//	if (UC_SUCCEEDED != result){
		triggerEvent(IClientServiceEvent, OnGetUserNotifyStatus(result, isNotify));
//	}

	return result;
}

bool ClientService::FindFile(std::string filepath, std::string filename)
{
	std::string tempfilepath = filepath + filename;
	if (!tempfilepath.empty())
	{
		boost::filesystem::path localPath = StringConvert::FromUTF8Str(tempfilepath);
		if (!boost::filesystem::exists(localPath))
		{
			return false;
		}		
	}

	return true;
}

std::string ClientService::GetFileName(std::string filepath, std::string filename, std::string tempfilename, int32_t namecount/* = 1*/)
{
	std::string str;
	std::string strname;

	if (!FindFile(filepath,filename)){
		return filename;
	}

	if (namecount != 1)	{
		filename = tempfilename;
	}

	str = StringConvert::Convert2String(namecount);
	if (filename.find_last_of(".") != string::npos){
		strname = filename.substr(0, filename.find_last_of(".")) + "(" + str + ")";
		strname += filename.substr(filename.find_last_of("."), filename.length());
	}
	else
	{
		strname = filename + "(" + str + ")";
	}
	namecount++;

	return GetFileName(filepath, strname, tempfilename, namecount);
}

std::wstring ClientService::GetDefaultFileSavePath(std::string filename)
{
	UC_LOG(INFO) << "GetDefaultFileSavePath, filename:" << filename;
	std::string logPath = _engine->GetConfig().logConfig.logPath;
	std::string path = logPath.substr(0, logPath.find("log"));
	path += "recvFile\\";

	//供JSON解析"\\ 转换成 \\\\"
	for (size_t i = 0; i < path.length(); i++){
		if (path[i] == '\\'){
			path.insert(i, string("\\"));
			i++;
		}
	}

	std::string str = GetFileName(path, filename, filename);
	path += str;
	UC_LOG(INFO) << "GetDefaultFileSavePath--GetFileName, filepath:" << path;
	std::wstring wfile = StringConvert::FromUTF8Str(path);

	return wfile;
}

int32_t ClientService::UpdateMeetingClient()
{
#if defined(WIN32) || defined(WIN64)
	//如果当前没有安装云会议或只安装了60000版本，则不升级
	int localMeetingClientVersion = GetLocalMeetingClientVersion();
	if (localMeetingClientVersion <= 0)
	{
		UC_LOG(INFO) << "UpdateMeetingClient enter. No upgrades. localMeetingClientVersion = " << localMeetingClientVersion;
		return 0;
	}

	IBee2MeetingPlugin *plugin = (IBee2MeetingPlugin*)sgCreateInstance("IBee2MeetingPlugin");
	if (plugin == NULL) {
		UC_LOG(ERROR) << "UpdateMeetingClient create instance failed.";
		return -1;
	}

	plugin->setBeePluginSink(&_bee2MeetingPluginSink);
	int32_t appId = 6;
	int32_t siteId = 60001;		//默认为非远程控制版本
	int32_t skinId = 1;
	int32_t actionType = 11;
	int32_t timeout = 20 * 1000;
	//云会议暂时只支持升级60001版本
// 	if (GetLoginInfo().IsSupportRemoteAssistant() && (localMeetingClientVersion ==2 || localMeetingClientVersion == 0))
// 		siteId = 60000;

	std::string s = GetLoginInfo().GetUccServerDomain_Pure();
	const char* domain = s.c_str();
	int32_t result = plugin->updateMeetingClient(appId, siteId, skinId, actionType, domain, timeout);
	UC_LOG(INFO) << "UpdateMeetingClient enter. siteId = " << siteId << ", domain = " << domain << ", result = " << result;
	return result;
#else
    return 0;
#endif
}

void ClientService::_RefreshToken(int expiresIn) {
	UC_LOG(INFO) << "refreshToken expiresIn=" << expiresIn;
	cout << "refreshToken expiresIn=" << expiresIn << endl;

	_CancelTimer();

	if (expiresIn > 20) {
		expiresIn -= 10;
	}

	Timer::GetInstance().Schedule(
		MakeCallback(this, &ClientService::GetAccessToken, OGT_REFRESH_TOKEN), expiresIn * 1000, &_timerId);
}

void ClientService::_CancelTimer() {
	if (_timerId > 0) {
		UC_LOG(INFO) << "CancelTimer timerId=" << _timerId;
		cout << "CancelTimer timerId=" << _timerId << endl;
		Timer::GetInstance().Cancel(_timerId);
		_timerId = -1;
	}
}


#if defined(WIN32) || defined(WIN64)
void Bee2MeetingPluginSink::NotifyUpdateResult(int Result)
{
	UC_LOG(INFO) << "NotifyUpdateResult enter, result = " << Result;
}
#endif

} /* namespace uc */
