#ifndef SYSTEMSERVICE_H_
#define SYSTEMSERVICE_H_

#include "CefClient.h"
#include "libAMRPlayer/WavePlayer.h"
#include "common/ThreadMessage.h"
#include <Windows.h>
#include <stdint.h>
#include <string>

namespace ucclient {

class SoundPlayer;

class SystemService {
public:
	SystemService(cefIntegration::CefClient *cefClient);
	~SystemService();

	std::string getClipboardData(HWND wnd);
	std::string hasValidClipboardData(HWND wnd);
	bool setClipboardData(const std::string& filePath, CefRefPtr<CefBrowser> browser);

	int playSound(const std::string& filename, const std::string& id, int64_t requestId, bool loop, CefRefPtr<CefBrowser> browser);
	int stopSound(const std::string& id);

	int32_t getCurrentTimeZoneDisplay(std::wstring &display);

private:
	SoundPlayer *_soundPlayer;
	cefIntegration::CefClient *_cefClient;
};

class SoundPlayer : public ISoundCallBack {
	struct PlayerData {
		std::string id;
		int64_t requestId;
		float seconds;
		WavePlayer *wavePlayer;
		CefWindowHandle handle;
		bool loop;
		std::wstring fileName;

		PlayerData() : requestId(0), seconds(0.0), wavePlayer(NULL), handle(NULL),loop(false),fileName(L""){}
		bool operator!() const { return !id.empty() && wavePlayer != NULL; }
	};

public:
	SoundPlayer(cefIntegration::CefClient *cefClient);
	~SoundPlayer();

	int playSound(const std::string& filename, const std::string& id, int64_t requestId, CefRefPtr<CefBrowser> browser, bool loop);
	int stopSound(const std::string& id);

private:
	virtual void onSoundPlayed(long cbData);
	virtual void onPlayingNotification(unsigned int percent, float seconds, long cbData);

	void onSoundPlayedInMainThread(long cbData);
	void onPlayingNotificationInMainThread(unsigned int percent, float seconds, long cbData);
	void executeSoundCallback(std::string msgid, int64_t requestId, CefWindowHandle handle);
	void executePlayingNotification(unsigned int percent, float seconds, std::string id, CefWindowHandle handle);
	const SoundPlayer::PlayerData& getPlayerData(long index) const;

private:
	cefIntegration::CefClient *_cefClient;

	typedef std::map<long, PlayerData> PLAYSERSET;
	PLAYSERSET _playerContainer;
};

}
#endif