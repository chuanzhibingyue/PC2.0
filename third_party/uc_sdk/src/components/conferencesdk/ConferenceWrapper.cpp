
//
//  ConferenceWrapper.cpp
//  BeeCoference
//
//  Created by Hualong Zhang on 15/5/20.
//  Copyright (c) 2015骞� Hualong Zhang. All rights reserved.
//

#include "ConferenceWrapper.h"
#include "ConferenceSink.h"
#include "CGNetTangCommonDef.h"
#include "GNetTangPropertiesDefine.h"
#include "ConferenceDef.h"
#include "audio_service/ConferenceLibraryAssist.h"
#include "LogManager.h"
#include "common/StringConvert.h"

using namespace uc;
namespace uc {

BSTR ConferenceWrapper::_logPath = NULL;
ConferenceWrapper& ConferenceWrapper::GetConferenceWrapper() {
	static ConferenceWrapper wrapper;
	return wrapper;
}

ConferenceWrapper::ConferenceWrapper() {
	UC_LOG(INFO) << "create ConferenceWrapper!";
#if defined(WIN32) || defined(WIN64)
	static ConferenceLibraryAssist conferenceLibraryAssist;
	_getTangService = conferenceLibraryAssist.GetFunCreateTangService();
	UC_LOG(INFO) << "conferenceLibraryAssist.GetFunCreateTangService done";
	_tangService = _getTangService();
	UC_LOG(INFO) << "_getTangService() done";

	//set log path
	CGNetTangVariant var;
	var.vt = VT_BSTR;
	var.bstrVal = _logPath;
	_tangService->setGNetTangConfig(GNETTANGCONFIG_SETAPPLOGPATH, var);

	UC_LOG(INFO) << "_tangService->setGNetTangConfig(GNETTANGCONFIG_SETAPPLOGPATH, var); done";
	_aVWizard = _tangService->getAVWizard();
	//_createConference = conferenceLibraryAssist.GetFunCreateConference();
	//_deleteConference = conferenceLibraryAssist.GetFunDeleteConference();
#else
	CREATE_CONFERENCE_FUN createConference = createGNetTangConferenceWithAESECB128;
	DELETE_CONFERENCE_FUN deleteConference = deleteGNetTangConference;
#endif
	UC_LOG(INFO) << "create ConferenceWrapper End";
}

ConferenceWrapper::~ConferenceWrapper() {
	if (NULL != _conferenceSink) {
		delete _conferenceSink;
		_conferenceSink = NULL;
	}

	::SysFreeString(_logPath);
	_logPath = NULL;
}

///////////////////////////////conference////////////////////////////////
int ConferenceWrapper::JoinConference(const std::string &joinKey, CallType callType, JoinType JoinType, int conferenceId, UCID uid, int tangUserId) {
	UC_LOG(INFO) << "JoinConference, joinKey:" << joinKey << ", callType:" << callType
		<< ", JoinType:" << JoinType << ", conferenceId:" << conferenceId << ", userId:" << uid.GetUserId();
	_ReleaseConference();

	UC_LOG(INFO) << "JoinConference new sink and conference!";
	_conferenceSink = new ConferenceSink(callType, JoinType, conferenceId, uid, tangUserId);

	_conference = _tangService->createGNetTangConference(joinKey.c_str(), _conferenceSink);
	UC_LOG(WARNING) << "createGNetTangConference return " << _conference;

	_conferenceSink->SetConfDelegate(_conferenceEvent);
	_conferenceSink->SetVideoDelegate(_videoEvent);
	_conferenceSink->SetDesktopDelegate(_desktopEvent);
	_conferenceSink->SetConference(_conference);
    return _conference->joinConf();
}

int ConferenceWrapper::LeaveConference() {
	UC_LOG(INFO) << "LeaveConference";

	if (_conference == NULL) {
		UC_LOG(ERROR) << "LeaveConference _conference is null";
		return -1;
	}
	int ret = _HangupPhone(_GetSelfUserID());
	UC_LOG(INFO) << "HangupPhone before leave conference, result is : " << ret;
	if (GetParticipantNum() <= 1 /*&& _conferenceSink->GetCallType() != SINGLE_CALL*/) {
		UC_LOG(WARNING) << "LeaveConference, need to end conference!";
		ret = _conference->endConf();
	}
	else {
		ret = _conference->leaveConf();
	}
	return ret;
}

int ConferenceWrapper::EndConference() {
	UC_LOG(INFO) << "EndConference";

	if (_conference == NULL) {
		UC_LOG(ERROR) << "EndConference _conference is null";
		return -1;
	}
	return _conference->endConf();
}

int ConferenceWrapper::ReconnectConference() {
    UC_LOG(INFO) << "ReconnectConference";

	if (_conference == NULL) {
		UC_LOG(ERROR) << "ReconnectConference _conference is null";
		return -1;
	}
    return _conference->reconnectConf();
}

///////////////////////////////audio///////////////////////////////
int ConferenceWrapper::SwitchToPhone(const std::string &phoneNumber) {
    UC_LOG(INFO) << "SwitchToPhone: " << phoneNumber;

	int ret = _StopVOIP();
    UC_LOG(INFO) << "StopVOIP, result is : " << ret;
    return _CallPhone(phoneNumber, _GetSelfUserID());
}

int ConferenceWrapper::SwitchToVOIP() {
    UC_LOG(INFO) << "SwitchToVOIP";
    
	int ret = _HangupPhone(_GetSelfUserID());
	UC_LOG(INFO) << "_HangupPhone, result is : " << ret;
    return _StartVOIP();
}

int ConferenceWrapper::MuteSelf() {
    UC_LOG(INFO) << "MuteSelf";

	if (_conferenceSink == NULL) {
		UC_LOG(ERROR) << "MuteSelf _conferenceSink is null";
		return -1;
	}
    IGNetTangAudioSession *pAudio = _conferenceSink->GetAudioService();
    if (NULL == pAudio) {
        UC_LOG(ERROR) << "MuteSelf, pAudio is NULL";
		return -1;
    }
    
	unsigned int userID;
	userID = _GetSelfUserID();
	UC_LOG(INFO) << "MuteSelf userId:" << userID;
	return pAudio->muteUser(&userID, 1);
}

int ConferenceWrapper::UnMuteSelf() {
    UC_LOG(INFO) << "UnMuteSelf";

	if (_conferenceSink == NULL) {
		UC_LOG(ERROR) << "UnMuteSelf _conferenceSink is null";
		return -1;
	}
    IGNetTangAudioSession *pAudio = _conferenceSink->GetAudioService();
	if (NULL == pAudio) {
        UC_LOG(ERROR) << "unMuteSelf, pAudio is NULL";
		return -1;
    }
	
	unsigned int userID;
	userID = _GetSelfUserID();
	UC_LOG(INFO) << "UnMuteSelf, userId:" << userID;
	return pAudio->unMuteUser(&userID, 1);
}

int ConferenceWrapper::HangupPhone() {
	int ret = _HangupPhone(_GetSelfUserID());
	UC_LOG(INFO) << "HangupPhone, result:" << ret;
	return ret;
}

int ConferenceWrapper::EnableLoudSpeaker() {
	UC_LOG(INFO) << "EnableLoudSpeaker";
#if defined(WIN32) || defined(WIN64)
	return S_OK;
#else
	if (_conferenceSink == NULL) {
		UC_LOG(ERROR) << "EnableLoudSpeaker _conferenceSink is null";
		return -1;
	}
	IGNetTangAudioSession *pAudio = _conferenceSink->GetAudioService();
	if (NULL == pAudio) {
		UC_LOG(ERROR) << "EnableLoudSpeaker, pAudio is NULL";
		return -1;
	}
	return pAudio->enableLoudSpeaker();
#endif
}

int ConferenceWrapper::DisableLoudSpeaker() {
	UC_LOG(INFO) << "DisableLoudSpeaker";
#if defined(WIN32) || defined(WIN64)
	return S_OK;
#else
	if (_conferenceSink == NULL) {
		UC_LOG(ERROR) << "DisableLoudSpeaker _conferenceSink is null";
		return -1;
	}
	IGNetTangAudioSession *pAudio = _conferenceSink->GetAudioService();
	if (NULL == pAudio) {
		UC_LOG(ERROR) << "DisableLoudSpeaker, pAudio is NULL";
		return -1;
	}
	return pAudio->disableLoudSpeaker();
#endif
}

int ConferenceWrapper::_StopVOIP() {
	UC_LOG(INFO) << "_StopVOIP";

	if (_conferenceSink == NULL) {
		UC_LOG(ERROR) << "_StopVOIP _conferenceSink is null";
		return -1;
	}
    IGNetTangAudioSession *pAudio = _conferenceSink->GetAudioService();
    if (NULL == pAudio) {
        UC_LOG(ERROR) << "_StopVOIP, pAudio is NULL";
        return -1;
    }
    
    if (_conferenceSink->RetrieveSelfAudioType() == VOIP) {
        IGNetTangAudioSession *pAudio = _conferenceSink->GetAudioService();
		if (NULL == pAudio) {
			UC_LOG(ERROR) << "GetAudioService, pAudio is NULL";
			return -1;
		}
        return pAudio->stopVoip();
    }
    
    return S_OK;
}

int ConferenceWrapper::_StartVOIP() {
	UC_LOG(INFO) << "_StartVOIP";

	if (_conferenceSink == NULL) {
		UC_LOG(ERROR) << "_StartVOIP _conferenceSink is null";
		return -1;
	}
    IGNetTangAudioSession *pAudio = _conferenceSink->GetAudioService();
    if (NULL == pAudio) {
        UC_LOG(ERROR) << "startVOIP, pAudio is NULL";
		return -1;
    } 
	
	int ret = pAudio->startVoip();
	UC_LOG(INFO) << "startVoip return:" << ret;
	return ret;
}

int ConferenceWrapper::_CallPhone(const std::string &phoneNumber, unsigned int userID) {
	UC_LOG(INFO) << "_CallPhone, phoneNumber:" << phoneNumber << ", userID:" << userID;

	if (_conferenceSink == NULL) {
		UC_LOG(ERROR) << "_CallPhone _conferenceSink is null";
		return -1;
	}
    IGNetTangAudioSession *pAudio = _conferenceSink->GetAudioService();
    if (NULL == pAudio) {
        UC_LOG(ERROR) << "_CallPhone, pAudio is NULL";
        return -1;
    }
    
	PhoneCallNum phoneCallNum;
	phoneCallNum.sPhoneNum = (char *)phoneNumber.c_str();
	phoneCallNum.nUserID = userID;
    int nRet = pAudio->call(&phoneCallNum, 1);
    return nRet;
}

unsigned int ConferenceWrapper::_GetSelfUserID() {
	UC_LOG(INFO) << "_GetSelfUserID";

	if (_conference == NULL) {
		UC_LOG(ERROR) << "_GetSelfUserID _conference is null";
		return -1;
	}
    IGNetTangUser *user = _conference->getMyself();
	if (NULL == user) {
		UC_LOG(ERROR) << "_GetSelfUserID, user is NULL";
		return 0;
	}
	
    return user->getUserID();
}

int ConferenceWrapper::_HangupPhone(unsigned int userID) {
	UC_LOG(INFO) << "_HangupPhone, userID:" << userID;

	if (_conferenceSink == NULL) {
		UC_LOG(ERROR) << "_HangupPhone _conferenceSink is null";
		return -1;
	}

    IGNetTangAudioSession *pAudio = _conferenceSink->GetAudioService();
    if (NULL == pAudio) {
        UC_LOG(ERROR) << "callPhone, pAudio is NULL";
        return -1;
    }
    
	//if (_conferenceSink->RetrieveSelfAudioType() == PHONE) {
       return pAudio->hangUp(&userID, 1);
    //}

	//return S_OK;
}

void ConferenceWrapper::_ReleaseConference() {
    if (NULL != _conferenceSink) {
        delete _conferenceSink;
        _conferenceSink = NULL;
    }

	if (NULL != _conference) {
		UC_LOG(INFO) << "delete Conference";
		//_deleteConference(_conference);
		_tangService->deleteGNetTangConference(_conference);
		_conference = NULL;
	}
}

void ConferenceWrapper::GetCallInfo(CallInfo& info) {
	if (NULL != _conferenceSink) {
		info.callType = _conferenceSink->GetCallType();
		info.JoinType = _conferenceSink->GetJoinType();
		info.conferenceId = _conferenceSink->GetConferenceId();
		info.callId = _conferenceSink->GetCallId();
		info.tangUserId = _conferenceSink->GetTangUserId();
	}
}

void ConferenceWrapper::SetAudioEvent(IConferenceEvent* event) {
	_conferenceEvent = event;
}

void ConferenceWrapper::SetVedioEvent(IVideoEvent* event) {
	_videoEvent = event;
}

void ConferenceWrapper::SetDesktopEvent(ISharingServiceEvent* event) {
	_desktopEvent = event;
}

unsigned int ConferenceWrapper::GetParticipantNum() {
	if (_conferenceSink == NULL) {
		UC_LOG(ERROR) << "GetParticipantNum _conferenceSink is null";
		return -1;
	}

	return _conferenceSink->GetParticipantNum();
}

void ConferenceWrapper::ChangeGroupCallType(int type) {
	if (_conferenceSink != NULL) {
		CallType callType = _conferenceSink->GetCallType();
		UC_LOG(INFO) << "ChangeGroupCallType, type:" << type << ", callType:" << callType;
		if (callType == SINGLE_CALL) {
			_conferenceSink->SetCallType((CallType)type);
		}
	}
}

void ConferenceWrapper::SetLogPath(const std::string& path) {
	if (_logPath != NULL) {
		::SysFreeString(_logPath);
		_logPath = NULL;
	}
	_logPath = ::SysAllocString(StringConvert::FromUTF8Str(path).c_str());
}

int ConferenceWrapper::GetMicrophoneNum() {
	int num = 0;
	int ret = _aVWizard->GetMicrophoneNum(num);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetMicrophoneNum return error:" << ret;
	}
	UC_LOG(INFO) << "GetMicrophoneNum, num:" << num;
	return num;
}

int ConferenceWrapper::GetSpeakerNum() {
	int num = 0;
	int ret = _aVWizard->GetSpeakerNum(num);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetSpeakerNum return error:" << ret;
	}
	UC_LOG(INFO) << "GetSpeakerNum, num:" << num;
	return num;
}

int ConferenceWrapper::GetSpeakerList(std::vector<string>& speakers) {
	int num = GetSpeakerNum();
	speakers.reserve(num);
	char name[MAX_PATH] = { 0 };
	for (int i = 0; i < num; i++) {
		memset(name, 0, MAX_PATH);
		_aVWizard->GetSpeakerName(i, name);
		speakers.push_back(name);
	}
	return 0;
}

int ConferenceWrapper::GetMicrophoneList(std::vector<string>& microphoneList) {
	int num = GetMicrophoneNum();
	microphoneList.reserve(num);
	char name[MAX_PATH] = { 0 };
	for (int i = 0; i < num; i++) {
		memset(name, 0, MAX_PATH);
		_aVWizard->GetMicrophoneName(i, name);
		microphoneList.push_back(name);
	}
	return 0;
}

int ConferenceWrapper::SpeakerDiagnosis(int index) {
	std::string avPath = GetAVFilePath();
	int ret = _aVWizard->SetSpeaker(index);
	if (ret != 0) {
		UC_LOG(ERROR) << "SetSpeaker, index:" << index << ", return error:" << ret;
		return -1;
	}
	return _aVWizard->StartPlayFile((char*)avPath.c_str());
}

int ConferenceWrapper::MikeDiagnosis(int index) {
	int ret = _aVWizard->SetMicrophone(index);
	if (ret != 0) {
		UC_LOG(ERROR) << "SetMicrophone, index:" << index << ", return error:" << ret;
		return -1;
	}
	return _aVWizard->StartLoopAudio();
}

int ConferenceWrapper::GetSpeakerLevel(int& level) {
	return _aVWizard->GetSpeakerLevel(level);
}

int ConferenceWrapper::GetMicrophoneLevel(int& level) {
	return _aVWizard->GetMicrophoneLevel(level);
}

int ConferenceWrapper::IsPlayFile(bool& isPlay) {
	return _aVWizard->IsPlayFile(isPlay);
}

std::string ConferenceWrapper::GetAVFilePath() {
	char appPath[MAX_PATH] = { 0 };
	::GetModuleFileNameA(NULL, appPath, MAX_PATH);

	std::string currentDir = appPath;
	int lastOf = currentDir.rfind('\\');
	std::string avFilePath = currentDir.substr(0, lastOf);
	avFilePath.append("\\Conference\\test.wav");
	return avFilePath;
}

int ConferenceWrapper::StopPlayFile() {
	return _aVWizard->StopPlayFile();
}

int ConferenceWrapper::StopLoopAudio() {
	return _aVWizard->StopLoopAudio();
}

int32_t ConferenceWrapper::GetCameraNum() {
	int num = 0;
	_aVWizard->GetCameraNum(num);
	return num;
}

bool ConferenceWrapper::IsCameraAvailable() {
	bool isAvailable = false;
	_aVWizard->IsCameraAvailable(isAvailable);
	return isAvailable;
}

int32_t ConferenceWrapper::CameraDiagnosis(int cameraIndex, void *pWindow) {
	return _aVWizard->StartPreview(cameraIndex, pWindow, 0);
}

int32_t ConferenceWrapper::StopCameraDiagnosis() {
	return _aVWizard->StopPreview();
}

int ConferenceWrapper::GetCameraList(std::vector<string>& cameras) {
	int num = GetCameraNum();
	cameras.reserve(num);
	char name[MAX_PATH] = { 0 };
	for (int i = 0; i < num; i++) {
		memset(name, 0, MAX_PATH);
		_aVWizard->GetCameraName(i, name);
		cameras.push_back(name);
	}
	return 0;
}

//////////////////////////////////video///////////////////////////////////
int ConferenceWrapper::StartLocalView(TangVideoViewHandle pWindow, unsigned int nWidth, unsigned int nHeight) {
	//start preview
	int cameraIndex = 0;
	int ret = StartPreview(pWindow, cameraIndex);
	if (ret != 0) {
		UC_LOG(ERROR) << "StartPreview return error:" << ret 
			<< ", nWidth:" << nWidth << ", nHeight:" << nHeight;
		return ret;
	}

	//start share
	ret = StartViewShare(0, nWidth, nHeight);
	if (ret != 0) {
		UC_LOG(ERROR) << "StartViewShare return error:" << ret 
			<< ", nWidth:" << nWidth << ", nHeight:" << nHeight;
	}
	return ret;
}

int ConferenceWrapper::StopLocalView() {
	int ret = StopViewShare();
	if (ret != 0) {
		UC_LOG(ERROR) << "StopViewShare return error:" << ret;
		return ret;
	}

	ret = StopPreview();
	if (ret != 0) {
		UC_LOG(ERROR) << "StopPreview return error:" << ret;
	}
	return ret;
}

int ConferenceWrapper::StartRemoteView(unsigned int userId, TangVideoViewHandle pWindow) {
	int ret = StartView(userId, pWindow);
	if (ret != 0) {
		UC_LOG(ERROR) << "StartView return error:" << ret << ", userId:" << userId;
	}
	return ret;
}

int ConferenceWrapper::StopRemoteView(unsigned int userId) {
	int ret = StopView(userId);
	if (ret != 0) {
		UC_LOG(ERROR) << "StopView return error:" << ret << ", userId:" << userId;
	}
	return ret;
}

int ConferenceWrapper::GetCameraIndex() {
	IGNetTangVideoSession *pVideo = _conferenceSink->GetVideoService();
	if (NULL == pVideo) {
		UC_LOG(ERROR) << "GetCameraIndex, pVideo is NULL";
		return -1;
	}

	int inedx = 0;
	char deviceName[256] = { 0 };
	int cameraCnt = pVideo->getCameraCount();
	for (int i = 0; i < cameraCnt; i++) {
		if (!pVideo->isDeviceOccupied(i)) {
			memset(deviceName, 0, 256);
			if (pVideo->getCameraName(i, deviceName) == 0) {
				inedx = i;
				break;
			}
		}
	}
	return inedx;
}

int ConferenceWrapper::StartPreview(TangVideoViewHandle pWindow, int& cameraIndex) {
	UC_LOG(INFO) << "StartPreview";
	IGNetTangVideoSession *pVideo = _conferenceSink->GetVideoService();
	if (NULL == pVideo) {
		UC_LOG(ERROR) << "StartPreview, pVideo is NULL";
		return -1;
	}

	int index = GetCameraIndex();
	if (index < 0) {
		UC_LOG(INFO) << "GetCameraIndex, return error index:" << index;
		return -1;
	}
	return pVideo->startPreview(index, pWindow);
}

int ConferenceWrapper::StopPreview() {
	UC_LOG(INFO) << "StopPreview";
	IGNetTangVideoSession *pVideo = _conferenceSink->GetVideoService();
	if (NULL == pVideo) {
		UC_LOG(ERROR) << "StopPreview, pVideo is NULL";
		return -1;
	}
	return pVideo->stopPreview();
}

int ConferenceWrapper::StartView(unsigned int nUserID, TangVideoViewHandle pWindow) {
	UC_LOG(INFO) << "StartView, nUserID:" << nUserID;
	if (nUserID == GetSelfUserID()) {
		UC_LOG(ERROR) << "Can not start view by user self!";
		return -1;
	}
	IGNetTangVideoSession *pVideo = _conferenceSink->GetVideoService();
	if (NULL == pVideo) {
		UC_LOG(ERROR) << "StartView, pVideo is NULL, nUserID:" << nUserID;
		return -1;
	}
	return pVideo->startView(nUserID, pWindow);
}

int ConferenceWrapper::StopView(unsigned int nUserID) {
	UC_LOG(INFO) << "StopView, nUserID:" << nUserID;
	IGNetTangVideoSession *pVideo = _conferenceSink->GetVideoService();
	if (NULL == pVideo) {
		UC_LOG(ERROR) << "StopView, pVideo is NULL, nUserID:" << nUserID;
		return -1;
	}
	return pVideo->stopView(nUserID);
}

int ConferenceWrapper::StartViewShare(int nDevicIndex, unsigned int nWidth, unsigned int nHeight) {
	unsigned int nUserID = _GetSelfUserID();
	UC_LOG(INFO) << "StartViewShare, nUserID:" << nUserID
		<< ", nDevicIndex:" << nDevicIndex << ", nWidth:" << nWidth << ", nHeight:" << nHeight;
	IGNetTangVideoSession *pVideo = _conferenceSink->GetVideoService();
	if (NULL == pVideo) {
		UC_LOG(ERROR) << "StartShare, pVideo is NULL, nUserID:" << nUserID 
			<< ", nDevicIndex:" << nDevicIndex << ", nWidth:" << nWidth
			<< ", nHeight:" << nHeight;
		return -1;
	}
	return pVideo->startShare(nUserID, nDevicIndex, nWidth, nHeight);
}

int ConferenceWrapper::StopViewShare() {
	unsigned int nUserID = _GetSelfUserID();
	UC_LOG(INFO) << "StopViewShare, nUserID:" << nUserID;
	IGNetTangVideoSession *pVideo = _conferenceSink->GetVideoService();
	if (NULL == pVideo) {
		UC_LOG(ERROR) << "StopShare, pVideo is NULL, nUserID:" << nUserID;
		return -1;
	}
	return pVideo->stopShare(nUserID);
}

int ConferenceWrapper::RequestResolution(unsigned int nUserID, unsigned int nWidth, unsigned int nHeight) {
	UC_LOG(INFO) << "RequestResolution, nUserID:" << nUserID << ", nWidth:" << nWidth << ", nHeight:" << nHeight;
	IGNetTangVideoSession *pVideo = _conferenceSink->GetVideoService();
	if (NULL == pVideo) {
		UC_LOG(ERROR) << "RequestResolution, pVideo is NULL, nUserID:" 
			<< nUserID << ", nWidth:" << nWidth << ", nHeight" << nHeight;
		return -1;
	}
	return pVideo->requestResolution(nUserID, nWidth, nHeight);
}

int ConferenceWrapper::ChangeShareCamera(unsigned int nUserID, int nDevicIndex) {
	UC_LOG(INFO) << "ChangeShareCamera, nUserID:" << nUserID << ", nDevicIndex:" << nDevicIndex;
	IGNetTangVideoSession *pVideo = _conferenceSink->GetVideoService();
	if (NULL == pVideo) {
		UC_LOG(ERROR) << "ChangeShareCamera, pVideo is NULL, nUserID:" << nUserID << ", nDevicIndex:" << nDevicIndex;
		return -1;
	}
	return pVideo->changeShareCamera(nUserID, nDevicIndex);
}

//////////////////////////////////desktop/////////////////////////////////////
int ConferenceWrapper::StartDesktopShare() {
	UC_LOG(INFO) << "StartDesktopShare";
	IGNetTangDesktopSession *pDesktop = _conferenceSink->GetDesktopService();
	if (NULL == pDesktop) {
		UC_LOG(ERROR) << "StartDesktopShare, pDesktop is NULL";
		return -1;
	}
	return pDesktop->startShare();
}

int ConferenceWrapper::StopDesktopShare() {
	UC_LOG(INFO) << "StopDesktopShare";
	IGNetTangDesktopSession *pDesktop = _conferenceSink->GetDesktopService();
	if (NULL == pDesktop) {
		UC_LOG(ERROR) << "StopDesktopShare, pDesktop is NULL";
		return -1;
	}
	return pDesktop->stopShare();
}

int ConferenceWrapper::StartDesktopView(VIEW view) {
	UC_LOG(INFO) << "StartDesktopView";
	IGNetTangDesktopSession *pDesktop = _conferenceSink->GetDesktopService();
	if (NULL == pDesktop) {
		UC_LOG(ERROR) << "StartDesktopView, pDesktop is NULL";
		return -1;
	}
	return pDesktop->startView(view);
}

int ConferenceWrapper::StopDesktopView() {
	UC_LOG(INFO) << "StopDesktopView";
	IGNetTangDesktopSession *pDesktop = _conferenceSink->GetDesktopService();
	if (NULL == pDesktop) {
		UC_LOG(ERROR) << "StopDesktopView, pDesktop is NULL";
		return -1;
	}
	return pDesktop->stopView();
}

}
