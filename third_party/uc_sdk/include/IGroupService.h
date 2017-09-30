/*
 * IGroupService.h
 *
 *  Created on: June 2, 2015
 *      Author: yongshuai.cheng
 */

#ifndef IGROUPSERVICE_H_
#define IGROUPSERVICE_H_

#include "UCSDK.h" 
#include "common/event.h"
#include "IUCEngine.h"
#include "UCMessage.h"
#include "GroupInfo.h"
#include "DocumentInfo.h"
#include "MessageInfo.h"
#include "MessageReadInfo.h"


namespace uc {

struct UserInfo{
	UserInfo() : role(GR_Member){
	}

	UCID      ucid;
	std::string userName;
	GroupRole role;
};

enum GroupStyle{
	ALL_GROUP_TYPE = -1,
	HIDE_GROUP_TYPE = 0,
	PROJECT_GROUP_TYPE = 1,
	MUTILCHAT_GROUP_TYPE = 2,
	CLOUD_GROUP_TYPE = 3, 
	GROUP_TYPE = 4    //PROJECT_GROUP_TYPE、MUTILCHAT_GROUP_TYPE合集
};

enum SetAdminType { 
	SAT_NOT_ADMIN = 0, 
	SAT_ADMIN = 1 
};

interface UC_API IGroupServiceEvent {
	virtual ~IGroupServiceEvent(){};

	/**
	* 聊天历史列表;
	* @param result  	     错误信息;
	* @param channel         消息来源;
	* @param type			 会话类型,讨论组/项目
	* @param groupId	     项目组ID/群ID;
	* @param msgCount		 message count;
	* @param min_timestamp   min timestamp;
	*/
	virtual void OnChatHistoryReceived(int32_t result, MessageChannel channel, ConversationType type, int64_t groupId, int32_t msgCount, int64_t min_timestamp) = 0;

	/**
	* 聊天历史列表;
	* @param result  	     错误信息;
	* @param type			 会话类型: 单人/讨论组/项目/云盘/会议;
	* @param relateId	     用户ID/项目组ID/群ID;
	* @param msgCount		 message count;
	* @param min_timestamp   min timestamp;
	* @param max_timestamp   max timestamp;
	*/
	virtual void OnSearchChatHistoryReceived(int32_t result, ConversationType type, int64_t relateId, int32_t msgCount, int64_t min_timestamp, int64_t max_timestamp) = 0;


	/**
	 * 消息发送结果;
	 * @param result	     错误信息;
	 * @param type			 会话类型,讨论组/项目
	 * @param groupId	     项目组ID/群ID;
	 * @param msgId          本地消息ID;
	 * @param msgSeq         服务器消息ID;
	 * @param msgTime        服务器消息发送时间;
	 */
	virtual void OnMessageSent(int32_t result, ConversationType type, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 消息撤回发送结果;
	* @param result	     错误信息;
	* @param groupId	 项目组ID/群ID;
	* @param type		 会话类型
	* @param msgId       本地消息ID;
	* @param msgSeq      服务器消息ID;
	* @param msgTime     服务器消息发送时间;
	*/
	virtual void OnRevocationMessage(int32_t result, int64_t groupId, ConversationType type, int64_t msgSeq, int32_t msgId, int64_t msgTime) = 0;

	/**
	* 接收文本聊天消息响应事件;
	* @param channel 消息来源
	* @param message 文本消息对象;
	*/
	virtual void OnTextMessageReceived(MessageChannel channel, const TextMessageInfo *message) = 0;

	/**
	* 接收图片消息响应事件;
	* @param channel 消息来源
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
	* 接收云盘文件修改事件;
	* @param channel 消息来源
	* @param operation 修改类型
	* @param message 文件消息对象;
	*/
	virtual void OnCloudFileModifiedMessageReceived(MessageChannel channel, CloudFileOperation operation, const FileMessageInfo &message) = 0;

	/**
	* 接收会议视频录制消息响应事件;
	* @param channel 消息来源
	* @param message 视频消息对象;
	*/
	virtual void OnRecordVideoReceived(MessageChannel channel, const MediaMessageInfo *message) = 0;

	/**
	 * 接收到group创建的消息
	 * @param channel 消息来源
	 * @param message 消息属性
	 * @param group group属性
	 */
	virtual void OnGroupCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const GroupInfo& group) = 0;

	/**
	 * 接收到group结束的消息
	 * @param channel 消息来源
	 * @param message 消息属性
	 * @param operatorId 操作者id
	 */
	virtual void OnGroupCloseMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId) = 0;

	/**
	 * 接受到成员退出讨论组的消息
	 * @param channel	消息来源
	 * @param message	消息属性
	 * @param operatorId 操作者
	 * @param groupId 讨论组id
	 * @param memberId 成员id
	 * @param avatar 头像信息
	 * @param groupName 讨论组名称
	 * @param namePinYin 讨论组拼音名
	 * @param nameFlag
	 */
	virtual void OnGroupMemberDelMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			int32_t memberId, const std::string &avatar, const std::string &groupName, const std::string &namePinYin, int8_t nameFlag) = 0;

	/**
	 * 接收到group添加成员的消息
	 * @param channel 消息来源
	 * @param message 消息属性
	 * @param operatorId 操作者id
	 * @param groupId 讨论组id
	 * @param memberList 成员列表
	 * @param avatar 讨论组头像
	 * @param groupName 讨论组名称
	 * @param namePinYin 拼音名称
	 * @param nameFlag
	 */
	virtual void OnGroupMemberAddMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
		const std::vector<UserInfo> &memberList, const std::string &avatar, const std::string &groupName,
			const std::string &namePinYin, int8_t nameFlag) = 0;

	/**
	* 接收到消息撤回消息
	* @param channel 消息来源
	* @param message 消息属性
	* @param seqId   要撤回的消息id
	* @param type    被撤回消息类型
	*/
	virtual void OnRevocationMesssageReceived(MessageChannel channel, const MessageInfo &message, int64_t seqId, int16_t type) = 0;

	/**
	 * 接收到讨论组踢人的消息
	 * @param channel 消息来源
	 * @param message 消息属性
	 * @param operatorId 操作者id
	 * @param groupId 讨论组id
	 * @param memberList 成员列表
	 * @param avatar 讨论组头像
	 * @param groupName 讨论组名称
	 * @param namePinYin 拼音名称
	 * @param nameFlag
	 */
	virtual void OnGroupMemberKickMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			const std::vector<int32_t> &memberList, const std::string &avatar, const std::string &groupName, const std::string &namePinYin,
			int8_t nameFlag) = 0;

	/**
	 * 接收到group名称更新的消息
	 * @param channel 消息来源
	 * @param message
	 * @param operatorId
	 * @param groupId
	 * @param groupName
	 * @param namePinYin
	 * @param nameFlag
	 */
	virtual void OnGroupNameUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			const std::string &groupName, const std::string &namePinYin, int8_t nameFlag) = 0;

	/**
	 * 接收到group头像更新的消息
	 * @param channel 消息来源
	 * @param message 消息属性
	 * @param operatorId 操作者id
	 * @param avatar 头像url
	 * @param groupId 讨论组id
	 */
	virtual void OnGroupAvatarUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
			const std::string &avatar, int32_t groupId) = 0;

	/**
	* 接收到group头像更新的消息
	* @param channel 消息来源
	* @param message 消息属性
	* @param groupId 讨论组id
	* @param status 讨论组状态
	* @param statusInfo 讨论组状态信息

	*/
	virtual void OnGroupStatusInfoUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId, 
		int32_t status, const std::string &statusInfo) = 0;

	/**
	 * 收到group是否免打扰的消息
	 * @param channel 消息来源
	 * @param message 消息属性
	 * @param groupId 讨论组id
	 * @param state 是否免打扰
	 */
	virtual void OnGroupDisturbSetMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId, int8_t state ) = 0;

	/**
	 * 收到group置顶的消息
	 * @param channel 消息来源
	 * @param message 消息属性
	 * @param operatorId 操作者id
	 * @param groupId 讨论组id
	 * @param isTop 是否置顶
	 */
	virtual void OnGroupTopMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			int8_t isTop) = 0;

	/**
	 * 接收到讨论组提醒规则更新的消息
	 * @param channel
	 * @param message
	 * @param groupId
	 * @param state
	 * @param rule
	 * @param time
	 */
	virtual void OnGroupReminderSetMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId,
			int8_t state, int32_t rule, int32_t time) = 0;

	/**
	 * 接收到文件上传的消息
	 * @param channel
	 * @param operation 0: create 1: share
	 * @param message
	 * @param doc
	 */
	virtual void OnDocumentUploadMessageReceived(MessageChannel channel, int32_t operation, const MessageInfo &message, const DocumentInfo &doc) = 0;

	/**
	* 云盘文件消息事件;
	* @param channel 消息来源
	* @param message 文件消息对象;
	*/
	virtual void OnCloudFileCreateMessageReceived(MessageChannel channel, const FileMessageInfo &message) = 0;

	/**
	 * 接收到文档删除的消息
	 * @param channel
	 * @param message
	 * @param operatorId
	 * @param contentId
	 * @param docName
	 */
	virtual void OnDocumentDeleteMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
			int64_t contentId, const std::string &docName) = 0;

	/**
	 * 接收到code上传的消息
	 * @param channel
	 * @param operation 0: create 1: share
	 * @param message
	 * @param doc
	 */
	virtual void OnCodeSnippetUploadMessageReceived(MessageChannel channel, int32_t operation, const MessageInfo &message, const DocumentInfo &doc) = 0;

	/**
	 * 接收到评论创建的消息
	 * @param channel
	 * @param message
	 * @param operatorId
	 * @param contentId
	 * @param docName
	 */
	virtual void OnCommentCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const CommentInfo& commentInfo) = 0;

    /**
	* 接收消息已读消息;
	* @param result  		 错误信息;
	* @param type			 会话类型,讨论组/项目
	* @param groupId         讨论组ID;
	* @param readType        消息已读类型:0 单条已读/1 批量已读;
	* @param msgId           本地消息ID（单条已读时有效）;
	* @param msgSeq          服务器消息ID（单条已读时有效）;
	*/
	virtual void OnMessageRead(int32_t result, ConversationType type, int64_t groupId, int32_t readType, int32_t msgId, int64_t msgSeq) = 0;

	/**
	* 接收消息已播放消息;
	* @param result  		 错误信息;
	* @param groupId         讨论组ID;
	* @param msgId           本地消息ID;
	* @param msgSeq          服务器消息ID;
	*/
	virtual void OnMessagePlayed(int32_t result, int64_t groupId, int32_t msgId, int64_t msgSeq) = 0;

	/**
	* 接收消息未读用户个数消息;
	* @param result  		  错误信息;
	* @param ConversationType 聊天类型;
	* @param groupId          讨论组ID;
	* @param info             消息未读信息列表;
	*/
	virtual void OnMessageUnreadUserCount(int32_t result, ConversationType type, int64_t userId, const std::list<MessageReadInfo>& info) = 0;
	
	/**
	* 接收消息未读用户列表消息;
	* @param result  		  错误信息;
	* @param ConversationType 聊天类型;
	* @param groupId          讨论组ID;
	* @param info             消息未读用户信息列表;
	*/
	virtual void OnMessageUnreadUserList(int32_t result, ConversationType type, int64_t userId, const std::list<MessageReadUserInfo>& info) = 0;

	/**
	* 音频通话邀请响应事件;
	* @param result 	     	错误信息;
	* @param ConversationType   聊天类型;
	* @param groupId         	讨论组ID;
	* @param msgId   		 	本地消息ID;
	* @param msgSeq          	服务器消息ID;
	* @param confId          	会议ID;
	* @param confPwd         	会议密码;
	* @param hostId          	主持人ID;
	*/
	virtual void OnCallInvited(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime,
		const std::string confId, const std::string confPwd, int32_t hostId) = 0;

	/**
	* 音频通话接受响应事件;
	* @param result 	     	错误信息;
	* @param ConversationType	聊天类型;
	* @param groupId         	讨论组ID;
	* @param confId          	会议ID;
	*/
	virtual void OnCallAccepted(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId, const std::string confId, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 音频通话拒绝响应事件;
	* @param result 	     	错误信息;
	* @param ConversationType   聊天类型;
	* @param groupId         	讨论组ID;
	* @param confId          	会议ID;
	*/
	virtual void OnCallRejected(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId,
		const std::string confId, int32_t hostId, std::string reason, 
		int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 音频通话结束响应事件;
	* @param result 	     	错误信息;
	* @param ConversationType   聊天类型;
	* @param groupId         	讨论组ID;
	* @param confId          	会议ID;
	*/
	virtual void OnCallStopped(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId, const std::string confId,
		int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 音频通话接通时长响应事件;
	* @param result 	     	错误信息;
	* @param ConversationType   聊天类型;
	* @param groupId         	讨论组ID;
	* @param duration        	通话时间;
	*/
	virtual void OnCallConnected(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId, int32_t duration, const std::string& phoneNum,
		int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	virtual void OnCallCanceled(MessageChannel channel, int32_t result, ConversationType type, int32_t userId, int64_t groupId,
		int32_t msgId, int64_t msgSeq, int64_t msgTime, const std::string& phoneNum) = 0;

	/**
	* 创建讨论组回调函数;
	* @param result  		 错误信息;
	* @param operatorId		 操作者ID;
	* @param info            讨论组信息;
	*/ 
	virtual void OnGroupCreated(int32_t result, int32_t operatorId, const GroupInfo& info) = 0;

	/**
	* 退出讨论组回调函数;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param userId  		 退出成员的ID;
	*/
	virtual void OnGroupExited(int32_t result, int64_t groupId, int32_t userId) = 0;

	/**
	* 结束讨论组回调函数;
	* @param result  		 错误信息;
	* @param groupId         讨论组ID;
	* @param operatorId      操作者ID;
	*/
	virtual void OnGroupEnded(int32_t result, int64_t groupId, int32_t operatorId) = 0;

	/**
	* 添加讨论组成员回调函数;
	* @param result  		 错误信息;
	* @param groupId         讨论组ID;
	* @param operatorId      操作者ID;
	* @param memberList      新增讨论组成员;
	*/
	virtual void OnMemberAdded(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<UserInfo>& memberList) = 0;

	/**
	* 设置讨论组管理员的回调函数;
	* @param result  		 错误信息;
	* @param groupId         讨论组ID;
	* @param type			 类型;
	* @param adminList       新增讨论组成员;
	*/
	virtual void OnAdminSet(int32_t result, int64_t groupId, SetAdminType type, const std::vector<int32_t>& adminList) = 0;

	/**
	* 移除讨论组成员回调函数;
	* @param result  		 错误信息;
	* @param groupId         讨论组ID;
	* @param operatorId      操作者ID;
	* @param memberList      移除讨论组成员; 
	*/
	virtual void OnMemberRemoved(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<int32_t>& memberList) = 0;

	/**
	* 讨论组名称更新回调函数;
	* @param result  		 错误信息;
	* @param groupId         讨论组ID;
	* @param operatorId      操作者ID;
	* @param groupName       新讨论组名称;
	* @param pinYinName      新讨论组拼音;
	*/
	virtual void OnGroupNameUpdated(int32_t result, int64_t groupId, int32_t operatorId, 
		const std::string& groupName, const std::string& pinYinName) = 0;

	/**
	* 讨论组Logo更新回调函数;
	* @param result  		 错误信息;
	* @param groupId         讨论组ID;
	* @param logoURL         logo路径;
	*/
	virtual void OnGroupLogoUpdated(int32_t result, int64_t groupId, const std::string& logoURL) = 0;

	/**
	* 讨论组免打扰设置回调函数;
	* @param result  		 错误信息;
	* @param operatorId      操作者ID;
	* @param isDisturb       是否免打扰;
	*/
	virtual void OnDisturbSet(int32_t result, int64_t groupId, int32_t isDisturb) = 0;

	/**
	* 讨论组置顶显示回调函数;
	* @param result  		 错误信息;
	* @param operatorId      操作者ID;
	* @param isDisturb       是否置顶;
	*/
	virtual void OnTopChatSet(int32_t result, int64_t groupId, int32_t isTop) = 0;

	/**
	* 讨论组设置提醒回调函数;
	* @param result  		 错误信息;
	* @param operatorId      操作者ID;
	* @param isAlert         是否提醒;
	* @param remindRule      提醒规则;
	* @param remindTime      提醒时间;
	*/
	virtual void OnReminderSet(int32_t result, int64_t groupId, int32_t isAlert, int32_t remindRule, int32_t remindTime) = 0;

	/**
	* 讨论组列表回调函数;
	* @param result  		 错误信息;
	* @param groupType 		 请求的讨论组类型;
	* @param allData		 是否需要所有讨论组信息：ture 需要; false: 不需要;
	* @param groupList		 讨论组列表;
	*/
	virtual void OnGroupListReceived(int32_t result, GroupStyle groupType, bool allData, const std::list<GroupInfo>& groupList) = 0;

	/**
	* 讨论组信息回调函数;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param info  		     讨论组信息;
	*/
	virtual void OnGroupInfoReceived(int32_t result, int64_t groupId, int32_t mountId, const GroupInfo& info) = 0;

	/**
	* 讨论组文档列表回调函数;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param info  		     讨论组文档列表;
	*/
	virtual void OnDocListReceived(int32_t result, int64_t groupId, const std::list<DocumentInfo>& info) = 0;

	/**
	* 获取@消息列表回调函数;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param atMsg  		 @消息<sender,seq>列表;
	*/
	virtual void OnAtMessageListReceived(int32_t result, int64_t groupId, const std::map<int64_t, int64_t>& atMsg) = 0;


	/**
	* 删除@消息列表回调函数;;
	* @param result  		 错误信息;
	* @param type  		     会话类型;
	* @param groupId		 讨论组ID;
	* @param seqList		 消息seq数组;
	*/
	virtual void OnAtMessageDeleted(int32_t result, ConversationType type, int64_t groupId, const std::vector<int64_t>& seqList) = 0;


	/**
	* 讨论组文档信息回调函数;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param vcInfo  		 讨论组文档列表;
	*/
	virtual void OnDocInfoReceived(int32_t result, int64_t groupId, const DocumentInfo& info) = 0;

	/**
	* 上传讨论组文档响应事件;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param operatorId		 操作者ID;
	* @param operation		 0: create 1: share;
	* @param DocumentInfo	 文档信息;
	*/
	virtual void OnDocumentUploaded(int32_t result, int64_t groupId, int32_t operatorId, int32_t operation, const DocumentInfo& info) = 0;

	/**
	* 上传讨论组代码响应事件;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param operatorId		 操作者ID;
	* @param operation		 0: create 1: share;
	* @param DocumentInfo	 文档信息;
	*/
	virtual void OnCodeSnippetUploaded(int32_t result, int64_t groupId, int32_t operatorId, int32_t operation, const DocumentInfo& info) = 0;

	/**
	* 删除讨论组文档响应事件sefdD;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param operatorId		 操作者ID;
	* @param docName         文档名称;
	* @param fileId          文档ID;
	*/
	virtual void OnDocumentDeleted(int32_t result, int64_t groupId, int32_t operatorId, 
		const std::string& docName, int64_t fileId) = 0;

	/**
	* 评论列表回调函数;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param fileId  		 文档或代码ID;
	* @param info  		     评论信息列表;
	*/
	virtual void OnCommentListReceived(int32_t result, int64_t groupId, int64_t fileId, const std::list<CommentInfo>& info) = 0;

   /**
	* 创建评论回调函数;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param info  		     评论信息;
	*/
	virtual void OnCommentCreated(int32_t result, int64_t groupId, const CommentInfo& info) = 0;

	/**
	* 组成员角色变更回调函数;
	* @param channel		 消息来源;
	* @param info			 角色变更消息;
	*/
	virtual void OnMemberRoleUpdated(MessageChannel channel, const RoleUpdatedMessageInfo &info) = 0;

};

interface UC_API IGroupService: public IUCService, Event<IGroupServiceEvent>, IUCEngineEvent {
	virtual ~IGroupService(){};

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
	virtual int32_t GetChatHistory(MessageChannel channel, ConversationType type, int64_t groupId, int64_t count, int64_t startTime, int64_t endTime) = 0;

	/**
	* 获取搜索历史聊天内容接口;
	* @param type               聊天类型;
	* @param groupId            请求目标ID（讨论组ID，组织ID）;
	* @param count              请求聊天内容数量，默认20，不能超过200;
	* @param startTime          请求聊天内容的开始时间;
	* @param endTime            请求聊天内容的结束时间;
	* @param reverse            消息请求方向 1：从starttime向后取， 0：从endtime向前取;
	* @return					 错误码;
	*/
	virtual int32_t GetSearchChatHistory(ConversationType type, int64_t relateId, int64_t count, int64_t startTime, int64_t endTime, int32_t reverse) = 0;

   /**
	* 重发消息;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ResendMessage(int32_t msgId) = 0;

	/**
	* 转发消息;
	* @param groupUid  			群ID/项目组ID;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ForwardMessage(UCID groupUid, int32_t msgId) = 0;

	/**
	* 消息撤回接口;
	* @param touserId 外部ID;
	* @param conversationType 会话类型;
	* @param seqId 需要撤消的消息ID;
	* @param callback 操作结束后的回调函数。 callback[0]-错误码, callback[1]-msgId
	*/
	virtual int32_t RevocationMessage(int64_t& touserId, ConversationType type, const int64_t& seqId, OUT int32_t& msgId) = 0;

	/**
     * 发送聊天文本消息接口;
	 * @param ConversationType   聊天类型;
	 * @param groupUid           群ID/项目组ID;
     * @param textType           文本类型：0x01:纯文本，0x02:MIME格式;
	 * @param textFormat		 文本格式;
	 * @param textData           文本内容;
	 * @param atUsers			 @用户集合
	 * @param [out]msgId         消息ID;
     * @return                   错误码;
     */
	virtual int32_t SendTextMessage(ConversationType type, UCID groupUid, int8_t textType, const std::string& textFormat, const std::string& textData, const std::vector<UCID>& atUsers, OUT int32_t& msgId) = 0;

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
	 const std::string& imageURL, const std::string& imageData, OUT int32_t& msgId) = 0;

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
	 const std::string& audioURL, int32_t audioDuration, OUT int32_t& msgId) = 0;

	/**
	 * 发送聊天视频消息接口;
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
	 const std::string& videoURL, const std::string& videoData, int32_t videoDuration, OUT int32_t& msgId) = 0;

	/**
	* 发送聊天报告消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param reportTitle		报告标题;
	* @param reportUrl          报告URL;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendReportMessage(ConversationType type, UCID groupUid, const std::string& reportTitle, const std::string& reportUrl, OUT int32_t& msgId) = 0;

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
		const std::string& videoThumb, const std::string& videoThumbURL, int32_t videoDuration, int32_t operatorId, const std::string& operatorName, const std::string& operatorAvatar, int32_t& msgId) = 0;

	/**
	* 发送云盘文件消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param fileName			文件/文件夹名称
	* @param fileSize			文件大小（单位：字节）
	* @param cloudType			云盘类型（1-够快云库, 2-安贞母婴中心）
	* @param detailContent      内容
	* @param createTime         operateTime（uccserver操作时间,单位：秒）
	* @param fileId				fileId
	* @param isSave				是否将这条消息保存到DB
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendCloudFileMessage(ConversationType type, CloudFileOperation operation, UCID groupUid, const std::string& fileName, 
		const std::string& fileNameTo, int64_t fileSize, int8_t count, int8_t isDir, int8_t cloudType, const std::string& detailContent, 
		bool isSave, OUT int32_t& msgId) = 0;

	/**
	* 发送音频通话邀请信息;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param confId             会议ID;
	* @param confPwd            会议密码;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendInviteCallMessage(ConversationType type, UCID groupUid, const std::string& confId,
		const std::string& confPwd, int32_t hostId, OUT int32_t& msgId, UCID privateId = { -1, -1, -1 }) = 0;

	/**
	* 发送同意音频通话消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendAcceptCallMessage(ConversationType type, UCID groupUid, const std::string& confId, int32_t hostId, OUT int32_t& msgId) = 0;

	/**
	* 发送拒绝音频通话消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendRejectCallMessage(ConversationType type, UCID groupUid, const std::string& confId, int32_t hostId, const std::string& reason, OUT int32_t& msgId) = 0;

	/**
	* 发送结束音频通话消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param confId             会议ID;
	* @param hostId             主持人ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendFinishCallMessage(ConversationType type, UCID groupUid, const std::string& confId, int32_t hostId, const std::string& reason, OUT int32_t& msgId) = 0;

	/**	
	* 发送取消音频通话;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendCancelCallMessage(ConversationType type, UCID groupUid, const string& phoneNum, OUT int32_t& msgId) = 0;

	/**
	* 发送未接通音频通话记录消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	//virtual int32_t SendNotRespondCallRecordMessage(ConversationType type, UCID groupUid, OUT int32_t& msgId) = 0;

	/**
	* 发送拒绝音频通话记录消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	//virtual int32_t SendRejectCallRecordMessage(ConversationType type, UCID groupUid, OUT int32_t& msgId) = 0;

	/**
	* 发送接通音频通话记录消息接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param druation           通话时长;
	* @param phoneNum           电话号码;
	* @param [out]msgId         消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendConnectedCallRecordMessage(ConversationType type, UCID groupUid, int32_t druation, const std::string& phoneNum, OUT int32_t& msgId) = 0;

    /**
	* 发送消息已读接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessageRead(ConversationType type, UCID groupUid, int32_t msgId, int64_t msgSeq) = 0;

    /**
	* 发送多条消息已读接口;
	* @param ConversationType   聊天类型;
	* @param groupId            群ID/项目组ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessagesRead(ConversationType type, int64_t groupId) = 0;
    /**
	* 发送消息已播放接口;
	* @param ConversationType   聊天类型;
	* @param groupUid           群ID/项目组ID;
	* @param msgId              本地消息ID;
	* @param msgSeq             服务器消息ID;
	* @return                   错误码;
	*/
	virtual int32_t SendMessagePlayed(ConversationType type, UCID groupUid, int32_t msgId, int64_t msgSeq) = 0;

	/**
	* 获取消息未读用户列表接口;
	* @param type               聊天类型;
	* @param groupId            群ID/项目组ID;
	* @param seq                服务器消息ID列表;
	* @return					错误码;
	*/
	virtual int32_t GetUnreadUserList(ConversationType type, int64_t groupId, std::list<int64_t> seq) = 0;

	/**
	 * 获取讨论组列表;
	 * @param groupType          请求讨论组的类型;
	 * @param page               请求讨论组的页数，默认1;
	 * @param count              请求讨论组的数量，默认1000;
	 * @param lastGroupTime      上一次请求讨论组列表时间，默认0;
	 * @param allData		     是否需要所有讨论组信息：true 需要; false: 不需要;
	 * @param update			是否是service自己更新： true 不需要抛回调,不需要cache
	 * @return					 错误码;
	 */
	virtual int32_t GetGroupList(GroupStyle groupType, int32_t page = 0, int32_t count = 1000, int32_t lastGroupTime = 0, bool allData = true, bool update = false) = 0;

	/**
	 * 获取讨论组信息;
	 * @param groupId            讨论组ID;
	 * @param groupInfo          讨论组信息;
	 * @return					 错误码;
	 *       返回UC_LOCALDATA_SUCCESSED表示groupInfo可用
	 *       返回其他错误，OnGroupListReceived返回正确的groupInfo;
	 */
	virtual int32_t GetGroupInfo(int64_t groupId, OUT GroupInfo &groupInfo, bool forced = false, int32_t mountId = 0) = 0;

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
	virtual int32_t CreateGroup(GroupStyle groupType, const std::string& groupName, const std::string& logourl, std::vector<UserInfo>& userInfo,
		int32_t nameFlag, int32_t memberType, int32_t mountId) = 0;

	/**
	 * 结束讨论组;
	 * @param groupId            讨论组ID;
	 * @return                   错误码;
	 */
	virtual int32_t EndGroup(int64_t groupId) = 0;

	/**
	 * 退出讨论组;
	 * @param groupId            讨论组ID;
	 * @return                   错误;
	 */
	virtual int32_t ExitGroup(int64_t groupId) = 0;

	/**
	 * 添加讨论组成员;
	 * @param groupId            讨论组ID;
	 * @param memberList         待添加讨论组用户;
	 * @return					 错误码;
	 */
	virtual int32_t AddMember(int64_t groupId, const std::vector<UserInfo>& memberList) = 0;

	/**
	* 设置和取消管理员角色;
	* @param groupId            讨论组ID;
	* @param type				类型;
	* @param adminList          待设置的讨论组用户;
	* @return					错误码;
	*/
	virtual int32_t SetAdmin(int64_t groupId, SetAdminType type, const std::vector<int32_t>& adminList) = 0;

	/**
	 * 删除讨论组成员;
	 * @param groupId            讨论组ID;
	 * @param memberList         待删除讨论组成员;
	 * @return                   错误码;
	 */
	virtual int32_t RemoveMember(int64_t groupId, const std::vector<int32_t>& memberList) = 0;

	/**
	 * 更新讨论组名称;
	 * @param groupId            讨论组ID;
	 * @param groupName          讨论组名称;
	 * @return                   错误码;
	 */
	virtual int32_t SetGroupName(int64_t groupId, const std::string& groupName) = 0;

	/**
	 * 获取讨论组文档列表;
	 * @param groupId            讨论组ID;
	 * @param page               请求讨论组文档的页数，默认1;
	 * @param count              请求讨论组文档的数量，默认20;
	 * @return			         错误码;
	 */
	virtual int32_t GetDocumentList(int64_t groupId, int32_t page = 1, int32_t count = 20) = 0;

	/**
	* 上传讨论组文档;
	* @param groupId            群ID/项目组ID;
	* @param operation          0: create 1: share;
	* @param info               DocumentInfo;
	* @param isSendMessage      是否发送云盘上传消息; 0–不发送; 1–发送
	* @param msgId				[out] message id;
	* @return                   错误码;
	*/
	virtual int32_t UploadDocument(int64_t groupId, int32_t operation, int32_t isSendMessage, const DocumentInfo& info, int32_t& msgId) = 0;

	/**
	* 批量上传讨论组文档;
	* @param groupId            群ID/项目组ID;
	* @param operation          0: create 1: share;
	* @param info               DocumentInfo;
	* @return                   错误码;
	*/
	virtual int32_t BatchUploadDocument(int64_t groupId, int32_t operation, const DocumentInfo &docs) = 0;
	/**
	* 分享讨论组代码片段;
	* @param groupId            群ID/项目组ID;
	* @param operation          0: create 1: share;
	* @param info               DocumentInfo;
	* @param msgId				[out] message id;
	* @return                   错误码;
	*/
	virtual int32_t UploadCodeSnippet(int64_t groupId, int32_t operation, const DocumentInfo& info, int32_t& msgId) = 0;

	/**
	 * 删除讨论组文档;
	 * @param groupId            讨论组ID;
	 * @param fileId             文件ID;
	 * @return			         错误码;
	 */
	virtual int32_t DeleteDocument(int64_t groupId, int64_t fileId) = 0;

	/**
	 * 获取讨论组文档信息;
	 * @param groupId            讨论组ID;
	 * @param fileId             文件ID;
	 * @return			         错误码;
	 */
	virtual int32_t GetDocumentInfo(int64_t groupId, int64_t fileId, const std::string &cloudFileInfo) = 0;
	
	/**
	* 上传讨论组文档;
	* @param ChatType           聊天类型;
	* @param groupId            群ID/项目组ID;
	* @param fileName           文件名称;
	* @param flieSize           文件大小;
	* @param fileURL            文件下载路径;
	* @param uploadTime         文档上传时间;
	* @return                   错误码;
	*/
	//virtual int32_t UploadCode(int64_t groupId, const std::string& fileName, int32_t flieSize,
	//	const std::string& fileURL, int64_t uploadTime) = 0;

 	/**
 	* 分享讨论组文档;
	* @param ChatType           聊天类型;
	* @param groupId            群ID/项目组ID;
 	* @param fileName           文件名称;
 	* @param flieSize           文件大小;
 	* @param fileURL            文件下载路径;
 	* @param uploadTime         文档上传时间;
 	* @return                   错误码;
 	*/
	//virtual int32_t ShareCode(int64_t groupId, const std::string& fileName, int32_t flieSize,
	//	const std::string& fileURL, int64_t uploadTime) = 0;

	/**
	 * 删除讨论组文档;
	 * @param groupId            讨论组ID;
	 * @param fileId             文件ID;
	 * @return			         错误码;
	 */
	//virtual int32_t DeleteCode(int64_t groupId, int64_t fileId) = 0;

	/**
	 * 设置讨论组头像;
	 * @param groupId            讨论组ID;
	 * @param logoURL            logo路径;
	 * @return			         错误码;
	 */
	virtual int32_t SetGroupLogo(int64_t groupId, const std::string& logoURL) = 0;

	/**
	 * 更新讨论组头像;
	 * @param groupId            讨论组ID;
	 * @param logoURL            本地logo路径;
	 * @return			         错误码;
	 */
	virtual int32_t UpdateGroupLogo(int64_t groupId, const std::string& localPath) = 0;

	/**
	 * 设置讨论组聊天置顶;
	 * @param groupId            讨论组ID;
	 * @param state              是否设置讨论组置顶:0:取消置顶/1:设置置顶;
	 * @return                   错误码;
	 */
	virtual int32_t SetTopChat(int64_t groupId, int32_t state) = 0;

	/**
	 * 设置讨论组免打扰;
	 * @param groupId            讨论组ID;
	 * @param state               是否设置讨论组面打扰:0:取消打扰/1:设置打扰;
	 * @return                   错误码;
	 */
	virtual int32_t SetDoNotDisturb(int64_t groupId, int32_t state) = 0;

	/**
	 * 设置讨论组提醒;
	 * @param groupId            讨论组ID;
	 * @param state              是否设置讨论组提醒:0:取消提醒/1:设置提醒;
	 * @param alertRule          讨论组提醒规则;
	 * @param alertTime          讨论组提醒时间;
	 * @return                   错误码;
	 */
	virtual int32_t SetReminder(int64_t groupId, int32_t state, int32_t alertRule, int32_t alertTime) = 0;

   /**
    * 获取评论列表;
	* @param groupId            讨论组ID;
    * @param fileId             文档或代码ID;
    * @param count              获取结果的数量,默认为20;
    * @param startTime          开始时间;
	* @param endTime            开始时间;
	* @return					错误码;
	*/
	virtual int32_t GetCommentList(int64_t groupId, int64_t fileId, int32_t count, int64_t startTime, int64_t endTime) = 0;

   /**
    * 发表评论;
	* @param groupId            讨论组ID;
    * @param fileId             文档或代码ID;
    * @param fileType           文档类型：0:文档/1:代码;
    * @param fileTitle          文档或代码标题;
	* @param content            评论内容;
	* @param msgId          本地消息ID;
	* @return					错误码;
	*/
	virtual int32_t CreateComment(int64_t groupId, int64_t fileId, int32_t fileType, const std::string& fileTitle, 
		const std::string content, int32_t& msgId) = 0;

	/**
	* 获取@消息列表;
	* @param groupId            组id
	* @param page				从0开始计数的页数,默认为0;
	* @param count              获取结果的数量,默认为100;
	* @return					错误码;
	*/
	virtual int32_t GetAtMessageList(int64_t groupId, int32_t page = 0, int32_t count = 100) = 0;

	/**
	* 批量删除@消息;
	* @param groupId			组id
	* @param seqList			消息的seq列表
	* @return					错误码;
	*/
	virtual int32_t DeleteAtMessage(ConversationType type, int64_t groupId, const std::vector<int64_t>& seqList) = 0;

};

}

#endif /* IGROUPSERVICE_H_ */
