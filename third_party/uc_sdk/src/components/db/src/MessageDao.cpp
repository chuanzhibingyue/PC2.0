///////////////////////////////////////////////////////////
//  MessageDao.cpp
//  Implementation of the Class MessageDao
//  Created on:      22-五月-2015 10:35:49
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "MessageDao.h"
#include "LogManager.h"
#include "message_service/MessageProcessor.h"

#include <limits>

namespace uc {

#undef max

MessageDao::MessageDao(){
	_db = SQLiteManager::GetInstance().GetUserDB();
}

MessageDao::~MessageDao(){
	SQLiteManager::GetInstance().ReleaseUserDB(_db);
}

int MessageDao::SaveMsg(MessageStoreItem& message) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_MESSAGE"
		"(MSG_ID, MSG_SEQ, MSG_TIME, SEND_STATE, PLAY_STATE, MSG_CONTENT, "
		"APP_ID, PROTOCOL_TYPE, PROTOCOL_ID, PRI, CONTROL_PRI, CHANNEL_PRI, CONVERSATION, "
		"FROM_ID, FROM_SITE_ID, FROM_RES_ID, TO_ID, TO_SITE_ID, TO_RES_ID, TO_PRIVATE_ID, TO_PRIVATE_SITE_ID, "
		"TO_PRIVATE_RES_ID, VERSION, CONVERSATION_TYPE, RELATE_ID, FLAG, OA_LAST_STATUS, APP_ACK_SENT, OA_EXTERNAL_DATA, MSG_AT_USERS) "
		"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");

	UCHead head;
	if ( NULL == message.msg )
	{
		UC_LOG(ERROR) << "SaveMsg error. message.msg is null";
		return DBError::DB_INSERT_ERROR;
	}
	message.msg->GetHead( head );

	int i = 1;
	stmt.bind(i++, head.GetId() );
	stmt.bind(i++, head.GetSeq() );
	stmt.bind(i++, head.GetTimestamp() );
	stmt.bind(i++, message.state );
	stmt.bind(i++, message.is_played );
	stmt.bind(i++, message.body.c_str() );
	stmt.bind(i++, head.GetAppId() );
	stmt.bind(i++, head.GetProtocoltype() );
	stmt.bind(i++, head.GetProtocolId() );
	stmt.bind(i++, head.GetPri());
	stmt.bind(i++, head.GetControlPri());
	stmt.bind(i++, head.GetChannelPri());
	stmt.bind(i++, head.GetConversation() );
	stmt.bind(i++, head.GetFrom().GetUserId() );
	stmt.bind(i++, head.GetFrom().GetSiteId() );
	stmt.bind(i++, head.GetFrom().GetResId() );
	stmt.bind(i++, head.GetTo().GetUserId() );
	stmt.bind(i++, head.GetTo().GetSiteId() );
	stmt.bind(i++, head.GetTo().GetResId() );
	stmt.bind(i++, head.GetPrivate().GetUserId());
	stmt.bind(i++, head.GetPrivate().GetSiteId());
	stmt.bind(i++, head.GetPrivate().GetResId());
	stmt.bind(i++, head.GetVersion() );
	stmt.bind(i++, message.conversation_type);
	stmt.bind(i++, message.relate_id );
	stmt.bind(i++, message.flag );
	stmt.bind(i++, message.oa_last_status);
	stmt.bind(i++, message.app_ack_sent);
	stmt.bind(i++, message.oa_external_data.c_str());
	stmt.bind(i++, head.GetGroupAtUsers_STR().c_str());
	int ret = ExecDML(stmt); 
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveMsg return error:" << ret 
			<< ", id:" << head.GetId() 
			<< ", seq:" << head.GetSeq()
			<< ", time:" << head.GetTimestamp()
			<< ", send state:" << message.state
			<< ", play state:" << message.is_played
			<< ", data:" << message.body.c_str()
			<< ", app_id:" << head.GetAppId()
			<< ", protocol_type:" << head.GetProtocoltype()
			<< ", protocol_id:" << head.GetProtocolId()
			<< ", pri:" << head.GetPri()
			<< ", control_pri:" << head.GetControlPri()
			<< ", channel_pri:" << head.GetChannelPri()
			<< ", conversation:" << head.GetConversation()
			<< ", from_id:" << head.GetFrom().GetUserId()
			<< ", from_siteid:" << head.GetFrom().GetSiteId()
			<< ", from_resid:" << head.GetFrom().GetResId()
			<< ", to_id:" <<head.GetTo().GetUserId()
			<< ", to_siteid:" << head.GetTo().GetSiteId()
			<< ", to_resid:" << head.GetTo().GetResId()
			<< ", to_private_id:" << head.GetPrivate().GetResId()
			<< ", to_private_siteid:" << head.GetPrivate().GetResId()
			<< ", to_private_resid:" << head.GetPrivate().GetResId()
			<< ", version:" << head.GetVersion()
			<< ", conversation_type:" << message.conversation_type
			<< ", relate_id: " << message.relate_id
			<< ", flag: " << message.flag
			<< ", oa_last_status" << message.oa_last_status
			<< ", app_ack_sent" << message.app_ack_sent
			<< ", oa_external_data" << message.oa_external_data.c_str();
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int MessageDao::SaveMsg(list<MessageStoreItem*>& messageList) {
	int ret = DBError::DB_SUCCESS;
	int msgId = 0;
	ExecDML("BEGIN TRANSACTION;", _db);
	for (list<MessageStoreItem*>::const_iterator itor = messageList.begin();
		itor != messageList.end(); ++itor) {
		ret = SaveMsg(**itor);
		if (ret != 0) {
			break;
		}
	}
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}

int MessageDao::SyncMsg(list<MessageStoreItem*>& messageList) {
	int ret = DBError::DB_SUCCESS;
	int msgId = 0;
	ExecDML("BEGIN TRANSACTION;", _db);
	for (list<MessageStoreItem*>::const_iterator itor = messageList.begin();
		itor != messageList.end(); ++itor) {
		MessageStoreItem* item = (*itor);
		assert ( item && item->msg );

		if ( NULL == item || NULL == item->msg )
		{
			UC_LOG(ERROR) << "SyncMsg error. message item is null or item->msg is null";
			return DBError::DB_UPDATE_ERROR;
		}
		UCHead head;
		item->msg->GetHead( head );
		head.SetId( MessageID::GetInstance().GetMsgId() );
		item->msg->SetHead( head );

		int count = 0;
		ret = IsMessageExist( item->conversation_type, item->relate_id, head.GetSeq(), count );
		if (ret != 0) {
			break;
		}

		if ( count > 0 )
		{
			ret = UpdateMessageFlag( item->conversation_type, item->relate_id, head.GetSeq(), item->flag );
			if (ret != 0) {
				break;
			}
		}
		else
		{
			ret = SaveMsg(*item);
			if (ret != 0) {
				break;
			}
		}
	}
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}

int MessageDao::GetMsg(int32_t conversation_type, int64_t relate_id, int64_t seq, MessageStoreItem& message)
{
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_MESSAGE WHERE CONVERSATION_TYPE = ? AND RELATE_ID = ? AND MSG_SEQ = ?;");
	int i = 1;
	stmt.bind(i++, conversation_type);
	stmt.bind(i++, relate_id);
	stmt.bind(i++, seq);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetMsg return error:" << ret << ", conversation_type:" << conversation_type
			<< " relate_id: " << relate_id << " seq: " << seq;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		SetMessage(query, message);
	} else {
		UC_LOG(ERROR) << "GetMsg return db empty!  conversation_type:" << conversation_type
			<< " relate_id: " << relate_id << " seq: " << seq;
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int MessageDao::GetMsg(int messageId, MessageStoreItem& message) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_MESSAGE WHERE MSG_ID = ?;");
	stmt.bind(1, messageId);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetMsg return error:" << ret << ", messageId:" << messageId;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		SetMessage(query, message);
	} else {
		UC_LOG(ERROR) << "GetMsg return db empty! messageId:" << messageId;
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int MessageDao::IsMessageExist(int32_t conversation_type, int64_t relate_id, int64_t seq, int& count)
{
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT count(*) FROM TBL_MESSAGE WHERE CONVERSATION_TYPE = ? AND RELATE_ID = ? AND MSG_SEQ = ?;");
	int i = 1;
	stmt.bind(i++, conversation_type);
	stmt.bind(i++, relate_id);
	stmt.bind(i++, seq);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "IsMessageExist return error:" << ret << ", conversation_type:" << conversation_type
			<< " relate_id: " << relate_id << " seq: " << seq;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof())
	{
		UC_LOG(INFO) << "count query return db empty!";
		count = 0;
		return DBError::DB_SUCCESS;
	}
	const char* number = query.fieldValue(0);
	if (number == NULL) 
	{
		UC_LOG(INFO) << "count query return NULL!";
		count = 0;
		return DBError::DB_SUCCESS;
	}

	count = atoi(number);
	return DBError::DB_SUCCESS;
}

int MessageDao::GetMaxMessageSeq(int32_t conversation_type, int64_t relate_id, int64_t& max_seq)
{
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT MAX(MSG_SEQ) FROM TBL_MESSAGE WHERE SEND_STATE = 0 AND CONVERSATION_TYPE = ? AND RELATE_ID = ?;");
	int i = 1;
	stmt.bind(i++, conversation_type);
	stmt.bind(i++, relate_id);

	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) 
	{
		UC_LOG(ERROR) << "GetMaxMessageSeq query return error:" << ret;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof())
	{
		UC_LOG(ERROR) << "GetMaxMessageSeq query return db empty!";
		return DBError::DB_QUERY_EMPTY;
	}
	const char* seq = query.fieldValue(0);
	if (seq == NULL) 
	{
		UC_LOG(ERROR) << "GetMaxMessageSeq query return NULL!";
		return DBError::DB_QUERY_EMPTY;
	}

	max_seq = StringConvert::Convert2Int64(seq);
	return DBError::DB_SUCCESS;
}

int MessageDao::GetMsgContentList(int32_t conversationType, int64_t relateId, int64_t startTime, int64_t endTime, int count, list<MessageStoreItem*>& message) {

	CppSQLite3Statement stmt;
	if ( endTime == 0 )
	{
		stmt = _db->compileStatement(
			"SELECT * FROM TBL_MESSAGE WHERE CONVERSATION_TYPE = ? AND RELATE_ID = ? AND "
			"MSG_SEQ >= ? ORDER BY MSG_SEQ DESC, MSG_SEQ DESC LIMIT 0, ?;");
		int i = 1;
		stmt.bind(i++, conversationType);
		stmt.bind(i++, relateId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, count);
	}
	else
	{
		stmt = _db->compileStatement(
			"SELECT * FROM TBL_MESSAGE WHERE CONVERSATION_TYPE = ? AND RELATE_ID = ? AND "
			"MSG_SEQ >= ? AND MSG_SEQ < ? ORDER BY MSG_SEQ DESC, MSG_SEQ DESC LIMIT 0, ?;");
		int i = 1;
		stmt.bind(i++, conversationType);
		stmt.bind(i++, relateId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, endTime);
		stmt.bind(i++, count);
	}
	
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetMsg return error:" << ret 
			<< "; conversation_type:" << conversationType
			<< ", relate_id:" << relateId
			<< ", startTime:" << startTime
			<< ", endTime:" << endTime
			<< ", count:" << count;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetMsg return db empty!"
			<< ";conversation_type:" << conversationType
			<< ", relate_id:" << relateId
			<< ", startTime:" << startTime
			<< ", endTime:" << endTime
			<< ", count:" << count;
		return DBError::DB_QUERY_EMPTY;
	}

	while (!query.eof()) {
		MessageStoreItem* store_item = new MessageStoreItem();
		SetMessage(query, *store_item);
		message.push_back(store_item);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

void MessageDao::SetMessage(CppSQLite3Query& query, MessageStoreItem& message) {
	int i = 0;

	UCMsg *msg = NULL;
	UCHead head;
	
	head.SetId(atoi(query.fieldValue(i++)));
	head.SetSeq(StringConvert::Convert2Int64(query.fieldValue(i++)));
	head.SetTimestamp(StringConvert::Convert2Int64(query.fieldValue(i++)));
	message.state = MsgSendState(atoi(query.fieldValue(i++)));
	message.is_played = atoi(query.fieldValue(i++));
	message.body = query.fieldValue(i++);
	head.SetAppId(atoi(query.fieldValue(i++)));
	head.SetProtocoltype(atoi(query.fieldValue(i++)));
	head.SetProtocolId(atoi(query.fieldValue(i++)));
	head.SetPri(atoi(query.fieldValue(i++)));
	head.SetControlPri(atoi(query.fieldValue(i++)));
	head.SetChannelPri(atoi(query.fieldValue(i++)));
	head.SetConversation(atoi(query.fieldValue(i++)));
	
	int32_t from_userid = atoi(query.fieldValue(i++));
	int32_t from_siteid = atoi(query.fieldValue(i++));
	int32_t from_resid =  atoi(query.fieldValue(i++));
	head.SetFrom(UCID(from_userid, from_siteid, from_resid));

	int32_t to_userid = atoi(query.fieldValue(i++));
	int32_t to_siteid = atoi(query.fieldValue(i++));
	int32_t to_resid =  atoi(query.fieldValue(i++));
	head.SetTo(UCID(to_userid, to_siteid, to_resid));

	int32_t to_private_userid = atoi(query.fieldValue(i++));
	int32_t to_private_siteid = atoi(query.fieldValue(i++));
	int32_t to_private_resid = atoi(query.fieldValue(i++));
	head.SetPrivate(UCID(to_private_userid, to_private_siteid, to_private_resid));

	head.SetVersion(atoi(query.fieldValue(i++)));
	message.conversation_type = (ConversationType)atoi(query.fieldValue(i++));
	message.relate_id = atoi(query.fieldValue(i++));
	message.flag = atoi(query.fieldValue(i++));
	message.oa_last_status = atoi(query.fieldValue(i++));
	message.app_ack_sent = atoi(query.fieldValue(i++));
	message.oa_external_data = query.fieldValue(i++);
	std::string sAtUsers = query.fieldValue(i++);
	head.SetGroupAtUsers_STR(sAtUsers);

	if (MessageProcessor::GetUCMsg( head, message.body, msg )  == UC_SUCCEEDED )
	{
		message.msg = msg;
	}
	else
	{
		UC_LOG(ERROR) << "SetMessage error. MessageProcessor::GetUCMsg get msg is null. from_userid: " << from_userid
			<< " to_userid: " << to_userid ;
	}
}

int MessageDao::SetMessageStatusById(int msgId, int state) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_MESSAGE SET SEND_STATE = ? WHERE MSG_ID = ?;");
	int i = 1;
	stmt.bind(i++, state);
	stmt.bind(i++, msgId);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "SetMessageStatusById return error:" << ret << ", state:" << state << ",msgId:" << msgId;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int MessageDao::SetMessageStatusBySeq(int64_t seq, int state) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_MESSAGE SET PLAY_STATE = ? WHERE MSG_SEQ = ?;");
	int i = 1;
	stmt.bind(i++, state);
	stmt.bind(i++, seq);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "SetMessageStatusBySeq return error:" << ret 
			<< ", state:" << state << ",seq:" << seq;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int MessageDao::SetMessageSeq(int msgId, int64_t time, int64_t seq) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_MESSAGE SET MSG_SEQ = ? WHERE MSG_ID = ? AND MSG_TIME = ?;");
	int i = 1;
	stmt.bind(i++, seq);
	stmt.bind(i++, msgId);
	stmt.bind(i++, time);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "SetMessageSeq return error:" << ret 
			<< ", msgId:" << msgId << ", time:"<< time << ", seq:" << seq;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int MessageDao::UpdateMessageFlag(int32_t conversation_type, int64_t relate_id, int64_t seq, int64_t flag)
{
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_MESSAGE SET FLAG = ? WHERE CONVERSATION_TYPE = ? AND RELATE_ID = ? AND MSG_SEQ = ?;");
	int i = 1;
	stmt.bind(i++, flag);
	stmt.bind(i++, conversation_type);
	stmt.bind(i++, relate_id);
	stmt.bind(i++, seq);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		return DBError::DB_DELETE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int MessageDao::UpdateMessageFlag(int msgId)
{
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_MESSAGE SET FLAG = 0 WHERE MSG_ID = ?;");
	stmt.bind(1, msgId);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateMessageFlag return error:" << ret 
			<< ", msgId:" << msgId;
		return DBError::DB_DELETE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int MessageDao::UpdateMessageInfo(int msgId, MsgSendState state, int64_t timeStamp, int64_t seq, int32_t conversation) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_MESSAGE SET MSG_SEQ = ?, MSG_TIME = ?, SEND_STATE = ?, CONVERSATION = ? WHERE MSG_ID = ?;");
	int i = 1;
	stmt.bind(i++, seq);
	stmt.bind(i++, timeStamp);
	stmt.bind(i++, state);
	stmt.bind(i++, conversation);
	stmt.bind(i++, msgId);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateMessageInfo return error:" << ret 
			<< ", msgId:" << msgId << ", state:"<< state 
			<< ", timeStamp:" << timeStamp << ", seq:" << seq;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int MessageDao::DeleteMessage(int msgId) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"DELETE FROM TBL_MESSAGE WHERE MSG_ID = ?;");
	stmt.bind(1, msgId);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "DeleteMessage return error:" << ret 
			<< ", msgId:" << msgId;
		return DBError::DB_DELETE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int32_t MessageDao::UpdateRevocationMsgState(int32_t type, int64_t seqId, int64_t relateId, int32_t state)
{
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_MESSAGE SET SEND_STATE = ? WHERE MSG_SEQ = ? AND RELATE_ID = ? AND CONVERSATION_TYPE = ?;");
	int i = 1;
	stmt.bind(i++, state);
	stmt.bind(i++, seqId);
	stmt.bind(i++, relateId);
	stmt.bind(i++, type);
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateMessageInfo return error:" << ret
			<< ", revocation type:" << type << ", state:" << state
			<< ", relateId:" << relateId << ", seqId:" << seqId;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

//////////////////////////////////////////////////////////////
MessageID& MessageID::GetInstance() {
	static MessageID instance;
	return instance;
}

MessageID::MessageID()
	:_messageID(0) {

}

void MessageID::InitMessageID() {
	CppSQLite3Query query;
	CppSQLite3DB* db = SQLiteManager::GetInstance().GetUserDB();
	do 
	{
		string sql = "SELECT MAX(MSG_ID) FROM TBL_MESSAGE;";
		int ret = AppDao().Query(sql, db, query);
		if (ret != 0) {
			UC_LOG(ERROR) << "Get max message id return error:" << ret;
			break;
		}

		const char* messageId = query.fieldValue(0);
		if (messageId == NULL) {
			UC_LOG(INFO) << "get max message id return NULL!";
			break;
		}
		_messageID = atoi(messageId);
		UC_LOG(INFO) << "current max message id:" << _messageID;
	} while (false);
	SQLiteManager::GetInstance().ReleaseUserDB(db);
}

int32_t MessageID::GetMsgId() {
	int ret = 0;
	_msgIdMtx.lock();
	if (_messageID == std::numeric_limits<int32_t>::max()) {
		_messageID = 0;
	}
	ret = ++_messageID;
	_msgIdMtx.unlock();
	return ret;
}

MessageID::~MessageID() {

}

} //namespace uc
