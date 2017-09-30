#ifndef __MESSAGESTORE_H__
#define __MESSAGESTORE_H__

#include "UCSDK.h"
#include "MessageInfo.h"
#include "UCMessage.h"
#include <boost/thread.hpp>
#include <map>
#include <set>
#include <list>

#define MAX_ITEM_COUNT 1000

namespace uc {

using namespace std;

struct MessageStoreItem {
	MessageStoreItem() : relate_id(0), flag(0), is_played(0), msg(NULL),
		                 state(STATE_NULL), conversation_type(NONE_CHAT), del_msg(true),
						 oa_last_status(-1), app_ack_sent(1), oa_external_data("")
	{
	}
	MessageStoreItem(bool del_message) : relate_id(0), flag(0), is_played(0), msg(NULL),
										 state(STATE_NULL), conversation_type(NONE_CHAT), del_msg(del_message),
										 oa_last_status(-1), app_ack_sent(1), oa_external_data("")
	{
	}
	~MessageStoreItem() 
	{
		if (del_msg)
		{
			delete msg; 
		}
	}

	UCMsg				*msg;
	std::string			body; // 保存base64(thrift(body)) 后的数据
	int64_t				relate_id;	
	int32_t				flag;
	int32_t				is_played;
	MsgSendState		state;
	int32_t             oa_last_status;
	int32_t             app_ack_sent;
	std::string         oa_external_data;
	ConversationType	conversation_type;
	bool				del_msg;
};
typedef std::list< MessageStoreItem* > MessagesList;

enum  MSG_STATUS {MSG_FROM_MEMORY_SUCCESS = 0, MSG_FROM_DB_SUCCESS, MSG_NEED_FROM_SERVER };

struct MessagesListInfo
{
	MessagesListInfo() : min_timestamp(0), max_timestamp(0), message_sync(false) {}

	bool            message_sync;
	int64_t			min_timestamp;
	int64_t			max_timestamp;
	MessagesList	messages_list;
};

typedef std::map< int64_t, MessagesListInfo > LatestMessageCache;

class MessageCompare
{
public:
	MessageCompare(int64_t timestamp):_timestamp(timestamp){}
	bool operator()(const MessageStoreItem* message)
	{
		assert ( message );
		assert ( message->msg );

		UCHead head;
		message->msg->GetHead( head );
		return ( _timestamp == head.GetSeq() );
	}
private:
	int64_t _timestamp;
};

class MessageMsgIdCompare
{
public:
	MessageMsgIdCompare(int32_t msgid) : _msgid(msgid) {}
	bool operator()(const MessageStoreItem* message)
	{
		assert ( message );
		assert ( message->msg );

		UCHead head;
		message->msg->GetHead( head );
		return ( _msgid == head.GetId() );
	}
private:
	int32_t _msgid;
};

class MessageFlagCompare
{
public:
	MessageFlagCompare(int32_t flag):_flag(flag){}
	bool operator()(const MessageStoreItem* message)
	{
		assert ( message );
		return ( _flag == message->flag );
	}
private:
	int64_t _flag;
};

class MessageStore
{
public:

	/**
     * Constructor.
     */
	MessageStore();

	/**
     * Destructor.
     */
	~MessageStore();
	
   /**
	* Get one person's history chat messages
	* @param to_id              user id or group id
	* @param type               conversation type
	* @param count              the count user want to get
	* @param startTime          [in, out] starttime of chat
	* @param endTime            [in, out] endtime of chat
	* @param update_flag_msgid  [in, out] need to update this msgid in db. change flag from 1 to 0
	* @msglist					result of msg list
	* @return                   true or false
	*/
	MSG_STATUS GetChatHistory(int64_t to_id, ConversationType type, int64_t count, int64_t& starttime, 
		                      int64_t& endtime, int32_t& update_flag_msgid, /**out **/std::list<MessageStoreItem*>& msglist);

	/**
	* Save IM message in cache
	* @param to_userid          user id
	* @param msg                message content
	* @return                   true or false
	*/
	bool SaveIMMessage(int64_t to_userid, const MessageStoreItem* msg);

	/**
	* Save IM message in cache
	* @param to_userid          user id
	* @param msg                message content
	* @return                   true or false
	*/
	bool SaveIMMessage(int64_t to_userid, const MessageStoreItem& msg);

	/**
	* Update IM message status
	* @param to_userid          user id
	* @param msgid              local key of one message
	* @param state              MsgSendState
	* @param timestamp          message seq
	* @return                   true or false
	*/
	bool UpdateIMMessageState(int64_t to_userid, int32_t msgid, int32_t state, int64_t timestamp);

	/**
	* Sync message in cache and DB
	* @param to_id              user id or group id
	* @param conversation_type  conversation type
	* @param count              the count user want to get
	* @param startTime          starttime of chat
	* @param endTime            endtime of chat
	* @param update_flag_msgid  need to update this msgid in db. change flag from 1 to 0
	* @param msg_list           [in,out] message content list
	* @param same_endtime       it's flag indicate if endtime got from server is equal with endtime user want to get
	* @return                   true or false
	*/
	bool SyncMessages(int64_t to_id, int64_t conversation_type, int64_t count, int64_t starttime, 
		              int64_t endtime, int32_t update_flag_msgid, MessagesList& msg_list, bool same_endtime);

	/**
	* Save group message in cache
	* @param groupid            group id
	* @param msg                message content
	* @return                   true or false
	*/
	bool SaveGroupMessage(int64_t groupid, const MessageStoreItem* msg);

	/**
	* Save group message in cache
	* @param groupid            group id
	* @param msg                message content
	* @return                   true or false
	*/
	bool SaveGroupMessage(int64_t groupid, const MessageStoreItem& msg);

	/**
	* Update group message status
	* @param groupid            group id
	* @param msgid              local key of one message
	* @param state              MsgSendState
	* @param timestamp          message seq
	* @return                   true or false
	*/
	bool UpdateGroupMessageState(int64_t groupid, int32_t msgid, int32_t state, int64_t timestamp);

	/**
	* Save openapi message in cache
	* @param app_id             app id
	* @param msg                message content
	* @return                   true or false
	*/
	bool SaveOpenAPIMessage(int64_t app_id, const MessageStoreItem& msg);

	/**
	* Update openapi message status
	* @param appid              app id
	* @param msgid              local key of one message
	* @param state              MsgSendState
	* @param timestamp          message seq
	* @return                   true or false
	*/
	bool UpdateOpenAPIMessageState(int64_t appid, int32_t msgid, int32_t state, int64_t timestamp);


	/**
	* Update message status by seq
	* @param to_id              to_user_id or group id
	* @param seq                message seq
	* @param type               converastion_type
	* @param state              int
	* @return                   true or false
	*/
	bool SetMessageStatusBySeq(int64_t to_id, int64_t seq, int64_t type, int state);

	/**
	* Get message status by msgid
	* @param id                 message id
	* @param msg                [out] message content
	* @return                   true or false
	*/
	bool GetMessageById(int32_t msg_id, MessageStoreItem& msg);

	/**
	* Get openapi message status by msgseq;
	* @param appid              app id 
	* @param msgseq             message seq
	* @param msg                [out] message content
	* @return                   true or false
	*/
	bool GetOpenApiMessageById(int64_t appid, int64_t msgseq, OUT MessageStoreItem &msg);

	/**
	* update openapi oa message status;
	* @param appid              app id
	* @param msgseq             server key of one message
	* @param state              oa message status
	* @return                   true or false
	*/
	bool UpdateOpenAPIOAMessageState(int64_t appid, int64_t msgseq, int32_t state, const std::string oaExternalData);

	/**
	* update openapi message ack status;
	* @param appid              app id
	* @param msgseq             server key of one message
	* @param state              message status
	* @return                   true or false
	*/
	bool UpdateOpenAPIMessageAckState(int64_t appid, int64_t msgseq, int32_t state);

	/**
	* Update IM message from cache
	* @param to_userid          user id
	* @param timestamp          message seq
	* @param type				conversation type
	* @return                   true or false
	*/
	bool UpdateRevocationMessageState(int64_t to_userid, int64_t timestamp, int32_t type);

	/**
	* Update IM message from db
	* @param relateId           relate id
	* @param state              MsgSendState
	* @param type				conversation type
	* @param seqId				message seq
	* @return                   true or false
	*/
	bool UpdateRevocationMsgFromDB(int64_t relateId, MsgSendState state, int32_t type, int64_t seqId);
	
	/**
	* Clear message list
	* @param msglist            message list
	* @return                   true or false
	*/
	bool ClearMessageList(MessagesList& msglist);

	/**
	* Initialize cache data
	* @param to_id              user id or group id
	* @param conversation_type  conversation type
	* @return                   true or false
	*/
	bool InitializeCache(int64_t to_id, int64_t conversation_type);

	/**
	* Clear message cache in memory
	*/
	void Clear();

protected:
	MSG_STATUS _GetMsgHistory(MessagesListInfo& msglist_info, int64_t count, 
						      int64_t& starttime, int64_t& endtime, 
						      /**out **/std::list<MessageStoreItem*>& msglist );

	MSG_STATUS _GetMsgHistoryFromDB(int64_t to_id, int64_t conversation_type, int64_t count, int64_t& starttime, 
		int64_t& endtime, int32_t& update_flag_msgid,/**out **/std::list<MessageStoreItem*>& msglist);

	bool _SaveMessage(MessagesListInfo& msglist_info, const MessageStoreItem* msg);

	bool _SyncMessages(int64_t to_id, int64_t conversation_type, int64_t count, int64_t starttime, int64_t endtime, 
		               int32_t update_flag_msgid, MessagesList& msg_list, bool same_endtime);

	bool _UpdateMessageState(MessagesListInfo& msglist_info, int32_t msgid, int32_t state, int64_t timestamp);

	bool _ClearMessageList(MessagesList& msglist);

	bool _InitCacheData(int64_t to_id, int64_t conversation_type, int64_t starttime, int64_t endtime, MessagesList& msg_list, bool sync = false);

	void _SwapMessageItem(const MessageStoreItem* msg, MessageStoreItem* destMsg);
	
	bool _IsGroupType(int64_t conversation_type);
	bool _IsSingleType(int64_t conversation_type);
	bool _IsOpenapiType(int64_t conversation_type);

	bool _RevocationMsgFormCache(int64_t to_userid, MessagesListInfo& msglist_info, int64_t timestamp);

private:
	LatestMessageCache		_latest_im_msg_cache;
	LatestMessageCache		_latest_group_msg_cache;
	LatestMessageCache		_latest_openapi_msg_cache;
	boost::recursive_mutex	_msglock;
};

}

#endif
