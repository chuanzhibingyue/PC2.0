/*
 * UCManager.h
 *
 *  Created on: June 27, 2015
 *      Author: gang wang
 */

#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include <string>

#include "ClientServiceStrategy.h"
#include "ContactServiceStrategy.h"
#include "IMServiceStrategy.h"
#include "CalendarServiceStrategy.h"
#include "GroupServiceStrategy.h"
#include "FileTransferServiceStrategy.h"
#include "CatchScreenServiceStrategy.h"
#include "MicroblogServiceStrategy.h"
#include "BBSServiceStragegy.h"
#include "UpdateServiceStrategy.h"
#include "SystemService.h"
#include "AudioServiceStrategy.h"
#include "SearchServiceStrategy.h"
#include "OpenApiServiceStrategy.h"
#include "ExternalServiceStrategy.h"
#include "SharingServiceStrategy.h"
#include "IUCEngine.h"
#include "CefClient.h"
#include "utils/NetworkMonitor.h"
#include "gkclient/GkClient.h"
#include "utils/PowerMonitor.h"

#ifdef PBX_SERVICE
#include "PbxServiceStrategy.h"
#endif // PBX_SERVICE

namespace ucclient {

class UCManager : public NetworkListener{
public:
	static UCManager& GetInstance() {
		return _instance;
	}
	
	~UCManager();
	
	void Initialize(cefIntegration::CefClient *cefClient);
	void UnInitialize();
	void LoadDefaultHtml();
	const std::wstring& GetClientVersion() { return _clientVersion; }

	ClientServiceStrategy* GetClientServiceStrategy() { return _clientSeviceStrategy;}
	ContactServiceStrategy* GetContactServiceStrategy() { return _contactSeviceStrategy; }
	IMServiceStrategy* GetIMStrategy() { return _imServiceStrategy; }
	CalendarServiceStrategy* GetCalendarServiceStrategy() { return _calendarServiceStrategy; }
	GroupServiceStrategy* GetGroupServiceStrategy() { return _groupServiceStrategy; }
	FileTransferServiceStrategy* GetFileTransferServiceStrategy() { return _fsServiceStrategy; }
#ifdef PBX_SERVICE
	PbxServiceStrategy* GetPbxServiceStrategy() { return _pbxServiceStrategy; }
#endif // PBX_SERVICE
	CatchScreenServiceStrategy* GetCatchScreenServiceStrategy() { return _catchScreenStrategy; }
	SystemService* getSystemService() { return _systemService; }
	AudioServiceStrategy* GetAudioServiceStrategy() { return _audioServiceStrategy; }
	SharingServiceStrategy* GetSharingServiceStrategy() { return _sharingServiceStrategy; }
	SearchServiceStrategy* GetSearchServiceStrategy() { return _searchService; }
	OpenApiServiceStrategy* GetOpenApiServiceStrategy() { return _openApiServiceStrategy; }
	MicroblogServiceStrategy* GetMicroblogServiceStrategy() { return _microblogServiceStrategy; }
	UpdateServiceStrategy* GetUpdateServiceStrategy() { return _updateServiceStrategy; }
	ExternalServiceStrategy* GetExternalServiceStrategy(){ return _externalServiceStrategy; }
	NetworkMonitor& GetNetworkMonitor() { return _netMonitor; }
	GkClient* GetGkClient() {return _gkclient; }
	BBSServiceStrategy *GetBBSServiceStrategy() { return m_bbsServiceStrategy; }

	void SetDefaultWinTitle(const std::wstring& title) { _winTitle = title; }
	std::wstring GetDefaultWinTitle() { return _winTitle; }

private:
	UCManager();
	void CreateUCEngineConfig(UCEngineConfig &config);

	virtual void OnNetWorkChanged(ConnectionState state);

	std::string GetLogPath();

private:
	static UCManager _instance;
	cefIntegration::CefClient *_cefClient;
	IUCEngine *_engine;
	
	ClientServiceStrategy *_clientSeviceStrategy;
	ContactServiceStrategy *_contactSeviceStrategy;
	IMServiceStrategy *_imServiceStrategy;
	CalendarServiceStrategy *_calendarServiceStrategy;
	GroupServiceStrategy* _groupServiceStrategy;
	OpenApiServiceStrategy *_openApiServiceStrategy;
	FileTransferServiceStrategy* _fsServiceStrategy;
#ifdef PBX_SERVICE
	PbxServiceStrategy* _pbxServiceStrategy;
#endif // PBX_SERVICE
	CatchScreenServiceStrategy* _catchScreenStrategy;
	SystemService* _systemService;
	AudioServiceStrategy *_audioServiceStrategy;
	SharingServiceStrategy* _sharingServiceStrategy;
	SearchServiceStrategy* _searchService;
	MicroblogServiceStrategy *_microblogServiceStrategy;
	UpdateServiceStrategy* _updateServiceStrategy;
	ExternalServiceStrategy* _externalServiceStrategy;
	BBSServiceStrategy *m_bbsServiceStrategy;
	NetworkMonitor _netMonitor;
	GkClient* _gkclient;
	PowerMonitor _powerMonitor;

	std::wstring _clientVersion;

	std::wstring _winTitle;
};

}

#endif