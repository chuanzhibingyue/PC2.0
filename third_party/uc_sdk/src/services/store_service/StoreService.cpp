#include "StoreService.h"

namespace uc {

StoreService::StoreService()
{

}

StoreService::~StoreService()
{

}

MessageStore* StoreService::GetMsgStore()
{
	return &_msg_store;
}

ConversationStore* StoreService::GetConvStore()
{
	return &_conv_store;
}

GroupListStore * StoreService::GetGroupListStore()
{
	return &_group_list_store;
}

MessageReadStore * StoreService::GetMessageReadStore(){
	return &_msg_read_store;
}

void StoreService::Clear()
{
	_msg_store.Clear();
	_conv_store.Clear();
	_msg_read_store.Clear();
//	_group_list_store.Clear();
}

}