/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file ConferenceDef.h
* @brief Defined all the conference data for Tang client.
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

#ifndef GNET_TANG_CLIENT_CONFERENCEDEF_H       
#define GNET_TANG_CLIENT_CONFERENCEDEF_H

#define IP_LENGTH 4            /**< define the length of IP address*/

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <Windows.h>
#include <list>
#include <vector>
#include <tchar.h>

//config value definition of conference manager.
#define CONFMGR_IS_SUPPORT 0    //是否支持控制服务
#define CONFMGR_PRIVILEGE 14    //权限值

//conference privilege definition of conference manager
#define CONFMGR_PRIVILEGE_KICK_MASK             0x0001  //踢人
#define CONFMGR_PRIVILEGE_CHANGEROLE_MASK       0x0002  //修改角色
#define CONFMGR_PRIVILEGE_CHANGECONFIG_MASK     0x0004  //修改会议配置
#define CONFMGR_PRIVILEGE_SERVICE_MASK          0x0008  //服务操作
#define CONFMGR_PRIVILEGE_SUBCONF_MASK          0x0010  //子会议操作
#define CONFMGR_PRIVILEGE_LOCK_MASK             0x0020  //锁会
#define CONFMGR_PRIVILEGE_BLACK_MASK            0x0040  //黑人操作
#define CONFMGR_PRIVILEGE_STOP_MASK             0x0080  //停会操作
#define CONFMGR_PRIVILEGE_CUSTOMEMSG_MASK       0x0100  //自定义消息
#define CONFMGR_PRIVILEGE_CHANNEL_MASK          0x0200  //请求通道信息
#define CONFMGR_PRIVILEGE_CONFIGDATA_MASK       0x0400  //修改配置数据
#define CONFMGR_PRIVILEGE_CHANGETHEME_MASK      0x0800  //修改会议主题

//conference service config simulator definition
#define CONF_SERVICE_CONFIG_TYPE(x)		        ((x>>24)&0x0F)  //配置类型
#define CONF_SERVICE_CONFIG_VALUE(x)	        (x&0x007FFFFF)  //配置值

/**
* @enum AppExecuteMode
* @brief Tang client run mode.
* which business will be used.
*/
enum AppExecuteMode
{
    PlayMode            = 0,    // 观看模式
    WebActive           = 1,    // 会议模式
    LoginMode           = 2,    // 登入模式
    OfflineMode         = 3,    // 脱机模式
    SetupMode           = 4,    // 安装模式
    CrashInvokeMode     = 5,    // 崩溃拉起模式
    UpdateRestartMode   = 6,    // 安装更新重启模式
};

/**
* @enum TangServiceStatus
* @brief Tang service status definition.
* which business will be used.
*/
enum TangServiceStatus
{
    ServiceClosed = 0,       // 关闭,未开启状态
    ServiceStarted = 1       // 开启状态
};

/**
* @enum TangConferenceStatus
* @brief Tang service status definition.
* which business will be used.
*/
enum TangConferenceStatus
{
	Started			= 0,			// 开始状态
	Stoped			= 1,			// 停止状态
    NotInit         = 512,          // The first status, not initialized
	//Ended			= 2,			// 结束状态	//comment by hongwei.hu,not support this status.
};

/**
* @enum RoleActionType
* @brief action type of change one user's role.
* which business will be used.
*/
enum RoleActionType
{
	RAT_ADD			= 1,			// 添加用户角色
	RAT_REMOVE		= 2,			// 删除用户角色
	RAT_UPDATE		= 3,			// 更新用户角色
};

/**< User list TDC header (property).
----------|--------|----------|-------|-----------|-------|-------|--------|--------|----------------|--------------------|---------------------|-------------------------|--------------------|---------|---------|--------------|-------|---------|---------|---------|----------------|---------------|----------------
rowIndex  |	userId | userName |isSelf |clientType |domain |Status |EarNum  |MikeNum |IsFollowStarter |IsVideoShareStarter |IsAudioShareStarter	| IsVideoShareFileStarter |IsVideoPollStarter  |IsPoller |IsPollee |CurrentPollee |IsMute |phoneNum |isBound  |cameraNum|isCameraStarted |hasChatMessage |isActiveSpeaker 
----------|--------|----------|-------|-----------|-------|-------|--------|--------|----------------|--------------------|---------------------|-------------------------|--------------------|---------|---------|--------------|-------|---------|---------|---------|----------------|---------------|----------------
*/
enum UserProperty
{
	rowIndex = 1,
	userId,
	userName,
	isSelf,
	clientType,
	domain,
	status,
	earNum,
	mikeNum,
	isFollowStarter,
	isVideoShareStarter,
	isAudioShareStarter,
	isVideoShareFileStarter,
	isVideoPollStarter,
	isAllowShareVideo,
	isPoller,
	isPollee,
	currentPollee,
	isMute,
	phoneId,
	phoneNum,
	isBound,
	isMutePhone,
	cameraNum,
	isCameraStarted,
	hasChatMessage,
	isVoipSpeaking,
	muteOperator,
	email,
	mobile,
	groupName,
	isDuring
};
/**< 用户列表值*/
#define COMMON_COLUMN_VALUE_TRUE	L"1"
#define COMMON_COLUMN_VALUE_FALSE	L"0"

/**< 扩展用户状态宏
用户状态：
0:已入会但脱机
1:已入会并联机
2:脱机并在黑名单
3:联机并在黑名单
扩展状态：
4:正常被踢出会议
5:退出会议
6:超方被踢出会议
*/
#define EXT_USER_STATUS_KICKED_OUT	(0x0004)
#define EXT_USER_STATUS_EXIT_CONF	(0x0005)
#define EXT_USER_STATUS_KICKED_TOOMANY (0x0006)

/**
* @enum UserStatus
* @brief user status enum.the lowest 2 bits stand for 
*  online/offline and blacklist status.other 30 bits are not used just now.
*/
struct UserStatus 
{
    UINT32 uIsOnline:1;
    UINT32 uIsInBlacklist:1;
    UINT32 uPadding:30;

    UserStatus()
    {
        uIsOnline = 0;
        uIsInBlacklist = 0;
        uPadding = 0;
    }
};

/**
* @struct RoleInfo
* @brief user role struct.
*/
struct RoleInfo
{
    UINT32 roleID;              /**< role id*/
    UINT32 maxUserCount;        /**< max user count for a specific role*/
    //TCHAR* strRoleName;         /**< name string of a role*/
    std::vector<UINT32> servicePrivilliges;   /**< privileges of a service*/

    /**
    * @brief constructor of RoleInfo struct.
    * @param [in] void.
    * @return 
    */
    RoleInfo()
    {
        roleID             = 0;
        maxUserCount       = 0;
        //strRoleName        = NULL;
    }

    /**
    * @brief copy constructor of RoleInfo struct.
    * @param [in] roleInfo.
    * @return 
    */
    RoleInfo(const RoleInfo &roleInfo)
    {
        if (&roleInfo == this)
        {
            return;
        }

        this->roleID = roleInfo.roleID;
        this->maxUserCount = roleInfo.maxUserCount;

        //if (NULL != roleInfo.strRoleName)
        //{
        //    //allocate memory for string variable and copy the value.
        //    size_t maxCount = wcslen(roleInfo.strRoleName)* sizeof(TCHAR);
        //    this->strRoleName = new TCHAR[maxCount];
        //    wcscpy_s(this->strRoleName, maxCount, roleInfo.strRoleName);
        //}

        //copy items to the new instance.
		std::vector<UINT32>::const_iterator iter;
        for (iter=roleInfo.servicePrivilliges.begin(); iter!=roleInfo.servicePrivilliges.end(); iter++)
        {
            this->servicePrivilliges.push_back((*iter));
        }
    }

    /**
    * @brief overload operator = for RoleInfo struct.
    * @param [in] right.right statement of operator =.
    * @return RoleInfo&. return RoleInfo instance to left.
    */
    RoleInfo& operator = (RoleInfo &right)
    {
        if (&right == this)
        {
            return *this;
        }

        this->roleID = right.roleID;
        this->maxUserCount = right.maxUserCount;

        //if (NULL != right.strRoleName)
        //{
        //    //recycle the old string memory.
        //    if (NULL != this->strRoleName)
        //    {
        //        delete[] this->strRoleName;
        //        this->strRoleName = NULL;
        //    }

        //    //allocate memory for string variable and copy the value.
        //    size_t maxCount = wcslen(right.strRoleName)* sizeof(TCHAR);
        //    this->strRoleName = new TCHAR[maxCount];
        //    wcscpy_s(this->strRoleName, maxCount, right.strRoleName);
        //}

        //clear items in service privileges list first.
        this->servicePrivilliges.clear();

        //copy the items to left.
		std::vector<UINT32>::iterator iter;
        for (iter=right.servicePrivilliges.begin(); iter!=right.servicePrivilliges.end(); iter++)
        {
            this->servicePrivilliges.push_back((*iter));
        }

        return *this;
    }

    /**
    * @brief destructor of RoleInfo struct.
    * @param [in] void.
    * @return 
    */
    ~RoleInfo()
    {
        servicePrivilliges.clear();

        /*if (NULL != strRoleName)
        {
            delete[] strRoleName;
            strRoleName = NULL;
        }*/
    }
};
typedef std::list<RoleInfo> UserRoleList;
typedef std::list<RoleInfo*> UserRolePtrList;
/**< default max user count value for role*/
#define ROLE_MAXCOUNT_DEFAULT_VALUE			0x0400;

/**
* @struct UserInfo
* @brief user information struct.
*/
struct UserInfo
{
	UINT32 uInedx;
    UINT32 userID;                     /**< user id*/
    TCHAR* strUserName;                /**< user name string*/
    UserStatus userStatus;             /**< user status*/
    UINT32 clientType;                 /**< client type*/
    UINT32 clientIP[IP_LENGTH];        /**< client IP*/
    UINT32 domain;                     /**< client domain*/
    //UserRolePtrList userRoleList;         /**< user role list*/
    std::vector<UINT32> userRoleList;  /**< user role list*/

    /**
    * @brief constructor of UserInfo struct.
    * @param [in] void.
    * @return 
    */
    UserInfo()
    {
		uInedx = 0;
        userID = 0;
        clientType  = 0;
        domain = 0;
        strUserName = NULL;
    }

    /**
    * @brief copy constructor of UserInfo struct.
    * @param [in] userInfo.
    * @return 
    */
    UserInfo(const UserInfo& userInfo)
    {
        if (&userInfo == this)
        {
            return;
        }

		this->uInedx = userInfo.uInedx;
        this->userID = userInfo.userID;
        this->userStatus = userInfo.userStatus;
        this->clientType = userInfo.clientType;
        this->domain = userInfo.domain;

        if (NULL != userInfo.strUserName)
        {
            //allocate memory for string variable and copy value.
            size_t maxCount = wcslen(userInfo.strUserName)* sizeof(TCHAR);
            this->strUserName = new TCHAR[maxCount];
            wcscpy_s(this->strUserName, maxCount, userInfo.strUserName);
        }

        for (int i=0; i<IP_LENGTH; i++)
        {
            this->clientIP[i] = userInfo.clientIP[i];
        }

        //copy items to the new instance.
        //UserRolePtrList::iterator iter;
        /*for (iter=userRoleList.begin(); iter!=userRoleList.end(); iter++)
        {
            RoleInfo* pRoleInfo = new RoleInfo();
            (*pRoleInfo) = (**iter);
            this->userRoleList.push_back(pRoleInfo);
        }*/
        userRoleList = userInfo.userRoleList;
    }

    /**
    * @brief overload operator = for UserInfo struct.
    * @param [in] right.right statement of operator =.
    * @return UserInfo&. return UserInfo instance to left.
    */
    UserInfo& operator = (UserInfo &right)
    {
        if (&right == this)
        {
            return *this;
        }

		this->uInedx = right.uInedx;
        this->userID = right.userID;
        this->userStatus = right.userStatus;
        this->clientType = right.clientType;
        this->domain = right.domain;
        
        if (NULL != right.strUserName)
        {
            //recycle the old string memory.
            if (NULL != this->strUserName)
            {
                delete[] this->strUserName;
                this->strUserName = NULL;
            }

            //allocate memory for string variable and copy the value.
            size_t maxCount = wcslen(right.strUserName)* sizeof(TCHAR);
            this->strUserName = new TCHAR[maxCount];
            wcscpy_s(this->strUserName, maxCount, right.strUserName);
        }

        for (int i=0; i<IP_LENGTH; i++)
        {
            this->clientIP[i] = right.clientIP[i];
        }

        //clear items in left role list first.
        /*UserRolePtrList::iterator iter;
        for (iter = this->userRoleList.begin(); iter!= this->userRoleList.end(); iter++)
        {
            delete (*iter);
            (*iter) = NULL;
        }*/

        userRoleList.clear();

        //copy items to left.
        std::vector<UINT32>::iterator iter;
        for (iter=right.userRoleList.begin(); iter!=right.userRoleList.end(); iter++)
        {
            /*RoleInfo* pRoleInfo = new RoleInfo();
            (*pRoleInfo) = (**iter);
            this->userRoleList.push_back(pRoleInfo);*/
            this->userRoleList.push_back(*iter);
        }

        return *this;
    }

    /**
    * @brief destructor of UserInfo struct.
    * @param [in] void.
    * @return 
    */
    ~UserInfo()
    {           
        if (NULL != strUserName)
        {
            delete[] strUserName;
            strUserName = NULL;
        }

        //recycle role info memory.
        /*UserRolePtrList::iterator iter;
        for (iter = this->userRoleList.begin(); iter!= this->userRoleList.end(); iter++)
        {
            delete (*iter);
            (*iter) = NULL;
        }*/

        userRoleList.clear();
    }
};
typedef std::list<UserInfo> UserInfoList;
typedef std::list<UserInfo *> UserInfoPtrList;

/**
* @struct ChannelInfo
* @brief channel information struct.
*/
struct ChannelInfo 
{
    UINT32 uServiceType;     /**< service type*/
    UINT32 uType;            /**< channel type*/
    UINT32 uChannelID;       /**< channel ID*/
    UINT32 uGroupID;         /**< group id*/
    UINT32 uChannelType;

    /**
    * @brief constructor of ChannelInfo struct.
    * @param [in] void.
    * @return 
    */
    ChannelInfo()
    {
        uServiceType  = 0;
        uType         = 0;
        uChannelID    = 0;
        uGroupID      = 0;
        uChannelType  = 0;
    }

    /**
    * @brief destructor of ChannelInfo struct.
    * @param [in] void.
    * @return 
    */
    ~ChannelInfo()
    {

    }
};
typedef std::list<ChannelInfo> ChannelInfoList;
typedef std::list<ChannelInfo*> ChannelInfoPtrList;

/** 
* @struct ServiceInfo
* @brief service information struct.
*/
struct ServiceInfo
{
    UINT32 Type:4;                   /**< service type*/
    UINT32 serviceProperty:4;        /**< service property*/
    UINT32 extensionBit:1;           /**< service extension bit*/
    //TODO: define the specificData;

    /**
    * @brief constructor of ServiceInfo struct.
    * @param [in] void.
    * @return 
    */
    ServiceInfo()
    {
        Type                = 0;
        serviceProperty     = 0;
        extensionBit        = 0;
    }

    /**
    * @brief copy constructor of ServiceInfo struct.
    * @param [in] serviceInfo.
    * @return 
    */
    ServiceInfo(const ServiceInfo &serviceInfo)
    {
        if (&serviceInfo == this)
        {
            return;
        }

        this->extensionBit = serviceInfo.extensionBit;
        this->Type = serviceInfo.Type;
        this->serviceProperty = serviceInfo.serviceProperty;
        //TODO: copy the specific data.
        //
    }

    /**
    * @brief overload operator = for ServiceInfo struct.
    * @param [in] right.right statement of operator =.
    * @return ServiceInfo&. return ServiceInfo instance to left.
    */
    ServiceInfo& operator = (ServiceInfo &right)
    {
        if (&right == this)
        {
            return *this;
        }

        this->extensionBit = right.extensionBit;
        this->Type = right.Type;
        this->serviceProperty = right.serviceProperty;
        //TODO: copy the specific data.
        //

        return *this;
    }

    /**
    * @brief destructor of ServiceInfo struct.
    * @param [in] void.
    * @return 
    */
    ~ServiceInfo()
    {

    }
};
typedef std::list<ServiceInfo> ServiceInfoList;
typedef std::list<ServiceInfo*> ServiceInfoPtrList;

/** 
* @struct ConferenceInfo
* @brief conference information struct.
*/
struct ConferenceInfo
{
    UINT32 userID;
    UINT32 confID;                  /**< conference id*/
    //TangConferenceStatus confStatus;/**< conference status*/
    //BOOL bIsUserLocked;				/**< conference user lock*/
    //BOOL bIsSystemLocked;				/**< conference user lock*/
    //__time64_t confStartTime;       /**< conference start time*/
    //__time64_t confEndTime;         /**< conference end time*/
    //UINT32 confLeftTime;            /**< conference left time*/
    //UINT32 confScale;               /**< conference scale*/
    std::list<RoleInfo> confRoleList;           /**< roles in a conference*/
    std::list<UINT32> confStartedServiceList;     /**< conference started services list*/
	std::list<UINT32> confSupportServiceList;     /**< conference started services list*/
    std::list<UINT32> confServiceConfigList;     /**< conference services config list*/
    std::list<UINT32> subConfIDList;             /**< subConference id list*/
	//TCHAR* strConferenceTheme;

    /**
    * @brief constructor of ConferenceInfo struct.
    * @param [in] void.
    * @return 
    */
    ConferenceInfo()
    {
        userID             = 0;
        confID             = 0;
        /*confScale          = 0;
        confStatus         = Stoped;
        confStartTime      = 0;
        confEndTime        = 0;
		bIsUserLocked	   = FALSE;
		bIsSystemLocked	   = FALSE;*/
		//strConferenceTheme = NULL;
        //confLeftTime       = 0;
    }

    /**
    * @brief destructor of ConferenceInfo struct.
    * @param [in] void.
    * @return 
    */
    ~ConferenceInfo()
    {
        //recycle role info memory.
        confRoleList.clear();

        //recycle service info memory.
        /*std::list<ServiceInfo*>::iterator iter2;
        for (iter2 = this->confServiceList.begin(); iter2!= this->confServiceList.end(); iter2++)
        {
            delete (*iter2);
            (*iter2) = NULL;
        }*/

		//clear conference started service list.
		confStartedServiceList.clear();
		
		//clear conference support service list.
		confSupportServiceList.clear();

		//clear conference service config list.
        confServiceConfigList.clear();

        //clear subConference id list.
        subConfIDList.clear();

		/*if(strConferenceTheme != NULL)
		{
			delete[] strConferenceTheme;
			strConferenceTheme = NULL;
		}*/
    }
};
typedef std::list<ConferenceInfo> ConfInfoList;
typedef std::list<ConferenceInfo*> ConfInfoPtrList;
#endif  //GNET_TANG_CLIENT_CONFERENCEDEF_H