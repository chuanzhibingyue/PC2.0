// UCSDK_P2PTest.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>

#include "IUCEngine.h"
#include "gtest/gtest.h"
#include "StringConvert.h"
#include "Gzip.h"
#include "LogManager.h"
#include "UCMessageType.h"
#include "MessageParser.h"
#include "UCMessage.h"
#include "ContactInfo.h"
#include "IP2PService.h"

using namespace std;
using namespace uc;

/**
 * 创建程序的工作目录
 */
#define APP_DIRECTORY_NAME "Bee";

static string g_AppDir = "";
static IUCEngine *g_ucEngine = NULL;


class UCMockP2P : public IP2PServiceEvent {
public:
	UCMockP2P() {
	}
	virtual ~UCMockP2P() {

	}

private:
	IP2PService* _ftService;

public:
	bool Init() {
		cout << "P2P Init" << endl;
		_ftService = (IP2PService*)g_ucEngine->QueryService(UCP2PService);
		_ftService->registerListener(this);
		_ftService->Initialize();
		return true;
	}

	bool UnInit() {
		cout << "FT UnInit" << endl;
		_ftService->removeListener(this);
		return true;
	}

protected:
	virtual void OnUploadFile() {
		cout << "UI 得到文件上传结果" << endl;
	}
	virtual void OnDownloadFile() {
		cout << "UI 得到文件下载结果" << endl;
	}

public:
	int32_t UploadFile() {
		cout << "UI 发起文件上传请求" << endl;
		return _ftService->UploadFile();
	}

	int32_t DownloadFile() {
		cout << "UI 发起文件下载请求" << endl;
		return _ftService->DownloadFile();
	}
};

void InitAppDirectory() {
	char path[PATH_MAX] = {0};

	getcwd(path, PATH_MAX);

	g_AppDir.append(path);
	g_AppDir.append("\\Bee");
};

int main(int argc, char* argv[])
{
	InitAppDirectory();

	string logPath = g_AppDir + "\\Log";

	cout << "mkdir: " << mkdir(logPath.c_str()) << endl;

	UCEngineConfig ucConfig;

	ucConfig.casServer = "https://testcloud.quanshi.com";
	ucConfig.appPath = g_AppDir;
	ucConfig.logConfig.logPath = logPath;
	ucConfig.logConfig.logToFile = true;
	ucConfig.logConfig.programName = "全时蜜蜂";
	ucConfig.logConfig.logFileName = "sdkclient.log";

	g_ucEngine = CreateUCEngine(ucConfig);

	UC_LOG(INFO) << "UC start";

	UCMockP2P p2p;
	p2p.Init();

	char input;
	bool run = true;

	while(run) {
		cin >> input;
		switch (input) {
		case 'D':
		case 'd': 
			p2p.DownloadFile();
			break;

		case 'U':
		case 'u': 
			p2p.UploadFile();
			break;
		case 'q':
		case 'Q':
			run = false;
			break;
		}
	}

	p2p.UnInit();

	UC_LOG(INFO) << "UC exit";

	LogManager::GetInstance().UnInit();
	DestroyUCEngine();

	return 0;
}