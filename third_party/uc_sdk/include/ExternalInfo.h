/*
 * ExternalInfo.h
 *
 *  Created on: Jan 27, 2016
 *      Author: yongshuai.cheng
 */

#ifndef  EXTERNALINFO_H_
#define  EXTERNALINFO_H_

#include "UCSDK.h"
#include <string>

namespace uc{

using namespace std;

enum ExternalContentType {
	EXTERNAL_CONTENT_TYPE_NONE = 0,
	EXTERNAL_CONTENT_TYPE_TEXT,
	EXTERNAL_CONTENT_TYPE_FILE,
	EXTERNAL_CONTENT_TYPE_MEDIA,
	EXTERNAL_CONTENT_TYPE_CODE,
	EXTERNAL_CONTENT_TYPE_TIP
};

class UC_API ExternalUser{
public:
	ExternalUser(void) {
		_userId          = 0;
		_userAccount     = "";
		_displayName     = "";
		_localAvatar     = "";
		_isSlef          = false;
	}
	virtual ~ExternalUser(void) {}

	int32_t GetUserId() const {
		return _userId;
	}

	void SetUserId(int32_t userId) {
		_userId = userId;
	}

	const string GetUserAccount() const {
		return _userAccount;
	}

	void SetUserAccount(const string& userAccount) {
		_userAccount = userAccount;
	}

	const string& GetDisplayName() const {
		return _displayName;
	}

	void SetDisplayName(const string& displayName) {
		_displayName = displayName;
	}

	const string& GetLocalAvatar() const {
		return _localAvatar;
	}

	void SetLocalAvatar(const string& localAvatar) {
		_localAvatar = localAvatar;
	}	

	bool GetIsSlef() const{
		return _isSlef;
	}

	void SetIsSelf(bool isSelf){
		_isSlef = isSelf;
	}

private:
	int32_t   _userId;         // 讨论组ID或用户ID;
	string    _userAccount;    // 讨论组mountId或用户账号;
	string    _displayName;    // 用户昵称(讨论组不用);
	string    _localAvatar;    // 用户本地头像;
	bool      _isSlef;         // 是否是当前用户发送;
};

class UC_API ExternalMessage
{
public:
	ExternalMessage(void) {
		_msgSeq = 0;
		_msgTime = 0;
		_contentType = EXTERNAL_CONTENT_TYPE_NONE;
		_conversationType = NONE_CHAT;
	}

	virtual ~ExternalMessage(void) {

	}

	int64_t GetMsgSeq() const {
		return _msgSeq;
	}

	void SetMsgSeq(int64_t msgSeq) {
		_msgSeq = msgSeq;
	}

	int64_t GetMsgTime() const {
		return _msgTime;
	}

	void SetMsgTime(int64_t msgTime) {
		_msgTime = msgTime;
	}


	const ExternalUser& GetFrom() const {
		return _from;
	}

	void SetFrom(const ExternalUser& from) {
		_from = from;
	}

	const ExternalUser& GetTo() const {
		return _to;
	}

	void SetTo(const ExternalUser& to) {
		_to = to;
	}

	ConversationType GetConversationType() const {
		return _conversationType;
	}

	void SetConversationType(ConversationType conversationType) {
		_conversationType = conversationType;
	}

	int32_t GetContentType() const {
		return _contentType;
	}

	void SetContentType(int32_t contentType) {
		_contentType = contentType;
	}

private:
	int64_t			_msgSeq;
	int64_t         _msgTime;
	ExternalUser    _from;
	ExternalUser    _to;
	ConversationType _conversationType;

protected:
	int32_t			_contentType;

};


class UC_API TextExternalMessage : public ExternalMessage {
public:
	TextExternalMessage() {
		_textContent = "";
		_contentType = EXTERNAL_CONTENT_TYPE_NONE;
	}

	virtual ~TextExternalMessage() {
	}

	const string& GetTextContent() const {
		return _textContent;
	}

	void SetTextContent(const string& textContent) {
		_textContent = textContent;
	}

private:
	string _textContent;
};




}        /* namespace uc */
#endif   /* EXTERNALINFO_H_*/
