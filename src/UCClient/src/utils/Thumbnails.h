#ifndef THUMBNAILS_UTIL_H_
#define THUMBNAILS_UTIL_H_

#include <string>

namespace ucclient {

class ThumbnailsUtil {
public:
	/**
	 * 创建缩略图
	 * @param path	原图片路径
	 * @param thumbnailPath 缩略图生成路径
	 * @param base64data 缩略图base64编码后数据
	 * @return 
	 */
	static bool  CreateThumbnails(const std::wstring& path, const std::wstring &thumbnailPath, std::string& base64Data);

	/**
	 * 生成缩略图存放路径
	 * @param imgPath	原图片路径
	 * @return 缩略图路径
	 */
	static std::wstring MakeThumbnailFileName(const std::wstring& srcPath);

	/**
	 * 生成缩略图base编码
	 * @param filePath		图片路径
	 * @param [out]base64	base64编码
	 */
	static bool GetFileBase64Data(const std::wstring& filePath, std::string& base64);

private:
	static std::wstring GetImageType(const std::wstring& path);

};

}
#endif