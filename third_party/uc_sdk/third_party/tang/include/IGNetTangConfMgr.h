#ifndef IGNETTANGCONFMGR
#define IGNETTANGCONFMGR

#include <sstream>
#include "ignettangbase.h"

#if defined(WIN32) || defined(WIN64)
#include "IAudioWizard.h"
#include "IVideoWizard.h"
#endif

#if defined(WIN32) || defined(WIN64)
#define TANG_USER_EXPORTS	__declspec(dllexport)
#ifdef CONFERENCE_EXPORTS
#define CONFERENCE extern "C" __declspec(dllexport)
#else
#define CONFERENCE extern "C" __declspec(dllimport)
#endif //CONFERENCE_EXPORTS

#include <comutil.h>
#define CGNetTangVariant VARIANT

#else // not defined(WIN32) || defined(WIN64)
#define CONFERENCE
#define TANG_USER_EXPORTS
#endif //defined(WIN32) || defined(WIN64)


/**
 * @brief 用户信息类
 *
 *  提供用户信息，类型/用户ID/用户名/邮箱/手机号/会中角色/接通状态等信息
 *
 */
class IGNetTangUser
{
public:
    virtual unsigned int 	getUserType() const = 0;
    virtual unsigned int 	getUserID() const = 0;
    virtual char* 			getUserName() const = 0;
    virtual char*     		getEmail() const = 0;
    virtual char*     		getMobile() const = 0;
    virtual unsigned int 	getRoles() const = 0;
    virtual unsigned int 	getAudioStatus() const = 0;
    virtual char*    		getPhoneNumber() const = 0;

    virtual  CGNetTangVariant getProperty(const char* propName) const = 0;
};

#if defined(WIN32) || defined(WIN64)
/**
 * @brief windows下有效，使用WebView的程序需要调用该接口初始化内部external，IE实例及消息分发器
 *
 *  初始化external对象，IE实例和消息分发器。
 *
 */
class IGNetTangForPC
{
public:
    virtual unsigned int InitControls(
                                        void* pExternal,
                                        void* pIEInstance,
                                        void* pMessageDispatcher ) =0;
};
#endif //defined(WIN32) || defined(WIN64)

/**
 * @brief 用户列表类
 *
 *  提供会中用户列表信息，会中用户个数/获取所有会中用户/获取单个会中用户/获取当前用户
 *
 */
class IGNetTangUserList
{
public:
    virtual unsigned int    getUserCount()                                               = 0;
//    virtual int             getUserByIndex(unsigned int index,IGNetTangUser** ppUser)      = 0;
//    virtual int             getUserByID(unsigned int userID,IGNetTangUser** ppUser)        = 0;
//    //virtual unsigned int    getAllUsers(CGNetTangUser* user_list, unsigned int count)    = 0; //return, real user sum
//    virtual int             getMyself(IGNetTangUser** ppUser)                              = 0;

    virtual IGNetTangUser*  getUserByIndex(unsigned int index)               = 0;
    virtual IGNetTangUser*  getUserByID(unsigned int userID)                 = 0;
    virtual IGNetTangUser*  getMyself()                                      = 0;
};

/**
 * @brief 会议管理类
 *
 * 类提供会议管理【加入会议/离开会议/会议重连]和服务管理【开启/关闭音频/视频/桌面共享等服务】
 *
 */
class IGNetTangConference
    : public IGNetTangUserList
#if defined(WIN32) || defined(WIN64)
    , public IGNetTangForPC
#endif //defined(WIN32) || defined(WIN64)
{
public:
    virtual  int            joinConf()                                              = 0;
    virtual  int            leaveConf()                                             = 0;
    virtual  int            endConf()                                               = 0;
    virtual  int            reconnectConf()                                         = 0;
    virtual unsigned int    getConfID()                                             = 0;

    virtual  int            createSession(GNetTangSessionType sessionType)          = 0;
    virtual  int            closeSession(int sessionID)                             = 0;
    virtual IGNetTangBaseSession* 	getSession(int sessionID)                       = 0;

    virtual int  getPropertyValue(const char* propName, CGNetTangVariant& value)    = 0;

//    virtual int             kickOutUserByID(unsigned int uUserID)                   = 0;
//    virtual int             lockConfRoom()                                          = 0;
//    virtual int             unlockConfRoom()                                        = 0;
//    virtual int             addRole(unsigned int uUserID,
//                                    unsigned int uRoles)                            = 0;
//    virtual int             switchUserRole(unsigned int uSrcUserID,
//                                           unsigned int uDestUserID,
//                                           unsigned int uSwitchedRole)              = 0;
};


/**
 * @brief 会议管理回调
 *
 * 调用者需实现IGNetTangConferenceSink类的子类，在createGNetTangConference时传入子类的指针
 *
 *  目的: 获得IGNetTangConference的回调，需要在onSessionCreated方法中返回会议/音频/视频/白板服务的指针，传入各服务Sink对象指针
 *
 */
class IGNetTangConferenceSink:public IGNetTangBaseSessionSink
{
public:
    virtual void onConfJoined(int statusCode)                                                                           = 0;
    virtual void onConfLeft(int statusCode)                                                                             = 0;
    virtual void onConfEnded(int statusCode)                                                                            = 0;
    virtual void onConfReconnected(int statusCode)                                                                      = 0;
    virtual void onSessionCreated(GNetTangSessionType sessionType,  IGNetTangBaseSession *pTangSession, int statusCode) = 0;
    virtual void onSessionClosed(GNetTangSessionType sessionType,int sessionID)                                         = 0;
    virtual void onConfPropertyChanged(const char *propName,
                                      const CGNetTangVariant &oldPropValue,
                                      const CGNetTangVariant &newPropValue)                                             = 0;

    virtual void onUserAdded(const IGNetTangUser* pUser)                                                                = 0;
    virtual void onUserRemoved(const IGNetTangUser* pUser)                                                              = 0;
    virtual void onUserPropertyChanged(const char*propName,
                                      const CGNetTangVariant &oldPropValue,
                                      const CGNetTangVariant &newPropValue,
                                      const IGNetTangUser* pUser)                                                       = 0;
};


#if defined(WIN32) || defined(WIN64)

class IGNetTangAVWizard : public IAudioWizard, public IVideoWizard
{
public:
	virtual void release() = 0;
};

#endif


class IGNetTangService
{
public:
    virtual IGNetTangConference*    createGNetTangConference(const char *joinKey,IGNetTangConferenceSink* pSink)        = 0;
    virtual int                     deleteGNetTangConference(IGNetTangConference* confInstance)                         = 0;

    virtual const char*             getGNetTangVersion()                                                                = 0;
    virtual int                     setGNetTangConfig(const char* configName, CGNetTangVariant& configValue)            = 0;

#if defined(WIN32) || defined(WIN64)
	virtual IGNetTangAVWizard*      getAVWizard()                                                                       = 0;
#endif

};


CONFERENCE IGNetTangService* getGNetTangService();


#endif // IGNETTANGCONFMGR

