/*
 * HTTPResponse.h
 *
 *  Created on: 2015年2月12日
 *      Author: jianxue
 */

#ifndef HTTPRESPONSE_H_
#define HTTPRESPONSE_H_

#include <map>
#include <string>
#include "UCSDK.h"

namespace uc {

using namespace std;

typedef map<string, string>	HeaderMap;

class HTTPResponse {
public:
	HTTPResponse();
	virtual ~HTTPResponse();

public:
	void 	SetStatusCode(int32_t statusCode);
	int32_t GetStatusCode() const;
	void	AddHeaderInfo(const string &name, const string &value);
	void 	ClearContent();
	const string& AppendContent(const char *content, int32_t length);
	const string& GetContent() const;
	const HeaderMap& GetHeaderInfo() const;
	void    ClearHeadInfo();
	string GetHeaderValue(const string& key);

private:
	int32_t _statusCode;
	string _url;
	string _content;
	HeaderMap _headerInfo;
};

} /* namespace uc */

#endif /* HTTPRESPONSE_H_ */
