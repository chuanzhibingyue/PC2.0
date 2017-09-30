#include "gtest/gtest.h"
#include <string>
#include "platform.h"
#include "boost/thread.hpp"
#include "ThreadMessage.h"


#ifdef PLATFORM_WIN
#include <windows.h>
#elif defined(PLATFORM_IOS) || defined (PLATFORM_MAC)
#include <CoreFoundation/CoreFoundation.h>
#endif


using namespace uc;

static int value = 0;

class ThreadMessageTest : public ::testing::Test {
public:
	ThreadMessageTest(){}
	virtual ~ThreadMessageTest() {}

protected:
	virtual void SetUp() {
		value = 0;
	}

	virtual void TearDown() {

	}

public:

};

#if defined(PLATFORM_IOS) || defined (PLATFORM_MAC)
static CFRunLoopRef threadRunLoopRef = CFRunLoopGetCurrent();

static void RunMessageLoop() {
	CFRunLoopRun();
}

static void StopMessageLoop() {
	threadRunLoopRef = CFRunLoopGetCurrent();
	CFRunLoopStop(threadRunLoopRef);
}

#elif defined(PLATFORM_WIN)

static void RunMessageLoop() {
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

static void StopMessageLoop() {
	PostQuitMessage(0);
}

#endif

#define TEST_MSG_ID 65558
#define TEST_QUIT_MSG_ID 65559

class TestMessageProc : public MessageProc
{
public:
    virtual bool onMessage(int msgID, long param1, long param2) {

		if (msgID == TEST_MSG_ID){
			value++;
			return true;
		}
		else if (msgID == TEST_QUIT_MSG_ID){
			StopMessageLoop();
			return true;
		}
		return false;
	}
};


void post(int time)
{
	int i = 0;
	while (i++ < time) {
		PostMainThreadMessage(TEST_MSG_ID, 0, 0);
	}

	PostMainThreadMessage(TEST_QUIT_MSG_ID, 0, 0);
}

TEST_F(ThreadMessageTest, checkMessageReceived) {

	AttachMainThreadMessageProc(new TestMessageProc());

	boost::thread _thread(post, 100);

	RunMessageLoop();

	DetachMainThreadMessageProc();

	EXPECT_EQ(100, value);
    
    boost::thread _thread2(post, 10);
    EXPECT_EQ(100, value);
}

