#include "UCManager.h"
#include "UCExtension.h"
#include "UCDefs.h"
#include "UCErrorCode.h"
#include "LogManager.h"
#include "CefJSConversion.h"
#include "utils/Logger.h"
#include "assist/AudioServiceStrategyAssist.h"
#include "utils/Environment.h"
#include "utils/FileUtil.h"
#include <fstream>
#define GLOBAL_EVENT_SHOW_BEE_MAINWND L"Global\\ShowBeeMainWnd"		//全局事件,用于云会议通知蜜蜂显示主窗口
#define GLOBAL_EVENT_MEETING_CLOSED	L"Global\\MeetingClosed"		//全局事件，用于云会议退出时能知蜜蜂
#define GLOBAL_EVENT_MEETING_JOINED L"Global\\MeetingJoined"		//全局事件，用于云会议入会成功时通知蜜蜂
#define GLOBAL_EVENT_MEETING_FAILED L"Global\\MeetingJoinFailed"	//全局事件，用于云会议入会失败时通知蜜蜂
#define GLOBAL_EVENT_CANCEL_JOIN_MEETING L"Global\\CancelJoinMeeting"	//全局事件，用于通知云会议用户取消了入会
#define GLOBAL_EVENT_MEETING_READIED L"Global\\MeetingReadied"		//全局事件，用于云会议准备成功后通知蜜蜂
#define IS_NEW_MEETING 1						//是否使用新的云会议客户端，1-是，0-否
const int WM_SENDPOSITIO = WM_USER + 1001;		//通知云会议蜜蜂坐标的消息ID
namespace ucclient {

using namespace cefIntegration; 
bool bInMeeting = false;
WaitEventNotify WaitMeetingJoinedThreadProc;
WaitEventNotify WaitMeetingClosedThreadProc;
WaitEventNotify WaitToShowMainWndThreadProc;
WaitEventNotify WaitMeetingJoinFailedThreadProc;
WaitEventNotify WaitMeetingReadiedThreadProc;
static inline int64_t StringToInt64(CefRefPtr<CefListValue> arguments, int index) {
	std::string str = arguments->GetString(index).ToString();
	return StringConvert::Convert2Int64(str);
}

bool UCExtension::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefProcessId source_process, CefRefPtr<CefProcessMessage> message) {
	if (message->GetName() == L"CallNativeMethodAync") {

		CefRefPtr<CefListValue> arguments = message->GetArgumentList();
		int argLen = arguments->GetSize();
	
		bool hasCallback = false;
		int res = UCCLIENT_SUCCESS;
	
		if ((argLen < 4) || (arguments->GetType(0) != VTYPE_INT) || 
			(arguments->GetType(1) != VTYPE_BOOL) || (arguments->GetType(2) != VTYPE_STRING)||
			(arguments->GetType(3) != VTYPE_STRING)) {
			res = UCCLIENT_ARGUMENT_ERROR;

			UC_LOG(ERROR) << "CallNativeMethodAync message argument error, argLen: "
						  << argLen << ", type0: " <<arguments->GetType(0) << ", type1: " <<arguments->GetType(1) << ", type2: " 
						  << arguments->GetType(2) << ", type3: " << arguments->GetType(3) << ", hwnd: " << browser->GetHost()->GetWindowHandle();
		}
		else {
			int requestId = arguments->GetInt(0);
			bool hasCallback = arguments->GetBool(1);
			CefString module = arguments->GetString(2);
			CefString method = arguments->GetString(3);

			CefRefPtr<CefProcessMessage> response = CefProcessMessage::Create("InvokeCallback");
		    CefRefPtr<CefListValue> responseArgs = response->GetArgumentList();

			// 调用UCManager的方法
			if (module == L"ClientService") {
				ClientServiceMethodProcess(method, argLen, arguments, res,responseArgs);
			} // if (module == L"ClientService")
			else if (module == L"GkClient") {
				GkClientMethodProcess(method, argLen, arguments, res, responseArgs);
			} // if (module == L"GkClient")
			else if (module == L"ContactService") {
				ContactServiceMethodProcess(method, argLen, arguments, res, responseArgs);
			} // else if (module == L"ContactService")
			else if (module == L"IMService") {
				IMServiceMethodProcess(method, argLen, arguments, res, responseArgs);
			} // else if (module == L"IMService")
			else if (module == L"GroupService") {
				GroupServiceMethodProcess(method, argLen, arguments, res, responseArgs);
			}
			else if (module == L"CalendarService") {
				CalendarServiceMethodProcess(method, argLen, arguments, res, responseArgs);
			} // else if (module == L"CalendarService")
			else if (module == L"FileTransferService") {
				FileTransferServiceMethodProcess(method, argLen, arguments, res, responseArgs);
			}
#ifdef PBX_SERVICE
			else if (module == L"PbxService") {
				PbxServiceMethodProcess(method, argLen, arguments, res, responseArgs);
			}
#endif // PBX_SERVICE
			else if (module == L"CatchScreenService") {
				hasCallback = false;
				CatchScreenServiceMethodProcess(browser, method, argLen, arguments, res, responseArgs, requestId);
			}
			else if (module == L"SearchService") {
				SearchServiceMethodProcess(method, argLen, arguments, res, responseArgs);
			}
			else if (module == L"System") {
				SystemServiceMethodProcess(browser, method, argLen, arguments, res, responseArgs, requestId, hasCallback);
			}
			else if (module == L"AudioService") {
				AudioServiceMethodProcess(browser, method, argLen, arguments, res, requestId);
			}
			else if (module == L"SharingService") {
				SharingServiceMethodProcess(browser, method, argLen, arguments, res, requestId);
			}
			else if (module == L"Log") {
				LogServiceMethodProcess(method, argLen, arguments, res);
			}
			else if (module == L"App") {
				AppMethodProcess(method, argLen, arguments, res, responseArgs);
			}
			else if (module == L"OpenApiService") {
				OpenApiServiceMethodProcess(browser, method, argLen, arguments, res, responseArgs, hasCallback, requestId);
			}
			else if (module == L"MicroblogService") {
				MicroblogServiceMethodProcess(method, argLen, arguments, res, responseArgs);
			}
			else if (module == L"UpdateService") {
				UpdateServiceMethodProcess(method, argLen, arguments, res, responseArgs, requestId);
			}
			else if (module == L"ExternalService"){
				ExternalServiceMethodProcess(method, argLen, arguments, res, responseArgs);
			}
			else if (module == L"BBSService"){
				BBSServiceMethodProcess(method, argLen, arguments, res, responseArgs);
			}
			else {
				res = UCCLIENT_MODULE_NOTFOUND;
			}		// if (module == L"ClientService") 

			if (hasCallback) {
		 		responseArgs->SetInt(0, requestId);
            	responseArgs->SetInt(1, res);
          
            	browser->SendProcessMessage(PID_RENDERER, response);	
			}

            if (0 != method.ToString().compare("WriteLog")) // 不打印日志打印的方法
            {
                UC_LOG(INFO) << "CallNativeMethodAync result, " << "requestId: " << requestId << ", hasCallback: " << hasCallback
                    << ", module: " << module.ToString() << ", method: " << method.ToString() << ", res: " << res
                    << ", hwnd: " << browser->GetHost()->GetWindowHandle();
            }

			if (res == UCCLIENT_ARGUMENT_ERROR) {
				std::wstring json;
				CefJSConversion::ListToString(arguments, json);
				UC_LOG(ERROR) << "wrong JS parameter: " << json;
			}
		}
		return true;
	}	

	return false;
}


void UCExtension::ClientServiceMethodProcess(CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"Login") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_STRING) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6)== VTYPE_INT)) {
				std::wstring account = arguments->GetString(4).ToWString();
				std::wstring password = arguments->GetString(5).ToWString();
				UCManager::GetInstance().GetClientServiceStrategy()->Login(account, password,arguments->GetInt(6));
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
	}
	else if (method == L"Relogin") {
		UCManager::GetInstance().GetClientServiceStrategy()->Relogin();
	}
	else if (method == L"Logout") {
		UCManager::GetInstance().GetClientServiceStrategy()->Logout();
	}
	else if (method == L"ModifyPassword") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_STRING) &&
			(arguments->GetType(5) == VTYPE_STRING)) {
			std::wstring oldPwd = arguments->GetString(4).ToWString();
			std::wstring newPwd = arguments->GetString(5).ToWString();
			UCManager::GetInstance().GetClientServiceStrategy()->ModifyPassword(oldPwd, newPwd);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"DeleteHistoryAccount") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {
				std::wstring account = arguments->GetString(4).ToWString();
				UCManager::GetInstance().GetClientServiceStrategy()->DeleteHistoryAccount(account);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetAutoLogin") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_STRING) &&
			(arguments->GetType(5) == VTYPE_BOOL)) {
				std::wstring account = arguments->GetString(4).ToWString();
				bool autoLogin = arguments->GetBool(5);
				UCManager::GetInstance().GetClientServiceStrategy()->SetAutoLogin(account, autoLogin);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"RememberPassword") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_STRING) &&
			(arguments->GetType(5) == VTYPE_BOOL)) {
				std::wstring account = arguments->GetString(4).ToWString();
				bool remember = arguments->GetBool(5);
				UCManager::GetInstance().GetClientServiceStrategy()->RememberPassword(account, remember);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ConnectUCAS") {
		UCManager::GetInstance().GetClientServiceStrategy()->ConnectUCAS();
	}
	else if (method == L"DisconnectUCAS") {
		UCManager::GetInstance().GetClientServiceStrategy()->DisconnectUCAS();
	}
	else if (method == L"ConnectUCC") {
		UCManager::GetInstance().GetClientServiceStrategy()->ConnectUCC();
	}
	else if (method == L"DisconnectUCC") {
		UCManager::GetInstance().GetClientServiceStrategy()->DisconnectUCC();
	}
	else if (method == L"GetNewConversationList") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int number = arguments->GetInt(4);
			UCManager::GetInstance().GetClientServiceStrategy()->
				GetNewConversationList(number);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetOldConversationList") {
		if ((argLen == 6) && (CefJSConversion::IsJsNumber(arguments,4)) &&
			(arguments->GetType(5) == VTYPE_INT)) {
			int64_t startTime = CefJSConversion::JSNumberToDouble(arguments,4);
			int number = arguments->GetInt(5);
			UCManager::GetInstance().GetClientServiceStrategy()->
				GetOldConversationList(startTime, number);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetLocalConversations") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int number = arguments->GetInt(4);
			UCManager::GetInstance().GetClientServiceStrategy()->
				GetLocalConversations(number);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetConversationTop") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_INT)) {
			ConversationType type = (ConversationType)arguments->GetInt(4);
			int64_t relateId = CefJSConversion::JSNumberToDouble(arguments, 5);
			int32_t state = arguments->GetInt(6);
			UCManager::GetInstance().GetClientServiceStrategy()->
				SetConversationTop(type, relateId, state);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendHeartBeat") {
		UCManager::GetInstance().GetClientServiceStrategy()->SendHeartBeat();
	}
	else if (method == L"GetCaptchaURL") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {

			std::wstring hashStr = arguments->GetString(4).ToWString();
			std::wstring URL = L"";

			URL = UCManager::GetInstance().GetClientServiceStrategy()->GetCaptchaURL(hashStr);
			responseArgs->SetString(2, URL);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"VerifyAccount") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_STRING)) {

			int32_t verifyType = arguments->GetInt(4);
			std::wstring account = arguments->GetString(5).ToWString();
			std::wstring captcha = arguments->GetString(6).ToWString();
			std::wstring hashStr = arguments->GetString(7).ToWString();
			
			UCManager::GetInstance().GetClientServiceStrategy()->VerifyAccount(verifyType, account, captcha, hashStr);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"VerifyCode") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_INT)) {

			int32_t verifyType = arguments->GetInt(4);
			std::wstring account = arguments->GetString(5).ToWString();
			std::wstring mobileCode = arguments->GetString(6).ToWString();
			int32_t resend = arguments->GetInt(7);
			
			UCManager::GetInstance().GetClientServiceStrategy()->VerifyCode(verifyType, account, mobileCode, resend);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ResetPassword") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_STRING) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_STRING)) {

			std::wstring account = arguments->GetString(4).ToWString();
			std::wstring newPwd = arguments->GetString(5).ToWString();
			std::wstring mobileCode = arguments->GetString(6).ToWString();
			
			UCManager::GetInstance().GetClientServiceStrategy()->ResetPassword(account, newPwd, mobileCode);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	} 
	else if (method == L"GetConfigInfo") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_STRING) && (arguments->GetType(5) == VTYPE_STRING)) {

			std::wstring app = arguments->GetString(4).ToWString();
			std::wstring name = arguments->GetString(5).ToWString();
			std::wstring value = L"";

			res = UCManager::GetInstance().GetClientServiceStrategy()->GetConfigInfo(app, name, value);
			responseArgs->SetString(2, value);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetValueByKey") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_STRING) && (arguments->GetType(5) == VTYPE_STRING)) {

			std::wstring app = arguments->GetString(4).ToWString();
			std::wstring name = arguments->GetString(5).ToWString();
			std::wstring value = L"";

			res = UCManager::GetInstance().GetClientServiceStrategy()->GetValueByKey(app, name, value);
			responseArgs->SetString(2, value);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetConfigInfos") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {

			std::wstring app = arguments->GetString(4).ToWString();
			std::map<wstring, wstring> configs;
			CefRefPtr<CefDictionaryValue> dicValues = CefDictionaryValue::Create();

			res = UCManager::GetInstance().GetClientServiceStrategy()->GetConfigInfos(app, configs);
			std::map<wstring, wstring>::const_iterator itor = configs.begin();
			while (itor != configs.end()) {
				dicValues->SetString(itor->first, itor->second);
				itor++;
			}
			responseArgs->SetDictionary(2, dicValues);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateConfigInfo") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_STRING) && (arguments->GetType(5) == VTYPE_STRING) &&
			(arguments->GetType(6) == VTYPE_STRING)) {

			std::wstring app = arguments->GetString(4).ToWString();
			std::wstring name = arguments->GetString(5).ToWString();
			std::wstring value = arguments->GetString(6).ToWString();

			res = UCManager::GetInstance().GetClientServiceStrategy()->UpdateConfigInfo(app, name, value);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateValueByKey") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_STRING) && (arguments->GetType(5) == VTYPE_STRING) &&
			(arguments->GetType(6) == VTYPE_STRING)) {

			std::wstring app = arguments->GetString(4).ToWString();
			std::wstring name = arguments->GetString(5).ToWString();
			std::wstring value = arguments->GetString(6).ToWString();

			res = UCManager::GetInstance().GetClientServiceStrategy()->UpdateValueBykey(app, name, value);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateConfigInfos") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_STRING) && (arguments->GetType(5) == VTYPE_DICTIONARY)) {

			std::wstring app = arguments->GetString(4).ToWString();
			CefRefPtr<CefDictionaryValue> dicValues = arguments->GetDictionary(5);
			std::vector<CefString> dicKeys;
			std::map<wstring, wstring> configInfos;
			CefString key;
			CefString value;
			
			dicValues->GetKeys(dicKeys);
			for (int index = 0; index < dicKeys.size(); index++) {
				key = dicKeys[index];
				value = dicValues->GetString(key);
				configInfos.insert(pair<wstring, wstring>(key, value));
			}

			res = UCManager::GetInstance().GetClientServiceStrategy()->UpdateConfigInfos(app, configInfos);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetProxyAuthInfo") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_STRING) &&
			(arguments->GetType(6) == VTYPE_STRING)) {
			int32_t authType = arguments->GetInt(4);
			std::wstring account = arguments->GetString(5).ToWString();
			std::wstring password = arguments->GetString(6).ToWString();
			UCManager::GetInstance().GetClientServiceStrategy()->SetProxyInfo(authType, account, password);
			UCManager::GetInstance().GetGkClient()->SetProxyAuthInfo(authType, account, password);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UploadLog") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_STRING) && (arguments->GetType(5) == VTYPE_STRING) &&
			(arguments->GetType(6) == VTYPE_STRING)) {
			std::wstring logURL = arguments->GetString(4).ToWString();
			std::wstring desc = arguments->GetString(5).ToWString();
			std::wstring version = arguments->GetString(6).ToWString();
			UCManager::GetInstance().GetClientServiceStrategy()->UploadLog(logURL, desc, version);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetPwdExpiredDays") {
		if (argLen == 4) {

			int days = UCManager::GetInstance().GetClientServiceStrategy()->GetPwdExpiredDays();

			responseArgs->SetInt(2, days);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetSessionId") {
		if (argLen == 4) {
			std::wstring sessionId = UCManager::GetInstance().GetClientServiceStrategy()->GetSessionId();
			responseArgs->SetString(2, sessionId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ArchiveLogFile") {
		if (argLen == 4) {
			UCManager::GetInstance().GetClientServiceStrategy()->ArchiveLogFile();
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetPasswordRule"){
		if (argLen == 4){
			int32_t days, memory, complex, allow_change;
			UCManager::GetInstance().GetClientServiceStrategy()->GetPasswordRule(days, memory, complex, allow_change);
			responseArgs->SetInt(2, days);
			responseArgs->SetInt(3, memory);
			responseArgs->SetInt(4, complex);
			responseArgs->SetInt(5, allow_change);
		}
	}
	else if (method == L"GetUccServerDomain"){
		if (argLen == 4){
			std::wstring uccDomain = UCManager::GetInstance().GetClientServiceStrategy()->GetUccServerDomain();
			responseArgs->SetString(2, uccDomain);
		}
	}
	else if (method == L"DeleteSession"){
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT) && (arguments->GetType(5) == VTYPE_INT)) {
			ConversationType type = (ConversationType)arguments->GetInt(4);
			int64_t relateId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			UCManager::GetInstance().GetClientServiceStrategy()->DeleteSession(type,relateId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetUpdateUserNotify")
	{
		if (argLen == 5 && arguments->GetType(4) == VTYPE_INT){
			int16_t isNotity = arguments->GetInt(4);
			UCManager::GetInstance().GetClientServiceStrategy()->SetUpdateUserNotify(isNotity);
		}
	}
	else if (method == L"GetUserNotifyStatus")
	{
		if (argLen == 4)
		{
			UCManager::GetInstance().GetClientServiceStrategy()->GetUserNotifyStatus();
		}
	}
	else {
		res = UCCLIENT_METHOD_NOTFOUND;
	}
}

void UCExtension::GkClientMethodProcess(CefString method, int argLen,
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"GetGkClientConnection") {
		res = UCCLIENT_SUCCESS;
		GkClientConnection conn = UCManager::GetInstance().GetGkClient()->GetGkClientConnection();
		responseArgs->SetInt(2, conn);
	}
	else if (method == L"Relogin") {
		res = UCCLIENT_SUCCESS;
		UCManager::GetInstance().GetGkClient()->GkClientLogin();
	}
	else {
		res = UCCLIENT_METHOD_NOTFOUND;
	}
}

void UCExtension::ContactServiceMethodProcess(CefString method, int argLen,
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"GetUpdatedContacts") {
		UCManager::GetInstance().GetContactServiceStrategy()->GetUpdatedContacts();
	}
	else if (method == L"GetContactInfo") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int userId = arguments->GetInt(4);
			ContactInfo info;
			bool localHas = UCManager::GetInstance().GetContactServiceStrategy()->
				GetContactInfo(userId, info);

			if (localHas) {
				res = UCCLIENT_SUCCESS;
				CefRefPtr<CefDictionaryValue> contactDictionary = ConvertContactToDictionary(info);
				responseArgs->SetDictionary(2, contactDictionary);
			}
			else {
				res = UCCLIENT_ERROR;
			}
		}
		else if ((argLen == 5) && (arguments->GetType(4) == VTYPE_LIST)) {
			CefRefPtr<CefListValue> ceflist = arguments->GetList(4);
			std::list<int32_t> userIds;
			for (std::size_t i = 0; i < ceflist->GetSize(); ++i) {
				if (ceflist->GetType(i) == VTYPE_INT) {
					int userId = ceflist->GetInt(i);
					if (userId > 0) {
						userIds.push_back(ceflist->GetInt(i));
					}
				}
			}
			if (userIds.size() > 0) {
//				UCManager::GetInstance().GetContactServiceStrategy()->GetContactInfo(userIds);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetBasicContactInfo") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int userId = arguments->GetInt(4);
			BasicContactInfo info;
			bool localHas = UCManager::GetInstance().GetContactServiceStrategy()->
				GetBasicContactInfo(userId, info);

			if (localHas) {
				res = UCCLIENT_SUCCESS;
				CefRefPtr<CefDictionaryValue> contactDictionary = ConvertBasicContactToDictionary(info);
				responseArgs->SetDictionary(2, contactDictionary);
			}
			else {
				res = UCCLIENT_ERROR;
			}

		}
		else if ((argLen == 5) && (arguments->GetType(4) == VTYPE_LIST)) {
			CefRefPtr<CefListValue> ceflist = arguments->GetList(4);
			std::list<int32_t> userIds;
			for (std::size_t i = 0; i < ceflist->GetSize(); ++i) {
				if (ceflist->GetType(i) == VTYPE_INT) {
					int userId = ceflist->GetInt(i);
					if (userId > 0) {
						userIds.push_back(ceflist->GetInt(i));
					}
				}
			}
			if (userIds.size() > 0) {
				UCManager::GetInstance().GetContactServiceStrategy()->
					GetBasicContactInfo(userIds);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetContactList") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_INT)) {
			int position = arguments->GetInt(4);
			int len = arguments->GetInt(5);
			std::vector<BasicContactInfo> contacts;
			res = UCManager::GetInstance().GetContactServiceStrategy()->
				GetContactList(position, len, contacts);
			if (res == UC_SUCCEEDED) {
				CefRefPtr<CefListValue> contactlist = CefListValue::Create();
				for (std::size_t i = 0; i < contacts.size(); ++i) {
					CefRefPtr<CefDictionaryValue> contactDictionary = ConvertBasicContactToDictionary(contacts[i]);
					contactlist->SetDictionary(i, contactDictionary);
				}

				responseArgs->SetList(2, contactlist);
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateContactAvatar") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_STRING) &&
			(arguments->GetType(6) == VTYPE_STRING)) {
			int32_t userId = arguments->GetInt(4);
			std::wstring remoteAvatarUrl = arguments->GetString(5).ToWString();
			std::wstring localAvatarPath = arguments->GetString(6).ToWString();
			UCManager::GetInstance().GetContactServiceStrategy()->
				UpdateContactAvatar(userId, remoteAvatarUrl, localAvatarPath);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"AddCommonContact") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t userId = arguments->GetInt(4);
			UCManager::GetInstance().GetContactServiceStrategy()->
				AddCommonContact(userId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"DeleteCommonContact") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t userId = arguments->GetInt(4);
			UCManager::GetInstance().GetContactServiceStrategy()->
				DeleteCommonContact(userId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SearchLocalContact") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_STRING)) {
			ContactInfo::Type type = ContactInfo::Type(arguments->GetInt(4));
			std::wstring keyword = arguments->GetString(5).ToWString();
			std::list<ContactInfo> contacts;
			res = UCManager::GetInstance().GetContactServiceStrategy()->
				SearchLocalContact(type, keyword, contacts);
			if (res == UC_SUCCEEDED) {
				CefRefPtr<CefListValue> contactlist = CefListValue::Create();
				int i = 0;
				for (std::list<ContactInfo>::iterator iter = contacts.begin();
					iter != contacts.end(); ++iter) {
					CefRefPtr<CefDictionaryValue> contactDictionary = ConvertContactToDictionary(*iter);
					contactlist->SetDictionary(i++, contactDictionary);
				}
				responseArgs->SetList(2, contactlist);
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetCurrentUserInfo") {
		ContactInfo contact;
		bool isOk = UCManager::GetInstance().GetContactServiceStrategy()->
			GetCurrentUserInfo(contact);
		if (isOk) {
			CefRefPtr<CefDictionaryValue> contactDictionary = ConvertContactToDictionary(contact);
			responseArgs->SetDictionary(2, contactDictionary);
		}
		else {
			res = UCCLIENT_ERROR;
		}
	}
	else if (method == L"UpdateUserInfo") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {
			std::wstring updateField = arguments->GetString(4).ToWString();
			UCManager::GetInstance().GetContactServiceStrategy()->
				UpdateUserInfo(updateField);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateUserTags") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {

			std::wstring updateTags = arguments->GetString(4).ToWString();
			UCManager::GetInstance().GetContactServiceStrategy()->
				UpdateUserTags(updateTags);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetColleagues") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t userId = arguments->GetInt(4);
			UCManager::GetInstance().GetContactServiceStrategy()->
				GetColleagues(userId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetOrgAddressBook") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t orgId = arguments->GetInt(4);
			int32_t taskId = 0;
			UCManager::GetInstance().GetContactServiceStrategy()->
				GetOrgAddressBook(orgId, taskId);
			responseArgs->SetInt(2, taskId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetOrgMember") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_INT) && (arguments->GetType(5) == VTYPE_INT)) {
			int32_t orgId = arguments->GetInt(4);
			int32_t limit = arguments->GetInt(5);
			std::wstring copyback = arguments->GetString(6);
			UCManager::GetInstance().GetContactServiceStrategy()->GetOrgMembers(orgId, limit, copyback);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetUserStatus") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_INT) && (arguments->GetType(6) == VTYPE_STRING)) {
			int32_t userId = arguments->GetInt(4);
			int32_t status = arguments->GetInt(5);
			wstring show = arguments->GetString(6).ToWString();
			res = UCManager::GetInstance().GetContactServiceStrategy()->SetUserStatus(userId, status, show);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SubscribeUserStatus") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_LIST)) {
			CefRefPtr<CefListValue> tagList = arguments->GetList(4);
			std::list<int32_t> userIds;
			for (std::size_t i = 0; i < tagList->GetSize(); ++i) {
				if (tagList->GetType(i) == VTYPE_INT) {
					userIds.push_back(tagList->GetInt(i));
				}
			}
			if (userIds.size() > 0) {
				res = UCManager::GetInstance().GetContactServiceStrategy()->SubscribeUserStatus(userIds);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UnsubscribeUserStatus") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_LIST)) {
			CefRefPtr<CefListValue> tagList = arguments->GetList(4);
			std::list<int32_t> userIds;
			for (std::size_t i = 0; i < tagList->GetSize(); ++i) {
				if (tagList->GetType(i) == VTYPE_INT) {
					userIds.push_back(tagList->GetInt(i));
				}
			}
			if (userIds.size() > 0) {
				res = UCManager::GetInstance().GetContactServiceStrategy()->UnsubscribeUserStatus(userIds);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
	}
	else if (method == L"GetUserStatus"){
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_LIST) && (arguments->GetType(5) == VTYPE_STRING)){
			CefRefPtr<CefListValue> userIdList = arguments->GetList(4);
			std::list<int32_t> userIds;
			for (std::size_t i = 0; i < userIdList->GetSize(); ++i) {
				if (userIdList->GetType(i) == VTYPE_INT) {
					userIds.push_back(userIdList->GetInt(i));
				}
			}

			wstring copyBack = arguments->GetString(5).ToWString();
			if (userIds.size() > 0)
			{
				res = UCManager::GetInstance().GetContactServiceStrategy()->GetUserStatus(userIds, copyBack);
			}
			else{
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
else if (method == L"UnsubscribeAllUserStatus") {
		if ((argLen == 4)) {
			res = UCManager::GetInstance().GetContactServiceStrategy()->UnsubscribeAllUserStatus();
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else {
		res = UCCLIENT_METHOD_NOTFOUND;
	}
}

void UCExtension::IMServiceMethodProcess(CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"GetChatHistory") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_INT) && (CefJSConversion::IsJsNumber(arguments, 5)) &&
			(arguments->GetType(6) == VTYPE_INT) && (CefJSConversion::IsJsNumber(arguments,7)) &&
			(CefJSConversion::IsJsNumber(arguments,8))) {
			    MessageChannel channel = (MessageChannel)arguments->GetInt(4);
				int64_t userId = (int64_t)CefJSConversion::JSNumberToDouble(arguments,5);
				int32_t count = arguments->GetInt(6);
				int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments,7);
				int64_t endTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments,8);
				res = UCManager::GetInstance().GetIMStrategy()->GetChatHistory(channel, userId, count, startTime, endTime);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ResendMessage") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t msgId = arguments->GetInt(4);
			res = UCManager::GetInstance().GetIMStrategy()->ResendMessage(msgId);
			responseArgs->SetInt(2, msgId);	
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ForwardMessage") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_DICTIONARY) &&
			(arguments->GetType(5) == VTYPE_INT)) {
				UCID ucid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), ucid)) {
					int32_t msgId = arguments->GetInt(5);
					res = UCManager::GetInstance().GetIMStrategy()->ForwardMessage(ucid, msgId);
					responseArgs->SetInt(2, msgId);	
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendTextMessage") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_INT) && (arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_STRING)) {
				UCID ucid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), ucid)) {
					int32_t textType = arguments->GetInt(5);
					std::wstring textFormat = arguments->GetString(6).ToWString();
					std::wstring textData = arguments->GetString(7).ToWString();
					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->SendTextMessage(ucid, textType, textFormat, textData, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendImageMessage") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_INT) &&
			(arguments->GetType(7) == VTYPE_STRING) &&(arguments->GetType(8) == VTYPE_STRING)) {
				UCID ucid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), ucid)) {
					std::wstring imageName = arguments->GetString(5).ToWString();
					int32_t imageSize = arguments->GetInt(6);
					std::wstring imageURL = arguments->GetString(7).ToWString();
					std::wstring imagePath = arguments->GetString(8).ToWString();

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendImageMessage(ucid, imageName, imageSize, imageURL, imagePath, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendAudioMessage") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_INT) &&
			(arguments->GetType(7) == VTYPE_STRING) &&(arguments->GetType(8) == VTYPE_INT)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					std::wstring audioName = arguments->GetString(5).ToWString();
					int32_t audioSize = arguments->GetInt(6);
					std::wstring audioURL = arguments->GetString(7).ToWString();
					int32_t audioDuration = arguments->GetInt(8);

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendAudioMessage(userId, audioName, audioSize, audioURL, audioDuration, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendVideoMessage") {
		if ((argLen == 10) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_INT) &&
			(arguments->GetType(7) == VTYPE_STRING) && (arguments->GetType(8) == VTYPE_STRING) &&
			(arguments->GetType(9) == VTYPE_INT)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					std::wstring videoName = arguments->GetString(5).ToWString();
					int32_t videoSize = arguments->GetInt(6);
					std::wstring videoURL = arguments->GetString(7).ToWString();
					std::wstring videoPath = arguments->GetString(8).ToWString();
					int32_t videoDuration = arguments->GetInt(9);

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendVideoMessage(userId, videoName, videoSize, videoURL, videoPath, videoDuration, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendReportMessage") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_DICTIONARY) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_STRING)) {
			UCID userId;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
				std::wstring reportTitle = arguments->GetString(5).ToWString();
				std::wstring reportURL = arguments->GetString(6).ToWString();
				int32_t msgId;

				res = UCManager::GetInstance().GetIMStrategy()->
					SendReportMessage(userId, reportTitle, reportURL, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendRecordVideoMessage") {
		if ((argLen == 14) && (arguments->GetType(4) == VTYPE_DICTIONARY) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_INT) &&
			(arguments->GetType(7) == VTYPE_STRING) && (arguments->GetType(8) == VTYPE_STRING) &&
			(arguments->GetType(9) == VTYPE_STRING) && (arguments->GetType(10) == VTYPE_INT) && 
			(arguments->GetType(11) == VTYPE_INT) && (arguments->GetType(12) == VTYPE_STRING) &&
			(arguments->GetType(13) == VTYPE_STRING)) {
			UCID userId;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
				std::wstring videoName = arguments->GetString(5).ToWString();
				int32_t videoSize = arguments->GetInt(6);
				std::wstring videoURL = arguments->GetString(7).ToWString();
				std::wstring videoThumb = arguments->GetString(8).ToWString();
				std::wstring videoThumbURL = arguments->GetString(9).ToWString();
				int32_t videoDuration = arguments->GetInt(10);
				int32_t operatorId = arguments->GetInt(11);
				std::wstring operatorName = arguments->GetString(12).ToWString();
				std::wstring operatorAvatar = arguments->GetString(13).ToWString();

				int32_t msgId;
				res = UCManager::GetInstance().GetIMStrategy()->
					SendRecordVideoMessage(userId, videoName, videoSize, videoURL, videoThumb, videoThumbURL, videoDuration, operatorId, operatorName, operatorAvatar, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendFileMessage") {
		if ((argLen == 10) && (arguments->GetType(4) == VTYPE_DICTIONARY)&& (arguments->GetType(5) == VTYPE_INT) &&
			(arguments->GetType(6) == VTYPE_STRING) && (arguments->GetType(7) == VTYPE_INT) &&
			(arguments->GetType(8) == VTYPE_STRING) && CefJSConversion::IsJsNumber(arguments,9)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					int32_t operation = arguments->GetInt(5);
					std::wstring fileName = arguments->GetString(6).ToWString();
					int32_t fileSize = arguments->GetInt(7);
					std::wstring fileURL = arguments->GetString(8).ToWString();
					int64_t fileId = (int64_t)CefJSConversion::JSNumberToDouble(arguments,9);

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendFileMessage(userId, operation, fileName, fileSize, fileURL, fileId, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendCloudFileMessage") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_DICTIONARY) && (arguments->GetType(5) == VTYPE_STRING) &&
			(arguments->GetType(6) == VTYPE_STRING) && (arguments->GetType(7) == VTYPE_INT) && (arguments->GetType(8) == VTYPE_STRING)) {
			UCID userId;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
				std::wstring& fileName = arguments->GetString(5).ToWString();
				int64_t fileSize = StringToInt64(arguments, 6);
				DocumentCloudType cloudType = DocumentCloudType(arguments->GetInt(7));
				std::wstring detailContent = arguments->GetString(8).ToWString();

				int32_t msgId;
				res = UCManager::GetInstance().GetIMStrategy()->SendCloudFileMessage(userId, fileName, fileSize,
					cloudType, detailContent, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"DeleteFileMessage") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_STRING) && CefJSConversion::IsJsNumber(arguments,6)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					std::wstring fileName = arguments->GetString(5).ToWString();
					int64_t fileId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,6);

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						DeleteFileMessage(userId, fileName, fileId, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendCodeMessage"){
		if ((argLen == 15) && (arguments->GetType(4) == VTYPE_DICTIONARY)&& 
			(arguments->GetType(5) == VTYPE_INT) && CefJSConversion::IsJsNumber(arguments,6) && 
			(arguments->GetType(7) == VTYPE_STRING) && (arguments->GetType(8) == VTYPE_STRING) &&
			(arguments->GetType(9) == VTYPE_STRING) && (arguments->GetType(10) == VTYPE_STRING) &&
			(arguments->GetType(11) == VTYPE_INT) && (arguments->GetType(12) == VTYPE_STRING) &&
			(arguments->GetType(13) == VTYPE_INT) && CefJSConversion::IsJsNumber(arguments,14)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					int32_t operation = arguments->GetInt(5);
					int64_t codeId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,6);
					std::wstring codeTitle = arguments->GetString(7).ToWString();
					std::wstring langType = arguments->GetString(8).ToWString();
					std::wstring codeHead = arguments->GetString(9).ToWString();
					std::wstring codeDesc = arguments->GetString(10).ToWString();
					int32_t codeSize = arguments->GetInt(11);
					std::wstring codeUrl = arguments->GetString(12).ToWString();
					int32_t codeLine = arguments->GetInt(13);
					int64_t createTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,14);
					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendCodeMessage(userId, operation, codeId, codeTitle, langType, codeHead, 
						codeDesc, codeSize, codeUrl, codeLine, createTime, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendInviteCallMessage") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_INT)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					std::wstring confId = arguments->GetString(5).ToWString();
					std::wstring confPwd = arguments->GetString(6).ToWString();
					int32_t hostId = arguments->GetInt(7);

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendInviteCallMessage(userId, confId, confPwd, hostId, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendAcceptCallMessage") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_STRING) &&
			(arguments->GetType(6) == VTYPE_INT)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					std::wstring confId = arguments->GetString(5).ToWString();
					int32_t hostId = arguments->GetInt(6);

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendAcceptCallMessage(userId, confId, hostId, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendRejectCallMessage") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_STRING) && 
			(arguments->GetType(6) == VTYPE_INT) &&
			(arguments->GetType(7) == VTYPE_STRING)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					std::wstring confId = arguments->GetString(5).ToWString();
					int32_t hostId = arguments->GetInt(6);
					std::wstring reason = arguments->GetString(7).ToWString();

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendRejectCallMessage(userId, confId, hostId, reason, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendFinishCallMessage") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_STRING) &&
			(arguments->GetType(6) == VTYPE_INT) &&
			(arguments->GetType(7) == VTYPE_STRING)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					std::wstring confId = arguments->GetString(5).ToWString();
					int32_t hostId = arguments->GetInt(6);
					std::wstring reason = arguments->GetString(7).ToWString();

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendFinishCallMessage(userId, confId, hostId, reason, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendCancelCallMessage") {
		if ((argLen == 6) && 
			(arguments->GetType(4) == VTYPE_DICTIONARY) && 
			(arguments->GetType(5) == VTYPE_STRING)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {

					std::string phoneNum = arguments->GetString(5);
					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendCancelCallMessage(userId, phoneNum, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	/*
	else if (method == L"SendNotRespondCallRecordMessage") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_DICTIONARY)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendNotRespondCallRecordMessage(userId, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendRejectCallRecordMessage") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_DICTIONARY)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendRejectCallRecordMessage(userId, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	*/
	else if (method == L"SendConnectedCallRecordMessage") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_DICTIONARY) &&
			(arguments->GetType(5) == VTYPE_INT) && 
			(arguments->GetType(6) == VTYPE_STRING)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					int32_t druation = arguments->GetInt(5);
					std::string phoneNum = arguments->GetString(6).ToString();

					int32_t msgId;
					res = UCManager::GetInstance().GetIMStrategy()->
						SendConnectedCallRecordMessage(userId, druation, phoneNum, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendMessageRead") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_DICTIONARY) &&
			(arguments->GetType(5) == VTYPE_INT) && (CefJSConversion::IsJsNumber(arguments, 6))) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					int32_t msgId = arguments->GetInt(5);
					int64_t msgSeq = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 6);

					res = UCManager::GetInstance().GetIMStrategy()->
						SendMessageRead(userId, msgId, msgSeq);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendMessagesReadById") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t userId = arguments->GetInt(4);

			res = UCManager::GetInstance().GetIMStrategy()->
				SendMessagesReadById(userId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendMessagePlayed") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_DICTIONARY) &&
			(arguments->GetType(5) == VTYPE_INT) && CefJSConversion::IsJsNumber(arguments,6)) {
				UCID userId;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), userId)) {
					int32_t msgId = arguments->GetInt(5);
					int64_t msgSeq = (int64_t) CefJSConversion::JSNumberToDouble(arguments,6);

					res = UCManager::GetInstance().GetIMStrategy()->
						SendMessagePlayed(userId, msgId, msgSeq);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else {
		res = UCCLIENT_METHOD_NOTFOUND;
	}
}

void UCExtension::GroupServiceMethodProcess(CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"GetChatHistory") {
		if ((argLen == 10) && (arguments->GetType(4) == VTYPE_INT)  &&(arguments->GetType(5) == VTYPE_INT) &&
			CefJSConversion::IsJsNumber(arguments,6) && (arguments->GetType(7) == VTYPE_INT) &&
			CefJSConversion::IsJsNumber(arguments,8) && CefJSConversion::IsJsNumber(arguments,9)) {

			MessageChannel channel = (MessageChannel)arguments->GetInt(4);;
			ConversationType type = (ConversationType)arguments->GetInt(5);
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments,6);
			int32_t count = arguments->GetInt(7);
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments,8);
			int64_t endTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments,9);

			res = UCManager::GetInstance().GetGroupServiceStrategy()->GetChatHistory(channel, type, groupId, count, startTime, endTime);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetSearchChatHistory") {
		if ((argLen == 10) && (arguments->GetType(4) == VTYPE_INT) &&
			CefJSConversion::IsJsNumber(arguments, 5) && (arguments->GetType(6) == VTYPE_INT) &&
			CefJSConversion::IsJsNumber(arguments, 7) && CefJSConversion::IsJsNumber(arguments, 8) &&
			(arguments->GetType(9) == VTYPE_INT)) {

			ConversationType type = (ConversationType)arguments->GetInt(4);
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			int32_t count = arguments->GetInt(6);
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 7);
			int64_t endTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 8);
			int32_t reverse = arguments->GetInt(9);
			res = UCManager::GetInstance().GetGroupServiceStrategy()->GetSearchChatHistory(type, groupId, count, startTime, endTime, reverse);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ResendMessage") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t msgId = arguments->GetInt(4);
			res = UCManager::GetInstance().GetGroupServiceStrategy()->ResendMessage(msgId);
			responseArgs->SetInt(2, msgId);	
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ForwardMessage"){
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_INT)) {
				UCID ucid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), ucid)) {
					int32_t msgId = arguments->GetInt(5);
					res = UCManager::GetInstance().GetGroupServiceStrategy()->ForwardMessage(ucid, msgId);
					responseArgs->SetInt(2, msgId);	
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"RevocationMessage"){
		if ((argLen == 7) && CefJSConversion::IsJsNumber(arguments, 4) && (arguments->GetType(5) == VTYPE_INT) &&
			(CefJSConversion::IsJsNumber(arguments, 6))){
			
			int64_t touserId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			ConversationType type = ConversationType(arguments->GetInt(5));
			int64_t seqId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 6);
			int32_t msgId;
			res = UCManager::GetInstance().GetGroupServiceStrategy()->RevocationMessage(touserId, type, seqId, msgId);
			responseArgs->SetInt(2, msgId);
		}
		else{
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendTextMessage") {
		if ((argLen == 10) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) && (arguments->GetType(6) == VTYPE_INT) && 
			(arguments->GetType(7) == VTYPE_STRING) && (arguments->GetType(8) == VTYPE_STRING) &&
			(arguments->GetType(9) == VTYPE_LIST)) {
				UCID groupJid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
					ConversationType type = ConversationType(arguments->GetInt(4));
					int8_t textType = arguments->GetInt(6);
					std::wstring textFormat = arguments->GetString(7).ToWString();
					std::wstring textData = arguments->GetString(8).ToWString();
					CefRefPtr<CefListValue> ceflist = arguments->GetList(9);
					std::vector<UCID> atUsers;
					for (std::size_t i = 0; i < ceflist->GetSize(); ++i) {
						UCID ucid;
						if(ConvertDictionaryToUCID(ceflist->GetDictionary(i), ucid)) {
							atUsers.push_back(ucid);
						}
					}

					int32_t msgId;
					res = UCManager::GetInstance().GetGroupServiceStrategy()->
						SendTextMessage(type, groupJid, textType, textFormat, textData, atUsers, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendImageMessage") {
		if ((argLen == 10) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY)&&
			(arguments->GetType(6) == VTYPE_STRING) && (arguments->GetType(7) == VTYPE_INT) &&
			(arguments->GetType(8) == VTYPE_STRING) && (arguments->GetType(9) == VTYPE_STRING)) {
				UCID groupJid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
					ConversationType type = ConversationType(arguments->GetInt(4));
					std::wstring imageName = arguments->GetString(6).ToWString();
					int32_t imageSize = arguments->GetInt(7);
					std::wstring imageURL = arguments->GetString(8).ToWString();
					std::wstring imagePath = arguments->GetString(9).ToWString();

					int32_t msgId;
					res = UCManager::GetInstance().GetGroupServiceStrategy()->
						SendImageMessage(type, groupJid, imageName, imageSize, imageURL, imagePath, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendAudioMessage") {
		if ((argLen == 10) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY)&&
			(arguments->GetType(6) == VTYPE_STRING) && (arguments->GetType(7) == VTYPE_INT) &&
			(arguments->GetType(8) == VTYPE_STRING) && (arguments->GetType(9) == VTYPE_INT)) {
				UCID groupJid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
					ConversationType type = ConversationType(arguments->GetInt(4));
					std::wstring audioName = arguments->GetString(6).ToWString();
					int32_t audioSize = arguments->GetInt(7);
					std::wstring audioURL = arguments->GetString(8).ToWString();
					int32_t audioDuration = arguments->GetInt(8);

					int32_t msgId;
					res = UCManager::GetInstance().GetGroupServiceStrategy()->
						SendAudioMessage(type, groupJid, audioName, audioSize, 
						audioURL, audioDuration, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendVideoMessage") {
		if ((argLen == 11) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) &&
			(arguments->GetType(6) == VTYPE_STRING) && (arguments->GetType(7) == VTYPE_INT) &&
			(arguments->GetType(8) == VTYPE_STRING) && (arguments->GetType(9) == VTYPE_STRING)&& 
			(arguments->GetType(10) == VTYPE_INT)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));
				std::wstring videoName = arguments->GetString(6).ToWString();
				int32_t videoSize = arguments->GetInt(7);
				std::wstring videoURL = arguments->GetString(8).ToWString();
				std::wstring videoPath = arguments->GetString(9).ToWString();
				int32_t videoDuration = arguments->GetInt(10);

				int32_t msgId;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
						SendVideoMessage(type, groupJid, videoName, videoSize, videoURL, 
						videoPath, videoDuration, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendReportMessage") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_INT) && (arguments->GetType(5) == VTYPE_DICTIONARY) &&
			(arguments->GetType(6) == VTYPE_STRING) && (arguments->GetType(7) == VTYPE_STRING)) {
			UCID userId;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), userId)) {
				ConversationType type = ConversationType(arguments->GetInt(4));
				std::wstring reportTitle = arguments->GetString(6).ToWString();
				std::wstring reportURL = arguments->GetString(7).ToWString();
				int32_t msgId;

				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendReportMessage(type, userId, reportTitle, reportURL, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendCloudFileMessage") {
		if ((argLen == 14) && (arguments->GetType(4) == VTYPE_INT) && (arguments->GetType(5) == VTYPE_INT) &&
			(arguments->GetType(6) == VTYPE_DICTIONARY) && (arguments->GetType(7) == VTYPE_STRING) && 
			(arguments->GetType(8) == VTYPE_STRING) && arguments->GetType(9) == VTYPE_STRING && (arguments->GetType(10) == VTYPE_INT) &&
			(arguments->GetType(11) == VTYPE_INT) && (arguments->GetType(12) == VTYPE_INT) &&
			(arguments->GetType(13) == VTYPE_STRING)) {
			UCID groupUid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(6), groupUid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));
				CloudFileOperation operation = CloudFileOperation(arguments->GetInt(5));
				std::wstring fileName = arguments->GetString(7).ToWString();
				std::wstring filenameTo = arguments->GetString(8).ToWString();
				int64_t fileSize = StringToInt64(arguments, 9);
				int8_t count = arguments->GetInt(10);
				int8_t isDir = arguments->GetInt(11);
				DocumentCloudType cloudType = DocumentCloudType(arguments->GetInt(12));
				std::wstring detailContent = arguments->GetString(13).ToWString();

				int32_t msgId;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->SendCloudFileMessage(type, operation, groupUid, fileName, filenameTo, fileSize,
					count, isDir, cloudType, detailContent, true, msgId);

				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UploadCloudDocument") {
		if ((argLen == 13) && (CefJSConversion::IsJsNumber(arguments, 4)) && (arguments->GetType(5) == VTYPE_INT) &&
			(arguments->GetType(6) == VTYPE_INT) && (arguments->GetType(7) == VTYPE_INT) && 
			(arguments->GetType(8) == VTYPE_STRING) && (arguments->GetType(9) == VTYPE_STRING) &&
			(arguments->GetType(10) == VTYPE_INT) && (arguments->GetType(11) == VTYPE_STRING) && 
			(arguments->GetType(12) == VTYPE_STRING)) {
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int32_t operation = arguments->GetInt(5);
			int32_t isSendMessage = arguments->GetInt(6);
			int32_t mountId = arguments->GetInt(7);
			std::wstring fileName = arguments->GetString(8).ToWString();
			int64_t fileSize = StringToInt64(arguments, 9);
			DocumentCloudType cloudType = DocumentCloudType(arguments->GetInt(10));
			std::wstring hash = arguments->GetString(11).ToWString();
			std::wstring detailContent = arguments->GetString(12).ToWString();

			int32_t msgId;
			res = UCManager::GetInstance().GetGroupServiceStrategy()->UploadCloudDocument(groupId, operation, isSendMessage,
				mountId, fileName, fileSize, cloudType, hash, detailContent, msgId);
			responseArgs->SetInt(2, msgId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendRecordVideoMessage") {
		if ((argLen == 15) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) &&
			(arguments->GetType(6) == VTYPE_STRING) && (arguments->GetType(7) == VTYPE_INT) &&
			(arguments->GetType(8) == VTYPE_STRING) && (arguments->GetType(9) == VTYPE_STRING) &&
			(arguments->GetType(10) == VTYPE_STRING) && (arguments->GetType(11) == VTYPE_INT) &&
			(arguments->GetType(12) == VTYPE_INT) && (arguments->GetType(13) == VTYPE_STRING) &&
			(arguments->GetType(14) == VTYPE_STRING)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));
				std::wstring videoName = arguments->GetString(6).ToWString();
				int32_t videoSize = arguments->GetInt(7);
				std::wstring videoURL = arguments->GetString(8).ToWString();
				std::wstring videoThumb = arguments->GetString(9).ToWString();
				std::wstring videoThumbURL = arguments->GetString(10).ToWString();
				int32_t videoDuration = arguments->GetInt(11);
				int32_t operatorId = arguments->GetInt(12);
				std::wstring operatorName = arguments->GetString(13).ToWString();
				std::wstring operatorAvatar = arguments->GetString(14).ToWString();

				int32_t msgId;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendRecordVideoMessage(type, groupJid, videoName, videoSize, videoURL,
					videoThumb, videoThumbURL, videoDuration, operatorId, operatorName, operatorAvatar, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendInviteCallMessage") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) &&
			(arguments->GetType(6) == VTYPE_STRING) && (arguments->GetType(7) == VTYPE_STRING) &&
			(arguments->GetType(8) == VTYPE_INT)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));
				std::wstring confId = arguments->GetString(6).ToWString();
				std::wstring confPwd = arguments->GetString(7).ToWString();
				int32_t hostId = arguments->GetInt(8);

				int32_t msgId;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendInviteCallMessage(type, groupJid, confId, confPwd, hostId, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendAcceptCallMessage") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) &&
			(arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_INT)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));
				std::wstring confId = arguments->GetString(6).ToWString();
				int32_t hostId = arguments->GetInt(7);

				int32_t msgId;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendAcceptCallMessage(type, groupJid, confId, hostId, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendRejectCallMessage") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) &&
			(arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_INT) &&
			(arguments->GetType(8) == VTYPE_STRING)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));
				std::wstring confId = arguments->GetString(6).ToWString();
				int32_t hostId = arguments->GetInt(7);
				std::wstring reason = arguments->GetString(8).ToWString();

				int32_t msgId;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendRejectCallMessage(type, groupJid, confId, hostId, reason, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendFinishCallMessage") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) &&
			(arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_INT) &&
			(arguments->GetType(8) == VTYPE_STRING)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));
				std::wstring confId = arguments->GetString(6).ToWString();
				int32_t hostId = arguments->GetInt(7);
				std::wstring reason = arguments->GetString(8).ToWString();

				int32_t msgId;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendFinishCallMessage(type, groupJid, confId, hostId, reason, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendCancelCallMessage") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) &&
			(arguments->GetType(6) == VTYPE_STRING)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));

				int32_t msgId;
				string phoneNum = arguments->GetString(6);
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendCancelCallMessage(type, groupJid, phoneNum, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	/*
	else if (method == L"SendNotRespondCallRecordMessage") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));

				int32_t msgId;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendNotRespondCallRecordMessagetype, groupJid, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendRejectCallRecordMessage") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));

				int32_t msgId;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendRejectCallRecordMessagetype, groupJid, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	*/
	else if (method == L"SendConnectedCallRecordMessage") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) &&
			 (arguments->GetType(6) == VTYPE_INT) &&
			 (arguments->GetType(7) == VTYPE_STRING)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));
				int32_t druation = arguments->GetInt(6);
				std::string phoneNum = arguments->GetString(7).ToString();

				int32_t msgId;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendConnectedCallRecordMessage(type, groupJid, druation, phoneNum, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendMessageRead") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) &&
			(arguments->GetType(6) == VTYPE_INT) && CefJSConversion::IsJsNumber(arguments,7)) {
			UCID groupJid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
				ConversationType type = ConversationType(arguments->GetInt(4));
				int32_t msgId = arguments->GetInt(6);
				int64_t msgSeq = (int64_t)CefJSConversion::JSNumberToDouble(arguments,7);

				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					SendMessageRead(type, groupJid, msgId, msgSeq);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendMessagesReadById") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT) &&
			CefJSConversion::IsJsNumber(arguments, 5)) {
			ConversationType type = ConversationType(arguments->GetInt(4));
			int64_t groupId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,5);	

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				SendMessagesReadById(type, groupId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendMessagePlayed") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_DICTIONARY) &&
			(arguments->GetType(6) == VTYPE_INT) && CefJSConversion::IsJsNumber(arguments,7)) {
				UCID groupJid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(5), groupJid)) {
					ConversationType type = ConversationType(arguments->GetInt(4));
					int32_t msgId = arguments->GetInt(6);
					int64_t msgSeq = (int64_t) CefJSConversion::JSNumberToDouble(arguments,7);

					res = UCManager::GetInstance().GetGroupServiceStrategy()->
						SendMessagePlayed(type, groupJid, msgId, msgSeq);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetUnreadUserList") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_INT) &&
			CefJSConversion::IsJsNumber(arguments, 5) && (arguments->GetType(6) == VTYPE_LIST)) {
			
			ConversationType type = ConversationType(arguments->GetInt(4));
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			CefRefPtr<CefListValue> ceflist = arguments->GetList(6);

			std::list<int64_t> list;
			for (std::size_t i = 0; i < ceflist->GetSize(); ++i) {
				if (CefJSConversion::IsJsNumber(ceflist, i)) {
					int64_t seq = (int64_t)CefJSConversion::JSNumberToDouble(ceflist, i);
					if (seq > 0){
						list.push_back(seq);
					}
				}
			}
			if (list.size() > 0){
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					GetUnreadUserList(type, groupId, list);
			}else{
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetGroupList") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_INT) &&	(arguments->GetType(6) == VTYPE_INT) && 
			(arguments->GetType(7) == VTYPE_INT) && (arguments->GetType(8) == VTYPE_BOOL)) {
			GroupStyle groupType = GroupStyle(arguments->GetInt(4));
			int32_t page = arguments->GetInt(5);
			int32_t count = arguments->GetInt(6);
			int32_t lastGroupTime = arguments->GetInt(7);
			bool allData = arguments->GetBool(8);
			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				GetGroupList(groupType, page, count, lastGroupTime, allData);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetGroupInfo") {
		if ((argLen == 5) && CefJSConversion::IsJsNumber(arguments, 4)) {
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			GroupInfo info;
			res = UCManager::GetInstance().GetGroupServiceStrategy()->GetGroupInfo(groupId, info);
			if (UC_LOCALDATA_SUCCESSED == res) {
				res = UCCLIENT_SUCCESS;
				CefRefPtr<CefDictionaryValue> groupDictionary = ConvertGroupInfoToDictionary(info);
				responseArgs->SetDictionary(2, groupDictionary);
			}
		} else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CreateGroup") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_INT) && 
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_LIST) &&
			(arguments->GetType(7) == VTYPE_INT) && (arguments->GetType(8) == VTYPE_INT)) {
			GroupStyle groupType = GroupStyle(arguments->GetInt(4));
			std::wstring groupName = arguments->GetString(5).ToWString();
			CefRefPtr<CefListValue> userInfoList = arguments->GetList(6);
			int32_t nameFlag = arguments->GetInt(7);
			int32_t memberType = arguments->GetInt(8);
			std::vector<UserInfo> userInfos;
			for (std::size_t i = 0; i < userInfoList->GetSize(); ++i) {
				if (userInfoList->GetType(i) == VTYPE_DICTIONARY) {
					UserInfo userInfo;
					ConvertDictionaryToUserInfo(userInfoList->GetDictionary(i), userInfo);
					userInfos.push_back(userInfo); 
				}
			}

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				CreateGroup(groupType, groupName, L"", userInfos, nameFlag, memberType);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"EndGroup") {
		if ((argLen == 5) && CefJSConversion::IsJsNumber(arguments, 4)) {
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				EndGroup(groupId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ExitGroup") {
		if ((argLen == 5) && CefJSConversion::IsJsNumber(arguments, 4)) {
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				ExitGroup(groupId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"AddMember") {
		if ((argLen == 6) && CefJSConversion::IsJsNumber(arguments, 4)&&
			(arguments->GetType(5) == VTYPE_LIST)) {
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			CefRefPtr<CefListValue> userInfoList = arguments->GetList(5);
			std::vector<UserInfo> userInfos;
			for (std::size_t i = 0; i < userInfoList->GetSize(); ++i) {
				if (userInfoList->GetType(i) == VTYPE_DICTIONARY) {
					UserInfo userInfo;
					ConvertDictionaryToUserInfo(userInfoList->GetDictionary(i), userInfo);
					userInfos.push_back(userInfo); 
				}
			}

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				AddMember(groupId, userInfos);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"RemoveMember") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT)&&
			(arguments->GetType(5) == VTYPE_LIST)) {
			int64_t groupId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
			CefRefPtr<CefListValue> memberList = arguments->GetList(5);
			std::vector<int32_t> members;
			for (std::size_t i = 0; i < memberList->GetSize(); ++i) {
				if (memberList->GetType(i) == VTYPE_INT) {
					members.push_back(memberList->GetInt(i)); 
				}
			}

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				RemoveMember(groupId, members);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetDocumentList") {
		if ((argLen == 7) && CefJSConversion::IsJsNumber(arguments, 4) &&
			(arguments->GetType(5) == VTYPE_INT) &&	(arguments->GetType(6) == VTYPE_INT)) {
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int32_t page = arguments->GetInt(5);
			int32_t count = arguments->GetInt(6);
			
			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				GetDocumentList(groupId, page, count);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UploadDocument") {
		if ((argLen == 9) && (CefJSConversion::IsJsNumber(arguments,4)) && (arguments->GetType(5) == VTYPE_INT) &&
			(arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_STRING) &&
			(arguments->GetType(8) == VTYPE_STRING)) {

			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments,4);
			int32_t operation = arguments->GetInt(5);
			std::wstring fileName = arguments->GetString(6).ToWString();
			int64_t fileSize = StringToInt64(arguments, 7);
			std::wstring fileURL = arguments->GetString(8).ToWString();
			
			int32_t msgId = 0;
			res = UCManager::GetInstance().GetGroupServiceStrategy()->UploadDocument(groupId, operation, fileName, fileSize, fileURL, msgId);
			responseArgs->SetInt(2, msgId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UploadCodeSnippet") {

		if ((argLen == 13) && (CefJSConversion::IsJsNumber(arguments,4)) && (arguments->GetType(5) == VTYPE_INT) &&
			(arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_STRING) &&
			(arguments->GetType(8) == VTYPE_STRING) &&
			(arguments->GetType(9) == VTYPE_STRING) &&
			(arguments->GetType(10) == VTYPE_INT) &&
			(arguments->GetType(11) == VTYPE_STRING) &&
			(arguments->GetType(12) == VTYPE_INT)) {

				int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments,4);
				int32_t operation = arguments->GetInt(5);
				std::wstring codeTitle = arguments->GetString(6).ToWString();
				std::wstring langType = arguments->GetString(7).ToWString();
				std::wstring codeHead =  arguments->GetString(8).ToWString();
				std::wstring codeDesc = arguments->GetString(9).ToWString();
				int32_t codeSize = arguments->GetInt(10);
				std::wstring codeUrl = arguments->GetString(11).ToWString();
				int32_t codeLine = arguments->GetInt(12);

				int32_t msgId = 0;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->UploadCodeSnippet(groupId, operation, codeTitle, langType, codeHead, codeDesc,codeSize, codeUrl, codeLine, msgId );
				responseArgs->SetInt(2, msgId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetGroupName") {
		if ((argLen == 6) && CefJSConversion::IsJsNumber(arguments, 4) &&
			(arguments->GetType(5) == VTYPE_STRING)) {

			int64_t groupId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
			std::wstring groupName = arguments->GetString(5).ToWString();

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				SetGroupName(groupId, groupName);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"DeleteDocument") {
		if ((argLen == 6) && CefJSConversion::IsJsNumber(arguments, 4) &&
			CefJSConversion::IsJsNumber(arguments,5)) {

			int64_t groupId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
			int64_t fileId = (int64_t)CefJSConversion::JSNumberToDouble(arguments,5);

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				DeleteDocument(groupId, fileId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetDocumentInfo") {
		if ((argLen == 7) && CefJSConversion::IsJsNumber(arguments, 4) && 
			CefJSConversion::IsJsNumber(arguments, 5) && arguments->GetType(6) == VTYPE_STRING) {
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int64_t fileId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			std::wstring cloudFileInfo = arguments->GetString(6).ToWString();

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				GetDocumentInfo(groupId, fileId, cloudFileInfo);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetGroupLogo") {
		if ((argLen == 6) && CefJSConversion::IsJsNumber(arguments, 4) &&
			(arguments->GetType(5) == VTYPE_STRING)) {

			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			std::wstring logoURL = arguments->GetString(5).ToWString();
			
			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				SetGroupLogo(groupId, logoURL);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateGroupLogo") {
		if ((argLen == 6) && CefJSConversion::IsJsNumber(arguments, 4) &&
			(arguments->GetType(5) == VTYPE_STRING)) {

			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			std::wstring localPath = arguments->GetString(5).ToWString();

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				UpdateGroupLogo(groupId, localPath);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetTopChat") {
		if ((argLen == 6) && CefJSConversion::IsJsNumber(arguments, 4)&&
			(arguments->GetType(5) == VTYPE_INT)) {

			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int32_t state = arguments->GetInt(5);

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				SetTopChat(groupId, state);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetDoNotDisturb") {
		if ((argLen == 6) && CefJSConversion::IsJsNumber(arguments, 4) &&
			(arguments->GetType(5) == VTYPE_INT)) {

			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int32_t state = arguments->GetInt(5);

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				SetDoNotDisturb(groupId, state);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}

	}
	else if (method == L"SetReminder") {
		if ((argLen == 8) && CefJSConversion::IsJsNumber(arguments, 4) &&
			(arguments->GetType(5) == VTYPE_INT) && (arguments->GetType(6) == VTYPE_INT) &&
			(arguments->GetType(7) == VTYPE_INT)) {

			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int32_t state = arguments->GetInt(5);
			int32_t alertRule = arguments->GetInt(6);
			int32_t alertTime = arguments->GetInt(7);

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				SetReminder(groupId, state, alertRule, alertTime);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetCommentList") {
		if ((argLen == 9) && CefJSConversion::IsJsNumber(arguments,4) &&
			CefJSConversion::IsJsNumber(arguments,5) && (arguments->GetType(6) == VTYPE_INT) &&
			CefJSConversion::IsJsNumber(arguments,7) && CefJSConversion::IsJsNumber(arguments,8)) {

				int64_t groupId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
				int64_t fileId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,5);
				int32_t count = arguments->GetInt(6);
				int64_t startTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,7);
				int64_t endTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,8);

				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					GetCommentList(groupId, fileId, count, startTime, endTime);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetAtMessageList") {
		if ((argLen == 7) && CefJSConversion::IsJsNumber(arguments, 4) &&
			(arguments->GetType(5) == VTYPE_INT) && (arguments->GetType(6) == VTYPE_INT)) {

			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int32_t page = arguments->GetInt(5);
			int32_t count = arguments->GetInt(6);

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				GetAtMessageList(groupId, page, count);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"DeleteAtMessage") {
		if ((argLen == 7) && arguments->GetType(4) == VTYPE_INT &&
			CefJSConversion::IsJsNumber(arguments, 5) && (arguments->GetType(6) == VTYPE_LIST)) {

			ConversationType type = (ConversationType)arguments->GetInt(4);
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			CefRefPtr<CefListValue> ceflist = arguments->GetList(6);
			std::vector<int64_t> seqList;
			for (std::size_t i = 0; i < ceflist->GetSize(); ++i) {
				if (CefJSConversion::IsJsNumber(ceflist, i)) {
					seqList.push_back((int64_t)CefJSConversion::JSNumberToDouble(ceflist, i));
				}
			}

			res = UCManager::GetInstance().GetGroupServiceStrategy()->
				DeleteAtMessage(type, groupId, seqList);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CreateComment") {
		if ((argLen == 9) && CefJSConversion::IsJsNumber(arguments,4) &&
			CefJSConversion::IsJsNumber(arguments,5) && (arguments->GetType(6) == VTYPE_INT) &&
			(arguments->GetType(7) == VTYPE_STRING) && (arguments->GetType(8) == VTYPE_STRING)) {

				int64_t groupId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
				int64_t fileId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,5);
				int32_t fileType = arguments->GetInt(6);
				std::wstring fileTitle = arguments->GetString(7).ToWString();
				std::wstring content = arguments->GetString(8).ToWString();

				int32_t msgId = 0;
				res = UCManager::GetInstance().GetGroupServiceStrategy()->
					CreateComment(groupId, fileId, fileType, fileTitle, content, msgId);
				responseArgs->SetInt(2, msgId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else {
		res = UCCLIENT_METHOD_NOTFOUND;
	}
}

void UCExtension::CalendarServiceMethodProcess(CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"CreateMeeting") {
		if ((argLen == 20) && (arguments->GetType(4) == VTYPE_LIST)&&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_STRING) 
			&& (arguments->GetType(7) == VTYPE_STRING) && (arguments->GetType(8) == VTYPE_STRING)
			&& (arguments->GetType(9) == VTYPE_STRING) && (arguments->GetType(10) == VTYPE_STRING) 
			&& (CefJSConversion::IsJsNumber(arguments, 11))
			&& (arguments->GetType(12) == VTYPE_INT) &&	(arguments->GetType(13) == VTYPE_INT) 
			&& (arguments->GetType(14) == VTYPE_DICTIONARY) && (arguments->GetType(15) == VTYPE_INT)
			&& (arguments->GetType(16) == VTYPE_INT) && (arguments->GetType(17) == VTYPE_INT)
			&& (arguments->GetType(18) == VTYPE_INT) && (arguments->GetType(19) == VTYPE_INT)) {
				
				CefRefPtr<CefListValue> attendeeList = arguments->GetList(4);
				std::vector<Attendee> attendees;
				for (std::size_t i = 0; i < attendeeList->GetSize(); ++i) {
					if (attendeeList->GetType(i) == VTYPE_DICTIONARY) {
						Attendee attendee;
						if (!ConvertDictionaryToAttendee(attendeeList->GetDictionary(i), attendee)) {
							res = UCCLIENT_ARGUMENT_ERROR;
							return;
						}
						attendees.push_back(attendee);					 
					}
				}

				std::wstring title = arguments->GetString(5).ToWString();
				std::wstring location = arguments->GetString(6).ToWString();
				std::wstring summary = arguments->GetString(7).ToWString();
				std::wstring timeZone = arguments->GetString(8).ToWString();
				std::wstring hostName = arguments->GetString(9).ToWString();
				std::wstring extrasInfo = arguments->GetString(10).ToWString();
				
				// js 传下来的64位，对应C++的double
				int64_t startTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,11);
				if (startTime <= 0){
					res = UCCLIENT_ARGUMENT_ERROR;
					return;
				}
				int32_t duration = arguments->GetInt(12);
				int32_t authorizerId = arguments->GetInt(13);
				int32_t isGNet = arguments->GetInt(15);
				int32_t isCycle = arguments->GetInt(16);
				CycleRole role;
				if (isCycle) {
					if (!ConvertDictionaryToCycleRole(arguments->GetDictionary(14), role)){
						res = UCCLIENT_ARGUMENT_ERROR;
						return;
					}
				}
				int32_t isWholeDay = arguments->GetInt(17);
				int32_t language = arguments->GetInt(18);
				int32_t emailLanguage = arguments->GetInt(19);
				res = UCManager::GetInstance().GetCalendarServiceStrategy()->
					CreateMeeting(attendees, title, location, summary, timeZone, hostName, extrasInfo, 
					startTime, duration, authorizerId, role, isGNet, 
					isCycle, isWholeDay, language, emailLanguage);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateMeeting") {
		if ((argLen == 21) && (CefJSConversion::IsJsNumber(arguments,4)) && (CefJSConversion::IsJsNumber(arguments,5))
			&& (arguments->GetType(6) == VTYPE_LIST) &&	(arguments->GetType(7) == VTYPE_STRING) 
			&& (arguments->GetType(8) == VTYPE_STRING) && (arguments->GetType(9) == VTYPE_STRING)
			&& (arguments->GetType(10) == VTYPE_STRING) && (arguments->GetType(11) == VTYPE_STRING)
			&& (CefJSConversion::IsJsNumber(arguments,12))
			&& (arguments->GetType(13) == VTYPE_INT) && (arguments->GetType(14) == VTYPE_INT)
			&& (arguments->GetType(15) == VTYPE_DICTIONARY) && (arguments->GetType(16) == VTYPE_INT)
			&& (arguments->GetType(17) == VTYPE_INT) && (arguments->GetType(18) == VTYPE_INT)
			&& (arguments->GetType(19) == VTYPE_INT) && (arguments->GetType(20) == VTYPE_INT)) {

			int64_t eventId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
			if (eventId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int64_t oriTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,5);
			CefRefPtr<CefListValue> attendeeList = arguments->GetList(6);
			std::vector<Attendee> attendees;
			for (std::size_t i = 0; i < attendeeList->GetSize(); ++i) {
				if (attendeeList->GetType(i) == VTYPE_DICTIONARY) {
					Attendee attendee;
					if (!ConvertDictionaryToAttendee(attendeeList->GetDictionary(i), attendee)) {
						res = UCCLIENT_ARGUMENT_ERROR;
						return;
					}
					attendees.push_back(attendee);
				}
			}

			std::wstring title = arguments->GetString(7).ToWString();
			std::wstring location = arguments->GetString(8).ToWString();
			std::wstring summary = arguments->GetString(9).ToWString();
			std::wstring timeZone = arguments->GetString(10).ToWString();
			std::wstring extrasInfo = arguments->GetString(11).ToWString();
			int64_t startTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,12);
			if (startTime <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int32_t duration = arguments->GetInt(13);
			int32_t authorizerId = arguments->GetInt(14);
			int32_t isGNet = arguments->GetInt(16);
			int32_t isCycle = arguments->GetInt(17);
			CycleRole role;
			if (isCycle) {
				if (!ConvertDictionaryToCycleRole(arguments->GetDictionary(15), role)){
					res = UCCLIENT_ARGUMENT_ERROR;
					return;
				}
			}

			int32_t isWholeDay = arguments->GetInt(18);
			int32_t language = arguments->GetInt(19);
			int32_t emailLanguage = arguments->GetInt(20);
				
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				UpdateMeeting(eventId, oriTime, attendees, title, location, 
				summary, timeZone, extrasInfo, startTime, duration, authorizerId, role,
				isGNet, isCycle, isWholeDay, language, emailLanguage);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CancelMeeting") {
		if ((argLen == 9) && (CefJSConversion::IsJsNumber(arguments,4)) 
			&& (CefJSConversion::IsJsNumber(arguments,5)) && (arguments->GetType(6) == VTYPE_INT) 
			&& (arguments->GetType(7) == VTYPE_INT)	&& (arguments->GetType(8) == VTYPE_INT)) {
			int64_t eventId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
			if (eventId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int64_t startTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,5);
			int32_t authorizerId = arguments->GetInt(6);
			int32_t language = arguments->GetInt(7);
			int32_t emailLanguage = arguments->GetInt(8);
				
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				CancelMeeting(eventId, startTime, authorizerId, language, emailLanguage);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetMeetingStartURL") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_STRING) &&
			(arguments->GetType(5) == VTYPE_STRING)) {

			std::wstring confId = arguments->GetString(4).ToWString();
			std::wstring password = arguments->GetString(5).ToWString();
			if (confId.empty() || password.empty()) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			
			int32_t siteId = 60001;
			std::string startURL = "";
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetMeetingStartURL(confId, password, startURL, siteId);
			responseArgs->SetInt(2, siteId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"IsInMeeting") {
		if (argLen == 4) {
			bool bMeetingRun = UCManager::GetInstance().GetCalendarServiceStrategy()->
				IsMeetingRunning();
			if (bMeetingRun && bInMeeting) {
				responseArgs->SetBool(2, true);
			}
			else{
				responseArgs->SetBool(2, false);
			}
			
			res = UCCLIENT_SUCCESS;
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetMeetingWebStartURL") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_STRING) &&
			(arguments->GetType(5) == VTYPE_STRING) && 
			(arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_STRING)) {

			std::wstring confId = arguments->GetString(4).ToWString();
			std::wstring mail = arguments->GetString(5).ToWString();
			std::wstring password = arguments->GetString(6).ToWString();
			std::wstring displayName = arguments->GetString(7).ToWString();
			if (confId.empty() || password.empty()) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetMeetingWebStartURL(confId, mail, password, displayName);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateAccreditSetting") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT)&&
			(arguments->GetType(5) == VTYPE_INT)) {
			int32_t authorizedPersonId = arguments->GetInt(4);
			int32_t status = arguments->GetInt(5);
			if (authorizedPersonId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				UpdateAccreditSetting(authorizedPersonId, status);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetAccreditUserList") {
		vector<Accrediting> infos;
		res = UCManager::GetInstance().GetCalendarServiceStrategy()->
			GetAccreditUserList(infos);
	}
	else if (method == L"GetMeetingList") {
		if ((argLen == 11) && (CefJSConversion::IsJsNumber(arguments,4)) 
			&& (CefJSConversion::IsJsNumber(arguments,5))
			&& (arguments->GetType(6) == VTYPE_INT) && (arguments->GetType(7) == VTYPE_INT)
			&& (CefJSConversion::IsJsNumber(arguments, 8)) && (arguments->GetType(9) == VTYPE_BOOL)
			&& (arguments->GetType(10) == VTYPE_INT)) {

			int64_t startTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
			int64_t endTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,5);
			if (startTime <= 0 && endTime <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int32_t limit = arguments->GetInt(6);
			int32_t authorizerId = arguments->GetInt(7);
			int64_t eventIdFilter = arguments->GetInt(8);
			bool filterAsc = arguments->GetBool(9);
			int32_t refreshflag = arguments->GetInt(10);
			int32_t requestId = 0;
			vector<Meeting> meetings;

			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetMeetingList(startTime, endTime, limit, authorizerId, eventIdFilter, filterAsc, refreshflag, requestId, meetings);

			responseArgs->SetInt(2, requestId);
			if (res == UC_SUCCEEDED) {
				CefRefPtr<CefListValue> meetingList = CefListValue::Create();
				for (std::size_t i = 0; i < meetings.size(); ++i) {
					CefRefPtr<CefDictionaryValue> conferenceDictionary = ConvertConferenceInfoToDictionary(meetings[i]);
					meetingList->SetDictionary(i, conferenceDictionary);
				}

				responseArgs->SetList(3, meetingList);
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetBriefMeetingList") {
		if ((argLen == 7) && (CefJSConversion::IsJsNumber(arguments, 4))
			&& (CefJSConversion::IsJsNumber(arguments, 5)) && (arguments->GetType(6) == VTYPE_INT)) {
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int64_t endTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			if (startTime <= 0 && endTime <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int32_t authorizerId = arguments->GetInt(6);
			int32_t requestId = 0;
			vector<Meeting> meetings;
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetBriefMeetingList(startTime, endTime, authorizerId, requestId, meetings);
			responseArgs->SetInt(2, requestId);
			if (res == UC_SUCCEEDED) {
				CefRefPtr<CefListValue> meetingList = CefListValue::Create();
				for (std::size_t i = 0; i < meetings.size(); ++i) {
					CefRefPtr<CefDictionaryValue> conferenceDictionary = ConvertConferenceInfoToDictionary(meetings[i]);
					meetingList->SetDictionary(i, conferenceDictionary);
				}

				responseArgs->SetList(3, meetingList);
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetBriefChangedMeetingList") {
		if ((argLen == 6) && (CefJSConversion::IsJsNumber(arguments, 4))
			&& (arguments->GetType(5) == VTYPE_INT)) {
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			if (startTime <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int32_t authorizerId = arguments->GetInt(5);
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetBriefChangedMeetingList(startTime, authorizerId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"AcceptMeetingInvitation") {
		if ((argLen == 7) &&
			(CefJSConversion::IsJsNumber(arguments,4))
			&& CefJSConversion::IsJsNumber(arguments, 5) && (arguments->GetType(6) == VTYPE_INT)) {
			int64_t eventId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
			if (eventId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			int32_t authorizerId = arguments->GetInt(6);
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				AcceptMeetingInvitation(eventId, startTime, authorizerId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"DenyMeetingInvitation") {
		if ((argLen == 7) && (CefJSConversion::IsJsNumber(arguments,4))
			&& (CefJSConversion::IsJsNumber(arguments, 5)) && (arguments->GetType(6) == VTYPE_INT)) {
			int64_t eventId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
			if (eventId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			int32_t authorizerId = arguments->GetInt(6);
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				DenyMeetingInvitation(eventId, startTime, authorizerId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetMeetingInfoById") {
		if ((argLen == 8) && (CefJSConversion::IsJsNumber(arguments, 4)) &&
			(CefJSConversion::IsJsNumber(arguments,5)) && (CefJSConversion::IsJsNumber(arguments,6)) &&
			(arguments->GetType(7) == VTYPE_INT)) {

			int64_t eventId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			if (eventId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int64_t startTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,5);
			int64_t endTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,6);
			int32_t authorizerId = arguments->GetInt(7);
			vector<Meeting> infos;
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetMeetingInfoById(eventId, startTime, endTime, authorizerId, infos);
			if (res == UC_SUCCEEDED) {
				CefRefPtr<CefListValue> conferencelist = CefListValue::Create();
				for (std::size_t i = 0; i < infos.size(); ++i) {
					CefRefPtr<CefDictionaryValue> conferenceDictionary = ConvertConferenceInfoToDictionary(infos[i]);
					conferencelist->SetDictionary(i, conferenceDictionary);
				}

				responseArgs->SetList(2, conferencelist);
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetMeetingInfoByIds") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_LIST) &&
			(arguments->GetType(5) == VTYPE_INT)) {
			CefRefPtr<CefListValue> idList = arguments->GetList(4);

			std::vector<int64_t> eventIds;
			for (std::size_t i = 0; i < idList->GetSize(); ++i) {
				if (CefJSConversion::IsJsNumber(idList,i)) {
					eventIds.push_back((int64_t) CefJSConversion::JSNumberToDouble(idList,i));
				}
			}
			if (eventIds.empty()) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}

			int64_t startTime = arguments->GetInt(5);
			int64_t endTime = arguments->GetInt(6);
			int32_t authorizerId = arguments->GetInt(7);
			int32_t conRequestId = 0;
			vector<Meeting> infos;
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetMeetingInfoByIds(eventIds, startTime, endTime, authorizerId, conRequestId, infos);
			responseArgs->SetInt(2, conRequestId);
			if (res == UC_SUCCEEDED) {
				CefRefPtr<CefListValue> conferencelist = CefListValue::Create();
				for (std::size_t i = 0; i < infos.size(); ++i) {
					CefRefPtr<CefDictionaryValue> conferenceDictionary = ConvertConferenceInfoToDictionary(infos[i]);
					conferencelist->SetDictionary(i, conferenceDictionary);
				}

				responseArgs->SetList(3, conferencelist);
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ForwardMeeting") {
		if ((argLen == 10) && (CefJSConversion::IsJsNumber(arguments,4)) 
			&& (arguments->GetType(5) == VTYPE_LIST) && (CefJSConversion::IsJsNumber(arguments, 6))
			&& (arguments->GetType(7) == VTYPE_INT) && (arguments->GetType(8) == VTYPE_INT)
			&& (arguments->GetType(9) == VTYPE_INT)) {

			int64_t eventId = (int64_t) CefJSConversion::JSNumberToDouble(arguments,4);
			if (eventId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			CefRefPtr<CefListValue> attendeeList = arguments->GetList(5);
			std::vector<Attendee> attendees;
			for (std::size_t i = 0; i < attendeeList->GetSize(); ++i) {
				if (attendeeList->GetType(i) == VTYPE_DICTIONARY) {
					Attendee attendee;
					ConvertDictionaryToAttendee(attendeeList->GetDictionary(i), attendee);
					attendees.push_back(attendee); 
				}
			}
			if (attendees.empty()) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int32_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 6);
			int32_t authorizerId = arguments->GetInt(7);
			int32_t language = arguments->GetInt(8);
			int32_t emailLanguage = arguments->GetInt(9);
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				ForwardMeeting(eventId, attendees, startTime, authorizerId, language, emailLanguage);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CreateRemoteAssistance") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {

			int32_t userId = arguments->GetInt(4);
			if (userId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				CreateRemoteAssistance(userId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetUserSchedule") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_LIST)&&
			(CefJSConversion::IsJsNumber(arguments,5)) && (CefJSConversion::IsJsNumber(arguments,6)) &&
			(arguments->GetType(7) == VTYPE_INT) && (CefJSConversion::IsJsNumber(arguments, 8))) {
			CefRefPtr<CefListValue> idList = arguments->GetList(4);

			std::vector<int32_t> userIdList;
			for (std::size_t i = 0; i < idList->GetSize(); ++i) {
				if (idList->GetType(i) == VTYPE_INT) {
					userIdList.push_back(idList->GetInt(i));
				}
			}

			if (userIdList.empty()) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}

			int64_t startTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,5);
			int64_t endTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,6);
			int32_t timeInterval = arguments->GetInt(7);
			int64_t eventIdFilter = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 8); 
			int32_t conRequestId;
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetUserSchedule(userIdList, startTime, endTime, timeInterval, eventIdFilter, conRequestId);
			responseArgs->SetInt(2, conRequestId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetUserBusyFreeInfo") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_LIST) &&
			(CefJSConversion::IsJsNumber(arguments, 5)) && (CefJSConversion::IsJsNumber(arguments, 6)) &&
			(arguments->GetType(7) == VTYPE_INT)) {
			CefRefPtr<CefListValue> idList = arguments->GetList(4);
			std::vector<int32_t> userIdList;
			for (std::size_t i = 0; i < idList->GetSize(); ++i) {
				if (idList->GetType(i) == VTYPE_INT) {
					userIdList.push_back(idList->GetInt(i));
				}
			}

			if (userIdList.empty()) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}

			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			int64_t endTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 6);
			int32_t shareUserId = arguments->GetInt(7);
			int32_t conRequestId;
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetUserBusyFreeInfo(userIdList, startTime, endTime, shareUserId, conRequestId);
			responseArgs->SetInt(2, conRequestId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CreateMinutes") {
		if ((argLen == 8) && CefJSConversion::IsJsNumber(arguments, 4)
			&& (CefJSConversion::IsJsNumber(arguments,5)) && (arguments->GetType(6) == VTYPE_STRING)
			&& (arguments->GetType(7) == VTYPE_STRING)) {

			int64_t eventId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			if (eventId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int64_t startTime = (int64_t) CefJSConversion::JSNumberToDouble(arguments,5);
			std::wstring title = arguments->GetString(6).ToWString();
			std::wstring summary = arguments->GetString(7).ToWString();
				
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				CreateMinutes(eventId, startTime, title, summary);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateMinutes") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_INT)
			&& (arguments->GetType(5) == VTYPE_INT)
			&& (arguments->GetType(6) == VTYPE_STRING)
			&& (arguments->GetType(7) == VTYPE_STRING)) {

			int32_t eventId = arguments->GetInt(4);
			int64_t minutesId = arguments->GetInt(5);
			if (minutesId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			std::wstring title = arguments->GetString(6).ToWString();
			std::wstring minutes = arguments->GetString(7).ToWString();
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				UpdateMinutes(eventId, minutesId, title, minutes);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetMinutesDetails") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT)
			&& (arguments->GetType(5) == VTYPE_INT)) {

			int64_t eventId = arguments->GetInt(4);
			int64_t minutesId = arguments->GetInt(5);
			if (minutesId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetMinutesDetails(eventId, minutesId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetMinutesList") {
		if ((argLen == 6) && CefJSConversion::IsJsNumber(arguments, 4)
			&& CefJSConversion::IsJsNumber(arguments, 5)) {
			
			int64_t eventId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			if (eventId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetMinutesList(eventId, startTime);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"DeleteMeetingRecording") {
		if ((argLen == 7) && CefJSConversion::IsJsNumber(arguments, 4)
			&& (arguments->GetType(5) == VTYPE_STRING)
			&& CefJSConversion::IsJsNumber(arguments, 5)) {

			int64_t eventId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			if (eventId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			std::wstring confId = arguments->GetString(5);
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 6);
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->DeleteMeetingRecording(eventId, confId, startTime);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UploadMeetingFile") {
		if ((argLen == 10) && (CefJSConversion::IsJsNumber(arguments, 4)) && (arguments->GetType(5) == VTYPE_INT) &&
			(arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_STRING) &&
			(arguments->GetType(8) == VTYPE_STRING) && 
			(arguments->GetType(9) == VTYPE_DOUBLE)) {

			int64_t eventId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int32_t operation = arguments->GetInt(5);
			std::wstring fileName = arguments->GetString(6).ToWString();
			int64_t fileSize = StringToInt64(arguments, 7);
			std::wstring fileURL = arguments->GetString(8).ToWString();
			int64_t createTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 9);

			int32_t msgId = 0;
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->UploadMeetingFile(eventId, operation, fileName, 
				fileSize, fileURL, createTime, msgId);
			responseArgs->SetInt(2, msgId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UploadCloudFile") {
		if ((argLen == 12) && (CefJSConversion::IsJsNumber(arguments, 4)) && (arguments->GetType(5) == VTYPE_INT) &&
			(arguments->GetType(6) == VTYPE_INT) && (arguments->GetType(7) == VTYPE_STRING) && 
			(arguments->GetType(8) == VTYPE_STRING) && (arguments->GetType(9) == VTYPE_INT) && 
			(arguments->GetType(10) == VTYPE_STRING) && (arguments->GetType(11) == VTYPE_STRING)) {
			int64_t groupId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int32_t operation = arguments->GetInt(5);
			int32_t mountId = arguments->GetInt(6);
			std::wstring fileName = arguments->GetString(7).ToWString();
			int64_t fileSize = StringToInt64(arguments, 8);
			DocumentCloudType cloudType = DocumentCloudType(arguments->GetInt(9));
			std::wstring hash = arguments->GetString(10).ToWString();
			std::wstring detailContent = arguments->GetString(11).ToWString();

			int32_t msgId;
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->UploadCloudFile(groupId, operation, mountId, 
				fileName, fileSize, cloudType, hash, detailContent, msgId);
			responseArgs->SetInt(2, msgId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"DeleteMeetingFile") {
		if ((argLen == 5) && CefJSConversion::IsJsNumber(arguments, 4)) {

			int64_t minutesId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);

			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				DeleteMeetingFile(minutesId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetMeetingAlarm") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT)
			&& (arguments->GetType(5) == VTYPE_INT)) {

			int32_t alarmStatus = arguments->GetInt(4);
			int32_t alarmTime = arguments->GetInt(5);
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				SetMeetingAlarm(alarmStatus, alarmTime);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CreateMeetingRoom") {
		if ((argLen == 15) && (arguments->GetType(4) == VTYPE_LIST) && (arguments->GetType(5) == VTYPE_STRING)
			&& (arguments->GetType(6) == VTYPE_STRING) && (arguments->GetType(7) == VTYPE_STRING)
			&& (CefJSConversion::IsJsNumber(arguments, 8)) && (CefJSConversion::IsJsNumber(arguments, 9))
			&& (CefJSConversion::IsJsNumber(arguments, 10)) && (CefJSConversion::IsJsNumber(arguments, 11))
			&& (arguments->GetType(12) == VTYPE_INT) && (arguments->GetType(13) == VTYPE_INT)
			&& (arguments->GetType(14) == VTYPE_INT)) {

			CefRefPtr<CefListValue> attendeeList = arguments->GetList(4);
			std::vector<Attendee> attendees;
			for (std::size_t i = 0; i < attendeeList->GetSize(); ++i) {
				if (attendeeList->GetType(i) == VTYPE_DICTIONARY) {
					Attendee attendee;
					if (!ConvertDictionaryToAttendee(attendeeList->GetDictionary(i), attendee)) {
						res = UCCLIENT_ARGUMENT_ERROR;
						return;
					}
					attendees.push_back(attendee);
				}
			}

			std::wstring title = arguments->GetString(5).ToWString();
			std::wstring remarks = arguments->GetString(6).ToWString();
			std::wstring password = arguments->GetString(7).ToWString();
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 8);
			int64_t endTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 9);
			if (startTime <= 0 || endTime <=0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int64_t roomHostId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 10);
			int64_t orgId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 11);
			int32_t isSetPassword = arguments->GetInt(12);
			int32_t defaultAttendsVisible = arguments->GetInt(13);
			int32_t defaultAttendsJoin = arguments->GetInt(14);

			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				CreateMeetingRoom(attendees, title, remarks, password, startTime, endTime, roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateMeetingRoom") {
		if ((argLen == 16) && (arguments->GetType(4) == VTYPE_STRING)
			&& (arguments->GetType(5) == VTYPE_LIST) && (arguments->GetType(6) == VTYPE_STRING)
			&& (arguments->GetType(7) == VTYPE_STRING) && (arguments->GetType(8) == VTYPE_STRING)
			&& (CefJSConversion::IsJsNumber(arguments, 9)) && (CefJSConversion::IsJsNumber(arguments, 10))
			&& (CefJSConversion::IsJsNumber(arguments, 11)) && (CefJSConversion::IsJsNumber(arguments, 12))
			&& (arguments->GetType(13) == VTYPE_INT) && (arguments->GetType(14) == VTYPE_INT)
			&& (arguments->GetType(15) == VTYPE_INT)) {
			std::wstring roomId = arguments->GetString(4).ToWString();;
			CefRefPtr<CefListValue> attendeeList = arguments->GetList(5);
			std::vector<Attendee> attendees;
			for (std::size_t i = 0; i < attendeeList->GetSize(); ++i) {
				if (attendeeList->GetType(i) == VTYPE_DICTIONARY) {
					Attendee attendee;
					if (!ConvertDictionaryToAttendee(attendeeList->GetDictionary(i), attendee)) {
						res = UCCLIENT_ARGUMENT_ERROR;
						return;
					}
					attendees.push_back(attendee);
				}
			}

			std::wstring title = arguments->GetString(6).ToWString();
			std::wstring remarks = arguments->GetString(7).ToWString();
			std::wstring password = arguments->GetString(8).ToWString();
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 9);
			int64_t endTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 10);
			if (startTime <= 0 || endTime <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
			int64_t roomHostId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 11);
			int64_t orgId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 12);
			int32_t isSetPassword = arguments->GetInt(13);
			int32_t defaultAttendsVisible = arguments->GetInt(14);
			int32_t defaultAttendsJoin = arguments->GetInt(15);

			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				UpdateMeetingRoom(roomId, attendees, title, remarks, password, startTime, endTime, roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CreateMeetingInMeetingRoom") {
		if ((argLen == 10) && (arguments->GetType(4) == VTYPE_LIST) && (arguments->GetType(5) == VTYPE_STRING) 
			&& (arguments->GetType(6) == VTYPE_STRING) && (arguments->GetType(7) == VTYPE_STRING) 
			&& (CefJSConversion::IsJsNumber(arguments, 8)) 
			&& (arguments->GetType(9) == VTYPE_INT)) {

			CefRefPtr<CefListValue> attendeeList = arguments->GetList(4);
			std::vector<Attendee> attendees;
			for (std::size_t i = 0; i < attendeeList->GetSize(); ++i) {
				if (attendeeList->GetType(i) == VTYPE_DICTIONARY) {
					Attendee attendee;
					if (!ConvertDictionaryToAttendee(attendeeList->GetDictionary(i), attendee)) {
						res = UCCLIENT_ARGUMENT_ERROR;
						return;
					}
					attendees.push_back(attendee);
				}
			}
			std::wstring roomId = arguments->GetString(5).ToWString();
			std::wstring title = arguments->GetString(6).ToWString();
			std::wstring summary = arguments->GetString(7).ToWString();
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 8);
			int32_t duration = arguments->GetInt(9);
			if (startTime <= 0 || duration <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}
		
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				CreateMeetingInMeetingRoom(attendees, roomId, title, summary, startTime, duration);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetMeetingRoomList") {
		if ((argLen == 5) && (CefJSConversion::IsJsNumber(arguments, 4))) {

			int64_t orgId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			if (orgId <= 0) {
				res = UCCLIENT_ARGUMENT_ERROR;
				return;
			}

			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				GetMeetingRoomList(orgId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CheckJoinMeetingRoom") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {

			std::wstring roomId = arguments->GetString(4).ToWString();

			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				CheckJoinMeetingRoom(roomId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CheckMeetingRoomPassword") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_STRING)
			&& (arguments->GetType(5) == VTYPE_STRING)) {

			std::wstring roomId = arguments->GetString(4).ToWString();
			std::wstring password = arguments->GetString(5).ToWString();

			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				CheckMeetingRoomPassword(roomId, password);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"DeleteMeetingRoom") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {

			std::wstring roomId = arguments->GetString(4).ToWString();
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				DeleteMeetingRoom(roomId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CheckRightsOfCreateRoom") {
		if ((argLen == 4)) {
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->
				CheckRightsOfCreateRoom();
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ResendMessage"){
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t msgId = arguments->GetInt(4);
			res = UCManager::GetInstance().GetCalendarServiceStrategy()->ResendMessage(msgId);
			responseArgs->SetInt(2, msgId);	
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ForwardMessage"){
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_DICTIONARY)&&
			(arguments->GetType(5) == VTYPE_INT)) {
				UCID ucid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(4), ucid)) {
					int32_t msgId = arguments->GetInt(5);
					res = UCManager::GetInstance().GetCalendarServiceStrategy()->ForwardMessage(ucid, msgId);
					responseArgs->SetInt(2, msgId);	
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendTextMessage") {
		if ((argLen == 10) && (CefJSConversion::IsJsNumber(arguments, 4)) &&
			(CefJSConversion::IsJsNumber(arguments, 5)) &&
			(arguments->GetType(6) == VTYPE_DICTIONARY) && (arguments->GetType(7) == VTYPE_INT) && 
			(arguments->GetType(8) == VTYPE_STRING) && (arguments->GetType(9) == VTYPE_STRING)) {
				UCID groupJid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(6), groupJid)) {
					int64_t eventId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
					int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
					int8_t textType = arguments->GetInt(7);
					std::wstring textFormat = arguments->GetString(8).ToWString();
					std::wstring textData = arguments->GetString(9).ToWString();
					int32_t msgId;
					res = UCManager::GetInstance().GetCalendarServiceStrategy()->
						SendTextMessage(eventId, startTime, groupJid, textType, textFormat, textData, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendImageMessage") {
		if ((argLen == 11) && (CefJSConversion::IsJsNumber(arguments, 4)) && 
			(CefJSConversion::IsJsNumber(arguments, 5)) &&
			(arguments->GetType(6) == VTYPE_DICTIONARY)&& (arguments->GetType(7) == VTYPE_STRING) && 
			(arguments->GetType(8) == VTYPE_INT) &&	(arguments->GetType(9) == VTYPE_STRING) && 
			(arguments->GetType(10) == VTYPE_STRING)) {
				UCID groupJid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(6), groupJid)) {
					int64_t eventId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
					int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
					std::wstring imageName = arguments->GetString(7).ToWString();
					int32_t imageSize = arguments->GetInt(8);
					std::wstring imageURL = arguments->GetString(9).ToWString();
					std::wstring imagePath = arguments->GetString(10).ToWString();

					int32_t msgId;
					res = UCManager::GetInstance().GetCalendarServiceStrategy()->
						SendImageMessage(eventId, startTime, groupJid, imageName, imageSize, imageURL, imagePath, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendAudioMessage") {
		if ((argLen == 11) && (CefJSConversion::IsJsNumber(arguments, 4)) 
			&& (CefJSConversion::IsJsNumber(arguments, 5)) &&
			(arguments->GetType(6) == VTYPE_DICTIONARY) && (arguments->GetType(7) == VTYPE_STRING) && 
			(arguments->GetType(8) == VTYPE_INT) &&	(arguments->GetType(9) == VTYPE_STRING) && 
			(arguments->GetType(10) == VTYPE_INT)) {
				UCID groupJid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(6), groupJid)) {
					int64_t eventId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
					int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
					std::wstring audioName = arguments->GetString(7).ToWString();
					int32_t audioSize = arguments->GetInt(8);
					std::wstring audioURL = arguments->GetString(9).ToWString();
					int32_t audioDuration = arguments->GetInt(10);

					int32_t msgId;
					res = UCManager::GetInstance().GetCalendarServiceStrategy()->
						SendAudioMessage(eventId, startTime, groupJid, audioName, audioSize, 
						audioURL, audioDuration, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendVideoMessage") {
		if ((argLen == 12) && (CefJSConversion::IsJsNumber(arguments, 4)) && 
			(CefJSConversion::IsJsNumber(arguments, 5)) &&
			(arguments->GetType(6) == VTYPE_DICTIONARY) && (arguments->GetType(7) == VTYPE_STRING) && 
			(arguments->GetType(8) == VTYPE_INT) && (arguments->GetType(9) == VTYPE_STRING) && 
			(arguments->GetType(10) == VTYPE_STRING)&& (arguments->GetType(11) == VTYPE_INT)) {
				UCID groupJid;
				if (ConvertDictionaryToUCID(arguments->GetDictionary(6), groupJid)) {
					int64_t eventId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
					int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);

					std::wstring videoName = arguments->GetString(7).ToWString();
					int32_t videoSize = arguments->GetInt(8);
					std::wstring videoURL = arguments->GetString(9).ToWString();
					std::wstring videoPath = arguments->GetString(10).ToWString();
					int32_t videoDuration = arguments->GetInt(11);

					int32_t msgId;
					res = UCManager::GetInstance().GetCalendarServiceStrategy()->
						SendVideoMessage(eventId, startTime, groupJid, videoName, videoSize, videoURL, 
						videoPath, videoDuration, msgId);
					responseArgs->SetInt(2, msgId);
				}
				else {
					res = UCCLIENT_ARGUMENT_ERROR;
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"StartMeetingClient") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {
				std::wstring params = arguments->GetString(4).ToWString();
				res = UCManager::GetInstance().GetCalendarServiceStrategy()->
						StartMeetingClient(params);
				
				//创建等待入会完成事件
				if (res == UCCLIENT_SUCCESS && IS_NEW_MEETING) {
					bInMeeting = true;
					UCExtension::CreateEvent(GLOBAL_EVENT_MEETING_READIED, WaitMeetingReadiedThreadProc);
					UCExtension::CreateEvent(GLOBAL_EVENT_MEETING_JOINED, WaitMeetingJoinedThreadProc);
					UCExtension::CreateEvent(GLOBAL_EVENT_MEETING_FAILED, WaitMeetingJoinFailedThreadProc);
				}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CheckMeetingClient") {
		res = UCManager::GetInstance().GetCalendarServiceStrategy()->CheckMeetingClient();
	}
	else if (method == L"KillMeetingClient") {
		res = UCManager::GetInstance().GetCalendarServiceStrategy()->KillMeetingClient();
	}
	else if (method == L"CancelJoinMeeting") {
		HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, GLOBAL_EVENT_CANCEL_JOIN_MEETING);
		UC_LOG(INFO) << "CancelJoinMeeting open event \"Global\\CancelJoinMeeting\" return: " << (int)hEvent << ", error code = " << GetLastError();
		if (hEvent != NULL) {
			res = SetEvent(hEvent);
			CloseHandle(hEvent);
		}
	}
	else if (method == L"SendPosition2MeetingClient") {
		if ((argLen == 6) && (CefJSConversion::IsJsNumber(arguments, 4))
			&& (CefJSConversion::IsJsNumber(arguments, 5))) {
			double posX = CefJSConversion::JSNumberToDouble(arguments, 4);
			double posY = CefJSConversion::JSNumberToDouble(arguments, 5);
			UC_LOG(ERROR) << "SendPosition2MeetingClient posX = " << posX << ", posY = " << posY;
			HWND hwnd = FindWindow(L"QUANSHI MEETING CLIENT CLASS", NULL);
			std::wstringstream ss;
			ss << posX << "," << posY;
			if (hwnd != NULL) {
				TCHAR strPosition[MAX_PATH] = { 0 };
				_tcscpy(strPosition, ss.str().c_str());
				COPYDATASTRUCT cds;
				cds.dwData = 0;
				cds.lpData = strPosition;
				cds.cbData = (_tcslen(strPosition) + 1)*sizeof(TCHAR);
				SendMessage(hwnd, WM_COPYDATA, (WPARAM)WM_SENDPOSITIO, (LPARAM)&cds);
				res = GetLastError();
				UC_LOG(INFO) << "after send position by WM_COPYDATA, GetLastError return = " << res;
			} else {
				UC_LOG(ERROR) << "SendPosition2MeetingClient find meeting window failed. error code = " << GetLastError();
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendRemoteAssistanceInviteMsg") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_DICTIONARY)
			&& (arguments->GetType(5) == VTYPE_STRING)
			&& (arguments->GetType(6) == VTYPE_STRING)) {
			UCID uid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(4), uid)) {
				std::wstring confId = arguments->GetString(5).ToWString();
				std::wstring confPwd = arguments->GetString(6).ToWString();

				int32_t msgId;
				res = UCManager::GetInstance().GetCalendarServiceStrategy()->
					SendRemotAssistanceInviteMsg(uid, confId, confPwd, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendRemoteAssistanceAcceptMsg") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_DICTIONARY)
			&& (arguments->GetType(5) == VTYPE_STRING)) {
			UCID uid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(4), uid)) {
				std::wstring confId = arguments->GetString(5).ToWString();
				int32_t msgId;
				res = UCManager::GetInstance().GetCalendarServiceStrategy()->
					SendRemotAssistanceAcceptMsg(uid, confId, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendRemoteAssistanceRejectMsg") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_DICTIONARY)
			&& (arguments->GetType(5) == VTYPE_STRING)) {
			UCID uid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(4), uid)) {
				std::wstring confId = arguments->GetString(5).ToWString();
				int32_t msgId;
				res = UCManager::GetInstance().GetCalendarServiceStrategy()->
					SendRemotAssistanceRejectMsg(uid, confId, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"RevocationMeetingMessage"){
		if ((argLen == 7) && CefJSConversion::IsJsNumber(arguments, 4) && CefJSConversion::IsJsNumber(arguments, 5) &&
			(CefJSConversion::IsJsNumber(arguments, 6))){
				int64_t groupId = CefJSConversion::JSNumberToDouble(arguments, 4);
				int64_t eventId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
				int64_t seqId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 6);

				int32_t msgId;
				res = UCManager::GetInstance().GetCalendarServiceStrategy()->RevocationMeetingMessage(groupId, eventId, seqId, msgId);
				responseArgs->SetInt(2, msgId);
		}
		else{
			res = UCCLIENT_ARGUMENT_ERROR;
		}
		
	}
	else {
		res = UCCLIENT_METHOD_NOTFOUND;
	}
}

void UCExtension::FileTransferServiceMethodProcess(CefString method, int argLen, CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"UploadFile") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {
			wstring filePath = arguments->GetString(4).ToWString();
			int32_t taskId = 0;

			res = UCManager::GetInstance().GetFileTransferServiceStrategy()->UploadFile(filePath, taskId);

			responseArgs->SetInt(2,taskId);
		} else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	} else if (method == L"IsFileUploading") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t taskId = arguments->GetInt(4);
			int32_t isUploading = 0;

			res = UCManager::GetInstance().GetFileTransferServiceStrategy()->IsFileUploading(taskId, isUploading);
			responseArgs->SetInt(2, isUploading);
		} else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	} else if (method == L"CancelUploadFile") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t taskId = arguments->GetInt(4);

			res = UCManager::GetInstance().GetFileTransferServiceStrategy()->CancelUploadFile(taskId);
		} else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	} else if (method == L"DownloadFile") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_STRING) 
			&& (arguments->GetType(5) == VTYPE_STRING)) {
			
			int32_t taskId = 0;
			wstring downloadURL = arguments->GetString(4).ToWString();
			wstring savePath = arguments->GetString(5).ToWString();

			res = UCManager::GetInstance().GetFileTransferServiceStrategy()->DownloadFile(downloadURL, savePath, taskId);
			responseArgs->SetInt(2, taskId);
		} else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}

	} else if (method == L"IsFileDownloading") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t taskId = arguments->GetInt(4);
			int32_t isDownloading = 0;

			res = UCManager::GetInstance().GetFileTransferServiceStrategy()->IsFileDownloading(taskId, isDownloading);
			responseArgs->SetInt(2, isDownloading);
		}
	} else if (method == L"CancelDownloadFile") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t taskId = arguments->GetInt(4);

			res = UCManager::GetInstance().GetFileTransferServiceStrategy()->CancelDownloadFile(taskId);
		}
	} else if (method == L"DeleteFileFromFS") {
		if ((argLen == 5)
			&& (arguments->GetType(4) == VTYPE_STRING)) {
			const std::string fid = arguments->GetString(4);
			res = UCManager::GetInstance().GetFileTransferServiceStrategy()->DeleteFileFromFS(fid);
		}
	} else if (method == L"ResumeTask") {
		if ((argLen == 5)
			&& (arguments->GetType(4) == VTYPE_INT)) {
			int32_t taskId = arguments->GetInt(4);
			res = UCManager::GetInstance().GetFileTransferServiceStrategy()->ResumeTask(taskId);
		}
	} else if (method == L"UpdateLocalFilePath") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_STRING) && (arguments->GetType(5) == VTYPE_STRING)) {
			std::wstring fileURL = arguments->GetString(4).ToWString();
			std::wstring filePath = arguments->GetString(5).ToWString();

			res = UCManager::GetInstance().GetFileTransferServiceStrategy()->UpdateLocalFilePath(fileURL, filePath);
		}
	} else if (method == L"GetLocalFilePath") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {
			std::wstring fileURL = arguments->GetString(4).ToWString();
			std::wstring filePath = L"";

			res = UCManager::GetInstance().GetFileTransferServiceStrategy()->GetLocalFilePath(fileURL, filePath);
			responseArgs->SetString(2, filePath);
		}
	} else {
		res = UCCLIENT_MODULE_NOTFOUND;
	}
}

#ifdef PBX_SERVICE
void UCExtension::PbxServiceMethodProcess(CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {

	if (0 == method.compare(L"checkLogin")) { // 查看登陆情况
		if (argLen == 4) {
			int nRet = -1;
			res = UCManager::GetInstance().GetPbxServiceStrategy()->CheckLogin(nRet);

			responseArgs->SetInt(2, nRet);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"makeCall")) { // VoIP呼叫
		if ((argLen == 9) &&
			(arguments->GetType(4) == VTYPE_STRING
			&& arguments->GetType(5) == VTYPE_INT
			&& arguments->GetType(6) == VTYPE_STRING
			&& arguments->GetType(7) == VTYPE_INT
			&& arguments->GetType(8) == VTYPE_STRING)) {
			__int64 timeStamp = 0;
			int32_t taskID = 0;
			std::string userName = arguments->GetString(4).ToString();
			int userID = arguments->GetInt(5);
			std::string number = arguments->GetString(6).ToString();
			int callType = arguments->GetInt(7);
			std::string postValue = arguments->GetString(8).ToString();
			res = UCManager::GetInstance().GetPbxServiceStrategy()->MakeCall(
				userName, userID, number, callType, postValue, timeStamp, taskID);

			responseArgs->SetDouble(2, timeStamp);
			responseArgs->SetInt(3, taskID);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"answerCall")) { // 来电应答
		if ((argLen == 6) &&
			(arguments->GetType(4) == VTYPE_INT
			&& arguments->GetType(5) == VTYPE_INT)) {

			int32_t taskID = arguments->GetInt(4);
			int callType = arguments->GetInt(5);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->AnswerCall(taskID, callType);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"rejectCall")) { // 拒接来电
		if ((argLen == 5)
			&& (arguments->GetType(4) == VTYPE_INT)) {

			int32_t taskID = arguments->GetInt(4);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->RejectCall(taskID);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"hangupCall")) { // 挂断通话
		if ((argLen == 5) &&
			(arguments->GetType(4) == VTYPE_INT)) {

			int32_t taskID = arguments->GetInt(4);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->HangupCall(taskID);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"holdCall")) { // 通话保持
		if ((argLen == 5) &&
			(arguments->GetType(4) == VTYPE_INT)) {

			int32_t taskID = arguments->GetInt(4);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->HoldCall(taskID);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"unHoldCall")) { // 解除通话保持
		if ((argLen == 5) &&
			(arguments->GetType(4) == VTYPE_INT)) {

			int32_t taskID = arguments->GetInt(4);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->UnHoldCall(taskID);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"muteCall")) { // 通话静音
		if ((argLen == 5) &&
			(arguments->GetType(4) == VTYPE_INT)) {

			int32_t taskID = arguments->GetInt(4);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->MuteCall(taskID);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"unMuteCall")) { // 解除通话静音
		if ((argLen == 5) && 
			(arguments->GetType(4) == VTYPE_INT)) {

			int32_t taskID = arguments->GetInt(4);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->UnMuteCall(taskID);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"adjustMicLevel")) { // 调整通话麦克风音量
		if ((argLen == 6) && 
			(arguments->GetType(4) == VTYPE_INT
			&& arguments->GetType(5) == VTYPE_INT)) {

			int32_t taskID = arguments->GetInt(4);
			int level = arguments->GetInt(5);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->AdjustMicLevel(taskID, level);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"refreshMic")) { // 刷新麦克风
		if (argLen == 4) {
			int micState = -2;
			res = UCManager::GetInstance().GetPbxServiceStrategy()->RefreshMic(micState);

			responseArgs->SetInt(2, micState);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"sendDTMF")) { // 发送DTMF // 通话中，按键0-9*#
		if ((argLen == 6) &&
			(arguments->GetType(4) == VTYPE_INT
			&& arguments->GetType(5) == VTYPE_STRING)) {

			int32_t taskID = arguments->GetInt(4);
			std::string dtmf = arguments->GetString(5).ToString();

			res = UCManager::GetInstance().GetPbxServiceStrategy()->SendDTMF(taskID, dtmf);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"getCurrentPBXAccount")) { // 获取当前PBX号码
		if (argLen == 4) {

			std::string pbxAccount = "";
			res = UCManager::GetInstance().GetPbxServiceStrategy()->GetCurrentPBXAccount(pbxAccount);
			responseArgs->SetString(2, pbxAccount);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"incomingCallUserName")) { // 呼入电话用户名
		if ((argLen == 6) &&
			(arguments->GetType(4) == VTYPE_INT
			&& arguments->GetType(5) == VTYPE_STRING)) {

			int32_t taskID = arguments->GetInt(4);
			std::string userName = arguments->GetString(5).ToString();

			res = UCManager::GetInstance().GetPbxServiceStrategy()->IncomingCallUserName(
				taskID, userName);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"getCallRecordLst")) { // 获取通话记录列表
		if ((argLen == 6) &&
			arguments->GetType(4) == VTYPE_INT
			&& (CefJSConversion::IsJsNumber(arguments, 5))) {

			int count = arguments->GetInt(4);
			int64_t timeStamp = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->GetCallRecordLst(count, timeStamp);

			responseArgs->SetInt(2, count);

// 			if (res == UC_SUCCEEDED) {
// 				CefRefPtr<CefListValue> contactlist = CefListValue::Create();
// 
// 				int index = 0;
// 				PbxCallRecordList::iterator itor = lstData.begin();
// 				for (; itor != lstData.end(); ++itor) {
// 					AC_PbxCallRecord &Temp = (AC_PbxCallRecord)*itor;
// 
// 					contactlist->SetDictionary(index++, ConvertPbxCallRecordToDictionary(Temp));
// 				}
// 
// 				responseArgs->SetList(3, contactlist);
// 			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"getCallDetailsLst")) { // 获取通话详情列表
		if ((argLen == 8) &&
			(arguments->GetType(4) == VTYPE_STRING
			&& arguments->GetType(5) == VTYPE_INT
			&& CefJSConversion::IsJsNumber(arguments, 6)
			&& arguments->GetType(7) == VTYPE_INT)) {

			std::string number = arguments->GetString(4).ToString();
			int userID = arguments->GetInt(5);
			int64_t timeStamp = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 6);
			int count = arguments->GetInt(7);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->GetCallDetailsLst(
				number, userID, timeStamp, count);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"searchCallRecord")) { // 通话记录匹配搜索
		if ((argLen == 7) &&
			(arguments->GetType(4) == VTYPE_STRING
			&& CefJSConversion::IsJsNumber(arguments, 5)
			&& arguments->GetType(6) == VTYPE_INT)) {

			std::string keyWord = arguments->GetString(4).ToString();
			int64_t timeStamp = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			int count = arguments->GetInt(6);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->SearchCallRecord(
				keyWord, timeStamp, count);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"saveCallDetails")) { // 获取通话详情列表
		if ((argLen == 12) &&
			(CefJSConversion::IsJsNumber(arguments, 4)
			&& arguments->GetType(5) == VTYPE_INT
			&& arguments->GetType(6) == VTYPE_STRING
			&& arguments->GetType(7) == VTYPE_INT
			&& arguments->GetType(8) == VTYPE_INT
			&& arguments->GetType(9) == VTYPE_INT
			&& arguments->GetType(10) == VTYPE_INT
			&& arguments->GetType(11) == VTYPE_INT)) {

			int64_t timeStamp = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 4);
			int taskID = arguments->GetInt(5);
			std::string number = arguments->GetString(6).ToString();
			int userID = arguments->GetInt(7);
			std::string name = number;
			int incoming = arguments->GetInt(8);
			int type = arguments->GetInt(9);
			int state = arguments->GetInt(10);
			int druation = arguments->GetInt(11);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->SaveCallDetails(
				timeStamp, taskID, number, userID, name, incoming, type, state, druation);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"makeTwoWayCall")) { // 双向呼叫
		if ((argLen == 7) &&
			(arguments->GetType(4) == VTYPE_STRING
			&& arguments->GetType(5) == VTYPE_STRING
			&& arguments->GetType(6) == VTYPE_INT)) {

			std::string toNumber = arguments->GetString(4).ToString();
			std::string fromNumber = arguments->GetString(5).ToString();
			int openRecord = arguments->GetInt(6);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->MakeTwoWayCall(
				toNumber, fromNumber, openRecord);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"switchRecordInCall")) { // 通话中开始或停止录音
		if ((argLen == 6) &&
			(arguments->GetType(4) == VTYPE_INT
			&& arguments->GetType(5) == VTYPE_STRING)) {

			int recordSwitch = arguments->GetInt(4);
			std::string callUUID = arguments->GetString(5).ToString();

			res = UCManager::GetInstance().GetPbxServiceStrategy()->SwitchRecordInCall(
				recordSwitch, callUUID);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"getAllRecord")) { // 获取所有录音列表目录
		if ((argLen == 5) &&
			(arguments->GetType(4) == VTYPE_INT)) {

			int recordType = arguments->GetInt(4);

			res = UCManager::GetInstance().GetPbxServiceStrategy()->GetAllRecord(recordType);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (0 == method.compare(L"getRecord")) { // 获取单次通话录音
		if ((argLen == 6) &&
			(arguments->GetType(4) == VTYPE_INT
			&& arguments->GetType(5) == VTYPE_STRING)) {

			int recordType = arguments->GetInt(4);
			std::string callUUID = arguments->GetString(5).ToString();

			res = UCManager::GetInstance().GetPbxServiceStrategy()->GetRecord(recordType, callUUID);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else {
		res = UCCLIENT_MODULE_NOTFOUND;
	}
}
#endif // PBX_SERVICE

void UCExtension::CatchScreenServiceMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs, int requestId) {
	if (method == L"StartCatchScreen") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {
			wstring savePath = arguments->GetString(4).ToWString();
			res = UCManager::GetInstance().GetCatchScreenServiceStrategy()->StartCatchScreen(savePath, browser, requestId);
		} else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	} else {
		res = UCCLIENT_MODULE_NOTFOUND;
	}
}

void UCExtension::SearchServiceMethodProcess(CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {

	if (method == L"SearchServer") {		
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_STRING) &&
		 (arguments->GetType(5) == VTYPE_INT) && (arguments->GetType(6) == VTYPE_INT) && 
		 (arguments->GetType(7) == VTYPE_LIST)) {
			wstring key = arguments->GetString(4).ToWString();
			int32_t page = arguments->GetInt(5);
			int32_t count = arguments->GetInt(6);

			std::list<int32_t> scope;
			CefRefPtr<CefListValue> ceflist = arguments->GetList(7);
			for (std::size_t i = 0; i < ceflist->GetSize(); ++i) {
				if (ceflist->GetType(i) == VTYPE_INT) {
					scope.push_back(ceflist->GetInt(i));
				}
			}

			int searchId = UCManager::GetInstance().GetSearchServiceStrategy()->GetSearchRequestId();
			res = UCManager::GetInstance().GetSearchServiceStrategy()->SearchServer(key, page, count, scope, searchId);
			responseArgs->SetInt(2, searchId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
			return;
		}
	}
	else if (method == L"SearchMessage") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_INT) &&
			CefJSConversion::IsJsNumber(arguments, 5) && (arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_INT) && (arguments->GetType(8) == VTYPE_INT)) {
			
			ConversationType type = (ConversationType)arguments->GetInt(4);
			int64_t relateId = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
			wstring key = arguments->GetString(6).ToWString();
			int32_t page = arguments->GetInt(7);
			int32_t count = arguments->GetInt(8);
		

			int searchId = UCManager::GetInstance().GetSearchServiceStrategy()->GetSearchRequestId();
			res = UCManager::GetInstance().GetSearchServiceStrategy()->SearchMessage(type, relateId, key, page, count, searchId);
			responseArgs->SetInt(2, searchId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
			return;
		}
	}else if (method == L"SearchLocal") {

		wstring key = L"";
		int page = 0;
		int count = 0;
		std::list<int32_t> scope;
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_STRING) && 
			(arguments->GetType(5) == VTYPE_INT) && (arguments->GetType(6) == VTYPE_INT) && 
			(arguments->GetType(7) == VTYPE_LIST)) {
			
			wstring key = arguments->GetString(4).ToWString();
			int32_t page = arguments->GetInt(5);
			int32_t count = arguments->GetInt(6);

			std::list<int32_t> scope;
			CefRefPtr<CefListValue> ceflist = arguments->GetList(7);
			for (std::size_t i = 0; i < ceflist->GetSize(); ++i) {
				if (ceflist->GetType(i) == VTYPE_INT) {
					scope.push_back(ceflist->GetInt(i));
				}
			}

			int searchId = UCManager::GetInstance().GetSearchServiceStrategy()->GetSearchRequestId();
			responseArgs->SetInt(2, searchId);
			res = UCManager::GetInstance().GetSearchServiceStrategy()->SearchLocal(key, page, count, scope, searchId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
			return;
		}

	} else {
		res = UCCLIENT_MODULE_NOTFOUND;
	}
}

void UCExtension::LogServiceMethodProcess(CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res) {
	if (method == L"WriteLog") {
		if (argLen == 8 && arguments->GetType(4) == VTYPE_STRING && arguments->GetType(5) == VTYPE_INT &&
			arguments->GetType(6) == VTYPE_INT && arguments->GetType(7) == VTYPE_STRING) {
			std::wstring fileName = arguments->GetString(4).ToWString();
			int line = arguments->GetInt(5);
			int logLevel = arguments->GetInt(6);
			std::wstring content = arguments->GetString(7).ToWString();

			Logger::WriteLog(fileName, line, logLevel, content);
		} else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	} 
	else {
		res = UCCLIENT_MODULE_NOTFOUND;
	}
}

void UCExtension::AppMethodProcess(CefString method, int argLen,
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"EnableAutoStart") {
		res = Environment::EnableAutoStart();
	}
	else if (method == L"DisableAutoStart") {
		res = Environment::DisableAutoStart();
	}
	else if (method == L"IsAutoStart") {
		res = Environment::IsAutoStart();
	}
	else if (method == L"GetClientVersion") {
		std::wstring clientVersion = UCManager::GetInstance().GetClientVersion();
		res = UCCLIENT_SUCCESS;
		responseArgs->SetString(2, clientVersion);
	}
	else {
		res = UCCLIENT_MODULE_NOTFOUND;
	}
}

void UCExtension::SystemServiceMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs, int requestId, bool hasCallback) {
	if (method == L"playSound") {
		hasCallback = false;
		if (argLen == 7 && arguments->GetType(4) == VTYPE_STRING && arguments->GetType(5) == VTYPE_STRING && arguments->GetType(6) == VTYPE_BOOL) {
			std::string fileName = arguments->GetString(4).ToString();
			const std::string &id = arguments->GetString(5).ToString();
			bool loop = arguments->GetBool(6);

			std::fstream file;
			file.open(fileName, std::ios::in);
			if (!file) {
				fileName = FileUtil::ProcessPath() + "\\resources\\html\\" + fileName;
			}
			
			res = UCManager::GetInstance().getSystemService()->playSound(fileName, id, requestId, loop,browser);
		} else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	} 
	else if (method == L"stopSound") {
		hasCallback = false;
		if (argLen == 5 && arguments->GetType(4) == VTYPE_STRING) {
			const std::string& id = arguments->GetString(4).ToString();
			UCManager::GetInstance().getSystemService()->stopSound(id);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	} 
	else if (method == L"GetCurrentTimeZoneDisplay") {
		std::wstring display;
		res = UCManager::GetInstance().getSystemService()->getCurrentTimeZoneDisplay(display);
		responseArgs->SetString(2, display);
	}
	else {
		res = UCCLIENT_MODULE_NOTFOUND;
	}
}

void UCExtension::AudioServiceMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res, int requestId) {
		static AudioServiceStrategyAssist assist;
		res = assist.CallAudioServiceMethod(StringConvert::ToUTF8Str(method), argLen, arguments, requestId);
}

void UCExtension::SharingServiceMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen,
	CefRefPtr<CefListValue> arguments, int &res, int requestId) {
	UC_LOG(INFO) << "SharingServiceMethodProcess, method:" << method.c_str() << ", argLen:" << argLen;
	if (method == L"StartDesktopShare") {
		if (argLen == 4) {
			res = UCManager::GetInstance().GetSharingServiceStrategy()->StartDesktopShare();
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"StopDesktopShare") {
		if (argLen == 4) {
			res = UCManager::GetInstance().GetSharingServiceStrategy()->StopDesktopShare();
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"StartDesktopView") {
		if (argLen == 5) {
			std::wstring sharingName= arguments->GetString(4).ToWString();
			res = UCManager::GetInstance().GetSharingServiceStrategy()->StartDesktopView(sharingName);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"StopDesktopView") {
		if (argLen == 4) {
			res = UCManager::GetInstance().GetSharingServiceStrategy()->StopDesktopView();
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else {

	}
}

void UCExtension::OpenApiServiceMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs, bool& hasCallback, int requestId) {
	
	if (method == L"GetAppList") {
		if (argLen == 4) {
			res = UCManager::GetInstance().GetOpenApiServiceStrategy()->GetAppList();
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetAppInfo") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_LIST)) {

			CefRefPtr<CefListValue> ceflist = arguments->GetList(4);
			std::list<int64_t> appIds;
			for (std::size_t i = 0; i < ceflist->GetSize(); ++i) {
				if (CefJSConversion::IsJsNumber(ceflist, i)) {
					int64_t appId = (int64_t)CefJSConversion::JSNumberToDouble(ceflist, i);
					appIds.push_back(appId);
				}
			}
			res = UCManager::GetInstance().GetOpenApiServiceStrategy()->GetAppInfo(appIds);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if(method == L"GetChatHistory") {
		if ((argLen == 8) && (CefJSConversion::IsJsNumber(arguments, 4)) &&
			(arguments->GetType(5) == VTYPE_INT) && (CefJSConversion::IsJsNumber(arguments, 6)) &&
			(CefJSConversion::IsJsNumber(arguments, 7))) {
			int64_t appId = arguments->GetInt(4);
			int32_t count = arguments->GetInt(5);
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 6);
			int64_t endTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 7);
			res = UCManager::GetInstance().GetOpenApiServiceStrategy()->GetChatHistory(appId, count, startTime, endTime);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetOAMessageStatus") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_LIST)) {

			CefRefPtr<CefListValue> infoList = arguments->GetList(4);

			std::list<AppMessageInfo> Infos;
			for (std::size_t i = 0; i < infoList->GetSize(); ++i) {
				if (infoList->GetType(i) == VTYPE_DICTIONARY) {
					AppMessageInfo info;
					ConvertDictionaryToAppMessageInfo(infoList->GetDictionary(i), info);
					Infos.push_back(info);
				}
			}

			res = UCManager::GetInstance().GetOpenApiServiceStrategy()->GetOAMessageStatus(Infos);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UpdateOAMessageStatus") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_LIST)) {

			CefRefPtr<CefListValue> infoList = arguments->GetList(4);

			std::list<AppMessageStatusInfo> Infos;
			for (std::size_t i = 0; i < infoList->GetSize(); ++i) {
				if (infoList->GetType(i) == VTYPE_DICTIONARY) {
					AppMessageStatusInfo info;
					ConvertDictionaryToAppMessageStatusInfo(infoList->GetDictionary(i), info);
					Infos.push_back(info);
				}
			}

			res = UCManager::GetInstance().GetOpenApiServiceStrategy()->UpdateOAMessageStatus(Infos);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetMessageAckStatus") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_LIST)) {

			CefRefPtr<CefListValue> infoList = arguments->GetList(4);

			std::list<AppMessageInfo> Infos;
			for (std::size_t i = 0; i < infoList->GetSize(); ++i) {
				if (infoList->GetType(i) == VTYPE_DICTIONARY) {
					AppMessageInfo info;
					ConvertDictionaryToAppMessageInfo(infoList->GetDictionary(i), info);
					Infos.push_back(info);
				}
			}

			res = UCManager::GetInstance().GetOpenApiServiceStrategy()->GetMessageAckStatus(Infos);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ResendMessage") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t msgId = arguments->GetInt(4);
			res = UCManager::GetInstance().GetOpenApiServiceStrategy()->ResendMessage(msgId);
			responseArgs->SetInt(2, msgId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendTextMessage") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_DICTIONARY) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_INT) && (arguments->GetType(8) == VTYPE_STRING)) {
			UCID ucid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(4), ucid)) {
				
				std::wstring textTitle = arguments->GetString(5).ToWString();
				std::wstring textContent = arguments->GetString(6).ToWString();
				int32_t textType = arguments->GetInt(7);
				std::wstring detailContent = arguments->GetString(8).ToWString();

				int32_t msgId;
				res = UCManager::GetInstance().GetOpenApiServiceStrategy()->SendTextMessage(ucid, textTitle, textContent, textType, detailContent, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendImageMessage") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_DICTIONARY) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_INT) &&
			(arguments->GetType(7) == VTYPE_STRING)) {
			UCID ucid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(4), ucid)) {
				std::wstring imageId = arguments->GetString(5).ToWString();
				int32_t imageType = arguments->GetInt(6);
				std::wstring imagePath = arguments->GetString(7).ToWString();

				int32_t msgId;
				res = UCManager::GetInstance().GetOpenApiServiceStrategy()->SendImageMessage(ucid, imageId, imageType, imagePath, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendFileMessage") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_DICTIONARY) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_INT) && 
			(arguments->GetType(7) == VTYPE_STRING) && (arguments->GetType(8) == VTYPE_STRING)) {
			UCID ucid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(4), ucid)) {
				std::wstring fileId = arguments->GetString(5).ToWString();
				int32_t fileType = arguments->GetInt(6);
				std::wstring fileName = arguments->GetString(7).ToWString();
				int64_t fileSize = StringToInt64(arguments, 8);

				int32_t msgId;
				res = UCManager::GetInstance().GetOpenApiServiceStrategy()->SendFileMessage(ucid, fileId, fileType, fileName, fileSize, msgId);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendMessageRead") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_DICTIONARY) &&
			(arguments->GetType(5) == VTYPE_INT) && (CefJSConversion::IsJsNumber(arguments, 6))) {
			UCID appUid;
			if (ConvertDictionaryToUCID(arguments->GetDictionary(4), appUid)) {
				int32_t msgId = arguments->GetInt(5);
				int64_t msgSeq = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 6);

				res = UCManager::GetInstance().GetOpenApiServiceStrategy()->
					SendMessageRead(appUid, msgId, msgSeq);
				responseArgs->SetInt(2, msgId);
			}
			else {
				res = UCCLIENT_ARGUMENT_ERROR;
			}
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendMessagesReadById") {
		if ((argLen == 5) && (CefJSConversion::IsJsNumber(arguments, 4))) {
			int64_t appId = arguments->GetInt(4);

			res = UCManager::GetInstance().GetOpenApiServiceStrategy()->
				SendMessagesRead(appId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SendRequest") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_LIST)) {
			
			hasCallback = false;
			RequestMethod mehtod = (RequestMethod)arguments->GetInt(4);
			std::wstring url = arguments->GetString(5).ToWString();
			std::wstring body = arguments->GetString(6).ToWString();
			CefRefPtr<CefListValue> infoList = arguments->GetList(7);
			std::list<HeadInfo> Infos;
			for (std::size_t i = 0; i < infoList->GetSize(); ++i) {
				if (infoList->GetType(i) == VTYPE_DICTIONARY) {
					HeadInfo info;
					ConvertDictionaryToHeadInfo(infoList->GetDictionary(i), info);
					Infos.push_back(info);
				}
			}

			res = UCManager::GetInstance().GetOpenApiServiceStrategy()->SendRequest(browser, requestId, mehtod, url, body, Infos);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else {
		res = UCCLIENT_METHOD_NOTFOUND;
	}
}

void UCExtension::BBSServiceMethodProcess(CefString method, int argLen, CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"BBSRequest") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {
			wstring bbsRequestData = arguments->GetString(4).ToWString();
			int32_t taskId = 0;
			res = UCManager::GetInstance().GetBBSServiceStrategy()->BBSRequest(bbsRequestData, taskId);
			responseArgs->SetInt(2, taskId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
}

void UCExtension::MicroblogServiceMethodProcess(CefString method, int argLen, 
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	
	if (method == L"GetHomePageURL") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t userId = arguments->GetInt(4);
			std::wstring url = UCManager::GetInstance().GetMicroblogServiceStrategy()->GetHomePageURL(userId);
			responseArgs->SetString(2, url);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetUserHomePageURL")
	{
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT && arguments->GetType(5) == VTYPE_INT)){
			int32_t selfId = arguments->GetInt(4);
			int32_t userId = arguments->GetInt(5);
			std::wstring url = UCManager::GetInstance().GetMicroblogServiceStrategy()->GetUserHomePageURL(selfId, userId);
			responseArgs->SetString(2, url);
		}
		else
		{
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetRelation") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t userId = arguments->GetInt(4);
			int32_t relation = UCManager::GetInstance().GetMicroblogServiceStrategy()->GetRelation(userId);
			responseArgs->SetInt(2, relation);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetNewMsgCount") {
		if (argLen == 4) {
			UCManager::GetInstance().GetMicroblogServiceStrategy()->GetNewMsgCount();
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"AddAttention") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t userId = arguments->GetInt(4);
			UCManager::GetInstance().GetMicroblogServiceStrategy()->AddAttention(userId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"CancelAttention") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int32_t userId = arguments->GetInt(4);
			UCManager::GetInstance().GetMicroblogServiceStrategy()->CancelAttention(userId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"ShareMeetingRecording") {

		if ((argLen == 10) && (arguments->GetType(4) == VTYPE_STRING)
			&& (arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_STRING)
			&& arguments->GetType(7) == VTYPE_STRING && CefJSConversion::IsJsNumber(arguments, 8)
			&& (arguments->GetType(9) == VTYPE_STRING)) {

			std::wstring urlVideo = arguments->GetString(4).ToWString();
			std::wstring urlThumb = arguments->GetString(5).ToWString();
			std::wstring fileName = arguments->GetString(6).ToWString();
			int64_t fileSize = StringToInt64(arguments, 7);
			int64_t voiceTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 8);
			std::wstring comments = arguments->GetString(9).ToWString();
			UCManager::GetInstance().GetMicroblogServiceStrategy()
				->ShareMeetingRecording(urlVideo, urlThumb, fileName, fileSize, voiceTime, comments);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else {
		res = UCCLIENT_METHOD_NOTFOUND;
	}
}

void UCExtension::UpdateServiceMethodProcess(CefString method, int argLen,
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs, int requestId) {
	if (method == L"CheckUpdates") {
		if (argLen == 4) {
			res = UCManager::GetInstance().GetUpdateServiceStrategy()->CheckUpdates(requestId);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else if (method == L"Update") {
		if (argLen == 4) {
			res = UCManager::GetInstance().GetUpdateServiceStrategy()->Update();
		}
	}
	else {
		res = UCCLIENT_MODULE_NOTFOUND;
	}
}

void UCExtension::ExternalServiceMethodProcess(CefString method, int argLen,
	CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs){
	if (method == L"GetChatHistory") {
		if ((argLen == 9) && (arguments->GetType(4) == VTYPE_INT) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_INT) &&
			CefJSConversion::IsJsNumber(arguments, 7) && CefJSConversion::IsJsNumber(arguments, 8)) {

			ConversationType type = (ConversationType)arguments->GetInt(4);
			wstring externalId = arguments->GetString(5).ToWString();
			int32_t count = arguments->GetInt(6);
			int64_t startTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 7);
			int64_t endTime = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 8);
	
			res = UCManager::GetInstance().GetExternalServiceStrategy()->GetChatHistory(type, externalId, count, startTime, endTime);
		}
		else {
			res = UCCLIENT_ARGUMENT_ERROR;
		}
	}
	else {
		res = UCCLIENT_MODULE_NOTFOUND;
	}
}

CefRefPtr<CefDictionaryValue> UCExtension::ConvertConversationToDictionary(ConversationInfo & info) {
	CefRefPtr<CefDictionaryValue> dictionary = CefDictionaryValue::Create();
	dictionary->SetBool(L"hidden", info.isHidden());
	dictionary->SetDouble(L"lastMsgSeq",(double) info.GetLastMsgSeq());
	dictionary->SetDouble(L"lastMsgTime",(double) info.GetLastMsgTime());
	dictionary->SetDouble(L"newMsgCount",info.GetNewMsgCount());
	dictionary->SetDouble(L"relateID",(double)info.GetRelateId());
	dictionary->SetInt(L"conversation",info.GetConversation());
	dictionary->SetInt(L"type",info.GetType());

	return dictionary;
}

CefRefPtr<CefDictionaryValue> UCExtension::ConvertBasicContactToDictionary(BasicContactInfo & contact) {
	CefRefPtr<CefDictionaryValue> contactDictionary = CefDictionaryValue::Create();
	contactDictionary->SetInt(L"userId", contact.GetUserId());
	contactDictionary->SetString(L"displayName", contact.GetDisplayName());
	contactDictionary->SetString(L"pinyin", contact.GetPinyin());
	contactDictionary->SetString(L"localAvatar", contact.GetLocalAvatar());
	contactDictionary->SetString(L"remoteAvatarUrl", contact.GetRemoteAvatarUrl());
	contactDictionary->SetString(L"personalSign", contact.GetPersonalSign());
	contactDictionary->SetString(L"position", contact.GetPosition());
	contactDictionary->SetInt(L"departmentId", contact.GetDepartmentId());
	contactDictionary->SetString(L"departmentName", contact.GetDepartmentName());
	contactDictionary->SetInt(L"relation", contact.GetRelation());

	return contactDictionary;
}

CefRefPtr<CefDictionaryValue> UCExtension::ConvertContactToDictionary(ContactInfo & contact) {
	CefRefPtr<CefDictionaryValue> contactDictionary = CefDictionaryValue::Create();
	contactDictionary->SetInt(L"userId", contact.GetUserId());
	contactDictionary->SetInt(L"siteId", contact.GetSiteId());
	contactDictionary->SetDouble(L"cardVersion", (double) contact.GetCardVersion());
	contactDictionary->SetInt(L"status", contact.GetStatus());
	contactDictionary->SetString(L"displayName", contact.GetDisplayName());
	contactDictionary->SetString(L"pinyin", contact.GetPingyin());
	contactDictionary->SetInt(L"sex", contact.GetSex());
	contactDictionary->SetString(L"account", contact.GetAccount());
	contactDictionary->SetString(L"email", contact.GetEmail());
	contactDictionary->SetString(L"mobile", contact.GetMobile());
	contactDictionary->SetString(L"workPhone", contact.GetWorkPhone());
	contactDictionary->SetString(L"localAvatar", contact.GetLocalAvatar());
	contactDictionary->SetString(L"remoteAvatarUrl", contact.GetRemoteAvatarUrl());
	contactDictionary->SetString(L"personalSign", contact.GetPersonalSign());
	contactDictionary->SetInt(L"departmentId", contact.GetDepartmentId());
	contactDictionary->SetString(L"departmentName", contact.GetDepartmentName());
	contactDictionary->SetString(L"position", contact.GetPosition());
	contactDictionary->SetInt(L"superiorId", contact.GetSuperiorId());
	contactDictionary->SetString(L"superiorName", contact.GetSuperiorName());
	contactDictionary->SetInt(L"colleagueNum", contact.GetColleagueNum());
	contactDictionary->SetInt(L"type", contact.GetType());

	
	CefRefPtr<CefListValue> tagList = CefListValue::Create();

	std::vector<ContactTag> tags;
	//contact.GetTags(tags);

	//for (std::size_t i = 0; i < tags.size(); i++) {
	//	CefRefPtr<CefDictionaryValue> tagDictionary = CefDictionaryValue::Create();
	//	tagDictionary->SetInt(L"id", tags[i].GetId());
	//	tagDictionary->SetInt(L"inputType", tags[i].GetInputType());
	//	tagDictionary->SetString(L"name", tags[i].GetName());
	//	tagDictionary->SetString(L"value", tags[i].GetValue());
	//	tagDictionary->SetInt(L"scope", tags[i].GetScope());
	//	tagDictionary->SetInt(L"state", tags[i].GetState());
	//	tagDictionary->SetInt(L"type", tags[i].GetType());
	//	tagDictionary->SetInt(L"userId", tags[i].GetUserId());
	//	tagList->SetDictionary(i, tagDictionary);
	//}
	//contactDictionary->SetList(L"tags", tagList);

	return contactDictionary;
}

bool UCExtension::ConvertDictionaryToContactTag(CefRefPtr<CefDictionaryValue> tagDictionary, 
	ContactTag &tag) {
	// cef bug：js对象中的整型值转换为CefDictionaryValue类型时都变为double类型
	// 所以下面的整型通过GetDouble方法获取
	if ((tagDictionary->GetType(L"id") == VTYPE_DOUBLE) &&
		(tagDictionary->GetType(L"inputType") == VTYPE_DOUBLE) &&
		(tagDictionary->GetType(L"name") == VTYPE_STRING) &&
		(tagDictionary->GetType(L"value") == VTYPE_STRING) &&
		(tagDictionary->GetType(L"scope") == VTYPE_DOUBLE) &&
		(tagDictionary->GetType(L"state") == VTYPE_DOUBLE) &&
		(tagDictionary->GetType(L"type") == VTYPE_DOUBLE) &&
		(tagDictionary->GetType(L"userId") == VTYPE_DOUBLE)) {

		tag.SetId((int32_t)tagDictionary->GetDouble(L"id"));
		tag.SetInputType(ContactTag::InputType((int32_t)tagDictionary->GetDouble(L"inputType")));
		tag.SetName(tagDictionary->GetString(L"name"));
		tag.SetValue(tagDictionary->GetString(L"value"));
		tag.SetScope(ContactTag::Scope((int32_t)tagDictionary->GetDouble(L"scope")));
		tag.SetState((int32_t)tagDictionary->GetDouble(L"state"));
		tag.SetType(ContactTag::Type((int32_t)tagDictionary->GetDouble(L"type")));
		tag.SetUserId((int32_t)tagDictionary->GetDouble(L"userId"));

		return true;
	}

	return false;
}

bool UCExtension::ConvertDictionaryToUCID(CefRefPtr<CefDictionaryValue> dictionary, UCID& ucid) {
	if ((dictionary->GetType(L"userId") == VTYPE_DOUBLE) &&
		(dictionary->GetType(L"siteId") == VTYPE_DOUBLE) &&
		(dictionary->GetType(L"resId") == VTYPE_DOUBLE)) {
		ucid.SetUserId((int32_t)dictionary->GetDouble(L"userId"));
		ucid.SetSiteId((int32_t)dictionary->GetDouble(L"siteId"));
		ucid.SetResId((int32_t)dictionary->GetDouble(L"resId"));
		return true;
	}
	return false;
}

bool UCExtension::ConvertDictionaryToAttendee(CefRefPtr<CefDictionaryValue> dictionary, 
	Attendee &attendee) {
	if (dictionary->GetType(L"account") == VTYPE_STRING) {

		attendee.SetAccount(dictionary->GetString(L"account"));
		if (dictionary->GetType(L"type") == VTYPE_DOUBLE) {
			attendee.SetType((uc::AttendeeType)(int)dictionary->GetDouble(L"type"));
		}
		if (dictionary->GetType(L"status") == VTYPE_DOUBLE) {
			attendee.SetStatus((uc::AttendeeStatus)(int)dictionary->GetDouble(L"status"));
		}
		if (dictionary->GetType(L"name") == VTYPE_STRING) {
			attendee.SetName(dictionary->GetString(L"name"));
		}
		return true;
	}

	return false;
}


bool UCExtension::ConvertDictionaryToCycleRole(CefRefPtr<CefDictionaryValue> dictionary, CycleRole &role) {

	if ((dictionary->GetType(L"repeatType") == VTYPE_STRING) 
		&& (dictionary->GetType(L"interval") == VTYPE_DOUBLE)
		&& ((dictionary->GetType(L"repeatCount") == VTYPE_DOUBLE) || (dictionary->GetType(L"repeatEndDate") == VTYPE_DOUBLE))){

		std::string repeatType = dictionary->GetString(L"repeatType");
		role.RepeatType(repeatType);
		role.Interval(dictionary->GetDouble(L"interval"));
		if (dictionary->GetType(L"repeatCount") == VTYPE_DOUBLE) {
			role.RepeatCount(dictionary->GetDouble(L"repeatCount"));
		}

		if (dictionary->GetType(L"repeatEndDate") == VTYPE_DOUBLE) {
			role.RepeatEndDate(dictionary->GetDouble(L"repeatEndDate"));
		} 
		
		if (role.RepeatCount() < 2 && role.RepeatEndDate() < 1) {
			return false;
		}

		if (repeatType.compare("weekly") == 0) {
			if (dictionary->GetType(L"weekdays") == VTYPE_STRING) {
				role.Weekdays(dictionary->GetString(L"weekdays"));
			}
			else{
				return false;
			}
		}else if (repeatType.compare("monthlyByDate") == 0) {
			if (dictionary->GetType(L"dayNo") == VTYPE_DOUBLE) {
				role.DayNo(dictionary->GetDouble(L"dayNo"));
			}
			else {
				return false;
			}
		}else if (repeatType.compare("monthlyBySetPos") == 0) {
			if ((dictionary->GetType(L"weekdays") == VTYPE_STRING) && (dictionary->GetType(L"weekNo") == VTYPE_DOUBLE)) {
				role.Weekdays(dictionary->GetString(L"weekdays"));
				role.WeekNo(dictionary->GetDouble(L"weekNo"));
			}
			else {
				return false;
			}
		}else if (repeatType.compare("yearlyByDate") == 0) {
			if ((dictionary->GetType(L"monthNo") == VTYPE_DOUBLE) && (dictionary->GetType(L"dayNo") == VTYPE_DOUBLE)) {
				role.MonthNo(dictionary->GetDouble(L"monthNo"));
				role.DayNo(dictionary->GetDouble(L"dayNo"));
			}
			else {
				return false;
			}
		}else if (repeatType.compare("yearlyBySetPos") == 0) {
			if ((dictionary->GetType(L"monthNo") == VTYPE_DOUBLE) && (dictionary->GetType(L"weekNo") == VTYPE_DOUBLE)
				&& (dictionary->GetType(L"weekdays") == VTYPE_STRING)) {
				role.MonthNo(dictionary->GetDouble(L"monthNo"));
				role.WeekNo(dictionary->GetDouble(L"weekNo"));
				role.Weekdays(dictionary->GetString(L"weekdays"));
			}
			else{
				return false;
			}
		}else if (repeatType.compare("daily") == 0) {
			return true;
		}
		else{
			return false;
		}
	}

	return false;
}

bool UCExtension::ConvertDictionaryToUserInfo(CefRefPtr<CefDictionaryValue> dictionary, 
	UserInfo &userInfo) {
	if ((dictionary->GetType(L"userId") == VTYPE_DOUBLE) &&
		(dictionary->GetType(L"username") == VTYPE_STRING)) {

		userInfo.ucid.SetUserId((int32_t)dictionary->GetDouble(L"userId"));
		userInfo.userName = dictionary->GetString(L"username");
		if (dictionary->GetType(L"role") == VTYPE_DOUBLE) {
			userInfo.role = GroupRole((int32_t)dictionary->GetDouble(L"role"));
		}

		return true;
	}

	return false;
}


CefRefPtr<CefDictionaryValue> UCExtension::ConvertConferenceInfoToDictionary( Meeting & meeting ) {
	CefRefPtr<CefDictionaryValue> conferenceDictionary = CefDictionaryValue::Create();
	conferenceDictionary->SetString(L"conferenceId", UTF2WJSON(meeting.GetConferneceId()));
	conferenceDictionary->SetDouble(L"eventId", (double)meeting.GetEventId());
	conferenceDictionary->SetDouble(L"parentId", (double)meeting.GetParentId());
	conferenceDictionary->SetDouble(L"groupId", (double)meeting.GetGroupId());
	conferenceDictionary->SetInt(L"authorizerId",meeting.GetAuthorizerId());
	conferenceDictionary->SetString(L"hostPassword", UTF2WJSON(meeting.GetHostPassword()));
	conferenceDictionary->SetString(L"attendeePassword", UTF2WJSON(meeting.GetAttendeePassword()));
	conferenceDictionary->SetString(L"billingCode", UTF2WJSON(meeting.GetBillingCode()));
	conferenceDictionary->SetInt(L"isGNet", meeting.IsGNet());
	conferenceDictionary->SetInt(L"isCycle", meeting.IsCycle());
	conferenceDictionary->SetInt(L"isWholeDay", meeting.IsWholeDay());
	conferenceDictionary->SetInt(L"updateFlag", meeting.GetUpdateFlag());
	conferenceDictionary->SetDouble(L"lastUpdateTime", (double)meeting.GetLastUpdateTime());
	conferenceDictionary->SetDouble(L"startTime", (double)meeting.GetStartTime());
	conferenceDictionary->SetDouble(L"oriTime", (double)meeting.GetOriTime());
	conferenceDictionary->SetString(L"title", URLDATA_UTF2WTEXT(meeting.GetTitle()));
	conferenceDictionary->SetString(L"summary", URLDATA_UTF2WTEXT(meeting.GetSummary()));
	conferenceDictionary->SetInt(L"duration", meeting.GetDuration());
	conferenceDictionary->SetString(L"location", URLDATA_UTF2WTEXT(meeting.GetLocation()));
	conferenceDictionary->SetString(L"timeZone", meeting.GetTimeZone());
	conferenceDictionary->SetInt(L"language", meeting.GetLanguage());
	conferenceDictionary->SetInt(L"emailLanguage", meeting.GetEmailLanguage());
	conferenceDictionary->SetInt(L"isCycle", meeting.IsCycle());
	conferenceDictionary->SetInt(L"hostId", meeting.GetHostId());
	conferenceDictionary->SetBool(L"isOld", meeting.IsOld());
	conferenceDictionary->SetInt(L"status", meeting.GetStatus());
	conferenceDictionary->SetString(L"hostName", URLDATA_UTF2WTEXT(meeting.GetHostName()));

	CefRefPtr<CefDictionaryValue> cycleRoleDictionary = CefDictionaryValue::Create();
	cycleRoleDictionary->SetString(L"repeatType", meeting.GetCycleRole().RepeatType());
	cycleRoleDictionary->SetInt(L"interval", meeting.GetCycleRole().Interval());
	cycleRoleDictionary->SetInt(L"repeatCount", meeting.GetCycleRole().RepeatCount());
	cycleRoleDictionary->SetInt(L"repeatEndDate", meeting.GetCycleRole().RepeatEndDate());
	cycleRoleDictionary->SetInt(L"monthNo", meeting.GetCycleRole().MonthNo());
	cycleRoleDictionary->SetInt(L"weekNo", meeting.GetCycleRole().WeekNo());
	cycleRoleDictionary->SetInt(L"dayNo", meeting.GetCycleRole().DayNo());
	cycleRoleDictionary->SetString(L"weekdays", meeting.GetCycleRole().Weekdays());
	conferenceDictionary->SetDictionary(L"cycleRole", cycleRoleDictionary);

	CefRefPtr<CefListValue> attendeeList = CefListValue::Create();
	const std::vector<Attendee>& attendees = meeting.GetAttendees();
	for (std::size_t i = 0; i < attendees.size(); i++) {
		CefRefPtr<CefDictionaryValue> attendeeDictionary = CefDictionaryValue::Create();
		attendeeDictionary->SetString(L"account", attendees[i].GetAccount());
		attendeeDictionary->SetInt(L"type", (int)attendees[i].GetType());
		attendeeDictionary->SetString(L"name", UTF2WJSON(attendees[i].GetName()));
		attendeeDictionary->SetInt(L"status", (int)attendees[i].GetStatus());
		attendeeList->SetDictionary(i, attendeeDictionary);
	}
	conferenceDictionary->SetList(L"attendees", attendeeList);

	return conferenceDictionary;
}

CefRefPtr<CefDictionaryValue> UCExtension::ConvertGroupInfoToDictionary(GroupInfo &info){

	CefRefPtr<CefDictionaryValue> groupDictionary = CefDictionaryValue::Create();
	groupDictionary->SetDouble(L"id", (double)info.GetId());
	groupDictionary->SetInt(L"siteId", info.GetSiteId());
	groupDictionary->SetString(L"name", UTF2WJSON(info.GetName()));
	groupDictionary->SetString(L"pinyin", UTF2WJSON(info.GetPinYinName()));
	groupDictionary->SetDouble(L"createTime", (double)info.GetCreateTime());
	groupDictionary->SetString(L"avatarIndex", UTF2WJSON(info.GetAvatarIndex()));
	groupDictionary->SetString(L"localAvatar", UTF2WJSON(info.GetLocalAvatar()));
	groupDictionary->SetInt(L"joinState", info.GetJoinState());
	groupDictionary->SetInt(L"memberCount", info.GetMemberCount());
	groupDictionary->SetInt(L"adminId", info.GetAdminId());
	groupDictionary->SetInt(L"alertState", info.GetGroupAlert());
	groupDictionary->SetInt(L"alertRule", info.GetAlertRule());
	groupDictionary->SetInt(L"alertTime", info.GetAlertTime());
	groupDictionary->SetInt(L"topState", info.GetGroupTop());
	groupDictionary->SetInt(L"status", info.GetGroupStatus());
	groupDictionary->SetInt(L"disturbState", info.GetGroupDisturb());
	groupDictionary->SetInt(L"nameFlag", info.GetNameFlag());
	groupDictionary->SetInt(L"groupType", info.GetGroupType());
	groupDictionary->SetInt(L"logoLevel", info.GetLogoLevel());
	groupDictionary->SetInt(L"mountId", info.GetMountId());

	CefRefPtr<CefListValue> memberList = CefListValue::Create();
	int32_t i = 0;
	const MemberList& members = info.GetMember();
	for (MemberList::const_iterator iter = members.begin(); iter != members.end(); ++iter) {
		CefRefPtr<CefDictionaryValue> memberDictionary = CefDictionaryValue::Create();
		memberDictionary->SetInt(L"userId", (*iter).userId);
		memberDictionary->SetDouble(L"joinTime", (double)(*iter).joinTime);

		memberList->SetDictionary(i++, memberDictionary);
	}
	groupDictionary->SetList(L"members", memberList);

	return groupDictionary;
}

bool UCExtension::ConvertDictionaryToAppMessageInfo(CefRefPtr<CefDictionaryValue> dictionary, AppMessageInfo &info){

	if ((dictionary->GetType(L"appId") == VTYPE_DOUBLE) &&
		(dictionary->GetType(L"msgSeq") == VTYPE_DOUBLE)) {

		info.appId = (int64_t)dictionary->GetDouble(L"appId");
		info.msgSeq = (int64_t)dictionary->GetDouble(L"msgSeq");

		return true;
	}

	return false;
}


bool UCExtension::ConvertDictionaryToAppMessageStatusInfo(CefRefPtr<CefDictionaryValue> dictionary, AppMessageStatusInfo &info){

	if ((dictionary->GetType(L"appId") == VTYPE_DOUBLE) &&
		(dictionary->GetType(L"msgSeq") == VTYPE_DOUBLE) &&
		(dictionary->GetType(L"status") == VTYPE_DOUBLE)) {

		info.appId = (int64_t)dictionary->GetDouble(L"appId");
		info.msgSeq = (int64_t)dictionary->GetDouble(L"msgSeq");
		info.status = (int32_t)dictionary->GetDouble(L"status");

		return true;
	}

	return false;
}

bool UCExtension::ConvertDictionaryToHeadInfo(CefRefPtr<CefDictionaryValue> dictionary, HeadInfo &info){

	CefDictionaryValue::KeyList keys;
	dictionary->GetKeys(keys);
	CefDictionaryValue::KeyList::const_iterator begin = keys.begin();
	CefDictionaryValue::KeyList::const_iterator end = keys.end();
	for (CefDictionaryValue::KeyList::const_iterator iter = begin; iter != end; ++iter) {
		CefString key = *iter;
		if (VTYPE_STRING == dictionary->GetType(key)){
			info.headKey = key;
			info.headValue = dictionary->GetString(key);
			
			return true;
		}
	}
	return false;
}

//等待显示主窗口
DWORD WINAPI WaitToShowMainWndThreadProc(__in LPVOID lpParam) {
	std::wstring title = ucclient::UCManager::GetInstance().GetDefaultWinTitle();
	HANDLE hEvent = (HANDLE)lpParam;
	while (1) {
		WaitForSingleObject(hEvent, INFINITE);
		UC_LOG(INFO) << "WaitToShowMainWndThreadProc notified.";
		ResetEvent(hEvent);
		HWND hWnd = FindWindow(UCCLIENT_CLASS_NAME, title.c_str());
		if (hWnd != NULL) {
			PostMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);

			DWORD dwThreadProcessID = GetWindowThreadProcessId(::GetForegroundWindow(), NULL);
			AttachThreadInput(dwThreadProcessID, GetCurrentThreadId(), TRUE);
			::SetForegroundWindow(hWnd);
			AttachThreadInput(dwThreadProcessID, GetCurrentThreadId(), FALSE);
		}
	}

	CloseHandle(hEvent);
	return 0;
}

//等待会议结束
DWORD WINAPI WaitMeetingClosedThreadProc(__in LPVOID lpParam) {
	std::wstring title = ucclient::UCManager::GetInstance().GetDefaultWinTitle();
	HANDLE hEvent = (HANDLE)lpParam;
	while (1) {
		WaitForSingleObject(hEvent, INFINITE);
		UC_LOG(INFO) << "WaitMeetingClosedThreadProc notified.";
		ResetEvent(hEvent);
		bInMeeting = false;
		HWND hWnd = FindWindow(UCCLIENT_CLASS_NAME, title.c_str());
		if (hWnd != NULL) {
			PostMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);

			DWORD dwThreadProcessID = GetWindowThreadProcessId(::GetForegroundWindow(), NULL);
			AttachThreadInput(dwThreadProcessID, GetCurrentThreadId(), TRUE);
			::SetForegroundWindow(hWnd);
			AttachThreadInput(dwThreadProcessID, GetCurrentThreadId(), FALSE);
		}

		UCManager::GetInstance().GetCalendarServiceStrategy()->
			NotifyMeetingClosed();
	}

	CloseHandle(hEvent);
	return 0;
}


//等待云会议准备成功
DWORD WINAPI WaitMeetingReadiedThreadProc(__in LPVOID lpParam) {

	HANDLE hEvent = (HANDLE)lpParam;
	while (1) {
		WaitForSingleObject(hEvent, INFINITE);
		UC_LOG(INFO) << "WaitMeetingReadiedThreadProc notified.";
		ResetEvent(hEvent);
		UCManager::GetInstance().GetCalendarServiceStrategy()->
			NotifyMeetingReadied();
	}

	CloseHandle(hEvent);
	return 0;
}


//等待入会成功
DWORD WINAPI WaitMeetingJoinedThreadProc(__in LPVOID lpParam) {

	HANDLE hEvent = (HANDLE)lpParam;
	while (1) {
		WaitForSingleObject(hEvent, INFINITE);
		UC_LOG(INFO) << "WaitMeetingJoinedThreadProc notified.";
		ResetEvent(hEvent);
		//创建等待会议结束事件
		UCExtension::CreateEvent(GLOBAL_EVENT_SHOW_BEE_MAINWND, WaitToShowMainWndThreadProc);
		UCExtension::CreateEvent(GLOBAL_EVENT_MEETING_CLOSED, WaitMeetingClosedThreadProc);
		UCManager::GetInstance().GetCalendarServiceStrategy()->
			NotifyMeetingJoined();
	}

	CloseHandle(hEvent);
	return 0;
}


//处理入会失败
DWORD WINAPI WaitMeetingJoinFailedThreadProc(__in LPVOID lpParam) {

	HANDLE hEvent = (HANDLE)lpParam;
	while (1) {
		WaitForSingleObject(hEvent, INFINITE);
		UC_LOG(INFO) << "WaitMeetingJoinFailedThreadProc notified.";
		ResetEvent(hEvent);
		UCManager::GetInstance().GetCalendarServiceStrategy()->
			NotifyMeetingJoinFailed();
	}

	CloseHandle(hEvent);
	return 0;
}
bool UCExtension::CreateEvent(const std::wstring& eventName, WaitEventNotify* waitFunc)
{
	UC_LOG(INFO) << "UCExtension::CreateEvent enter. event name = " << eventName;
	if (eventName.empty() || waitFunc == NULL) {
		return false;
	}

	if (NULL != OpenEvent(READ_CONTROL, FALSE, eventName.c_str())) {
		return true;
	}

	//如果没有则创建事件
	HANDLE hEvent = ::CreateEvent(NULL, TRUE, FALSE, eventName.c_str());

	if (hEvent == NULL) {
		UC_LOG(ERROR) << "UCExtension::CreateEvent: \"" << eventName << "\" failed. error code = " << ::GetLastError();
		return false;
	}

	//在子线程中等以避免阻塞主线程 
	HANDLE tdUI = CreateThread(NULL, 0,
		waitFunc,
		hEvent,
		0, NULL);

	if (tdUI == NULL) {
		UC_LOG(ERROR) << "UCExtension::CreateEvent create wait thread failed. error code = " << ::GetLastError();
		return false;
	}

	return true;
}

}