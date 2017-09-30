#ifndef _ICALENDAR_H
#define _ICALENDAR_H

#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <sstream>
#include "element.h"

using namespace std;

class ICalendar {
public:
	ICalendar(const char *FileName): FileName((char *)FileName) { LoadFromFile(); }
	ICalendar(){}
	~ICalendar() {
		for_each(Events.begin(), Events.end(), DeleteItem());
	}
	void LoadFromFile();
	void LoadICalendarStr(const std::string& str);
	//Event* GetEventByUID(char *UID);

	void AddEvent(VEvent *NewEvent);
	void DeleteEvent(VEvent *DeletedEvent);
	void ModifyEvent(VEvent *ModifiedEvent);

	class Query;
	
private:
	string GetProperty(const string &Line) const {
		// if VALUE=DATE or VALUE=DATE-TIME used, then the date is not directly
		// after the property name so we just search for the string after ':'
		string Temp = Line.substr(Line.find_first_of(':')+1);
		unsigned int Length = Temp.length();
		if (Length > 0 && Temp[Length-1] == '\r')
			Temp.resize(Length-1);
		return Temp;
	}
	string GetSubProperty(const string &Line, const char *SubProperty) const {
		size_t Pos = Line.find(SubProperty);
		if (Pos == string::npos)
			return "";
		Pos += strlen(SubProperty) + 1;
		return Line.substr(Pos, Line.find_first_of(';', Pos)-Pos);
	}

	string GetAttendeeAcount(const string &Line, const char *SubProperty = "CN=") const {
		//char* SubProperty = "CN";
		size_t Pos = Line.find(SubProperty);
		if (Pos == string::npos)
			return "";

		Pos += strlen(SubProperty) + 1;

		//兼容server返回数据中丢失双引号的问题
		int npos = Line.find_first_of('\"', Pos);
		if (npos < 0)
		{
			int npos1 = Line.find_first_of(':', Pos);
			int npos2 = Line.find_first_of(';', Pos);
			if (npos1 > 0 && npos2 > 0){
				npos = npos1 < npos2 ? npos1 : npos2;
			}
			else if (npos1 > 0){
				npos = npos1;
			}
			else if (npos2 > 0){
				npos = npos2;
			}else{
				npos = Pos;
			}
		}
			
		string Account = Line.substr(Pos, npos-Pos);
		return Account;
// 		if(Account.length() < 4 ){
// 			return "";
// 		}
// 		return Account.substr(2, Account.length()-4);
	}

	void FixLineEnd(string &Line, unsigned int Length) {
		if (Length > 0 && Line[Length-1] == '\r')
			Line.resize(Length-1);
		Line += "\r\n";
	}
	
	char *FileName;

public:
	list<VEvent *> Events;
};

class ICalendar::Query {
public:
	Query(ICalendar *Calendar): Calendar(Calendar), EventsIterator(Calendar->Events.begin()) {}
	~Query() { for_each(RecurrentEvents.begin(), RecurrentEvents.end(), DeleteItem()); }
	void ResetPosition() {
	  	for_each(RecurrentEvents.begin(), RecurrentEvents.end(), DeleteItem());
	  	RecurrentEvents.clear();
	  	EventsIterator = Calendar->Events.begin();
	}
	VEvent* GetNextEvent(bool WithAlarm = false);
	
	EventsCriteria Criteria;
	
private:
	ICalendar *Calendar;
	list<VEvent *> RecurrentEvents;
	list<VEvent *>::iterator EventsIterator;
};

inline TimeUnit ConvertFrequency(string Name) {
	if (Name == "SECONDLY")
		return SECOND;
	if (Name == "MINUTELY")
		return MINUTE;
	if (Name == "HOURLY")
		return HOUR;
	if (Name == "DAILY")
		return DAY;
	if (Name == "WEEKLY")
		return WEEK;
	if (Name == "MONTHLY")
		return MONTH;
	
	return YEAR;
}

inline AlarmAction ConvertAlarmAction(string Name) {
	if (Name == "AUDIO")
		return AUDIO;
	if (Name == "PROCEDURE")
		return PROCEDURE;
	if (Name == "EMAIL")
		return EMAIL;
	
	return DISPLAY;
}

// int StringToInt(const string& str) {
// 	stringstream stream(str);
// 	int i;
// 	stream>>i;
// 	return i;
// }
#endif // _ICALENDAR_H
