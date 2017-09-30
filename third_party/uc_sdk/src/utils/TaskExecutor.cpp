#include "TaskExecutor.h"
#include "CommonDef.h"

namespace uc {

TaskExecutor TaskExecutor::_instance;

TaskExecutor::TaskExecutor() : _pool(EXECUTOR_POOL_SIZE){
}

void TaskExecutor::Init() {
	_pool.Init();
}

bool TaskExecutor::Schedule(Callback *task) {
	return _pool.Schedule(task);
}

void TaskExecutor::Shutdown() {
	_pool.Shutdown();
}


}
