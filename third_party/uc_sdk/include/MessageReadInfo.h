/*
 * MessageReadInfo.h
 *
 *  Created on: June 02, 2015
 *      Author: yongshuai cheng
 */

#ifndef  MESSAGEREADINFO_H_
#define  MESSAGEREADINFO_H_

#include <string>
#include <list>
#include "UCSDK.h"

namespace uc{

#define MESSAGE_UNREAD_DEFAULT   -1
#define MESSAGE_UNREAD_NONE      0
#define MESSAGE_UNREAD_ONE       1

typedef std::list<int32_t>  UserIdList;


class UC_API MessageReadInfo
{
public:
	MessageReadInfo() : _type(NONE_CHAT), _seq(0), _count(-1), _allRead(false){}
	~MessageReadInfo(){}

	void SetConversationType(ConversationType type){ _type = type; }
	ConversationType GetConversationType() const { return _type; }

	void SetRelateId(int64_t relateId){ _relateId = relateId; }
	int64_t GetRelateId() const { return _relateId; };

	void SetMsgSeq(int64_t seq){ _seq = seq; }
	int64_t GetMsgSeq() const { return _seq; };


	void SetUnreadCount(int32_t count){ _count = count; }
	int32_t GetUnreadCount() const{ return _count; }
	void RefUnReadCount(int32_t count = 1){ _count -= count; }

	void SetAllRead(bool allRead){ _allRead = allRead; }
	bool GetAllRead() const{ return _allRead; }


private:
	ConversationType  _type;          // 聊天类型;
	int64_t           _relateId;      // 会话ID;
	int64_t			  _seq;           // 服务器消息ID;
	int32_t           _count;         // 未读人数;
	bool              _allRead;       // 是否全部已读 true:全部已读/false:未全部已读;
};


class UC_API MessageReadUserInfo : public MessageReadInfo{
public:
	MessageReadUserInfo(){}
	~MessageReadUserInfo(){}



	void AddUser(const int32_t& userId){ _userList.push_back(userId); }
	void AddUser(const UserIdList& userList){ _userList = userList; }
	const UserIdList & GetUser() const { return _userList; }
	void RemoveUser(int32_t userId) {
		UserIdList::iterator it = _userList.begin();
		for (; it != _userList.end();){
			if (userId == (*it)){
				it = _userList.erase(it);
				break;
			}
			else {
				++it;
			}
		}
	}
	void ClearUser(){ _userList.clear(); }
 
private:
	UserIdList        _userList;      //未读用户列表;

};

} // namespace uc


#endif // MESSAGEREADINFO_H_
