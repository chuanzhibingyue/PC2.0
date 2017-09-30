#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include <queue>
#include <string>
#include <mutex>

#include "common/platform.h"

namespace uc {

class MessageQueue {
public:
	struct MessageInfo {
		int32_t msgId;
		std::string body;
	};

	MessageQueue();

	void EnqueueMessage(int32_t msgId, std::string body);

	std::string DequeueMessage(std::vector<int32_t>& dequeuedMsg);

private:
	std::queue<MessageInfo> _msgQueue;

	const int _maxDequeueSize;
};

}

#endif