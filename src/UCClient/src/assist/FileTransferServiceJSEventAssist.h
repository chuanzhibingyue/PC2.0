/*
 * CalendarServiceStrategy.h
 *
 *  Created on: Aug 19, 2015
 *      Author: hualong zhang
 */

#ifndef FILETRANSFER_SERVICE_JS_EVENT_ASSIST_H_
#define FILETRANSFER_SERVICE_JS_EVENT_ASSIST_H_

#include "JSEventAssist.h"
#include "common/platform.h"
#include "ITangUser.h"

namespace ucclient {
using namespace uc;
class FileTransferServiceJSEventAssist : virtual public JSEventAssist {
public:
	FileTransferServiceJSEventAssist(cefIntegration::CefClient *cefClient);
	virtual ~FileTransferServiceJSEventAssist();
	std::string MakeParametersForOnFileUploaded(int32_t errorCode, int32_t taskId,
		const std::string &fileName, int64_t fileSize, int32_t operatorId, int64_t operatorTime, const std::string& fid,
		const std::string& downloadURL, const std::string& convertFid, const std::string& convertDownloadURL);
	std::string MakeParametersForOnFileDeleted(int32_t errorCode, const std::string& fid);
	std::string MakeParametersForOnFileDownloaded(int32_t errorCode, int32_t taskId, const std::string &savePath);
	std::string MakeParametersForOnFileTransmission(int32_t errorCode, int32_t taskId, int32_t percent);
};

}

#endif //FILETRANSFER_SERVICE_JS_EVENT_ASSIST_H_