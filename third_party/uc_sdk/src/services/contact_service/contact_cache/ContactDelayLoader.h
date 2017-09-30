#ifndef CONTACT_DELAY_LOADING_H_
#define CONTACT_DELAY_LOADING_H_

#include <map>
#include <list>
#include "base/Timer.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread.hpp"

namespace uc {

class ContactService;

enum CONTACT_INFO_RECEIVED_EVENT {
	DISPATCH_CONTACT_INFO_RECEIVED = 0,
	DISPATCH_CONTACT_UPDATED,
	DISPATCH_CONTACT_ADDED,
	DISPATCH_BASIC_CONTACT_INFO_RECEIVED,
};

class ContactDelayLoader {
public:
	ContactDelayLoader(ContactService *contactService);
	~ContactDelayLoader();

	void GetContact(int32_t userId, CONTACT_INFO_RECEIVED_EVENT dispatchEvent);

	void GetContact(const std::list<int32_t> &userIds, CONTACT_INFO_RECEIVED_EVENT dispatchEvent);
private:
	void GetContact();

private:
	ContactService *_contactService;

	struct DelayLoadingItem {
		int32_t userId;
		CONTACT_INFO_RECEIVED_EVENT dispatchEvent;
	};

	typedef std::map<CONTACT_INFO_RECEIVED_EVENT, std::list<int32_t> > WaitingMap;
	WaitingMap _waitingItems;

	boost::recursive_mutex _mtx;

	timer_id _id;
};

}
#endif
