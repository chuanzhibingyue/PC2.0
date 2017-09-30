#include "includes.h"
#include "UCFSConn.h"
#include "UCUtil.h"

namespace uc {

	UCFSConn *glpFSC = NULL;
	void *createInstanceFSC(const ISink *sink){

		if (NULL == glpFSC) {
			glpFSC = new UCFSConn();
			glpFSC->setMgrSink(sink);
		}

		return (void *)glpFSC;
	}

	void destroyInstanceFSC(void *lp){

		if (NULL != glpFSC) {
			delete (UCFSConn *)glpFSC;
			glpFSC = NULL;
		}
	}

	UCFSConn::UCFSConn()
	{
	}

	UCFSConn::~UCFSConn()
	{
	}

	int UCFSConn::init()
	{
		LOGI("UCFSConn::init()");
		getSink()->onNotify(ST_FSCONN);

		return 1;
	}

	void UCFSConn::destroy()
	{

	}

	int UCFSConn::send(int16_t cmd)
	{
		return 1;
	}
}