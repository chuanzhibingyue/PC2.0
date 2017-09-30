#ifndef IMAGE_UTIL_H_
#define IMAGE_UTIL_H_

#include <string>

namespace cefIntegration {

class ImageUtil {
public:
	static int Crop(const std::wstring &imagePath, int x, int y, int width, int height, int targetWidth, int targetHeight, const std::wstring &savePath);
};

}

#endif