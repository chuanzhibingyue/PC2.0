//
//  ConferenceSink.h
//  Tang
//
//  Created by Hualong Zhang on 15/5/14.
//  Copyright (c) 2015年 Hualong Zhang. All rights reserved.
//

#ifndef __Tang__GNetTangConfMgr__
#define __Tang__GNetTangConfMgr__

#include <stdio.h>
#include "IGNetTangConfMgr.h"
#include "IConferenceEvent.h"
#include "IGNetTangAudioSession.h"
#include "ignettangvideosession.h"
#include "IGNetTangDesktopSession.h"
#include "AudioServiceSink.h"
#include "VideoServiceSink.h"
#include "SharingServiceSink.h"
#include "TangUser.h"
#include "audio_service/CallDef.h"

using namespace uc;

class ConferenceSink : public IGNetTangConferenceSink {
public:
	ConferenceSink(CallType callType, JoinType joinType, int conferenceId, UCID uid, int tangUserId);
    virtual ~ConferenceSink();
    virtual void onConfJoined(int statusCode);
    virtual void onConfLeft(int statusCode);
	virtual void onConfEnded(int statusCode);
    virtual void onSessionCreated(GNetTangSessionType serviceType,  IGNetTangBaseSession *pTangService, int statusCode);
    virtual void onConfPropertyChanged(const char *propName,
                                      const CGNetTangVariant &oldPropValue,
                                      const CGNetTangVariant &newPropValue);
    
    virtual void onUserAdded(const IGNetTangUser* pUser);
    virtual void onUserRemoved(const IGNetTangUser* pUser);
    virtual void onUserPropertyChanged(const char*propName,
                                      const CGNetTangVariant &oldPropValue,
                                      const CGNetTangVariant &newPropValue,
                                      const IGNetTangUser* pUser);
    
    virtual void onConfReconnected(int statusCode);

    virtual void onSessionClosed(GNetTangSessionType sessionType,int sessionID) {
        //todo:
    }
    
    void onSessionErrorHandle(CGNetTangSessionErrorInfo *pErrorInfo);

	void SetConfDelegate(IConferenceEvent *delegate) { 
		m_pConfDelegate = delegate;
		m_pAudioSink->SetDelegate(delegate);
	}

	void SetVideoDelegate(IVideoEvent *delegate) {
		m_pVideoDelegate = delegate; 
		m_pVideoSink->SetDelegate(delegate);
	}

	void SetDesktopDelegate(ISharingServiceEvent *delegate) { 
		m_pDesktopDelegate = delegate; 
		m_pDesktopSink->SetDelegate(delegate);
	}
    
    void SetConference(IGNetTangConference *conference) { _conference = conference; }
    
	IGNetTangAudioSession *GetAudioService();

	IGNetTangVideoSession *GetVideoService();

	IGNetTangDesktopSession *GetDesktopService();
    
    AudioType RetrieveSelfAudioType();
    
    unsigned int GetParticipantNum() { return m_nParticipantNum;}

	CallType GetCallType() { return m_callType; }
	void SetCallType(CallType type) { m_callType = type; }

	JoinType GetJoinType() { return m_joinType; }
	void SetJoinType(JoinType type) { m_joinType = type; }

	int GetConferenceId() { return m_conferenceId; }
	UCID GetCallUCID() { return m_callId; }
	int GetCallId() { return m_callId.GetUserId(); }
	int GetTangUserId() { return m_tangUserId; }
    
private:
    void _RetrieveTangUserFromCGNetTangUser(const IGNetTangUser* pUser, TangUser &tangUser);
    void _RetrieveAudioType(unsigned int audioStatus, AudioType &audioType, bool &isMute);
    int _RetrievePropertyTypeFromAudioStatus(unsigned int oldAudioStatus, unsigned int newAudioStatus);

	//event agent
    IConferenceEvent *m_pConfDelegate;
	IVideoEvent *m_pVideoDelegate;
	ISharingServiceEvent *m_pDesktopDelegate;

	//conference sdk callback
    IGNetTangConference *_conference;
	AudioServiceSink* m_pAudioSink;
	VideoServiceSink* m_pVideoSink;
	SharingServiceSink* m_pDesktopSink;

	//conference info
    unsigned int m_nParticipantNum;
	CallType m_callType;
	JoinType m_joinType;
	int m_conferenceId;
	UCID m_callId;
	int m_tangUserId;

	//service id
	int m_audioService;
	int m_desktopService;

	int m_audioSessionID;
	int m_desktopSessionID;
	int m_videoSessionID;
};

#endif /* defined(__Tang__GNetTangConfMgr__) */
