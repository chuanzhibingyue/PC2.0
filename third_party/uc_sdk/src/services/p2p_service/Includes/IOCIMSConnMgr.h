#pragma once

#include "IOMgr.h"

namespace uc {

	void *createInstance(const ISink *sink);
	void destroyInstance(void *lp);

	class ICIMSConnSink : public ISink
	{
	public:
		virtual void onGetWanIP(int32_t wanIP) = 0;
	};

	class ICIMSConnMgr : public IMgr
	{
	public:
		virtual int getWanIP() = 0;

		;
	};
}
