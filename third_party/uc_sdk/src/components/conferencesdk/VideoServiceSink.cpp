#include "VideoServiceSink.h"
#include "LogManager.h"

using namespace uc;

VideoServiceSink::VideoServiceSink(int conferenceId)
:m_conferenceId(conferenceId) {

}

VideoServiceSink::~VideoServiceSink() {

}

void VideoServiceSink::onSessionErrorHandle(CGNetTangSessionErrorInfo *pErrorInfo) {
	if (NULL == pErrorInfo->description || NULL == pErrorInfo->statusCode) {
		UC_LOG(ERROR) << "pErrorInfo is NULL";
		return;
	}

	UC_LOG(ERROR) << "video error: messageID:" << pErrorInfo->messageID
		<< ",description:" << pErrorInfo->description
		<< ",statusCode:" << pErrorInfo->statusCode;
}

//视频业务属性更新通知
void VideoServiceSink::onVideoPropertyChanged(const char *propName,
	const CGNetTangVariant &oldPropValue,
	const CGNetTangVariant &newPropValue) {
	UC_LOG(INFO) << "onVideoPropertyChanged, propName:" << propName
		<< ", oldPropValue:" << oldPropValue.intVal
		<< ", newPropValue:" << newPropValue.intVal;
	//视频服务状态 true: 可用； false : 不可用
	if (strcmp(propName, "isServiceReady") == 0) {
		bool isReady = newPropValue.boolVal;
		//TODO
	}
	//视频设备状态变化通知
	else if (strcmp(propName, "notifyEvent") == 0) {
		int status = newPropValue.uintVal;
		//TODO
	}
	else {
		//TODO
	}
}

//新增视频共享用户通知
void VideoServiceSink::onVideoInstanceAdded(const IGNetTangVideoInstance* pInst) {
	if (pInst != NULL) {
		unsigned int userId = ((IGNetTangVideoInstance*)pInst)->getVideoUserID();
		m_pDelegate->OnVideoInstanceAdded(userId);
	}
}

//移除视频共享用户通知
void VideoServiceSink::onVideoInstanceRemoved(const IGNetTangVideoInstance* pInst) {
	if (pInst != NULL) {
		unsigned int userId = ((IGNetTangVideoInstance*)pInst)->getVideoUserID();
		m_pDelegate->OnVideoInstanceRemoved(userId);
	}
}

void VideoServiceSink::onVideoInstancePropertyChanged(const char*propName,
	const CGNetTangVariant &oldPropValue,
	const CGNetTangVariant &newPropValue,
	const IGNetTangVideoInstance* pInst) {

}

//远程观看视频实时帧率输出
void  VideoServiceSink::onVideoFrameRate(unsigned int fps, unsigned int resample_fps,
	unsigned int number_of_streams) {
	UC_LOG(INFO) << "onVideoFrameRate, fps:" << fps << ", resample_fps:" << resample_fps
		<< ", number_of_streams:" << number_of_streams;
}