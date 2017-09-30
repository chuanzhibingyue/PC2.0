/*
 * ExternalServiceStrategy.h
 *
 *  Created on: Jan 28, 2016
 *      Author: yongshuai.cheng
 */

#ifndef EXTERNAL_SERVICE_STRATEGY_H_
#define EXTERNAL_SERVICE_STRATEGY_H_

#include "IExternalService.h"
#include "CefClient.h"

namespace ucclient {
	
using namespace uc;

class ExternalServiceStrategy : public IExternalServiceEvent {
public:
	ExternalServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);

	int32_t ShowExternalMessage(const std::wstring& externalId);

	int32_t GetChatHistory(ConversationType type, const std::wstring& externalId, int32_t count, int64_t startTime, int64_t endTime);

	// IIMServiceEvent方法
	virtual void OnChatHistoryReceived(int32_t result, ConversationType type, int64_t relateId, int32_t msgCount, int64_t min_timestamp, const list<ExternalMessage*>& msgList);


private:
	cefIntegration::CefClient *_cefClient;
	IExternalService *_externalService;
	IUCEngine *_engine;
};

}

#endif