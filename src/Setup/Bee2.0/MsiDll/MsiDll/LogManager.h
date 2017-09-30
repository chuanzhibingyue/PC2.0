#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <string>
#include <sstream>

#if defined(WIN32)
#	include "objbase.h"
#   ifdef LOG_EXPORTS
#       define LOG_API __declspec(dllexport)
#   else 
#		define LOG_API __declspec(dllimport)
#   endif
#else
#   define LOG_API
#endif

namespace uc {
using namespace std;

//log级别定义（glog级别定义对应）
#undef ERROR
enum LogLevel
{
	INFO = 0,
	WARNING,
	ERROR
};

class LOG_API LogManager {
public:
	static LogManager& GetInstance();
	virtual ~LogManager();
public:
	void Init(
		const string& logPath,				//日志保存路径
		const string& programName,			//程序名称
		const string& logFileName,			//log文件名称
		bool logToStderr = false,			//值为true的时候，日志信息输出到stderr，并非文件。默认值为 false
		int stderrThreshold = 2,			//严重性级别在该门限值以上的日志信息除了写入日志文件以外，还要输出到stderr。默认值为2
		int logBufLevel = 0,				//日志级别小于等于该级别时，进行Buffer缓存
		int logBufSecs = 10,				//回写缓存区的时间配置
		int minLogLevel = 0,				//严重性级别在该门限值以上的日志信息才进行记录。默认值为0
		int maxLogSize = 20,				//日志文件大小M，默认20M
		int maxBackupIndex = 5,				//日志文件最大备份数
		bool stopLoggingIfDiskFull = true	//磁盘空间满时，是否停止记录日志
		);
	void UnInit();

private:
	LogManager();
	LogManager(const LogManager&);
	LogManager & operator = (const LogManager &);
};

class Log;

////////////////////////log 宏定义////////////////////////
#define UC_LOG(severity) UC_LOG_##severity.Stream()
#define UC_LOG_INFO (uc::Log( __FILE__, __LINE__, uc::LogLevel(INFO)))
#define UC_LOG_WARNING (uc::Log( __FILE__, __LINE__, uc::LogLevel(WARNING)))
#define UC_LOG_ERROR (uc::Log( __FILE__, __LINE__, uc::LogLevel(ERROR)))
#define UC_LOG_FLUSH() { uc::Log::Flush(); }

class LOG_API LogStream
{
public:
	LogStream();
	~LogStream();
	LogStream& operator<< (bool val);
	LogStream& operator<< (short val);
	LogStream& operator<< (unsigned short val);
	LogStream& operator<< (int val);
	LogStream& operator<< (unsigned int val);
	LogStream& operator<< (long val);
	LogStream& operator<< (unsigned long val);
	LogStream& operator<< (long long val);
	LogStream& operator<< (unsigned long long val);
	LogStream& operator<< (const void* val);
	LogStream& operator<< (float val);
	LogStream& operator<< (double val);
	LogStream& operator<< (char val);
	LogStream& operator<< (const char* val);
	LogStream& operator<< (const string& val);
	LogStream& operator<< (const wchar_t* val);
	LogStream& operator<< (const wstring& val);
	LogStream& operator<< (streambuf* val);
	const stringstream& GetStream() { return _stream; }
private:
	stringstream _stream;
};

class LOG_API Log
{
public:
	Log(const char* file, int line, LogLevel severity);
	~Log();

	LogStream & Stream() { return _stream; }

	// Flushes all log files that contains messages that are at least of
	// the specified severity level.  Thread-safe.
	static void Flush(LogLevel level = INFO);

private:
	std::string _file;
	int _line;
	LogLevel _severity;
	LogStream _stream;
};

} //namespace uc 
#endif //LOGMANAGER_H