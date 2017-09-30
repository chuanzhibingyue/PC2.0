/*
 * CallRequestAssist.h
 *
 *  Created on: 2015年8月3日
 *      Author: hualong.zhang
 */

#ifndef CALLRESPONSEASSIST_H_
#define CALLRESPONSEASSIST_H_

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "common/platform.h"
#include "CallDef.h"
#include "HTTPResponseAssist.h"
#include <vector>

namespace uc {
class CallResponseAssist : virtual public HTTPResponseAssist {

public:
	CallResponseAssist();
	virtual ~CallResponseAssist();
	void RetrieveUserIdAndToken(int &tempUserId, std::string &token);
	void RetrieveConfrenceIdAndReserveCode(int &tempConferenceId);
	void RetrieveUserStatus(std::vector<UserCallStatus>& userStatus);
	void RetrieveCallInfo(int32_t& confId, int32_t& from, int32_t& to, int32_t& type);
};

}

#endif //CALLRESPONSEASSIST_H_
