/*
 * LoginInfo.h
 *
 *  Created on: 2015年5月7日
 *      Author: jianxue
 */

#ifndef LOGININFO_H_
#define LOGININFO_H_

#include <map>
#include "ServerInfo.h"
#include "UCMessage.h"

namespace uc {

using namespace std;

typedef map<int16_t, ServerInfo> ServerMap;

#define IM                  L"im"
#define CONF                L"conf"
#define SCAN                L"scan"
#define CALENDAR            L"calendar"
#define APP_CENTER          L"app_center"
#define APP_MOMENTS         L"app_moments"
#define CLOUD_DISK          L"cloud_disk"
#define CALL                L"call"
#define REMOTE_ASSISTANT    L"remote_assistant"
#define BROADCAST_STATION   L"broadcast_station"
#define MEETING_ROOM        L"meeting_room"
#define CLOUD_MEETING       L"cloud_meeting"
#define CI_PBX				L"pbx"

struct CapabilityInfo
{
    int8_t cloudDisk;       // 云盘：0 – 不开通 1 – 试用 2 – 正式
    int8_t im;              // 聊天：0 – 不开通 1 – 试用 2 – 正式
    int8_t calendar;        // 日历：0 – 不开通 1 – 试用 2 – 正式
    int8_t conf;            // 会议：0 – 不开通 1 – 试用 2 – 正式
    int8_t call;            // 呼：0 – 不开通 1 – 试用 2 – 正式
    int8_t appCenter;       // 应用全部：0 – 不开通 1  –试用 2 –正式
    int8_t appMoments;      // 应用动态：0 – 不开通 1  –试用 2 –正式
    int8_t scan;            // 扫一扫：0 – 不开通 1  –试用 2 –正式
	int8_t remoteAssistant; // 远程协助: 0 – 不开通 1 – 开通
	int8_t pbx;				// pbx：0 – 不开通 1 – 试用 2 – 正式
	int8_t broadcastStation; //公告板:0 – 不开通 1 – 试用 2 – 正式
	int8_t meetingRoom;      //会议室:0 – 不开通 1 – 试用 2 – 正式
	int8_t cloudMeeting;      //云会议:0 – 不开通 1 – 试用 2 – 正式

    CapabilityInfo(){
        cloudDisk = 0;
        im = 0;
        calendar = 0;
        conf = 0;
        call  = 0;
        appCenter = 0;
        appMoments = 0;
        scan = 0;
        remoteAssistant = 0;
		pbx = 0;
		broadcastStation = 0;
		meetingRoom = 0;
		cloudMeeting = 0;
    }
};

class LoginInfo {
public:
	LoginInfo();
	virtual ~LoginInfo();

public:
	bool AddServerInfo(ServerType type, const string &domain);
	bool AddServerInfo(ServerType type, const string &domain, const string &cname, const string &token);
	
	ServerInfo GetServerInfo(ServerType type) const;

	const string& GetAccount() const {
		return _account;
	}

	void SetAccount(const string& account) {
		_account = account;
	}

	const string& GetClientInfo() const {
		return _clientInfo;
	}

	void SetClientInfo(const string& clientInfo) {
		_clientInfo = clientInfo;
	}

	uint16_t GetClientType() const {
		return _clientType;
	}

	void SetClientType(uint16_t clientType) {
		_clientType = clientType;
	}

	int64_t GetLastKickTime() const {
		return _lastKickTime;
	}

	void SetLastKickTime(int64_t lastKickTime) {
		_lastKickTime = lastKickTime;
	}

	const string& GetPassword() const {
		return _password;
	}

	void SetPassword(const string& password) {
		_password = password;
	}

	int32_t GetResId() const {
		return _curUser.GetResId();
	}

	void SetResId(int32_t resId) {
		_curUser.SetResId(resId);
	}

	const string& GetSessionId() const {
		return _sessionID;
	}

	void SetSessionId(const string& sessionId) {
		_sessionID = sessionId;
	}

	int32_t GetSiteId() const {
		return _curUser.GetSiteId();
	}

	void SetSiteId(int32_t siteId) {
		_curUser.SetSiteId(siteId);
	}

	const string& GetUccServerDomain() const {
		return _uccServerDomain;
	}

	string GetUccServerDomain_Pure() const {	
		string sDomain = _uccServerDomain;
		int pos1 = sDomain.find_first_of('//') + 2;
		if (pos1 > 2) {
			int pos2 = sDomain.find_last_of(':');
			if (pos2 > 0 && pos2 > pos1) {
				sDomain = sDomain.substr(pos1, pos2 - pos1);
			}
			else{
				sDomain = sDomain.substr(pos1, sDomain.length() - pos1);
			}
		}
		return sDomain;
	}

	void SetUccServerDomain(const string& uccServerDomain) {
		_uccServerDomain = uccServerDomain;
	}

	int32_t GetUserId() const {
		return _curUser.GetUserId();
	}

	void SetUserId(int32_t userId) {
		_curUser.SetUserId(userId);
	}

	const UCID& GetCurUser() const {
		return _curUser;
	}

	void SetCurUser(const UCID& curUser) {
		_curUser = curUser;
	}

	const string& GetCustomerCode() const {
		return _customerCode;
	}

	void SetCustomerCode(const string& customerCode) {
		_customerCode = customerCode;
	}

	const string& GetDisplayName() const {
		return _displayName;
	}

	void SetDisplayName(const string& displayName) {
		_displayName = displayName;
	}

	int32_t GetPwdExpiredDays() const {
		return _pwdExpiredDays;
	}

	void SetPwdExpiredDays(int32_t days) {
		_pwdExpiredDays = days;
	}

	bool IsForceChangePwd() const {
		return _forceChangePwd;
	}

	void SetForceChangePwd(bool forceChange) {
		_forceChangePwd = forceChange;
	}

	void SetAccessToken(string accessToken) {
		_accessToken = accessToken;
	}

	const string& GetAccessToken() const {
		return _accessToken;
	}

	void SetRefreshToken(string refreshToken) {
		_refreshToken = refreshToken;
	}

	const string& GetRefreshToken() {
		return _refreshToken;
	}

    void SetCapabilityInfo(CapabilityInfo info) {
        _capabilityInfo.cloudDisk = info.cloudDisk;
        _capabilityInfo.im = info.im;
        _capabilityInfo.calendar = info.calendar;
        _capabilityInfo.conf = info.conf;
        _capabilityInfo.call = info.call;
        _capabilityInfo.appCenter = info.appCenter;
        _capabilityInfo.appMoments = info.appMoments;
        _capabilityInfo.scan = info.scan;
        _capabilityInfo.remoteAssistant = info.remoteAssistant;
		_capabilityInfo.pbx = info.pbx;
		_capabilityInfo.broadcastStation = info.broadcastStation;
		_capabilityInfo.meetingRoom = info.meetingRoom;
		_capabilityInfo.cloudMeeting = info.cloudMeeting;
	}

	void SetAccountType(int32_t type){
		_AccountType = type;
	}
	int32_t GetAccountType(){
		return _AccountType;
	}

	void SetPbxKey(const std::string &obj){
		_pbxKey = obj;
	}

	const std::string GetPbxKey() const {
		return _pbxKey;
	}

	void SetPbxToken(const std::string &obj){
		_pbxToken = obj;
	}

	const std::string GetPbxToken() const {
		return _pbxToken;
	}

	int8_t IsSupportRemoteAssistant() const {
		return _capabilityInfo.remoteAssistant;
	}

private:
	LoginInfo(const LoginInfo &info);
	LoginInfo& operator=(const LoginInfo &info);
	void ClearServerMap();

private:
	string _account;
	string _displayName;
	string _password;
	string _clientInfo;
	uint16_t _clientType;
	UCID	_curUser;
	string _uccServerDomain;
	int64_t _lastKickTime;
	string _sessionID;
	ServerMap _servers;
	string _customerCode;
	int32_t _pwdExpiredDays;
	bool _forceChangePwd;
	string _accessToken;
	string _refreshToken;
    CapabilityInfo _capabilityInfo;
	int32_t _AccountType; // 用户类型， 1：普通用户，2：企业用户

	std::string _pbxKey;
	std::string _pbxToken;
};

} /* namespace uc */

#endif /* LOGININFO_H_ */
