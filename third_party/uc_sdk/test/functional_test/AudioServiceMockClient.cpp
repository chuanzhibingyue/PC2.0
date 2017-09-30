#include "IAudioService.h"
#include "LogManager.h"
#include "StringConvert.h"
#include "AudioServiceMockClient.h"
#include "IUCEngine.h"
#include "UCErrorCode.h"
//#include "gtest/gtest.h"
//#include "UCCommonFunctionMock.h"

using namespace uc;

AudioServiceMockClient::AudioServiceMockClient(){
	_audioService = NULL;
	_tempConferenceId = 0;
	_tempUserId = 0;
	_reserveCode = "";
	_token = "";
}

void AudioServiceMockClient::Init(IUCEngine *ucEngine) {
	_audioService = (IAudioService*)ucEngine->QueryService(UCAudioService);
	_audioService->registerListener(this);
}	
	
void AudioServiceMockClient::UnInit() {
	_audioService->removeListener(this);
}

int32_t AudioServiceMockClient::CreateCall(int64_t groupId) {
	return _audioService->CreateCall(groupId);
}

int32_t AudioServiceMockClient::JoinCall(int tempConferenceId, const std::string &reserveCode, int64_t groupId) {
	return _audioService->JoinCall(tempConferenceId, reserveCode, groupId);
}

int32_t AudioServiceMockClient::LeaveCall(int tempConferenceId, int tempUserId) {
	return _audioService->LeaveCall(tempConferenceId, tempUserId);
}

void AudioServiceMockClient::OnCallCreated(int32_t errorCode, int tempConferenceId, int tempUserId, 
	const std::string &reserveCode, const std::string &token) {
	NotifyResult(errorCode);
	UC_LOG(INFO) << "OnCallCreated" << ", errorCode: " << errorCode << ", tempConferenceId: " << tempConferenceId << ", tempUserId: "
		<< tempUserId << ", reserveCode: " << reserveCode << ", token: " << token;
	_tempConferenceId = tempConferenceId;
	_tempUserId = tempUserId;
	_reserveCode = reserveCode;
	_token = token;
}

void AudioServiceMockClient::OnCallJoined(int32_t errorCode, int tempConferenceId, 
	int tempUserId, const std::string &token) {
	NotifyResult(errorCode);
	UC_LOG(INFO) << "OnCallJoined" << ", errorCode: " << errorCode << ", tempConferenceId: " << tempConferenceId << ", tempUserId: "
		<< tempUserId << ", token: " << token;
	_tempConferenceId = tempConferenceId;
	_tempUserId = tempUserId;
	_token = token;
}

void AudioServiceMockClient::OnCallLeaved(int32_t errorCode, int tempConferenceId) {
	NotifyResult(errorCode);
	UC_LOG(INFO) << "OnCallLeaved" << ", errorCode: " << errorCode << ", tempConferenceId: " << tempConferenceId;
}

int32_t AudioServiceMockClient::JoinConference(const std::string &joinKey) {
	return _audioService->JoinConference(joinKey);
}

void AudioServiceMockClient::OnConferenceJoined(int statusCode) {
	NotifyResult(statusCode);
	UC_LOG(INFO) << "OnConferenceJoined, statusCode: " << statusCode;
}

void AudioServiceMockClient::OnAudioStarted(int statusCode) {
	NotifyResult(statusCode);
	UC_LOG(INFO) << "OnAudioStarted, statusCode: " << statusCode;
}

void AudioServiceMockClient::OnConferenceLeft() {
	NotifyResult(UC_SUCCEEDED);
	UC_LOG(INFO) << "OnConferenceLeft";
}

void AudioServiceMockClient::OnUserAdded(const ITangUser *tangUser) {
	NotifyResult(UC_SUCCEEDED);
	UC_LOG(INFO) << "OnUserAdded: " << tangUser->ReturnDescription();
}

void AudioServiceMockClient::OnUserRemoved(const ITangUser *tangUser) {
	NotifyResult(UC_SUCCEEDED);
	UC_LOG(INFO) << "OnUserRemoved: " << tangUser->ReturnDescription();
}

void AudioServiceMockClient::OnUserPropertyChanged(const ITangUser *tangUser, int properType) {
	NotifyResult(UC_SUCCEEDED);
	UC_LOG(INFO) << "OnUserPropertyChanged: " << tangUser->ReturnDescription() << ", properType: " << properType;
}

void AudioServiceMockClient::OnConferenceEnded() {
	NotifyResult(UC_SUCCEEDED);
	UC_LOG(INFO) << "OnConferenceEnded";
}

void AudioServiceMockClient::OnConferenceDisconnected() {
	NotifyResult(UC_SUCCEEDED);
	UC_LOG(INFO) << "OnConferenceDisconnected";
}

void AudioServiceMockClient::OnConferenceReconnected(int statusCode) {
	NotifyResult(UC_SUCCEEDED);
	UC_LOG(INFO) << "onConferenceReconnected, statusCode: " << statusCode;
}

void AudioServiceMockClient::OnCallPhoneFailed(const std::string &phoneNumber, const std::string &errorCode) {
	NotifyResult(UC_SUCCEEDED);
	UC_LOG(INFO) << "OnCallPhoneFailed, phoneNumber: " << phoneNumber << ", errorCode: " << errorCode;
}

void AudioServiceMockClient::OnLoudSpeakerStatusChanged(bool isLoud) {
	NotifyResult(UC_SUCCEEDED);
	UC_LOG(INFO) << "OnLoudSpeakerStatusChanged, isLoud: " << isLoud;
}