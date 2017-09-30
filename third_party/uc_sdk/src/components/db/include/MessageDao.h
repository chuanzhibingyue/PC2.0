///////////////////////////////////////////////////////////
//  MessageDao.h
//  Implementation of the Class MessageDao
//  Created on:      22-五月-2015 10:35:49
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(MESSAGEDAO_H_)
#define MESSAGEDAO_H_

#include "AppDao.h"
#include "MessageInfo.h"
#include "store_service/StoreService.h"
#include <boost/thread/mutex.hpp>

#include <list>

namespace uc {

using namespace std;

class MessageDao : public AppDao {
public:
	MessageDao();
	virtual ~MessageDao();

	int SaveMsg(MessageStoreItem& message);
	int SaveMsg(list<MessageStoreItem*>& messageList);
	int SyncMsg(list<MessageStoreItem*>& messageList);
	int GetMsg(int messageId, MessageStoreItem& message);
	int GetMsg(int32_t conversation_type, int64_t relate_id, int64_t seq, MessageStoreItem& message);
	int GetMsgContentList(int32_t conversationType, int64_t relateId, int64_t startTime, int64_t endTime, int count, list<MessageStoreItem*>& message);
	int SetMessageStatusById(int msgId, int state);
	int SetMessageStatusBySeq(int64_t seq, int state);
	int SetMessageSeq(int msgId, int64_t time, int64_t seq);
	int UpdateMessageInfo(int msgId, MsgSendState state, int64_t timeStamp, int64_t seq, int32_t conversation);
	int UpdateMessageFlag(int msgId);
	int UpdateMessageFlag(int32_t conversation_type, int64_t relate_id, int64_t seq, int64_t flag);
	int DeleteMessage(int msgId);
	int GetMaxMessageSeq(int32_t conversation_type, int64_t relate_id, int64_t& max_seq);
	int IsMessageExist(int32_t conversation_type, int64_t relate_id, int64_t seq, int& count);
	int32_t UpdateRevocationMsgState(int32_t type, int64_t seqId, int64_t relateId, int32_t state);


private:
	void SetMessage(CppSQLite3Query& query, MessageStoreItem& message);

private:
	CppSQLite3DB* _db;

};

class MessageID {
public:
	virtual ~MessageID();
	static MessageID& GetInstance();
	void InitMessageID();
	int32_t GetMsgId();

private:
	MessageID();

private:
	boost::recursive_mutex _msgIdMtx;
	volatile int32_t _messageID;
};

}
#endif // !defined(MESSAGEDAO_H_)
