/*
 * UccRequestFactory.h
 *
 *  Created on: May 7, 2015
 *      Author: gang wang
 */

#ifndef UCC_REQUEST_FACTORY_H_
#define UCC_REQUEST_FACTORY_H_

#include <list>
#include <vector>

#include <string>

#include "HTTPRequest.h"

namespace uc {

enum StatusOperation
{
	USER_STATUS_GET=1, //获取状态;
	USER_STATUS_SUB,   //订阅状态;
	USER_STATUS_UNSUB, //部分取消订阅状态;
	USER_STATUS_UNSUBALL, //全部取消订阅状态;
};

class ContactService;

class ContactRequestFactory {
public:
	ContactRequestFactory(ContactService* contactService);

	void CreateContactListRequest(int page, int count, uint64_t lastSyncTime,
                                  HTTPRequest& request);

	void CreateGetProfileRequest(const std::list<int32_t> &userIds, HTTPRequest& request);

	void CreateGetProfileRequest(const int32_t &userId, HTTPRequest& request);

	void CreateContactAddRequest(int32_t contactId, HTTPRequest& request);

	void CreateContactDeleteRequest(int32_t contactId, HTTPRequest& request);

	void CreateContactSearchRequest(const std::string& keyword, int customerCode, int scope, int page, int count, HTTPRequest& request);

	void CreateUserColleagueRequest(int32_t contactId,int page, int count, HTTPRequest& request);

	void CreateOrgListRequest(int orgId, const std::string& customerCode, HTTPRequest& request);

	void CreateOrgMembersRequest(int orgId, int limit, const std::string& customerCode, HTTPRequest& request);

	void CreateUserUpdateRequest(const std::string &updateField, HTTPRequest& request);
    
	void CreateTagUpdateRequest(const std::string& userTags,  HTTPRequest& request);

	void CreateUserStatusRequest(StatusOperation opt, const std::list<int32_t>& userIds, HTTPRequest& request);

	void CreateUserStatusSetRequest(int32_t userId, int32_t status, HTTPRequest& request);

private:
	ContactService *_contactService;
};

}
#endif
