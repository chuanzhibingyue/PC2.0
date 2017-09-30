/*
* MessageProcessor.h
*
*  Created on: June 9, 2015
*      Author: yongshuai.cheng
*/

#ifndef MESSAGEPROCESSOR_H_
#define MESSAGEPROCESSOR_H_

#include "common/StringConvert.h"
#include "UCMessageType.h"
#include "UCSDK.h"
#include "UCEngine.h"
#include "UCErrorCode.h"
#include "store_service/MessageStore.h"
#define BOOST_SPIRIT_THREADSAFE
#include "boost/property_tree/ptree.hpp"

namespace uc {


class MessageProcessor {
public:
	MessageProcessor(UCEngine *engine);
	virtual ~MessageProcessor();
	
public:
	// Message Request;
	//int32_t CreateResendMessage(MessageInfo messageInfo, OUT HTTPRequest &request);

	int32_t CreateForwardMessage(ConversationType type, UCID fromUid, UCID toUid, MessageStoreItem &msgItem, OUT UCMsg *&msg);

	int32_t CreateTextMessage(ConversationType type, UCID fromUid, UCID toUid, int8_t textType,
		const std::string& textFormat, const std::string& textData, const std::string& textTitle, 
		const std::string& textDetial, int32_t conferenceId, int64_t eventId, int64_t startTime, const std::vector<UCID>& atUsers, OUT UCMsg *&msg);

	int32_t CreateImageMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& imageName,
		int32_t imageSize, const std::string& imageURL, const std::string& imageData, const std::string& imageId, 
		int8_t imageType, int32_t conferenceId, int64_t eventId, int64_t startTime, OUT UCMsg *&msg);

	int32_t CreateAudioMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& audioName,
		int32_t audioSize, const std::string& audioURL, int32_t audioDuration, int32_t conferenceId, int64_t eventId, int64_t startTime,
			OUT UCMsg *&msg);

	int32_t CreateVideoMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& videoName,
			int32_t videoSize, const std::string& videoURL, const std::string& videoData, int32_t videoDuration, 
			int32_t conferenceId, int64_t eventId, int64_t startTime, OUT UCMsg *&msg);
	
	int32_t CreateReportMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& reportTitle,
		const std::string& reportUrl, int64_t eventId, OUT UCMsg *&msg);
	
	int32_t CreateRecordVideoMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& videoName, 
		int32_t videoSize, const std::string& videoURL, const std::string& videoThumb, const std::string& videoThumbURL, int32_t videoDuration, int32_t operatorId,
		const std::string& operatorName, const std::string& operatorAvatar, int32_t conferenceId, int64_t eventId, int64_t startTime, OUT UCMsg *&msg);

	int32_t CreateSendFileMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& fileName,
		int32_t fileSize, const std::string& fileURL, int64_t fileId, const std::string& apiFileId, int64_t apiFileSize, 
		int8_t fileType, int64_t eventId, int64_t conferenceId, int64_t startTime, OUT UCMsg *&msg);

	int32_t CreateSendCloudFileMessage(ConversationType type, CloudFileOperation operation, UCID fromUid, UCID targetUid,
		const std::string& operatorName, const std::string& fileName, const std::string& fileNameTo, int64_t fileSize,
		int8_t count, int8_t isDir, int8_t cloudType, const std::string& detailContent, int64_t createTime,
		int64_t fileId, OUT UCMsg *&msg);

	int32_t CreateShareFileMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& fileName,
		    int32_t fileSize, const std::string& fileURL, int64_t fileId, int64_t eventId, int64_t conferenceId,
			int64_t startTime, UCMsg *&msg);

	int32_t CreateDeleteFileMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& fileName,
			int64_t fileId, OUT UCMsg *&msg);

	int32_t CreateSendCodeMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& creatorName, 
		int64_t codeId, const std::string& codeTitle, const std::string& langType, const std::string& codeHead, const std::string& codeDesc, 
		int32_t codeSize, const std::string& codeUrl, int32_t codeLine, int64_t createTime, UCMsg *&msg);

	int32_t CreateShareCodeMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& creatorName, 
		int64_t codeId, const std::string& codeTitle, const std::string& langType, const std::string& codeHead, const std::string& codeDesc, 
		int32_t codeSize, const std::string& codeUrl, int32_t codeLine, int64_t createTime,  UCMsg *&msg);

	int32_t CreateCallInvitationMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId,
			const std::string& confPwd, int32_t hostId, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime,
			UCID privateId = {-1, -1, -1});

	
	int32_t CreateCallAgreeMentMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId, int32_t hostId, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime);

	int32_t CreateCallRejectMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId, int32_t hostId, const std::string& reason, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime);

	int32_t CreateCallFinishMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId, int32_t hostId, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime);

	int32_t CreateCallCancelMessage(ConversationType type, UCID fromUid, UCID targetUid, const string& phoneNum, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime);

	int32_t CreateNotRespondCallRecordMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& phoneNum, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime);

	int32_t CreateRejectCallRecordMessage(ConversationType type, UCID fromUid, UCID targetUid, const string& phoneNum, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime);

	int32_t CreateConnectedCallRecordMessage(ConversationType type, UCID fromUid, UCID targetUid, int32_t druation, const string& phoneNum, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime);

	int32_t CreateMessageReadMessage(ConversationType type, UCID fromUid, UCID targetUid, int32_t msgId, int64_t msgSeq, OUT HTTPRequest &request);

	int32_t CreateMessagePlayedMessage(ConversationType type, UCID fromUid, UCID targetUid, int32_t msgId, int64_t msgSeq, OUT HTTPRequest &request);

	int32_t CreateAssistInvitationMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId,
		const std::string& confPwd, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime);

	int32_t CreateAssistAcceptMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId,
		OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime);

	int32_t CreateAssistRejectMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId,
		OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime);

	int32_t CreateMessageRequest(UCMsg &msg, HTTPRequest &request);

	int32_t CreateMessageRequest(BinMsg &destMsg, HTTPRequest &request);

	MessageInfo* GetMessageInfo(boost::property_tree::wptree jsonData);

public:
	/**
	 * 根据 appid 和 protocolType 获取ConversationType
	 * @param appId
	 * @param protocolType
	 */
	static ConversationType GetConversationType(int32_t appId, int32_t protocolType);

	/**
	 * 获取RelateId
	 * @param type		会话类型
	 * @param curUid	当前用户id
	 * @param fromUid	发送者id
	 * @param toUid		接收者id
	 * @return
	 */
	static int64_t GetRelateId(ConversationType type, int64_t curUid, int64_t fromUid, int64_t toUid);

	/**
	 * 根据head&body信息生成UCMsg对象
	 * @param head 头信息
	 * @param body base64(thrift(body))后的数据
	 * @param msg UCMsg 对象
	 * @return
	 */
	static int32_t GetUCMsg(UCHead &head, const string &body, UCMsg *&msg);

	/**
	 * 从会议消息内容中获取会议的
	 * eventid, conferenceId, authorizerId
	 * 如果是非会议消息则返回 0
	 * @param msg
	 * @return
	 */
	static void GetIdsFromUCMsg(const UCMsg &msg, int64_t &eventId, int64_t &conferenceId, int64_t &authorizerId);

	/**
	* 获取数据库中此URL对应的文件的下载路径和大小
	* @param fileURL	下载文件URL
	* @param filePath	文件下载路径
	* @param fileSize	下载文件大小
	* @return
	*/
	static int32_t _GetLocalFilePathAndSize(const std::string &fileURL, std::string &filePath, int64_t &fileSize);

	/**
	* 核对数据库中文件与此路径下文件是否相同
	* @param filePath	文件下载路径
	* @param fileSize	下载文件大小
	* @return
	*/
	static int32_t CheckFile(const std::string &filePath, int64_t fileSize);

	/**
	* 去掉URL中&u=****的信息
	* @param fileurl	下载文件的URL
	* @返回去掉&u=****信息后的URL
	*/
	static std::string RetrieveNoUserDownloadURL(const std::string &fileurl);
public:
	int32_t GetMessageFromJSON(boost::property_tree::wptree jsonData, UCMsg *&msg, string &body);

	/**
	 * 将UCMsg对象编码成thrift二进制流和base64(thrift(body))数据
	 * @param msg 	UCMsg对象
	 * @param data 	thrift二进制流数据
	 * @param body	base64（thrift(body))数据
	 * @return 操作是否成功
	 */
	int32_t EncodeMessageData(UCMsg &msg, BinMsg &data, string &body);

	/**
	 * 保存消息内容到数据库
	 * @param head 消息头信息
	 * @param body thrift后的body内容
	 * @param bodyLen body长度
	 * @param state 消息的发送状态
	 */

	int32_t SaveMessage(ConversationType type, int64_t relateId, int32_t flag, int32_t isPlayed, MsgSendState state,
		UCMsg *msg, const string &body);

	void ClearMessageList(std::list<MessageInfo*> msgList);

private:
	int16_t GetCloudFileMessageIdByOperation(CloudFileOperation operation);

private:
	UCEngine *_engine;
};

}
/* namespace uc */
#endif /* MESSAGEPROCESSOR_H_ */
