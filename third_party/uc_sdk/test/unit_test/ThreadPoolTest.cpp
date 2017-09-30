#include "gtest/gtest.h"
#include "utils/ThreadPool.h"

static int value = 0;
static void increase() {
	value++;
}

static void workLongTime() {
	int i = 0;
	
	while (i++ < 10000) {
		int j = 0;
		while (j++ < 10000) {
		}
	}
	
	value++;
}

class ThreadPoolTest : public ::testing::Test {
public:
	ThreadPoolTest() : _threadPool(NULL){}
	virtual ~ThreadPoolTest() {}

protected:
	virtual void SetUp() {
		value = 0;
	}

	virtual void TearDown() {
		if (_threadPool != NULL) {
			delete _threadPool;
			_threadPool = NULL;
		}
	}

public:
	uc::ThreadPool *_threadPool;
};


TEST_F(ThreadPoolTest, CheckTaskExecuted) {
	_threadPool = new uc::ThreadPool(3);
	_threadPool->Init();

	//uc::TaskFunc task = &increase;

	int i = 0;
	while (i < 10) {
		uc::Callback0* cb = new uc::Callback0(increase);
		_threadPool->Schedule(cb);
		i++;
	}

	_threadPool->Wait();
	_threadPool->Shutdown();

	EXPECT_EQ(10, value);
}


TEST_F(ThreadPoolTest, CheckCannotScheduleAfterShutdown) {
	_threadPool = new uc::ThreadPool(2);
	_threadPool->Init();

	_threadPool->Wait();
	_threadPool->Shutdown();

	uc::Callback0* cb = new uc::Callback0(increase);
	//uc::TaskFunc task = &increase;
	bool res = _threadPool->Schedule(cb);

	EXPECT_EQ(false, res);
}

TEST_F(ThreadPoolTest, CheckWaitForLongTimewWork) {
	_threadPool = new uc::ThreadPool(2);
	_threadPool->Init();

	//uc::TaskFunc task = &workLongTime;
	int i = 0;
	while (i < 10) {
		uc::Callback0* cb = new uc::Callback0(workLongTime);
		_threadPool->Schedule(cb);
		i++;
	}

	_threadPool->Wait();
	_threadPool->Shutdown();

	EXPECT_EQ(10, value);
}
