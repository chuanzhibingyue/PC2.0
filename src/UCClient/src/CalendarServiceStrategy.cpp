#include "CalendarServiceStrategy.h"
#include "ServiceObjectConversion.h"
#include "UCDefs.h"
#include <fstream>
#include "IClientService.h"
#include "utils/Thumbnails.h"
#include <tlhelp32.h>
#include "Shlobj.h"

namespace ucclient {

CalendarServiceStrategy::CalendarServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient) 
	: _engine(engine), _cefClient(cefClient) {
	_calendarService = (ICalendarService*)_engine->QueryService(UCCalendarService);
}

int32_t CalendarServiceStrategy::CreateMeeting(const std::vector<Attendee>& attendees, const std::wstring& title, const std::wstring& location,
	const std::wstring& summary, const std::wstring& timeZone, const std::wstring& hostName, const std::wstring extrasInfo, int64_t startTime, int32_t duration,
	int32_t authorizerId, CycleRole& cycleRole, int32_t isGnet, int32_t isCycle, int32_t isWholeDay, int32_t language, int32_t emailLanguage) {
		
		std::string utf8Title = StringConvert::ToUTF8Str(title);
		std::string utf8Location = StringConvert::ToUTF8Str(location);
		std::string utf8Summary = StringConvert::ToUTF8Str(summary);
		std::string utf8TimeZone = StringConvert::ToUTF8Str(timeZone);
		std::string utf8HostName = StringConvert::ToUTF8Str(hostName);
		std::string utf8ExtrasInfo = StringConvert::ToUTF8Str(extrasInfo);
		
		return  _calendarService->CreateMeeting(attendees, utf8Title, utf8Location, utf8Summary, utf8TimeZone, 
			utf8HostName, utf8ExtrasInfo, startTime, duration, authorizerId, cycleRole, isGnet, isCycle, isWholeDay, language, emailLanguage);
}

int32_t CalendarServiceStrategy::CreateRemoteAssistance(int32 userId) {
	return _calendarService->CreateRemoteAssistance(userId);
}

int32_t CalendarServiceStrategy::UpdateMeeting(int64_t eventId, int64_t oriTime, const std::vector<Attendee>& attendees,
	const std::wstring& title, const std::wstring& location, const std::wstring& summary,
	const std::wstring& timeZone, const std::wstring extrasInfo, int64_t startTime, int32_t duration, int32_t authorizerId,
	CycleRole& cycleRole, int32_t isGnet, int32_t isCycle, int32_t isWholeDay, int32_t language, int32_t emailLanguage) {

	std::string utf8Title = StringConvert::ToUTF8Str(title);
	std::string utf8Location = StringConvert::ToUTF8Str(location);
	std::string utf8Summary = StringConvert::ToUTF8Str(summary);
	std::string utf8TimeZone = StringConvert::ToUTF8Str(timeZone);
	std::string utf8ExtrasInfo = StringConvert::ToUTF8Str(extrasInfo);
	
	return  _calendarService->UpdateMeeting(eventId, oriTime, attendees,
		utf8Title, utf8Location, utf8Summary, utf8TimeZone, utf8ExtrasInfo, startTime, duration, authorizerId,
			cycleRole, isGnet, isCycle, isWholeDay, language, emailLanguage);
}

int32_t CalendarServiceStrategy::CancelMeeting(int64_t eventId, int64_t startTime, int32_t authorizerId, int32_t language, int32_t emailLanguage) {
	return _calendarService->CancelMeeting(eventId, startTime, authorizerId, language, emailLanguage);	
}

int32_t CalendarServiceStrategy::GetMeetingStartURL(const std::wstring& confId, const std::wstring& password, std::string& startURL, int32_t& siteId) {
	std::string utf8ConfId = StringConvert::ToUTF8Str(confId);
	std::string utf8Password = StringConvert::ToUTF8Str(password);

	return _calendarService->GetMeetingStartURL(utf8ConfId, utf8Password, startURL, siteId);
}


int32_t CalendarServiceStrategy::GetMeetingWebStartURL(const std::wstring& confId, const std::wstring& mail, 
	const std::wstring& password, const std::wstring& displayName) {

	std::string utf8ConfId = StringConvert::ToUTF8Str(confId);
	std::string utf8Mail = StringConvert::ToUTF8Str(mail);
	std::string utf8Password = StringConvert::ToUTF8Str(password);
	std::string utf8DisplayName = StringConvert::ToUTF8Str(displayName);
	return _calendarService->GetMeetingWebStartURL(utf8ConfId, utf8Mail, utf8Password, utf8DisplayName);
}

int32_t CalendarServiceStrategy::UpdateAccreditSetting(int32_t authorizedPersonId, int32_t status) {
	return _calendarService->UpdateAccreditSetting(authorizedPersonId, status);
}

int32_t CalendarServiceStrategy::GetAccreditUserList(OUT std::vector<Accrediting>& infos) {
	return _calendarService->GetAccreditUserList(infos);
}

int32_t CalendarServiceStrategy::GetMeetingList(int64_t startTime, int64_t endTime, int32_t limit, int32_t authorizerId,
	int64_t eventIdFilter, bool filterAsc, int32_t refreshflag, OUT int32_t& requestId, OUT vector<Meeting>& meetings) {
	return _calendarService->GetMeetingList(startTime, endTime, limit, authorizerId, eventIdFilter, filterAsc, refreshflag, requestId, meetings);
}

int32_t CalendarServiceStrategy::GetBriefMeetingList(int64_t startTime, int64_t endTime, int32_t authorizerId, OUT int32_t& requestId, OUT vector<Meeting>& meetings) {
	return _calendarService->GetBriefMeetingList(startTime, endTime, authorizerId, requestId, meetings);
}

int32_t CalendarServiceStrategy::GetBriefChangedMeetingList(int64_t startTime, int32_t authorizerId) {
	return _calendarService->GetBriefChangedMeetingList(startTime, authorizerId);
}

int32_t CalendarServiceStrategy::AcceptMeetingInvitation(int64_t eventId, int64_t startTime, int32_t authorizerId) {
	return _calendarService->AcceptMeetingInvitation(eventId, startTime, authorizerId);
}

int32_t CalendarServiceStrategy::DenyMeetingInvitation(int64_t eventId, int64_t startTime, int32_t authorizerId) {
	return _calendarService->DenyMeetingInvitation(eventId, startTime, authorizerId);
}

int32_t CalendarServiceStrategy::GetMeetingInfoById(int64_t eventId, int64_t startTime, int64_t endTime,
	int32_t authorizerId, OUT vector<Meeting>& meetings) {
	return _calendarService->GetMeetingInfoById(eventId, startTime, endTime, authorizerId, meetings);
}

int32_t CalendarServiceStrategy::GetMeetingInfoByIds(const std::vector<int64_t>& eventIds, int64_t startTime, int64_t endTime,
	int32_t authorizerId, int32_t& requestId, vector<Meeting>& meetings) {
	return _calendarService->GetMeetingInfoByIds(eventIds, startTime, endTime, authorizerId, requestId, meetings);
}

int32_t CalendarServiceStrategy::ForwardMeeting(int64_t eventId, const std::vector<Attendee>& attendees,
	int64_t startTime, int32_t authorizerId, int32_t language, int32_t emailLanguage) {
	return _calendarService->ForwardMeeting(eventId, attendees, startTime, authorizerId, language, emailLanguage);
}

int32_t CalendarServiceStrategy::GetUserSchedule(const std::vector<int32_t>& userIdList, int64_t startTime, 
	int64_t endTime, int32_t timeInterval, int64_t eventIdFilter, OUT int32_t& requestId) {
	return _calendarService->GetUserSchedule(userIdList, startTime, endTime, timeInterval, eventIdFilter, requestId);
}

int32_t CalendarServiceStrategy::CreateMinutes(int64_t eventId, int64_t startTime, const std::wstring& title, const std::wstring& minutes) {

	std::string utf8Title = StringConvert::ToUTF8Str(title);
	std::string utf8Minutes = StringConvert::ToUTF8Str(minutes);
		
	return _calendarService->CreateMinutes(eventId, startTime, utf8Title, utf8Minutes);
}

int32_t CalendarServiceStrategy::UpdateMinutes(int64_t eventId, int64_t minutesId, const std::wstring& title, const std::wstring& minutes) {
	std::string utf8Title = StringConvert::ToUTF8Str(title);
	std::string utf8Minutes = StringConvert::ToUTF8Str(minutes);
	return _calendarService->UpdateMinutes(eventId, minutesId, utf8Title, utf8Minutes);
}

int32_t CalendarServiceStrategy::GetMinutesDetails(int64_t eventId, int64_t minutesId) {
	return  _calendarService->GetMinutesDetails(eventId, minutesId);
}

int32_t CalendarServiceStrategy::GetMinutesList(int64_t eventId, int64_t startTime) {
	return _calendarService->GetMinutesList(eventId, startTime);
}

int32_t CalendarServiceStrategy::DeleteMeetingRecording(int64_t eventId, const std::wstring& confId, int64_t startTime) {
	std::string utf8ConfId = StringConvert::ToUTF8Str(confId);
	return _calendarService->DeleteMeetingRecording(eventId, utf8ConfId, startTime);
}

int32_t CalendarServiceStrategy::RevocationMeetingMessage(int64_t groupId, int64_t eventId, int64_t seqId, int32_t& msgId){
	return _calendarService->RevocationMeetingMessage(groupId, eventId, seqId, msgId);
}

int32_t CalendarServiceStrategy::SetMeetingAlarm(int32_t alarmStatus, int32_t alarmTime) {
	return _calendarService->SetMeetingAlarm(alarmStatus, alarmTime);
}


int32_t CalendarServiceStrategy::CreateMeetingRoom(const std::vector<Attendee>& attendees, const std::wstring& title, const std::wstring& remarks, const std::wstring& password, int64_t startTime, int64_t endTime, int64_t roomHostId, int64_t orgId, int32_t isSetPassword /*= 1*/, int32_t defaultAttendsVisible /*= 1*/, int32_t defaultAttendsJoin /*= 1*/)
{
	std::string utf8Title = StringConvert::ToUTF8Str(title);
	std::string utf8Remarks = StringConvert::ToUTF8Str(remarks);
	std::string utf8Password= StringConvert::ToUTF8Str(password);
	return _calendarService->CreateMeetingRoom(attendees, utf8Title, utf8Remarks, utf8Password, startTime, endTime, roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin);
}

int32_t CalendarServiceStrategy::UpdateMeetingRoom(std::wstring& roomId, const std::vector<Attendee>& attendees, const std::wstring& title, const std::wstring& remarks, const std::wstring& password, int64_t startTime, int64_t endTime, int64_t roomHostId, int64_t orgId, int32_t isSetPassword /*= 1*/, int32_t defaultAttendsVisible /*= 1*/, int32_t defaultAttendsJoin /*= 1*/)
{
	std::string utf8Roomid = StringConvert::ToUTF8Str(roomId);
	std::string utf8Title = StringConvert::ToUTF8Str(title);
	std::string utf8Remarks = StringConvert::ToUTF8Str(remarks);
	std::string utf8Password = StringConvert::ToUTF8Str(password);
	return _calendarService->UpdateMeetingRoom(utf8Roomid, attendees, utf8Title, utf8Remarks, utf8Password, startTime, endTime, roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin);
}

int32_t CalendarServiceStrategy::CreateMeetingInMeetingRoom(const std::vector<Attendee>& attendees, const std::wstring& roomId, const std::wstring& title, const std::wstring& summary, int64_t startTime, int32_t duration)
{
	std::string utf8Roomid = StringConvert::ToUTF8Str(roomId);
	std::string utf8Title = StringConvert::ToUTF8Str(title);
	std::string utf8Summary = StringConvert::ToUTF8Str(summary);
	return _calendarService->CreateMeetingInMeetingRoom(attendees, utf8Roomid, utf8Title, utf8Summary, startTime, duration);
}

int32_t CalendarServiceStrategy::GetMeetingRoomList(int64_t orgId)
{
	return _calendarService->GetMeetingRoomList(orgId);
}

int32_t CalendarServiceStrategy::CheckJoinMeetingRoom(const std::wstring& roomId)
{
	std::string utf8Roomid = StringConvert::ToUTF8Str(roomId);
	return _calendarService->CheckJoinMeetingRoom(utf8Roomid);
}

int32_t CalendarServiceStrategy::CheckMeetingRoomPassword(const std::wstring& roomId, const std::wstring& password)
{
	std::string utf8Roomid = StringConvert::ToUTF8Str(roomId);
	std::string utf8Password = StringConvert::ToUTF8Str(password);
	return _calendarService->CheckMeetingRoomPassword(utf8Roomid, utf8Password);
}

int32_t CalendarServiceStrategy::DeleteMeetingRoom(const std::wstring& roomId)
{
	std::string utf8Roomid = StringConvert::ToUTF8Str(roomId);
	return _calendarService->DeleteMeetingRoom(utf8Roomid);
}


int32_t CalendarServiceStrategy::CheckRightsOfCreateRoom()
{
	return _calendarService->CheckRightsOfCreateRoom();
}


int32_t CalendarServiceStrategy::ResendMessage(int32_t msgId){
	return _calendarService->ResendMessage(msgId);
}

int32_t CalendarServiceStrategy::ForwardMessage(UCID& userUid, int32_t msgId){
	return _calendarService->ForwardMessage(userUid, msgId);
}

int32_t CalendarServiceStrategy::SendTextMessage(int64_t eventId, int64_t startTime, UCID groupId, int8_t textType, 
	std::wstring& textFormat, std::wstring& textData, OUT int32_t& msgId){

	std::string utf8Format = StringConvert::ToUTF8Str(textFormat);
	std::string utf8Text = StringConvert::ToUTF8Str(textData);
	return _calendarService->SendTextMessage(eventId, startTime, groupId, textType, utf8Format, utf8Text, msgId);
}

int32_t CalendarServiceStrategy::SendImageMessage(int64_t eventId, int64_t startTime, UCID groupId, std::wstring& imageName, 
	int32_t imageSize, std::wstring& imageURL, std::wstring& imagePath, OUT int32_t& msgId){

	std::string utf8ImageName = StringConvert::ToUTF8Str(imageName);
	std::string utf8ImageURL = StringConvert::ToUTF8Str(imageURL);

	std::wstring thumbnailPath = L"";
	std::string thunmbnailData = "";

	IClientService *service = (IClientService*)_engine->QueryService(UCClientService);
	if (service != NULL) {
		thumbnailPath = StringConvert::FromUTF8Str(service->GetDocumentDataDir(IMAGE_DIR));
	}

	ThumbnailsUtil::CreateThumbnails(imagePath, thumbnailPath, thunmbnailData);

	return _calendarService->SendImageMessage(eventId, startTime, groupId, utf8ImageName, imageSize, utf8ImageURL, thunmbnailData, msgId);
}

int32_t CalendarServiceStrategy::SendAudioMessage(int64_t eventId, int64_t startTime, UCID groupId, std::wstring& audioName,
	int32_t audioSize, std::wstring& audioURL, int32_t audioDuration, OUT int32_t& msgId){

	std::string utf8AudioName = StringConvert::ToUTF8Str(audioName);
	std::string utf8AudioURL = StringConvert::ToUTF8Str(audioURL);

	return _calendarService->SendAudioMessage(eventId, startTime, groupId, utf8AudioName, audioSize, utf8AudioURL,
			audioDuration, msgId);
}

int32_t CalendarServiceStrategy::SendVideoMessage(int64_t eventId, int64_t startTime, UCID groupId, std::wstring& videoName,
	int32_t videoSize, std::wstring& videoURL, std::wstring& videoPath, int32_t videoDuration, OUT int32_t& msgId){

	std::string utf8VideoName = StringConvert::ToUTF8Str(videoName);
	std::string utf8VideoURL = StringConvert::ToUTF8Str(videoURL);
	std::string utf8VideoData = StringConvert::ToUTF8Str(videoPath);

	return _calendarService->SendVideoMessage(eventId, startTime, groupId, utf8VideoName, videoSize, utf8VideoURL,
			utf8VideoData, videoDuration, msgId);	
}

int32_t CalendarServiceStrategy::StartMeetingClient(const std::wstring& params) {
	WCHAR path[MAX_PATH] = { 0 };
	::SHGetSpecialFolderPath(0, path, CSIDL_PROFILE,false);
	wcscat_s(path, MAX_PATH, L"\\Quanshi\\Quanshi Meeting\\TangRoute.exe");
	
	SHELLEXECUTEINFOW sei = { sizeof(SHELLEXECUTEINFOW) };  
    sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;  
	sei.lpFile = path;  
	sei.lpVerb = L"open";
	sei.nShow = SW_SHOWNORMAL;
	sei.lpParameters = params.c_str();
    BOOL res = ShellExecuteExW(&sei);
	if (res != TRUE) {
		return GetLastError();
	}
	else {
		return UCCLIENT_SUCCESS;
	}
}

int32_t CalendarServiceStrategy::CheckMeetingClient() {
	HKEY key;
	int32_t res = UCCLIENT_ERROR;
	if((res = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Tang", 0, KEY_QUERY_VALUE,  
		&key)) == ERROR_SUCCESS) {	
		DWORD type = REG_DWORD;
		WCHAR data[256];
		DWORD dwLen = 256;
		res = RegQueryValueEx(key, L"ultimate", NULL, &type, (LPBYTE)data, &dwLen);
		RegCloseKey(key);
	}

	if (res != ERROR_SUCCESS) {
		UC_LOG(ERROR) << "fail to check regedit of meeting client, res: " << res;
		return res;
	}

	WCHAR path[MAX_PATH] = {0};
	::SHGetSpecialFolderPath(0, path, CSIDL_PROFILE,false);
	wcscat_s(path, MAX_PATH, L"\\Quanshi\\Quanshi Meeting\\TangRoute.exe");

	if (!std::wifstream(path).good()) {
		UC_LOG(ERROR) << path << "does not exist.";
		return UCCLIENT_ERROR;
	}

	return UCCLIENT_SUCCESS;
}


int32_t CalendarServiceStrategy::KillMeetingClient() {
	
	UC_LOG(INFO) << "KillMeetingClient enter.";

	//事件通知退出
// 	HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Global\\CloseMeeting");
// 	if (NULL != hEvent) {
// 		if (SetEvent(hEvent))
// 			return UCCLIENT_SUCCESS;
// 	}

	//拷贝升级文件前先杀掉相关进程
	PROCESSENTRY32 pe32;
	//在使用这个结构之前，先设置它的大小
	pe32.dwSize = sizeof(pe32);
	//给系统内的所有进程拍一个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//遍历进程快照，轮流显示每个进程的信息
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);

	HANDLE hProcess = NULL;
	while (bMore)
	{
		if (wcscmp(L"TangChat.exe", pe32.szExeFile) == 0 || wcscmp(L"TangClient.exe", pe32.szExeFile) == 0
			|| wcscmp(L"TangClient.worker.exe", pe32.szExeFile) == 0)
		{
			//打开目标进程  
			hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
			if (NULL != hProcess)
			{
				BOOL ret = TerminateProcess(hProcess, 0);
				if (!ret) UC_LOG(ERROR) << "TerminateProcess, process:" << pe32.szExeFile << ", error : " << ::GetLastError();
				CloseHandle(hProcess);
			}
			else
			{
				UC_LOG(ERROR) << "OpenProcess, process:" << pe32.szExeFile << ", return error:" << ::GetLastError();
			}
		}


		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);
	return UCCLIENT_SUCCESS;
}

void CalendarServiceStrategy::OnMeetingCreated(int32_t errorCode, Meeting& meeting){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_CREATED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", meeting: ";

	jsScript << ServiceObjectConversion::ConvertMeetingToJSON(meeting) << "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingCreated event to JS";				
}

void CalendarServiceStrategy::OnRemotAssistanceCreated(int32_t errorCode, const std::string& confId, const std::string& passWd, const std::string& startURL, int32_t userId){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_REMOTE_ASSISTANCE_CREATED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", conferenceId: \""
		<< UTF2WJSON(confId) << "\", attendeePassword: \"" << UTF2WJSON(passWd)
		<< "\", startURL: \"" << UTF2WJSON(startURL)
		<< "\", userId: " << userId << "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRemotAssistanceCreated event to JS";
}

void CalendarServiceStrategy::OnMeetingUpdated(int32_t errorCode, int64_t eventId, int64_t startTime, Meeting& meeting){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_UPDATED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", eventId: "<< eventId
		<< ", startTime: " << startTime <<", meeting: ";

	jsScript << ServiceObjectConversion::ConvertMeetingToJSON(meeting) << "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingUpdated event to JS";											
}

void CalendarServiceStrategy::OnMeetingCanceled(int32_t errorCode, int64_t eventId, int64_t startTime/* = 0*/, int32_t authorizerId /*= 0*/){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_CANCELED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode  << ", eventId: " << eventId << ", authorizerId: " 
		<< authorizerId << ",startTime: " << startTime << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingCanceled event to JS";													
}

void CalendarServiceStrategy::OnMeetingStartURLReceived(int32_t errorCode, const std::string& confId, const std::string& startURL){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_STARTURL_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", confId: \"" 
		<< UTF2WJSON(confId) << "\", startURL: \"" << UTF2WJSON(startURL) << "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingStartURLReceived event to JS";											
}

void CalendarServiceStrategy::OnMeetingWebStartURLReceived(int32_t errorCode, const std::string& confId, const std::string& startURL){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_WEBSTARTURL_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", confId: \"" 
		<< UTF2WJSON(confId) << "\", startURL: \"" << UTF2WJSON(startURL) << "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingWebStartURLReceived event to JS";									
}

void CalendarServiceStrategy::OnAccreditSettingUpdated(int32_t errorCode, int32_t authorizedPersonId, int32_t status){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_ACCREDIT_SETTING_UPDATED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", authorizedPersonId: "
		<< authorizedPersonId << ", status:" << status << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAccreditSettingUpdated event to JS";											
}

void CalendarServiceStrategy::OnAccreditUserListReceived(int32_t errorCode, std::vector<Accrediting>& accreditUserList){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_ACCREDIT_USERLIST_RECEIVED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", accreditUsers: [";

	for (std::size_t i = 0; i < accreditUserList.size(); ++i) {
		if (i != 0)
			jsScript << ",";
		jsScript << ServiceObjectConversion::ConvertAccreditInfoToJSON(accreditUserList[i]);
	}
	
	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAccreditUserListReceived event to JS";																			
}

void CalendarServiceStrategy::OnMeetingListReceived(int32_t errorCode, int32_t requestId, int64_t startTime, int64_t endTime, int64_t eventIdFilter, int32_t authorizerId, std::vector<Meeting>& meetings){
	UC_LOG(INFO) << "++(6)++event received. request id = " << requestId;
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_LIST_RECEIVED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", requestId: "
		<< requestId << ", startTime: "<< startTime <<", endTime: " << endTime
		<< ", eventIdFilter: " << eventIdFilter << ", authorizerId: " << authorizerId << ", meetings: [";
	for (std::size_t i = 0; i < meetings.size(); ++i) {
		if (i != 0)
			jsScript << ", ";
		std::wstring json = ServiceObjectConversion::ConvertMeetingToJSON(meetings[i]);
		jsScript << json;
	}
	
	jsScript << "]},});document.dispatchEvent(e);";

	std::wstring json = StringConvert::removeLineBreak(jsScript.str());
	_cefClient->ExecuteJSForAllBrowsers(json);
	UC_LOG(INFO) << "++(7)++after ExecuteJSForAllBrowsers, request id = " << requestId;
	UC_LOG(INFO) << "dispatch OnMeetingListReceived event to JS, JsScript = " << json;												
}

void CalendarServiceStrategy::OnBriefMeetingListReceived(int32_t errorCode, int32_t requestId, int64_t startTime, int64_t endTime, int32_t authorizerId, std::vector<Meeting>& meetings) {
	UC_LOG(INFO) << "OnBriefMeetingListReceived. request id = " << requestId;
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_BRIEF_MEETING_LIST_RECEIVED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", requestId: "
		<< requestId << ", startTime: " << startTime << ", endTime: " << endTime
		<< ", authorizerId: " << authorizerId << ", meetings: [";
	for (std::size_t i = 0; i < meetings.size(); ++i) {
		if (i != 0)
			jsScript << ", ";
		std::wstring json = ServiceObjectConversion::ConvertMeetingToJSON(meetings[i]);
		jsScript << json;
	}

	jsScript << "]},});document.dispatchEvent(e);";

	std::wstring json = StringConvert::removeLineBreak(jsScript.str());
	_cefClient->ExecuteJSForAllBrowsers(json);
	UC_LOG(INFO) << "dispatch OnBriefMeetingListReceived event to JS, JsScript = " << json;
}

void CalendarServiceStrategy::OnBriefChangedMeetingListReceived(int32_t errorCode, int64_t startTime, int32_t authorizerId, std::vector<Meeting>& meetings) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_BRIEF_CHANGED_MEETING_LIST_RECEIVED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", startTime: " << startTime << ", authorizerId: "
		<< authorizerId << ", meetings: [";
	for (std::size_t i = 0; i < meetings.size(); ++i) {
		if (i != 0)
			jsScript << ", ";
		std::wstring json = ServiceObjectConversion::ConvertMeetingToJSON(meetings[i]);
		jsScript << json;
	}

	jsScript << "]},});document.dispatchEvent(e);";

	std::wstring json = StringConvert::removeLineBreak(jsScript.str());
	_cefClient->ExecuteJSForAllBrowsers(json);
	UC_LOG(INFO) << "dispatch OnBriefChangedMeetingListReceived event to JS, JsScript = " << json;
}

void CalendarServiceStrategy::OnMeetingInfoReceivedById(int32_t errorCode, int64_t eventId, int32_t authorizerId, std::vector<Meeting>& meetings){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_INFO_RECEIVED_BY_ID_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode  << ", eventId:"
		<< eventId << ", authorizerId: " << authorizerId << ", meetings: [";

	for (std::size_t i = 0; i < meetings.size(); ++i) {
		if (i != 0)
			jsScript << ", ";
		jsScript << ServiceObjectConversion::ConvertMeetingToJSON(meetings[i]);
	}
	
	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingInfoReceivedById event to JS";												
}

void CalendarServiceStrategy::OnMeetingInfoReceivedByIds(int32_t errorCode, int32_t requesetId, int32_t authorizerId, std::vector<Meeting>& meetings){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_INFO_RECEIVED_BY_IDS_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", requesetId:"
		<< requesetId << ", authorizerId: " << authorizerId << ", meetings: [";

	for (std::size_t i = 0; i < meetings.size(); ++i) {
		if (i != 0)
			jsScript << ", ";
		jsScript << ServiceObjectConversion::ConvertMeetingToJSON(meetings[i]);
	}
	
	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingInfoReceivedByIds event to JS";										
}

void CalendarServiceStrategy::OnMeetingInvitationAccepted(int32_t errorCode, int64_t eventId, int64_t startTime/* = 0*/, int32_t authorizerId/* = 0*/){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_INVITATION_ACCEPTED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", eventId: "
		<< eventId << ", authorizerId: " << authorizerId << ", startTime: " << startTime
		<< "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingInvitationAccepted event to JS";								
}

void CalendarServiceStrategy::OnMeetingInvitationDenied(int32_t errorCode, int64_t eventId, int64_t startTime/* = 0*/, int32_t authorizerId/* = 0*/){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_INVITATION_DENIED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", eventId: "
		<< eventId << ", authorizerId: " << authorizerId << ", startTime: " << startTime
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingInvitationDenied event to JS";												
}

void CalendarServiceStrategy::OnMeetingForwarded(int32_t errorCode, Meeting& meeting){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_FORWARDED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", meeting: ";

	jsScript << ServiceObjectConversion::ConvertMeetingToJSON(meeting) << "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingForwarded event to JS";												
}

void CalendarServiceStrategy::OnUserScheduleReceived(int32_t errorCode, int32_t requestId, std::vector<UserSchedule>& schedules){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_USER_SCHEDULE_RECEIVED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", requestId:"
		<< requestId << ", schedules: [";

	for (std::size_t i = 0; i < schedules.size(); ++i) {
		if (i != 0)
			jsScript << ",";

		jsScript << ServiceObjectConversion::ConvertUserScheduleToJSON(schedules[i]);
	}
	
	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUserScheduleReceived event to JS";								
}

void CalendarServiceStrategy::OnMinutesCreated( int32_t errorCode,	int64_t eventId, int64_t startTime,
	int64_t minutesId, const std::string& minutesTitle) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MINUTES_CREATED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", eventId: "
		<< eventId << ", minutesId:" << minutesId << ", minutesTitle:\"" << URLDATA_UTF2WJSON(minutesTitle)
		<< "\",startTime:" << startTime << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMinutesCreated event to JS";						
}

void CalendarServiceStrategy::OnMinutesUpdated(int32_t errorCode, int64_t eventId, int64_t minutesId, const std::string& minutesTitle){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MINUTES_UPDATED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", eventId: "
		<< eventId << ", minutesId: " << minutesId << ", minutesTitle: \"" 
		<< URLDATA_UTF2WJSON(minutesTitle) << "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMinutesUpdated event to JS";											
}

void CalendarServiceStrategy::OnMinutesDetailsReceived(int32_t errorCode, int64_t eventId, int64_t minutesId, const Minutes& info) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MINUTES_DETAILS_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", eventId: "
		<< eventId << ", minutesId: " << minutesId << ", creatorId: " << info.GetCreatorId()
		<< ", createTime: " << info.GetCreateTime() << ", updateTime: " << info.GetUpdateTime()
		<< ", content: \"" << URLDATA_UTF2WJSON(info.GetContent())
		<< "\",startTime: " << info.GetStartTime() << ",title: \"" 
		<< URLDATA_UTF2WJSON(info.GetTitle()) << "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMinutesDetailsReceived event to JS";													
}

void CalendarServiceStrategy::OnMinutesListReceived(int32_t errorCode,
	int64_t eventId, int64_t startTime, std::vector<Minutes>& infos) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MINUTES_LIST_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", eventId: "
		<< eventId << ", startTime: " << startTime << ", minutes:[";

	int32_t i = infos.size() - 1;
	while(i>=0) {
		jsScript << ServiceObjectConversion::ConvertMinutesToJSON(infos[i]) << ",";
		i--;
	}

	jsScript<< "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMinutesListReceived event to JS";											
}

void CalendarServiceStrategy::OnMeetingRecordingDeleted(int32_t errorCode, int64_t eventId, int64_t startTime/* = 0*/) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_RECORDING_DELETED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", eventId: "
		<< eventId << ", startTime:" << startTime << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingRecordingDeleted event to JS";
}


void CalendarServiceStrategy::OnMeetingFileUploaded(int32_t errorCode, int64_t eventId, int32_t operation, int32_t operatorId, const DocumentInfo& info)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_FILE_UPLOADED_EVENT
		<< "\",{detail:" << "{errorCode:" << errorCode << ", eventId:" << eventId
		<< ", operatorId:" << operatorId << ",operation:" << operation << ",msgId:" << info.GetMsgId();

	jsScript << ", document:" << ServiceObjectConversion::ConvertDocumentInfoToJSON(info);
	jsScript << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingFileUploaded event to JS";
}

void CalendarServiceStrategy::OnMeetingFileDeleted(int32_t errorCode, int64_t minutesId, int32_t operatorId)
{
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_FILE_DELETED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", minutesId: "
		<< minutesId << ", operatorId:" << operatorId << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingFileDeleted event to JS";
}

void CalendarServiceStrategy::OnMessageSent(int32_t result, int64_t groupId, int64_t eventId, int64_t startTime, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MESSAGE_SENT_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ", groupId: "
		<< groupId << ", eventId: " << eventId
		<< ", startTime: " << startTime << ", msgId: " 
		<< msgId << ", msgSeq: " << msgSeq << ", msgTime: " << msgTime <<"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageSent event to JS";
}

void CalendarServiceStrategy::OnTextMessageReceived(MessageChannel channel, const ConfTextMessageInfo *message) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_TEXT_MESSAGE_RECEIVED_EVENT
			 << "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMeetingTextMessageToJSON(const_cast<ConfTextMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnTextMessageReceived event to JS";
}

void CalendarServiceStrategy::OnImageMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_IMAGE_MESSAGE_RECEIVED_EVENT
			 << "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMeetingMediaMessgeToJSON(const_cast<ConfMediaMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnImageMessageReceived event to JS";
}

void CalendarServiceStrategy::OnAudioMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_AUDIO_MESSAGE_RECEIVED_EVENT
		<< "\",{detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMeetingMediaMessgeToJSON(const_cast<ConfMediaMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAudioMessageReceived event to JS";
}

void CalendarServiceStrategy::OnVideoMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_VIDEO_MESSAGE_RECEIVED_EVENT
			 << "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMeetingMediaMessgeToJSON(const_cast<ConfMediaMessageInfo&>(*message));

	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnVideoMessageReceived event to JS";
}


void CalendarServiceStrategy::OnReportMessageReceived(MessageChannel channel, const ReportMessageInfo *message){

	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnReportMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_REPORT_MESSAGE_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendReportMessageInfoToJSON(const_cast<ReportMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnReportMessageReceived event to JS";
}

void CalendarServiceStrategy::OnRecordVideoMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_RECORD_VIDEO_MESSAGE_RECEIVED_EVENT
		<< "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMeetingMediaMessgeToJSON(const_cast<ConfMediaMessageInfo&>(*message));

	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnConferenceRecordReceived event to JS";
}

void CalendarServiceStrategy::OnMessageUnreadUserCount(int32_t result, int64_t groupId, const std::list<MessageReadInfo>& info){

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MESSAGE_UNREAD_USER_COUNT_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ", groupId:" << groupId
		<< ", info:[";

	for (std::list<MessageReadInfo>::const_iterator iter = info.begin(); iter != info.end(); ++iter) {
		jsScript << ServiceObjectConversion::ConvertMessageReadInfoToJSON(*iter) << ",";
	}

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageUnreadUserCount event to JS";
}

void CalendarServiceStrategy::OnMeetingInviteMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_INVITE_MESSAGE_RECEIVED_EVENT
			 << "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendMeetingToJSON(meeting);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingInviteMessageReceived event to JS";
}

void CalendarServiceStrategy::OnMeetingUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) {

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_UPDATE_MESSAGE_RECEIVED_EVENT
			<< "\",{detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendMeetingToJSON(meeting);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingUpdateMessageReceived event to JS";
}

void CalendarServiceStrategy::OnMeetingForwardMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_FORWARD_MESSAGE_RECEIVED_EVENT
			 << "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendMeetingToJSON(meeting);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingForwardMessageReceived event to JS";
}

void CalendarServiceStrategy::OnMeetingAcceptMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_ACCEPT_MESSAGE_RECEIVED_EVENT
			<< "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendMeetingToJSON(meeting);

	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingAcceptMessageReceived event to JS";
}

void CalendarServiceStrategy::OnMeetingDenyMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_DENY_MESSAGE_RECEIVED_EVENT
			 << "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendMeetingToJSON(meeting);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingDenyMessageReceived event to JS";
}

void CalendarServiceStrategy::OnMeetingCancelMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_CANCEL_MESSAGE_RECEIVED_EVENT
		     << "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendMeetingToJSON(meeting);

	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingCancelMessageReceived event to JS";
}

void CalendarServiceStrategy::OnMinutesCreateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
	int64_t updateTime, int64_t minutesId, const std::string &minutesTitle, int32_t state, int64_t eventId, const std::string &confName, int64_t startTime/* = 0*/) {

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MINUTES_CREATE_MESSAGE_RECEIVED_EVENT
			 << "\",{detail: " << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "operatorId: " << operatorId << ", ";
	jsScript << "minutesId: " << minutesId << ", ";
	jsScript << "minutesTitle: \"" << URLDATA_UTF2WJSON(minutesTitle) << "\", ";
	jsScript << "eventId: " << eventId << ", ";
	jsScript << "state: " << state << ", ";
	jsScript << "confName: \"" << URLDATA_UTF2WJSON(confName) << "\", ";
	jsScript << "startTime: " << startTime << ", ";
	jsScript << "updateTime: " << updateTime << ", ";

	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMinutesCreateMessageReceived event to JS";
}

void CalendarServiceStrategy::OnMinutesUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
	int64_t updateTime, int64_t minutesId, const std::string &minutesTitle, int32_t state, int64_t eventId, const std::string &confName, int64_t startTime/* = 0*/) {

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MINUTES_UPDATE_MESSAGE_RECEIVED_EVENT
			 << "\",{detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "operatorId: " << operatorId << ", ";
	jsScript << "minutesId: " << minutesId << ", ";
	jsScript << "minutesTitle: \"" << URLDATA_UTF2WJSON(minutesTitle) << "\", ";
	jsScript << "eventId: " << eventId << ", ";
	jsScript << "state: " << state << ", ";
	jsScript << "confName: \"" << URLDATA_UTF2WJSON(confName) << "\", ";
	jsScript << "startTime: " << startTime << ", ";
	jsScript << "updateTime: " << updateTime << ", ";

	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMinutesUpdateMessageReceived event to JS";
}

void CalendarServiceStrategy::OnAccreditMessageReceived( MessageChannel channel, const MessageInfo &message ) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_ACCREDIT_MESSAGE_RECEIVED_EVENT
			 << "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAccreditMessageReceived event to JS";
}

void CalendarServiceStrategy::OnAccreditCancelMessageReceived( MessageChannel channel, const MessageInfo &message ) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_ACCREDIT_CANCEL_MESSAGE_RECEIVED_EVENT
			<< "\",{detail: " << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAccreditCancelMessageReceived event to JS";
}

void CalendarServiceStrategy::OnMeetingAlarmed(std::vector<Meeting>& infos) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETING_ALARMED_EVENT
		<< "\",{detail: " << "{meetings: [";

	for (std::size_t i = 0; i < infos.size(); ++i) {
		if (i != 0)
			jsScript << ", ";
		jsScript << ServiceObjectConversion::ConvertMeetingToJSON(infos[i]);
	}

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnConferenceAlarmed event to JS";
}

void CalendarServiceStrategy::OnPartJoinMessageReceived(MessageChannel channel, const MessageInfo &message) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_PARTJOIN_MESSAGE_RECEIVED_EVENT
		<< "\",{detail: " << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnPartJoinMessageReceived event to JS";
}

void CalendarServiceStrategy::OnPartCancelMessageReceived(MessageChannel channel, const MessageInfo &message) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_PARTCANCEL_MESSAGE_RECEIVED_EVENT
		<< "\",{detail: " << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnPartCancelMessageReceived event to JS";
}

void CalendarServiceStrategy::OnRemoteAssistanceInviteMsgSent(int32_t result, const std::string& conferenceId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_REMOTE_ASSISTANCE_INVITE_MSG_SENT_ENVNT
		<< "\",{detail: " << "{errorCode: " << result << 
		", conferenceId: \"" << UTF2WJSON(conferenceId) << "\", msgId: "
		<< msgId << ", msgSeq: " << msgSeq << ", msgTime: " << msgTime << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRemoteAssistanceInviteMsgSent event to JS";
}

void CalendarServiceStrategy::OnRemoteAssistanceAcceptMsgSent(int32_t result, const std::string& conferenceId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_REMOTE_ASSISTANCE_ACCEPT_MSG_SENT_ENVNT
		<< "\",{detail: " << "{errorCode: " << result <<
		", conferenceId: \"" << UTF2WJSON(conferenceId) << "\", msgId: "
		<< msgId << ", msgSeq: " << msgSeq << ", msgTime: " << msgTime << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRemoteAssistanceAcceptMsgSent event to JS";
}

void CalendarServiceStrategy::OnRemoteAssistanceRejectMsgSent(int32_t result, const std::string& conferenceId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_REMOTE_ASSISTANCE_REJECT_MSG_SENT_ENVNT
		<< "\",{detail: " << "{errorCode: " << result <<
		", conferenceId: \"" << UTF2WJSON(conferenceId) << "\", msgId: "
		<< msgId << ", msgSeq: " << msgSeq << ", msgTime: " << msgTime << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRemoteAssistanceRejectMsgSent event to JS";
}

void CalendarServiceStrategy::OnRemoteAssistanceInviteMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId, const std::string& confPwd) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_REMOTE_ASSISTANCE_INVITE_MSG_RECEIVED_ENVNT
		<< "\",{detail: " << "{channel: " << channel
		<< ", conferenceId: \"" << UTF2WJSON(confId)
		<< "\", conferencePwd: \"" << UTF2WJSON(confPwd) << "\", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRemoteAssistanceInviteMsgReceived event to JS";
}

void CalendarServiceStrategy::OnRemoteAssistanceAcceptMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_REMOTE_ASSISTANCE_ACCEPT_MSG_RECEIVED_ENVNT
		<< "\",{detail: " << "{channel: " << channel
		<< ", conferenceId: \"" << UTF2WJSON(confId) << "\", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRemoteAssistanceAcceptMsgReceived event to JS";
}

void CalendarServiceStrategy::OnRemoteAssistanceRejectMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_REMOTE_ASSISTANCE_REJECT_MSG_RECEIVED_ENVNT
		<< "\",{detail: " << "{channel: " << channel
		<< ", conferenceId: \"" << UTF2WJSON(confId) << "\", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRemoteAssistanceRejectMsgReceived event to JS";
}

void CalendarServiceStrategy::OnRemoteAssistanceEndMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_REMOTE_ASSISTANCE_END_MSG_RECEIVED_ENVNT
		<< "\",{detail: " << "{channel: " << channel
		<< ", conferenceId: \"" << UTF2WJSON(confId) << "\", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRemoteAssistanceEndMsgReceived event to JS";
}

int32_t CalendarServiceStrategy::SendRemotAssistanceInviteMsg(UCID& uid, std::wstring& confId, std::wstring& confPwd, OUT int32_t& msgId) {
	std::string utf8confId = StringConvert::ToUTF8Str(confId);
	std::string utf8confPwd = StringConvert::ToUTF8Str(confPwd);
	return _calendarService->SendRemoteAssistanceInviteMessage(uid, utf8confId, utf8confPwd, msgId);
}

int32_t CalendarServiceStrategy::SendRemotAssistanceAcceptMsg(UCID& uid, std::wstring& confId, OUT int32_t& msgId) {
	std::string utf8confId = StringConvert::ToUTF8Str(confId);
	return _calendarService->SendRemoteAssistanceAcceptMessage(uid, utf8confId, msgId);
}

int32_t CalendarServiceStrategy::SendRemotAssistanceRejectMsg(UCID& uid, std::wstring& confId, OUT int32_t& msgId) {
	std::string utf8confId = StringConvert::ToUTF8Str(confId);
	return _calendarService->SendRemoteAssistanceRejectMessage(uid, utf8confId, msgId);
}

void CalendarServiceStrategy::NotifyMeetingReadied() {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_NOTIFY_MEETING_READIED_ENVNT
		<< "\");document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch NotifyMeetingReadied event to JS";
}

void CalendarServiceStrategy::NotifyMeetingJoined() {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_NOTIFY_MEETING_JOINED_ENVNT
	 << "\");document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch NotifyMeetingJoined event to JS";
}

void CalendarServiceStrategy::NotifyMeetingJoinFailed() {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_NOTIFY_MEETING_JOIN_FAILED_ENVNT
		<< "\");document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch NotifyMeetingJoinFailed event to JS";
}

void CalendarServiceStrategy::NotifyMeetingClosed() {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_NOTIFY_MEETING_ClOSED_ENVNT
		<< "\");document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch NotifyMeetingClosed event to JS";
}

bool CalendarServiceStrategy::IsMeetingRunning() {
	UC_LOG(INFO) << "IsMeetingRunning enter.";
	//拷贝升级文件前先杀掉相关进程
	PROCESSENTRY32 pe32;
	//在使用这个结构之前，先设置它的大小
	pe32.dwSize = sizeof(pe32);
	//给系统内的所有进程拍一个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//遍历进程快照，轮流显示每个进程的信息
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);

	HANDLE hProcess = NULL;
	bool bRuning = false;
	while (bMore) {
		if (wcscmp(L"TangChat.exe", pe32.szExeFile) == 0 || wcscmp(L"TangClient.exe", pe32.szExeFile) == 0) {
			bRuning = true;
			break;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);
	UC_LOG(INFO) << "IsMeetingRunning over, is meeting running = " << bRuning;
	return bRuning;
}

void CalendarServiceStrategy::OnDocumentUploadMessageReceived(MessageChannel channel, int32_t operation, int64_t eventId, const MessageInfo &message, const DocumentInfo &doc) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_DOCUMENTUPLOAD_MESSAGE_RECEIVED
		<< "\", {detail:" << "{channel: " << channel << ", operation: " << operation << ", eventId: "
		<< eventId << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendDocumentInfoToJSON(doc);

	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch CalendarService::OnDocumentUploadMessageReceived event to JS";
}

void CalendarServiceStrategy::OnDocumentDeleteMessageReceived(MessageChannel channel, const MessageInfo &message, int64_t eventId, int64_t contentId, int32_t operatorId, const std::string &docName) {

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_DOCUMENTDELETE_MESSAGE_RECEIVED
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "eventId: " << eventId <<", operatorId: " << operatorId << ", " << "contentId:" << contentId << ", "
		<< "docName:\"" << UTF2WJSON(docName) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch calendar::OnDocumentDeleteMessageReceived event to JS";

}

void CalendarServiceStrategy::OnMeetingMessageRevocation(int32_t result, int64_t msgSeq, int32_t msgId, int64_t msgTime, int64_t eventId)
{
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_REVOCATION_MSG_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ", msgId:" << msgId << ", msgSeq:" << msgSeq 
		<< ", msgTime:" << msgTime
		<< ", revocationType:" << 0
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingMessageRevocation event to JS";
}

void CalendarServiceStrategy::OnMeetingMessageRevocationRecevied(MessageChannel channel, const MessageInfo &message, int64_t eventId, int64_t seqId)
{
	std::wostringstream jsScript;
	jsScript << L"var e= new CustomEvent(\"" << CALENDAR_SERVICE_REVOCATION_MESSAGE_EVENT
		<< "\",{detail:" << "{channel:" << channel << ",";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "seqId:" << seqId << ", type:" << 0 << ", eventId:" << eventId <<",";

	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingMessageRevocationRecevied event to JS";
}

int32_t CalendarServiceStrategy::UploadMeetingFile(int64_t eventId, int32_t operation, const std::wstring& fileName, int64_t fileSize, const std::wstring& fileURL, int64_t createTime, OUT int32_t& msgId) {
	std::string utf8FileName = StringConvert::ToUTF8Str(fileName);
	std::string utf8FileURL = StringConvert::ToUTF8Str(fileURL);

	DocumentInfo info;
	info.SetFileName(utf8FileName);
	info.SetFileSize(fileSize);
	info.SetFileUrl(utf8FileURL);
	info.SetUploadTime(createTime);
	info.SetCreateTime(createTime);

	return _calendarService->UploadMeetingFile(eventId, operation, info, msgId);
}

int32_t CalendarServiceStrategy::DeleteMeetingFile(int64_t minutesId) {
	return _calendarService->DeleteMeetingFile(minutesId);
}

int32_t CalendarServiceStrategy::UploadCloudFile(int64_t eventId, int32_t operation, int32_t mountId,
	const std::wstring& fileName, int64_t fileSize, DocumentCloudType cloudType, const std::wstring& hashCode,
	const std::wstring& detailContent, OUT int32_t& msgId) {
	std::string utf8FileName = StringConvert::ToUTF8Str(fileName);
	std::string utf8DetailContent = StringConvert::ToUTF8Str(detailContent);

	DocumentInfo info;
	info.SetFileName(utf8FileName);
	info.SetFileSize(fileSize);
	info.SetCloudType(DocumentCloudType(cloudType));
	info.SetDetailContent(utf8DetailContent);
	info.SetMountId(mountId);
	info.SetDetailType(1);
	info.SetHashCode(StringConvert::ToUTF8Str(hashCode));

	return _calendarService->UploadMeetingFile(eventId, operation, info, msgId);
}
void CalendarServiceStrategy::OnMeetingRoomCreated(int32_t errorCode, MeetingRoom& room)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETINGROOM_CREATED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", meetingRoom: ";

	jsScript << ServiceObjectConversion::ConvertMeetingRoomToJSON(room) << "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingRoomCreated event to JS";
}

void CalendarServiceStrategy::OnMeetingRoomUpdated(int32_t errorCode, MeetingRoom& room)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETINGROOM_UPDATED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", meetingRoom: ";

	jsScript << ServiceObjectConversion::ConvertMeetingRoomToJSON(room) << "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingRoomUpdated event to JS";
}

void CalendarServiceStrategy::OnMeetingCreatedInMeetingRoom(int32_t errorCode, const std::string& roomId, const Meeting& meeting)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETINGROOM_MEETING_CREATED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", roomId: \"" << UTF2WJSON(roomId) << "\", meeting: ";

	jsScript << ServiceObjectConversion::ConvertMeetingToJSON(meeting) << "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingCreatedInMeetingRoom event to JS";

}

void CalendarServiceStrategy::OnMeetingRoomListReceived(int32_t errorCode, int64_t orgId, std::list<MeetingRoom>& rooms)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETINGROOM_LIST_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", orgId: " << orgId << ", rooms: [";
	std::list<MeetingRoom>::const_iterator iter = rooms.begin();
	while(iter != rooms.end()) {
		if (iter != rooms.begin())
			jsScript << ", ";
		std::wstring json = ServiceObjectConversion::ConvertMeetingRoomToJSON(*iter);
		jsScript << json;
		iter++;
	}

	jsScript << "]},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingRoomListReceived event to JS";
}

void CalendarServiceStrategy::OnCheckJoinMeetingRoom(int32_t errorCode, const std::string& roomId, int32_t hasPassword, int32_t isValid, int32_t mayJoin)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_CHECK_JOIN_MEETINGROOM_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", roomId: \"" << UTF2WJSON(roomId) << "\", hasPassword: " << hasPassword 
		<< ", isValid: " << isValid << ", mayJoin: " << mayJoin << "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCheckJoinMeetingRoom event to JS";
}

void CalendarServiceStrategy::OnCheckMeetingRoomPassword(int32_t errorCode, const std::string& roomId, const std::string& password, int32_t value)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_CHECK_MEETINGROOM_PASSWORD_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", roomId: \"" << UTF2WJSON(roomId) << "\", password: \"" << UTF2WJSON(password)
		<< "\", value: " << value << "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCheckMeetingRoomPassword event to JS";
}

void CalendarServiceStrategy::OnMeetingRoomDeleted(int32_t errorCode, const std::string& roomId)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_MEETINGROOM_DELETED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", roomId: \"" << UTF2WJSON(roomId) << "\"},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingRoomDeleted event to JS";
}

void CalendarServiceStrategy::OnRightsChecked(int32_t errorCode)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_RIGHTS_CHECKED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << "},});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRightsChecked event to JS";
}


int32_t CalendarServiceStrategy::GetUserBusyFreeInfo(const std::vector<int32_t>& userIdList, int64_t startTime, int64_t endTime, int32_t shareUserId, OUT int32_t& requestId)
{
	return _calendarService->GetUserBusyFreeInfo(userIdList, startTime, endTime, shareUserId, requestId);
}

void CalendarServiceStrategy::OnUserBusyFreeInfoReceived(int32_t errorCode, int32_t requestId, std::vector<UserBusyFreeInfo>& schedules)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CALENDAR_SERVICE_USER_BUSYFREE_INFO_RECEIVED_EVENT
		<< "\",{detail: " << "{errorCode: " << errorCode << ", requestId:"
		<< requestId << ", schedules: [";

	for (std::size_t i = 0; i < schedules.size(); ++i) {
		if (i != 0)
			jsScript << ",";

		jsScript << ServiceObjectConversion::ConvertUserBusyFreeInfoToJSON(schedules[i]);
	}

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUserBusyFreeInfoReceived event to JS";

}

}