#ifndef __STORESERVICE_H__
#define __STORESERVICE_H__

#include "MessageStore.h"
#include "ConversationStore.h"
#include "GroupListStore.h"
#include "MessageReadStore.h"

namespace uc {

class StoreService
{
public:
	/**
     * Constructor.
     */
	StoreService();

	/**
     * Destructor.
     */
	~StoreService();

	/**
	* Get message store
	* @return                  return message store pointer
	*/
	MessageStore*	GetMsgStore();

	/**
	* Get conversation store
	* @return				   return conversation store pointer
	*/
	ConversationStore* GetConvStore();

	/**
	* Get group list store
	* @return					return  group list store pointer
	*/
	GroupListStore * GetGroupListStore();

	/**
	* Get message read store
	* @return					return  group list store pointer
	*/
	MessageReadStore * GetMessageReadStore();

	/**
	* Clear all cache in memory
	*/
	void Clear();

private:
	MessageStore		_msg_store;
	ConversationStore	_conv_store;
	GroupListStore		_group_list_store;
	MessageReadStore    _msg_read_store;

};

extern StoreService theStore;

}

#endif