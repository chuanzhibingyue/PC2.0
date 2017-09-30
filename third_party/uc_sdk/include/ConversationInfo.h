/*
 * ConversationInfo.h
 *
 *  Created on: 2015年6月5日
 *      Author: jianxue
 */

#ifndef CONVERSATIONINFO_H_
#define CONVERSATIONINFO_H_

#include "UCSDK.h"
#include <string>
#include <vector>
#include "MessageInfo.h"

namespace uc {

interface UC_API ConversationInfo {
	ConversationInfo() {
		_type = NONE_CHAT;
		_state = STATE_SENDSUCCESS;
		_relateID = 0;
		_conversation = 0;
		_newMsgCount = 0;
		_lastMsgTime = 0;
		_lastMsgSeq = 0;
		_hidden = 0;
		_topIndex = -1;
		_eventId = 0;
		_conferenceId = 0;
		_authorizerId = 0;
		_oaLastStatus = -1;
		_appAckSent = 1;
		_oaExternalData = "";
	}

	virtual ~ConversationInfo() {
	}

	bool isHidden() const {
		return _hidden;
	}

	void SetHidden(bool hidden) {
		_hidden = hidden;
	}

	int64_t GetLastMsgSeq() const {
		return _lastMsgSeq;
	}

	void SetLastMsgSeq(int64_t lastMsgSeq) {
		_lastMsgSeq = lastMsgSeq;
	}

	int64_t GetLastMsgTime() const {
		return _lastMsgTime;
	}

	void SetLastMsgTime(int64_t lastMsgTime) {
		_lastMsgTime = lastMsgTime;
	}

	int32_t GetNewMsgCount() const {
		return _newMsgCount;
	}

	void SetNewMsgCount(int32_t newMsgCount) {
		_newMsgCount = newMsgCount;
	}

	int64_t GetRelateId() const {
		return _relateID;
	}

	void SetRelateId(int64_t relateId) {
		_relateID = relateId;
	}

	int32_t GetConversation() const {
		return _conversation;
	}

	void SetConversation(int32_t conversation) {
		_conversation = conversation;
	}

	ConversationType GetType() const {
		return _type;
	}

	void SetType(ConversationType type) {
		_type = type;
	}

	MsgSendState GetSendState() const {
		return _state;
	}

	void SetSendState(MsgSendState state) {
		_state = state;
	}

	int64_t GetTopIndex() const {
		return _topIndex;
	}

	void SetTopIndex(int64_t topIndex) {
		_topIndex = topIndex;
	}

	int64_t GetConferenceId() const {
		return _conferenceId;
	}

	int64_t GetEventId() const {
		return _eventId;
	}

	void SetEventId(int64_t eventId) {
		_eventId = eventId;
	}

	int64_t GetAuthorizerId() const {
		return _authorizerId;
	}

	void SetAuthorizerId(int64_t authorizerId) {
		_authorizerId = authorizerId;
	}

	void SetConferenceId(int64_t conferenceId) {
		_conferenceId = conferenceId;
	}

	int32_t GetOALastStatus() const {
		return _oaLastStatus;
	}

	void SetOALastStatus(int32_t oaLastStatus) {
		_oaLastStatus = oaLastStatus;
	}

	int32_t GetAppAckSent() const {
		return _appAckSent;
	}

	void SetAppAckSent(int32_t appAckSent) {
		_appAckSent = appAckSent;
	}

	const std::string& GetOAExternalData() const {
		return _oaExternalData;
	}

	void SetOAExternalData(const std::string& oaExternalData) {
		_oaExternalData = oaExternalData;
	}

private:
	ConversationType _type;
	MsgSendState _state;
	int64_t _relateID; //
	int32_t _conversation;
	int32_t _newMsgCount; // 新消息数量
	int64_t _lastMsgTime; // 最后发送或者接收的消息时间
	int64_t _lastMsgSeq; // 从ucas或者list返回消息的seq值
	int64_t _eventId; // 保存会议id
	int64_t _conferenceId; // 会议id
	int64_t _authorizerId; // 授权人id
	int64_t _topIndex;
	int32_t _oaLastStatus; //OA消息最新状态;
	int32_t _appAckSent;   //应用中心消息PuskAck结果; 
	std::string _oaExternalData; // OA消息扩展数据
	bool	_hidden;       //是否隐藏conversion;
};

}

#endif /* CONVERSATIONINFO_H_ */
