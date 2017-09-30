#include "UpdateServiceStrategy.h"
#include "common/StringConvert.h"
#include "UpdateManagerImpl.h"
#include "LogManager.h"
#include "UCDefs.h"
#include "utils/FileUtil.h"
#include "Configuration.h"

namespace ucclient {

using namespace uc;

UpdateServiceStrategy::UpdateServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient)
	: _engine(engine), _cefClient(cefClient) {
	GetAppConfig();
}

void UpdateServiceStrategy::InitUpdate(string& upgradeServer, string& packageInfo, string& packageDownload, int& httpPort) {
	IClientService *clientService = (IClientService*)_engine->QueryService(UCClientService);
	clientService->GetUpgradeInfo(upgradeServer, packageInfo, packageDownload);
	httpPort = GetHttpPort(upgradeServer);
	UC_LOG(INFO) << "InitUpdate, upgradeServer:" << upgradeServer << ", packageInfo:" << packageInfo
		<< ", packageDownload:" << packageDownload << ", httpPort:" << httpPort;
}

int UpdateServiceStrategy::CheckUpdates(int requestId) {
	int httpPort = 0;
	std::string upgradeServer, packageInfo, packageDownload;

	InitUpdate(upgradeServer, packageInfo, packageDownload, httpPort);
	UC_LOG(INFO) << "CheckUpdates, upgradeServer:" << upgradeServer << ", packageInfo:" << packageInfo << ", packageDownload:" << packageDownload << ", httpPort:" << httpPort
		<< ", appId:" << _appConfig.appId << ", siteId:" << _appConfig.siteId << ", skinId:" << _appConfig.skinId;

	UpdateManagerImpl::GetInstance().Init(this, UpdateServiceStrategy::CheckUpdatesCallBack, UpdateServiceStrategy::ProgressCallBack, UpdateServiceStrategy::ErrorCallBack, TRUE);
	int ret = UpdateManagerImpl::GetInstance().CheckUpdates(_appConfig.appId, _appConfig.siteId, _appConfig.skinId, (_bstr_t)(upgradeServer.c_str()), httpPort, _appConfig.timeOut, L"");
	if (ret != 0) {
		UC_LOG(ERROR) << "CheckUpdates,  serverUrl:" << upgradeServer << ", httpPort:" << httpPort;
	}
	return ret;
}

void _stdcall UpdateServiceStrategy::CheckUpdatesCallBack(void* userdata, int type, BOOL isNeedUpdate, const std::wstring& version, const std::wstring& notes) {
	((UpdateServiceStrategy*)(userdata))->OnUpdatesChecked(isNeedUpdate, version, notes);
}

void _stdcall UpdateServiceStrategy::ErrorCallBack(void* userdata, int type, int err, const std::wstring& version) {
	((UpdateServiceStrategy*)(userdata))->OnError(type, err, version);
}

void _stdcall UpdateServiceStrategy::ProgressCallBack(void* userdata, int type, int value) {
	((UpdateServiceStrategy*)(userdata))->OnProgressChecked(value);
}

void UpdateServiceStrategy::OnUpdatesChecked(BOOL isNeedUpdate, const wstring& version, const wstring& notes) {
	std::vector<std::wstring> noteList;
	if (!notes.empty()) {
		StringConvert::Split(notes, wstring(L"\n"), noteList);
	}

	std::wstringstream notesScript;
	notesScript << "[";
	for (std::vector<std::wstring>::iterator itor = noteList.begin();
		itor != noteList.end(); ++itor){
		if (itor != noteList.begin())
			notesScript << ",";
		notesScript << "\"" << *itor << "\"";
	}
	notesScript << "]";

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CHECK_UPDATES_EVENT
		<< "\",{detail:" << "{errorCode: " << 0 
		<< ", updateFlag: " << isNeedUpdate 
		<< ", version: \"" << version
		<< "\", notes: " << notesScript.str()
		<< ", downloadUrl: \"" << L""
		<< "\"}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUpdatesChecked event to JS:" << jsScript.str();
}

void UpdateServiceStrategy::OnError(int type, int err, const std::wstring& version) {
	wstring downloadPackageUrl = GetDownLoadUrl();
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CHECK_UPDATES_EVENT
		<< "\",{detail:" << "{errorCode: " << err 
		<< ", updateFlag: " << false 
		<< ", version: \"" << version
		<< "\", notes: " << L"[]"
		<< ", downloadUrl: \"" << downloadPackageUrl
		<< "\"}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnUpdatesChecked event to JS:" << jsScript.str();
}

void UpdateServiceStrategy::OnProgressChecked(int value) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << CHECK_PROGRESS_EVENT
		<< "\",{detail:" << "{checkProgress: " << value
		<< "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnProgressChecked event to JS:" << jsScript.str();
}

wstring UpdateServiceStrategy::GetDownLoadUrl() {
	string uccServer;
	IClientService *clientService = (IClientService*)_engine->QueryService(UCClientService);
	clientService->GetUccServerDomain(uccServer);
	wstring downloadPackageUrl = StringConvert::FromUTF8Str(uccServer);
	
	UC_LOG(INFO) << "downloadPackageUrl:" << downloadPackageUrl;
	return downloadPackageUrl;
}

int UpdateServiceStrategy::Update() {
	int httpPort = 0;
	std::string upgradeServer, packageInfo, packageDownload;
	InitUpdate(upgradeServer, packageInfo, packageDownload, httpPort);

	std::stringstream ids;
	ids << httpPort << ";" << _appConfig.appId << ";" << _appConfig.siteId << ";" << _appConfig.skinId << ";" << _appConfig.timeOut;
	string cmdLine = upgradeServer + ";" + packageInfo + ";" + packageDownload + ";" + ids.str() + ";" + _engine->GetConfig().logConfig.logPath 
		+ ";" + StringConvert::ToUTF8Str(GetDownLoadUrl());
	UC_LOG(INFO) << "Update, cmdLine:" << cmdLine;

	int ret = UpdateManagerImpl::GetInstance().RealUpdate((_bstr_t)cmdLine.c_str());
	if (ret != 0) {
		UC_LOG(ERROR) << "Update return error:" << ret << ", cmdLine:" << cmdLine;
	}

	// exit bee.exe
	_cefClient->Quit();

	return ret;
}

wstring UpdateServiceStrategy::GetLocalVersion() {
	wstring localVersion;
	int ret = UpdateManagerImpl::GetInstance().GetLocalVersion(localVersion);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetLocalVersion return error:" << ret;
	}
	return localVersion;
}

void UpdateServiceStrategy::GetAppConfig() {
	std::wstring modulePath;
	FileUtil::ProcessPath(modulePath);
	std::wstring configPath = modulePath + L"\\config\\bee.config";
	Configuration xmlConfig(configPath);

	_appConfig.appId = xmlConfig.QueryIntValue("/configuration/app/appid", 30);
	_appConfig.siteId = xmlConfig.QueryIntValue("/configuration/app/siteid", 50000);
	_appConfig.skinId = xmlConfig.QueryIntValue("/configuration/app/skinid", 1);
}

int UpdateServiceStrategy::GetHttpPort(const string& server) {
	int ret = 80;
	size_t index = server.find(":");
	if (index != string::npos) {
		string domain = server.substr(index + 3); //oncloud2.quanshi.com:80/upgradeserver/version"
		index = domain.find(":");
		string url  = domain.substr(index + 1); //80/upgradeserver/version
		index = url.find("/");
		string port = url.substr(0, index); //80
		ret = std::atoi(port.c_str());
	}
	return ret;
}

}