#include "ContactServiceStrategy.h"
#include "ServiceObjectConversion.h"
#include "UCDefs.h"

namespace ucclient {

ContactServiceStrategy::ContactServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient) 
	: _engine(engine), _cefClient(cefClient) {
	_contactService = (IContactService*)_engine->QueryService(UCContactService);
}

void ContactServiceStrategy::GetUpdatedContacts() {
	_contactService->GetUpdatedContacts();
}

bool ContactServiceStrategy::GetContactInfo(int32_t userId, ContactInfo& contact){
	return _contactService->GetContactInfo(userId, contact);
}

void ContactServiceStrategy::GetContactInfo(const std::list<int32_t> &userIds) {
	_contactService->GetContactInfo(userIds);
}
    
bool ContactServiceStrategy::GetBasicContactInfo(int32_t userId, BasicContactInfo &contact) {
	return _contactService->GetBasicContactInfo(userId, contact);
}

void ContactServiceStrategy::GetBasicContactInfo(const std::list<int32_t> &userIds) {
	_contactService->GetBasicContactInfo(userIds);
}

int32_t ContactServiceStrategy::GetContactList(unsigned int position, unsigned int len,
	std::vector<BasicContactInfo>& contact) {
	return _contactService->GetContactList(position, len, contact);
}

void ContactServiceStrategy::UpdateContactAvatar(int32_t userId, const std::wstring& remoteAvatarUrl,
	const std::wstring& localAvatarPath) {
	
	std::string utf8RemoteAvatarUrl = StringConvert::ToUTF8Str(remoteAvatarUrl);
	std::string utf8LocalAvatarPath = StringConvert::ToUTF8Str(localAvatarPath);
	
	_contactService->UpdateContactAvatar(userId, utf8RemoteAvatarUrl, utf8LocalAvatarPath);
}
    
void ContactServiceStrategy::AddCommonContact(int32_t userId) {
	_contactService->AddCommonContact(userId);
}

void ContactServiceStrategy::DeleteCommonContact(int32_t userId) {
	_contactService->DeleteCommonContact(userId);
}

int32_t ContactServiceStrategy::SearchLocalContact(ContactInfo::Type type, const std::wstring& keyword,
	std::list<ContactInfo>& contacts) {

		std::string utf8Keyword = StringConvert::ToUTF8Str(keyword);

	return _contactService->SearchLocalContact(type, utf8Keyword, contacts);
}

bool ContactServiceStrategy::GetCurrentUserInfo(ContactInfo &contact) {
	return _contactService->GetCurrentUserInfo(contact);
}

bool ContactServiceStrategy::GetCurrentUserFullInfo(ContactInfo &contact){
	return _contactService->GetCurrentUserFullInfo(contact);
}

void ContactServiceStrategy::UpdateUserInfo(const std::wstring& updateField) {
	std::string utf8UpdateField = StringConvert::ToUTF8Str(updateField);

	_contactService->UpdateUserInfo(utf8UpdateField);
}

void ContactServiceStrategy::UpdateUserTags(const std::wstring& updateTags) {

	std::string utf8UpdateTags = StringConvert::ToUTF8Str(updateTags);
	_contactService->UpdateUserTags(utf8UpdateTags);
}
    
void ContactServiceStrategy::GetColleagues(int32_t userId) {
	_contactService->GetColleagues(userId);
}

void ContactServiceStrategy::GetOrgAddressBook(int orgId, int32_t& taskId) {
	_contactService->GetOrgAddressBook(orgId, taskId);
}

void  ContactServiceStrategy::GetOrgMembers(int orgId, int limit, std::wstring copyback) {
	std::string utf8copyback = StringConvert::ToUTF8Str(copyback);
	_contactService->GetOrgMembers(orgId, limit, utf8copyback);
}


int32_t ContactServiceStrategy::SetUserStatus(int32_t userId, int32_t status, const std::wstring& show){
	std::string utf8Show = StringConvert::ToUTF8Str(show);
	return _contactService->SetUserStatus(userId, status, utf8Show);
}

int32_t ContactServiceStrategy::SubscribeUserStatus(std::list<int32_t>& userIds){
	return _contactService->SubscribeUserStatus(userIds);
}

int32_t ContactServiceStrategy::UnsubscribeUserStatus(std::list<int32_t>& userIds){
	return _contactService->UnsubscribeUserStatus(userIds);
}

int32_t ContactServiceStrategy::UnsubscribeAllUserStatus(){
	return _contactService->UnsubscribeAllUserStatus();
}

int32_t ContactServiceStrategy::GetUserStatus(const std::list<int32_t>& userIds, IN std::wstring & copyBack)
{
	std::string utf8CopyBack = StringConvert::ToUTF8Str(copyBack);
	return _contactService->GetUserStatus(userIds, utf8CopyBack);
}

void ContactServiceStrategy::OnContactLoaded(int32_t result) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_CONTACT_LOADED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << L"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnContactLoaded event to JS, result: " << result;
}

void ContactServiceStrategy::OnUserStatusReceived(int32_t result, const std::list<UserStatus> &infos){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_USER_STATUS_RECEIVED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << L", status: [";
		for (auto info : infos)	{
			jsScript << ServiceObjectConversion::ConvertUserStatusToJSON(info) << ",";
		}
		
	jsScript << L"]},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUserStatusReceived event to JS. ";
}

void ContactServiceStrategy::OnGetUserStatusResponse(int32_t result, const std::list<UserStatus> &infos, std::string &copyBack)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_GET_USER_STATUS_RESPONSE_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << L", status: [";
	for (auto info : infos)	{
		jsScript << ServiceObjectConversion::ConvertUserStatusToJSON(info) << ",";
	}

	jsScript << L"]";
	jsScript << ",copyBack: \"" << UTF2WJSON(copyBack) << "\"},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGetUserStatusResponse event to JS. ";
}

void ContactServiceStrategy::OnUserStatusSet(int32_t result, int32_t userId, int32_t status){

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_USER_STATUS_SET_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result <<", userId: " << userId << L", status : " 
		<< status << L"}, }); "
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUserStatusSet event to JS, userId: " << userId;
}

void ContactServiceStrategy::OnContactInfoReceived(int32_t result, const std::list<int32_t>* userIds,
	const std::list<ContactInfo*>* contacts) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << CONTACT_SERVICE_CONTACT_INFO_RECEIVED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << L", userIds: [" ;
	
	if (userIds != NULL) {
		for (std::list<int32_t>::const_iterator iter = userIds->begin(); 
			iter != userIds->end(); ++iter) {
			jsScript << *iter << L",";
		}
	}
	
	jsScript << L"], contacts: [";
	if (contacts != NULL) {
		for (std::list<ContactInfo*>::const_iterator iter = contacts->begin(); iter != contacts->end(); ++iter) {
			if (iter != contacts->begin())
				jsScript << L", ";

			jsScript << ServiceObjectConversion::ConvertContactToJSON(**iter);
		}
	}
	jsScript << L"]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnContactInfoReceived event to JS";
}

void ContactServiceStrategy::OnBasicContactInfoReceived(int32_t result, const std::list<int32_t>* userIds,
    const std::list<BasicContactInfo>* contacts) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_BASIC_CONTACT_INFO_RECEIVED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << L", userIds: [" ;
	
	if (userIds != NULL) {
		for (std::list<int32_t>::const_iterator iter = userIds->begin(); iter != userIds->end(); ++iter) {
			if (iter != userIds->begin())
				jsScript << L",";
			jsScript << *iter;
		}
	}

	jsScript << L"], contacts: [";
	if (contacts != NULL) {
		for (std::list<BasicContactInfo>::const_iterator iter = contacts->begin(); iter != contacts->end(); ++iter) {
			if (iter != contacts->begin())
				jsScript << L",";
			jsScript << ServiceObjectConversion::ConvertBasicContactInfoToJSON(*iter);
		}
	}
	jsScript << L"]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnBasicContactInfoReceived event to JS";
}

void ContactServiceStrategy::OnContactAdded(int32_t result, int32_t userId) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_CONTACT_ADDED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << L", userId: "<< userId << L"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnContactAdded event to JS";
}

void ContactServiceStrategy::OnContactDeleted(int32_t result, int32_t userId) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_CONTACT_DELETED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << L", userId: "<< userId << L"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnContactDeleted event to JS";
}

void ContactServiceStrategy::OnContactUpdated(std::list<ContactInfo*> *contacts) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << CONTACT_SERVICE_CONTACT_UPDATED_EVENT
		<< L"\",{detail:" << L"{contacts: [";

	if (contacts != NULL) {
		for (std::list<ContactInfo*>::const_iterator iter = contacts->begin(); iter != contacts->end(); ++iter) {
			if (iter != contacts->begin())
				jsScript << ",";
			jsScript << ServiceObjectConversion::ConvertContactToJSON(**iter);
		}
	}

	jsScript << L"]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnContactUpdated event to JS";
}
    
void ContactServiceStrategy::OnUserInfoUpdated(int32_t result, const ContactInfo *info) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << CONTACT_SERVICE_USER_INFO_UPDATED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result <<  L", ";

	if (info != NULL) {
		jsScript << L"userInfo: " << ServiceObjectConversion::ConvertContactToJSON(*info);
	}

	jsScript << L"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUserInfoUpdated event to JS";
}
    
void ContactServiceStrategy::OnUserTagsUpdated(int32_t result, const std::string &updateTags) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_USER_TAGS_UPDATED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << ", "
		<< L"updateTags: \"" << UTF2WJSON(updateTags) << "\", ";
	jsScript << L"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUserTagsUpdated event to JS";
}

void ContactServiceStrategy::OnSystemTagsAdded(std::vector<ContactTag*>* tags) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_SYSTEM_TAGS_ADDED_EVENT
		<< L"\",{detail:" << L"{tags: [";

	if (tags != NULL) {
		for (std::vector<ContactTag*>::const_iterator iter = tags->begin(); iter != tags->end(); ++iter) {
			if (iter != tags->begin())
				jsScript << L",";
			jsScript << ServiceObjectConversion::ConvertContactTagToJSON(**iter);
		}
	}

	jsScript << L"]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnSystemTagsAdded event to JS";
}
    
void ContactServiceStrategy::OnSystemTagsUpdated(std::vector<ContactTag*>* tags) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_SYSTEM_TAGS_UPDATED_EVENT
		<< L"\",{detail:" << L"{tags: [";

	if (tags != NULL) {
		for (std::vector<ContactTag*>::const_iterator iter = tags->begin(); iter != tags->end(); ++iter) {
			if (iter != tags->begin())
				jsScript << L",";
			jsScript << ServiceObjectConversion::ConvertContactTagToJSON(**iter);
		}
	}

	jsScript << L"]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnSystemTagsUpdated event to JS";
}
    
void ContactServiceStrategy::OnSystemTagsDeleted(std::vector<int32_t>& tagIds) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_SYSTEM_TAGS_DELETED_EVENT
		<< L"\",{detail:" << L"{tagIds: [";

	if (!tagIds.empty()) {
		for (int i = 0; i < tagIds.size(); ++i) {
			jsScript << tagIds[i] << L",";
		}
	}

	jsScript << L"]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnSystemTagsDeleted event to JS";
}

void ContactServiceStrategy::OnColleagueReceived(int32_t result, int32_t userId, int32_t page,
	int32_t colNum, int32_t leaderNum, std::list<ContactInfo*> *colleagues,
	std::list<ContactInfo*> *leaders) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_COLLEAGUE_RECEIVED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << L", userId:" << userId
		<< L", page:" << page << L", colleagueNumber:" << colNum <<  L", colleagues: [";

	if (colleagues != NULL) {
		for (std::list<ContactInfo*>::const_iterator iter = colleagues->begin(); iter != colleagues->end(); ++iter) {
			if (iter != colleagues->begin())
				jsScript << L",";
			jsScript << ServiceObjectConversion::ConvertContactToJSON(**iter);
		}
	}

	jsScript << L"], leaders: [";
	if (leaders != NULL) {
		for (std::list<ContactInfo*>::const_iterator iter = leaders->begin(); iter != leaders->end(); ++iter) {
			if (iter != leaders->begin())
				jsScript << L",";
			jsScript << ServiceObjectConversion::ConvertContactToJSON(**iter);
		}
	}

	jsScript << L"]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnColleagueReceived event to JS";
}
	
void ContactServiceStrategy::OnOrgAddressBookReceived(int32_t result, DepartmentInfo *department,
	DepartmentInfo *myDepartment, int32_t taskId) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_ORG_ADDRESS_BOOK_RECEIVED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << L", taskId: " << taskId
		<< L", ";

	if (department != NULL) {
		jsScript << L"department: " << ServiceObjectConversion::ConvertDepartmentInfoToJSON(*department) << L", ";
	}

	if (myDepartment != NULL) {
		jsScript << L"myDepartment: " << ServiceObjectConversion::ConvertDepartmentInfoToJSON(*myDepartment) << L", ";
	}

	jsScript << L"},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnOrgAddressBookReceived event to JS";
}

void ContactServiceStrategy::OnOrgAddrMembersReceived(int32_t result, int orgId, int userCount, 
	std::vector<int32_t> *memberIds, std::string& copyback) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_ORG_MEMBER_RECEIVED_EVENT
		<< L"\",{detail:" << L"{errorCode: " << result << L", orgId:" << orgId 
		<< L", userCount:" << userCount << L", memberIds: ";

	if (memberIds != NULL) {
		jsScript << L"[";
		for(int i = 0; i < memberIds->size(); ++i) {
			jsScript << (*memberIds)[i] << L",";
		}
		jsScript << L"]";
	}

	jsScript << L",copyBack: \"" << UTF2WJSON(copyback) 
		<< L"\"},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());
	
}

void ContactServiceStrategy::OnDeptUpdated(int32_t operatorId, int32_t deptId, 
    const std::string &oldDeptName, const std::string &newDeptName, 
    const std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_DEPT_UPDATED_EVENT
		<< L"\",{detail:" << L"{operatorId: " << operatorId
		<< L", departmentId: " << deptId
		<< L", timestamp: " << timestamp
		<< L", oldDepartmentName: \"" << UTF2WJSON(oldDeptName)
		<< L"\",newDepartmentName: \"" << UTF2WJSON(newDeptName)
		<< L"\", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDeptUpdated event to JS";
}
    
void ContactServiceStrategy::OnDeptTransfered(int32_t operatorId, int32_t userId, 
    const std::string &userName, int32_t deptId, const std::string &oldDeptName,
    const std::string &deptName, const std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_DEPT_TRANSFERED_EVENT
		<< L"\",{detail:" << L"{operatorId: " << operatorId 
		<< L", timestamp: " << timestamp
		<< L", userId: " << userId 
		<< L", userName: \"" << UTF2WJSON(userName) << L"\", departmentId: " << deptId
		<< L",oldDeptmentName: \"" << UTF2WJSON(oldDeptName)
		<< L"\", departmentName: \"" << UTF2WJSON(deptName)
		<< L"\", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDeptTransfered event to JS";
}
    
void ContactServiceStrategy::OnPositionUpdated(int32_t operatorId, int32_t userId, 
    const std::string &userName, const std::string &newPosition, 
    const std::string &oldPosition, const std::string& deptName, 
    const std::string& desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_POSITION_UPDATED_EVENT
		<< L"\",{detail:" << L"{operatorId: " << operatorId 
		<< L", timestamp: " << timestamp
		<< L", userId: " << userId << L",userName: \"" << UTF2WJSON(userName)
		<< L"\",newPosition: \"" << UTF2WJSON(newPosition)
		<< L"\",oldPosition: \"" << UTF2WJSON(oldPosition)
		<< L"\", departmentName: \"" << UTF2WJSON(deptName)
		<< L"\", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnPositionUpdated event to JS";
}
    
void ContactServiceStrategy::OnEmployeeEntry(int32_t operatorId, int32_t userId, int32_t deptId, 
    const std::string &userName, const std::string &deptName,  
    const std::string &position, const std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_EMPLOYEE_ENTRY_EVENT
		<< L"\",{detail:" << "{operatorId: " << operatorId 
		<< L", timestamp: " << timestamp
		<< L", userId: " << userId 
		<< L", departmentId: " << deptId
		<< L",userName: \"" << UTF2WJSON(userName)
		<< L"\", departmentName: \"" << UTF2WJSON(deptName)
		<< L"\",position: \"" << UTF2WJSON(position)
		<< L"\", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnEmployeeEntry event to JS";
}
    
void ContactServiceStrategy::OnEmployeeLeaved(int32_t operatorId, int32_t userId,
    const std::string &userName, const std::string &deptName, 
    const std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_EMPLOYEE_LEAVED_EVENT
		<< L"\",{detail:" << L"{operatorId: " << operatorId 
		<< L", timestamp: " << timestamp
		<< L", userId: " << userId << L",userName: \"" << UTF2WJSON(userName)
		<< L"\", departmentName: \"" << UTF2WJSON(deptName)
		<< L"\", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnEmployeeLeaved event to JS";
}
    
void ContactServiceStrategy::OnEmployeeRightsUpdated(int32_t operatorId, int32_t userId, 
    const std::string &userName, const std::string &deptName, int32_t deptId, 
    const std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_EMPLOYEE_RIGHTS_UPDATED_EVENT
		<< L"\",{detail:" << L"{operatorId: " << operatorId << L", userId: " << userId 
		<< L", timestamp: " << timestamp
		<< L",userName: \"" << UTF2WJSON(userName) << L"\", departmentName: \" " << UTF2WJSON(deptName)
		<< L"\", departmentId: " << deptId << L", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnEmployeeRightsUpdated event to JS";
}
    
void ContactServiceStrategy::OnDeptParentChanged(int32_t operatorId, int32_t deptId, 
    const std::string &deptName, const std::string &oldDeptName, 
    const std::string &newDeptName, const std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << CONTACT_SERVICE_DEPT_PARENT_CHANGED_EVENT
		<< L"\",{detail:" << L"{operatorId: " << operatorId << L", departmentId: "<< deptId << L", timestamp: " << timestamp
		<< L", departmentName: \"" << UTF2WJSON(deptName) << L"\", oldDepartmentName: \"" << UTF2WJSON(oldDeptName)
		<< L"\",newDepartmentName: \"" << UTF2WJSON(newDeptName)
		<< L"\", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDeptParentChanged event to JS";
}
    
void ContactServiceStrategy::OnCompanyJoined(int32_t operatorId, int32_t companyId, 
	int32_t userId, const std::string &companyName, const std::string &userName, 
    const std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_COMPANY_JOINED_EVENT
		<< L"\",{detail:" << L"{operatorId: " << operatorId << L", companyId: "<< companyId << L", timestamp: " << timestamp
		<< L", userId: " << userId << L",companyName: \"" << UTF2WJSON(companyName)
		<< L"\", userName: \"" << UTF2WJSON(userName) << L"\", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCompanyJoined event to JS";
}
    
void ContactServiceStrategy::OnDeptDeleted(int32_t operatorId, int32_t deptId, 
    const std::string &deptName, const std::string &parentDeptName, 
    const std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_DEPT_DELETED_EVENT
		<< L"\",{detail:" << L"{operatorId: " << operatorId << L", timestamp: " << timestamp
		<< L", departmentId: " << deptId << L",departmentName: \"" << UTF2WJSON(deptName)
		<< L"\", parentDepartmentName: \"" << UTF2WJSON(parentDeptName)
		<< L"\", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDeptDeleted event to JS";
}
    
void ContactServiceStrategy::OnEmployeeEntryConfirmed(int32_t operatorId, int32_t deptId, 
    const std::string &userName, const std::string &deptName, 
    int32_t userId, std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_EMPLOYEE_ENTRY_CONFIRMED_EVENT
		<< L"\",{detail:" << L"{operatorId: " << operatorId << L", departmentId: "<< deptId << L", timestamp: " << timestamp
		<< L", userName: \"" << UTF2WJSON(userName) << L"\",departmentName: \"" << UTF2WJSON(deptName)
		<< L"\", userId: " << userId << L", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnEmployeeEntryConfirmed event to JS";
}
    
void ContactServiceStrategy::OnEmployeeLeaveConfirmed(int32_t operatorId, int32_t userId, 
	int32_t deptId, const std::string &userName, const std::string &deptName, 
	const std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_EMPLOYEE_LEAVE_CONFIRMED_EVENT
		<< L"\",{detail:" << "{operatorId: " << operatorId << L", userId: "<< userId << L", timestamp: " << timestamp
		<< L", departmentId: " << deptId << L", userName: \"" << UTF2WJSON(userName)
		<< L"\",departmentName: \"" << UTF2WJSON(deptName)
		<< L"\", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnEmployeeLeaveConfirmed event to JS";
}
    
 void ContactServiceStrategy::OnDeptTransferConfirmed(int32_t operatorId, int32_t userId, 
    const std::string &userName, const std::string &deptName, 
    const std::string &desc, int64_t timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CONTACT_SERVICE_DEPT_TRANSFER_CONFIRMED_EVENT
		<< L"\",{detail:" << L"{operatorId: " << operatorId << L", userId: "<< userId << L", timestamp: " << timestamp
		<< L", userName: \"" << UTF2WJSON(userName) << L"\",departmentName: \"" << UTF2WJSON(deptName)
		<< L"\", description: \"" << UTF2WJSON(desc) << L"\"},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDeptTransferConfirmed event to JS";
 }

}