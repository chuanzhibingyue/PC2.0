#ifndef REGULAR_UPDATED_CACHE_H_
#define REGULAR_UPDATED_CACHE_H_

#include <list>
#include <map>
#include <vector>

#include "ContactCache.h"
#include "DepartmentInfo.h"
#include "LogManager.h"
#include "UCErrorCode.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include "DBManager.h"

namespace uc {

class ContactService;

class RegularUpdatedCache : public ContactCache {
public:
    RegularUpdatedCache(ContactService *contactService);
	~RegularUpdatedCache();

	virtual void Initialize();

	virtual void UnInitialize();

	BasicContactInfo GetBasicContact(int32_t userId);

	BasicContactInfo GetBasicContact(const std::string& account);

    virtual void GetBasicContact(const std::list<int32_t> &userIds,
        std::list<BasicContactInfo>& contacts, 
        std::list<int32_t> &foundList, std::list<int32_t> &notFoundList);

    virtual ContactInfoImpl* GetContact(int32_t userId);

    virtual void GetContact(const std::list<int32_t> &userIds, std::list<ContactInfoImpl*> &contacts,
        std::list<int32_t> &foundList, std::list<int32_t> &notFoundList);

    virtual int32_t GetBasicContactList(unsigned int position, unsigned int len, 
        std::vector<BasicContactInfo>& contacts);

	virtual bool GetCurrentUserInfo(ContactInfo &contact);

	virtual bool GetCurrentUserFullInfo(ContactInfo &contact);

    virtual void GetCommonContactUserIds(std::list<int32_t> &userIds);

    virtual int SaveContact(const ContactInfoImpl& contact);
    
	virtual int SaveContact(const std::list<ContactInfoImpl*>& contactList);

    virtual bool RemoveCommonContact(int32_t userId);

    virtual uint32_t GetCommonContactSize();

    virtual bool IsCommonContact(int32_t userId);
    
    virtual void UpdateAvatar(int32_t userId, const std::string &remoteAvatarUrl,
        const std::string & localAvatarPath);

    virtual void GetLocalAvatar(ContactInfoImpl& contact);

	template<typename T> void GetLocalAvatarList(T& contacts);

    void RefreshContactUpdatedTime(int32_t userId);

    /**
     * contact service完成contact/update时调用
     */
    void RefreshCommonContactUpdatedTime();

private:
    BasicContactInfo GetMemoryBasicContact(int32_t userId);
	BasicContactInfo GetMemoryBasicContact(const std::string& account);
    BasicContactInfo SaveBasicContact(const ContactInfoImpl* contact);

    void SaveBasicContact(const BasicContactInfo& contact, int64_t timestamp, bool isCommon = true);

	void GetContactFromServerIfTimeout(const std::list<int32_t> &userIds);

	void GetContactFromServerIfTimeout(int32_t userId);

    bool IsTimeOut(int64_t timestamp);

	bool CheckLocalPath(std::string path);

	void UpdateAvatarToDB(const std::string &remoteAvatarUrl, const std::string &localAvatarPath);

private:
	// 当前登录用户信息
	ContactInfoImpl *userInfo;

    // 常用联系人列表
	typedef std::vector<BasicContactInfo > BasicContactVector;
    BasicContactVector _contactVector;

    // 所有联系人map
	typedef std::map<int32_t, BasicContactInfo > BasicContactMap;
    BasicContactMap _contactMap;
    boost::recursive_mutex _mtx;

    // 记录内存中联系人更新的时间
    typedef std::map<int32_t, int64_t> ContactUpdatedTimeMap;
    ContactUpdatedTimeMap _contactUpdatedTimeMap;

    ContactService *_contactService;
};

template<typename T>
inline void RegularUpdatedCache::GetLocalAvatarList(T& contacts) {
	if (contacts.empty()) {
		return;
	}

	typedef typename T::iterator Iterator;

	std::map<string, string> paths;
	std::list<string> urls;
	for (Iterator iter = contacts.begin(); iter != contacts.end(); ++iter) {
		urls.push_back((*iter)->GetRemoteAvatarUrl());
		(*iter)->SetLocalAvatar("");
	}

	int32_t res = DBManager::GetInstance().GetFSLocalPath(urls, paths, true);
	if (res != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "fail to get contact local avatar, res: " << res;
	}
	else {
		for (Iterator iter = contacts.begin(); iter != contacts.end(); ++iter) {
			std::string path = paths[(*iter)->GetRemoteAvatarUrl()];
			if (CheckLocalPath(path)) {
				(*iter)->SetLocalAvatar(path);
			}
			else {
				(*iter)->SetLocalAvatar("");
			}
		}
	}
}

template<>
inline void RegularUpdatedCache::GetLocalAvatarList<DepartmentInfo>(DepartmentInfo& depart) {
	GetLocalAvatarList<std::vector<ContactInfo*>>(const_cast<std::vector<ContactInfo*>&>(depart.GetStaffs()));
}

}

#endif