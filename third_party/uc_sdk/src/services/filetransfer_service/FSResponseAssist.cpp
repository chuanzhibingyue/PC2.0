/*
 * CallRequestAssist.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: hualong.zhang
 */

#include <iostream>
#include "FSResponseAssist.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include "common/StringConvert.h"
#include <CommonDef.h>
#include "LogManager.h"
#include "UCErrorCode.h"
#include "HTTPResponse.h"
#include "FSDef.h"

namespace uc {
	using namespace std;


	FSResponseAssist::FSResponseAssist(const std::string& serverDomain, int userId)
	: _serverDomain(serverDomain)
	, _userId(userId) {
	}

	FSResponseAssist::~FSResponseAssist() {

	}

	int32_t FSResponseAssist::GuardStatementForHeader(const HeaderMap &header) {
		//判断返回头信息
		HeaderMap::const_iterator itor = header.find(TANG_RESULT);
		if (itor == header.end()) {
			//与大鹏沟通，如果没有tangresult可以认为成功
			UC_LOG(INFO) << "error, get response X-Tang-Result error";
		} else {
			int32_t tangResult = (int32_t)StringConvert::Convert2Int64(itor->second);
			if (tangResult != UC_SUCCEEDED) {
				UC_LOG(ERROR) << "error, response X-Tang-Result = " << tangResult;

				//获取错误码
				int32_t tangErrorCode = FT_ERROR_RESPONSE_GETXTANGNERRORCODE;
				itor = header.find(TANG_ERROR_CODE);
				if (itor != header.end()) {
					tangErrorCode = (int32_t)StringConvert::Convert2Int64(itor->second);
					UC_LOG(ERROR) << "error, response X-Tang-Error-Code = " << tangErrorCode;
				}
				return tangErrorCode;
			}
		}
		return UC_SUCCEEDED;
	}

	std::string FSResponseAssist::RetrieveConvertedFid(const HeaderMap &header, int32_t convertedType) {
		std::string downloadURL;
		if (convertedType == FS_CONVERT_TYPE_THUMBID) {
			return RetrieveValueFromKey(header, TANG_FILE_THUMBID);
		}
		else if (convertedType == FS_CONVERT_TYPE_MP4) {
			return RetrieveValueFromKey(header, TANG_FILE_MP4ID);
		}
		else
		{
			return "";
		}
	}

	std::string FSResponseAssist::RetrieveValueFromKey(const HeaderMap &header, const std::string &key) {
		auto itor = header.find(key);
		if (itor == header.end()) {
			return "";
		}
		return itor->second;
	}

	std::string FSResponseAssist::RetrieveDownloadURL(const std::string& fid) {
		std::stringstream stream;
		stream << _serverDomain << FS_HDDOWN_METHOD;
		stream << "?fid=" << fid;
		stream << "&u=" << _userId;
		return stream.str();
	}

	int32_t FSResponseAssist::RetrieveFileLength(const HeaderMap &header, int64_t &len) {
		auto itor = header.find(FILE_LENGTH);
		if (itor == header.end()) {
			UC_LOG(ERROR) << "RetrieveFileLength error. File-Length is not exist";
			return FT_ERROR_RESPONSE_GETFILELENGTH;
		}
		int64_t length = StringConvert::Convert2Int64(itor->second);
		if (length <= 0) {
			UC_LOG(ERROR) << "RetrieveFileLength error. File-Length is not exist";
			return FT_ERROR_RESPONSE_FILELENGTH;
		}

		len = length;
		return UC_SUCCEEDED;
	}

	int32_t FSResponseAssist::RetrieveContentLength(const HeaderMap &header, int64_t fileSize, int64_t offset) {
		auto itor = header.find(CONTENT_LENGTH);
		if (itor == header.end()) {
			UC_LOG(ERROR) << "RetrieveContentLength error. get Content-Length error.";
			return FT_ERROR_RESPONSE_GETCONTENTLENGTH;
		}

		int64_t length = StringConvert::Convert2Int64(itor->second);
		int64_t resultLen = fileSize - offset;
		if (length != resultLen) {
			UC_LOG(ERROR) << "RetrieveContentLength error. Content-Length =  " << length << "result Length = " << resultLen;
			return FT_ERROR_RESPONSE_CONTENTLENGTH;
		}

		return UC_SUCCEEDED;
	}

	int64_t FSResponseAssist::RetrieveOffset(const HeaderMap &header) {
		std::string fileOffset = RetrieveValueFromKey(header, TANG_FILE_OFFSET);
		if (fileOffset.empty()) {
			//该文件未上传过
			UC_LOG(INFO) << "FSResponseAssist::RetrieveOffset error, get response X-Tang-Offset error";
			return 0;
		} else {
			//该文件上传过一部分
			return StringConvert::Convert2Int64(fileOffset);
		}
	}
}