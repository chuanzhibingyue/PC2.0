/*
 * UCSDK.h
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#ifndef UCSDK_H_
#define UCSDK_H_

#include "common/platform.h"

#define OUT
#define IN

#if defined(PLATFORM_WIN)
#pragma warning( disable: 4251 )
#pragma warning( disable: 4819 )
#endif

/**
 * A macro for quelling compiler warnings about unused variables.
 */
#ifndef UNUSED_PARAM
#  define UNUSED_PARAM(p) ((void)&(p))
#endif

/**
 * 定义客户端类型
 */
#define UCCLIENT_IOS		(1) // iPhone & iPod touch
#define UCCLIENT_ANDROID	(2)
#define UCCLIENT_WIN		(3)
#define UCCLIENT_XXX		(4)
#define UCCLIENT_IPAD		(5)
#define UCCLIENT_WEB		(6)

/**
 * 定义HTTP请求相关信息
 */
#define HTTP_METHOD_GET		"GET"
#define HTTP_METHOD_POST	"POST"

// 针对连接建立
#define HTTP_TIMEOUT	30000

//针对request
#define HTTP_RESPONSE_TIMEOUT	90000

// 针对request, -1 表示不设置，也就是无限
#define HTTP_RESPONSE_INFINITY -1

enum UCServiceType {
	UCClientService = 1,
	UCContactService,
	UCFileTransferService,
	UCIMService,
	UCGroupService,
	UCAudioService,
	UCVideoService,
	UCCalendarService,
	UCSharingService,
	UCSearchService,
	UCOpenApiService,
	UCMicroblogService,
	UCExternalService,
	UCBBSService,
	UCP2PService,
	UCPbxService
};

/**
 * 聊天类型;
 *SINGLE_CHAT			单人聊天;
 *GROUP_CHAT			群组聊天;
 *PROJECT_CHAT			项目组聊天;
 *CONFERENCE_CHAT		会议组聊天;
 *ORG_CHAT				组织消息
 *CLOUD_CHAT			云盘讨论组消息
 *WELCOME				欢迎消息
*/
enum ConversationType {
	NONE_CHAT = -1,
	CONFERENCE_CHAT = 0,
	PROJECT_CHAT,
	GROUP_CHAT,
	SINGLE_CHAT,
	OPENAPI_CHAT,
	ORG_CHAT,
	CLOUD_CHAT,
	WELCOME = 10
};

namespace Revocation{
enum RevocationMsgType{
	SINGLE_TYPE = 0,  //表示p2p
	GROUP_TYPE,		//表示group
	ORG_TYPE,		//表示org
	AppId_TYPE,     //表示外部应用appid
};
}

enum MessageChannel {
	UCAS = 1,  //实时消息;
	LIST,      //消息列表;
	CHAT,      //会话聊天界面;
	HISTORY,   //历史消息界面;
	SEARCH,    //搜索消息界面;
};

enum FileSendType {
	FILE_SEND = 1,
	FILE_FORWARD,
};

enum DocDirType {
	AUDIO_DIR = 1,
	VIDEO_DIR,
	IMAGE_DIR,
	FILE_DIR,
	AVATAR_DIR
};

enum DocumentCloudType {
	CT_FILE_TRANSFER = 0,
	CT_GoKuai,
	CT_AnZhen
};

enum CloudFileOperation {
	CFO_NONE = -1,
	CFO_CloudFileCreate = 0,
	CFO_CloudFileUpdate,
	CFO_CloudFileDel,
	CFO_CloudFileMove,
	CFO_CloudFileRename,
};

#if defined(PLATFORM_WIN)
    #define PBX_SERVICE 1
    #define SHARE_SERVICE 1
    #define AUDIO_SERVICE 1
    #define VIDEO_SERVICE 1
#endif

#endif /* UCSDK_H_ */
