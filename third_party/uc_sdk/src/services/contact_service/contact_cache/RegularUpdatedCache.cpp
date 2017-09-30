#include "RegularUpdatedCache.h"
#include "LogManager.h"
#include "UCErrorCode.h"
#include "components/db/include/DBManager.h"
#include "contact_service/ContactService.h"
#include "utils/UCTime.h"
#include "Macro.h"
#include <boost/filesystem.hpp>
#include "utils/TaskExecutor.h"

namespace uc {

#define CONTACT_TIMEOUT_MILLI_SECONDS (1000 * 60 * 60 * 24UL)            // 24小时

RegularUpdatedCache::RegularUpdatedCache(ContactService *contactService) : _contactService(contactService){
	userInfo = ContactInfoImpl::CreateContactInfoImpl();
}

RegularUpdatedCache::~RegularUpdatedCache() {
	delete userInfo;
}

void RegularUpdatedCache::Initialize() {
	UC_LOG(INFO) << "load common contacts from database";

    std::list<ContactInfoImpl*> info;
	int32_t res = DBManager::GetInstance().GetContactListByType(CTI_COMMON, info);

    if (res == UC_SUCCEEDED) {
        UC_LOG(INFO) << "load common contacts from database successfully, size: " << info.size();
            
        if (!info.empty()) {
            for (std::list<ContactInfoImpl*>::iterator iter = info.begin();
                iter != info.end(); ++iter) {
				this->SaveBasicContact(*iter);
            }
        }
    }
    else if (res == DBError::DB_QUERY_EMPTY) {
        UC_LOG(WARNING) << "database has no cached common contact";
        res = UC_SUCCEEDED;
    }
	else{
		UC_LOG(ERROR) << "failed to load common contacts from database, result: " << res;
        res = UC_FAILED;
	}

	_listener->OnContactCacheInitialized(res);
	SAFE_CLEAR_CONTAINER(std::list<ContactInfoImpl*>, info);
}

void RegularUpdatedCache::UnInitialize() {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);

	_contactVector.clear();
	_contactMap.clear();
	_contactUpdatedTimeMap.clear();
}

BasicContactInfo RegularUpdatedCache::GetMemoryBasicContact(int32_t userId) {
    boost::unique_lock<boost::recursive_mutex> lock(_mtx);
    
    BasicContactMap::iterator iter = _contactMap.find(userId);
    if(iter != _contactMap.end()) {
        return iter->second;
    }

    return BasicContactInfo();
}

BasicContactInfo RegularUpdatedCache::GetMemoryBasicContact(const std::string& account) {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);

	for (auto item : _contactMap){
		if (item.second.GetAccount().compare(account) == 0){
			return item.second;
		}
	}

	return BasicContactInfo();
}

BasicContactInfo RegularUpdatedCache::GetBasicContact(int32_t userId) {
	//UC_LOG(INFO) << "Get basic contact info from contact cache, userId: " << userId;

	int64_t timestamp = 0;
    
    BasicContactInfo contact = GetMemoryBasicContact(userId);
    if (contact.GetUserId() == 0) {
    	UC_LOG(WARNING) << "Didn't find basic contact info from contact cache, userId: " << userId;

    	ContactInfoImpl* contactImpl = ContactInfoImpl::CreateContactInfoImpl();
		if (DBManager::GetInstance().GetContact(userId, *contactImpl) == UC_SUCCEEDED) {
			contact = SaveBasicContact(contactImpl);
    	}
    	else {
    		UC_LOG(WARNING) << "Didn't find contact info from database, userId: " << userId;
    	}

    	delete contactImpl;
    }

    GetContactFromServerIfTimeout(userId);

    return contact;
}

BasicContactInfo RegularUpdatedCache::GetBasicContact(const std::string& account) {

	int64_t timestamp = 0;
	BasicContactInfo contact = GetMemoryBasicContact(account);
	if (contact.GetUserId() == 0) {
		UC_LOG(WARNING) << "Didn't find basic contact info from contact cache, account: " << account;

		ContactInfoImpl* contactImpl = ContactInfoImpl::CreateContactInfoImpl();
		if (DBManager::GetInstance().GetContact(account, *contactImpl) == UC_SUCCEEDED) {
			contact = SaveBasicContact(contactImpl);
		}
		else {
			UC_LOG(WARNING) << "Didn't find contact info from database, account: " << account;
		}

		delete contactImpl;
	}

	return contact;
}

void RegularUpdatedCache::GetBasicContact(const std::list<int32_t> &userIds,
    std::list<BasicContactInfo >& contacts, 
    std::list<int32_t> &foundList, std::list<int32_t> &notFoundList) {

    //UC_LOG(INFO) << "Get basic contact info list from contact cache";

    for (std::list<int32_t>::const_iterator iter = userIds.begin();
        iter != userIds.end(); ++iter) {
        const BasicContactInfo& contact = GetMemoryBasicContact(*iter);
        if (contact.GetUserId() != 0) {
            foundList.push_back(*iter);
            contacts.push_back(contact);
        }
        else {
            notFoundList.push_back(*iter);
        }
    }

    std::list<ContactInfoImpl*> contactImpl;
    if (!notFoundList.empty()) {
        if (DBManager::GetInstance().GetContactListByUserID(notFoundList, contactImpl) == UC_SUCCEEDED &&
            !contactImpl.empty()) {

            for (std::list<ContactInfoImpl*>::iterator iter = contactImpl.begin();
                iter != contactImpl.end(); ++iter) {

                const BasicContactInfo& contact = SaveBasicContact(*iter);
                contacts.push_back(contact);
                
                foundList.push_back((*iter)->GetUserId());
                notFoundList.remove((*iter)->GetUserId());
            }
        }
    }

    for (std::list<ContactInfoImpl*>::iterator iter = contactImpl.begin();
        iter != contactImpl.end(); ++iter) {
        delete *iter;
    }

    GetContactFromServerIfTimeout(foundList);
}

void RegularUpdatedCache::GetContactFromServerIfTimeout(const std::list<int32_t> &userIds) {
    boost::unique_lock<boost::recursive_mutex> lock(_mtx);

    std::list<int32_t> timeoutList;
    for (std::list<int32_t>::const_iterator iter = userIds.begin();
        iter != userIds.end(); ++iter) {
		ContactUpdatedTimeMap::iterator iterMap = _contactUpdatedTimeMap.find(*iter);
		if (iterMap != _contactUpdatedTimeMap.end() && IsTimeOut(iterMap->second)) {
            timeoutList.push_back(*iter);
        }
    }

    lock.unlock();

    if (!timeoutList.empty()) {
		_contactService->GetContactDelayLoader()->GetContact(timeoutList, DISPATCH_CONTACT_UPDATED);

    }
}

void RegularUpdatedCache::GetContactFromServerIfTimeout(int32_t userId) {
    boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	ContactUpdatedTimeMap::iterator iter = _contactUpdatedTimeMap.find(userId);
	if (iter != _contactUpdatedTimeMap.end()) {
		int64_t timestamp = iter->second;
		lock.unlock();

		if (IsTimeOut(timestamp)) {
			UC_LOG(WARNING) << "userId: " << userId << " profile time out, so get profile from server";

			std::list<int32_t> userIds;
			userIds.push_back(userId);
			_contactService->GetContactDelayLoader()->GetContact(userIds, DISPATCH_CONTACT_UPDATED);
		}
	}
    
}

ContactInfoImpl* RegularUpdatedCache::GetContact(int32_t userId) {
	//UC_LOG(INFO) << "Get contact info from contact cache, userId: " << userId;

	ContactInfoImpl* contactImpl = ContactInfoImpl::CreateContactInfoImpl();

	if (DBManager::GetInstance().GetContact(userId, *contactImpl) == DBError::DB_SUCCESS) {
		SaveBasicContact(contactImpl);
		return contactImpl;
    }
    else {
    	UC_LOG(WARNING) << "Didn't find contact info from database, userId: " << userId;
        delete contactImpl;

        return NULL;
    }
}

void RegularUpdatedCache::GetContact(const std::list<int32_t> &userIds, std::list<ContactInfoImpl*> &contacts,
        std::list<int32_t> &foundList, std::list<int32_t> &notFoundList) {
    std::list<ContactInfoImpl*> contactImpl;
    notFoundList = userIds;

    if (!userIds.empty()) {
		////sxp
		if (DBManager::GetInstance().GetContactListByUserID(const_cast< std::list<int32_t> &>(userIds), contactImpl) == DBError::DB_SUCCESS &&
            !contactImpl.empty()) {

            for (std::list<ContactInfoImpl*>::iterator iter = contactImpl.begin();
                iter != contactImpl.end(); ++iter) {

				SaveBasicContact(*iter);
				//没有标签信息的需重新向Server请求
				if ((*iter)->GetUserTags() != ""){
					contacts.push_back(*iter);
					foundList.push_back((*iter)->GetUserId());
					notFoundList.remove((*iter)->GetUserId());
				}
            }

        }
    }
}

int32_t RegularUpdatedCache::GetBasicContactList(unsigned int position, unsigned int len, 
    std::vector<BasicContactInfo>& contacts) {
    boost::unique_lock<boost::recursive_mutex> lock(_mtx);

    uint32_t size = (uint32_t)_contactVector.size();
    if (position >= size) {
        UC_LOG(ERROR) << "Get basic contact list argument error , position: " << position
            << ", len: " << len << ", size: " << size;
        return UC_FAILED;
    }

    if (len == 0 || len > _contactVector.size() - position) {
        len = size - position;
    }
    
    for ( std::size_t i = 0; i < len; ++i) {
        contacts.push_back(_contactVector[position + i]);
    }
    
    return UC_SUCCEEDED;
}

bool RegularUpdatedCache::GetCurrentUserInfo(ContactInfo &contact) {
	if (userInfo->GetUserId() != 0) {
		contact = *userInfo;
		return true;
	}

	UC_LOG(ERROR) << "Didn't find current user info.";
	return false;
}

bool RegularUpdatedCache::GetCurrentUserFullInfo(ContactInfo &contact){
	if (userInfo->GetUserId() != 0) {
		contact = *userInfo;
		return true;
	}
	UC_LOG(ERROR) << "Didn't find current user full info.";
	return false;
}

void RegularUpdatedCache::GetCommonContactUserIds(list<int32_t> &userIds){
    boost::unique_lock<boost::recursive_mutex> lock(_mtx);

    for (std::size_t i = 0; i < _contactVector.size(); ++i) {
        userIds.push_back(_contactVector[i].GetUserId());
    }
}

int RegularUpdatedCache::SaveContact(const ContactInfoImpl& contact) {
	if (contact.GetUserId() == _contactService->GetCurUserId()) {
		*userInfo = contact;
	}
	
	SaveBasicContact(const_cast<ContactInfoImpl*>(&contact));

	int32_t res = DBManager::GetInstance().SaveContact(contact);

    if (res != UC_SUCCEEDED) {
        UC_LOG(ERROR) << "save contact to database, result: " << res;
    }

	return res;
}

int RegularUpdatedCache::SaveContact(const std::list<ContactInfoImpl*>& contactList) {
	UC_LOG(INFO) << "Save Contact list";
	std::list<ContactInfoImpl*>::const_iterator iter = contactList.begin();
	for (; iter != contactList.end(); ++iter) {
		if ((*iter)->GetUserId() == _contactService->GetCurUserId()) {
			*userInfo = **iter;
		}
		SaveBasicContact(*iter);
	}

	int32_t res = DBManager::GetInstance().SaveContact(contactList);
	
    if (res != UC_SUCCEEDED) {
        UC_LOG(ERROR) << "save contact list to database, result: " << res;
    }

	return res;
}

bool RegularUpdatedCache::RemoveCommonContact(int32_t userId) {
    boost::unique_lock<boost::recursive_mutex> lock(_mtx);
    
    BasicContactMap::iterator iter = _contactMap.find(userId);
    if(iter != _contactMap.end()) {
        _contactMap.erase(iter);
        
        for (std::size_t index = 0; index < _contactVector.size(); ++index) {
			if (userId == _contactVector[index].GetUserId()) {
                _contactVector.erase(_contactVector.begin() + index);
                break;
            }
        }
        
        int32_t res = DBManager::GetInstance().UpdateUserType(userId, CTI_OTHERS);
    	if (res != UC_SUCCEEDED) {
        	UC_LOG(ERROR) << "update user type to database, result: " << res;
    	}

        return true;
    }
    
    return false;
}

void RegularUpdatedCache::UpdateAvatar(int32_t userId, const std::string &remoteAvatarUrl,
        const std::string &localAvatarPath) {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);

	BasicContactMap::iterator iter = _contactMap.find(userId);
	if (iter != _contactMap.end() && iter->second.GetRemoteAvatarUrl().compare(remoteAvatarUrl) == 0) {
		iter->second.SetLocalAvatar(localAvatarPath);
	}
	else {
		UC_LOG(ERROR) << "fail to update contact avatar in cache, userid: " << userId;
	}

	lock.unlock();
    
	TaskExecutor::GetInstance().Schedule(MakeCallback(this, &RegularUpdatedCache::UpdateAvatarToDB, remoteAvatarUrl, localAvatarPath));
}

void RegularUpdatedCache::UpdateAvatarToDB(const std::string &remoteAvatarUrl, const std::string &localAvatarPath){
	int32_t res = DBManager::GetInstance().UpdateFSLocalPath(remoteAvatarUrl, localAvatarPath);
	if (res != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "fail to update contact avatar, remoteAvatarUrl: " << remoteAvatarUrl
			<< ", localAvatarPath: " << localAvatarPath << ", res: " << res;
	}
	else {
		UC_LOG(INFO) << "update contact avatar, remoteAvatarUrl: " << remoteAvatarUrl
			<< ", localAvatarPath: " << localAvatarPath << ", res: " << res;
	}
}

uint32_t RegularUpdatedCache::GetCommonContactSize() {
    boost::unique_lock<boost::recursive_mutex> lock(_mtx);
    return (uint32_t)_contactVector.size();
}

bool RegularUpdatedCache::IsTimeOut(int64_t timestamp) {
	int64_t curTime = Time::GetSysTime();
	return (curTime < timestamp || curTime - timestamp >= CONTACT_TIMEOUT_MILLI_SECONDS);
}

void RegularUpdatedCache::GetLocalAvatar(ContactInfoImpl& contact) {
	std::string path;
	int32_t res = DBManager::GetInstance().GetFSLocalPath(contact.GetRemoteAvatarUrl(), path, true);
	if (res != UC_SUCCEEDED) {
		contact.SetLocalAvatar("");
		UC_LOG(ERROR) << "fail to get contact local avatar, userId: " << contact.GetUserId() << ", res: " << res;
	}
	else {
		if (CheckLocalPath(path)) {
			contact.SetLocalAvatar(path);
		}
		else {
			contact.SetLocalAvatar("");
		}
	}
}

bool RegularUpdatedCache::CheckLocalPath(std::string path) {
	if (!path.empty()){
		boost::filesystem::path localPath = StringConvert::FromUTF8Str(path);
		if (!boost::filesystem::exists(localPath) || boost::filesystem::file_size(localPath) <= 0) {
			return false;
		}

		return true;
	}
	else {
		return false;
	}
}
/**
 * 按拼音字母对Contact列表排序
 */
struct CompareContactFunc {

    inline bool operator() (const BasicContactInfo& first, const BasicContactInfo& second) {
		if (first.GetPinyin().empty()) {
			return false;
		}
		else if (second.GetPinyin().empty()) {
			return true;
		}

		int res = first.GetPinyin().compare(second.GetPinyin());
		if (res < 0) {
			return true;
		}
		else {
			// res为0时，返回false，表示小于。
			return false;
		}
    }

};

BasicContactInfo RegularUpdatedCache::SaveBasicContact(const ContactInfoImpl* contact) {
	BasicContactInfo basicInfo(contact);
	SaveBasicContact(basicInfo, contact->GetLastUpdateTime(), contact->IsCommon());
    return basicInfo;
}

void RegularUpdatedCache::SaveBasicContact(const BasicContactInfo& contact, int64_t timestamp, bool isCommon) {
	if (contact.GetUserId() == 0 || contact.GetPinyin().empty() || contact.GetDisplayName().empty()) {
        return;
    }
	
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	if (isCommon) {
        BasicContactMap::iterator iter = _contactMap.find(contact.GetUserId());
        
        bool isPush = true, isSort = true;
        if(iter != _contactMap.end()) {
            // cache中已有该联系人信息，则更新数据
            std::size_t index = 0;
            for (; index < _contactVector.size() && 
                (contact.GetUserId() != _contactVector[index].GetUserId()); ++index) {
            }

            if (index != _contactVector.size()) {
                if (_contactVector[index].GetPinyin().compare(contact.GetPinyin()) == 0) {
                    isSort = false;
                }

                _contactVector[index] = contact;
                 isPush = false;
            }
        }
        
        if (isPush) {
            _contactVector.push_back(contact);
        }

        if (isSort) {
            std::sort(_contactVector.begin(), _contactVector.end(), CompareContactFunc());
        }
	}

	_contactMap[contact.GetUserId()] = contact;
	_contactUpdatedTimeMap[contact.GetUserId()] = timestamp;
}

bool RegularUpdatedCache::IsCommonContact(int32_t userId) {
    boost::unique_lock<boost::recursive_mutex> lock(_mtx);
    for (std::size_t index = 0; index < _contactVector.size(); ++index) {
        if (userId == _contactVector[index].GetUserId()) {
            return true;
        }
    }

    return false;
}

void RegularUpdatedCache::RefreshContactUpdatedTime(int32_t userId) {
    int64_t curTime = Time::GetSysTime();

    boost::unique_lock<boost::recursive_mutex> lock(_mtx);
    _contactUpdatedTimeMap[userId] = curTime;
    lock.unlock();

    int32_t res = DBManager::GetInstance().UpdateLastUpdateTime(userId, curTime);
    if (res != UC_SUCCEEDED) {
        UC_LOG(ERROR) << "update last updated time to database, result: " << res << ", userId" << userId;
    }
}

void RegularUpdatedCache::RefreshCommonContactUpdatedTime() {
    int64_t curTime = Time::GetSysTime();

    boost::unique_lock<boost::recursive_mutex> lock(_mtx);
    for (std::size_t i = 0; i < _contactVector.size(); ++i) {
        _contactUpdatedTimeMap[_contactVector[i].GetUserId()] = curTime;
    }
    lock.unlock();

    int32_t res = DBManager::GetInstance().UpdateLastUpdateTimeByTypes(CTI_COMMON, curTime);
    if (res != UC_SUCCEEDED) {
        UC_LOG(ERROR) << "update last updated time for common contacts to database, result: " << res;
    }
}

}
