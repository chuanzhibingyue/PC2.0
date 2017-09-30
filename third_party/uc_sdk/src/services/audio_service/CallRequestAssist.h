/*
 * CallRequestAssist.h
 *
 *  Created on: 2015年8月3日
 *      Author: hualong.zhang
 */

#ifndef CALLREQUESTASSIST_H_
#define CALLREQUESTASSIST_H_

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include "common/platform.h"
#include "CallDef.h"
#include "audio_service/AudioService.h"
#include <vector>

namespace uc {
	using namespace boost::property_tree;
	class HTTPRequest;

typedef struct _UserJID_ {
	int userId;
	std::string sessionId;
	int resId;
}UserJID;

class CallRequestAssist {

public:
	CallRequestAssist(AudioService* service, int clientType);
	virtual ~CallRequestAssist();
	void MakeRequestOfCreateCall(int64_t groupId, HTTPRequest &request);
	void MakeRequestOfJoinCall(int tempConferenceId, int64_t groupId, HTTPRequest &request);
	void MakeRequestOfLeaveCall(int tempConferenceId, int tempUserId, HTTPRequest &request);
	void MakeRequestOfChangeCallType(int groupId, int conferenceId, HTTPRequest &request);
	void MakeRequestOfGetUserCallStatus(std::vector<int32_t>& userIds, HTTPRequest &request);
	void MakeRequestOfGetLastAudioInvite(int32_t userId, HTTPRequest &request);
private:
	void _PutUserJIDToWPTree(wptree &ptRoot);
	void _PutUserInfoToWPTree(wptree &ptRoot);
	void _RetrieveJsonFromWptree(const wptree &ptRoot, std::wstring &json);
	void _RetrieveRequest(const std::wstring &json, const std::string &method, HTTPRequest &request);
	void _RemoveQuotationMarkOfIntValueInString(int value, std::wstring &strDest);

	//member variable
	int _clientType;
	AudioService* _service;
};

}

#endif //CALLREQUESTASSIST_H_
