/*
 * GroupService.h
 *
 *  Created on: Jun 2, 2015
 *      Author: ethan
 */

#ifndef GROUPSERVICE_H_
#define GROUPSERVICE_H_

#include "IGroupService.h"
#include "GroupInfo.h"
#include "boost/thread/recursive_mutex.hpp"
#define BOOST_SPIRIT_THREADSAFE
#include "boost/property_tree/ptree.hpp"
#include "rapidjson/document.h"
#include "QueuedNetWorkManager.h"
#include "base/Timer.h"

#ifdef AUDIO_SERVICE
    #include "audio_service/AudioService.h"
#else
    #define AudioService int
#endif

namespace uc {

#define SYNC_GROUP_NUMBER    1000


typedef std::list<GroupInfo> GroupList;
typedef std::list<DocumentInfo> DocumentList;
typedef std::map<int64_t, DocumentList> DocumentMap;
typedef std::list<CommentInfo> CommentList;

enum SYNC_GROUP_LIST_STATUS { STATUS_NONE = 0 , STATUS_DOING, STATUS_DONE };
struct AtMsgInfo{
	int64_t _sender;
	int64_t _seq;
public:
	AtMsgInfo(){
		_sender = 0;
		_seq = 0;
	}

	AtMsgInfo(int64_t sender, int64_t seq){
		_sender = sender;
		_seq = seq;
	}
};
class GroupService : public IGroupService, public QueuedNetWorkManagerListener{
public:
	GroupService(IUCEngine *engine);
	virtual ~GroupService();

	/**
     * 获取历史聊天内容接口;
	 * @param channel            消息来源;
	 * @param type               聊天类型;
	 * @param groupId            请求目标ID（讨论组ID，组织ID）;
     * @param count              请求聊天内容数量，默认20，不能超过200;
	 * @param startTime          请求聊天内容的开始时间;
	 * @param endTime            请求聊天内容的结束时间;
     * @return					 错误码;
     */
	virtual int32_t GetChatHistory(MessageChannel channel,ConversationType type, int64_t groupId, int64_t count, int64_t startTime, int64_t endTime);

	/**
	* 获取搜索历史聊天内容接口;
	* @param type               聊天类型;
	* @param groupId            请求目标ID（讨论组ID，组织ID）;
	* @param count              请求聊天内容数量，默认20，不能超过200;
	* @param startTime          请求聊天内容的开始时间;
	* @param endTime            请求聊天内容的结束时间;
	* @param reverse            消息请求方向 1：从starttime向后取， 0：从endtime向前取;
	* @return					错误码;
	*/
	virtual int32_t GetSearchChatHistory(ConversationType type, int64_t relateId, int64_t count, int64_t startTime, int64_t endTime, int32_t reverse);

	/**
	* 重发消息;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ResendMessage(int32_t msgId);

	/**
	* 转发消息;
	* @param groupUid  			群ID/项目组ID;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ForwardMessage(UCID groupUid, int32_t msgId);

	/**
	* 撤消消息接口;
	* @param sessionId 用户会话ID;
	* @param userId 用户ID;
	* @param touserId 外部ID;
	* @param conversationType 会话类型;
	* @param seqId 需要撤消的消息ID;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	*/
	virtual int32_t RevocationMessage(int64_t& touserId, ConversationType type, const int64_t& seqId, OUT int32_t& msgId);

	/**
     * 发送聊天文本消息接口;
	 * @param ConversationType   聊天类型;
	 * @param groupUid           群ID/项目组ID;
     * @param textType           文本类型：纯文本，MIME格式;
	 * @param textFormat		 文本格式;
	 * @param textData           文本内容;
	 * @param atUsers			 @用户集合
	 * @param [out]msgId         消息ID;
     * @return                   错误码;
     */
	virtual int32_t SendTextMessage(ConversationType type, UCID groupUid, int8_t textType, const std::string& textFormat, const std::string& textData, const std::vector<UCID>& atUsers, int32_t& msgId);

	/**
	 * 发送聊天图片消息接口;
	 * @param ConversationType   聊天类型;
	 * @param groupUid           群ID/项目组ID;
	 * @param imageName	         图片名称;
	 * @param imageSize	         图片大小;
	 * @param imageURL	         图片下载路径;
	 * @param imageData	         图片缩略图;
	 * @param [out]msgId         消息ID;
	 * @return			         错误码;
     */
	virtual int32_t SendImageMessage(ConversationType type, UCID groupUid, const std::string& imageName, int32_t imageSize,
	 const std::string& imageURL, const std::string& imageData, int32_t& msgId);

	/**
	 * 发送聊天音频消息接口;
	 * @param ConversationType   聊天类型;
	 * @param groupUid           群ID/项目组ID;
	 * @param audioName          音频名称;
	 * @param audioSize          音频大小;
	 * @param audioURL           音频下载路径;
	 * @param audioDuration      音频时长;
	 * @param [out]msgId         消息ID;
	 * @return                   错误码;
      */
	virtual int32_t SendAudioMessage(ConversationType type, UCID groupUid, const std::string& audioName, int32_t audioSize,
	 const std::string& audioURL, int32_t audioDuration, int32_t& msgId);

	/**
	 * 发送聊天音频消息接口;
	 * @param ConversationType   聊天类型;
	 * @param groupUid           群ID/项目组ID;
	 * @param videoName          视频名称;
	 * @param videoSize          视频大小;
	 * @param videoURL           视频下载路径;
	 * @param videoData          视频缩略图;
	 * @param videoDuration      视频时长;
	 * @param [out]msgId         消息ID;
	 * @return                   错误码;
     */
	virtual int32_t SendVideoMessage(ConversationType type, UCID groupUid, const std::string& videoName, int32_t videoSize,
	 const std::string& videoURL, const std::string& videoData, int32_t videoDuration, int32_t& msgId);

	/**
	* 发送聊天报告消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param reportTitle		报告标题;
	* @param reportUrl          报告URL;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendReportMessage(ConversationType type, UCID groupUid, const std::string& reportTitle, const std::string& reportUrl, OUT int32_t& msgId);

	virtual int32_t SendCloudFileMessage(ConversationType type, CloudFileOperation operation, UCID groupUid, const std::string& fileName, 
		const std::string& fileNameTo, int64_t fileSize, int8_t count, int8_t isDir, int8_t cloudType, const std::string& detailContent, bool isSave, OUT int32_t& msgId);

	/**
	* 发送会议录像信息;
	* @param ConversationType    聊天类型;
	* @param groupUid            群ID/项目组ID;
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
	virtual int32_t SendRecordVideoMessage(ConversationType type, UCID groupUid, const std::string& videoName, int32_t videoSize, const std::string& videoURL, 
		const std::string& videoThumb, const std::string& videoThumbURL, int32_t videoDuration, int32_t operatorId, const std::string& operatorName, const std::string& operatorAvatar, int32_t& msgId);

	/**
	* 发送音频通话邀请消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param confId             会议ID;
	* @param confPwd            会议密码;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendInviteCallMessage(ConversationType type, UCID groupUid, const std::string& confId,
		const std::string& confPwd, int32_t hostId, int32_t& msgId, UCID privateId);

	virtual int32_t SendInviteCallMessage(int32_t callType, ConversationType type, UCID groupUid, const std::string& confId,
		const std::string& confPwd, int32_t hostId, int32_t& msgId, UCID privateId, AudioService* service);

	/**
	* 发送同意音频通话消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendAcceptCallMessage(ConversationType type, UCID groupUid, const std::string& confId, int32_t hostId, int32_t& msgId);

	/**
	* 发送拒绝音频通话消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendRejectCallMessage(ConversationType type, UCID groupUid, const std::string& confId, int32_t hostId, const std::string& reason, int32_t& msgId);

	/**
	* 发送结束音频通话消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendFinishCallMessage(ConversationType type, UCID groupUid, const std::string& confId, int32_t hostId, const std::string& reason, int32_t& msgId);

	/**
	* 发送取消音频通话消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendCancelCallMessage(ConversationType type, UCID groupUid, const string& phoneNum,  int32_t& msgId);

	/**
	* 发送未接通音频通话记录消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	//virtual int32_t SendNotRespondCallRecordMessage(ConversationType type, UCID groupUid, int32_t& msgId);

	/**
	* 发送拒绝音频通话记录消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	//virtual int32_t SendRejectCallRecordMessage(ConversationType type, UCID groupUid, int32_t& msgId);

	/**
	* 发送接通音频通话记录消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param druation           时长;
	* @param phoneNum           电话号码;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendConnectedCallRecordMessage(ConversationType type, UCID groupUid, int32_t druation, const std::string& phoneNum, int32_t& msgId);

   /**
	* 发送消息已读接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessageRead(ConversationType type, UCID groupUid, int32_t msgId, int64_t msgSeq);

   /**
	* 发送多条消息已读接口;
	* @param ConversationType   聊天类型;
	* @param groupId            群ID/项目组ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessagesRead(ConversationType type, int64_t groupId);

   /**
	* 发送消息已播放接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessagePlayed(ConversationType type, UCID groupUid, int32_t msgId, int64_t msgSeq);

	/**
	* 获取消息未读用户列表接口;
	* @param type               聊天类型;
	* @param groupId            群ID/项目组ID;
	* @param seq                服务器消息ID列表;
	* @return					错误码;
	*/
	virtual int32_t GetUnreadUserList(ConversationType type, int64_t groupId, std::list<int64_t> seq);

	/**
	 * 获取讨论组列表;
	 * @param groupType          请求讨论组的类型;
	 * @param page               请求讨论组的页数，默认1;
	 * @param count              请求讨论组的数量，默认200;
	 * @param lastGroupTime      上一次请求讨论组列表时间，默认0;
	 * @param allData		     是否需要所有讨论组信息：ture 需要; false: 不需要;
	 * @param update			是否是service自己更新： true 不需要抛回调,不需要cache
	 * @return					 错误码;
	 */
	virtual int32_t GetGroupList(GroupStyle groupType, int32_t page = 1, int32_t count = 200, int32_t lastGroupTime = 0, bool allData = true, bool update = false);

	/**
	 * 获取讨论组信息;
	 * @param groupId            讨论组ID;
	 * @param groupInfo          讨论组信息;
	 * @param forced				 是否强制去服务器重新获取（当用户被踢出group，又重新被邀请， 此时内存中的数据并不真实，需要强制去服务器获取）
	 * @return					 错误码;
	 *       返回UC_LOCALDATA_SUCCESSED表示groupInfo可用
	 *       返回其他错误，OnGroupListReceived返回正确的groupInfo;
	 */
	virtual int32_t GetGroupInfo(int64_t groupId, GroupInfo &groupInfo, bool forced = false, int32_t mountId = 0);
	
	/**
	 * 创建讨论组;
	 * @param groupType          讨论组类型;
	 * @param groupName          讨论组名称;
	 * @param membreInfos        讨论组人员信息;
	 * @param nameFlag           是否为正式名称:0:否/1:是;
	 * @param memberType         讨论组成员上限类型:0:小讨论组/1:大讨论组;
	 * @param mountId            如果是云盘讨论组，则为云盘id，否则忽略;
	 * @return                   错误码;
	 */
	virtual int32_t CreateGroup(GroupStyle groupType, const std::string& groupName, const std::string& logourl, 
		std::vector<UserInfo>& userInfo, int32_t nameFlag, int32_t memberType, int32_t mountId);
	/**
	* 创建呼讨论组;
	* @param groupType          讨论组类型;
	* @param groupName          讨论组名称;
	* @param userInfo			讨论组人员信息;
	* @param service			service回调;
	* @param isNeedCreate       是否需要创建call
	* @return                   错误码;
	*/
	virtual int32_t CreateCallGroup(GroupStyle groupType, const std::string& groupName,
		std::vector<UserInfo>& userInfo, AudioService* service);

	virtual int32_t CreateCallGroup(GroupStyle groupType, const std::string& groupName,
		std::vector<UserInfo>& userInfo, AudioService* service, int conferenceId, int callType);

	virtual int32_t AddMemberToGroupCall(int64_t groupId, const std::vector<UserInfo>& memberList,
		int conferenceId, int callType, AudioService* service);

	/**
	 * 结束讨论组;
	 * @param groupId            讨论组ID;
	 * @return                   错误;
	 */
	virtual int32_t EndGroup(int64_t groupId);

	/**
	 * 退出讨论组;
	 * @param groupId            讨论组ID;
	 * @return                   错误;
	 */
	virtual int32_t ExitGroup(int64_t groupId);

	/**
	 * 添加讨论组成员;
	 * @param groupId            讨论组ID;
	 * @param memberList         待添加讨论组用户;
	 * @return					 错误码;
	 */
	virtual int32_t AddMember(int64_t groupId, const std::vector<UserInfo>& memberList);

	virtual int32_t SetAdmin(int64_t groupId, SetAdminType type, const std::vector<int32_t>& adminList);
	
	/**
	 * 删除讨论组成员;
	 * @param groupId            讨论组ID;
	 * @param memberList         待删除讨论组成员;
	 * @return                   错误码;
	 */
	virtual int32_t RemoveMember(int64_t groupId, const std::vector<int32_t>& memberList);

	/**
	 * 更新讨论组名称;
	 * @param groupId            讨论组ID;
	 * @param groupName          讨论组名称;
	 * @return                   错误码;
	 */
	virtual int32_t SetGroupName(int64_t groupId, const std::string& groupName);

	/**
	 * 获取讨论组文档列表;
	 * @param groupId            讨论组ID;
	 * @param page               请求讨论组文档的页数，默认1;
	 * @param count              请求讨论组文档的数量，默认20;
	 * @return			         错误码;
	 */
	virtual int32_t GetDocumentList(int64_t groupId, int32_t page = 1, int32_t count = 20);

	/**
	* 上传讨论组文档;
	* @param groupId            群ID/项目组ID;
	* @param operation          0: create 1: share;
	* @param info               DocumentInfo;
	* @param isSendMessage      是否发送云盘上传消息; 0–不发送; 1–发送
	* @return                   错误码;
	*/
	virtual int32_t UploadDocument(int64_t groupId, int32_t operation, int32_t isSendMessage, const DocumentInfo& info, int32_t& msgId);

	/**
	* 批量上传讨论组文档;
	* @param groupId            群ID/项目组ID;
	* @param operation          0: create 1: share;
	* @param info               DocumentInfo;
	* @return                   错误码;
	*/
	virtual int32_t BatchUploadDocument(int64_t groupId, int32_t operation, const DocumentInfo &docs);

	/**
	* 分享讨论组代码片段;
	* @param groupId            群ID/项目组ID;
	* @param operation          0: create 1: share;
	* @param info               DocumentInfo;
	* @return                   错误码;
	*/
	virtual int32_t UploadCodeSnippet(int64_t groupId, int32_t operation, const DocumentInfo& info, int32_t& msgId);

	/**
	 * 删除讨论组文档;
	 * @param groupId            讨论组ID;
	 * @param fileId             文档ID;
	 * @return			         错误码;
	 */
	virtual int32_t DeleteDocument(int64_t groupId, int64_t fileId);

	/**
	 * 获取讨论组文档信息;
	 * @param groupId            讨论组ID;
	 * @param fileId             文档ID;
	 * @return			         错误码;
	 */
	virtual int32_t GetDocumentInfo(int64_t groupId, int64_t fileId, const std::string &cloudFileInfo);

	/**
	 * 设置讨论组头像;
	 * @param groupId            讨论组ID;
	 * @param logoURL            logo路径;
	 * @return			         错误码;
	 */
	virtual int32_t SetGroupLogo(int64_t groupId, const std::string& logoURL);

	/**
	 * 更新讨论组头像;
	 * @param groupId            讨论组ID;
	 * @param logoURL            本地logo路径;
	 * @return			         错误码;
	 */
	virtual int32_t UpdateGroupLogo(int64_t groupId, const std::string& localPath);

	/**
	 * 设置讨论组聊天置顶;
	 * @param groupId            讨论组ID;
	 * @param state              是否设置讨论组置顶:0:取消置顶/1:设置置顶;
	 * @return                   错误码;
	 */
	virtual int32_t SetTopChat(int64_t groupId, int32_t state);


	/**
	 * 设置讨论组免打扰;
	 * @param groupId            讨论组ID;
	 * @param state              是否设置讨论组面打扰:0:取消打扰/1:设置打扰;
	 * @return                   错误码;
	 */
	virtual int32_t SetDoNotDisturb(int64_t groupId, int32_t state);

	/**
	 * 设置讨论组提醒;
	 * @param groupId            讨论组ID;
	 * @param state              是否设置讨论组提醒:0:取消提醒/1:设置提醒;
	 * @param alertRule          讨论组提醒规则;
	 * @param alertTime          讨论组提醒时间;
	 * @return                   错误码;
	 */
	virtual int32_t SetReminder(int64_t groupId, int32_t state, int32_t alertRule, int32_t alertTime);

	/**
    * 获取评论列表;
	* @param groupId            讨论组ID;
    * @param fileId             文档或代码ID;
    * @param count              获取结果的数量,默认为20;
    * @param startTime          开始时间;
	* @param endTime            开始时间;
	* @return					错误码;
	*/
	virtual int32_t GetCommentList(int64_t groupId, int64_t fileId, int32_t count, int64_t startTime, int64_t endTime);

	/**
    * 发表评论;
	* @param groupId            讨论组ID;
    * @param fileId             文档或代码ID;
    * @param fileType           文档类型：0:文档/1:代码;
    * @param fileTitle          文档或代码标题;
	* @param content            评论内容;
	* @return					错误码;
	*/
	virtual int32_t CreateComment(int64_t groupId, int64_t fileId, int32_t fileType, const std::string& fileTitle, 
		const std::string content, int32_t& msgId);

	/**
	* 获取@消息列表;
	* @param groupId            组id
	* @param page				从0开始计数的页数,默认为0;
	* @param count              获取结果的数量,默认为100;
	* @return					错误码;
	*/
	virtual int32_t GetAtMessageList(int64_t groupId, int32_t page = 0, int32_t count = 100);

	/**
	* 批量删除@消息;
	* @param groupId			组id
	* @param seqList			消息的seq列表
	* @return					错误码;
	*/
	virtual int32_t DeleteAtMessage(ConversationType type, int64_t groupId, const std::vector<int64_t>& seqList);

	virtual int32_t OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state);

	virtual void OnLoginSucceeded();

	virtual void OnUCASConnected(int32_t errorCode);
	
	virtual void OnSendMessageResponse(const std::vector<MessageResponse> &messageResponses);
public:
//	int64_t GetLastGroupSyncTime();
	void ParseGroupInfo(const rapidjson::Value &data, GroupInfo& info, bool isSearch = false);
	int32_t GetUnreadUserCount(ConversationType type, int64_t groupId, std::list<int64_t> seq);
	bool GetGroupInfoByExternalId(const string& externalId, GroupInfo& info);

	
protected:

	DocumentList* GetDocumentListById(int64_t groupId);
	DocumentInfo* GetDocumentInfoById(int64_t groupId, int64_t fileId);
	void AddDocument(int64_t groupId, DocumentInfo &document);
	void AddDocumentList(int64_t groupId, DocumentList& documents);
	void RemoveDocumentListById(int64_t groupId);
	void RemoveDocumentInfoById(int64_t groupId, int64_t fileId);

	UCID GetCurUser();
	bool IsCurUser(UCID userUid);
	std::string GetFileFormat(std::string fileName);
	std::string GetCurUserName();
	void GetDocumentInfo(int32_t type, boost::property_tree::wptree jsonData, DocumentInfo& docInfo);
	void GetCommentInfo(boost::property_tree::wptree jsonData, CommentInfo& commentInfo);
	void SetGroupCreateRequest(GroupStyle groupType, const std::string& groupName, const std::string& logourl,
		std::vector<UserInfo>& userInfo, int32_t nameFlag, int32_t memberType, int32_t mountId, HTTPRequest& request);
	int32_t GetGroupCreatedResponse(int32_t errorCode, HTTPResponse *response, GroupInfo& groupInfo);
	int32_t SetAddMemberRequest(int64_t groupId, const std::vector<UserInfo>& memberList, HTTPRequest& request);
	int32_t GetAddMemberResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId);

	int32_t GetGroupMemberCount(int64_t groupId);
	UserIdList GetGroupMemberList(int64_t groupId);

	int32_t GetRevocationConversationType(ConversationType type);
	void CacheAtMsg(int64_t groupId, AtMsgInfo info);
	void CacheAtMsgList(int64_t groupId, std::map<int64_t, int64_t> infos);
	void DeleteAtMsgFromCache(int64_t groupId, const std::vector<int64_t>& seqList);
	bool GetAtMsgFromCache(int64_t groupId, std::vector<AtMsgInfo>& info);

protected:
	//UCC
	int32_t OnGetChatHistoryResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, MessageChannel channel, ConversationType type, 
		int64_t count, int64_t uistartTime, int64_t uiendTime, int64_t sdkstarttime, int64_t sdkendtime, int32_t update_flag_msgid);
	int32_t OnGeSearchChatHistoryResponse(int32_t errorCode, HTTPResponse *response, int64_t relateId, ConversationType type, int64_t count, int64_t uistartTime, int64_t uiendTime);
	int32_t OnSendMessageResponse(int32_t errorCode, HTTPResponse *response, ConversationType type, int64_t groupId, int32_t msgId, bool saveUnread);
	int32_t OnSendInviteCallMessageResponse(int32_t errorCode, HTTPResponse *response, int32_t conferenceId, int32_t callType, ConversationType type, int64_t groupId, int32_t msgId, bool saveUnread, AudioService* service);
	int32_t OnSendMessageReadResponse(int32_t errorCode, HTTPResponse *response, ConversationType type, int64_t groupId, int32_t readType, int32_t msgId, int64_t msgSeq);
	int32_t OnSendMessagePlayedResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId,	int32_t msgId, int64_t msgSeq);
	int32_t OnGetUnreadUserCountResponse(int32_t errorCode, HTTPResponse *response, ConversationType type, int64_t groupId, std::list<int64_t> seq);
	int32_t OnGetUnreadUserListResponse(int32_t errorCode, HTTPResponse *response, ConversationType type, int64_t groupId, std::list<int64_t> seq);
	int32_t OnCreateGroupResponse(int32_t errorCode, HTTPResponse *response, int32_t mountId);
	int32_t OnCreateCallGroupResponse(int32_t errorCode, HTTPResponse *response, AudioService* service);
	int32_t OnCreateCallGroupResponse(int32_t errorCode, HTTPResponse *response, AudioService* service, int conferenceId, int callType);
	int32_t OnAddMemberToGroupCallResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, std::vector<UserInfo> member, int conferenceId, int callType, AudioService* service);
	int32_t OnExitGroupResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId);
	int32_t OnEndGroupResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId);
	int32_t OnAddMemberResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, std::vector<UserInfo> member);
	int32_t OnSetAdminResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, SetAdminType type, std::vector<int32_t> adminList);
	int32_t OnRemoveMemberResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, std::vector<int32_t> memberList);
	int32_t OnSetGroupNameResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId);
	int32_t OnSetGroupLogoResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, std::string logoUrl);
	int32_t OnSetTotChatResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t state);
	int32_t OnSetDisturbResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t state);
	int32_t OnSetReminderResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t state,
		int32_t alertRule, int32_t alertTime);
	int32_t OnGetDocListResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId);
	int32_t OnGetDocumentInfoResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int64_t fileId);
	int32_t OnUploadDocumentResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t content_type, int32_t operation, int32_t msgId, std::string fileName);
	int32_t OnBatchUploadDocumentResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId);
	int32_t OnDeleteDocumentResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int64_t fileId);
	int32_t OnGetGroupInfoResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t mountId);
	int32_t OnGetGroupListResponse(int32_t errorCode, HTTPResponse *response, GroupStyle groupType, bool update, bool allData);
	int32_t OnGetCommentListResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int64_t fileId);
	int32_t OnCreateCommentResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int64_t fileId, std::string content, int32_t msgId);
	int32_t OnGetAtMessageListResponse(int32_t errorCode, HTTPResponse* response, int64_t groupId);
	int32_t OnDeleteAtMessageResponse(int32_t errorCode, HTTPResponse* response, ConversationType type, int64_t groupId, std::vector<int64_t> seqList);
	int32_t OnRevocationMessageResponse(int32_t errorCode, HTTPResponse *response, int64_t to_userId, ConversationType type, int64_t seqId, int32_t msgId);

	//UCAS
	void OnTextMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread);
	void OnMediaMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread);
	void OnReportMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnStatusInfoUpdate(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnCreateGroupNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnCloseGroupNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnAddMemberNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnExitGroupNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);
	void OnKickMemberNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnUpdateNameNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);
	void OnUpdateAvatarNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnSetDisturbNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);
	void OnSetTopNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnSetReminderNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnUploadDocumentNotify(MessageChannel channel, int32_t operation, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread);
	void OnUploadCodeSnippetNotify(MessageChannel channel, int32_t operation, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread);
	void OnDeleteDocumentNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnCreateCommentNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread);

	void OnMessageReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnMessageBatchReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnMessagePlayedNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnAtMessageReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnInviteCallNotify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnAcceptCallNotify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnRejectCallNofify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);
	void OnFinishCallNotify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnConnectedCallNotify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave);
	void OnCancelCallNotify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnCloudFileModifiedNotify(MsgType msgType, MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnMemberRoleUpdateNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	void OnMessageReadUserNotify(ConversationType type, int64_t groupId, int64_t msgSeq);
	void OnRevocationNotify(MessageChannel channel,UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave);
	
private:
	int32_t StopService();
	void OnBatchUploadDocumentFired();
	void BatchUpoadDocument(int64_t groupId, const std::string& data);
	void ConvertDocumentToJson(int64_t groupId, int32_t operation, const DocumentInfo& info, bool isBatch,
		boost::property_tree::wptree& tree);
	bool IsUCCloudDiscussionCloudFileMsg(MsgType msgType);
	CloudFileOperation GetCloudFileOperation(MsgType msgType);

private:
	DocumentMap					_documentMap;
	boost::recursive_mutex		_documentMutex;


	struct list_group_call_cache_item{
		GroupStyle _type;
		int32_t _page;
		int32_t _count;
		int32_t _last_time;
		bool _all_data;
	};

	boost::recursive_mutex		_grouplist_status_mutex;
	SYNC_GROUP_LIST_STATUS _sync_status;
	std::list<list_group_call_cache_item> _list_group_call_cache;

	struct DocumentUploadOperation{
		DocumentInfo info;
		int32_t operation;
	};
	std::map<int64_t, std::list<DocumentUploadOperation>> _batchUploadOpts;
	boost::recursive_mutex	_batchUploadOptsMutex;
	timer_id _batchUploadTimerId;

	std::map<int64_t, std::vector<AtMsgInfo>> _atMsgCache;
};

} /* namespace uc */

#endif /* GROUPSERVICE_H_ */
