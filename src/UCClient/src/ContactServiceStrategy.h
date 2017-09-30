/*
 * ContactServiceStrategy.h
 *
 *  Created on: July 2, 2015
 *      Author: gang wang
 */

#ifndef CONTACT_SERVICE_STRATEGY_H_
#define CONTACT_SERVICE_STRATEGY_H_

#include <string>

#include "IContactService.h"
#include "CefClient.h"

namespace ucclient {

using namespace uc;

class ContactServiceStrategy : public IContactServiceEvent {
public:
	ContactServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);

	void GetUpdatedContacts();

	bool GetContactInfo(int32_t userId, ContactInfo& contact);

    void GetContactInfo(const std::list<int32_t> &userIds);
    
	bool GetBasicContactInfo(int32_t userId, BasicContactInfo &contact);

    void GetBasicContactInfo(const std::list<int32_t> &userIds);

    int32_t GetContactList(unsigned int position, unsigned int len, OUT std::vector<BasicContactInfo>& contact);

    void UpdateContactAvatar(int32_t userId, const std::wstring& remoteAvatarUrl,
        const std::wstring& localAvatarPath);
    
	void AddCommonContact(int32_t userId);

	void DeleteCommonContact(int32_t userId);

	int32_t SearchLocalContact(ContactInfo::Type type, const std::wstring& keyword,
		OUT std::list<ContactInfo>& contacts);

    bool GetCurrentUserInfo(ContactInfo &contact);

	bool GetCurrentUserFullInfo(ContactInfo &contact);

	void UpdateUserInfo(const std::wstring& updateField);

	void UpdateUserTags(const std::wstring& updateTags);
    
	void GetColleagues(int32_t userId);

	void GetOrgAddressBook(int orgId, int32_t& taskId);

	void GetOrgMembers(int orgId, int limit, std::wstring copyback);

	int32_t SetUserStatus(int32_t userId, int32_t status, const std::wstring& show);

	int32_t SubscribeUserStatus(std::list<int32_t>& userIds);

    int32_t UnsubscribeUserStatus(std::list<int32_t>& userIds);

	int32_t UnsubscribeAllUserStatus();

	int32_t GetUserStatus(const std::list<int32_t>& userIds, IN std::wstring & copyBack);

    virtual void OnContactLoaded(int32_t result);

	virtual void OnUserStatusReceived(int32_t result, const std::list<UserStatus> &infos);

	virtual void OnGetUserStatusResponse(int32_t result, const std::list<UserStatus> &status, std::string &copyBack);

	virtual void OnUserStatusSet(int32_t result, int32_t userId, int32_t status);
   
	virtual void OnContactInfoReceived(int32_t result, const std::list<int32_t>* userIds,
        const std::list<ContactInfo*>* contacts);

    virtual void OnBasicContactInfoReceived(int32_t result, const std::list<int32_t>* userIds,
        const std::list<BasicContactInfo>* contacts);

	virtual void OnContactAdded(int32_t result, int32_t userId);

	virtual void OnContactDeleted(int32_t result, int32_t userId);

	virtual void OnContactUpdated(std::list<ContactInfo*> *contact);
    
	virtual void OnUserInfoUpdated(int32_t result, const ContactInfo *info);
    
	virtual void OnUserTagsUpdated(int32_t result, const std::string &updateTags);

    virtual void OnSystemTagsAdded(std::vector<ContactTag*>* tags);
    
    virtual void OnSystemTagsUpdated(std::vector<ContactTag*>* tags);
    
    virtual void OnSystemTagsDeleted(std::vector<int32_t>& tagIds);

	virtual void OnColleagueReceived(int32_t result, int32_t userId, int32_t page,
		int32_t colNum, int32_t leaderNum, std::list<ContactInfo*> *colleagues,
		std::list<ContactInfo*> *leaders);
	
	virtual void OnOrgAddressBookReceived(int32_t result, DepartmentInfo *department,
		DepartmentInfo *myDepartment, int32_t taskId);
    
	virtual void OnOrgAddrMembersReceived(int32_t result, int orgId, int userCount, 
		std::vector<int32_t> *memberIds, std::string& copyback);

    virtual void OnDeptUpdated(int32_t operatorId, int32_t deptId, 
        const std::string &oldDeptName, const std::string &newDeptName, 
        const std::string &desc, int64_t timestamp);
    
    virtual void OnDeptTransfered(int32_t operatorId, int32_t userId, 
        const std::string &userName, int32_t deptId, const std::string &oldDeptName,
        const std::string &deptName, const std::string &desc, int64_t timestamp);
    
    virtual void OnPositionUpdated(int32_t operatorId, int32_t userId, 
        const std::string &userName, const std::string &newPosition, 
        const std::string &oldPosition, const std::string& deptName, 
        const std::string& desc, int64_t timestamp);
    
    virtual void OnEmployeeEntry(int32_t operatorId, int32_t userId, int32_t deptId, 
        const std::string &userName, const std::string &deptName,  
        const std::string &position, const std::string &desc, int64_t timestamp);
    
    virtual void OnEmployeeLeaved(int32_t operatorId, int32_t userId,
        const std::string &userName, const std::string &deptName, 
        const std::string &desc, int64_t timestamp);
    
    virtual void OnEmployeeRightsUpdated(int32_t operatorId, int32_t userId, 
        const std::string &userName, const std::string &deptName, int32_t deptId, 
        const std::string &desc, int64_t timestamp);
    
    virtual void OnDeptParentChanged(int32_t operatorId, int32_t deptId, 
        const std::string &deptName, const std::string &oldDeptName, 
        const std::string &newDeptName, const std::string &desc, int64_t timestamp);
    
    virtual void OnCompanyJoined(int32_t operatorId, int32_t companyId, int32_t userId, 
        const std::string &companyName, const std::string &userName, 
        const std::string &desc, int64_t timestamp);
    
    virtual void OnDeptDeleted(int32_t operatorId, int32_t deptId, 
        const std::string &deptName, const std::string &parentDeptName, 
        const std::string &desc, int64_t timestamp);
    
    virtual void OnEmployeeEntryConfirmed(int32_t operatorId, int32_t deptId, 
        const std::string &userName, const std::string &deptName, 
        int32_t userId, std::string &desc, int64_t timestamp);
    
    virtual void OnEmployeeLeaveConfirmed(int32_t operatorId, int32_t userId, int32_t deptId, 
        const std::string &userName, const std::string &deptName, 
        const std::string &desc, int64_t timestamp);
    
    virtual void OnDeptTransferConfirmed(int32_t operatorId, int32_t userId, 
        const std::string &userName, const std::string &deptName, 
        const std::string &desc, int64_t timestamp);

	
private:

	cefIntegration::CefClient *_cefClient;
	IContactService *_contactService;
	IUCEngine *_engine;
};

}

#endif