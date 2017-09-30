#ifndef GNETTANGPROPERTIESDEFINE
#define GNETTANGPROPERTIESDEFINE

//All of meeting data/string/command which will be used by 3rd-parter should be defined here -Loren
#if defined(WIN32) || defined(WIN64)
#else
#define S_OK                                0x0L
#define S_FALSE                             0x1L
#define S_SUCCEED                           200
#endif

//1. Common Property
#define PROP_NOTIFYEVENT            "notifyEvent"
//#define PROP_ISSERVICEREADY         "isServiceReady" // bool; 0, not;1, yes, can call current service API now.
#define PROP_ISSESSIONREADY         "isSessionReady" // bool; 0, not;1, yes, can call current service API now.

//2. Conference Property
#define PROP_CONF_USERID                    "userID"
#define PROP_CONFSTATUS                     "confStatus"
#define PROP_CONF_ROOMLOCKED                "confRoomLocked"
#define PROP_IS_DATA_READY                  "isConfDataReady"
#define PROP_CONF_THEME                     "confTitle"
#define PROP_IS_DATA_READY_AFTER_NETDOWN	"isConfDataReady"
#define PROP_UPDATE_USERDATA_OVER           "isUserDataReady"
#define PROP_NETWORKQUALITY                 "networkQuality"
#define PROP_NETTYPE                        "nettype"


//2.1 User Property
#define PROP_USERTYPE                       "userType"
#define PROP_CLIENTTYPE                     "clientType"
#define PROP_USERID                         "userId"
#define	PROP_USERNAME                       "userName"
#define	PROP_EMAIL                          "email"
#define	PROP_MOBILE                         "mobile"
#define PROP_USERROLES                      "userRoles"
#define PROP_USERAUDIOSTATUS                "audioStatus" 
#define PROP_ISSPEAKING                     "isSpeaking" //0,none;1, speaking
#define	PROP_PHONENUM                       "phoneNum"
#define PROP_STATUS                         "status"
#define	PROP_VIDEOSHARESTARTER              "videoShareStatus"

//2.1.PROP_USERTYPE
#define TANGUSERTYPE_DATAUSER           0
#define TANGUSERTYPE_PHONEUSER          1

//2.1.PROP_CLIENTTYPE
#define TANGCLIENTTYPE_PC               2
#define TANGCLIENTTYPE_PUREPHONE        3
#define TANGCLIENTTYPE_MOBILE           6

//2.1.PROP_USERROLES
#define TNAGUSERROLE_NONE               0
#define TANGUSERROLE_GUEST              1
#define TANGUSERROLE_PRESENTER          2
#define TANGUSERROLE_HOST               4
#define TANGUSERROLE_PRESENTERHOST  (TANGUSERROLE_PRESENTER | TANGUSERROLE_HOST)

//2.1.3 PROP_USERAUDIOSTATUS
#define TANGUSERAUDIOSTATUS_NONE                    0
#define TANGUSERAUDIOSTATUS_VOIPUNMUTE              1
#define TANGUSERAUDIOSTATUS_VOIPMUTEBYSELF	        2
#define TANGUSERAUDIOSTATUS_VOIPMUTEBYHOST	        3
#define TANGUSERAUDIOSTATUS_PSTNUNMUTE		        4
#define TANGUSERAUDIOSTATUS_PSTNMUTEBYSELF	        5
#define TANGUSERAUDIOSTATUS_PSTNMUTEBYHOST	        6
#define TANGUSERAUDIOSTATUS_PSTNCALLING 	        7

//3. Audio Property
#define PROP_AUDIO_NOTIFYEVENT            "notifyEvent"
#define PROP_AUDIO_CONFERMUTESTATUS       "conferMuteStatus"
#define PROP_AUDIO_NETWORKQUALITY		  "networkQuality"
#define PROP_AUDIO_WIZARDMICLEVEL         "wizardMicLevel"
#define PROP_AUDIO_WIZARDSPEAKERLEVEL     "wizardSpeakerLevel"
#define PROP_AUDIO_MICVOLUME			  "microVolume"
#define PROP_AUDIO_SPEAKERVOLUME          "speakerVolume"
#define PROP_AUDIO_AFTERMUTE_JOIN         "isAfterMuteJoin"
#define PROP_AUDIO_LOUDSPEAKERSTATUS      "loudSpeakerStatus"
#define PROP_AUDIO_ISSERVICEREADY         "isServiceReady" // bool; 0, not;1, yes, can call current service API now.

//4.Video Session  Property
//Not NOW


//5.Chat Property
#define  PROP_CHAT_RECORD_STATUS                "recording"
#define  PROP_CHAT_ERROR_OPERATION             	"operation"
#define  PROP_CHAT_ERROR_INFO					"errorInfo"
#define  PROP_CHAT_ERROR_ID		    			"errorID"

//5.1 Chat Message
#define  PROP_CHAT_MESSAGES                     "messages"
#define  PROP_CHAT_MESSAGE_TYPE                 "type"
#define  PROP_CHAT_MESSAGE_LOCALTIME            "localTime"
#define  PROP_CHAT_MESSAGE_TIMESTAMP            "timeStamp"
#define  PROP_CHAT_MESSAGE_SENDER_NAME          "senderName"
#define  PROP_CHAT_MESSAGE_RECEIVER_NAME        "receiverName"
#define  PROP_CHAT_MESSAGE_CONTENT              "content"

//5.1.1 PROP_CHAT_MESSAGE_TYPE
#define TANGCHAT_MSGTYPE_PUBLIC   0
#define TANGCHAT_MSGTYPE_PRIVATE  1

//6. Document Proerpty
//7. Desktop Property
//8. Whiteborad property

//Tang VAR
typedef unsigned short GNETTANG_VARTYPE;

typedef enum  {
    TANG_VT_EMPTY = 0,
    TANG_VT_UINT = 1,
    TANG_VT_BOOL = 2,
    TANG_VT_I4 = 3,
    TANG_VT_I8 = 4,
    TANG_VT_BSTR = 5,
    TANG_VT_DISPATCH = 6
} TANG_VARENUM;

////Session Type
//typedef enum
//{
//    TMC_SESSIONTYPE_NONE = 0,
//    TMC_SESSIONTYPE_AUDIO,
//    TMC_SESSIONTYPE_VIDEO,
//    TMC_SESSIONTYPE_WHITEBOARD,
//    TMC_SESSIONTYPE_DOCUMENT,
//    TMC_SESSIONTYPE_DESKTOP,
//    TMC_SESSIONTYPE_CHAT,
//    TMC_SESSIONTYPE_ALL
//} GNetTangSessionType;

////Conference Join
//enum TANG_JOINCONF_STATUS
//{
//    JOINCONFSTATUS_SUCCEEDED = 0,
//    JOINCONFSTATUS_RECONNECTSUCCEEDED,
//    JOINCONFSTATUS_NETWORKCONNECTFAILED,
//    JOINCONFSTATUS_NETWORKAUTHFAILED,
//    JOINCONFSTATUS_GETCONFINFOFAILED,
//    JOINCONFSTATUS_GETUSERINFOFAILED,
//};

////Conference Left
//enum TANG_LEFTCONF_REASON
//{
//    LEFTCONFREASON_UNKNOWN          =0,
//    LEFTCONFREASON_SELFLEFT,
//    LEFTCONFREASON_HOSTENDMEETING,
//    LEFTCONFREASON_HOSTKICKOUT,
//    LEFTCONFREASON_SERVERSTOPPED,
//    LEFTCONFREASON_NETWORKDISCONNECT
//};

//network type
enum TANG_NetworkType
{
    TANG_NetWorkType_Unknown = 0,
    TANG_NetworkType_Ethernet,
    TANG_NetworkType_WIFI,
    TANG_NetworkType_4G,
    TANG_NetworkType_3G,
    TANG_NetworkType_2G,
};

////NETWORKQUALITY
//enum TANG_SESSION_NETWORKQUALITY
//{
//    NETWORKQUALITY_VERYGOOD = 0,
//    NETWORKQUALITY_GOOD,
//    NETWORKQUALITY_NORMAL,
//    NETWORKQUALITY_BAD,
//    NETWORKQUALITY_VERYBAD,
//};

//TangConfig
#define GNETTANGCONFIG_SETRUNNINGMODE                  "GNetTangConfig_SetRunningMode"
#define GNETTANGCONFIG_SETAPPLOGPATH                   "GNetTangConfig_SetAppLogPath"
#define GNETTANGCONFIG_SETPHONECALLBACKDISPLAYNAME     "GNetTangConfig_SetPhoneCallbackDisplayName"
#define GNETTANGCONFIG_SETTHREADTYPE                   "GNetTangConfig_SetThreadType" //线程模式，默认0，一般不需要调用

#endif // GNETTANGPROPERTIESDEFINE

