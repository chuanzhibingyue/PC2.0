/*
 * ConversationDao.cpp
 *
 *  Created on: 2015年6月19日
 *      Author: jianxue
 */

#include "ConversationDao.h"
#include "LogManager.h"
#include "message_service/MessageProcessor.h"
#include "UCTime.h"

namespace uc {

ConversationDao::ConversationDao() {
	_db = SQLiteManager::GetInstance().GetUserDB();
}

ConversationDao::~ConversationDao() {
	SQLiteManager::GetInstance().ReleaseUserDB(_db);
}

int32_t ConversationDao::SaveConversations(std::list<ConversationInfo*>& conversations) {
	int ret = DBError::DB_SUCCESS;
	ExecDML("BEGIN TRANSACTION;", _db);
	for (std::list<ConversationInfo*>::const_iterator itor = conversations.begin();
		itor != conversations.end(); ++itor) {
			ConversationInfoInternal* conv_internal = (ConversationInfoInternal*)(*itor);
			assert( conv_internal );
			ret = SaveConversationInfo(*conv_internal);
			if (ret != 0) {
				break;
			}
	}
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}

int32_t ConversationDao::DeleteConversation(ConversationType type, int64_t relateId) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_DEL_CONVERSATION"
		"(CONVERSATION_TYPE, RELATE_ID,REMOVE_TIME) "
		"VALUES(?,?,?);");
	int64_t  tmNow = Time::GetSysTime();

	int i = 1;
	stmt.bind(i++, type);
	stmt.bind(i++, relateId);
	stmt.bind(i++, tmNow);

	int ret = ExecDML(stmt);
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "DeleteConversation return error:" << ret
			<< "; conversation_type:" << type
			<< "; relate_id:" << relateId
			<< "; remove_time:" << tmNow;
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int32_t ConversationDao::GetDeleteTime(ConversationType type, int64_t relateId, int64_t & removeTime) {
	removeTime = 0;

	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT REMOVE_TIME FROM TBL_DEL_CONVERSATION where CONVERSATION_TYPE like ? and RELATE_ID like ? ;");
	stmt.bind(1, type);
	stmt.bind(2, relateId);

	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetDeleteTime query return error:" << ret;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetDeleteTime query return db empty!";
		return DBError::DB_QUERY_EMPTY;
	}
	const char* seq = query.fieldValue(0);
	if (seq == NULL) {
		UC_LOG(ERROR) << "GetDeleteTime query return NULL!";
		return DBError::DB_QUERY_EMPTY;
	}

	removeTime = StringConvert::Convert2Int64(seq);
	return DBError::DB_SUCCESS;
}

int32_t ConversationDao::SaveConversationInfo(const ConversationInfoInternal &conversation) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_CONVERSATION"
		"(CONVERSATION_TYPE, RELATE_ID, HIDDEN, NEWMSG_NUM, MSG_ID, MSG_SEQ, "
		"MSG_TIME, SEND_STATE, MSG_CONTENT, APP_ID, PROTOCOL_TYPE, "
		"PROTOCOL_ID, CONVERSATION, FROM_ID, FROM_SITE_ID, FROM_RES_ID, TO_ID, "
		"TO_SITE_ID, TO_RES_ID, VERSION, OA_LAST_STATUS, APP_ACK_SENT) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");

	UCMsg* uc_msg = (UCMsg*)conversation.GetMsg();
	if (uc_msg == NULL)
	{
		UC_LOG(ERROR) << "ConversationDao::SaveConversationInfo error. uc_msg is null";
		return DBError::DB_INSERT_ERROR;
	}

	UCHead head;
	uc_msg->GetHead( head );

	int i = 1;
	stmt.bind(i++, conversation.GetType());
	stmt.bind(i++, conversation.GetRelateId());
	stmt.bind(i++, conversation.isHidden());
	stmt.bind(i++, conversation.GetNewMsgCount());
	stmt.bind(i++, head.GetId());
	stmt.bind(i++, head.GetSeq());
	stmt.bind(i++, head.GetTimestamp());
	stmt.bind(i++, conversation.GetSendState());
	stmt.bind(i++, conversation.GetBody().c_str());
	stmt.bind(i++, head.GetAppId());
	stmt.bind(i++, head.GetProtocoltype());
	stmt.bind(i++, head.GetProtocolId());
	stmt.bind(i++, head.GetConversation());
	stmt.bind(i++, head.GetFrom().GetUserId());
	stmt.bind(i++, head.GetFrom().GetSiteId());
	stmt.bind(i++, head.GetFrom().GetResId());
	stmt.bind(i++, head.GetTo().GetUserId());
	stmt.bind(i++, head.GetTo().GetSiteId());
	stmt.bind(i++, head.GetTo().GetResId());
	stmt.bind(i++, head.GetVersion());
	stmt.bind(i++, conversation.GetOALastStatus());
	stmt.bind(i++, conversation.GetAppAckSent());
	
	int ret = ExecDML(stmt); 
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveConversationInfo return error:" << ret 
			<< "; conversation_type:" << conversation.GetType() 
			<< "; relate_id:" << conversation.GetRelateId() 
			<< "; conversation:" << conversation.GetConversation() 
			<< "; newmsg_num:" << conversation.GetNewMsgCount() 
			<< "; last_updatemsg_time:" << conversation.GetLastMsgTime() 
			<< "; last_update_msgseq:" << conversation.GetLastMsgSeq() 
			<< "; hidden:" << conversation.isHidden()
			<< "; oa_last_status:" << conversation.GetOALastStatus()
			<< "; app_ack_sent:" << conversation.GetAppAckSent();
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

void ConversationDao::SetConversation(CppSQLite3Query& query, ConversationInfoInternal& conversation) {

	UCMsg *msg = NULL;
	UCHead head;
	int i = 0;
	conversation.SetType(ConversationType(atoi(query.fieldValue(i++))));
	conversation.SetRelateId(atoi(query.fieldValue(i++)));
	bool isHidden = (atoi(query.fieldValue(i++)) == 0)? false:true;
	conversation.SetHidden(isHidden);
	conversation.SetNewMsgCount(atoi(query.fieldValue(i++)));
	head.SetId(atoi(query.fieldValue(i++)));
	head.SetSeq(StringConvert::Convert2Int64(query.fieldValue(i++)));
	head.SetTimestamp(StringConvert::Convert2Int64(query.fieldValue(i++)));
	conversation.SetSendState(MsgSendState(atoi(query.fieldValue(i++))));
	conversation.SetBody(query.fieldValue(i++));
	head.SetAppId(atoi(query.fieldValue(i++)));
	head.SetProtocoltype(atoi(query.fieldValue(i++)));
	head.SetProtocolId(atoi(query.fieldValue(i++)));
	head.SetConversation(atoi(query.fieldValue(i++)));

	int32_t from_userid = atoi(query.fieldValue(i++));
	int32_t from_siteid = atoi(query.fieldValue(i++));
	int32_t from_resid =  atoi(query.fieldValue(i++));
	head.SetFrom(UCID(from_userid, from_siteid, from_resid));
	
	int32_t to_userid = atoi(query.fieldValue(i++));
	int32_t to_siteid = atoi(query.fieldValue(i++));
	int32_t to_resid =  atoi(query.fieldValue(i++));
	head.SetTo(UCID(to_userid, to_siteid, to_resid));

	head.SetVersion(atoi(query.fieldValue(i++)));
	conversation.SetOALastStatus(atoi(query.fieldValue(i++)));
	conversation.SetAppAckSent(atoi(query.fieldValue(i++)));

	conversation.SetLastMsgSeq( head.GetSeq() );
	conversation.SetLastMsgTime( head.GetTimestamp() );

	MessageProcessor::GetUCMsg( head, conversation.GetBody(), msg );
	conversation.SetMsg(msg);

	if (conversation.GetType() == CONFERENCE_CHAT) {
		int64_t eventId = 0;
		int64_t conferenceId = 0;
		int64_t authorizerId = 0;
		
		MessageProcessor::GetIdsFromUCMsg(*msg, eventId, conferenceId, authorizerId);

		conversation.SetEventId(eventId);
		conversation.SetConferenceId(conferenceId);
		conversation.SetAuthorizerId(authorizerId);
	}
}

int32_t ConversationDao::GetMaxMessageSeqData(int64_t &msgSeq) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT MAX(MSG_SEQ) FROM TBL_CONVERSATION;");
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetMaxMessageSeqData query return error:" << ret;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetMaxMessageSeqData query return db empty!";
		return DBError::DB_QUERY_EMPTY;
	}
	const char* seq = query.fieldValue(0);
	if (seq == NULL) {
		UC_LOG(ERROR) << "GetMaxMessageSeqData query return NULL!";
		return DBError::DB_QUERY_EMPTY;
	}

	msgSeq = StringConvert::Convert2Int64(seq);
	return DBError::DB_SUCCESS;
}

int32_t ConversationDao::RemoveConversation(int32_t type, int64_t relateId) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"DELETE FROM TBL_CONVERSATION WHERE CONVERSATION_TYPE = ? AND RELATE_ID = ?;");
	stmt.bind(1, type);
	stmt.bind(2, relateId);

	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "RemoveConversation return error:" << ret << "; type:" << type << "; relateId:" << relateId;
		return DBError::DB_DELETE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int32_t ConversationDao::RemoveAllConversations() {
	string sql = "DELETE FROM TBL_CONVERSATION;";
	int ret = ExecDML(sql, _db);
	if (ret != 0) {
		UC_LOG(ERROR) << "RemoveAllConversations return error:" << ret;
		return DBError::DB_DELETE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int32_t ConversationDao::LoadLatestConversations(int32_t number, std::list<ConversationInfo*> &conversations) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_CONVERSATION ORDER BY MSG_SEQ DESC LIMIT 0, ?;");
	stmt.bind(1, number);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "LoadLatestConversations query return error:" << ret << ", number:" << number;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "LoadLatestConversations query return db empty! number:" << number;
		return DBError::DB_QUERY_EMPTY;
	}

	while (!query.eof()) {
		ConversationInfoInternal* info = new ConversationInfoInternal();
		SetConversation(query, *info);
		conversations.push_back(info);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

int32_t ConversationDao::SaveConversationInfo(int32_t type, int64_t msgSeq, int64_t msgTime) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_CONVERSATION"
		"(CONVERSATION_TYPE, RELATE_ID, MSG_SEQ, MSG_TIME) "
		"VALUES (?, ?, ?, ?);");

	int i = 1;
	stmt.bind(i++, type);
	stmt.bind(i++, 0);
	stmt.bind(i++, msgSeq);
	stmt.bind(i++, msgTime);

	int ret = ExecDML(stmt);
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveConversationInfo return error:" << ret
			<< "; conversation_type:" << type
			<< "; last_updatemsg_time:" << msgTime
			<< "; last_update_msgseq:" << msgSeq;
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

} /* namespace uc */
