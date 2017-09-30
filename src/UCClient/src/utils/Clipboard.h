#ifndef CLIPBOARD_UTIL_H_
#define CLIPBOARD_UTIL_H_

#include <Windows.h>
#include <string>
#include <vector>

namespace ucclient {

class Clipboard {
public:
	Clipboard();
	~Clipboard();

	std::string getClipboardDataFormat(HWND wnd);
	std::vector<std::string> getClipboardData(HWND wnd);
	std::wstring getTempFilePath() const;
	bool setClipboardData(const std::string& filePath, HWND wnd);

private:
	std::vector<std::string> getTextData(unsigned int format);
	std::vector<std::string> getBitmapData();
	std::vector<std::string> getFileData();
};

}
#endif