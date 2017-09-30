/*
 * UCFile.h
 *
 *  Created on: 2015-9-7
 *      Author: hualong.zhang
 */

#ifndef FSDEF_H_
#define FSDEF_H_

#define TANG_FILE_ID		"X-Tang-File-Id"
#define TANG_FILE_LENGTH	"X-Tang-File-Length"
#define TANG_FILE_OFFSET	"X-Tang-File-Offset"
#define TANG_RESULT			"X-Tang-Result"
#define TANG_ERROR_CODE		"X-Tang-Error-Code"
#define TANG_COOKIE			"Cookie"
#define CONTENT_LENGTH		"Content-Length"
#define FILE_LENGTH			"File-Length"
#define CONTENT_DISPOSITION	"Content-Disposition"
#define TANG_NEW_URL		"X-Tang-New-URL"
#define TANG_FILE_THUMBID	"X-Tang-File-Thumbnail-Id"	 // 传输文件缩略图ID
#define TANG_FILE_MP4ID		"X-Tang-File-Mp4-Id"		 // 传输文件缩略图ID

typedef enum convert_type
{

}ConvertType;
#define FS_CONVERT_TYPE_DEFAULT		0
#define FS_CONVERT_TYPE_THUMBID		1
#define FS_CONVERT_TYPE_MP4			2


#define FS_URL_FID					"fid="
#define FS_URL_QUESTION_MARK_FID	"?" FS_URL_FID


#endif /* FSDEF_H_ */
