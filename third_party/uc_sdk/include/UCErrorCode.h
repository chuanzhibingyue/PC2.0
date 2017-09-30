#ifndef UCERRORCODE_H_
#define UCERRORCODE_H_

/**
 * 处理结果
 */
#define UC_SUCCEEDED 						    0			// 函数成功
#define UC_LOCALDATA_SUCCESSED  			    10          // 函数从本地获取数据成功
#define UC_FAILED 							    100			// 函数内部错误
#define UCC_STATUS_CODE_ERROR 				    101   		// http状态码错误
#define JSON_PARSE_ERROR 					    102   		// 解析json错误
#define UCAS_CLOSED_ABNORMLLY 				    103			// ucas连接非正常关闭
#define UC_SERVER_ERROR						    104			// htpp请求成功，但服务器出现了内部错误
#define UC_PENDING								105			// 表示异步操作正在执行
#define UC_CANCEL								106			// 用户取消了操作
#define UC_LOGINNING							107			// 正在登陆不能重复调用login或者relogin函数
#define UCAS_CLOSED_NODATA						108	        // 长时间没有收到ucas的任何消息
#define UCAS_TOOMANY_CONNECTION					109	        // UCAS接收到太多连接请求
#define UCAS_MESSAGE_SYNC_LOCAL_FAILED  		110	        // 消息缓存同步失败
#define UC_JSON_PARSE_ONE_DATA_ERROR 		    111   		// 解析单条数据json错误
#define GRANTTYPE_NOTSUPPORT					112			// Oauth授权类型不支持
#define FORBID_SENDCLOUDFILE_TO_CLOUDGROUP		113			// 不允许往文件库讨论组发送云盘文件
#define EMPTY_FILEID_CLOUDFILEINFO				114			// fileId和cloudFileInfo不能都为空
#define MESSAGE_NOT_SUPPORTED					115			// 该消息类型不支持
#define ERROR_INPUT_PARAMETER					116			// 输入参数错误
#define UC_URL_NOT_EXIST						404
/*
* server error code
*/
#define SESSION_EXPIRED							10101		// session过期或者无效

//Login error code from UCC by SJ on 20150630
#define EMPTY_USER_PASS         			    10137       //用户名和密码不能为空
#define EMPTY_CLIENT_INFO					    10138		//错误的客户端信息
#define EMPTY_CLIENT_TYPE					    10139		//检查客户端类型
#define CLIENT_FORCE_UPDATE					    10152       //客户端版本需要强制更新
#define CLIENT_ID_MAC						    10411		//客户端id和mac地址不能为空
#define CHECK_ID_CUSTOMER					    10141		//检查站点id和客户编码的正确性,get site id or customer code error,初始化ucas服务令牌错误
#define COMPANY_TYPE						    10415		//查询用户公司类型和导入方式出错
#define CHECK_USER_STATUS					    10144       //检查用户是否有使用uc产品的状态this user have not open uc product
#define GET_RESORUCE_ID						    10412		//generate resource id failed
#define GET_USER_TAGS						    10416		//生成用户mq队列get user tags error,获取用户新标签失败
#define GET_USER_ECOLOGY					    10417		//检查用户是否是企业生态用户check user is ecology error.
#define HASH_IP								    10145		//Mq federate 集群hash算法出错,hash federate culster ip error
#define UCAS_CHANNEL_ERROR					    10140		//调用ucas接口初始化ucas数据,输入的站点id和客户编码不存在

//Login error code from UMS by SJ on 20150630
#define USER_NO_EXIST						    10128		//request user account not exists or ums inner error
#define UMS_ERROR							    10434		//批量获取用户联系人版本号失败,unexcept ums error
#define USER_CLOSED							    10433		//用户账号被停用

//Login error code from MQ by SJ on 20150630
#define MQ_ERROR							    10414		//Mq队列创建失败

//Login error code from UCAS by SJ on 20150630
#define EMPTY_PASSWORD						    20101		//用户名密码不能为空
#define USER_ID_ERROR						    20102		//通过用户id获取站点信息失败
#define USER_ACCOUNT_ERROR					    20103		//用户账号错误
#define USER_INVALID						    20104		//账号无效(该站点下不存在此用户)
#define USER_MULTI_SITE						    20105		//用户有多个站点,客户端需要提示用户输入新的站点url重新登录
	
//FileTransport error code
#define FT_FAIL                                 60001		// 一般性错误
#define FT_ERROR_PARAMETER                      60002		// 参数错误
#define FT_ERROR_FILE_OPERATOR					60003		// 获取文件信息错误
#define FT_ERROR_FILE_UPLOADING					60004		// 文件正在上传
#define FT_ERROR_TASK_RUNNING					60005		// 任务正在进行
#define FT_ERROR_RESPONSE_GETXTANGRESULT		60007		// 查询-网络回应头X-Tang-Result字段出错
#define FT_ERROR_RESPONSE_GETXTANGFILEOFFSET	60008		// 查询-网络回应头X-Tang-File-Offset字段出错
#define FT_ERROR_RESPONSE_GETCOOKIE				60009		// 查询-网络回应头Cookie字段出错
#define FT_ERROR_RESPONSE_GETCONTENTLENGTH		60010		// 查询-网络回应头Content-Length字段出错
#define FT_ERROR_RESPONSE_DATA					60011		// 查询-网络回应数据无效
#define FT_ERROR_RESPONSE_GETFILEID				60012		// 查询-网络回应Tang-File-ID出错
#define FT_ERROR_RESPONSE_STATUSCODE			60013		// 网络回应头StatusCode出错 
#define FT_ERROR_RESPONSE_XTANGRESULT			60014		// 网络回应头X-Tang-Result字段无效
#define FT_ERROR_RESPONSE_XTANGFILEOFFSET		60015		// 网络回应头X-Tang-File-Offset字段无效
#define FT_ERROR_RESPONSE_CONTENTLENGTH			60016		// 网络回应头Content-length字段无效
#define FT_ERROR_RESPONSE_COOKIE				60017		// 网络回应头Cookie字段出错
#define FT_ERROR_RESPONSE_FILELENGTH			60018		// 网络回应头File-length字段无效 
#define FT_ERROR_FILE_SIZE						60019		// 文件大小不对
#define FT_ERROR_TASK_NOT_EXIST					60020		// 任务不存在
#define FT_ERROR_TASK_CANCELED					60021		// 任务已经取消
#define FT_ERROR_RESPONSE_GETXTANGNEWURL		60022		// 查询-网络回应头X-Tang-New-URL字段出错
#define FT_ERROR_RESPONSE_GETXTANGNERRORCODE	60023		// 查询-网络回应头X-Tang-Error-Code字段出错
#define FT_ERROR_FILE_OPEN						60024		// 文件打开失败
#define FT_ERROR_FILE_NOT_EXIST					60025		// 文件不存在
#define FT_ERROR_FILE_READ						60026		// 读取文件失败
#define FT_ERROR_FILE_WRITE						60027		// 写文件失败
#define FT_ERROR_RESPONSE_GETFILELENGTH			60028		// 查询-网络回应头File-Length字段出错
#define FT_ERROR_CREATE_DIR						60029		// 创建文件目录失败
#define FT_ERROR_FILE_MD5						60030		// 获取文件MD5失败
#define FT_ERROR_FILE_RENAME					60031		// 重命名文件失败
#define	FT_ERROR_FILE_DOWNLOADING				60032		//文件正在下载
#define FT_ERROR_DATA_SEND						60033		//文件上传时，发送的数据为空或大小不对
#define FT_ERROR_FILE_TOO_BIG					60034		//文件太大
#define FT_ERROR_FILE_CAN_NOT_OPERATE			60035		//文件无法操作
#define FT_ERROR_DOWNLOAD_URL					60036		//下载url不是蜜蜂FS的
#define FT_ERROR_DOWNLOAD_URL_CHANGED			18			//下载URL发生变动
#define FT_ERROR_FILE_DELETED					22			//文件被删除

/*
FS服务器错误码定义
*/
#define INVALID_PARAMETER						1
#define NO_ENOUGH_SPACE							2
#define UPLOADING_UNCOMPLETED					4
#define OVERWRITTEN								5
#define MD5_CHECKING_FAILED						6
#define CONVERSION_STARTING_FAILED				7
#define RESOURCE_NOT_FOUND						8
#define WRONG_OFFSET							15			//offset错误
#define HDFS_ERROR								16			//HDFS错误
#define WRONG_FS_SERVER							17			//错误的FS Server
#define LOCAL_NOT_FOUND							18			//本地FS找不到该文件，需要从云端FS下载
#define CHECK_SESSION_FAILED					19			//调用Ucc server的checkSession接口异常失败
#define DELETE_FILE_WITHOUT_RIGHT				20			//试图删除一个文件，却没有权限
#define DELETE_FILE_FAILED						21			//试图删除一个文件，失败了
#define FILE_DELETED							22			//试图删除一个文件，却没有权限

#endif /* UCERRORCODE_H */

