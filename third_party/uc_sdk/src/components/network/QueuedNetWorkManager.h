#ifndef QUEUED_NETWORKMANAGER_H_
#define QUEUED_NETWORKMANAGER_H_

#include "NetworkManager.h"
#include "MessageQueue.h"
#include "event.h"

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>

namespace uc {

struct MessageResponse{
	int32_t code;
	int32_t msgId;
	int64_t msgSeq;
	int64_t sendTime;
	ConversationType type;
	int64_t userId;
};

class QueuedNetWorkManagerListener {
public:
	virtual ~QueuedNetWorkManagerListener() {}

	virtual void OnSendMessageResponse(const std::vector<MessageResponse> &messageResponses) = 0;
};

class QueuedNetWorkManager : public NetworkManager, public Event<QueuedNetWorkManagerListener> {
public:
	QueuedNetWorkManager(uint8_t threads);

	void SendQueuedRequest(ConversationType type, int64_t userId, int32_t msgId, const std::string & body);

private:
	virtual void BeforeHandlerSleep();
	virtual void AfterHttpRequestDone();

	void SendRequest();
	void SendQueuedRequest(const std::string &body, std::vector<int32_t> &dequeuedMsg);
	int32_t OnSendMessageResponse(int32_t errorCode, HTTPResponse *response, std::vector<int32_t> dequeuedMsg);

	void GetResponse(const std::string &content, int32_t& result, std::vector<MessageResponse> &messageResponses);
	MessageResponse GetMessageResponse(boost::property_tree::wptree::value_type& v);
	void TriggerFailResponse(const std::vector<int32_t> &dequeuedMsg, int32_t res);
	void ClearMsgId(const std::vector<int32_t>& dequeuedMsg);

private:
	struct MessageId {
		ConversationType type;
		int64_t userId;
	};
	typedef std::map<int32_t, MessageId> MsgMap;

	MsgMap _msgIds;;
	MessageQueue _msgQueue;

	std::recursive_mutex _mtx;
};

}
#endif