/*
 * MessageInfo.h
 *
 *  Created on: May 20, 2015
 *      Author: yongshuai cheng
 */

#ifndef  SEARCHINFO_H_
#define  SEARCHINFO_H_

#include "UCSDK.h"
#include "UCMessage.h"
#include <string>
#include "MessageInfo.h"

namespace uc{


struct SearchMessageInfo
{
public:
	SearchMessageInfo(void) {
		_conversationType = NONE_CHAT;
		_relateId = 0;
		_timeStamp = 0;
		_msgList.clear();
		_matchCounts = 0;
		_eventId = 0;
	}

	virtual ~SearchMessageInfo(void) {
		for (std::list<MessageInfo*>::iterator itor = _msgList.begin(); itor != _msgList.end(); ++itor){
			delete *itor;
			*itor = NULL;
		}
	}

	ConversationType GetConversationType() const {
		return _conversationType;
	}

	void SetConversationType(ConversationType conversationType) {
		_conversationType = conversationType;
	}

	int64_t GetRelateId() const {
		return _relateId;
	}

	void SetRelateId(int64_t relateId) {
		_relateId = relateId;
	}


	int64_t GetTimeStamp() const {
		return _timeStamp;
	}

	void SetTimeStamp(int64_t timeStamp) {
		_timeStamp = timeStamp;
	}

	std::list<MessageInfo*> GetMessageList()const {
		return _msgList;
	}

	void SetMessageList(std::list<MessageInfo*> & msgList){
		_msgList = msgList;
	}
	void AddMessageInfo(MessageInfo * info){
		_msgList.push_back(info);
	}

	void SetMatchCounts(int64_t matchcounts){
		_matchCounts = matchcounts;
	}

	int64_t GetMatchCounts() const{
		return _matchCounts;
	}

	void SetEventId(int32_t eventid){
		_eventId = eventid;
	}

	int32_t GetEventId() const{
		return _eventId;
	}

private:
	int64_t			_relateId;
	ConversationType _conversationType;
	int64_t         _timeStamp;  //最大消息时间，用于排序;
	std::list<MessageInfo*> _msgList; 
	int64_t         _matchCounts;
	int32_t         _eventId;
};
}        /* namespace uc */
#endif   /* SEARCHINFO_H_*/
