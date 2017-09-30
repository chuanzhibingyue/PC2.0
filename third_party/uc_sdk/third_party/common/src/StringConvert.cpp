/*
 * StringConvert.cpp
 *
 *  Created on: 2015年5月22日
 *      Author: jianxue
 */

#include "common/StringConvert.h"
#include "common/platform.h"
#include <sstream>
#include <codecvt>
#include "modp_base64/modp_b64.h"

namespace uc {


template <typename T>
std::basic_string<T>& str_replace(std::basic_string<T>& str, const std::basic_string<T>& old_value, const std::basic_string<T>& new_value) {
	for (typename std::basic_string<T>::size_type pos( 0 ); pos != std::basic_string<T>::npos; pos += new_value.length()) {
		if ( ( pos = str.find( old_value, pos ) ) != std::basic_string<T>::npos ) {  
			str.replace(pos, old_value.length(), new_value);   
		} 
		else { 
			break;   
		}
	}   
	return str;
} 

string StringConvert::Unicode2Ascii(wstring wstrsrc)
{
#if defined(PLATFORM_WIN)
	int nLength = ::WideCharToMultiByte(CP_OEMCP, 0, wstrsrc.c_str(), -1, NULL, 0, NULL, NULL);
	if (nLength <= 0) return string("");
	char *szbuffer = new char[nLength + 2];
	memset(szbuffer, 0, nLength + 2);
	int len = ::WideCharToMultiByte(CP_OEMCP, 0, wstrsrc.c_str(), -1, szbuffer, nLength, NULL, NULL);
	if (len > 0) {
		szbuffer[len] = 0;
	}
	string strnew = szbuffer;
	delete[] szbuffer;
	return strnew;
#else
    setlocale (LC_CTYPE,"");
    std::size_t iLen =wcstombs(NULL,wstrsrc.c_str(),0);
    char* str = new char[iLen + 1];
    memset(str, 0, sizeof(char) *(iLen+1));
    wcstombs(str,wstrsrc.c_str(),iLen+1);
    std::string ret(str);
    delete[] str;
    return ret;
#endif
}

string StringConvert::ToUTF8Str(const wstring &data) {
	if (data.empty()) {
		return "";
	}

	string utf8;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > convert;
	try {
		utf8 = convert.to_bytes(data);
	} catch(const std::range_error& ) {
		utf8 = convert.to_bytes(data.substr(0, convert.converted()));
	}

	return utf8;
}

wstring StringConvert::FromUTF8Str(const string &utf8) {
	if (utf8.empty()) {
		return L"";
	}

	wstring wdata;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > convert;
	try {
		wdata = convert.from_bytes(utf8);
	} catch(const std::range_error& ) {
		wdata = convert.from_bytes(utf8.substr(0, convert.converted()));
	}

	return wdata;
}

bool StringConvert::Base64Encode(const char *src, int32_t srcLen, std::string& dest) {
	if (src == NULL || srcLen <= 0) {
		return false;
	}

	int destLen = modp_b64_encode_len(srcLen);
	char* sz_dest = new char[destLen+1];
	memset(sz_dest, 0, destLen+1);

	destLen = modp_b64_encode(sz_dest, src, srcLen);
	if (destLen < 0) {
		delete [](sz_dest);
		sz_dest = NULL;
		return false;
	}
	dest = std::string(sz_dest, destLen + 1);
	delete [](sz_dest);
	sz_dest = NULL;

	return true;
}

bool StringConvert::Base64Decode(const char *src, int32_t srcLen, std::string& dest) {
	if (src == NULL || srcLen <= 0) {
		return false;
	}

	int destLen = modp_b64_decode_len(srcLen);
	char* sz_dest = new char[destLen+1];
	memset(sz_dest, 0, destLen+1);

	destLen = modp_b64_decode(sz_dest, src, srcLen);
	if (destLen < 0)
	{
		delete [](sz_dest);
		sz_dest = NULL;
		return false;
	}
	dest = std::string(sz_dest, destLen + 1);
	delete [](sz_dest);
	sz_dest = NULL;

	return true;
}

int64_t StringConvert::Convert2Int64(const string& value) {
	stringstream streamValue;
	streamValue << value;
	int64_t result = 0;
	streamValue >> result;
	return result;
}

string StringConvert::Convert2String(int64_t value) {
	stringstream streamValue;
	streamValue << value;
	string result;
	streamValue >> result;
	return result;
}

string StringConvert::JsonEscape(const std::string& input) {
	std::ostringstream ss;
	for (auto iter = input.cbegin(); iter != input.cend(); iter++) {
		switch (*iter) {
			case '\\': ss << "\\\\"; break;
			case '"': ss << "\\\""; break;
			case '/': ss << "\\/"; break;
			case '\b': ss << "\\b"; break;
			case '\f': ss << "\\f"; break;
			case '\n': ss << "\\n"; break;
			case '\r': ss << "\\r"; break;
			case '\t': ss << "\\t"; break;
			//case '\'': ss << "\\'"; break;
			default: ss << *iter; break;
		}
	}
	return ss.str();
}

int StringConvert::WStringToInt(const std::wstring& s)
{
	int i;
	std::wistringstream(s) >> i;
	return i;
}

int64_t StringConvert::WStringToInt64( const std::wstring& s )
{
	int64_t l;
	std::wistringstream(s) >> l;
	return l;
}

std::string StringConvert::URLDecode(const std::string &src) {
	std::string result;
	char tmp[3];
	for (unsigned int i = 0; i < src.length(); ++i) {
		if (src[i] == '%' && i + 2 < src.length()) {
			tmp[0] = src[++i];
			tmp[1] = src[++i];
			tmp[2] = '\0';

			result += static_cast<char>(strtol(tmp, 0, 16));
		}
		else if (src[i] == '+') {
			result += ' ';
		}
		else {
			result += src[i];
		}
	}

	return result;
}

std::string StringConvert::URLEncode(const std::string &c) {
	std::string escaped;
	int max = c.length();
	
	for(int i=0; i<max; i++)
	{
		if ( (48 <= c[i] && c[i] <= 57) ||//0-9
			(65 <= c[i] && c[i] <= 90) ||//ABC...XYZ
			(97 <= c[i] && c[i] <= 122) || //abc...xyz
			(c[i]=='~' || c[i]=='-' || c[i]=='_' || c[i]=='.')
			)
		{
			escaped.append( &c[i], 1);
		}
		else
		{
			escaped.append("%");
			escaped.append(Char2Hex(c[i]));//converts char 255 to string "FF"
		}
	}
	return escaped;
}

std::string StringConvert::Char2Hex(char dec) {
	char dig1 = (dec&0xF0)>>4;
	char dig2 = (dec&0x0F);
	if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48 in ascii
	if (10<= dig1 && dig1<=15) dig1+=65-10; //A,65 in ascii
	if ( 0<= dig2 && dig2<= 9) dig2+=48;
	if (10<= dig2 && dig2<=15) dig2+=65-10;

	std::string r;
	r.append( &dig1, 1);
	r.append( &dig2, 1);
	return r;
}

std::string& StringConvert::Replace(std::string& str, const std::string& old_value, const std::string& new_value) {
	return str_replace(str, old_value, new_value);
}

std::wstring& StringConvert::Replace(std::wstring& str, const std::wstring& old_value, const std::wstring& new_value) {
	return str_replace(str, old_value, new_value);
}

std::string& StringConvert::removeLineBreak(std::string& str) {
	int pos = str.find_first_of("\r\n", 0);
	while(pos != std::string::npos) {
		str.replace(pos, 1, "");
		pos = str.find_first_of("\r\n", pos);
	}

	return str;
}

std::wstring& StringConvert::removeLineBreak(std::wstring& str) {
	int pos = str.find_first_of(L"\r\n", 0);
	while(pos != std::wstring::npos) {
		str.replace(pos, 1, L"");
		pos = str.find_first_of(L"\r\n", pos);
	}

	return str;
}

} /* namespace uc */
