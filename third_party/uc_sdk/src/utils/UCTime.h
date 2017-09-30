#if !defined(UC_TIME_H_)
#define UC_TIME_H_

#include "UCSDK.h"

#ifndef WIN32
#include <sys/time.h>
#else
#include <sys/timeb.h>
#endif

namespace uc {

class Time
{
public:
	static int64_t GetSysTime() {
		int64_t timestamp = 0;
#ifdef WIN32
		struct timeb sysTime;    
		ftime(&sysTime);
		timestamp = sysTime.time * 1000 + sysTime.millitm;
#else
		struct timeval tv;
		if (gettimeofday(&tv, NULL) == 0) {
			timestamp = (int64_t)(tv.tv_sec * 1000L + tv.tv_usec / 1000L);
		}
#endif
		return timestamp;
	}
	
};

}
#endif//UC_TIME_H_
