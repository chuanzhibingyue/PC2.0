/*
 * TaskExecutor.h
 *
 *  Created on: June 9, 2015
 *      Author: gang wang
 */
#ifndef TASK_EXECUTOR_H_
#define TASK_EXECUTOR_H_

#include "ThreadPool.h"

namespace uc {

class TaskExecutor {
public:
	static TaskExecutor& GetInstance() {
		return _instance;
	}

	void Init();

	bool Schedule(Callback *task);

	void Shutdown();

private:
	TaskExecutor();
	TaskExecutor(const TaskExecutor &);

	static TaskExecutor _instance;

	ThreadPool _pool;
};

}
#endif
