#include "ContactDelayLoader.h"
#include "contact_service/ContactService.h"
#include <iterator>
#include "LogManager.h"

namespace uc {

const int DELAY_LOADING_TIME_MILLI = 50;

ContactDelayLoader::ContactDelayLoader(ContactService *contactService) : _contactService(contactService), _id(-1){
}

ContactDelayLoader::~ContactDelayLoader() {
}

void ContactDelayLoader::GetContact(int32_t userId, CONTACT_INFO_RECEIVED_EVENT dispatchEvent) {
	UC_LOG(INFO) << "lazy load contact, userid: " << userId;
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);

	WaitingMap::iterator iter = _waitingItems.find(dispatchEvent);
	if (iter == _waitingItems.end()) {
		_waitingItems[dispatchEvent] = std::list<int32_t>();
	}
	_waitingItems[dispatchEvent].push_back(userId);

	lock.unlock();

	if (_id > 0) {
		Timer::GetInstance().Cancel(_id);
	}
	Timer::GetInstance().Schedule(MakeCallback(this, &ContactDelayLoader::GetContact), DELAY_LOADING_TIME_MILLI, &_id);
}

void ContactDelayLoader::GetContact(const std::list<int32_t> &userIds, CONTACT_INFO_RECEIVED_EVENT dispatchEvent) {
	UC_LOG(INFO) << "lazy load contact list";
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);

	WaitingMap::iterator iter = _waitingItems.find(dispatchEvent);
	if (iter == _waitingItems.end()) {
		_waitingItems[dispatchEvent] = userIds;
	}
	else {
		std::copy(userIds.begin(), userIds.end(), std::back_insert_iterator<std::list<int32_t> >(_waitingItems[dispatchEvent]));
	}

	lock.unlock();

	if (_id > 0) {
		Timer::GetInstance().Cancel(_id);
	}
	Timer::GetInstance().Schedule(MakeCallback(this, &ContactDelayLoader::GetContact), DELAY_LOADING_TIME_MILLI, &_id);
}

void ContactDelayLoader::GetContact() {
	UC_LOG(INFO) << "the lazy timer is timeout";
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);

	for (WaitingMap::iterator iter = _waitingItems.begin(); iter != _waitingItems.end(); ++iter) {
		_contactService->GetContactsFromServer(_waitingItems[iter->first], iter->first);
		_waitingItems[iter->first].clear();
	}

	_id = -1;
}

}