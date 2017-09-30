#ifndef FONT_H_
#define FONT_H_

#include <vector>
#include <string>

namespace cefIntegration {

class Font {
public:
	static void EnumerateFonts(std::vector<std::wstring> &fonts);
};

}

#endif