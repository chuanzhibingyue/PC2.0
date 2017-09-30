#ifndef UCMOCKBASE_H_
#define UCMOCKBASE_H_

#include <boost/thread.hpp>
#include "boost/thread/recursive_mutex.hpp"

class UCMockBase {
public:
	UCMockBase();
	virtual ~UCMockBase();

public:
	void NotifyResult(int32_t resultCode);
	void WaitAssertEQ(int32_t expected);

private:
	int32_t _resultCode;
	boost::recursive_mutex _methodMtx;
	boost::condition_variable_any _methodCod;
};

#endif