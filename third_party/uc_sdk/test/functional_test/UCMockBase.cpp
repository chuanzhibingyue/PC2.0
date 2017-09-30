#include "UCMockBase.h"
#include "UCErrorCode.h"
#include "gtest/gtest.h"

UCMockBase::UCMockBase() {
	_resultCode = UC_FAILED;
}

UCMockBase::~UCMockBase() {
}

void UCMockBase::NotifyResult(int32_t resultCode) {
	_methodMtx.lock();
	_resultCode = resultCode;
	_methodCod.notify_one();
	_methodMtx.unlock();
	std::cout << "NotifyResult" << std::endl;
}

void UCMockBase::WaitAssertEQ(int32_t expected) {
	boost::unique_lock<boost::recursive_mutex> lock(_methodMtx);

	_methodCod.wait(lock);
	std::cout << "WaitAssertEQ: " << _resultCode << std::endl;
	ASSERT_EQ(expected, _resultCode);
	lock.unlock();
}

