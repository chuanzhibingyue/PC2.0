#include "GroupListStore.h"
#include "DBManager.h"
#include <algorithm> 
#include "utils/TaskExecutor.h"
namespace uc{


GroupListStore::~GroupListStore()
{
	Clear();
}

void GroupListStore::LoadDataFromDB()
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	_LoadGroupListFromDB();
}

GET_STATUS GroupListStore::GetGroupList(GroupStyle groupType, int32_t page, int32_t count, int32_t lastGroupTime, bool allData, bool update, OUT GroupList &groupList)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	UC_LOG(INFO) << "GetGroupList: grouptype = " << groupType << ",page=" << page << ",count=" << count << ",lastGroupTime=" << lastGroupTime << ",allData=" << allData << ",_synced=" << _synced;

	// 删除loadGroup的逻辑，考虑到优化，会在loginsuccess的时候调用LoadDataFromDB优先加载数据
// 	// 1. 只从DB load一次
// 	if (!_synced)
// 	{
// 		_LoadGroupListFromDB();
// 	}

	// 2. 填充groupList
	for (GroupInfo & info : _group_list_cache)
	{
		if ((groupType == GROUP_TYPE) && (info.GetGroupType() == MUTILCHAT_GROUP_TYPE || info.GetGroupType() == PROJECT_GROUP_TYPE)){

			if (!allData)
			{
				if (1 == info.GetJoinState() || 1 == info.GetGroupStatus())
					continue;
			}
			groupList.push_back(info);
		}
		else if(info.GetGroupType() == groupType || groupType == -1)
		{
			if (!allData)
			{
				if (1 == info.GetJoinState() || 1 == info.GetGroupStatus())
					continue;
			}
			groupList.push_back(info);
		}
		
	}

	_UpdateLastModifyTime();

	if (_synced && !update) // 已经和服务器同步了
		return GROUPLIST_FROM_MEMORY;
	else
		return GROUPLIST_NEED_FROM_SERVER;
}

void GroupListStore::StoreGroupList(GroupList groupList, bool synced)
{
	if (synced)
	{
		
		TaskExecutor::GetInstance().Schedule(MakeCallback(this, &GroupListStore::_SaveGroup, groupList));
		//DBManager::GetInstance().SaveGroup(groupList);
		_synced = true;
	}
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	if (_group_list_cache.empty())
	{
		_group_list_cache = groupList;
	}
	else
	{
		//需要将groupList中的如果cache中有的，需要替换（不能替换local avatar），没有的append在list后
		for (GroupInfo & info_cache_item : _group_list_cache)
		{
			std::list<GroupInfo>::iterator iter = std::find_if(groupList.begin(), groupList.end(), [&](GroupInfo &info){
				return info.GetId() == info_cache_item.GetId();
			});

			if (iter != groupList.end())
			{
				// 先获取localPath
				std::string localPath = info_cache_item.GetLocalAvatar();
				// 替换groupInfo
				info_cache_item = *iter;
				// localPath并非从服务获取的，因此需要这样做
				info_cache_item.SetLocalAvatar(localPath);
				groupList.erase(iter);
			}
		}
		_group_list_cache.splice(_group_list_cache.end(), groupList);
	}
	_UpdateLastModifyTime();
}

void GroupListStore::_SaveGroup(GroupList groupList){
	int32_t res =  DBManager::GetInstance().SaveGroup(groupList);
	if (res != DBError::DB_SUCCESS) {
		UC_LOG(ERROR) << "Fail to save group info";
	}
}

int64_t GroupListStore::GetLastModifyTime()
{
	return _lastModifyTime;
}

void GroupListStore::UpdateGroupInfo(GroupInfo & info)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	GroupInfo *groupInfo = _GetGroupInfoById(info.GetId());
	if (!groupInfo)
	{
		_group_list_cache.push_back(info);
	}
	else
	{
		// 先获取localPath
		std::string localPath = groupInfo->GetLocalAvatar();
		// 替换groupInfo
		*groupInfo = info;
		// localPath并非从服务获取的，因此需要这样做
		groupInfo->SetLocalAvatar(localPath); 
		
	}
	_UpdateLastModifyTime();
}

int32_t GroupListStore::UpdateGroupLogo(int64_t groupId, const std::string& localPath)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

 	GroupInfo *groupInfo = _GetGroupInfoById(groupId);
 	if (groupInfo){
 		groupInfo->SetLocalAvatar(localPath);
 	}
	else
	{
		GroupInfo info;
		info.SetId(groupId);
		info.SetLocalAvatar(localPath);
		_group_list_cache.push_back(info);

	}
	return DBManager::GetInstance().SetGroupLocalAvatar(groupId, localPath);

}

bool GroupListStore::UpdateGroupName(int64_t groupId, const std::string &name,const std::string & pinyiName)
{
	bool ret = false;
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	GroupInfo *groupInfo = _GetGroupInfoById(groupId);
	if (groupInfo)
	{
		if ( 0 != groupInfo->GetName().compare(name))
		{
			groupInfo->SetName(name);
			groupInfo->SetPinYinName(pinyiName);

			ret = true;
		}
	}

	return ret;
}

bool GroupListStore::UpdateGroupAvatarIndex(int64_t groupId,const std::string & avatarIndex)
{
	bool ret = false;
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	GroupInfo *groupInfo = _GetGroupInfoById(groupId);
	if (groupInfo)
	{
		if (0 != groupInfo->GetAvatarIndex().compare(avatarIndex))
		{
			groupInfo->SetAvatarIndex(avatarIndex);

			ret = true;
		}
	}

	return ret;
}

void GroupListStore::DeleteMember(int64_t groupId, const int32_t & memberId, bool isSelf)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	//从memberlist中删除退出的用户； 如果是自己，需要设置讨论组数据的joinstate状态;
	if (isSelf){
		GroupInfo *groupInfo = _GetGroupInfoById(groupId);
		if (groupInfo){
			groupInfo->SetJoinState(CURRENT_USER_NOT_IN_GROUP);
		}
	}

	_RemoveMemberInfoById(groupId, memberId);
}

void GroupListStore::CloseGroup(int64_t groupId)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	GroupInfo *groupInfo = _GetGroupInfoById(groupId);
	if (groupInfo){
		groupInfo->SetGroupStatus(GROUP_STATE_CLOSED);
	}
}

void GroupListStore::AddMemberInGroup(int64_t groupId, const MemberInfo& memberInfo, bool isSelf)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	if (isSelf){
		GroupInfo *groupInfo = _GetGroupInfoById(groupId);
		if (groupInfo){
			groupInfo->SetJoinState(CURRENT_USER_IN_GROUP);
		}
	}
	_AddGroupMember(groupId, memberInfo);
}

void GroupListStore::SetGroupDisturb(int64_t groupId, int32_t state)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	GroupInfo *groupInfo = _GetGroupInfoById(groupId);
	if (groupInfo)
	{
		groupInfo->SetGroupDisturb(state);
	}
}

void GroupListStore::SetGroupTop(int64_t groupId, int32_t isTop)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	GroupInfo *groupInfo = _GetGroupInfoById(groupId);
	if (groupInfo)
	{
		groupInfo->SetGroupTop(isTop);
	}
}

void GroupListStore::SetGroupAlertCondition(int64_t groupId, int8_t state, int32_t rule, int32_t time)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	GroupInfo *groupInfo = _GetGroupInfoById(groupId);
	if (groupInfo)
	{
		groupInfo->SetGroupAlert(state);
		groupInfo->SetAlertRule(rule);
		groupInfo->SetAlertTime(time);
	}
}

void GroupListStore::RemoveMemberInfoById(int64_t groupId, std::vector<int32_t> userList)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	_RemoveMemberInfoById(groupId, userList);
}

bool GroupListStore::GetGroupInfo(int64_t groupId, OUT GroupInfo &groupInfo)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	GroupInfo *groupInfoTemp = _GetGroupInfoById(groupId);
	if (!groupInfoTemp)
		return false;

	groupInfo = *groupInfoTemp;

	return true;
}

bool GroupListStore::GetGroupInfoByMountId(int32_t mountId, OUT GroupInfo &groupInfo) {
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	GroupInfo *groupInfoTemp = _GetGroupInfoByMountId(mountId);
	if (!groupInfoTemp)
		return false;

	groupInfo = *groupInfoTemp;

	return true;
}


bool GroupListStore::GetGroupInfoByExternalId(const string& externalId, GroupInfo& info){
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	GroupList::iterator it = _group_list_cache.begin();
	for (; it != _group_list_cache.end(); ++it){
		if ((*it).GetExternalId().compare(externalId) == 0){
			info = *it;
			return true;
		}
	}
	return false;
}
bool GroupListStore::UpdateStatusInfo(int64_t groupId, const std::string& status) {
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	GroupInfo *groupInfoTemp = _GetGroupInfoById(groupId);
	if (!groupInfoTemp)
		return false;

	groupInfoTemp->SetStatusInfo(status);
	return true;
}

void GroupListStore::Clear()
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);
	_group_list_cache.clear();
	_synced = false;
}

void GroupListStore::ResetSync(){
	_synced = false;
}

void GroupListStore::_LoadGroupListFromDB()
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	UC_LOG(INFO) << "[group_list_cache] Load group list from DB";
	// 首先清除cache, 因为用户点击登录以后点取消，仍然会收到一个登录成功，如果不清除数据，会有脏数据在cache中。
	_group_list_cache.clear();
	DBManager::GetInstance().GetAllGroup(_group_list_cache);
}

void GroupListStore::_UpdateLastModifyTime()
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	for (GroupInfo &info : _group_list_cache)
	{
		if (info.GetModifyTime() > _lastModifyTime)
			_lastModifyTime = info.GetModifyTime();
	}
}

GroupInfo* GroupListStore::_GetGroupInfoById(int64_t groupId)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	GroupList::iterator it = _group_list_cache.begin();
	for (; it != _group_list_cache.end(); ++it){
		if ((*it).GetId() == groupId){
			return &(*it);
		}
	}
	return NULL;
}

GroupInfo* GroupListStore::_GetGroupInfoByMountId(int32_t mountId) {
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	GroupList::iterator it = _group_list_cache.begin();
	for (; it != _group_list_cache.end(); ++it){
		if ((*it).GetMountId() == mountId){
			return &(*it);
		}
	}
	return NULL;
}

void GroupListStore::_RemoveGroupInfoById(int64_t groupId)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	GroupList::iterator it = _group_list_cache.begin();
	for (; it != _group_list_cache.end(); ++it){
		const GroupInfo &groupInfo = *it;
		if (groupInfo.GetId() == groupId){
			_group_list_cache.erase(it);
			break;
		}
	}
}

MemberList* GroupListStore::_GetMemberListById(int64_t groupId)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	GroupList::iterator it = _group_list_cache.begin();
	for (; it != _group_list_cache.end(); ++it){
		if ((*it).GetId() == groupId){
			return (MemberList*)&((*it).GetMember());
		}
	}

	return NULL;
}

MemberInfo GroupListStore::_GetMemberInfoById(int64_t groupId, int32_t userId)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	MemberList *memberList = _GetMemberListById(groupId);
	if (memberList == NULL)
		return MemberInfo();

	MemberList::iterator it = memberList->begin();
	for (; it != memberList->end(); ++it){
		if ((*it).userId == userId){
			return *it;
		}
	}

	return MemberInfo();
}

void GroupListStore::_AddGroupMember(int64_t groupId, const MemberInfo& memberInfo)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	MemberInfo member = _GetMemberInfoById(groupId, memberInfo.userId);
	if (member.userId != 0)
	{
		UC_LOG(WARNING) << "The user is in group but still want to add... userId=" << member.userId << " groupId=" << groupId;
		return;
	}

	GroupInfo* groupInfo = _GetGroupInfoById(groupId);
	if (groupInfo != NULL)
	{
		groupInfo->AddMember(memberInfo);
		groupInfo->SetMemberCount(groupInfo->GetMemberCount() + 1);
	}
}

void GroupListStore::_RemoveMemberListById(int64_t groupId)
{
	MemberList *memberList = _GetMemberListById(groupId);
	if (memberList == NULL)
		return;

	memberList->clear();
}

void GroupListStore::_RemoveMemberInfoById(int64_t groupId, int32_t userId)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);


	MemberList *memberList = _GetMemberListById(groupId);
	if (memberList == NULL)
		return;

	GroupInfo *info = _GetGroupInfoById(groupId);

	MemberList::iterator it = memberList->begin();
	for (; it != memberList->end(); ++it){
		const MemberInfo &memberInfo = *it;
		if (userId == memberInfo.userId){
			memberList->erase(it);
			info->SetMemberCount(info->GetMemberCount() - 1);
			break;
		}
	}
}

void GroupListStore::_RemoveMemberInfoById(int64_t groupId, std::vector<int32_t> userList)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	for (unsigned int i = 0; i < userList.size(); ++i){
		_RemoveMemberInfoById(groupId, userList[i]);
	}
}

void GroupListStore::_RemoveMemberInfoById(const GroupInfo& groupInfo, int32_t userId)
{
	boost::unique_lock<boost::recursive_mutex> lock(_group_list_mutex);

	_RemoveMemberInfoById(groupInfo.GetId(), userId);
}

}