#include "SystemService.h"
#include "utils/Clipboard.h"
#include "utils/FileUtil.h"
#include "common/StringConvert.h"
#include "UCDefs.h"
#include <sstream>
#include <vector>
#include "common/CallOnMainThread.h"
#include "ClientDefs.h"

namespace ucclient {

using namespace uc;

#define SOUNDMSG_ID 65432

const WCHAR* CURRENT_ZONE_REGKEY = L"SYSTEM\\CurrentControlSet\\Control\\TimeZoneInformation";
const WCHAR* TIME_ZONE_KEY_NAME_REGKEY = L"TimeZoneKeyName";
const WCHAR* ALL_TIME_ZONES_REGKEY = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones\\";
const WCHAR* DISPLAY_REGKEY = L"Display";

SystemService::SystemService(cefIntegration::CefClient *cefClient)
	: _soundPlayer(new SoundPlayer(cefClient)), _cefClient(cefClient) {
	//cleanup picture temp file when login
	FileUtil::RemoveFolder(Clipboard().getTempFilePath());
}

SystemService::~SystemService() {
	delete _soundPlayer;
}

std::string SystemService::getClipboardData(HWND wnd) {
	Clipboard clipboard;

	std::string format = clipboard.getClipboardDataFormat(wnd);
	vector<std::string> data = Clipboard().getClipboardData(wnd);

	std::stringstream ss;
	ss << "{";
	ss << "\"format\":\"" << format.c_str() <<"\", ";
	
	ss << "\"items\": [";
	for (int i = 0; i < data.size(); i++) {
		if (i != 0)
			ss << ",";

		ss << "\"" << StringConvert::JsonEscape(data[i]) << "\"";
	}
	ss << "] ";

	ss << "}";

	return ss.str();
}

std::string SystemService::hasValidClipboardData(HWND wnd) {
	std::string format = Clipboard().getClipboardDataFormat(wnd);

	std::stringstream ss;
	ss << "{";
	ss << "\"valid\":" << (format.empty() ? 0 : 1 );
	ss << "}";

	return ss.str();
}

bool SystemService::setClipboardData(const std::string& filePath, CefRefPtr<CefBrowser> browser) {
	Clipboard clipboard;
	return clipboard.setClipboardData(filePath, browser->GetHost()->GetWindowHandle());
}

int SystemService::playSound(const std::string& filename, const std::string& id, int64_t requestId, bool loop,CefRefPtr<CefBrowser> browser)
{
	return _soundPlayer->playSound(filename, id, requestId, browser,loop);
}

int SystemService::stopSound(const std::string& id) {
	return _soundPlayer->stopSound(id);
}

int32_t SystemService::getCurrentTimeZoneDisplay(std::wstring &display) {
	HKEY key;
	int32_t res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, CURRENT_ZONE_REGKEY,
		0, KEY_READ | KEY_WOW64_64KEY, &key);

	WCHAR buffer[MAX_PATH];
	DWORD bufferLen = MAX_PATH;
	if (res == ERROR_SUCCESS) {
		res = RegQueryValueEx(key, TIME_ZONE_KEY_NAME_REGKEY, NULL, NULL, reinterpret_cast<LPBYTE>(&buffer), &bufferLen);
		RegCloseKey(key);

		std::wstring timezone = ALL_TIME_ZONES_REGKEY;
		timezone.append(buffer);

		memset(buffer, 0, MAX_PATH);
		bufferLen = MAX_PATH;
		res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, timezone.c_str(),
			0, KEY_READ | KEY_WOW64_64KEY, &key);

		if (res == ERROR_SUCCESS) {
			res = RegQueryValueEx(key, DISPLAY_REGKEY, NULL, NULL, reinterpret_cast<LPBYTE>(&buffer), &bufferLen);
			display.append(buffer);
			RegCloseKey(key);
		}
	}

	return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SoundPlayer::SoundPlayer(cefIntegration::CefClient *cefClient) : _cefClient(cefClient) {
}

SoundPlayer::~SoundPlayer(){
	PLAYSERSET::iterator iter = _playerContainer.begin();
	while (iter != _playerContainer.end()){
		PlayerData& data = iter->second;
		data.wavePlayer->Stop();
		delete data.wavePlayer;
		++iter;
	}
}

int SoundPlayer::playSound(const std::string& filename, const std::string& id, int64_t requestId, CefRefPtr<CefBrowser> browser, bool loop)
{
	WavePlayer *wavePlayer = new WavePlayer();
	wavePlayer->SetCallback(this, (long)wavePlayer);

	DWORD length;
	std::wstring wfilename = StringConvert::FromUTF8Str(filename);
	wavePlayer->Play(wfilename.c_str(), &length);

	if (length == 0) {
		delete wavePlayer;
		return UCCLIENT_ERROR;
	}

	PlayerData playerData;
	playerData.id = id;
	playerData.requestId = requestId;
	playerData.seconds = round((float)length / 1000);
	playerData.wavePlayer = wavePlayer;
	playerData.handle = browser->GetHost()->GetWindowHandle();
	playerData.loop = loop;
	playerData.fileName = wfilename;
	_playerContainer.insert(make_pair((long)wavePlayer, playerData));

	return 0;
}

int SoundPlayer::stopSound(const std::string& id) {
	PLAYSERSET::iterator iter = _playerContainer.begin();
	while (iter != _playerContainer.end()){
		PlayerData& data = iter->second;
		if (data.id == id) {
			data.loop = false;
			data.wavePlayer->Stop();
		}
		
		++iter;
	}

	return 0;
}

void SoundPlayer::executeSoundCallback(std::string msgId, int64_t requestId, CefWindowHandle handle) {
	CefRefPtr<CefProcessMessage> response = CefProcessMessage::Create("InvokeCallback");
	CefRefPtr<CefListValue> responseArgs = response->GetArgumentList();
	responseArgs->SetInt(0, requestId);
	responseArgs->SetString(1, msgId);

	if (_cefClient->GetBrowser(handle))
		_cefClient->GetBrowser(handle)->SendProcessMessage(PID_RENDERER, response);
}

void SoundPlayer::executePlayingNotification(unsigned int percent, float seconds, std::string id, CefWindowHandle handle) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << PLAYINGSOUND_NOTIFICATION
		<< L"\",{\"detail\":" << L"{\"id\":\"" << id.c_str()
		<< L"\", \"percent\":" << percent << L", \"seconds\":" << round(seconds);
	jsScript << L"}});document.dispatchEvent(e);";
	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());
}

void SoundPlayer::onSoundPlayed(long cbData) {
	CallOnMainThread(this, &SoundPlayer::onSoundPlayedInMainThread, cbData);
}

void SoundPlayer::onSoundPlayedInMainThread(long cbData) {
	const PlayerData& data = getPlayerData(cbData);
	if (data.loop)
	{
		WavePlayer *wavePlayer = new WavePlayer();
		wavePlayer->SetCallback(this, (long)wavePlayer);
		DWORD length;
		wavePlayer->Play(data.fileName.c_str(), &length);
		
		PlayerData data1 = data;
		data1.wavePlayer = wavePlayer;

		_playerContainer.insert(make_pair((long)wavePlayer, data1));
	}

	executePlayingNotification((unsigned int)100, data.seconds, data.id, data.handle);

	if (!data) {
		delete data.wavePlayer;
	}
	_playerContainer.erase(cbData);
}

void SoundPlayer::onPlayingNotification(unsigned int percent, float seconds, long cbData) {
	CallOnMainThread(this, &SoundPlayer::onPlayingNotificationInMainThread, percent, seconds, cbData);
}

void SoundPlayer::onPlayingNotificationInMainThread(unsigned int percent, float seconds, long cbData) {
	const PlayerData& data = getPlayerData(cbData);
	executePlayingNotification(percent - 1, seconds, data.id, data.handle);
}

const SoundPlayer::PlayerData& SoundPlayer::getPlayerData(long index) const {
	static PlayerData static_player_data;
	PLAYSERSET::const_iterator iter = _playerContainer.find(index);
	if (iter != _playerContainer.end()) {
		return iter->second;
	}
	else {
		return static_player_data;
	}
}

}