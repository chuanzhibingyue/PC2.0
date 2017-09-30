#include "MessageQueue.h"

namespace uc {

const int fixedLength = 4;

MessageQueue::MessageQueue() : _maxDequeueSize(1024*1024){
}

void MessageQueue::EnqueueMessage(int32_t msgId, std::string body){
	MessageInfo info;
	info.msgId = msgId;
	info.body = body;

	_msgQueue.push(info);
}

std::string MessageQueue::DequeueMessage(std::vector<int32_t>& dequeuedMsg) {
	int curSize = 0;
	std::string body;
	while (!_msgQueue.empty() && (_msgQueue.front().body.size() + curSize + fixedLength < _maxDequeueSize)) {
		const MessageInfo& info =_msgQueue.front();
		int32_t len = info.body.size();
		body.append((const char*)&len, fixedLength);
		body.append(info.body);

		dequeuedMsg.push_back(info.msgId);
		_msgQueue.pop();
	}

	return body;
}

}