///////////////////////////////////////////////////////////
//  GroupDao.cpp
//  Implementation of the Class GroupDao
//  Created on:      22-五月-2015 10:35:36
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "GroupDao.h"
#include "LogManager.h"

namespace uc {

GroupDao::GroupDao() {
	_db = SQLiteManager::GetInstance().GetUserDB();
}

GroupDao::~GroupDao() {
	SQLiteManager::GetInstance().ReleaseUserDB(_db);
}

int GroupDao::SaveGroupInfo(const GroupInfo& data) {
	//group 基本信息
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_DISCUSSION_GROUP(GROUP_ID, GROUP_NAME, "
		"GROUP_NAME_EN, AVATAR_URL, LOCAL_AVATAR_URL, JOIN_STATE, CREATE_TIME, MEMBER_COUNT, SITE_ID, "
		"IS_TOP, NAME_FLAG, IS_DISPLAY, MEMBER_TYPE, MSG_NOT_DISTURB, STATE, ADMIN_ID, MOUNT_ID, EXTERNAL_ID, "
		"ALERT_SWITCH, ALERT_RULE, ALERT_TIME, MODIFY_TIME, STATUS_INFO) "
		"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? ,?);");
	int i = 1;
	stmt.bind(i++, data.GetId());
	stmt.bind(i++, data.GetName().c_str());
	stmt.bind(i++, data.GetPinYinName().c_str());
	stmt.bind(i++, data.GetAvatarIndex().c_str());
	stmt.bind(i++, data.GetLocalAvatar().c_str());
	stmt.bind(i++, data.GetJoinState());
	stmt.bind(i++, data.GetCreateTime());
	stmt.bind(i++, data.GetMemberCount());
	stmt.bind(i++, data.GetSiteId());
	stmt.bind(i++, data.GetGroupTop());
	stmt.bind(i++, data.GetNameFlag());
	stmt.bind(i++, data.GetGroupType());
	stmt.bind(i++, data.GetMemberType());
	stmt.bind(i++, data.GetGroupDisturb());
	stmt.bind(i++, data.GetGroupStatus());
	stmt.bind(i++, data.GetAdminId());
	stmt.bind(i++, data.GetMountId());
	stmt.bind(i++, data.GetExternalId().c_str());
	stmt.bind(i++, data.GetGroupAlert());
	stmt.bind(i++, data.GetAlertRule());
	stmt.bind(i++, data.GetAlertTime());
	stmt.bind(i++, data.GetModifyTime());
	stmt.bind(i++, data.GetStatusInfo().c_str());
	int ret = ExecDML(stmt); 
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveGroup return error:" << ret
			<< "; GROUP_ID:" << data.GetId()
			<< ", GROUP_NAME: " << data.GetName()
			<< ", GROUP_NAME_EN:" << data.GetPinYinName()
			<< ", AVATAR_URL:" << data.GetAvatarIndex()
			<< ", LOCAL_AVATAR_URL:" << data.GetLocalAvatar()
			<< ", JOIN_STATE:" << data.GetJoinState()
			<< ", CREATE_TIME:" << data.GetCreateTime()
			<< ", MEMBER_COUNT:" << data.GetMemberCount()
			<< ", SITE_ID:" << data.GetSiteId()
			<< ", IS_TOP:" << data.GetGroupTop()
			<< ", NAME_FLAG:" << data.GetNameFlag()
			<< ", IS_DISPLAY:" << data.GetGroupType()
			<< ", MEMBER_TYPE:" << data.GetMemberType()
			<< ", MSG_NOT_DISTURB:" << data.GetGroupDisturb()
			<< ", STATE:" << data.GetGroupStatus()
			<< ", ADMIN_ID:" << data.GetAdminId()
			<< ", MOUNT_ID:" << data.GetMountId()
			<< ", EXTERNAL_ID" << data.GetExternalId()
			<< ", ALERT_SWITCH:" << data.GetGroupAlert()
			<< ", ALERT_RULE:" << data.GetAlertRule()
			<< ", ALERT_TIME:" << data.GetAlertTime()
			<< ", MODIFY_TIME:" << data.GetModifyTime()
			<< ", STATUS_INFO:" << data.GetStatusInfo();
		return DBError::DB_INSERT_ERROR;
	}

	//group 关联成员信息,先清除旧的讨论组成员数据，在保存新的成员数据；
	ret = UpdateMember(data.GetId(), data.GetMember());
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateMember return error:" << ret << ", groupID:" << data.GetId();
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
} 

int GroupDao::SaveGroup(const list<GroupInfo>& data) {
	int ret = DBError::DB_SUCCESS;
	ExecDML("BEGIN TRANSACTION;", _db);
	for (list<GroupInfo>::const_iterator itor = data.begin();
		itor != data.end(); ++itor) {
		ret = SaveGroupInfo(*itor);
		if (ret != 0) {
			break;
		}
	}
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}

int GroupDao::GetAllGroup(list<GroupInfo> & data)
{
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_DISCUSSION_GROUP ;");
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetAllGroup return error:" << ret;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetAllGroup return db empty! ";
		return DBError::DB_QUERY_EMPTY;
	}

	while (!query.eof()) {
		GroupInfo info;
		SetGroupInfo(query, info);
		data.push_back(info);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;

}

int GroupDao::GetGroup(int64_t groupId, GroupInfo& data) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_DISCUSSION_GROUP WHERE GROUP_ID = ?;");
	stmt.bind(1, groupId);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetGroup return error:" << ret << ", groupId:" << groupId;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		SetGroupInfo(query, data);
	} else {
		UC_LOG(ERROR) << "GetGroup return db empty! groupId:" << groupId;
		return DBError::DB_QUERY_EMPTY;
	}

	return DBError::DB_SUCCESS;
}

void GroupDao::SetGroupInfo(CppSQLite3Query& query, GroupInfo& data) {
	int i = 0;
	data.SetId(atoi(query.fieldValue(i++)));
	data.SetName(query.fieldValue(i++));
	data.SetPinYinName(query.fieldValue(i++));
	data.SetAvatarIndex(query.fieldValue(i++));
	data.SetLocalAvatar(query.fieldValue(i++));
	data.SetJoinState(atoi(query.fieldValue(i++)));
	data.SetCreateTime(atoi(query.fieldValue(i++)));
	data.SetMemberCount(atoi(query.fieldValue(i++)));
	data.SetSiteId(atoi(query.fieldValue(i++)));
	data.SetGroupTop(atoi(query.fieldValue(i++)));
	data.SetNameFlag(atoi(query.fieldValue(i++)));
	data.SetGroupType(atoi(query.fieldValue(i++)));
	data.SetMemberType(atoi(query.fieldValue(i++)));
	data.SetGroupDisturb(atoi(query.fieldValue(i++)));
	data.SetGroupStatus(atoi(query.fieldValue(i++)));
	data.SetAdminId(atoi(query.fieldValue(i++)));
	data.SetMountId(atoi(query.fieldValue(i++)));
	data.SetExternalId(query.fieldValue(i++));
	data.SetGroupAlert(atoi(query.fieldValue(i++)));
	data.SetAlertRule(atoi(query.fieldValue(i++)));
	data.SetAlertTime(atoi(query.fieldValue(i++)));
	data.SetModifyTime(atoi(query.fieldValue(i++)));
	data.SetStatusInfo(query.fieldValue(i++));	

	std::list<MemberInfo> memberList;
	GetMember(data.GetId(), memberList);
	data.AddMember(memberList);

	// 过滤解析group bug引起的脏数据
	if (data.GetGroupType() != 3) {
		data.SetMountId(0);
	}
	if (data.GetGroupType() != 2){
		data.SetExternalId("");
	}
}

int GroupDao::GetGroup(const string& keyWord, int32_t page, int32_t count, list<GroupInfo>& groupList) {
	int offset = (page > 0)? (page - 1)*count : 0;
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_DISCUSSION_GROUP WHERE (GROUP_NAME LIKE ? OR GROUP_NAME_EN LIKE ?) ORDER BY GROUP_NAME_EN ASC LIMIT ?, ?;");
	string keyName = "%" + keyWord + "%";

	int i = 1;
	stmt.bind(i++, keyName.c_str());
	stmt.bind(i++, keyName.c_str());
	stmt.bind(i++, offset);
	stmt.bind(i++, count);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetGroup return error:" << ret << ", keyWord:" << keyWord;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetGroup return db empty! keyWord:" << keyWord;
		return DBError::DB_QUERY_EMPTY;
	}

	while (!query.eof()) {
		GroupInfo info;
		SetGroupInfo(query, info);
		groupList.push_back(info);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

int GroupDao::GetMember(int64_t groupId, list<MemberInfo>& members) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT USER_ID, JOIN_TIME FROM TBL_GROUP_MEMBER_RELATION WHERE GROUP_ID = ?;");
	stmt.bind(1, groupId);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetMember return error:" << ret
			<< "; groupId:" << groupId;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetMember return db empty! groupId:" << groupId;
		return DBError::DB_QUERY_EMPTY;
	}

	while (!query.eof()) {
		MemberInfo data;
		int i = 0;
		data.userId = atoi(query.fieldValue(i++));
		data.joinTime = atoi(query.fieldValue(i++));
		members.push_back(data);
		query.nextRow();
	}

	return DBError::DB_SUCCESS;
}

int GroupDao::SetGroupLocalAvatar(int64_t groupId, const string& avatar) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_DISCUSSION_GROUP SET LOCAL_AVATAR_URL = ? WHERE GROUP_ID = ?;");
	int i = 1;
	stmt.bind(i++, avatar.c_str());
	stmt.bind(i++, groupId);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "SetLocalGroupAvatar return error:" << ret 
			<< "; avatar_url:" << avatar
			<< ", group_id:" << groupId;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int GroupDao::SetGroupStatus(int64_t groupId, int state) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_DISCUSSION_GROUP SET STATE = ? WHERE GROUP_ID = ?;");
	int i = 1;
	stmt.bind(i++, state);
	stmt.bind(i++, groupId);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "SetGroupStatus return error:" << ret
			<< ", state:" << state << ", groupId:" << groupId;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int GroupDao::UpdateMember(int64_t groupId, const list<MemberInfo>& members) {
	int ret = DBError::DB_SUCCESS;
	ret = DeleteMember(groupId);
	if (ret != DBError::DB_SUCCESS){
		return ret;
	}
	for (list<MemberInfo>::const_iterator itor = members.begin();
		itor != members.end(); ++itor) {
		ret = AddMember(groupId, *itor);
		if (ret != 0) {
			break;
		}
	}
	return ret;
} 

int GroupDao::DeleteMember(int64_t groupId) {
 	CppSQLite3Statement stmt = _db->compileStatement(
 		"DELETE FROM TBL_GROUP_MEMBER_RELATION WHERE GROUP_ID = ?;");
 	int i = 1;
 	stmt.bind(i++, groupId);
 	int ret = ExecDML(stmt); 
 	stmt.reset();
 	if (ret != 0) {
 		UC_LOG(ERROR) << "DeleteMember return error:" << ret
 			<< ", groupId:" << groupId ;
 		return DBError::DB_DELETE_ERROR;
 	}
	return DBError::DB_SUCCESS;
 }

int GroupDao::AddMember(int64_t groupId, const MemberInfo& member) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_GROUP_MEMBER_RELATION(GROUP_ID, USER_ID, JOIN_TIME) "
		"VALUES(?, ?, ?);");
	int i = 1;
	stmt.bind(i++, groupId);
	stmt.bind(i++, member.userId);
	stmt.bind(i++, member.joinTime);
	int ret = ExecDML(stmt); 
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "AddMemberInfo return error:" << ret
			<< ", groupId:" << groupId << ", userId:" 
			<< member.userId << ", joinTime:" << member.joinTime; 
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
} 

string GroupDao::GetGroupLocalAvatar(int64_t groupId) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT LOCAL_AVATAR_URL FROM TBL_DISCUSSION_GROUP WHERE GROUP_ID = ?;");
	stmt.bind(1, groupId);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetGroupLocalAvatar return error:" << ret
			<< "; groupId:" << groupId;
		return "";
	}

	if (query.eof()) {
		return "";
	}
	return query.fieldValue(0);
}

} //namespace uc
