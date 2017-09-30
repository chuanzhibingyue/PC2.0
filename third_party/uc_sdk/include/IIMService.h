/*
* IIMService.h
*
*  Created on: Jun 1, 2015
*      Author: ethan
*/

#ifndef IIMSERVICE_H_
#define IIMSERVICE_H_

#include "UCSDK.h"
#include "common/event.h"
#include "IUCEngine.h"
#include "UCMessage.h"
#include "MessageInfo.h"
#include "DocumentInfo.h"
#include "MessageReadInfo.h"

namespace uc {

interface UC_API IIMServiceEvent {
	virtual ~IIMServiceEvent(){};

	/**
	* 获取聊天历史列表响应事件;
	* @param result 	     错误信息;
	* @param channel         消息来源;
	* @param userId		     用户ID;
	* @param msgCount		 message count;
	* @param min_timestamp   min timestamp;
	*/					     
	virtual void OnChatHistoryReceived(int32_t result,  MessageChannel channel, int64_t userId, int32_t msgCount, int64_t min_timestamp) = 0;

	/**
	* 消息发送结果响应事件;
	* @param result 	     错误信息;
	* @param userId	         用户ID;
	* @param msgId           本地消息ID;
	* @param msgSeq          服务器消息ID;
	* @param msgTime         服务器消息发送时间;
	*/
	virtual void OnMessageSent(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 接收文本聊天消息响应事件;
	* @param channel 消息的来源;
	* @param message 文本消息对象;
	*/
	virtual void OnTextMessageReceived(MessageChannel channel, const TextMessageInfo *message) = 0;

	/**
	* 接收图片消息响应事件;
	* @param channel 消息的来源;
	* @param message 图片消息对象;
	*/
	virtual void OnImageMessageReceived(MessageChannel channel, const MediaMessageInfo *message) = 0;

	/**
	* 接收音频消息响应事件;
	* @param channel 消息来源
	* @param message 音频消息对象;
	*/
	virtual void OnAudioMessageReceived(MessageChannel channel, const MediaMessageInfo *message) = 0;

	/**
	* 接收视频消息响应事件;
	* @param channel 消息来源
	* @param message 视频消息对象;
	*/
	virtual void OnVideoMessageReceived(MessageChannel channel, const MediaMessageInfo *message) = 0;

	/**
	* 接收报告消息响应事件;
	* @param channel 消息来源
	* @param message 报告消息对象;
	*/
	virtual void OnReportMessageReceived(MessageChannel channel, const ReportMessageInfo *message) = 0;

	/**
	* 接收会议视频录制消息响应事件;
	* @param channel 消息来源
	* @param message 视频消息对象;
	*/
	virtual void OnRecordVideoReceived(MessageChannel channel, const MediaMessageInfo *message) = 0;

	/**
	* 接收到文件消息响应事件;
	* @param channel 消息来源
	* @param operation 0:create; 1:forward
	* @param message 文件消息对象;
	*/
	virtual void OnFileMessageReceived(MessageChannel channel, int32_t operation, const FileMessageInfo *message) = 0;

	/**
	* 云盘文件消息事件;
	* @param channel 消息来源
	* @param message 文件消息对象;
	*/
	virtual void OnCloudFileCreateMessageReceived(MessageChannel channel, const FileMessageInfo &message) = 0;

	/**
	* 文件删除响应事件;
	* @param userUid         发送者ID;
	* @param msgId           本地消息ID;
	* @param msgSeq          服务器消息ID;
	* @param fileName        文件名称;
	* @param fileId          文档ID;
	*/
	virtual void OnFileDeleted(UCID userUid, int32_t msgId, int64_t msgSeq, const std::string& fileName, int64_t fileId) = 0;

	/**
	* 接收到文件消息响应事件;
	* @param channel 消息来源
	* @operation     0:create; 1:forward
	* @param message 代码消息对象;
	*/
	virtual void OnCodeMessageReceived(MessageChannel channel, int32_t operation, const CodeMessageInfo& message) = 0;

	/**
	* 消息已读响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param readType        消息已读类型:0 单条已读/1 批量已读;
	* @param msgId           本地消息ID（单条已读时有效）;
	* @param msgSeq          服务器消息ID（单条已读时有效）;
	*/
	virtual void OnMessageRead(int32_t result, int32_t userId, int32_t readType, int32_t msgId, int64_t msgSeq) = 0;

	/**
	* 消息已播放响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param msgId           本地消息ID;
	* @param msgSeq          服务器消息ID;
	*/
	virtual void OnMessagePlayed(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq) = 0;

	/**
	* 接收消息未读用户个数消息;
	* @param result  		 错误信息;
	* @param userId          用户ID;
	* @param info            消息未读信息列表;
	*/
	virtual void OnMessageUnreadUserCount(int32_t result, int32_t userId, const std::list<MessageReadInfo>& info) = 0;

	/**
	* 音频通话邀请响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param confId          会议ID;
	* @param confPwd         会议密码;
	* @param hostId          主持人ID;
	*/
	virtual void OnCallInvited(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, const std::string confId, const std::string confPwd, int32_t hostId,
		int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 音频通话接受响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param confId          会议ID;
	*/
	virtual void OnCallAccepted(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, const std::string confId, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 音频通话拒绝响应事件;
	* @param result 	     错误信息;
	* @param msgId           本地消息ID;
	* @param msgSeq          服务器消息ID;
	* @param userId          用户ID;
	* @param confId          会议ID;
	*/
	virtual void OnCallRejected(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime,
		const std::string confId, int32_t hostId, std::string reason) = 0;

	/**
	* 音频通话结束响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param confId          会议ID;
	*/
	virtual void OnCallStopped(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, const std::string confId, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 音频通话取消响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param msgId           本地消息ID;
	* @param msgSeq          服务器消息ID;
	*/
	virtual void OnCallCanceled(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime, const std::string& phoneNum) = 0;

	/**
	* 音频通话未接通响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param msgId           本地消息ID;
	* @param msgSeq          服务器消息ID;
	*/
	virtual void OnCallNotResponded(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 音频通话接通时长响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param msgId           本地消息ID;
	* @param msgSeq          服务器消息ID;
	* @param duration        通话时间;
	*/
	virtual void OnCallConnected(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime, int32_t duration, const std::string& phoneNum) = 0;

};

interface UC_API IIMService: public IUCService, Event<IIMServiceEvent>, IUCEngineEvent {
	virtual ~IIMService(){};


	/**
	* 获取历史聊天内容接口;
	* @param channel            消息来源;
	* @param userId             请求用户ID;
	* @param count              请求聊天内容数量，默认20，不能超过200;
	* @param startTime          请求聊天内容的开始时间;
	* @param endTime            请求聊天内容的结束时间;
	* @return					 错误码;
	*/
	virtual int32_t GetChatHistory(MessageChannel channel, int64_t userId, int64_t count, int64_t startTime, int64_t endTime) = 0;

	/**
	* 重发消息;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ResendMessage(int32_t msgId) = 0;
	
	/**
	* 转发消息;
	* @param toUser 			接收者ID;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ForwardMessage(UCID userUid, int32_t msgId) = 0;

	/**
	* 发送文本聊天消息接口;
	* @param toUid 			    接收者ID;
	* @param textType		    文本类型：0x01:纯文本，0x02:MIME格式;
	* @param textFormat		    文本格式;
	* @param textData		    文本内容;
	* @param [out]msgId		    消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendTextMessage(UCID toUser, int8_t textType, const std::string& textFormat, 
		const std::string& textData, OUT int32_t& msgId) = 0;

	/**
	* 发送图片聊天消息接口;
	* @param userUid            接收者ID;
	* @param imageName	        图片名称;
	* @param imageSize	        图片大小;
	* @param imageURL	        图片下载路径;
	* @param imageData	        图片缩略图;
	* @param [out]msgId         消息ID;
	* @return			        错误码;
	*/
	virtual int32_t SendImageMessage(UCID userUid, const std::string& imageName, int32_t imageSize,
		const std::string& imageURL, const std::string& imageData, OUT int32_t& msgId) = 0;

	/**
	* 发送音频聊天消息接口;
	* @param userUid            接收者ID;
	* @param audioName          音频名称;
	* @param audioSize          音频大小;
	* @param audioURL           音频下载路径;
	* @param audioDuration      音频时长;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendAudioMessage(UCID userUid, const std::string& audioName, int32_t audioSize,
		const std::string& audioURL, int32_t audioDuration, OUT int32_t& msgId) = 0;

	/**
	* 发送视频聊天消息接口;
	* @param userUid            接收者ID;
	* @param videoName          视频名称;
	* @param videoSize          视频大小;
	* @param videoURL           视频下载路径;
	* @param videoData          视频缩略图;
	* @param videoDuration      视频时长;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendVideoMessage(UCID userUid, const std::string& videoName, int32_t videoSize,
		const std::string& videoURL, const std::string& videoData, int32_t videoDuration, OUT int32_t& msgId) = 0;

	/**
	* 发送聊天报告消息接口;
	* @param userUid            接收者;
	* @param reportTitle		报告标题;
	* @param reportUrl          报告URL;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendReportMessage(UCID userUid, const std::string& reportTitle, const std::string& reportUrl, OUT int32_t& msgId) = 0;

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
		const std::string& videoThumb, const std::string& videoThumbURL, int32_t videoDuration, int32_t operatorId, const std::string& operatorName, const std::string& operatorAvatar, int32_t& msgId) = 0;

	/**
	* 发送文件消息接口;
	* @param userUid            接收者ID;
	* @param operation          0: create 1: share;;
	* @param info               DocumentInfo;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendFileMessage(UCID userUid, int32_t operation, const DocumentInfo& info, OUT int32_t& msgId) = 0;

	/**
	* 发送云盘文件消息接口;
	* @param userUid            接收者ID;
	* @param fileName			文件/文件夹名称
	* @param fileSize			文件大小（单位：字节）
	* @param cloudType			云盘类型（1-够快云库, 2-安贞母婴中心）
	* @param detailContent      内容
	* @param createTime         operateTime（uccserver操作时间,单位：秒）
	* @param fileId				fileId
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendCloudFileMessage(UCID userUid, const std::string& fileName, int64_t fileSize,
		DocumentCloudType cloudType, const std::string& detailContent, OUT int32_t& msgId) = 0;

	/**
	* 删除文件消息接口;
	* @param userUid            接收者ID;
	* @param fileName           文件名称;
	* @param fileId             文档ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t DeleteFileMessage(UCID userUid, const std::string& fileName, int64_t fileId, OUT int32_t& msgId) = 0;

	/**
	* 发送代码消息接口;
	* @param userUid            接收者ID;
	* @param operation          0: create 1: share;;
	* @param info               DocumentInfo;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendCodeMessage(UCID userUid, int32_t operation, const DocumentInfo& info, OUT int32_t& msgId) = 0;

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
		const std::string& confPwd, int32_t hostId, OUT int32_t& msgId) = 0;

	/**
	* 同意音频通话消息接口;
	* @param userUid            接收者ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendAcceptCallMessage(UCID userUid, const std::string& confId, int32_t hostId, OUT int32_t& msgId) = 0;

	/**
	* 拒绝音频通话消息接口;
	* @param userUid            接收者ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendRejectCallMessage(UCID userUid, const std::string& confId, int32_t hostId, const std::string& reason, OUT int32_t& msgId) = 0;

	/**
	* 结束音频通话消息接口;
	* @param userUid            接收者ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendFinishCallMessage(UCID userUid, const std::string& confId, int32_t hostId, const std::string& reason, OUT int32_t& msgId) = 0;

	/**
	* 取消音频通话消息接口;
	* @param userUid            接收者ID;
	* @param phoneNum           电话号码;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendCancelCallMessage(UCID userUid, const string& phoneNum, OUT int32_t& msgId) = 0;

	/**
	* 未接通音频通话记录消息接口;
	* @param userUid            接收者ID;
	* @param phoneNum           电话号码;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendNotRespondCallRecordMessage(UCID userUid, const std::string& phoneNum, OUT int32_t& msgId) = 0;

	/**
	* 拒绝音频通话记录消息接口;
	* @param userUid            接收者ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	//virtual int32_t SendRejectCallRecordMessage(UCID userUid, OUT int32_t& msgId) = 0;

	/**
	* 接通音频通话记录消息接口;
	* @param userUid            接收者ID;
	* @param druation           通话时长;
	* @param phoneNum           电话号码;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendConnectedCallRecordMessage(UCID userUid, int32_t druation, const std::string& phoneNum, OUT int32_t& msgId) = 0;

	/**
	* 发送消息已读接口;
	* @param userUid            接收者ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessageRead(UCID userUid, int32_t msgId, int64_t msgSeq) = 0;

	/**
	* 发送多条消息已读接口;
	* @param toUserId           用户ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessagesRead(int32_t toUserId) = 0;

	/* 发送消息已播放接口;
	* @param userUid            接收者ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessagePlayed(UCID userUid, int32_t msgId, int64_t msgSeq) = 0;

};

}
#endif /* IIMSERVICE_H_ */
