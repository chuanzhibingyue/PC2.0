/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd.

******************************************************************************/
/**
* @file GlobalDefine.h
* @brief Defined all the global variable for Tang client.
*
* @author Hongwei Hu (hongwei.hu@gnetis.com)
*
* @date Janu. 25 2010
*
* @version 1.0.0
*
* @warning
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug

******************************************************************************/

#ifndef GNET_TANG_CLIENT_GLOBALDEFINE_H
#define GNET_TANG_CLIENT_GLOBALDEFINE_H

/**
* @enum TangServiceType
* @brief Tang service type definition.
* which business will be used.
*/
enum TangServiceType
{
    NetServiceType = 0,         /**< Web */
    ConferenceServiceType = 1,  /**< Conference */
	MaxServiceType = 10,		/**< max共享内存使用 */
    AudioServiceType = 258,     /**< Audio */
    VideoServiceType = 259,     /**< Video */
    WhiteboardServiceType = 775,/**< White board */
    DocumentServiceType = 776,  /**< 文档共享 */
    DesktopServiceType = 777,   /**< 桌面共享 */
    ChatServiceType = 516,   /**< 聊天 */
    QaServiceType = 517,   /**< 问答 */
    SurveyServiceType = 518,   /**< 调查 */
	UcServiceType = 800,   /**< UC */
    AllServiceType = -1         /**< 全部 */
};

/** 
* @enum TANG_SERVICE_STATUS
* @brief The status of service.
*/
typedef enum
{
    SERVICE_STATUS_IDLE = 0,        /**< Service is not start. */
    SERVICE_STATUS_OPEN_PENDING,	/**< Service is starting. */
    SERVICE_STATUS_OPEN_FAILED,     /**< Service start failed. */
    SERVICE_STATUS_OPEN,			/**< Service started successfully and ready to use. */
    SERVICE_STATUS_ERROR,			/**< Service can not work yet. */
    SERVICE_STATUS_CLOSED			/**< Service is already closed. */
} TANG_SERVICE_STATUS;/**< The status of service. */

/**
* @enum TangChannelType
* @brief Tang channel type definition.
* which business will be used.
*/
enum TangChannelType
{
    TANG_CHANNEL_TYPE_CONTROL    = 0,
    TANG_CHANNEL_TYPE_DISTRIBUTE = 1
};

/**
* @define adapter name macros
* @brief This is adapter name string.
* @{ */
#define ADAPTOR_NAME_MAINWND                   L"MainWnd"				   /**< name string of main frame adapter */
#define ADAPTOR_NAME_UTILITY                   L"Util"					   /**< name string of Utility adapter */
//daming: 2011-08-10 名字由Session改成sessions
#define ADAPTOR_NAME_SESSION                   L"sessions"				   /**< name string of session adapter */
#define ADAPTOR_NAME_CONFMGR                   L"ConferenceAdaptor"        /**< name string of conference adapter */
#define ADAPTOR_NAME_DOCSHARING                L"DocSharingAdaptor"        /**< name string of document sharing adapter */
#define ADAPTOR_NAME_DESKTOPSHARING            L"DesktopSharingAdaptor"    /**< name string of desktop sharing adapter */
#define ADAPTOR_NAME_WHITEBOARD                L"WhiteBoardAdaptor"        /**< name string of white board adapter */
#define ADAPTOR_NAME_AUDIO					   L"AudioAdaptor"			   /**< name string of audio service adapter */
#define ADAPTOR_NAME_VIDEO                     L"VideoAdaptor"             /**< name string of video service adapter */
#define ADAPTOR_NAME_PHONE                     L"PhoneAdaptor"             /**< name string of phone module adapter */
#define ADAPTOR_NAME_CHAT                      L"ChatAdaptor"              /**< name string of chat module adapter */
#define ADAPTOR_NAME_QA                        L"QaAdaptor"                /**< name string of qa module adapter */
#define ADAPTOR_NAME_SURVEY                    L"SurveyAdaptor"            /**< name string of survey module adapter */
#define ADAPTOR_NAME_UPDATE                    L"UpdateAdaptor"            /**< name string of update module adapter */

#define ADAPTOR_NAME_FILETRANSPORT             L"FileTranspost"            /**< name string of filetransport module adapter */

#define DISPATCH_NAME_PHONELIST            	   L"PhoneList"                /**< name string of phone user list dispatch object */

/**<define service dispatch object name for UI rebuild on 2011-5-18 */
#define  DISPATCH_NAME_CONFMGR                    L"conference"
#define  DISPATCH_NAME_DOC_SHARE                  L"docShare"
#define  DISPATCH_NAME_DESKTOP_SHARE              L"desktopShare"
#define  DISPATCH_NAME_WHITEBOARD                 L"whiteBoard"
#define  DISPATCH_NAME_AUDIO                      L"audio"
#define  DISPATCH_NAME_VIDEO                      L"video"
#define  DISPATCH_NAME_POHONE                     L"phone"
#define  DISPATCH_NAME_CHAT                       L"chat"
#define  DISPATCH_NAME_QA                         L"QA"
#define  DISPATCH_NAME_SURVEY                     L"survey"
#define  DISPATCH_NAME_USERLIST                   L"userList"               /**< name string of user list dispatch object */
#define  DISPATCH_NAME_PHONE_LIST		          L"phoneList"
#define	 DISPATCH_NAME_EVENTCENTER                L"eventCenter"			/**< name string of event center */
#define	 DISPATCH_NAME_UC_MAIN				      L"uc"				        /**< name string of UC */
#define	 DISPATCH_NAME_UC_CHAT				      L"uc"				        /**< name string of UC */
#define	 DISPATCH_NAME_UCCLIENT_SESSIONLIST       L"ucclientsessionlist"    /**< session list of ucclient */
#define  DISPATCH_NAME_AUDIOWIZARD                L"audioWizard"
#define  DISPATCH_NAME_VIDEOWIZARD                L"videoWizard"
/**
* @define module name macros for native dispatch implementation
* @brief This is adapter name string.
* @{ */
#define MODULE_NAME_CLIENT_HOST                L"TangClient"				/**< module name string for client host */
#define MODULE_NAME_WHITEBOARD				   L"plugin\\Whiteboard"		/**< module name string for whiteboard */
#define MODULE_NAME_AVMODULE	               L"plugin\\AVModule"			/**< module name string for audio and video */
#define MODULE_NAME_DOC_SHARE	               L"plugin\\DocSharing"		/**< module name string for doc share */
#define MODULE_NAME_TEXT		               L"plugin\\Text"				/**< module name string for chat qa and suery */
#define MODULE_NAME_DESKTOP_SHARE		       L"plugin\\DesktopSharing"	/**< module name string for desktop share */
#define MODULE_NAME_UPDATE				       L"Update"	                /**< module name string for desktop share */
#define	MODULE_NAME_UC						   L"uc"				        /**< module name string forUC */
#define	MODULE_NAME_CONFERENCE				   L"plugin\\Conference"		/**< module name string for conference */
#define MODULE_NAME_AVWIZARD	               L"plugin\\AVWizard"			/**< module name string for audio and video Wizard */

enum TangStatus
{
	TANG_STATUS_CHANNEL_AUTH_SUCCESSED = 200,
	TANG_STATUS_CHANNEL_AUTH_FAILED = 400,
};

/**
* @enum UCChatLoadType
* @brief UCChat创建后加载类型.
*/
enum UCChatLoadType
{
    UCChatLoadType_Chat = 1,      /**< 聊天类型 */
    UCChatLoadType_Meeting = 2,   /**< 会议类型 */
    UCChatLoadType_Phone = 3,     /**< 电话类型 */
};

/** 
* @enum SessionIDType
* @brief 会话ID类型定义
*/
enum SessionIDType
{
    SessionIDType_UserID        = 1,    /**< 用户ID类型*/
    SessionIDType_GroupID       = 2,    /**< 群ID类型*/
    SessionIDType_SeminarID     = 3,    /**< 讨论组ID类型*/
    SessionIDType_ConferenceID  = 4,    /**< 会议ID类型*/
};

/**
* @brief filter the hresult value to common error code to application layer.
* @param [in] void.
* @return HRESULT return common error code if the hresult is defined in tangerrorcode.h,
*  otherwise return the original value.
*/
inline HRESULT HResult_To_App_Layer(HRESULT hr)
{
	HRESULT hrRes = 0;
	if ( SUCCEEDED(hr) )
	{
		hrRes = 200;
	}
	else
	{
		// 去掉最高位，因为js的数值类型是无符号的，否则js会报错
		hrRes = hr&(0x0FFFFFFF);
	}
	return hrRes;
};

#define SAFE_DELETE(ptr) \
{ \
    if ((ptr) != NULL) \
{ \
    delete (ptr); \
    (ptr) = NULL; \
} \
}

//
// "SAFE_DELETE_ARRAY" is a macro which safely deletes the array.
//

#define SAFE_DELETE_ARRAY(ptr) \
{ \
    if ((ptr) != NULL) \
{ \
    delete[] (ptr); \
    (ptr) = NULL; \
} \
}

//
// "SYSFREESTRING" is a macro which safely does a SysFreeString
//

#define SYSFREESTRING(ptr) \
{ \
    if ((ptr) != NULL) \
{ \
    SysFreeString((ptr)); \
    (ptr) = NULL; \
} \
}

//
// "SYSALLOCSTRING" is a macro which safely does a SysAllocString
//

#define SYSALLOCSTRING(ptr, str) \
{ \
    (ptr) = SysAllocString((str)); \
}
#endif  //GNET_TANG_CLIENT_GLOBALDEFINE_H