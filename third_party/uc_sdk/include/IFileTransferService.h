/*
 * IFileTransferService.h
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#ifndef IFILETRANSFERSERVICE_H_
#define IFILETRANSFERSERVICE_H_

#include "UCSDK.h"
#include "IUCEngine.h"
#include "base/HTTPRequestEvent.h"

namespace uc {
	interface UC_API IFileTransferServiceEvent;
	interface UC_API IFileTransferService: public IUCService, Event<IFileTransferServiceEvent> {
		virtual ~IFileTransferService(){};

		/**
		* 上传文件
		* @param filePath			上传文件的本地全路径
		* @param [out]taskId		本次上传任务ID
		* @param convertType		转换格式，0--不转换，1--缩略图，2--mp4，默认不转换
		* @param convertWidth		转换缩略图时横向大小，单位为象素，convertType为1时才生效，默认为200
		* @param convertHeight		转换缩略图时纵向大小，单位为象素，convertType为1时才生效，默认为200
		* @return					成功返回0，否则返回错误码
		*/
		virtual int32_t UploadFile(
			const std::string& filePath,
			OUT int32_t& taskId,
			int32_t convertType = 0,
			int32_t convertWidth = 200,
			int32_t convertHeight = 200
			) = 0;

		/**
		* 文件是否正在上传
		* @param taskId					上传任务ID
		* @param [out] isUploading		是否上传中 0--否，1--是
		* @return						成功返回0，否则返回错误码
		*/
		virtual int32_t IsFileUploading(int32_t taskId, OUT int32_t& isUploading) = 0;

		/**
		* 取消上传
		* @param taskId				上传任务ID
		* @return					成功返回0，否则返回错误码
		*/
		virtual int32_t CancelUploadFile(int32_t taskId)= 0;

		/**
		* 下载文件
		* @param downloadURL		文件下载URL
		* @param savePath			本地保存全路径
		* @param [out]taskId		本次下载任务ID
		* @param safeDwon			是否安全下载
		* @param cvtWidth			转换宽度
		* @param cvtHeight			转换高度
		*/
		virtual int32_t DownloadFile(
			const std::string& downloadURL,
			const std::string& savePath,
			OUT int32_t& taskId,
			int32_t safeDown = 0,
			int32_t cvtWidth = 0,
			int32_t cvtHeight = 0
			) = 0;

		/**
		* 文件是否正在下载
		* @param taskId					下载任务ID
		* @param [out]isDownloading		是否下载中 0--否，1--是
		* @return						成功返回0，否则返回错误码
		*/
		virtual int32_t IsFileDownloading(int32_t taskId, OUT int32_t& isDownloading) = 0;

		/**
		* 取消下载
		* @param taskId				下载任务ID
		* @return					成功返回0，否则返回错误码
		*/
		virtual int32_t CancelDownloadFile(int32_t taskId) = 0;

		/**
		* fid转downloadUrl
		* @param fid				fid
		* @param downloadUrl		下载Url
		* @return					成功返回0，否则返回错误码
		*/
		virtual int32_t GetDownloadUrlFromFid(const std::string &fid, std::string &downloadUrl) = 0;

		/*
		* 删除FS服务器上的文件
		* @param fid				fid或下载链接
		* @return					成功返回0，否则返回错误码
		*/
		virtual int32_t DeleteFileFromFS(const std::string &fid) = 0;

		/*
		* 重启失败的任务
		* 之前失败的任务ID
		* @return					成功返回0，否则返回错误码
		*/
		virtual int32_t ResumeTask(int32_t taskId) = 0;

		/**
		 * 获取文件本地路径
		 * @param fileURL	文件URL
		 * @param filePath 	本地文件路径
		 * @return 成功返回0，否则返回错误码
		 */
		virtual int32_t GetLocalFilePath(const std::string &fileURL, std::string &filePath) = 0;

		/**
		 * 更新文件本地路径信息
		 * @param fileURL	文件URL
		 * @param filePath	本地文件路径
		 * @return 成功返回0，否则返回错误码
		 */
		virtual int32_t UpdateLocalFilePath(const std::string &fileURL, const std::string &filePath) = 0;
	};

	interface UC_API IFileTransferServiceEvent {
		virtual ~IFileTransferServiceEvent(){};

		/**
		* 文件上传进度的返回事件
		* @param errorCode				错误码
		* @param taskId					上传任务ID
		* @param percent				上传进度，取值0--100
		*/
		virtual void OnFileUploading(
			int32_t errorCode, 
			int32_t taskId, 
			int32_t percent = 0
			) = 0;

		/**
		* 文件上传成功的返回事件
		* @param errorCode				错误码
		* @param taskId					上传任务ID
		* @param fileName				文件名称
		* @param fileSize				文件大小
		* @param operatorId				操作者ID
		* @param operatorTime			操作时间
		* @param fid					文件ID
		* @param downloadURL			文件的下载链接地址
		* @param cvtFid					文件转换后的文件ID
		* @param convertDownloadURL		文件转换后的下载链接地址
		*/
		virtual void OnFileUploaded(
			int32_t errorCode,
			int32_t taskId,
			const std::string& fileName,
			int64_t fileSize,
			int32_t operatorId,
			int64_t opreatorTime,
			const std::string& fid,
			const std::string& downloadURL, 
			const std::string& cvtFid = "",
			const std::string& convertDownloadURL = ""
			) = 0;
		
		/**
		* 文件下载结果的返回事件
		* @param errorCode				错误码
		* @param taskId					下载任务ID
		* @param percent				下载进度，取值0--100
		*/
		virtual void OnFileDownloading(
			int32_t errorCode, 
			int32_t taskId, 
			int32_t percent = 0
			) = 0;

		/**
		* 文件下载结果的返回事件
		* @param errorCode				错误码
		* @param taskId					下载任务ID
		* @param savePath				文件下载成功后本地保存路径
		*/
		virtual void OnFileDownloaded(
			int32_t errorCode, 
			int32_t taskId, 
			const std::string& savePath
			) = 0;

		/**
		* 文件删除结果的返回事件
		* @param errorCode				错误码
		* @param fid					fid或downloadUrl
		*/
		virtual void OnFileDeleted(
			int32_t errorCode,
			const std::string& fid
			) = 0;
	};
}

#endif /* IFILETRANSFERSERVICE_H_ */
