#ifndef CONTACT_CACHE_LISTENER_H_
#define CONTACT_CACHE_LISTENER_H_

#include "common/platform.h"

namespace uc {

class ContactCacheListener {
public:
	virtual void OnContactCacheInitialized(int32_t res) = 0;
};

}


#endif