/*
 * ContactInfo.h
 *
 *  Created on: June 4, 2015
 *      Author: gang wang
 */

#ifndef CONTACT_INFO_H_
#define CONTACT_INFO_H_

#include <vector>
#include <string>

#include "UCSDK.h"
#include "ContactTag.h"

namespace uc {

/**
 * 联系人类型。CTI_COMMON-常用联系人，CTI_USER-当前登陆用户，
 * CTI_OTHERS-非常用联系人
*/
enum ContactType {
	CTI_COMMON = 0, CTI_USER, CTI_OTHERS
};

enum ContactStatus {
	CS_ONLINE = 1, CS_BUSY, CS_AWAY, CS_MEETING, CS_OFFLINE,CS_ONCALL
};

enum ContactSex {
	CS_MAN = 1, CS_WOMAN
};


class UC_API UserStatus{
public:
	UserStatus():_userId(0), _status(1),
		_refCount(0), _show(""){}
	~UserStatus(){}

	void SetUserId(int32_t userId){ _userId = userId; }
	int32_t GetUserId() const{ return _userId; }

	void SetUserStatus(int32_t status){ _status = status; }
	int32_t GetUserStatus() const{ return _status; }

	void SetRefCount(int32_t refCount){ _refCount = refCount; }
	int32_t GetRefCount() const{ return _refCount; }
	void AddRefCount(int32_t count = 1){ _refCount += count; };
	void SubRefCount(int32_t count = 1){ _refCount -= count; };

	void SetStatusShow(const std::string& show){ _show = show; }
	const std::string& GetStatusShow() const{ return _show; }

private:
	int32_t _userId;
	int32_t _status;
	int32_t _refCount;
	std::string  _show;
};

class ContactInfoImpl;

class UC_API ContactInfo {
public:
	typedef ContactType Type;
	typedef ContactStatus Status;
	typedef ContactSex Sex;

    ContactInfo();
	ContactInfo(const ContactInfo& info);
	ContactInfo(ContactInfoImpl* impl);
	~ContactInfo();

    ContactInfo& operator=(const ContactInfo& other);

    ContactInfo& operator=(const ContactInfoImpl& other);

    void Reset(ContactInfoImpl* impl);

	/**
     * 获取联系人用户id
     * @return 用户id
     */
	int32_t GetUserId() const;

	/**
     * 设置联系人用户id
     * @param userId 用户id
     */
	void SetUserId(int32_t userId);

	/**
     * 获取联系人站点id
     * @return 站点id
     */
	int32_t GetSiteId() const;

	/**
     * 设置联系人站点id
     * @param siteId 站点id
     */
	void SetSiteId(int32_t siteId);

	/**
     * 获取联系人名片版本号
     * @return 名片版本号
     */
	int64_t GetCardVersion() const;

	/**
     * 设置联系人名片版本号
     * @param cardVersion 名片版本号
     */
	void SetCardVersion(int64_t cardVersion);

	/**
     * 获取联系人状态
     * @return 联系人状态
     */
	Status GetStatus() const;

	/**
     * 设置联系人状态
     * @param status 联系人状态
     */
	void SetStatus(Status status);

	/**
     * 获取联系人姓名
     * @return 姓名
     */
	const std::string& GetDisplayName() const;

	/**
     * 设置联系人姓名
     * @param displayName 联系人姓名
     */
	void SetDisplayName(const std::string& displayName);

	/**
     * 获取联系人姓名的拼音
     * @return 姓名的拼音
     */
	const std::string& GetPingyin() const;

	/**
     * 设置联系人姓名的拼音
     * @param pingyin 姓名的拼音
     */
	void SetPingyin(const std::string& pingyin);

	/**
     * 获取联系人性别
     * @return 性别
     */
	Sex GetSex() const;

	/**
     * 设置联系人性别
     * @param sex 性别
     */
	void SetSex(Sex sex);

	/**
     * 获取联系人账号
     * @return 账号
     */
	const std::string& GetAccount() const;

	/**
     * 设置联系人账号
     * @param account 账号
     */
	void SetAccount(const std::string& account);

	/**
     * 获取联系人邮箱名
     * @return 邮箱名
     */
	const std::string& GetEmail() const;

	/**
     * 设置联系人邮箱名
     * @param email 邮箱名
     */
	void SetEmail(const std::string& email);

	/**
     * 获取联系人手机号码
     * @return 手机号码
     */
	const std::string& GetMobile() const;

	/**
     * 设置联系人手机号码
     * @param mobile 手机号码
     */
	void SetMobile(const std::string& mobile);

	/**
     * 获取联系人工作电话
     * @return 工作电话
     */
	const std::string& GetWorkPhone() const;

	/**
     * 设置联系人手机号码
     * @param workPhone 工作电话
     */
	void SetWorkPhone(const std::string& workPhone);

	/**
     * 获取联系人本地头像路径
     * @return 本地头像路径
     */
	const std::string& GetLocalAvatar() const;

	/**
     * 设置联系人本地头像路径
     * @param localAvatar 本地头像路径
     */
	void SetLocalAvatar(const std::string& localAvatar);

	/**
     * 获取联系人远程头像url
     * @return 远程头像url
     */
	const std::string& GetRemoteAvatarUrl() const;

	/**
     * 设置联系人远程头像url
     * @param remoteAvatar 远程头像url
     */
	void SetRemoteAvatarUrl(const std::string& remoteAvatarUrl);

	/**
     * 获取联系人签名
     * @return 签名
     */
	const std::string& GetPersonalSign() const;

	/**
     * 设置联系人签名
     * @param personalSign 签名
     */
	void SetPersonalSign(const std::string& personalSign);

	/**
	* 获取联系人TAG信息
	* @return TAG信息
	*/
	const std::string& GetUserTags() const;

	/**
	* 设置联系人TAG信息
	* @param personalSign TAG信息
	*/
	void SetUserTags(const std::string& userTags);

	/**
     * 获取联系人部门id
     * @return 部门id
     */
	int32_t GetDepartmentId() const;

	/**
     * 设置联系人部门id
     * @param departmentId 部门id
     */
	void SetDepartmentId(int32_t departmentId);

	/**
     * 获取联系人部门名称
     * @return 部门名称
     */
	const std::string& GetDepartmentName() const;

	/**
     * 设置联系人部门名称
     * @param departmentName 部门名称
     */
	void SetDepartmentName(const std::string& departmentName);

	/**
     * 获取联系人职位
     * @return 职位
     */
	const std::string& GetPosition() const;

	/**
     * 设置联系人职位
     * @param position 职位
     */
	void SetPosition(const std::string& position);

	/**
     * 获取联系人上级id
     * @return 上级id
     */
	int32_t GetSuperiorId() const;

	/**
     * 设置联系人上级id
     * @param superiorId 上级id
     */
	void SetSuperiorId(int32_t superiorId);


	/**
     * 获取联系人上级姓名
     * @return 上级姓名
     */
	const std::string& GetSuperiorName() const;

	/**
     * 设置联系人上级姓名
     * @param superiorName 上级姓名
     */
	void SetSuperiorName(const std::string& superiorName);

	/**
     * 获取联系人同事数量
     * @return 同事数量
     */
	int32_t GetColleagueNum() const;

	/**
     * 设置联系人同事数量
     * @param colleagueNum 同事数量
     */
	void SetColleagueNnum(int32_t colleagueNum);
    
	/**
     * 获取联系人类型
     * @return 类型
     */
	Type GetType() const;

	/**
     * 设置联系人同事类型
     * @param type 类型
     */
	void SetType(Type type);

	int32_t GetRelation() const;

	void SetRelation(int32_t relation);
private:

	ContactInfoImpl* _contactInfoImpl;
};

class UC_API BasicContactInfo {
public:
	BasicContactInfo(): _userId(0) {
	}

	BasicContactInfo(int32_t userId, const std::string &account, const std::string &displayName, 
		const std::string &localAvatar, const std::string &remoteAvatar, const std::string &personalSign, 
		const std::string &pinyin, const std::string &position, int32_t departmentId, const std::string &departmentName, 
		int32_t siteId) : _userId(userId), _account(account), _displayName(displayName), _localAvatar(localAvatar),
		_remoteAvatar(remoteAvatar), _personalSign(personalSign), _pinyin(pinyin), _position(position),
		_departmentId(departmentId), _departmentName(departmentName), _siteId(siteId){
		_relation = 0;
	}

	BasicContactInfo(const BasicContactInfo &info) 
		: _userId(info._userId), _account(info._account), _displayName(info._displayName),
		_localAvatar(info._localAvatar), _remoteAvatar(info._remoteAvatar), 
		_personalSign(info._personalSign), _pinyin(info._pinyin), _position(info._position),
        _departmentId(info._departmentId), _departmentName(info._departmentName),_siteId(info._siteId) {
		_relation = info._relation;
	}

    BasicContactInfo(const ContactInfo &info)
		: _userId(info.GetUserId()), _account(info.GetAccount()), _displayName(info.GetDisplayName()), _localAvatar(info.GetLocalAvatar()),
        _remoteAvatar(info.GetRemoteAvatarUrl()), _personalSign(info.GetPersonalSign()), _pinyin(info.GetPingyin()),
		_position(info.GetPosition()), _departmentId(info.GetDepartmentId()), _departmentName(info.GetDepartmentName()), _siteId(info.GetSiteId()){
		_relation = info.GetRelation();
    }

    BasicContactInfo(const ContactInfoImpl* impl);
    
	int32_t GetUserId() const {
        return _userId;
    }

	void SetUserId(int32_t userId) {
        _userId = userId;
    }


	const std::string& GetAccount() const {
		return _account;
	}

	void SetAccount(const std::string& account) {
		_account = account;
	}


    const std::string& GetDisplayName() const {
        return _displayName;
    }

    void SetDisplayName(const std::string& displayName) {
        _displayName = displayName;
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

	const std::string& GetPinyin() const {
        return _pinyin;
    }

    void SetPinyin(const std::string& pinyin) {
        _pinyin = pinyin;
    }

    const std::string& GetPosition() const {
        return _position;
    }

    void SetPosition(const std::string& position) {
        _position = position;
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

	int32_t GetRelation() const {
		return _relation;
	}

	void SetRelation(int32_t relation) {
		_relation = relation;
	}

	int32_t GetSiteId() const {
		return _siteId;
	}

	void SetSiteId(int32_t siteId){
		_siteId = siteId;
	}

private:
	int32_t _userId;
	std::string  _account;
	std::string _displayName;
	std::string _localAvatar;
	std::string _remoteAvatar;
	std::string _personalSign;
	std::string _pinyin;
    std::string _position;
    int32_t     _departmentId;
    std::string _departmentName;
	int32_t		_relation;
	int32_t		_siteId;
};


}

#endif
