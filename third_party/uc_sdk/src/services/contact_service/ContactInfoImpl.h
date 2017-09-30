/*
 * ContactInfoImpl.h
 *
 *  Created on: May 5, 2015
 *      Author: gang wang
 */

#ifndef CONTACT_INFO_IMPL_H_
#define CONTACT_INFO_IMPL_H_

#include <string>
#include <boost/thread/recursive_mutex.hpp>
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>

#include "ContactInfo.h"

namespace uc {
    
class ContactInfoImpl {
public:
	~ContactInfoImpl();

	/**
     * 构造默认的ContactInfoImpl
     * @return ContactInfoImpl指针
     */
    static ContactInfoImpl* CreateContactInfoImpl();

    /**
     * 复制ContactInfoImpl对象
     * @return ContactInfoImpl指针
     */
    static ContactInfoImpl* CreateContactInfoImpl(const ContactInfoImpl& other);

	/**
     * 通过profile json数据构造ContactInfoImpl对象，不包含tag
     * @param ptParse json对象
	 * @param flag    消息列表过来的数据和detail_info = 0时为false（不解析user_tags）,其它为true
	 * @return 成功时，返回ContactInfoImpl指针。失败时，返回NULL。
     */
    static ContactInfoImpl* CreateContactInfoImpl(const boost::property_tree::wptree &ptParse, bool flag);
    
	/**
     * 通过json数据修改ContactInfoImpl属性，不包含tag。
     * @param ptParse json对象
	 * @return contact 操作的结果
     */
    int32_t Update(const boost::property_tree::wptree &ptParse);

	/**
     * 通过ContactInfoImpl修改对象属性，不包含tag
     */
    void Update(const ContactInfoImpl& contact);

    ContactInfoImpl& operator=(const ContactInfoImpl& other);
    
    void SetCommon() {
    	_type = CTI_COMMON;
    }

    void SetCurrentUser() {
    	_type = CTI_USER;
    }

    void SetOthers() {
    	_type = CTI_OTHERS;
    }
    
    bool IsCommon() const{
    	return _type == CTI_COMMON;
    }
    
    bool IsCurrentUser() const{
    	return _type == CTI_USER;
    }
    
    bool IsOthers()const {
    	return _type == CTI_OTHERS;
    }
    
    int32_t GetUserId() const {
        return _userId;
    }

    void SetUserId(int32_t userId) {
        _userId = userId;
    }

    int32_t GetSiteId() const {
        return _siteId;
    }

    void SetSiteId(int32_t siteId) {
        _siteId = siteId;
    }

    int64_t GetCardVersion() const {
        return _cardVersion;
    }

    void SetCardVersion(int64_t cardVersion) {
        _cardVersion = cardVersion;
    }

    ContactInfo::Status GetStatus() const {
          return _status;
    }

    void SetStatus(ContactInfo::Status status) {
        _status = status;
    }

    const std::string& GetDisplayName() const {
        return _displayName;
    }

    void SetDisplayName(const std::string& displayName) {
        _displayName = displayName;
    }

    const std::string& GetPingyin() const {
        return _pingyin;
    }

    void SetPingyin(const std::string& pingyin) {
        _pingyin = pingyin;
    }

    void SetSex(ContactInfo::Sex sex) {
        _sex = sex;
    }

    ContactInfo::Sex GetSex() const {
        return _sex;
    }

    const std::string& GetAccount() const {
        return _account;
    }

    void SetAccount(const std::string& account) {
        _account = account;
    }

    const std::string& GetEmail() const {
        return _email;
    }

    void SetEmail(const std::string& email) {
        _email = email;
    }

    const std::string& GetMobile() const {
        return _mobile;
    }

    void SetMobile(const std::string& mobile) {
        _mobile = mobile;
    }

    const std::string& GetWorkPhone() const {
        return _workPhone;
    }

    void SetWorkPhone(const std::string& workPhone) {
        _workPhone = workPhone;
    }

    const std::string& GetLocalAvatar() const {
        return _localAvatar;
    }

    void SetLocalAvatar(const std::string& localAvatar) {
        _localAvatar = localAvatar;
    }

    const std::string& GetRemoteAvatarUrl() const {
        return _remoteAvatar;
    }

    void SetRemoteAvatarUrl(const std::string& remoteAvatar) {
        _remoteAvatar = remoteAvatar;
    }

    const std::string& GetPersonalSign() const {
        return _personalSign;
    }

    void SetPersonalSign(const std::string& personalSign) {
        _personalSign = personalSign;
    }

	const std::string& GetUserTags() const{
		return _usertags;
	}

	void SetUserTags(const std::string& usertags){
		_usertags = usertags;
	}

    int32_t GetDepartmentId() const {
          return _departmentId;
    }

    void SetDepartmentId(int32_t departmentId) {
        _departmentId = departmentId;
    }

    const std::string& GetDepartmentName() const {
        return _departmentName;
    }

    void SetDepartmentName(const std::string& departmentName) {
        _departmentName = departmentName;
    }

    const std::string& GetPosition() const {
        return _position;
    }

    void SetPosition(const std::string& position) {
        _position = position;
    }

    int32_t GetSuperiorId() const {
          return _superiorId;
    }

    void SetSuperiorId(int32_t superiorId) {
          _superiorId = superiorId;
    }

    const std::string& GetSuperiorName() const {
        return _superiorName;
    }

    void SetSuperiorName(const std::string& superiorName) {
        _superiorName = superiorName;
    }

    int32_t GetColleagueNum() const {
          return _colleagueNum;
    }

    void SetColleagueNnum(int32_t colleagueNum) {
        _colleagueNum = colleagueNum;
    }

    ContactInfo::Type GetType() const {
        return _type;
    }

    void SetType(ContactInfo::Type type) {
        _type = type;
    }

    int64_t GetLastUpdateTime() const {
        return _lastUpdateTime;
    }

    void SetLastUpdateTime(int64_t lastUpdateTime) {
        _lastUpdateTime = lastUpdateTime;
    }
    
	bool Compare(const ContactInfoImpl& other);

	int32_t GetRelation() const;

	void SetRelation(int32_t relation);

private:
	ContactInfoImpl();
	ContactInfoImpl(const ContactInfoImpl&);
    
	int32_t _userId;
	int32_t _siteId;

	int64_t  _cardVersion;
    ContactInfo::Status _status;
    
	std::string _displayName;
	std::string _pingyin;
	ContactInfo::Sex _sex;
	std::string _account;
	std::string _email;
	std::string _mobile;
	std::string _workPhone;
	std::string _localAvatar;
	std::string _remoteAvatar;
	std::string _personalSign;
	std::string _usertags;
	
	int32_t 	_departmentId;
	std::string _departmentName;
	std::string _position;
	int32_t 	_superiorId;
	std::string _superiorName;
	int32_t 	_colleagueNum;
	int32_t		_relation;

    int64_t _lastUpdateTime;

    ContactInfo::Type _type;

	boost::recursive_mutex _mtx;
};
    
} /* namespace uc */
#endif /* CONTACT_INFO_H_ */
