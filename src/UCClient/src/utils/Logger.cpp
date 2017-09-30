#include "Logger.h"
#include "common/StringConvert.h"
#include "LogManager.h"

namespace ucclient {

using namespace uc;

void Logger::WriteLog(const std::wstring &fileName, int line, int logLevel, const std::wstring &content) {
	std::string name = StringConvert::ToUTF8Str(fileName);
	uc::Log(name.c_str(), line, LogLevel(logLevel)).Stream() << StringConvert::ToUTF8Str(content);
}

}