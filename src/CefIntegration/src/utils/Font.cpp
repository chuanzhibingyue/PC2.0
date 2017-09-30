#include "Font.h"
#include <Windows.h>
#include <algorithm>

namespace cefIntegration {

bool comp(std::wstring& left, std::wstring& right){
	if ((L'@' != left[0]) && (L'@' == right[0]))
		return true;

	if ((L'@' == left[0]) && (L'@' != right[0]))
		return false;

	return left.compare(right) < 0;
}

int CALLBACK EnumerateFontsCallBack(ENUMLOGFONTEX *lpelf,
	NEWTEXTMETRICEX *lpntm, DWORD fontType, LPARAM lParam) {
	std::vector<std::wstring> *fonts = (std::vector<std::wstring>*)lParam;
	fonts->push_back(lpelf->elfLogFont.lfFaceName);

	return TRUE;
}

void Font::EnumerateFonts(std::vector<std::wstring> &fonts) {
	LOGFONT		lf;
	lf.lfFaceName[0] = L'\0';
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfPitchAndFamily = 0;

	HDC screenDc = ::GetDC(NULL);

	::EnumFontFamiliesEx(screenDc, &lf, (FONTENUMPROC)EnumerateFontsCallBack,
		(LPARAM)&fonts, 0);
	std::sort(fonts.begin(), fonts.end(), comp);

	fonts.erase(std::unique(fonts.begin(), fonts.end()), fonts.end());

	::ReleaseDC(NULL, screenDc);
}

}