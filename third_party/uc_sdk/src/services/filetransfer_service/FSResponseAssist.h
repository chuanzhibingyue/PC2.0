/*
 * FSRequestAssist.h
 *
 *  Created on: 2015年9月7日
 *      Author: hualong.zhang
 */

#ifndef FSRESPONSEASSIST_H_
#define FSRESPONSEASSIST_H_

#include <boost/property_tree/ptree.hpp>
#include "common/platform.h"
#include "HTTPResponseAssist.h"
#include "HTTPRequest.h"

namespace uc {
class FSResponseAssist : virtual public HTTPResponseAssist {

public:
	FSResponseAssist(const std::string& serverDomain, int userId);
	virtual ~FSResponseAssist();
	int32_t GuardStatementForHeader(const HeaderMap &header);
	std::string RetrieveConvertedFid(const HeaderMap &header, int32_t convertedType);
	std::string RetrieveValueFromKey(const HeaderMap &header, const std::string &key);
	std::string RetrieveDownloadURL(const std::string& fid);
	int32_t RetrieveFileLength(const HeaderMap &header, int64_t &len);
	int32_t RetrieveContentLength(const HeaderMap &header, int64_t fileSize, int64_t offset);
	int64_t RetrieveOffset(const HeaderMap &header);
private:
	const std::string _serverDomain;
	const int _userId;
};

}

#endif //FSRESPONSEASSIST_H_
