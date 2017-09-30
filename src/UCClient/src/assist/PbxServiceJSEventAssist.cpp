#include "PbxServiceJSEventAssist.h"
#include "LogManager.h"
#include "common/StringConvert.h"

namespace ucclient {
	using namespace uc;

	PbxServiceJSEventAssist::PbxServiceJSEventAssist(cefIntegration::CefClient *cefClient)
		: JSEventAssist(cefClient) {
	}

	PbxServiceJSEventAssist::~PbxServiceJSEventAssist() {
	}

	std::string PbxServiceJSEventAssist::MakeParametersForOnLoginPbx(int32_t errorCode) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));

		return _RetrieveParameters(parameters);

	}

	std::string PbxServiceJSEventAssist::MakeParametersForOnStatusChanged(
		__int64 timeStamp, int32_t taskID, int callState, const char* resultCode, const char* callUUID) {
		std::stringstream jsScript;
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(0)));
		jsScript << _RetrieveParameters(parameters);

		parameters.clear();
		jsScript << ", data:{";
		{
			parameters.insert(make_pair("timeStamp", StringConvert::Convert2String(timeStamp)));
			parameters.insert(make_pair("callID", StringConvert::Convert2String(taskID)));
			parameters.insert(make_pair("callState", StringConvert::Convert2String(callState)));
			parameters.insert(make_pair("resultCode", _AddSingleQuoteSurroundString(resultCode)));
			parameters.insert(make_pair("callUUID", _AddSingleQuoteSurroundString(callUUID)));
			jsScript << _RetrieveParameters(parameters);
		}
		jsScript << "}";

		return jsScript.str();
	}

	std::string PbxServiceJSEventAssist::MakeParametersForOnMakeCall(int32_t errorCode, __int64 timeStamp,
		int32_t taskID, int micState, const char* toNumber) {
		std::stringstream jsScript;
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		jsScript << _RetrieveParameters(parameters);

		parameters.clear();
		jsScript << ", data:{";
		{
			parameters.insert(make_pair("timeStamp", StringConvert::Convert2String(timeStamp)));
			parameters.insert(make_pair("callID", StringConvert::Convert2String(taskID)));
			parameters.insert(make_pair("micState", StringConvert::Convert2String(micState)));
			parameters.insert(make_pair("toNumber", _AddSingleQuoteSurroundString(toNumber)));
			jsScript << _RetrieveParameters(parameters);
		}
		jsScript << "}";

		return jsScript.str();
	}

	std::string PbxServiceJSEventAssist::MakeParametersForOnCallIncoming(
		__int64 timeStamp, int32_t taskID, int callType, const char* fromNumber, const char* postValue) {
		std::stringstream jsScript;
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(0)));
		jsScript << _RetrieveParameters(parameters);

		parameters.clear();
		jsScript << ", data:{";
		{
			parameters.insert(make_pair("timeStamp", StringConvert::Convert2String(timeStamp)));
			parameters.insert(make_pair("callID", StringConvert::Convert2String(taskID)));
			parameters.insert(make_pair("callType", StringConvert::Convert2String(callType)));
			parameters.insert(make_pair("fromNumber", _AddSingleQuoteSurroundString(fromNumber)));
			parameters.insert(make_pair("postValue", _AddSingleQuoteSurroundString(postValue)));
			jsScript << _RetrieveParameters(parameters);
		}
		jsScript << "}";

		return jsScript.str();
	}

	std::string PbxServiceJSEventAssist::MakeParametersForOnCallHangup(const AC_PbxCallRecord &data) {

		std::stringstream jsScript;
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(0)));
		jsScript << _RetrieveParameters(parameters);

		parameters.clear();
		jsScript << ", data:{"; 
		{
			parameters.insert(make_pair("time", StringConvert::Convert2String(data.GetOperatetime())));
			parameters.insert(make_pair("callID", StringConvert::Convert2String(data.GetCallid())));
			parameters.insert(make_pair("name", _AddSingleQuoteSurroundString(data.GetName())));
			parameters.insert(make_pair("number", _AddSingleQuoteSurroundString(data.GetNumber())));
			parameters.insert(make_pair("state", StringConvert::Convert2String(data.GetStateEx())));
			parameters.insert(make_pair("userID", StringConvert::Convert2String(data.GetCallid())));
			parameters.insert(make_pair("type", StringConvert::Convert2String(data.GetType())));
			parameters.insert(make_pair("druation", StringConvert::Convert2String(data.GetDruation())));
			parameters.insert(make_pair("incoming", StringConvert::Convert2String(data.IsIncoming())));
			jsScript << _RetrieveParameters(parameters);
		}
		jsScript << "}";

		return jsScript.str();
	}

	std::string PbxServiceJSEventAssist::MakeParametersForOnGetCallRecordLst(
		int32_t errorCode, const PbxCallRecordList &lstData) {
		std::stringstream jsScript;
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		jsScript << _RetrieveParameters(parameters);

		jsScript << ", data:[";
		PbxCallRecordList::const_iterator itor = lstData.begin();
		for (; itor != lstData.end(); ++itor) {
			AC_PbxCallRecord &Temp = (AC_PbxCallRecord)*itor;

			if (itor != lstData.begin()) {
				jsScript << ",";
			}

			jsScript << "{"
				<< "name:\"" << Temp.GetName() << "\""
				<< ","
				<< "userID:" << Temp.GetUserid()
				<< ","
				<< "number:\"" << Temp.GetNumber() << "\""
				<< ","
				<< "type :" << Temp.GetType()
				<< ","
				<< "incoming:" << Temp.IsIncoming()
				<< ","
				<< "time:" << Temp.GetOperatetime()
				<< ","
				<< "state:" << Temp.GetStateEx()
				<< ","
				<< "druation:" << Temp.GetDruation()
				<< ","
				<< "count:" << Temp.GetCallid()
				<< "}";
		}
		jsScript << "]";

		return jsScript.str();
	}

	std::string PbxServiceJSEventAssist::MakeParametersForOnGetCallDetailsLst(
		int32_t errorCode, const PbxCallRecordList &lstData) {
		std::stringstream jsScript;
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		jsScript << _RetrieveParameters(parameters);

		jsScript << ", data:[";
		PbxCallRecordList::const_iterator itor = lstData.begin();
		for (; itor != lstData.end(); ++itor) {
			AC_PbxCallRecord &Temp = (AC_PbxCallRecord)*itor;

			if (itor != lstData.begin()) {
				jsScript << ",";
			}

			jsScript << "{"
				<< "name:\"" << Temp.GetName() << "\""
				<< ","
				<< "userID:" << Temp.GetUserid()
				<< ","
				<< "number:\"" << Temp.GetNumber() << "\""
				<< ","
				<< "type :" << Temp.GetType()
				<< ","
				<< "incoming:" << Temp.IsIncoming()
				<< ","
				<< "time:" << Temp.GetOperatetime()
				<< ","
				<< "state:" << Temp.GetStateEx()
				<< ","
				<< "druation:" << Temp.GetDruation() 
				<< "}";
		}
		jsScript << "]";

		return jsScript.str();
	}

	std::string PbxServiceJSEventAssist::MakeParametersForOnSearchCallRecord(
		int32_t errorCode, const PbxCallRecordList &lstData) {
		std::stringstream jsScript;
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		jsScript << _RetrieveParameters(parameters);

		jsScript << ", data:[";
		PbxCallRecordList::const_iterator itor = lstData.begin();
		for (; itor != lstData.end(); ++itor) {
			AC_PbxCallRecord &Temp = (AC_PbxCallRecord)*itor;

			if (itor != lstData.begin()) {
				jsScript << ",";
			}

			jsScript << "{"
				<< "name:\"" << Temp.GetName() << "\""
				<< ","
				<< "userID:" << Temp.GetUserid()
				<< ","
				<< "number:\"" << Temp.GetNumber() << "\""
				<< ","
				<< "type :" << Temp.GetType() 
				<< "}";
		}
		jsScript << "]";

		return jsScript.str();
	}

	std::string PbxServiceJSEventAssist::MakeParametersForOnServiceStateChanged(int stateCode) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(stateCode)));

		return _RetrieveParameters(parameters);
	}

}