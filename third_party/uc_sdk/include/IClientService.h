/*
 * IClientService.h
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#ifndef ICLIENTSERVICE_H_
#define ICLIENTSERVICE_H_

#include <string>
#include <list>
#include "common/event.h"
#include "UCSDK.h"
#include "IUCEngine.h"
#include "ConversationInfo.h"
#include "AccountInfo.h"

namespace uc {

enum OauthGrantType {
	OGT_PASSWORD = 0,
	OGT_REFRESH_TOKEN
};

interface UC_API IClientServiceEvent {
	virtual ~IClientServiceEvent(){};

	/**
	 * 登录结果的返回事件
	 * @param errorCode
	 * @param authType
	 * @param cloudDiskType
	 */
    virtual void OnLogin(int32_t errorCode, int32_t authType, int32_t cloudDiskType, const std::string &capability = "") = 0;

	/**
	* 获取access token的返回事件
	*/
	virtual void OnAccessTokenReceived(int32_t errorCode, OauthGrantType grantType, const std::string &accessToken) = 0;

	/**
	 * 登出结果的返回事件
	 * @param errorCode
	 */
	virtual void OnLogout(int32_t errorCode) = 0;

	/**
	 * 密码重设的回调
	 * @param result 操作的结果
	 */
	virtual void OnPasswordModified(int32_t errorCode) = 0;

	/**
	 * 其他登陆终端修改密码的回调
	 */
	virtual void OnPasswordModifyMessageReceived() = 0;

	/**
	  * 服务器通知密码复杂性规则变化的回调
	  * @param type 修改后的类型。1）8~30位，不限制类型； 2）8~30位，数字或字母组合
	  * 3）8~30位，数字、符号和字母组合
	  */
	virtual void OnPasswordRuleUpdateMessageReceived(int32_t type) = 0;

	/**
	  * 服务器通知密码到期的回调
	  * @param dueDays 将到期天数
	  */
	virtual void OnPasswordExpiredMessageReceived(int32_t dueDays) = 0;

	/**
	 * ucas 连接的回调事件
	 * @param errorCode
	 */
	virtual void OnUCASConnected(int32_t errorCode) = 0;

	/**
	 * ucas 断开连接的回调事件
	 * @param errorCode
	 */
	virtual void OnUCASDisconnected(int32_t errorCode) = 0;

	/**
	 * list conversation回调事件
	 * @param errorCode
	 * @param conversations
	 * @param isLocal 是否是本地数据库中的conversation
	 * @param totalUnRead 未读消息总数
	 */
	virtual void OnConversationListReceived(int32_t errorCode, const std::list<ConversationInfo*> *conversations, bool isLocal, int32_t totalUnRead, std::map<int64_t, int32_t>& groupAtCounts) = 0;

	/**
	* list conversation结束的通知
	* @param conversationSize  conversation 数量
	*/
	virtual void OnConversationListComplete(int32_t conversationSize) = 0;

	/**
	* list conversation回调事件
	* @param errorCode      操作结果;
	* @param type			会话类型;
	* @param toId			置顶的converstaionId(单人或者讨论组ID);
	* @param state			是否置顶：1：置顶/0：取消置顶
	* @param index			置顶顺序(升级排列);
	*/
	virtual void OnConversationTopSet(int32_t errorCode, ConversationType type, int64_t relateId, int32_t state, int64_t index) = 0;


	/**
	* 用户登录通知回调事件
	* @param errorCode      操作结果;
	* @param userId			用户ID;
	* @param deviceType		用户所登录的设备;
	* @param deviceName		设备名;
	*/
	virtual void OnUserLoginNotify(int32_t errorCode, int64_t userId, DeviceType::type deviceType, std::string deviceName) = 0;

	/**
	* 设置多设备同时登录通知消息回调事件
	* @param errorCode  操作结果
	* @param userId     用户ID
	*/
	virtual void OnUpdateUserNotifySet(int32_t errorCode, int64_t userId) = 0;

	/**
	* 获取多设备同时登录通知消息回调事件
	* @param errorCode  操作结果
	* @param isNotify   通知状态
	*/
	virtual void OnGetUserNotifyStatus(int32_t errorCode, int16_t isNotify) = 0;

	/**
	 * 验证用户账号的回调事件
	 * @param errorCode    错误码;
	 * @param verifyType   认证方式;
	 * @param mobile       手机认证;
	 * @param email        邮箱认证;
	 * @param exprised     过期时间倒计时（单位:秒）;
	 */
	virtual void OnAccountVerified(int32_t errorCode, int32_t verifyType, const std::string &mobile, const std::string &email, int32_t exprised) = 0;

	/**
	 * 手机短信验证码验证的回调事件
	 * @param errorCode
	 * @param exprised     过期时间倒计时（单位:秒）;
	 */
	virtual void OnCodeVerified(int32_t errorCode, int32_t exprised) = 0;

	/**
	* 手机短信验证码验证重发的回调事件
	* @param errorCode
	*/
	virtual void OnResendCodeComplete(int32_t errorCode, int32_t exprised) = 0;

	/**
	 * 忘记密码的回调事件
	 * @param errorCode
	 */
	virtual void OnPasswordReset(int32_t errorCode) = 0;

	/**
	 * 日志上传的回调事件
	 */
	virtual void OnLogUploaded(int32_t errorCode) = 0;

	/**
	 * session过期事件
	 */
	virtual void OnSessionExpired() = 0;

	/**
	* 压缩log结束事件
	*/
	virtual void OnArchiveLogFinish(std::string filePath) = 0;

};

interface UC_API IClientService : public IUCService, Event<IClientServiceEvent> {
	virtual ~IClientService(){};

	/**
	 * 用户登录
	 * @param account
	 * @param password
	 * @param clientType
	 * @param clientId
	 * @param macAddress
	 * @param pinNumber;
	 * @param version
	 * @param storeType
	 * @param phoneNumber
	 */
	virtual int32_t Login(uint16_t clientType, const string &account, const string &password, const string &clientId,
			const string &macAddress, const string &pinNumber, const string &version,
			const string &storeType, const string &phoneNumber, const int32_t & accountType) = 0;

	/**
	* 获取access token
	*/
	virtual void GetAccessToken(OauthGrantType grantType) = 0;

	/**
	 * 登出
	 */
	virtual void Logout() = 0;

	/**
	 * 重新登录
	 */
	virtual int32_t Relogin(bool fromReConnect) = 0;

	/**
	* set proxy url info
	* @param proxyUrl
	*/
	virtual bool SetProxyUrl(const std::string& proxyUrl) = 0;

	/**
	* set proxy auth info
	* @param auth_type
	* @param auth_account
	* @param auth_passwd
	*/
	virtual bool SetProxyInfo(int32_t auth_type, const std::string& auth_account, const std::string& auth_passwd) = 0;

	/**
	* get proxy auth info
	* @param auth_type
	* @param auth_account
	* @param auth_passwd
	*/
	virtual bool GetProxyInfo(std::string& auth_account, std::string& auth_passwd, int &auth_type) = 0;

	/**
	 * 修改用户登录密码
	 * @param oldPassword 老密码
	 * @param newPassword 新密码
	 */
	virtual void ModifyPassword(const string &oldPassword, const string &newPassword) = 0;

	/**
	 * 连接UCAS服务器
	 */
	virtual void ConnectUCAS() = 0;

	/**
	 * 断开与UCAS服务器的连接
	 */
	virtual void DisconnectUCAS() = 0;

	// 下面两个接口是1.0临时解决方案，2.0里将废弃
	/**
	* 连接UCC服务器
	*/
	virtual void ConnectUCC() = 0;

	/**
	* 断开与UCC服务器的连接
	*/
	virtual void DisconnectUCC() = 0;

	/**
	 * 获取本地的conversations
	 * @param number
	 * @param conversations 返回conversation数组
	 */
	virtual void GetLocalConversations(int32_t number) = 0;

	/**
	 * 获取新的conversation列表
	 * @param number 请求的数量
	 */
	virtual void GetNewConversationList(int32_t number) = 0;


	virtual int32_t GetConversationInfo(ConversationType type, int64_t relateId, OUT ConversationInfo& info) = 0;

	/**
	* 设置会话置顶;
	* @param type 会话类型;
	* @param relateId 操作置顶状态的会话ID(userId, groupId,orgId);
	* @param state 操作的置顶状态(1:置顶/0:取消置顶);
	*/
	virtual int32_t SetConversationTop(ConversationType type, int64_t relateId, int32_t state) = 0;

	/**
	 * 获取<startTime的历史conversation列表
	 * @param endTime 请求的开始时间
	 * @param number 数量
	 */
	virtual void GetOldConversationList(int64_t endTime, int32_t number) = 0;

	/**
	 * 往uccserver发送heartbeat包
	 */
	virtual void SendHeartBeat() = 0;

	/**
	 * 获取历史账号信息,按最后登陆时间排序
	 */
	virtual int32_t LoadHistoryAccounts(std::list<AccountInfo> &accounts) = 0;

	/**
	 * 获取数据保存路径
	 */
	virtual std::string GetDocumentDataDir(DocDirType type) = 0;

	/**
	 * 删除历史账户;
	 */
	virtual int32_t DeleteHistoryAccount(const std::string& account) = 0;

	/**
	 * 设置是否忘记密码;
	 */
	virtual int32_t RememberPassword(const std::string& account, bool remember) = 0;

	/**
	 * 设置是否自动登录;
	 */
	virtual int32_t SetAutoLogin(const std::string& account, bool autoLogin) = 0;

	/**
	 * 获取验证码
	 * @param hashStr hash字符串用来给服务器端生成验证码
	 * @return
	 */
	virtual std::string GetCaptchaURL(const string &hashStr) = 0;

	/**
	 * 验证用户账号
	 * @param verifyType 验证方式: 0(手机验证码)/1(邮件验证码)
	 * @param clientType 客户端类型
	 * @param account 账号
	 * @param captcha 验证码 (PC需要传递)
	 * @param hashStr hash值 (PC需要传递)
	 */
	virtual void VerifyAccount(int32_t verifyType, uint16_t clientType, const std::string &account, const std::string &captcha, const std::string &hashStr) = 0;

	/**
	 * 验证码验证
	 * @param verifyType 验证方式: 0(手机验证码)/1(邮件验证码)
	 * @param clientType	客户端类型
	 * @param account		账号
	 * @param mobileCode	短信验证码
	 * @param resend 1:重发 0:不重发只验证
	 */
	virtual void VerifyCode(int32_t verifyType, uint16_t clientType, const std::string &account, const std::string &mobileCode, int32_t resend) = 0;

	/**
	 * 设置新的密码
	 * @param clientType 	客户端类型
	 * @param account 		账号
	 * @param newPwd 		新密码
	 * @param confirmPwd 	确认密码
	 * @param mobileCode 	短信验证码
	 */
	virtual void ResetPassword(uint16_t clientType, const std::string &account, const std::string &newPwd,
			const std::string &confirmPwd, const std::string &mobileCode) = 0;


	/**
	 * 获取当前登录用户的uic信息
	 */
	virtual const UCID& GetCurUCID() const = 0;

	/**
	 * 获取指定app的config信息
	 * @param app
	 * @param configs
	 */
	virtual int32_t GetConfigInfos(const std::string &app, std::map<string, string> &configs) = 0;

	/**
	 * 更新指定app的config信息
	 * @param app
	 * @param configs
	 */
	virtual int32_t UpdateConfigInfos(const std::string &app, std::map<string, string> &configs) = 0;

	/**
	 * 根据app&name值获取config的数据
	 * @param app
	 * @param name
	 * @param value
	 * @return
	 */
	virtual int32_t GetConfigInfo(const std::string &app, const std::string &name, std::string &value) = 0;

	/**
	 * 根据app&name的值更新config数据
	 * @param app
	 * @param name
	 * @param value
	 * @return
	 */
	virtual int32_t UpdateConfigInfo(const std::string &app, const std::string &name, const std::string &value) = 0;

	/**
	* 根据app&name值获取keyValue的数据
	* @param app
	* @param name
	* @param value
	* @return
	*/
	virtual int32_t GetValueByKey(const std::string &app, const std::string &name, std::string &value) = 0;

	/**
	* 根据app&name的值更新keyValue数据
	* @param app
	* @param name
	* @param value
	* @return
	*/
	virtual int32_t UpdateValueBykey(const std::string &app, const std::string &name, const std::string &vaule) = 0;
	
	/**
	 * 上传日志
	 * @param logURL 日志文件在FS中的URL
	 * @param desc 描述信息
	 * @param version 程序版本号
	 */
	virtual void UploadLog(const std::string &logURL, const std::string &desc, const std::string &version) = 0;

	/**
	 * 获取密码过期的天数
	 */
	virtual int32_t GetPwdExpiredDays() = 0;

	/**
	 * 获取sessionid
	 */
	virtual std::string GetSessionId() = 0;

	/**
	* 将日志文件打包
	* 会抛出回调OnArchiveLogFinish
	*/
	virtual void ArchiveLogFile() = 0;

	/**
	 * 返回升级信息
	 * @param upgradeServer 升级服务器信息
	 * @param packageInfo
	 * @param packageDownload
	 */
	virtual void GetUpgradeInfo(std::string &upgradeServer, std::string &packageInfo, std::string &packageDownload) = 0;

	/**
	* 获取密码规则
	* @param days, "pwd_exprised_days" 1-30d 2-60d 3-90d 4-180d 5-永不过期
	* @param memory,  "pwd_memory" 1-3次 2-5次 3-10次 4-不记忆
	* @param complex, "pwd_complex" 1- 8到20位不限类型 2-8到20位数字字母组合 3- 8到20位数字,符号与字母组合
	* @param allow_change,  is_allow_change_pwd的值。 用此判断是否可以更改密码
	*/
	virtual void GetPasswordRule(OUT int32_t & days, OUT int32_t & memory, OUT int32_t & complex, OUT int32_t &allow_change) = 0;

	/**
	* @param value, 1 启用log，2，不使用证书，3 两个功能都打开
	*/
	virtual void EnableCurlSettings(int value) = 0;

	/**
	* 获取UCC server的URL
	* @param uccServerDomain UCC server的URL
	*/
	virtual void GetUccServerDomain(std::string &uccServerDomain) = 0;
	
	/**
	* 获取默认文件保存路径;
	*/
    virtual std::wstring GetDefaultFileSavePath(std::string filename) = 0;

	/**
	* 获取小讨论组成员个数上限;
	*/
	virtual int32_t GetMinGroupMemberLimit() = 0;

	/**
	* 获取大讨论组成员个数上限;
	*/
	virtual int32_t GetMaxGroupMemberLimit() = 0;
	
	/**
	* 获取讨论组个数上限;
	*/
	virtual int32_t GetGroupCountLimit() = 0;

	virtual int GetModuleFlag() = 0;

	/**
	* 设置多设备同时登录通知消息
	* @param isNotity  1:允许通知提示，0：不允许通知提示
	*/
	virtual void SetUpdateUserNotify(int16_t isNotify) = 0;

	/**
	* 获取多设备同时登录通知消息设置状态
	*/
	virtual void GetUserNotifyStatus() = 0;

	//设置和获取本地云会议版本-1--未安装云会议， 0--安装60000版本， 1--安装了60001版本， 2--同时安装了60000和60001版本
	virtual void SetLocalMeetingClientVersion(int version) = 0;
	virtual int32_t GetLocalMeetingClientVersion() = 0;

	/**
	* 删除session接口，使用此接口将在DB里保存
	*/
	virtual void DeleteSession(ConversationType type, int64_t relateId) = 0;
};

}


#endif /* ICLIENTSERVICE_H_ */
