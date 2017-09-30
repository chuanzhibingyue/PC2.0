/*
 * CalendarServiceStrategy.h
 *
 *  Created on: Aug 19, 2015
 *      Author: hualong zhang
 */

#ifndef PBX_SERVICE_JS_EVENT_ASSIST_H_
#define PBX_SERVICE_JS_EVENT_ASSIST_H_

#include "JSEventAssist.h"
#include "common/platform.h"
#include "ITangUser.h"
#include <list>
#include "AC_PbxCallRecord.h"

namespace ucclient {
	using namespace uc;

	class PbxServiceJSEventAssist : virtual public JSEventAssist {
	public:
		PbxServiceJSEventAssist(cefIntegration::CefClient *cefClient);
		virtual ~PbxServiceJSEventAssist();

	public:
		std::string MakeParametersForOnLoginPbx(int32_t errorCode); 

		std::string MakeParametersForOnServiceStateChanged(int stateCode);

		std::string MakeParametersForOnStatusChanged(__int64 timeStamp, 
			int32_t taskID, int callState, const char* resultCode, const char* callUUID);

		std::string MakeParametersForOnMakeCall(int32_t errorCode, __int64 timeStamp,
			int32_t taskID, int micState, const char* toNumber);

		std::string MakeParametersForOnCallIncoming(__int64 timeStamp, 
			int32_t taskID, int callType, const char* fromNumber, const char* postValue);

		std::string MakeParametersForOnCallHangup(const AC_PbxCallRecord &data);

		std::string MakeParametersForOnGetCallRecordLst(
			int32_t errorCode, const PbxCallRecordList &lstData);

		std::string MakeParametersForOnGetCallDetailsLst(
			int32_t errorCode, const PbxCallRecordList &lstData);

		std::string MakeParametersForOnSearchCallRecord(
			int32_t errorCode, const PbxCallRecordList &lstData);
	};
}

#endif //PBX_SERVICE_JS_EVENT_ASSIST_H_