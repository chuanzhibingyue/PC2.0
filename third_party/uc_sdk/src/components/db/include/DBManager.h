///////////////////////////////////////////////////////////
//  DBManager.h
//  Implementation of the Class DBManager
//  Created on:      22-五月-2015 10:34:22
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(DBMANAGER_H_)
#define DBMANAGER_H_

#include "SQLConfig.h"
#include "AccountInfo.h"
#include "CalendarData.h"
#include "contact_service/ContactInfoImpl.h"
#include "MessageInfo.h"
#include "ExternalInfo.h"
#include "GroupInfo.h"
#include "ConversationInfo.h"
#include "ContactTag.h"
#include "DBManagerDef.h"
#include "store_service/StoreService.h"
#include "client_service/ProxyInfo.h"
#include "AC_PbxCallRecord.h"

#include <list>
#include <map>

namespace uc {

using namespace std;

class DBManager {
public:
	virtual ~DBManager();
	
	/**
	 * 获取DBManager单例对象
	 * @return DBManager对象
	 */
	static DBManager& GetInstance();

	/**
	 * 初始化公共数据库
	 * @param dbPath 公共数据库路径
	 * @return 成功：0 失败：非0
	 */
	int InitCommonDB(const string& dbPath);

	/**
	 * 初始化用户数据库
	 * @param dbPath 用户数据库路径
	 * @return 成功：0 失败：非0
	 */
	int InitUserDB(const string& dbPath);

	/**
	* 初始化外部数据库
	* @param dbPath 用户数据库路径
	* @return 成功：0 失败：非0
	*/
	int InitExternalDB(const string& dbPath);

#ifdef PBX_SERVICE
	/**
	* 初始化PBX数据库
	* @param dbPath 用户数据库路径
	* @return 成功：0 失败：非0
	*/
	int InitPbxDB(const std::string& dbPath, const std::string &strVer);
#endif // PBX_SERVICE
	//////////////////////历史账户///////////////////////
	/**
	 * 保存用户的账户信息
	 * @param account 账户数据
	 * @return 成功：0 失败：非0
	 */
	int SaveAccount(const AccountInfo& account);

	/**
	 * 批量保存用户的账户信息
	 * @param accountList 账户数据列表
	 * @return 成功：0 失败：非0
	 */
	int SaveAccount(const list<AccountInfo>& accountList);

	/**
	 * 获取用户的账户信息
	 * @param userAccount 用户账户
	 * @param accountInfo 账户数据
	 * @return 成功：0 失败：非0
	 */
	int GetAccount(const string& userAccount, AccountInfo& accountInfo);

	/**
	 * 获取用户的账户信息
	 * @param accountList 历史账户列表(调用方释放)
	 * @return 成功：0 失败：非0
	 */
	int GetAccountList(list<AccountInfo>& accountList);
	
	/**
	 * 删除用户账户信息
	 * @param userAccount 用户账户
	 * @return 成功：0 失败：非0
	 */
	int DeleteAccount(const string& userAccount);

	/**
	 * 设置自动登录
	 * @param userAccount 用户账户
	 * @param autoLogin 是否登录
	 * @return 成功：0 失败：非0
	 */
	int SetAutoLogin(const std::string& userAccount, bool autoLogin);

	/**
	 * 设置忘记密码
	 * @param userAccount 用户账户
	 * @param remember 是否登录
	 * @return 成功：0 失败：非0
	 */
	int RememberPassword(const string& userAccount, bool remember);

	/**
	 * 更新账号的本地头像路径
	 * @param userAccount 用户账号
	 * @param localAvatar 头像路径
	 */
	int UpdateAccountAvatar(const std::string &userAccount, const std::string &localAvatar);

	//////////////////////Proxy//////////////////////
	int SaveProxyAuthInfo(const ProxyInfo& proxyInfo);

	int GetAuthInfo(ProxyInfo& proxyInfo);

	int DeleteAuthInfo(const std::string& proxy_url);

	//////////////////////消息///////////////////////
	/**
	 * 初始化消息id
	 * @return
	 */
	void InitMessageID();

	/**
	 * 生成消息id
	 * @return 消息id
	 */
	int32_t GetMessageID();

	/**
	 * 保存消息数据
	 * @param messageList 待保存的消息数据
	 * @return 成功：0 失败：非0
	 */
	int SaveMessage(MessageStoreItem& message);

	/**
	 * 批量保存消息数据
	 * @param messageList 待保存的消息数据
	 * @return 成功：0 失败：非0
	 */
	int SaveMessage(list<MessageStoreItem*>& messageList);

	/**
	 * 批量保存消息数据
	 * @param messageList 待保存的消息数据
	 * @return 成功：0 失败：非0
	 */
	int SyncMsg(list<MessageStoreItem*>& messageList);

	/**
	 * 根据messageid获取消息
	 * @param messageId 消息id
	 * @param data 获取到的消息
	 * @return 成功：0 失败：非0
	 */
	int GetMessage(int messageId, MessageStoreItem& data);

	/**
	 * 根据type/relatedId/seq获取消息
	 * @return 成功：0 失败：非0
	 */
	int GetMessage(int32_t conversation_type, int64_t relate_id, int64_t seq, MessageStoreItem& message);

	/**
	 * 分页获取消息
	 * @param conversationType 会话类型
	 * @param relateId 会话关联id
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @param count 每页多少条
	 * @param message 获取到消息列表
	 * @return 成功：0 失败：非0
	 */
	int GetMessage(int32_t conversationType, int64_t relateId, int64_t startTime, int64_t endTime, int count, list<MessageStoreItem*>& message);

	/**
	 * 设置消息状态
	 * @param messageId 消息id
	 * @param state 消息状态
	 * @return 成功：0 失败：非0
	 */
	int SetMessageStatusById(int messageId, int state);

	/**
	 * 设置消息播放状态
	 * @param messageSeq 消息seq
	 * @param state 播放状态
	 * @return 成功：0 失败：非0
	 */
	int SetMessageStatusBySeq(int64_t messageSeq, int state);

	/**
	 * 设置消息seq
	 * @param msgId 消息id
	 * @param time 消息时间
	 * @param seq 消息seq
	 * @return 成功：0 失败：非0
	 */
	int SetMessageSeq(int msgId, int64_t time, int64_t seq);

	/**
	 * 获取消息最大的message seq
	 * @param conversation_type 会话类型
	 * @param relate_id 相关会话ID
	 * @param max_timestamp 最大seq
	 * @return 成功：0 失败：非0
	 */
	int GetMaxMessageSeq(int32_t conversation_type, int64_t relate_id, int64_t& max_seq);

	/**
	 * 根据消息id更新消息内容
	 * @state  消息状态
	 * @timeStamp 消息时间
	 * @seq 消息的seq值
	 * @conversation 消息的conversation值
	 */
	int UpdateMessageInfo(int msgId, MsgSendState state, int64_t timeStamp, int64_t seq, int32_t conversation);

	/**
	 * 根据消息id更新消息flag
	 * @param msgId 消息id
	 * @return 成功：0 失败：非0
	 */
	int UpdateMessageFlag(int msgId);

	/**
	 * 删除消息
	 * @param msgId 待删除的消息id
	 * @return 成功：0 失败：非0
	 */
	int DeleteMessage(int msgId);

	/////////////////////////会话///////////////////////////

	/**
	 * 从当前数据库中获取最新conversation列表
	 * @param number 请求的数量
	 * @param conversations 返回的实际数量有可能小于请求的number数量
	 * @return 成功: 0 失败: 非0
	 */
	int LoadLatestConversations(int32_t number, std::list<ConversationInfo*> &conversations);

	/**
	 * Save conversation list in DB
	 * @param conversations conversation list data
	 * @return true: 0 false: 非0
	 */
	int SaveConversations(std::list<ConversationInfo*>& conversations);

	/**
	* Delete conversation
	*/
	int DeleteConversation(ConversationType type, int64_t relateId);

	/**
	* Get conversation remove time, NULL == 0;
	*/
	int GetDeleteTime(ConversationType type, int64_t relateId, int64_t & removeTime);


	int32_t SaveConversationInfo(int32_t type, int64_t msgSeq, int64_t msgTime);

	/**
	 * 从表中获取last_msgseq的最大值
	 * @param msgSeq
	 * @return 成功：0 失败：非0
	 */
	int GetMaxMessageSeqData(int64_t &msgSeq);

	/**
	 * 根据指定的type和relateId从数据库中删除
	 * @param conversationId
	 * @return 成功：0 失败：非0
	 */
	int RemoveConversation(int32_t type, int64_t relateId);



	/**
	 * 清空所有的conversation对象
	 * @return 成功：0 失败：非0
	 */
	int RemoveAllConversations();

	//////////////////////联系人///////////////////////
	/**
	 * 保存联系人信息
	 * @param contact 联系人数据
	 * @return 成功：0 失败：非0
	 */
	int SaveContact(const ContactInfoImpl& data);


	/**
	 * 批量保存联系人信息
	 * @param contactList 联系人数据列表
	 * @return 成功：0 失败：非0
	 */
	int SaveContact(const list<ContactInfoImpl*>& contactList);

	/**
	 * 获取联系人信息
	 * @param userId 指定的用户id
	 * @param data   获取到的联系人数据
	 * @return 成功：0 失败：非0
	 */
	int GetContact(int userId, ContactInfoImpl& data);

	/**
	* 获取联系人信息
	* @param account 指定的用户账号
	* @param data   获取到的联系人数据
	* @return 成功：0 失败：非0
	*/
	int GetContact(const string& account, ContactInfoImpl& data);

	/**
	 * 按照类型获取联系人列表信息
	 * @param type 指定的用户类型
	 * @param data   获取到的联系人列表信息(调用方释放)
	 * @return 成功：0 失败：非0
	 */
	int GetContactListByType(int type, list<ContactInfoImpl*>& data);

	/**
	 * 按照name模糊查询获取联系人列表信息
	 * @param keyWord 指定的用户name关键字
	 * @param type 指定的用户类型
	 * @param contactList 获取到的联系人列表信息(调用方释放)
	 * @return 成功：0 失败：非0
	 */
	int GetContactListByName(const string& keyWord, int type, list<ContactInfoImpl*>& contactList);

	/**
	 * 按照用户id列表获取联系人列表信息
	 * @param userIDList 指定的用户列表
	 * @param contactList 获取到的联系人列表信息(调用方释放)
	 * @return 成功：0 失败：非0
	 */
	int GetContactListByUserID(const list<int>& userIDList, list<ContactInfoImpl*>& contactList);

	/**
	 * 根据指定的用户id更新联系人信息
	 * @param userId 指定的用户id
	 * @param data 联系人信息
	 * @return 成功：0 失败：非0
	 */
	int UpdateContact(int userId, const ContactInfoImpl& data);

	/**
	 * 更新指定userId用户的profile更新时间
	 * @param userId 指定的用户id
	 * @param timestamp 信息更新时间
	 * @return 成功：0 失败：非0
	 */
	int UpdateLastUpdateTime(int userId, int64_t timestamp);

	/**
	 * 更新指定类型用户的profile更新时间
	 * @param type 联系人类型
	 * @param timestamp 信息更新时间
	 * @return 成功：0 失败：非0
	 */
	int UpdateLastUpdateTimeByTypes(int type, int64_t timestamp);

	/**
	 * 更新指定用户的类型
	 * @param userId 指定的用户id
	 * @param type 联系人类型
	 * @return 成功：0 失败：非0
	 */
	int UpdateUserType(int userId, int type);

	/**
	* 获取指定联系人本地头像
	* @param userId 联系人id
	* @return 联系人本地头像
	*/
	//string GetContactLocalAvatar(int userId);

	////////////////////////conference//////////////////////////
	/**
	 * 保存会议信息
	 * @param data 会议数据
	 * @return 成功：0 失败：非0
	 */
	int SaveMeeting(const Meeting& data);

	/**
	 * 保存会议信息
	 * @param data 批量会议数据
	 * @return 成功：0 失败：非0
	 */
	int SaveMeetings(const vector<Meeting>& data);

	/**
	* 更新会议信息
	* @param data 会议数据
	* @param eventId 原事件id
	* @param oriStart 原开始时间
	* @param authorizerId 原授权人ID
	* @return 成功：0 失败：非0
	*/
	int UpdateMeeting(const Meeting& data, int64_t eventId, int64_t oriStart = 0, int32_t authorizerId = 0);

	/**
	* 更新参会人信息
	* @param attendees 参会人数据
	* @param eventId 事件id
	* @param startTime 开始时间
	* @param authorizerId 授权人ID
	* @return 成功：0 失败：非0
	*/
	int UpdateAttendees(const std::vector<Attendee>& attendees, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);

	/**
	* 增加参会人
	* @param attendees 参会人数据
	* @param eventId 事件id
	* @param startTime 开始时间
	* @param authorizerId 授权人ID
	* @return 成功：0 失败：非0
	*/
	int AddAttendees(const std::vector<Attendee>& attendees, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);

	/**
	* 更新会议状态
	* @param status 会议状态：0 - 正常， 1 - 已取消，2 - 已拒绝
	* @param eventId 原事件id
	* @param startTime 原开始时间
	* @param authorizerId 原授权人ID
	* @return 成功：0 失败：非0
	*/
	int UpdateMeetingStatus(uc::MeetingStatus status, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);

	/**
	* 更新会议状态
	* @param status 参会人状态：0 - 未处理， 1 - 已接收，2 - 已拒绝
	* @parma account 参会人账号
	* @param eventId 原事件id
	* @param startTime 原开始时间
	* @param authorizerId 原授权人ID
	* @return 成功：0 失败：非0
	*/
	int UpdateAttendeeStatus(uc::AttendeeStatus status, const std::string& account, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);

	/**
	 * 获取会议信息
	 * @param [OUT] data 会议信息
	 * @param eventId 事件id
	 * @param startTime 会议开始时间
	 * @param authorizerId 授权人id
	 * @return 成功：0 失败：非0
	 */
	int GetMeeting(Meeting& meeting, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);
	int GetMeeting(Meeting& meeting, const std::string& confId);

	/**
	* 通过parentId获取会议信息
	* @param [OUT] data 会议信息
	* @param eventId 事件id
	* @param startTime 会议开始时间
	* @param authorizerId 授权人id
	* @return 成功：0 失败：非0
	*/
	int GetMeetingByParentId(int64_t parentId, int64_t oriTime, int32_t authorizerId, Meeting& data);
	
	/**
	* 通过eventId获取会议ID
	* @param eventId 事件id
	* @return 成功返回会议ID, 失败返回0
	*/
	int GetConferenceIdByEventId(int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);

	/**
	* 通过eventId获取讨论组ID
	* @param eventId 事件id
	* @return 成功返回会议ID, 失败返回0
	*/
	int64_t GetEventIdByGroupId(int64_t groupId);

	/**
	* 通过conferenceId获取eventID
	* @param conferenceId
	* @return 成功返回事件ID, 失败返回0
	*/
	int64_t GetEventIdByConferenceId(int32_t conferenceId, int32_t authorizerId = 0);

	/**
	 * 获取会议信息
	 * @param eventIds 事件列表
	 * @param startTime 会议开始时间
	 * @param endTime 会议结束时间
	 * @param authorizerId 授权人id
	 * @param data 会议信息列表
	 * @return 成功：0 失败：非0
	 */
	int GetMeetingByIds(vector<Meeting>& data, const vector<int64_t>& eventIds, int64_t startTime, int64_t endTime, int32_t authorizerId = 0);

	/**
	 * 获取会议信息
	 * @param startTime 开始时间
	 * @param endTime　结束时间
	 * @param limit　每页查询数量
	 * @param data 获取到的会议列表信息
	 * @return 成功：0 失败：非0
	 */
	int GetMeetingList(vector<Meeting>& data, int64_t startTime, int64_t endTime, int32_t limit = 200, int32_t authorizerId = 0, int64_t eventIdFilter = 0, bool bFilterAsc = true);

	/**
	 * 获取历史会议信息
	 * @param authorizerId　授权人id
	 * @param endTime　截止时间点
	 * @param limit　每页查询数量
	 * @param data 获取到的会议列表信息
	 * @return 成功：0 失败：非0
	 */
	int GetOldMeetingList(vector<Meeting>& data, int64_t endTime, int32_t limit = 200, int32_t authorizerId = 0, int64_t eventIdFilter = 0);

	/**
	 * 获取新的会议信息
	 * @param authorizerId　授权人id
	 * @param startTime 起始时间
	 * @param limit　每页查询数量
	 * @param data 获取到的会议列表信息
	 * @return 成功：0 失败：非0
	 */
	int GetNewMeetingList(vector<Meeting>& data, int64_t startTime, int32_t limit = 200, int32_t authorizerId = 0, int64_t eventIdFilter = 0);


	/**
	 * 删除会议信息
	 * @param eventId 事件id
	 * @param startTime 开始时间
	 * @param authorizerId 授权人ID
	 * @return 成功：0 失败：非0
	 */
	int DeleteMeetingByEventId(int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);
	int DeleteMeetings(const vector<Meeting>& data);


	/**
	 * 删除会议信息(暂时未使用)
	 * @param conferenceId 会议id
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @return 成功：0 失败：非0
	 */
	int DeleteMeetingsWithCommonEventId(int64_t eventId, int64_t startTime, int64_t endTime);

	/**
	 * 删除会议信息
	 * @param conferenceId 会议id
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @param page 翻页数
	 * @param limit 每页数量
	 * @return 成功：0 失败：非0
	 */
	int DeleteMeetingsWithTimeRange(int64_t startTime, int64_t endTime, int32_t limit = 200, int64_t eventIdFilter = 0, bool bfilterAsc = true, int32_t authorizerId = 0);

	/**
	 * 保存日程授权人信息
	 * @param data 授权信息
	 * @return 成功：0 失败：非0
	 */
	int SaveAccreditUser(const Accrediting& data);

	/**
	 * 获取日程授权人列表
	 * @param data 日程授权人信息
	 * @return 成功：0 失败：非0
	 */
	int GetAccreditUserList(vector<Accrediting>& data);

	/**
	 * 会议数据清空
	 * @return 成功：0 失败：非0
	 */
	int CleanupMeetings();

	/**
	* 会议数据状态置旧
	* @return 成功：0 失败：非0
	*/
	int SetAllMeetingOld();

	//////////////////////group//////////////////////////
	/**
	 * 保存讨论组列表信息
	 * @param data 讨论组列表信息
	 * @return 成功：0 失败：非0
	 */
	int SaveGroup(const list<GroupInfo>& data);

	/**
	* 获取所有讨论组
	* @param 出参, 获取到的讨论组
	* @return 成功：0 失败：非0
	*/
	int GetAllGroup(OUT list<GroupInfo>&data);

	/**
	 * 获取指定讨论组信息
	 * @param groupId 讨论组id
	 * @param data 讨论组信息
	 * @return 成功：0 失败：非0
	 */
	int GetGroup(int64_t groupId, GroupInfo& data);

	/**
	 * 更新指定讨论组本地头像
	 * @param groupId 讨论组id
	 * @param avatar 头像URL
	 * @return 成功：0 失败：非0
	 */
	int SetGroupLocalAvatar(int64_t groupId, const string& avatar);

	/**
	 * 更新指定讨论组状态
	 * @param groupId 讨论组id
	 * @param state 讨论组状态
	 * @return 成功：0 失败：非0
	 */
	int SetGroupStatus(int64_t groupId, int state);

	/**
	* 获取指定讨论组本地头像
	* @param groupId 讨论组id
	* @return 讨论组本地头像
	*/
	string GetGroupLocalAvatar(int64_t groupId);

	/**
	* 保存Value在KeyValue 表信息
	* @param application 应用名称
	* @param key 配置属性名称
	* @param value 配置参数值
	* @return 成功：0 失败：非0
	*/
	int SaveValueByKey(const string& application, const string& key, const string& value);

	/**
	* 获取value在KeyValue表信息
	* @param application 应用名称
	* @param key 配置属性名称
	* @param value 获取到的配置参数值
	* @return 成功：0 失败：非0
	*/
	int GetValueByKey(const string& application, const string& key, string& value);

	//////////////////////配置信息///////////////////////
	/**
	 * 保存配置信息
	 * @param application 应用名称
	 * @param key 配置属性名称
	 * @param value 配置参数值
	 * @return 成功：0 失败：非0
	 */
	int SaveConfigValue(const string& application, const string& key, const string& value);

	/**
	 * 批量保存配置信息
	 * @param application 应用名称
	 * @param value 配置参数值键值对
	 * @return 成功：0 失败：非0
	 */
	int SaveConfigValue(const string& application, const map<string, string>& value);

	/**
	 * 获取配置信息
	 * @param application 应用名称
	 * @param key 配置属性名称
	 * @param value 获取到的配置参数值
	 * @return 成功：0 失败：非0
	 */
	int GetConfigValue(const string& application, const string& key, string& value);

	/**
	 * 获取指定应用的配置信息
	 * @param application 应用名称
	 * @param value 应用的配置数据
	 * @return 成功：0 失败：非0
	 */
	int GetConfigValue(const string& application, map<string, string>& valueMap);

	/**
	 * 更新配置信息
	 * @param application 应用名称
	 * @param key 配置属性名称
	 * @param value 需要更形的配置参数值
	 * @return 成功：0 失败：非0
	 */
	int UpdateConfigValue(const string& application, const string& key, const string& value);

	/**
	 * 更新配置信息
	 * @param application 应用名称
	 * @param value 待更新的属性参数键值对
	 * @return 成功：0 失败：非0
	 */
	int UpdateConfigValue(const string& application, const map<string, string>& value);

	//////////////////////search//////////////////////////
	/**
	 * 联系人匹配搜索
	 * @param keyWord 搜索关键字
	 * @param page 第几页
	 * @param count 每页数量
	 * @param contactList 匹配的联系人列表
	 * @return 成功：0 失败：非0
	 */
	int SearchContact(const string& keyWord, int32_t page, int32_t count, list<ContactInfo*>& contactList);

	/**
	 * 讨论组匹配搜索
	 * @param keyWord 搜索关键字
	 * @param page 第几页
	 * @param count 每页数量
	 * @param contactList 匹配的联系人列表
	 * @return 成功：0 失败：非0
	 */
	int SearchGroup(const string& keyWord, int32_t page, int32_t count, list<GroupInfo>& groupList);

	//////////////////////FileTransfer本地路径信息///////////////////////
	/**
	 * 获取文件本地路径信息
	 * @param fileURL	文件URL
	 * @param filePath	文件本地存储路径
	 * @param excludeIfNotExist	如果查询到本地路径文件不存在，是否返回结果。
	 * @return 成功：0 失败：非0
	 */
	int32_t GetFSLocalPath(const string &fileURL, string &filePath, bool excludeIfNotExist = false);
	
	/**
	 * 批量获取文件本地路径信息
	 * @param fileURLs	文件URL列表
	 * @param filePaths	查找到的映射路径
	 * @param excludeIfNotExist	如果查询到本地路径文件不存在，是否返回结果。
	 * @return 成功：0 失败：非0
	 */
	int32_t GetFSLocalPath(const std::list<string> &fileURLs, std::map<string, string> &filePaths, bool excludeIfNotExist = false);

	/**
	 * 更新文件本地路径信息
	 * @param fileURL	文件URL
	 * @param filePath	文件本地存储路径
	 * @return 成功：0 失败：非0
	 */
	int32_t UpdateFSLocalPath(const string &fileURL, const string &filePath);

	/**
	* 获取文件本地路径信息
	* @param fileURL	文件URL
	* @param filePath	文件本地存储路径
	* @param fileSize   文件大小。
	* @return 成功：0 失败：非0
	*/
	int32_t GetFSLocalPathAndSize(const std::string &fileURL, std::string &filePath, int64_t &fileSize);
	/**
	* 更新文件本地路径信息
	* @param fileURL	文件URL
	* @param filePath	文件本地存储路径
	* @param fileSize   文件大小
	* @return 成功：0 失败：非0
	*/
	int32_t UpdateFSLocalPathAndSize(const string &fileURL, const string &filePath, int64_t fileSize);

	/**
	* 更新撤回消息情况
	* @param type		会话类型
	* @param seqId		消息的seq值
	* @param relateId	消息关了ID
	* @param state	    消息撤回的状态值
	* @return 成功：0 失败：非0
	*/
	int32_t UpdateRevocationMsgState(int32_t type, int64_t seqId, int64_t relateId, int32_t state);

	//保存电话号码
	int32_t SaveCallPhoneNum(const string& phoneNum);

	//获取历史号码
	int32_t GetHistoryPhoneNum(list<string>& phoneNums);
	
	//////////////////////外部数据库///////////////////////
	int32_t HaveData(string externalId);
	int32_t GetExternalMessage(int32_t conversationType, string externalId, int64_t startTime, int64_t endTime, int count, list<ExternalMessage*>& message);

#ifdef PBX_SERVICE
	/**
	* 获取通话列表
	*/
	int32_t GetPbxCallRecordLst(int64_t timeStamp, int count, int selMask, PbxCallRecordList &lstData);
	/**
	* 获取通话详情
	*/
	int32_t SelectPbxCallDetails(const AC_PbxCallRecord &cond, int64_t timeStamp,
		int count, int selMask, PbxCallRecordList &lstData);
	/**
	* 通话记录匹配搜索
	*/
	int32_t SearchPbxCallRecord(const string& keyWord, int64_t timeStamp,
		int32_t count, PbxCallRecordList &lstData);
	/**
	* 添加通话详情
	*/
	int32_t InsertPbxCallDetails(const AC_PbxCallRecord &obj);
	/**
	* 更行通话详情
	*/
	int32_t UpdatePbxCallDetails(const AC_PbxCallRecord &cond, const AC_PbxCallRecord &data);
#endif // PBX_SERVICE

private:
	DBManager();
};

}
#endif // !defined(DBMANAGER_H_)
