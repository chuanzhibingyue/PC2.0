/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file InternalMessage.h
* @brief Definites a base class for all the internal messages in Tang Client.
* 
* @author Xiaogang Yu (xiaogang.yu@gnetis.com)
*
* @date Feb. 4 2010
*
* @version 1.0.0
* 
* @warning null
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug 

******************************************************************************/

#ifndef GNET_TANG_CLIENT_INTERNALMESSAGE_H
#define GNET_TANG_CLIENT_INTERNALMESSAGE_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <Message.h>
#include <conferencedef.h>

/**<Channel managerment：0x0000-0x00FF*/
#define INTERNEL_CHANNEL_AUTH_RESULT                (0x0000)    /**<Channel authentication result. */
#define INTERNEL_CHANNEL_EVENT                      (0x0001)    /**<Channel event: connected, disconnected. */
#define INTERNEL_CHANNEL_STATISTICS                 (0x0002)    /**<Channel networking statistics. */
#define INTERNEL_VIDEO_CHANNEL_STATISTICS           (0x0008)    /**<Channel VEDIO networking statistics. */
#define INTERNEL_AUDIO_CHANNEL_STATISTICS           (0x0009)    /**<Channel AUDIO networking statistics. */
#define INTERNEL_CHANNEL_TIMEOUTEVENT               (0x0003)    /**<Channel CtrlMessage Timeout Notify */
#define INTERNEL_CHANNEL_BWADJUSTMENTEVENT          (0x0004)    /**<Channel bandwidth adjust Notify */
#define INTERNEL_CHANNEL_RECVINFOEVENT              (0x0005)    /**<Channel Receive msg info Notify */
#define INTERNEL_CHANNEL_QUCKLOSTRATENOTIFY         (0x0007)    /**<Channel quick lost rate Notify */

/**<会议管理内部消息ID：0x0100-0x01FF*/
#define INTERNAL_MESSAGE_ID_CONFERENCE_SYSTEM_LOCK  (0x0100)    /**<会议系统锁内部消息ID*/
#define INTERNAL_MESSAGE_ID_USER_EXIT_CONFERENCE    (0x0101)    /**<用户退会内部消息ID*/
#define INTERNAL_MESSAGE_ID_USER_STATUS_CHANGED     (0x0102)    /**<用户状态变更内部消息ID*/
#define INTERNAL_MESSAGE_ID_USER_ROLE_CHANGED       (0x0103)    /**<用户角色变更内部消息ID*/
#define INTERNAL_MESSAGE_ID_DTS_CHANNEL_INFO		(0x0104)    /**<DTS通道信息内部消息*/
#define INTERNAL_MESSAGE_ID_USER_TDC_READY			(0x0105)    /**<DTS通道信息内部消息*/

#define INTERNAL_MESSAGE_ID_DOWNLOAD_PLUGIN_FAILED	(0x0106)    /**<下载插件失败内部消息*/
#define INTERNAL_MESSAGE_ID_SETTING_CHANGE	        (0x0107)    /**<桌面设置改变内部消息*/
#define INTERNAL_MESSAGE_ID_SCREENLOCK_CHANGE	    (0x0108)    /**<锁屏状态改变内部消息*/

#define INTERNAL_MESSAGE_ID_UPLOADLOGSTATUS         (0x0151)	/**<日志上传状态*/
#define INTERNAL_MESSAGE_ID_UPLOADLOGPROGRESS       (0x0152)	/**<日志上传进度内部消息*/
#define INTERNAL_MESSAGE_ID_LOGFILEURL				(0x0153)	/**<日志上传成功后在服务器上的url*/


#ifdef _DEBUG
#define INTERNAL_MESSAGE_ID_SEND_MESSAGE_REPORT     (0x0014)
#endif




/**<白板内部消息ID：0x0200-0x02FF*/
#define INTERNAL_MESSAGE_ID_STARTCOMMENT                    (0x0201)    // 开启批注
#define INTERNAL_MESSAGE_ID_STARTCOMMENT_NOTIFY             (0x0202)    // 成功开启批注
#define INTERNAL_MESSAGE_ID_STARTCOMMENT_ERR_NOTIFY         (0x0203)    // 开启批注失败

#define INTERNAL_MESSAGE_ID_STOPCOMMENT                     (0x0205)    // 关闭批注
#define INTERNAL_MESSAGE_ID_STOPCOMMENT_NOTIFY              (0x0206)    // 成功关闭批注
#define INTERNAL_MESSAGE_ID_STOPCOMMENT_ERR_NOTIFY          (0x0207)    // 关闭批注失败

#define INTERNAL_MESSAGE_ID_TURNOVERCOMMENT                 (0x0209)    // 翻页批注
#define INTERNAL_MESSAGE_ID_TURNOVERCOMMENT_NOTIFY          (0x0210)    // 成功翻页批注
#define INTERNAL_MESSAGE_ID_TURNOVERCOMMENT_ERR_NOTIFY      (0x0211)    // 翻页批注失败

#define INTERNAL_MESSAGE_ID_SAVECOMMENT                     (0x0213)    // 保存批注
#define INTERNAL_MESSAGE_ID_SAVECOMMENT_NOTIFY              (0x0214)    // 保存批注成功
#define INTERNAL_MESSAGE_ID_SAVECOMMENT_ERR_NOTIFY          (0x0215)    // 保存批注失败

#define INTERNAL_MESSAGE_ID_PRINTCOMMENT                    (0x0217)    // 打印批注
#define INTERNAL_MESSAGE_ID_PRINTCOMMENT_NOTIFY             (0x0218)    // 打印批注成功
#define INTERNAL_MESSAGE_ID_PRINTCOMMENT_ERR_NOTIFY         (0x0219)    // 打印批注失败

#define INTERNAL_MESSAGE_ID_SHOWCOMMENT                     (0x0221)    // 显示批注
#define INTERNAL_MESSAGE_ID_SHOWCOMMENT_NOTIFY              (0x0222)    // 显示批注成功
#define INTERNAL_MESSAGE_ID_SHOWCOMMENT_ERR_NOTIFY          (0x0223)    // 显示批注失败

#define INTERNAL_MESSAGE_ID_CHECKCOMMENTFIGURE              (0x0225)    // 查询批注是否有图形
#define INTERNAL_MESSAGE_ID_CHECKCOMMENTFIGURE_NOTIFY       (0x0226)    // 查询批注是否有图形成功
#define INTERNAL_MESSAGE_ID_CHECKCOMMENTFIGURE_ERR_NOTIFY   (0x0227)    // 查询批注是否有图形失败

#define INTERNAL_MESSAGE_ID_CHECKCOMMENTEXIST               (0x0229)    // 查询批注是否存在

// 文档共享内部信令ID：0x0300-0x03FF
#define INTERNAL_MESSAGE_ID_FSS_ONPAGE              (0x0300)    // onpage
#define INTERNAL_MESSAGE_ID_FSS_ONPAGECOUNT         (0x0301)    //OnPageCount
#define INTERNAL_MESSAGE_ID_FSS_ONCONVERTSTATUS     (0x0302)    //OnConvertStatus
#define INTERNAL_MESSAGE_ID_FSS_ONUPLOADPROCESS            (0x0303)    //上传文档进度


/**<升级更新内部消息ID：0x0400-0x04FF*/
#define INTERNAL_MESSAGE_CHECK_UPDATE_PROGRESS		(0x0400)	// 检查新版本进度
#define INTERNAL_MESSAGE_DOWNLOAD_PROGRESS			(0x0401)	// 下载进度
#define INTERNAL_MESSAGE_INSTALL_PROGRESS			(0x0402)	// 安装进度
#define INTERNAL_MESSAGE_UPDATE_ERROR				(0x0403)	// 错误消息
#define INTERNAL_MESSAGE_NEED_UPDATE				(0x0404)	// 是否需要升级

/**<桌面共享的内部消息：0x500-0x5FF*/
#define INTERNAL_MESSAGE_SHARE_MOUSE_POS            (0x0501) // 共享端的鼠标位置
#define INTERNAL_MESSAGE_SHARE_RFB_HEADER            (0x0502) // 共享端的鼠标位置
#define INTERNAL_MESSAGE_SHARE_DISPLAY_VIEW			(0x0503)	//共享端显示窗口的消息
#define INTERNAL_MESSAGE_RESET_VIEW_SIZE			(0x0504)	//分辨率更改后，重设显示区域大小
#define INTERNAL_MESSAGE_REMOVE_INSTANCE			(0x0505)	//清理本地过期的共享实例
#define INTERNAL_MESSAGE_VNC_CONN_TIMEOUT			(0x0506)	//等待VNC连接进来超时的通知消息

/**<音视频模块的内部消息：0x600-0x6FF*/
#define INTERNAL_MESSAGE_AUDIOENGINE_NOTIFY            (0x0601) // 音频通知事件
#define INTERNAL_MESSAGE_VIDEOENGINE_NOTIFY            (0x0602) // 视频通知事件
#define INTERNAL_MESSAGE_TRANSBAD_NOTIFY			   (0x0603) //传输特性内部消息
#define INTERNAL_MESSAGE_VIDEO_RTCP					   (0x0604) //RTCP处理内部消息
#define INTERNAL_MESSAGE_VIDEO_RENDER				   (0x0605) //render window内部消息
#define INTERNAL_MESSAGE_VIDEO_DELAYRENDER			   (0x0606) //延时render window内部消息
#define INTERNAL_MESSAGE_AUDIO_ACTIVESPEAKING_NOTIFY   (0x0607) //是否正在说话通知内部消息
#define INTERNAL_MESSAGE_AUDIO_MICSPEAKERLEVEL_NOTIFY  (0x0608) //音量变化通知内部消息
#define INTERNAL_MESSAGE_AUDIO_MICLEVEL_NOTIFY		   (0x0609) //音量变化通知内部消息
#define INTERNAL_MESSAGE_AUDIO_SPEAKERLEVEL_NOTIFY	   (0x060A) //音量变化通知内部消息

/**<消息队列服务内部消息：0x700-0x7FF*/
// #define INTERNAL_MESSAGE_MQ_LOGIN_RESULT				(0x0701) //登录MQ结果
#define INTERNAL_MESSAGE_MQ_SENDMSG_FAIL				(0x0702) //发送MQ消息失败
// #define INTERNAL_MESSAGE_MQ_DISCONNECT					(0x0703) //MQ断线
#define INTERNAL_MESSAGE_MQ_NEWMSG_NOTIFY				(0x0704) //MQ新消息通知
#define INTERNAL_MESSAGE_JS_NEWMSG_NOTIFY				(0x0705) //JS新消息通知
#define INTERNAL_MESSAGE_UPDATE_SESSIONDATA				(0x0706) //更新会话数据

/**< 会话服务内部消息： */
#define INTERNAL_MESSAGE_CREATESESSION_REQUEST          (0x0711)    // 创建会话请求
#define INTERNAL_MESSAGE_CREATESESSION_RESPONSE         (0x0712)    // 创建会话回应
#define INTERNAL_MESSAGE_CREATESESSION_RESULT_NOTIFY    (0x0713)    // 创建会话结果通知
#define INTERNAL_MESSAGE_CLOSESESSION_NOTIFY            (0x0714)    // 销毁会话通知
#define INTERNAL_MESSAGE_HIDESESSION_NOTIFY             (0x0715)     // 隐藏会话通知

#define INTERNAL_MESSAGE_P2P_TRANS_PROGRESS             (0x0721) //文件传输回调内部消息
#define INTERNAL_MESSAGE_UPLOAD_TRANS_PROGRESS          (0x0722) //上传进度回调内部消息
#define INTERNAL_MESSAGE_DOWNLOAD_TRANS_PROGRESS        (0x0723) //下载进度回调内部消息

#define INTERNAL_MESSAGE_AJAXREQUEST_CALLBACK           (0x0731) // JS Ajax请求回调消息

#define INTERNAL_MESSAGE_SYS_OSLEAVE_CHANGE             (0x0801) //OS离开状态通知
#define INTERNAL_MESSAGE_SYS_HASFULLSCREEN_CHANGE       (0x0802) //全屏检测状态通知
#define INTERNAL_MESSAGE_CATCHSCREEN_NOTIFY             (0x0803) //截图完成通知
#define INTERNAL_MESSAGE_ZIPLOG_NOTIFY                  (0x0804) //压缩日志完成通知

#define INTERNAL_MESSAGE_MEETING_REMIND_NOTIFY		    (0x0805) //会议提醒设置消息通知
#define INTERNAL_MESSAGE_GROUP_REMIND_NOTIFY			(0x0806) //讨论组提醒设置消息通知

#define INTERNLA_MESSAGE_HTTP_RESPONSE                  (0x0807) // http 响应消息

#define INTERNAL_MESSAGE_CONNECT_NOTIFY                 (0x0808) //UCAS连接状态消息
/*
* @class InternalMessage
* @brief Definite a base class for all the internal messages in Tang Client.
* 
* All the internal messages should inherit from this class in Tang Client.
* 
* @author Xiaogang Yu
* @version 1.0.0
*/
class InternalMessage : public Message
{
public:
    InternalMessage()
    {
        // this field identify as tang internal message.
        msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
    }

    virtual ~InternalMessage(){}

    UINT32          internalMsgType;   /**< The internal message unique identification in client. */
};

/*
* @class ConferenceSystemLockMessage 会议系统加锁内部消息
* @brief when the administrator add a system for the conference,this message will be used to notify other
*  modules that a system lock is added.
* 
* This message is inherited from InternaMessage base class .
* 
* @author hongwei.hu
* @version 1.0.0
*/
class ConferenceSystemLockMessage:public InternalMessage
{
public:
    ConferenceSystemLockMessage()
    {
        //message content.
        internalMsgType = INTERNAL_MESSAGE_ID_CONFERENCE_SYSTEM_LOCK;

        confID = 0x0000;
    }

    UINT32               confID;
};

/*
* @class UserExitConferenceMessage 用户退会内部消息
* @brief this message used to notify other business modules that
*  user is exited the conference.
* 
* This message is inherited from InternaMessage base class .
* 
* @author hongwei.hu
* @version 1.0.0
*/
class UserExitConferenceMessage:public InternalMessage
{
public:
    UserExitConferenceMessage()
    {
        //message content.
        internalMsgType = INTERNAL_MESSAGE_ID_USER_EXIT_CONFERENCE;

        userIdList.clear();
    }

    std::vector<UINT32>               userIdList;
};

/*
* @class UserStatusChangedMessage 用户状态变更内部消息
* @brief this message used to notify other business modules that
*  the status of user is changed.
* 
* This message is inherited from InternaMessage base class .
* 
* @author hongwei.hu
* @version 1.0.0
*/
class UserStatusChangedMessage:public InternalMessage
{
public:
    UserStatusChangedMessage()
    {
        //message content.
        internalMsgType = INTERNAL_MESSAGE_ID_USER_STATUS_CHANGED;

        userID = 0x0000;
    }

    UINT32               userID;
    UserStatus           status;
};

/*
* @class UserRoleChangedMessage 用户角色变更内部消息
* @brief this message used to notify other business modules that
*  user's role is changed.
* 
* This message is inherited from InternaMessage base class .
* 
* @author hongwei.hu
* @version 1.0.0
*/
class UserRoleChangedMessage:public InternalMessage
{
public:
    UserRoleChangedMessage()
    {
        //message content.
        internalMsgType = INTERNAL_MESSAGE_ID_USER_ROLE_CHANGED;

        uRoleIDList.clear();
    }

    UINT32                          userID;
    std::vector<UINT32>             uRoleIDList;
};

/*
* @class DtsChannelInfoNotify Dts通道信息内部消息
* @brief this message used to notify other business modules that
*  Dts channel info is ready which module required.
* 
* This message is inherited from InternaMessage base class .
* 
* @author hongwei.hu
* @version 1.0.0
*/
class DtsChannelInfoNotify:public InternalMessage
{
public:
	DtsChannelInfoNotify()
	{
		//message content.
		internalMsgType = INTERNAL_MESSAGE_ID_DTS_CHANNEL_INFO;

		uGroupId = 0x0000;
		uChannelId = 0x0000;
	}

	UINT32                          uGroupId;
	UINT32							uChannelId;
};

/*
* @class UserListTdcReadyNotify 用户列表tdc初始化内部消息
* @brief this message used to notify other business modules that
*  user list tdc is initialized.
* 
* This message is inherited from InternaMessage base class .
* 
* @author hongwei.hu
* @version 1.0.0
*/
class UserListTdcReadyNotify:public InternalMessage
{
public:
	UserListTdcReadyNotify()
	{
		//message content.
		internalMsgType = INTERNAL_MESSAGE_ID_USER_TDC_READY;
	}
};

/*
* @class DownloadPluginFailedNotify 用户列表tdc初始化内部消息
* @brief this message used to notify conference Manager that
*  the specific plugin download failed.
* 
* This message is inherited from InternaMessage base class .
* 
* @author hongwei.hu
* @version 1.0.0
*/
class CreateServiceFailedNotify:public InternalMessage
{
public:
    CreateServiceFailedNotify()
    {
        //message content.
        internalMsgType = INTERNAL_MESSAGE_ID_DOWNLOAD_PLUGIN_FAILED;

        uServiceType = 0;
        hRet = 0;
    }

    UINT32                          uServiceType;
    HRESULT                         hRet;
};

/*
* @class DesktopSettingChangeNotify 用户列表tdc初始化内部消息
* @brief this message used to notify conference Manager that
*  desktop setting is changed.
* 
* This message is inherited from InternaMessage base class .
* 
* @author hongwei.hu
* @version 1.0.0
*/
class DesktopSettingChangeNotify:public InternalMessage
{
public:
    DesktopSettingChangeNotify()
    {
        //message content.
        internalMsgType = INTERNAL_MESSAGE_ID_SETTING_CHANGE;
    }
};

/*
* @class ScreenLockChangeNotify 锁屏状态改变内部消息
* @brief this message used to notify conference Manager that
*  ScreenLock status is changed.
* 
* This message is inherited from InternaMessage base class .
* 
* @author huajian.yin
* @version 1.0.0
*/
class ScreenLockChangeNotify:public InternalMessage
{
public:
	ScreenLockChangeNotify()
	{
		//message content.
		m_iStatus = 0;//lockstatus：1：lock；0：unlock
		internalMsgType = INTERNAL_MESSAGE_ID_SCREENLOCK_CHANGE;
	}
public:
	UINT32 m_iStatus;
};

class LogUploadStatus:public InternalMessage
{
public:
	LogUploadStatus()
	{
		m_iStatus = 0;
		msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
		internalMsgType = INTERNAL_MESSAGE_ID_UPLOADLOGSTATUS;
	}

public:
	UINT32 m_iStatus;
};

class LogUploadProgress:public InternalMessage
{
public:
	LogUploadProgress()
	{
		m_iProgress = 0;
		msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
		internalMsgType = INTERNAL_MESSAGE_ID_UPLOADLOGPROGRESS;
	}

public:
	UINT32 m_iProgress;
};

class LogFileUrl:public InternalMessage
{
public:
	LogFileUrl()
	{
		m_strUrl = L"";
		msgType = TANG_CLIENT_MESSAGE_TYPE_INTERNAL;
		internalMsgType = INTERNAL_MESSAGE_ID_LOGFILEURL;
	}

public:
	std::wstring m_strUrl;
};

#ifdef _DEBUG
class SendReportMessage:public InternalMessage
{
public:
    SendReportMessage(BYTE* lpSendBuffer, UINT32 uBufferSize)
    {
        internalMsgType = INTERNAL_MESSAGE_ID_SEND_MESSAGE_REPORT;

		m_lpSendBuffer = lpSendBuffer;
		m_uBufferSize = uBufferSize;
    }

	~SendReportMessage()
	{
        if (NULL != m_lpSendBuffer)
        {
            delete m_lpSendBuffer;
            m_lpSendBuffer = NULL;
        }
	}

	UINT32 m_uBufferSize;
	BYTE* m_lpSendBuffer;
};
#endif

#endif //GNET_TANG_CLIENT_INTERNALMESSAGE_H