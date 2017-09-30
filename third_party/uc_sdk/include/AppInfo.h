#ifndef APPINFO_H_
#define APPINFO_H_

#include <string>
#include "UCSDK.h"

namespace uc {

using namespace std;

class UC_API AppInfo
{
public:
	AppInfo(void) {
		_appId              = 0;
		_appTitle           = "";
		_appDesc            = "";
		_appLogo            = "";
		_pcUrl              = "";
		_iosUrl             = "";
		_androidUrl         ="";
		_enableSendMessage  = 0;
		_status             = 0;
		_eventUrl           = "";
		_access             = 0;
	}

	virtual ~AppInfo(void) {
	}

	int64_t GetAppId() const{
		return _appId;
	}
	void SetAppId(int64_t appId){
		_appId = appId;
	}

	const string& GetAppTitle() const {
		return _appTitle;
	}
	void SetAppTitle(const string& appTitle) {
		_appTitle = appTitle;
	}
		
	const string& GetAppDesc() const {
		return _appDesc;
	}
	void SetAppDesc(const string& appDesc) {
		_appDesc = appDesc;
	}

	const string& GetAppLogo() const {
		return _appLogo;
	}
	void SetAppLogo(const string& appLogo) {
		_appLogo = appLogo;
	}
	
	const string& GetPcUrl() const {
		return _pcUrl;
	}
	void SetPcUrl(const string& pcUrl) {
		_pcUrl = pcUrl;
	}

	const string& GetIosUrl() const {
		return _iosUrl;
	}
	void SetIosUrl(const string& iosUrl) {
		_iosUrl = iosUrl;
	}

	const string& GetAndroidUrl() const {
		return _androidUrl;
	}
	void SetAndroidUrl(const string& androidUrl) {
		_androidUrl = androidUrl;
	}

	int32_t GetEnableSendMessage() const{
		return _enableSendMessage;
	}
	void SetEnableSendMessage(int32_t enableSendMessage){
		_enableSendMessage = enableSendMessage;
	}

	int32_t GetStatus() const{
		return _status;
	}
	void SetStatus(int32_t status){
		_status = status;
	}

	const string& GetEventUrl() const {
		return _eventUrl;
	}
	void SetEventUrl(const string& eventUrl) {
		_eventUrl = eventUrl;
	}

	int32_t GetAccess() const{
		return _access;
	}
	void SetAccess(int32_t access){
		_access = access;
	}

private:
	int64_t   _appId;
	string    _appTitle;
	string    _appDesc;
	string    _appLogo;
	string    _pcUrl;
	string    _iosUrl;
	string    _androidUrl;
	int32_t   _enableSendMessage;
	int32_t   _status;
	string    _eventUrl;
	int32_t   _access;
};

} /* namespace uc */

#endif /* APPINFO_H_ */
