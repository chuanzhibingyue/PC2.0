/*
 * IContactService.h
 *
 *  Created on: Feb 11, 2015
 *      Author: ethan
 */

#ifndef ICONTACTSERVICE_H_
#define ICONTACTSERVICE_H_

#include <list>

#include "UCSDK.h"
#include "common/event.h"
#include "IUCEngine.h"
#include "ContactInfo.h"
#include "DepartmentInfo.h"

namespace uc {

interface UC_API IContactServiceEvent {
	virtual ~IContactServiceEvent() {};
    
    /**
     * 联系人信息本地加载回调
     * @param result ，加载结果
     */
    virtual void OnContactLoaded(int32_t result) = 0;

    /**
     * 用户状态变更回调
     * @param result 操作结果
     * @param status 用户状态
     */
    virtual void OnUserStatusReceived(int32_t result, const std::list<UserStatus> &status) = 0;

	/**
	* 用户状态get回调
	* @param result 操作结果
	* @param status 用户状态
	* @param copyBack, 调用get接口时传入的值
	*/
	virtual void OnGetUserStatusResponse(int32_t result, const std::list<UserStatus> &status, std::string &copyBack) = 0;
	/**
	 * 获取联系人信息的回调，
	 * @param contacts 联系人信息列表。该容器只保证在OnContactInfo作用域中有效，
	 * 勿在外部引用
	 */
	virtual void OnContactInfoReceived(int32_t result, const std::list<int32_t>* userIds,
        const std::list<ContactInfo*>* contacts) = 0;

    /**
     * 获取联系人基本信息的回调，
     * @param contacts 联系人信息列表。该容器只保证在OnContactInfo作用域中有效，
     * 勿在外部引用
     */
    virtual void OnBasicContactInfoReceived(int32_t result, const std::list<int32_t>* userIds,
        const std::list<BasicContactInfo>* contacts) = 0;

	/**
	 * 添加常用联系人的回调，
	 * @param result 操作的结果
	 * @param userId 联系人的用户ID
	 * 勿在外部引用
	 */
	virtual void OnContactAdded(int32_t result, int32_t userId) = 0;

	/**
	 * 删除常用联系人的回调，触发场景：（1）在contact/inclist接口响应中，有cantact被delete；
     * （2）用户主动调删除常用联系人接口；（3）ucas中接收到其他终端删除常用联系人的消息
	 * @param result 操作的结果
	 * @param userId 联系人的用户ID
	 */
	virtual void OnContactDeleted(int32_t result, int32_t userId) = 0;

	/**
	 * 更新常用联系人的回调，触发场景：（1）主动调contactservice的updateCommonContact接口；
     * （2）处理ucas发来的常用联系人信息变更消息;
	 * @param contacts 联系人信息列表。该容器只保证在OnContactUpdated作用域中有效，
	 * 勿在外部引用
	 */
	virtual void OnContactUpdated(std::list<ContactInfo*> *contact) = 0;
    
    /**
     * 登陆用户信息修改的回调
     * @param result 操作的结果
     * @param updateField 更新的字段
     */
	virtual void OnUserInfoUpdated(int32_t result, const ContactInfo *info) = 0;
 
    /**
     * 用户标签更新的回调
     * @param result 操作的结果
     * @param updateField 更新的字段
     */
	virtual void OnUserTagsUpdated(int32_t result, const std::string &userTags) = 0;

    /**
     * 后台管理员增加标签的回调
     * @param tags 增加的标签
     */
    virtual void OnSystemTagsAdded(std::vector<ContactTag*>* tags) = 0;
    
    /**
     * 后台管理员修改标签的回调
     * @param tags 更新的标签
     */
    virtual void OnSystemTagsUpdated(std::vector<ContactTag*>* tags) = 0;
    
    /**
     * 后台管理员删除标签的回调
     * @param tagIds 删除的标签
     */
    virtual void OnSystemTagsDeleted(std::vector<int32_t>& tagIds) = 0;

	/**
	*用户状态设置的回调
	* @param result 操作的结果
	* @param userId 用户ID
	* @param status 用户状态
	*/
	virtual void OnUserStatusSet(int32_t result, int32_t userId, int32_t status) = 0;


	/**
	 * 获取同事列表的回调，
	 * @param result 操作的结果
	 * @param userId 联系人的用户ID
	 * @param page 分页获取的页数
	 * @param colNum 同事数量
	 * @param leaderNum 上级数量
	 * @param colleagues 同事列表。该容器只保证在onColleagueReceived作用域中有效，
	 * 勿在外部引用
	 * @param leaders 上级列表。该容器只保证在onColleagueReceived作用域中有效，
	 * 勿在外部引用
	 */
	virtual void OnColleagueReceived(int32_t result, int32_t userId, int32_t page,
		int32_t colNum, int32_t leaderNum, std::list<ContactInfo*> *colleagues,
		std::list<ContactInfo*> *leaders) = 0;
	
	/**
	 * 获取组织通讯录的回调，
	 * @param result 操作的结果
	 * @param department 部门信息。只保证在onOrgAddrListReceived作用域中有效，
	 * 勿在外部引用
	 * @param myDepartment 用户自己的部门信息。只保证在onOrgAddrListReceived作用域中有效，
	 * 勿在外部引用
	 */
	virtual void OnOrgAddressBookReceived(int32_t result, DepartmentInfo *department,
		DepartmentInfo *myDepartment, int32_t taskId) = 0;
    
    /**
     * 获取部门所有成员的回调
     * @param result 操作的结果
     * @param orgId 部门id
     * @param userCount 返回的数量
     * @param memberIds 成员id列表。只保证在onOrgAddrListReceived作用域中有效，
     * 勿在外部引用
	 * @param copyBack, 调用get接口时传入的值
     */
    virtual void OnOrgAddrMembersReceived(int32_t result, int orgId, int userCount, 
		std::vector<int32_t> *memberIds, std::string& copyback) = 0;

    /**
     * 部门名称变更的回调
     * @param operatorId 操作发起人用户ID
     * @param deptId 部门ID
     * @param oldDeptName 原部门名称
     * @param newDeptName 新部门名称
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnDeptUpdated(int32_t operatorId, int32_t deptId, 
        const std::string &oldDeptName, const std::string &newDeptName,
        const std::string &desc, int64_t timestamp) = 0;
    
    /**
     * 员工部门调动的回调
     * @param operatorId 操作发起人用户ID
     * @param userId :用户ID
     * @param userName 用户姓名
     * @param deptId 新部门ID
     * @param oldDeptName 旧部门名称
     * @param newDeptName 新部门名称
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnDeptTransfered(int32_t operatorId, int32_t userId, 
        const std::string &userName, int32_t deptId, const std::string &oldDeptName,
        const std::string &deptName, const std::string &desc, int64_t timestamp) = 0;
    
    /**
     * 员工职位调整的回调
     * @param operatorId 操作发起人用户ID
     * @param userId :用户ID
     * @param userName 用户姓名
     * @param newPosition 新职位名称
     * @param oldPosition 旧职位名称
     * @param deptName 职位所在部门名称
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnPositionUpdated(int32_t operatorId, int32_t userId, 
        const std::string &userName, const std::string &newPosition,
        const std::string &oldPosition, const std::string& deptName,
        const std::string& desc, int64_t timestamp) = 0;
    
    /**
     * 员工入职的回调
     * @param operatorId 操作发起人用户ID
     * @param userId :员工用户ID
     * @param deptId 入职部门ID
     * @param userName 员工姓名
     * @param deptName 员工部门名称
     * @param position 员工职位名称
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnEmployeeEntry(int32_t operatorId, int32_t userId, int32_t deptId, 
        const std::string &userName, const std::string &deptName,
        const std::string &position, const std::string &desc, int64_t timestamp) = 0;
    
    /**
     * 员工离职的回调
     * @param operatorId 操作发起人用户ID
     * @param userId :员工用户ID
     * @param userName 员工姓名
     * @param deptName 员工部门名称
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnEmployeeLeaved(int32_t operatorId, int32_t userId,
        const std::string &userName, const std::string &deptName,
        const std::string &desc, int64_t timestamp) = 0;
    
    /**
     * 员工权限变更的回调
     * @param operatorId 操作发起人用户ID
     * @param userId :员工用户ID
     * @param userName 员工姓名
     * @param deptName 员工部门名称
     * @param deptId 用户权限变更部门ID
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnEmployeeRightsUpdated(int32_t operatorId, int32_t userId, 
        const std::string &userName, const std::string &deptName, int32_t deptId,
        const std::string &desc, int64_t timestamp) = 0;
    
    /**
     * 部门整体移动的回调
     * @param operatorId 操作发起人用户ID
     * @param deptId 移动的部门ID
     * @param deptName 移动的部门名称
     * @param oldDeptName 原上级部门名称
     * @param newDeptName 新上级部门名称
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnDeptParentChanged(int32_t operatorId, int32_t deptId, 
        const std::string &deptName, const std::string &oldDeptName,
        const std::string &newDeptName, const std::string &desc, int64_t timestamp) = 0;
    
    /**
     * 员工被指定为生态企业员工的回调
     * @param operatorId 操作发起人用户ID
     * @param companyId 生态企业ID
     * @param userId 用户ID
     * @param companyName 生态企业名称
     * @param userName 用户姓名
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnCompanyJoined(int32_t operatorId, int32_t companyId, int32_t userId, 
        const std::string &companyName, const std::string &userName,
        const std::string &desc, int64_t timestamp) = 0;
    
    /**
     * 部门删除的回调
     * @param operatorId 操作发起人用户ID
     * @param deptId 删除的部门ID
     * @param deptName 删除的部门名称
     * @param parentDeptName 所在父级部门名称
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnDeptDeleted(int32_t operatorId, int32_t deptId, 
        const std::string &deptName, const std::string &parentDeptName,
        const std::string &desc, int64_t timestamp) = 0;
    
    /**
     * 管理员确认员工入职消息的回调
     * @param operatorId 操作发起人用户ID
     * @param deptId 入职部门ID
     * @param userName 员工姓名
     * @param deptName 员工部门名称
     * @param userId 入职员工ID
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnEmployeeEntryConfirmed(int32_t operatorId, int32_t deptId, 
        const std::string &userName, const std::string &deptName,
        int32_t userId, std::string &desc, int64_t timestamp) = 0;
    
    /**
     * 管理员确认员工离职消息的回调
     * @param operatorId 操作发起人用户ID
     * @param userId 员工用户ID
     * @param deptId 入职部门ID
     * @param userName 员工姓名
     * @param deptName 员工部门名称
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnEmployeeLeaveConfirmed(int32_t operatorId, int32_t userId, int32_t deptId, 
        const std::string &userName, const std::string &deptName,
        const std::string &desc, int64_t timestamp) = 0;
    
    /**
     * 管理员确认员工调岗(部门变更)消息的回调
     * @param operatorId 操作发起人用户ID
     * @param userId 员工用户ID
     * @param userName 员工姓名
     * @param deptName 员工部门名称
     * @param desc 消息描述
	 * @param timestamp 时间戳
     */
    virtual void OnDeptTransferConfirmed(int32_t operatorId, int32_t userId, 
        const std::string &userName, const std::string &deptName,
        const std::string &desc, int64_t timestamp) = 0;

};

interface UC_API IContactService: public IUCService, Event<IContactServiceEvent> {
    IContactService() {}
    virtual ~IContactService() {};

	/**
	 * 通过服务器更新本地常用联系人信息，更新成功后会在OnContactUpdated中发通知
	 */
	virtual void GetUpdatedContacts() = 0;

	/**
	 * 获取联系人信息(所有联系人)。取得信息后，通过OnContactInfoReceived事件通知调用模块
	 * @param userId 联系人用户id
     * @param contact 如果本地缓存中存在该联系人信息，通过该参数返回
     * @return 本地缓存如果存在则返回true;
	 */
	virtual bool GetContactInfo(int32_t userId, OUT ContactInfo& contact) = 0;

    /**
     * 批量获取联系人信息(所有联系人)。取得信息后，通过OnContactInfoReceived事件通知调用模块
     * @param userIds 联系人用户id列表
     */
    virtual void GetContactInfo(const std::list<int32_t> &userIds) = 0;
    
	/**
	 * 获取联系人基本信息。取得信息后，通过OnBasicContactInfoReceived事件通知调用模块
	 * @param userId 联系人用户id
     * @param contact 如果本地缓存中存在该联系人信息，通过该参数返回
	 * @return 本地缓存如果存在则返回true;
	 */
	virtual bool GetBasicContactInfo(int32_t userId, OUT BasicContactInfo& contact) = 0;

    /**
     * 获取联系人基本信息。取得信息后，通过OnOnBasicContactInfoReceived事件通知调用模块
     * @param userIds 联系人用户id
     */
    virtual void GetBasicContactInfo(const std::list<int32_t> &userIds) = 0;

    /**
     * 批量获取常用联系人
     * @param position 起始位置
     * @param len 个数。如果为0，则取到末尾
     * @return 操作结果
     */
    virtual int32_t GetContactList(unsigned int position, unsigned int len, OUT std::vector<BasicContactInfo>& contact) = 0;

    /**
     * 修改联系人的头像路径
     * @param userIds 联系人用户id
     * @param remoteAvatarUrl 头像url地址
     * @param localAvatarPath 头像本地路径
     */
    virtual void UpdateContactAvatar(int32_t userId, const std::string& remoteAvatarUrl,
        const std::string& localAvatarPath) = 0;
    
	/**
	 * 添加常用联系人
	 * @param userId 常用联系人用户id
	 */
	virtual void AddCommonContact(int32_t userId) = 0;

	/**
	 * 删除常用联系人
	 * @param userId 常用联系人用户id
	 */
	virtual void DeleteCommonContact(int32_t userId) = 0;

	/**
	 * 搜索数据库联系人
	 * @param type 联系人类型
	 * @param keyword 搜索关键词
	 * @param contacts 保存搜索到的联系人信息
	 */
    virtual int32_t SearchLocalContact(ContactInfo::Type type, const std::string& keyword,
		OUT std::list<ContactInfo>& contacts) = 0;

    /**
     * 修改登录用户的信息
     * @param contact 保存用户信息
     * @return 成功查找，返回true。否则返回false。
     */
    virtual bool GetCurrentUserInfo(ContactInfo &contact) = 0;

	/**
	* 获取当前用户全部信息，包含tag信息
	* @param contact 保存用户信息
	* @return 成功查找，返回true。否则返回false。
	*/
	virtual bool GetCurrentUserFullInfo(ContactInfo &contact) = 0;

	/**
	 * 修改登录用户的信息
	 * @param updateField 需要修改的字段和信息
	 */
	virtual void UpdateUserInfo(const std::string& updateField) = 0;

    /**
     * 更新用户标签
     * @param targetUserId 目标用户的id
     * @param updateTags   需更新的标签信息
     */
	virtual void UpdateUserTags(const std::string& updateTags) = 0;
    
	/**
	 * 请求同事列表
	 * @param userId 查询用户的id
	 */
	virtual void GetColleagues(int32_t userId) = 0;

	/**
	 * 请求组织通讯录
	 * @param orgId 部门id
	 */
	virtual void GetOrgAddressBook(int orgId, int32_t& taskId) = 0;

    /**
     * 根据传递的组织id获取当前组织及子组织的所有成员及总数
     * @param orgId 部门id
     * @param limit 返回数量限制
     */
    virtual void GetOrgMembers(int orgId, int limit, std::string& copyback) = 0;

	/**
	* 设置用户状态
	* @param userId 用户id
	* @param status 用户状态
	* @param show 状态提示语
	*/
	virtual int32_t SetUserStatus(int32_t userId, int32_t status, const std::string& show) = 0;

	/**
	* 订阅用户状态
	* @param userIds 用户id
	* @return 操作结果
	*/
	virtual int32_t SubscribeUserStatus(std::list<int32_t>& userIds) = 0;

	/**取消订阅用户状态
	* @param userIds 用户id
	* @return 操作结果
	*/
	virtual int32_t UnsubscribeUserStatus(std::list<int32_t>& userIds) = 0;

	/**
	* 取消订阅全部用户状态
	* @return 操作结果
	*/
	virtual int32_t UnsubscribeAllUserStatus() = 0;

	/**
	* 获取用户状态（单次）
	* @param userIds 用户id列表
	* @param copyBack 传入的参数，原封不动的通过回调返回
	*/
	virtual int32_t GetUserStatus(const std::list<int32_t>& userIds, IN std::string & copyBack) = 0;

};

}

#endif /* ICONTACTSERVICE_H_ */
