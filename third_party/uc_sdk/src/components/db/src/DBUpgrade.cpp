
#include "DBUpgrade.h"
#include "DBManagerDef.h"
#include "AppDao.h"
#include "LogManager.h"

namespace uc {

DBUpgrade::DBUpgrade() {
	_upgradeMap[DBV1] = DBUpgrade::UpgradeV1;
	_upgradeMap[DBV2] = DBUpgrade::UpgradeV2;
	_upgradeMap[DBV3] = DBUpgrade::UpgradeV3;
}

int DBUpgrade::Upgrade(const string& commonDBPath, const string& userDBPath) {
	CppSQLite3DB commonDb, userDb;
	commonDb.open(commonDBPath.c_str());
	userDb.open(userDBPath.c_str());
	int oldVersion = GetDBVersion(&userDb);
	if (oldVersion >= DB_NEW_VERSION) {
		UC_LOG(WARNING) << "Not need to upgrade! oldVersion:" << oldVersion << ", newVersion:" << DB_NEW_VERSION;
		commonDb.close();
		userDb.close();
		return DBError::DB_SUCCESS;
	}

	int ret = 0;
	while (oldVersion != DB_NEW_VERSION) {
		UpgradePtr upGradeFunc = GetUpgradeFunc(oldVersion + 1);
		if (upGradeFunc != NULL) {
			if (upGradeFunc(&commonDb, &userDb) != 0) {
				ret = -1;
				break;
			}
			oldVersion++;
		} else {
			break;
		}
	}

	SetDBVersion(&userDb, oldVersion);
	commonDb.close();
	userDb.close();
	return ret;
}

int DBUpgrade::GetDBVersion(CppSQLite3DB* userDb) {
	CppSQLite3Buffer sql;
	sql.format("SELECT CFG_VALUE FROM TBL_CLIENT_CONFIG WHERE APPLICATION = 'DBManager' AND CFG_NAME = 'DBVersion';");
	CppSQLite3Query query;
	int ret = AppDao().Query((const char*)sql, userDb, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetDBVersion return error:" << ret << ", sql:" << (const char*)sql;
		return 0;
	}
	if (query.eof()) {
		UC_LOG(WARNING) << "GetDBVersion return db empty! sql:" << (const char*)sql;
		return 0;
	}

	int version = atoi(query.fieldValue(0));
	UC_LOG(INFO) << "DBVersion:" << version;
	return version;
}

int DBUpgrade::SetDBVersion(CppSQLite3DB* userDb, int version) {
	CppSQLite3Buffer sql;
	sql.format("UPDATE TBL_CLIENT_CONFIG SET CFG_VALUE = '%d' WHERE APPLICATION = 'DBManager' AND CFG_NAME = 'DBVersion';", version);
	int ret = AppDao().ExecDML((const char*)sql, userDb);
	if (ret != 0) {
		UC_LOG(ERROR) << "SetDBVersion return error:" << ret << ", sql:" << (const char*)sql;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

UpgradePtr DBUpgrade::GetUpgradeFunc(int version) {
	if (_upgradeMap.find(version) != _upgradeMap.end()) {
		return _upgradeMap[version];
	}
	return NULL;
}

int DBUpgrade::UpgradeV1(CppSQLite3DB* commonDb, CppSQLite3DB* userDb) {
	const string upgradeSQL = "ALTER TABLE TBL_HISTORY_ACCOUNT RENAME TO TEMP_TBL_HISTORY_ACCOUNT;" //将当前数据表名改为临时表
		"CREATE TABLE IF NOT EXISTS TBL_HISTORY_ACCOUNT ("	//创建新表
		"USER_ACCOUNT VARCHAR NOT NULL, "
		"DISPLAY_NAME VARCHAR NOT NULL, "
		"AVATAR_URL VARCHAR NOT NULL DEFAULT '', "
		"LOGIN_TIME INTEGER NOT NULL DEFAULT 0, "
		"PASSWORD VARCHAR, "
		"LAST_MANUAL_LOGIN_TIME INTEGER NOT NULL DEFAULT 0, "
		"RECORD_PWD BOOLEAN DEFAULT 0, "
		"AUTO_LOGIN BOOLEAN DEFAULT 0, "
		"CONSTRAINT UK_HISTORY_ACCOUNT UNIQUE (LOGIN_TIME DESC) ON CONFLICT REPLACE "
		");"
		"INSERT INTO TBL_HISTORY_ACCOUNT SELECT USER_ACCOUNT, DISPLAY_NAME, AVATAR_URL, " //导入数据
		"LOGIN_TIME, PASSWORD, LAST_MANUAL_LOGIN_TIME, RECORD_PWD, AUTO_LOGIN FROM TEMP_TBL_HISTORY_ACCOUNT;"
		"DROP TABLE TEMP_TBL_HISTORY_ACCOUNT;"; //删除临时表
	AppDao().ExecDML("BEGIN TRANSACTION;", commonDb);
	int ret = AppDao().ExecDML(upgradeSQL.c_str(), commonDb);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpgradeV1 return error:" << ret << ", sql:" << upgradeSQL;
		ret = DBError::DB_UPGRADE_ERROR;
	}
	AppDao().ExecDML("COMMIT TRANSACTION;", commonDb);
	return ret;
}

int DBUpgrade::UpgradeV2(CppSQLite3DB* commonDb, CppSQLite3DB* userDb) {
//	DWORD start = GetTickCount();
#if 0
	const string upgradeSQL = "ALTER TABLE TBL_MESSAGE RENAME TO TEMP_TBL_MESSAGE;"
		"CREATE TABLE IF NOT EXISTS TBL_MESSAGE ("
		"MSG_ID INTEGER PRIMARY KEY NOT NULL, "
		"MSG_SEQ BIGINT NOT NULL DEFAULT 0, "
		"MSG_TIME INTEGER NOT NULL, "
		"SEND_STATE TINYINT NOT NULL, "
		"PLAY_STATE TINYINT NOT NULL, "
		"MSG_CONTENT VARCHAR NOT NULL, "
		"APP_ID INTEGER NOT NULL, "
		"PROTOCOL_TYPE INTEGER NOT NULL, "
		"PROTOCOL_ID INTEGER NOT NULL, "
		"CONVERSATION INTEGER NOT NULL, "
		"FROM_ID INTEGER NOT NULL, "
		"FROM_SITE_ID INTEGER NOT NULL DEFAULT 0, "
		"FROM_RES_ID INTEGER NOT NULL DEFAULT 0, "
		"TO_ID INTEGER NOT NULL, "
		"TO_SITE_ID INTEGER NOT NULL DEFAULT 0, "
		"TO_RES_ID INTEGER NOT NULL DEFAULT 0, "
		"VERSION INTEGER NOT NULL, "
		"CONVERSATION_TYPE INT NOT NULL, "
		"RELATE_ID BIGINT NOT NULL "
		"C1 INTEGER NOT NULL, "
		"C2 VARCHAR NOT NULL "
		");"
		"DROP INDEX IF EXISTS INDEX_MSG_CONVERSATION;"
		"DROP INDEX IF EXISTS INDEX_MSG_SEQ;"
		"DROP INDEX IF EXISTS INDEX_MSG_TIME;"
		"CREATE INDEX INDEX_MSG_CONVERSATION ON TBL_MESSAGE (CONVERSATION DESC);"
		"CREATE INDEX INDEX_MSG_SEQ ON TBL_MESSAGE (MSG_SEQ DESC);"
		"CREATE INDEX INDEX_MSG_TIME ON TBL_MESSAGE (MSG_TIME DESC);"
		"INSERT INTO TBL_MESSAGE "
		"SELECT MSG_ID, MSG_SEQ, MSG_TIME, SEND_STATE, PLAY_STATE, "
		"MSG_CONTENT, APP_ID, PROTOCOL_TYPE, PROTOCOL_ID, CONVERSATION, "
		"FROM_ID, FROM_SITE_ID, FROM_RES_ID, TO_ID, TO_SITE_ID, "
		"TO_RES_ID, VERSION, CONVERSATION_TYPE, RELATE_ID, 100, 'TEST' FROM TEMP_TBL_MESSAGE;"
		"DROP TABLE TEMP_TBL_MESSAGE;";
#else
	const string upgradeSQL = "ALTER TABLE TBL_MESSAGE ADD C1 INTEGER DEFAULT 100;"
		"ALTER TABLE TBL_MESSAGE ADD C2 VARCHAR DEFAULT 'TEST';";
#endif
	AppDao().ExecDML("BEGIN TRANSACTION;", userDb);
	int ret = AppDao().ExecDML(upgradeSQL.c_str(), userDb);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpgradeV1 return error:" << ret << ", sql:" << upgradeSQL;
		ret = DBError::DB_UPGRADE_ERROR;
	}
	AppDao().ExecDML("COMMIT TRANSACTION;", userDb);

//	DWORD end = GetTickCount();
//	printf("UpgradeV2 start:%d, end:%d, const:%d\n", start, end, (end - start));
	return 0;
}

int DBUpgrade::UpgradeV3(CppSQLite3DB* commonDb, CppSQLite3DB* userDb) {
	//TODO
	return 0;
}

}
