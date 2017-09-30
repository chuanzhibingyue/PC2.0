///////////////////////////////////////////////////////////
//  ContactDao.cpp
//  Implementation of the Class ContactDao
//  Created on:      22-五月-2015 10:35:26
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "ContactDao.h"
#include "LogManager.h"
#include "DBManager.h"
#include "utils/Macro.h"
#include <boost/filesystem.hpp>

namespace uc {

using namespace std;

ContactDao::ContactDao() {
	_db = SQLiteManager::GetInstance().GetUserDB();
}

ContactDao::~ContactDao() {
	SQLiteManager::GetInstance().ReleaseUserDB(_db);
}

int ContactDao::SaveContact(const ContactInfoImpl& data) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_CONTACT(USER_ID, USER_ACCOUNT, DISPLAY_NAME, DISPLAY_EN, " 
		"SEX, DEPT_ID, DEPT_NAME, POSITION, WORK_PHONE, MOBILE, EMAIL, SUPERIOR_ID, SUPERIOR_NAME, "
		"SITE_ID, PERSONAL_SIGN, AVATAR_URL, TYPE, FELLOW_NUM, CARD_VERSION, LAST_UPDATE_TIME, USER_TAGS) "
		"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	int i = 1;
	stmt.bind(i++, data.GetUserId());
	stmt.bind(i++, data.GetAccount().c_str());
	stmt.bind(i++, data.GetDisplayName().c_str());
	stmt.bind(i++, data.GetPingyin().c_str());
	stmt.bind(i++, data.GetSex());
	stmt.bind(i++, data.GetDepartmentId());
	stmt.bind(i++, data.GetDepartmentName().c_str());
	stmt.bind(i++, data.GetPosition().c_str());
	stmt.bind(i++, data.GetWorkPhone().c_str());
	stmt.bind(i++, data.GetMobile().c_str());
	stmt.bind(i++, data.GetEmail().c_str());
	stmt.bind(i++, data.GetSuperiorId());
	stmt.bind(i++, data.GetSuperiorName().c_str());
	stmt.bind(i++, data.GetSiteId());
	stmt.bind(i++, data.GetPersonalSign().c_str());
	stmt.bind(i++, data.GetRemoteAvatarUrl().c_str());
	stmt.bind(i++, data.GetType());
	stmt.bind(i++, data.GetColleagueNum());
	stmt.bind(i++, data.GetCardVersion());
	stmt.bind(i++, data.GetLastUpdateTime());
	stmt.bind(i++, data.GetUserTags().c_str());
	int ret = ExecDML(stmt); 
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveContact return error:" << ret
			<< "; USER_ID:" << data.GetUserId()
			<< ", USER_ACCOUNT:" << data.GetAccount()
			<< ", DISPLAY_NAME:" << data.GetDisplayName()
			<< ", DISPLAY_EN:" << data.GetPingyin()
			<< ", SEX:" << data.GetSex()
			<< ", DEPT_ID:" << data.GetDepartmentId()
			<< ", DEPT_NAME:" << data.GetDepartmentName()
			<< ", POSITION:"<< data.GetPosition()
			<< ", WORK_PHONE:" << data.GetWorkPhone()
			<< ", MOBILE:" << data.GetMobile()
			<< ", EMAIL:" << data.GetEmail()
			<< ", SUPERIOR_ID:" << data.GetSuperiorId()
			<< ", SUPERIOR_NAME:" << data.GetSuperiorName()
			<< ", SITE_ID:" << data.GetSiteId()
			<< ", PERSONAL_SIGN:" << data.GetPersonalSign()
			<< ", AVATAR_URL:" << data.GetRemoteAvatarUrl()
			<< ", TYPE:" << data.GetType()
			<< ", FELLOW_NUM:" << data.GetColleagueNum()
			<< ", CARD_VERSION:" << data.GetCardVersion()
			<< ", LAST_UPDATE_TIME:" << data.GetLastUpdateTime()
			<< ", USER_TAGS:" << data.GetUserTags();
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ContactDao::SaveContact(const list<ContactInfoImpl*>& contactList) {
	int ret = DBError::DB_SUCCESS;
	ExecDML("BEGIN TRANSACTION;", _db);
	for (list<ContactInfoImpl*>::const_iterator itor = contactList.begin();
		itor != contactList.end(); ++itor) {
		ret = SaveContact(**itor);
		if (ret != 0) {
			break;
		}
	}
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}

int ContactDao::GetContact(int userId, ContactInfoImpl& data) {
	// 联合contact和fs表查找联系人信息
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT c.USER_ID, c.USER_ACCOUNT, c.DISPLAY_NAME, c.DISPLAY_EN, c.SEX, c.DEPT_ID, c.DEPT_NAME, c.POSITION, c.WORK_PHONE, "
		"c.MOBILE, c.EMAIL, c.SUPERIOR_ID, c.SUPERIOR_NAME, c.SITE_ID, c.PERSONAL_SIGN, ifnull(f.LOCALPATH, \"\"), c.AVATAR_URL, c.TYPE, "
		"c.FELLOW_NUM, c.CARD_VERSION, c.LAST_UPDATE_TIME, c.USER_TAGS FROM TBL_CONTACT c LEFT JOIN TBL_FILE_TRANSFER f ON c.AVATAR_URL = f.FILEURL "
		"WHERE c.USER_ID = ?;");

	stmt.bind(1, userId);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetContact return error:" << ret << "; userId:" << userId;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		SetContact(query, data);
	} else {
		UC_LOG(ERROR) << "GetContact return db empty error! userId:" << userId;
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int ContactDao::GetContact(const string& account, ContactInfoImpl& data){
	// 联合contact和fs表查找联系人信息
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT c.USER_ID, c.USER_ACCOUNT, c.DISPLAY_NAME, c.DISPLAY_EN, c.SEX, c.DEPT_ID, c.DEPT_NAME, c.POSITION, c.WORK_PHONE, "
		"c.MOBILE, c.EMAIL, c.SUPERIOR_ID, c.SUPERIOR_NAME, c.SITE_ID, c.PERSONAL_SIGN, ifnull(f.LOCALPATH, \"\"), c.AVATAR_URL, c.TYPE, "
		"c.FELLOW_NUM, c.CARD_VERSION, c.LAST_UPDATE_TIME, c.USER_TAGS FROM TBL_CONTACT c LEFT JOIN TBL_FILE_TRANSFER f ON c.AVATAR_URL = f.FILEURL "
		"WHERE c.USER_ACCOUNT = ?;");

	stmt.bind(1, account.c_str());
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetContact return error:" << ret << "; account:" << account;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		SetContact(query, data);
	}
	else {
		UC_LOG(ERROR) << "GetContact return db empty error! account:" << account;
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int ContactDao::GetContactListByType(int type, list<ContactInfoImpl*>& contactList) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT c.USER_ID, c.USER_ACCOUNT, c.DISPLAY_NAME, c.DISPLAY_EN, c.SEX, c.DEPT_ID, c.DEPT_NAME, c.POSITION, c.WORK_PHONE, "
		"c.MOBILE, c.EMAIL, c.SUPERIOR_ID, c.SUPERIOR_NAME, c.SITE_ID, c.PERSONAL_SIGN, ifnull(f.LOCALPATH, \"\"), c.AVATAR_URL, c.TYPE, "
		"c.FELLOW_NUM, c.CARD_VERSION, c.LAST_UPDATE_TIME, c.USER_TAGS FROM TBL_CONTACT c LEFT JOIN TBL_FILE_TRANSFER f ON c.AVATAR_URL = f.FILEURL "
		"WHERE c.TYPE = ?;");

	stmt.bind(1, type);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetContactListByType return error:" << ret << "; type:" << type;
		return DBError::DB_QUERY_ERROR;
	}

	ret = SetContactList(query, contactList);
	if (ret != DBError::DB_SUCCESS) {
		UC_LOG(ERROR) << "SetContactList return error:" << ret << "; type:" << type;
	}

	return ret;
}

int ContactDao::GetContactListByName(const string& keyWord, int type, list<ContactInfoImpl*>& contactList) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT c.USER_ID, c.USER_ACCOUNT, c.DISPLAY_NAME, c.DISPLAY_EN, c.SEX, c.DEPT_ID, c.DEPT_NAME, c.POSITION, c.WORK_PHONE, "
		"c.MOBILE, c.EMAIL, c.SUPERIOR_ID, c.SUPERIOR_NAME, c.SITE_ID, c.PERSONAL_SIGN, ifnull(f.LOCALPATH, \"\"), c.AVATAR_URL, c.TYPE, "
		"c.FELLOW_NUM, c.CARD_VERSION, c.LAST_UPDATE_TIME, c.USER_TAGS FROM TBL_CONTACT c LEFT JOIN TBL_FILE_TRANSFER f ON c.AVATAR_URL = f.FILEURL "
		"WHERE (c.DISPLAY_NAME LIKE ? OR c.DISPLAY_EN LIKE ?) AND TYPE = ?;");

	string keyName = "%" + keyWord + "%";
	int i = 1;
	stmt.bind(i++, keyName.c_str());
	stmt.bind(i++, keyName.c_str());
	stmt.bind(i++, type);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetContactListByName return error:" << ret
			<< "; keyWord:" << keyName 
			<< ", type:" << type;
		return DBError::DB_QUERY_ERROR;
	}

	ret = SetContactList(query, contactList);
	if (ret != DBError::DB_SUCCESS) {
		UC_LOG(ERROR) << "SetContactList return error:" << ret 
			<< "; keyWord:" << keyName
			<< ", type:" << type;
	}
	return ret;
}

int ContactDao::GetContactListByName(const string& keyWord, int page, int count, list<ContactInfo*>& contactList) {

	int offset = (page > 0)? (page - 1)*count : 0;

	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT c.USER_ID, c.USER_ACCOUNT, c.DISPLAY_NAME, c.DISPLAY_EN, c.SEX, c.DEPT_ID, c.DEPT_NAME, c.POSITION, c.WORK_PHONE, "
		"c.MOBILE, c.EMAIL, c.SUPERIOR_ID, c.SUPERIOR_NAME, c.SITE_ID, c.PERSONAL_SIGN, ifnull(f.LOCALPATH, \"\"), c.AVATAR_URL, c.TYPE, "
		"c.FELLOW_NUM, c.CARD_VERSION, c.LAST_UPDATE_TIME, c.USER_TAGS FROM TBL_CONTACT c LEFT JOIN TBL_FILE_TRANSFER f ON c.AVATAR_URL = f.FILEURL "
		"WHERE (c.DISPLAY_NAME LIKE ? OR c.DISPLAY_EN LIKE ?) ORDER BY c.DISPLAY_EN ASC LIMIT ?, ?;");

	string keyName = "%" + keyWord + "%";

	int i = 1;
	stmt.bind(i++, keyName.c_str());
	stmt.bind(i++, keyName.c_str());
	stmt.bind(i++, offset);
	stmt.bind(i++, count);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetContactListByName return error:" << ret
			<< "; keyWord:" << keyName 
			<< ", page:" << page
			<< ", cout:" << count;
		return DBError::DB_QUERY_ERROR;
	}

	ret = SetContactList(query, contactList);
	if (ret != DBError::DB_SUCCESS) {
		UC_LOG(ERROR) << "SetContactList return error:" << ret 
			<< "; keyWord:" << keyName
			<< ", page:" << page
			<< ", cout:" << count;
	}
	return ret;
}

int ContactDao::GetContactListByUserID(const list<int>& userIDList, list<ContactInfoImpl*>& contactList) {
	if (userIDList.empty()) {
		UC_LOG(ERROR) << "GetContactListByUserID input userIDList is empty!";
		return DBError::DB_INPUT_ERROR;
	}
	stringstream users;
	for (list<int>::const_iterator itor = userIDList.begin();
		itor != userIDList.end(); ++itor) {
			users << *itor << ",";
	}
	string userList(users.str());
	userList.erase(userList.length() - 1);

	CppSQLite3Buffer sql;
	sql.format(
		"SELECT c.USER_ID, c.USER_ACCOUNT, c.DISPLAY_NAME, c.DISPLAY_EN, c.SEX, c.DEPT_ID, c.DEPT_NAME, c.POSITION, c.WORK_PHONE, "
		"c.MOBILE, c.EMAIL, c.SUPERIOR_ID, c.SUPERIOR_NAME, c.SITE_ID, c.PERSONAL_SIGN, ifnull(f.LOCALPATH, \"\"), c.AVATAR_URL, c.TYPE, "
		"c.FELLOW_NUM, c.CARD_VERSION, c.LAST_UPDATE_TIME, c.USER_TAGS FROM TBL_CONTACT c LEFT JOIN TBL_FILE_TRANSFER f ON c.AVATAR_URL = f.FILEURL "
		"WHERE c.USER_ID IN (%s);", userList.c_str());

	CppSQLite3Query query;
	int ret = Query((const char*)sql, _db, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetContactListByUserID query return error:" << ret 
			<< ", sql:" << (const char*)sql;
		return DBError::DB_QUERY_ERROR;
	}

	ret = SetContactList(query, contactList);
	if (ret != DBError::DB_SUCCESS) {
		UC_LOG(ERROR) << "SetContactList return error:" << ret 
			<< "; sql:" << (const char*)sql;
	}
	return ret;
}

int ContactDao::UpdateContact(int userId, const ContactInfoImpl& data) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_CONTACT SET USER_ACCOUNT = ?, DISPLAY_NAME = ?, DISPLAY_EN = ?, "
		"SEX = ?, DEPT_ID = ?, DEPT_NAME = ?, POSITION = ?, WORK_PHONE = ?, MOBILE = ?, EMAIL = ?, "
		"SUPERIOR_ID = ?, SUPERIOR_NAME = ?, SITE_ID = ?, PERSONAL_SIGN = ?, "
		"AVATAR_URL = ?, TYPE = ?, FELLOW_NUM = ?, CARD_VERSION = ?, LAST_UPDATE_TIME = ?, USER_TAGS = ? WHERE USER_ID = ?;");
	int i = 1;
	stmt.bind(i++, data.GetAccount().c_str());
	stmt.bind(i++, data.GetDisplayName().c_str());
	stmt.bind(i++, data.GetPingyin().c_str());
	stmt.bind(i++, data.GetSex());
	stmt.bind(i++, data.GetDepartmentId());
	stmt.bind(i++, data.GetDepartmentName().c_str());
	stmt.bind(i++, data.GetPosition().c_str());
	stmt.bind(i++, data.GetWorkPhone().c_str());
	stmt.bind(i++, data.GetMobile().c_str());
	stmt.bind(i++, data.GetEmail().c_str());
	stmt.bind(i++, data.GetSuperiorId());
	stmt.bind(i++, data.GetSuperiorName().c_str());
	stmt.bind(i++, data.GetSiteId());
	stmt.bind(i++, data.GetPersonalSign().c_str());
	stmt.bind(i++, data.GetRemoteAvatarUrl().c_str());
	stmt.bind(i++, data.GetType());
	stmt.bind(i++, data.GetColleagueNum());
	stmt.bind(i++, data.GetCardVersion());
	stmt.bind(i++, data.GetLastUpdateTime());
	stmt.bind(i++, data.GetUserTags().c_str());
	stmt.bind(i++, data.GetUserId());
	stmt.bind(i++, userId);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateContact return error:" << ret
			<< "; USER_ID:" << userId
			<< ", USER_ACCOUNT:" << data.GetAccount()
			<< ", DISPLAY_NAME:" << data.GetDisplayName()
			<< ", DISPLAY_EN:" << data.GetPingyin()
			<< ", SEX:" << data.GetSex()
			<< ", DEPT_ID:" << data.GetDepartmentId()
			<< ", DEPT_NAME:" << data.GetDepartmentName()
			<< ", POSITION:"<< data.GetPosition()
			<< ", WORK_PHONE:" << data.GetWorkPhone()
			<< ", MOBILE:" << data.GetMobile()
			<< ", EMAIL:" << data.GetEmail()
			<< ", SUPERIOR_ID:" << data.GetSuperiorId()
			<< ", SUPERIOR_NAME:" << data.GetSuperiorName()
			<< ", SITE_ID:" << data.GetSiteId()
			<< ", PERSONAL_SIGN:" << data.GetPersonalSign()
			<< ", AVATAR_URL:" << data.GetRemoteAvatarUrl()
			<< ", TYPE:" << data.GetType()
			<< ", FELLOW_NUM:" << data.GetColleagueNum()
			<< ", CARD_VERSION:" << data.GetCardVersion()
			<< ", LAST_UPDATE_TIME:" << data.GetLastUpdateTime()
			<< ", USER_TAGS:" << data.GetUserTags();
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ContactDao::UpdateLastUpdateTime(int userId, int64_t timestamp) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_CONTACT SET LAST_UPDATE_TIME = ? WHERE USER_ID = ?;");
	int i = 1;
	stmt.bind(i++, timestamp);
	stmt.bind(i++, userId);

	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateContact return error:" << ret
			<< "; LAST_UPDATE_TIME:" << userId
			<< ", user_id:" << userId;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ContactDao::UpdateLastUpdateTimeByTypes(int type, int64_t timestamp) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_CONTACT SET LAST_UPDATE_TIME = ? WHERE TYPE = ?;");
	int i = 1;
	stmt.bind(i++, timestamp);
	stmt.bind(i++, type);

	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateContact return error:" << ret
			<< "; LAST_UPDATE_TIME:" << timestamp
			<< ", TYPE:" << type;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ContactDao::UpdateUserType(int userId, int type) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_CONTACT SET TYPE = ? WHERE USER_ID = ?;");
	int i = 1;
	stmt.bind(i++, type);
	stmt.bind(i++, userId);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateUserType return error:" << ret
			<< "; type:" << type << ", user_id:" << userId;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

// string ContactDao::GetContactLocalAvatar(int userId) {
// 	CppSQLite3Statement stmt = _db->compileStatement(
// 		"SELECT ifnull(f.LOCALPATH, \"\") FROM TBL_CONTACT c LEFT JOIN TBL_FILE_TRANSFER f ON c.AVATAR_URL = f.FILEURL "
// 		"WHERE c.USER_ID = ?;");
// 	stmt.bind(1, userId);
// 	CppSQLite3Query query;
// 	int ret = Query(stmt, query);
// 	if (ret != 0) {
// 		UC_LOG(ERROR) << "GetContactLocalAvatar return error:" << ret << "; userId:" << userId;
// 		return "";
// 	}
// 
// 	if (query.eof()) {
// 		UC_LOG(ERROR) << "GetContactLocalAvatar return db empty error! userId:" << userId;
// 		return "";
// 	}
// 	return query.fieldValue(0);
// }

void ContactDao::SetContact(CppSQLite3Query& query, ContactInfoImpl& data) {
	int i = 0;
	data.SetUserId(atoi(query.fieldValue(i++)));
	data.SetAccount(query.fieldValue(i++));
	data.SetDisplayName(query.fieldValue(i++));
	data.SetPingyin(query.fieldValue(i++));
	data.SetSex(ContactInfo::Sex(atoi(query.fieldValue(i++))));
	data.SetDepartmentId(atoi(query.fieldValue(i++)));
	data.SetDepartmentName(query.fieldValue(i++));
	data.SetPosition(query.fieldValue(i++));
	data.SetWorkPhone(query.fieldValue(i++));
	data.SetMobile(query.fieldValue(i++));
	data.SetEmail(query.fieldValue(i++));
	data.SetSuperiorId(atoi(query.fieldValue(i++)));
	data.SetSuperiorName(query.fieldValue(i++));
	data.SetSiteId(atoi(query.fieldValue(i++)));
	data.SetPersonalSign(query.fieldValue(i++));
	data.SetLocalAvatar(query.fieldValue(i++));
	data.SetRemoteAvatarUrl(query.fieldValue(i++));
	data.SetType(ContactInfo::Type(atoi(query.fieldValue(i++))));
	data.SetColleagueNnum(atoi(query.fieldValue(i++)));
	data.SetCardVersion(StringConvert::Convert2Int64(query.fieldValue(i++)));
	data.SetLastUpdateTime(StringConvert::Convert2Int64(query.fieldValue(i++)));
	data.SetUserTags(query.fieldValue(i++));

	if (!data.GetLocalAvatar().empty()){
		boost::filesystem::path localPath = StringConvert::FromUTF8Str(data.GetLocalAvatar());
		if (!boost::filesystem::exists(localPath) || boost::filesystem::file_size(localPath) <= 0) {
			data.SetLocalAvatar("");
		}
	}

}

int ContactDao::SetContactList(CppSQLite3Query& query, list<ContactInfoImpl*>& contactList) {
	if (query.eof()) {
		return DBError::DB_QUERY_EMPTY;
	}

	while (!query.eof()) {
		ContactInfoImpl* data = ContactInfoImpl::CreateContactInfoImpl();
		SetContact(query, *data);
		contactList.push_back(data);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

int ContactDao::SetContactList(CppSQLite3Query& query, list<ContactInfo*>& contactList) {
	if (query.eof()) {
		return DBError::DB_QUERY_EMPTY;
	}

	while (!query.eof()) {
		ContactInfoImpl* impl = ContactInfoImpl::CreateContactInfoImpl();
		SetContact(query, *impl);
		ContactInfo* info = new ContactInfo(impl);
		contactList.push_back(info);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

} //namespace uc
