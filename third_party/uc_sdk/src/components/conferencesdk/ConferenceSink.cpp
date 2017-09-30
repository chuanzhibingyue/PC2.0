//
//  ConferenceSink.cpp
//  Tang
//
//  Created by Hualong Zhang on 15/5/14.
//  Copyright (c) 2015年 Hualong Zhang. All rights reserved.
//

#include "ConferenceWrapper.h"
#include "ConferenceSink.h"
#include "CGNetTangCommonDef.h"
#include "ConferenceDef.h"
#include "LogManager.h"
#include "TangUser.h"

using namespace uc;


ConferenceSink::ConferenceSink(CallType callType, JoinType joinType, 
	int conferenceId, UCID uid, int tangUserId)
:_conference(NULL)
,m_nParticipantNum(0)
,m_callType(callType)
,m_joinType(joinType)
,m_conferenceId(conferenceId)
, m_callId(uid)
,m_tangUserId(tangUserId)
, m_audioService(0)
, m_desktopService(0)
,m_audioSessionID(0)
,m_desktopSessionID(0)
,m_videoSessionID(0) {
	m_pAudioSink = new AudioServiceSink(conferenceId);
	m_pVideoSink = new VideoServiceSink(conferenceId);
	m_pDesktopSink = new SharingServiceSink(conferenceId);
}

ConferenceSink::~ConferenceSink() {
	if (m_pAudioSink != NULL) {
		delete m_pAudioSink;
		m_pAudioSink = NULL;
	}

	if (m_pVideoSink != NULL) {
		delete m_pVideoSink;
		m_pVideoSink = NULL;
	}

	if (m_pDesktopSink != NULL) {
		delete m_pDesktopSink;
		m_pDesktopSink = NULL;
	}
}

void ConferenceSink::onConfJoined(int statusCode) {
	UC_LOG(INFO) << "onConfJoined, statusCode: " << statusCode;
	//重连之后加入会议成功
	if (statusCode == 1) {
		statusCode = 0;
		UC_LOG(INFO) << "Reconnect join conf!";
	}

	if (statusCode == 0) {
		m_audioService = _conference->createSession(TMC_SESSIONTYPE_AUDIO); //Audio
		UC_LOG(INFO) << "onConfJoined createSession audio: " << m_audioService;
		m_desktopService = _conference->createSession(TMC_SESSIONTYPE_DESKTOP); //Desktop Share
		UC_LOG(INFO) << "onConfJoined createSession share: " << m_desktopService;
	}

	if (NULL == m_pConfDelegate) return;
	m_pConfDelegate->OnConferenceJoined(statusCode, GetCallType(), GetJoinType(), GetConferenceId(), GetCallId());
}

void ConferenceSink::onConfLeft(int statusCode) {
	UC_LOG(WARNING) << "onConfLeft, statusCode: " << statusCode << ", conference id:" << GetConferenceId() 
		<< ", ParticipantNum:" << GetParticipantNum();
	if (NULL == m_pConfDelegate) return;

	//network disconnect
	if (statusCode == LEFTCONFREASON_NETWORKDISCONNECT) {
		UC_LOG(WARNING) << "Conference disconnect!";
		m_pConfDelegate->OnConferenceDisconnected(GetConferenceId());
		return;
	}

	//close call
	m_pConfDelegate->OnCallClose(GetConferenceId(), GetCallType(), GetCallId());
	if (GetParticipantNum() <= 0) {
		UC_LOG(INFO) << "the conference will end after the user leave, conference id:" << GetConferenceId();
	}
}

void ConferenceSink::onConfEnded(int statusCode) {
	UC_LOG(WARNING) << "onConfEnded, statusCode: " << statusCode << ", conference id:" << GetConferenceId();
	if (NULL == m_pConfDelegate) return;
	m_pConfDelegate->OnConferenceEnded(GetConferenceId(), GetCallType(), statusCode);
}

void ConferenceSink::onSessionCreated(GNetTangSessionType serviceType,  IGNetTangBaseSession *pTangService, int statusCode) {
	UC_LOG(INFO) << "onSessionCreated, serviceType:" << serviceType << ", statusCode: " << statusCode << ", joinType:" << GetJoinType();
	if (statusCode != S_OK) {
		UC_LOG(ERROR) << "onSessionCreated error";
		m_pConfDelegate->OnAudioStarted(statusCode, GetCallType(), GetJoinType(), GetConferenceId(), GetCallUCID());
		return;
	}
    
	if (serviceType == TMC_SESSIONTYPE_AUDIO) {
		UC_LOG(INFO) << "onSessionCreated, Audio Service started";

		IGNetTangAudioSession *pAudio = (IGNetTangAudioSession *)pTangService;
		m_audioSessionID = pAudio->getSessionID();
		pAudio->setSessionCallback((IGNetTangAudioSessionSink *)m_pAudioSink);

#if defined(WIN32) || defined(WIN64)
		unsigned int nMicCount = 0;
		unsigned int nSpeakerCount = 0;
		pAudio->getMicCount(nMicCount);
		pAudio->getSpeakerCount(nSpeakerCount);
		UC_LOG(WARNING) << "MicCount:" << nMicCount << ", SpeakerCount:" << nSpeakerCount;
#endif
		int ret = pAudio->startVoip();
		UC_LOG(INFO) << "startVoip return:" << ret << ", audio session id:" << m_audioSessionID;
		m_pConfDelegate->OnAudioStarted(ret, GetCallType(), GetJoinType(), GetConferenceId(), GetCallUCID());
	}
	else if (serviceType == TMC_SESSIONTYPE_VIDEO) {
		IGNetTangVideoSession* pVideo = (IGNetTangVideoSession*)pTangService;
		m_videoSessionID = pVideo->getSessionID();
		pVideo->setSessionCallback((IGNetTangVideoSessionSink*)m_pVideoSink);
		UC_LOG(INFO) << "Video session created, video session id:" << m_videoSessionID;
	}
	else if (serviceType == TMC_SESSIONTYPE_DESKTOP) {
		IGNetTangDesktopSession* pDesktop = (IGNetTangDesktopSession*)pTangService;
		m_desktopSessionID = pDesktop->getSessionID();
		pDesktop->setSessionCallback((IGNetTangDesktopSessionSink*)m_pDesktopSink);
		m_pDesktopSink->SetConference(_conference);
		m_pDesktopSink->setSession(pDesktop);
		UC_LOG(INFO) << "Desktop session created, desktop session id:" << m_desktopSessionID;
	}
	else {
		UC_LOG(INFO) << "onSessionCreated serviceType:" << serviceType;
	}
}

void ConferenceSink::onConfPropertyChanged(const char *propName,
    const CGNetTangVariant &oldPropValue,
    const CGNetTangVariant &newPropValue) {
	UC_LOG(INFO) << "onConfPropertyChanged: propName: " << propName 
		<< ", old value:" << oldPropValue.intVal << ", new value:" << newPropValue.intVal;
	if (strcmp(propName, "tcpConStatus") == 0) {
		if (newPropValue.intVal == 0) {
			//network disconnect
			UC_LOG(WARNING) << "Conference disconnect!";
			m_pConfDelegate->OnConferenceDisconnected(GetConferenceId());
		}
	}
}

void ConferenceSink::onUserAdded(const IGNetTangUser* pUser) {
    UC_LOG(INFO) << "onUserAdded, userName: " << pUser->getUserName() << ", phoneNumber: " << pUser->getPhoneNumber();
	if (NULL == m_pConfDelegate) return;
    
	TangUser tangUser;
    _RetrieveTangUserFromCGNetTangUser(pUser, tangUser);
    UC_LOG(INFO) << "onUserAdded, TangUser: " << tangUser.ReturnDescription();
    m_nParticipantNum++;
	m_pConfDelegate->OnUserAdded(&tangUser, GetConferenceId(), GetCallType());
}

void ConferenceSink::onUserRemoved(const IGNetTangUser* pUser) {
    UC_LOG(INFO) << "onUserRemoved, userName: " << pUser->getUserName() << ", phoneNumber: " << pUser->getPhoneNumber();
	if (NULL == m_pConfDelegate) return;

    TangUser tangUser;
    _RetrieveTangUserFromCGNetTangUser(pUser, tangUser);
	UC_LOG(INFO) << "onUserRemoved, TangUser: " << tangUser.ReturnDescription();
    m_nParticipantNum--;
	m_pConfDelegate->OnUserRemoved(&tangUser, GetConferenceId(), GetCallType());
}

void ConferenceSink::onUserPropertyChanged(const char*propName,
                                  const CGNetTangVariant &oldPropValue,
                                  const CGNetTangVariant &newPropValue,
                                           const IGNetTangUser* pUser) {
	UC_LOG(INFO) << "onUserPropertyChanged: propName: " << propName << ", userName:" << pUser->getUserName() << ", phoneNum:" << pUser->getPhoneNumber();
	if (NULL == m_pConfDelegate) return;
    
    if (strcmp(propName, "audioStatus") == 0) {
		TangUser tangUser;
		_RetrieveTangUserFromCGNetTangUser(pUser, tangUser);
        UC_LOG(INFO) << "onUserPropertyChanged: propName: audioStatus : " << pUser->getAudioStatus()
			<< ", TangUser phoneNumber: " << tangUser.ReturnPhoneNumber();
		m_pConfDelegate->OnUserPropertyChanged(&tangUser, PROPERTY_AUDIOTYPE_MUTE, GetConferenceId(), GetCallType());
    }
	else if (strcmp(propName, "isSpeaking") == 0) {
		UC_LOG(INFO) << "onUserPropertyChanged: propName: isSpeaking : " 
			<< newPropValue.uintVal << ", userId:" << pUser->getUserName();
		m_pConfDelegate->OnSpeakingStatusChanged(GetConferenceId(), GetCallType(), std::atoi(pUser->getUserName()), newPropValue.uintVal);
	}
	else {
		//other propName
	}
}

void ConferenceSink::onConfReconnected(int statusCode) {
    UC_LOG(WARNING) << "onConfReconnected, statusCode: " << statusCode << ", conference id:" << GetConferenceId();

	SetJoinType(Join_Call);

	if (statusCode == 0) {
		if (m_audioSessionID <= 0) {
			m_audioService = _conference->createSession(TMC_SESSIONTYPE_AUDIO); //Audio
			UC_LOG(INFO) << "onConfReconnected createSession audio!";
		}

		if (m_desktopSessionID <= 0) {
			m_desktopService = _conference->createSession(TMC_SESSIONTYPE_DESKTOP); //Desktop Share
			UC_LOG(INFO) << "onConfReconnected createSession share!";
		}
	}

	if (NULL == m_pConfDelegate) return;
	m_pConfDelegate->OnConferenceReconnected(statusCode, GetConferenceId());
}

void ConferenceSink::onSessionErrorHandle(CGNetTangSessionErrorInfo *pErrorInfo) {
    UC_LOG(INFO) << "ConferenceSink onSessionErrorHandle, description: " << pErrorInfo->description << ", statusCode: " << pErrorInfo->statusCode;
}

void ConferenceSink::_RetrieveTangUserFromCGNetTangUser(const IGNetTangUser* pUser, TangUser &tangUser) {
    tangUser.tangUserID = pUser->getUserID();
	tangUser.userName = pUser->getUserName();
	tangUser.phoneNumber = pUser->getPhoneNumber();
    _RetrieveAudioType(pUser->getAudioStatus(), tangUser.audioType, tangUser.isMute);
	tangUser.SetDescription();
}

void ConferenceSink::_RetrieveAudioType(unsigned int audioStatus, AudioType &audioType, bool &isMute) {
	UC_LOG(INFO) << "_RetrieveAudioType, audioStatus:" << audioStatus << ", audioType:" << audioType;
    switch (audioStatus) {
        case 1:
        {
            audioType = VOIP;
            isMute = false;
        }
            break;
        case 2:
        case 3:
        {
            audioType = VOIP;
            isMute = true;
        }
            break;
        case 4:
        {
            audioType = PHONE;
            isMute = false;
        }
            break;
        case 5:
        case 6:
        {
            audioType = PHONE;
            isMute = true;
        }
            break;
        default:
        {
            audioType = NONE;
            isMute = true;
        }
            break;
    }
}

IGNetTangAudioSession* ConferenceSink::GetAudioService() {
	UC_LOG(INFO) << "GetAudioService, conference:" << _conference << ", audioService:" << m_audioService;
	if (_conference != NULL && m_audioService > 0) {
		IGNetTangAudioSession* session = (IGNetTangAudioSession*)_conference->getSession(m_audioService);
		UC_LOG(INFO) << "GetAudioService return " << session;
		return session;
	}

	UC_LOG(ERROR) << "get audio session return NULL!";
	return NULL;
}

IGNetTangDesktopSession* ConferenceSink::GetDesktopService() {
	UC_LOG(INFO) << "GetDesktopService, conference:" << _conference << ", desktopService:" << m_desktopService;
	if (_conference != NULL && m_desktopService > 0) {
		IGNetTangDesktopSession* session = (IGNetTangDesktopSession*)_conference->getSession(m_desktopService);
		UC_LOG(INFO) << "GetDesktopService return " << session;
		return session;
	}

	UC_LOG(ERROR) << "get desktop session return NULL!";
	return NULL;
}

IGNetTangVideoSession* ConferenceSink::GetVideoService() {
	//TODO
	UC_LOG(ERROR) << "get video session return NULL!";
	return NULL;
}

AudioType ConferenceSink::RetrieveSelfAudioType() {
    IGNetTangUser * pUser = _conference->getMyself();
	AudioType audioType;
	bool isMute;
    _RetrieveAudioType(pUser->getAudioStatus(), audioType, isMute);

	UC_LOG(INFO) << "getSelfAudioType, userID: " << pUser->getUserID() << ", audioStatus: " << audioType << ", isMute" << isMute;
	return audioType;
}

int ConferenceSink::_RetrievePropertyTypeFromAudioStatus(unsigned int oldAudioStatus, unsigned int newAudioStatus) {
    AudioType oldAudioType;
    bool oldIsMute;
    _RetrieveAudioType(oldAudioStatus, oldAudioType, oldIsMute);

    AudioType newAudioType;
    bool newIsMute;
    _RetrieveAudioType(newAudioStatus, newAudioType, newIsMute);
    
    int propertyType = 0;
    if (oldAudioType != newAudioType) {
        propertyType |= PROPERTY_AUDIOTYPE;
    }
    
    if (oldIsMute != newIsMute) {
        propertyType |= PROPERTY_MUTE;
    }
    
    return propertyType;
}
