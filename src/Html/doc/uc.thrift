// 命名规则：类名为大写开头，成员变量小写开头，enum的成员为下划线开头；
namespace cpp uc
namespace php uc
namespace java com.gnet.uc.thrift
//thrift 说明部分
//1基本类型（括号内为对应的Java类型）：
//bool（boolean）: 布尔类型(TRUE or FALSE)
//byte（byte）: 8位带符号整数
//i16（short）: 16位带符号整数
//i32（int）: 32位带符号整数
//i64（long）: 64位带符号整数
//double（double）: 64位浮点数
//string（String）: 采用UTF-8编码的字符串
//
//2.特殊类型（括号内为对应的Java类型）：
//binary（ByteBuffer）：未经过编码的字节流
//3.Structs（结构）：
//struct定义了一个很普通的OOP对象，但是没有继承特性。
//struct UserProf// 命名规则：类名为大写开头，成员变量小写开头，enum的成员为下划线开头；
//struct UserProfile {
//1: i32 uid,
//2: string name,
//3: string blurb
//}
//如果变量有默认值，可以直接写在定义文件里:
//struct UserProfile {
//1: i32 uid = 1,
//2: string name = "User1",
//3: string blurb
//}
//4.容器，除了上面提到的基本数据类型，Thrift还支持以下容器类型：
//list(java.util.ArrayList)：
//set(java.util.HashSet)：
//map（java.util.HashMap）：
//typedef list<double> Vector
//用法如下：
//struct Node {
//1: i32 id,
//2: string name,
//3: list<i32> subNodeList,
//4: map<i32,string> subNodeMap,
//5: set<i32> subNodeSet
//}

//@ 状态消息类型
enum PresenceMessageType{
    DefaultType = 0x01;
}

//@ 状态消息ID
enum PresenceMessageID{
    DefaultId = 0x0001;
}
//@ 状态类型
enum PresenceType{
  Online = 0x0001, // 在线
	Busy = 0x0002, // 请勿打扰
	Away = 0x0003, // 离开
	Meeting = 0x0004, // 会议中
	Offline = 0x0005, // 离线
	Call = 0x0006,    // 呼中（新增）
}
//@ 设备类型
enum  DeviceType{
	Iphone = 0x01,
	Android = 0x02,
	PC = 0x03,
	Background = 0x04, // 大道后台
	Ipad = 0x05,
}

//
//状态消息
struct MessagePresence{
	1:i32 pri,   // 状态优先级
	2:string show,  // 用户输入的状态字符串，例如：我去吃饭了
	3:PresenceType status, // 联系人状态
	4:DeviceType deviceType, // 设备类型
	5:optional PresenceType oldStatus, // 本次更改之前的用户状态，会or呼 结束后恢复用户状态场景使用
}

//@ ping消息类型
enum PingMessageType{
        DefaultType = 0x01;
}

//@ ping消息ID
enum PingMessageID{
        DefaultId = 0x0001;
}
//
//心跳消息
//interval 单位秒，表示客户端设置的ping的间隔，如果没有interval，
//服务器会使用客户端最后设置的interval， 默认值为5*60秒
struct MessagePing{
	1:required i32 interval,  // 心跳间隔
	2:required DeviceType deviceType, // 发送心跳的设备类型
}

//
// 消息状态应答sent, received, read 消息
//
//服务器收到消息在给发送方回sent ack的时候，带上原消息id和服务器生成的全局seq
//服务器会在消息里面加上seq，并转发给接收方
//接收方在回sent/received/read ack的时候，也要带上 id和seq
//客户端应该根据from + id 去重, 只在界面显示一条消息
//对相同的from+id， 服务器可能会生成不同的seq（因为服务器没有去重）
//服务器转发给客户端的ack(received, read) 只包括 id，
//而没有seq （服务器会在转发前去掉该ack消息里面的seq）
//客户端应该根据服务器生成的seq与服务器同步

//服务器收到消息后，回复"已发"消息
//注：支持全存储后，服务器会为该消息生成一个全局seq

//@ACK消息号定义
enum AckMessageID{
	AckSent=0x0001,//	已发
	AckReceived=0x0002,//	已达
	AckRead=0x0003,//	已读
	AckPlay=0x0004, // 已播放
}
//@ ack消息类型
enum AckMessageType{
        // 单条消息ACK
        DefaultType = 0x01,
        // 备注：1、批量ACK是对整个conversation进行ACK，也包括已发、已达、已读，
        // 2、批量ACK消息中消息头中的id、seq字段没有意义，赋默认值0即可; 
        // 3、批量ACK消息的消息体没有意义，但为了保证消息协议的一致性，消息体仍然使用单条消息ACK的消息体    
        BatchAckType = 0x02, 
}
//@已发消息
struct AckSent{
	1:i32 id,//客户端发送消息前生成的id
	2:i64 seq,//服务器收到消息生成的id
	3:i32 topicId,//话题id
}

//接受方受到消息时，发送"已达"消息
//服务器收到已达消息后，应该去掉seq，再把该消息转发给消息发送方
struct AckReceived{
	1:i32 id,//客户端发送消息前生成的id
	2:i64 seq,//服务器收到消息生成的id
}

//接受方读过消息后，发送"已读"消息
//服务器收到已读消息后，应该去掉seq，再把该消息转发给消息发送方
struct AckRead{
	1:i32 id,//客户端发送消息前生成的id
	2:i64 seq,//服务器收到消息生成的id
}

//接受方播放过消息后，发送"已播放"消息
//服务器收到已播放消息后，应该去掉seq，再把该消息转发给消息发送方
struct AckPlay{
	1:i32 id,//客户端发送消息前生成的id
	2:i64 seq,//服务器收到消息生成的id
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//睡眠消息
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//睡眠模式 （为了支持米聊切换到iphone后台后能够收到push消息而不离线）
//1. 客户端给服务器发送sleep消息包
//没有提供mode时，的缺省值为 light，服务器收到客服端接下来发送的任何消息，
//都认为客服端已经醒来 .如果mode为deep，服务器只有收到客户端的wakeup 消息时，
//才认为客户端已经醒来。 wakeup的消息格式为同时，服务器会给客户端发一个iq result
//注： 服务器只对wakeup回iq result，对sleep，不会回任何iq
//2. 服务器端在收到sleep消息包后，除了继续给客户端发送消息外，还会给客户端push相应的消息
//3. 服务器端在接收到客户端的任何消息包后，即认为客户端已经醒了（wakeup），不再继续push消息

//@ 睡眠消息体定义
enum SleepMessageType{
    DefaultType = 0x01, // 默认类型 
}

//@ 睡眠消息号定义
enum SleepMessageId{
    IQRequest = 0x0001, // 请求睡眠或醒来消息（客户端来发送）
    IQConfirm = 0x0002, // 同意醒来IQ确认消息（服务器来发送）
}

//@ 睡眠模式定义
enum SleepMode{
    light = 0x0001, // 普通睡眠模式
    deep = 0x0002, // 深度睡眠模式
    wakeup = 0x0003, // 醒来模式
}

//@请求睡眠模式
struct SleepSet{
	1:required SleepMode mode,// light|deep|wakeup
	2:optional i32 unreadCount, // 未读消息条数
}

//@服务器回应请求（此消息不需要消息体）
struct WakeupResult{ 
      // 此消息体不需要属性
      //1:i32 result=200,//返回值
}

// 消息类型高字节-组定义(已经废弃，改用PriType）
enum GroupType{
    to_person = 0x00,  // 发给单个人（高四字节为0）
    to_group = 0x10,  // 发给多个人（高四字节为1）   
}
// 消息类型高字节-服务器定义(已经废弃，改用PriType）
enum ContentType{
    thrift_type = 0x01, // 消息为thrift格式（低四字节为1）
    mime_type = 0x02, // 消息为mime格式（低四字节为2）
}

// 消息属性pri字段枚举值-定义
enum PriType{
    thrift_type = 0x01, // 消息为thrift格式（第1位为1）
    mime_type = 0x02, // 消息为mime格式（第2位为1）
    audio_type = 0x04, // 语音消息（第3位为1）
    control_type = 0x08, // （暂时不用，改用ControlPriType）
    group_type = 0x10, // 组消息（第5位为1)
    direct_type = 0x20, // 上下行标志（0：下行，1：上行；Server—>Client为下行，Client—>Server为上行）
}

// 扩展属性controlPri字段枚举值定义
enum ControlPriType{
    unread_count_type = 0x01,   // 是否进行未读计数（bit为0不计数，bit为1计数）
    display_type = 0x02,        // 是否入库显示（bit为0不显示，bit为1显示）
    webpush_send_type = 0x04,   // 是否下发webpush（bit为0不下发，bit为1下发）
    webpush_count_type = 0x08,  // 是否webpush计数（暂时不用）
}

// 消息计数channel枚举定义
enum ChannelPriType{
    channel0 = 0x01,            // 普通聊天消息channel 
    channel1 = 0x02,            // 会议讨论消息channel
    channel2 = 0x04,            // 会议纪要消息channel
    channel3 = 0x08,            // 会议接受/拒绝消息channel
    channel4 = 0x10,            // 会议邀请、取消消息channel
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//聊天消息
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//聊天消息类型
enum ChatMessageType
{
    NormalChat = 0x01,      //单人聊天消息
	  DiscussionChat = 0x02,   //讨论组
	  GroupChat = 0x03,       //群消息
}
 
// 单人聊天消息号 
enum ChatMessageId
{
  TextContent = 0x0001,     // 文本
	MediaContent = 0x0002,    // 富媒体
	DocumentSend = 0x0003,    // 文档发送消息，消息体为DocumentContent
	DocumentForward = 0x0004, // 文档转发消息
  DocumentDel = 0x0005,     // 文档删除消息
	CodeCreate = 0x0006,      // 代码创建消息
	CodeForward = 0x0007,     // 代码分享（转发）
	CodeDel = 0x0008,         // 代码删除
} 

//讨论组消息号(注意：此枚举非群聊消息号）
enum GroupMessageId
{
	TextContent = 0x0001,	//文本
	MediaContent = 0x0002,	//富媒体
	DisMemberAdd = 0x0003,	//成员添加
	DisMemberDel = 0x0004,	//成员退出
	DisNameUpdate = 0x0005,	//讨论组名称变更
	DiscussionCreate = 0x0006,	//创建讨论组
	DiscussionTop = 0x0007, // 讨论组聊天置顶
	SummaryCreate = 0x0008, // 文档发送消息(消息体为SummaryCreateContent)
	DocumentForward = 0x0009, // 文档分享/转发消息(消息体为SummaryCreateContent)
	AlertRuleUpdate = 0x000a, // 项目组提醒规则更新
	DiscussionClose = 0x000b, // 结束讨论组通知
	MsgNotifyUpdate = 0x000c, // 新消息通知状态更新
	DisMemberKick = 0x000d,    // 讨论组踢人（发起人权限） 
	DocumentDel = 0x000e,     // 文档删除
	CodeCreate = 0x000f,      // 代码创建
	CodeForward = 0x0010,     // 代码分享（转发）
	CodeDel = 0x0011,         // 代码删除
	CommentCreate = 0x0012,   // 评论创建
	CommentDel = 0x0013,      // 评论删除
	DisAvatarUpdate = 0x0014, // 项目组头像更新
	DisStateUpdate = 0x0015, // 讨论组会中状态更新
}

//群聊消息号（消息体定义与讨论组相同，用同一套）
enum ClusterMessageId
{
	TextContent = 0x0001,	//文本
	MediaContent = 0x0002,	//富媒体
	DisMemberAdd = 0x0003,	//成员添加
	DisMemberDel = 0x0004,	//成员退出
	DisNameUpdate = 0x0005,	//讨论组名称变更
	DiscussionCreate = 0x0006,	//创建讨论组
	DiscussionTop = 0x0007, // 讨论组聊天置顶
	SummaryCreate = 0x0008, // 文档发送消息(消息体为SummaryCreateContent)
	DocumentForward = 0x0009, // 文档分享/转发消息(消息体为SummaryCreateContent)
	MsgNotifyUpdate = 0x000c, // 新消息通知状态更新
	DisMemberKick = 0x000d,    // 群聊踢人（发起人权限） 
	DocumentDel = 0x000e,     // 文档删除
	CodeCreate = 0x000f,      // 代码创建
	CodeForward = 0x0010,     // 代码分享（转发）
	CodeDel = 0x0011,         // 代码删除
	CommentCreate = 0x0012,   // 评论创建
	CommentDel = 0x0013,      // 评论删除
	DisAvatarUpdate = 0x0014, // 群聊头像更新
	DisStateUpdate = 0x0015, // 讨论组会中状态更新
}
// 富媒体类型定义
enum ChatMediaType
{
  MediaTypeAudio = 0x0001,  // 录音
	MediaTypeImage = 0x0002,  // 图片
	MediaTypeVideo = 0x0003,  // 视频
	MediaTypeEmotion = 0x0004, // 大表情（消息体为MediaContent，将整个表情的内容进行base64编码后，赋值到缩略图字段media_thumb即可）
}
// 文本聊天内容格式定义
enum TextContentType
{
    PlainText = 0x01, // 纯文本格式
	  MimeText = 0x02, // mime格式
}

// 文本聊天
struct TextContent
{
	1:required byte type,        // 文本格式（纯文本、mime格式，见TextContentType枚举定义）  
	2:required string text,      // 文字内容
}
// 富媒体消息
struct MediaContent
{
	1:optional string media_filename,  // 文件名称
	2:required i32 media_filesize,      // 文件大小
	3:required ChatMediaType media_type,      // 文件类型mime_type，1：audio/amr 表示录音，2： image/jpeg //表示图片，3：video/mp4 表示视频
	4:required string media_down_url,  // 表示资源下载URL
	5:optional string media_thumb, // 缩略图（图片、视频有此项），将缩略图字节buffer经过base64算法加密生成此串
	6:optional i32 media_duration,      // 时长，语音、视频有此项
}

struct GroupMemberInfo{
  1:required i32 memberId,  //  成员ID
	2:required i32 join_time, //  加入时间
	3:optional byte join_state, // 加入状态 （默认0：加入，1：退出）
}
// 讨论组临时名称与正式名称的区别：1、临时名称会随成员的加入/退出而改变，正式名称不会随成员的加入或退出改变
// 2、通过发送消息创建的讨论组均为临时名称，通过讨论组名称修改功能更改过的讨论组名称变为正式名称
// 3、通过安排会议创建的讨论组默认使用会议主题作为正式名称
// 添加成员
struct GroupMemberAddContent
{
	1:required i32 operatorid,		//操作发起人用户ID
	2:required i32 groupid,			//讨论组ID(i32)
	3:required list<GroupMemberInfo> memberList, //新增成员的用户ID数组【数组类型定义规则】，数组元素类型为GroupMemberInfo
	4:optional string avatar,               // 讨论组头像（前四个人的ID串，用逗号分隔）
  5:optional string group_name,           // 讨论组名称
	6:optional string name_pinyin,          // 讨论组名称的拼音
	7:optional byte name_flag,              // 讨论组名称标志，0：临时名称，1：正式名称	
}
//退出讨论组
struct GroupMemberDelContent
{
	1:required i32 operatorid,		//操作发起人用户ID
	2:required i32 groupid,			//讨论组ID(i32)
	3:required i32 memberId, 		//退出成员用户ID
	4:optional string avatar,               // 讨论组头像（前四个人的ID串，用逗号分隔）
	5:optional string group_name,           // 讨论组名称
	6:optional string name_pinyin,          // 讨论组名称的拼音
	7:optional byte name_flag,              // 讨论组名称标志，0：临时名称，1：正式名称,
}
//修改讨论组名称
struct GroupNameUpdateContent
{
	1:required i32 operatorid,		//操作发起人用户ID
	2:required i32 groupid,			//讨论组ID(i32)
	3:required string group_name,   	//讨论组名称
	4:required string name_pinyin,          // 讨论组名称的拼音
	5:required byte name_flag,              // 讨论组名称标志，0：临时名称，1：正式名称,
}
// 创建新的讨论组
struct GroupCreateContent
{
	1:required i32 operatorid,		//操作发起人用户ID
	2:required i32 groupid,			//讨论组ID(i32)
	3:required list<GroupMemberInfo> memberList, //新增成员的用户ID数组【数组类型定义规则】，数组元素类型为GroupMemberInfo
	4:optional string avatar,               // 讨论组头像（项目自定义头像，群聊：前四个人的ID串，用逗号分隔）
	5:optional string group_name,           // 讨论组名称
	6:optional string name_pinyin,          // 讨论组名称的拼音
	7:required i32 create_time,             // 创建时间（精确到秒） 
	8:required i32 siteID,                  // 站点ID
	9:required byte name_flag,              // 讨论组名称标志，0：临时名称，1：正式名称,
	10:optional byte group_type,            // 讨论组类型
	11:optional i32 memberCount,            // 讨论组成员数量 
	12:optional i32 group_status,           // 讨论组状态（按位区分含义）
	13:optional string status_info,           // 讨论组状态扩展信息（JSON）
}

// 讨论组踢人消息体定义：此消息适用于讨论组&群聊&纯会议讨论组三种场景
struct GroupMemberKickContent{
    1:required i32 operatorid,            // 操作发起人用户ID
    2:required i32 groupid,               // 讨论组ID(i32)
    3:required list<i32> memberList,      // 被踢出讨论组的用户ID数组
    4:optional string avatar,             // 讨论组头像（前四个人的ID串，用逗号分隔）
    5:optional string group_name,         // 讨论组名称
    6:optional string name_pinyin,        // 讨论组名称的拼音
    7:optional byte name_flag,            // 讨论组名称标志，0：临时名称，1：正式名称,
}

// 讨论组头像更新-消息体定义：此消息仅适用于项目组场景
struct GroupAvatarUpdateContent{
    1:required i32 operatorid,            // 操作发起人用户ID
    2:required string avatar,             // 讨论组头像URL
    3:required i32 groupid,               // 讨论组ID
}

// 讨论组会中状态更新-消息体定义：适用于呼叫、即时会议等会中场景
struct GroupStateUpdateContent{
    // 按位区分含义：第1位表示是否结束（0：未结束，1：结束）
    // 第2位表示是否在呼叫中（0：未在呼叫中，1：呼叫中）
    1:required i32 group_status,          // 讨论组状态
    2:optional string status_info,         // 讨论组状态扩展信息（JSON）
}


// 讨论组置顶消息体-定义
struct GroupTopContent{
  1:required i32 operatorid,		// 操作发起人用户ID
	2:required i32 groupid,                 // 置顶的讨论组ID
	3:required byte isTop,                  // 是否置顶， 1：置顶，0：取消置顶
}

//@ 讨论组的文档上传（分享）消息体
struct SummaryCreateContent{
  1:required i32 operatorid,              // 操作发起人用户ID
	2:required string summary_name,         // 文档名称
	3:optional string down_url,             // 下载URL
	4:optional i32  size,                   // 文档大小（单位：字节）
	5:optional i64 contentId,               // 文档内容唯一标识
}

//@ 一对一文档发送（分享）消息体
struct DocumentContent{
  1:required i32 operatorid,              // 操作发起人用户ID
	2:required string doc_name,             // 文档名称
	3:required string down_url,             // 下载URL
	4:optional i32  size,                   // 文档大小（单位：字节）
	5:optional i64 contentId,               // 文档内容唯一标识
}

//@ 文档删除消息体
struct DocumentDelContent{
  1:required i32 operatorid,              // 操作发起人用户ID
  2:required i64 contentId,               // 文档唯一标识
  3:optional string doc_name,                // 文档名称     
}

//@代码创建（分享）消息体定义
struct CodeCreateContent{
    1:required i64 contentId,             // 代码唯一标识
    2:required string title,              // 代码标题               
    3:required string langType,           // 代码语言类型
    4:required string contentHead,        // 代码缩影（前10行代码）
    5:optional string desc,               // 代码描述
    6:required i32 size,                  // 代码占用空间大小（单位：字节）
    7:required i32 operatorid,            // 代码上传者用户ID
    8:optional string operatorName,       // 代码上传者用户姓名）
    9:required string downUrl,            // 代码下载超链接
    10:required i32 totalLines,           // 代码总行数
    11:required i64 createTime,           // 代码创建时间（单位：秒）
}

//@ 代码删除消息体
struct CodeDelContent{
  1:required i32 operatorid,              // 操作发起人用户ID
  2:required i64 contentId,               // 代码唯一标识
  3:optional string title,                // 代码标题名称     
}

//@ 发表评论消息体定义
struct CommentCreateContent{
 1:required i32 operatorid,               // 操作发起人用户ID
 2:optional string operatorName,          // 操作发起人用户姓名
 3:required i64 commentId,                // 评论ID
 4:required string commentDesc,           // 评论内容
 5:required i64 contentId,                // 内容ID
 6:required string contentName,           // 内容标题
 7:required byte contentType,             // 内容类型
 8:required i64 createTime,               // 评论时间
}

//@ 评论删除消息体定义
struct CommentDelContent{
 1:required i32 operatorid,               // 操作发起人用户ID
 2:required i64 commentId,                // 被删除的评论ID
 3:optional i64 contentId,                // 被删除评论所对应的内容ID
 4:optional string contentName,           // 被删除评论所对应的内容标题 
}

//@ 周期提醒规则定义,规则随意组合可以实现一周多天提醒
enum AlertRuleType{
  NoDay=0x00,                              // no day
  Monday=0x01,                            // Monday
  Tuesday=0x02,                           // Tuesday
  Wednesday=0x04,                          // Wednesday
  Thursday=0x08,                           // Thursday
  Friday=0x10,                             // Friday
  Saturday=0x20,                           // Saturday
  Sunday=0x40,                             // Sunday
}

//@ 项目组提醒规则更新消息体
struct AlertRuleContent{
  1:required i32 groupid,                 // 讨论组ID
  2:required byte state,                  // 提醒开启状态(0：关闭，1：打开) 
  3:required i32 rule,                    // 周期提醒规则（0为非周期提醒，大于等于1表示周期规则）
  4:required i32 time,                    // 首次提醒时间（精确到秒的时间戳，对于周期提醒，取小时+分钟与规则组成提醒时间） 
}

//@ 结束讨论组通知消息体
struct DiscussionClose{
  1:required i32 operatorId,              // 操作者ID
}

//@ 消息通知设置更新消息体
struct MsgNotifyContent{
  1:required i32 groupid,                 // 讨论组ID
  2:required byte state,                  // 新消息通知状态（0：不作通知，1：通知）
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 系统通知消息
/////////////////////////////////////////////////////////////////////////////////////////////////
enum SystemProtoMessageType{
	SystemWelcome = 0x01,  // 系统欢迎消息
	ClientUpdate = 0x02,   // 客户端更新消息
	CompleteInfoType = 0x03, // 完善个人信息类型
	PwdRuleUpdate = 0x04, // 密码规则变化通知
	clientInstallGuide = 0x05, // 客户端引导安装
	PasswordModify = 0x06, // 用户密码修改成功之后通知此用户其他客户端下线
	SensitiveWordDelete = 0x07, // 敏感词删除消息（此消息不需要消息体）
	GroupRemind = 0x08, // 项目组提醒消息（按照用户指定的规则来提醒）
}

// 系统通知-消息号定义
enum SystemMessageId{
	DefaultId=0x0001,// 默认
}

//@ 用户密码修改消息通知-消息体定义-此消息体无内容
struct PasswordModifyContent{
        1:optional string password, //  用户修改后的新密码
}
// 系统欢迎消息（此消息直接由客户端生成，暂不用服务器下发）
struct SystemWelcomeContent{
	1:required string title, // 消息标题
	2:required string image_url, //欢迎图片URL
	3:optional string desc, // 消息描述，此项暂无
	4:optional string target_link, // 图片超链接
}

enum ClientUpdateProtoMessageId{
	IphoneType = 0x0001,
	AndroidType = 0x0002,
	PCType = 0x0003,
}

// 客户端软件更新消息
struct ClientUpdateContent{
	1:required i32 version_code,       // 整型版本号
	2:required byte is_force_upgrade,  // 是否要强制更新（1：强制更新，0：非强制），重大版本升级需要强制更新
	3:optional i16 max_sdk_version,    // 运行允许最大SDK版本
	4:required i16 min_sdk_version,    // 运行所需最小SDK版本要求
	5:required string version_name,    // 版本字符串，如:1.0,1.1等
	6:required string desc,            // 更新描述
	7:required string url,             // 更新包下载URL
}

// 完善个人信息-消息号定义
enum CompleteInfoMessageId{
    tagAdd = 0x0001,  // 标签添加
    tagUpdate = 0x0002, // 标签更新
    tagDel = 0x0003, // 标签删除
}

// 待完善信息的输入类型
enum InputType{
	SingleLine = 0x0001, // 单行文本类型
	MultiLine = 0x0002, // 多行文本类型
	Number = 0x0003,  // 电话号码或数字类型
	Date = 0x0004, // 日期类型
}

// 待完善信息的标签定义
struct InfoCell{
	1:required i32 tagId,   // 标签ID(用于提交请求)
	2:required string label,  // 标签名称（用于显示）
	3:optional i16 inputType, // 待完善信息的输入类型(目前不用，均为单行文本）
	4:required i16 scope,     // 填写对象（0：所有人，1：管理员，2：员工填写）
	5:required i16 type,      // 标签类型（1：基本标签，2：自定义标签）
	6:required i16 state,     // 启用禁用（0：禁用，1启用）
	7:optional string value, // 标签默认值（管理员填写时，输入的默认名称通过此字段下发给所有客户端）
}

// 标签添加、标签更新——消息体
struct CompleteInfoContent{
	1:required list<InfoCell> info_cell_list, // 待完善信息的标签集合
	2:optional string desc, // 消息描述
}

// 标签删除消息体
struct TagDelContent{
  1:required list<i32> delTagIds, // 删除的标签ID集合
}

//@ 密码规则变化通知-消息号定义
enum PwdRuleUpdateMessageId{
	OutOfDateType = 0x0001, // 密码过期
	ComplexType = 0x0002, //密码复杂性变化
}

//@ 密码规则编号定义
enum PwdComplexType{
	type_1 = 0x0001, // 8~30位，不限制类型
	type_2 = 0x0002, // 8~30位，数字或字母组合
	type_3 = 0x0003, // 8~30位，数字、符号和字母组合
}

//@ 密码复杂性变化通知-消息体定义
struct PwdComplexUpdateContent{
    1:required PwdComplexType type, // 新的密码规则编号
}

//@ 密码到期通知-消息体定义
struct PwdExpireContent{
	// 密码即将到期时间（单位：天）
	// 正数表示距离到期还剩几天，负数表示已经过期几天，0表示已经到期
	1:required i32 dueDays,        
}

//@ 引导安装客户端-消息号定义
enum ClientGuideMessageId{
	MobileClientGuide = 0x0001, // 引导安装移动客户端消息号
	PCClientGuide = 0x0002, //  引导安装PC客户端消息号
}

//@ PC客户端引导安装消息
struct PCClientGuide{
    1:required string downUrl, // PC客户端下载链接
}

//@ 移动客户端引导安装消息
struct MobileClientGuide{
	1:required string androidMarketUrl, // android市场APP访问链接
	2:required string iphoneMarketUrl, //  iphone市场APP访问链接
}

//@ 项目组提醒消息体
struct GroupRemindContent{
  1:required list<i32> groupIdList, // 在该时刻提醒的讨论组
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件传输消息
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//@ 文件传输消息-消息类型定义
enum FileTransmissionMessageType{
    DefaultType = 0x01, // 默认消息类型
}

//@ 文件传输消息-消息号定义
enum FileTransmissionMessageId{
    TransInvite = 0x0001, // 传输邀请
	TransReceive = 0x0002, // 接受邀请
	TransReject = 0x0003, // 拒绝邀请
	TransStop = 0x0004, // 停止传输
}

//@ 分块信息定义
//@ 文件传输时，接收端是按分好的块儿取，取完一块儿数据用该块儿的MD5值进行验证
//@ 验证成功则继续取下一块儿，验证失败则表示数据传输错误，停止传输
// struct FileBlockInfo{
//  1:required string blockMD5, // 该分块的MD5值（每一个分块也当作一个文件来对待）
//	2:required i32    startByte, // 该分块在文件中的起始字节
//	3:required i32    endByte, // 该分块在文件中的结束字节
// }

//@ 文件传输消息-邀请消息体定义
struct FileTransmissionInviteContent{
    1:required string fileMD5, // 文件标识（MD5加密串）
	2:required string fileName, // 文件名称
	3:required i64    fileSize, // 文件大小（单位：字节）
	4:required i64    blockSize, // 文件分块大小（单位：字节）
	5:required byte   nZip, // 是否zip压缩（1：压缩，0：非压缩）
	6:required i64    taskId, // 发送端任务编号
	//3:required string fileType, // 文件类型，txt/pdf/doc/xls/ppt等
}

//@ 文件传输消息-同意传输消息体定义
struct FileTransmissionReceiveContent{
	1:required i64    sendTaskId,  // 发送端任务编号
	2:required i64    receiveTaskId, // 接收端任务编号
	3:required string ip,      // P2P方式传输时接收方IP地址       
	4:required i16 port,       // P2P方式传输时接收方端口号 
	5:required string serverList, // 中转服务器列表，多个中转服务器之间用分号分隔，例如http://192.168.40.140;http//192.168.40.140;
	6:required byte   transType, //传输类型，来自接收文件方法的返回值
	7:required i64    transPoint, // 传输点
}

//@ 文件传输消息-拒绝传输消息体定义
struct FileTransmissionRejectContent{
	1:required i64    taskId, // 发送端任务编号
	2:required string fileName, // 文件名称
	3:required i64    fileSize, // 文件大小（单位：字节）
}

//@ 文件传输消息-停止传输消息体定义
//@ 任何一方停止传输，都需要把发送端任务ID和接收端任务ID一并赋值，没有则传0
//@ flag是UI上用于区分是发送端停止还是接收端停止
struct FileTransmissionStopContent{
	1:required i64    sendTaskId, // 发送端的任务id
	2:required i64    receiveTaskId, // 接收端任务ID
	3:required string fileName, // 文件名称
	4:required i64    fileSize, // 文件大小
	5:required byte   flag, // 0x00, 发送端停止，0x01:接收端停止
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 组织消息
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 组织消息-消息类型定义
enum OrganizeProtoMessageType{
    OrganizeType = 0x01,              // 组织变动
}

// 组织变动消息的消息号定义
enum OrganizeProtoMessageId{
	DeptUpdate = 0x0001,          // 部门名称变更
	DeptTransfer = 0x0002,            // 员工部门调动
	PositionUpdate = 0x0003,          // 职位调整
	EmployeeEntry = 0x0004,           // 员工入职
	EmployeeLeave = 0x0005,           // 员工离职
	EmployeeRightsUpdate = 0x0006,    // 员工权限变更
	EmployeeJoin = 0x0007,            // 部门加入新员工(由员工部门调动引起的一类消息),此类消息已经删除，用员工部门调动消息代替
	DeptParentChange = 0x0008,        // 部门移动(一个部门移动到另一个部门作为这个部门的子部门)
	CompanyJoin = 0x0009,             // 员工被指定为生态企业员工
        DeptDelete = 0x000a,              // 部门删除
	EmployeeEntryConfirm = 0x000b,    // 员工入职确认
	EmployeeLeaveConfirm = 0x000c,    // 员工离职确认
	DeptTransferConfirm  = 0x000d,    // 员工部门调动确认
	EmployeeEntryReject = 0x000e,    // 员工入职拒绝消息(与员工入职确认消息公用一个消息体)
	EmployeeLeaveReject = 0x000f,    // 员工离职拒绝消息(与员工离职确认消息公用一个消息体)
	DeptTransferReject  = 0x0010,    // 员工部门调动拒绝消息(与员工部门调动确认消息公用一个消息体)
}

//员工部门调动确认(或拒绝)消息体内容
//由系统管理员发出确认和取消操作,部门管理者收到这个消息
struct DeptTransferConfirmContent{
	1:required i32 operator_id, // 操作者ID
	2:required string user_name, // 用户姓名
        3:optional i32 dept_id, // 部门ID
        4:required string dept_name, // 部门名称
	5:required i32 user_id, // 用户ID
	6:optional string desc,   // 描述
}
//员工离职确认(或拒绝)消息体内容
//由系统管理员发出确认和取消操作,部门管理者收到这个消息
struct EmployeeLeaveConfirmContent{
	1:required i32 operator_id, // 操作者ID
	2:required string user_name, // 用户姓名
        3:optional i32 dept_id, // 部门ID
        4:required string dept_name, // 部门名称
	5:required i32 user_id, // 用户ID
	6:optional string desc,   // 描述
}
//员工入职确认(或拒绝)消息体内容
//由系统管理员发出确认和取消操作,部门管理者收到这个消息
struct EmployeeEntryConfirmContent{
	1:required i32 operator_id, // 操作者ID
	2:required string user_name, // 用户姓名
        3:optional i32 dept_id, // 部门ID
        4:required string dept_name, // 部门名称
	5:required i32 user_id, // 入职用户ID
	6:optional string desc,   // 描述
}
// 员工被加入生态企业
struct CompanyJoinContent{
	1:required i32 operator_id, // 操作者ID
	2:optional i32 company_id, // 生态企业ID
	3:required string company_name, //  生态企业名称
	4:required i32 user_id, // 用户ID
	5:required string user_name, // 用户姓名
	6:optional string desc,   // 描述
}

// 部门删除
// 部门上级部门的管理者收到部门已删除消息，上级部门的一般用户不会收到相关消息。
struct DeptDeleteContent{
	1:required i32 operator_id, // 操作者ID
	2:required i32 dept_id, // 部门ID
	3:required string dept_name, // 部门名称
	4:required string parent_dept_name,  // 上级部门名称
	5:optional string desc,   // 描述
}

// 部门移动
// 系统管理员完成操作，新旧部门上级部门的管理者都会收到部门异动消息，部门管理者收到消息，部门内其他用户不会收到消息
struct DeptParentChangeContent{
	1:required i32 operator_id, // 操作者ID
	2:required i32 dept_id, // 部门ID
	3:required string dept_name, // 移动的部门名称
	4:required string old_dept_name, // 原上级部门名称
	5:required string new_dept_name,  // 新上级部门名称
	6:optional string desc,   // 描述
}
// 部门名称变更
// 系统管理员完成操作，部门内所有用户都收到消息
struct DeptUpdateContent{
	1:required i32 operator_id, // 操作者ID
	2:required i32 dept_id, // 部门ID
	3:required string old_dept_name, // 原部门名称
	4:required string new_dept_name,  // 新部门名称
	5:optional string desc,   // 描述
}
// 用户部门调动
// 系统管理员完成操作，新旧部门管理者接收调岗完成消息。用户当事人接收调岗完成消息。
struct DeptTransferContent{
	1:required i32 operator_id, // 操作者ID
	2:required i32 user_id, // 用户ID
	3:required string user_name, // 用户姓名
	4:required i32 dept_id, // 新部门ID
	5:optional string old_dept_name, // 原部门名称
	6:required string dept_name, // 新部门名称
	7:optional string desc, // 消息描述
}

// 用户职位调整,员工本人收到消息
struct PositionUpdateContent{
	1:required i32 operator_id, // 操作者ID
	2:required i32 user_id, // 用户ID
	3:required string user_name, // 用户姓名
	4:required string new_position, // 新职位名称
	5:optional string old_position, // 旧职位名称
	6:required string dept_name, // 部门名称
	7:optional string desc, // 消息描述
}

// 员工入职,入职员工所在部门的每一个员工都会收到消息
struct EmployeeEntryContent{
	1:required i32 operator_id, // 操作者ID
	2:required i32 dept_id, // 部门ID
	3:required string user_name, // 员工姓名
	4:required string dept_name, // 员工部门名称
	5:required string position, // 员工职位名称
	7:required i32 user_id, // 员工用户ID
	6:optional string desc, // 消息描述

}

// 员工离职
// 只有离职员工（包含部门管理者）所在部门的部门管理者会收到消息，其他（包括员工本人）均不会收到消息
struct EmployeeLeaveContent{
	1:required i32 operator_id, // 操作者ID
	2:required i32 user_id, // 员工用户ID
	3:required string user_name, // 员工姓名
	4:required string dept_name, // 员工部门名称
	5:optional string desc, // 消息描述
}
// 员工权限变更（成为部门领导）
// 员工被指定为部门领导后发出此消息，该部门的所有员工包括部门领导均会收到此消息，取消部门领导不会发出任何消息。
struct EmployeeRightsChangeContent{
	1:required i32 operator_id, // 操作者ID
	2:required i32 user_id, // 员工用户ID
	3:required string user_name, // 员工姓名
	4:required string dept_name, // 部门名称
	5:required i32 dept_id, // 部门ID
	6:optional string desc, // 消息描述
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 会议消息
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 //@会议消息类型
enum ConfMessageType{
	NormalInstantConfInviteMsg=0x01,//  单人即时会议邀请消息
	ConfInviteMsg=0x02,//	预约会议邀请消息
	ConfUpdateMsg=0x03,//	预约会议更新消息
	ConfCancelMsg=0x04,//	预约会议取消消息
	ConfForwardMsg=0x05,//  预约会议转发消息
	ConfAlertMsg=0x06,//    预约会议提醒消息
	ConfShareMsg=0x07, //   预约会议授权消息
	DiscussionInstantConfInviteMsg = 0x08, // 多人即时会议邀请消息
	ConfChatMsg=0x09,  //   纯会议讨论消息
	ConfSummaryMsg=0x0a,  // 预约会议纪要消息
	ConfStateMsg=0x0b, // 会中状态消息
}

//@即时会议邀请消息-消息号定义(单人和讨论组共用此定义） 
enum InstantConfInviteMessageId{
	IQInvite=0x0001,   // 邀请
	IQReceive=0x0002,  // 接受
	IQReject=0x0003,   // 拒绝
}

//@即时会议邀请消息（单人或讨论组）
struct InstantIQInviteContent{
	1:required i32 confId,//会议ID
	2:required i32 hostId,//主持人ID
	3:required string confPwd,//会议密码
	4:optional byte isReserve, // 是否是预约会议,适用于会中邀请场景（1：预约，0：默认为即时）
}

//@接受邀请（单人或讨论组）
// 该消息为预留，目前无此需求
struct InstantIQReceiveContent{
	1:required i32 confId,//会议ID
	2:optional byte isReserve, // 是否是预约会议,适用于会中邀请场景（1：预约，0：默认为即时）
}

//@拒绝邀请（单人或讨论组）
struct InstantIQRejectContent{
	1:required i32 confId,//会议ID
	2:optional byte isReserve, // 是否是预约会议,适用于会中邀请场景（1：预约，0：默认为即时）
}

//@预约会议邀请消息-消息号定义
enum ConfInviteMessageId{
	IQInvite=0x0001,//  邀请
	IQReceive=0x0002,//	接受
	IQReject=0x0003,//	拒绝
}


//@会议邀请（安排或更新或转发）
struct IQInviteContent{
	1:required i32 confId,//会议ID
	2:optional i32 shareUserID,//授权人的ID
	3:required i32 hostId,//主持人ID
	4:required i32 groupID,// 关联讨论组ID
	5:required string icalendar,// icalendar协议字符串
	6:required byte isRecurrent, // 是否是周期会议（1:周期，0：单场会议）
	7:optional i32 startTime, // 会议开始时间
	// 8:optional string forwardToIds, // 转发目标ID串
}

//@接受邀请（适用于安排、更新及转发场景）
struct IQReceiveContent{
	1:required i32 confId,//会议ID
	2:optional i32 shareUserID,//授权人的ID
	3:optional i32 forwardUserID, // 转发消息的用户ID，接受转发会议邀请时需要填充
	4:optional i32 startTime, // 会议开始时间（接受周期会议的单场时传此属性）
	5:required i32 hostId, // 主持人ID
	6:optional string icalendar,// icalendar协议字符串
}

//@拒绝邀请（适用于安排、更新及转发场景）
struct IQRejectContent{
	1:required i32 confId,//会议ID
	2:optional i32 shareUserID,//授权人的ID
	3:optional i32 forwardUserID, // 转发消息的用户ID，拒绝转发会议邀请时需要填充
	4:optional i32 startTime, // 会议开始时间（拒绝周期会议的单场时传此属性）
	5:required i32 hostId, // 主持人ID
	6:optional string icalendar,// icalendar协议字符串
}


 //@预约会议更新消息-消息号定义（同预约会议邀请消息）
 //@ 此枚举已不建议使用，请使用ConfInviteMessageId
enum ConfUpdateMessageId{
	IQInvite=0x0001,//  邀请
	IQReceive=0x0002,//	接受
	IQReject=0x0003,//	拒绝
}

//@预约会议更新消息-消息体定义（同预约会议邀请消息）

//@预约会议转发消息-消息号定义（同预约会议邀请消息）

//@预约会议转发消息-消息体定义 （同预约会议邀请消息）

//@预约会议取消消息-消息号定义
enum ConfCancelMessageId{
	DefaultId=0x0001,// 取消单场会议
	RecurrentConfCancel=0x0002, // 取消周期会议
}


//@预约会议取消消息
struct ConfCancelContent{
	1:required i32 confId,//会议ID
	2:optional i32 shareUserID,//授权人的ID
	3:optional i32 startTime, // 会议开始时间(取消单场会议时赋此属性，通过该属性可以判断是取消周期中具体某一场）
	4:optional string icalendar,// icalendar协议字符串
}

//@预约会议提醒消息-消息号定义	
enum ConfAlertMessageId{
	FifteenMinutesAlert=0x0001,// 会前15分钟提醒
	ZeroMinutesAlert=0x0002, // 会议即将开始提醒
	ConfStartAlert=0x0003, // 会议已开始（用于更新会议状态）
	ConfEndAlert=0x0004, // 会议已结束（用于更新会议状态）
	PartJoinAlert=0x0005, // 参会人加入会议提醒（此消息只给该帐号的其它在线终端发送，并不给其它参会人发送）
	PartQuitAlert=0x0006, // 参会人退出会议提醒（此消息只给该帐号的其它在线终端发送，并不给其它参会人发送）
}

//@ 入会人员角色
enum RoleType{
	Host = 0x04, // 主持人
	Speaker = 0x05, // 主讲人
	Part = 0x00, // 普通参会人
}


//@预约会议提醒消息-消息体定义（会议开始、结束及参会人入会、退会均用此消息）
struct ConfAlertContent{
	1:required i32 confId,//会议ID
	2:optional i32 shareUserID,//授权人的ID
	3:optional byte isInstant, // 是否为即时会议（1：即时，0：非即时）
	4:optional i32 startTime, // 会议开始时间，对周期会议的会前提醒、会议开始、结束、及参会人入会退会消息有此属性
	5:optional RoleType role, // (人员入会退会消息有此属性，人员角色）
	6:optional string icalendar,// icalendar协议字符串
}

//@预约会议讨论消息消息号定义
enum ConfChatMessageId{
  TextType = 0x01,  // 文字消息或图文混排消息 
  MediaType = 0x02, // 富媒体消息
}

//@ 纯会议讨论消息
struct ConfChatContent{
  1:required i32 confId, // 讨论所属会议ID
  2:optional i32 startTime, // 会议开始时间
  3:optional TextContent text, // 文本消息体
  4:optional MediaContent media, // 富媒体消息体
   
  // 以后有其它消息继续加 
}

//@ 预约会议纪要消息-消息号定义
enum ConfSummaryMessageId{
  SummaryCreate = 0x01, // 纪要上传消息
  SummaryUpdate = 0x02, // 纪要更新消息
  SummaryForward = 0x03, // 纪要转发消息   
}

//@ 会议纪要状态-枚举
enum ConfSummaryState{
  StateNew = 0x01, // 新建状态（只有创建人可见，其它人不可见）
  StatePublish = 0x02,// 发布状态（所有参会人成员均可见）
  StateDelete = 0x03, // 删除状态 （所有人均不可见）
}
//@ 会议纪要消息体（适用于上传、更新、转发会议纪要场景）
struct ConfSummaryContent{
  1:required i32 operatorid,              // 操作发起人用户ID
  2:required i32 summary_id,              // 会议纪要ID
	3:required string summary_name,         // 会议纪要名称
	4:required byte state,                  // 会议纪要状态（新建、发布、删除）
	5:required i32 confId,                  // 会议纪要所属会议ID
	6:optional string confName,             // 会议名称
	7:optional i32 startTime,               // 会议开始时间
	8:optional i32 updateTime,              // 更新时间	
}

//@ 会中状态消息
enum ConfStateMessageId{
  callState = 0x01,  // 外呼状态通知消息
   
}

//@ 外呼状态消息定义
struct CallStateContent{
   1:required i32 userId,      // 外呼号码所属用户ID 
   2:required string partyId,  // 给用户分配的会中参会人ID
   3:required string callSeq,         // 外呼序号
   4:required i32 oldCallState,    // 外呼旧状态
   5:required i32 newCallState,    // 外呼新状态
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 日历消息
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//@日历授权消息类型定义
enum CalendarMessageType{
        CalendarShareType = 0x0007;
}

//@日历授权消息-消息号定义
enum ConfShareMessageId{
	ConfShareCreate=0x0001,// 设置会议授权
	ConfShareCancel=0x0002, // 取消会议授权
	ConfShareGrantorDisable=0x0003, //  授权者帐号停用
	ConfShareGranteeDisable=0x0004, //  被授权者帐号停用
	ConfShareGranteeTransfer=0x0005, // 被授权者调岗
	ConfShareGrantorLeave=0x0006, // 授权者离职
	ConfShareGranteeLeave=0x0007, // 被授权者离职
}

//@日历授权消息（归为日历应用）
struct ConfShareContent{
	// 会议授权消息目前均不需要消息体
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 音频聊天消息
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//@ 音频聊天消息-消息类型定义
enum AudioChatMessageType{
	DefaultType = 0x01, // 单人音频聊天类型，默认消息类型
	AudioCallRecord = 0x02, // 音频呼叫记录，音频呼叫结束后由发起方（即主持人）来发送一条音频呼叫记录。归属到一对一聊天来显示
	AudioUnfamiliarCallRecord = 0x03, // 陌生号码呼叫记录，由发起人自己发给自己，单独在消息列表首页来显示，点击直接进入拨号
	DiscussionType = 0x04,  // 项目组音频聊天
	GroupType = 0x05,    // 群音频聊天
}

//@ 音频聊天消息-消息号定义(适用于一对一、项目组、群聊）
enum AudioChatMessageId{
	AudioInvite = 0x0001, // 音频聊天邀请（一对一为控制消息，项目组/群聊场景入库显示）
	AudioReceive = 0x0002, // 接受音频聊天邀请（控制消息）
	AudioReject = 0x0003, // 拒绝音频聊天邀请（一对一入库显示，项目组/群聊为控制消息）
	AudioStop = 0x0004, // 停止音频聊天（控制消息，一对一收到此消息直接退出，组场景停止方自己退出，其它人更新状态）
  AudioCancelCall = 0x0005, // 一对一场景接收方未接听时发起方主动挂断（一对一场景）
  AudioBlockCall = 0x0006, // 未接通的呼叫记录（一对一场景）
	AudioRejectCall = 0x0007, // 拒绝的呼叫记录（废弃）
	AudioRecentCall = 0x0008, // 音频通话时长记录（一对一、组都适用）
}

struct AudioInviteContent{
	1:required string confId, // 会议ID
	2:required i32 hostId, // 会议主持人ID
	3:required string confPwd, // 会议密码
}

struct AudioReceiveContent{
    1:required string confId, // 会议ID
    2:required i32 hostId, // 会议主持人ID
}

struct AudioRejectContent{
    1:required string confId, // 会议ID
    2:required i32 hostId, // 会议主持人ID
}

struct AudioStopContent{
    1:required string confId, // 会议ID
    2:required i32 hostId, // 会议主持人ID
}

//@ 音频呼叫记录消息-消息号定义（废弃）
//@ 音频呼叫结束后由发起方（即主持人）来发送一条音频呼叫记录消息，详情规则见UC通讯协议说明。
enum AudioCallMessageId{
	AudioBlockCall = 0x0001, // 未接通的呼叫记录
	AudioRejectCall = 0x0002, // 拒绝的呼叫记录
	AudioRecentCall = 0x0003, // 音频通话时长记录
}

// @ 未接通的呼叫-消息体定义
struct AudioBlockCallContent{
    1:optional string phoneNum, // 陌生电话号码（只有陌生号码呼叫记录才需要填此属性） 
}

// @ 拒绝的呼叫-消息体定义
struct AudioRejectCallContent{
    1:optional string phoneNum, // 陌生电话号码（只有陌生号码呼叫记录才需要填此属性） 
}


// @ 音频呼叫通话时长记录-消息体定义
struct AudioRecentCallContent{
    1:required i32 duration, // 通话时长，单位：秒  
	  2:optional string phoneNum, // 陌生电话号码（只有陌生号码呼叫记录才需要填此属性）
}

// @音频呼叫发起方主动挂断-消息体定义(一对一场景接收方未接听时发起方主动挂断）
struct AudioCancelCallContent{
    1:optional string phoneNum, // 陌生电话号码（只有陌生号码呼叫记录才需要填此属性） 
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 通讯录更新消息
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//@ 通讯录消息协议类型定义
enum AddressBookMessageType{
	CardUpdateMsg=0x01, // 名片变更消息
	ContacterRelationUpdateMsg=0x02, // 联系人关系更新消息
}

//@ 名片更新消息消息号定义
enum CardUpdateMessageId{
    DefaultId=0x0001, // 默认
}
//@ 联系人关系变更消息号定义
enum ContacterRelationUpdateMessageId{
	ContacterAdd=0x0001, // 联系人关系添加
	ContacterDel=0x0002, // 联系人关系解除
}
//@ 名片更新消息体定义
struct CardUpdateContent{
	1:required i32 userId, // 发生名片变更的用户ID
	2:required i32 cardVersion, // 联系人名片版本号
}
//@ 联系人关系变更消息体定义
struct ContacterAddContent{
        // 暂无属性
}

struct ContacterDelContent{
        // 暂无属性
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 工作圈消息
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//@ 工作圈动态更新消息类型定义
enum TimeNewsMessageType{
	   DefaultType=0x01, // 目前没有类型，暂定一默认类型
}

//@ 工作圈动态更新消息号定义 
enum TimeNewsMessageId{
     WB_ATTENTION=0x01, // 关注通知
     WB_TIMENEWS_PUBLISH=0x02, // 动态更新发布通知
     WB_TIMENEWS_COMMENT=0x03, // 动态更新评论通知
     WB_TIMENEWS_FORWARD=0x04, // 动态更新转发通知
     WB_TIMENEWS_PRAISE=0x05, // 动态更新赞通知
}

//@ 工作圈案例发布消息类型定义
enum CaseMessageType{
	   DefaultType=0x01, // 目前没有类型，暂定一默认类型
}

//@ 工作圈案例发布消息号定义 
enum CaseMessageId{
     WB_CASE_PUBLISH=0x01, // 案例发布通知
     WB_CASE_COMMENT=0x02, // 案例评论通知
     WB_CASE_FORWARD=0x03, // 案例转发通知
     WB_CASE_PRAISE=0x04, // 案例赞通知
}

//@ 工作圈调研投票消息类型定义
enum PollMessageType{
	   DefaultType=0x01, // 目前没有类型，暂定一默认类型
}

//@ 工作圈调研投票消息号定义 
enum PollMessageId{
     WB_POLL_PUBLISH=0x01, // 调研发布通知
     WB_POLL_COMMENT=0x02, // 调研评论通知
     WB_POLL_FORWARD=0x03, // 调研转发通知
     WB_POLL_RESPONSE=0x04, // 调研投票通知
     WB_POLL_PRAISE=0x05, // 调研赞通知
}

//@ 工作圈动态更新消息体定义
//@ 工作圈案例发布消息体定义
//@ 工作圈调研投票消息体定义
//@ 动态更新、案例发布、调研投票共用此消息体定义
struct WBMessageContent{
     1:required string text, // 消息内容描述 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// 外部API消息
///////////////////////////////////////////////////////////////////////////////////////////////////////


//@ API消息类型定义
enum APIMessageType{
     AppMsg = 0x01, // 默认类型
}

//@ API消息号定义
enum APIMessageId{
     Text = 0x01,   // 文本消息
}

//@ API文本消息详情类型定义
enum APITextDetailType{
     TextType = 0x00, // 文本类型
     URLType = 0x01,  // URL链接类型
}

//@ API文本消息体定义
struct APITextContent{     
     1:optional string title,       // 消息标题
     2:required string content,     // 消息内容
     3:optional byte detailType,    // 详情类型（见APITextDetailType枚举定义）
     4:optional string detailContent, // 详情信息
}
//
//应用id	值	描述
//聊天消息	0x0001	IM聊天类业务
//心跳消息	0x0002	系统中的心跳处理业务
//状态消息	0x0003	联系人状态处理业务
//系统通知	0x0004	软件系统的相关消息，比如：软件有更新之类
//文件传输	0x0005	文件传输业务
//日历	        0x0006	日历业务
//会议	        0x0007	会议业务
//组织管理	0x0008	系统中用户的组织结构业务
//通知消息	0x0009	软件的用户方发起的消息，比如：某公司通知大家放假之类的
//Ack消息	0x000a	消息状态应答sent, received, read 消息
//音频聊天消息	0x000b	音频聊天消息
//通讯录更新消息 0x000c 通讯录更新消息，包括名片变更、联系人关系更新等
// 睡眠消息     0x000d  睡眠消息，客户端请求睡眠，服务器响应醒来
//第三方应用消息 0x0100以后递增	第三方接入的应用消息
//应用id
enum AppId {
	AppChat = 0x0001,//聊天消息
	AppHeartbeat = 0x0002,//心跳消息
	AppPresence = 0x0003,//状态消息
	AppNotify = 0x0004,//系统通知
	AppFiletransfer = 0x0005,//文件传输
	AppCalendar = 0x0006,//日历
	AppMeeting = 0x0007,//会议
	AppOrganization = 0x0008,//组织管理
	AppCommon_notify = 0x0009, // 普通通知消息
	AppAck = 0x000a, // 消息状态应答sent, received, read 消息
	AppAudio = 0x000b,//音频聊天
	AppAddressBook = 0x000c, // 通讯录更新消息
	AppSleep = 0x000d, // 睡眠消息
	AppTimeNews=0x000e, // 工作圈动态更新
	AppCase=0x000f, // 工作圈案例检测
	AppPoll=0x0010, // 工作圈调研投票
	AppAPI=0x0100,  // 对外API消息类型
}

//@JID,标识一个终端设备
struct JID{
	1:required i32 userID,//账号ID、群ID、讨论组ID
	2:required i32 siteID,//用户属于哪个部点，站点ID
	3:required i32 resID,//资源ID
}
//@uc message消息头定义
struct UcMessageHead{
	 1:required i16  version=0x0100,//协议版本号
	 2:required i16  appid,//应用id
	 3:required byte pri=0,//消息属性，见PriType枚举定义
	 4:required byte protocoltype,//协议类型
	 5:required i16  protocolid,//消息号
	 6:required i32  id,//本地消息id
	 7:required i64  seq,//全局seq
	 8:required i64  timestamp,//消息时间戳（timestamp,精确到毫秒）
	10:required JID from,//消息发送者
	11:required JID to,//消息接收者
	12:required i32 conversation, // 会话ID
	13:required i32 length, // body长度（单位：字节）
	14:optional JID toPrivate, // 讨论组内发私信的接收者
	15:optional byte controlPri=0x0f,  // 扩展字段，用于表示消息控制属性（见ControlPriType枚举定义）
	16:optional i16 channelPri,   // 扩展字段，用来表示消息属于哪个频道（见ChannelPriType枚举定义）
}

//uc message消息定义
struct UcMessageBody{
	//编号15~50
	15:optional MessagePresence presence,// 状态消息
	16:optional MessagePing ping, // 心跳消息
	20:optional AckSent ackSent, // 已发回执
	21:optional AckReceived ackReceived, // 已达回执
	22:optional AckRead ackRead, // 已读回执
	23:optional SleepSet sleep, // 睡眠或醒来请求
	24:optional WakeupResult wakeupConfirm, // 服务器确认醒来
	25:optional AckPlay ackPlay, // 已播放回执AckPlay
	// 编号50~100为聊天消息预留
	50:optional TextContent text,                   // 聊天文字消息
	51:optional MediaContent media,                 // 聊天多媒体消息
	52:optional GroupMemberAddContent groupAdd,     // 讨论组添加成员
	53:optional GroupMemberDelContent groupDel,     // 成员退出讨论组
	54:optional GroupNameUpdateContent groupUpdate, // 讨论组名称更新
	55:optional GroupCreateContent groupCreate,     // 讨论组创建
	56:optional GroupTopContent    groupTop,        // 讨论组置顶消息体
	57:optional SummaryCreateContent summaryCreate, // 文档上传消息体
	58:optional DocumentContent    docSend,         // 文档发送消息体
	59:optional AlertRuleContent   alertRule,       // 提醒规则更新消息体
	60:optional DiscussionClose    groupClose,      // 结束讨论组消息体
	61:optional MsgNotifyContent   msgNotify,       // 消息免打扰消息体
	62:optional GroupMemberKickContent groupKick,   // 讨论组踢人消息体
	63:optional CodeCreateContent codeCreate,       // 代码创建消息体
	64:optional CodeDelContent    codeDel,          // 代码删除消息体
	65:optional DocumentDelContent docDel,          // 文档删除消息体
	66:optional CommentCreateContent commentCreate, // 评论创建消息体
	67:optional CommentDelContent commentDel,       // 评论删除消息体
	68:optional GroupAvatarUpdateContent groupAvatarUpdate, // 项目组头像更新消息
	69:optional GroupStateUpdateContent groupState, // 讨论组状态更新消息
	// 编号100～149为系统通知消息预留
	100:optional SystemWelcomeContent welcome, // 系统欢迎消息
	101:optional ClientUpdateContent clientUpdate, // 客户端更新消息
	102:optional CompleteInfoContent completeInfo, // 标签添加、标签更新消息体
	103:optional PwdComplexUpdateContent pwdComplexUpdate, // 密码复杂性变化
	104:optional PwdExpireContent pwdExpire, // 密码到期通知
	105:optional PCClientGuide pcGuide, // PC客户端引导安装消息（移动客户端收到此消息）
	106:optional MobileClientGuide mobileGuide, // 移动客户端引导安装（PC客户端收到此消息）
	107:optional PasswordModifyContent pwdModify, // 用户密码修改消息通知（用户的所有终端收到此消息）
	108:optional TagDelContent tagDel, // 标签删除消息体
	109:optional GroupRemindContent remind, // 项目组提醒消息体
	// 编号150～199为组织消息预留
	150:optional DeptUpdateContent deptUpdate, // 部门名称变更
	151:optional DeptTransferContent deptTransfer, // 员工部门调动
	152:optional PositionUpdateContent positionUpdate, // 员工职位调整
	153:optional EmployeeEntryContent employeeEntry, // 员工入职
	154:optional EmployeeLeaveContent employeeLeave, // 员工离职
	155:optional EmployeeRightsChangeContent employeeRights, // 员工权限变更
	156:optional DeptParentChangeContent deprtParentChange, // 部门移动
	157:optional CompanyJoinContent companyJoin, // 员工加入生态企业
	158:optional DeptDeleteContent deptDelete, // 部门删除
	159:optional EmployeeEntryConfirmContent entryConfirm, // 员工入职确认消息
	160:optional EmployeeLeaveConfirmContent leaveConfirm, // 员工离职确认消息
	161:optional DeptTransferConfirmContent deptConfirm, // 员工部门调动确认消息
	// 编号200～249为会议消息预留
	200:optional InstantIQInviteContent instantInvite, // 即时会议邀请消息
	201:optional IQInviteContent confInvite, // 预约会议邀请
	202:optional IQReceiveContent confReceive, // 预约会议接受邀请
	203:optional IQRejectContent confReject, // 预约会议拒绝邀请
	204:optional ConfCancelContent confCancel, // 预约会议取消
	205:optional ConfAlertContent confAlert, // 会前提醒消息
	207:optional InstantIQReceiveContent instantReceive, // 即时会议接受邀请消息
	208:optional InstantIQRejectContent instantReject, // 即时会议拒绝邀请消息
	209:optional ConfChatContent confChat, // 纯会议讨论消息
	210:optional ConfSummaryContent confSummary, // 会议纪要消息
	211:optional CallStateContent callState, // 外呼状态消息
	// 编号250～299为通讯录应用预留
	250:optional CardUpdateContent cardUpdate, // 名片变更消息
	251:optional ContacterAddContent contacterAdd, // 联系人关系建立
	252:optional ContacterDelContent contacterDel, // 联系人关系移除
	// 编号300~349为文件传输消息预留
	300:optional FileTransmissionInviteContent transInvite, // 传输邀请
	301:optional FileTransmissionReceiveContent transReceive, // 同意传输
	302:optional FileTransmissionRejectContent transReject, // 拒绝传输
	303:optional FileTransmissionStopContent transStop, // 拒绝传输
	// 编号350~399为音频聊天消息预留
	350:optional AudioInviteContent audioInvite, // 音频聊天邀请
	351:optional AudioReceiveContent audioReceive, // 同意音频聊天
	352:optional AudioRejectContent audioReject, // 拒绝音频聊天
	353:optional AudioStopContent audioStop, // 停止音频聊天   
	354:optional AudioRecentCallContent callRecent, // 音频通话时长呼叫记录
	355:optional AudioBlockCallContent  callBlock, // 未接通的音频呼叫记录
	356:optional AudioRejectCallContent callReject, // 拒绝的音频呼叫记录
	357:optional AudioCancelCallContent callCancel, // 一对一场景发起方主动取消呼叫（接收方未接听时）
	// 编号400~449为日历消息预留
  400:optional ConfShareContent confShare, // 日程授权（共享）消息
  // 编号450~600为工作圈消息预留
  450:optional WBMessageContent wbContent, // 工作圈动态更新、案例发布、调研投票消息体
  
  // 编号10000以上全部为第三方应用的消息体
  10000:optional APITextContent apiText,    // API第三方应用文本消息
}

//消息服务
service ucService {
  string SendUcMessage(1:UcMessageHead head,2:UcMessageBody body)

}
