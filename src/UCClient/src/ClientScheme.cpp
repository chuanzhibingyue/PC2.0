#include "common/StringConvert.h"
#include "UCManager.h"
#include "ServiceObjectConversion.h"
#include "ClientScheme.h"
#include "UCErrorCode.h"
#include "utils/Thumbnails.h"
#include "LogManager.h"
#include "utils/Utils.h"

namespace ucclient {
using namespace uc;

#define SUCCESS_STATUS	200;
#define FAIL_STATUS		201;

bool ClientSchemeHandler::ProcessRequest(CefRefPtr<CefRequest> request,CefRefPtr<CefCallback> callback)
{	
	_dataW.resize(0);
	_data.resize(0);

	_status = FAIL_STATUS;
	
	std::string url = StringConvert::URLDecode(request->GetURL().ToString());
	std::wstring module, method;
	std::vector<std::wstring> params;
	ParseUrl(url, module, method, params);

	if (module.empty() || method.empty()) {
		callback->Continue();
		return false;
	}

	UC_LOG(INFO) << "ClientSchemeHandler::ProcessRequest, url: " << url;

	if (module == L"ContactService") {
		_status = handleContactService(method, params, _dataW);
	}
	else if (module == L"ClientService") {
		_status = handleClientService(method, params, _dataW);
	}
	else if (module == L"CalendarService") {
		_status = handleCalendarService(method, params, _dataW);
	}
	else if (module == L"System") {
		_status = handleSystemService(method, params, _dataW);
	}
	else if (module == L"FileTransferService") {
		_status = handleFileTransferService(method, params, _dataW);
	}
	else if (module == L"GroupService") {
		_status = handleGroupService(method, params, _dataW);
	}
	else if (module == L"UpdateService") {
		_status = handleUpdateService(method, params, _dataW);
	}
	else if (module == L"MicroblogService") {
		_status = handleMicroblogService(method, params, _dataW);
	}
	else if (module == L"ExternalService"){
		_status = handleExternalService(method, params, _dataW);
	}
	else if (module == L"Util") {
		_status = handleUtilService(method, params, _dataW);
	}
    else if (module == L"Window") {
        _status = handleWindow(method, params, _dataW);
    }
	else if (module == L"AudioService") {
		_status = handleAudioService(method, params, _dataW);
	}
	else {

	}

	callback->Continue();

	UC_LOG(INFO) << "finish ClientSchemeHandler::ProcessRequest, url: " << url;

    return true;
}

int ClientSchemeHandler::handleGroupService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
	int status = FAIL_STATUS;

	if (method == L"GetGroupInfoSync") {
		if (params.size() != 1) 
			return status;
		
		int32_t groupId = StringConvert::WStringToInt(params[0]);
		GroupInfo info;
		int32_t result = UCManager::GetInstance().GetGroupServiceStrategy()->GetGroupInfo(groupId, info);
		if (UC_LOCALDATA_SUCCESSED == result) {
			data = ServiceObjectConversion::ConvertGroupInfoToJSON(info);
			status = SUCCESS_STATUS;
		}
	} 
	else if (method == L"GetGroupInfoByMountIdSync") {
		if (params.size() != 1)
			return status;

		int32_t mountId = StringConvert::WStringToInt(params[0]);
		GroupInfo info;
		int32_t result = UCManager::GetInstance().GetGroupServiceStrategy()->GetGroupInfo(0, info, false, mountId);
		if (UC_LOCALDATA_SUCCESSED == result) {
			data = ServiceObjectConversion::ConvertGroupInfoToJSON(info);
			status = SUCCESS_STATUS;
		}
	}

	return status;
}

int ClientSchemeHandler::handleExternalService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data){
	int status = FAIL_STATUS;

	if (method == L"ShowExternalMessage") {
		if (params.size() != 1)
			return status;

		std::wstring externalId = params[0];
		data = StringConvert::FromUTF8Str(StringConvert::Convert2String(
			UCManager::GetInstance().GetExternalServiceStrategy()->ShowExternalMessage(externalId)));
		status = SUCCESS_STATUS;
	}

	return status;
}

int ClientSchemeHandler::handleFileTransferService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
	int status = FAIL_STATUS;

	if (method == L"getLocalFilePathSync") {
		if (params.size() != 1) 
			return status;

		std::wstring fileURL = L"";
		std::wstring filePath = L"";

		fileURL = params[0];
		UCManager::GetInstance().GetFileTransferServiceStrategy()->GetLocalFilePath(fileURL, filePath);
		data = filePath;
		status = SUCCESS_STATUS;
	} else if (method == L"getDownloadUrlFromFidSync") {
		if (params.size() != 1)
			return status;

		UCManager::GetInstance().GetFileTransferServiceStrategy()->GetDownloadUrlFromFid(params[0], data);
		status = SUCCESS_STATUS;
	}

	return status;
}

int ClientSchemeHandler::handleSystemService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
	int status = FAIL_STATUS;

	if (method == L"getClipboardData") {
		std::string hello = UCManager::GetInstance().getSystemService()->getClipboardData(NULL);
		data = StringConvert::FromUTF8Str(hello);
		status = SUCCESS_STATUS;
		UC_LOG(INFO) << "*****getClipboardData: " << data;
	} 
	else if (method == L"setClipboardData") {
		if (params.size() != 1) {
			return status;
		}
			
		if (UCManager::GetInstance().getSystemService()->setClipboardData(StringConvert::ToUTF8Str(params[0]), _browser)) {
			status = SUCCESS_STATUS;
		}

		UC_LOG(INFO) << "*****setClipboardData: " << params[0];
	}
	else if (method == L"hasValidClipboardData") {
		data = StringConvert::FromUTF8Str(UCManager::GetInstance().getSystemService()->hasValidClipboardData(NULL));
		UC_LOG(INFO) << "*****hasValidClipboardData: " << data;
		status = SUCCESS_STATUS;	
	} 
	else if (method == L"createThumbnailsSync") {
		if (params.size() != 1 ) 
			return status;
		
		std::wstring path = params[0];
		std::wstring thumbnailPath = L"";
		std::string base64Data = "";
		bool bRet = ThumbnailsUtil::CreateThumbnails(path, thumbnailPath, base64Data);
		if ( bRet ) {
			wstringstream ss;

			ss << L"{\"path\":" << L"\"" << thumbnailPath << L"\", \"data\":" << L"\"" << StringConvert::FromUTF8Str(base64Data) << L"\"}";
			data = ss.str();
			status = SUCCESS_STATUS;
		}
	} 

	return status;
}

int ClientSchemeHandler::handleContactService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
	int status = FAIL_STATUS;
	if (method == L"GetContactInfoSync") {
		if (params.size() != 1)  
			return status;
		
		int32_t userId = StringConvert::WStringToInt(params[0]);
		ContactInfo info;
		bool localHas = UCManager::GetInstance().GetContactServiceStrategy()->GetContactInfo(userId, info);
		if (localHas) {
			data = ServiceObjectConversion::ConvertContactToJSON(info);
			status = SUCCESS_STATUS;
		}
	}
	else if(method == L"GetBasicContactInfoSync") {
		int32_t userId = StringConvert::WStringToInt(params[0]);
		BasicContactInfo info;
		bool localHas = UCManager::GetInstance().GetContactServiceStrategy()->GetBasicContactInfo(userId, info);
		if (localHas) {
			data = ServiceObjectConversion::ConvertBasicContactInfoToJSON(info);
			status = SUCCESS_STATUS;
		}
	}
	else if (method == L"GetCurrentUserInfoSync") {
		ContactInfo info;
		bool isOk = UCManager::GetInstance().GetContactServiceStrategy()->GetCurrentUserInfo(info);

		if (isOk) {
			data = ServiceObjectConversion::ConvertContactToJSON(info);
			status = SUCCESS_STATUS;
		}
	}
	else if (method == L"GetCurrentUserFullInfoSync"){
		ContactInfo info;
		bool isOk = UCManager::GetInstance().GetContactServiceStrategy()->
			GetCurrentUserFullInfo(info);
		if (isOk)
		{
			data = ServiceObjectConversion::ConvertContactToJSON(info);
			status = SUCCESS_STATUS;
		}

	}

	return status;
}

int ClientSchemeHandler::handleClientService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
	int status = FAIL_STATUS;

	if (method == L"GetDocumentDataDirSync") {
		if (params.size() != 1)  
			return status;
		
		DocDirType type = (DocDirType)StringConvert::WStringToInt(params[0]);
		data = StringConvert::FromUTF8Str(UCManager::GetInstance().GetClientServiceStrategy()->GetDocumentDataDir(type));
		status = SUCCESS_STATUS;
	}
	else if (method == L"LoadHistoryAccounts") {
		std::list<AccountInfo> accounts;
		int32_t result  = UCManager::GetInstance().GetClientServiceStrategy()->LoadHistoryAccounts(accounts);
		if (0 != result || accounts.empty()) 
			return status;

		data.append(L"[");
		std::list<AccountInfo>::iterator it = accounts.begin();
		while(it != accounts.end() ) {
			const AccountInfo& info = *it;
			if (it != accounts.begin())
				data.append(L",");
			data.append(ServiceObjectConversion::ConvertAccountInfoToJSON(info));
			++it;							
		}
		data.append(L"]");
		
		status = SUCCESS_STATUS;
	} else if (method == L"GetCurUCID") {
		UCID curID = UCManager::GetInstance().GetClientServiceStrategy()->GetCurUCID();
		std::wstring json = ServiceObjectConversion::ConvertUCIDToJSON(curID);
		data.append(json);

		status = SUCCESS_STATUS;
    } else if (method == L"GetDefaultFileSavePath") {
		std::wstring filename = params[0];
		std::wstring fullpath = UCManager::GetInstance().GetClientServiceStrategy()->GetDefaultFileSavePath(filename);
		std::string fname = StringConvert::ToUTF8Str(fullpath);

		wstringstream json;
		json << "[\"" << fullpath << "\", " << "\"" << StringConvert::FromUTF8Str(fname.substr(fname.find_last_of("\\") + 1, fname.length())) << "\"]";
		data = json.str();

        status = SUCCESS_STATUS;
	} else if (method == L"GetMinGroupMemberLimit") {
		data = StringConvert::FromUTF8Str(StringConvert::Convert2String(
			UCManager::GetInstance().GetClientServiceStrategy()->GetMinGroupMemberLimit()));
		status = SUCCESS_STATUS;
	}
	else if (method == L"GetMaxGroupMemberLimit") {
		data = StringConvert::FromUTF8Str(StringConvert::Convert2String(
			UCManager::GetInstance().GetClientServiceStrategy()->GetMaxGroupMemberLimit()));
		status = SUCCESS_STATUS;
	}
	else if (method == L"GetGroupCountLimit") {
		data = StringConvert::FromUTF8Str(StringConvert::Convert2String(
			UCManager::GetInstance().GetClientServiceStrategy()->GetGroupCountLimit()));
		status = SUCCESS_STATUS;
	}
	else if (method == L"UpdateValueBykeySync"){
		std::wstring app = params[0];
		std::wstring name = params[1];
		std::wstring value = params[2];
		UCManager::GetInstance().GetClientServiceStrategy()->UpdateValueBykey(app, name, value);
		status = SUCCESS_STATUS;
	}
	else if (method == L"GetValueByKeySync"){
		std::wstring app = params[0];
		std::wstring name = params[1];

		UCManager::GetInstance().GetClientServiceStrategy()->GetValueByKey(app, name, data);
		status = SUCCESS_STATUS;
	}
	else if (method == L"GetConversationInfoSync"){

		ConversationType type = (ConversationType)StringConvert::WStringToInt(params[0]);
		int64_t relateId = StringConvert::WStringToInt64(params[1]);
		ConversationInfo info;
		int32_t result = UCManager::GetInstance().GetClientServiceStrategy()->GetConversationInfo(type, relateId, info);
		if (UC_LOCALDATA_SUCCESSED == result) {
			data = ServiceObjectConversion::ConvertConverstaionInfoToJSON(info);
			status = SUCCESS_STATUS;
		}
	}

	return status;
}

int ClientSchemeHandler::handleCalendarService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
	int status = FAIL_STATUS;

	if (method == L"GetMeetingInfoByIdSync"){
		if (params.size() != 4) 
			return status;
		
		int64_t eventId = StringConvert::WStringToInt64(params[0]);
		if (eventId <= 0) {
			return status;
		}
		int64_t startTime = StringConvert::WStringToInt64(params[1]);
		int64_t endTime = StringConvert::WStringToInt64(params[2]);
		int32_t authorizerId = StringConvert::WStringToInt(params[3]);
		vector<Meeting> infos;
		int32_t result = UCManager::GetInstance().GetCalendarServiceStrategy()->GetMeetingInfoById(eventId, startTime, endTime, authorizerId, infos);
		if (0 != result || infos.empty()) 
			return status;
		
		data.append(L"[");
		std::vector<Meeting>::iterator it = infos.begin();
		while(it != infos.end() ) {
			if (it != infos.begin())
				data.append(L",");

			Meeting info = *it;
			data.append(ServiceObjectConversion::ConvertMeetingToJSON(info));
			++it;							
		}
		data.append(L"]");
		
		status = SUCCESS_STATUS;
	} 
	else if (method == L"GetMeetingInfoByIdsSync") {
		if (params.size() != 4)
			return status;
		
		vector<int64_t> eventIds;
		std::wstring ids = params[0];
		int pos = ids.find_first_of(',');
		while((pos = ids.find_first_of(',')) != -1) {
			std::wstring id = ids.substr(0, pos);
			eventIds.push_back(StringConvert::WStringToInt64(id));
			ids = ids.substr(pos+1, ids.length()-pos);
		}
		if (ids.length() >1) {
			eventIds.push_back(StringConvert::WStringToInt64(ids));
		}

		if (eventIds.empty()) {
			return status;
		}
		int64_t startTime = StringConvert::WStringToInt64(params[1]);
		int64_t endTime = StringConvert::WStringToInt64(params[2]);
		int32_t authorizerId = StringConvert::WStringToInt(params[3]);
		vector<Meeting> infos;
		int32_t requestId = 0;
		int32_t result = UCManager::GetInstance().GetCalendarServiceStrategy()->GetMeetingInfoByIds(eventIds, startTime, endTime, authorizerId, requestId, infos);
		if (0 != result || infos.empty()) 
			return status;
		
		data.append(L"[");
		std::vector<Meeting>::iterator it = infos.begin();
		while(it != infos.end() ) {
			if (it != infos.begin())
				data.append(L",");

			Meeting info = *it;
			data.append( ServiceObjectConversion::ConvertMeetingToJSON(info));
			++it;							
		}
		data.append(L"]");
		
		status = SUCCESS_STATUS;
	} 
	else if (method == L"GetMeetingListSync") {
		if (params.size() != 7) 
			return status;

		int64_t startTime = StringConvert::WStringToInt64(params[0]);
		int64_t endTime = StringConvert::WStringToInt64(params[1]);
		if (startTime <=0 && endTime <=0) {
			return status;
		}
		int32_t limit = StringConvert::WStringToInt(params[2]);
		int32_t authorizerId = StringConvert::WStringToInt(params[3]);
		int64_t eventIdFilter = StringConvert::WStringToInt64(params[4]);
		std::wstring s = params[5];
		bool bFilterAsc = true;
		if (s.compare(L"false") == 0 || s.compare(L"FALSE") == 0 || s.compare(L"0") == 0) {
			bFilterAsc = false;
		}
		
		int32_t refresh = StringConvert::WStringToInt(params[6]);
		vector<Meeting> infos;
		int32_t requestId;
		int32_t result = UCManager::GetInstance().GetCalendarServiceStrategy()->GetMeetingList(startTime, endTime, limit, authorizerId, eventIdFilter, bFilterAsc, refresh, requestId, infos);
		if (0 != result || infos.empty()) 
			return status;

		data.append(L"[");
		std::vector<Meeting>::iterator it = infos.begin();
		while(it != infos.end() ) {
			if (it != infos.begin())
				data.append(L",");

			Meeting info = *it;
			data.append(ServiceObjectConversion::ConvertMeetingToJSON(info));
			++it;							
		}
		data.append(L"]");
		
		status = SUCCESS_STATUS;
	} 
	else if (method == L"GetAccreditUserListSync") {
		if (params.size() != 0) 
			return status;
		
		vector<Accrediting> infos;
		int32_t result = UCManager::GetInstance().GetCalendarServiceStrategy()->GetAccreditUserList(infos);
		if (0 != result || infos.empty())
			return status;
		
		data.append(L"[");
		std::vector<Accrediting>::iterator it = infos.begin();
		while(it != infos.end() ) {
			if (it != infos.begin())
				data.append(L",");

			data.append(ServiceObjectConversion::ConvertAccreditInfoToJSON(*it));
			++it;							
		}
		data.append(L"]");
		
		status = SUCCESS_STATUS;
	}
	else if (method == L"GetMeetingStartURLSync") {
		if (params.size() != 2)
			return status;

		std::wstring confId = params[0];
		std::wstring passWord = params[1];
		std::string startURL = "";
		int32_t siteId = 60001;
		int32_t result = UCManager::GetInstance().GetCalendarServiceStrategy()->GetMeetingStartURL(confId, passWord, startURL, siteId);
		if (0 != result || startURL.empty())
			return status;
		std::wstringstream ss;
		ss << L"{" << "\"startURL\": \"" << StringConvert::FromUTF8Str(startURL) << L"\", \"siteId\": " << siteId << L"}";
		data.append(ss.str());
		status = SUCCESS_STATUS;
	}

	return status;
}

int ClientSchemeHandler::handleUpdateService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
	int status = FAIL_STATUS;
	if (method == L"GetLocalVersion") {
		data = UCManager::GetInstance().GetUpdateServiceStrategy()->GetLocalVersion();
		status = SUCCESS_STATUS;
	}
	return status;
}

int ClientSchemeHandler::handleMicroblogService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
	int status = FAIL_STATUS;
	if (method == L"GetRelationSync") {
		if (params.size() != 1)
			return status;
		int64_t userId = StringConvert::WStringToInt(params[0]);
		int32_t relation = UCManager::GetInstance().GetMicroblogServiceStrategy()->GetRelation(userId);

		wstringstream ss;
		ss << relation;
		ss >> data;

		status = SUCCESS_STATUS;
	}
	return status;
}

int ClientSchemeHandler::handleUtilService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
	int status = FAIL_STATUS;

	if (method == L"getMeetingClientStatus") {
		wstringstream ss;
		ss << L"{\"MeetingClient\": { \"visible\":" << (Utils::isWindowVisible(L"UCChat_MainFrame", L"") ? 1 : 0) << L"}}";
		data = ss.str();
		status = SUCCESS_STATUS;
	}

	return status;
}

int ClientSchemeHandler::handleWindow(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
    int status = FAIL_STATUS;

    if (method == L"GetTrayPos") {
        std::wstring iconPos;
        _cefClient->GetTrayPos(_browser, iconPos);
        data = iconPos;
        status = SUCCESS_STATUS;
    }
    else if (method == L"GetWindowStatue")
    {
        wstringstream ss;
        ss << _cefClient->GetWindowStatue(_browser);
        data = ss.str();
        status = SUCCESS_STATUS;
    }
    else if (method == L"IsWindowOpen")
    {
        wstringstream ss;
        ss << _cefClient->IsWindowOpen(_browser, params[0]);
        data = ss.str();
        status = SUCCESS_STATUS;
    }
	else if (method == L"GetWindowPos") {
		_cefClient->getWindowManager()->GetWindowPos(_browser, data);
		status = SUCCESS_STATUS;
	}

    return status;
}
int ClientSchemeHandler::handleAudioService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data) {
	int status = FAIL_STATUS;

	if (method == L"GetSpeakerList") {
		data = UCManager::GetInstance().GetAudioServiceStrategy()->GetSpeakerList();
		status = SUCCESS_STATUS;
	}
	else if (method == L"GetMicrophoneList") {
		data = UCManager::GetInstance().GetAudioServiceStrategy()->GetMicrophoneList();
		status = SUCCESS_STATUS;
	}
	else if (method == L"GetSpeakerNum") {
		wstringstream speakerNum;
		speakerNum << UCManager::GetInstance().GetAudioServiceStrategy()->GetSpeakerNum();
		data = speakerNum.str();
		status = SUCCESS_STATUS;
	}
	else if (method == L"GetMicrophoneNum") {
		wstringstream microphoneNum;
		microphoneNum << UCManager::GetInstance().GetAudioServiceStrategy()->GetMicrophoneNum();
		data = microphoneNum.str();
		status = SUCCESS_STATUS;
	}
	else if (method == L"GetCameraList") {
		data = UCManager::GetInstance().GetAudioServiceStrategy()->GetCameraList();
		status = SUCCESS_STATUS;
	}
	else if (method == L"CameraDiagnosis") {
		if (params.size() != 2) {
			return status;
		}
		wstringstream result;
		std::wstring camera = params[0];
		int64_t hwnd = StringConvert::WStringToInt64(params[1]);
		result << UCManager::GetInstance().GetAudioServiceStrategy()->CameraDiagnosis(camera, (void*)hwnd);
		data = result.str();
		status = SUCCESS_STATUS;
	}
	else if (method == L"StopCameraDiagnosis") {
		wstringstream result;
		result << UCManager::GetInstance().GetAudioServiceStrategy()->StopCameraDiagnosis();
		data = result.str();
		status = SUCCESS_STATUS;
	}
	else {
		//nothing to do
		UC_LOG(ERROR) << "invalid method:" << method;
	}
	return status;
}

void ClientSchemeHandler::ParseUrl(std::string & url, std::wstring& module, std::wstring& name, std::vector<std::wstring>& params){
    std::wstring wurl = StringConvert::FromUTF8Str(url);

    //parse the module, name and argments from url
    std::size_t modulePos = wurl.find(L"client://javascript/");
	if(modulePos != std::wstring::npos) {
		modulePos += wcslen(L"client://javascript/");
	}
	std::size_t namePos = wurl.find(L".", modulePos);
	std::size_t idPos = wurl.find(L".", namePos + 1);
	std::size_t markPos = wurl.find(L"?");

	if (modulePos != std::wstring::npos && namePos != std::wstring::npos && idPos != std::wstring::npos && markPos != std::wstring::npos) {
		module = wurl.substr(modulePos, namePos - modulePos);
		name = wurl.substr(namePos + 1, idPos - namePos - 1);
		
		if (markPos + 1 >= wurl.size() || wurl.at(markPos + 1) == L'\0') {
			return;
		}
		else if (wurl.at(markPos + 1) == L'#') {
			params.push_back(L"");
			return;
		}
		StringConvert::Split(wurl.substr(markPos + 1, wurl.size()- markPos), wstring(L"###"), params);
	}
}

void ClientSchemeHandler::GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) {
    response->SetMimeType("text/html");
    response->SetStatus(_status);
	
	CefResponse::HeaderMap headers;

	headers.insert(std::pair<CefString, CefString>("Access-Control-Allow-Origin", "*"));
	response->SetHeaderMap(headers);
	
	if (!_dataW.empty()) {
		_data.resize(0);
		_data = StringConvert::ToUTF8Str(_dataW);
	}

	response_length = _data.size();
}

bool ClientSchemeHandler::ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) {
	bool hasData = false;
	bytes_read = 0;

	if (_offset < _data.length()) {
		// Copy the next block of data into the buffer.
		int transfer_size = min(bytes_to_read, static_cast<int>(_data.length() - _offset));
		memcpy(data_out, _data.c_str() + _offset, transfer_size);
		_offset += transfer_size;

		bytes_read = transfer_size;
		hasData = true;
	}

	return hasData;
}

}