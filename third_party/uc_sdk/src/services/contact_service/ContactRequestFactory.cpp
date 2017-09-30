
#include <sstream>

#include "ContactRequestFactory.h"
#include "UCEngine.h"
#include "ContactService.h"
#include "CommonDef.h"
#include "LogManager.h"

namespace uc {

ContactRequestFactory::ContactRequestFactory(ContactService* contactService) 
	: _contactService(contactService){
}

void ContactRequestFactory::CreateContactListRequest(int page, int count, uint64_t lastSyncTime, HTTPRequest& request) {
	std::stringstream	urlStream;
	std::string urlMethod(CONTACT_LIST_METHOD);
	_contactService->GETRequestUrl(urlMethod, urlStream);
	urlStream << "&page=" << page << "&count=" 
			<< count << "&last_contact_time=" << lastSyncTime;

	UC_LOG(INFO) << "request updated contact list from server, url: " << urlStream.str();

	request.Init(urlStream.str(), HTTP_METHOD_GET);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
}

void ContactRequestFactory::CreateGetProfileRequest(const std::list<int32_t> &userIds, HTTPRequest& request) {
	if (!userIds.empty()) {
		std::stringstream	urlStream;
		std::string urlMethod(USER_PROFILE_METHOD);
		_contactService->GETRequestUrl(urlMethod, urlStream);
		urlStream << "&user_ids=[";

		std::list<int32_t>::const_iterator iterBegin = userIds.begin();
		std::list<int32_t>::const_iterator iter = iterBegin;
		for (; iter != userIds.end(); ++iter) {
			if (iter != iterBegin) {
				urlStream << ",";
			}
			urlStream << *iter;
		}
		urlStream << "]&detail_info=0";

		UC_LOG(INFO) << "request get profile from server, url: " << urlStream.str();

		request.Init(urlStream.str(), HTTP_METHOD_GET);
		request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	}
}

void ContactRequestFactory::CreateGetProfileRequest(const int32_t &userId, HTTPRequest& request){
	if (userId > 0) {
		std::stringstream	urlStream;
		std::string urlMethod(USER_PROFILE_METHOD);
		_contactService->GETRequestUrl(urlMethod, urlStream);
		urlStream << "&user_ids=[" << userId << "]&detail_info=1";

		UC_LOG(INFO) << "request get profile from server, url: " << urlStream.str();

		request.Init(urlStream.str(), HTTP_METHOD_GET);
		request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	}
}

void ContactRequestFactory::CreateContactAddRequest(int32_t contactId, HTTPRequest& request) {
	std::stringstream	urlStream;
	std::string urlMethod(CONTACT_ADD_METHOD);
	_contactService->POSTRequestUrl(urlMethod, urlStream);

	std::stringstream	bodyStream;
	_contactService->POSTRequestBody(bodyStream);
	bodyStream << "&contact_id=" << contactId;

	UC_LOG(INFO) << "request add contact from server, url: " << urlStream.str();

	request.Init(urlStream.str(), HTTP_METHOD_POST);
	request.SetBody(bodyStream.str());
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
}

void ContactRequestFactory::CreateContactDeleteRequest(int32_t contactId, HTTPRequest& request) {
	std::stringstream	urlStream;
	std::string urlMethod(CONTACT_DELETE_METHOD);
	_contactService->POSTRequestUrl(urlMethod, urlStream);

	std::stringstream	bodyStream;
	_contactService->POSTRequestBody(bodyStream);
	bodyStream << "&contact_id=" << contactId;

	UC_LOG(INFO) << "request delete contact from server, url: " << urlStream.str();

	request.Init(urlStream.str(), HTTP_METHOD_POST);
	request.SetBody(bodyStream.str());
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
}

void ContactRequestFactory::CreateContactSearchRequest(const std::string& keyword, int customerCode, int scope,
	int page, int count, HTTPRequest& request) {
	std::stringstream	urlStream;
	std::string urlMethod(CONTACT_SEARCH_METHOD);
	_contactService->GETRequestUrl(urlMethod, urlStream);
	urlStream << "&page=" << page << "&count="
			<< count << "&customer_code=" << customerCode
			<< "&query=" << keyword << "&scope=" << scope;

	UC_LOG(INFO) << "request updated contact list from server, url: " << urlStream.str();

	request.Init(urlStream.str(), HTTP_METHOD_GET);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
}

void ContactRequestFactory::CreateUserColleagueRequest(int32_t contactId,int page, int count, HTTPRequest& request) {
	std::stringstream	urlStream;
	std::string urlMethod(USER_COLLEAGUE_METHOD);
	_contactService->GETRequestUrl(urlMethod, urlStream);

	urlStream << "&target_user_id=" << contactId << "&page="
		<< page<< "&count=" << count;

	UC_LOG(INFO) << "request user colleague from server, url: " << urlStream.str();

	request.Init(urlStream.str(), HTTP_METHOD_GET);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
}

void ContactRequestFactory::CreateOrgListRequest(int orgId, const std::string& customerCode, HTTPRequest& request) {
	std::stringstream	urlStream;
	std::string urlMethod(ORG_LIST_METHOD);
	_contactService->GETRequestUrl(urlMethod, urlStream);
	urlStream << "&org_id=" << orgId << "&customer_code=" << customerCode;

	UC_LOG(INFO) << "request organization list from server, url: " << urlStream.str();

	request.Init(urlStream.str(), HTTP_METHOD_GET);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
}

void ContactRequestFactory::CreateOrgMembersRequest(int orgId, int limit, const std::string& customerCode, 
	HTTPRequest& request) {
	std::stringstream	urlStream;
	std::string urlMethod(ORG_MEMBER_METHOD);
	_contactService->GETRequestUrl(urlMethod, urlStream);
	urlStream << "&org_id=" << orgId << "&customer_code=" << customerCode << "&limit_count=" << limit;

	UC_LOG(INFO) << "request organization members from server, url: " << urlStream.str();

	request.Init(urlStream.str(), HTTP_METHOD_GET);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
}

void ContactRequestFactory::CreateUserUpdateRequest(const std::string &updateField, HTTPRequest& request) {
	std::stringstream urlStream;

	std::string urlMethod(USER_UPDATE_METHOD);
	_contactService->POSTRequestUrl(urlMethod, urlStream);

	std::stringstream	bodyStream;
	_contactService->POSTRequestBody(bodyStream);
	bodyStream << "&email=" << "&data=" << StringConvert::URLEncode(updateField);

	UC_LOG(INFO) << "update user info from server, url: " << urlStream.str()
			<< ", body: " << bodyStream.str();

	request.SetBody(bodyStream.str());
	request.Init(urlStream.str(), HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
}
    
void ContactRequestFactory::CreateTagUpdateRequest(const std::string& updateTags, HTTPRequest& request) {
	std::stringstream	urlStream;
	std::string urlMethod(TAGS_UPDATE_METHOD);
	_contactService->POSTRequestUrl(urlMethod, urlStream);

	std::stringstream	bodyStream;
	_contactService->POSTRequestBody(bodyStream);
	bodyStream << "&data=" << StringConvert::URLEncode(updateTags);
      
    UC_LOG(INFO) << "update user tag from server, url: " << urlStream.str()
        << ", body: " << bodyStream.str();
        
    request.SetBody(bodyStream.str());
    request.Init(urlStream.str(), HTTP_METHOD_POST);
    request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");      
}

void ContactRequestFactory::CreateUserStatusRequest(StatusOperation opt, const std::list<int32_t>& userIds, HTTPRequest& request){
	std::stringstream	urlStream;
	std::string urlMethod(USER_STATUS_GET_MEHTOD);
	_contactService->POSTRequestUrl(urlMethod, urlStream);

	std::stringstream	bodyStream;
	_contactService->POSTRequestBody(bodyStream);

	if (USER_STATUS_GET == opt){

		bodyStream << "&getusers=[";
		std::list<int32_t>::const_iterator iter = userIds.begin();
		for (; iter != userIds.end(); ++iter) {
			if (iter != userIds.begin()) {
				bodyStream << ",";
			}
			bodyStream << *iter;
		}
		bodyStream << "]";

	}else if (USER_STATUS_SUB == opt){

		bodyStream << "&subusers=[";
		std::list<int32_t>::const_iterator iter = userIds.begin();
		for (; iter != userIds.end(); ++iter) {
			if (iter != userIds.begin()) {
				bodyStream << ",";
			}
			bodyStream << *iter;
		}
		bodyStream << "]";

	}else if (USER_STATUS_UNSUB == opt){
		bodyStream << "&unsubusers=[";
		std::list<int32_t>::const_iterator iter = userIds.begin();
		for (; iter != userIds.end(); ++iter) {
			if (iter != userIds.begin()) {
				bodyStream << ",";
			}
			bodyStream << *iter;
		}
		bodyStream << "]";
	}else if (USER_STATUS_UNSUBALL == opt){
		bodyStream << "&unsuball=" << true;
	}

	request.SetBody(bodyStream.str());
	request.Init(urlStream.str(), HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
}

void ContactRequestFactory::CreateUserStatusSetRequest(int32_t userId, int32_t status, HTTPRequest& request){
	std::stringstream	urlStream;
	std::string urlMethod(USER_STATUS_SET_MEHTOD);
	_contactService->POSTRequestUrl(urlMethod, urlStream);

	std::stringstream	bodyStream;
	_contactService->POSTRequestBody(bodyStream);
	bodyStream << "&status=" << status;

	request.SetBody(bodyStream.str());
	request.Init(urlStream.str(), HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
}

}
