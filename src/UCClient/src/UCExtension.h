/*
 * UCExtension.h
 *
 *  Created on: June 26, 2015
 *      Author: gang wang
 */

#ifndef UC_EXTENSION_H_
#define UC_EXTENSION_H_

#include "Delegate.h"
#include "ContactInfo.h"
#include "CalendarData.h"

namespace ucclient {

using namespace cefIntegration;
typedef DWORD WINAPI WaitEventNotify(__in LPVOID lpParam);
class UCExtension : public BrowserDelegate {
public:
	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
        CefProcessId source_process, CefRefPtr<CefProcessMessage> message);
	
private:
	void ClientServiceMethodProcess(CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	void GkClientMethodProcess(CefString method, int argLen,
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	void ContactServiceMethodProcess(CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	void IMServiceMethodProcess(CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	void GroupServiceMethodProcess(CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	void CalendarServiceMethodProcess(CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	void FileTransferServiceMethodProcess(CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

#ifdef PBX_SERVICE
	void PbxServiceMethodProcess(CefString method, int argLen,
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);
#endif // PBX_SERVICE

	void CatchScreenServiceMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs, int requestId);

	void SearchServiceMethodProcess(CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	void LogServiceMethodProcess(CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res);

	void AppMethodProcess(CefString method, int argLen,
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	void SystemServiceMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs, int requestId, bool hasCallback);
	void AudioServiceMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, int requestId);
	void SharingServiceMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen,
		CefRefPtr<CefListValue> arguments, int &res, int requestId);

	void OpenApiServiceMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen,
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs, bool& hasCallback, int requestId);

	void MicroblogServiceMethodProcess(CefString method, int argLen,
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	void UpdateServiceMethodProcess(CefString method, int argLen,
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs, int requestId);

	void ExternalServiceMethodProcess(CefString method, int argLen,
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	void BBSServiceMethodProcess(CefString method, int argLen,
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	CefRefPtr<CefDictionaryValue> ConvertConversationToDictionary(ConversationInfo & info);
	CefRefPtr<CefDictionaryValue> ConvertContactToDictionary(ContactInfo & contact);
	CefRefPtr<CefDictionaryValue> ConvertBasicContactToDictionary(BasicContactInfo & contact);
	CefRefPtr<CefDictionaryValue> ConvertConferenceInfoToDictionary(Meeting & info);
	CefRefPtr<CefDictionaryValue> ConvertGroupInfoToDictionary(GroupInfo &info);
	
	bool ConvertDictionaryToContactTag(CefRefPtr<CefDictionaryValue> tagDictionary, ContactTag &tag);
	bool ConvertDictionaryToUCID(CefRefPtr<CefDictionaryValue> dictionary, UCID& ucid);
	bool ConvertDictionaryToAttendee(CefRefPtr<CefDictionaryValue> dictionary, Attendee &attendee);
	bool ConvertDictionaryToCycleRole(CefRefPtr<CefDictionaryValue> dictionary, CycleRole &role);
	bool ConvertDictionaryToUserInfo(CefRefPtr<CefDictionaryValue> dictionary, UserInfo &userInfo);
	bool ConvertDictionaryToWindowFeature(CefRefPtr<CefDictionaryValue> dictionary, WindowFeature &feature);
	void ConverListToCaptions(CefRefPtr<CefListValue> captionList, vector<RECT> &captions) ;
	bool ConvertDictionaryToMenuFeature(CefRefPtr<CefDictionaryValue> dictionary, MenuFeature &feature) ;
	bool ConvertDictionaryToAppMessageInfo(CefRefPtr<CefDictionaryValue> dictionary, AppMessageInfo &info);
	bool ConvertDictionaryToAppMessageStatusInfo(CefRefPtr<CefDictionaryValue> dictionary, AppMessageStatusInfo &info);
	bool ConvertDictionaryToHeadInfo(CefRefPtr<CefDictionaryValue> dictionary, HeadInfo &info);
public:
	static bool CreateEvent(const std::wstring& eventName, WaitEventNotify* waitFunc);
	IMPLEMENT_REFCOUNTING(UCExtension);
};

}

#endif