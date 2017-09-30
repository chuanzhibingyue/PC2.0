#ifndef _GNET_UC_COMMON_DEFINITIONS_
#define _GNET_UC_COMMON_DEFINITIONS_


// 用于升级服务的内核对象命名
#define UC_HANDLE_NAME L"Global\\UpdateForUC"
// 用于通知升级服务准备升级
#define UC_NEED_UPDATE L"Global\\NeedUpdateForUC"
// 用于服务通知主进程，升级进程成功启动
#define UC_UPDATE_START L"Global\\UpdateStartForUC"
// 用于通知升级完成
#define UC_UPDATE_OVER L"Global\\UpdateOverForUC"
// 用于通知升级失败
#define UC_UPDATE_ERROR L"Global\\UpdateError"
// 用户删除升级的文件
#define UC_UPDATE_FILE_HANDLE	L"Global\\HandleUpdateFileForUC"
// 用于升级服务的传参
#define	INSTALL_UC	L"installUC"
// 升级服务互斥体
#define MUTEX_UCDAEMONSVC L"MutexofUCDaemonSvc"


//update exe
#define UC_UPDATE_PATH	L"update.exe"

//client exe
#define UC_PROCESS_NAME L"bee.exe"

#define UC_UPDATE_TEMP_FILE_PATH	L"Quanshi\\OnCloud\\Update\\UpdateTemp"
#endif // _GNET_UC_COMMON_DEFINITIONS_
