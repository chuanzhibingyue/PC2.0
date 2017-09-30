/*
 * ContactService.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#include "ContactService.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include <sstream>

#include "UCEngine.h"
#include "common/StringConvert.h"
#include "utils/Macro.h"
#include "client_service/ClientService.h"
#include "components/db/include/DBManager.h"
#include "boost/property_tree/json_parser.hpp" 
#include "LogManager.h"
#include "components/network/NetworkManager.h"
#include "UCErrorCode.h"
#include "CommonDef.h"
#include "contact_cache/RegularUpdatedCache.h"
#include "utils/UCTime.h"
#include "utils/TaskExecutor.h"
#define CONTACT_CONFIG_APPLICATION "ContactService"
#define LAST_CONTACT_SYNC_TIME "last_contact_sync_time"

namespace uc {

ContactService::ContactService(IUCEngine *engine) : _lastContactSyncTime(-1),
_updateStatus(CUS_NOT_START){
    UC_LOG(INFO) << "Constructing ContactService";

	_engine = engine;
	_requestFactory = new ContactRequestFactory(this);
	_dispatcher = new ContactEventDispatcher(this);
    _processor = new ContactMsgProcessor(this, _dispatcher);

    _contactCache = new RegularUpdatedCache(this);
	_contactCache->SetListener(this);

	_contactDelayLoader = new ContactDelayLoader(this);

	((UCEngine*)_engine)->registerListener(this);
}

ContactService::~ContactService() {
	UC_LOG(INFO) << "Destructing ContactService";

    UnInitialize();

    SAFE_DELETE(_contactCache);
	SAFE_DELETE(_contactDelayLoader);
	SAFE_DELETE(_requestFactory);
    SAFE_DELETE(_processor);
    SAFE_DELETE(_dispatcher);
}

void ContactService::Initialize() {
	UC_LOG(INFO) << "Initializing ContactService";

	// 先清除cache
	UnInitialize();

    _contactCache->Initialize();
    
    // 更新常用联系人信息
    GetUpdatedContacts();
}

void ContactService::UnInitialize() {
	UC_LOG(INFO) << "UnInitialize ContactService";
    
    _contactCache->UnInitialize();

    _lastContactSyncTime = 0;
    _updateStatus = CUS_NOT_START;
}

bool ContactService::GetContactInfo(int32_t userId, ContactInfo& contact) {
	UC_LOG(INFO) << "Get Contact from ContactService, userId: " << userId;

    ContactInfoImpl* impl = _contactCache->GetContact(userId);
    if (impl != NULL) {

		//如果用户标签信息为空，需重新向服务器请求
		if (impl->GetUserTags() == ""){
			GetContactInfoFromServer(userId, DISPATCH_CONTACT_INFO_RECEIVED);
			return false;
		}
		contact.Reset(impl);
        return true;
    }
    else {
		GetContactInfoFromServer(userId, DISPATCH_CONTACT_INFO_RECEIVED);
    }
    return false;
}

void ContactService::GetContactInfo(const std::list<int32_t>& userIds) {
    UC_LOG(INFO) << "Get Contact in bulk from ContactService";
    if (userIds.empty()) {
        _dispatcher->ContactInfoReceived(UC_FAILED, NULL, NULL);
        return;
    }

	std::list<ContactInfoImpl*> contacts;
	std::list<int32_t> notFoundList;
	std::list<int32_t> *foundList = new std::list<int32_t>();
	_contactCache->GetContact(userIds, contacts, *foundList, notFoundList);

	if (!contacts.empty()) {
		std::list<ContactInfo*> *infoList = new std::list<ContactInfo*>();
		ConvertContactInfo(*infoList, contacts);
		_dispatcher->ContactInfoReceived(UC_SUCCEEDED, foundList, infoList);
	}
	else {
		delete foundList;
	}

	SAFE_CLEAR_CONTAINER(std::list<ContactInfoImpl*>, contacts);

	if (!notFoundList.empty()) {
		_contactDelayLoader->GetContact(notFoundList, DISPATCH_CONTACT_INFO_RECEIVED);
	}
}

int32_t ContactService::SaveContactIntoCache(ContactInfoImpl& contact) {
	UC_LOG(INFO) << "Save Contact into cache, userId: " << contact.GetUserId();

    contact.SetLastUpdateTime(Time::GetSysTime());
    return _contactCache->SaveContact(contact);
}

int32_t ContactService::SaveContactIntoCache(std::list<ContactInfoImpl*>& contact) {
	UC_LOG(INFO) << "Save Contact into cache";

    int64_t curTime = Time::GetSysTime();
    for (std::list<ContactInfoImpl*>::iterator iter = contact.begin();
        iter != contact.end(); ++iter) {
        (*iter)->SetLastUpdateTime(curTime);
    }

    return _contactCache->SaveContact(contact);
}

int32_t ContactService::SaveContact(const boost::property_tree::wptree &ptParse) {
    UC_LOG(INFO) << "Save Contact by wptree";

    int32_t res = UC_FAILED;

    ContactInfoImpl* contact = ContactInfoImpl::CreateContactInfoImpl(ptParse,false);
    if (contact != NULL) {
		// 从db中获取本地url路径
		_contactCache->GetLocalAvatar(*contact);
		SetContactType(*contact, false);
        res = SaveContactIntoCache(*contact);
    }

    delete contact;
	return res;
}


bool ContactService::GetBasicContactInfo(int userId, BasicContactInfo& contact) {
    UC_LOG(INFO) << "Get basic information for Contact, userId: " << userId;

    BasicContactInfo info = _contactCache->GetBasicContact(userId);
    if (info.GetUserId() != 0) {
        contact = info;
        return true;
    }
    else {
        _contactDelayLoader->GetContact(userId, DISPATCH_BASIC_CONTACT_INFO_RECEIVED);
    }

    return false;
}

bool ContactService::GetBasicContactInfo(const string& account, BasicContactInfo& contact){

	UC_LOG(INFO) << "Get basic information for Contact, account: " << account;

	BasicContactInfo info = _contactCache->GetBasicContact(account);
	if (info.GetUserId() != 0) {
		contact = info;
		return true;
	}

	return false;
}

void ContactService::GetBasicContactInfo(const std::list<int32_t>& userIds) {
    UC_LOG(INFO) << "Get basic contact info in bulk from ContactService";

    if (userIds.empty()) {
        _dispatcher->BasicContactInfoReceived(UC_FAILED, NULL, NULL);
        return;
    }

	std::list<int32_t> notFoundList;
	std::list<int32_t> *foundList = new std::list<int32_t>();
	std::list<BasicContactInfo> *contactList = new std::list<BasicContactInfo>();

	_contactCache->GetBasicContact(userIds, *contactList, *foundList, notFoundList);
	if (!contactList->empty()) {
		_dispatcher->BasicContactInfoReceived(UC_SUCCEEDED, foundList, contactList);
	}
	else {
		delete foundList;
		delete contactList;
	}

	if (!notFoundList.empty()) {
		_contactDelayLoader->GetContact(notFoundList, DISPATCH_BASIC_CONTACT_INFO_RECEIVED);
	}
}

int32_t ContactService::GetContactList(unsigned int position, unsigned int len, 
	std::vector<BasicContactInfo>& contacts) {
	return _contactCache->GetBasicContactList(position, len, contacts);
}

int32_t ContactService::GetContactInfoFromServer(const int32_t& userId, CONTACT_INFO_RECEIVED_EVENT dispatchWhich)
{
	UC_LOG(INFO) << "Get Single Contact info from server, dispatchWhich: " << dispatchWhich;

	if (userId == 0)
	{
		UC_LOG(INFO) << "Get Contacts info from server, userIds is null, dispatchWhich: " << dispatchWhich;
		return UC_FAILED;
	}
	HTTPRequest	request;
	_requestFactory->CreateGetProfileRequest(userId, request);

	std::list<int32_t>* ids = new std::list<int32_t>();
	ids->push_back(userId);
	_engine->SendAsyncRequest(request, MakeRequestCallback(
		this, &ContactService::ContactInfoReceived, ids, dispatchWhich, true));

	return UC_SUCCEEDED;
}

int32_t ContactService::GetContactsFromServer(const std::list<int32_t>& userIds, CONTACT_INFO_RECEIVED_EVENT dispatchWhich) {
	UC_LOG(INFO) << "Get Contacts info from server, dispatchWhich: " << dispatchWhich;
	if (userIds.empty())
	{
		UC_LOG(INFO) << "Get Contacts info from server, userIds is null, dispatchWhich: " << dispatchWhich;
		return UC_FAILED;
	}
	HTTPRequest	request;
	_requestFactory->CreateGetProfileRequest(userIds, request);

	std::list<int32_t>* ids = new std::list<int32_t>();
	std::copy(userIds.begin(), userIds.end(), std::back_inserter(*ids));
	_engine->SendAsyncRequest(request, MakeRequestCallback(
		this, &ContactService::ContactInfoReceived, ids, dispatchWhich,false));
	
	return UC_SUCCEEDED;
}

void ContactService::ConvertContactInfo(std::list<ContactInfo*> &infoList, 
    std::list<ContactInfoImpl*>& implList) {
    for (std::list<ContactInfoImpl*>::iterator iter = implList.begin();
        iter != implList.end(); ++iter) {
        infoList.push_back(new ContactInfo(*iter));
    }
    implList.clear();
}

int32_t ContactService::ContactInfoReceived(int32_t errorCode, HTTPResponse *response, 
	std::list<int32_t>* userIds, CONTACT_INFO_RECEIVED_EVENT dispatchWhich, bool isSave) {

    UC_LOG(WARNING) << "Get Contacts info response, dispatchWhich: " << dispatchWhich
        << ", errorCode: " << errorCode
        << ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
        << ", body: " << (response != NULL ? response->GetContent() : "");

    std::list<ContactInfoImpl*> implList;

    int32_t res = UC_FAILED;
    bool deleteIds = true;
    if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {
        res = _responseParser.ParseGetProfile(response->GetContent(), implList,isSave);
        
        if (res == UC_SUCCEEDED) {
            SetContactsType(implList, false); 
			((RegularUpdatedCache*)_contactCache)->GetLocalAvatarList<std::list<ContactInfoImpl*> >(implList);
            SaveContactIntoCache(implList);
        }
    }
    else {
        res = UCC_STATUS_CODE_ERROR;
    }

    if (dispatchWhich ==DISPATCH_CONTACT_INFO_RECEIVED) {
        std::list<ContactInfo*> *infoList = new std::list<ContactInfo*>();
        ConvertContactInfo(*infoList, implList);
        _dispatcher->ContactInfoReceived(res, userIds, infoList);
        deleteIds = false;
    }
    else if (dispatchWhich == DISPATCH_CONTACT_UPDATED && res == UC_SUCCEEDED) {
        std::list<ContactInfo*> *infoList = new std::list<ContactInfo*>();
        ConvertContactInfo(*infoList, implList);
		if (infoList->size() > 0) {
			 _dispatcher->ContactUpdated(infoList);
		}
    }
    else if (dispatchWhich == DISPATCH_CONTACT_ADDED) {
        if (!userIds->empty()) {
            _dispatcher->ContactAdded(res, *(userIds->begin()));
        }
    }
    else if (dispatchWhich == DISPATCH_BASIC_CONTACT_INFO_RECEIVED) {
		std::list<BasicContactInfo> *basicList = NULL;
		if (res == UC_SUCCEEDED) {
			basicList = new std::list<BasicContactInfo>();
			for (std::list<ContactInfoImpl*>::iterator iter = implList.begin();
				iter != implList.end(); ++iter) {
				basicList->push_back(BasicContactInfo(*iter));
			}
		}
        
        _dispatcher->BasicContactInfoReceived(res, userIds, basicList);
        deleteIds = false;
    }

    SAFE_CLEAR_CONTAINER(std::list<ContactInfoImpl*>, implList);
    
    if (deleteIds) {
        SAFE_DELETE(userIds);
    }

	return 0;
}

void ContactService::AddCommonContact(int32_t userId) {
    UC_LOG(INFO) << "add common contact , userid: " << userId;
	if (_contactCache->IsCommonContact(userId)) {
		UC_LOG(WARNING) << "user " << userId << " has become already your common contact ";
        _dispatcher->ContactAdded(UC_SUCCEEDED, userId);
        return;
	}

	HTTPRequest	request;
	_requestFactory->CreateContactAddRequest(userId, request);

    _engine->SendAsyncRequest(request, MakeRequestCallback(
        this, &ContactService::ContactAdded, userId));
}

int32_t ContactService::ContactAdded(int32_t errorCode, HTTPResponse *response, int32_t userId) {
    UC_LOG(WARNING) << "add contact response, userid: " << userId
        << ", errorCode: " << errorCode
        << ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
        << ", body: " << (response != NULL ? response->GetContent() : "");

    int32_t res = UC_FAILED;
    ContactInfoImpl * contactPtr = NULL;
    if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {
        res = _responseParser.ParseContactAdd(response->GetContent(), &contactPtr);
    }
    else {
        res = UCC_STATUS_CODE_ERROR;
    }

    if (res == UC_SUCCEEDED) {
        contactPtr->SetCommon();
		_contactCache->GetLocalAvatar(*contactPtr);
        SaveContactIntoCache(*contactPtr);
    }

	SAFE_DELETE(contactPtr);

    _dispatcher->ContactAdded(res, userId);
	return res;
}

void ContactService::DeleteCommonContact(int32_t userId) {
    UC_LOG(INFO) << "delete common contact , userid: " << userId;

	UC_LOG(INFO) << "add common contact , userid: " << userId;
	if (!_contactCache->IsCommonContact(userId)) {
		UC_LOG(WARNING) << "your common contact list doesn't include user "
            << userId;
        _dispatcher->ContactDeleted(UC_SUCCEEDED, userId);
        return;
	}

	HTTPRequest	request;
	_requestFactory->CreateContactDeleteRequest(userId, request);

    _engine->SendAsyncRequest(request, MakeRequestCallback(
        this, &ContactService::ContactDeleted, userId));
}

int32_t ContactService::ContactDeleted(int32_t errorCode, HTTPResponse *response, int32_t userId) {
    UC_LOG(WARNING) << "delete contact response, errorCode: " << errorCode
        << ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
        << ", body: " << (response != NULL ? response->GetContent() : "");

    int32_t res = UC_FAILED;
    if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {
        res = _responseParser.ParseContactDelete(response->GetContent());
    }
    else {
        res = UCC_STATUS_CODE_ERROR;
    }

    if (res == UC_SUCCEEDED) {
        _contactCache->RemoveCommonContact(userId);
    }

    _dispatcher->ContactDeleted(res, userId);
	return res;
}

int32_t ContactService::SearchLocalContact(ContactInfo::Type type, const std::string& keyword,
		std::list<ContactInfo>& contacts) {
	// 数据库中搜索

	std::list<ContactInfoImpl*> implList;
    int32_t res = DBManager::GetInstance().GetContactListByName(keyword, type, implList);
    if (res != UC_SUCCEEDED) {
        UC_LOG(ERROR) << "fail to search contact from database, type: " << type
            << ", keyword: " << keyword << ", result: " << res;
        res = UC_FAILED;
    }
    else {
        UC_LOG(WARNING) << "search contact from database, type: " << type
            << ", keyword: " << keyword << ", result: " << res;

        for (std::list<ContactInfoImpl*>::iterator iter = implList.begin();
            iter != implList.end(); ++iter) {
            contacts.push_back(ContactInfo(*iter));
        }

        implList.clear();
    }

	return res;
}

void ContactService::UpdateUserTags(const std::string& updateTags) {
   
	HTTPRequest	request;
	_requestFactory->CreateTagUpdateRequest(updateTags, request);

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &ContactService::UserTagsUpdated, updateTags));

}
    
int32_t ContactService::UserTagsUpdated(int32_t errorCode, HTTPResponse *response, 
	std::string updateTags) {
	UC_LOG(INFO) << "update user tags response, errorCode: " << errorCode
        << ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
        << ", body: " << (response != NULL ? response->GetContent() : "");
        
    int32_t res = UC_FAILED;
    if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {
        res = _responseParser.ParseUpdateTags(response->GetContent());
    }
    else {
        res = UCC_STATUS_CODE_ERROR;
    }
 
	_dispatcher->UserTagsUpdated(res, updateTags);
	return res;
}

void ContactService::GetColleagues(int userId) {
	int page = 1;
	GetColleaguesFromServer(userId, page);
}

void ContactService::GetColleaguesFromServer(int32_t userId, int page) {
    UC_LOG(INFO) << "get colleagues from server, userId: " << userId
        << ", page: " << page;

    HTTPRequest request;

    _requestFactory->CreateUserColleagueRequest(userId, page, USER_COLLEAGUE_PAGESIZE, request);
    _engine->SendAsyncRequest(request, MakeRequestCallback(
        this, &ContactService::ColleagueReceived, userId, page));
}

int32_t ContactService::ColleagueReceived(int32_t errorCode, HTTPResponse *response, 
    int32_t userId, int page) {
    UC_LOG(WARNING) << "get colleagues response, page: " << page
        <<", errorCode: " << errorCode 
        << ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
        << ", body: " << (response != NULL ? response->GetContent() : "");

    int32_t res = UC_FAILED;
    std::list<ContactInfoImpl*> colleagues;
    std::list<ContactInfoImpl*> leaders ;
    int leaderNum = 0, colleagueNum = 0, count = 0;

    if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {    
        res = _responseParser.ParseUserColleague(response->GetContent(), &leaderNum,&colleagueNum, colleagues, leaders);

        if (res == UC_SUCCEEDED){
            count = (int)colleagues.size();
			((RegularUpdatedCache*)_contactCache)->GetLocalAvatarList<std::list<ContactInfoImpl*> >(colleagues);
			((RegularUpdatedCache*)_contactCache)->GetLocalAvatarList<std::list<ContactInfoImpl*> >(leaders);
        }
    }
    else {
        res = UCC_STATUS_CODE_ERROR;
    }
        
    if (res != UC_SUCCEEDED) {
        SAFE_CLEAR_CONTAINER(std::list<ContactInfoImpl*>, colleagues);
        SAFE_CLEAR_CONTAINER(std::list<ContactInfoImpl*>, leaders);
    }

	std::list<ContactInfo*> *colleagueList = new std::list<ContactInfo*>();
    ConvertContactInfo(*colleagueList, colleagues);

	std::list<ContactInfo*> *leaderList = new std::list<ContactInfo*>();
    ConvertContactInfo(*leaderList, leaders);

	if (page > 1 && count == 0) {
		return res;
	}

    _dispatcher->ColleagueReceived(res, userId, page, colleagueNum, leaderNum, colleagueList, leaderList);

    if (count >= USER_COLLEAGUE_PAGESIZE) {
        GetColleaguesFromServer(userId, ++page);
    }

	return res;
}

void ContactService::GetOrgAddressBook(int orgId, int32_t& taskId) {
    UC_LOG(INFO) << "get organization address list, orgId: " << orgId;
	taskId = MakeRequestId();
	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
    if (clientService != NULL) {
		HTTPRequest	request;
		_requestFactory->CreateOrgListRequest(orgId, clientService->GetLoginInfo().GetCustomerCode(), request);

		_engine->SendAsyncRequest(request, MakeRequestCallback(
			this, &ContactService::OrgAddrListReceived, taskId));
	}
}

int32_t ContactService::OrgAddrListReceived(int32_t errorCode, HTTPResponse *response, int32_t taskId) {
    UC_LOG(WARNING) << "get organization address list response, errorCode: " 
        << errorCode << ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
        << ", body: " << (response != NULL ? response->GetContent() : "");

    int32_t res = UC_FAILED;
    DepartmentInfo* depart = NULL;
    DepartmentInfo* myDepart = NULL;

    if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {
        depart = new DepartmentInfo();
        myDepart = new DepartmentInfo();
        
        res = _responseParser.ParseOrgList(response->GetContent(), *depart, *myDepart);
        if (res != UC_SUCCEEDED) {
            delete depart;
            delete myDepart;
            myDepart = depart = NULL;
        }
		else {
			((RegularUpdatedCache*)_contactCache)->GetLocalAvatarList<DepartmentInfo >(*depart);
			((RegularUpdatedCache*)_contactCache)->GetLocalAvatarList<DepartmentInfo >(*myDepart);
		}

    }
    else {
        res = UCC_STATUS_CODE_ERROR;
    }

	_dispatcher->OrgAddressBookReceived(res, depart, myDepart, taskId);
	return res;
}

void ContactService::GetOrgMembers(int orgId, int limit, std::string& copyback) {
    UC_LOG(INFO) << "get organization members, orgId: " << orgId << ", limit: " << limit;

    ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
    if (clientService != NULL) {
        HTTPRequest request;
        _requestFactory->CreateOrgMembersRequest(orgId, limit, clientService->GetLoginInfo().GetCustomerCode(), request);

        _engine->SendAsyncRequest(request, MakeRequestCallback(
            this, &ContactService::OrgAddrMembersReceived, orgId, copyback));
    }
}

int32_t ContactService::OrgAddrMembersReceived(int32_t errorCode, HTTPResponse *response, int orgId, std::string copyback) {
    UC_LOG(WARNING) << "get organization members response, errorCode: " 
        << errorCode << ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
        << ", body: " << (response != NULL ? response->GetContent() : "") << ", orgId: " << orgId;

    int32_t res = UC_FAILED;
    int userCount = 0;
    std::vector<int32_t> *memberIds = new std::vector<int32_t>();
    if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {
		res = _responseParser.ParseOrgMember(response->GetContent(), userCount, *memberIds);
    }

    _dispatcher->OrgAddrMembersReceived(res, orgId, userCount, memberIds,copyback);
	return res;
}

int32_t ContactService::SubscribeUserStatus(std::list<int32_t>& userIds){

	UserStatusList infos;
	_GetUserStatus(userIds, infos);
	if (!infos.empty()){
		_dispatcher->UserStatusReceived(UC_SUCCEEDED, infos);
	}
	
	if (userIds.empty()){
		return UC_SUCCEEDED;
	}
	
	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService != NULL) {
		HTTPRequest request;
		_requestFactory->CreateUserStatusRequest(USER_STATUS_SUB, userIds, request);

		std::string copyBack = "";
		_engine->SendAsyncRequest(request, MakeRequestCallback(
			this, &ContactService::UserStatusGetResponse, USER_STATUS_SUB,copyBack));
	}
	return UC_SUCCEEDED;
}

int32_t ContactService::GetUserStatus(const std::list<int32_t>& userIds, IN std::string & copyBack){
	
	UserStatusList infoList;
	std::list<int32_t> tempUserIds = userIds;

	// Notifing UI if all user's status we cached, otherwise getting from Server. 
	_GetUserStatus(tempUserIds, infoList);
	if (tempUserIds.empty())
	{
		if (!infoList.empty()){
			_dispatcher->UserGetStatusResponse(UC_SUCCEEDED, infoList, copyBack);
		}

		return UC_SUCCEEDED;
	}

	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService != NULL) {
		HTTPRequest request;
		_requestFactory->CreateUserStatusRequest(USER_STATUS_GET, userIds, request);

		_engine->SendAsyncRequest(request, MakeRequestCallback(
			this, &ContactService::UserStatusGetResponse, USER_STATUS_GET,copyBack));
	}
	return UC_SUCCEEDED;
}

int32_t ContactService::UserStatusGetResponse(int32_t errorCode, HTTPResponse *response, StatusOperation opt, std::string copyBack){	
	UC_LOG(INFO) << "get user status response, errorCode: " << errorCode << ", opt: " << opt 
		<<", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
		<< ", body: " << (response != NULL ? response->GetContent() : "");

	int32_t res = UC_FAILED;
	UserStatusList infos;
	if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {
		res = _responseParser.ParseGetUserStatus(response->GetContent(), infos);
		if (res == UC_SUCCEEDED){
			//UpdateUserStatus(infos);
			if (opt == USER_STATUS_SUB){
				UpdateUserStatus(infos);
				_dispatcher->UserStatusReceived(res, infos);	
			}
			else if (opt == USER_STATUS_GET){
				_dispatcher->UserGetStatusResponse(res, infos, copyBack);
			}
		}
	}
	if (!infos.empty()){
		infos.clear();
	}

	return res;
}

int32_t ContactService::UnsubscribeUserStatus(std::list<int32_t>& userIds){

	_UnsubscribeUserStatus(userIds);
	if (userIds.empty()){
		return UC_SUCCEEDED;
	}

	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService != NULL) {
		HTTPRequest request;
		_requestFactory->CreateUserStatusRequest(USER_STATUS_UNSUB, userIds, request);

		_engine->SendAsyncRequest(request, MakeRequestCallback(
			this, &ContactService::UserStatusResponse, USER_STATUS_UNSUB, userIds));
	}
	return UC_SUCCEEDED;
}

int32_t ContactService::UnsubscribeAllUserStatus(){

	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService != NULL) {
		HTTPRequest request;
		std::list<int32_t> userIds;
		_requestFactory->CreateUserStatusRequest(USER_STATUS_UNSUBALL, userIds, request);

		_engine->SendAsyncRequest(request, MakeRequestCallback(
			this, &ContactService::UserStatusResponse, USER_STATUS_UNSUBALL, userIds));
	}
	return UC_SUCCEEDED;
}


int32_t ContactService::UserStatusResponse(int32_t errorCode, HTTPResponse *response, StatusOperation opt, std::list<int32_t> userIds){

	UC_LOG(INFO) << "subscribe user status response, opt: " << opt << ", errorCode: " << errorCode 
		<< ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)	<< ", body: " << (response != NULL ? response->GetContent() : "");

	int32_t res = UC_FAILED;
	int32_t status = PresenceType::Online;
	if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {
		res = _responseParser.ParseUserStatus(response->GetContent());
		if (res == UC_SUCCEEDED){
			 if (USER_STATUS_UNSUBALL == opt){
				 _ClearUserStatus();
			 }
			 else if(USER_STATUS_UNSUB == opt){
				 _UpdateUserStatus(userIds);
			 }
		}
	}

	return res;

}

int32_t ContactService::SetUserStatus(int32_t userId, int32_t status, const std::string& show){

	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService != NULL) {
		HTTPRequest request;
		_requestFactory->CreateUserStatusSetRequest(userId, status, request);

		_engine->SendAsyncRequest(request, MakeRequestCallback(
			this, &ContactService::UserStatusSetResponse, userId, status, show));
	}

	return UC_SUCCEEDED;
}

int32_t ContactService::UserStatusSetResponse(int32_t errorCode, HTTPResponse *response, int32_t userId, int32_t status, std::string show){

	UC_LOG(INFO) << "set user status response, errorCode: " << errorCode << ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
		<< ", body: " << (response != NULL ? response->GetContent() : "");

	int32_t res = UC_FAILED;
	if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {
		res = _responseParser.ParseSetUserStatus(response->GetContent(), status);
		if (res == UC_SUCCEEDED){
			boost::unique_lock<boost::recursive_mutex> lock(_user_status_mutex);
			UserStatusMap::iterator iter = _user_status_cache.find(userId);
			if (iter != _user_status_cache.end()){
				iter->second.SetUserStatus(status);
				iter->second.SetStatusShow(show);
			}
		}
	}

	_dispatcher->UserStatusSet(res, userId, status);
	return res;
}

int32_t ContactService::OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state) {
	UCHead head;
    msg.GetHead(head);
	if (head.GetAppId() == AppId::AppPresence) {
		// 状态消息
		_processor->ProcessPresenceMsg((MessagePresenceMsg&) msg);
	}
	else if (head.GetAppId() == AppId::AppAddressBook) {
		// 通讯录好友更新消息
		if (head.GetProtocoltype() == AddressBookMessageType::CardUpdateMsg) {
			_processor->ProcessContactUpdateMsg((CardUpdateContentMsg&) msg);
		}
		else if (head.GetProtocoltype() == AddressBookMessageType::ContacterRelationUpdateMsg) {
			if (head.GetProtocolId() == ContacterRelationUpdateMessageId::ContacterAdd) {
				_processor->ProcessContactAddMsg((ContacterAddContentMsg&) msg);
			}
			else if (head.GetProtocolId() == ContacterRelationUpdateMessageId::ContacterDel) {
				_processor->ProcessContactDelMsg((ContacterDelContentMsg&) msg);
			}	
		}
	}
	else if (head.GetAppId() == AppId::AppNotify) {
		// 系统通知消息
		if (head.GetProtocoltype() == SystemProtoMessageType::CompleteInfoType) {
			if (head.GetProtocolId() == CompleteInfoMessageId::tagAdd) {
				_processor->ProcessCompleteInfoTypeMsg((CompleteInfoContentMsg&) msg, 1);
			}
			else if (head.GetProtocolId() == CompleteInfoMessageId::tagDel) {
				_processor->ProcessTagDelContentMsg((TagDelContentMsg&) msg);
			}
			else if (head.GetProtocolId() == CompleteInfoMessageId::tagUpdate) {
				_processor->ProcessCompleteInfoTypeMsg((CompleteInfoContentMsg&) msg, 2);
			}
		}
	}
	else if (head.GetAppId() == AppId::AppOrganization) {
		if (head.GetProtocoltype() == OrganizeProtoMessageType::OrganizeType) {
			if (head.GetProtocolId() == OrganizeProtoMessageId::DeptUpdate) {
				_processor->ProcessDeptUpdated((DeptUpdateContentMsg&) msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::DeptTransfer) {
				_processor->ProcessDeptTransfered((DeptTransferContentMsg&) msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::EmployeeEntry) {
				_processor->ProcessEmployeeEntry((EmployeeEntryContentMsg&) msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::EmployeeLeave) {
				_processor->ProcessEmployeeLeaved((EmployeeLeaveContentMsg&) msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::EmployeeRightsUpdate) {
				_processor->ProcessEmployeeRightsUpdated((EmployeeRightsChangeContentMsg&) msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::PositionUpdate) {
				_processor->ProcessPositionUpdated((PositionUpdateContentMsg& )msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::DeptParentChange) {
				_processor->ProcessDeptParentChanged((DeptParentChangeContentMsg&) msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::CompanyJoin) {
				_processor->ProcessCompanyJoined((CompanyJoinContentMsg&) msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::DeptDelete) {
				_processor->ProcessDeptDeleted((DeptDeleteContentMsg&) msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::EmployeeEntryConfirm) {
				_processor->ProcessEmployeeEntryConfirmed((EmployeeEntryConfirmContentMsg&) msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::EmployeeLeaveConfirm) {
				_processor->ProcessEmployeeLeaveConfirmed((EmployeeLeaveConfirmContentMsg&) msg);
			}
			else if (head.GetProtocolId() == OrganizeProtoMessageId::DeptTransferConfirm) {
				_processor-> ProcessDeptTransferConfirmed((DeptTransferConfirmContentMsg&) msg);
			}
        }
	}
	return 0;
}

void ContactService::OnLoginSucceeded() {
	this->Initialize();

	boost::unique_lock<boost::recursive_mutex> lock(_user_status_mutex);
	_user_status_cache.clear();
}

void ContactService::OnLogoutSucceeded() {
	
	this->UnInitialize();
}

void ContactService::OnUCASConnected(int32_t errorCode){
	if (errorCode == UC_SUCCEEDED){

		//断线重连后，向Server同步当前缓存的用户状态;
		// 特别注意，sign out re-sign in也会进这里，但是此时的list应该为空。
		boost::unique_lock<boost::recursive_mutex> lock(_user_status_mutex);
		std::list<int32_t> userIds;
		for (auto item : _user_status_cache){
			userIds.push_back(item.first);
		}
		if (!userIds.empty()){
			std::string copyBack = "";
			GetUserStatus(userIds,copyBack);
		}

	}

}

void ContactService::OnContactCacheInitialized(int32_t res) {
	 _dispatcher->ContactLoaded(res);
}

void ContactService::GetUpdatedContacts() {
    UC_LOG(WARNING) << "get updated contacts , _updateStatus: " << _updateStatus;

    if (_updateStatus == CUS_UPDATING) {
        UC_LOG(WARNING)<< "contacts are updating";
        return;
    }

    _updateStatus = CUS_UPDATING;

    int page = 1;
    // get last contact sync time from database
    int64_t lastSyncTime;
    int32_t res = GetLastContactSyncTime(&lastSyncTime);
    if(res != UC_SUCCEEDED) {
        // _dispatcher->ContactUpdated(res, NULL);
        return;
    }

    GetUpdatedContacts(page, lastSyncTime, 0);
}

void ContactService::GetUpdatedContacts(int page, uint64_t lastSyncTime, uint64_t newSyncTime) {
    UC_LOG(WARNING) << "get updated contact list , page:" << page
        << ", lastSyncTime: " << lastSyncTime
        << ", newSyncTime: " << newSyncTime;

    HTTPRequest request;
    _requestFactory->CreateContactListRequest(page, CONTACT_LIST_PAGESIZE, lastSyncTime, request);
    _engine->SendAsyncRequest(request, MakeRequestCallback(
        this, &ContactService::ContactUpdated, page, lastSyncTime, newSyncTime));
}

int32_t ContactService::ContactUpdated(int32_t errorCode, HTTPResponse *response, int page, 
    uint64_t lastSyncTime, uint64_t newSyncTime) {

    UC_LOG(WARNING) << "updated contact list response, page:" << page
        << ", errorCode: " << errorCode
        << ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
        << ", body: " << (response != NULL ? response->GetContent() : "");

    std::list<ContactInfoImpl*> contactList;
    std::list<ContactInfoImpl*> deletedContacts;

    int32_t res = UC_FAILED;
    int count = 0;
    if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200) {
        if (page == 1) {
            res = _responseParser.ParseContactList(response->GetContent(), &newSyncTime, contactList, deletedContacts);
        }
        else {
            res = _responseParser.ParseContactList(response->GetContent(), NULL, contactList, deletedContacts);
        }

    }
    else {
        res = UCC_STATUS_CODE_ERROR;
    }
        
    if (res == UC_SUCCEEDED) {
        count = (int)(contactList.size() + deletedContacts.size());
        
        if (!contactList.empty()) {
			// 过滤垃圾数据
			std::list<ContactInfoImpl*>::iterator iter = contactList.begin();
            for (; iter !=  contactList.end();) {
				if ((*iter)->GetPingyin().empty() || (*iter)->GetDisplayName().empty()) {
					delete *iter;
					iter = contactList.erase(iter);
				}
				else {
					++iter;
				}
            }

            // 保存常用联系人
            SetContactsType(contactList, true);
			((RegularUpdatedCache*)_contactCache)->GetLocalAvatarList<std::list<ContactInfoImpl*> >(contactList);
            SaveContactIntoCache(contactList);

            std::list<ContactInfo*> *infoList = new std::list<ContactInfo*>();
            ConvertContactInfo(*infoList, contactList);
            _dispatcher->ContactUpdated(infoList);
        }
      
        // 触发常用联系人删除事件
        if (deletedContacts.size() > 0) {
            std::list<ContactInfoImpl*>::iterator iter = deletedContacts.begin();
            for (; iter !=  deletedContacts.end(); ++iter) {
				if (_contactCache->IsCommonContact((*iter)->GetUserId())) {
					_contactCache->RemoveCommonContact((*iter)->GetUserId());
					_dispatcher->ContactDeleted(UC_SUCCEEDED, (*iter)->GetUserId());
				}
            }
        }

        if (count < CONTACT_LIST_PAGESIZE) {
            _updateStatus = CUS_UPDATED;

            ((RegularUpdatedCache*)_contactCache)->RefreshCommonContactUpdatedTime();

            std::list<int32_t> userIds;
			_contactCache->GetCommonContactUserIds(userIds);
        }
        else if (count >= CONTACT_LIST_PAGESIZE) {
            GetUpdatedContacts(++page, lastSyncTime, newSyncTime);
        }
    }
    else {
        _updateStatus = CUS_UPDATED_ERROR;
    }

	SAFE_CLEAR_CONTAINER(std::list<ContactInfoImpl*>, deletedContacts);
	SAFE_CLEAR_CONTAINER(std::list<ContactInfoImpl*>, contactList);

	return res;
}

int32_t ContactService::GetCurUserId() {
    ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
    if (clientService != NULL) {
        return clientService->GetLoginInfo().GetUserId();
    }
    
    return 0;
}

bool ContactService::IsCurUser(UCID userUid){
	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService != NULL) {
		return (userUid.GetUserId() == clientService->GetLoginInfo().GetUserId()
			&& userUid.GetSiteId() == clientService->GetLoginInfo().GetSiteId()
			&& userUid.GetResId() == clientService->GetLoginInfo().GetResId());
	}

	return false;
}

void ContactService::GETRequestUrl(const std::string& methodUrl, std::stringstream &url) {
	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
    if (clientService != NULL) {
		const LoginInfo& info = clientService->GetLoginInfo();
		url << info.GetUccServerDomain() << methodUrl << "?user_id=" <<info.GetUserId()
			<< "&session_id=" << info.GetSessionId();
    }
}

void ContactService::POSTRequestUrl(const std::string& methodUrl, std::stringstream &url) {
	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
    if (clientService != NULL) {
		const LoginInfo& info = clientService->GetLoginInfo();
		url << info.GetUccServerDomain() << methodUrl;
    }
}

void ContactService::POSTRequestBody(std::stringstream &body){
	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
    if (clientService != NULL) {
		const LoginInfo& info = clientService->GetLoginInfo();
		body << "user_id=" << info.GetUserId() << "&session_id=" << info.GetSessionId();
    }
}

void ContactService::SetContactsType(const std::list<ContactInfoImpl*>& contactList, bool isCommon) {
	std::list<ContactInfoImpl*>::const_iterator iter = contactList.begin();

    for (; iter != contactList.end(); ++iter) {
        SetContactType(**iter, isCommon);
    }
}

void ContactService::SetContactType(ContactInfoImpl& contact, bool isCommon) {
    if (isCommon) {
        contact.SetCommon();
    }
	else if(contact.GetUserId() == GetCurUserId()) {
        contact.SetCurrentUser();
    }
	else if (_contactCache->IsCommonContact(contact.GetUserId())){
		contact.SetCommon();
	}
    else {
         contact.SetOthers();
    }
}

int32_t ContactService::GetLastContactSyncTime(int64_t* time) {
    UC_LOG(INFO) << "get last contact sync time";

    int32_t res = UC_SUCCEEDED;
    if (_lastContactSyncTime == -1) {
        std::string application(CONTACT_CONFIG_APPLICATION);
        std::string cfgName(LAST_CONTACT_SYNC_TIME);
        
        std::string strTime;
        res = DBManager::GetInstance().GetConfigValue(application, cfgName, strTime);
        if(res == UC_SUCCEEDED) {
            std::istringstream s(strTime);
            s >> _lastContactSyncTime;
            (*time) = _lastContactSyncTime;
        }
        else {
            if (res == DBError::DB_QUERY_EMPTY){
                // 数据库查询结果为空时，默认设为0
                *time = _lastContactSyncTime = 0;
                res = UC_SUCCEEDED;
            }
            else {
                res = UC_FAILED;
                 UC_LOG(ERROR) << "fail to get last contact sync time, result: " << res;
            }
        }
    }
    else {
        *time = _lastContactSyncTime;
    }

    if (res == UC_SUCCEEDED) {
        UC_LOG(INFO) << "get last contact sync time, result: " << res
            << ", last_sync_time: " << *time;
    }
    
    return res;
}
    
int32_t ContactService::SaveLastContactSyncTime(uint64_t newSyncTime) {
    _lastContactSyncTime = newSyncTime;

    std::string application(CONTACT_CONFIG_APPLICATION);
    std::string cfgName(LAST_CONTACT_SYNC_TIME);
    
    stringstream ss;
    ss << newSyncTime;
    int32_t res = DBManager::GetInstance().SaveConfigValue(application, cfgName, ss.str());
    if (res != UC_SUCCEEDED) {
        UC_LOG(ERROR) << "fail to save last contact sync time, result: " << res;
        res = UC_FAILED;
    }
    else {
        UC_LOG(INFO) << "save last contact sync time, result: " << res
            << ", last_sync_time: " << newSyncTime;
    }
    
    return res;
}

bool ContactService::GetCurrentUserInfo(ContactInfo &contact) {
	return _contactCache->GetCurrentUserInfo(contact);;
}

bool ContactService::GetCurrentUserFullInfo(ContactInfo &contact){
	bool ret = _contactCache->GetCurrentUserFullInfo(contact);
	if (contact.GetUserTags() == "")
	{
		//_contactDelayLoader->GetContact(GetCurUserId(), DISPATCH_CONTACT_INFO_RECEIVED);
		GetContactInfoFromServer(GetCurUserId(), DISPATCH_CONTACT_INFO_RECEIVED);
	}
	return ret;
}

void ContactService::UpdateUserInfo(const std::string& updateField) {

    UC_LOG(INFO) << "Update user info, field: " << updateField;

	HTTPRequest	request;

	boost::property_tree::wptree *ptParse = new boost::property_tree::wptree();
	std::wstringstream jsonStream(StringConvert::FromUTF8Str(updateField));
	try {
		boost::property_tree::json_parser::read_json(jsonStream, *ptParse);  
	} catch (boost::property_tree::ptree_error & e) {
    	UC_LOG(ERROR) << "encounter an error when parsing updateField into an json object "
    		<< ", exception: " << e.what() << ", updateField: " << updateField;

		_dispatcher->UserInfoUpdated(JSON_PARSE_ERROR, NULL);

        delete ptParse;
		return;
    }

	_requestFactory->CreateUserUpdateRequest(updateField, request);

    _engine->SendAsyncRequest(request, MakeRequestCallback(
        this, &ContactService::UserInfoUpdated, ptParse));
}

void ContactService::UpdateContactAvatar(int32_t userId, const std::string& remoteAvatarUrl,
        const std::string& localAvatarPath) {
	UC_LOG(INFO) << "update contact avatar, userId: " << userId 
		<< ", remoteAvatarUrl: " << remoteAvatarUrl
		<< ", localAvatarPath: " << localAvatarPath;

	_contactCache->UpdateAvatar(userId, remoteAvatarUrl,localAvatarPath);
	// 如果是当前用户,那么需要更新historyaccount表里面AVATAR_URL字段的值
	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService != NULL) {
		if (clientService->GetLoginInfo().GetUserId() == userId) {
			std::string account = clientService->GetLoginInfo().GetAccount();	
			TaskExecutor::GetInstance().Schedule(MakeCallback(this, &ContactService::_UpdateAccountAvatar, account, localAvatarPath));
			//DBManager::GetInstance().UpdateAccountAvatar(clientService->GetLoginInfo().GetAccount(), localAvatarPath);
		}
	}
}

void ContactService::_UpdateAccountAvatar(const std::string& userAccount, const std::string& localAvatarPath)
{
	int32_t res = DBManager::GetInstance().UpdateAccountAvatar(userAccount, localAvatarPath);
	if (res != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "fail to update contact avatar, userAccount: " << userAccount
			<< ", localAvatarPath: " << localAvatarPath << ", res: " << res;
	}
	else {
		UC_LOG(INFO) << "update contact avatar, userAccount: " << userAccount
			<< ", localAvatarPath: " << localAvatarPath << ", res: " << res;
	}
}

int32_t ContactService::UserInfoUpdated(int32_t errorCode, HTTPResponse *response, 
    boost::property_tree::wptree* ptParse) {
    UC_LOG(INFO) << "update user info response, errorCode: "
        << errorCode << ", status_code: " << (response != NULL ? response->GetStatusCode() : 0)
        << ", body: " << (response != NULL ? response->GetContent() : "");

    int32_t res = UC_FAILED;
    ContactInfoImpl *impl = _contactCache->GetContact(GetCurUserId());
    if (errorCode == UC_SUCCEEDED && response != NULL && response->GetStatusCode() == 200 &&
		impl != NULL) {
        res = _responseParser.ParseUpdateUserInfo(response->GetContent());
    
		if (res == UC_SUCCEEDED) {
			impl->Update(*ptParse);
			_contactCache->GetLocalAvatar(*impl);
            SaveContactIntoCache(*impl);
		}
    }
    else {
        res = UCC_STATUS_CODE_ERROR;
    }
    
	ContactInfo *contact = NULL;
	if (impl != NULL) {
		contact = new ContactInfo(impl);
	}

	_dispatcher->UserInfoUpdated(res, contact);

    SAFE_DELETE(ptParse);
	return res;
}

void ContactService::UpdateUserStatus(const UserStatusList& infos){
	boost::unique_lock<boost::recursive_mutex> lock(_user_status_mutex);
	for (auto info : infos){
		UserStatusMap::iterator iter = _user_status_cache.find(info.GetUserId());
		if (iter != _user_status_cache.end()){
			iter->second.SetUserId(info.GetUserId());
			iter->second.SetUserStatus(info.GetUserStatus());
		}else{
			_user_status_cache[info.GetUserId()] = info;
		}
	}
}

int32_t ContactService::_GetUserStatus(std::list<int32_t>& userIds, UserStatusList& infoList){
	boost::unique_lock<boost::recursive_mutex> lock(_user_status_mutex);
	std::list<int32_t>::iterator iter = userIds.begin();
	for (; iter != userIds.end();){
		UserStatus info;
		if (UC_SUCCEEDED == _GetUserStatus(*iter, info)){
			infoList.push_back(info);
			userIds.erase(iter++);
		}
		else{
			++iter;
		}
	}

	return UC_SUCCEEDED;
}

int32_t ContactService::_GetUserStatus(int32_t userId, UserStatus& info){

	UserStatusMap::iterator iter = _user_status_cache.find(userId);
	if (iter != _user_status_cache.end()){
		iter->second.AddRefCount();
		info = iter->second;
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t ContactService::_UnsubscribeUserStatus(std::list<int32_t>& userIds){
	boost::unique_lock<boost::recursive_mutex> lock(_user_status_mutex);
	std::list<int32_t>::iterator iter = userIds.begin();
	for (; iter != userIds.end();){
		if (UC_SUCCEEDED == _UnsubscribeUserStatus(*iter)){
			userIds.erase(iter++);
		}
		else{
			++iter;
		}
	}

	return UC_SUCCEEDED;
}

int32_t ContactService::_UnsubscribeUserStatus(int32_t userId){
	UserStatusMap::iterator iter = _user_status_cache.find(userId);
	if (iter != _user_status_cache.end()){
		int32_t refCount = iter->second.GetRefCount();
		if (refCount > 1){
			iter->second.SubRefCount();
			return UC_SUCCEEDED;
		}else if (refCount == 1){
			iter->second.SubRefCount();
			return UC_FAILED;
		}
	}
	return UC_FAILED;
}

void ContactService::_UpdateUserStatus(std::list<int32_t>& userIds){

	boost::unique_lock<boost::recursive_mutex> lock(_user_status_mutex);
	for (auto userId : userIds){
		_user_status_cache.erase(userId);
	}
}

void ContactService::_UpdateUserStatus(UserStatusList infos){

	boost::unique_lock<boost::recursive_mutex> lock(_user_status_mutex);
	for (auto info : infos){
		_user_status_cache[info.GetUserId()] = info;
	}
}

void ContactService::_ClearUserStatus(){
	boost::unique_lock<boost::recursive_mutex> lock(_user_status_mutex);
	_user_status_cache.clear();	
}

} /* namespace uc */
