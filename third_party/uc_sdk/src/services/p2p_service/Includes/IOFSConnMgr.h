#pragma once

#include "IOMgr.h"

namespace uc {
	void *createInstanceFSC(const ISink *sink);
	void destroyInstanceFSC(void *lp);

	enum{
		pdu_init,
		pdu_data,
	};

	class IFSConnSink : public ISink
	{
	public:
	};

	class IFSConnMgr : public IMgr
	{
	public:
		virtual int send(int16_t cmd) = 0;
	};
}

