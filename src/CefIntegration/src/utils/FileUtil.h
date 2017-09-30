#ifndef FILE_UTIL_H_
#define FILE_UTIL_H_

#include <string>
#include <stdint.h>
#include <vector>

namespace cefIntegration {

class FileUtil {
public:
	static void ProcessPath(std::wstring& processPath);
	static std::string ProcessPath();

	static bool IsFolder(std::wstring& path);
	static int OpenFolder(const std::wstring& path);
	static bool CreateFolder(std::wstring& path);

	static bool FileExist(std::wstring& path);
	static int DeleteFileEx(std::wstring& path);

	static long long GetFileSize(const std::wstring& path);

	static int OpenFile(const std::wstring& path);
	
	static int SaveFile (const std::wstring& path, const std::string& content);
	static int CopyFileEX(const std::wstring& src, const std::wstring& dest);

	static void GetDirectoryContent(const std::wstring& dir, std::vector<std::wstring> &contents);

private:
	static std::wstring ConvertFileSeperator(const std::wstring& path);
	static int OpenFolderAndSelectFile(const std::wstring& path);
};

}
#endif