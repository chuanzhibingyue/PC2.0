#include "ServiceObjectConversion.h"

#include <sstream>
#include "common/StringConvert.h"

namespace ucclient {

std::wstring ServiceObjectConversion::ConvertUCIDToJSON(const UCID &jid) {
	std::wstringstream ss;

	ss  << L"{" << "\"userId\": " << jid.GetUserId() << ", " << "\"siteId\": " 
		<< jid.GetSiteId() << ", "<< "\"resId\": " << jid.GetResId() << "}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertConverstaionInfoToJSON(const ConversationInfo &info){
	std::wstringstream ss;

	ss << "{"
		<< "\"type\":" << info.GetType() << ","
		<< "\"relateID\":" << info.GetRelateId() << ","
		<< "\"conversation\":" << info.GetConversation() << ","
		<< "\"newMsgCount\":" << info.GetNewMsgCount() << ","
		<< "\"lastMsgTime\":" << info.GetLastMsgTime() << ","
		<< "\"lastMsgSeq\":" << info.GetLastMsgSeq() << ","
		<< "\"topIndex\":" << info.GetTopIndex() << ","
		<< "\"eventId\":" << info.GetEventId() << ","
		<< "\"conferenceId\":" << info.GetConferenceId() << ","
		<< "\"authorizerId\":" << info.GetAuthorizerId() << ","
		<< "\"hidden\":" << info.isHidden() << "}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertGroupInfoToJSON(const GroupInfo &info) {
	std::wstringstream ss;

	ss << L"{";
	ss << ServiceObjectConversion::AppendGroupInfoToJSON(info);
	ss << "}";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendGroupInfoToJSON(const GroupInfo &info) {
	std::wstringstream ss;

	ss << "\"id\": " << info.GetId() << ", "
		<< "\"mountId\": " << info.GetMountId() << ", "
		<< "\"externalId\": \"" << UTF2WJSON(info.GetExternalId()) << "\", "
		<< "\"siteId\": " << info.GetSiteId() << ", "
		<< "\"name\": \"" << UTF2WJSON(info.GetName()) << "\", "
		<< "\"pinyin\": \"" << UTF2WJSON(info.GetPinYinName()) << "\", "
		<< "\"createTime\": " << info.GetCreateTime() << ", "
		<< "\"avatarIndex\": \"" << UTF2WJSON(info.GetAvatarIndex()) << "\", "
		<< "\"localAvatar\": \"" << UTF2WJSON(info.GetLocalAvatar()) << "\", "
		<< "\"joinState\": " << info.GetJoinState() << ", "
		<< "\"memberCount\": " << info.GetMemberCount() << ", "
		<< "\"adminId\": " << info.GetAdminId() << ", "
		<< "\"alertState\": " << info.GetGroupAlert() << ", "
		<< "\"alertRule\": " << info.GetAlertRule() << ", "
		<< "\"alertTime\": " << info.GetAlertTime() << ", "
		<< "\"topState\": " << info.GetGroupTop() << ", "
		<< "\"status\": " << info.GetGroupStatus() << ", "
		<< "\"disturbState\": " << info.GetGroupDisturb() << ", "
		<< "\"nameFlag\": " << info.GetNameFlag() << ", "
		<< "\"groupType\": " << info.GetGroupType() << ", "
		<< "\"statusInfo\": \"" << UTF2WJSON(info.GetStatusInfo()) << "\", "
		<< "\"memberType\": " << info.GetMemberType() << ", "
		<< "\"logoLevel\": " << info.GetLogoLevel() << ", \"members\":[";

	const std::list<MemberInfo> &members = info.GetMember();
	for (std::list<MemberInfo>::const_iterator iter = members.begin(); iter != members.end();) {
		if (iter != members.begin())
			ss << ",";

		ss << ConvertMemberInfoToJSON(*iter);
		iter++;
	}
	ss << "]";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMemberInfoToJSON(const MemberInfo &info) {
	std::wstringstream ss;
	ss << L"{" << "\"userId\": " << info.userId << ", " << "\"joinTime\": " << info.joinTime 
		<< ",\"role\": " << info.role << "}";
	 return ss.str();
}

std::wstring ServiceObjectConversion::ConvertDocumentInfoToJSON(const DocumentInfo &info) {
	std::wstringstream ss;

	ss << L"{";
	ss << ServiceObjectConversion::AppendDocumentInfoToJSON(info);
	ss << L"}";
		
	return ss.str();
}

std::wstring ServiceObjectConversion::AppendDocumentInfoToJSON(const DocumentInfo &info) {
	std::wstringstream ss;
	ss << "\"commentCount\": " << info.GetCommentCount() << ", "
		<< "\"contentType\": " << info.GetContentType() << ", "
		<< "\"createTime\": " << info.GetCreateTime() << ", "
		<< "\"description\": \"" << UTF2WJSON(info.GetDescription()) << "\", "
		<< "\"fileFormat\": \"" << UTF2WJSON(info.GetFileFormat()) << "\", "
		<< "\"fileId\": " << info.GetFileId() << ", "
		<< "\"fileName\": \"" << UTF2WJSON(info.GetFileName()) << "\", "
		<< "\"fileSize\": " << info.GetFileSize() << ", "
		<< "\"fileUrl\": \"" << UTF2WJSON(info.GetFileUrl()) << "\", "
		<< "\"preview\": \"" << UTF2WJSON(info.GetPreview()) << "\", "
		<< "\"modifyTime\": " << info.GetModifyTime() << ", "
		<< "\"thumbnail\": \"" << UTF2WJSON(info.GetThumbnail()) << "\", "
		<< "\"total\": " << info.GetTotal() << ", "
		<< "\"totalLines\": " << info.GetTotalLines() << ", "
		<< "\"uploadTime\": " << info.GetUploadTime() << ", "
		<< "\"userId\": " << info.GetCreatorId() << ", "
		<< "\"username\":\"" << UTF2WJSON(info.GetCreatorName()) << "\","
		<< "\"creatorId\": " << info.GetCreatorId() << ", "
		<< "\"creatorName\": \"" << UTF2WJSON(info.GetCreatorName()) << "\", "
		<< "\"codeId\": " << info.GetFileId() << ","
		<< "\"codeTitle\": \"" << UTF2WJSON(info.GetFileName()) << "\","
		<< "\"langType\": \"" << UTF2WJSON(info.GetFileFormat()) << "\","
		<< "\"codeHead\": \"" << UTF2WJSON(info.GetThumbnail()) << "\","
		<< "\"codeDesc\": \"" << UTF2WJSON(info.GetDescription()) << "\","
		<< "\"codeSize\": " << info.GetFileSize() << ","
		<< "\"codeUrl\": \"" << UTF2WJSON(info.GetFileUrl()) << "\","
		<< "\"codeLine\": " << info.GetTotalLines() << ","
		<< "\"createTime\": " << info.GetCreateTime() << ","
		<< "\"cloudType\": " << info.GetCloudType() << ", "
		<< "\"hashCode\": \"" << UTF2WJSON(info.GetHashCode()) << "\", "
		<< "\"mountId\": " << info.GetMountId() << ", "
		<< "\"detailType\": " << info.GetDetailType() << ", "
		<< "\"detailContent\": \"" << UTF2WJSON(info.GetDetailContent()) << "\", "
		<< "\"eventId\": " << info.GetEventId() << ", "
		<< "\"messageSeq\": " << info.GetMessageSeq() << ", "
		<< "\"fileLocalpath\":\"" << UTF2WJSON(info.GetFileLocalPath()) << "\",";
		
	
	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertCommentInfoToJSON(const CommentInfo &info){
	std::wstringstream ss;

	ss << L"{";
	ss << ServiceObjectConversion::AppendCommentInfoToJSON(info);
	ss << L"}";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendCommentInfoToJSON(const CommentInfo &info){
	std::wstringstream ss;
	ss  << "\"fileId\": " << info.GetFileId() << ", "
		<< "\"fileType\": " << info.GetFileType() << ", "
		<< "\"fileTitle\": \"" << UTF2WJSON(info.GetFileTitle()) << "\", "
		<< "\"commentId\": " << info.GetCommentId() << ", "
		<< "\"userName\": \"" << UTF2WJSON(info.GetDisplayName()) << "\", "
		<< "\"userId\": " << info.GetUserId() << ", "
		<< "\"content\": \"" << UTF2WJSON(info.GetContent()) << "\", "
		<< "\"createTime\": " << info.GetCreateTime() << ", "
		<< "\"cloudType\": " << info.GetCloudType() << ", ";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertAttendeeToJSON(const Attendee &attendee) {
	std::wstringstream ss;
	ss << L"{" << "\"account\": \"" << UTF2WJSON(attendee.GetAccount()) << "\", "
		<< "\"type\": " << attendee.GetType() << ", "
		<< "\"status\": " << attendee.GetStatus() << ", "
		<< "\"name\": \"" << UTF2WJSON(attendee.GetName()) << "\"}";
	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertCycleRoleToJSON(const CycleRole &role) {
	std::wstringstream ss;
	ss << L"{" << "\"repeatType\": \"" << UTF2WJSON(role.RepeatType()) << "\", "
		<< "\"interval\": " << role.Interval() << ", "
		<< "\"repeatCount\": " << role.RepeatCount() << ", "
		<< "\"repeatEndDate\": " << role.RepeatEndDate() << ", "
		<< "\"monthNo\": " << role.MonthNo() << ", "
		<< "\"weekNo\": " << role.WeekNo() << ", "
		<< "\"dayNo\": " << role.DayNo() << ", "
		<< "\"weekdays\": \"" << UTF2WJSON(role.Weekdays()) << "\"}";
	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertAccountInfoToJSON(const AccountInfo &info){
	std::wstringstream ss;

	ss << L"{" << "\"userAccount\": \"" << UTF2WJSON(info.GetUserAccount()) << "\", "
		<< "\"userName\": \"" << UTF2WJSON(info.GetDisplayName()) << "\", "
		<< "\"localAvatar\": \"" << UTF2WJSON(info.GetLocalAvatar()) << "\", "
		<< "\"loginTime\": " << info.GetLoginTime() << ", "
		<< "\"password\": \"" << UTF2WJSON(info.GetPassword()) << "\", "
		<< "\"lastManualLoginTime\": " << info.GetLastManualLoginTime() << ", "
		<< "\"savePassword\": " << info.IsSavePassword() << ", "
		<< "\"autoLogin\": " << info.IsAutoLogin() << ", "
		<< "\"accounttype\": " << info.GetAccountType() << "}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertAddressToJSON(const Address &address) {
	std::wstringstream ss;
	ss << L"{" << "\"longitude\": \"" << UTF2WJSON(address.GetLongitude()) << "\", "
		<< "\"latitude\": \"" << UTF2WJSON(address.GetLatitude()) << "\", "
		<< "\"type\": " << address.GetType() << ", "
		<< "\"name\": \"" << UTF2WJSON(address.GetName()) << "\", "
		<< "\"location\": \"" << UTF2WJSON(address.GetLocation()) << "\"}";
	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertAccreditInfoToJSON(const Accrediting &info) {
	std::wstringstream ss;

	ss << L"{" << "\"accreditUserId\": " << info.GetUserId() << ", "
		<< "\"status\": " << info.GetStatus() << "}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertScheduleMapToJSON(const std::map<int64_t, TimeSlot>& mapInfo){
	std::wstringstream ss;
	std::map<int64_t, TimeSlot>::const_iterator it = mapInfo.begin();
	while (it != mapInfo.end()){
		if (it != mapInfo.begin())
		{
			ss << ",";
		}
		ss << "{" << "\"" << it->first << "\":";
		ss << ConvertTimeSlotToJSON(it->second);
		ss << "}";
		it++;
	}

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertTimeSlotToJSON(const TimeSlot &ts) {
	std::wstringstream ss;

	ss << L"{" << "\"startTime\": " << ts.GetStartTime() << ", "
		<< "\"endTime\": " << ts.GetEndTime() << "}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertUserScheduleToJSON(const UserSchedule &userSchedule) {
	std::wstringstream ss;

	ss << L"{" << "userId: " << userSchedule.GetUserId() << ", busyTimes: [";

	std::vector<TimeSlot>& tms = const_cast<std::vector<TimeSlot>&>(userSchedule.GetBusyTimes());
	for (std::size_t i = 0; i < tms.size(); ++i) {
		if (i > 0)
			ss << ",";
		ss << ConvertTimeSlotToJSON(tms[i]);
	}

	ss << "]}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertUserBusyFreeInfoToJSON(const UserBusyFreeInfo &info) {
	std::wstringstream ss;
	const std::map<int64_t, TimeSlot> mapInfo = info.GetInfo();

	ss << L"{" << "\"" << info.GetUserId() << "\": [";

	ss << ConvertScheduleMapToJSON(mapInfo);

	ss << "]}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertContactToJSON(const ContactInfo &contact){
	std::wstringstream ss;

	ss  << L"{" << "\"status\": " << contact.GetStatus() << ", "
		<< "\"userId\": " << contact.GetUserId() << ", "
		<< "\"siteId\": " << contact.GetSiteId() << ", "
		<< "\"displayName\": \"" << UTF2WJSON(contact.GetDisplayName()) << "\", "
		<< "\"cardVersion\": " << contact.GetCardVersion() << ", "
		<< "\"pinyin\": \"" << UTF2WJSON(contact.GetPingyin()) << "\", "
		<< "\"sex\": " << contact.GetSex() << ", "
		<< "\"account\": \"" << UTF2WJSON(contact.GetAccount()) << "\", "
		<< "\"email\": \"" << UTF2WJSON(contact.GetEmail()) << "\", "
		<< "\"mobile\": \"" << UTF2WJSON(contact.GetMobile()) << "\", "
		<< "\"workPhone\": \"" << UTF2WJSON(contact.GetWorkPhone()) << "\", "
		<< "\"localAvatar\": \"" << UTF2WJSON(contact.GetLocalAvatar()) << "\", "
		<< "\"remoteAvatarUrl\": \"" << UTF2WJSON(contact.GetRemoteAvatarUrl()) << "\", "
		<< "\"personalSign\": \"" << UTF2WJSON(contact.GetPersonalSign()) << "\", "
		<< "\"userTags\": \"[" << UTF2WJSON(contact.GetUserTags()) << "]\", "
		<< "\"departmentId\": " << contact.GetDepartmentId() << ", "
		<< "\"departmentName\": \"" << UTF2WJSON(contact.GetDepartmentName()) << "\", "
		<< "\"position\": \"" << UTF2WJSON(contact.GetPosition()) << "\", "
		<< "\"superiorId\": " << contact.GetSuperiorId() << ", "
		<< "\"superiorName\": \"" << UTF2WJSON(contact.GetSuperiorName()) << "\", "
		<< "\"colleagueNum\": " << contact.GetColleagueNum() << ", "
		<< "\"relation\": " << contact.GetRelation() << ", "
		<< "\"type\": " << contact.GetType() << ", \"tags\": [";
	
// 	std::vector<ContactTag> tags;
// 	contact.GetTags(tags);
// 
// 	for (std::size_t i = 0; i < tags.size(); i++) {
// 		if (i > 0)
// 			ss << ",";
// 		ss << ConvertContactTagToJSON(tags[i]);
// 	}

	ss << "]}";

	return ss.str();
 }

std::wstring ServiceObjectConversion::ConvertContactTagToJSON(const ContactTag &tag) {
	std::wstringstream ss;

	ss  << L"{" << "\"id\": " << tag.GetId() << ", "
		<< "\"inputType\": " << tag.GetInputType() << ", "
		<< "\"name\": \"" << UTF2WJSON(tag.GetName()) << "\", "
		<< "\"value\": \"" << UTF2WJSON(tag.GetValue()) << "\", "
		<< "\"scope\": " << tag.GetScope() << ", "
		<< "\"state\": " << tag.GetState() << ", "
		<< "\"type\": " << tag.GetType() << ", "
		<< "\"userId\": " << tag.GetUserId() << "}";
	
	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertBasicContactInfoToJSON(const BasicContactInfo &contact) {
	std::wstringstream ss;

	ss << L"{" << "\"userId\": " << contact.GetUserId() << ", "
		<< "\"account\": \"" << UTF2WJSON(contact.GetAccount()) << "\", "
		<< "\"pinyin\": \"" << UTF2WJSON(contact.GetPinyin()) << "\", "
		<< "\"displayName\": \"" << UTF2WJSON(contact.GetDisplayName()) << "\", "
		<< "\"localAvatar\": \"" << UTF2WJSON(contact.GetLocalAvatar()) << "\", "
		<< "\"remoteAvatarUrl\": \"" << UTF2WJSON(contact.GetRemoteAvatarUrl()) << "\", "
		<< "\"departmentId\": " << contact.GetDepartmentId() << ", "
		<< "\"departmentName\": \"" << UTF2WJSON(contact.GetDepartmentName()) << "\", "
		<< "\"position\": \"" << UTF2WJSON(contact.GetPosition()) << "\", "
		<< "\"relation\": \"" << contact.GetRelation() << "\", "
		<< "\"siteId\": " << contact.GetSiteId() << ", "
		<< "\"personalSign\": \"" << UTF2WJSON(contact.GetPersonalSign()) << "\"}";
	
	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertDepartmentInfoToJSON(const DepartmentInfo &depart) {
	std::wstringstream ss;

	ss << L"{" << "departmentName: \"" << UTF2WJSON(depart.GetDepartName()) << "\", "
		<< "id: " << depart.GetId() << ", "
		<< "parentId: " << depart.GetParentId() << ", "
		<< "staffNumber: " << depart.GetStaffNum() << ", "
		<< "totalNumber: " << depart.GetTotalNum() << ", "
		<< "onlineNumber: " << depart.GetOnlineNum() << ", staffs:[";

	std::vector<ContactInfo*> staffs = depart.GetStaffs();

	if (!staffs.empty()) {
		for (std::size_t i = 0; i < staffs.size(); ++i) {
			if (i > 0)
				ss << ",";
			ss << ConvertContactToJSON(*staffs[i]);
		}
	}

	ss << "], subDepartments: [";
	std::vector<DepartmentInfo*> subDeparts = depart.GetSubDepart();
	for (std::size_t i = 0; i < subDeparts.size(); ++i) {
		if (i > 0)
			ss << ",";
		ss << ConvertDepartmentInfoToJSON(*subDeparts[i]);
	}
	
	ss << "]}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMessageInfoToJSON(const MessageInfo &info) {
	std::wstringstream ss;

	ss << L"{";
	ss << AppendMessageInfoToJSON(info);

	int32_t type = info.GetContentType();
	if (type == CONTENT_TYPE_TEXT) {
		ss << AppendTextMessageInfoToJSON(dynamic_cast<TextMessageInfo&>(const_cast<MessageInfo&>(info)));
	}
	else if (type == CONTENT_TYPE_FILE) {
		ss << AppendFileMessageInfoToJSON(dynamic_cast<FileMessageInfo&>(const_cast<MessageInfo&>(info)));
	}
	else if (type == CONTENT_TYPE_MEDIA) {
		ss << AppendMediaMessageInfoToJSON(dynamic_cast<MediaMessageInfo&>(const_cast<MessageInfo&>(info)));
	}

	ss << L"}";
	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertTextMessageInfoToJSON(const TextMessageInfo &info) {
	std::wstringstream ss;

	ss << L"{";

	ss << AppendMessageInfoToJSON(dynamic_cast<MessageInfo&>(const_cast<TextMessageInfo&>(info)));
	ss << AppendTextMessageInfoToJSON(info);

	ss << "}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMediaMessageInfoToJSON(const MediaMessageInfo &info) {
	std::wstringstream ss;
	
	ss << L"{";

	ss << AppendMessageInfoToJSON(dynamic_cast<MessageInfo&>(const_cast<MediaMessageInfo&>(info)));
	ss << AppendFileMessageInfoToJSON(dynamic_cast<FileMessageInfo&>(const_cast<MediaMessageInfo&>(info)));
	ss << AppendMediaMessageInfoToJSON(info);

	ss << L"}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertFileMessageInfoToJSON(const FileMessageInfo &info) {
	std::wstringstream ss;

	ss << L"{";

	ss << AppendMessageInfoToJSON(dynamic_cast<MessageInfo&>(const_cast<FileMessageInfo&>(info)));
	ss << AppendFileMessageInfoToJSON(info);

	ss << L"}";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendReportMessageInfoToJSON(const ReportMessageInfo &info){
	std::wstringstream ss;

	ss << AppendMessageInfoToJSON(dynamic_cast<MessageInfo&>(const_cast<ReportMessageInfo&>(info)));
	ss << "reportTitle: \"" << UTF2WJSON(info.GetReportTitle()) << "\", "
		<< "reportUrl: \"" << UTF2WJSON(info.GetReportUrl()) << "\", "
		<< "eventId: " << info.GetEventId() << ", ";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendTextMessageInfoToJSON(const TextMessageInfo &info) {
	std::wstringstream ss;

	ss << AppendMessageInfoToJSON(dynamic_cast<MessageInfo&>(const_cast<TextMessageInfo&>(info)));
	ss << "textContent: \"" << UTF2WJSON(info.GetTextContent()) << "\", "
		<< "textFormat: \"" << UTF2WJSON(info.GetTextFormat()) << "\", "
		<< "textType: " << info.GetTextType() << ", ";

	return ss.str();
}	

std::wstring ServiceObjectConversion::AppendRoleUpdatedMessageInfoToJSON(const RoleUpdatedMessageInfo &info) {
	std::wstringstream ss;

	ss << AppendMessageInfoToJSON(dynamic_cast<MessageInfo&>(const_cast<RoleUpdatedMessageInfo&>(info)));

	const std::map<int32_t, GroupRole>&updatedRoles = info.GetUpdateRoles();
	ss << "\"roles\":[";
	for (std::map<int32_t, GroupRole>::const_iterator iter = updatedRoles.begin(); iter != updatedRoles.end(); ++iter) {
		ss << "{\"userId\": " << iter->first << ", \"role\": " << iter->second << "},";
	}
	ss << "], ";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendMediaMessageInfoToJSON(const MediaMessageInfo &info) {
	std::wstringstream ss;

	ss << AppendFileMessageInfoToJSON(dynamic_cast<FileMessageInfo&>(const_cast<MediaMessageInfo&>(info)));
	ss << "thumbnails: \"" << UTF2WJSON(info.GetThumbnails()) << "\", "
		<< "thumbnailsURL: \"" << UTF2WJSON(info.GetThumbnailsURL()) << "\", "
		<< "duration: " << info.GetDuration() << ", "
		<< "operatorId: " << info.GetOperatorId() << ", "
		<< "operatorName: \"" << URLDATA_UTF2WJSON(info.GetOperatorName()) << "\", "
		<< "operatorAvatar: \"" << UTF2WJSON(info.GetOperatorAvatar()) << "\", ";

	return ss.str();
}	

std::wstring ServiceObjectConversion::AppendMessageInfoToJSON(const MessageInfo &info) {
	std::wstringstream ss;
	ss  << L"\"appid\": " << info.GetAppId() << ", " 
		<< "\"protocolId\": " << info.GetProtocolId() << ", "
		<< "\"protocolType\": " << info.GetProtocolType() << ", "
		<< "\"conversation\": " << info.GetConversation() << ", "
		<< "\"conversationType\": " << info.GetConversationType() << ", "
		<< "\"msgId\": " << info.GetMsgId() << ", "
		<< "\"msgSeq\": " << info.GetMsgSeq() << ", "
		<< "\"sendState\": " << info.GetSendState() << ", "
		<< "\"isPlayed\": " << info.GetPlayedState() << ", "
		<< "\"msgTime\": " << info.GetMsgTime() << ", "
		<< "\"version\": " << info.GetVersion() << ", "
		<< "\"contentType\": " << info.GetContentType() << ", "
		<< "\"from\": " << ConvertUCIDToJSON(const_cast<UCID&>(info.GetFrom())) << ", "
		<< "\"to\": " << ConvertUCIDToJSON(const_cast<UCID&>(info.GetTo())) << ", "
		<< "\"toPrivate\": " << ConvertUCIDToJSON(const_cast<UCID&>(info.GetToPrivate())) << ", "
		<< "\"atUsers\": [";
	
	const std::vector<UCID>& ucids = info.GetGroupAtUsers();
	for (int i = ucids.size()-1; i >= 0; i--) {
		ss << ConvertUCIDToJSON(ucids[i]);
		if (i > 0)
			ss << ",";
		}

	ss << "], ";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendFileMessageInfoToJSON(const FileMessageInfo &info) {
	std::wstringstream ss;

	ss  << AppendMessageInfoToJSON(dynamic_cast<MessageInfo&>(const_cast<FileMessageInfo&>(info)));
	ss << "\"fileName\": \"" << URLDATA_UTF2WJSON(info.GetFileName()) << "\", "
		<< "\"fileSize\": " << info.GetFileSize() << ", "
		<< "\"fileId\": " << info.GetFileId() << ", "
		<< "\"fileURL\": \"" << UTF2WJSON(info.GetFileUrl()) << "\","
		<< "\"localPath\": \"" << UTF2WJSON(info.GetFilePath()) << "\","
		<< "\"cloudType\": " << info.GetCloudType() << ", "
		<< "\"detailContent\": \"" << UTF2WJSON(info.GetDetailContent()) << "\", ";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendCodeMessageInfoToJSON(const CodeMessageInfo &info) {
	std::wstringstream ss;

	ss  << AppendMessageInfoToJSON(dynamic_cast<MessageInfo&>(const_cast<CodeMessageInfo&>(info)));
	ss  << "\"creatorId\": " << info.GetCreatorId() << ", "
		<< "\"creatorName\": \"" << UTF2WJSON(info.GetCreatorName()) << "\", "
		<< "\"codeId\": " << info.GetCodeId() << ","
		<< "\"codeTitle\": \"" << UTF2WJSON(info.GetCodeTile()) << "\","
		<< "\"langType\": \"" << UTF2WJSON(info.GetLandType()) << "\","
		<< "\"codeHead\": \"" << UTF2WJSON(info.GetCodeHead()) << "\","
		<< "\"codeDesc\": \"" << UTF2WJSON(info.GetCodeDesc()) << "\","
		<< "\"codeSize\": " << info.GetCodeSize()<< ","
		<< "\"codeUrl\": \"" << UTF2WJSON(info.GetCodeUrl()) << "\","
		<< "\"codeLine\": " << info.GetCodeLine() << ","
		<< "\"createTime\": " << info.GetCreateTime() << ",";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertContactToJSON(const list<ContactInfo*>& contacts) {
	std::wstringstream ss;
	for (std::list<ContactInfo*>::const_iterator iter = contacts.begin(); iter != contacts.end(); ++iter) {
		if (iter != contacts.begin())
			ss << ",";
		ss << ServiceObjectConversion::ConvertContactToJSON(**iter);
	}
	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertGroupInfoToJSON(const list<GroupInfo>& groups) {
	std::wstringstream ss;

	for (std::list<GroupInfo>::const_iterator iter = groups.begin(); iter != groups.end(); ++iter) {
		if (iter != groups.begin())
			ss << ",";

		ss << ServiceObjectConversion::ConvertGroupInfoToJSON(*iter);
	}

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMeetingToJSON(const Meeting &meeting) {
	std::wstringstream ss;

	ss << L"{";
	ss << ServiceObjectConversion::AppendMeetingToJSON(meeting);
	ss << "}";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendMeetingToJSON(const Meeting &meeting) {
	std::wstringstream ss;

	ss << "\"conferenceId\": \"" << UTF2WJSON(meeting.GetConferneceId()) << "\", "
		<< "\"eventId\": " << meeting.GetEventId() << ", "
		<< "\"parentId\": " << meeting.GetParentId() << ", "
		<< "\"groupId\": " << meeting.GetGroupId() << ", "
		<< "\"roomId\": \"" << UTF2WJSON(meeting.GetRoomId()) << "\", "
		<< "\"authorizerId\": " << meeting.GetAuthorizerId() << ", "
		<< "\"hostPassword\": \"" << UTF2WJSON(meeting.GetHostPassword()) << "\", "
		<< "\"attendeePassword\": \"" << UTF2WJSON(meeting.GetAttendeePassword()) << "\", "
		<< "\"billingCode\": \"" << UTF2WJSON(meeting.GetBillingCode()) << "\", "
		<< "\"isGNet\": " << meeting.IsGNet() << ", "
		<< "\"isCycle\": " << meeting.IsCycle() << ", "
		<< "\"isWholeDay\": " << meeting.IsWholeDay() << ", "
		<< "\"startTime\": " << meeting.GetStartTime() << ", "
		<< "\"oriTime\": " << meeting.GetOriTime() << ", "
		<< "\"updateFlag\": " << meeting.GetUpdateFlag() << ", "
		<< "\"lastUpdateTime\": " << meeting.GetLastUpdateTime() << ", "
		<< "\"status\": " << meeting.GetStatus() << ", "
		<< "\"attendees\": [";

	const std::vector<Attendee>& attendee = meeting.GetAttendees();
	for (std::size_t i = 0; i < attendee.size(); ++i) {
		if (i > 0)
			ss << ",";
		ss << ConvertAttendeeToJSON(attendee[i]);
	}

	ss << "], \"title\": \"" << URLDATA_UTF2WJSON(meeting.GetTitle()) << "\", "
		<< "\"summary\": \"" << URLDATA_UTF2WJSON(meeting.GetSummary()) << "\", "
		<< "\"duration\": " << meeting.GetDuration() << ", "
		<< "\"location\": \"" << URLDATA_UTF2WJSON(meeting.GetLocation()) << "\", "
		<< "\"extrasInfo\": \"" << URLDATA_UTF2WJSON(meeting.GetExtrasInfo()) << "\", "
		<< "\"timeZone\": \"" << UTF2WJSON(meeting.GetTimeZone()) << "\", "
		<< "\"language\": " << meeting.GetLanguage() << ", "
		<< "\"emailLanguage\": " << meeting.GetEmailLanguage() << ", "
		<< "\"hostId\": " << meeting.GetHostId() << ", "
		<< "\"isOld\": " << meeting.IsOld() << ", "
		<< "\"hostName\": \"" << URLDATA_UTF2WJSON(meeting.GetHostName()) << "\", "
		<< "\"cycleRole\": ";
	ss << ConvertCycleRoleToJSON(meeting.GetCycleRole());

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMeetingToJSON(const list<Meeting*>& meetings) {
	std::wstringstream ss;

	for (list<Meeting*>::const_iterator iter = meetings.begin(); iter != meetings.end(); ++iter) {
		if (iter != meetings.begin())
			ss << ",";

		ss << ServiceObjectConversion::ConvertMeetingToJSON(**iter);
	}

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMeetingRoomToJSON(const MeetingRoom &room) {
	std::wstringstream ss;

	ss << L"{";
	ss << ServiceObjectConversion::AppendMeetingRoomToJSON(room);
	ss << "}";

	return ss.str();
}


std::wstring ServiceObjectConversion::AppendMeetingRoomToJSON(const MeetingRoom &room) {
	std::wstringstream ss;

	ss << "\"conferenceId\": \"" << UTF2WJSON(room.GetConferneceId()) << "\", "
		<< "\"roomId\": \"" << UTF2WJSON(room.GetRoomId()) << "\", "
		<< "\"orgId\": " << room.GetOrgId() << ", "
		<< "\"isSetPassword\": " << room.GetIsSetPassword() << ", "
		<< "\"hostPassword\": \"" << UTF2WJSON(room.GetHostPassword()) << "\", "
		<< "\"attendeePassword\": \"" << UTF2WJSON(room.GetAttendeePassword()) << "\", "
		<< "\"defaultAttendsVisible\": " << room.GetDefaultAttendeesVisible() << ", "
		<< "\"defaultAttendsJoin\": " << room.GetDefaultAttendeesJoin() << ", "
		<< "\"roomPassword\": \"" << UTF2WJSON(room.GetRoomPassword()) << "\", "
		<< "\"startTime\": " << room.GetStartTime() << ", "
		<< "\"endTime\": " << room.GetEndTime() << ", "
		<< "\"roomHostId\": " << room.GetRoomHostId() << ", "
		<< "\"billingCode\": \"" << UTF2WJSON(room.GetBillingCode()) << "\", "
		<< "\"attendees\": [";

	const std::vector<Attendee>& attendee = room.GetAttendees();
	for (std::size_t i = 0; i < attendee.size(); ++i) {
		if (i > 0)
			ss << ",";
		ss << ConvertAttendeeToJSON(attendee[i]);
	}

	ss << "], \"title\": \"" << UTF2WJSON(room.GetTitle()) << "\", "
		<< "\"remarks\": \"" << UTF2WJSON(room.GetRemarks()) << "\"";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMeetingRoomToJSON(const list<MeetingRoom>& rooms) {
	std::wstringstream ss;

	for (list<MeetingRoom>::const_iterator iter = rooms.begin(); iter != rooms.end(); ++iter) {
		if (iter != rooms.begin())
			ss << ",";

		ss << ServiceObjectConversion::ConvertMeetingRoomToJSON(*iter);
	}

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertSearchMessageInfosToJSON(const list<SearchMessageInfo*>& messages){
	std::wstringstream ss;

	for (std::list<SearchMessageInfo*>::const_iterator iter = messages.begin(); iter != messages.end(); ++iter) {
		if (iter != messages.begin())
			ss << ", ";

		SearchMessageInfo * info = *iter;
		ss << "{\"relateId\":" << (*iter)->GetRelateId() << ","
			<< "\"conversationType\":" << (*iter)->GetConversationType() << ","
			<< "\"count\":" << (*iter)->GetMatchCounts() << ","
			<< "\"eventId\":" << (*iter)->GetEventId() << ","
			<< "\"message\":[" <<ServiceObjectConversion::ConvertMessageInfosToJSON((*iter)->GetMessageList()) << "]}";
	}

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMessageInfosToJSON(const list<MessageInfo*>& messages) {
	std::wstringstream ss;

	for (std::list<MessageInfo*>::const_iterator iter = messages.begin(); iter!= messages.end(); ++iter) {
		if (iter != messages.begin())
			ss << ", ";

		ss << ServiceObjectConversion::ConvertMessageInfoToJSON(**iter);	
	}

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMeetingTextMessageToJSON( const ConfTextMessageInfo &info ) {
	std::wstringstream ss;

	ss << L"{";
	ss << ServiceObjectConversion::AppendMeetingTextMessageToJSON(info);
	ss << L"}";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMeetingMediaMessgeToJSON( const ConfMediaMessageInfo &info ) {
	std::wstringstream ss;
	
	ss << L"{";
	ss << ServiceObjectConversion::AppendMeetingMediaMessgeToJSON(info);
	ss << L"}";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendMeetingTextMessageToJSON(const ConfTextMessageInfo &info) {
	std::wstringstream ss;

	ss << AppendTextMessageInfoToJSON(dynamic_cast<TextMessageInfo&>(const_cast<ConfTextMessageInfo&>(info)));
	ss << AppendMeetingMessageToJSON(dynamic_cast<ConfMessageInfo&>(const_cast<ConfTextMessageInfo&>(info)));

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendMeetingMediaMessgeToJSON(const ConfMediaMessageInfo &info) {
	std::wstringstream ss;

	ss << AppendMediaMessageInfoToJSON(dynamic_cast<MediaMessageInfo&>(const_cast<ConfMediaMessageInfo&>(info)));
	ss << AppendMeetingMessageToJSON(dynamic_cast<ConfMessageInfo&>(const_cast<ConfMediaMessageInfo&>(info)));

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendMeetingMessageToJSON(const ConfMessageInfo &info) {
	std::wstringstream ss;
	ss << "eventId: " << info.GetEventId() << ", startTime: " << info.GetStartTime() << ", ";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMinutesToJSON(const Minutes& minutes) {
	std::wstringstream ss;
	ss << L"{" << "\"minutesId\": " << minutes.GetMinutesId() << ", "
		<< "\"eventId\": " << minutes.GetEventId() << ", "
		<< "\"conferenceId\": \"" << UTF2WJSON(minutes.GetConferenceId()) << "\", "
		<< "\"startTime\": " << minutes.GetStartTime() << ", "
		<< "\"creatorId\": " << minutes.GetCreatorId() << ", "
		<< "\"createTime\": " << minutes.GetCreateTime() << ", "
		<< "\"updateTime\": " << minutes.GetUpdateTime() << ", "
		<< "\"title\": \"" << URLDATA_UTF2WJSON(minutes.GetTitle()) << "\", "
		<< "\"content\": \"" << URLDATA_UTF2WJSON(minutes.GetContent()) << "\", "
		<< "\"type\": " << minutes.GetType() << ", ";

	ss << AppendMeetingRecordingToJSON(minutes.GetRecording());
	ss << AppendDocumentInfoToJSON(minutes.GetDocumentInfo());
	ss << L"}";
	return ss.str();
}

std::wstring ServiceObjectConversion::AppendMeetingRecordingToJSON(const MeetingRecording &recording) {
	std::wstringstream ss;
	ss << "\"videoURL\": \"" << UTF2WJSON(recording.GetVideoURL()) << "\", "
		<< "\"videoThumb\": \"" << UTF2WJSON(recording.GetVideoThumb()) << "\", "
		<< "\"videoThumbURL\": \"" << UTF2WJSON(recording.GetVideoThumbURL()) << "\", "
		<< "\"videoName\": \"" << URLDATA_UTF2WJSON(recording.GetVideoName()) << "\", "
		<< "\"operatorId\": " << recording.GetOperatorId() << ", "
		<< "\"operatorName\": \"" << URLDATA_UTF2WJSON(recording.GetOperatorName()) << "\", "
		<< "\"operatorAvatar\": \"" << UTF2WJSON(recording.GetOperatorAvatar()) << "\", "
		<< "\"length\": " << recording.GetLength() << L", "
		<< "\"size\": " << recording.GetSize() << L", "
		<< "\"videoStartTime\": " << recording.GetStartTime() << ", "
		<< "\"videoEndTime\": " << recording.GetEndTime() << ", ";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMinutesToJSON(const vector<Minutes>& minutes){
	std::wstringstream ss;

	vector<Minutes>::const_iterator it = minutes.begin();
	while (it != minutes.end()) {
		if (it != minutes.begin())
			ss << ",";

		ss << ServiceObjectConversion::ConvertMinutesToJSON(*it);
		it++;
	}

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendAppMessageInfoToJSON(const AppInfo &info){
	std::wstringstream ss;
	ss << L"{" << "\"appId\": " << info.GetAppId() << ", "
		<< "\"appTitle\": \""  << UTF2WJSON(info.GetAppTitle()) << "\", "
		<< "\"appDesc\": \""  << UTF2WJSON(info.GetAppDesc()) << "\", "
		<< "\"appLogo\": \"" << UTF2WJSON(info.GetAppLogo()) << "\", "
		<< "\"pcUrl\": \"" << UTF2WJSON(info.GetPcUrl()) << "\", "
		<< "\"iosUrl\": \"" << UTF2WJSON(info.GetIosUrl()) << "\", "
		<< "\"androidUrl\": \"" << UTF2WJSON(info.GetAndroidUrl()) << "\", "
		<< "\"enableSendMessage\": " << info.GetEnableSendMessage() << ", "
		<< "\"status\": " << info.GetStatus() << ", "
		<< "\"eventUrl\": \"" << UTF2WJSON(info.GetEventUrl()) << "\", "
		<< "\"access\": " << info.GetAccess() << L"}";
	return ss.str();
}


std::wstring ServiceObjectConversion::ConvertMessageReadInfoToJSON(const MessageReadInfo &info){
	std::wstringstream ss;
	
	ss << L"{";
	ss << ServiceObjectConversion::AppendMessageReadInfoToJSON(info);
	ss << L"}";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendMessageReadInfoToJSON(const MessageReadInfo &info){
	std::wstringstream ss;
	ss << "\"msgSeq\": " << info.GetMsgSeq() << ", "
		<< "\"unReadCount\": " << info.GetUnreadCount() << ", "
		<< "\"type\": " << info.GetConversationType() << ", "
		<< "\"allRead\": " << info.GetAllRead() << ",";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertMessageReadUserInfoToJSON(const MessageReadUserInfo &info){
	std::wstringstream ss;

	ss << L"{";
	ss << ServiceObjectConversion::AppendMessageReadUserInfoToJSON(info);
	ss << L"}";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendMessageReadUserInfoToJSON(const MessageReadUserInfo &info){
	std::wstringstream ss;
	ss << "\"msgSeq\": " << info.GetMsgSeq() << ", "
		<< "\"unReadCount\": " << info.GetUnreadCount() << ", "
		<< "\"type\": " << info.GetConversationType() << ", "
		<< "\"allRead\": "<< info.GetAllRead() << "," 
		<< "\"unReadUser\": [";

	const UserIdList &list = info.GetUser();
	for (UserIdList::const_iterator iter = list.begin(); iter != list.end(); ++iter){
		if (iter != list.begin()){
			ss << ",";
		}
		ss << *iter;
	}
	ss << "]";
	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertUserStatusToJSON(const UserStatus &info){
	std::wstringstream ss;
	ss << L"{";
	ss << ServiceObjectConversion::AppendUserStatusToJSON(info);
	ss << L"}";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendUserStatusToJSON(const UserStatus &info){
	std::wstringstream ss;
	ss << "\"userId\": " << info.GetUserId() << ", "
		<< "\"status\": " << info.GetUserStatus() << ", "
		<< "\"show\": \""  << UTF2WJSON(info.GetStatusShow()) << "\", ";

	return ss.str();
}

std::wstring ServiceObjectConversion::ConvertExternalUserToJSON(const ExternalUser &user){
	std::wstringstream ss;

	ss << L"{" << "\"userId\": " << user.GetUserId() << ", "
		<< "\"userAccount\": \"" << UTF2WJSON(user.GetUserAccount()) << "\", " 
		<< "\"displayName\": \"" << UTF2WJSON(user.GetDisplayName()) << "\", " 
		<< "\"localAvatar\": \"" << UTF2WJSON(user.GetLocalAvatar()) << "\", "
		<< "\"isSelf\": " << (int32_t)user.GetIsSlef() << ", "
        <<"}";

	return ss.str();
}

std::wstring ServiceObjectConversion::AppendExternalMessageToJSON(const ExternalMessage &info){
	std::wstringstream ss;
	ss << L"\"msgSeq\": " << info.GetMsgSeq() << ", "
		<< "\"msgTime\": " << info.GetMsgSeq() << ", "
		<< "\"contentType\": " << info.GetContentType() << ", "
		<< "\"from\": " << ConvertExternalUserToJSON(const_cast<ExternalUser&>(info.GetFrom())) << ", "
		<< "\"to\": " << ConvertExternalUserToJSON(const_cast<ExternalUser&>(info.GetTo())) << ", ";

	return ss.str();
}
std::wstring ServiceObjectConversion::AppendTextExternalMessageToJSON(const TextExternalMessage &info){
	std::wstringstream ss;
	ss << L"{";
	ss << AppendExternalMessageToJSON(dynamic_cast<ExternalMessage&>(const_cast<TextExternalMessage&>(info)));
	ss << "\"textContent\": \"" << UTF2WJSON(info.GetTextContent()) << "\", ";
	ss << L"}";
	return ss.str();
}

}