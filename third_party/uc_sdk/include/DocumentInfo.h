/*
 * DocunmentInfo.h
 *
 *  Created on: May 20, 2015
 *      Author: yongshuai cheng
 */

#ifndef  DOCUMENT_H_
#define  DOCUMENT_H_

#include "UCSDK.h"
#include <string>


namespace uc{

class UC_API DocumentInfo
{
public:
	DocumentInfo(void);
	~DocumentInfo(void);

	int32_t GetCommentCount() const {
		return _commentCount;
	}

	void SetCommentCount(int32_t commentCount) {
		_commentCount = commentCount;
	}

	int32_t GetContentType() const {
		return _contentType;
	}

	void SetContentType(int32_t contentType) {
		_contentType = contentType;
	}

	int64_t GetCreateTime() const {
		return _createTime;
	}

	void SetCreateTime(int64_t createTime) {
		_createTime = createTime;
	}

	const std::string& GetDescription() const {
		return _description;
	}

	void SetDescription(const std::string& description) {
		_description = description;
	}

	const std::string& GetFileFormat() const {
		return _fileFormat;
	}

	void SetFileFormat(const std::string& fileFormat) {
		_fileFormat = fileFormat;
	}

	int64_t GetFileId() const {
		return _fileId;
	}

	void SetFileId(int64_t fileId) {
		_fileId = fileId;
	}

	const std::string& GetFileName() const {
		return _fileName;
	}

	void SetFileName(const std::string& fileName) {
		_fileName = fileName;
	}

	int64_t GetFileSize() const {
		return _fileSize;
	}

	void SetFileSize(int64_t fileSize) {
		_fileSize = fileSize;
	}

	const std::string& GetFileUrl() const {
		return _fileUrl;
	}

	void SetFileUrl(const std::string& fileUrl) {
		_fileUrl = fileUrl;
	}

	const std::string& GetPreview() const {
		return _preview;
	}

	void SetPreview(const std::string& preview) {
		_preview = preview;
	}
	
	int64_t GetModifyTime() const {
		return _modifyTime;
	}

	void SetModifyTime(int64_t modifyTime) {
		_modifyTime = modifyTime;
	}

	std::string GetThumbnail() const {
		return _thumbnail;
	}

	void SetThumbnail(std::string thumbnail) {
		_thumbnail = thumbnail;
	}

	int32_t GetTotal() const {
		return _total;
	}

	void SetTotal(int32_t total) {
		_total = total;
	}

	int32_t GetTotalLines() const {
		return _totalLines;
	}

	void SetTotalLines(int32_t totalLines) {
		_totalLines = totalLines;
	}

	int64_t GetUploadTime() const {
		return _uploadTime;
	}

	void SetUploadTime(int64_t uploadTime) {
		_uploadTime = uploadTime;
	}

	int32_t GetCreatorId() const {
		return _creatorId;
	}

	void SetCreatorId(int32_t userId) {
		_creatorId = userId;
	}

	const std::string& GetCreatorName() const {
		return _creatorName;
	}

	void SetCreatorName(const std::string& userName) {
		_creatorName = userName;
	} 

	int32_t GetMsgId() const {
		return _msgId;
	}

	void SetMsgId(int32_t msgId) {
		_msgId = msgId;
	}

	DocumentCloudType GetCloudType() const {
		return _cloudType;
	}

	void SetCloudType(DocumentCloudType cloudType) {
		_cloudType = cloudType;
	}

	const std::string& GetHashCode() const {
		return _hashCode;
	}

	void SetHashCode(const std::string& hashCode) {
		_hashCode = hashCode;
	}

	int32_t GetMountId() const {
		return _mountId;
	}

	void SetMountId(int32_t mountId) {
		_mountId = mountId;
	}

	int16_t GetDetailType() const {
		return _detailType;
	}

	void SetDetailType(int16_t detailType) {
		_detailType = detailType;
	}

	const std::string& GetDetailContent() const {
		return _detailContent;
	}

	void SetDetailContent(const std::string& detailContent) {
		_detailContent = detailContent;
	}

	const int64_t GetMessageSeq() const{
		return _msgSeq;
	}

	void SetMessageSeq(int64_t msgSeq){
		_msgSeq = msgSeq;
	}
	
	int64_t GetEventId() const {
		return _eventId;
	}

	void SetEventId(int64_t id) {
		_eventId = id;
	}

	const std::string& GetFileLocalPath() const {
		return _localpath;
	}

	void SetFileLocalPath(const std::string& localpath) {
		_localpath = localpath;
	}

private:
	int32_t					_msgId;			 //message id;
	int64_t					_fileId;         //文档ID;
	int32_t					_creatorId;      //文档创建者ID;
	std::string				 _creatorName;    //文档创建者名称;
	std::string				 _fileName;       //文档名称;
	std::string				_fileFormat;     //文档格式;
	std::string				 _fileUrl;        //文档下载URL;
	std::string				 _preview;       //文档预览地址;
	int32_t					_total;          //文档总数;
	int64_t					_uploadTime;     //文档上传时间;
	int64_t					_createTime;     //文档保存时间;
	int64_t					_modifyTime;     //文档更新时间;
	int64_t					_fileSize;       //文档大小;
	int32_t					_contentType;    //文档类型：0:文档/1:代码;
	int32_t					_commentCount;   //评论总数;
	int32_t					_totalLines;     //代码行数;
	std::string				_thumbnail;      //缩略图;
	std::string				_description;    //描述信息;
	DocumentCloudType		_cloudType;		 //云盘类型
	std::string				_hashCode;       //第三方文件唯一标识
	int32_t					_mountId;		 //第三方文件库id
	int16_t					_detailType;	 //详细内容格式 1-JSON(够快云库), URL(安贞医院)
	std::string				_detailContent;	 //内容
	int64_t					_eventId;		//会议事件ID（只针对会议文件使用）
	std::string				_localpath;      //文件存储的地址
	int64_t                 _msgSeq;         //消息的SEQ
};

class UC_API CommentInfo{
public:
	CommentInfo(void);
	~CommentInfo(void);

	int64_t GetFileId() const {
		return _fileId;
	}
	void SetFileId(int64_t fileId) {
		_fileId = fileId;
	}

	const std::string& GetFileTitle() const {
		return _fileTitle;
	}
	void SetFileTitle(const std::string& fileTitle) {
		_fileTitle = fileTitle;
	} 

	int32_t GetFileType() const {
		return _fileType;
	}
	void SetFileType(int32_t fileType) {
		_fileType = fileType;
	}

	int64_t GetCommentId() const {
		return _commentId;
	}
	void SetCommentId(int64_t commentId) {
		_commentId = commentId;
	}

	int32_t GetUserId() const {
		return _userId;
	}
	void SetUserId(int32_t userId) {
		_userId = userId;
	}

	const std::string& GetDisplayName() const {
		return _userName;
	}
	void SetUserName(const std::string& userName) {
		_userName = userName;
	} 

	int64_t GetCreateTime() const {
		return _createTime;
	}
	void SetCreateTime(int64_t createTime) {
		_createTime = createTime;
	}

	const std::string& GetContent() const {
		return _content;
	}
	void SetContent(const std::string& content) {
		_content = content;
	} 

	DocumentCloudType GetCloudType() const {
		return _cloudType;
	}
	void SetCloudType(DocumentCloudType cloudType) {
		_cloudType = cloudType;
	}

	int32_t GetMsgId() const {
		return _msgId;
	}

	void SetMsgId(int32_t msgId) {
		_msgId = msgId;
	}

private:
	int64_t				_fileId;         //文档或代码ID;
	std::string			_fileTitle;      //文档或代码标题;
	int32_t				_fileType;       //文档类型：0:文档/1:代码;
	int64_t				_commentId;      //评论ID;
	int32_t				_userId;         //评论者ID
	std::string			_userName;       //评论者名称;
	int64_t				_createTime;     //创建时间;
	std::string			_content;        //评论内容;
	DocumentCloudType	_cloudType;		 //云盘类型
	int32_t				_msgId;			 //本地消息id
};

}        /* namespace uc */
#endif   /* DOCUMENT_H_*/
