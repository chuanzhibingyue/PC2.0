/*
 * IMService.h
 *
 *  Created on: Jun 1, 2015
 *      Author: ethan
 */

#ifndef IMSERVICE_H_
#define IMSERVICE_H_

#include "IIMService.h"
#include <boost/property_tree/json_parser.hpp>
#include "MessageInfo.h"
#include <mutex>

#ifdef AUDIO_SERVICE
    #include "audio_service/AudioService.h"
#else
    #define AudioService int
#endif

namespace uc {

class IMService: public IIMService {
public:
	IMService(IUCEngine *engine);
	virtual ~IMService();

   /**
	* 通过ID获取聊天内容;
	* @param channel            消息来源;
	* @param userId             请求用户ID;
	* @param count              请求聊天内容数量，默认20，不能超过200;
	* @param startTime          请求聊天内容的开始时间;
	* @param endTime            请求聊天内容的结束时间;
	* @return					错误码;
	*/
	virtual int32_t GetChatHistory(MessageChannel channel, int64_t userId, int64_t count, int64_t startTime, int64_t endTime);

	/**
	* 重发消息;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ResendMessage(int32_t msgId);

	/**
	* 转发消息;
	* @param toUser  			接收者ID;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ForwardMessage(UCID userUid, int32_t msgId);

   /**
	* 发送文本聊天信息;
	* @param toUid              接收者ID;
	* @param textType           文本类型：纯文本，MIME格式;
	* @param textFormat		    文本格式;
	* @param textData           文本内容;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendTextMessage(UCID toUser, int8_t textType, const std::string& textFormat, const std::string& textData, int32_t& msgId);

   /**
	* 发送图片聊天信息;
	* @param userId              接收者ID;
	* @param imageName	         图片名称;
	* @param imageSize	         图片大小;
	* @param imageURL	         图片下载路径;
	* @param imageData	         图片缩略图;
	* @param [out]msgId          消息ID;
	* @return			         错误码;
	*/
	virtual int32_t SendImageMessage(UCID userUid, const std::string& imageName, int32_t imageSize,
			const std::string& imageURL, const std::string& imageData, int32_t& msgId);

   /**
	* 发送音频聊天信息;
	* @param userUid             接收者ID;
	* @param audioName           音频名称;
	* @param audioSize           音频大小;
	* @param audioURL            音频下载路径;
	* @param audioDuration       音频时长;
	* @param [out]msgId          消息ID;
	* @return                    错误码;
	*/ 
	virtual int32_t SendAudioMessage(UCID userUid, const std::string& audioName, int32_t audioSize,
		const std::string& audioURL, int32_t audioDuration, int32_t& msgId);

   /**
	* 发送视频聊天信息;
	* @param userUid             接收者ID;
	* @param videoName           视频名称;
	* @param videoSize           视频大小;
	* @param videoURL            视频下载路径;
	* @param videoData           视频缩略图;
	* @param videoDuration       视频时长;
	* @param [out]msgId          消息ID;
	* @return                    错误码;
	*/
	virtual int32_t SendVideoMessage(UCID userUid, const std::string& videoName, int32_t videoSize,
		const std::string& videoURL, const std::string& videoData, int32_t videoDuration, int32_t& msgId);

	/**
	* 发送聊天报告消息接口;
	* @param userUid           接收者;
	* @param reportTitle		报告标题;
	* @param reportUrl          报告URL;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendReportMessage(UCID userUid, const std::string& reportTitle, const std::string& reportUrl, OUT int32_t& msgId);
	
	/**
	* 发送会议录像信息;
	* @param userUid             接收者ID;
	* @param videoName           视频名称;
	* @param videoSize           视频大小;
	* @param videoURL            视频下载路径;
	* @param videoThumb          视频缩略图;
	* @param videoDuration       录制视频的时长;
	* @param operatorId          录制视频的用户ID;
	* @param operatorName        录制视频的用户名称;
	* @param operatorAvatar      录制视频的用图像URL;
	* @param [out]msgId          消息ID;
	* @return                    错误码;
	*/
	virtual int32_t SendRecordVideoMessage(UCID userUid, const std::string& videoName, int32_t videoSize, const std::string& videoURL,
		const std::string& videoThumb, const std::string& videoThumbURL, int32_t videoDuration, int32_t operatorId, const std::string& operatorName,
		const std::string& operatorAvatar, int32_t& msgId);

   /**
	* 发送文件消息接口;
	* @param userUid            接收者ID;
	* @param operation          0: create 1: share;;
	* @param info               DocumentInfo;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendFileMessage(UCID userUid, int32_t operation, const DocumentInfo& info, OUT int32_t& msgId);

	virtual int32_t SendCloudFileMessage(UCID userUid, const std::string& fileName, int64_t fileSize,
		DocumentCloudType cloudType, const std::string& detailContent, OUT int32_t& msgId);

	/**
	* 删除文件消息接口;
	* @param userUid            接收者ID;
	* @param fileName           文件名称;
	* @param fileId             文档ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t DeleteFileMessage(UCID userUid, const std::string& fileName, int64_t fileId, OUT int32_t& msgId);

	/**
	* 发送代码消息接口;
	* @param userUid            接收者ID;
	* @param operation          0: create 1: share;;
	* @param info               DocumentInfo;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendCodeMessage(UCID userUid, int32_t operation, const DocumentInfo& info, OUT int32_t& msgId);

   /**
	* 邀请音频通话消息接口;
	* @param userUid            接收者ID;
	* @param confId             会议ID;
	* @param confPwd            会议密码;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendInviteCallMessage(UCID userUid, const std::string& confId,
		const std::string& confPwd, int32_t hostId, int32_t& msgId);

	virtual int32_t SendInviteCallMessage(int callType, UCID userUid, const std::string& confId,
		const std::string& confPwd, int32_t hostId, int32_t& msgId, AudioService* service);

   /**
	* 同意音频通话消息接口;
	* @param userUid            接收者ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendAcceptCallMessage(UCID userUid, const std::string& confId, int32_t hostId, int32_t& msgId);
	
   /**
	* 拒绝音频通话消息接口;
	* @param userUid            接收者ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendRejectCallMessage(UCID userUid, const std::string& confId, int32_t hostId, const std::string& reason, int32_t& msgId);

   /**
	* 结束音频通话消息接口;
	* @param userUid            接收者ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendFinishCallMessage(UCID userUid, const std::string& confId, int32_t hostId, const std::string& reason, int32_t& msgId);

   /**
	* 取消音频通话消息接口;
	* @param userUid             接收者ID;
	* @param [out]msgId          消息ID;
	* @return                    错误码;
	*/
	virtual int32_t SendCancelCallMessage(UCID userUid, const string& phoneNum, int32_t& msgId);

   /**
	* 未接通音频通话记录消息接口;
	* @param userUid             接收者ID;
	* @param phoneNum            电话号码;
	* @param [out]msgId          消息ID;
	* @return                    错误码;
	*/
	virtual int32_t SendNotRespondCallRecordMessage(UCID userUid, const std::string& phoneNum, int32_t& msgId);

   /**
	* 拒绝音频通话记录消息接口;
	* @param userUid             接收者ID;
	* @param [out]msgId          消息ID;
	* @return                    错误码;
	*/
	//virtual int32_t SendRejectCallRecordMessage(UCID userUid, int32_t& msgId);

   /**
	* 接通音频通话记录消息接口;
	* @param userUid             接收者ID;
	* @param druation            通话时长ID;
	* @param phoneNum            电话号码;
	* @param [out]msgId          消息ID;
	* @return                    错误码;
	*/
	virtual int32_t SendConnectedCallRecordMessage(UCID userUid, int32_t druation, const std::string& phoneNum, int32_t& msgId);

   /**
	* 发送消息已读接口;
	* @param userUid            接收者ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessageRead(UCID userUid, int32_t msgId, int64_t msgSeq);

   /**
	* 发送多条消息已读接口;
	* @param userId             用户ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessagesRead(int32_t toUserId);

   /**
	* 发送消息已播放接口;
	* @param userUid            接收者ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessagePlayed(UCID userUid, int32_t msgId, int64_t msgSeq);


	// override UCEngineEvent OnMessageReceived
	virtual int32_t OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state);

public:

	int32_t GetMessageFromJSON(const boost::property_tree::wptree &jsonMsg, UCMsg *&msg, string &body);

protected:
	//UCC
	int32_t OnGetChatHistoryResponse(int32_t errorCode, HTTPResponse* response, int64_t userId, MessageChannel channel, 
		int64_t count, int64_t uistartTime, int64_t uiendTime, int64_t sdkstarttime, int64_t sdkendtime, int32_t update_flag_msgid);
	int32_t OnSendMessageResponse(int32_t errorCode, HTTPResponse *response, int32_t userId, int32_t msgId, bool saveUnread);
	int32_t OnSendInviteCallMessageResponse(int32_t errorCode, HTTPResponse *response, int32_t conferenceId, int32_t callType, int32_t userId, int32_t msgId, bool saveUnread, AudioService* service);
	int32_t OnSendMessageReadResponse(int32_t errorCode, HTTPResponse *response, int32_t userId, int32_t readType, int32_t msgId, int64_t msgSeq);
	int32_t OnSendMessagePlayedResponse(int32_t errorCode, HTTPResponse *response, int32_t userId, int32_t msgId, int64_t msgSeq);
	int32_t OnGetUnreadUserCountResponse(int32_t errorCode, HTTPResponse *response, int32_t userId, std::list<int64_t> seq);

	void OnTextMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread);
	void OnMediaMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread);
	void OnReportMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnFileMessageNotify(MessageChannel channel, int32_t operation, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread);
	void OnDeleteFileNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);
	void OnCloudFileCreateNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnCodeMessageNotify(MessageChannel channel, int32_t operation, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave, bool& saveUnread);
	void OnMessageReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);
	void OnMessageBatchReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnMessagePlayedNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);
	void OnInviteCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnAcceptCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnRejectCallNofify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnFinishCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnCancelCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnNotRespondCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnConnectedCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);

	void OnMessageUnreadUserNotify(int32_t userId, int64_t msgSeq);

protected:

	UCID GetCurUser();
	std::string GetCurUserName();
	bool IsCurUser(UCID userUid);

	int32_t GetUnreadUserCount(int32_t userId, std::list<int64_t> seq);
private:
	std::recursive_mutex _lock4UnReadMsg;
};

} /* namespace uc */

#endif /* IMSERVICE_H_ */
