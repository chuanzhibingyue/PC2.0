#include "icalendar.h"
#include <iostream>

void ICalendar::LoadFromFile() {
	string Line, NextLine;
	Component CurrentComponent = VCALENDAR, PrevComponent = VCALENDAR;
	VEvent *NewEvent = NULL;
	Alarm NewAlarm;
	// for getting some UIDs for events without them
	unsigned int NoUID = 0;
	fstream File;

	File.open(FileName, ios::in | ios::binary);
	if (!File.is_open()) {
		File.clear();
		File.open(FileName, ios::out | ios::binary | ios::trunc);
		File << "BEGIN:VCALENDAR\r\n";
		File << "VERSION:2.0\r\n";
		File << "PRODID:-//Juliusz Gonera//NONSGML remind.me//EN\r\n";
		File << "END:VCALENDAR\r\n";
		File.close();

		return;
	}

	getline(File, NextLine);

	while (!File.eof()) {
		Line = NextLine;
		// lines can be wrapped after 75 octets so we may have to unwrap them
		for (;;) {
			getline(File, NextLine);
			if (NextLine[0] != '\t' && NextLine[0] != ' ')
				break;
			Line += NextLine.substr(1);
		}

		switch (CurrentComponent) {
			case VCALENDAR:
				if (Line.find("BEGIN:VEVENT") == 0) {
					NewEvent = new VEvent;
					CurrentComponent = VEVENT;
				}
				break;

			case VEVENT:
				if (Line.find("UID") == 0) {
					NewEvent->UID = GetProperty(Line);
				} else if (Line.find("SUMMARY") == 0) {
					NewEvent->Summary = GetProperty(Line);
				} else if (Line.find("DTSTAMP") == 0) {
					NewEvent->DtStamp = GetProperty(Line);
				} else if (Line.find("DTSTART") == 0) {
					NewEvent->DtStart = GetProperty(Line);
				} else if (Line.find("DTEND") == 0) {
					NewEvent->DtEnd = GetProperty(Line);
				} else if (Line.find("DESCRIPTION") == 0) {
					NewEvent->Description = GetProperty(Line);
				} else if (Line.find("CATEGORIES") == 0) {
					NewEvent->Categories = GetProperty(Line);
				} else if (Line.find("RRULE") == 0) {
					NewEvent->RRule.Freq = ConvertFrequency(GetSubProperty(Line, "FREQ"));
					NewEvent->RRule.Interval = atoi(GetSubProperty(Line, "INTERVAL").c_str());
					if (NewEvent->RRule.Interval == 0)
						NewEvent->RRule.Interval = 1;
					NewEvent->RRule.Count = atoi(GetSubProperty(Line, "COUNT").c_str());
					NewEvent->RRule.Until = GetSubProperty(Line, "UNTIL");
				}else if (Line.find("LAST-MODIFIED") == 0) {
					NewEvent->DtLastModify = GetProperty(Line);
				} else if (Line.find("LOCATION") == 0) {
					NewEvent->Location = GetProperty(Line);
				} else if (Line.find("CLASS") == 0) {
					NewEvent->Class = GetProperty(Line);
				} else if (Line.find("STATUS") == 0) {
					NewEvent->Status = GetProperty(Line);
				} else if (Line.find("X-MOZILLA-ALARM-DEFAULT-LENGTH") == 0) {
					NewEvent->MozillaAlarmDefaultLength = GetProperty(Line);
				} else if (Line.find("X-WR-EVENTID") == 0) {
					NewEvent->EventId = GetProperty(Line);
				} else if (Line.find("X-WR-CONFERENCEID") == 0) {
					NewEvent->ConferenceId = GetProperty(Line);
				} else if (Line.find("X-WR-ISGNET") == 0) {
					NewEvent->IsGnet = GetProperty(Line);
				} else if (Line.find("X-WR-HOSTNAME") == 0) {
					NewEvent->HostName = GetProperty(Line);
				} else if (Line.find("X-WR-HOSTID") == 0) {
					NewEvent->HostId = GetProperty(Line);
				} else if (Line.find("X-WR-PCODE1") == 0) {
					NewEvent->PCode1 = GetProperty(Line);
				} else if (Line.find("X-WR-PCODE2") == 0) {
					NewEvent->PCode2 = GetProperty(Line);
				} else if (Line.find("X-WR-GROUP-ID") == 0) {
					NewEvent->GroupId = GetProperty(Line);
				} else if (Line.find("X-WR-BIllINGCODE") == 0) {
					NewEvent->BillingCode = GetProperty(Line);
				} else if (Line.find("ATTENDEE") == 0) {
					//ATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"399610\":MAILTO:youremailhere
					string str = GetSubProperty(Line, "ROLE");
					//非主持人,即参会人
					if (str.compare("CHAIR") != 0) {
						VAttendee attendee;
						str = GetSubProperty(Line, "PARTSTAT");
						if (str.compare("NEEDS-ACTION") == 0) {
							attendee.Status = 0;
						} else if (str.compare("ACCEPTED")) {
							attendee.Status = 1;
						} else if (str.compare("DECLINED")) {
							attendee.Status = 2;
						} else {
							attendee.Status = 0;
						}

						str = GetAttendeeAcount(Line);
						if (str == "") {
							str = GetSubProperty(Line, "MAILTO");
							attendee.Account = str;
							attendee.Type = 1;
						}
						else{
							int i = str.find(',');
							if (i != -1)
							{
								int a = 0;
								while (i != -1 && i-a>2) {
									attendee.Account = str.substr(a, i-1);
									attendee.Type = 0;
									NewEvent->Attendees.push_back(attendee);
									a = i;
									i = str.find(',');
								}
							} else {
								attendee.Account = str;
								attendee.Type = 0;
								NewEvent->Attendees.push_back(attendee);
							}
						}
					}
				} else if (Line.find("BEGIN:VALARM") == 0) {
					NewAlarm.Clear();
					PrevComponent = CurrentComponent;
					CurrentComponent = VALARM;
				} else if (Line.find("END:VEVENT") == 0) {
					if (NewEvent->UID.empty())
						NewEvent->UID = NoUID++;

					Events.push_back(NewEvent);
					CurrentComponent = VCALENDAR;
				}
				break;

			case VALARM:
				if (Line.find("ACTION") == 0) {
					NewAlarm.Action = ConvertAlarmAction(GetProperty(Line));
				} else if (Line.find("TRIGGER") == 0) {
					NewAlarm.Trigger = GetProperty(Line);
				} else if (Line.find("DESCRIPTION") == 0) {
					NewAlarm.Description = GetProperty(Line);
				} else if (Line.find("END:VALARM") == 0) {
					NewEvent->Alarms.push_back(NewAlarm);
					CurrentComponent = PrevComponent;
				}
				break;
		}
	}

	File.close();
}


void ICalendar::LoadICalendarStr( const std::string& str )
{
	string Line, NextLine;
	Component CurrentComponent = VCALENDAR, PrevComponent = VCALENDAR;
	VEvent *NewEvent = NULL;
	Alarm NewAlarm;
	// for getting some UIDs for events without them
	unsigned int NoUID = 0;

	bool bFromUCAS = false;
	int begin = 0;
	int pos = str.find("\r\n", 0);
	if (pos == std::string::npos) {
		pos = str.find('\n');
		bFromUCAS = true;
	}

	while(pos != string::npos) {
		string Line = str.substr(begin, pos-begin);	

		for (;;) {
			if (!bFromUCAS) {
				begin = pos + 2;
				pos = str.find("\r\n", begin);
			}
			else {
				begin = pos + 1;
				pos = str.find('\n', begin);
			}
			string nextLine = str.substr(begin, pos - begin);

			if (nextLine[0] != '\t' && nextLine[0] != ' ')
				break;

			Line += nextLine.substr(1);
		}
	
		switch (CurrentComponent) {
			case VCALENDAR:
				if (Line.find("BEGIN:VEVENT") == 0) {
					NewEvent = new VEvent;
					CurrentComponent = VEVENT;
				}
				break;

			case VEVENT:
				if (Line.find("UID") == 0) {
					NewEvent->UID = GetProperty(Line);
				} else if (Line.find("SUMMARY") == 0) {
					NewEvent->Summary = GetProperty(Line);
				} else if (Line.find("DTSTAMP") == 0) {
					NewEvent->DtStamp = GetProperty(Line);
				} else if (Line.find("DTSTART") == 0) {
					NewEvent->DtStart = GetProperty(Line);
				} else if (Line.find("DTEND") == 0) {
					NewEvent->DtEnd = GetProperty(Line);
				} else if (Line.find("DESCRIPTION") == 0) {
					NewEvent->Description = GetProperty(Line);
				} else if (Line.find("CATEGORIES") == 0) {
					NewEvent->Categories = GetProperty(Line);
				} else if (Line.find("RRULE") == 0) {
					NewEvent->RRule.Freq = ConvertFrequency(GetSubProperty(Line, "FREQ"));
					NewEvent->RRule.Interval = atoi(GetSubProperty(Line, "INTERVAL").c_str());
					if (NewEvent->RRule.Interval == 0)
						NewEvent->RRule.Interval = 1;
					NewEvent->RRule.Count = atoi(GetSubProperty(Line, "COUNT").c_str());
					NewEvent->RRule.Until = GetSubProperty(Line, "UNTIL");
				}else if (Line.find("LAST-MODIFIED") == 0) {
					NewEvent->DtLastModify = GetProperty(Line);
				} else if (Line.find("LOCATION") == 0) {
					NewEvent->Location = GetProperty(Line);
				} else if (Line.find("CLASS") == 0) {
					NewEvent->Class = GetProperty(Line);
				} else if (Line.find("STATUS") == 0) {
					NewEvent->Status = GetProperty(Line);
				} else if (Line.find("X-MOZILLA-ALARM-DEFAULT-LENGTH") == 0) {
					NewEvent->MozillaAlarmDefaultLength = GetProperty(Line);
				} else if (Line.find("X-WR-EVENTID") == 0) {
					NewEvent->EventId = GetProperty(Line);
				} else if (Line.find("X-WR-CONFERENCEID") == 0) {
					NewEvent->ConferenceId = GetProperty(Line);
				} else if (Line.find("X-WR-ISGNET") == 0) {
					NewEvent->IsGnet = GetProperty(Line);
				} else if (Line.find("X-WR-WHOLETYPE") == 0) {
					NewEvent->IsWholeDay = GetProperty(Line);
				} else if (Line.find("X-WR-HOSTNAME") == 0) {
					NewEvent->HostName = GetProperty(Line);
				} else if (Line.find("X-WR-HOSTID") == 0) {
					NewEvent->HostId = GetProperty(Line);
				} else if (Line.find("X-WR-PCODE1") == 0) {
					NewEvent->PCode1 = GetProperty(Line);
				} else if (Line.find("X-WR-PCODE2") == 0) {
					NewEvent->PCode2 = GetProperty(Line);
				} else if (Line.find("X-WR-GROUP-ID") == 0) {
					NewEvent->GroupId = GetProperty(Line);
				} else if (Line.find("X-WR-ROOMID") == 0) {
					NewEvent->RoomId = GetProperty(Line);
				} else if (Line.find("X-WR-BIllINGCODE") == 0) {
					NewEvent->BillingCode = GetProperty(Line);
				} else if (Line.find("X-WR-UPD-FLAG") == 0) {
					NewEvent->UpdateFlag = GetProperty(Line);
				} else if (Line.find("X-WR-EXTRASINFO") == 0) {
					NewEvent->ExtrasInfo = GetProperty(Line);
				} else if (Line.find("ATTENDEE") == 0) {
					//ATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"399610\":MAILTO:youremailhere
					//ATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN="399610,121345,134124,":MAILTO:youremailhere
					string str = GetSubProperty(Line, "ROLE");
					//非主持人,即参会人
					if (str.compare("CHAIR") != 0) {
						VAttendee attendee;
						str = GetSubProperty(Line, "PARTSTAT");
						if (str.compare("NEEDS-ACTION") == 0) {
							attendee.Status = 0;
						} else if (str.compare("ACCEPTED") == 0) {
							attendee.Status = 1;
						} else if (str.compare("DECLINED") == 0) {
							attendee.Status = 2;
						} else {
							attendee.Status = 0;		//默认为未处理状态
						}

						str = GetAttendeeAcount(Line);					
						if (str == "") {
							str = GetSubProperty(Line, "MAILTO");
							attendee.Account = str;
							attendee.Type = 2;
						}
						else {
							int pos = str.find(',');
							int beg = 0;
							int status = attendee.Status;
							if (pos != -1) {
								while (pos != -1) {
									attendee.Account = str.substr(beg, pos-beg);
									if (string::npos != attendee.Account.find('@')) {
										//邮件账户
										attendee.Type = 2;
									}
									else {
										//内部账户，暂时未考虑电话用户
										attendee.Type = 1;
									}
									NewEvent->Attendees.push_back(attendee);
									beg = pos+1;
									pos = str.find(',', beg);
								}
							}else{
								attendee.Account = str;
								if (string::npos != str.find('@')) {
									//邮件账户
									attendee.Type = 2;
								}
								else {
									//内部账户，暂时未考虑电话用户
									attendee.Type = 1;
								}
								NewEvent->Attendees.push_back(attendee);
							} 
						}
					}
				} else if (Line.find("BEGIN:VALARM") == 0) {
					NewAlarm.Clear();
					PrevComponent = CurrentComponent;
					CurrentComponent = VALARM;
				} else if (Line.find("END:VEVENT") == 0) {
					if (NewEvent->UID.empty())
						NewEvent->UID = NoUID++;

					Events.push_back(NewEvent);
					CurrentComponent = VCALENDAR;
				}
				break;

			case VALARM:
				if (Line.find("ACTION") == 0) {
					NewAlarm.Action = ConvertAlarmAction(GetProperty(Line));
				} else if (Line.find("TRIGGER") == 0) {
					NewAlarm.Trigger = GetProperty(Line);
				} else if (Line.find("DESCRIPTION") == 0) {
					NewAlarm.Description = GetProperty(Line);
				} else if (Line.find("END:VALARM") == 0) {
					NewEvent->Alarms.push_back(NewAlarm);
					CurrentComponent = PrevComponent;
				}
				break;
		}

// 		if (!bFromUCAS ) {
// 			begin = pos+2;
// 			pos = str.find("\r\n", begin);
// 		} else {
// 			begin = pos+1;
// 			pos = str.find('\n', begin);
// 		}
	}
}

/*Event* ICalendar::GetEventByUID(char *UID) {
	for (list<Event *>::iterator Iterator = Events.begin(); Iterator != Events.end(); ++Iterator) {
		if ((*Iterator)->UID.find(UID) == 0) {
			return *Iterator;
		}
	}

	return NULL;
}*/

void ICalendar::AddEvent(VEvent *NewEvent) {
	char Temp[16];
	string Line;
	streamoff Offset;

	NewEvent->DtStamp.SetToNow();
	NewEvent->UID = NewEvent->DtStamp;
	NewEvent->UID += '-';
	sprintf(Temp, "%d", rand());
	NewEvent->UID += Temp;

	Events.push_back(NewEvent);

	// for some reason tellg() modifies the get pointer under Windows if the file
	// is not opened in the binary mode (possibly because of UTF-8?)
	fstream File(FileName, ios::in | ios::out | ios::binary);

	do {
		Offset = File.tellg();
		getline(File, Line);
	} while (!File.eof() && Line.find("END:VCALENDAR") != 0);
	File.seekp(Offset, ios::beg);

	File << *NewEvent;
	File << "END:VCALENDAR\r\n";

	File.close();
}

void ICalendar::DeleteEvent(VEvent *DeletedEvent) {
	fstream File;
	string Data, Line, PartialData;
	unsigned int Length;
	bool Copy = true, Deleted = false;

	File.open(FileName, ios::in | ios::binary);
	File.seekg(0, ios::end);
	Length = File.tellg();
	File.seekg(0, ios::beg);

	// to avoid reallocating memory
	Data.reserve(Length);

	while (!File.eof()) {
		getline(File, Line);

		Length = Line.length();
		if (Length <= 1)
			continue;

		// getline() removes only '\n' from the end of the line (not '\r')
		FixLineEnd(Line, Length);

		if (Line.find("BEGIN:VEVENT") == 0) {
			Copy = false;
			Deleted = false;
			PartialData = "";
		} else if (Line.find("UID:") == 0 && Line.find(DeletedEvent->UID) == 4) {
			Deleted = true;
		}

		if (Copy == true)
			Data += Line;
		else if (Deleted == false)
			PartialData += Line;

		if (Line.find("END:VEVENT") == 0) {
			Copy = true;

			if (Deleted == false)
				Data += PartialData;
		}
	}

	File.close();
	File.clear();

	// again problems in non-binary mode - "\r\n" changed to "\r\r\n"
	File.open(FileName, ios::out | ios::binary | ios::trunc);
	File << Data;
	File.close();

	for (list<VEvent *>::iterator Iterator = Events.begin(); Iterator != Events.end();) {
		if (*Iterator == DeletedEvent) {
			delete *Iterator;
			Iterator = Events.erase(Iterator);
		} else
			++Iterator;
	}
}

void ICalendar::ModifyEvent(VEvent *ModifiedEvent) {
	fstream File;
	string Data, Line, PartialData;
	unsigned int Length;
	bool Copy = true, Modified = false;

	File.open(FileName, ios::in | ios::binary);
	File.seekg(0, ios::end);
	Length = File.tellg();
	File.seekg(0, ios::beg);

	// we will probably need at least such amount of memory
	Data.reserve(Length);

	while (!File.eof()) {
		getline(File, Line);

		Length = Line.length();
		if (Length <= 1)
			continue;

		// getline() removes only '\n' from the end of the line (not '\r')
		FixLineEnd(Line, Length);

		if (Line.find("BEGIN:VEVENT") == 0) {
			Copy = false;
			Modified = false;
			PartialData = "";
		} else if (Line.find("UID:") == 0 && Line.find(ModifiedEvent->UID) == 4) {
			Modified = true;
			Data += *ModifiedEvent;
		}

		if (Copy == true)
			Data += Line;
		else if (Modified == false)
			PartialData += Line;

		if (Line.find("END:VEVENT") == 0) {
			Copy = true;

			if (Modified == false)
				Data += PartialData;
		}
	}

	File.close();
	File.clear();

	// again problems in non-binary mode - "\r\n" changed to "\r\r\n"
	File.open(FileName, ios::out | ios::binary | ios::trunc);
	File << Data;
	File.close();
}


/// ICalendar::Query

VEvent* ICalendar::Query::GetNextEvent(bool WithAlarm) {
	static VEvent *RecurrentEvent = NULL;
	/* not all events have DtEnd, but we need some DtEnd for various checks,
	   so we will use this for temporary DtEnd derived from DtStart (following
	   RFC 2445, 4.6.1) */
	Date DtEnd;
	unsigned long Difference;
	unsigned short Rest;

	if (RecurrentEvent != NULL) {
		RecurrentEvent->DtStart[RecurrentEvent->RRule.Freq] += RecurrentEvent->RRule.Interval;
		if (!RecurrentEvent->DtEnd.IsEmpty())
			RecurrentEvent->DtEnd[RecurrentEvent->RRule.Freq] += RecurrentEvent->RRule.Interval;
		++RecurrentEvent->RecurrenceNo;

		if (
		(!WithAlarm &&
		RecurrentEvent->DtStart <= Criteria.To &&
		(RecurrentEvent->RRule.Until.IsEmpty() || RecurrentEvent->RRule.Until >= RecurrentEvent->DtStart) &&
		(RecurrentEvent->RRule.Count == 0 || RecurrentEvent->RecurrenceNo < RecurrentEvent->RRule.Count)) ||
		(WithAlarm && RecurrentEvent->HasAlarm(Criteria.From, Criteria.To))
		) {
			RecurrentEvents.push_back(new VEvent(*RecurrentEvent));
			return RecurrentEvents.back();
		}

		delete RecurrentEvent;
		RecurrentEvent = NULL;
	}

	if (RecurrentEvent == NULL) {
		for (; EventsIterator != Calendar->Events.end(); ++EventsIterator) {
			if ((*EventsIterator)->DtEnd.IsEmpty()) {
				DtEnd = (*EventsIterator)->DtStart;
				if ((*EventsIterator)->DtStart.WithTime == false)
					++DtEnd[DAY];
			} else {
				DtEnd = (*EventsIterator)->DtEnd;
			}

			if (
			Criteria.AllEvents == true || (
			!WithAlarm &&
			// DtEnd is non-inclusive (according to RFC 2445)
			(DtEnd > Criteria.From || (*EventsIterator)->DtStart >= Criteria.From) &&
			(*EventsIterator)->DtStart <= Criteria.To
			) ||
			(WithAlarm && (*EventsIterator)->HasAlarm(Criteria.From, Criteria.To))
			) {
				if (Criteria.AllEvents == false && Criteria.IncludeRecurrent == true && (*EventsIterator)->RRule.IsEmpty() == false)
					RecurrentEvent = new VEvent(**EventsIterator);
				return *(EventsIterator++);

			} else if (
			(*EventsIterator)->RRule.IsEmpty() == false &&
			(*EventsIterator)->DtStart < Criteria.From &&
			((*EventsIterator)->RRule.Until.IsEmpty() || (*EventsIterator)->RRule.Until >= Criteria.From) &&
			Criteria.IncludeRecurrent == true
			) {
				RecurrentEvent = new VEvent(**EventsIterator);

				Difference = Criteria.From.Difference(DtEnd, RecurrentEvent->RRule.Freq);
				Rest = Difference%RecurrentEvent->RRule.Interval;

				if (Rest != 0)
					Difference += RecurrentEvent->RRule.Interval - Rest;
				
				cout << Criteria.From.Format() << endl;
				cout << DtEnd.Format() << endl;
				cout << Difference << endl;
				RecurrentEvent->DtStart[RecurrentEvent->RRule.Freq] += Difference;
				DtEnd[RecurrentEvent->RRule.Freq] += Difference;
				RecurrentEvent->RecurrenceNo = Difference / RecurrentEvent->RRule.Interval;

				// <= because DtEnd is non-inclusive (according to RFC 2445)
				while (DtEnd <= Criteria.From) {
					RecurrentEvent->DtStart[RecurrentEvent->RRule.Freq] += RecurrentEvent->RRule.Interval;
					DtEnd[RecurrentEvent->RRule.Freq] += RecurrentEvent->RRule.Interval;
					++RecurrentEvent->RecurrenceNo;
				}

				if (
				(!WithAlarm &&
				RecurrentEvent->DtStart <= Criteria.To &&
				// < because DtStart counts as the first occurence
				(RecurrentEvent->RRule.Count == 0 || RecurrentEvent->RecurrenceNo < RecurrentEvent->RRule.Count)) ||
				(WithAlarm && RecurrentEvent->HasAlarm(Criteria.From, Criteria.To))
				) {
					++EventsIterator;
					if (!RecurrentEvent->DtEnd.IsEmpty())
						RecurrentEvent->DtEnd = DtEnd;
					RecurrentEvents.push_back(new VEvent(*RecurrentEvent));
					return RecurrentEvents.back();
				}

				delete RecurrentEvent;
				RecurrentEvent = NULL;
			}
		}
	}

	return NULL;
}
