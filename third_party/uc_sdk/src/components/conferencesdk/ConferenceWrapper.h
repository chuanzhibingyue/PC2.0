//
//  ConferenceWrapper.h
//  BeeCoference
//
//  Created by Hualong Zhang on 15/5/20.
//  Copyright (c) 2015年 Hualong Zhang. All rights reserved.
//

#ifndef CONFERENCE_CONFERENCEWRAPER_H_
#define CONFERENCE_CONFERENCEWRAPER_H_

#include <stdio.h>
#include "IConference.h"
#include "ISharingService.h"
#include "IVideoService.h"
#include "IConferenceEvent.h"
#include "audio_service/CallDef.h"

class IGNetTangConference;
class ConferenceSink;
class IGNetTangService;
class IGNetTangAVWizard;

#include "ConferenceDef.h"

namespace uc {

	class ConferenceWrapper : public IConference, ISharingService, IVideoService {
	public:
		static ConferenceWrapper& GetConferenceWrapper();
		~ConferenceWrapper();

		//conference
		virtual int JoinConference(const std::string &joinKey, CallType callType, JoinType JoinType, int conferenceId, UCID uid, int tangUserId);
		virtual int LeaveConference();
		virtual int EndConference();
		virtual int ReconnectConference();

		//audio service
		virtual int SwitchToPhone(const std::string &phoneNumber);
		virtual int SwitchToVOIP();
		virtual int MuteSelf();
		virtual int UnMuteSelf();
		virtual int HangupPhone();
		virtual int EnableLoudSpeaker();
		virtual int DisableLoudSpeaker();
		void GetCallInfo(CallInfo& info);

		virtual int GetMicrophoneNum();
		virtual int GetSpeakerNum();
		virtual int GetSpeakerList(std::vector<string>& speakers);
		virtual int GetMicrophoneList(std::vector<string>& microphoneList);
		virtual int SpeakerDiagnosis(int index);
		virtual int MikeDiagnosis(int index);
		virtual int GetSpeakerLevel(int& level);
		virtual int GetMicrophoneLevel(int& level);
		virtual int IsPlayFile(bool& isPlay);
		virtual int StopPlayFile();
		virtual int StopLoopAudio();

		virtual int32_t GetCameraNum();
		virtual int32_t GetCameraList(std::vector<string>& cameras);
		virtual bool IsCameraAvailable();
		virtual int32_t CameraDiagnosis(int cameraIndex, void *pWindow);
		virtual int32_t StopCameraDiagnosis();

		//video service
		virtual int StartLocalView(TangVideoViewHandle pWindow, unsigned int nWidth, unsigned int nHeight);
		virtual int StopLocalView();

		virtual int StartRemoteView(unsigned int userId, TangVideoViewHandle pWindow);
		virtual int StopRemoteView(unsigned int userId);

		virtual int RequestResolution(unsigned int nUserID, unsigned int nWidth, unsigned int nHeight);
		virtual int ChangeShareCamera(unsigned int nUserID, int nDevicIndex);

		//desktop
		virtual int StartDesktopShare();
		virtual int StopDesktopShare();
		virtual int StartDesktopView(VIEW view);
		virtual int StopDesktopView();
		
		//set event
		void SetAudioEvent(IConferenceEvent* event);
		void SetVedioEvent(IVideoEvent* event);
		void SetDesktopEvent(ISharingServiceEvent* event);

		unsigned int GetParticipantNum();
		unsigned int GetSelfUserID() { return _GetSelfUserID(); }
		
		void ChangeGroupCallType(int type);

		static void SetLogPath(const std::string& path);
	private:
		ConferenceWrapper();
		ConferenceWrapper(const ConferenceWrapper&);

		//conference
		void _ReleaseConference();
		//CREATE_CONFERENCE_FUN _createConference;
		//DELETE_CONFERENCE_FUN _deleteConference;
		CREATE_TANG_SERVICE_FUN _getTangService;
		IGNetTangConference *_conference;
		IGNetTangService * _tangService;
		IGNetTangAVWizard * _aVWizard;

		//audio
		int _StartVOIP();
		int _StopVOIP();
		int _CallPhone(const std::string &phoneNumber, unsigned int userID);
		unsigned int _GetSelfUserID();
		int _HangupPhone(unsigned int userID);

		std::string GetAVFilePath();

		//video
		int GetCameraIndex();
		int StartPreview(TangVideoViewHandle pWindow, int& cameraIndex);
		int StopPreview();
		int StartView(unsigned int nUserID, TangVideoViewHandle pWindow);
		int StopView(unsigned int nUserID);
		int StartViewShare(int nDevicIndex, unsigned int nWidth, unsigned int nHeight);
		int StopViewShare();

		//conference sink
		ConferenceSink *_conferenceSink;

		//event
		IConferenceEvent *_conferenceEvent;
		IVideoEvent *_videoEvent;
		ISharingServiceEvent *_desktopEvent;

		static BSTR _logPath;
	};

}
#endif //CONFERENCE_CONFERENCEWRAPER_H_
