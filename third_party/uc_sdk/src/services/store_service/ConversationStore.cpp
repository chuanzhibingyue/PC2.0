#include "ConversationStore.h"
#include "DBManager.h"
#include "LogManager.h"
#include "message_service/MessageProcessor.h"
#include "CommonDef.h"
#include "common/CallOnMainThread.h"

namespace uc {

bool compare_timestamp(const ConversationInfo* lhs, const ConversationInfo* rhs)
{
	assert( lhs );
	assert( rhs );

	return ( lhs->GetLastMsgSeq() > rhs->GetLastMsgSeq() );
}

ConversationStore::ConversationStore() : _timerId(0)
{
}

ConversationStore::~ConversationStore()
{
}

bool ConversationStore::GetLocalConversations(int32_t count, ConvList &conversations)
{
	UC_LOG(INFO) << "[conv_cache] get local conversations. count: " << count;
	boost::unique_lock<boost::recursive_mutex> lock(_convlock);

	_ClearDBConvCache();
	int res = DBManager::GetInstance().LoadLatestConversations(count, conversations);
	if ( res != DBError::DB_SUCCESS )
	{
		UC_LOG(ERROR) << "[conv_cache] get local conversations from DB failed. error code: " << res;
		return false;
	}
	
	_conv_list_db_cache.assign( conversations.begin(), conversations.end() );
	return true;
	
}

bool ConversationStore::GetConversationList(int32_t userid, int32_t count, int64_t starttime, 
	                                        int64_t endtime, ConvList &conversations)
{
	UC_LOG(INFO) << "[conv_cache] get conversation list data. userid: " << userid << " count: " << count
		<< " starttime: " << starttime << " endtime: " << endtime;

	boost::unique_lock<boost::recursive_mutex> lock(_convlock);
	if ( _conv_info_cache.conv_list.empty() )
	{
		return false;
	}

	UC_LOG(INFO) << "[conv_cache] get conversation list data from memory cacche. userid: " << userid
		<< " count: " << count << " starttime: " << starttime << " endtime: " << endtime 
		<< " conv_list size: " << _conv_info_cache.conv_list.size();

	_conv_info_cache.conv_list.sort( compare_timestamp );
	int64_t _internal_count = 0;
	if ( starttime == 0 && endtime == 0 )
	{
		// always get conversation list from server
		return false;
		/**
		ConvList::iterator iter = _conv_info_cache.conv_list.begin();
		for ( ; iter != _conv_info_cache.conv_list.end(); iter++ )
		{
			conversations.push_back(*iter);
			_internal_count++;

			if (_internal_count == count)
				break;
		}
		*/
	}
	else if ( starttime == 0 )
	{
		if ( endtime > _conv_info_cache.max_timestamp )
		{
			UC_LOG(INFO) << "[conv_cache] starttime == 0 but endtime is large than cache max_timestamp, so it needs get from server. endtime: "
				<< endtime
				<< " cache_max_timestamp: "
				<< _conv_info_cache.max_timestamp;
			return false;
		}

		ConvList::iterator iter = std::find_if( _conv_info_cache.conv_list.begin(), 
												_conv_info_cache.conv_list.end(), 
												ConvCompareWithTimestamp(endtime) );

		if ( iter == _conv_info_cache.conv_list.end() )
		{
			UC_LOG(INFO) << "[conv_cache] can not find endtime timestamp in cache, maybe lost message is cache, maybe wrong timestamp from upper layer. anyway, get data from server again";
			return false;
		}

		if ( ++iter == _conv_info_cache.conv_list.end() )
		{
			UC_LOG(INFO) << "[conv_cache] the prev conversation can not find, get data from server again";
			return false;
		}

		if ( std::distance( iter,  _conv_info_cache.conv_list.end() ) < count )
		{
			UC_LOG(INFO) << "[conv_cache] cache data number is less than count, need to update from server";
			return false;
		}

		for ( ; iter !=  _conv_info_cache.conv_list.end(); iter++ )
		{
			conversations.push_back(*iter);
			_internal_count++;

			if (_internal_count == count)
				break;
		}	
	}
	else if ( endtime == 0 )
	{
		if ( starttime < _conv_info_cache.min_timestamp )
		{
			UC_LOG(INFO) << "[conv_cache] endtime == 0 but starttime is less than cache min_timestamp, so it needs get from server. starttime: "
				<< starttime
				<< " cache_min_timestamp: "
				<< _conv_info_cache.min_timestamp;
			return false;
		}

		ConvList::iterator iter = std::find_if( _conv_info_cache.conv_list.begin(),
												_conv_info_cache.conv_list.end(),
												ConvCompareWithTimestamp(starttime) );

		if ( iter == _conv_info_cache.conv_list.end() )
		{
			UC_LOG(INFO) << "[conv_cache] can not find start timestamp in cache, maybe lost message is cache, maybe wrong timestamp from upper layer. anyway, get data from server again";
			return false;
		}

		ConvList::iterator iter_tmp = iter;
		_internal_count = std::distance( _conv_info_cache.conv_list.begin(), ++iter_tmp );
		if ( _internal_count > count )
		{
			_internal_count = count;
		}

		while ( _internal_count > 0 )
		{
			conversations.push_front(*iter);
			_internal_count--;
			if ( _internal_count == 0 )
				break;

			iter--;
		}
	}
	else
	{
		UC_LOG(WARNING) << "[conv_cache] there has no case: starttime and endtime are not null so far.";
	}
	
	return true;
}

bool ConversationStore::SyncConversations(ConvList &conversations)
{
	UC_LOG(INFO) << "[conv_cache] sync conversations.";
	boost::unique_lock<boost::recursive_mutex> lock(_convlock);

	if ( conversations.empty() )
	{
		return true;
	}

	ConvList::iterator iter = conversations.begin();
	for ( ; iter != conversations.end(); ++iter )
	{
		ConversationInfoInternal* conv_internal = (ConversationInfoInternal*)(*iter);
		assert( conv_internal );
		SaveConversation( conv_internal );

		if (_conv_info_cache.max_timestamp == 0 && _conv_info_cache.min_timestamp == 0 )
		{
			_conv_info_cache.max_timestamp = conv_internal->GetLastMsgSeq();
			_conv_info_cache.min_timestamp = conv_internal->GetLastMsgSeq();
		}
		else
		{
			if ( conv_internal->GetLastMsgSeq() > _conv_info_cache.max_timestamp )
			{
				_conv_info_cache.max_timestamp = conv_internal->GetLastMsgSeq();
			}

			if ( conv_internal->GetLastMsgSeq() < _conv_info_cache.min_timestamp )
			{
				_conv_info_cache.min_timestamp = conv_internal->GetLastMsgSeq();
			}
		}	
	}

	return true;
}

bool ConversationStore::SaveConversation(ConversationInfoInternal* conversation)
{
	UC_LOG(INFO) << "[conv_cache] save conversation in cache. conversation: " << conversation;
	if ( NULL == conversation )
	{
		return false;
	}

	return SaveConversation( *conversation );
}

bool ConversationStore::SaveConversation(const ConversationInfoInternal& conversation)
{
	UC_LOG(INFO) << "[conv_cache] save conversation in cache.";
	{
		boost::unique_lock<boost::recursive_mutex> lock(_convlock);

		UCMsg* uc_msg = (UCMsg*)conversation.GetMsg();
		if ( NULL == uc_msg )
		{
			UC_LOG(ERROR) << "[conv_cache] get message is null";
			return false;
		}

		UCMsg * conv_msg = NULL;
		UCHead head;
		uc_msg->GetHead( head );
		MessageProcessor::GetUCMsg( head, conversation.GetBody(), conv_msg );

		if ( NULL == conv_msg )
		{
			UC_LOG(ERROR) << "[conv_cache] MessageProcessor::GetUCMsg failed, _msg is null";
			return false;
		}

		ConversationInfoInternal* conv_info = new ConversationInfoInternal();
		conv_info->SetBody( conversation.GetBody().c_str() );
		conv_info->SetLastMsgSeq( head.GetSeq() );
		conv_info->SetConversation( head.GetConversation() );
		conv_info->SetLastMsgTime( head.GetTimestamp() );
		conv_info->SetMsg( conv_msg );
		conv_info->SetRelateId( conversation.GetRelateId() );
		conv_info->SetSendState( conversation.GetSendState() );
		conv_info->SetType( conversation.GetType() );
		conv_info->SetEventId( conversation.GetEventId());
		conv_info->SetConferenceId(conversation.GetConferenceId());
		conv_info->SetAuthorizerId(conversation.GetAuthorizerId());
		conv_info->SetOALastStatus(conversation.GetOALastStatus());
		conv_info->SetAppAckSent(conversation.GetAppAckSent());
		conv_info->SetOAExternalData(conversation.GetOAExternalData());
		conv_info->SetTopIndex(conversation.GetTopIndex());


		if ( conv_info->GetLastMsgSeq() == 0 )
		{
			conv_info->SetLastMsgSeq( _conv_info_cache.max_timestamp + 1 );
		}

		ConvList::iterator iter = std::find_if( _conv_info_cache.conv_list.begin(),
												_conv_info_cache.conv_list.end(),
												ConvCompare(conv_info->GetRelateId(), conv_info->GetType()) );
		if ( iter == _conv_info_cache.conv_list.end() )
		{
			_conv_info_cache.conv_list.push_front( conv_info );
		}
		else
		{
			ConversationInfoInternal* del_conv = (ConversationInfoInternal*)(*iter);
			conv_info->SetTopIndex(del_conv->GetTopIndex());
			delete del_conv;
			_conv_info_cache.conv_list.erase( iter );
			_conv_info_cache.conv_list.push_front( conv_info );
		}
	}

	if (_timerId == 0)
	{
		Timer::GetInstance().Schedule(MakeCallback(this, &ConversationStore::_executionAction, WORKER_THREAD_MODE), 30 * 1000, &_timerId);
	}

	return true;
}

void ConversationStore::Clear()
{
	UC_LOG(INFO) << "clear all conversations data in memory and flash memory data in DB";

	boost::unique_lock<boost::recursive_mutex> lock(_convlock);

	if ( _conv_info_cache.conv_list.empty() )
		return;

	_ClearDBConvCache();
	_FlashConversationsInDB();

	ConvList::iterator iter = _conv_info_cache.conv_list.begin();
	for ( ; iter != _conv_info_cache.conv_list.end(); ++iter )
	{
		ConversationInfoInternal* conv_internal = (ConversationInfoInternal*)(*iter);
		delete conv_internal;
	}

	_conv_info_cache.conv_list.clear();
}

bool ConversationStore::_ClearDBConvCache()
{
	ConvList::iterator iter = _conv_list_db_cache.begin();
	for ( ; iter != _conv_list_db_cache.end(); ++iter )
	{
		ConversationInfoInternal* conv_internal = (ConversationInfoInternal*)(*iter);
		delete conv_internal;
	}

	_conv_list_db_cache.clear();
	return true;
}

bool ConversationStore::_FlashConversationsInDB()
{

	if ( _conv_info_cache.conv_list.empty() )
	{
		return false;
	}
		
	//DBManager::GetInstance().RemoveAllConversations();
	DBManager::GetInstance().SaveConversations( _conv_info_cache.conv_list );

	return true;
}

void ConversationStore::_executionAction(int mode)
{
	switch (mode)
	{
	case WORKER_THREAD_MODE:
		CallOnMainThread(this, &ConversationStore::_executionAction, (int)MAIN_THREAD_MODE);
		break;
	case MAIN_THREAD_MODE:{
		boost::unique_lock<boost::recursive_mutex> lock(_convlock);
		_timerId = 0;
		_FlashConversationsInDB();
	}
		break;
	default:
		break;
	}
}

bool ConversationStore::GetConversationMessage(int64_t msgSeq, ConversationInfo &conversation){
	boost::unique_lock<boost::recursive_mutex> lock(_convlock);

	ConvList::iterator iter = std::find_if(_conv_info_cache.conv_list.begin(), 
											_conv_info_cache.conv_list.end(),
											[&](ConversationInfo *info){return info->GetLastMsgSeq() == msgSeq;});
	if (iter != _conv_info_cache.conv_list.end()){
		conversation.SetAppAckSent((*iter)->GetAppAckSent());
		conversation.SetOALastStatus((*iter)->GetOALastStatus());
		conversation.SetOAExternalData((*iter)->GetOAExternalData());
		return true;
	}
	return false;
}


bool ConversationStore::GetConversationInfo(ConversationType type, int64_t relateId, OUT ConversationInfo & info){
	boost::unique_lock<boost::recursive_mutex> lock(_convlock);

	ConvList::iterator iter = std::find_if(_conv_info_cache.conv_list.begin(),
		_conv_info_cache.conv_list.end(),
		[&](ConversationInfo *info){return ((info->GetRelateId() == relateId) && info->GetType() == type); });
	if (iter != _conv_info_cache.conv_list.end()){
		info = *(*iter);
		return true;
	}
	return false;
}

bool ConversationStore::UpdataConversationTopIndex(ConversationType type, int64_t relateId, int32_t state, int64_t topIndex){
	boost::unique_lock<boost::recursive_mutex> lock(_convlock);

	ConvList::iterator iter = std::find_if(_conv_info_cache.conv_list.begin(),
		_conv_info_cache.conv_list.end(),
		[&](ConversationInfo *info){return ((info->GetRelateId() == relateId) && info->GetType() == type); });
	if (iter != _conv_info_cache.conv_list.end()){
		if (1 == state){
			(*iter)->SetTopIndex(topIndex);
		} else if (0 == state){
			(*iter)->SetTopIndex(-1);
		}
		
		return true;
	}
	return false;
}

bool ConversationStore::UpdataConversationOAMessageState(int64_t msgSeq, int32_t state, const std::string oaExternalData){
	boost::unique_lock<boost::recursive_mutex> lock(_convlock);

	ConvList::iterator iter = std::find_if(_conv_info_cache.conv_list.begin(),
		_conv_info_cache.conv_list.end(),
		[&](ConversationInfo *info){return info->GetLastMsgSeq() == msgSeq; });
	if (iter != _conv_info_cache.conv_list.end()){
		(*iter)->SetOALastStatus(state);
		(*iter)->SetOAExternalData(oaExternalData);
		return true;
	}
	return false;
}


bool ConversationStore::UpdataConversationMessageAckState(int64_t msgSeq, int32_t state){

	boost::unique_lock<boost::recursive_mutex> lock(_convlock);

	ConvList::iterator iter = std::find_if(_conv_info_cache.conv_list.begin(),
		_conv_info_cache.conv_list.end(),
		[&](ConversationInfo *info){return info->GetLastMsgSeq() == msgSeq; });
	if (iter != _conv_info_cache.conv_list.end()){
       (*iter)->SetAppAckSent(state);
		return true;
	}
	return false;
}


}
