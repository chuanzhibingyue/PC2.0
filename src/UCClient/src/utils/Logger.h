#include <string>

namespace ucclient {

class Logger {
public:
	static void WriteLog(const std::wstring &fileName, int line, int logLevel, const std::wstring &content);
};

}
