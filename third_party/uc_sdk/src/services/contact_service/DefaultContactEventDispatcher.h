/*
 * UccRequestFactory.h
 *
 *  Created on: May 28, 2015
 *      Author: gang wang
 */

#ifndef DEFAULT_CONTACT_EVENT_DISPATCHER_H
#define DEFAULT_CONTACT_EVENT_DISPATCHER_H

#include <list>

#include "ContactInfo.h"
#include "DepartmentInfo.h"

namespace uc {

class ContactService;
    
/**
 * 默认在调用线程触发Contactserivce事件
 */  
class DefaultContactEventDispatcher{
public:
	DefaultContactEventDispatcher(ContactService *contactService);

	void ContactLoaded(int32_t result);



	void ContactInfoReceived(int32_t result, std::list<int32_t>* userIds, 
		std::list<ContactInfo*> *contacts);

	void BasicContactInfoReceived(int32_t result, std::list<int32_t>* userIds, 
		std::list<BasicContactInfo> *basicContacts);

	void ContactUpdated(std::list<ContactInfo*> *contacts);

	void UserInfoUpdated(int32_t result, ContactInfo *info);

	void UserTagsUpdated(int32_t result, std::string userTags);

    void SystemTagsAdded(std::vector<ContactTag*>* tags);

    void SystemTagsUpdated(std::vector<ContactTag*>* tags);

    void SystemTagsDeleted(std::vector<int32_t> *tagIds);
	
	void ContactAdded(int32_t result, int32_t userId);
	
	void ContactDeleted(int32_t result, int32_t userId);

	void ColleagueReceived(int32_t result, int32_t userId, int32_t page, int32_t colNum,
		int32_t leaderNum, std::list<ContactInfo*> *colleagues, 
		std::list<ContactInfo*> *leaders);
	
	void OrgAddressBookReceived(int32_t result, DepartmentInfo *depart, 
		DepartmentInfo *myDeparts, int32_t taskId);
    
    void OrgAddrMembersReceived(int32_t result, int orgId, int userCount, 
		std::vector<int32_t> *memberIds, std::string copyback);
    
    void DeptUpdated(int32_t operatorId, int32_t deptId, std::string* oldDeptName,
    	std::string* newDeptName, std::string* desc, int64_t timestamp);
    
    void DeptTransfered(int32_t operatorId, int32_t userId, std::string *userName,
    	int32_t deptId, std::string *oldDeptName, std::string *deptName,
    	std::string *desc, int64_t timestamp);
	
	void PositionUpdated(int32_t operatorId, int32_t userId, std::string* userName,
		std::string* newPosition, std::string* oldPosition, std::string* deptName,
		std::string* desc, int64_t timestamp);
	
	void EmployeeEntry(int32_t operatorId, int32_t userId, int32_t deptId, 
		std::string* userName, std::string* deptName,  std::string* position,
		std::string* desc, int64_t timestamp);
    
    void EmployeeLeaved(int32_t operatorId, int32_t userId, 
    	std::string* userName, std::string *deptName, std::string *desc, int64_t timestamp);
    
    void EmployeeRightsUpdated(int32_t operatorId, int32_t userId,
		std::string *userName, std::string *deptName, int32_t deptId, std::string *desc, int64_t timestamp);
    
    void DeptParentChanged(int32_t operatorId, int32_t deptId,
		std::string *deptName, std::string *oldDeptName,std::string *newDeptName,
		std::string *desc, int64_t timestamp);
    
    void CompanyJoined( int32_t operatorId, int32_t company_id,
		int32_t userId, std::string *companyName, std::string *userName,
		std::string *desc, int64_t timestamp);

    void DeptDeleted(int32_t operatorId, int32_t deptId, 
		std::string *deptName, std::string *parentDeptName, std::string *desc, int64_t timestamp);

    void EmployeeEntryConfirmed(int32_t operatorId, int32_t deptId,
		std::string *userName, std::string *deptName, int32_t userId,
		std::string *desc, int64_t timestamp);

    void EmployeeLeaveConfirmed(int32_t operatorId, int32_t userId,
		int32_t deptId, std::string *userName, std::string *deptName,
		std::string *desc, int64_t timestamp);

    void DeptTransferConfirmed(int32_t operatorId, int32_t userId,
		std::string *userName, std::string *deptName, std::string *desc, int64_t timestamp);

	void UserStatusSet(int32_t result, int32_t userId, int32_t status);

	void UserStatusReceived(int32_t result, std::list<UserStatus> infos);

	void UserGetStatusResponse(int32_t result, std::list<UserStatus> infos, std::string copyBack);

private:
    ContactService *_contactService;
};
    
}

#endif
