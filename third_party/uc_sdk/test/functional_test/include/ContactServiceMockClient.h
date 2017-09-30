#ifndef CONTACT_SERVICE_MOCK_CLIENT_H_
#define CONTACT_SERVICE_MOCK_CLIENT_H_

#include "IContactService.h"

using namespace uc;

class ContactServiceMockClient : public IContactServiceEvent{
public:	
	void Init(IUCEngine *ucEngine);
	void UnInit();
	void GetUpdatedContacts();

	//bool GetContactInfo(int32_t userId, OUT ContactInfo& contact);

	//void GetContactInfo(const std::list<int32_t> &userIds);


	virtual void OnContactLoaded(int32_t result){}

	virtual void OnContactPresenceReceived(int32_t userId, ContactInfo::Status status){}

	virtual void OnContactInfoReceived(int32_t result, const std::list<int32_t>* userIds,
        const std::list<ContactInfo*>* contacts);

    virtual void OnBasicContactInfoReceived(int32_t result, const std::list<int32_t>* userIds,
        const std::list<BasicContactInfo*>* contacts){}

	virtual void OnContactAdded(int32_t result, int32_t userId){}

	virtual void OnContactDeleted(int32_t result, int32_t userId){}

	virtual void OnContactUpdated(std::list<ContactInfo*> *contact);
    

	virtual void OnUserInfoUpdated(int32_t result, const ContactInfo *info){}
    

    virtual void OnContactTagsUpdated(int32_t result, std::list<ContactTag*> *addedTags,
		std::list<ContactTag*> *updatedTags, std::list<ContactTag*> *deletedTags){};
    

    virtual void OnUserTagsUpdated(int32_t result, const std::list<ContactTag*>* fieldList){}


    virtual void OnSystemTagsAdded(std::vector<ContactTag*>* tags){};
    
    virtual void OnSystemTagsUpdated(std::vector<ContactTag*>* tags){}
    
    virtual void OnSystemTagsDeleted(std::vector<int32_t>& tagIds){}

	virtual void OnColleagueReceived(int32_t result, int32_t userId, int32_t page,
		int32_t colNum, int32_t leaderNum, std::list<ContactInfo*> *colleagues,
		std::list<ContactInfo*> *leaders){}
	
	virtual void OnOrgAddressBookReceived(int32_t result, DepartmentInfo *department,
		DepartmentInfo *myDepartment){}
    

    virtual void OnDeptUpdated(int32_t operatorId, int32_t deptId, 
        const std::wstring &oldDeptName, const std::wstring &newDeptName, 
        const std::wstring &desc){}
    
    virtual void OnDeptTransfered(int32_t operatorId, int32_t userId, 
        const std::wstring &userName, int32_t deptId, const std::wstring &oldDeptName,
        const std::wstring &deptName, const std::wstring &desc){}
    
    virtual void OnPositionUpdated(int32_t operatorId, int32_t userId, 
        const std::wstring &userName, const std::wstring &newPosition, 
        const std::wstring &oldPosition, const std::wstring& deptName, 
        const std::wstring& desc){}
    

    virtual void OnEmployeeEntry(int32_t operatorId, int32_t userId, int32_t deptId, 
        const std::wstring &userName, const std::wstring &deptName,  
        const std::wstring &position, const std::wstring &desc){}
    

    virtual void OnEmployeeLeaved(int32_t operatorId, int32_t userId,
        const std::wstring &userName, const std::wstring &deptName, 
        const std::wstring &desc){}
    
    virtual void OnEmployeeRightsUpdated(int32_t operatorId, int32_t userId, 
        const std::wstring &userName, const std::wstring &deptName, int32_t deptId, 
        const std::wstring &desc){}
    

    virtual void OnDeptParentChanged(int32_t operatorId, int32_t deptId, 
        const std::wstring &deptName, const std::wstring &oldDeptName, 
        const std::wstring &newDeptName, const std::wstring &desc){}
    

    virtual void OnCompanyJoined(int32_t operatorId, int32_t companyId, int32_t userId, 
        const std::wstring &companyName, const std::wstring &userName, 
        const std::wstring &desc){}
    
    virtual void OnDeptDeleted(int32_t operatorId, int32_t deptId, 
        const std::wstring &deptName, const std::wstring &parentDeptName, 
        const std::wstring &desc){}
    

    virtual void OnEmployeeEntryConfirmed(int32_t operatorId, int32_t deptId, 
        const std::wstring &userName, const std::wstring &deptName, 
        int32_t userId, std::wstring &desc){}    

    virtual void OnEmployeeLeaveConfirmed(int32_t operatorId, int32_t userId, int32_t deptId, 
        const std::wstring &userName, const std::wstring &deptName, 
		const std::wstring &desc){}
    

    virtual void OnDeptTransferConfirmed(int32_t operatorId, int32_t userId, 
        const std::wstring &userName, const std::wstring &deptName, 
        const std::wstring &desc){}
public:

	ClientInfo _clientinfo;
	int resultCode;

	boost::recursive_mutex mthodMtx;
	boost::condition_variable methodCond;

private:
	IContactService *_contactSerivce;
};

#endif