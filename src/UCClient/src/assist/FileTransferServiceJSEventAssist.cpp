#include "FileTransferServiceJSEventAssist.h"
#include "LogManager.h"
#include "common/StringConvert.h"

namespace ucclient {
	using namespace uc;

	FileTransferServiceJSEventAssist::FileTransferServiceJSEventAssist(cefIntegration::CefClient *cefClient)
	: JSEventAssist(cefClient) {

	}

	FileTransferServiceJSEventAssist::~FileTransferServiceJSEventAssist() {

	}

	std::string FileTransferServiceJSEventAssist::MakeParametersForOnFileUploaded(int32_t errorCode, int32_t taskId,
		const std::string &fileName, int64_t fileSize, int32_t operatorId, int64_t operatorTime, const std::string& fid,
		const std::string& downloadURL, const std::string& convertFid, const std::string& convertDownloadURL) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		parameters.insert(make_pair("taskId", StringConvert::Convert2String(taskId)));
		parameters.insert(make_pair("fileName", _AddSingleQuoteSurroundString(fileName)));
		parameters.insert(make_pair("fileSize", StringConvert::Convert2String(fileSize)));
		parameters.insert(make_pair("operatorId", StringConvert::Convert2String(operatorId)));
		parameters.insert(make_pair("operatorTime", StringConvert::Convert2String(operatorTime)));
		parameters.insert(make_pair("fid", _AddSingleQuoteSurroundString(fid)));
		parameters.insert(make_pair("downloadURL", _AddSingleQuoteSurroundString(downloadURL)));
		parameters.insert(make_pair("convertFid", _AddSingleQuoteSurroundString(convertFid)));
		parameters.insert(make_pair("convertDownloadURL", _AddSingleQuoteSurroundString(convertDownloadURL)));

		return _RetrieveParameters(parameters);
	}

	std::string FileTransferServiceJSEventAssist::MakeParametersForOnFileDeleted(int32_t errorCode,	const std::string& fid) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		parameters.insert(make_pair("fid", _AddSingleQuoteSurroundString(fid)));

		return _RetrieveParameters(parameters);
	}

	std::string FileTransferServiceJSEventAssist::MakeParametersForOnFileDownloaded(int32_t errorCode, int32_t taskId,
		const std::string &savePath) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		parameters.insert(make_pair("taskId", StringConvert::Convert2String(taskId)));
		parameters.insert(make_pair("savePath", _AddSingleQuoteSurroundString(savePath)));

		return _RetrieveParameters(parameters);
	}

	std::string FileTransferServiceJSEventAssist::MakeParametersForOnFileTransmission(int32_t errorCode, int32_t taskId,
		int32_t percent) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		parameters.insert(make_pair("taskId", StringConvert::Convert2String(taskId)));
		parameters.insert(make_pair("percent", StringConvert::Convert2String(percent)));

		return _RetrieveParameters(parameters);
	}
}