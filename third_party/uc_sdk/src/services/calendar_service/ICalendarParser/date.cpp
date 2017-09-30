#include "date.h"
#include <sstream>

#if defined(WIN32) || defined(WIN64)
#define snprintf _snprintf
#endif

string Date::Format() const {
	memset((char*)buffer, 0, sizeof(buffer));
	sprintf((char*)buffer, "%.4d/%.2d/%.2d", Data[YEAR], Data[MONTH], Data[DAY]);
	if (WithTime == true)
		snprintf((char*)buffer+10, 5, " %.2d:%.2d", Data[HOUR], Data[MINUTE]);
	return string((char*)buffer);
}

Date::operator string() const {
	memset((char*)buffer, 0, sizeof(buffer));
	sprintf((char*)buffer, "%.4d%.2d%.2d", Data[YEAR], Data[MONTH], Data[DAY]);
	if (WithTime == true)
		sprintf((char*)buffer+8, "T%.2d%.2d%.2d", Data[HOUR], Data[MINUTE], Data[SECOND]);
	return string((char*)buffer);
}

Date &Date::operator =(const string &Text) {
	if (Text.length() >= 8) {
		sscanf(Text.c_str(), "%4hd%2hd%2hd", &Data[YEAR], &Data[MONTH], &Data[DAY]);
		
		if (Text.length() >= 15) {
			sscanf(Text.c_str()+9, "%2hd%2hd%2hd", &Data[HOUR], &Data[MINUTE], &Data[SECOND]);
			WithTime = true;
		} else {
			Data[HOUR] = Data[MINUTE] = Data[SECOND] = 0;
			WithTime = false;
		}
	} else
		Data[YEAR] = Data[MONTH] = Data[DAY] = 0;
	return *this;
}

// difference *this - OtherDate
unsigned long Date::Difference(Date &OtherDate, TimeUnit Unit, bool RoundUp) const {
	unsigned long Result = 0;
	
	switch (Unit) {
		case YEAR:
			Result = Data[YEAR] - OtherDate.Data[YEAR];
			break;
			
		case MONTH:
			Result = (Data[YEAR] - OtherDate.Data[YEAR])*12 + Data[MONTH] - OtherDate.Data[MONTH];
			break;
			
		case DAY:
			if (OtherDate.Data[YEAR] == Data[YEAR]) {
				if (OtherDate.Data[MONTH] == Data[MONTH]) {
					Result = Data[DAY] - OtherDate.Data[DAY];
				} else {
					Result = (OtherDate.DaysInMonth() - OtherDate.Data[DAY]) + Data[DAY];
					
					for (char i=OtherDate.Data[MONTH]+1; i<Data[MONTH]; ++i)
						Result += OtherDate.DaysInMonth(i);
				}
			} else {
				// number of days in the starting year
				Result = OtherDate.DaysInMonth() - OtherDate.Data[DAY];
				for (char i=OtherDate.Data[MONTH]+1; i<=12; ++i)
					Result += OtherDate.DaysInMonth(i);
				
				// number of days in years between the starting and ending years
				for (short i=OtherDate.Data[YEAR]+1; i<Data[YEAR]; ++i)
					Result += IsLeapYear(i) ? 366 : 365;
				
				// number of days in the ending year
				for (char i=1; i<Data[MONTH]; ++i)
					Result += DaysInMonth(i);
				Result += Data[DAY];
			}
			
			break;
			
		case HOUR:
			if (Data[DAY] == OtherDate.Data[DAY]) {
				Result = Data[HOUR] - OtherDate.Data[HOUR];
			} else {
				Result = 24 - OtherDate.Data[HOUR];
				Result += (Difference(OtherDate, DAY, false) - 1) * 24;
				Result += Data[HOUR];
			}
			break;
			
		case MINUTE:
			if (Data[HOUR] == OtherDate.Data[HOUR]) {
				Result = Data[MINUTE] - OtherDate.Data[MINUTE];
			} else {
				Result = 60 - OtherDate.Data[MINUTE];
				Result += (Difference(OtherDate, HOUR, false) - 1) * 60;
				Result += Data[MINUTE];
			}
			break;
			
		case SECOND:
			if (Data[MINUTE] == OtherDate.Data[MINUTE]) {
				Result = Data[SECOND] - OtherDate.Data[SECOND];
			} else {
				Result = 60 - OtherDate.Data[SECOND];
				Result += (Difference(OtherDate, MINUTE, false) - 1) * 60;
				Result += Data[SECOND];
			}
			break;
			
		case WEEK:
			Result = Difference(OtherDate, DAY, false)/7 + 1;
			break;
	}
	
	if (RoundUp == true && Unit < SECOND && Compare(OtherDate, (TimeUnit)(Unit+1)) > 0)
		++Result;
	return Result;
}

void Date::SetToNow() {
	time_t Timestamp;
	tm *CurrentTime;
	
	time(&Timestamp);
	CurrentTime = localtime(&Timestamp);
	
	Data[YEAR] = CurrentTime->tm_year + 1900;
	Data[MONTH] = CurrentTime->tm_mon + 1;
	Data[DAY] = CurrentTime->tm_mday;
	Data[HOUR] = CurrentTime->tm_hour;
	Data[MINUTE] = CurrentTime->tm_min;
	Data[SECOND] = CurrentTime->tm_sec;
	
	WithTime = true;
}

long long diff_tm(struct tm *a, struct tm *b)
{
	return a->tm_sec - b->tm_sec
		+ 60LL * (a->tm_min - b->tm_min)
		+ 3600LL * (a->tm_hour - b->tm_hour)
		+ 86400LL * (a->tm_yday - b->tm_yday)
		+ (a->tm_year - 70) * 31536000LL
		- (a->tm_year - 69) / 4 * 86400LL
		+ (a->tm_year - 1) / 100 * 86400LL
		- (a->tm_year + 299) / 400 * 86400LL
		- (b->tm_year - 70) * 31536000LL
		+ (b->tm_year - 69) / 4 * 86400LL
		- (b->tm_year - 1) / 100 * 86400LL
		+ (b->tm_year + 299) / 400 * 86400LL;
}

unsigned long Date::ToUnixTime() {
	struct tm time;
	time.tm_year = Data[YEAR] - 1900;
	time.tm_mon = Data[MONTH] - 1;
	time.tm_mday = Data[DAY];
	time.tm_hour = Data[HOUR];
	time.tm_min = Data[MINUTE];
	time.tm_sec = Data[SECOND];

	time_t ret = mktime(&time);
	return ret + diff_tm(&time, gmtime(&ret));
}

Date::DatePart &Date::DatePart::operator +=(short Value) {
	switch (Part) {
		case YEAR:
			BaseDate.Data[YEAR] += Value;
			break;
		
		case MONTH:
			BaseDate.Data[MONTH] += Value;
			BaseDate[YEAR] += (BaseDate.Data[MONTH]-1)/12;
			BaseDate.Data[MONTH] = (BaseDate.Data[MONTH]-1)%12 + 1;
			break;
		
		case DAY:
			BaseDate.Data[DAY] += Value;
			while (BaseDate.Data[DAY] > BaseDate.DaysInMonth()) {
				BaseDate.Data[DAY] -= BaseDate.DaysInMonth();
				BaseDate[MONTH] += 1;
			}
			break;
		
		case HOUR:
			BaseDate.Data[HOUR] += Value;
			BaseDate[DAY] += BaseDate.Data[HOUR]/24;
			BaseDate.Data[HOUR] %= 24;
			break;
		
		case MINUTE:
			BaseDate.Data[MINUTE] += Value;
			BaseDate[HOUR] += BaseDate.Data[MINUTE]/60;
			BaseDate.Data[MINUTE] %= 60;
			break;
		
		case SECOND:
			BaseDate.Data[SECOND] += Value;
			BaseDate[MINUTE] += BaseDate.Data[SECOND]/60;
			BaseDate.Data[SECOND] %= 60;
			break;
		
		case WEEK:
			BaseDate[DAY] += Value*7;
			break;
	}
	return *this;
}

Date::DatePart &Date::DatePart::operator -=(short Value) {
	switch (Part) {
		case YEAR:
			BaseDate.Data[YEAR] -= Value;
			break;
		
		case MONTH:
			BaseDate.Data[MONTH] -= Value;
			if (BaseDate.Data[MONTH] < 1) {
				BaseDate[YEAR] -= -(BaseDate.Data[MONTH]-12)/12;
				BaseDate.Data[MONTH] = 12 + BaseDate.Data[MONTH]%12;
			}
			break;
		
		case DAY:
			BaseDate.Data[DAY] -= Value;
			while (BaseDate.Data[DAY] < 1) {
				BaseDate[MONTH] -= 1;
				BaseDate.Data[DAY] += BaseDate.DaysInMonth();
			}
			break;
		
		case HOUR:
			BaseDate.Data[HOUR] -= Value;
			if (BaseDate.Data[HOUR] < 0) {
				BaseDate[DAY] -= -(BaseDate.Data[HOUR]-24)/24;
				BaseDate.Data[HOUR] = 24 + BaseDate.Data[HOUR]%24;
			}
			break;
		
		case MINUTE:
			BaseDate.Data[MINUTE] -= Value;
			if (BaseDate.Data[MINUTE] < 0) {
				BaseDate[HOUR] -= -(BaseDate.Data[MINUTE]-60)/60;
				BaseDate.Data[MINUTE] = 60 + BaseDate.Data[MINUTE]%60;
			}
			break;
		
		case SECOND:
			BaseDate.Data[SECOND] -= Value;
			if (BaseDate.Data[SECOND] < 0) {
				BaseDate[MINUTE] -= -(BaseDate.Data[SECOND]-60)/60;
				BaseDate.Data[SECOND] = 60 + BaseDate.Data[SECOND]%60;
			}
			break;
		
		case WEEK:
			BaseDate[DAY] -= Value*7;
			break;
	}
	return *this;
}
