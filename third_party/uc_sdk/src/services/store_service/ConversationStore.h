#ifndef __CONVERSATIONSTORE_H__
#define __CONVERSATIONSTORE_H__

#include "client_service/ConversationInfoInternal.h"
#include <boost/thread.hpp>
#include "Timer.h"

namespace uc {

typedef std::list< ConversationInfo* > ConvList;
struct ConvListInfo
{
	ConvListInfo() : min_timestamp(0), max_timestamp(0) {}

	int64_t			min_timestamp;
	int64_t			max_timestamp;
	ConvList	    conv_list;
};

class ConvCompare
{
public:
	ConvCompare(int64_t relate_id, ConversationType type):_relate_id(relate_id), _type(type){}
	bool operator()(const ConversationInfo* conv_info)
	{
		assert ( conv_info );
		if ( conv_info->GetRelateId() == _relate_id && conv_info->GetType() == _type )
		{
			return true;
		}
		
		return false;
	}
private:
	int64_t				_relate_id;
	ConversationType	_type;
};

class ConvCompareWithTimestamp
{
public:
	ConvCompareWithTimestamp(int64_t timestamp):_timestamp(timestamp){}
	bool operator()(const ConversationInfo* conv_info)
	{
		assert ( conv_info );
		return ( _timestamp == conv_info->GetLastMsgSeq() );
	}
private:
	int64_t _timestamp;
};

class ConversationStore
{
public:
	/**
     * Constructor.
     */
	ConversationStore();

	/**
     * Destructor.
     */
	~ConversationStore();

	/**
	* Get local conversations from db
	* @param count              the count user want to get
	* @param conversations	    result of conversation list
	* @return                   true or false
	*/
	bool GetLocalConversations(int32_t count, ConvList &conversations);

	/**
	* Get conversations from server
	* @param userid             login user id
	* @param count				the count user want to get
	* @param startTime          starttime of chat
	* @param endTime            endtime of chat
	* @param conversations		result of conversation list
	* @return                   true or false
	*/
	bool GetConversationList(int32_t userid, int32_t count, int64_t starttime, 
		                     int64_t endtime, ConvList &conversations);

	/**
	* sync conversations from server to cache
	* @param conversations	    conversation list from server
	* @return                   true or false
	*/
	bool SyncConversations(ConvList &conversations);

	/**
	* save conversation in cache
	* @param conversation	    one conversation data
	* @return                   true or false
	*/
	bool SaveConversation(ConversationInfoInternal* conversation);

	/**
	* save conversation in cache
	* @param conversation	    one conversation data
	* @return                   true or false
	*/
	bool SaveConversation(const ConversationInfoInternal& conversation);
	
	/**
	* get conversation info by msgseq;
	* @param msgSeq      	    sever key of one message
	* @param conversation	    one conversation data
	* @return                   true or false
	*/
	bool GetConversationMessage(int64_t msgSeq, OUT ConversationInfo &conversation);

	/**
	* get conversation info by relateId  and type;
	* @param type      	        conversation type
	* @param relateId      	    
	* @param conversation	    one conversation data
	* @return                   true or false
	*/
	bool GetConversationInfo(ConversationType type, int64_t relateId, OUT ConversationInfo & info);

	/**
	* update conversation top index;
	* @param type      	        conversation type
	* @param relateId	            
	* @param state
	* @param topIndex
	* @return                   true or false
	*/
	bool UpdataConversationTopIndex(ConversationType type, int64_t relateId, int32_t state, int64_t topIndex);
	
	/**
	* update openapi oa message status;
	* @param msgSeq             server key of one message
	* @param state              message status
	* @return                   true or false
	*/
	bool UpdataConversationOAMessageState(int64_t msgSeq, int32_t state, const std::string oaExternalData);

	/**
	* update openapi message ack status;
	* @param msgSeq             server key of one message
	* @param state              message status
	* @return                   true or false
	*/
	bool UpdataConversationMessageAckState(int64_t msgSeq, int32_t state);

	/**
	* Clear conversation cache in memory
	*/
	void Clear();

protected:
	bool _ClearDBConvCache();
	bool _FlashConversationsInDB();
	void _executionAction(int mode);

private:
	ConvListInfo						_conv_info_cache;
	ConvList							_conv_list_db_cache;
	boost::recursive_mutex				_convlock;
	uc::timer_id						_timerId;
};

}

#endif