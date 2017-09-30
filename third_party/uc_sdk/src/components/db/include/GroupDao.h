///////////////////////////////////////////////////////////
//  GroupDao.h
//  Implementation of the Class GroupDao
//  Created on:      22-五月-2015 10:35:36
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(GROUPDAO_H_)
#define GROUPDAO_H_

#include "AppDao.h"
#include "GroupInfo.h"

#include <list>

namespace uc {

using namespace std;


class GroupDao : public AppDao {
public:
	GroupDao();
	virtual ~GroupDao();

	int SaveGroup(const list<GroupInfo>& data);
	int GetAllGroup(list<GroupInfo> & data);
	int GetGroup(int64_t groupId, GroupInfo& data);
	int GetGroup(const string& keyWord, int32_t page, int32_t count, list<GroupInfo>& groupList);
	int GetMember(int64_t groupId, list<MemberInfo>& members);
	int SetGroupLocalAvatar(int64_t groupId, const string& avatar);
	int SetGroupStatus(int64_t groupId, int state);
	string GetGroupLocalAvatar(int64_t groupId);

private:
	void SetGroupInfo(CppSQLite3Query& query, GroupInfo& data);
	int UpdateMember(int64_t groupId, const list<MemberInfo>& members);
	int DeleteMember(int64_t groupId);
	int AddMember(int64_t groupId, const MemberInfo& member);
	int SaveGroupInfo(const GroupInfo& data);

private:
	CppSQLite3DB* _db;

};

}
#endif // !defined(GROUPDAO_H_)
