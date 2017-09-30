/*
 * CallRequestAssist.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: hualong.zhang
 */

#include <iostream>
#include "CallRequestAssist.h"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include "common/StringConvert.h"
#include <HTTPRequest.h>
#include <CommonDef.h>
#include "LogManager.h"

namespace uc {
	using namespace std;
	//using namespace boost::property_tree;

#define PARAMETER_USERID				L"userId"
#define PARAMETER_SESSIONID				L"sessionId"
#define PARAMETER_RESID					L"resId"
#define PARAMETER_GROUPID				L"groupId"
#define PARAMETER_USERINFO				L"userInfo"
#define PARAMETER_USERNAME				L"userName"
#define PARAMETER_CLIENTTYPE			L"clientType"

#define PARAMETER_TEMPCONFERENCEID		L"tempConferenceId"
#define PARAMETER_RESERVECODE			L"reserveCode"
#define PARAMETER_TEMPUSERID			L"tempUserId"
#define PARAMETER_CALLUSERIDS			L"callUserIds"


	CallRequestAssist::CallRequestAssist(AudioService* service, int clientType) {
		if (UCCLIENT_WIN == clientType)
		{
			_clientType = CALL_WIN;
		} else {
			_clientType = CALL_MOBILE;
		}

		_service = service;
		assert(_service != NULL);
	}

	CallRequestAssist::~CallRequestAssist() {

	}

	void CallRequestAssist::MakeRequestOfCreateCall(int64_t groupId, 
		HTTPRequest &request) {
		wptree ptRoot;
		_PutUserJIDToWPTree(ptRoot);
		if (0 != groupId) { ptRoot.put(PARAMETER_GROUPID, groupId);}
		_PutUserInfoToWPTree(ptRoot);
		
		wstring json;
		_RetrieveJsonFromWptree(ptRoot, json);
		_RetrieveRequest(json, CALL_CREATE_METHOD, request);
	}

	void CallRequestAssist::MakeRequestOfChangeCallType(int groupId, 
		int conferenceId, HTTPRequest &request) {
		wptree ptRoot;
		ptRoot.put(PARAMETER_USERID, _service->RetrieveUserID());
		ptRoot.put(PARAMETER_SESSIONID, StringConvert::FromUTF8Str(_service->RetrieveSessionID()));
		ptRoot.put(PARAMETER_TEMPCONFERENCEID, conferenceId);
		ptRoot.put(PARAMETER_GROUPID, groupId);

		wstringstream wsStream;
		write_json(wsStream, ptRoot);
		wstring json = wsStream.str();
		_RetrieveRequest(json, CHANGE_CALL_TYPE, request);
	}

	void CallRequestAssist::MakeRequestOfJoinCall(int tempConferenceId, int64_t groupId, HTTPRequest &request) {
		wptree ptRoot;
		_PutUserJIDToWPTree(ptRoot);
		if (0 != groupId) { ptRoot.put<int64_t>(PARAMETER_GROUPID, groupId); }
		ptRoot.put(PARAMETER_TEMPCONFERENCEID, tempConferenceId);
		_PutUserInfoToWPTree(ptRoot);

		wstring json;
		_RetrieveJsonFromWptree(ptRoot, json);
		_RemoveQuotationMarkOfIntValueInString(tempConferenceId, json);
		_RetrieveRequest(json, CALL_JOIN_METHOD, request);
	}

	void CallRequestAssist::MakeRequestOfLeaveCall(int tempConferenceId, int tempUserId,
		HTTPRequest &request) {
		wptree ptRoot;
		_PutUserJIDToWPTree(ptRoot);
		ptRoot.put(PARAMETER_TEMPCONFERENCEID, tempConferenceId);
		ptRoot.put(PARAMETER_TEMPUSERID, tempUserId);

		wstring json;
		_RetrieveJsonFromWptree(ptRoot, json);
		_RemoveQuotationMarkOfIntValueInString(tempConferenceId, json);
		_RemoveQuotationMarkOfIntValueInString(tempUserId, json);
		_RetrieveRequest(json, CALL_LEAVE_METHOD, request);
	}

	void CallRequestAssist::MakeRequestOfGetUserCallStatus(std::vector<int32_t>& userIds, HTTPRequest &request) {
		std::wstringstream ids;
		ids << "{\"userId\":" << _service->RetrieveUserID()
			<< ",\"sessionId\":\"" << StringConvert::FromUTF8Str(_service->RetrieveSessionID())
			<< "\",\"callUserIds\":[";
		for (std::vector<int32_t>::iterator itor = userIds.begin();
			itor != userIds.end(); ++itor) {
			if (itor != userIds.begin())
				ids << ",";
			ids << *itor;
		}
		ids << "]}";
		
		_RetrieveRequest(ids.str(), GET_CALL_STATUS, request);
	}

	void CallRequestAssist::MakeRequestOfGetLastAudioInvite(int32_t userId, HTTPRequest &request) {
		std::wstringstream wsStream;
		wsStream << "session_id=" << StringConvert::FromUTF8Str(_service->RetrieveSessionID())
			<< "&user_id=" << _service->RetrieveUserID()
			<< "&target_id=" << _service->RetrieveUserID();
		UC_LOG(INFO) << "MakeRequestOfGetLastAudioInvite, request:" << wsStream.str();
		_RetrieveRequest(wsStream.str(), GET_LAST_AUDIO_INVITE, request);
	}

	void CallRequestAssist::_RetrieveJsonFromWptree(const wptree &ptRoot, wstring &json) {
		wstringstream wsStream;
		write_json(wsStream, ptRoot);
		json = wsStream.str();

		int32_t resID = ptRoot.get<int32_t>(PARAMETER_RESID);
		_RemoveQuotationMarkOfIntValueInString(resID, json);
	}

	void CallRequestAssist::_RetrieveRequest(const wstring &json, const string &method, HTTPRequest &request) {
		string url(_service->_RetrieveServerDomain() + method);
		request.Init(url, HTTP_METHOD_POST);
		request.AddHeaderInfo(HTTP_HEADER_CONTENT_TYPE, HTTP_HEADER_CONTENT_TYPE_VALUE);
		request.SetBody(StringConvert::ToUTF8Str(json));
	}

	void CallRequestAssist::_PutUserJIDToWPTree(wptree &ptRoot) {
		ptRoot.put(PARAMETER_USERID, _service->RetrieveUserID());
		ptRoot.put(PARAMETER_SESSIONID, StringConvert::FromUTF8Str(_service->RetrieveSessionID()));
		
		ptRoot.put(PARAMETER_RESID, _service->RetrieveResID());
	}

	void CallRequestAssist::_PutUserInfoToWPTree(wptree &ptRoot) {
		wptree ptChild;
		string userId = StringConvert::Convert2String(_service->RetrieveUserID());
		ptChild.put(PARAMETER_USERNAME, StringConvert::FromUTF8Str(userId));
		ptChild.put(PARAMETER_CLIENTTYPE, _clientType);
		ptRoot.put_child(PARAMETER_USERINFO, ptChild);
	}

	void CallRequestAssist::_RemoveQuotationMarkOfIntValueInString(int value, wstring &strDest) {
		string strValue = StringConvert::Convert2String(value);
		wstring wstrValue = StringConvert::FromUTF8Str(strValue);
		wstring wstrValueWithQuotationMark = wstrValue;
		wstrValueWithQuotationMark.insert(0, L"\"");
		wstrValueWithQuotationMark.append(L"\"");
		strDest = strDest.replace(strDest.find(wstrValueWithQuotationMark), wstrValueWithQuotationMark.size(), wstrValue);
	}

}
