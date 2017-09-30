/*
 * MessageInfo.cpp
 *
 *  Created on: 2015年7月17日
 *      Author: xuejian
 */

#include "MessageInfo.h"
#include "MessageProcessor.h"

namespace uc {

void MessageInfo::InfoFromUCHead(const UCHead &head) {
	_msgId = head.GetId();
	_msgSeq = head.GetSeq();
	_msgTime = head.GetTimestamp();
	_version = head.GetVersion();
	_appId = head.GetAppId();
	_protocolType = head.GetProtocoltype();
	_protocolId = head.GetProtocolId();
	_from = head.GetFrom();
	_to = head.GetTo();
	_conversation = head.GetConversation();
	_conversationType = MessageProcessor::GetConversationType(_appId, _protocolType);
	_toPrivate = head.GetPrivate();
	if (!head.GetGroupAtUsers().empty()) {
		_groupAtUsers = head.GetGroupAtUsers();
	}
	
}

}


