#include "MessageReadStore.h"
#include "UCErrorCode.h"
#include "LogManager.h"

namespace uc{

MessageReadStore::MessageReadStore(){

}

MessageReadStore::~MessageReadStore(){

}

int32_t MessageReadStore::SaveMessageReadInfo(const ConvKey& key, const MessageReadUserInfo& info){

	boost::unique_lock<boost::recursive_mutex> lock(_message_read_mutex);
	MessageReadUserList *list;
	if (!_GetMessageReadUserList(key, list)){
		MessageReadUserList infos;
		infos.push_back(info);
		_message_readuser_cache[key] = infos;
	}else{
		MessageReadUserList::iterator iter = std::find_if((*list).begin(), (*list).end(), [&](MessageReadUserInfo item)->bool{return item.GetMsgSeq() == info.GetMsgSeq(); });
		if (iter != (*list).end()){
			*iter = info;
		}else{
			(*list).push_back(info);
		}
	}

	return UC_SUCCEEDED;
}

int32_t MessageReadStore::GetMessageReadList(const ConvKey& key, int64_t seq, MessageReadInfo& info){
	
	boost::unique_lock<boost::recursive_mutex> lock(_message_read_mutex);
	return _GetMessageReadInfo(key, seq, info);
}

int32_t MessageReadStore::GetMessageReadList(const ConvKey& key, const std::vector<int64_t>& seqList, std::list<MessageReadInfo>& infoList){

	boost::unique_lock<boost::recursive_mutex> lock(_message_read_mutex);
	
	std::vector<int64_t>::const_iterator iter = seqList.begin();
	for (auto seq : seqList){
		MessageReadInfo info;
		if (UC_SUCCEEDED == _GetMessageReadInfo(key, seq, info)){
			infoList.push_back(info);
		}
	}
	return UC_SUCCEEDED;
}

int32_t MessageReadStore::GetMessageReadList(const ConvKey& key, std::list<int64_t>& seqList, std::list<MessageReadInfo>& infoList){

	boost::unique_lock<boost::recursive_mutex> lock(_message_read_mutex);
	std::list<int64_t>::iterator iter = seqList.begin();
	for (; iter != seqList.end(); ){
		MessageReadInfo info;
		if (UC_SUCCEEDED == _GetMessageReadInfo(key, *iter, info)){
			infoList.push_back(info);
			seqList.erase(iter++);
		}else{
			++iter;
		}
	}
	return UC_SUCCEEDED;
}

int32_t MessageReadStore::GetMessageReadList(const ConvKey& key, std::list<int64_t>& seqList, std::list<MessageReadUserInfo>& infoList){

	boost::unique_lock<boost::recursive_mutex> lock(_message_read_mutex);
	std::list<int64_t>::iterator iter = seqList.begin();
	for (; iter != seqList.end();){
		MessageReadUserInfo info;
		if (UC_SUCCEEDED == _GetMessageReadInfo(key, *iter, info)){
			if (info.GetUnreadCount() == info.GetUser().size()){
				infoList.push_back(info);
				seqList.erase(iter++);
			}
			else{
				++iter;
			}
		}
		else{
			++iter;
		}
	}
	return UC_SUCCEEDED;
}

int32_t MessageReadStore::UpdateMessageReadInfo(const ConvKey& key, int64_t seq, int32_t userId){
	boost::unique_lock<boost::recursive_mutex> lock(_message_read_mutex);
	MessageReadUserList *list;
	if (_GetMessageReadUserList(key, list)){
		for (auto &item : *list){
			if (item.GetMsgSeq() == seq){
				UserIdList list = item.GetUser();
				if (list.empty()){
					item.RefUnReadCount();
				}
				else{
					UserIdList::iterator iterItem = std::find_if(list.begin(), list.end(), [&](int32_t id)->bool{return (userId == id); });
					if (iterItem != list.end()){
						list.erase(iterItem);
						item.RefUnReadCount();
					}
				}
				if (item.GetUnreadCount() == MESSAGE_UNREAD_DEFAULT){
					item.SetAllRead(true);
					UC_LOG(ERROR) << "unread count error, seq: " << item.GetMsgSeq() << ", type:" << item.GetConversationType();
				}
				if (item.GetUnreadCount() == MESSAGE_UNREAD_NONE){
					item.SetAllRead(true);
				}
			}
		}
	}
	return UC_SUCCEEDED;
}

int32_t MessageReadStore::UpdateMessageReadInfo(const ConvKey& key, int64_t seq, std::list<MessageReadInfo>& infoList){
	boost::unique_lock<boost::recursive_mutex> lock(_message_read_mutex);
	MessageReadUserList *list;
	if (_GetMessageReadUserList(key, list))
	{
		for (auto &item : (*list)){
			if (item.GetMsgSeq() < seq && item.GetRelateId() == key.GetRelateId() &&
				item.GetUnreadCount() == MESSAGE_UNREAD_ONE){
				item.SetAllRead(true);
				item.RefUnReadCount();
				infoList.push_back(item);
			}
		}
	}
	
	return UC_SUCCEEDED;
}

void MessageReadStore::Clear(){
	boost::unique_lock<boost::recursive_mutex> lock(_message_read_mutex);
	_message_readuser_cache.clear();
}

int32_t MessageReadStore::_GetMessageReadInfo(const ConvKey& key, int64_t seq, MessageReadInfo& info){

	MessageReadUserList *list;
	if (!_GetMessageReadUserList(key, list)){
		UC_LOG(ERROR) << "why find the message read user info list error with conversation[" << key.GetType() << "] and relate id[" << key.GetRelateId() << "].";
		return UC_FAILED;
	}
	for (auto &item : (*list)){
		if (item.GetMsgSeq() == seq){
			info = item;
			return UC_SUCCEEDED;
		}
	}

	return UC_FAILED;
}

int32_t MessageReadStore::_GetMessageReadInfo(const ConvKey& key, int64_t seq, MessageReadUserInfo& info){

	MessageReadUserList *list;
	if (!_GetMessageReadUserList(key, list)){
		UC_LOG(ERROR) << "why find the message read user info list error with conversation[" << key.GetType() << "] and relate id[" << key.GetRelateId() << "].";
		return UC_FAILED;
	}
	for (auto &item : (*list)){
		if (item.GetMsgSeq() == seq){
			info = item;
			return UC_SUCCEEDED;
		}
	}

	return UC_FAILED;
}


bool MessageReadStore::_GetMessageReadUserList(const ConvKey& key, MessageReadUserList*& list){

	MessageReadUserMap::iterator iter = _message_readuser_cache.find(key);
	if (iter == _message_readuser_cache.end()){
		return false;
	}

	list = &iter->second;
	return true;
}
}