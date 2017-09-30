/*
 * ContactService.h
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#ifndef CONTACTSERVICE_H_
#define CONTACTSERVICE_H_

#include <vector>
#include <list>
#include <map>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include "IContactService.h"
#include "ContactInfoImpl.h"
#include "ContactRequestFactory.h"
#include "ContactResponseParser.h"
#include "ContactEventDispatcher.h"
#include "ContactMsgProcessor.h"
#include "components/parser/msg_parser/include/UCMessageType.h"
#include "IClientService.h"
#include "contact_cache/ContactCache.h"
#include "contact_cache/ContactCacheListener.h"
#include "contact_cache/ContactDelayLoader.h"

namespace uc {

typedef std::list<UserStatus>   UserStatusList;
typedef std::map<int32_t, UserStatus> UserStatusMap;

class ContactService: public IContactService, public IUCEngineEvent, public ContactCacheListener{

    friend class ContactMsgProcessor;
	friend class RegularUpdatedCache;
	friend class ContactDelayLoading;

public:
	ContactService(IUCEngine *engine);
	virtual ~ContactService();
 	
// 以下是继承自IContactService的接口
	
	virtual void GetUpdatedContacts();

	virtual bool GetContactInfo(int32_t userId, OUT ContactInfo& contact);

	virtual void GetContactInfo(const std::list<int32_t> &userIds);
    
    bool GetBasicContactInfo(int userId, BasicContactInfo& contact);

    virtual void GetBasicContactInfo(const std::list<int32_t> &userIds);

    virtual void UpdateContactAvatar(int32_t userId, const std::string& remoteAvatarUrl,
        const std::string& localAvatarPath);

    virtual int32_t GetContactList(unsigned int position, unsigned int len, OUT std::vector<BasicContactInfo>& contact);

	virtual void AddCommonContact(int32_t userId);

	virtual void DeleteCommonContact(int32_t userId);

    virtual int32_t SearchLocalContact(ContactInfo::Type type, const std::string& keyword,
		OUT std::list<ContactInfo>& contacts);

    virtual bool GetCurrentUserInfo(ContactInfo &contact);

	virtual bool GetCurrentUserFullInfo(ContactInfo &contact);
    
	virtual void UpdateUserInfo(const std::string& updateField);

	virtual void UpdateUserTags(const std::string& updateTags);
    
	virtual void GetColleagues(int32_t userId);

	virtual void GetOrgAddressBook(int orgId, int32_t& taskId);

	virtual void GetOrgMembers(int orgId, int limit, std::string& copyback);

	virtual int32_t SubscribeUserStatus(std::list<int32_t>& userIds);

	virtual int32_t UnsubscribeUserStatus(std::list<int32_t>& userIds);

	virtual int32_t UnsubscribeAllUserStatus();

	int32_t GetUserStatus(const std::list<int32_t>& userIds, IN OUT std::string & copyBack);

	virtual int32_t SetUserStatus(int32_t userId, int32_t status, const std::string& show);

    
// IUCEngineEvent
	virtual int32_t OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state);
    virtual void OnLoginSucceeded();
    virtual void OnLogoutSucceeded();
	virtual void OnUCASConnected(int32_t errorCode);

// ContactCacheListener
	virtual void OnContactCacheInitialized(int32_t res);

// 以下是contact service自定义的成员函数
	/**
     * 初始化ContactService, 包括加载本地通讯录，更新通讯录
     */
    void Initialize();
    
    /**
     * 结束ContactService, 包括释放内存缓存的联系人信息,重置一些内部标志位
     */
    void UnInitialize();

    /**
	 * 保存联系人到本地(给service使用)
	 * @param ptParse profile的json数据
	 */
    int32_t SaveContact(const boost::property_tree::wptree &ptParse);

	/**
	 * 获取当前登陆用户id
	 * @param 登陆用户id
	 */
    int32_t GetCurUserId();
	
	bool IsCurUser(UCID userUid);

	/**
	 * 拼接ucc GET请求的url
	 * @param methodUrl path
	 * @param url 拼接后的url
	 */
    void GETRequestUrl(const std::string& methodUrl, std::stringstream &url);

	/**
	 * 拼接ucc POST请求的url
	 * @param methodUrl path
	 * @param url 拼接后的url
	 */
    void POSTRequestUrl(const std::string& methodUrl, std::stringstream &url);

	/**
	 * 拼接ucc POST请求的body
	 * @param body 拼接后的body
	 */
    void POSTRequestBody(std::stringstream &body);

    /**
     * 设置ContactInfo中联系人属性,保存到cache前设定
     * @param contactList 联系人信息列表
     * @param isCommon 是否强制设为常用联系人。如果是false，则根据查询缓存数据设置
     */
    void SetContactsType(const std::list<ContactInfoImpl*>& contactList, bool isCommon);
    
    /**
     * 设置ContactInfo中联系人属性，保存到cache前设定
     * @param contactList 联系人信息列表
     * @param isCommon 是否强制设为常用联系人。如果是false，则根据查询缓存数据设置
     */
    void SetContactType(ContactInfoImpl& contact, bool isCommon);

    /**
     * 批量从服务器获取联系人信息，成功获取后，通过事件通知调用模块
     * @param userIds 批量查询的联系人用户id
     * @param dispatchWhich 成功后，触发事件的类型。
     */
	int32_t GetContactsFromServer(const std::list<int32_t>& userIds, CONTACT_INFO_RECEIVED_EVENT dispatchWhich = DISPATCH_CONTACT_INFO_RECEIVED);

	/**
	* 从服务器单个获取联系人信息，成功获取后，通过事件通知调用模块
	* @param userIds 查询的联系人用户id
	* @param dispatchWhich 成功后，触发事件的类型。
	*/
	int32_t GetContactInfoFromServer(const int32_t& userId, CONTACT_INFO_RECEIVED_EVENT dispatchWhich);

	/**
     * 从数据库获取上一次常用联系人更新的时间
     * @param time 上一次常用联系人更新的时间
     * @return 查询结果
     */
    int32_t GetLastContactSyncTime(int64_t* time);

	ContactCache * GetContactCache() {return _contactCache;}

	ContactDelayLoader * GetContactDelayLoader() {return _contactDelayLoader;}
private:
     /**
     * 将ContactInfoImpl列表转换成ContactInfo列表
     * @param infoList 转换后的联系人列表，抛给上层使用的contact类型
     * @param implList 被转换的内部使用的contact类型
     */   
	void ConvertContactInfo(OUT std::list<ContactInfo*> &infoList, 
    	IN OUT std::list<ContactInfoImpl*>& implList);

    /**
     * 指定page，lastSyncTime，newSyncTime，更新常用联系人信息
     * @param page 分页号
     * @param lastSyncTime 上次成功更新时间
     * @param newSyncTime 更新成功结束，通过该值更新lastSyncTime
     */
	void GetUpdatedContacts(int page, uint64_t lastSyncTime, uint64_t newSyncTime);

    /**
     * 在数据库中保存上一次常用联系人更新的时间
     * @param newSyncTime 上一次常用联系人更新的时间
     * @return 保存结果
     */
    int32_t SaveLastContactSyncTime(uint64_t newSyncTime);
    
    /**
     * 保存联系人到本地。contact的type已确定
     * @param contact 联系人信息
     */
	int32_t SaveContactIntoCache(ContactInfoImpl& contact);

	/**
     * 批量保存联系人到本地。contact的type已确定
     * @param contact 联系人信息列表
     */
	int32_t SaveContactIntoCache(std::list<ContactInfoImpl*>& contact);
    
    /**
     * 在本地添加常用联系人的tag
     * @param tag 标签
     */
    void AddContactTag(const ContactTag& tag);
    
    /**
     * 更新本地常用联系人的tag
     * @param tag 标签
     */
	void updateContactTag(const ContactTag& tag);

	/**
     * 删除本地常用联系人的tag
     * @param tag 标签
     */
	void deleteContactTag(const ContactTag& tag);

    // 以下是网络事件的回调
	void SearchContactFromServer(const std::string& keyword, int customerCode, 
		int scope, int page);

	void GetColleaguesFromServer(int32_t userId, int page);

	int32_t ContactInfoReceived(int32_t errorCode, HTTPResponse *response, 
		std::list<int32_t>* userIds, CONTACT_INFO_RECEIVED_EVENT dispatchWhich,bool isSave);

	int32_t ContactDeleted(int32_t errorCode, HTTPResponse *response, int32_t userId);

	int32_t ContactAdded(int32_t errorCode, HTTPResponse *response, int32_t userId);

	int32_t ContactSearched(int32_t errorCode, HTTPResponse *response, std::string* keyword, int customerCode, int scope, int page);


	int32_t UserTagsResponse(int32_t errorCode, HTTPResponse *response);
	
	int32_t UserTagsUpdated(int32_t errorCode, HTTPResponse *response, std::string useTags);
	
	int32_t ColleagueReceived(int32_t errorCode, HTTPResponse *response, int32_t userId, int page);

	int32_t OrgAddrListReceived(int32_t errorCode, HTTPResponse *response, int32_t taskId);

	int32_t OrgAddrMembersReceived(int32_t errorCode, HTTPResponse *response, int orgId, std::string copyback);

	int32_t ContactUpdated(int32_t errorCode, HTTPResponse *response, int page, uint64_t lastSyncTime, uint64_t newSyncTime);

	int32_t UserInfoUpdated(int32_t errorCode, HTTPResponse *response, boost::property_tree::wptree* ptParse);

	int32_t UserStatusSetResponse(int32_t errorCode, HTTPResponse *response, int32_t userId, int32_t status, std::string show);

	int32_t UserStatusResponse(int32_t errorCode, HTTPResponse *response, StatusOperation opt, std::list<int32_t> userIds);

	int32_t UserStatusGetResponse(int32_t errorCode, HTTPResponse *response, StatusOperation opt, std::string copyBack);

public:
	void UpdateUserStatus(const UserStatusList& infos);

	bool GetBasicContactInfo(const string& account, BasicContactInfo& contact);

	int32_t MakeRequestId() {
		static int32_t temp = 1;
		return ((int32_t)time(NULL) + temp++);
	}
private:
	
	//UI订阅用户状态时，先判读并更新cache中计数;
	int32_t _GetUserStatus(std::list<int32_t>& userIds, UserStatusList& infoList);
	int32_t _GetUserStatus(int32_t userId, UserStatus& info);

	//UI取消订阅用户状态时，先判读并更新memery cache中计数;
	int32_t _UnsubscribeUserStatus(std::list<int32_t>& userIds);
	int32_t _UnsubscribeUserStatus(int32_t userId);

	//向Server取消订阅用户状态时，更新memery cache;
	void _UpdateUserStatus(std::list<int32_t>& userIds);
	
	//向Server订阅用户状态时，更新memery cache;
	void _UpdateUserStatus(UserStatusList infos);

	//取消订阅全部用户状态时，清空memery cache;
	void _ClearUserStatus();

	void _UpdateAccountAvatar(const std::string& userAccount, const std::string& localAvatarPath);
    
private:
	enum ContactUpdateStatus {
		CUS_NOT_START, CUS_UPDATING, CUS_UPDATED, CUS_UPDATED_ERROR
	};

	ContactUpdateStatus _updateStatus;
    int64_t _lastContactSyncTime;
    
    ContactCache *_contactCache;
	ContactDelayLoader *_contactDelayLoader;

	ContactRequestFactory *_requestFactory;
	ContactResponseParser _responseParser;
    
	ContactEventDispatcher *_dispatcher;
    ContactMsgProcessor *_processor;

	boost::recursive_mutex		_user_status_mutex;
	UserStatusMap  _user_status_cache;	
};

} /* namespace uc */
#endif /* CONTACTSERVICE_H_ */
