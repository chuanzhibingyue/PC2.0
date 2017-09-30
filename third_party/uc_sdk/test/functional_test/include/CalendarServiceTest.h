#ifndef CALENDAR_SERVICE_TEST_H_
#define CALENDAR_SERVICE_TEST_H_
#include <string>
#include "gtest/gtest.h"
#include "IUCEngine.h"
#include "ClientInfo.h"

using namespace uc;

class CalendarServiceTest : public ::testing::Test {
public:
	CalendarServiceTest();
	virtual ~CalendarServiceTest() {}
public:
	static void SetUpTestCase();
	static void TearDownTestCase();
	void SetUp();
	void TearDown();	
public:
	IUCEngine *_ucEngine;
	ClientInfo _info;
};

#endif