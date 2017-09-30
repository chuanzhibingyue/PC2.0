#ifndef _ELEMENT_H
#define _ELEMENT_H

#include <string>
#include <list>
#include <vector>
#include "date.h"

using namespace std;

struct DeleteItem {
	template <typename T>
	void operator()(T *ptr) { delete ptr; }
};

// Custom data types

typedef enum { VCALENDAR, VEVENT, VALARM } Component;
typedef enum { DISPLAY=0, PROCEDURE, AUDIO, EMAIL } AlarmAction;

struct Recurrence {
	Recurrence(): Freq(YEAR), Interval(0), Count(0) {}
	operator string() const;
	bool IsEmpty() const { return (Interval == 0); }
	void Clear() { Interval = 0; }
	
	TimeUnit Freq;
	unsigned short Interval, Count;
	Date Until;
};

struct AlarmTrigger {
	AlarmTrigger(): Before(true), Value(0), Unit(MINUTE) {}
	AlarmTrigger &operator =(const string &Text);
	operator string() const;
	
	bool Before;
	unsigned short Value;
	TimeUnit Unit;
};
//参会人
struct VAttendee {
	std::string Account;	//参会人账号:内部联系人为userId,外部联系人为邮箱
	int Type;				//参会人类型：1 内部用户, 2 mail, 3 phone
	int Status;				//参会人状态：0 未处理， 1 接受， 2 拒绝
};
struct Alarm {
	Alarm(): Active(false), Action(DISPLAY) {}
	operator string() const;
	void Clear() {
		Description.clear();
	}
	
	bool Active;
	AlarmAction Action;
	AlarmTrigger Trigger;
	string Description;
};

struct VEvent {
	VEvent(): RecurrenceNo(0), BaseEvent(this) {}
	VEvent(const VEvent &Base):
		UID(Base.UID),
		Summary(Base.Summary),
		Description(Base.Description),
		Categories(Base.Categories),
		DtStamp(Base.DtStamp),
		DtStart(Base.DtStart),
		DtEnd(Base.DtEnd),
		RRule(Base.RRule),
		Alarms(Base.Alarms),
		RecurrenceNo(Base.RecurrenceNo),
		Location(Base.Location),
		HostName(Base.HostName),
		PCode1(Base.PCode1),
		PCode2(Base.PCode2),
		Class(Base.Class),
		Status(Base.Status),
		DtLastModify(Base.DtLastModify),
		EventId(Base.EventId),
		ConferenceId(Base.ConferenceId),
		IsGnet(Base.IsGnet),
		IsWholeDay(Base.IsWholeDay),
		HostId(Base.HostId),
		GroupId(Base.GroupId),
		RoomId(Base.RoomId),
		BillingCode(Base.BillingCode),
		UpdateFlag(Base.UpdateFlag),
		ExtrasInfo(Base.ExtrasInfo),
		MozillaAlarmDefaultLength(Base.MozillaAlarmDefaultLength)
	{
		Attendees = Attendees;
		BaseEvent = Base.BaseEvent == (VEvent *)&Base ? (VEvent *)&Base : Base.BaseEvent;
	}
	~VEvent() {
	}
	operator string() const;
	bool HasAlarm(const Date &From, const Date &To);

	string UID, Summary, Description, Categories, Location, HostName, PCode1, PCode2, Class, Status;
	Date DtStamp, DtStart, DtEnd, DtLastModify;
	string EventId, ConferenceId, IsGnet, IsWholeDay, HostId, GroupId, BillingCode, MozillaAlarmDefaultLength, UpdateFlag, RoomId, ExtrasInfo;
	Recurrence RRule;
	list<Alarm> Alarms;
	vector<VAttendee> Attendees;
	unsigned short RecurrenceNo;
	VEvent *BaseEvent;
};

struct EventsCriteria {
	EventsCriteria(): AllEvents(false), IncludeRecurrent(true) {}
	
	Date From, To;
	bool AllEvents, IncludeRecurrent;
};

inline ostream &operator <<(ostream &stream, const Recurrence &RRule) {
	stream << RRule.operator string();
	return stream;
}

inline ostream &operator <<(ostream &stream, const Alarm &Alarm) {
	stream << Alarm.operator string();
	return stream;
}

inline ostream &operator <<(ostream &stream, const VEvent &Event) {
	stream << Event.operator string();
	return stream;
}

#endif // _ELEMENT_H
