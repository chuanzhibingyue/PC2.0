#include "UCMockClient.h"
#include "UCErrorCode.h"
#include "ICalendarService.h"
#include <boost/thread.hpp>
#include "LogManager.h"
#include "StringConvert.h"
#include "ClientInfo.h"
#include "UCCommonFunctionMock.h"
#include "CalendarServiceTest.h"
#include "CalendarServiceMockClient.h"


CalendarServiceTest::CalendarServiceTest() { 
	_ucEngine = NULL;
}

void CalendarServiceTest::SetUp() {
	cout <<"\r\n"<<endl;
	_info.account = "jie.shen@quanshi.com";
	_info.password = "redapple";
	_info.clientType = UCCLIENT_WIN;
	_info.mac = "6c:40:08:9f:ba:ca";
	_info.clientId = "3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba";
	_info.pinNumber = "E40D42D2-4C88-47D0-8029-03DEEC649312";
	_info.version = "1.9.590.16577";
	_info.storeType = "0";
	_info.phoneNumber = "1234567890";
	_info.newPass = "buildbuild";
	_info.confirmPass = "buildbuild";

	UCEngineConfig ucConfig;
	CreateEngineConfig(ucConfig);		 
	_ucEngine = CreateUCEngine(ucConfig);
}

void CalendarServiceTest::TearDown() {	
	DestroyUCEngine();
	_ucEngine = NULL;
}

void CalendarServiceTest::SetUpTestCase(){

}
void CalendarServiceTest::TearDownTestCase(){

}


TEST_F(CalendarServiceTest, CreateConferenceSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

// 	CalendarServiceMockClient calendarMock;
// 	calendarMock.Init(_ucEngine);

	//参会人
	std::vector<Attendee> attendees;
	Attendee attendee;
	attendee.SetAccount("1228650");
	attendees.push_back(attendee);
	attendee.SetAccount("1189278");
	attendees.push_back(attendee);
	attendee.SetAccount("399610");
	attendees.push_back(attendee);
	attendee.SetAccount("217705");
	attendees.push_back(attendee);
	//会议标题
	std::string title = "这是会议title";
	//会议地点
	std::string location = "这是会议location";
	//会议摘要
	std::string summary = "这是会议summary";
	//时区
	std::string timezone = "8";
	int64_t startTime =1436995000;
	int32_t length = 60;
	//calendarMock.CreateConference(attendees, title, location, summary, timezone, startTime, length, 1, 0);

	//calendarMock.UnInit();
	mockClient.UnInit();
}