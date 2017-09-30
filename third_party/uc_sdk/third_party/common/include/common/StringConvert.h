#ifndef STRINGCONVERT_H_
#define STRINGCONVERT_H_

#include <string>
#include <vector>

#include "common/platform.h"

namespace uc {

using namespace std;

class UC_API StringConvert {
public:
	static string Unicode2Ascii(wstring wstrsrc);
	static string ToUTF8Str(const wstring &data);
	static wstring FromUTF8Str(const string &utf8);

	static bool Base64Encode(const char *src, int32_t srcLen, std::string& dest);
	static bool Base64Decode(const char *src, int32_t srcLen, std::string& dest);

	static std::string URLDecode(const std::string &src);
	static std::string URLEncode(const std::string &c);

	static string JsonEscape(const std::string& input);

	static std::string& removeLineBreak(std::string& str);
	static std::wstring& removeLineBreak(std::wstring& str);

	static std::string& Replace(std::string& str, const std::string& old_value, const std::string& new_value);
	static std::wstring& Replace(std::wstring& str, const std::wstring& old_value, const std::wstring& new_value);

	static int64_t Convert2Int64(const string& value);
	static int WStringToInt(const std::wstring& s);
	static int64_t WStringToInt64(const std::wstring& s);
	static string Convert2String(int64_t value);
	static std::string Char2Hex(char dec);

	template<typename T> 
	static void Split(const T& s, const T& delim, std::vector<T>& ret) {
		if (s.size() > 0)
		{
			size_t last = 0;
			size_t index = s.find(delim, last);
			while (index != T::npos)
			{
				ret.push_back(s.substr(last, index - last));
				last = index + delim.size();
				index = s.find(delim, last);
			}
			if (index - last > 0)
			{
				ret.push_back(s.substr(last, index - last));
			}
		}
	}
};

#define ESCAPE_BLANK(str) (StringConvert::Replace(std::string(str), " ", ""))
#define UTF2W(str) StringConvert::FromUTF8Str((str))
#define UTF2WJSON(str) StringConvert::FromUTF8Str(StringConvert::JsonEscape((str)))
#define W2UTF(str) StringConvert::ToUTF8Str((str))
#define URLDATA_UTF2W(str) StringConvert::FromUTF8Str(StringConvert::URLDecode(ESCAPE_BLANK(str)))
#define URLDATA_UTF2WJSON(str) StringConvert::FromUTF8Str(StringConvert::JsonEscape(StringConvert::URLDecode(ESCAPE_BLANK(str))))
#define URLDATA_UTF2WTEXT(str) StringConvert::FromUTF8Str(StringConvert::URLDecode(ESCAPE_BLANK(str)))

} /* namespace uc */

#endif /* STRINGCONVERT_H_ */
