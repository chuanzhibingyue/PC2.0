///////////////////////////////////////////////////////////
//  ExternalDao.cpp
//  Implementation of the Class ExternalDao
//  Created on:      Jan 28 2016 
//  Original author: yongshuai.cheng
///////////////////////////////////////////////////////////

#include "ExternalDao.h"
#include "LogManager.h"
#include "message_service/MessageProcessor.h"

#include <limits>

namespace uc {

#undef max

ExternalDao::ExternalDao(){
	_db = SQLiteManager::GetInstance().GetExternalDB();
}

ExternalDao::~ExternalDao(){
	SQLiteManager::GetInstance().ReleaseExternalDB(_db);
}

int ExternalDao::HaveData(string externalId)
{
	CppSQLite3Buffer sql;
	sql.format(
		"SELECT * FROM TAB_CHATMSG WHERE conv_key = '%s' limit 1;", externalId.c_str());

	CppSQLite3Query query;
	int ret = Query((const char*)sql, _db, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "ExternalDao::HaveData query return error:" << ret
			<< ", sql:" << (const char*)sql;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof())
	{
		UC_LOG(INFO) << "ExternalDao::HaveData return DBError::DB_QUERY_EMPTY";
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int ExternalDao::GetMsgContentList(int32_t conversationType, string externalId, int64_t startTime, int64_t endTime, int count, list<ExternalMessage*>& message) {

	CppSQLite3Statement stmt;
	if ( endTime == 0 )
	{
		stmt = _db->compileStatement(
			"SELECT * FROM TAB_CHATMSG WHERE conv_key = ? AND "
			"send_time >= ? ORDER BY send_time DESC, send_time DESC LIMIT 0, ?;");
		int i = 1;
		stmt.bind(i++, externalId.c_str());
		stmt.bind(i++, startTime);
		stmt.bind(i++, count);
	}
	else
	{
		stmt = _db->compileStatement(
			"SELECT * FROM TAB_CHATMSG WHERE conv_key = ? AND "
			"send_time >= ? AND send_time < ? ORDER BY send_time DESC, send_time DESC LIMIT 0, ?;");
		int i = 1;
		stmt.bind(i++, externalId.c_str());
		stmt.bind(i++, startTime);
		stmt.bind(i++, endTime);
		stmt.bind(i++, count);
	}
	
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetMsg return error:" << ret 
			<< "; conversation_type:" << conversationType
			<< ", relate_id:" << externalId
			<< ", startTime:" << startTime
			<< ", endTime:" << endTime
			<< ", count:" << count;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetMsg return db empty!"
			<< ";conversation_type:" << conversationType
			<< ", relate_id:" << externalId
			<< ", startTime:" << startTime
			<< ", endTime:" << endTime
			<< ", count:" << count;
		return DBError::DB_QUERY_EMPTY;
	}

	while (!query.eof()) {
		ExternalMessage* store_item = NULL;
		SetMessage(query, store_item);
		message.push_back(store_item);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

void ExternalDao::SetMessage(CppSQLite3Query& query, ExternalMessage *&message) {
	
	message = new TextExternalMessage;
	int i = 0;
	query.fieldValue(i++);
	query.fieldValue(i++);
	message->SetMsgSeq(StringConvert::Convert2Int64(query.fieldValue(i++)));
	ExternalUser to;
	to.SetUserAccount(query.fieldValue(i++));
	message->SetTo(to);
	ExternalUser from;
	from.SetUserAccount(query.fieldValue(i++));
	message->SetFrom(from);
	((TextExternalMessage*)message)->SetTextContent(query.fieldValue(i++));
	((TextExternalMessage*)message)->SetContentType(EXTERNAL_CONTENT_TYPE_TEXT);
}

} //namespace uc
