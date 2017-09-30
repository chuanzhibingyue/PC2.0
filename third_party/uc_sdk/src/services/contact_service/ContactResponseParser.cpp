
#define BOOST_SPIRIT_THREADSAFE

#include "ContactResponseParser.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include "LogManager.h"
#include "UCErrorCode.h"
#include "common/StringConvert.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "jsonhelper.h"

namespace uc {

using namespace boost::property_tree;

int32_t ContactResponseParser::ParseContactList(const std::string &jsonBody, uint64_t *timestamp,
	std::list<ContactInfoImpl*>& contacts, std::list<ContactInfoImpl*>& deletedContacts) {
	boost::property_tree::wptree ptParse;

	int res = UC_SUCCEEDED;
	try {
		GetJsonPtree(jsonBody, ptParse);

        res = GetStatus(ptParse);
        if (res != 0) {
            return res;
        }

		if(timestamp != NULL) {
			*timestamp = ptParse.get<uint64_t>(L"timestamp");
		}
		
        boost::property_tree::wptree& dataTree = ptParse.get_child(L"data");
        BOOST_FOREACH(const boost::property_tree::wptree::value_type &v, dataTree) {
            ContactInfoImpl* contactPtr = ContactInfoImpl::CreateContactInfoImpl(v.second,false);
            if (contactPtr == NULL) {
				break;
			}

            int isDelete = v.second.get<int>(L"is_delete", 0);
            if (isDelete == 0) {
                contactPtr->SetCommon();
                contacts.push_back(contactPtr);
            }
            else {
                deletedContacts.push_back(contactPtr);
            }
        }

	} catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an error when parsing contact list "
    		<< "from an json object, exception: " << e.what();

		res = JSON_PARSE_ERROR;
	}

	if (res != UC_SUCCEEDED) {
		std::list<ContactInfoImpl*>::iterator iter = contacts.begin();
		std::list<ContactInfoImpl*>::iterator iterEnd = contacts.end();
		for (; iter != iterEnd; ) {
			delete *iter;
			iter = contacts.erase(iter);
		}

		iter = deletedContacts.begin();
		iterEnd = deletedContacts.end();
		for (; iter != iterEnd; ) {
			delete *iter;
			iter = deletedContacts.erase(iter);
		}
	}

	return res;
}

int32_t ContactResponseParser::ParseGetProfile(const std::string &jsonBody,
	std::list<ContactInfoImpl*>& contacts, bool isParseTag) {
	return GetContactListFromJsonEx(jsonBody, contacts, isParseTag);
}

int32_t ContactResponseParser::ParseContactAdd(const std::string &jsonBody,
	ContactInfoImpl** contact) {
	std::list<ContactInfoImpl*> contacts;
    int32_t res = GetContactListFromJsonEx(jsonBody, contacts);
	if (res == UC_SUCCEEDED && !contacts.empty()) {
		if (contact != NULL) {
			*contact = *(contacts.begin());
		}
		
		return UC_SUCCEEDED;
	}
    
	return JSON_PARSE_ERROR;
}

int32_t ContactResponseParser::ParseContactDelete(const std::string &jsonBody) {
	boost::property_tree::wptree ptParse;

	try {
		GetJsonPtree(jsonBody, ptParse);

        return GetStatus(ptParse);

	} catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an error when parsing contact deleted response"
    		<< ", exception: " << e.what();
		return JSON_PARSE_ERROR;
	}
}

int32_t ContactResponseParser::ParseContactSearch(const std::string &jsonBody,
	std::list<ContactInfoImpl*>& contacts) {
	return GetContactListFromJsonEx(jsonBody, contacts);
}

int32_t ContactResponseParser::ParseUserColleague(const std::string &jsonBody,
	int *leaderNum, int *colleagueNum, std::list<ContactInfoImpl*>& colleagues, 
	std::list<ContactInfoImpl*>& leaders) {
	boost::property_tree::wptree ptParse;

	int res = UC_SUCCEEDED;
	try {
		GetJsonPtree(jsonBody, ptParse);

        res = GetStatus(ptParse);
        if (res != 0) {
            return res;
        }

		boost::property_tree::wptree::iterator iter = ptParse.get_child(L"data").begin();
		if (iter != ptParse.get_child(L"data").end()) {
			if(leaderNum != NULL) {
			*leaderNum = iter->second.get<uint32_t>(L"leader_count");
			}

			if(colleagueNum != NULL) {
				*colleagueNum = iter->second.get<uint32_t>(L"colleague_count");
			}

			boost::property_tree::wptree collTree = iter->second.get_child(L"colleague");
			BOOST_FOREACH(const boost::property_tree::wptree::value_type &v, collTree) {
				ContactInfoImpl* contactPtr = ContactInfoImpl::CreateContactInfoImpl();
				GetColleagueFromJson(v.second, *contactPtr);
				colleagues.push_back(contactPtr);
			}

			boost::property_tree::wptree leaderTree = iter->second.get_child(L"leader");
			BOOST_FOREACH(const boost::property_tree::wptree::value_type &v, leaderTree) {
				ContactInfoImpl* contactPtr = ContactInfoImpl::CreateContactInfoImpl();
				GetColleagueFromJson(v.second, *contactPtr);
				leaders.push_back(contactPtr);
			}
		}

		return UC_SUCCEEDED;
	} catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an error when parsing user colleagues "
    		<< "from an json object, exception: " << e.what();

		res =  JSON_PARSE_ERROR;
	}

	if (res != UC_SUCCEEDED) {
		std::list<ContactInfoImpl*>::iterator iter = colleagues.begin();
		std::list<ContactInfoImpl*>::iterator iterEnd = colleagues.end();
		for (; iter != iterEnd; ) {
			delete *iter;
			iter = colleagues.erase(iter);
		}

		iter = leaders.begin();
		iterEnd = leaders.end();
		for (; iter != iterEnd; ) {
			delete *iter;
			iter = leaders.erase(iter);
		}
	}

	return res;
}

int32_t ContactResponseParser::ParseOrgList(const std::string &jsonBody,
	DepartmentInfo& departmentInfo, DepartmentInfo& myDepartment) {
	boost::property_tree::wptree ptParse;

	try {
		GetJsonPtree(jsonBody, ptParse);

        int32_t code = GetStatus(ptParse);
        if (code != 0) {
            return code;
        }

		boost::property_tree::wptree::iterator iter = ptParse.get_child(L"data").begin();
		if (iter != ptParse.get_child(L"data").end()) {

			boost::property_tree::wptree& dataTree = iter->second.get_child(L"dept_list");

			departmentInfo.SetId(dataTree.get<uint32_t>(L"org_id"));
			departmentInfo.SetDepartName(StringConvert::ToUTF8Str(dataTree.get<std::wstring>(L"org_name")));
			departmentInfo.SetTotalNum(dataTree.get<uint32_t>(L"total", 0));
			departmentInfo.SetStaffNum(dataTree.get<uint32_t>(L"count", 0));
			departmentInfo.SetParentId(dataTree.get<uint32_t>(L"parent_id", 0));
			departmentInfo.SetOnlineNum(dataTree.get<uint32_t>(L"online", 0));

			boost::optional<wptree&> memberListOptional = dataTree.get_child_optional(L"member_list");
			if (memberListOptional) {
				std::vector<ContactInfoImpl*> contactImpl;
				BOOST_FOREACH(const wptree::value_type &v, memberListOptional.get()) {
					ContactInfoImpl* contactPtr = ContactInfoImpl::CreateContactInfoImpl(v.second,false);
					//GetMemberListFromJson(v.second, *contactPtr);
					if (contactPtr != NULL) {
						contactImpl.push_back(contactPtr);
					}
				}
				std::vector<ContactInfo*> contactInfo;
				for(std::size_t i = 0; i < contactImpl.size(); ++i) {
					contactInfo.push_back(new ContactInfo(contactImpl[i]));
				}
				departmentInfo.SetStaffs(contactInfo);
			}
			
			boost::optional<wptree&> myDeptOptional = dataTree.get_child_optional(L"my_dept");
			if (myDeptOptional) {
				wptree::const_iterator myDeptIter =  myDeptOptional.get().begin();

				departmentInfo.SetId(myDeptIter->second.get<uint32_t>(L"org_id"));
				departmentInfo.SetDepartName(StringConvert::ToUTF8Str(myDeptIter->second.get<std::wstring>(L"org_name")));
				departmentInfo.SetStaffNum(myDeptIter->second.get<uint32_t>(L"count", 0));
				departmentInfo.SetParentId(myDeptIter->second.get<uint32_t>(L"parent_id", 0));
				departmentInfo.SetOnlineNum(myDeptIter->second.get<uint32_t>(L"online", 0));
			}

			boost::property_tree::wptree deptList = dataTree.get_child(L"dept_list");
			if (!deptList.empty()) {
				std::vector<DepartmentInfo*> subDepartments;

				BOOST_FOREACH(boost::property_tree::wptree::value_type &v, deptList) {
					DepartmentInfo *depart = new DepartmentInfo();

					depart->SetId(v.second.get<uint32_t>(L"org_id"));
					depart->SetDepartName(StringConvert::ToUTF8Str(v.second.get<std::wstring>(L"org_name", L"")));
					depart->SetStaffNum(v.second.get<uint32_t>(L"count", 0));
					// key和服务器商量是否改名parent_id
					depart->SetParentId(v.second.get<uint32_t>(L"parend_id", 0));

					subDepartments.push_back(depart);
				}
				departmentInfo.SetSubDepart(subDepartments);
			}
		
			return UC_SUCCEEDED;
		}
		
	} catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an error when parsing org list "
    		<< "from an json object, exception: " << e.what();
	}
	return JSON_PARSE_ERROR;
}


int32_t ContactResponseParser::ParseOrgMember(const std::string &jsonBody, int &userCount, 
	std::vector<int32_t> &memberIds){
	boost::property_tree::wptree ptParse;

	try {
		GetJsonPtree(jsonBody, ptParse);

        int32_t code = GetStatus(ptParse);
        if (code != 0) {
            return code;
        }

        boost::property_tree::wptree::iterator iter = ptParse.get_child(L"data").begin();
		if (iter != ptParse.get_child(L"data").end()) {
			userCount = iter->second.get<uint32_t>(L"user_count");

			boost::property_tree::wptree memberList = iter->second.get_child(L"member_ids");
			if (!memberList.empty()) {
				BOOST_FOREACH(boost::property_tree::wptree::value_type &v, memberList) {

					memberIds.push_back(v.second.get_value<uint32_t>());
				}
			}
		}

		return UC_SUCCEEDED;
    }
    catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an error when parsing org member "
    		<< "from an json object, exception: " << e.what();
	}

	return JSON_PARSE_ERROR;
}

int32_t ContactResponseParser::ParseUpdateUserInfo(const std::string &jsonBody) {
	return GetStatusEx(jsonBody);
}

int32_t ContactResponseParser::ParseResetPassword(const std::string &jsonBody) {
	return GetStatusEx(jsonBody);
}

int32_t ContactResponseParser::GetContactListFromJsonEx(const std::string &jsonBody,
	std::list<ContactInfoImpl*>& contacts, bool isParseTag) {
	boost::property_tree::wptree ptParse;

	try {
		GetJsonPtree(jsonBody, ptParse);
        
        int32_t code = GetStatus(ptParse);
		if (code != 0) {
			return code;
		}

		GetContactListFromJson(ptParse.get_child(L"data"), contacts, isParseTag);

		return UC_SUCCEEDED;
	} catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an error when get getting contact list "
    		<< "from an json object, exception: " << e.what();

		std::list<ContactInfoImpl*>::iterator iter = contacts.begin();
		std::list<ContactInfoImpl*>::iterator iterEnd = contacts.end();
		for (; iter != iterEnd; ) {
			delete *iter;
			iter = contacts.erase(iter);
		}

		return JSON_PARSE_ERROR;
	}
}

int32_t ContactResponseParser::ParseContactTags(const std::string &jsonBody,
	std::list<ContactTag*>& addedTags, std::list<ContactTag*>& updatedTags, 
	std::list<ContactTag*>& deletedTags) {
    boost::property_tree::wptree ptParse;
        
    try {
        GetJsonPtree(jsonBody, ptParse);
        
        int32_t code = GetStatus(ptParse);
        if (code != 0) {
            return code;
        }
        
        boost::property_tree::wptree::const_iterator iter = ptParse.get_child(L"data").begin();
        if (iter != ptParse.get_child(L"data").end()) {
            GetTagsFromJson(iter->second, addedTags, updatedTags, deletedTags);
        }
        
        return UC_SUCCEEDED;
    } catch (boost::property_tree::ptree_error & e) {
    	UC_LOG(ERROR) << "encounter an error when parsing contact tags "
    		<< "from an json object, exception: " << e.what();

		std::list<ContactTag*>::iterator iter = addedTags.begin();
		std::list<ContactTag*>::iterator iterEnd = addedTags.end();
		for (; iter != iterEnd; ) {
			delete *iter;
			iter = addedTags.erase(iter);
		}

		iter = updatedTags.begin();
		iterEnd = updatedTags.end();
		for (; iter != iterEnd; ) {
			delete *iter;
			iter = updatedTags.erase(iter);
		}

		iter = deletedTags.begin();
		iterEnd = deletedTags.end();
		for (; iter != iterEnd; ) {
			delete *iter;
			iter = deletedTags.erase(iter);
		}

        return JSON_PARSE_ERROR;
    }
}
    
int32_t ContactResponseParser::ParseUpdateTags(const std::string &jsonBody) {
    return GetStatusEx(jsonBody);
}
    
void ContactResponseParser::GetTagsFromJson(const boost::property_tree::wptree &ptParse,
	std::list<ContactTag*>& addedTags, std::list<ContactTag*>& updatedTags, 
	std::list<ContactTag*>& deletedTags) {
    BOOST_FOREACH(const boost::property_tree::wptree::value_type &v, ptParse) {
        ContactTag *tag = new ContactTag();
        try {
        	tag->SetId(v.second.get<int32_t>(L"tag_id"));
			tag->SetName(StringConvert::ToUTF8Str(v.second.get<std::wstring>(L"tag_name", L"")));
			tag->SetValue(StringConvert::ToUTF8Str(v.second.get<std::wstring>(L"tag_value", L"")));
			tag->SetScope(ContactTag::Scope(
                v.second.get<int32_t>(L"tag_scope", ContactTag::CTS_DEFAULT)));
			tag->SetType(ContactTag::Type(
                v.second.get<int32_t>(L"tag_type", ContactTag::CTT_BASIC)));
			tag->SetUserId(v.second.get<int32_t>(L"user_id"));

            int32_t flag = v.second.get<int32_t>(L"tag_flag");
            if (flag == 1) {
				addedTags.push_back(tag);
			}
			else if (flag == 2) {
				updatedTags.push_back(tag);
			}
			else if (flag == 3) {
				deletedTags.push_back(tag);
			}
			else {
				delete tag;
				return;
			}
        } catch (boost::property_tree::ptree_error & e) {
        	UC_LOG(ERROR) << "encounter an error when getting tags "
    		<< "from an json object, exception: " << e.what();
            delete tag;
        }
    }
}
    
int32_t ContactResponseParser::GetContactListFromJson(const boost::property_tree::wptree &ptParse,
	std::list<ContactInfoImpl*>& contacts, bool isParseTag) {

	BOOST_FOREACH(const boost::property_tree::wptree::value_type &v, ptParse) {
		ContactInfoImpl* contactPtr = ContactInfoImpl::CreateContactInfoImpl(v.second,isParseTag);
		if (contactPtr == NULL) {
			return UC_FAILED;
		}

		contacts.push_back(contactPtr);
	}

	return UC_SUCCEEDED;
}

void ContactResponseParser::GetColleagueFromJson(const boost::property_tree::wptree &ptParse,
	ContactInfoImpl &contact) {

	contact.SetUserId(ptParse.get<int32_t>(L"user_id"));
	contact.SetDisplayName(StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"display_name", L"")));
	contact.SetRemoteAvatarUrl(StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"avatar", L"")));
	contact.SetPosition(StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"position", L"")));
	contact.SetDepartmentName(StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"department", L"")));
	contact.SetPersonalSign(StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"personal_sign", L"")));
}

int32_t ContactResponseParser::ParseSetUserStatus(const std::string &jsonBody, int32_t &status){
	int32_t result = UC_FAILED;
	rapidjson::Document document;
	if (!document.Parse<0>(jsonBody.c_str()).HasParseError()) {
		result = document["code"].GetInt();
// 		if (0 == result) {
// 			rapidjson::Value &data = document["data"];
// 			status = jsonhelper::getIntValue(data, "status", 1);
// 		}
	}

	return result;
}

int32_t ContactResponseParser::ParseUserStatus(const std::string &jsonBody){
	int32_t result = UC_FAILED;
	rapidjson::Document document;
	if (!document.Parse<0>(jsonBody.c_str()).HasParseError()) {
		result = document["code"].GetInt();
	}

	return result;
}


int32_t ContactResponseParser::ParseGetUserStatus(const std::string &jsonBody, std::list<UserStatus> &infos){
	int32_t result = UC_FAILED;
	rapidjson::Document document;
	if (!document.Parse<0>(jsonBody.c_str()).HasParseError()) {
		result = document["code"].GetInt();
		if (0 == result) {
			rapidjson::Value &data = document["data"];
			for (rapidjson::SizeType i = 0; i < data.Size(); i++) {
				if (jsonhelper::getIntValue(data[i], "user_id", 0) == 0){
					continue;
				}
				UserStatus info;
				info.SetUserId(jsonhelper::getIntValue(data[i], "user_id", 0));
				info.SetUserStatus(jsonhelper::getIntValue(data[i], "status", 1));
				info.AddRefCount();
				infos.push_back(info);
			}
		}
	}

	return result;
}

void ContactResponseParser::GetJsonPtree(const std::string &jsonBody,
	boost::property_tree::wptree &ptParse) {
	std::wstringstream jsonStream(StringConvert::FromUTF8Str(jsonBody));
  
	boost::property_tree::json_parser::read_json(jsonStream, ptParse);  
}

int32_t ContactResponseParser::GetStatus(const boost::property_tree::wptree &ptParse) {
    return ptParse.get<int32_t>(L"code");
}

int32_t ContactResponseParser::GetStatusEx(const std::string &jsonBody) {
	boost::property_tree::wptree ptParse;

	try {
		GetJsonPtree(jsonBody, ptParse);

        return GetStatus(ptParse);
	} catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an error when getting status "
    		<< "from an json object, exception: " << e.what();

        return JSON_PARSE_ERROR;
	}
}


}
