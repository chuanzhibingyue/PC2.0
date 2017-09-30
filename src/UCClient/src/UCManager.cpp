#include "UCManager.h"
#include "utils/FileUtil.h"
#include "utils/HTTPProxy.h"
#include "Configuration.h"
#include "Shlobj.h"
#include "UCDefs.h"
#include "common/CallOnMainThread.h"
#include "gkclient/GKClientBrowserDelegate.h"

namespace ucclient {

UCManager UCManager::_instance;

UCManager::UCManager() : _clientSeviceStrategy(NULL), _contactSeviceStrategy(NULL),
	_imServiceStrategy(NULL), _calendarServiceStrategy(NULL), 
	_groupServiceStrategy(NULL), _catchScreenStrategy(NULL),
	_systemService(NULL), _audioServiceStrategy(NULL), 
	_sharingServiceStrategy(NULL),
	_searchService(NULL), _microblogServiceStrategy(NULL)
#ifdef PBX_SERVICE
	,_pbxServiceStrategy(NULL)
#endif // PBX_SERVICE
{
}

UCManager::~UCManager() {
	if (_clientSeviceStrategy != NULL) {
		delete _clientSeviceStrategy;
		_clientSeviceStrategy = NULL;
	}

	if (_contactSeviceStrategy != NULL) {
		delete _contactSeviceStrategy;
		_contactSeviceStrategy = NULL;
	}

	if (_imServiceStrategy != NULL) {
		delete _imServiceStrategy;
		_imServiceStrategy = NULL;
	}

	if (_calendarServiceStrategy != NULL) {
		delete _calendarServiceStrategy;
		_calendarServiceStrategy = NULL;
	}

	if (_groupServiceStrategy != NULL) {
		delete _groupServiceStrategy;
		_groupServiceStrategy = NULL;
	}

	if (_fsServiceStrategy != NULL ) {
		delete _fsServiceStrategy;
		_fsServiceStrategy = NULL;
	}
#ifdef PBX_SERVICE
	if (_pbxServiceStrategy != NULL) {
		delete _pbxServiceStrategy;
		_pbxServiceStrategy = NULL;
	}
#endif // PBX_SERVICE

	if (_catchScreenStrategy != NULL) {
		delete _catchScreenStrategy;
		_catchScreenStrategy = NULL;
	}

	if (_systemService != NULL) {
		delete _systemService;
		_systemService = NULL;
	}

	if (_audioServiceStrategy != NULL) {
		delete _audioServiceStrategy;
		_audioServiceStrategy = NULL;
	}

	if (_sharingServiceStrategy != NULL) {
		delete _sharingServiceStrategy;
		_sharingServiceStrategy = NULL;
	}

	if (_searchService != NULL) {
		delete _searchService;
		_searchService = NULL;
	}

	if (_microblogServiceStrategy != NULL) {
		delete _microblogServiceStrategy;
		_microblogServiceStrategy = NULL;
	}

	if (_openApiServiceStrategy != NULL) {
		delete _openApiServiceStrategy;
		_openApiServiceStrategy = NULL;
	}

	if (_externalServiceStrategy != NULL) {
		delete _externalServiceStrategy;
		_externalServiceStrategy = NULL;
	}

	if (_updateServiceStrategy != NULL) {
		delete _updateServiceStrategy;
		_updateServiceStrategy = NULL;
	}

	if (_gkclient != NULL) {
		delete _gkclient;
		_gkclient = NULL;
	}
}

void UCManager::Initialize(cefIntegration::CefClient *cefClient) {
	UCEngineConfig config;
	CreateUCEngineConfig(config);
	_engine = CreateUCEngine(config);
	
	_cefClient = cefClient;
	_clientSeviceStrategy = new ClientServiceStrategy(_engine, _cefClient);
	_contactSeviceStrategy = new ContactServiceStrategy(_engine, _cefClient); 
	_imServiceStrategy = new IMServiceStrategy(_engine, _cefClient); 
	_calendarServiceStrategy = new  CalendarServiceStrategy(_engine, _cefClient);
	_groupServiceStrategy = new GroupServiceStrategy(_engine, _cefClient);
	_fsServiceStrategy = new FileTransferServiceStrategy(_engine, _cefClient);
#ifdef PBX_SERVICE
	_pbxServiceStrategy = new PbxServiceStrategy(_engine, _cefClient);
#endif // PBX_SERVICE
	_catchScreenStrategy = new CatchScreenServiceStrategy(_cefClient);
	_systemService = new SystemService(_cefClient);
	_audioServiceStrategy = new AudioServiceStrategy(_engine, _cefClient);
	_sharingServiceStrategy = new SharingServiceStrategy(_engine, _cefClient);
	_searchService = new SearchServiceStrategy(_engine, _cefClient);
	_microblogServiceStrategy = new MicroblogServiceStrategy(_engine, _cefClient);
	_openApiServiceStrategy = new OpenApiServiceStrategy(_engine, _cefClient);
	_updateServiceStrategy = new UpdateServiceStrategy(_engine, _cefClient);
	_externalServiceStrategy = new ExternalServiceStrategy(_engine, _cefClient);
	m_bbsServiceStrategy = new BBSServiceStrategy(_engine, _cefClient);

	IClientService *clientService = (IClientService*)_engine->QueryService(UCClientService);
	IContactService *contactService = (IContactService*)_engine->QueryService(UCContactService);
	IIMService *imService = (IIMService*)_engine->QueryService(UCIMService);
	ICalendarService* calendarService = (ICalendarService*)_engine->QueryService(UCCalendarService);
	IGroupService *groupService = (IGroupService*)_engine->QueryService(UCGroupService);
	IFileTransferService *fsService = (IFileTransferService*)_engine->QueryService(UCFileTransferService);
#ifdef PBX_SERVICE
	IPbxService *lpPbxService = (IPbxService*)_engine->QueryService(UCPbxService);
#endif // PBX_SERVICE
	IAudioService *audioService = (IAudioService *)_engine->QueryService(UCAudioService);
	ISharingService* sharingService = (ISharingService*)_engine->QueryService(UCSharingService);
	ISearchService *searchService = (ISearchService*)_engine->QueryService(UCSearchService);
	IMicroblogService *microblogService = (IMicroblogService*)_engine->QueryService(UCMicroblogService);
	IOpenApiService *openApiService = (IOpenApiService*)_engine->QueryService(UCOpenApiService);
	IExternalService* externalService = (IExternalService*)_engine->QueryService(UCExternalService);
	IBBSService *bbsService = (IBBSService *)_engine->QueryService(UCBBSService);

	clientService->registerListener(_clientSeviceStrategy);
	contactService->registerListener(_contactSeviceStrategy);
	imService->registerListener(_imServiceStrategy);
	calendarService->registerListener(_calendarServiceStrategy);
	groupService->registerListener(_groupServiceStrategy);
	fsService->registerListener(_fsServiceStrategy);
#ifdef PBX_SERVICE
	lpPbxService->registerListener(_pbxServiceStrategy);
#endif // PBX_SERVICE
	audioService->registerListener(_audioServiceStrategy);
	sharingService->registerListener(_sharingServiceStrategy);
	searchService->registerListener(_searchService);
	microblogService->registerListener(_microblogServiceStrategy);
	openApiService->registerListener(_openApiServiceStrategy);
	externalService->registerListener(_externalServiceStrategy);
	bbsService->registerListener(m_bbsServiceStrategy);

	// get curl log settings.
	std::wstring modulePath;
	ucclient::FileUtil::ProcessPath(modulePath);
	std::wstring configPath = modulePath + L"\\config\\bee.config";
	ucclient::Configuration xmlConfig(configPath);
	std::string curlLog = xmlConfig.QueryStringValue("/configuration/curl-log", "false");
	int curlSettings = 0;
	if (curlLog.compare("true") == 0)
	{
		curlSettings |= 1;
	}

	std::string verifyHost = xmlConfig.QueryStringValue("/configuration/dont-verify-host", "false");
	if (verifyHost.compare("true") == 0)
	{
		curlSettings |= 2;
	}
	clientService->EnableCurlSettings(curlSettings);

	_gkclient = new GkClient(_cefClient);
	cefClient->RegisterBrowserDelegate(new gkclient::GKClientBrowserDelegate(cefClient, _gkclient));

	clientService->registerListener(_gkclient);
	groupService->registerListener(_gkclient);

	clientService->SetProxyUrl(HTTPProxy::GetInstance()->QueryProxyInfo(config.casServer));
	std::string proxyAccount;
	std::string proxyPassword;
	int authType = 0;
	if (clientService->GetProxyInfo(proxyAccount, proxyPassword, authType)){
		_cefClient->SetAuthInfo(StringConvert::FromUTF8Str(proxyAccount), StringConvert::FromUTF8Str(proxyPassword));
		_gkclient->SetProxyAuthInfo(authType, StringConvert::FromUTF8Str(proxyAccount), StringConvert::FromUTF8Str(proxyPassword));
#ifdef PBX_SERVICE
		lpPbxService->SetProxyInfo(authType, proxyAccount, proxyPassword);
#endif // PBX_SERVICE
	}

	_netMonitor.registerListener(this);
	_netMonitor.registerListener(_gkclient);
	_netMonitor.WatchForNetworkChange();
}

void UCManager::UnInitialize() {
	DestroyUCEngine();

	HTTPProxy::GetInstance()->Reset();
	_netMonitor.StopWatching();
	_netMonitor.removeAllListeners();
}

void UCManager::LoadDefaultHtml() {
	cefIntegration::WindowFeature feature;
	feature.width = 465;
	feature.height = 640;
	feature.centerScreen = true;

	RECT rect;
	rect.left = 37;
	rect.top = 0;
	rect.right = feature.width - 47;
	rect.bottom = 50;
	feature.captions.push_back(rect);
	feature.title = UCCLIENT_APP_NAME;
	feature.resizable = false;
	feature.hasTaskBarIcon = true;
	feature.isTop = false;
	feature.title = ucclient::UCManager::GetInstance().GetDefaultWinTitle();
	feature.behaviorIfSCClose = cefIntegration::BSC_QUIT;
	
	_cefClient->getWindowManager()->Open(NULL, L"index.html", L"index", feature, cefIntegration::ABOW_DEFAULT);
}

void UCManager::CreateUCEngineConfig(UCEngineConfig &config) {
	std::wstring modulePath;
	FileUtil::ProcessPath(modulePath);
	std::wstring configPath = modulePath + L"\\config\\bee.config";
	Configuration xmlConfig(configPath);

	config.version = xmlConfig.QueryStringValue("/configuration/app/version", "");
	_clientVersion = StringConvert::FromUTF8Str(config.version);

	std::string casDomain = xmlConfig.QueryStringValue("/configuration/ucsdk/casDomain", "testcloud.quanshi.com");
	config.casServer = "https://" + casDomain;

	std::wstring documentPath = FileUtil::GetDocumentPath() + L"\\Quanshi Files\\Bee\\" + StringConvert::FromUTF8Str(casDomain);
	std::wstring appDataPath = FileUtil::GetAppDataPath() + L"\\Quanshi\\Bee\\";
	config.appPath = StringConvert::ToUTF8Str(appDataPath + StringConvert::FromUTF8Str(casDomain));
	config.documentPath = StringConvert::ToUTF8Str(documentPath);
	config.logConfig.logPath = StringConvert::ToUTF8Str(appDataPath) + "log";//GetLogPath();
	config.logConfig.logToFile = true;
	config.logConfig.programName = StringConvert::ToUTF8Str(UCCLIENT_APP_NAME);
	config.logConfig.logFileName = "client.log";
	config.modulePath = StringConvert::ToUTF8Str(modulePath);

	FileUtil::CreateFolderRecursively(StringConvert::FromUTF8Str(config.appPath));
	FileUtil::CreateFolderRecursively(documentPath);
	FileUtil::CreateFolderRecursively(StringConvert::FromUTF8Str(config.appPath));
	FileUtil::CreateFolder(StringConvert::FromUTF8Str(config.logConfig.logPath));

	_clientVersion = StringConvert::FromUTF8Str(xmlConfig.QueryStringValue("/configuration/app/version", ""));

	UC_LOG(WARNING) << "Create UCEngine, client version: " << _clientVersion;
}

void UCManager::OnNetWorkChanged(ConnectionState state) {
	if (!ON_MAIN_THREAD()) {
		uc::CallOnMainThread(this, &UCManager::OnNetWorkChanged, state);
		return;
	}

	UC_LOG(INFO) << "dispatch OnNetWorkChanged event to JS, state: " << state;

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << NETWORK_CHANGED
		<< L"\",{detail:" << L"{connectState: " << state << "},});"
		<< L"document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());
}

std::string UCManager::GetLogPath() {
	std::string logPath = FileUtil::GetAppDataPathA();
	logPath += "\\Quanshi\\Bee\\log";
	return logPath;
}

}


