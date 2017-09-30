/*
 * UccRequestFactory.h
 *
 *  Created on: May 7, 2015
 *      Author: gang wang
 */

#ifndef UCC_RESPONSE_PARSER_H_
#define UCC_RESPONSE_PARSER_H_

#include <string>  
#include <list> 
#include <vector> 
#define BOOST_SPIRIT_THREADSAFE
#include "boost/property_tree/ptree.hpp"
#include "boost/smart_ptr/shared_ptr.hpp"

#include "ContactInfoImpl.h"
#include "DepartmentInfo.h"
#include "UCSDK.h"

namespace uc {

class ContactResponseParser {
public:
    int32_t ParseContactList(const std::string &jsonBody, uint64_t *timestamp,
            std::list<ContactInfoImpl*>& contacts, std::list<ContactInfoImpl*>& deletedContacts);

	int32_t ParseGetProfile(const std::string &jsonBody,
		std::list<ContactInfoImpl*>& contacts, bool isParseTag);

	int32_t ParseContactAdd(const std::string &jsonBody,
		ContactInfoImpl** contacts);

	int32_t ParseContactDelete(const std::string &jsonBody);

	int32_t ParseContactSearch(const std::string &jsonBody,
		std::list<ContactInfoImpl*>& contacts);

	int32_t ParseUserColleague(const std::string &jsonBody, int *leaderNum, int *colleagueNum,
		std::list<ContactInfoImpl*>& colleagues, std::list<ContactInfoImpl*>& leaders);

	int32_t ParseOrgList(const std::string &jsonBody, DepartmentInfo& departmentInfo,
		DepartmentInfo& myDepartment);

	int32_t ParseOrgMember(const std::string &jsonBody, int &userCount, std::vector<int32_t> &memberIds);

	int32_t ParseUpdateUserInfo(const std::string &jsonBody);

	int32_t ParseResetPassword(const std::string &jsonBody);

    int32_t ParseContactTags(const std::string &jsonBody, std::list<ContactTag*>& addedTags,
		std::list<ContactTag*>& updatedTags, std::list<ContactTag*>& deletedTags);
    
    int32_t ParseUpdateTags(const std::string &jsonBody);

	int32_t ParseSetUserStatus(const std::string &jsonBody, int32_t &status);

	int32_t ParseGetUserStatus(const std::string &jsonBody, std::list<UserStatus> &infos);

	int32_t ParseUserStatus(const std::string &jsonBody);

private:
	int32_t GetContactListFromJson(const boost::property_tree::wptree &ptParse,
		std::list<ContactInfoImpl*>& contacts, bool isParseTag);

	int32_t GetContactListFromJsonEx(const std::string &jsonBody,
		std::list<ContactInfoImpl*>& contacts, bool isParseTag = true);

	void GetColleagueFromJson(const boost::property_tree::wptree &ptParse,
		ContactInfoImpl &contact);

	void GetJsonPtree(const std::string &jsonBody, boost::property_tree::wptree &ptParse);

	int32_t GetStatus(const boost::property_tree::wptree &ptParse);
	int32_t GetStatusEx(const std::string &jsonBody);
    
    void GetTagsFromJson(const boost::property_tree::wptree &ptParse,
        std::list<ContactTag*>& addedTags, std::list<ContactTag*>& updatedTags, 
		std::list<ContactTag*>& deletedTags);
};

}
#endif
