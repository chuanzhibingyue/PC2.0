/*
 * CallRequestAssist.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: hualong.zhang
 */

#include <iostream>
#include "CallResponseAssist.h"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include "common/StringConvert.h"
#include <CommonDef.h>
#include "LogManager.h"
#include "UCErrorCode.h"
#include "HTTPResponse.h"

namespace uc {
	using namespace std;


	CallResponseAssist::CallResponseAssist() {

	}

	CallResponseAssist::~CallResponseAssist() {

	}

	void CallResponseAssist::RetrieveUserIdAndToken(int &tempUserId, string &token) {
		boost::property_tree::wptree ptData;
		ptData = _ptRoot.get_child(L"data");
		tempUserId = ptData.get<int32_t>(L"tempUserId");
		token = StringConvert::ToUTF8Str(ptData.get<std::wstring>(L"token"));
	}

	void CallResponseAssist::RetrieveConfrenceIdAndReserveCode(int &tempConferenceId) {
		boost::property_tree::wptree ptData;
		ptData = _ptRoot.get_child(L"data");
		tempConferenceId = ptData.get<int32_t>(L"tempConferenceId");
	}

	void CallResponseAssist::RetrieveUserStatus(std::vector<UserCallStatus>& userStatus) {
		boost::property_tree::wptree ptData;
		ptData = _ptRoot.get_child(L"data");

		BOOST_FOREACH(const boost::property_tree::wptree::value_type &value, ptData){
			boost::property_tree::wptree jsonValue = value.second;
			if (!jsonValue.empty()) {
				UserCallStatus status;
				status.userId = jsonValue.get<int32_t>(L"userId");
				status.status = jsonValue.get<int32_t>(L"status");
				userStatus.push_back(status);
			}
		}
	}

	void CallResponseAssist::RetrieveCallInfo(int32_t& confId, int32_t& from, int32_t& to, int32_t& type) {
		boost::property_tree::wptree ptData;
		ptData = _ptRoot.get_child(L"data");
		if (ptData.empty()) {
			return;
		}

		confId = ptData.get<int32_t>(L"conf_id");
		from = ptData.get<int32_t>(L"from");
		to = ptData.get<int32_t>(L"to");
		type = ptData.get<int32_t>(L"type");
	}
}
