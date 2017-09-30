#ifndef AUDIO_SERVICE_MOCK_CLIENT_H_
#define AUDIO_SERVICE_MOCK_CLIENT_H_

#include "IAudioService.h"
#include "UCMockBase.h"
#include <string>

using namespace uc;
using namespace std;

class AudioServiceMockClient : public IAudioServiceEvent, public UCMockBase {
public:
	AudioServiceMockClient();

	//UCMockClient::~UCMockClient(){}
	~AudioServiceMockClient(){}

	void Init(IUCEngine *ucEngine); 
	void UnInit();

	int32_t CreateCall(int64_t groupId = 0);
	int32_t JoinCall(int tempConferenceId, const std::string &reserveCode, int64_t groupId = 0);
	int32_t LeaveCall(int tempConferenceId, int tempUserId);
	int32_t JoinConference(const std::string &joinKey);

	int _tempConferenceId;
	int _tempUserId;
	std::string _reserveCode;
	std::string _token;

protected:
		virtual void OnCallCreated(int32_t errorCode, int tempConferenceId, int tempUserId, 
			const std::string &reserveCode, const std::string &token);
		virtual void OnCallJoined(int32_t errorCode, int tempConferenceId, 
			int tempUserId, const std::string &token);
		virtual void OnCallLeaved(int32_t errorCode, int tempConferenceId);

		virtual void OnConferenceJoined(int statusCode);
		virtual void OnAudioStarted(int statusCode);
		virtual void OnConferenceLeft();
		virtual void OnUserAdded(const ITangUser *tangUser);
		virtual void OnUserRemoved(const ITangUser *tangUser);
		virtual void OnUserPropertyChanged(const ITangUser *tangUser, int properType);
		virtual void OnConferenceEnded();
		virtual void OnConferenceDisconnected();
		virtual void OnConferenceReconnected(int statusCode);
		virtual void OnCallPhoneFailed(const std::string &phoneNumber, const std::string &errorCode);
		virtual void OnLoudSpeakerStatusChanged(bool isLoud);
private:
	IAudioService *_audioService;
};

#endif //AUDIO_SERVICE_MOCK_CLIENT_H_