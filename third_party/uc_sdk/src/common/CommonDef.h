/*
 * CommonDef.h
 *
 *  Created on: 2015年6月25日
 *      Author: jianxue
 */

#ifndef COMMONDEF_H_
#define COMMONDEF_H_

/**
 * 定义服务器URL地址
 */
#define UCCSERVER_URL	"https://oncloud.quanshi.com"
#define CASSERVER_URL	"http://bee.quanshi.com"

/**
Http 请求头信息
*/
#define HTTP_HEADER_CONTENT_TYPE			"Content-Type"
#define HTTP_HEADER_CONTENT_TYPE_VALUE		"application/x-www-form-urlencoded"

/**
 * 定义服务器URL命令
 */
#define LOOKUP_METHOD					"/cas/casapi/auth/lookup"			//
#define CAPTCHA_METHOD					"/cas/casapi/auth/captcha"			// pc端获取验证码
#define ACCOUNT_VERIFY_METHOD			"/cas/casapi/auth/accountVerify" 	// 验证用户账号
#define CODE_VERIFY_METHOD				"/cas/casapi/auth/codeVerify"		// 验证手机短信验证码
#define RESET_PASSWORD_METHOD			"/cas/casapi/auth/forgetPassword"	// 忘记密码


#define LOGIN_METHOD 					"/uccserver/uccapi/user/login"				// 登录
#define AUTHORIZE_METHOD 				"/uccserver/uccapi/user/authorize"			// 获取accesstoken
#define REFRESH_TOKEN_METHOD 			"/uccserver/uccapi/user/token"				// 更新accesstoken
#define LOGOUT_METHOD 					"/uccserver/uccapi/user/logout"				// 登出
#define CONTACT_LIST_METHOD				"/uccserver/uccapi/contact/inclist"			// 增量获取常量联系人列表
#define CONTACT_ADD_METHOD				"/uccserver/uccapi/contact/add"				// 添加常用联系人
#define CONTACT_DELETE_METHOD			"/uccserver/uccapi/contact/delete"			// 删除常用联系人
#define USER_PROFILE_METHOD				"/uccserver/uccapi/user/profile"			// 获取用户个人信息
#define USER_COLLEAGUE_METHOD			"/uccserver/uccapi/user/colleague"			// 获取同事列表
#define USER_UPDATE_METHOD				"/uccserver/uccapi/user/update"				// 更新用户信息（头像，签名）
#define USER_MODIFYPWD_METHOD			"/uccserver/uccapi/user/resetPassword"		// 重设密码
#define USER_SETTING_METHOD             "/uccserver/uccapi/user/setting"			// 获取用户多端登录状态
#define USER_SETTING_UPDATE_METHOD      "/uccserver/uccapi/user/settingUpdate"      // 设置用户多端登录提醒
#define UPLOAD_LOG_METHOD				"/uccserver/uccapi/common/logUpload"		// 上传日志
#define ORG_LIST_METHOD					"/uccserver/uccapi/org/list"				// 获取组织通讯录
#define ORG_MEMBER_METHOD				"/uccserver/uccapi/org/allMembers"			// 获取当前组织及子组织的所有成员及总数
#define ORG_STATUS_SUBSCRIBE_METHOD		"/uccserver/uccapi/org/statusSubscribe"		// 部门状态订阅
#define ORG_STATUS_UNSUBSCRIBE_METHOD	"/uccserver/uccapi/org/statusUnsubscribe"	// 部门状态取消订阅
#define SEARCH_LIST_METHOD				"/uccserver/uccapi/search/list"				// 搜索
#define CONTACT_SEARCH_METHOD			"/uccserver/uccapi/contact/search"			// 搜索
#define TAGS_UPDATE_METHOD				"/uccserver/uccapi/user/tagUpdate"			// 修改用户标签
#define	USER_STATUS_GET_MEHTOD    	    "/presenceserver/presenceapi/presence/status" // 获取用户状态
#define	USER_STATUS_SET_MEHTOD    	    "/presenceserver/presenceapi/presence/set"  // 设置用户状态
#define CONTENT_BY_ID_METHOD			"/uccserver/uccapi/message/contentbyid"		// 通过ID获取聊天内容;
#define MESSGAES_READ_METHOD			"/uccserver/uccapi/message/statuschangebyid"// 设置消息批量已读;
#define MESSAGE_UNREADUSER_COUNT_METHOD "/uccserver/uccapi/message/unreadusercount" // 获取消息未读用户个数;
#define MESSAGE_UNREADUSER_LIST_METHOD  "/uccserver/uccapi/message/unreaduserlist"  // 设置消息未读用户列表;
#define MESSAGE_ATMESSAGE_LIST_METHOD	"/uccserver/uccapi/message/atmsglist"		// 批量获得讨论组@消息id列表
#define MESSAGE_ATMESSAGE_DEL_METHOD	"/uccserver/uccapi/message/atmsgdel"		// 批量删除讨论组@消息
#define MESSAGE_REVOCATION_METHOD       "/uccserver/uccapi/message/revocation"      // 消息撤回
#define GROUP_LIST_METHOD				"/uccserver/uccapi/group/all"				// 获取讨论组列表;
#define GROUP_INFO_METHOD				"/uccserver/uccapi/group/info"				// 获取讨论组信息;
#define GROUP_CREATE_METHOD				"/uccserver/uccapi/group/create"			// 创建讨论组;
#define GROUP_EXIT_METHOD				"/uccserver/uccapi/group/quit"				// 退出讨论组;
#define GROUP_DOC_LIST_METHOD			"/uccserver/uccapi/group/meeting"			// 获取讨论组文档列表;
#define GROUP_DOC_SEND_METHOD			"/uccserver/uccapi/group/meetingSave"		// 发送讨论组文档;
#define GROUP_DOC_BATCHSAVE_METHOD		"/uccserver/uccapi/file/BatchSave"			// 文件批量上传;
#define GROUP_DOC_SHARE_METHOD			"/uccserver/uccapi/group/meetingSave"		// 分享讨论组文档;
#define GROUP_DOC_DELETE_METHOD			"/uccserver/uccapi/group/fileDel"			// 删除讨论组文档;
#define GROUP_DOC_INFO_METHOD			"/uccserver/uccapi/group/fileInfo"			// 获取讨论组文档信息;
#define GROUP_COMMENT_LIST_METHOD		"/uccserver/uccapi/comments/list"			// 获取评论列表;
#define GROUP_CREATE_COMMENT_METHOD		"/uccserver/uccapi/comments/submit"			// 创建评论;
#define GROUP_LOGO_SET_METHOD			"/uccserver/uccapi/group/logoUpdate"		// 设置项目组logo;
#define GROUP_TOP_SET_METHOD			"/uccserver/uccapi/group/setTop"			// 讨论组置顶;
#define GROUP_NODISTURB_SET_METHOD		"/uccserver/uccapi/group/nodisturb"		    // 设置讨论组免打扰;
#define GROUP_REMIND_SET_METHOD			"/uccserver/uccapi/group/remind"			// 设置讨论组提醒;
#define GROUP_MEMEBER_ADD_METHOD		"/uccserver/uccapi/group/addMember"		    // 添加讨论组成员;
#define GROUP_SET_ADMIN_METHOD			"/uccserver/uccapi/group/setAdmin"		    // 设置管理员;
#define GROUP_MEMEBER_DEL_METHOD		"/uccserver/uccapi/group/kick"				// 删除讨论组成员;
#define GROUP_NAME_UPDATE_METHOD		"/uccserver/uccapi/group/nameUpdate"		// 更新讨论组名称;
#define GROUP_END_METHOD				"/uccserver/uccapi/group/end"				// 结束讨论组;
#define MESSAGE_SEND_METHOD				"/uccserver/uccapi/message/send"			//发送消息协议;
#define HEARTBEAT_METHOD				"/uccserver/uccapi/message/heartbeat"		// 发送心跳包
#define CONFERENCE_CREATE_METHOD		"/uniform/rs/conferencepc3/CreateReservConference_new"	//创建约会
#define CONTROL_CONFERENCE_CREATE_METHOD		"/uniform/rs/conferencepc3/CreateControlConference"	//创建远程控制约会
#define CONFERENCE_UPDATE_METHOD		"/uniform/rs/conferencepc3/updateReservConference_new"	//变更约会
#define CONFERENCE_CANCEL_METHOD		"/uniform/rs/conferencepc3/cancelReservConference_new"	//取消约会
#define CONFERENCE_GETSTARTURL_METHOD			"/uniform/rs/conferencepc3/joinReservNew"			//获取入会调起串
#define CONFERENCE_GETWEBSTARTURL_METHOD		"/uniform/rs/conferencepc3/mailJoinNew"					//获取Web入会调起串
#define ACCREDITSETTING_UPDATE_METHOD			"/uniform/rs/conference/saveGrantSet"					//更新授权设置
#define SHAREUSER_LIST_METHOD					"/uniform/rs/conference/getShareList"					//获取共享用户列表
#define CONFERENCE_LIST_METHOD					"/uniform/rs/conference/getConferenceList"				//获取会议列表
#define CONFERENCE_BRIEF_LIST_METHOD			"/uniform/rs/conference/briefList"						//获取简明会议列表
#define CONFERENCE_BRIEF_CHANGE_LIST_METHOD		"/uniform/rs/conference/changedBriefList"				//获取变更的简明会议列表
#define CONFERENCE_INVITATION_PROCESS_METHOD	"/uniform/rs/conference/processConferenceInvitation"	//处理约会邀请
#define CONFERENCE_INFO_BYID_METHOD				"/uniform/rs/conference/getConferenceInfoById"			//获取会议详情
#define CONFERENCE_INFO_BYIDS_METHOD			"/uniform/rs/conference/getConferenceInfoByIds"		//批量获取会议详情
#define CONFERENCE_FORWARD_METHOD				"/uniform/rs/conference/forwardConference_new"				//转发会议
#define BUSYFREE_STATUS_METHOD					"/uniform/rs/conference/getUserBusyFreeStatus"			//获取用户日程安排
#define BUSYFREE_INFO_METHOD					"/uniform/rs/conference/getUserBusyFreeInfo"			//获取用户日程安排（新）
#define CONFERENCE_SMMMARY_CREATE_METHOD		"/uniform/rs/confsummary/saveConfSummary"				//创建会议纪要
#define CONFERENCE_SMMMARY_UPDATE_METHOD		"/uniform/rs/confsummary/updateConfSummary"			//更新会议纪要
#define CONFERENCE_SMMMARY_VIEW_METHOD			"/uniform/rs/confsummary/viewConfSummary"				//查看会议纪要
#define CONFERENCE_SUMMARYLIST_METHOD			"/uniform/rs/confsummary/getSummaryListByEventId"	//通过会议id获取会议经要列表
#define CONFERENCE_RECORD_DELETE_METHOD			"/uniform/rs/conference/delRecordVideo"					//删除会议视频录象
#define CONFERENCE_UPLOAD_FILE_METHOD			"/uniform/rs/confsummary/uploadFile"					//上传会议文档
#define CONFERENCE_DELETE_FILE_METHOD			"/uniform/rs/confsummary/deleteFile"					//删除会议文档
#define MEETING_ROOM_CREATE_METHOD				"/uniform/room/create"									//创建会议室
#define MEETING_ROOM_UPDATE_METHOD				"/uniform/room/edit"									//编辑会议室
#define MEETING_ROOM_CREATE_MEETING_METHOD		"/uniform/room/createEvent"								//创建会议室会议
#define MEETING_ROOM_LIST_METHOD				"/uniform/room/listByOrgId"								//获取会议室列表
#define MEETING_ROOM_CHECK_METHOD				"/uniform/room/check"									//检查会议室是否可以入会
#define MEETING_ROOM_CHECK_PASSWORD_METHOD		"/uniform/room/checkPassword"							//检查会议室密码是否有效
#define MEETING_ROOM_DELETE_METHOD				"/uniform/room/delete"									//删除会议室
#define MEETING_ROOM_CHECK_RIGHTS_METHOD		"/uniform/room/checkRights"								//检查是否有权限创建会议室
#define CONVERSATION_LIST_METHOD				"/uccserver/uccapi/message/sessions"					//获取会话列表
#define CONVERSATION_SET_TOP_METHOD				"/uccserver/uccapi/message/sessiontop"                  //设置会话置顶
#define CONVERSATION_SET_UNTOP_METHOD			"/uccserver/uccapi/message/sessionuntop"				//取消会话置顶
#define SEARCH_MESSAGE_LIST_METHOD				"/uccserver/uccapi/search/list"						//搜索接口
#define FS_HDCHK_METHOD							"/hdchk"		//文件上传check接口
#define FS_HDUP_METHOD							"/hdup"			//文件上传接口
#define FS_HDDOWN_METHOD						"/hddown"		//文件下载接口
#define FS_HDDEL_METHOD							"/hddel"		//文件删除接口
#define FS_HDSAFEDOWN_METHOD					"/hdsafedown"	//文件安全下载接口（不检测session）
#define CALL_CREATE_METHOD						"/uniform/rs/call/createCallConference"				//创建呼
#define CALL_JOIN_METHOD						"/uniform/rs/call/joinCallConference"				//加入呼
#define CALL_LEAVE_METHOD						"/uniform/rs/call/leaveCallConference"				//离开呼
#define CHANGE_CALL_TYPE						"/uniform/rs/call/changeCallType"					//变更呼类型
#define GET_CALL_STATUS							"/uniform/rs/call/getUserCallStatus"				//获取call status
#define GET_LAST_AUDIO_INVITE					"/uccserver/uccapi/message/lastaudioinvite"			//获取上一次呼信息
#define APP_LIST_METHOD	                        "/extapi/app/list"                                  //获取应用列表
#define APP_INFO_METHOD	                        "/extapi/app/batchget"                              //获取应用信息
#define OA_MESSAGE_STATUS_METHOD	            "/uccserver/uccapi/message/oastatusbyid"            //获取OA消息状态
#define UPDATE_OA_MESSGAE_STATUS_METHOD	        "/uccserver/uccapi/message/oastatuschangebyappid"   //通过APPID修改OA消息状态
#define MESSAGE_ACK_STATUS_METHOD	            "/uccserver/uccapi/message/appacksentbyid"          //获取消息ACK状态

#define MICROBLOG_HOMEPAGE_PC_METHOD			"/microblog/beeweb/api/l/auth"						// pc端登陆
#define MICROBLOG_HOMEPAGE_METHOD				"/microblog/webapp/mainframe.htm#"
#define MICROBLOG_NEWMSGCOUNT_METHOD			"/microblog/weibo/amsgcount"						// 未读消息数量
#define MICROBLOG_ADDATTENTION_METHOD			"/microblog/personal/addattention"					// 添加关注人
#define MICROBLOG_CANCELATTENTION_METHOD		"/microblog/personal/cancelattention"				// 取消关注人
#define MICROBLOG_GETATTENTIONLIST_METHOD		"/microblog/personal/aattention/"					// 获取关注人列表
#define MICROBLOG_SHARE_METHOD					"/microblog/weiboapi/timenews/publishtimenews"				// 分享接口

#define BBS_MAIN_PATH							"/bbsapp"								// 公告接口主Path

/**
 * UCEngine常量参数 
 */
#define EXECUTOR_POOL_SIZE					1 // services共用的线程池中线程数量


/**
 * ContactService常量参数
 */
#define CONTACT_LIST_PAGESIZE				200 // 增量获取常量联系人列表的分页大小
#define CONTACT_SEARCH_PAGESIZE				12 // 增量获取获取搜索结果的分页大小
#define USER_COLLEAGUE_PAGESIZE				12 // 增量获取获取同事列表的分页大小


//ACK READ;
#define MESSAGE_ACK_READ					0x0001
#define BATCH_MESSAGE_ACK_READ				0x0002

#define MESSAGE_VERSION						0x0100

#define HEARTBEAT_INTERVAL					5 * 60 * 1000

#define WORKER_THREAD_MODE					0x0001
#define MAIN_THREAD_MODE					0x0002

// 
#define UCC_REQUEST_THREADS					2
#define FS_REQUEST_THREADS					3
#define FS_REQUEST_PERFORM_NUMBER           10

#endif /* COMMONDEF_H_ */
