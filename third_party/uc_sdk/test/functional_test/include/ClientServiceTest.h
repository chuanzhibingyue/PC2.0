#ifndef CLIENT_SERVICE_TEST_H_
#define CLIENT_SERVICE_TEST_H_
#include <string>
#include "IUCEngine.h"
#include "ClientInfo.h"

#include "gtest/gtest.h"

using namespace std;
using namespace uc;

class ClientServiceTest : public ::testing::Test {
public:
	ClientServiceTest();
	virtual ~ClientServiceTest() {}
public:
	static void SetUpTestCase();
	static void TearDownTestCase();
	void SetUp();
	void TearDown();	
public:
	IUCEngine *ucEngine;
	ClientInfo info;
};

#endif