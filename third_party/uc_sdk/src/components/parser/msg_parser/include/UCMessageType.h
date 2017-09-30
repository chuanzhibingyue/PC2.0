#ifndef UCMESSAGETYPE_H
#define UCMESSAGETYPE_H

#include "UCMessage.h"
#include "uc_types.h"

namespace uc {

/////////////////////////////////////////
//   uc 消息定义
/////////////////////////////////////////
#define MSG_CLASS(T) \
class T##Msg:public UCMsg { \
public: \
	T##Msg(); \
	T##Msg(MsgType type); \
	~T##Msg() { \
		if (_message != NULL) { \
			delete _message; \
			_message = NULL; \
		} \
	} \
	const T& Get_##T() const { \
		return *_message; \
	} \
public: \
	MSG_CONTENT_##T(); \
private: \
	void EncodeBody(); \
	void DecodeBody(UcMessageBody *body); \
private: \
	T* _message; \
};

#define MEMBER_ITEM( t, m ) \
	inline void set_##m(const t& data) { \
		_message->__set_##m(data); \
	} \
	inline const t& get_##m() { \
		return _message->m; \
	}

class NullMsg:public UCMsg {
public:
	NullMsg(){}
	NullMsg(MsgType type){ this->_msgType = type; }
	~NullMsg(){}
private:
	void EncodeBody() {}
	void DecodeBody(UcMessageBody *body) {}
};

////////////////////状态消息//////////////////////
#define MSG_CONTENT_MessagePresence()\
	MEMBER_ITEM(int32_t,            pri);\
	MEMBER_ITEM(std::string,        show);\
	MEMBER_ITEM(PresenceType::type, status);\
	MEMBER_ITEM(DeviceType::type,   deviceType);\
	MEMBER_ITEM(PresenceType::type,  oldStatus);
MSG_CLASS(MessagePresence);

////////////////////心跳消息/////////////////////
#define MSG_CONTENT_MessagePing()\
	MEMBER_ITEM(int32_t,            interval);\
	MEMBER_ITEM(DeviceType::type,   deviceType);
MSG_CLASS(MessagePing);

////////////////////已发回执////////////////////
#define MSG_CONTENT_AckSent()\
	MEMBER_ITEM(int32_t,            id);\
	MEMBER_ITEM(int64_t,            seq);\
	MEMBER_ITEM(int32_t,            topicId);
MSG_CLASS(AckSent);

////////////////////已达回执////////////////////
#define MSG_CONTENT_AckReceived()\
	MEMBER_ITEM(int32_t,            id);\
	MEMBER_ITEM(int64_t,            seq);
MSG_CLASS(AckReceived);

/////////////////////已读回执///////////////////
#define MSG_CONTENT_AckRead()\
	MEMBER_ITEM(int32_t,            id);\
	MEMBER_ITEM(int64_t,            seq);\
	MEMBER_ITEM(int8_t,     sessionType);\
	MEMBER_ITEM(JID,          ackedUser);
MSG_CLASS(AckRead);

/////////////////////批量已读回执///////////////////
#define MSG_CONTENT_BatchAckRead()\
	MEMBER_ITEM(int8_t,               sessionType);\
	MEMBER_ITEM(std::vector<int64_t>,     seqList);\
	MEMBER_ITEM(JID,					ackedUser);
MSG_CLASS(BatchAckRead);

/////////////////////已播放///////////////////
#define MSG_CONTENT_AckPlay()\
	MEMBER_ITEM(int32_t,            id);\
	MEMBER_ITEM(int64_t,            seq);
MSG_CLASS(AckPlay);

////////////////////睡眠或醒来请求/////////////////
#define MSG_CONTENT_SleepSet()\
	MEMBER_ITEM(SleepMode::type,    mode);\
	MEMBER_ITEM(int32_t,            unreadCount);
MSG_CLASS(SleepSet);

////////////////////服务器确认醒来/////////////////
#define MSG_CONTENT_WakeupResult() 
MSG_CLASS(WakeupResult);

///////////////////聊天文字消息///////////////////
#define MSG_CONTENT_TextContent()\
	MEMBER_ITEM(int8_t,           type);\
	MEMBER_ITEM(std::string,      text);\
	MEMBER_ITEM(std::string,      text_style);
MSG_CLASS(TextContent);

///////////////////聊天多媒体消息//////////////////
#define MSG_CONTENT_MediaContent()\
	MEMBER_ITEM(std::string,           media_filename);\
	MEMBER_ITEM(int32_t,               media_filesize);\
	MEMBER_ITEM(ChatMediaType::type,   media_type);\
	MEMBER_ITEM(std::string,           media_down_url);\
	MEMBER_ITEM(std::string,           media_thumb);\
	MEMBER_ITEM(int32_t,               media_duration);\
	MEMBER_ITEM(int32_t,               operatorId);\
	MEMBER_ITEM(std::string,           operatorName);\
	MEMBER_ITEM(std::string,           operatorAvatar);\
	MEMBER_ITEM(std::string,           media_thumb_url);
MSG_CLASS(MediaContent);

///////////////////聊天报告消息///////////////////
#define MSG_CONTENT_ReportContent()\
	MEMBER_ITEM(int64_t,          eventId);\
	MEMBER_ITEM(std::string,      reportTitle);\
	MEMBER_ITEM(std::string,      reportUrl);\
	MEMBER_ITEM(std::string,      externalData);
MSG_CLASS(ReportContent);

////////////////////组成员角色变更通知//////////////////
#define	RoleMap std::map<int32_t, int8_t>
#define MSG_CONTENT_MemberRoleUpdateContent()\
	MEMBER_ITEM(RoleMap,					roleMap);
MSG_CLASS(MemberRoleUpdateContent);

////////////////////讨论组添加成员//////////////////
#define MSG_CONTENT_GroupMemberAddContent()\
	MEMBER_ITEM(int32_t,								   operatorid);\
	MEMBER_ITEM(int32_t,								   groupid);\
	MEMBER_ITEM(std::vector<GroupMemberInfo>,           memberList);\
	MEMBER_ITEM(std::string,							   avatar);\
	MEMBER_ITEM(std::string,							   group_name);\
	MEMBER_ITEM(std::string,							   name_pinyin);\
	MEMBER_ITEM(int8_t,								   name_flag);
MSG_CLASS(GroupMemberAddContent);

/////////////////////消息撤回///////////////////////
#define MSG_CONTENT_RevocationContent()\
	MEMBER_ITEM(int64_t,           seq);\
	MEMBER_ITEM(int16_t,           type);
MSG_CLASS(RevocationContent);

/////////////////////成员退出讨论组/////////////////
#define MSG_CONTENT_GroupMemberDelContent()\
	MEMBER_ITEM(int32_t,           operatorid);\
	MEMBER_ITEM(int32_t,           groupid);\
	MEMBER_ITEM(int32_t,           memberId);\
	MEMBER_ITEM(std::string,       avatar);\
	MEMBER_ITEM(std::string,       group_name);\
	MEMBER_ITEM(std::string,       name_pinyin);\
	MEMBER_ITEM(int8_t,            name_flag);
MSG_CLASS(GroupMemberDelContent);

////////////////////讨论组名称更新//////////////////
#define MSG_CONTENT_GroupNameUpdateContent()\
	MEMBER_ITEM(int32_t,           operatorid);\
	MEMBER_ITEM(int32_t,           groupid);\
	MEMBER_ITEM(std::string,       group_name);\
	MEMBER_ITEM(std::string,       name_pinyin);\
	MEMBER_ITEM(int8_t,            name_flag);
MSG_CLASS(GroupNameUpdateContent);

/////////////////////讨论组创建/////////////////
#define MSG_CONTENT_GroupCreateContent()\
	MEMBER_ITEM(int32_t,							operatorid);\
	MEMBER_ITEM(int32_t,							   groupid);\
	MEMBER_ITEM(std::vector<GroupMemberInfo>,       memberList);\
	MEMBER_ITEM(std::string,						    avatar);\
	MEMBER_ITEM(std::string,						group_name);\
	MEMBER_ITEM(std::string,					   name_pinyin);\
	MEMBER_ITEM(int32_t,						   create_time);\
	MEMBER_ITEM(int32_t,							    siteID);\
	MEMBER_ITEM(int8_t,							     name_flag);\
	MEMBER_ITEM(int8_t,							    group_type);\
	MEMBER_ITEM(int32_t,						   memberCount);\
	MEMBER_ITEM(int32_t,						  group_status);\
	MEMBER_ITEM(std::string,					   status_info);\
	MEMBER_ITEM(std::string,				cloud_library_info);
MSG_CLASS(GroupCreateContent);

/////////////////////讨论组踢人/////////////////
#define MSG_CONTENT_GroupMemberKickContent()\
	MEMBER_ITEM(int32_t,                   operatorid);\
	MEMBER_ITEM(int32_t,                      groupid);\
	MEMBER_ITEM(std::vector<int32_t>,      memberList);\
	MEMBER_ITEM(std::string,                   avatar);\
	MEMBER_ITEM(std::string,               group_name);\
	MEMBER_ITEM(std::string,              name_pinyin);\
	MEMBER_ITEM(int8_t,                     name_flag);
MSG_CLASS(GroupMemberKickContent);

/////////////////////讨论组头像更新/////////////////
#define MSG_CONTENT_GroupAvatarUpdateContent()\
	MEMBER_ITEM(int32_t,            operatorid);\
	MEMBER_ITEM(std::string,            avatar);\
	MEMBER_ITEM(int32_t,               groupid);
MSG_CLASS(GroupAvatarUpdateContent);

/////////////////////讨论组会中状态更新/////////////////
#define MSG_CONTENT_GroupStateUpdateContent()\
	MEMBER_ITEM(int32_t,               group_status);\
	MEMBER_ITEM(std::string,            status_info);
MSG_CLASS(GroupStateUpdateContent);

/////////////////////讨论组置顶消息体/////////////////
#define MSG_CONTENT_GroupTopContent()\
	MEMBER_ITEM(int32_t,           operatorid);\
	MEMBER_ITEM(int32_t,           groupid);\
	MEMBER_ITEM(int8_t,            isTop);
MSG_CLASS(GroupTopContent);

/////////////////////文档上传消息体/////////////////
#define MSG_CONTENT_SummaryCreateContent()\
	MEMBER_ITEM(int32_t,           operatorid);\
	MEMBER_ITEM(std::string,       summary_name);\
	MEMBER_ITEM(std::string,       down_url);\
	MEMBER_ITEM(int32_t,           size); \
	MEMBER_ITEM(int64_t,           contentId);
MSG_CLASS(SummaryCreateContent);

////////////////////文档发送消息体/////////////////
#define MSG_CONTENT_DocumentContent()\
	MEMBER_ITEM(int32_t,           operatorid);\
	MEMBER_ITEM(std::string,           doc_name);\
	MEMBER_ITEM(std::string,           down_url);\
	MEMBER_ITEM(int32_t,               size); \
	MEMBER_ITEM(int64_t,               contentId);
MSG_CLASS(DocumentContent);

////////////////////云盘文档上传（更新）消息体/////////////////
#define MSG_CONTENT_CloudFileContent()\
	MEMBER_ITEM(int32_t,				operatorId);\
	MEMBER_ITEM(std::string,           operatorName);\
	MEMBER_ITEM(std::string,			 fileName);\
	MEMBER_ITEM(std::string,           fileNameTo);\
	MEMBER_ITEM(int64_t,					  size);\
	MEMBER_ITEM(int32_t,					fileCounts); \
	MEMBER_ITEM(int8_t,						isDir); \
	MEMBER_ITEM(int16_t,				cloudType); \
	MEMBER_ITEM(int8_t,				   detailType); \
	MEMBER_ITEM(std::string,		detailContent); \
	MEMBER_ITEM(int64_t,				createTime); \
	MEMBER_ITEM(int64_t,               contentId);
MSG_CLASS(CloudFileContent);

////////////////////文档删除消息体/////////////////
#define MSG_CONTENT_DocumentDelContent()\
	MEMBER_ITEM(int32_t,          operatorid);\
	MEMBER_ITEM(int64_t,           contentId);\
	MEMBER_ITEM(std::string,        doc_name);
MSG_CLASS(DocumentDelContent);

////////////////////评论创建消息体/////////////////
#define MSG_CONTENT_CommentCreateContent()\
	MEMBER_ITEM(int32_t,          operatorid);\
	MEMBER_ITEM(std::string,           operatorName);\
	MEMBER_ITEM(int64_t,           commentId);\
	MEMBER_ITEM(std::string,           commentDesc);\
	MEMBER_ITEM(int64_t,           contentId);\
	MEMBER_ITEM(std::string,           contentName);\
	MEMBER_ITEM(int8_t,           contentType);\
	MEMBER_ITEM(int64_t,        createTime);\
	MEMBER_ITEM(int16_t,		cloudType);
MSG_CLASS(CommentCreateContent);

////////////////////评论删除消息体/////////////////
#define MSG_CONTENT_CommentDelContent()\
	MEMBER_ITEM(int32_t,          operatorid);\
	MEMBER_ITEM(int64_t,           commentId);\
	MEMBER_ITEM(int64_t,           contentId);\
	MEMBER_ITEM(std::string,           contentName);
MSG_CLASS(CommentDelContent);

////////////////////代码创建消息体/////////////////
#define MSG_CONTENT_CodeCreateContent()\
	MEMBER_ITEM(int64_t,          contentId);\
	MEMBER_ITEM(std::string,           title);\
	MEMBER_ITEM(std::string,        langType); \
	MEMBER_ITEM(std::string,        contentHead); \
	MEMBER_ITEM(std::string,        desc); \
	MEMBER_ITEM(int32_t,        size); \
	MEMBER_ITEM(int32_t,        operatorid); \
	MEMBER_ITEM(std::string,        operatorName); \
	MEMBER_ITEM(std::string,        downUrl); \
	MEMBER_ITEM(int32_t,        totalLines); \
	MEMBER_ITEM(int64_t,        createTime); 
MSG_CLASS(CodeCreateContent);

////////////////////代码删除消息体/////////////////
#define MSG_CONTENT_CodeDelContent()\
	MEMBER_ITEM(int32_t,         operatorid); \
	MEMBER_ITEM(int64_t,          contentId); \
	MEMBER_ITEM(std::string,           title);
MSG_CLASS(CodeDelContent);

/////////////////////提醒规则更新消息体////////////////
#define MSG_CONTENT_AlertRuleContent()\
	MEMBER_ITEM(int32_t,           groupid);\
	MEMBER_ITEM(int8_t,           state);\
	MEMBER_ITEM(int32_t,           rule);\
	MEMBER_ITEM(int32_t,      time);
MSG_CLASS(AlertRuleContent);

//////////////////////结束讨论组消息体////////////////
#define MSG_CONTENT_DiscussionClose()\
	MEMBER_ITEM(int32_t,      operatorId);
MSG_CLASS(DiscussionClose);

////////////////////消息免打扰消息体//////////////////
#define MSG_CONTENT_MsgNotifyContent()\
	MEMBER_ITEM(int32_t,           groupid);\
	MEMBER_ITEM(int8_t,            state);
MSG_CLASS(MsgNotifyContent);

//////////////////系统欢迎消息///////////////////
#define MSG_CONTENT_SystemWelcomeContent()\
	MEMBER_ITEM(std::string,           title);\
	MEMBER_ITEM(std::string,           image_url);\
	MEMBER_ITEM(std::string,           desc);\
	MEMBER_ITEM(std::string,           target_link);
MSG_CLASS(SystemWelcomeContent);

/////////////////客户端更新消息////////////////////
#define MSG_CONTENT_ClientUpdateContent()\
	MEMBER_ITEM(int32_t,           version_code);\
	MEMBER_ITEM(int8_t,            is_force_upgrade);\
	MEMBER_ITEM(int16_t,           max_sdk_version);\
	MEMBER_ITEM(int16_t,           min_sdk_version);\
	MEMBER_ITEM(std::string,       version_name);\
	MEMBER_ITEM(std::string,       desc);\
	MEMBER_ITEM(std::string,       url);
MSG_CLASS(ClientUpdateContent);

//////////////////标签添加、标签更新消息体////////////////////
#define MSG_CONTENT_CompleteInfoContent()\
	MEMBER_ITEM(std::vector<InfoCell>,           info_cell_list);\
	MEMBER_ITEM(std::string,						desc);
MSG_CLASS(CompleteInfoContent);

//////////////////密码复杂性变化////////////////////
#define MSG_CONTENT_PwdComplexUpdateContent()\
	MEMBER_ITEM(PwdComplexType::type,           type);
MSG_CLASS(PwdComplexUpdateContent);

///////////////////密码到期通知///////////////////
#define MSG_CONTENT_PwdExpireContent()\
	MEMBER_ITEM(int32_t,      dueDays);
MSG_CLASS(PwdExpireContent);

/////////////////PC客户端引导安装消息/////////////////////
#define MSG_CONTENT_PCClientGuide()\
	MEMBER_ITEM(std::string,      downUrl);
MSG_CLASS(PCClientGuide);

/////////////////移动客户端引导安装/////////////////////
#define MSG_CONTENT_MobileClientGuide()\
	MEMBER_ITEM(std::string,      androidMarketUrl);\
	MEMBER_ITEM(std::string,      iphoneMarketUrl);
MSG_CLASS(MobileClientGuide);

//////////////////用户密码修改消息通知////////////////////
#define MSG_CONTENT_PasswordModifyContent()\
	MEMBER_ITEM(std::string,      password);
MSG_CLASS(PasswordModifyContent);

////////////////标签删除消息体/////////////////
#define MSG_CONTENT_TagDelContent()\
	MEMBER_ITEM(std::vector<int32_t>,      delTagIds);
MSG_CLASS(TagDelContent);

//////////////////项目组提醒消息体//////////////////
#define MSG_CONTENT_GroupRemindContent()\
	MEMBER_ITEM(std::vector<int32_t>,      groupIdList);
MSG_CLASS(GroupRemindContent);

//////////////////部门名称变更/////////////////
#define MSG_CONTENT_DeptUpdateContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(int32_t,           dept_id);\
	MEMBER_ITEM(std::string,       old_dept_name);\
	MEMBER_ITEM(std::string,       new_dept_name);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(DeptUpdateContent);

/////////////////////员工部门调动///////////////
#define MSG_CONTENT_DeptTransferContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(int32_t,           user_id);\
	MEMBER_ITEM(std::string,       user_name);\
	MEMBER_ITEM(int32_t,           dept_id);\
	MEMBER_ITEM(std::string,       old_dept_name);\
	MEMBER_ITEM(std::string,       dept_name);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(DeptTransferContent);

///////////////////员工职位调整/////////////////
#define MSG_CONTENT_PositionUpdateContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(int32_t,           user_id);\
	MEMBER_ITEM(std::string,       user_name);\
	MEMBER_ITEM(std::string,       new_position);\
	MEMBER_ITEM(std::string,       old_position);\
	MEMBER_ITEM(std::string,       dept_name);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(PositionUpdateContent);

///////////////////员工入职///////////////
#define MSG_CONTENT_EmployeeEntryContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(int32_t,           dept_id);\
	MEMBER_ITEM(std::string,       user_name);\
	MEMBER_ITEM(std::string,       dept_name);\
	MEMBER_ITEM(std::string,       position);\
	MEMBER_ITEM(int32_t,           user_id);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(EmployeeEntryContent);

///////////////////员工离职///////////////
#define MSG_CONTENT_EmployeeLeaveContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(int32_t,           user_id);\
	MEMBER_ITEM(std::string,       user_name);\
	MEMBER_ITEM(std::string,       dept_name);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(EmployeeLeaveContent);

///////////////////员工权限变更////////////////
#define MSG_CONTENT_EmployeeRightsChangeContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(int32_t,           user_id);\
	MEMBER_ITEM(std::string,       user_name);\
	MEMBER_ITEM(std::string,       dept_name);\
	MEMBER_ITEM(int32_t,           dept_id);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(EmployeeRightsChangeContent);

//////////////////部门移动/////////////////
#define MSG_CONTENT_DeptParentChangeContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(int32_t,           dept_id);\
	MEMBER_ITEM(std::string,       dept_name);\
	MEMBER_ITEM(std::string,       old_dept_name);\
	MEMBER_ITEM(std::string,       new_dept_name);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(DeptParentChangeContent);

/////////////////员工加入生态企业///////////////////
#define MSG_CONTENT_CompanyJoinContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(int32_t,           company_id);\
	MEMBER_ITEM(std::string,       company_name);\
	MEMBER_ITEM(int32_t,           user_id);\
	MEMBER_ITEM(std::string,       user_name);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(CompanyJoinContent);

//////////////////部门删除/////////////////
#define MSG_CONTENT_DeptDeleteContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(int32_t,           dept_id);\
	MEMBER_ITEM(std::string,       dept_name);\
	MEMBER_ITEM(std::string,       parent_dept_name);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(DeptDeleteContent);

//////////////////员工入职确认消息////////////////
#define MSG_CONTENT_EmployeeEntryConfirmContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(std::string,       user_name);\
	MEMBER_ITEM(int32_t,           dept_id);\
	MEMBER_ITEM(std::string,       dept_name);\
	MEMBER_ITEM(int32_t,           user_id);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(EmployeeEntryConfirmContent);

////////////////员工离职确认消息//////////////////
#define MSG_CONTENT_EmployeeLeaveConfirmContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(std::string,       user_name);\
	MEMBER_ITEM(int32_t,           dept_id);\
	MEMBER_ITEM(std::string,       dept_name);\
	MEMBER_ITEM(int32_t,           user_id);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(EmployeeLeaveConfirmContent);

//////////////////员工部门调动确认消息//////////////////
#define MSG_CONTENT_DeptTransferConfirmContent()\
	MEMBER_ITEM(int32_t,           operator_id);\
	MEMBER_ITEM(std::string,       user_name);\
	MEMBER_ITEM(int32_t,           dept_id);\
	MEMBER_ITEM(std::string,       dept_name);\
	MEMBER_ITEM(int32_t,           user_id);\
	MEMBER_ITEM(std::string,       desc);
MSG_CLASS(DeptTransferConfirmContent);

/////////////////即时会议邀请消息/////////////////
#define MSG_CONTENT_InstantIQInviteContent()\
	MEMBER_ITEM(int32_t,           confId);\
	MEMBER_ITEM(int32_t,           hostId);\
	MEMBER_ITEM(std::string,       confPwd);\
	MEMBER_ITEM(int8_t,            isReserve);
MSG_CLASS(InstantIQInviteContent);

////////////////预约会议邀请///////////////////
#define MSG_CONTENT_IQInviteContent()\
	MEMBER_ITEM(int32_t,           confId);\
	MEMBER_ITEM(int32_t,           shareUserID);\
	MEMBER_ITEM(int32_t,           hostId);\
	MEMBER_ITEM(int32_t,           groupID);\
	MEMBER_ITEM(std::string,       icalendar);\
	MEMBER_ITEM(int8_t,            isRecurrent);\
	MEMBER_ITEM(int32_t,           startTime);\
	MEMBER_ITEM(int64_t,           eventId);\
	MEMBER_ITEM(int64_t,           parentEventId);\
	MEMBER_ITEM(int32_t,           oriStartTime);
MSG_CLASS(IQInviteContent);

/////////////////预约会议接受邀请//////////////////
#define MSG_CONTENT_IQReceiveContent()\
	MEMBER_ITEM(int32_t,           confId);\
	MEMBER_ITEM(int32_t,           shareUserID);\
	MEMBER_ITEM(int32_t,           forwardUserID);\
	MEMBER_ITEM(int32_t,           startTime);\
	MEMBER_ITEM(int32_t,           hostId);\
	MEMBER_ITEM(std::string,       icalendar);\
	MEMBER_ITEM(int64_t,		   eventId);\
	MEMBER_ITEM(int64_t,           parentEventId);\
	MEMBER_ITEM(int32_t,           oriStartTime);
MSG_CLASS(IQReceiveContent);

//////////////////预约会议拒绝邀请//////////////////
#define MSG_CONTENT_IQRejectContent()\
	MEMBER_ITEM(int32_t,           confId);\
	MEMBER_ITEM(int32_t,           shareUserID);\
	MEMBER_ITEM(int32_t,           forwardUserID);\
	MEMBER_ITEM(int32_t,           startTime);\
	MEMBER_ITEM(int32_t,           hostId);\
	MEMBER_ITEM(std::string,       icalendar);\
	MEMBER_ITEM(int64_t,		   eventId);\
	MEMBER_ITEM(int64_t,           parentEventId);\
	MEMBER_ITEM(int32_t,           oriStartTime);
MSG_CLASS(IQRejectContent);

//////////////////预约会议取消/////////////////
#define MSG_CONTENT_ConfCancelContent()\
	MEMBER_ITEM(int32_t,           confId);\
	MEMBER_ITEM(int32_t,           shareUserID);\
	MEMBER_ITEM(int32_t,           startTime);\
	MEMBER_ITEM(std::string,       icalendar);\
	MEMBER_ITEM(int64_t,		   eventId);\
	MEMBER_ITEM(int64_t,           parentEventId);\
	MEMBER_ITEM(int32_t,           oriStartTime);
MSG_CLASS(ConfCancelContent);

//////////////////会前提醒消息////////////////
#define MSG_CONTENT_ConfAlertContent()\
	MEMBER_ITEM(int32_t,           confId);\
	MEMBER_ITEM(int32_t,           shareUserID);\
	MEMBER_ITEM(int8_t,            isInstant);\
	MEMBER_ITEM(int32_t,           startTime);\
	MEMBER_ITEM(RoleType::type,    role);\
	MEMBER_ITEM(std::string,       icalendar);\
	MEMBER_ITEM(int64_t,       eventId);
MSG_CLASS(ConfAlertContent);

//////////////////即时会议接受邀请消息////////////////
#define MSG_CONTENT_InstantIQReceiveContent()\
	MEMBER_ITEM(int32_t,           confId);\
	MEMBER_ITEM(int8_t,           isReserve);
MSG_CLASS(InstantIQReceiveContent);

/////////////////即时会议拒绝邀请消息//////////////////
#define MSG_CONTENT_InstantIQRejectContent()\
	MEMBER_ITEM(int32_t,     confId);\
	MEMBER_ITEM(int8_t,      isReserve);
MSG_CLASS(InstantIQRejectContent);

/////////////////纯会议讨论消息//////////////////
#define MSG_CONTENT_ConfChatContent()\
	MEMBER_ITEM(int32_t,           confId);\
	MEMBER_ITEM(int32_t,           startTime);\
	MEMBER_ITEM(TextContent,       text);\
	MEMBER_ITEM(MediaContent,      media);\
	MEMBER_ITEM(int64_t,           eventId);\
	MEMBER_ITEM(RevocationContent, revocation);
MSG_CLASS(ConfChatContent);

//////////////////会议纪要消息//////////////////
#define MSG_CONTENT_ConfSummaryContent()\
	MEMBER_ITEM(int32_t,           operatorid);\
	MEMBER_ITEM(int32_t,           summary_id);\
	MEMBER_ITEM(std::string,       summary_name);\
	MEMBER_ITEM(int8_t,            state);\
	MEMBER_ITEM(int32_t,           confId);\
	MEMBER_ITEM(std::string,       confName);\
	MEMBER_ITEM(int32_t,           startTime);\
	MEMBER_ITEM(int32_t,           updateTime);\
	MEMBER_ITEM(int64_t,           eventId);
MSG_CLASS(ConfSummaryContent);

/////////////////外呼状态消息//////////////////
#define MSG_CONTENT_CallStateContent()\
	MEMBER_ITEM(int32_t,           userId);\
	MEMBER_ITEM(std::string,       partyId);\
	MEMBER_ITEM(std::string,       callSeq);\
	MEMBER_ITEM(int32_t,           oldCallState);\
	MEMBER_ITEM(int32_t,           newCallState);
MSG_CLASS(CallStateContent);

//////////////////会议文档上传//////////////////
#define MSG_CONTENT_ConfUploadContent()\
	MEMBER_ITEM(int64_t,           operatorid);\
	MEMBER_ITEM(std::string,       doc_name);\
	MEMBER_ITEM(std::string,       down_url);\
	MEMBER_ITEM(int64_t,           size);\
	MEMBER_ITEM(int64_t,           contentId);\
	MEMBER_ITEM(int64_t,           eventId);
MSG_CLASS(ConfUploadContent);

/////////////////会议文档删除///////////////////
#define MSG_CONTENT_ConfDelDocContent()\
	MEMBER_ITEM(int64_t,           operatorid);\
	MEMBER_ITEM(std::string,       doc_name);\
	MEMBER_ITEM(int64_t,           contentId);\
	MEMBER_ITEM(int64_t,           eventId);
MSG_CLASS(ConfDelDocContent);

/////////////////名片变更消息//////////////////
#define MSG_CONTENT_CardUpdateContent()\
	MEMBER_ITEM(int32_t,           userId);\
	MEMBER_ITEM(int32_t,      cardVersion);
MSG_CLASS(CardUpdateContent);

////////////////联系人关系建立//////////////////
#define MSG_CONTENT_ContacterAddContent() 
MSG_CLASS(ContacterAddContent);

/////////////////联系人关系移除///////////////////
#define MSG_CONTENT_ContacterDelContent() 
MSG_CLASS(ContacterDelContent);

/////////////////传输邀请//////////////////
#define MSG_CONTENT_FileTransmissionInviteContent()\
	MEMBER_ITEM(std::string,      fileMD5);\
	MEMBER_ITEM(std::string,      fileName);\
	MEMBER_ITEM(int64_t,          fileSize);\
	MEMBER_ITEM(int64_t,          blockSize);\
	MEMBER_ITEM(int8_t,           nZip);\
	MEMBER_ITEM(int64_t,          taskId);
MSG_CLASS(FileTransmissionInviteContent);

////////////////同意传输////////////////////
#define MSG_CONTENT_FileTransmissionReceiveContent()\
	MEMBER_ITEM(int64_t,         sendTaskId);\
	MEMBER_ITEM(int64_t,         receiveTaskId);\
	MEMBER_ITEM(std::string,     ip);\
	MEMBER_ITEM(int16_t,         port);\
	MEMBER_ITEM(std::string,     serverList);\
	MEMBER_ITEM(int8_t,          transType);\
	MEMBER_ITEM(int64_t,         transPoint);
MSG_CLASS(FileTransmissionReceiveContent);

////////////////拒绝传输///////////////////
#define MSG_CONTENT_FileTransmissionRejectContent()\
	MEMBER_ITEM(int64_t,           taskId);\
	MEMBER_ITEM(std::string,       fileName);\
	MEMBER_ITEM(int64_t,           fileSize);
MSG_CLASS(FileTransmissionRejectContent);

/////////////////停止传输//////////////////
#define MSG_CONTENT_FileTransmissionStopContent()\
	MEMBER_ITEM(int64_t,           sendTaskId);\
	MEMBER_ITEM(int64_t,           receiveTaskId);\
	MEMBER_ITEM(std::string,       fileName);\
	MEMBER_ITEM(int64_t,           fileSize);\
	MEMBER_ITEM(int8_t,            flag);
MSG_CLASS(FileTransmissionStopContent);

//////////////////音频聊天邀请////////////////
#define MSG_CONTENT_AudioInviteContent()\
	MEMBER_ITEM(std::string,       confId);\
	MEMBER_ITEM(int32_t,           hostId);\
	MEMBER_ITEM(std::string,       confPwd);
MSG_CLASS(AudioInviteContent);

////////////////同意音频聊天///////////////////
#define MSG_CONTENT_AudioReceiveContent()\
	MEMBER_ITEM(std::string,      confId);\
	MEMBER_ITEM(int32_t,      hostId);
MSG_CLASS(AudioReceiveContent);

////////////////拒绝音频聊天///////////////////
#define MSG_CONTENT_AudioRejectContent()\
	MEMBER_ITEM(std::string,      confId);\
	MEMBER_ITEM(int32_t,      hostId);\
	MEMBER_ITEM(std::string,  reason);
MSG_CLASS(AudioRejectContent);

/////////////////停止音频聊天//////////////////
#define MSG_CONTENT_AudioStopContent()\
	MEMBER_ITEM(std::string,      confId);\
	MEMBER_ITEM(int32_t,      hostId);
MSG_CLASS(AudioStopContent);

////////////////音频通话时长呼叫记录//////////////////
#define MSG_CONTENT_AudioRecentCallContent()\
	MEMBER_ITEM(int32_t,           duration);\
	MEMBER_ITEM(std::string,      phoneNum);
MSG_CLASS(AudioRecentCallContent);

///////////////未接通的音频呼叫记录//////////////////
#define MSG_CONTENT_AudioBlockCallContent()\
	MEMBER_ITEM(std::string,      phoneNum);
MSG_CLASS(AudioBlockCallContent);

/////////////////拒绝的音频呼叫记录//////////////////
#define MSG_CONTENT_AudioRejectCallContent()\
	MEMBER_ITEM(std::string,      phoneNum);
MSG_CLASS(AudioRejectCallContent);

////////////////音频呼叫发起方主动挂断//////////////////
#define MSG_CONTENT_AudioCancelCallContent()\
	MEMBER_ITEM(std::string,      phoneNum);
MSG_CLASS(AudioCancelCallContent);

////////////////日程授权共享消息///////////////////
#define MSG_CONTENT_ConfShareContent() 
MSG_CLASS(ConfShareContent);

////////////////工作圈更新消息///////////////////
#define MSG_CONTENT_WBMessageContent()\
	MEMBER_ITEM(std::string,      text);
MSG_CLASS(WBMessageContent);

////////////////远程协助邀请消息体定义///////////////////
#define MSG_CONTENT_RemoteAssistanceInviteContent()\
	MEMBER_ITEM(std::string,         confId);\
	MEMBER_ITEM(std::string,         confPwd);
MSG_CLASS(RemoteAssistanceInviteContent);

////////////////远程协助接受消息体定义///////////////////
#define MSG_CONTENT_RemoteAssistanceAcceptContent()\
	MEMBER_ITEM(std::string,         confId);
MSG_CLASS(RemoteAssistanceAcceptContent);

////////////////远程协助拒绝消息体定义///////////////////
#define MSG_CONTENT_RemoteAssistanceRejectContent()\
	MEMBER_ITEM(std::string,         confId);
MSG_CLASS(RemoteAssistanceRejectContent);

////////////////远程协助结束消息体定义///////////////////
#define MSG_CONTENT_RemoteAssistanceEndContent()\
	MEMBER_ITEM(std::string,         confId);
MSG_CLASS(RemoteAssistanceEndContent);


////////////////会话置顶多端同步消息体定义///////////////////
#define MSG_CONTENT_SessionTopContent()\
	MEMBER_ITEM(JID,         to); \
	MEMBER_ITEM(int8_t,      sessionType); \
	MEMBER_ITEM(int64_t,     topindex);
MSG_CLASS(SessionTopContent);

////////////////@消息已读多端同步消息体定义///////////////////
#define MSG_CONTENT_AtMessageReadedContent()\
	MEMBER_ITEM(JID,         to); \
	MEMBER_ITEM(int8_t,      sessionType); \
	MEMBER_ITEM(std::vector<int64_t>,     seqList);
MSG_CLASS(AtMessageReadedContent);

////////////////@消息已读多端同步消息体定义///////////////////
#define MSG_CONTENT_UserLoginNotifyContent()\
	MEMBER_ITEM(JID,         user); \
	MEMBER_ITEM(DeviceType::type,  deviceType); \
	MEMBER_ITEM(std::string, deviceName);
MSG_CLASS(UserLoginNotifyContent);

////////////////API文本消息体定义///////////////////
#define MSG_CONTENT_APITextContent()\
	MEMBER_ITEM(std::string,      title);\
	MEMBER_ITEM(std::string,      content);\
	MEMBER_ITEM(int8_t,           detailType);\
	MEMBER_ITEM(std::string,      detailContent);\
	MEMBER_ITEM(int8_t,			  detailAuth);
MSG_CLASS(APITextContent);

////////////////API图片消息体定义///////////////////
#define MSG_CONTENT_APIImageContent()\
	MEMBER_ITEM(std::string,      mediaId);\
	MEMBER_ITEM(int8_t,           detailType);\
	MEMBER_ITEM(std::string,      mediaThumb);
MSG_CLASS(APIImageContent);

////////////////API文件消息体定义///////////////////
#define MSG_CONTENT_APIFileContent()\
	MEMBER_ITEM(std::string,      mediaId);\
	MEMBER_ITEM(int8_t,           detailType);\
	MEMBER_ITEM(int64_t,          fileSize);\
	MEMBER_ITEM(std::string,      fileName);
MSG_CLASS(APIFileContent);

/////////////////API OA消息体定义///////////////////
#define MSG_CONTENT_APIOAContent()\
	MEMBER_ITEM(std::string,      title);\
	MEMBER_ITEM(int16_t,          status);\
	MEMBER_ITEM(std::string,      color);\
	MEMBER_ITEM(std::string,      bkColor);\
	MEMBER_ITEM(std::string,      elements);\
	MEMBER_ITEM(std::string,      detailURL);\
	MEMBER_ITEM(int8_t,           detailAuth);\
	MEMBER_ITEM(std::string,      titleElements)\
	MEMBER_ITEM(std::string,      titleStyle);\
	MEMBER_ITEM(std::string,      exData);\
	MEMBER_ITEM(std::string,      customizedType);\
	MEMBER_ITEM(std::string,      customizedData);
MSG_CLASS(APIOAContent);

////////////////////API状态变更消息体定义///////////////
#define MSG_CONTENT_APIOAStatusChangeContent()\
	MEMBER_ITEM(int64_t,      seq);\
	MEMBER_ITEM(int16_t,          newStatus);\
	MEMBER_ITEM(std::string, externalData);
MSG_CLASS(APIOAStatusChangeContent);

////////////////API Ack消息体定义///////////////////
#define MSG_CONTENT_APIAckSentContent()\
	MEMBER_ITEM(int32_t,         id);\
	MEMBER_ITEM(int64_t,         seq);\
	MEMBER_ITEM(int32_t,         conversation);\
	MEMBER_ITEM(int8_t,          result);
MSG_CLASS(APIAckSentContent);

////////////////////应用事件通知消息体定义///////////////
#define MSG_CONTENT_APIAppEventChangeNotify()\
	MEMBER_ITEM(int64_t,      appId);\
	MEMBER_ITEM(int32_t,      eventCount);\
	MEMBER_ITEM(std::string,  externalData);
MSG_CLASS(APIAppEventChangeNotify);


////////////////////应用变更通知消息体定义///////////////
#define MSG_CONTENT_APIAppChangeNotifyContent()\
	MEMBER_ITEM(int16_t,      event);\
	MEMBER_ITEM(int64_t,      appId);\
	MEMBER_ITEM(std::string,  externalData);
MSG_CLASS(APIAppChangeNotifyContent);

////////////////////Customized消息体定义///////////////
#define MSG_CONTENT_APICustomizedContent()\
	MEMBER_ITEM(std::string,      data1);\
	MEMBER_ITEM(std::string,      data2);\
	MEMBER_ITEM(std::string,      data3);\
	MEMBER_ITEM(std::string,      data4);
MSG_CLASS(APICustomizedContent);



} //namespace uc

#endif //UCMESSAGETYPE_H
