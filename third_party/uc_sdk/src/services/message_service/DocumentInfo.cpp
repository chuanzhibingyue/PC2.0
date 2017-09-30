#include "DocumentInfo.h"


namespace uc{

DocumentInfo::DocumentInfo(){

	_fileId             = 0;
	_creatorId			= 0;
	_creatorName	    = "";
	_fileName		    = "";
	_fileFormat	        = "";
	_fileUrl		    = "";
	_total			    = 0;
	_uploadTime	        = 0;
	_createTime	        = 0;
	_modifyTime	        = 0;
	_fileSize		    = 0;
	_contentType	    = 0;
	_commentCount	    = 0;
	_totalLines	        = 0;
	_thumbnail		    = "";
	_description	    = "";
	_cloudType			= CT_FILE_TRANSFER;
	_eventId			= 0;

}

DocumentInfo::~DocumentInfo(void){

}


/////////////////////////////////////////////////////////////////////////////////////////////////////
CommentInfo::CommentInfo(void){
	_fileId      = 0;  
	_fileTitle   = "";
	_fileType    = 0;  
	_commentId   = 0; 
	_userId      = 0;    
	_userName    = "";  
	_createTime  = 0;
	_content     = "";   
	_cloudType	 = CT_FILE_TRANSFER;
	_msgId = 0;
}

CommentInfo::~CommentInfo(void){

}



} /**namespace uc*/
