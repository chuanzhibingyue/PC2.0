/*
 * FSRequestAssist.h
 *
 *  Created on: 2015年9月7日
 *      Author: hualong.zhang
 */

#ifndef FSREQUESTASSIST_H_
#define FSREQUESTASSIST_H_

#include <boost/property_tree/ptree.hpp>
#include "common/platform.h"
#include <string>

namespace uc {
	using namespace boost::property_tree;
	class HTTPRequest;
	class UCFile;

class FSRequestAssist {

public:
	FSRequestAssist(const std::string& serverDomain, int userId, const std::string &sessionId);
	virtual ~FSRequestAssist();
	HTTPRequest MakeRequestOfCheckFile(const UCFile &ucFile);
	HTTPRequest MakeRequestOfUploadFile(const std::string &cookie, int64_t needSendSize);
	std::string RetrieveNoUserDownloadURL(const std::string &downloadURL);
	std::string RetrieveFidFromDownloadUrl(const std::string &downloadUrl);
	std::string MakeURLOfDownloadFile(const std::string &downloadURL, int32_t cvtWidth, int32_t cvtHeight, int64_t offset);
	HTTPRequest MakeRequestOfDeleteFile(const std::string &fid);
	HTTPRequest MakeRequestOfDownloadFile(const std::string &url);
	std::string MakeBodyHeader(const UCFile &ucFile, int64_t offset);
	int64_t CalculateNeedSendSize(int64_t size, int64_t offset, int64_t bodyHeaderLength);
	std::string _RetrieveUploadHeader(const UCFile &ucFile);
	std::string _RetrieveCommonParameter();
private:

	std::string _serverDomain;
	int _userId;
	std::string _sessionId;
};

}

#endif //FSREQUESTASSIST_H_
