#include "Thumbnails.h"
#include "common/StringConvert.h"
#include "cximage/ximage.h"
#include <sstream>
#include "IUCEngine.h"

#define THUMBNAILS_WIDTH	200		//缩略图最大宽度
#define THUMBNAILS_HEIGHT	150		//缩略图最大高度
#define THUMBNAILS_PREVN	L"thumb_"

namespace ucclient {

using namespace uc;

bool ThumbnailsUtil::CreateThumbnails(const std::wstring& path, const std::wstring &thumbnailPath, std::string& base64Data) {

	CxImage image;
	
	bool	result = false;
	if (L"gif" == GetImageType(path)){
		image.SetFrame(0);
		result = image.Load(path.c_str(), CXIMAGE_FORMAT_GIF);
	}
	else{
		result = image.Load(path.c_str());
	}
	
	if (!result) {
		UC_LOG(ERROR) << "CreateThumbnails load source image failed. Source image path: " << path;
		return false;
	}

	int orgWidth = image.GetWidth();
	int orgHeight = image.GetHeight();
	int newWidth = 0;
	int newHeight = 0;

	if ( orgWidth > orgHeight ) {
		//图片的宽大于高
		newWidth = orgWidth < THUMBNAILS_WIDTH ? orgWidth : THUMBNAILS_WIDTH;
		newHeight = orgHeight * newWidth/orgWidth;
	} else {
		//图片的高大于宽
		newHeight = orgHeight < THUMBNAILS_HEIGHT ? orgHeight : THUMBNAILS_HEIGHT;
		newWidth = orgWidth * newHeight/orgHeight;
	}
	
	image.Resample(newWidth, newHeight, 2);
	image.SetJpegQuality(75);
	std::wstring destPath = thumbnailPath + L"\\" + MakeThumbnailFileName(path) + L".png";
	DeleteFile(destPath.c_str());
	image.Save(destPath.c_str(), CXIMAGE_FORMAT_PNG);

	if (!GetFileBase64Data(destPath, base64Data)) {
		UC_LOG(ERROR)<< "CreateThumbnails get thumbnails base64 code failed.";
		return false;
	}

	return true;
}

std::wstring ThumbnailsUtil::MakeThumbnailFileName( const std::wstring& srcPath ) {
	std::wstring fileName = L"";
	int pos = srcPath.rfind(L"\\");
	if (pos != wstring::npos) {
		fileName = srcPath.substr(pos+1,srcPath.length()-pos-1);
		pos = fileName.find(L".");
		if (pos != wstring::npos){
			fileName = fileName.substr(0, pos);
		}
	}
	else{
		pos = srcPath.rfind(L"/");
		fileName = srcPath.substr(pos + 1, srcPath.length() - pos - 1);
		pos = fileName.find(L".");
		if (pos != wstring::npos){
			fileName = fileName.substr(0, pos);
		}
	}
	
	return THUMBNAILS_PREVN + fileName;
}

bool ThumbnailsUtil::GetFileBase64Data( const std::wstring& filePath, std::string& base64 ) {
	FILE* file = NULL;
	errno_t err = _wfopen_s(&file, filePath.c_str(), L"rb");
	if (err != 0) {
		UC_LOG(ERROR)<< "GetFileBase64Data open file failed. file path: " << filePath;
		return false;
	}

	fseek(file, 0, SEEK_END);
	int len = ftell(file);
	char* buffer = new char[len];
	memset(buffer, 0, len);
	fseek(file, 0, SEEK_SET);
	fread(buffer,sizeof(char),len, file);
	fclose(file);
	
	if(!StringConvert::Base64Encode(buffer, len, base64)){
		UC_LOG(ERROR)<< "GetFileBase64Data base64 encode failed.";
		delete [] buffer;
		buffer = NULL;
		return false;
	}

	delete [] buffer;
	buffer = NULL;

	return true;
}

std::wstring ThumbnailsUtil::GetImageType(const std::wstring& path){

	std::wstring imageType = L"";
	int pos = path.rfind(L".");
	if (pos != wstring::npos) {
		imageType = path.substr(pos + 1, path.length() - pos - 1);
	}
	std::transform(imageType.begin(), imageType.end(), imageType.begin(), tolower);
	return imageType;
}

}