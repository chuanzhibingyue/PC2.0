#include "ImageUtil.h"
#include "FileUtil.h"
#include "cximage/ximage.h"
#include "ClientDefs.h"

namespace cefIntegration {

int ImageUtil::Crop(const std::wstring &imagePath, int x, int y, int width, int height, int targetWidth, int targetHeight, const std::wstring &savePath) {
	CxImage image;
	image.Load(imagePath.c_str());

	CxImage saveImage;
	if (!image.Crop(x, y, x + width, y + height, &saveImage)) {
		return CEF_CROP_IMAGE_ERROR;
	}

	if ((width != targetWidth) || (height != targetHeight)) {
		if (!saveImage.Resample(targetWidth, targetHeight, 2)) {
			return CEF_CROP_IMAGE_ERROR;
		}
	}

	std::wstring directory;
	size_t lastSlashIndex = savePath.find_last_of(L"/\\");
	if (std::string::npos != lastSlashIndex)
	{
		directory = savePath.substr(0, lastSlashIndex);
	}
	FileUtil::CreateFolder(directory);

	if (!saveImage.Save(savePath.c_str(), saveImage.GetType())){
		return CEF_SAVE_CROPED_IMAGE_ERROR;
	}
	
	return CEF_SUCCESS;
}

}