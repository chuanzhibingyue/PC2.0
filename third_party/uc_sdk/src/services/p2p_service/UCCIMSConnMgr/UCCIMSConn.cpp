#include "includes.h"
#include "UCCIMSConn.h"

namespace uc {

	UCCIMSConn *glpCIMSC = NULL;
	void *createInstanceCIMSC(const ISink *sink){

		if (NULL == glpCIMSC) {
			glpCIMSC = new UCCIMSConn();
			glpCIMSC->setMgrSink(sink);
		}

		return (void *)glpCIMSC;
	}

	void destroyInstanceCIMSC(void *lp){

		if (NULL != glpCIMSC) {
			delete (UCCIMSConn *)glpCIMSC;
			glpCIMSC = NULL;
		}
	}

	UCCIMSConn::UCCIMSConn()
	{
	}


	UCCIMSConn::~UCCIMSConn()
	{
	}

	int UCCIMSConn::init()
	{
		LOGI("UCCIMSConn::init()");

		m_lpNWMgr = (INetWorkMgr *)createInstanceNW((INetWorkSink *)this);
		m_lpNWMgr->init();

		getSink()->onNotify(ST_CIMSCONN);
		return 1;
	}

	void UCCIMSConn::destroy()
	{

	}

	int UCCIMSConn::getWanIP()
	{
		m_lpNWMgr->send(1, 1, 1, NULL);
		return 0;
	}

	void UCCIMSConn::onNotify(int nType) const
	{

	}

	int UCCIMSConn::onConn(int32_t session, int ret, int32_t fileID, int32_t peerID)
	{
		return 0;
	}

	int UCCIMSConn::OnAccept(int32_t session, int ret, int32_t fileID, int32_t peerID)
	{
		return 0;
	}

	int UCCIMSConn::onRecv(int32_t session, int len, char *buf)
	{
		getSink()->onGetWanIP(1);
		return 1;
	}

	int UCCIMSConn::onError(int32_t session, int ret)
	{
		return 1;
	}

}