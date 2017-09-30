#include "ContactEventDispatcher.h"
#include "Macro.h"
#include "ContactService.h"
#include "common/CallOnMainThread.h"

namespace uc {

ContactEventDispatcher::ContactEventDispatcher(ContactService *contactService) 
	: _default(new DefaultContactEventDispatcher(contactService)) {
}

ContactEventDispatcher::~ContactEventDispatcher() {
	delete _default;
}

void ContactEventDispatcher::ContactLoaded(int32_t result) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::ContactLoaded,
		result);
}


void ContactEventDispatcher::ContactInfoReceived(int32_t result, std::list<int32_t>* userIds, 
	std::list<ContactInfo*> *contacts) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::ContactInfoReceived, result, 
		userIds, contacts);
}

void ContactEventDispatcher::BasicContactInfoReceived(int32_t result, std::list<int32_t>* userIds, 
		std::list<BasicContactInfo> *basicContacts) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::BasicContactInfoReceived, result, 
		userIds, basicContacts);
}

void ContactEventDispatcher::ContactUpdated(std::list<ContactInfo*> *contacts){
	CallOnMainThread(_default, &DefaultContactEventDispatcher::ContactUpdated, contacts);
}
    
void ContactEventDispatcher::UserInfoUpdated(int32_t result,
		ContactInfo* info) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::UserInfoUpdated, result, info);
}

void ContactEventDispatcher::UserTagsUpdated(int32_t result, std::string userTags) {

	CallOnMainThread(_default, &DefaultContactEventDispatcher::UserTagsUpdated, result, userTags);
}

void ContactEventDispatcher::SystemTagsAdded(std::vector<ContactTag*>* tags) {
    CallOnMainThread(_default, &DefaultContactEventDispatcher::SystemTagsAdded,tags);
}

void ContactEventDispatcher::SystemTagsUpdated(std::vector<ContactTag*>* tags) {
   CallOnMainThread(_default, &DefaultContactEventDispatcher::SystemTagsUpdated,tags);
}
void ContactEventDispatcher::SystemTagsDeleted(std::vector<int32_t> *tagIds) {
    CallOnMainThread(_default, &DefaultContactEventDispatcher::SystemTagsDeleted,tagIds);
}
                                                  
void ContactEventDispatcher::ContactAdded(int32_t result, int32_t userId) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::ContactAdded, result,  userId);
}

void ContactEventDispatcher::ContactDeleted(int32_t result, int32_t userId) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::ContactDeleted, result, userId);
}

void ContactEventDispatcher::ColleagueReceived(int32_t result, int32_t userId, int32_t page,
	int32_t colNum, int32_t leaderNum, std::list<ContactInfo*> *colleagues, 
	std::list<ContactInfo*> *leaders) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::ColleagueReceived,
		result, userId, page, colNum, leaderNum, colleagues,leaders);
}

void ContactEventDispatcher::OrgAddressBookReceived(int32_t result, DepartmentInfo *depart,
	DepartmentInfo *myDepart, int32_t taskId) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::OrgAddressBookReceived, result, 
		depart, myDepart, taskId);
}

void ContactEventDispatcher::OrgAddrMembersReceived(int32_t result, int orgId, int userCount, 
	std::vector<int32_t> *memberIds, std::string copyback) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::OrgAddrMembersReceived, result, 
		orgId, userCount, memberIds,copyback);
}

void ContactEventDispatcher::DeptUpdated(int32_t operatorId, int32_t deptId, 
	std::string* oldDeptName, std::string* newDeptName, std::string* desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::DeptUpdated, operatorId, deptId, 
		oldDeptName, newDeptName, desc, timestamp);
}

void ContactEventDispatcher::DeptTransfered(int32_t operatorId, int32_t userId, 
	std::string *userName, int32_t deptId, std::string *oldDeptName,
	std::string *deptName, std::string *desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::DeptTransfered, operatorId, 
		userId, userName, deptId, oldDeptName, deptName, desc, timestamp); 
}

void ContactEventDispatcher::PositionUpdated(int32_t operatorId, int32_t userId,
	std::string* userName, std::string* newPosition, std::string* oldPosition,
	std::string* deptName, std::string* desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::PositionUpdated, operatorId, 
		userId, userName, newPosition, oldPosition, deptName, desc, timestamp);
}
    
void ContactEventDispatcher::EmployeeEntry(int32_t operatorId, int32_t userId,
	int32_t deptId, std::string* userName, std::string* deptName,
	std::string* position, std::string* desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::EmployeeEntry, operatorId, userId, deptId,
    	userName, deptName, position, desc, timestamp);
}
    
void ContactEventDispatcher::EmployeeLeaved(int32_t operatorId, int32_t userId,
	std::string* userName, std::string *deptName, std::string *desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::EmployeeLeaved, operatorId, userId,
        userName, deptName, desc, timestamp);
}
    
void ContactEventDispatcher::EmployeeRightsUpdated(int32_t operatorId, int32_t userId,
	std::string *userName, std::string *deptName, int32_t deptId, std::string *desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::EmployeeRightsUpdated, 
		operatorId, userId, userName, deptName, deptId, desc, timestamp);
}

void ContactEventDispatcher::DeptParentChanged(int32_t operatorId, int32_t deptId,
	std::string *deptName, std::string *oldDeptName, std::string *newDeptName,
    std::string *desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::DeptParentChanged, 
		operatorId, deptId, deptName, oldDeptName, newDeptName, desc, timestamp);
}

void ContactEventDispatcher::CompanyJoined( int32_t operatorId, int32_t companyId,
	int32_t userId, std::string *companyName, std::string *userName, std::string *desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::CompanyJoined, operatorId, 
		companyId, userId, companyName, userName, desc, timestamp);
}
    
void ContactEventDispatcher::DeptDeleted(int32_t operatorId, int32_t deptId, 
	std::string *deptName, std::string *parentDeptName, std::string *desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::DeptDeleted, operatorId, 
		deptId, deptName, parentDeptName, desc, timestamp);
}

void ContactEventDispatcher::EmployeeEntryConfirmed(int32_t operatorId, int32_t deptId,
	std::string *userName, std::string *deptName, int32_t userId, std::string *desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::EmployeeEntryConfirmed, 
		operatorId, deptId, userName, deptName, userId, desc, timestamp);
}

void ContactEventDispatcher::EmployeeLeaveConfirmed(int32_t operatorId, int32_t userId,
    int32_t deptId, std::string *userName, std::string *deptName, std::string *desc, int64_t timestamp) {
    CallOnMainThread(_default, &DefaultContactEventDispatcher::EmployeeLeaveConfirmed, operatorId, 
		userId, deptId, userName, deptName, desc, timestamp);
}
    
void ContactEventDispatcher::DeptTransferConfirmed(int32_t operatorId, int32_t userId,
	std::string *userName, std::string *deptName, std::string *desc, int64_t timestamp) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::DeptTransferConfirmed, operatorId, userId, 
        userName, deptName, desc, timestamp);
}

void ContactEventDispatcher::UserStatusSet(int32_t result, int32_t userId, int32_t status){
	CallOnMainThread(_default, &DefaultContactEventDispatcher::UserStatusSet, result,
		userId, status);
}

void ContactEventDispatcher::UserStatusReceived(int32_t result, std::list<UserStatus> infos) {
	CallOnMainThread(_default, &DefaultContactEventDispatcher::UserStatusReceived, result, 
		infos);
}

void ContactEventDispatcher::UserGetStatusResponse(int32_t result, std::list<UserStatus> infos, std::string copyBack){
	CallOnMainThread(_default, &DefaultContactEventDispatcher::UserGetStatusResponse, result, infos, copyBack);
}

}

