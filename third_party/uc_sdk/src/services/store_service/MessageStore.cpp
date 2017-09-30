#include "MessageStore.h"
#include "DBManager.h"
#include "LogManager.h"
#include "UCErrorCode.h"
#include "message_service/MessageProcessor.h"

namespace uc {

bool compare_timestamp(const MessageStoreItem* lhs, const MessageStoreItem* rhs)
{
	assert( lhs );
	assert( lhs->msg );
	assert( rhs );
	assert( rhs->msg );
	
	UCHead lhead, rhead;
	lhs->msg->GetHead( lhead );
	rhs->msg->GetHead( rhead );
	return ( lhead.GetSeq() >rhead.GetSeq() );
}

MessageStore::MessageStore()
{

}

MessageStore::~MessageStore()
{
	// TODO: message store 是单实例，跟SDK一个生命周期，在SDK unInit的时做了clear，析构时不需要再clear;
	// 如果以后message store变成多实例，有可能需要析构时做clear;
	//Clear();
}

MSG_STATUS MessageStore::GetChatHistory(int64_t to_id, ConversationType type, int64_t count, int64_t& starttime, 
	                                    int64_t& endtime, int32_t& update_flag_msgid, /**out **/std::list<MessageStoreItem*>& msglist)
{	
	UC_LOG(INFO) << "[msg_cache] get chat cache data. to_userid: " << to_id 
		<< " conversation_type: " << type << " count: " << count
		<< " starttime: " << starttime << " endtime: " << endtime;

	LatestMessageCache::iterator iter;
	if (_IsSingleType(type))
	{
		iter = _latest_im_msg_cache.find(to_id);
		if ( iter == _latest_im_msg_cache.end() )
		{
			InitializeCache(to_id, type);
			return _GetMsgHistoryFromDB(to_id, type, count, starttime, endtime, update_flag_msgid, msglist);
		}
	}
	else if (_IsGroupType(type))
	{
		iter = _latest_group_msg_cache.find(to_id);
		if (iter == _latest_group_msg_cache.end())
		{
			InitializeCache(to_id, type);
			return _GetMsgHistoryFromDB(to_id, type, count, starttime, endtime, update_flag_msgid, msglist);
		}
	}
	else if (_IsOpenapiType(type))
	{
		iter = _latest_openapi_msg_cache.find(to_id);
		if (iter == _latest_openapi_msg_cache.end())
		{
			InitializeCache(to_id, type);
			return _GetMsgHistoryFromDB(to_id, type, count, starttime, endtime, update_flag_msgid, msglist);
		}
	}
	else
	{
		UC_LOG(ERROR) << "conversation_type is wrong. we do not support this type so far. conversation_type: " << type;
		return MSG_NEED_FROM_SERVER;
	}

	MessagesListInfo& msglist_info = iter->second;
	if (_GetMsgHistory(msglist_info, count, starttime, endtime, msglist) != MSG_FROM_MEMORY_SUCCESS)
	{
		return _GetMsgHistoryFromDB(to_id, type, count, starttime, endtime, update_flag_msgid, msglist);
	}

	UC_LOG(INFO) << "get message list from memory cache success";
	return MSG_FROM_MEMORY_SUCCESS;
}
MSG_STATUS MessageStore::_GetMsgHistory(MessagesListInfo& msglist_info, int64_t count, 
								        int64_t& starttime, int64_t& endtime, 
								        /**out **/std::list<MessageStoreItem*>& msglist )
{
	UC_LOG(INFO) << "[msg_cache] the request param. count: "
		      << count << " starttime: " << starttime 
			  << " endtime: " << endtime;

	boost::unique_lock<boost::recursive_mutex> lock(_msglock);
	if ( msglist_info.messages_list.empty() )
	{
		UC_LOG(INFO) << "[msg_cache] cache data is empty, need to update from DB";
		return MSG_NEED_FROM_SERVER;
	}

	int64_t _internal_count = 0;
	if ( starttime == 0 && endtime == 0 )
	{
		if (!msglist_info.message_sync){
			UC_LOG(INFO) << "[msg_cache] cache data is not sync form server, need to update from DB";
			return MSG_NEED_FROM_SERVER;
		}
		MessagesList::iterator iter = msglist_info.messages_list.begin();
		for ( ; iter != msglist_info.messages_list.end(); iter++ )
		{
			msglist.push_back(*iter);
			_internal_count++;

			if (_internal_count == count)
				break;
		}
	}
	else if ( starttime == 0 )
	{
		if ( endtime > msglist_info.max_timestamp )
		{
			UC_LOG(INFO) << "[msg_cache] starttime == 0 but endtime is large than cache max_timestamp, so it needs get from DB. endtime: "
				      << endtime
					  << " cache_max_timestamp: "
					  << msglist_info.max_timestamp;
			return MSG_NEED_FROM_SERVER;
		}

		MessagesList::iterator iter = std::find_if(msglist_info.messages_list.begin(), 
												   msglist_info.messages_list.end(), 
												   MessageCompare(endtime) );

		if ( iter == msglist_info.messages_list.end() )
		{
			UC_LOG(INFO) << "[msg_cache] can not find endtime timestamp in cache, maybe lost message is cache, maybe wrong timestamp from upper layer. anyway, get data from DB again";
			return MSG_NEED_FROM_SERVER;
		}

		if ( ++iter == msglist_info.messages_list.end() )
		{
			UC_LOG(INFO) << "[msg_cache] the prev message can not find, get data from DB again";
			return MSG_NEED_FROM_SERVER;
		}

		if ( std::distance( iter, msglist_info.messages_list.end() ) < count )
		{
			UC_LOG(INFO) << "[msg_cache] cache data number is less than count, need to update from DB";
			return MSG_NEED_FROM_SERVER;
		}

		for ( ; iter != msglist_info.messages_list.end(); iter++ )
		{
			msglist.push_back(*iter);
			_internal_count++;

			if (_internal_count == count)
				break;
		}	
	}
	else if ( endtime == 0 )
	{
		if ( starttime < msglist_info.min_timestamp )
		{
			UC_LOG(INFO) << "[msg_cache] endtime == 0 but starttime is less than cache min_timestamp, so it needs get from DB. starttime: "
				<< starttime
				<< " cache_min_timestamp: "
				<< msglist_info.min_timestamp;
			return MSG_NEED_FROM_SERVER;
		}

		MessagesList::iterator iter = std::find_if(msglist_info.messages_list.begin(),
												   msglist_info.messages_list.end(),
												   MessageCompare(starttime) );

		if ( iter == msglist_info.messages_list.end() )
		{
			UC_LOG(INFO) << "[msg_cache] can not find start timestamp in cache, maybe lost message is cache, maybe wrong timestamp from upper layer. anyway, get data from DB again";
			return MSG_NEED_FROM_SERVER;
		}

		MessagesList::iterator iter_tmp = iter;
		_internal_count = std::distance( msglist_info.messages_list.begin(), ++iter_tmp );
		if ( _internal_count > count )
		{
			_internal_count = count;
		}

		while ( _internal_count > 0 )
		{
			msglist.push_front(*iter);
			_internal_count--;

			if ( _internal_count == 0 )
				break;
			iter--;
		}
	}
	else
	{
		if ( starttime < msglist_info.min_timestamp 
			|| endtime > msglist_info.max_timestamp )
		{
			UC_LOG(INFO) << "[msg_cache] starttime is less than cache min_timestamp or endtime is large than cache max_timestamp. starttime: "
				<< starttime
				<< " cache_min_timestamp: "
				<< msglist_info.min_timestamp
				<< " endtime: "
				<< endtime
				<< " cache_max_timestamp: "
				<< msglist_info.max_timestamp;
			return MSG_NEED_FROM_SERVER;
		}

		MessagesList::iterator iter_start = std::find_if(msglist_info.messages_list.begin(), 
														 msglist_info.messages_list.end(), 
														 MessageCompare(starttime) );
		if ( iter_start == msglist_info.messages_list.end() )
		{
			UC_LOG(INFO) << "[msg_cache] can not find start timestamp in cache, maybe lost message is cache, maybe wrong timestamp from upper layer. anyway, get data from DB again";
			return MSG_NEED_FROM_SERVER;
		}

		MessagesList::iterator iter_end = std::find_if(msglist_info.messages_list.begin(), 
													   msglist_info.messages_list.end(),
													   MessageCompare(endtime) );
		if ( iter_end == msglist_info.messages_list.end() )
		{
			UC_LOG(INFO) << "[msg_cache] can not find endtime timestamp in cache, maybe lost message is cache, maybe wrong timestamp from upper layer. anyway, get data from DB again";
			return MSG_NEED_FROM_SERVER;
		}

		_internal_count = std::distance( iter_end, iter_start++ );
		if ( _internal_count > count )
		{
			_internal_count = count;
		}

		while ( _internal_count > 0 )
		{
			msglist.push_back(*iter_end);
			_internal_count--;
			iter_end++;
		}
	}

	return MSG_FROM_MEMORY_SUCCESS;
}

MSG_STATUS MessageStore::_GetMsgHistoryFromDB(int64_t to_id, int64_t conversation_type, int64_t count, int64_t& starttime,
	                                          int64_t& endtime, int32_t& update_flag_msgid,/**out **/std::list<MessageStoreItem*>& msglist)

{
	boost::unique_lock<boost::recursive_mutex> lock(_msglock);

	if ( starttime == 0 && endtime == 0 )
	{
		UC_LOG(INFO) << "[msg_cache] starttime and endtime is 0. need to update latest messages from server";
		DBManager::GetInstance().GetMaxMessageSeq( (int)conversation_type, to_id, starttime );
		UC_LOG(INFO) << "[msg_cache] get max message seq from DB. seq: " << starttime;
		return MSG_NEED_FROM_SERVER;
	}
	else if ( starttime == 0 )
	{
		UC_LOG(INFO) << "[msg_cache] ready to get data from DB. starttime: " << starttime
			<< " endtime: " << endtime;

		DBManager::GetInstance().GetMessage( (int)conversation_type, to_id, starttime, endtime, (int)count, msglist );
		if (msglist.empty()){
			update_flag_msgid = -1;
			return MSG_NEED_FROM_SERVER;
		}

		MessageStoreItem item;
		int ret = DBManager::GetInstance().GetMessage((int)conversation_type, to_id, endtime, item);
		if ((ret == DBError::DB_SUCCESS && item.flag == 1) || ret == DBError::DB_QUERY_EMPTY){
			MessagesList::iterator iterPreValid = msglist.begin();
			MessageStoreItem* latest_item = (*iterPreValid);
			while (latest_item->state == STATE_SENDFAIL && iterPreValid != msglist.end())
			{
				latest_item = (*++iterPreValid);
			}
			assert(latest_item);

			UCHead head;
			latest_item->msg->GetHead(head);
			starttime = head.GetSeq();

			_ClearMessageList(msglist);
			update_flag_msgid = -1;
			return MSG_NEED_FROM_SERVER;
		}

		MessagesList::iterator iter = std::find_if(msglist.begin(), msglist.end(), MessageFlagCompare(1) );
		MessagesList::iterator iter_tmp = iter;

		if ( iter_tmp == msglist.end() )
		{
			return MSG_FROM_DB_SUCCESS;
		}

		if ( ++iter_tmp == msglist.end() )
		{
			if (msglist.size() >= count )
			{
				return MSG_FROM_DB_SUCCESS;
			}
			else
			{
				MessageStoreItem* message = (*iter);
				assert( message->msg );

				UCHead head;
				message->msg->GetHead( head );
				endtime = head.GetSeq();
				update_flag_msgid = head.GetId();

				starttime = 0;
			}
		}
		else
		{
			MessageStoreItem* message = (*iter);
			assert( message->msg );

			UCHead head;
			message->msg->GetHead( head );
			endtime = head.GetSeq();
			update_flag_msgid = head.GetId();

			// 找flag前一条msg（不能是失败的）bug 5564
			MessageStoreItem* message_t = (*++iter);
			while (message_t->state == STATE_SENDFAIL && iter != msglist.end())
			{
				++iter;
				if (iter != msglist.end())
					message_t = (*iter);
			}
			
			assert( message_t->msg );

			UCHead head1;
			message_t->msg->GetHead( head1 );
			starttime = head1.GetSeq();
		}

		_ClearMessageList( msglist );

		UC_LOG(INFO) << "[msg_cache] it needs to get from server. starttime: " << starttime
			<< " endtime: " << endtime;

		return MSG_NEED_FROM_SERVER;
	}
	else if ( endtime == 0 )
	{
		UC_LOG(INFO) << "[msg_cache] ready to get data from server. starttime: " << starttime
			<< " endtime: " << endtime;
		return MSG_NEED_FROM_SERVER;
	}
	else
	{
		UC_LOG(WARNING) << "[msg_cache] there has no case: starttime and endtime are not null so far. but we can reserve for updating data from server to db later";
		return MSG_FROM_DB_SUCCESS;
	}

	return MSG_FROM_DB_SUCCESS;
}

bool MessageStore::SaveIMMessage(int64_t to_userid, const MessageStoreItem* msg)
{
	UC_LOG(INFO) << "[msg_cache] save IM message in cache. to_userid: " << to_userid
		<< " msg: " << msg;
	if ( NULL == msg )
	{
		return false;
	}

	return SaveIMMessage( to_userid, *msg );

}

bool MessageStore::SaveIMMessage(int64_t to_userid, const MessageStoreItem& msg)
{
	UC_LOG(INFO) << "[msg_cache] SaveIMMessage in cache. to_userid: " << to_userid;
	assert( msg.msg );
	boost::unique_lock<boost::recursive_mutex> lock(_msglock);

	LatestMessageCache::iterator iter = _latest_im_msg_cache.find( to_userid );
	if ( iter == _latest_im_msg_cache.end() )
	{
		UC_LOG(INFO) << "[msg_cache] can not find this user in cache. so we do not save this message. to_userid: " << to_userid;
		return true;
	}

	UCMsg* uc_msg = NULL;
	UCHead head;
	msg.msg->GetHead( head );
	if (MessageProcessor::GetUCMsg( head, msg.body, uc_msg ) != UC_SUCCEEDED )
	{
		UC_LOG(ERROR) << "[msg_cache] getucmsg is failed";
		return false;
	}

	MessageStoreItem *info = new MessageStoreItem();
	info->msg = uc_msg;
	info->body = msg.body.c_str();
	info->conversation_type = msg.conversation_type;
	info->flag = msg.flag;
	info->is_played = msg.is_played;
	info->relate_id = msg.relate_id;
	info->state = msg.state;

	MessagesListInfo& msglist_info = iter->second;
	return _SaveMessage( msglist_info, info );
}

bool MessageStore::SaveGroupMessage(int64_t groupid, const MessageStoreItem* msg)
{
	UC_LOG(INFO) << "[msg_cache] save group message in cache. groupid: " << groupid
		<< " msg: " << msg;
	if ( NULL == msg )
	{
		return false;
	}
	
	return SaveGroupMessage( groupid, *msg );
}

bool MessageStore::SaveGroupMessage(int64_t groupid, const MessageStoreItem& msg)
{
	UC_LOG(INFO) << "[msg_cache] SaveGroupMessage in cache. groupid: " << groupid;
	assert( msg.msg );
	boost::unique_lock<boost::recursive_mutex> lock(_msglock);

	LatestMessageCache::iterator iter = _latest_group_msg_cache.find( groupid );
	if ( iter == _latest_group_msg_cache.end() )
	{
		UC_LOG(INFO) << "[msg_cache] can not find this group in cache. so we do not save this message. groupid: " << groupid;
		return true;
	}

	UCMsg* uc_msg = NULL;
	UCHead head;
	msg.msg->GetHead( head );
	if (MessageProcessor::GetUCMsg( head, msg.body, uc_msg ) != UC_SUCCEEDED )
	{
		UC_LOG(ERROR) << "[msg_cache] getucmsg is failed";
		return false;
	}

	MessageStoreItem *info = new MessageStoreItem();
	info->msg = uc_msg;
	info->body = msg.body.c_str();
	info->conversation_type = msg.conversation_type;
	info->flag = msg.flag;
	info->is_played = msg.is_played;
	info->relate_id = msg.relate_id;
	info->state = msg.state;

	MessagesListInfo& msglist_info = iter->second;
	return _SaveMessage( msglist_info, info );
}

bool MessageStore::SaveOpenAPIMessage(int64_t app_id, const MessageStoreItem& msg)
{
	UC_LOG(INFO) << "[msg_cache] SaveOpenAPIMessage in cache. app_id: " << app_id;
	assert(msg.msg);
	boost::unique_lock<boost::recursive_mutex> lock(_msglock);

	LatestMessageCache::iterator iter = _latest_openapi_msg_cache.find(app_id);
	if (iter == _latest_openapi_msg_cache.end())
	{
		UC_LOG(INFO) << "[msg_cache] can not find this openapi appid in cache. so we do not save this message. appid: " << app_id;
		return true;
	}

	UCMsg* uc_msg = NULL;
	UCHead head;
	msg.msg->GetHead(head);
	if (MessageProcessor::GetUCMsg(head, msg.body, uc_msg) != UC_SUCCEEDED)
	{
		UC_LOG(ERROR) << "[msg_cache] getucmsg is failed";
		return false;
	}

	MessageStoreItem *info = new MessageStoreItem();
	info->msg = uc_msg;
	info->body = msg.body.c_str();
	info->conversation_type = msg.conversation_type;
	info->flag = msg.flag;
	info->is_played = msg.is_played;
	info->relate_id = msg.relate_id;
	info->state = msg.state;

	MessagesListInfo& msglist_info = iter->second;
	return _SaveMessage(msglist_info, info);
}

bool MessageStore::_SaveMessage(MessagesListInfo& msglist_info, const MessageStoreItem* msg)
{
	UCHead head;
	msg->msg->GetHead( head );

	UC_LOG(INFO) << "[msg_cache] save message in cache. msgid: " << head.GetId()
		<< " msgseq: " << head.GetSeq() << " content: " << msg->body;

	if ( head.GetSeq() == 0 )
	{
		UC_LOG(INFO) << "[msg_cache] msg seq is 0. so we give temp seq == max_timestamp +1.";
		head.SetSeq( msglist_info.max_timestamp + 1 );
		msg->msg->SetHead( head );
	}
	MessagesList::iterator iter = std::find_if( msglist_info.messages_list.begin(), 
												msglist_info.messages_list.end(), 
												MessageMsgIdCompare( head.GetId() ) );
	if ( iter != msglist_info.messages_list.end() )
	{

		MessagesList::iterator iter_seq = std::find_if(msglist_info.messages_list.begin(),
													   msglist_info.messages_list.end(),
													   MessageCompare(head.GetSeq()));
		if (iter_seq != msglist_info.messages_list.end())
		{
			UC_LOG(INFO) << "[msg_cache] there has the same msgid and msgseq in cache. so ignore it.";
			delete msg;
			return true;
		}

		UC_LOG(INFO) << "[msg_cache] there has the same msgid but msgseq is different in cache. so we need to change msgid then save it.";

		head.SetId(DBManager::GetInstance().GetMessageID());
		msg->msg->SetHead(head);
	}

	if ( msglist_info.messages_list.size() < MAX_ITEM_COUNT )
	{
		msglist_info.messages_list.push_front( const_cast<MessageStoreItem*>(msg) );
		if ( head.GetSeq() > msglist_info.max_timestamp )
		{
			msglist_info.max_timestamp = head.GetSeq();
		}
	}
	else
	{
		msglist_info.messages_list.push_front( const_cast<MessageStoreItem*>(msg) );
		MessageStoreItem* message = msglist_info.messages_list.back();
		msglist_info.messages_list.pop_back();
		delete message;

		if ( head.GetSeq() > msglist_info.max_timestamp )
		{
			msglist_info.max_timestamp = head.GetSeq();
		}
		
		UCHead last_head;
		MessageStoreItem* item = msglist_info.messages_list.back();
		assert( item->msg );
		item->msg->GetHead( last_head );

		if ( last_head.GetSeq() > 0 &&
			last_head.GetSeq() < msglist_info.min_timestamp )
		{
			msglist_info.min_timestamp = last_head.GetSeq();
		}				
	}
	UC_LOG(INFO) << "[msg_cache] max_timestamp: " << msglist_info.max_timestamp
		<< " min_timestamp: " << msglist_info.min_timestamp;

	return true;
}

bool MessageStore::SyncMessages(int64_t to_id, int64_t conversation_type,  int64_t count, int64_t starttime,
	                            int64_t endtime, int32_t update_flag_msgid, MessagesList& msg_list, bool same_endtime)
{
	UC_LOG(INFO) << "[msg_cache] sync message in cache. to_id: " << to_id << " conversation_type: " << conversation_type
		<< " count: " << count << " starttime: " << starttime << " endtime: " << endtime;

	return _SyncMessages( to_id, conversation_type, count, starttime, endtime, update_flag_msgid, msg_list, same_endtime );
}

bool MessageStore::_SyncMessages(int64_t to_id, int64_t conversation_type, int64_t count, int64_t starttime, int64_t endtime,
	int32_t update_flag_msgid, MessagesList& msg_list, bool same_endtime)
{
	boost::unique_lock<boost::recursive_mutex> lock(_msglock);
	if (update_flag_msgid == -1)
	{
		MessageStoreItem item;
		if (DBManager::GetInstance().GetMessage((int)conversation_type, to_id, endtime, item) == DBError::DB_SUCCESS
			&& item.flag == 1)
		{
			assert(item.msg);
			UCHead head;
			item.msg->GetHead(head);

			if (DBManager::GetInstance().UpdateMessageFlag(head.GetId()) != DBError::DB_SUCCESS)
			{
				UC_LOG(ERROR) << "[msg_cache] update message flag to 0 failed.";
				return false;
			}
		}
	}

	if (update_flag_msgid > 0)
	{
		if (DBManager::GetInstance().UpdateMessageFlag(update_flag_msgid) != DBError::DB_SUCCESS)
		{
			UC_LOG(ERROR) << "[msg_cache] update message flag to 0 failed.";
			return false;
		}
	}

	if ( msg_list.empty() )
	{
		UC_LOG(INFO) << "[msg_cache] sync message list is empty.";
		int ret = DBManager::GetInstance().GetMessage( (int)conversation_type, to_id, starttime, endtime, (int)count, msg_list );

		if (ret == DBError::DB_QUERY_EMPTY)
		{
			UC_LOG(ERROR) << "[msg_cache] get message list from db empty. ";
			return true;
		}

		if ( ret != DBError::DB_SUCCESS )
		{
			UC_LOG(ERROR) << "[msg_cache] get message list from db error. error code: " << ret;
			return false;
		}
		_InitCacheData( to_id, conversation_type, starttime, endtime, msg_list, true);
		return true;
	}

	msg_list.sort( compare_timestamp );
	if ( msg_list.size() >= count )
	{
		UC_LOG(INFO) << "[msg_cache] msg list got from server is large than count, we need to set flag to 1 on last item.";
		msg_list.back()->flag = 1;
	}

	int ret = DBManager::GetInstance().SyncMsg( msg_list );
	if ( ret != DBError::DB_SUCCESS )
	{
		UC_LOG(ERROR) << "[msg_cache] save message in DB error. error code: " << ret;
		return false;
	}

	if ( msg_list.size() < count || !same_endtime )
	{
		UC_LOG(INFO) << "[msg_cache] msg list got from server is less than count or same endtime is false, we need to sync up from db.";
		_ClearMessageList( msg_list );

		int ret1 = DBManager::GetInstance().GetMessage( (int)conversation_type, to_id, starttime, endtime, (int)count, msg_list );

		if (ret1 == DBError::DB_QUERY_EMPTY)
		{
			UC_LOG(ERROR) << "[msg_cache] get message list from db empty. ";
			return true;
		}

		if ( ret1 != DBError::DB_SUCCESS )
		{
			UC_LOG(ERROR) << "[msg_cache] get message list from db error. error code: " << ret1;
			return false;
		}
	}
	_InitCacheData( to_id, conversation_type, starttime, endtime, msg_list, true);

	return true;
}

bool MessageStore::InitializeCache(int64_t to_id, int64_t conversation_type)
{
	boost::unique_lock<boost::recursive_mutex> lock(_msglock);
	MessagesList msglist;
	msglist.clear();
	return _InitCacheData(to_id, conversation_type, 0, 0, msglist);
}

bool MessageStore::_InitCacheData(int64_t to_id, int64_t conversation_type, int64_t starttime, int64_t endtime, MessagesList& msg_list, bool sync)
{
	UC_LOG(INFO) << "init cache data. to_id: " << to_id << " conversation_type: " << conversation_type
		<< " starttime: " << starttime << " endtime: " << endtime << " sync: " << sync;

	if ( starttime != 0 || endtime != 0 )
	{
		UC_LOG(INFO) << "it does not need to init cache data. since it has already init";
		return true;
	}

	if (_IsSingleType(conversation_type))
	{
		LatestMessageCache::iterator iter = _latest_im_msg_cache.find( to_id );
		if (iter != _latest_im_msg_cache.end() && iter->second.message_sync)
		{
			UC_LOG(INFO) << "it does not need to init cache data. since it has already init. _latest_im_msg_cache is not empty";
			return true;
		}
	}
	else if (_IsGroupType(conversation_type))
	{
		LatestMessageCache::iterator iter = _latest_group_msg_cache.find( to_id );
		if (iter != _latest_group_msg_cache.end() && iter->second.message_sync)
		{
			UC_LOG(INFO) << "it does not need to init cache data. since it has already init. _latest_group_msg_cache is not empty";
			return true;
		}
	}
	else if (_IsOpenapiType(conversation_type))
	{
		LatestMessageCache::iterator iter = _latest_openapi_msg_cache.find(to_id);
		if (iter != _latest_openapi_msg_cache.end() && iter->second.message_sync)
		{
			UC_LOG(INFO) << "it does not need to init cache data. since it has already init. _latest_openapi_msg_cache is not empty";
			return true;
		}
	}
	else
	{
		UC_LOG(WARNING) << "we do not support this conversation type so far";
		return false;
	}

	MessagesListInfo msglist_info;
	for ( MessagesList::const_iterator msg_iter = msg_list.begin(); msg_iter != msg_list.end(); ++msg_iter )
	{
		assert( (*msg_iter)->msg );
		UCMsg* uc_msg = NULL;
		UCHead head_item;
		(*msg_iter)->msg->GetHead( head_item );
		if ( MessageProcessor::GetUCMsg( head_item, (*msg_iter)->body, uc_msg) != UC_SUCCEEDED )
			continue;

		MessageStoreItem *info = new MessageStoreItem();
		info->msg = uc_msg;
		_SwapMessageItem(*msg_iter, info);
		info->app_ack_sent = (*msg_iter)->app_ack_sent;
		info->oa_last_status = (*msg_iter)->oa_last_status;
		info->oa_external_data = (*msg_iter)->oa_external_data;

		msglist_info.messages_list.push_back( info );
	}

	int64_t max_seq = 0;
	if ( _IsSingleType(conversation_type) )
	{
		if (!sync){
			
			_latest_im_msg_cache[to_id] = msglist_info;
			return true;
		}

		if (msg_list.empty()){
			return true;
		}
		UCHead front_head;
		UCHead end_head;

		assert(msg_list.front()->msg);
		assert(msg_list.back()->msg);

		msg_list.front()->msg->GetHead(front_head);
		msg_list.back()->msg->GetHead(end_head);

		max_seq = front_head.GetSeq();

		for (auto &item : _latest_im_msg_cache[to_id].messages_list){
			if (item->state != STATE_SENDING){
				continue;
			}
			UCHead head;
			item->msg->GetHead(head);
			head.SetSeq(++max_seq);
		}

		for (auto &item : msglist_info.messages_list){
			_latest_im_msg_cache[to_id].messages_list.push_back(item);
		}

		_latest_im_msg_cache[to_id].message_sync = true;
		_latest_im_msg_cache[to_id].max_timestamp = max_seq;
		_latest_im_msg_cache[to_id].min_timestamp = end_head.GetSeq();

		_ClearMessageList(msg_list);
		int64_t _internal_count = 0;
		for (auto item : _latest_im_msg_cache[to_id].messages_list){
			assert((*item).msg);
			UCMsg* uc_msg = NULL;
			UCHead head_item;
			(*item).msg->GetHead(head_item);
			if (MessageProcessor::GetUCMsg(head_item, (*item).body, uc_msg) != UC_SUCCEEDED)
				continue;

			MessageStoreItem* store_item = new MessageStoreItem();
			store_item->msg = uc_msg;
			_SwapMessageItem(item, store_item);
			msg_list.push_back(store_item);
			_internal_count++;
			if (_internal_count == 10)
				break;
		}

		UC_LOG(INFO) << "[msg_cache] starttime and endtime is null and there has no data in cache. we need cache latest data in memory. max_timestamp: " << max_seq
			<< " min_timestamp: " << end_head.GetSeq();
	}

	if ( _IsGroupType(conversation_type))
	{
		if (!sync){
			_latest_group_msg_cache[to_id] = msglist_info;
			return true;
		}

		if(msg_list.empty()){
			return true;
		}
		UCHead front_head;
		UCHead end_head;

		assert(msg_list.front()->msg);
		assert(msg_list.back()->msg);

		msg_list.front()->msg->GetHead(front_head);
		msg_list.back()->msg->GetHead(end_head);

		max_seq = front_head.GetSeq();

		for (auto &item : _latest_group_msg_cache[to_id].messages_list){
			if (item->state != STATE_SENDING){
				continue;
			}
			UCHead head;
			item->msg->GetHead(head);
			head.SetSeq(++max_seq);
		}

		for (auto item : msglist_info.messages_list){
			_latest_group_msg_cache[to_id].messages_list.push_back(item);
		}
		_latest_group_msg_cache[to_id].message_sync = true;	
		_latest_group_msg_cache[to_id].max_timestamp = max_seq;
		_latest_group_msg_cache[to_id].min_timestamp = end_head.GetSeq();

		_ClearMessageList(msg_list);
		int64_t _internal_count = 0;

		for (auto item : _latest_group_msg_cache[to_id].messages_list){
			assert((*item).msg);
			UCMsg* uc_msg = NULL;
			UCHead head_item;
			(*item).msg->GetHead(head_item);
			if (MessageProcessor::GetUCMsg(head_item, (*item).body, uc_msg) != UC_SUCCEEDED)
				continue;

			MessageStoreItem* store_item = new MessageStoreItem();
			store_item->msg = uc_msg;
			_SwapMessageItem(item, store_item);
			msg_list.push_back(store_item);
			_internal_count++;
			if (_internal_count == 10)
				break;
		}
		UC_LOG(INFO) << "[msg_cache] starttime and endtime is null and there has no data in cache. we need cache latest data in memory. max_timestamp: " << max_seq
			<< " min_timestamp: " << end_head.GetSeq();
	}

	if ( _IsOpenapiType(conversation_type) )
	{

		if (!sync){
			_latest_openapi_msg_cache[to_id] = msglist_info;
			return true;
		}

		if (msg_list.empty()){
			return true;
		}

		UCHead front_head;
		UCHead end_head;

		assert(msg_list.front()->msg);
		assert(msg_list.back()->msg);

		msg_list.front()->msg->GetHead(front_head);
		msg_list.back()->msg->GetHead(end_head);
		max_seq = front_head.GetSeq();
		for (auto &item : _latest_openapi_msg_cache[to_id].messages_list){
			if (item->state != STATE_SENDING){
				continue;
			}
			UCHead head;
			item->msg->GetHead(head);
			head.SetSeq(++max_seq);
		}
		for (auto item : msglist_info.messages_list){
			_latest_openapi_msg_cache[to_id].messages_list.push_back(item);
		}

		_latest_openapi_msg_cache[to_id].message_sync = true;
		_latest_openapi_msg_cache[to_id].max_timestamp = max_seq;
		_latest_openapi_msg_cache[to_id].min_timestamp = end_head.GetSeq();

		_ClearMessageList(msg_list);
		int64_t _internal_count = 0;
		for (auto item : _latest_openapi_msg_cache[to_id].messages_list){
			assert((*item).msg);
			UCMsg* uc_msg = NULL;
			UCHead head_item;
			(*item).msg->GetHead(head_item);
			if (MessageProcessor::GetUCMsg(head_item, (*item).body, uc_msg) != UC_SUCCEEDED)
				continue;
			MessageStoreItem* store_item = new MessageStoreItem();
			store_item->msg = uc_msg;
			_SwapMessageItem(item, store_item);
			msg_list.push_back(store_item);
			_internal_count++;
			if (_internal_count == 10)
				break;
		}

		UC_LOG(INFO) << "[msg_cache] starttime and endtime is null and there has no data in cache. we need cache latest data in memory. max_timestamp: " << max_seq
			<< " min_timestamp: " << end_head.GetSeq();
	}

	return true;
}

void MessageStore::_SwapMessageItem(const MessageStoreItem* msg, MessageStoreItem* destMsg){
	destMsg->body = msg->body;
	destMsg->conversation_type = msg->conversation_type;
	destMsg->flag = msg->flag;
	destMsg->is_played = msg->is_played;
	destMsg->relate_id = msg->relate_id;
	destMsg->state = msg->state;
	destMsg->app_ack_sent = msg->app_ack_sent;
	destMsg->oa_last_status = msg->oa_last_status;
}

bool MessageStore::UpdateRevocationMsgFromDB(int64_t relateId, MsgSendState state, int32_t type, int64_t seqId)
{
	UC_LOG(INFO) << "[msg_cache] update revocation message state. relateId: " << relateId << " state: " 
		<< state<< " revocation type: " << type << " seqId: " << seqId;

	if (DBManager::GetInstance().UpdateRevocationMsgState(type, seqId, relateId, state))
	{
		UC_LOG(ERROR) << "[revocation_msg] update db failed when the message is revoked";
	}
	return true;
}

bool MessageStore::UpdateRevocationMessageState(int64_t to_userid, int64_t timestamp, int32_t type)
{
	UC_LOG(INFO) << "[msg_cache] update revocation message state. to_userid: " << to_userid << 
		" timestamp: " << timestamp<< " revocation type: " << type;

	LatestMessageCache::iterator iter;
	if (type == SINGLE_CHAT)
	{
		iter = _latest_im_msg_cache.find(to_userid);
		if (iter == _latest_im_msg_cache.end())
		{
			UC_LOG(INFO) << "[msg_cache] can not find this user in cache. so we do not revocation this message. to_userid: " << to_userid;
			return true;
		}
		MessagesListInfo& msglist_info = iter->second;
		if (!_RevocationMsgFormCache(to_userid, msglist_info, timestamp))
		{
			return false;
		}
	}
	else
	{
		iter = _latest_group_msg_cache.find(to_userid);
		if (iter == _latest_group_msg_cache.end())
		{
			UC_LOG(INFO) << "[msg_cache] can not find this user in cache. so we do not revocation this message. to_userid: " << to_userid;
			return true;
		}
		MessagesListInfo& msglist_info = iter->second;
		if (!_RevocationMsgFormCache(to_userid, msglist_info, timestamp))
		{
			return false;
		}
	}

	return true;
}

bool MessageStore::_RevocationMsgFormCache(int64_t to_userid, MessagesListInfo& msglist_info, int64_t timestamp)
{
	if (msglist_info.messages_list.empty())
	{
		UC_LOG(INFO) << "[msg_cache] cache data is empty, need to update from DB";
		return false;
	}

	MessagesList::iterator iter = std::find_if(msglist_info.messages_list.begin(), msglist_info.messages_list.end(), MessageCompare(timestamp));
	if (iter == msglist_info.messages_list.end())
	{
		UC_LOG(INFO) << "[msg_cache] can not find this user in cache. so we do not revocation this message. to_userid: " << to_userid;
		return false;
	}
	msglist_info.messages_list.erase(iter);
	return true;
}

bool MessageStore::UpdateIMMessageState(int64_t to_userid, int32_t msgid, int32_t state, int64_t timestamp)
{
	UC_LOG(INFO) << "[msg_cache] update im message state. to_userid: " << to_userid
		<< " msgid: " << msgid << " state: " << state << " timestamp: " << timestamp;

	boost::unique_lock<boost::recursive_mutex> lock(_msglock);
	LatestMessageCache::iterator iter = _latest_im_msg_cache.find( to_userid );
	if ( iter != _latest_im_msg_cache.end() )
	{
		MessagesListInfo& msglist_info = iter->second;
		return _UpdateMessageState( msglist_info, msgid, state, timestamp );
	}
	else
	{
		UC_LOG(ERROR) << "[msg_cache] it's strange. why cannot find this user in memory?";
	}

	return true;
}

bool MessageStore::UpdateGroupMessageState(int64_t groupid, int32_t msgid, int32_t state, int64_t timestamp)
{
	UC_LOG(INFO) << "[msg_cache] update group message state. groupid: " << groupid
		<< " msgid: " << msgid << " state: " << state << " timestamp: " << timestamp;

	boost::unique_lock<boost::recursive_mutex> lock(_msglock);
	LatestMessageCache::iterator iter = _latest_group_msg_cache.find( groupid );
	if ( iter != _latest_group_msg_cache.end() )
	{
		MessagesListInfo& msglist_info = iter->second;
		return _UpdateMessageState( msglist_info, msgid, state, timestamp );
	}
	else
	{
		UC_LOG(ERROR) << "[msg_cache] it's strange. why cannot find this group in memory?";
	}

	return true;
}

bool MessageStore::UpdateOpenAPIMessageState(int64_t appid, int32_t msgid, int32_t state, int64_t timestamp){
	UC_LOG(INFO) << "[msg_cache] update openapi message state. appid: " << appid
		<< " msgid: " << msgid << " state: " << state << " timestamp: " << timestamp;

	boost::unique_lock<boost::recursive_mutex> lock(_msglock);
	LatestMessageCache::iterator iter = _latest_openapi_msg_cache.find(appid);
	if (iter != _latest_openapi_msg_cache.end())
	{
		MessagesListInfo& msglist_info = iter->second;
		return _UpdateMessageState(msglist_info, msgid, state, timestamp);
	}
	else
	{
		UC_LOG(ERROR) << "[msg_cache] it's strange. why cannot find this openapi in memory?";
	}

	return true;
}

bool MessageStore::_UpdateMessageState(MessagesListInfo& msglist_info, int32_t msgid, int32_t state, int64_t timestamp)
{
	MessagesList::iterator iter = std::find_if( msglist_info.messages_list.begin(), 
		                                        msglist_info.messages_list.end(), 
		                                        MessageMsgIdCompare( msgid ) );
	if ( iter == msglist_info.messages_list.end() )
	{
		UC_LOG(ERROR) << "[msg_cache] it's strange. why cannot find this message in memory?";
		return true;
	}

	MsgSendState old_state = (*iter)->state;
	(*iter)->state = (MsgSendState)state;
	if ( timestamp > 0 )
	{
		assert( (*iter)->msg );
		UCHead head;
		(*iter)->msg->GetHead( head );
		head.SetSeq( timestamp );
		head.SetTimestamp( timestamp );
		(*iter)->msg->SetHead( head );
	}
	
	if ( timestamp > msglist_info.max_timestamp )
	{
		msglist_info.max_timestamp = timestamp;
	}

	if ( STATE_SENDFAIL == state )
	{
		UC_LOG(INFO) << "[msg_cache] state == STATE_SENDFAIL. we need save this message in db.";
		DBManager::GetInstance().SaveMessage( **iter );
	}

	if ( STATE_SENDFAIL == old_state && STATE_SENDSUCCESS == state )
	{
		UC_LOG(INFO) << "[msg_cache] oldstate == STATE_SENDFAIL and STATE_SENDSUCCESS == state.delete this message from db.";
		DBManager::GetInstance().DeleteMessage( msgid );
	}

	return true;
}

bool MessageStore::SetMessageStatusBySeq(int64_t to_id, int64_t seq, int64_t type, int state)
{
	UC_LOG(INFO) << "[msg_cache] set message status by seq. to_id: " << to_id
		<< " seq: " << seq << " type: " << type << " state: " << state;

	if (_IsSingleType(type))
	{
		LatestMessageCache::iterator iter = _latest_im_msg_cache.find( to_id );
		if ( iter != _latest_im_msg_cache.end() )
		{
			MessagesList& msg_list = iter->second.messages_list;
			MessagesList::iterator iter1 = std::find_if(msg_list.begin(), 
														msg_list.end(), 
														MessageCompare(seq) );

			if ( iter1 != msg_list.end() )
			{
				(*iter1)->is_played = state;
			}
		}
	}
	else if (_IsGroupType(type))
	{
		LatestMessageCache::iterator iter = _latest_group_msg_cache.find( to_id );
		if ( iter != _latest_group_msg_cache.end() )
		{
			MessagesList& msg_list = iter->second.messages_list;
			MessagesList::iterator iter1 = std::find_if(msg_list.begin(), 
														msg_list.end(), 
														MessageCompare(seq) );

			if ( iter1 != msg_list.end() )
			{
				(*iter1)->is_played = state;
			}
		}
	}
	else if (_IsOpenapiType(type))
	{
		LatestMessageCache::iterator iter = _latest_openapi_msg_cache.find(to_id);
		if (iter != _latest_openapi_msg_cache.end())
		{
			MessagesList& msg_list = iter->second.messages_list;
			MessagesList::iterator iter1 = std::find_if(msg_list.begin(),
				msg_list.end(),
				MessageCompare(seq));

			if (iter1 != msg_list.end())
			{
				(*iter1)->is_played = state;
			}
		}
	}
	else
	{
		UC_LOG(ERROR) << "[msg_cache] we do not support this type so far. type: " << type;
	}

	return true;
}

bool MessageStore::ClearMessageList(MessagesList& msglist)
{
	boost::unique_lock<boost::recursive_mutex> lock(_msglock);
	return _ClearMessageList( msglist );
}

bool MessageStore::_ClearMessageList(MessagesList& msglist)
{
	for ( MessagesList::iterator it = msglist.begin(); it != msglist.end(); ++it )
	{
		delete ( *it );
	}
	msglist.clear();

	return true;
}

bool MessageStore::GetMessageById(int32_t msg_id, MessageStoreItem& msg)
{
	UC_LOG(INFO) << "get message by msgid. msg_id: " << msg_id;

	int32_t result = DBManager::GetInstance().GetMessage(msg_id, msg);
	if(DBError::DB_SUCCESS != result)
	{
		return false;
	}

	return true;
}


bool MessageStore::GetOpenApiMessageById(int64_t appid, int64_t msgseq, MessageStoreItem &msg){
	UC_LOG(INFO) << "get openapi message by msgseq. appid: "<< appid <<"  msgseq: " << msgseq;

	boost::unique_lock<boost::recursive_mutex> lock(_msglock);
	LatestMessageCache::iterator iter = _latest_openapi_msg_cache.find(appid);
	if (iter != _latest_openapi_msg_cache.end()){
		MessagesList::iterator iter_start = std::find_if(iter->second.messages_list.begin(),
											iter->second.messages_list.end(),
											MessageCompare(msgseq));
		if (iter_start != iter->second.messages_list.end()){
			msg.app_ack_sent = (*iter_start)->app_ack_sent;
			msg.oa_external_data = (*iter_start)->oa_external_data;
			msg.oa_last_status = (*iter_start)->oa_last_status;
			return true;
		}									
	}
	
	int32_t result = DBManager::GetInstance().GetMessage(OPENAPI_CHAT, appid, msgseq, msg);
	if (DBError::DB_SUCCESS == result){
		return true;
	}
	
	UC_LOG(WARNING) << "get openapi message error, where memory cache and db has not this message. ";
	return false;
}

bool MessageStore::UpdateOpenAPIOAMessageState(int64_t appid, int64_t msgseq, int32_t state, const std::string oaExternalData){

	boost::unique_lock<boost::recursive_mutex> lock(_msglock);
	LatestMessageCache::iterator iter = _latest_openapi_msg_cache.find(appid);
	if (iter != _latest_openapi_msg_cache.end()){
		
		MessagesList::iterator iter_start = std::find_if(iter->second.messages_list.begin(),
			iter->second.messages_list.end(),
			MessageCompare(msgseq));

		if (iter_start != iter->second.messages_list.end()){
			(*iter_start)->oa_last_status = state;
			(*iter_start)->oa_external_data = oaExternalData;
			return true;
		}
	}

	UC_LOG(INFO) << "when update oa message state, memory cache has not this message, msgseq:" << msgseq;
	return false;
}

bool MessageStore::UpdateOpenAPIMessageAckState(int64_t appid, int64_t msgseq, int32_t state){

	boost::unique_lock<boost::recursive_mutex> lock(_msglock);
	LatestMessageCache::iterator iter = _latest_openapi_msg_cache.find(appid);
	if (iter != _latest_openapi_msg_cache.end()){

		MessagesList::iterator iter_start = std::find_if(iter->second.messages_list.begin(),
			iter->second.messages_list.end(),
			MessageCompare(msgseq));

		if (iter_start != iter->second.messages_list.end()){
			(*iter_start)->app_ack_sent = state;
			return true;
		}
	}

	UC_LOG(INFO) << "when update message ack state, memory cache has not this message, msgseq:" << msgseq;
	return false;
}

void MessageStore::Clear()
{
	UC_LOG(INFO) << "clear all message data in memory";

	boost::unique_lock<boost::recursive_mutex> lock(_msglock);

	// clear IM memory cache
	for ( LatestMessageCache::iterator im_iter = _latest_im_msg_cache.begin();
		  im_iter != _latest_im_msg_cache.end();
		  ++im_iter )
	{
		MessagesListInfo& im_msg_list = im_iter->second;
		for ( MessagesList::iterator im_msg_iter = im_msg_list.messages_list.begin();
			  im_msg_iter != im_msg_list.messages_list.end();
			  ++im_msg_iter )
		{
			delete ( *im_msg_iter );
		}
	}

	// clear group memory cache
	for ( LatestMessageCache::iterator group_iter = _latest_group_msg_cache.begin();
		group_iter != _latest_group_msg_cache.end();
		++group_iter )
	{
		MessagesListInfo& group_msg_list = group_iter->second;
		for ( MessagesList::iterator group_msg_iter = group_msg_list.messages_list.begin();
			group_msg_iter != group_msg_list.messages_list.end();
			++group_msg_iter )
		{
			delete ( *group_msg_iter );
		}
	} 

	// clear openapi memory cache
	for (LatestMessageCache::iterator api_iter = _latest_openapi_msg_cache.begin();
		api_iter != _latest_openapi_msg_cache.end();
		++api_iter)
	{
		MessagesListInfo& api_msg_list = api_iter->second;
		for (MessagesList::iterator api_msg_iter = api_msg_list.messages_list.begin();
			api_msg_iter != api_msg_list.messages_list.end();
			++api_msg_iter)
		{
			delete (*api_msg_iter);
		}
	}

	_latest_im_msg_cache.clear();
	_latest_group_msg_cache.clear();
	_latest_openapi_msg_cache.clear();
}

bool MessageStore::_IsGroupType(int64_t conversation_type){
	return GROUP_CHAT == conversation_type || PROJECT_CHAT == conversation_type || CLOUD_CHAT == conversation_type || CONFERENCE_CHAT == conversation_type;
}

bool MessageStore::_IsSingleType(int64_t conversation_type){
	return SINGLE_CHAT == conversation_type;
}

bool MessageStore::_IsOpenapiType(int64_t conversation_type){
	return OPENAPI_CHAT == conversation_type;
}

}
