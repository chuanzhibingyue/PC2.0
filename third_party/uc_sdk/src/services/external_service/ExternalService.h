/*
 * ExternalService.h
 *
 *  Created on: Jan 27, 2016
 *      Author: yongshuai.cheng
 */

#ifndef EXTERNALSERVICE_H_
#define EXTERNALSERVICE_H_

#include "IExternalService.h"
#include "common/event.h"


namespace uc {

enum EXTERNALSHOW{
	STATUS_EXTERNAL_HIDE = 0,
	STATUS_EXTERNAL_SHOW
};

class ExternalService : public IExternalService {
public:
	ExternalService(IUCEngine *engine);
	virtual ~ExternalService();

	//Implement interface of class IExternalService
	virtual int32_t GetChatHistory(ConversationType type, const string& externalId, int32_t count, int64_t startTime, int64_t endTime);

	virtual int32_t ShowExternalMessage(const string& externalId);

public: // UCEngine event
	virtual void OnUCASConnected(int32_t errorCode);

private:
	bool   InitExternalDB();
	string GetLoginAccount();
	string GetExternalDirectory();
	string GetExternalDBFile();
	string GetCurUserAccount();
	int64_t GetRelateId(ConversationType type, const string& externalId);
	bool ClearMessageList(list<ExternalMessage*>& msglist);

    
	

private:

	bool   _bInitDB;

};

} /* namespace uc */

#endif /* EXTERNALSERVICE_H_ */
