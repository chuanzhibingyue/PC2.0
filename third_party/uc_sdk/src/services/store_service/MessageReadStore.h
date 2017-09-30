#ifndef __MESSAGEREADSTORE_H__
#define __MESSAGEREADSTORE_H__

#include "UCSDK.h"
#include "MessageReadInfo.h"
#include <boost/thread.hpp>

namespace uc {


/**
* class ConvKey include conversation type and relate id.
*  because the message will appear the same seq and user id may be 
*  equal group id.
*/
class ConvKey{

public:
	ConvKey():_type(NONE_CHAT), _relateId(0){}
	ConvKey(ConversationType type, int64_t relateId) :_type(type), _relateId(relateId){}

	inline bool operator < (const ConvKey& key) const{

		return std::tie(this->_type , this->_relateId) < 
			std::tie(key._type, key._relateId);
	}

	ConversationType GetType()const {
		return _type;
	}

	int64_t GetRelateId()const {
		return _relateId;
	}

private:

	ConversationType _type;
	int64_t _relateId;
};

typedef std::list<MessageReadUserInfo>         MessageReadUserList;
typedef std::map<ConvKey, MessageReadUserList> MessageReadUserMap;

class MessageReadStore{

public:
	MessageReadStore();
	~MessageReadStore();
public:

	/**
	* save message read user
	*/
	int32_t SaveMessageReadInfo(const ConvKey& key, const MessageReadUserInfo& info);

	/**
	* get message read list
	*/
	int32_t GetMessageReadList(const ConvKey& key, int64_t seq, MessageReadInfo& info);

	/**
	* get message read list
	*/
	int32_t GetMessageReadList(const ConvKey& key, const std::vector<int64_t>& seqList, std::list<MessageReadInfo>& infoList);
	
	/**
	* get message read list
	*/
	int32_t GetMessageReadList(const ConvKey& key, std::list<int64_t>& seqList, std::list<MessageReadInfo>& infoList);
	
	/**
	* get message read user list
	*/
	int32_t GetMessageReadList(const ConvKey& key, std::list<int64_t>& seqList, std::list<MessageReadUserInfo>& infoList);

	/**
	* update message read info
	*/
	int32_t UpdateMessageReadInfo(const ConvKey& key, int64_t seq, int32_t userId);

	/**
	* update message read info
	*/
	int32_t UpdateMessageReadInfo(const ConvKey& key, int64_t seq, UserIdList list);

	/**
	* update message read info
	*/
	int32_t UpdateMessageReadInfo(const ConvKey& key, int64_t seq, std::list<MessageReadInfo>& infoList);
	
	/**
	* Clear message read cache in memory
	*/
	void Clear();

private:
	
	int32_t _GetMessageReadInfo(const ConvKey& key, int64_t seq, MessageReadInfo& info);
	int32_t _GetMessageReadInfo(const ConvKey& key, int64_t seq, MessageReadUserInfo& info);
	bool  _GetMessageReadUserList(const ConvKey& key, MessageReadUserList*& list);

private:
	boost::recursive_mutex		_message_read_mutex;
	MessageReadUserMap          _message_readuser_cache;

};




}

#endif
