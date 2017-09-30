#include "GkClient.h"
#include "gkclient/GKClientExport.h"
#include "../UCDefs.h"
#include "LogManager.h"
#include "../UCManager.h"
#include "common/StringConvert.h"
#include "UCErrorCode.h"
#include "../utils/FileUtil.h"
#include "../Configuration.h"

#include <vector>
#include <cassert>

namespace ucclient {

const int64_t RESTART_OPERATIONS_TIMER_PERIOD = 1000 * 1;
const int64_t SYNC_MOUNT_LIST_DELAY = 1000 * 2;
const int64_t LOGIN_GKCLIENT_DELAY_SECONDS = 5;

static int64_t loginGkClientDelay = LOGIN_GKCLIENT_DELAY_SECONDS * 1000;

template<class T>
static inline int GetIntValueFromJson(const T &doc, const std::wstring & key, int defaultVaule) {
	if (doc.HasMember(key.c_str())) {
		if (doc[key.c_str()].IsInt()) {
			return doc[key.c_str()].GetInt();
		}
		else if (doc[key.c_str()].IsString()) {
			std::wstring value = doc[key.c_str()].GetString();
			return _wtoi(value.c_str());
		}
	}

	return defaultVaule;
}

template<class T>
static inline int64_t GetInt64ValueFromJson(const T &doc, const std::wstring & key, int64_t defaultVaule) {
	if (doc.HasMember(key.c_str())) {
		if (doc[key.c_str()].IsInt64()) {
			return doc[key.c_str()].GetInt64();
		}
		else if (doc[key.c_str()].IsString()) {
			std::wstring value = doc[key.c_str()].GetString();
			return _wtoi(value.c_str());
		}
	}

	return defaultVaule;
}

template<class T>
static inline std::wstring GetStringValueFromJson(const T &doc, const std::wstring & key, const std::wstring &defaultVaule) {
	return (doc.HasMember(key.c_str()) && doc[key.c_str()].IsString()) ? doc[key.c_str()].GetString() : defaultVaule;
}

static std::wstring SerializeJson(GkClient::WValue& value) {
	GkClient::WStringBuffer buffer;
	GkClient::WWriter writer(buffer);
	value.Accept(writer);

	return buffer.GetString();
}

GkClient::GkClient(cefIntegration::CefClient *cefClient) : _cefClient(cefClient), _gkClientConnection(GKC_NOT_START), _timerId(-1){
	InitializeCriticalSection(&_cs);

	std::wstring modulePath; 
	FileUtil::ProcessPath(modulePath);
	std::wstring configPath = modulePath + L"\\config\\bee.config";
	Configuration xmlConfig(configPath);

	std::string gkclientApi = xmlConfig.QueryStringValue("/configuration/gkclient/api", "");
	std::string gkclientWeb = xmlConfig.QueryStringValue("/configuration/gkclient/web", "");
	loginGkClientDelay = 1000 * xmlConfig.QueryIntValue("/configuration/gkclient/delay", LOGIN_GKCLIENT_DELAY_SECONDS);

	std::wstringstream ss;
	ss << L"{\"api\":\"" << StringConvert::FromUTF8Str(gkclientApi) << L"\","
		<< L"\"web\":\"" << StringConvert::FromUTF8Str(gkclientWeb) << L"\"}";
	GKClient_SetHost(ss.str());
}

GkClient::~GkClient() {
	DeleteCriticalSection(&_cs);
	::GKClient_Exit();
}

void GkClient::GkClientLogin(const std::wstring& token) {
	if ((0 == _token.compare(token)) && IsLogin()) {
		UC_LOG(INFO) << "GK isLogin, token=" << token.c_str();
		return;
	}

	_token = token;

	DWORD threadID;
	HANDLE handle = ::CreateThread(NULL, 0, ThreadProc, (LPVOID)this, 0, &threadID);
	CloseHandle(handle);
}

DWORD WINAPI GkClient::ThreadProc(LPVOID lpParam) {
	GkClient * client = (GkClient*) lpParam;

	std::wstringstream ss;
	UCID ucid = UCManager::GetInstance().GetClientServiceStrategy()->GetCurUCID();
	ss << L"{\"token\":\"" << client->_token << L"\", \"site_id\":" << ucid.GetSiteId()
		<< L", \"userId\": " << ucid.GetUserId() << "}";

	wstring str = GKClient_Login(ss.str());

	UC_LOG(INFO) << "GKClient_Login, res: " << str;

	WDocument document;
	if (document.Parse<0>(str.c_str()).HasParseError()) {
		UC_LOG(ERROR) << "error message format from GKClient_Login";
		UCManager::GetInstance().GetGkClient()->OnGkClientLogin(GKCLIENT_JSON_PARSE_ERROR);
		return 0;
	}

	if (document.HasMember(L"error") && document[L"error"].IsInt()) {
		UCManager::GetInstance().GetGkClient()->OnGkClientLogin(document[L"error"].GetInt());
	}
	else {
		UC_LOG(ERROR) << "no valid errorCode from GKClient_Login";
		UCManager::GetInstance().GetGkClient()->OnGkClientLogin(GKCLIENT_JSON_PARSE_ERROR);
	}

	return 0;
} 

void GkClient::OnGkClientLogin(int errorCode) {
	_gkClientConnection = (errorCode == 0) ? GKC_SUCCESS : GKC_ERROR;
	if (0 == errorCode) {
		_curUserId = UCManager::GetInstance().GetClientServiceStrategy()->GetCurUCID().GetUserId();
	}

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GKCLIENT_LOGIN_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGkClientLogin event to JS, errorCode: " << errorCode;
}

void GkClient::SetProxyAuthInfo(int32_t type, const wstring& account, const wstring& password) {
	WDocument document;

	WValue usernameVal(rapidjson::kStringType);
	usernameVal.SetString(account.c_str(), document.GetAllocator());
	document.SetObject().AddMember(L"username", usernameVal, document.GetAllocator());

	WValue passwordVal(rapidjson::kStringType);
	passwordVal.SetString(password.c_str(), document.GetAllocator());
	document.AddMember(L"password", passwordVal, document.GetAllocator());

	WValue authTypeVal(type);
	document.AddMember(L"auth_type", authTypeVal, document.GetAllocator());

	std::wstring authInfo = SerializeJson(document);

	GKClient_SetProxy(authInfo);
}

void GkClient::GkClientLogin() {
	_gkClientConnection = GKC_CONNECTING;
	UCManager::GetInstance().GetClientServiceStrategy()->GetAccessToken(OGT_PASSWORD);
} 

void GkClient::SetUserMessage(const wstring& content) {
	UC_LOG(INFO) << "receive message from gkclient, content: " << content;

	WDocument document;
	if (document.Parse<0>(content.c_str()).HasParseError()) {
		UC_LOG(ERROR) << "error message format from gkclient";
		return ;
	}

	GkClientMessage message(document);
	ProcessGkMessage(message, content);
}

void GkClient::ProcessGkMessage(GkClientMessage& message, const wstring& content) {
	WDocument &document = message.MessageDoc();

	if (!document.HasMember(L"type") || !document[L"type"].IsString() || !document.HasMember(L"body")) {
		UC_LOG(ERROR) << "no valid type or body from gkclient message";
		return;
	}

	WValue& body = document[L"body"];
	if (!body.HasMember(L"mount_id") || !body[L"mount_id"].IsInt()) {
		UC_LOG(ERROR) << "mount_id is not found in gkclient message body";
		return;
	}

	int mountId = body[L"mount_id"].GetInt();
	GkClientTaskType type = GetTaskType(document);

	EnterCriticalSection(&_cs);

	// if mount is not associated with a group, check if we need process it.
	if (MountIdNotAssociated(mountId)) {
		LeaveCriticalSection(&_cs);

		UC_LOG(WARNING) << "mountId " << mountId << " is not associated with a group, so ignore it. type: " 
			<< type;
		GKClient_UserMessageResult(content, UCCLIENT_SUCCESS);

		return;
	}

	std::shared_ptr<GkClientTask> task;

	task = std::make_shared<GkClientTask>(message, content, mountId, UCID(), GKTS_WAITING_GROUP_INFO);
	AddGkClientTask(task);
	SetUserMessageIfGroupExists(*task);


	NotifyResultToGk(task);

	LeaveCriticalSection(&_cs);
}

void GkClient::SetUserMessageIfGroupExists(GkClientTask &task) {
	GroupInfo info;
	if (UCManager::GetInstance().GetGroupServiceStrategy()->GetGroupInfo(0, info, false, task.mountId) == UC_LOCALDATA_SUCCESSED) {
		task.state = GKTS_GROUPID_RECEIVED;
		UCID ucid;
		ucid.SetUserId(info.GetId());
		ucid.SetSiteId(info.GetSiteId());
		ucid.SetResId(0);
		task.groupUid = ucid;
		SetUserMessageInternal(task);
	}
}

void GkClient::SetUserMessageInternal(GkClientTask &task) {
	std::wstring type = (task.message.MessageDoc())[L"type"].GetString();
	if (type == L"file") {
		ProcessGkFileMessage(task);
	}
	else {
		task.state = GKTS_DONE;
		task.res = UCCLIENT_SUCCESS;
	}
}

void GkClient::ProcessGkFileMessage(GkClientTask &task) {
	task.type = GKTT_FILE;
	WValue& body = (task.message.MessageDoc())[L"body"];
	
	if (!body.HasMember(L"type") || !body[L"type"].IsString()) {
		UC_LOG(ERROR) << "type is not found in gkclient message body";
		task.state = GKTS_DONE;
		task.res = GKCLIENT_JSON_PARSE_ERROR;
		return;
	}

	std::wstring type = body[L"type"].GetString();
	CloudFileOperation opt = GetCloudFileOperation(type);
	if (CFO_NONE == opt) {
		UC_LOG(ERROR) << "type is error in gkclient message body";
		task.state = GKTS_DONE;
		task.res = GKCLIENT_JSON_PARSE_ERROR;
		return;
	}

	std::wstring filename = GetStringValueFromJson(body, L"filename", L"");
	std::wstring filenameTo = GetStringValueFromJson(body, L"filename_to", L"");
	int isDir = GetIntValueFromJson(body, L"dir", 0);
	int64_t filesize = GetInt64ValueFromJson(body, L"filesize", 0);
	std::wstring hash = GetStringValueFromJson(body, L"hash", L"");
	std::wstring detailContent = SerializeJson(body);

	int32_t msgId = 0, res;

	res = UCManager::GetInstance().GetGroupServiceStrategy()->SendCloudFileMessage(
		CLOUD_CHAT, opt, task.groupUid, filename, filenameTo, filesize, 1, isDir, CT_GoKuai, detailContent, false, msgId);
	
	if (res != UCCLIENT_SUCCESS) {
		UC_LOG(ERROR) << "ProcessGkFileMessage error!";
	}
	
	task.res = UCCLIENT_SUCCESS;
	task.state = GKTS_DONE;
}

void GkClient::OnLogin(int32_t errorCode, int32_t authType, int32_t cloudDiskType, const std::string &capability) {
	UC_LOG(INFO) << "OnLogin errorCode=" << errorCode << ", ClientConnection=" << _gkClientConnection;
	if (!IsLogin()) {
		CancelLogin();

		if (CanLoginEx(errorCode)) {
			Timer::GetInstance().Schedule(MakeCallback(this, &GkClient::GkClientLogin), loginGkClientDelay, &_timerId);
		}
	}
}

void GkClient::OnAccessTokenReceived(int32_t errorCode, OauthGrantType grantType, const std::string &accessToken) {

	int moduleFlag = UCManager::GetInstance().GetClientServiceStrategy()->GetModuleFlag();
	if (0 == (moduleFlag & 0x0001)) {
		UC_LOG(INFO) << "Cloud disk Not open !";
		return;
	}

	if (errorCode == 0 && grantType == OGT_PASSWORD) {
		GkClientLogin(StringConvert::FromUTF8Str(accessToken));
		return;
	}

	if (errorCode != 0) {
		UC_LOG(ERROR) << "get access token error, errorCode: " << errorCode << "grantType: " << grantType;
		if (OGT_PASSWORD == grantType) {
			OnGkClientLogin(errorCode);
		}
	}
}

void GkClient::OnLogout(int32_t errorCode) {
	CancelLogin();

	::GKClient_Logout();
}

void GkClient::OnGroupInfoReceived(int32_t result, int64_t groupId, int32_t mountId, const GroupInfo& info) {
	EnterCriticalSection(&_cs);

	if (result == CLOUD_DISCUSSION_NOT_EXISTS && mountId != 0) {
		AddNotAssociatedMountId(mountId);
	}

	for (std::list<std::shared_ptr<GkClientTask>>::iterator iter = _gkClientTasks.begin(); iter != _gkClientTasks.end();) {
		if ((*iter)->mountId == mountId && GKTS_WAITING_GROUP_INFO == (*iter)->state) {

			if (result == 0) {
				(*iter)->state = GKTS_GROUPID_RECEIVED;

				UCID ucid;
				ucid.SetUserId(info.GetId());
				ucid.SetSiteId(info.GetSiteId());
				(*iter)->groupUid = ucid;

				SetUserMessageInternal(**iter);
			}
			else {
				(*iter)->state = GKTS_DONE;
				(*iter)->res = result;
			}
			
			if (GKTS_DONE == (*iter)->state) {
				NotifyUserMessageResult(**iter);
				iter = _gkClientTasks.erase(iter);
				continue;
			}
		}

		++iter;
	}
	LeaveCriticalSection(&_cs);
}

void GkClient::OnGroupCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const GroupInfo& group) {
	EnterCriticalSection(&_cs); 
	 
	if (group.GetMountId() != 0) {
		RemoveNotAssociatedMountId(group.GetMountId());
	}

	LeaveCriticalSection(&_cs);
}

void GkClient::OnNetWorkChanged(ConnectionState state) {
	// inform gkclient of the network change
	GKClient_ConnectionStatus(state);
}

void GkClient::OnGroupCreated(int32_t result, int32_t operatorId, const GroupInfo& info) {
	EnterCriticalSection(&_cs);
	
	if (result == 0 && info.GetMountId() != 0) {
		RemoveNotAssociatedMountId(info.GetMountId());
	}
	LeaveCriticalSection(&_cs);
}


void GkClient::NotifyResultToGk(std::shared_ptr<GkClientTask> task) {
	if (task->state == GKTS_DONE) {
		NotifyUserMessageResult(*task);
		RemoveGkClientTask(task);
	}
}

void GkClient::NotifyUserMessageResult(GkClientTask& task) {
	if (!task.jsonMsg.empty()) {
		GKClient_UserMessageResult(task.jsonMsg, task.res);
	}
}

void GkClient::AddGkClientTask(std::shared_ptr<GkClientTask> task) {
	_gkClientTasks.push_back(task);
}

void GkClient::RemoveGkClientTask(std::shared_ptr<GkClientTask> task) {
	for (std::list<std::shared_ptr<GkClientTask>>::iterator iter = _gkClientTasks.begin(); iter != _gkClientTasks.end(); ++iter) {
		if (task->jsonMsg == (*iter)->jsonMsg) {
			_gkClientTasks.erase(iter);
			break;
		}
	}
}

bool GkClient::MountIdNotAssociated(int32_t mountId) {
	std::set<int32_t>::iterator iter = _nonAssociatedMountIds.find(mountId);
	return iter != _nonAssociatedMountIds.end() ? true : false;
}

void GkClient::AddNotAssociatedMountId(int32_t mountId) {
	_nonAssociatedMountIds.insert(mountId);
}

void GkClient::RemoveNotAssociatedMountId(int32_t mountId) {
	_nonAssociatedMountIds.erase(mountId);
}

CloudFileOperation GkClient::GetCloudFileOperation(const std::wstring &type) {
	if (type == L"add") {
		return CFO_CloudFileCreate;
	}
	else if (type == L"update") {
		return CFO_CloudFileUpdate;
	}
	else if (type == L"move") {
		return CFO_CloudFileMove;
	}
	else if (type == L"rename") {
		return CFO_CloudFileRename;
	}
	else if (type == L"delete") {
		return CFO_CloudFileDel;
	}
	else {
		return CFO_NONE;
	}
}

GkClientTaskType GkClient::GetTaskType(WDocument & doc) {
	std::wstring type = GetStringValueFromJson(doc, L"type", L"");
	std::wstring subType = GetStringValueFromJson(doc[L"body"], L"type", L"");
	if (type == L"mount") {
		if (subType == L"delete") {
			return GKTT_END_GROUP;
		}
		else if (subType == L"update_name") {
			return GKTT_UPDATE_NAME;
		}
		else if (subType == L"update_url") {
			return GKTT_UPDATE_URL;
		}
	}
	else if (type == L"member"){
		if (subType == L"add" && GetIntValueFromJson(doc[L"body"], L"count", 0) <= 1) {
			return GKTT_CREATE_GROUP;
		}
		else if (subType == L"add") {
			return GKTT_ADD_MEMBER;
		}
		else if (subType == L"delete") {
			return GKTT_KICK_MEMBER;
		}
	}
	else if (type == L"file"){
		return GKTT_FILE;
	}

	return GKTT_NONE;
}

bool GkClient::IsLogin() {
	UC_LOG(INFO) << "ClientConnection=" << _gkClientConnection << ", curUserId=" << _curUserId
		<< ", GetCurUCID=" << UCManager::GetInstance().GetClientServiceStrategy()->GetCurUCID().GetUserId();
	cout << "ClientConnection=" << _gkClientConnection << ", curUserId=" << _curUserId
		<< ", GetCurUCID=" << UCManager::GetInstance().GetClientServiceStrategy()->GetCurUCID().GetUserId() << endl;
	return  (_gkClientConnection == GKC_SUCCESS) && 
		(_curUserId == UCManager::GetInstance().GetClientServiceStrategy()->GetCurUCID().GetUserId());
}

bool GkClient::CanLogin(int32_t loginCode, int32_t cloudDiskType) {
	return loginCode == 0 && cloudDiskType != 0;
}

bool GkClient::CanLoginEx(int32_t loginCode) {
	if (0 != loginCode){
		return false;
	}
	
	int moduleFlag = UCManager::GetInstance().GetClientServiceStrategy()->GetModuleFlag();
	int mask = (0x0001 | 0x0002);
	bool bRet = (0 != (moduleFlag & mask));
	UC_LOG(INFO) << "moduleFlag=" << moduleFlag << ", ret=%s" << (bRet ? "T" : "F") 
		<< ", loginGkClientDelay" << loginGkClientDelay;

	return bRet;
}

void GkClient::CancelLogin() {
	_gkClientConnection = GKC_NOT_START;

	if (_timerId > 0) {
		Timer::GetInstance().Cancel(_timerId);
		_timerId = -1;
	}
}

}