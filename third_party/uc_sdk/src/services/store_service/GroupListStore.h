#ifndef __GROUPLLISTSTORE_H__
#define __GROUPLLISTSTORE_H__

#include "UCSDK.h"
#include "IGroupService.h"
#include "UCMessage.h"
#include <boost/thread.hpp>
namespace uc {

#define CURRENT_USER_IN_GROUP       0
#define CURRENT_USER_NOT_IN_GROUP   1
#define GROUP_STATE_OPENED          0
#define GROUP_STATE_CLOSED          1

typedef std::list<GroupInfo> GroupList;

enum  GET_STATUS { 
	GROUPLIST_FROM_MEMORY = 0, 
	GROUPLIST_NEED_FROM_SERVER 
}; 

	

class GroupListStore
{
public:
	GroupListStore() :_synced(false), _lastModifyTime(0){};
	~GroupListStore();
public:

	/**
	* 外部调用从DB里获取数据
	*/

	void LoadDataFromDB();

	/**
	* 获取讨论组列表;
	* @param groupType          请求讨论组的类型;
	* @param page               请求讨论组的页数，默认1;
	* @param count              请求讨论组的数量，默认200;
	* @param lastGroupTime      上一次请求讨论组列表时间，默认0;
	* @param allData		     是否需要所有讨论组信息：true 需要; false: 不需要;
	* @return					 返回状态;
	*/
	GET_STATUS GetGroupList(GroupStyle groupType, int32_t page, int32_t count, int32_t lastGroupTime, bool allData, bool update, OUT GroupList &groupList);

	/**
	* 存储讨论组列表;
	* @param groupList          讨论组列表;
	* @param synced             是否已经同步了，false则需要同步到DB，否则仅仅更新内存
	* @return					
	*/
	void StoreGroupList(GroupList groupList,bool synced);

	/**
	* 获取最后修改时间;
	* @return 最后修改时间 int64_t
	*/
	int64_t GetLastModifyTime();

	/**
	* 更新讨论组， 如果没有在cache_list，则push_back;
	* @param info        讨论组;
	* @return
	*/
	void UpdateGroupInfo(GroupInfo & info);

	/**
	* 更新内存中的Group logo(Avatar), 同时写DB，这是用来更新本地项目组头像的;
	* @param groupId          讨论组Id;
	* @param localPath        logo路径;
	* @return
	*/
	int32_t UpdateGroupLogo(int64_t groupId, const std::string& localPath);

	/**
	* 更新内存中的Group 信息;
	* @param groupid					id;
	* @param name						来自于消息get_group_name;
	* @param pinyiName					来自于消息get_name_pinyin;
	* @return 是否更新了 true 更新， false 未更新
	*/
	bool UpdateGroupName(int64_t groupId, const std::string &name,const std::string & pinyiName);


	/**
	* 更新内存中的Group Avatar Index 信息;
	* @param groupid					id;
	* @param avatarIndex			来自消息的get_avatar
	* @return 是否更新了 true 更新， false 未更新
	*/
	bool UpdateGroupAvatarIndex(int64_t groupId,const std::string & avatarIndex);

	/**
	* 根据groupid和memberId,删除用户
	* @param groupid			id;
	* @param memberId			来自于消息get_memberId;
	* @param isSelf				是否自己
	* @return
	*/
	void DeleteMember(int64_t groupId, const int32_t & memberId,bool isSelf);

	/**
	* 根据groupid删除group;
	* @param groupid					id;
	* @return
	*/
	void CloseGroup(int64_t groupId);

	/**
	* 添加user到group (如果当前group不存在，则不添加);
	* @param groupid					id;
	* @param memberInfo				    用户信息;
	* @param isSelf				        是否自己
	* @return
	*/
	void AddMemberInGroup(int64_t groupId, const MemberInfo& memberInfo, bool isSelf);

	/**
	* 设置免打扰
	* @param groupid					id;
	* @param state				来自于get_state;
	* @return
	*/
	void SetGroupDisturb(int64_t groupId, int32_t state);

	/**
	* 设置Top
	* @param groupid					id;
	* @param state				来自于get_isTop;
	* @return
	*/
	void SetGroupTop(int64_t groupId, int32_t isTop);

	/**
	* 设置Alert条件
	* @param groupid					id;
	* @param state				来自于get_state;
	* @param rule				来自于get_rule;
	* @param time				来自于get_time;
	* @return
	*/
	void SetGroupAlertCondition(int64_t groupId, int8_t state, int32_t rule, int32_t time);

	/**
	* 批量删除group member
	* @param groupid					id;
	* @param userList				需要删除的列表;
	* @return
	*/
	void RemoveMemberInfoById(int64_t groupId, std::vector<int32_t> userList);

	/**
	* 获取GroupInfo
	* @param groupid					id;
	* @param groupInfo				出参，groupInfo;
	* @return							是否有数据， 现在内存cache所有数据，不应该没有
	*/
	bool GetGroupInfo(int64_t groupId, OUT GroupInfo &groupInfo);

	/**
	* 更新group call状态
	* @param groupid					id;
	* @param status				group call状态;
	* @return					
	*/
	bool UpdateStatusInfo(int64_t groupId, const std::string& status);

	/**
	* 获取GetGroupInfoByMountId
	* @param mountId					云盘库id;
	* @param groupInfo				出参，groupInfo;
	* @return							是否有数据， 现在内存cache所有数据，不应该没有
	*/
	bool GetGroupInfoByMountId(int32_t mountId, OUT GroupInfo &groupInfo);
	
   /**
	* 通过外部ID获取讨论组ID
	* @param externalId					外部ID;
	* @param groupInfo				出参，groupInfo;
	* @return							是否有数据， 现在内存cache所有数据，不应该没有;
	*/
	bool GetGroupInfoByExternalId(const string& externalId, GroupInfo& info);

	/**
	* Clear message cache in memory
	*/
	void Clear();

	/**
	* reset _sync flag
	*/
	void ResetSync();

private:
	/*
	* 从DB中将grouplist信息load到内存中
	*/
	void _LoadGroupListFromDB();

	void _UpdateLastModifyTime();

private:

	GroupInfo* _GetGroupInfoById(int64_t groupId);
	GroupInfo* _GetGroupInfoByMountId(int32_t mountId);
	void _RemoveGroupInfoById(int64_t groupId);

	MemberList* _GetMemberListById(int64_t groupId);
	MemberInfo _GetMemberInfoById(int64_t groupId, int32_t userId);

	void _AddGroupMember(int64_t groupId, const MemberInfo& memberInfo);
	void _RemoveMemberListById(int64_t groupId);
	void _RemoveMemberInfoById(int64_t groupId, int32_t userId);
	void _RemoveMemberInfoById(int64_t groupId, std::vector<int32_t> userList);
	void _RemoveMemberInfoById(const GroupInfo& groupInfo, int32_t userId);
	void _SaveGroup(GroupList groupList);

private:
	GroupList					_group_list_cache;
	boost::recursive_mutex		_group_list_mutex;

	GET_STATUS _get_status;
	bool _synced; // 是否和服务器同步了

	int64_t						_lastModifyTime;

};




}

#endif
