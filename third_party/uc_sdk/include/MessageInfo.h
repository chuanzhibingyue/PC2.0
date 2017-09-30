/*
 * MessageInfo.h
 *
 *  Created on: May 20, 2015
 *      Author: yongshuai cheng
 */

#ifndef  MESSAGEINFO_H_
#define  MESSAGEINFO_H_

#include "UCSDK.h"
#include "UCMessage.h"
#include <string>
#include "GroupInfo.h"

namespace uc{


#define MESSAGE_PLAYED         1
#define MESSAGE_NOTPALYED      0

enum MsgSendState{
	STATE_NULL   = -1,
	STATE_SENDSUCCESS,     //发送成功;
	STATE_SENDING,         //正在发送;
	STATE_SENDFAIL,        //发送失败;
	STATE_REVOCATION,      //消息撤回;
};

enum MsgContentType {
	CONTENT_TYPE_NONE = 0,
	CONTENT_TYPE_TEXT,
	CONTENT_TYPE_FILE,
	CONTENT_TYPE_MEDIA,
	CONTENT_TYPE_REPORT,
	CONTENT_TYPE_CODE,
	CONTENT_TYPE_TIP
};

class UC_API MessageInfo
{
public:
	MessageInfo(void) {
		_msgId = 0;
		_msgSeq = 0;
		_msgTime = 0;
		_protocolType = 0;
		_protocolId = 0;
		_appId = 0;
		_version = 0;
		_conversation = 0;
		_isPlayed = 0;
		_sendState = STATE_NULL;
		_msgData = "";
		_from.SetUserId(0);
		_from.SetSiteId(0);
		_from.SetResId(0);
		_to.SetUserId(0);
		_to.SetSiteId(0);
		_to.SetResId(0);
		_contentType = CONTENT_TYPE_NONE;
		_conversationType = NONE_CHAT;
		_relateId = 0;
		_flag = 0;
		_oaLastStatus = -1;
		_appAckSent = 1;
		_oaExternalData = "";
		_toPrivate.SetUserId(-1);
		_toPrivate.SetSiteId(-1);
		_toPrivate.SetResId(-1);
		_matchcount = 0;
		_eventId = 0;
	}

	virtual ~MessageInfo(void) {

	}

	int32_t GetAppId() const {
		return _appId;
	}

	void SetAppId(int32_t appId) {
		_appId = appId;
	}

	int32_t GetConversation() const {
		return _conversation;
	}

	void SetConversation(int32_t conversation) {
		_conversation = conversation;
	}

	const std::string& GetMsgData() const {
		return _msgData;
	}

	void SetMsgData(const std::string& msgData) {
		_msgData = msgData;
	}

	int32_t GetMsgId() const {
		return _msgId;
	}

	void SetMsgId(int32_t msgId) {
		_msgId = msgId;
	}

	int32_t GetFlag() const {
		return _flag;
	}

	void SetFlag(int32_t flag) {
		_flag = flag;
	}

	int64_t GetMsgSeq() const {
		return _msgSeq;
	}

	void SetMsgSeq(int64_t msgSeq) {
		_msgSeq = msgSeq;
	}

	MsgSendState GetSendState() const {
		return _sendState;
	}

	void SetSendState(MsgSendState state) {
		_sendState = state;
	}

	int32_t GetPlayedState() const{
		return _isPlayed;
	}

	void SetPlayedState(int32_t state){
		_isPlayed = state;
	}

	int64_t GetMsgTime() const {
		return _msgTime;
	}

	void SetMsgTime(int64_t msgTime) {
		_msgTime = msgTime;
	}

	int32_t GetProtocolId() const {
		return _protocolId;
	}

	void SetProtocolId(int32_t protocolId) {
		_protocolId = protocolId;
	}

	int32_t GetProtocolType() const {
		return _protocolType;
	}

	void SetProtocolType(int32_t protocolType) {
		_protocolType = protocolType;
	}

	int32_t GetVersion() const {
		return _version;
	}

	void SetVersion(int32_t version) {
		_version = version;
	}

	const UCID& GetFrom() const {
		return _from;
	}

	void SetFrom(const UCID& from) {
		_from = from;
	}

	int32_t GetIsPlayed() const {
		return _isPlayed;
	}

	void SetIsPlayed(int32_t isPlayed) {
		_isPlayed = isPlayed;
	}

	const UCID& GetTo() const {
		return _to;
	}

	void SetTo(const UCID& to) {
		_to = to;
	}

	int32_t GetContentType() const {
		return _contentType;
	}

	void SetContentType(int32_t contentType) {
		_contentType = contentType;
	}

	void InfoFromUCHead(const UCHead &head);

	ConversationType GetConversationType() const {
		return _conversationType;
	}

	void SetConversationType(ConversationType conversationType) {
		_conversationType = conversationType;
	}

	int64_t GetRelateId() const {
		return _relateId;
	}

	void SetRelateId(int64_t relateId) {
		_relateId = relateId;
	}


	int32_t GetOALastStatus() const {
		return _oaLastStatus;
	}

	void SetOALastStatus(int32_t oaLastStatus) {
		_oaLastStatus = oaLastStatus;
	}

	int32_t GetAppAckSent() const {
		return _appAckSent;
	}

	void SetAppAckSent(int32_t appAckSent) {
		_appAckSent = appAckSent;
	}

	const std::string& GetOAExternalData() const {
		return _oaExternalData;
	}

	void SetOAExternalData(const std::string& oaExternalData) {
		_oaExternalData = oaExternalData;
	}

	const UCID& GetToPrivate() const {
		return _toPrivate;
	}

	void SetToPrivate(const UCID& id) {
		_toPrivate = id;
	}

	const std::vector<UCID>& GetGroupAtUsers() const {
		return _groupAtUsers;
	}

	void SetGroupAtUsers(const std::vector<UCID>& ucids) {
		_groupAtUsers = ucids;
	}

	int64_t GetMatchCount() const{
		return _matchcount;
	}

	void SetMatchCount(int64_t matchCount){
		_matchcount = matchCount;
	}

	int32_t GetSearchEventId() const{
		return _eventId;
	}

	void SetSearchEventId(int32_t eventId){
		_eventId = eventId;
	}

private:
	int32_t			_msgId;
	int64_t			_msgSeq;
	int64_t			_msgTime;
	int32_t			_version;
	int32_t			_appId;
	int32_t			_protocolType;
	int32_t			_protocolId;
	UCID			_from;
	UCID			_to;
	int32_t			_conversation;
	int32_t			_isPlayed; /*1:已播放/0:未播放*/
	MsgSendState	_sendState;
	std::string		_msgData;
	ConversationType _conversationType;
	int64_t			_relateId;
	int32_t			_flag;
	int32_t         _oaLastStatus;  //OA消息最新状态;
	int32_t			_appAckSent;  //应用中心消息PuskAck结果;
	std::string     _oaExternalData; // OA消息扩展数据
	UCID			_toPrivate;	//to Group中的某个人
	std::vector<UCID>	_groupAtUsers;	//group中@人员集合
	int64_t         _matchcount;
	int32_t         _eventId;
protected:
	int32_t			_contentType;
};

class UC_API ConfMessageInfo {
public:
	ConfMessageInfo() {
		_eventId = 0;
		_conferenceId = 0;
		_startTime = 0;
	}
	virtual ~ConfMessageInfo() {

	}

	int64_t GetConferenceId() const {
		return _conferenceId;
	}

	void SetConferenceId(int64_t conferenceId) {
		_conferenceId = conferenceId;
	}

	int64_t GetEventId() const {
		return _eventId;
	}

	void SetEventId(int64_t eventId) {
		_eventId = eventId;
	}

	int64_t GetStartTime() const {
		return _startTime;
	}

	void SetStartTime(int64_t startTime) {
		_startTime = startTime;
	}

private:
	int64_t _eventId;
	int64_t _conferenceId;
	int64_t _startTime;
};

class UC_API TextMessageInfo: public MessageInfo {
public:
	TextMessageInfo() {
		_contentType = CONTENT_TYPE_TEXT;
		_textType = 1;
		_textContent = "";
	}

	virtual ~TextMessageInfo() {
	}

	const std::string& GetTextContent() const {
		return _textContent;
	}

	void SetTextContent(const std::string& textContent) {
		_textContent = textContent;
	}

	const std::string& GetTextFormat() const {
		return _textFormat;
	}

	void SetTextFormat(const std::string& textFormat) {
		_textFormat = textFormat;
	}

	int8_t GetTextType() const {
		return _textType;
	}

	void SetTextType(int8_t textType) {
		_textType = textType;
	}

private:
	int8_t _textType; // 1: 纯文本 2:MIME格式
	std::string _textFormat;
	std::string _textContent;
};

class UC_API ReportMessageInfo : public MessageInfo {
public:
	ReportMessageInfo() {
		_contentType = CONTENT_TYPE_REPORT;
		_eventId = 0;
		_reportTitle = "";
		_reportUrl = "";
		_externalData = "";
	}

	virtual ~ReportMessageInfo() {
	}

	const std::string& GetReportUrl() const {
		return _reportUrl;
	}

	void SetReportUrl(const std::string& reportUrl) {
		_reportUrl = reportUrl;
	}

	const std::string& GetReportTitle() const {
		return _reportTitle;
	}

	void SetReportTitle(const std::string& reportTitle) {
		_reportTitle = reportTitle;
	}

	const std::string& GetExternalData() const {
		return _externalData;
	}

	void SetExternalData(const std::string& externalData) {
		_externalData = externalData;
	}


	int64_t GetEventId() const {
		return _eventId;
	}

	void SetEventId(int64_t eventId) {
		_eventId = eventId;
	}

private:
	int64_t _eventId; // 1: 纯文本 2:MIME格式
	std::string _reportTitle;
	std::string _reportUrl;
	std::string _externalData;
};

class UC_API ConfTextMessageInfo : public ConfMessageInfo, public TextMessageInfo {
public:
	ConfTextMessageInfo() {

	}

	virtual ~ConfTextMessageInfo() {

	}
};

class UC_API FileMessageInfo : public MessageInfo {
public:
	FileMessageInfo() {
		_contentType = CONTENT_TYPE_FILE;
		_cloudType = CT_FILE_TRANSFER;
		_detailContent = "";
		_fileName = "";
		_fileSize = 0;
		_fileURL = "";
		_filePath = "";
		_fileId = 0;
	}

	virtual ~FileMessageInfo() {
	}

	DocumentCloudType GetCloudType() const {
		return _cloudType;
	}

	void SetCloudType(DocumentCloudType cloudType) {
		_cloudType = cloudType;
	}

	const std::string& GetDetailContent() const {
		return _detailContent;
	}

	void SetDetailContent(const std::string& detailContent) {
		_detailContent = detailContent;
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
		return _fileURL;
	}

	void SetFileUrl(const std::string& fileUrl) {
		_fileURL = fileUrl;
	}

	const std::string& GetFilePath() const {
		return _filePath;
	}

	void SetFilePath(const std::string& filePath) {
		_filePath = filePath;
	}

	int32_t GetFileId() const {
		return _fileId;
	}

	void SetFileId(int32_t fileId) {
		_fileId = fileId;
	}
	
private:
	DocumentCloudType		_cloudType;
	std::string				_detailContent;
	std::string _fileName;
	int64_t	_fileSize;
	std::string _fileURL;
	std::string _filePath;
	int32_t _fileId;
};

class UC_API MediaMessageInfo : public FileMessageInfo {
public:
	MediaMessageInfo() {
		_contentType = CONTENT_TYPE_MEDIA;
		_thumbnails = "";
		_duration = 0;
		_operatorId = 0;
		_operatorName = "";
		_operatorAvatar = "";
		_thumbnailsURL = "";
	}
	virtual ~MediaMessageInfo() {
	}

	int32_t GetDuration() const {
		return _duration;
	}

	void SetDuration(int32_t duration) {
		_duration = duration;
	}

	const std::string& GetThumbnails() const {
		return _thumbnails;
	}

	void SetThumbnails(const std::string& thumbnails) {
		_thumbnails = thumbnails;
	}

	const std::string& GetThumbnailsURL() const {
		return _thumbnailsURL;
	}

	void SetThumbnailsURL(const std::string& thumbnailsURL) {
		_thumbnailsURL = thumbnailsURL;
	}

	int32_t GetOperatorId() const {
		return _operatorId;
	}

	void SetOperatorId(int32_t id) {
		_operatorId = id;
	}

	const std::string& GetOperatorName() const {
		return _operatorName;
	}

	void SetOperatorName(const std::string& operatorName) {
		_operatorName = operatorName;
	}
	
	const std::string& GetOperatorAvatar() const {
		return _operatorAvatar;
	}

	void SetOperatorAvatar(const std::string& operatorAvatar) {
		_operatorAvatar = operatorAvatar;
	}

private:
	std::string _thumbnails;
	int32_t _duration;
	int32_t _operatorId;
	std::string _operatorName;
	std::string _operatorAvatar;
	std::string _thumbnailsURL;
};

class UC_API CodeMessageInfo: public MessageInfo {
public:
	CodeMessageInfo() {
		_contentType = CONTENT_TYPE_CODE;
		_codeId      = 0;
		_codeTitle   = "";
		_langType    = "";
		_codeHead    = "";
		_codeUrl     = "";
		_codePath    = "";
		_codeDesc    = "";
		_codeSize    = 0;
		_codeLine    = 0;
		_createTime  = 0;
		_creatorId   = 0;
		_creatorName = "";
	
	}
	virtual ~CodeMessageInfo() {
	}

	int64_t GetCodeId() const {
		return _codeId;
	}
	void SetCodeId(int64_t codeId) {
		_codeId = codeId;
	}

	const std::string& GetCodeTile() const {
		return _codeTitle;
	}
	void SetCodeTile(const std::string& codeTitle) {
		_codeTitle = codeTitle;
	}

	const std::string& GetLandType() const {
		return _langType;
	}
	void SetLandType(const std::string& langType) {
		_langType = langType;
	}

	const std::string& GetCodeHead() const {
		return _codeHead;
	}
	void SetCodeHead(const std::string& codeHead) {
		_codeHead = codeHead;
	}

	const std::string& GetCodeUrl() const {
		return _codeUrl;
	}
	void SetCodeUrl(const std::string& codeUrl) {
		_codeUrl = codeUrl;
	}

	const std::string& GetCodePath() const {
		return _codePath;
	}

	void SetCodePath(const std::string& codePath) {
		_codePath = codePath;
	}

	const std::string& GetCodeDesc() const {
		return _codeDesc;
	}
	void SetCodeDesc(const std::string& codeDesc) {
		_codeDesc = codeDesc;
	}

	int32_t GetCodeSize() const {
		return _codeSize;
	}
	void SetCodeSize(int32_t codeSize) {
		_codeSize = codeSize;
	}

	int32_t GetCodeLine() const {
		return _codeLine;
	}
	void SetCodeLine(int32_t codeLine) {
		_codeLine = codeLine;
	}

	int32_t GetCreateTime() const {
		return _createTime;
	}
	void SetCreateTime(int32_t createTime) {
		_createTime = createTime;
	}

	int32_t GetCreatorId() const {
		return _creatorId;
	}
	void SetCreatorId(int32_t creatorId) {
		_creatorId = creatorId;
	}
	
	const std::string& GetCreatorName() const {
		return _creatorName;
	}
	void SetCreatorName(const std::string& creatorName) {
		_creatorName = creatorName;
	}

private:
	int64_t     _codeId;
	std::string _codeTitle;
	std::string _langType ;
	std::string _codeHead;
	std::string _codeUrl;
	std::string _codePath;
	std::string _codeDesc;
	int32_t     _codeSize;
	int32_t     _codeLine;
	int32_t     _createTime;
	int32_t     _creatorId;
	std::string _creatorName;
};

class UC_API ConfMediaMessageInfo : public ConfMessageInfo, public MediaMessageInfo {
public:
	ConfMediaMessageInfo() {

	}
	virtual ~ConfMediaMessageInfo() {
	}
};

class UC_API TipMessageInfo : public MessageInfo {
public:
	TipMessageInfo() {
		_contentType = CONTENT_TYPE_TIP;
	}

	virtual ~TipMessageInfo() {
	}

private:
//	int32_t _operateType;
//	UCID _operator;
};

class UC_API RoleUpdatedMessageInfo : public MessageInfo {
public:
	RoleUpdatedMessageInfo() {
	}
	virtual ~RoleUpdatedMessageInfo() {
	}

	const std::map<int32_t, GroupRole>& GetUpdateRoles() const {
		return _updatedRoles;
	}

	void AddRoles(int32_t userId, GroupRole role) {
		_updatedRoles[userId] = role;
	}
private:
	std::map<int32_t, GroupRole> _updatedRoles;
};

}        /* namespace uc */
#endif   /* MESSAGEINFO_H_*/
