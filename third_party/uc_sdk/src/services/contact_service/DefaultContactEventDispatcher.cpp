#include "DefaultContactEventDispatcher.h"
#include "Macro.h"
#include "ContactService.h"

#include "LogManager.h"

namespace uc {

	DefaultContactEventDispatcher::DefaultContactEventDispatcher(ContactService *contactService)
		: _contactService(contactService) {
	}

	void DefaultContactEventDispatcher::ContactLoaded(int32_t result) {
		UC_LOG(WARNING) << "dispatches ContactLoaded, result: " << result;

		triggerEventEx(IContactServiceEvent, *_contactService, OnContactLoaded(result));
	}

	void DefaultContactEventDispatcher::ContactInfoReceived(int32_t result,
		std::list<int32_t>* userIds, std::list<ContactInfo*> *contacts) {

		UC_LOG(WARNING) << "dispatches ContactInfoReceived, result: " << result;

		triggerEventEx(IContactServiceEvent, *_contactService,
			OnContactInfoReceived(result, userIds, contacts));

		SAFE_DELETE(userIds);
		SAFE_DELETE_CONTAINER(std::list<ContactInfo*>, contacts);
	}

	void DefaultContactEventDispatcher::BasicContactInfoReceived(int32_t result, std::list<int32_t>* userIds,
		std::list<BasicContactInfo> *basicContacts) {
		UC_LOG(WARNING) << "dispatches BasicContactInfoReceived, result: " << result;

		triggerEventEx(IContactServiceEvent, *_contactService,
			OnBasicContactInfoReceived(result, userIds, basicContacts));

		SAFE_DELETE(userIds);
		SAFE_DELETE(basicContacts);
	}

	void DefaultContactEventDispatcher::ContactUpdated(std::list<ContactInfo*> *contacts){
		UC_LOG(WARNING) << "dispatches ContactUpdated";

		triggerEventEx(IContactServiceEvent, *_contactService, OnContactUpdated(contacts));

		SAFE_DELETE_CONTAINER(std::list<ContactInfo*>, contacts);
	}

	void DefaultContactEventDispatcher::UserInfoUpdated(int32_t result,
		ContactInfo* info) {
		UC_LOG(WARNING) << "dispatches UserInfoUpdated, result: " << result;

		triggerEventEx(IContactServiceEvent, *_contactService, OnUserInfoUpdated(result, info));

		SAFE_DELETE(info);
	}

	void DefaultContactEventDispatcher::UserTagsUpdated(int32_t result, std::string userTags) {
		UC_LOG(WARNING) << "dispatches UserTagsUpdated, result: " << result;

		triggerEventEx(IContactServiceEvent, *_contactService, OnUserTagsUpdated(result, userTags));

	}

	void DefaultContactEventDispatcher::SystemTagsAdded(std::vector<ContactTag*>* tags) {
		UC_LOG(WARNING) << "dispatches SystemTagsAdded";

		triggerEventEx(IContactServiceEvent, *_contactService, OnSystemTagsAdded(tags));

		SAFE_DELETE_CONTAINER(std::vector<ContactTag*>, tags);
	}

	void DefaultContactEventDispatcher::SystemTagsUpdated(std::vector<ContactTag*>* tags) {
		UC_LOG(WARNING) << "dispatches SystemTagsUpdated";

		triggerEventEx(IContactServiceEvent, *_contactService, OnSystemTagsUpdated(tags));

		SAFE_DELETE_CONTAINER(std::vector<ContactTag*>, tags);

	}
	void DefaultContactEventDispatcher::SystemTagsDeleted(std::vector<int32_t> *tagIds) {
		UC_LOG(WARNING) << "dispatches SystemTagsDeleted";

		triggerEventEx(IContactServiceEvent, *_contactService, OnSystemTagsDeleted(*tagIds));

		SAFE_DELETE(tagIds);
	}

	void DefaultContactEventDispatcher::ContactAdded(int32_t result, int32_t userId) {
		UC_LOG(WARNING) << "dispatches ContactAdded, result: " << result
			<< ", userId" << userId;

		triggerEventEx(IContactServiceEvent, *_contactService, OnContactAdded(result, userId));
	}

	void DefaultContactEventDispatcher::ContactDeleted(int32_t result, int32_t userId) {
		UC_LOG(WARNING) << "dispatches ContactDeleted, result: " << result
			<< ", userId" << userId;
		triggerEventEx(IContactServiceEvent, *_contactService, OnContactDeleted(result, userId));
	}

	void DefaultContactEventDispatcher::ColleagueReceived(int32_t result, int32_t userId, int32_t page,
		int32_t colNum, int32_t leaderNum, std::list<ContactInfo*> *colleagues,
		std::list<ContactInfo*> *leaders) {
		UC_LOG(WARNING) << "dispatches ColleagueReceived, result: " << result
			<< ", userId" << userId << ", page: " << page
			<< ", colNum: " << colNum << ", leaderNum: " << leaderNum;

		triggerEventEx(IContactServiceEvent, *_contactService, OnColleagueReceived(result, userId, page,
			colNum, leaderNum, colleagues, leaders));

		SAFE_DELETE_CONTAINER(std::list<ContactInfo*>, colleagues);
		SAFE_DELETE_CONTAINER(std::list<ContactInfo*>, leaders);
	}

	void DefaultContactEventDispatcher::OrgAddressBookReceived(int32_t result, DepartmentInfo *depart,
		DepartmentInfo *myDeparts, int32_t taskId) {
		UC_LOG(WARNING) << "dispatches OrgAddressBookReceived, result: " << result;
		triggerEventEx(IContactServiceEvent, *_contactService, OnOrgAddressBookReceived(result, depart, myDeparts, taskId));

		SAFE_DELETE(depart);
		SAFE_DELETE(myDeparts);
	}

	void DefaultContactEventDispatcher::OrgAddrMembersReceived(int32_t result, int orgId, int userCount,
		std::vector<int32_t> *memberIds, std::string copyback) {
		UC_LOG(WARNING) << "dispatches OrgAddrMembersReceived, result: " << result << ", orgId" << orgId
			<< ", userCount" << userCount;
		triggerEventEx(IContactServiceEvent, *_contactService, OnOrgAddrMembersReceived(result, orgId, userCount, memberIds,copyback));

		SAFE_DELETE(memberIds);
	}

	void DefaultContactEventDispatcher::DeptUpdated(int32_t operatorId, int32_t deptId,
		std::string* oldDeptName, std::string* newDeptName, std::string* desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches DeptUpdated, operatorId: " << operatorId
			<< ", deptId: " << deptId;
		triggerEventEx(IContactServiceEvent, *_contactService, OnDeptUpdated(operatorId, deptId, *oldDeptName,
			*newDeptName, *desc, timestamp));

		SAFE_DELETE(desc);
		SAFE_DELETE(oldDeptName);
		SAFE_DELETE(newDeptName);
	}

	void DefaultContactEventDispatcher::DeptTransfered(int32_t operatorId, int32_t userId,
		std::string *userName, int32_t deptId, std::string *oldDeptName,
		std::string *deptName, std::string *desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches DeptTransfered, operatorId: " << operatorId
			<< ", userId: " << userId;
		triggerEventEx(IContactServiceEvent, *_contactService,
			OnDeptTransfered(operatorId, userId, *userName, deptId, *oldDeptName, *deptName, *desc, timestamp));

		SAFE_DELETE(userName);
		SAFE_DELETE(oldDeptName);
		SAFE_DELETE(deptName);
		SAFE_DELETE(desc);
	}

	void DefaultContactEventDispatcher::PositionUpdated(int32_t operatorId, int32_t userId,
		std::string* userName, std::string* newPosition, std::string* oldPosition,
		std::string* deptName, std::string* desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches EmployeeEntryConfirmed, operatorId: " << operatorId
			<< ", userId: " << userId;

		triggerEventEx(IContactServiceEvent, *_contactService,
			OnPositionUpdated(operatorId, userId, *userName, *newPosition, *oldPosition, *deptName,
			*desc, timestamp));

		SAFE_DELETE(userName);
		SAFE_DELETE(newPosition);
		SAFE_DELETE(oldPosition);
		SAFE_DELETE(desc);
		SAFE_DELETE(deptName);
	}

	void DefaultContactEventDispatcher::EmployeeEntry(int32_t operatorId, int32_t userId,
		int32_t deptId, std::string* userName, std::string* deptName,
		std::string* position, std::string* desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches EmployeeEntry, operatorId: " << operatorId
			<< ", userId: " << userId;

		triggerEventEx(IContactServiceEvent, *_contactService, OnEmployeeEntry(operatorId, userId, deptId,
			*userName, *deptName, *position, *desc, timestamp));

		SAFE_DELETE(userName);
		SAFE_DELETE(deptName);
		SAFE_DELETE(position);
		SAFE_DELETE(desc);
	}

	void DefaultContactEventDispatcher::EmployeeLeaved(int32_t operatorId, int32_t userId,
		std::string* userName, std::string *deptName, std::string *desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches EmployeeLeaved, operatorId: " << operatorId
			<< ", userId: " << userId;

		triggerEventEx(IContactServiceEvent, *_contactService, OnEmployeeLeaved(operatorId, userId,
			*userName, *deptName, *desc, timestamp));

		SAFE_DELETE(userName);
		SAFE_DELETE(deptName);
		SAFE_DELETE(desc);
	}

	void DefaultContactEventDispatcher::EmployeeRightsUpdated(int32_t operatorId, int32_t userId,
		std::string *userName, std::string *deptName, int32_t deptId, std::string *desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches EmployeeRightsUpdated, operatorId: " << operatorId
			<< ", userId: " << userId;
		triggerEventEx(IContactServiceEvent, *_contactService,
			OnEmployeeRightsUpdated(operatorId, userId, *userName, *deptName, deptId, *desc, timestamp));

		SAFE_DELETE(userName);
		SAFE_DELETE(deptName);
		SAFE_DELETE(desc);
	}

	void DefaultContactEventDispatcher::DeptParentChanged(int32_t operatorId, int32_t deptId,
		std::string *deptName, std::string *oldDeptName, std::string *newDeptName,
		std::string *desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches EmployeeRightsUpdated, operatorId: " << operatorId
			<< ", deptId: " << deptId;
		triggerEventEx(IContactServiceEvent, *_contactService,
			OnDeptParentChanged(operatorId, deptId, *deptName, *oldDeptName, *newDeptName, *desc, timestamp));

		SAFE_DELETE(deptName);
		SAFE_DELETE(oldDeptName);
		SAFE_DELETE(newDeptName);
		SAFE_DELETE(desc);
	}

	void DefaultContactEventDispatcher::CompanyJoined(int32_t operatorId, int32_t companyId,
		int32_t userId, std::string *companyName, std::string *userName, std::string *desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches CompanyJoined, operatorId: " << operatorId
			<< ", companyId: " << companyId << ", userId: " << userId;
		triggerEventEx(IContactServiceEvent, *_contactService,
			OnCompanyJoined(operatorId, companyId, userId, *companyName, *userName, *desc, timestamp));

		SAFE_DELETE(userName);
		SAFE_DELETE(companyName);
		SAFE_DELETE(desc);
	}

	void DefaultContactEventDispatcher::DeptDeleted(int32_t operatorId, int32_t deptId,
		std::string *deptName, std::string *parentDeptName, std::string *desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches DeptDeleted, operatorId: " << operatorId
			<< ", deptId: " << deptId;
		triggerEventEx(IContactServiceEvent, *_contactService,
			OnDeptDeleted(operatorId, deptId, *deptName, *parentDeptName, *desc, timestamp));

		SAFE_DELETE(parentDeptName);
		SAFE_DELETE(deptName);
		SAFE_DELETE(desc);
	}

	void DefaultContactEventDispatcher::EmployeeEntryConfirmed(int32_t operatorId, int32_t deptId,
		std::string *userName, std::string *deptName, int32_t userId, std::string *desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches EmployeeEntryConfirmed, operatorId: " << operatorId
			<< ", deptId: " << deptId;

		triggerEventEx(IContactServiceEvent, *_contactService, OnEmployeeEntryConfirmed(operatorId, deptId,
			*userName, *deptName, userId, *desc, timestamp));

		SAFE_DELETE(userName);
		SAFE_DELETE(deptName);
		SAFE_DELETE(desc);
	}

	void DefaultContactEventDispatcher::EmployeeLeaveConfirmed(int32_t operatorId, int32_t userId,
		int32_t deptId, std::string *userName, std::string *deptName, std::string *desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches EmployeeLeaveConfirmed, operatorId: " << operatorId
			<< ", userId: " << userId << ", deptId: " << deptId;
		triggerEventEx(IContactServiceEvent, *_contactService, OnEmployeeLeaveConfirmed(operatorId, userId, deptId,
			*userName, *deptName, *desc, timestamp));

		SAFE_DELETE(userName);
		SAFE_DELETE(deptName);
		SAFE_DELETE(desc);
	}

	void DefaultContactEventDispatcher::DeptTransferConfirmed(int32_t operatorId, int32_t userId,
		std::string *userName, std::string *deptName, std::string *desc, int64_t timestamp) {
		UC_LOG(WARNING) << "dispatches DeptTransferConfirmed, operatorId: " << operatorId
			<< ", userId: " << userId;

		triggerEventEx(IContactServiceEvent, *_contactService,
			OnDeptTransferConfirmed(operatorId, userId, *userName, *deptName, *desc, timestamp));

		SAFE_DELETE(userName);
		SAFE_DELETE(deptName);
		SAFE_DELETE(desc);
	}

	void DefaultContactEventDispatcher::UserStatusSet(int32_t result, int32_t userId, int32_t status){
		triggerEventEx(IContactServiceEvent, *_contactService, OnUserStatusSet(result, userId, status));
	}

	void DefaultContactEventDispatcher::UserStatusReceived(int32_t result, std::list<UserStatus> infos){
		triggerEventEx(IContactServiceEvent, *_contactService, OnUserStatusReceived(result, infos));
	}

	void DefaultContactEventDispatcher::UserGetStatusResponse(int32_t result, std::list<UserStatus> infos, std::string copyBack){
		triggerEventEx(IContactServiceEvent, *_contactService, OnGetUserStatusResponse(result, infos, copyBack));
	}

}

