#include "ThreadPool.h"

#include "boost/foreach.hpp"

namespace uc {

ThreadPool::ThreadPool(int poolSize) : _poolSize(poolSize), _busyWorkers(0), 
	_shutdown(false) {
}

ThreadPool::~ThreadPool() {
	if (!_shutdown) {
		Shutdown();
	}
}

void ThreadPool::Init() {
	boost::unique_lock<boost::recursive_mutex> lock(_workerMtx);

	for (int i = 0 ; i < _poolSize; i++) {
		Worker *worker = new Worker(this);
		worker->Run();

		_workers.push_back(worker);
	}
}

bool ThreadPool::Execute() {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);

	_idleCond.notify_all();
	while (!_shutdown && _tasks.empty()) {
		_taskCond.wait(lock);
	}

	if (!_shutdown) {
		_busyWorkers++;

		Callback* task = _tasks.front();
		_tasks.pop_front();

		lock.unlock();
		if (task != NULL) {
			(*task)();
			delete task;
		}

		_busyWorkers--;
		return true;
	}
    else
	{
		return false;
    }
}

bool ThreadPool::Schedule(Callback *task) {
	if (_poolSize == 0 || _shutdown) {
		return false;
	}

	boost::unique_lock<boost::recursive_mutex> lock(_mtx);

    _tasks.push_back(task);
    _taskCond.notify_one();

    return true;
}

void ThreadPool::Wait() {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	while (_busyWorkers != 0 || !_tasks.empty()) {
		_idleCond.wait(lock);
	}
}

void ThreadPool::Shutdown() {
	_shutdown = true;

	_taskCond.notify_all();

	boost::unique_lock<boost::recursive_mutex> lock(_workerMtx);

	BOOST_FOREACH(Worker *worker, _workers) {
		if (worker != NULL) {
			worker->Join();
			delete worker;
		}
	}
	_workers.clear();

	Clear();
}


void ThreadPool::Clear() {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	BOOST_FOREACH(Callback *task, _tasks) {
		if (task != NULL) {
			delete task;
		}
	}
	_tasks.clear();
}

}