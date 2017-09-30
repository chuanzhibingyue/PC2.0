/*
 * CallRequestAssist.h
 *
 *  Created on: 2015年8月3日
 *      Author: hualong.zhang
 */

#ifndef HTTPRESPONSEASSIST_H_
#define HTTPRESPONSEASSIST_H_

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include "common/platform.h"

namespace uc {
	class HTTPResponse;
	using namespace boost::property_tree;
class HTTPResponseAssist {

public:
	HTTPResponseAssist();
	virtual ~HTTPResponseAssist();
	int32_t GuardStatementForCallBack(int32_t errorCode, const HTTPResponse *response);
	int32_t GuardStatementForCallBackCode(int32_t errorCode, int32_t statusCode);
	int32_t RetrieveHttpContentCode() const;
	const wptree &ReturnWPTree() const;
public:
	static int32_t GuardStatementForCallBack(const std::string &method, int32_t errorCode, const HTTPResponse *response);

private:
	int _RetrieveErrorCodeFromHttpContent(const std::string &content);
	bool _RetrieveJsonPtreeFromHttpContent(const std::string &content);
	void _RetrieveJsonPtreeFromHttpContentInternal(wptree &ptRoot, const std::string &content);

protected:
	wptree _ptRoot;
};

}

#endif //HTTPRESPONSEASSIST_H_