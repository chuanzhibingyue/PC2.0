/*
 * ExternalService.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: yongshuai.cheng
 */

#include "ExternalService.h"
#include "UCEngine.h"
#include "UCErrorCode.h"
#include "client_service/ClientService.h"
#include "DBManager.h"
#include "ContactInfo.h"
#include "contact_service/ContactService.h"
#include "message_service/GroupService.h"

#if defined(WIN32) || defined(WIN64)
    #include <io.h>
#elif defined(__APPLE__)
    #include "sys/uio.h"
#else
    #include "sys/io.h"
#endif

#ifdef PLATFORM_WIN
#define EXTERNAL_REGISTER_PATH  (L"Software\\Bee")
#define EXTERNAL_REGISTER_KEY   (L"ExternalDir")
#endif

namespace uc {

ExternalService::ExternalService(IUCEngine *engine) {
	// TODO Auto-generated constructor stub
	_bInitDB = false;
	_engine = engine;
	_engine->registerListener(this);
}

ExternalService::~ExternalService() {
	// TODO Auto-generated destructor stub
}

int32_t ExternalService::GetChatHistory(ConversationType type, const string& externalId, int32_t count, int64_t startTime, int64_t endTime){
	bool bDbInit = InitExternalDB();

	list<ExternalMessage*> msgList;

	if (!bDbInit)
	{
		triggerEvent(IExternalServiceEvent, OnChatHistoryReceived(UC_FAILED, type, 0, 0, 0, msgList));

		UC_LOG(ERROR) << "external_service OnChatHistoryReceived from cache.result: " << UC_FAILED
			<< " relateId : " << 0 << " message_count : " << msgList.size() << " min_timestamp: " << 0;

		return UC_SUCCEEDED;
	}
	
	int64_t relateId = GetRelateId(type, externalId);
	int64_t miniTimestamp = 0;
	int32_t result = DBManager::GetInstance().GetExternalMessage(type, externalId, startTime, endTime, count, msgList);
	if (result == DBError::DB_SUCCESS){
		miniTimestamp = [](list<ExternalMessage*> & list)->int64_t{
			std::list<ExternalMessage*>::iterator iter = list.begin();
			int64_t ret = (*iter)->GetMsgSeq();
			//int64_t ret = (&list.begin())->GetMsgSeq();
			for (auto &info : list)
			{
				if (info->GetMsgSeq() < ret)
					ret = info->GetMsgSeq();
			}
			return ret;
		}(msgList);

		if (SINGLE_CHAT == type){
			ContactService* contactService = (ContactService*)_engine->QueryService(UCContactService);
			for (auto &item : msgList){
				BasicContactInfo fromInfo;
				if (contactService && contactService->GetBasicContactInfo(item->GetFrom().GetUserAccount(), fromInfo)){
					ExternalUser from;
					from.SetUserId(fromInfo.GetUserId());
					from.SetUserAccount(fromInfo.GetAccount());
					from.SetDisplayName(fromInfo.GetDisplayName());
					from.SetLocalAvatar(fromInfo.GetLocalAvatar());
					if (item->GetFrom().GetUserAccount().compare(GetCurUserAccount()) == 0){
						from.SetIsSelf(true);
					}
					item->SetFrom(from);
				}
				BasicContactInfo toInfo;
				if (contactService && contactService->GetBasicContactInfo(item->GetTo().GetUserAccount(), toInfo)){
					ExternalUser to;
					to.SetUserId(toInfo.GetUserId());
					to.SetUserAccount(toInfo.GetAccount());
					to.SetDisplayName(toInfo.GetDisplayName());
					to.SetLocalAvatar(toInfo.GetLocalAvatar());
					item->SetTo(to);
				}
			}
		}
		else if (GROUP_CHAT == type){
			ContactService* contactService = (ContactService*)_engine->QueryService(UCContactService);
			GroupService* groupService = (GroupService*)_engine->QueryService(UCGroupService);
			for (auto &item : msgList){
				BasicContactInfo fromInfo;
				if (contactService && contactService->GetBasicContactInfo(item->GetFrom().GetUserAccount(), fromInfo)){
					ExternalUser from;
					from.SetUserId(fromInfo.GetUserId());
					from.SetUserAccount(fromInfo.GetAccount());
					from.SetDisplayName(fromInfo.GetDisplayName());
					from.SetLocalAvatar(fromInfo.GetLocalAvatar());
					if (item->GetFrom().GetUserAccount().compare(GetCurUserAccount()) == 0){
						from.SetIsSelf(true);
					}
					item->SetFrom(from);
				}
				GroupInfo toInfo;
				if (groupService && groupService->GetGroupInfoByExternalId(item->GetTo().GetUserAccount(), toInfo)){
					ExternalUser to;
					to.SetUserId(toInfo.GetId());
					to.SetUserAccount(toInfo.GetExternalId());
					to.SetDisplayName(toInfo.GetName());
					to.SetLocalAvatar(toInfo.GetLocalAvatar());
					item->SetTo(to);
				}
			}
		}
	}
	
	// 如果查询结果为空，则仍然认为是查询成功，msgList.size() = 0
	if (result == DBError::DB_QUERY_EMPTY)
		result = DBError::DB_SUCCESS;

	triggerEvent(IExternalServiceEvent, OnChatHistoryReceived(result, type, relateId, msgList.size(), miniTimestamp, msgList));

	UC_LOG(INFO) << "external_service OnChatHistoryReceived from cache.result: " << result
		<< " relateId : " << relateId << " message_count : " << msgList.size() << " min_timestamp: " << miniTimestamp;

	return UC_SUCCEEDED;
}

int32_t ExternalService::ShowExternalMessage(const string& externalId){

	string dbPath = GetExternalDirectory() + "/" + GetExternalDBFile();
    
#if defined(WIN32) || defined(WIN64)
	if (_access(dbPath.c_str(), 0) == -1){
		UC_LOG(ERROR) << "exteral message db path isn't exist, path = " << dbPath;
		return STATUS_EXTERNAL_HIDE;
	}
#endif
    
	bool bDbInit = InitExternalDB();
	if (!bDbInit)
		return STATUS_EXTERNAL_HIDE;

	if (DBManager::GetInstance().HaveData(externalId) != DBError::DB_SUCCESS)
		return STATUS_EXTERNAL_HIDE;


	return STATUS_EXTERNAL_SHOW;
}


void ExternalService::OnUCASConnected(int32_t errorCode){
	if (errorCode == UC_SUCCEEDED){
		// Reset DB if reconnect ucas
		_bInitDB = false;
	}
}

bool ExternalService::InitExternalDB(){
	if (!_bInitDB){
		string dbPath = GetExternalDirectory() + "/" + GetExternalDBFile();
		if (DBManager::GetInstance().InitExternalDB(dbPath) == DBError::DB_SUCCESS){
			_bInitDB = true;
		}
	}

	return _bInitDB;
}

string ExternalService::GetLoginAccount(){

	string account = "";
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService){
		account = clientService->GetLoginInfo().GetAccount();
	}
	return account;
}

string ExternalService::GetExternalDirectory(){

	string dir = "";
#ifdef PLATFORM_WIN
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = MAX_PATH;
	TCHAR data[MAX_PATH] = {0};

	if (RegOpenKeyEx(HKEY_CURRENT_USER, EXTERNAL_REGISTER_PATH, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS){
		if (RegQueryValueEx(hKey, EXTERNAL_REGISTER_KEY, NULL, &dwType, (LPBYTE)data, &dwSize) == ERROR_SUCCESS){
			dir = StringConvert::ToUTF8Str(data) + "/" + GetLoginAccount();
		}
	}
	RegCloseKey(hKey);
	
#endif
	return dir;
}

string ExternalService::GetExternalDBFile(){
	return GetLoginAccount() + ".sqlite";
}


std::string ExternalService::GetCurUserAccount(){
	std::string account = "";
	if (_engine){
		account = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetAccount();
	}
	return 	account;
}


int64_t ExternalService::GetRelateId(ConversationType type, const string& externalId){
	if (SINGLE_CHAT == type){
		BasicContactInfo info;
		ContactService* contactService = (ContactService*)_engine->QueryService(UCContactService);
		if (contactService && contactService->GetBasicContactInfo(externalId, info)){
			return (int64_t)info.GetUserId();
		}
	}
	else if (GROUP_CHAT == type){
		GroupInfo info;
		GroupService* groupService = (GroupService*)_engine->QueryService(UCGroupService);
		if (groupService && groupService->GetGroupInfoByExternalId(externalId, info)){
			return info.GetId();
		}
	}

	return 0;
}

bool ExternalService::ClearMessageList(list<ExternalMessage*>& msglist)
{
	for (list<ExternalMessage*>::iterator it = msglist.begin(); it != msglist.end(); ++it)
	{
		delete (*it);
		(*it) = NULL;
	}
	msglist.clear();

	return true;
}

} /* namespace uc */
