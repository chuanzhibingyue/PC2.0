/*
 * ServiceObjectConversion.h
 *
 *  Created on: July 14, 2015
 *      Author: gang wang
 */

#ifndef SERVICE_OBJECT_CONVERSION_H_
#define SERVICE_OBJECT_CONVERSION_H_

#include "DocumentInfo.h"
#include "GroupInfo.h"
#include "UCMessage.h"
#include "ContactInfo.h"
#include "DepartmentInfo.h"
#include "CalendarData.h"
#include "MessageInfo.h"
#include "SearchInfo.h"
#include "AccountInfo.h"
#include "AppInfo.h"
#include "MessageReadInfo.h"
#include "ExternalInfo.h"
#include "ConversationInfo.h"

namespace ucclient {

using namespace uc;

class ServiceObjectConversion {
public:
	static std::wstring ConvertUCIDToJSON(const UCID &jid);


	static std::wstring ConvertConverstaionInfoToJSON(const ConversationInfo &info);

	static std::wstring ConvertGroupInfoToJSON(const GroupInfo &info);
	static std::wstring ConvertGroupInfoToJSON(const list<GroupInfo>& groups);
	static std::wstring AppendGroupInfoToJSON(const GroupInfo &info);

	static std::wstring ConvertAccountInfoToJSON(const AccountInfo &info);
	static std::wstring ConvertAttendeeToJSON(const Attendee &attendee);
	static std::wstring ConvertCycleRoleToJSON(const CycleRole &role);
	static std::wstring ConvertMemberInfoToJSON(const MemberInfo &info);
	static std::wstring ConvertAddressToJSON(const Address &address);

	static std::wstring ConvertDocumentInfoToJSON(const DocumentInfo &info);
	static std::wstring AppendDocumentInfoToJSON(const DocumentInfo &info);

	static std::wstring ConvertCommentInfoToJSON(const CommentInfo &infon);
	static std::wstring AppendCommentInfoToJSON(const CommentInfo &info);

	static std::wstring ConvertMeetingToJSON(const Meeting &meeting);
	static std::wstring ConvertMeetingToJSON(const list<Meeting*>& meetings);
	static std::wstring AppendMeetingToJSON(const Meeting &meeting);

	static std::wstring ConvertMeetingRoomToJSON(const MeetingRoom & room);
	static std::wstring ConvertMeetingRoomToJSON(const list<MeetingRoom>& meetings);
	static std::wstring AppendMeetingRoomToJSON(const MeetingRoom &meeting);

	static std::wstring ConvertMinutesToJSON(const Minutes& minutes);
	static std::wstring ConvertMinutesToJSON(const vector<Minutes>& minutes);
	static std::wstring AppendMeetingRecordingToJSON(const MeetingRecording &recording);
	static std::wstring ConvertAccreditInfoToJSON(const Accrediting &info);
	static std::wstring ConvertTimeSlotToJSON(const TimeSlot &ts);
	static std::wstring ConvertScheduleMapToJSON(const std::map<int64_t, TimeSlot>& mapInfo);
	static std::wstring ConvertUserScheduleToJSON(const UserSchedule &userSchedule);
	static std::wstring ConvertUserBusyFreeInfoToJSON(const UserBusyFreeInfo &info);
	static std::wstring ConvertDepartmentInfoToJSON(const DepartmentInfo &depart);

	static std::wstring ConvertContactToJSON(const ContactInfo &contact);
	static std::wstring ConvertContactToJSON(const list<ContactInfo*>& contacts);
	static std::wstring ConvertContactTagToJSON(const ContactTag &tag);
	static std::wstring ConvertBasicContactInfoToJSON(const BasicContactInfo &contact);

    static std::wstring ConvertMessageInfoToJSON(const MessageInfo &info);
	static std::wstring AppendMessageInfoToJSON(const MessageInfo &info);

    static std::wstring ConvertTextMessageInfoToJSON(const TextMessageInfo &info);
	static std::wstring AppendTextMessageInfoToJSON(const TextMessageInfo &info);

	static std::wstring ConvertSearchMessageInfosToJSON(const list<SearchMessageInfo*>& messages);
	static std::wstring ConvertMessageInfosToJSON(const list<MessageInfo*>& messages);
    static std::wstring ConvertMediaMessageInfoToJSON(const MediaMessageInfo &info);
	static std::wstring AppendMediaMessageInfoToJSON(const MediaMessageInfo &info);
	static std::wstring AppendRoleUpdatedMessageInfoToJSON(const RoleUpdatedMessageInfo &info);

	static std::wstring AppendReportMessageInfoToJSON(const ReportMessageInfo &info);

    static std::wstring ConvertFileMessageInfoToJSON(const FileMessageInfo &info);
	static std::wstring AppendFileMessageInfoToJSON(const FileMessageInfo &info);

	static std::wstring ConvertMeetingTextMessageToJSON(const ConfTextMessageInfo &info);
	static std::wstring ConvertMeetingMediaMessgeToJSON(const ConfMediaMessageInfo &info);
	static std::wstring AppendMeetingMessageToJSON(const ConfMessageInfo &info);
	static std::wstring AppendMeetingTextMessageToJSON(const ConfTextMessageInfo &info);
	static std::wstring AppendMeetingMediaMessgeToJSON(const ConfMediaMessageInfo &info);

	static std::wstring AppendCodeMessageInfoToJSON(const CodeMessageInfo &info);

	static std::wstring AppendAppMessageInfoToJSON(const AppInfo &info);

	static std::wstring ConvertMessageReadInfoToJSON(const MessageReadInfo &info);
	static std::wstring AppendMessageReadInfoToJSON(const MessageReadInfo &info);

	static std::wstring ConvertMessageReadUserInfoToJSON(const MessageReadUserInfo &info);
	static std::wstring AppendMessageReadUserInfoToJSON(const MessageReadUserInfo &info);

	static std::wstring ConvertUserStatusToJSON(const UserStatus &info);
	static std::wstring AppendUserStatusToJSON(const UserStatus &info);

	static std::wstring ConvertExternalUserToJSON(const ExternalUser &user);
	static std::wstring AppendExternalMessageToJSON(const ExternalMessage &info);
	static std::wstring AppendTextExternalMessageToJSON(const TextExternalMessage &info);
};
}

#endif