#ifndef GROUP_SERVICE_TEST_H_
#define GROUP_SERVICE_TEST_H_
#include <string>
#include "gtest/gtest.h"
#include "IUCEngine.h"
#include "ClientInfo.h"


using namespace std;
using namespace uc;

class GroupServiceTest : public ::testing::Test {
public:
	GroupServiceTest();
	virtual ~GroupServiceTest() {}
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