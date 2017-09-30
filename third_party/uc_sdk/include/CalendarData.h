/*
* CalendarData.h
*
*  Created on: May 22, 2015
*      Author: hezhuo
*/

#ifndef CALENDAR_DATA_H_
#define CALENDAR_DATA_H_

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "UCSDK.h"
#include "DocumentInfo.h"

namespace uc {
    using namespace std;

	//参会人类型：1 内部用户， 2 邮件用户， 3 电话用户
	enum AttendeeType { 
		AT_INTERNAL = 1, AT_MAIL, AT_PHONE
	};

	//参会人状态：0 未处理， 1 接受， 2 拒绝
	enum AttendeeStatus {
		AS_UNTREATED = 0, AS_ACCEPTED, AS_DENIED
	};

	//会议状态：0 正常， 1 已取消
	enum MeetingStatus {
		MS_NORMAL = 0, MS_CANCELED
	};

	//参会人
	class Attendee {
	public:
		typedef uc::AttendeeType TYPE;
		typedef uc::AttendeeStatus STATUS;
		Attendee() {
			_account = "";
			_type = TYPE::AT_INTERNAL;
			_status = STATUS::AS_UNTREATED;
			_name = "";
		}
		Attendee& operator=(const Attendee& other) {
			if (this == &other) {
				return *this;
			}
			_account = other._account;
			_type = other._type;
			_status = other._status;
			_name = other._name;
			return *this;
		}
		const std::string& GetAccount() const { return _account; }
		void SetAccount(const std::string& account){ _account = account; }
		const std::string& GetName() const { return _name; }
		void SetName(const std::string& name){ _name = name; }
		TYPE GetType() const { return _type; }
		void SetType(TYPE type){ _type = type; }
		STATUS GetStatus() const { return _status; }
		void SetStatus(STATUS status) { _status = status; }

	private:
		std::string _account;	//参会人账号:内部联系人为userId,外部联系人为邮箱
		TYPE _type;				//参会人类型：1 内部用户, 2 mail, 3 phone
		STATUS _status;			//参会人状态：0 未处理 , 1 接受，2 拒绝
		std::string _name;		//参会人名称
	};

	//时间段
	class TimeSlot
	{
	public:
		TimeSlot() {
			_beg = 0;
			_end = 0;
		}
		TimeSlot(int64_t beg, int64_t end) {
			_beg = beg;
			_end = end;
		}
		int64_t GetStartTime() const { return _beg; }
		void SetStartTime(int64_t startTime){ _beg = startTime; }
		int64_t GetEndTime() const { return _end; }
		void SetEndTime(int64_t endTime){ _end = endTime; }
	private:
		int64_t _beg;
		int64_t _end;
	};
	
	//用户日程
	class UserSchedule {
	public:
		UserSchedule(){
			_userId = 0;
		}

		int32_t GetUserId() const { 
			return _userId; 
		}

		void SetUserId(int32_t userId) { 
			_userId = userId; 
		}

		const std::vector<TimeSlot>& GetBusyTimes() const {
			return _busyTimes;
		}
		void SetBusyTimes(const std::vector<TimeSlot>& slots){
			_busyTimes = slots;
		}

	private:
		int32_t _userId;					//用户ID
		std::vector<TimeSlot> _busyTimes;	//忙碌时间段列表
	};

	//新的用户忙闲对象
	class UserBusyFreeInfo {
	public:
		int32_t GetUserId() const {
			return _userId;
		}

		void SetUserId(int32_t userId) {
			_userId = userId;
		}

		const std::map<int64_t,TimeSlot>& GetInfo() const {
			return _info;
		}

		void SetInfo(const std::map<int64_t, TimeSlot>& info){
			_info = info;
		}
	
	private:
		int32_t _userId;					//用户ID
		std::map<int64_t, TimeSlot> _info;	//eventId与时间段的对应关系
	};

	//周期会议规则
	class CycleRole {
	public:
		CycleRole() {
			_repeatType = "";
			_interval = 1;
			_weekdays = "";
			_monthNo = 1;
			_weekNo = 1;
			_dayNo = 1;
			_repeatCount = 0;
			_repeatEndDate = 0;
		}
		std::string RepeatType() const { return _repeatType; }
		void RepeatType(std::string val) { _repeatType = val; }
		int32_t Interval() const { return _interval; }
		void Interval(int32_t val) { _interval = val; }
		std::string Weekdays() const { return _weekdays; }
		void Weekdays(std::string val) { _weekdays = val; }
		int32_t MonthNo() const { return _monthNo; }
		void MonthNo(int32_t val) { _monthNo = val; }
		int32_t WeekNo() const { return _weekNo; }
		void WeekNo(int32_t val) { _weekNo = val; }
		int32_t DayNo() const { return _dayNo; }
		void DayNo(int32_t val) { _dayNo = val; }
		int32_t RepeatCount() const { return _repeatCount; }
		void RepeatCount(int32_t val) { _repeatCount = val; }
		int64_t RepeatEndDate() const { return _repeatEndDate; }
		void RepeatEndDate(int64_t val) { _repeatEndDate = val; }

	private:
		std::string _repeatType;	//周期类型，如daily\weekly\monthlyByDate\monthlyBySetPo\yearlyBySetPos
		int32_t _interval;			//间隔：每几天/周/月/年
		std::string _weekdays;		//星期几:"SU,MO,TU,WE,TH,FR,SA"
		int32_t _monthNo;			//第几月 1--12
		int32_t _weekNo;			//第几周 1--52
		int32_t _dayNo;				//第几天 1--365
		int32_t _repeatCount;		//周期会议循环次数，取值2--999， 与repeatEndDate二选一
		int64_t _repeatEndDate;		//周期会议结束时间
	};

	//会议地点信息
	class Address{
	public:
		const std::string& GetLongitude() const { return _longitude; }
		void SetLongitude(const std::string& val){ _longitude = val; }
		const std::string& GetLatitude() const { return _latitude; }
		void SetLatitude(const std::string& val){ _latitude = val; }
		int8_t GetType() const { return _type; }
		void SetType(int8_t val) { _type = val; }
		const std::string& GetName() const { return _name; }
		void SetName(const std::string& val){ _name = val; }
		const std::string& GetLocation() const { return _location; }
		void SetLocation(const std::string& val){ _location = val; }
	private:
		std::string _longitude;		//经度
		std::string _latitude;		//纬度
		int8_t _type;				//1-外出 2-公司内
		std::string _name;			//名称
		std::string _location;		//地点
	};
	//会议信息
	class Meeting {

	public:
		Meeting() 
		: _eventId(0),
		_parentId(0),
		_startTime(0),
		_oriTime(0),
		_duration(0),
		_conferenceId(""),
		_authorizerId(0),
		_groupId(0),
		_hostId(0),
		_hostPassword(""),
		_attendeePassword(""),
		_billingCode(""),
		_status(MeetingStatus::MS_NORMAL),
		_isCycle(0),
		_isGnet(1),
		_isWholeDay(0),
		_language(1),
		_emailLanguage(1),
		_isOld(false),
		_updateFlag(0),
		_lastUpdateTime(0),
		_roomId(""),
		_location(""),
		_extrasInfo("") {
		}

		const std::string& GetConferneceId() const { return _conferenceId; }
		void SetConferenceId(const std::string& conferenceId){ _conferenceId = conferenceId; }
		void SetConferenceId(int32_t iVal){
			std::stringstream ss;
			ss << iVal;
			_conferenceId = ss.str();
		}
		
		int64_t GetEventId() const {return _eventId;}
		void SetEventId(int64_t eventId){ _eventId = eventId;}
		
		int64_t GetParentId() const { return _parentId; }
		void SetParentId(int64_t parentId){ _parentId = parentId; }

		int64_t GetStartTime() const { return _startTime; }
		void SetStartTime(int64_t startTime){ _startTime = startTime; }

		int64_t GetOriTime() const { return _oriTime; }
		void SetOriTime(int64_t oriTime){ _oriTime = oriTime; }

		int32_t GetAuthorizerId() const { return _authorizerId; }
		void SetAuthorizerId(int32_t authorizerId){ _authorizerId = authorizerId; }

		int32_t GetHostId() const { return _hostId; }
		void SetHostId(int32_t hostId){ _hostId = hostId; }

		const std::string& GetHostName() const { return _hostName; }
		void SetHostName(const std::string& hostName){ _hostName = hostName; }

		int64_t GetGroupId() const { return _groupId;}
		void SetGroupId(int64_t groupId) { _groupId = groupId;};

		const std::string& GetHostPassword() const { return _hostPassword; }
		void SetHostPassword(const std::string& hostPassword){ _hostPassword = hostPassword;}

		const std::string& GetAttendeePassword() const { return _attendeePassword; }
		void SetAttendeePassword(const std::string& attendeePassword){ _attendeePassword = attendeePassword; }

		const std::string& GetBillingCode() const { return _billingCode; }
		void SetBillingCode(const std::string& val) { _billingCode = val; }

		int32_t GetDuration() const { return _duration; }
		void SetDuration(int32_t duration){ _duration = duration; }
		
		const std::vector<Attendee>& GetAttendees() const { return _attendees; }
		void SetAttendees(const std::vector<Attendee>& attendees){ _attendees = attendees; }
		void AddAttendees(const std::vector<Attendee>& attendees){
			if (attendees.size() > 0) {
				_attendees.insert(_attendees.end(), attendees.begin(), attendees.end());
			}			
		}

		const std::string& GetTitle() const { return _title; }
		void SetTitle(const std::string& title){ _title = title; }
		
		const std::string& GetSummary() const { return _summary; }
		void SetSummary(const std::string& summary){ _summary = summary; }
		
		const std::string& GetLocation() const { return _location; }
		void SetLocation(const std::string& location){ _location = location; }

		const std::string& GetExtrasInfo() const { return _extrasInfo; }
		void SetExtrasInfo(const std::string& val){ _extrasInfo = val; }
		
		const std::string& GetTimeZone() const { return _timeZone; }
		void SetTimeZone(const std::string& timeZone){ _timeZone = timeZone; }
		
		int32_t GetLanguage() const { return _language; }
		void SetLanguage(int32_t language){ _language = language; }
		
		int32_t GetEmailLanguage() const { return _emailLanguage; }
		void SetEmailLanguage(int32_t emailLanguage){ _emailLanguage = emailLanguage; }
			
		int32_t GetUpdateFlag() const { return _updateFlag; }
		void SetUpdateFlag(int32_t flag){ _updateFlag = flag; }
		
		int64_t GetLastUpdateTime() const { return _lastUpdateTime; }
		void SetLastUpdateTime(int64_t val) { _lastUpdateTime = val; }

		MeetingStatus GetStatus() const { return _status; }
		void SetStatus(MeetingStatus val) { _status = val; }

		const uc::CycleRole& GetCycleRole() const { return _cycleRole; }
		void SetCycleRole(const uc::CycleRole& val) { _cycleRole = val; }
		
		int32_t IsCycle() const { return _isCycle; }
		void IsCycle(int32_t iVal) { _isCycle = iVal; }

		int32_t IsGNet() const { return _isGnet; }
		void IsGNet(int32_t iVal) { _isGnet = iVal; }

		int32_t IsWholeDay() const { return _isWholeDay; }
		void IsWholeDay(int32_t iVal) { _isWholeDay = iVal; }
		
		bool IsOld() const { return _isOld; }
		void IsOld(bool val) { _isOld = val; }
		void IsOld(int32_t iVal) { _isOld = (iVal == 0 ? false : true); }

		const uc::Address& GetAddress() const { return _address; }
		void SetAddress(const uc::Address& val) { _address = val; }

		const std::string& GetRoomId() const { return _roomId; }
		void SetRoomId(const std::string& val) { _roomId = val; }

	private:
		std::string _conferenceId;					//会议ID
		int64_t _eventId;						//日程事件ID
		int64_t _parentId;						//周期单场变更前的事件ID,普通单场时同eventId
		int64_t _startTime;						//起始时间
		int64_t _oriTime;						//周期单场变更前的开始时间,普通单场时同startTime
		int32_t _duration;						//会议时长
		//int32_t _userId;						//用户ID
		int32_t _authorizerId;					//共享人ID
		int32_t _hostId;						//主持人ID
		std::vector<Attendee> _attendees;		//参会人列表 
		std::string _hostName;					//主持人名称
		std::string _hostPassword;				//主持人密码
		std::string _attendeePassword;			//参会人密码
		std::string _billingCode;				//外呼号
		std::string _title;						//会议标题
		std::string _summary;					//会议摘要
		std::string _location;					//会议地点
		std::string _timeZone;					//时区
		int64_t _groupId;						//会议关联的GroupId
		int32_t _isGnet;							//是否使用云会议
		int32_t _isCycle;							//是否周期会议
		int32_t _isWholeDay;						//是否全天会议
		int32_t _language;						//会议语言
		int32_t _emailLanguage;					//email语言
		CycleRole _cycleRole;					//周期会议规则
		int32_t _updateFlag;					//会议变更类型：0 无效值 1 只变更参会人 2 不变更参会人只变更其他信息  3 参会人和其他信息都变更
		MeetingStatus _status;						//会议状态：0-正常， 1-已取消，2-已拒绝
		bool _isOld;							//是否是旧的会议信息，预留
		int64_t _lastUpdateTime;				//最后一次更新时间，预留
		Address _address;						//会议位置信息
		std::string _roomId;						//会议室ID,如果是会议室的会时有效
		std::string _extrasInfo;				//附加信息
	};

	//授权人信息
	class Accrediting {
	public:
		Accrediting() {
			_userId = 0;
			_status = 0;
		}
		int32_t GetUserId() const { return _userId; }
		void SetUserId(int32_t accreditUserId){ _userId = accreditUserId; }
		int32_t GetStatus() const { return _status; }
		void SetStatus(int32_t status){ _status = status; }
	
	private:
		int32_t _userId;		//授权或被授权人ID
		int32_t _status;				//授权状态：0 自己授权给他人；1 他人授权给自己
	};

	//会议更新范围
	class SyncRange {
	public:
		SyncRange() {
			_startTime = 0;
			_endTime = 0;
		}

		SyncRange(int64_t start, int64_t end) {
			_startTime = start;
			_endTime = end;
		}

		int64_t GetStartTime() const { return _startTime; }
		int64_t GetEndTime() const { return _endTime; }
		void SetStartTime(int64_t start) { _startTime = start;}
		void SetEndTime(int64_t end) { _endTime = end; }

	private:
		int64_t _startTime;
		int64_t _endTime;
	};

	//会议视频录制信息
	class MeetingRecording {
	public:
		MeetingRecording() {
			_videoName = "";
			_videoURL = "";
			_videoThumb = "";
			_videoThumbURL = "";
			_opeatorId = 0;
			_operatorName = "";
			_operatorAvatar = "";
			_startTime = 0;
			_endTime = 0;
			_length = 0;
			_size = 0;
		}

		const std::string& GetVideoName() const { return _videoName; }
		void SetVideoName(const std::string& val) { _videoName = val; }
		string GetVideoURL() const { return _videoURL; }
		void SetVideoURL(string val) { _videoURL = val; }
		string GetVideoThumb() const { return _videoThumb; }
		void SetVideoThumb(string val) { _videoThumb = val; }
		string GetVideoThumbURL() const { return _videoThumbURL; }
		void SetVideoThumbURL(string val) { _videoThumbURL = val; }
		int32_t GetOperatorId() const { return _opeatorId; }
		void SetOperatorId(int32_t val) { _opeatorId = val; }
		string GetOperatorName() const { return _operatorName; }
		void SetOperatorName(string val) { _operatorName = val; }
		string GetOperatorAvatar() const { return _operatorAvatar; }
		void SetOperatorAvatar(string val) { _operatorAvatar = val; }
		int32_t GetLength() const { return _length; }
		void SetLength(int32_t val) { _length = val; }
		int32_t GetSize() const { return _size; }
		void SetSize(int32_t val) { _size = val; }
		int64_t GetStartTime() const { return _startTime; }
		void SetStartTime(int64_t val) { _startTime = val; }
		int64_t GetEndTime() const { return _endTime; }
		void SetEndTime(int64_t val) { _endTime = val; }

	private:
		string _videoName;				//录象名称
		string _videoURL;				//录像URL
		string _videoThumb;				//录像缩略图数据
		string _videoThumbURL;			//录像缩略图URL
		int32_t _opeatorId;				//操作者ID
		string _operatorName;			//操作者名称
		string _operatorAvatar;			//操作者图像的URL
		int32_t _length;				//视频时长
		int32_t _size;					//视频大小
		int64_t _startTime;				//录制开始时间（预留）
		int64_t _endTime;				//录制结束时间（预留）

	};

	//会议纪要信息
	class Minutes {
	public:
		Minutes() {
			_minutesId = 0;			//会议纪要ID
			_eventId = 0;			//所属事件ID
			_conferenceId = "";		//所属会议ID
			_startTime = 0;
			_creatorId = 0;			//创建人ID
			_createTime = 0;		//创建时间
			_updateTime = 0;		//更新时间
			_title = "";			//会议纪要标题
			_content = "";			//会议纪要内容
			_type = 0;				//0--会议纪要，1--会议录制，2--会议文档
		}

		int64_t GetMinutesId() const { return _minutesId; }
		void SetMinutesId(int64_t val) { _minutesId = val; }
		int64_t GetEventId() const { return _eventId; }
		void SetEventId(int64_t val) { _eventId = val; }
		const std::string& GetConferenceId() const { return _conferenceId; }
		void SetConferenceId(const std::string& val) { _conferenceId = val; }
		int64_t GetStartTime() const { return _startTime; }
		void SetStartTime(int64_t val) { _startTime = val; }
		int32_t GetCreatorId() const { return _creatorId; }
		void SetCreatorId(int32_t val) { _creatorId = val; }
		int64_t GetCreateTime() const { return _createTime; }
		void SetCreateTime(int64_t val) { _createTime = val;}
		int64_t GetUpdateTime() const { return _updateTime; }
		void SetUpdateTime(int64_t val) { _updateTime = val; }
		const std::string& GetTitle() const { return _title; }
		void SetTitle(const std::string& val) { _title = val; }
		const std::string& GetContent() const { return _content; }
		void SetContent(const std::string& val) { _content = val; }
		int32_t GetType() const { return _type; }
		void SetType(int32_t val) { _type = val; }
		const MeetingRecording& GetRecording() const { return _recording; }
		void SetRecording(const MeetingRecording& val) { _recording = val; }
		const DocumentInfo& GetDocumentInfo() const { return _docinfo; }
		void SetDocumentInfo(const DocumentInfo& val) { _docinfo = val; }

	private:
		int64_t _minutesId;			//会议纪要ID
		int64_t _eventId;			//所属事件ID
		std::string _conferenceId;	//所属会议ID
		int64_t _startTime;			//所属会议开始时间
		int32_t _creatorId;			//创建人ID
		int64_t _createTime;		//创建时间
		int64_t _updateTime;		//更新时间
		int32_t _type;				//会议纪要类型：0--普通会议纪要， 1 -- 会议视频
		std::string _title;			//会议纪要标题
		std::string _content;		//会议纪要内容
		MeetingRecording _recording;	//会议视频信息（type为1时有效）
		DocumentInfo _docinfo;			//会议文件信息（type为2时有效）
	};

	//会议信息
	class MeetingRoom {

	public:
		MeetingRoom()
			: _roomId(""),
			_hostId(0),
			_startTime(0),
			_endTime(0),
			_orgId(0),
			_conferenceId(""),
			_defaultAttendeesVisible(1),
			_defaultAttendeesJoin(1),
			_isSetPassword(1),
			_roomPassword(""),
			_hostPassword(""),
			_attendeePassword(""),
			_title(""),
			_remarks(""){
	}		

		const std::string& GetRoomId() const { return _roomId; }
		void SetRoomId(const std::string& roomId){ _roomId = roomId; }

		int64_t GetRoomHostId() const { return _hostId; }
		void SetRoomHostId(int64_t hostId){ _hostId = hostId; }

		int64_t GetOrgId() const { return _orgId; }
		void SetOrgId(int64_t orgId){ _orgId = orgId; }

		const std::string& GetConferneceId() const { return _conferenceId; }
		void SetConferenceId(const std::string& conferenceId){ _conferenceId = conferenceId; }
// 		void SetConferenceId(int32_t iVal){
// 			std::stringstream ss;
// 			ss << iVal;
// 			_conferenceId = ss.str();
// 		}

		int64_t GetStartTime() const { return _startTime; }
		void SetStartTime(int64_t startTime){ _startTime = startTime; }

		int64_t GetEndTime() const { return _endTime; }
		void SetEndTime(int64_t endTime){ _endTime = endTime; }

		void SetDefaultAttendeesVisible(int32_t val) {	_defaultAttendeesVisible = val;}
		int32_t GetDefaultAttendeesVisible() const { return _defaultAttendeesVisible;}

		void SetDefaultAttendeesJoin(int32_t val) { _defaultAttendeesJoin = val;}
		int32_t GetDefaultAttendeesJoin() const { return _defaultAttendeesJoin; }

		void SetIsSetPassword(int32_t val) { _isSetPassword = val; }
		int32_t GetIsSetPassword() const { return _isSetPassword; }

		const std::string& GetRoomPassword() const { return _roomPassword; }
		void SetRoomPassword(const std::string& password){ _roomPassword = password; }

		const std::string& GetHostPassword() const { return _hostPassword; }
		void SetHostPassword(const std::string& hostPassword){ _hostPassword = hostPassword; }

		const std::string& GetAttendeePassword() const { return _attendeePassword; }
		void SetAttendeePassword(const std::string& attendeePassword){ _attendeePassword = attendeePassword; }

		const std::vector<Attendee>& GetAttendees() const { return _attendees; }
		void SetAttendees(const std::vector<Attendee>& attendees){ _attendees = attendees; }
		void AddAttendees(const std::vector<Attendee>& attendees){
			if (attendees.size() > 0) {
				_attendees.insert(_attendees.end(), attendees.begin(), attendees.end());
			}
		}

		const std::string& GetTitle() const { return _title; }
		void SetTitle(const std::string& title){ _title = title; }

		const std::string& GetRemarks() const { return _remarks; }
		void SetRemarks(const std::string& remarks){ _remarks = remarks; }

		const std::string& GetBillingCode() const { return _billingCode; }
		void SetBillingCode(const std::string& code){ _billingCode = code; }

	private:
		std::string _roomId;					//会议室ID
		int64_t _hostId;						//主持人ID
		int64_t _orgId;							//所属组织ID
		std::string _conferenceId;				//会议ID
		int64_t _startTime;						//会议室有效起始时间
		int64_t _endTime;						//会议室有效结束时间
		int32_t _defaultAttendeesVisible;		//只允许默认参与者可见, 1表示是, 0表示否，默认1
		int32_t _defaultAttendeesJoin;			//只允许默认参与者加入, 1表示是, 0表示否，默认1
		int32_t _isSetPassword;					//是否设置会议室密码, 1表示是, 0表示否，默认1
		std::vector<Attendee> _attendees;		//会议室成员列表 
		std::string _roomPassword;				//会议室密码
		std::string _hostPassword;				//主持人密码
		std::string _attendeePassword;			//参会人密码
		std::string _title;						//会议室标题
		std::string _remarks;					//会议室备注
		std::string _billingCode;				//外呼号
	};

} /* namespace uc */
#endif /* CALENDAR_DATA_H_ */
