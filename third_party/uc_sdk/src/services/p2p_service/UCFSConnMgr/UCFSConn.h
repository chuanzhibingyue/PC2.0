#pragma once
#include "IOFSConnMgr.h"

namespace uc {

	class UCFSConn : public IFSConnMgr
	{
	public:
		UCFSConn();
		~UCFSConn();

	public:
		virtual int init();
		virtual void destroy();

		virtual int send(int16_t cmd);

	protected:
		IFSConnSink *getSink() const { return (IFSConnSink *)mlpSink; }

	};
}

