/*
* IOpenApiService.h
*
*  Created on: Sep 9, 2015
*      Author: yongshuai.cheng
*/

#ifndef IOPENAPISERVICE_H_
#define IOPENAPISERVICE_H_

#include "UCSDK.h"
#include "common/event.h"
#include "IUCEngine.h"
#include "UCMessage.h"
#include "MessageInfo.h"
#include "AppInfo.h"

namespace uc {

enum RequestMethod{
	HTTP_NULL_METHOD = -1,
	HTTP_GET_METHOD = 1,
	HTTP_POST_METHOD,
};

struct AppMessageInfo{
	int64_t appId;        //应用ID;
	int64_t msgSeq;		  //消息ID;
};

struct AppMessageStatusInfo{
	AppMessageStatusInfo(){
		appId    = 0;
		msgSeq   = 0;
		status   = -1;
		externalData = "";
	}
	int64_t appId;				//应用ID;
	int64_t msgSeq;				//消息ID;
	int16_t status;				//消息状态;
	std::string externalData;   //扩展数据;
};

struct HeadInfo{
	std::string headKey;
	std::string headValue;
};

interface UC_API IOpenApiServiceEvent {
	virtual ~IOpenApiServiceEvent(){};

	/**
	* 获取聊天历史列表响应事件;
	* @param result 	        错误信息;
	* @param appId		        应用ID;
	* @param msgCount		    message count;
	* @param min_timestamp		min timestamp;
	*/					     
	virtual void OnChatHistoryReceived(int32_t result, int64_t appId, int32_t msgCount, int64_t min_timestamp) = 0;

	/**
	* 获取应用列表响应事件;
	* @param result 	        错误信息;
	* @param appList 	        应用信息列表;
	*/
	virtual void OnAppListReceived(int32_t result, const std::list<AppInfo>& appList) = 0;

	/**
	* 获取应用信息响应事件;
	* @param result 	        错误信息;
	* @param appList 	        应用信息列表;
	*/
	virtual void OnAppInfoReceived(int32_t result, const std::list<AppInfo>& appList) = 0;

	/**
	* 获取OA消息状态响应事件;
	* @param result 	        错误信息;
	* @param infos 	            OA消息状态列表;
	*/
	virtual void OnOAMessageStatusReceived(int32_t result, const std::list<AppMessageStatusInfo>& infos) = 0;

	/**
	* 修改OA消息状态响应事件;
	* @param result 	        错误信息;
	* @param infos 	            OA消息状态列表;
	*/
	virtual void OnOAMessageStatusUpdated(int32_t result, const std::list<AppMessageStatusInfo>& infos) = 0;

   /**
	* 获取消息Ack状态响应事件;
	* @param result 	        错误信息;
	* @param infos 	            OA消息状态列表;
	*/
	virtual void OnMessageAckStatusReceived(int32_t result, const std::list<AppMessageStatusInfo>& infos) = 0;

	/**
	* 消息发送结果响应事件;
	* @param result 	        错误信息;
	* @param appId	            应用ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @param msgTime            服务器消息发送时间;
	*/
	virtual void OnMessageSent(int32_t result, int64_t appId, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 接收到HttpRequest请求返回响应事件;
	* @param result 	        错误信息;
	* @param reqUrl 	        Http请求Url;
	* @param content	        Http返回内容;
	*/
	virtual void OnResponseReceived(int32_t result, int32_t requestId, const std::string& content) = 0;

	/**
	* 获取应用状态响应事件;
	* @param channel            消息的来源;
	* @param message            消息属性;
	* @param appId 				应用ID;
	* @param status 	        应用状态;
	* @param externalData 	    扩展数据;
	*/
	virtual void OnAppStatusChangedReceived(MessageChannel channel, const MessageInfo &message, int64_t appId, int16_t status, const std::string& externalData) = 0;

	/**
	* 获取应用事件变化响应事件;
	* @param channel            消息的来源;
	* @param message            消息属性;
	* @param appId 				应用ID;
	* @param eventCount	        新事件数量;
	* @param externalData 	    扩展数据;
	*/
	virtual void OnAppEventChangedReceived(MessageChannel channel, const MessageInfo &message, int64_t appId, int32_t eventCount, const std::string& externalData) = 0;


	/**
	* 消息Push结果响应事件;
	* @param channel            消息的来源;
	* @param message            消息属性;
	* @param result 	        错误信息;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @param conversation       话题ID;
	* @param msgTime            服务器消息发送时间;
	*/
	virtual void OnPushMessageAck(MessageChannel channel, const MessageInfo &message, int32_t result, int32_t msgId, int64_t msgSeq, int32_t conversation) = 0;

	/**
	* OA消息状态变化事件;
	* @param channel            消息的来源;
	* @param message            消息属性;
	* @param statusInfo         OA消息最新状态;
	*/
	virtual void OnOAMessageStatusChangedReceived(MessageChannel channel, const MessageInfo &message, const AppMessageStatusInfo &statusInfo) = 0;

	/**
	* 接收OA消息响应事件;
	* @param channel            消息的来源;
	* @param message            消息属性;
	* @param title              消息标题;
	* @param status             消息状态;
	* @param titleColor         标题字体颜色;
	* @param titleBkColor       标题背景颜色;
	* @param data               消息内容;
	* @param detailUrl          消息详情URL;
	* @param detailAuth		    认证信息;
	* @param titleElements		标题栏扩展元素;
	* @param titleStyle		    标题栏风格;
	* @param customizedType		客户定制数据类型, 遵循mime规范;
	* @param customizedData		客户定制数据;
	*/
	virtual void OnOAMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& title, int16_t status,
		const std::string& titleColor, const std::string& titleBkColor, const std::string& data, const std::string& detailUrl,
		int8_t detailAuth, const std::string& titleElements, const std::string& titleStyle, const std::string& customizedType
		, const std::string& customizedData) = 0;

	/**
	* 接收定制消息响应事件;
	* @param channel            消息的来源;
	* @param message            消息属性;
	* @param frist              参数1;
	* @param second             参数2;
	* @param third				参数3;
	* @param four				参数4;

	*/
	virtual void OnCustomizedMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& frist, 
		const std::string& second, const std::string& third, const std::string& four ) = 0;


	/**
	* 接收文本聊天消息响应事件;
	* @param channel            消息的来源;
	* @param message            消息属性;
	* @param textTitle		    文本标题;
	* @param textContent		文本内容;
	* @param detailType		    详情格式：0x01:文本类型，0x02:URL链接类型;
	* @param detailContent		详情内容;
	* @param detailAuth		    认证信息;
	*/
	virtual void OnTextMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& textTitle, const std::string& textContent, int8_t detailType,
		const std::string& detailContent, int8_t detailAuth) = 0;

	/**
	* 接收图片消息响应事件;
	* @param channel            消息的来源;
	* @param message            消息属性;
	* @param imageId	        图片ID或图片URL;
	* @param imageType	        图片来源类型：0x01:FS图片ID，0x02:URL链接类型;
	* @param imageData	        图片缩略图;
	*/
	virtual void OnImageMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string &imageId, int8_t imageType, const std::string &imageData) = 0;

	/**
	* 接收到文件消息响应事件;
	* @param channel            消息来源;
	* @param message            消息属性;
	* @param fileId	            文件ID或文件URL;
	* @param fileType	        文件来源类型：0x01:FS文件ID，0x02:URL链接类型;
	* @param fileName	        文件名称;
	* @param fileSize	        文件大小;
	*/
	virtual void OnFileMessageReceived(MessageChannel channel, const MessageInfo &message, const std::string& fileId, int8_t fileType, const std::string fileName,
		int64_t fileSize) = 0;

	/**
	* 消息已读响应事件;
	* @param result 	     错误信息;
	* @param appId           用户ID;
	* @param readType        消息已读类型:0 单条已读/1 批量已读;
	* @param msgId           本地消息ID（单条已读时有效）;
	* @param msgSeq          服务器消息ID（单条已读时有效）;
	*/
	virtual void OnMessageRead(int32_t result, int64_t appId, int32_t readType, int32_t msgId, int64_t msgSeq) = 0;
	
};

interface UC_API IOpenApiService : public IUCService, Event<IOpenApiServiceEvent>, IUCEngineEvent {
	virtual ~IOpenApiService(){};


	/**
	* 获取应用历史聊天内容接口;
	* @param appId              请求应用ID;
	* @param count              请求聊天内容数量，默认20，不能超过200;
	* @param startTime          请求聊天内容的开始时间;
	* @param endTime            请求聊天内容的结束时间;
	* @return					错误码;
	*/
	virtual int32_t GetChatHistory(int64_t appId, int64_t count, int64_t startTime, int64_t endTime) = 0;

	/**
	* 获取应用消息列表接口;
	* @return				    错误码;
	*/
	virtual int32_t GetAppList() = 0;

	/**
	* 获取应用消息信息接口;
	* @param appIds             应用ID列表;
	* @return				    错误码;
	*/
	virtual int32_t GetAppInfo(const std::list<int64_t>& appIds) = 0;

	/**
	* 获取OA消息状态接口;
	* @param infos              OA消息列表;
	* @return				    错误码;
	*/
	virtual int32_t GetOAMessageStatus(const std::list<AppMessageInfo> &infos) = 0;
	
	/**
	*修改OA消息状态接口;
	*@param infos               OA消息状态列表;
	*@return				    错误码;
	*/
	virtual int32_t UpdateOAMessageStatus(const std::list<AppMessageStatusInfo> &infos) = 0;

	/**
	* 获取消息ACK状态接口;
	* @param infos              OA消息列表;
	* @return				    错误码;
	*/
	virtual int32_t GetMessageAckStatus(const std::list<AppMessageInfo> &infos) = 0;

	/**
	* 重发消息;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ResendMessage(int32_t msgId) = 0;
	
	/**
	* 发送文本聊天消息接口;
	* @param toApp 			    接收者ID;
	* @param textTitle		    文本标题;
	* @param textContent		文本内容;
	* @param textType		    详情格式：0x01:文本类型，0x02:URL链接类型;
	* @param detailContent		详情内容;
	* @param [out]msgId		    消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendTextMessage(UCID toApp, const std::string& textTitle, const std::string& textContent, int8_t textType,
		const std::string& detailContent, OUT int32_t& msgId) = 0;

	/**
	* 发送图片聊天消息接口;
	* @param toApp              接收者ID;
	* @param imageId	        图片ID或图片URL
	* @param imageType	        图片来源类型：0x01:FS图片ID，0x02:URL链接类型;
	* @param imageData	        图片缩略图;
	* @param [out]msgId         消息ID;
	* @return			        错误码;
	*/
	virtual int32_t SendImageMessage(UCID toApp, const std::string& imageId, int8_t imageType, const std::string imageData, OUT int32_t& msgId) = 0;

	/**
	* 发送文件消息接口;
	* @param toApp              接收者ID;
	* @param fileId	            文件ID或文件URL
	* @param fileType	        文件来源类型：0x01:FS文件ID，0x02:URL链接类型;
	* @param fileName	        文件名称;
	* @param fileSize	        文件大小;
	* @param [out]msgId         消息ID;
	* @return			        错误码;
	*/
	virtual int32_t SendFileMessage(UCID toApp, const std::string& fileId, int8_t fileType, const std::string fileName,
		int64_t fileSize, OUT int32_t& msgId) = 0;

	/**
	* 发送消息已读接口;
	* @param appUid             接收者ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessageRead(UCID appUid, int32_t msgId, int64_t msgSeq) = 0;

	/**
	* 发送多条消息已读接口;
	* @param toAppId            应用ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessagesRead(int64_t toAppId) = 0;

	/**
	* 发送HttpRequest请求接口;
	* @param requestId          http请求ID;
	* @param reqMethod          http请求方法;
	* @param reqUrl	            http请求Url;
	* @param body	            Post body
	* @param heads	            http请求头类型列表;
	* @return			        错误码;
	*/
	virtual int32_t SendRequest(int32_t requestId, RequestMethod method, const std::string &url, const std::string &body, const std::list<HeadInfo> &heads) = 0;
};

}
#endif /* IOPENAPISERVICE_H_ */
