/*
 * GroupInfo.h
 *
 *  Created on: June 02, 2015
 *      Author: yongshuai cheng
 */

#ifndef  GROUPINFO_H_
#define  GROUPINFO_H_

#include <string>
#include <list>
#include "DocumentInfo.h"
#include "UCSDK.h"

namespace uc{

enum GroupRole{
	GR_Member = 0,
	GR_Admin = 1,
};

struct MemberInfo{
	MemberInfo() : userId(0), joinTime(0), role(GR_Member){}
	MemberInfo(int32_t _userId, int32_t _joinTime): userId(_userId), joinTime(_joinTime) {}

	int32_t       userId;
	int32_t       joinTime;
	GroupRole	  role;
};

enum GroupStatus{

	GROUP_END_STATUS = 1,
	GROUP_NOT_END_STATUS = 2,
	MEMBER_EXIT_STATUS = 3,
	MEMBER_NOT_EXIT_STATUS = 4,
	GROUP_END_MEMBER_EXIT_STATUS = 4
};

typedef std::list<MemberInfo>  MemberList;

class UC_API GroupInfo{
public:
	GroupInfo();
	~GroupInfo();

	//GroupInfo(const GroupInfo& info);

	void SetId(int64_t id){_id = id;}
	int64_t GetId() const {return _id;};

	void SetSiteId(long siteId){_siteId = siteId;}
	int32_t GetSiteId() const {return _siteId;}

	void SetName(const std::string& name){_name = name;}
	std::string GetName() const {return _name;}

	void SetPinYinName(const std::string& name){_pinYinName = name;}
	std::string GetPinYinName() const {return _pinYinName;}

	void SetCreateTime(int32_t createTime){_createTime = createTime;}
	int32_t GetCreateTime() const {return _createTime;}

	void SetAvatarIndex(const std::string& avatarIndex){_avatarIndex = avatarIndex;}
	std::string GetAvatarIndex() const {return _avatarIndex;}
	
	void SetLocalAvatar(const std::string& avatarIndex){_localAvatar = avatarIndex;}
	std::string GetLocalAvatar() const {return _localAvatar;}
	
	void SetJoinState(int32_t state) { _joinState = state; }
	int32_t GetJoinState() const { return _joinState; }

	void SetMemberCount(int32_t memberCount){_memberCount = memberCount;}
	int32_t GetMemberCount() const {return _memberCount;}

	void SetAdminId(int32_t adminId){_adminId = adminId;}
	int32_t GetAdminId() const {return _adminId;}

	void SetGroupAlert(int32_t isAlert){_alertState = isAlert;}
	int32_t GetGroupAlert() const {return _alertState;}

	void SetAlertRule(int32_t alertRule){_alertRule = alertRule;}
	int32_t GetAlertRule() const {return _alertRule;}

	void SetAlertTime(int32_t alertTime){_alertTime = alertTime;}
	int32_t GetAlertTime() const {return _alertTime;}

	void SetGroupTop(int32_t isTop){_topState = isTop;}
	int32_t GetGroupTop() const {return _topState;}

	void SetGroupStatus(int32_t status){_status = status;}
	int32_t GetGroupStatus() const {return _status;}

	void SetGroupDisturb(int32_t noDisturb){_disturbState = noDisturb;}
	int32_t GetGroupDisturb() const {return _disturbState;}

	void SetNameFlag(int32_t nameFlag){_nameFlag = nameFlag;}
	int32_t GetNameFlag() const {return _nameFlag;}

	void SetGroupType(int32_t groupType){_groupType = groupType;}
	int32_t GetGroupType() const {return _groupType;}

	void SetMemberType(int32_t memberType){ _memberType = memberType; }
	int32_t GetMemberType() const { return _memberType; }

	void SetLogoLevel(int32_t logoLevel){_logoLevel = logoLevel;}
	int32_t GetLogoLevel() const {return _logoLevel;}

	void SetModifyTime(int32_t modifyTime){ _modifyTime = modifyTime; }
	int32_t GetModifyTime() const { return _modifyTime; }

	void SetMountId(int32_t mountId){ _mountId = mountId; }
	int32_t GetMountId() const { return _mountId; }

	void SetExternalId(const std::string& externalId){ _externalId = externalId; }
	std::string GetExternalId() const { return _externalId; }

	static int ParseMountId(const std::string &jsonInfo);

	void AddMember(const MemberInfo& info){_memberList.push_back(info);}
 	void AddMember(const MemberList& vcInfo){ _memberList = vcInfo;}
	void RemoveMember(int32_t userId) {
		MemberList::iterator it = _memberList.begin();
		for (;it != _memberList.end(); ){
			MemberInfo &memberInfo = *it;
			if (userId == (*it).userId){
				it = _memberList.erase(it);
				break;
			} else {
				++it;
			}
		}
	}
 	const MemberList& GetMember() const {return _memberList;}

	void SetStatusInfo(const std::string& status) { _statusInfo = status; }
	std::string GetStatusInfo() const { return _statusInfo; }
 
private:

	int64_t			_id;           //讨论组ID;
	int32_t			_siteId;       //讨论组站点ID;
	std::string   	_name;         //讨论组名称;
	std::string   	_pinYinName;   //讨论组拼音名称;
	int32_t        	_createTime;   //讨论组创建时间;
	std::string   	_avatarIndex;  //讨论组头像;
	std::string     _localAvatar;  //本地讨论组头像路径;
	int32_t			_joinState;    //当前用户加入讨论组状态：0:退出/1:加入;
	int32_t			_memberCount;  //讨论组人员个数;
	int32_t			_adminId;      //讨论组管理员;
	int32_t			_alertState;   //当前用户讨论组提醒状态：0:提醒/1:不提醒;
	int32_t			_alertRule;    //当前用户讨论组提醒规则;
	int32_t			_alertTime;    //当前用户讨论组提醒时间;
	int32_t			_topState;     //当前用户讨论组置顶聊天状态:0:取消置顶/1:设置置顶;
	int32_t			_status;       //讨论组状态：0：打开/1：关闭 ;
	int32_t         _memberType;   //讨论组限制成员类型：0：小讨论组/1：大讨论组;
	int32_t			_disturbState; //当前用户讨论组免打扰状态:0:取消打扰/1:设置打扰;
	int32_t			_nameFlag;     //讨论组名称标志;
	int32_t			_groupType;    //讨论组类型;
	int32_t			_logoLevel;    //讨论组logo级别;
	int32_t			_modifyTime; // 讨论组更新时间， 每次动态获取以这个时间为准;
	MemberList		_memberList;   //讨论组成员列表;
	std::string     _statusInfo;   //讨论组扩展信息;
	int32_t			_mountId;		// 云盘id;
	std::string          _externalId;    //外部ID;
};

} // namespace uc


#endif // GROUPINFO_H_
