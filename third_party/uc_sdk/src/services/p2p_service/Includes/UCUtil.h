#pragma once

#include "includes.h"
#include "UCTemplate.h"

// #include <boost/serialization/access.hpp>
// #include <boost/archive/binary_iarchive.hpp>
// #include <boost/archive/binary_oarchive.hpp>

namespace uc {

	// typedef std::pair<int32_t, int32_t> File_Key;
	class File_Key : public std::pair<int32_t, int32_t>
	{
	public:
		File_Key() : pair(0, 0){}
		File_Key(int32_t fileID, int32_t peerID) : pair(fileID, peerID){}

		inline int32_t getFileID(){ return first; }
		inline int32_t getPeerID() { return second; }
		inline void setFileID(int32_t n) { first = n; }
		inline void setPeerID(int32_t n) { second = n; }
	};
	// class File_Key{
	// public:
	// 	File_Key(){
	// 		_fileID = 0;
	// 		_peerID = 0;
	// 	}
	// 
	// 	File_Key(int32_t fileID, int32_t peerID){
	// 		_fileID = fileID;
	// 		_peerID = peerID;
	// 	}
	// 	
	// 	//拷贝构造函数  
	// 	File_Key(const File_Key &obj)  
	// 	{
	// 	}
	// 
	// public:
	// 	int32_t _fileID;
	// 	int32_t _peerID;
	// 
	// public:
	// 	bool operator == (const File_Key &obj) const { 
	// 
	// 		return (_fileID == obj._fileID && _peerID == obj._peerID);
	// 	}
	// 
	// 	bool operator < (const File_Key & cmp) const
	// 	{
	// 		return _fileID < cmp._fileID;
	// 	} 
	// };

	class BlockInfo {

	public:
		BlockInfo(){

			_id = 0;
			//_srFS = false;

			_offset = 0;
		}

		BlockInfo(const BlockInfo &obj){

			_id = obj._id;
			//_srFS = obj._srFS;

			_offset = obj._offset;
		}

	public:
		int _id;	// 块ID
		//int _srFS;	// 是否和FS连接 // sr = send/recv

		int32_t _offset;// 本块已经处理大小

// 	private:
// 		friend class boost::serialization::access;  
// 
// 		template<class Archive>  
// 		void serialize(Archive& ar, const unsigned int version)  
// 		{  
// 			ar & _id;
// 			ar & _srFS;
// 			ar & _offset;
// 		}
	};

	typedef LckList<BlockInfo *> LSTportionBLOCK; // 一部分
	typedef LckList<int> LSTwholeBLOCK; // 完整的

	enum Sink_Type {
		ST_CIMSCONN,
		ST_FILEMGR,
		ST_FILETRANSFER,
		ST_FSCONN,
		ST_NETWORK,
		ST_PEERCONN,
	};

	class FT_OperateTask
	{
	public: 
		FT_OperateTask(int32_t fileID = 0, int32_t blockSum = 1, int32_t blockSize = 0, int32_t extraBlockSize = 0){

			_fileID = fileID;
			_blockSum = blockSum; // 分块总数(包括额外块)
			_blockSize = blockSize;
			_extraBlockSize = extraBlockSize; // 额外块大小 = filesize % _blockSize
		}

		int32_t getFileOffset(int32_t blockID) { return blockID * _blockSize; }
		bool isOwn(int32_t fileID) { return (fileID == _fileID); }

		int addBlockInfo(BlockInfo &bi){
			
			BlockInfo *lpT = new BlockInfo();
			lpT->_id = bi._id;
			m_lstPB.push_back(lpT);
			return 0;
		}

		int updateBlockInfo(int32_t blockID, int32_t offset, int32_t len){

			BlockInfo *lpB = getPBlockInfo(blockID);
			if (NULL == lpB)
			{ // 块ID不存在,选择新块下载
				return P2P_ERROR_4;
			}

			if (lpB->_offset != offset) { // 数据异常
				return P2P_ERROR_5;
			}

			lpB->_offset += len;
			if (((0 == lpB->_id) && (0 != _extraBlockSize) && (lpB->_offset == _extraBlockSize)) || 
				lpB->_offset == _blockSize)
			{ // 当前块完成.选择新块下载
				remotePBlockInfo(blockID);
				addWBlock(blockID);

				return P2P_ERROR_4;
			}
			// 继续下载当前块
			return P2P_ERROR_SUCCESSED;
		}

	protected:
		void addWBlock(int id){
			m_lstWB.push_back(id);
		}

		BlockInfo *getPBlockInfo(int id) { 

			LSTportionBLOCK::iterator it;
			for( it = m_lstPB.begin(); it != m_lstPB.end(); ++it)  
			{  
				BlockInfo *lpT = *it;
				if (lpT->_id == id)
				{
					return lpT;
				}
			}

			return (BlockInfo *)NULL; 
		}

		void remotePBlockInfo(int id){

			LSTportionBLOCK::iterator it;
			it = m_lstPB.begin();
			while (it != m_lstPB.end())
			{
				BlockInfo *lpT = *it;
				if (lpT->_id == id)
				{
					delete lpT;
					lpT = NULL;

					it = m_lstPB.erase(it);
					continue;
				}

				++it;
			}
		}

	private:
		int32_t _fileID; // 文件ID
		int32_t _blockSum; // 分块总数(包括额外块)
		int32_t _blockSize; // 平均每块大小
		int32_t _extraBlockSize; // 额外块大小 = filesize % _blockSize

		// 拥有完整块信息.
		LSTwholeBLOCK m_lstWB;
		// 正在传输的块信息
		LSTportionBLOCK m_lstPB;
	};
	typedef LckList<FT_OperateTask *> LSTOTASK;

}