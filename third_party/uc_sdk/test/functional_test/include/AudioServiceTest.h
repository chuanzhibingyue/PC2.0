#ifndef AUDIO_SERVICE_TEST_H_
#define AUDIO_SERVICE_TEST_H_
#include <string>
#include "IUCEngine.h"
//#include "ClientInfo.h"
#include "gtest/gtest.h"

using namespace uc;
using namespace std;

class ClientInfo;
class UCMockClient;
class AudioServiceMockClient;

class AudioServiceTest : public ::testing::Test {
public:
	AudioServiceTest();
	virtual ~AudioServiceTest() {}
public:
	static void SetUpTestCase();
	static void TearDownTestCase();
	void SetUp();
	void TearDown();	
public:
	ClientInfo *_info;
	IUCEngine *_ucEngine;
	UCMockClient *_mockClient;
	AudioServiceMockClient *_service;
};

#endif //AUDIO_SERVICE_TEST_H_