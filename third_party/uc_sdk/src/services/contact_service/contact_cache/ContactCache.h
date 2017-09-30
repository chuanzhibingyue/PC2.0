#ifndef CONTACT_CACHE_H_
#define CONTACT_CACHE_H_

#include "contact_service/ContactInfoImpl.h"
#include "ContactCacheListener.h"
#include <list>

namespace uc {

class ContactCache {
public:
	virtual ~ContactCache(){};

    /**
     * 通过加载DB中的信息，初始化cache
     */
    virtual void Initialize() = 0;

    /**
     * 释放缓存
     */
    virtual void UnInitialize() = 0;

    /**
     * 获取指定userid用户的BasicContact，适用于所有联系人
     */
    virtual BasicContactInfo GetBasicContact(int32_t userId) = 0;

	/**
	* 获取指定account用户的BasicContact，适用于所有联系人
	*/
	virtual BasicContactInfo GetBasicContact(const std::string& account) = 0;

    /**
     * 批量获取指定用户的BasicContact，适用于所有联系人
     * param userIds 用户id列表
     * param contacts 本地查找到的联系人信息
     * param foundList 本地查找到信息的用户id列表
     * param notFoundList 本地未查找到信息的用户id列表
     */
    virtual void GetBasicContact(const std::list<int32_t> &userIds,
        std::list<BasicContactInfo>& contacts,  std::list<int32_t> &foundList, 
		std::list<int32_t> &notFoundList) = 0;

    /**
     * 获取指定userid用户的Contact，适用于所有联系人
     */
    virtual ContactInfoImpl* GetContact(int32_t userId) = 0;

    /**
     * 批量获取指定用户的Contact，适用于所有联系人
     * param userIds 用户id列表
     * param contacts 本地查找到的联系人信息
     * param foundList 本地查找到信息的用户id列表
     * param notFoundList 本地未查找到信息的用户id列表
     */
    virtual void GetContact(const std::list<int32_t> &userIds, std::list<ContactInfoImpl*> &contacts, 
         std::list<int32_t> &foundList, std::list<int32_t> &notFoundList) = 0;

    /**
     * 获取常用联系人BasicContact列表
     */
    virtual int32_t GetBasicContactList(unsigned int position, unsigned int len, 
        std::vector<BasicContactInfo>& contacts) = 0;

	/**
	* 获取当前登录用户信息
	*/
	virtual bool GetCurrentUserInfo(ContactInfo &contact) = 0;

	/**
	* 获取当前登录用户全部信息
	*/
	virtual bool GetCurrentUserFullInfo(ContactInfo &contact) = 0;

    /**
     * 获取常用联系人BasicContact列表
     */
    virtual void GetCommonContactUserIds(std::list<int32_t> &userIds) = 0;

    /**
     * 保存联系人到cache中
     */
    virtual int SaveContact(const ContactInfoImpl& contact) = 0;
    
    /**
     * 批量保存联系人到cache中
     */
	virtual int SaveContact(const std::list<ContactInfoImpl*>& contactList) = 0;

    /**
     * 从cache中删除常用联系人
     */
    virtual bool RemoveCommonContact(int32_t userId) = 0;

    /**
     * 获取常用联系人的数量
     */
    virtual uint32_t GetCommonContactSize() = 0;

    /**
     * 判断指定用户是否是常用联系人
     */
    virtual bool IsCommonContact(int32_t userId) = 0;

    /**
     * 更新联系人的头像地址
     */
    virtual void UpdateAvatar(int32_t userId, const std::string &remoteAvatarUrl,
        const std::string & localAvatarPath) = 0;

    /**
     * 获取本地头像路径
     * 比较contact中remoteAvatarUrl是否与cache中一致，不一致则将contact的localAvatarPath设为空字符串，
     * 否则设成cache保存的localAvatarPath
     * 注意：contactservice将非cache来源的contact往cache中存储或上层抛联系人信息时，
     * 一定要调用该方法。保证localAvatarPath路径正确
     */
    virtual void GetLocalAvatar(ContactInfoImpl& contact) = 0;

    void SetListener(ContactCacheListener *listener) {
        _listener = listener;
    }
    
protected:
    ContactCacheListener *_listener;
};

}


#endif