/*
 * OpenApiService.h
 *
 *  Created on: Sep 9, 2015
 *      Author: yongshuai.cheng
 */

#ifndef OPENAPISERVICE_H_
#define OPENAPISERVICE_H_

#include "IOpenApiService.h"
#include <boost/property_tree/json_parser.hpp>
#include "MessageInfo.h"


namespace uc {

class OpenApiService : public IOpenApiService {

public:
	OpenApiService(IUCEngine *engine);
	virtual ~OpenApiService();
	/**
	* 获取应用历史聊天内容接口;
	* @param userId             请求应用ID;
	* @param count              请求聊天内容数量，默认20，不能超过200;
	* @param startTime          请求聊天内容的开始时间;
	* @param endTime            请求聊天内容的结束时间;
	* @return					错误码;
	*/
	virtual int32_t GetChatHistory(int64_t appId, int64_t count, int64_t startTime, int64_t endTime);

	/**
	* 获取应用消息列表接口;
	* @return				    错误码;
	*/
	virtual int32_t GetAppList();

	/**
	* 获取应用消息信息接口;
	* @param appId              应用ID列表;
	* @return				    错误码;
	*/
	virtual int32_t GetAppInfo(const std::list<int64_t>& appIds);

	/**
	* 获取OA消息状态接口;
	* @param infos              App消息列表;
	* @return				    错误码;
	*/
	virtual int32_t GetOAMessageStatus(const std::list<AppMessageInfo> &infos);

	/**
	*修改OA消息状态接口;
	*@param infos               OA消息状态列表;
	*@return				    错误码;
	*/
	virtual int32_t UpdateOAMessageStatus(const std::list<AppMessageStatusInfo> &infos);

	/**
	* 获取消息ACK状态接口;
	* @param infos              App消息列表;
	* @return				    错误码;
	*/
	virtual int32_t GetMessageAckStatus(const std::list<AppMessageInfo> &infos);

	/**
	* 重发消息;
	* @param msgId              '消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ResendMessage(int32_t msgId);

	/**
	* 发送文本聊天消息接口;
	* @param toApp 			    接收者ID;
	* @param textTitle		    文本标题;
	* @param textContent		文本内容;
	* @param textType		    文本格式：0x01:文本类型，0x02:URL链接类型;
	* @param detailContent		文本详情信息;
	* @param [out]msgId		    消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendTextMessage(UCID toApp, const std::string& textTitle, const std::string& textContent, int8_t textType,
		const std::string& detailContent, OUT int32_t& msgId);

	/**
	* 发送图片聊天消息接口;
	* @param toApp              接收者ID;
	* @param imageId	        图片ID或图片URL
	* @param imageType	        图片来源类型：0x01:FS图片ID，0x02:URL链接类型;
	* @param imageData	        图片缩略图;
	* @param [out]msgId         消息ID;
	* @return			        错误码;
	*/
	virtual int32_t SendImageMessage(UCID toApp, const std::string& imageId, int8_t imageType, const std::string imageData, OUT int32_t& msgId);

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
		int64_t fileSize, OUT int32_t& msgId);

	/**
	* 发送消息已读接口;
	* @param appUid             接收者ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessageRead(UCID appUid, int32_t msgId, int64_t msgSeq);

	/**
	* 发送多条消息已读接口;
	* @param toAppId            应用ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessagesRead(int64_t toAppId);

	/**
	* 发送HttpRequest请求接口;
	* @param requestId          http请求ID;
	* @param reqMethod          http请求方法;
	* @param reqUrl	            http请求Url;
	* @param body	            Post body
	* @param heads	            http请求头类型列表;
	* @return			        错误码;
	*/
	virtual int32_t SendRequest(int32_t requestId, RequestMethod method, const std::string &url, const std::string &body, const std::list<HeadInfo> &heads);

	// override UCEngineEvent OnMessageReceived
	virtual int32_t OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state);

public:
	void ParseAppInfo(const boost::property_tree::wptree &ptParse, AppInfo& info);

protected:
	//UCC
	int32_t OnGetAppListResponse(int32_t errorCode, HTTPResponse* response);
	int32_t OnGetAppInfoResponse(int32_t errorCode, HTTPResponse* response, std::list<int64_t> appIds);
	int32_t OnGetChatHistoryResponse(int32_t errorCode, HTTPResponse* response, int64_t appId, int64_t count, int64_t uistartTime, int64_t uiendTime, int64_t sdkstarttime, int64_t sdkendtime, int32_t update_flag_msgid);
	int32_t OnGetOAMessageStatusResponse(int32_t errorCode, HTTPResponse *response, const std::list<AppMessageInfo> infos);
	int32_t OnUpdateOAMessageStatusResponse(int32_t errorCode, HTTPResponse *response, const std::list<AppMessageStatusInfo> infos);
	int32_t OnGetMessageAckStatusResponse(int32_t errorCode, HTTPResponse *response, const std::list<AppMessageInfo> infos);
	int32_t OnSendMessageResponse(int32_t errorCode, HTTPResponse *response, int64_t appId, int32_t msgId);
	int32_t OnSendRequestResponse(int32_t errorCode, HTTPResponse *response, int32_t requestId);
	int32_t OnSendMessageReadResponse(int32_t errorCode, HTTPResponse *response, int64_t appId, int32_t readType, int32_t msgId, int64_t msgSeq);

	void OnAppStatusChangeNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnAppEventChangeNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnTextMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);
	void OnMediaMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);
	void OnFileMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnAckSentMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnOAMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnCustomizedNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnOAStatusChangedNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnMessageReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnMessageBatchReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);

protected:

	UCID GetCurUser();
	std::string GetCurUserName();
	bool IsCurUser(UCID userUid);

};

} /* namespace uc */

#endif /* OPENAPISERVICE_H_ */
