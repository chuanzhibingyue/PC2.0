#ifndef BBS_SERVICE_STRATEHY_H_
#define BBS_SERVICE_STRATEHY_H_

#include "IBBSService.h"
#include "CefClient.h"
namespace ucclient {
	using namespace uc;

	class BBSServiceStrategy : public IBBSServiceEvent {
	public:
		BBSServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);
		virtual ~BBSServiceStrategy();

	public:
		int32_t BBSRequest(const std::wstring& requestData, OUT int32_t& taskId);
		void OnResponse(int32_t taskID, int32_t errorcode, const std::string& response);
	private:
		cefIntegration::CefClient *m_cefClient;
		IBBSService *m_bbsService;
		IUCEngine *m_engine;
	};
}
#endif //BBS_SERVICE_STRATEHY_H_