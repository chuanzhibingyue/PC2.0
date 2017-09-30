#ifndef FILEHELP_H_
#define FILEHELP_H_

#include <map>
#include "common/platform.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "common/StringConvert.h"

namespace fs = boost::filesystem;
using boost::filesystem::path;
namespace uc
{
	namespace filehelp
	{
		inline bool create_directories(const std::string &filePath) {
			if (filePath.length()<3) {
				return false;
			}
			const std::wstring unicodeFilePath = StringConvert::FromUTF8Str(filePath);
			fs::path p(unicodeFilePath);
			fs::path directory = p.parent_path();
			if (!fs::is_directory(directory))
			{
				fs::create_directories(directory);
				if (!fs::is_directory(directory)) {
					return false;
				}
			}
			return true;
		}

		inline bool exists(const std::string &filePath) {
			const std::wstring unicodeFilePath = StringConvert::FromUTF8Str(filePath);
			return fs::exists(unicodeFilePath);
		}

		inline int64_t file_size(const std::string &filePath) {
			const std::wstring unicodeFilePath = StringConvert::FromUTF8Str(filePath);
			return fs::file_size(unicodeFilePath);
		}

		inline void copy_file(const std::string &from, const std::string &to) {
			create_directories(to);
			const std::wstring unicodefrom = StringConvert::FromUTF8Str(from);
			const std::wstring unicodeto = StringConvert::FromUTF8Str(to);
			return fs::copy_file(unicodefrom, unicodeto, fs::copy_option::overwrite_if_exists);
		}

		inline void rename(const std::string &from, const std::string &to) {
			const std::wstring unicodefrom = StringConvert::FromUTF8Str(from);
			const std::wstring unicodeto = StringConvert::FromUTF8Str(to);
			return fs::rename(unicodefrom, unicodeto);
		}

		inline bool remove(const std::string &filePath) {
			const std::wstring unicodeFilePath = StringConvert::FromUTF8Str(filePath);
			return fs::remove(unicodeFilePath);
		}

		inline const string filename(const std::string &filePath) {
			const std::wstring unicodeFilePath = StringConvert::FromUTF8Str(filePath);
			fs::path p(unicodeFilePath);
			return StringConvert::ToUTF8Str(p.filename().wstring());
		}
	}
}

#endif // FILEHELP_H_