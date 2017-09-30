#ifndef FILE_UTIL_H_
#define FILE_UTIL_H_

#include <string>

namespace ucclient {

class FileUtil {
public:
	static void ProcessPath(std::wstring& processPath);
	static std::string ProcessPath();

	static bool CreateFolder(const std::wstring& path);
	static bool CreateFolderRecursively(const std::wstring& path);
	static void RemoveFolder(const std::wstring& path);

	static std::wstring GetDocumentPath();
	static std::wstring GetAppDataPath();
	static std::string GetAppDataPathA();
};

}
#endif