#include "P2P_FileTask.h"

#define FT_RATE 1000
namespace uc {

	P2P_FileTask::P2P_FileTask(File_Key &fk) {

		m_FKey = fk;

		_isUpload = false;
		_progress = -1;
		_size = 0;
		_currSize = 0;
	}

	P2P_FileTask::~P2P_FileTask(void) {

		if (!_fstream.is_open()) {
			if (!_isUpload) {
				_fstream.flush();
			}
			_fstream.close();
		}
	}

	int P2P_FileTask::initUploadTask(std::wstring path, int32_t currSize) {

		_isUpload = true;
		_path = path;
#ifdef _USE_FILE
		if (!fs::exists(_path)) {
			return P2P_ERROR_3;
		}

		fs::path p(_path);
		_name = p.filename().wstring();
		_size = (int32_t)fs::file_size(p);

		_fstream.open(p, std::ios::in | std::ios::binary);

		if (!_fstream.is_open()) {
			return P2P_ERROR_4;
		}
#else
		_name = L"test.text";
		_size = 68452;
#endif // _USE_FILE
		_currSize = currSize;

		return P2P_ERROR_SUCCESSED;
	}

	int P2P_FileTask::initDownLoadTask(std::wstring path, int32_t fileSize) {

		_isUpload = false;
		_path = path;

		_cachPath = path + L".std";

#ifdef _USE_FILE
		_size = fileSize;

		fs::path p(_path);
		_name = p.filename().wstring();

		if (fs::exists(_cachPath)) {
			_currSize = (int32_t)fs::file_size(_cachPath);
		} else {
			fs::create_directories(p.parent_path());
		}

		_fstream.open(_cachPath, std::ios::out | std::ios::app | std::ios::binary);

		if (!_fstream.is_open()){
			return P2P_ERROR_4;
		}
#else
		_size = 68452;
		_name = L"test.txt";
		_currSize = 0;
#endif // _USE_FILE

		return P2P_ERROR_SUCCESSED;
	}

	// return read file size
	int P2P_FileTask::read(int32_t offset, int32_t len, char* &buf) { 

		if (offset >= _size || NULL == buf) {
			return -1;
		}

		if ((offset + len) > _size) {
			len = _size - offset;
		}
#ifdef _USE_FILE
		_fstream.seekg(offset);
		_fstream.read(buf, len);

		int count = _fstream.gcount();
		if (count != len) {
			buf = NULL;
			return -1;
		}
#endif // _USE_FILE

		_currSize += len;

		return len;
	}

	int P2P_FileTask::write(int32_t offset, int32_t len, char *buf) {

		if ((offset + len) > _size) {
			return -1;
		}
		// seekp：设置输出文件流的文件流指针位置
		// seekg：设置输入文件流的文件流指针位置
#ifdef _USE_FILE
		_fstream.seekp(offset);
		_fstream.write(buf, len);
#endif // _USE_FILE

		_currSize += len;

		return len;
	}

	int P2P_FileTask::makeProgress() {

		if (_size <= 0) {
			return FT_RATE;
		}

		int nProgress = (int)(_currSize * FT_RATE / _size);
		if (nProgress > _progress) {
			_progress = nProgress;
			return _progress;
		}

		return -1;
	}

}