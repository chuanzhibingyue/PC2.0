#ifndef IBBSERVICE_H_
#define IBBSERVICE_H_

#include "UCSDK.h"
#include "IUCEngine.h"

namespace uc {

	interface UC_API IBBSServiceEvent {
		virtual ~IBBSServiceEvent() {};

		virtual void OnResponse(int32_t taskID, int32_t errorcode, const std::string& response) = 0;
	};

	interface UC_API IBBSService : public IUCService, Event<IBBSServiceEvent> {
		virtual ~IBBSService() {};
		virtual int32_t BBSRequest(const std::string& requestData, OUT int32_t& taskId) = 0;
	};
}


#endif /* IBBSERVICE_H_*/