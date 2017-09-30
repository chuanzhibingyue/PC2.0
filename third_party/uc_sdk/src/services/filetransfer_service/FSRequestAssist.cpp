/*
 * CallRequestAssist.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: hualong.zhang
 */

#include <iostream>
#include "FSRequestAssist.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include "common/StringConvert.h"
#include <HTTPRequest.h>
#include "UCFile.h"
#include <CommonDef.h>
#include "LogManager.h"
#include "FSDef.h"

namespace uc {
	using namespace std;
	//using namespace boost::property_tree;


	FSRequestAssist::FSRequestAssist(const std::string& serverDomain, int userId,
		const std::string &sessionId)
		: _serverDomain(serverDomain)
		, _userId(userId)
		, _sessionId(sessionId) {
	}

	FSRequestAssist::~FSRequestAssist() {

	}

	HTTPRequest FSRequestAssist::MakeRequestOfCheckFile(const UCFile &ucFile) {
		std::stringstream stream;
		stream << _serverDomain;
		stream << FS_HDCHK_METHOD;
		stream << "?";
		stream << _RetrieveUploadHeader(ucFile);

		HTTPRequest request;
		request.Init(stream.str(), HTTP_METHOD_GET);
		return request;
	}

	std::string FSRequestAssist::RetrieveNoUserDownloadURL(const std::string &downloadURL) {
		//去掉原来的userId
		std::string url = downloadURL;
		int32_t pos = url.find("&u=");
		if (-1 != pos) {
			url = url.substr(0, pos);
		}
		return url;
	}

	std::string FSRequestAssist::RetrieveFidFromDownloadUrl(const std::string &downloadUrl) {
		const std::string noUserDownloadUrl = RetrieveNoUserDownloadURL(downloadUrl);
		int offset = noUserDownloadUrl.find(FS_URL_FID);
		int fidStart = offset + strlen(FS_URL_FID);
		return noUserDownloadUrl.substr(fidStart);
	}

	std::string FSRequestAssist::MakeURLOfDownloadFile(const std::string &downloadURL,
		int32_t cvtWidth, int32_t cvtHeight, int64_t offset) {
		std::stringstream stream;
		stream << downloadURL;
		stream << "&u=" << _userId;
		stream << "&k=" << _sessionId;
		stream << "&w=" << cvtWidth;
		stream << "&h=" << cvtHeight;
		stream << "&off=" << offset;
		
		return stream.str();
	}

	HTTPRequest FSRequestAssist::MakeRequestOfDownloadFile(const std::string &url) {
		HTTPRequest request;
		request.Init(url, HTTP_METHOD_GET);
		return request;
	}

	HTTPRequest FSRequestAssist::MakeRequestOfDeleteFile(const std::string &fid) {
		std::stringstream stream;
		stream << _RetrieveCommonParameter();
		stream << "&fid=" << fid;
		HTTPRequest request;
		request.Init(_serverDomain + FS_HDDEL_METHOD, HTTP_METHOD_POST);
		request.SetBody(stream.str());
		return request;
	}

	std::string FSRequestAssist::MakeBodyHeader(const UCFile &ucFile, int64_t offset) {
		stringstream stream;
		stream << _RetrieveUploadHeader(ucFile);
		stream << "&off=" << offset;
		stream << "&data=";

		return stream.str();
	}

	std::string FSRequestAssist::_RetrieveUploadHeader(const UCFile &ucFile) {
		stringstream stream;
		stream << _RetrieveCommonParameter();
		stream << "&nam=" << ucFile.URLEncodeFileName();
		stream << "&md5=" << ucFile._md5;
		stream << "&len=" << ucFile._size;
		return stream.str();
	}

	std::string FSRequestAssist::_RetrieveCommonParameter() {
		stringstream stream;
		stream << "a=8";
		stream << "&u=" << _userId;
		stream << "&k=" << _sessionId;
		return stream.str();
	}

	int64_t FSRequestAssist::CalculateNeedSendSize(int64_t size, int64_t offset, int64_t bodyHeaderLength) {
		//需要上传的大小=文件总大小-已经上传过的大小+body头的大小
		return (size - offset + bodyHeaderLength);
	}

	HTTPRequest FSRequestAssist::MakeRequestOfUploadFile(const std::string &cookie, int64_t needSendSize) {
		UC_LOG(INFO) << "MakeRequestOfUploadFile::cookie: " << cookie << ", needSendSize: " << needSendSize;
		HTTPRequest request;
		request.Init(_serverDomain + FS_HDUP_METHOD, HTTP_METHOD_POST);
		if (!cookie.empty())
		{
			request.AddHeaderInfo(TANG_COOKIE, cookie);
		}
		request.AddHeaderInfo(CONTENT_LENGTH, (uint32_t)needSendSize);
		return request;
	}
}
