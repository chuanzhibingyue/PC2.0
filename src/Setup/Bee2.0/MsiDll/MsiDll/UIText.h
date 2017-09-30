
//以下在定制化界面时需要修改
#define INSTALL_DIR_OLD				L"\\Quanshi\\Bee"			//老的安装目录
#define INSTALL_DIR					L"\\Quanshi\\Bee2.0"		//安装目录
#define TARGET_NAME					L"bee.exe"					//目标程序	
#define REG_AUTO_RUN				L"Bee"						//开机自启动标记
#define CLIENT_WINDOW_TITLE			L"bee"						//蜜蜂主程序窗口名称
#define MAIN_PROCESS				L"bee.exe"					//安装卸载时需要关闭的程序
#define WORKER_PROCESS				L"bee.worker.exe"			//安装卸载时需要关闭的程序
#define UPDATE_PROCESS				L"update.exe"				//安装卸载时需要关闭的程序
#define DESKTOP_LNK					L"\\蜜蜂.lnk"				//桌面快捷名称
#define STARTPROGRAM_FLODER_LNK		L"\\蜜蜂"					//程序列表快捷文件夹名称
#define STARTPROGRAM_UNINSTALL_LNK  L"\\卸载蜜蜂.lnk"			//程序列表卸载快捷名称
#define STARTPROGRAM_LNK			L"\\蜜蜂.lnk"				//程序列表启动快捷名称
#define QUICKSTART_LNK				L"\\Microsoft\\Internet Explorer\\Quick Launch\\蜜蜂.lnk"	//快速启动栏名称
#define INSTALL_DIR_TEMP			L"\\Bee2.0"					//安装目录名称
#define INSTALL_LOG_PATH			L"Quanshi Files\\Bee\\Install"	//安装卸载日志目录
#define APP_LOG_PATH				L"\\Quanshi\\Bee"				//应用程序运行日志目录

//以下可以固定
#define MSIEXEC						L"\\msiexec.exe"			//系统安装进程
#define REG_SOFTWARE				L"Software\\Bee"			//注册表项名称
#define REG_PRODEUCTCODE			L"ProductCode"				//注册表-记录产品ID
#define REG_PACKAGECODE				L"PackageCode"				//注册表-记录包ID
#define REG_VERSION					L"Version"					//注册表-记录版本号
#define REG_DESKLNK					L"DeskLnk"					//注册表-记录是否创建桌面快捷
#define REG_AUTORUN					L"AutoRun"					//注册表-记录是否创开机自启动
#define REG_STARTLNK				L"StartLnk"					//注册表-记录是否创建开始菜单快捷
#define REG_INSTALLDIR				L"installDir"				//注册表-记录安装目录
#define MEETING_SETUP				L"UCCMeetingSetup.exe"		//云会议安装包名称
#define MEETING_PROCESS				L"TangClient.exe"			//云会议主进程
#define MEETING_WORKER_PROCESS		L"TangClient.worker.exe"	//云会议辅助进程
#define MEETING_CHAT_PROCESS		L"TangChat.exe"				//云会议会议窗口进程
#define REG_INSTALLMEETING_FLAG		L"Software\\BeeInstallMeeting"	//注册表-标记云会议是否由蜜蜂安装
#define MEETING_UNINSTALL			L"\\Quanshi\\Quanshi Meeting\\6\\60000\\uninst.exe"	//云会议卸载程序路径	
#define INSTALL_WINDOW_TITLE		L"InstallBee"					//安装程序窗口title
#define UNINSTALL_WINDOW_TITLE		L"UninstallBee"					//卸载程序窗口title
#define REGMANGER_PROCESS			L"BeeRegMgr.exe"				//注册右键程序
#define MEETING_REG_FLAG			L"Software\\Tang"				//云会议安装标记
#define MEETING_REG_INSTALLFLAG		L"ultimate"						//云会议安装标记