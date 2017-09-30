#include "IClientService.h"
#include <boost/thread.hpp>
#include "LogManager.h"
#include "StringConvert.h"
#include "UCErrorCode.h"
#include "ClientInfo.h"
#include "UCMockClient.h"
#include "gtest/gtest.h"
#include "UCCommonFunctionMock.h"

void CreateEngineConfig(UCEngineConfig &ucConfig) {
	char path[PATH_MAX] = {0};
		string appDir;

		getcwd(path, PATH_MAX);
		appDir.append(path);
		appDir.append("/Bee");
	#ifdef PLATFORM_WIN
		mkdir(appDir.c_str());
	#else
		mkdir(appDir.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
	#endif

		string logPath = appDir + "/Log";

	#ifdef PLATFORM_WIN
		mkdir(logPath.c_str());
	#else
		mkdir(logPath.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
	#endif
		
		ucConfig.appPath = appDir;
		ucConfig.logConfig.logPath = logPath;
		ucConfig.logConfig.logToFile = true;
		ucConfig.logConfig.programName = "È«Ê±ÃÛ·ä";
		ucConfig.logConfig.logFileName = "sdkclient.log";

		//cout <<"%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
}

void UC_EXPECT_EQ(boost::recursive_mutex &mutex, boost::condition_variable_any &condition, int32_t expected, int32_t &actual) {
	boost::unique_lock<boost::recursive_mutex> lock(mutex);
	condition.wait(lock);
	cout <<"***********get resultCode from server:"<<actual<<endl;
	ASSERT_EQ(expected, actual);
	lock.unlock();
}

void UC_Lock_Notify(boost::recursive_mutex &mutex, boost::condition_variable &condition)
{
	mutex.lock();
	condition.notify_one();
	mutex.unlock();
}