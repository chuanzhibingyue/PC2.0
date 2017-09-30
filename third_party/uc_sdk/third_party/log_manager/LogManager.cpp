#include "LogManager.h"
#include "common/StringConvert.h"

#ifndef WIN32
#include "glog/logging.h"
#else
#include "windows/glog/logging.h"
#endif

namespace uc {

LogManager::LogManager() {

}

LogManager::~LogManager() {

}

LogManager& LogManager::GetInstance() {
	static LogManager instance;
	return instance;
}

void LogManager::Init(const string& logPath, const string& programName, const string& logFileName, 
		bool logToStderr, int stderrThreshold, int logBufLevel, int logBufSecs, 
		int minLogLevel, int maxLogSize, int maxBackupIndex,	bool stopLoggingIfDiskFull) {
	//init glog
	static bool is_init = false;
	if (!is_init)
	{
		google::InitGoogleLogging(programName.c_str());
		is_init = true;
	}

	//log dir
	FLAGS_log_dir = logPath;

	//log to stderr
	FLAGS_logtostderr = logToStderr;

	//stderrthreshold
	FLAGS_stderrthreshold = stderrThreshold;

	//logbuflevel
	FLAGS_logbuflevel = logBufLevel;

	//logbufsecs
	FLAGS_logbufsecs = logBufSecs;

	//min log level
	FLAGS_minloglevel = minLogLevel;

	//max_log_size, default 10M
	FLAGS_max_log_size = maxLogSize;

	//max_backup_index, default 5
	FLAGS_max_backup_index = maxBackupIndex;

	//stop_logging_if_full_disk
	FLAGS_stop_logging_if_full_disk = stopLoggingIfDiskFull;

	const string dest = FLAGS_log_dir + "//" + logFileName;
	google::SetLogDestination(INFO, dest.c_str());
	google::SetLogDestination(WARNING, "");
	google::SetLogDestination(ERROR, "");

	//coredump catch
#ifndef WIN32
	google::InstallFailureSignalHandler();
#endif
}

void LogManager::UnInit() {
	google::ShutdownGoogleLogging();
}

////////////////////////////////////////////////////////////////////
LogStream::LogStream() {
	_stream.clear();
}

LogStream::~LogStream() {

}

LogStream& LogStream::operator<< (bool val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (short val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (unsigned short val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (int val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (unsigned int val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (long val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (unsigned long val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (long long val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (unsigned long long val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (const void* val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (float val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (double val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (char val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (const char* val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (const string& val) {
	_stream << val;
	return *this;
}

LogStream& LogStream::operator<< (const wchar_t* val) {
	wstring info(val);
	_stream << StringConvert::ToUTF8Str(info);
	return *this;
}

LogStream& LogStream::operator<< (const wstring& val) {
	_stream << StringConvert::ToUTF8Str(val);
	return *this;
}

LogStream& LogStream::operator<< (streambuf* val) {
	_stream << val;
	return *this;
}

Log::Log(const char* file, int line, LogLevel severity) : _file(file){
	_line = line;
	_severity = severity;
}

Log::~Log() {
	int offset = 0;
	int length = 10000; //default buffer size
	string logInfo = _stream.GetStream().str();
	while (offset < logInfo.length())
	{
		string data = logInfo.substr(offset, length);
		offset += length;
		google::LogMessage(_file.c_str(), _line, _severity).stream() << data.c_str();
	}
}

void Log::Flush(LogLevel level) {
	google::FlushLogFiles(level);
}

} //namespace uc