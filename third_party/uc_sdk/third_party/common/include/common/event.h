/*
 * event.h
 *
 *  Created on: 2013-7-5
 *      Author: ethan
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <set>

#define triggerEvent(context, expression) \
	for (set<context *>::iterator listener = _listeners.begin(); listener != _listeners.end(); ++listener) {\
		((context *)(*listener))->expression;\
	}

#define triggerEvent_condition(context, condition, expression) \
	for (set<context *>::iterator listener = _listeners.begin(); condition && listener != _listeners.end(); ++listener) {\
		((context *)(*listener))->expression;\
	}

#define triggerEventEx(context, listeners, expression) \
	for (set<context *>::iterator listener = (listeners).getListeners()->begin(); listener != (listeners).getListeners()->end(); ++listener) {\
		((context *)(*listener))->expression;\
	}

namespace uc {

using namespace std;

template <class T>
class Event {
protected:
	std::set<T *> _listeners;

public:
	Event(){};
	virtual ~Event(){};

	void registerListener(T *listener) {
		_listeners.insert(listener);
	}

	void removeListener(T *listener) {
		_listeners.erase(listener);
	}

	void removeAllListeners() {
		_listeners.clear();
	}

	set<T *> *getListeners() {
		return &_listeners;
	}
};

} /* namespace uc */
#endif /* EVENT_H_ */
