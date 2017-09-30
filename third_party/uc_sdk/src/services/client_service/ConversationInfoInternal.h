/*
 * ConversationInfoInternal.h
 *
 *  Created on: 2015年8月13日
 *      Author: xuejian
 */

#ifndef CONVERSATIONINFOINTERNAL_H_
#define CONVERSATIONINFOINTERNAL_H_

#include "ConversationInfo.h"
#include "UCMessage.h"

namespace uc {

class ConversationInfoInternal : public ConversationInfo {
public:
	ConversationInfoInternal() {
		_msg = NULL;
		_body = "";
		_del_msg = true;
	}
	ConversationInfoInternal(bool del_message) {
		_msg = NULL;
		_body = "";
		_del_msg = del_message;
	}
	virtual ~ConversationInfoInternal() {
		if ( _del_msg ) {
			delete _msg;
			_msg = NULL;
		}
	}

	const std::string& GetBody() const {
		return _body;
	}

	void SetBody(const std::string& body) {
		_body = body;
	}

	const UCMsg* GetMsg() const{
		return _msg;
	}

	void SetMsg(UCMsg* msg) {
		_msg = msg;
	}

private:
	UCMsg *_msg;
	std::string _body;
	bool _del_msg;
};

};

#endif /* CONVERSATIONINFOINTERNAL_H_ */
