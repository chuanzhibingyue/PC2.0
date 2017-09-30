#include "ZipWrapper.h"
#include "zlib.h"
#include "minizip/zip.h"
#include <string>
#include "common/platform.h"

namespace uc {

int ZipWrapper::Compress(unsigned char *source, unsigned long sourcelength, unsigned char *dest, unsigned long *destlength) {
	int err = compress2(dest, destlength, source, sourcelength, Z_BEST_COMPRESSION);
	return err;

	if (err != Z_OK) {
		return -1;
	}
	return 0;
}

int ZipWrapper::Decompress(unsigned char *zdata, unsigned long nzdata, unsigned char *data, unsigned long *ndata) {
	int err = 0;
	z_stream d_stream = { 0 }; /* decompression stream */
	static char dummy_head[2] =
	{
		0x8 + 0x7 * 0x10,
		(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
	};
	d_stream.zalloc = (alloc_func)0;
	d_stream.zfree = (free_func)0;
	d_stream.opaque = (voidpf)0;
	d_stream.next_in = zdata;
	d_stream.avail_in = 0;
	d_stream.next_out = data;
	if (inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) return -1;
	while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
		d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
		if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
		if (err != Z_OK)
		{
			if (err == Z_DATA_ERROR)
			{
				d_stream.next_in = (Bytef*)dummy_head;
				d_stream.avail_in = sizeof(dummy_head);
				if ((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK)
				{
					return -2;
				}
			}
			else return -1;
		}
	}
	if (inflateEnd(&d_stream) != Z_OK) return -1;
	*ndata = d_stream.total_out;
	return 0;
}

bool ZipWrapper::ZipFile(const std::string &destFile, std::vector<std::string> &sourceFiles) {
	
	zipFile zipFl = zipOpen(destFile.c_str(), APPEND_STATUS_CREATE);
	if (zipFl == NULL) {
		return false;
	}

	zip_fileinfo fileInfo;
    memset(&fileInfo, 0, sizeof(zip_fileinfo));

	int32_t bufSize = 16384;
	int32_t readSize = 0;
	char *buf = new char[bufSize];

	memset(buf, 0, bufSize);

	std::string sourceFile = "";
	std::string fileName = "";

	for (std::vector<std::string>::size_type index = 0; index < sourceFiles.size(); index++) {
		sourceFile = sourceFiles[index];
		fileName = ZipWrapper::GetFileName(sourceFile);
		if (zipOpenNewFileInZip(zipFl, fileName.c_str(), &fileInfo, NULL, 0, NULL, 0, NULL, Z_DEFLATED, 5) != ZIP_OK) {
			delete[]buf;
			zipClose(zipFl, NULL);
			return false;
		}

		FILE *fl = fopen(sourceFile.c_str(), "rb");
		if (fl == NULL) {
			delete[]buf;
			zipCloseFileInZip(zipFl);
			zipClose(zipFl, NULL);
			return false;
		}

		readSize = fread(buf, 1, bufSize, fl);

		while (readSize > 0) {
			if (zipWriteInFileInZip(zipFl, buf, readSize) != ZIP_OK) {
				delete[]buf;
				zipCloseFileInZip(zipFl);
				zipClose(zipFl, NULL);
				return false;
			}
			memset(buf, 0, bufSize);
			readSize = fread(buf, 1, bufSize, fl);
		}

		fclose(fl);

		zipCloseFileInZip(zipFl);
	}

	delete[]buf;
	zipClose(zipFl, NULL);
	return 0;
}

std::string ZipWrapper::GetFileName(const std::string &filePath) {
#if defined(PLATFORM_WIN)
	char drive[_MAX_DRIVE] = { 0 };
	char dir[_MAX_DIR] = { 0 };
	char fname[_MAX_FNAME] = { 0 };
	char ext[_MAX_EXT] = { 0 };
    
    _splitpath(filePath.c_str(), drive, dir, fname, ext);
    std::string strFileName = "";
    
    strFileName.append(fname);
    strFileName.append(ext);
    
    return strFileName;
#else
    std::string::size_type pos = filePath.rfind("/");
    return filePath.substr(pos + 1, filePath.length() - pos);
#endif
}

}