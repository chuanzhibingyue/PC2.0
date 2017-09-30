#include "timer.h"

namespace uc {

Timer Timer::_instance;

Timer::Timer() {	
}

Timer::~Timer() {
}
    
void Timer::Start() {
    _nextTimerId = 1;
	_shutDown = false;
	_thread = std::thread(std::bind(&Timer::SubThreadProc, this));
}

bool Timer::Schedule(Callback *callback, uint64_t delay, uint64_t interval, timer_id *id) {
	return Schedule(callback, delay, true, interval, id);
}

bool Timer::Schedule(Callback *callback, uint64_t delay, timer_id *id) {
	return Schedule(callback, delay, false, 0, id);
}

bool Timer::Schedule(Callback *callback, uint64_t delay, bool isPeriodic,
	uint64_t interval, timer_id *id) {
	std::shared_ptr<TimerTask> task(new TimerTask());
	_idmtx.lock();
	*id = _nextTimerId++;
	_idmtx.unlock();
	task->id = *id;
	int64_t currTime = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();
	task->nextRun = currTime + delay;
	task->callback = callback;
	task->canelled = false;

	if (isPeriodic) 
	{
		if (interval > 0) 
		{
			task->interval = interval;
		}
		else 
		{
			return false;
		}
	}

	task->isPeriodic = isPeriodic;
	
	return PushTask(task);
}

bool Timer::PushTask(std::shared_ptr<TimerTask> task) {
	std::unique_lock<std::recursive_mutex> lock(_mtx);

	if (_shutDown) {
		return false;
	}

	_taskMap[task->id] = task;
	_taskQueue.insert(task);
	_taskCond.notify_all();

	return true;
}

void Timer::Shutdown() {
	std::unique_lock<std::recursive_mutex> lock(_mtx);
	_shutDown = true;
	_taskQueue.clear();
	_taskMap.clear();


	lock.unlock();
	_taskCond.notify_all();
	_thread.join();
}

void Timer::Cancel(timer_id id) {
	std::unique_lock<std::recursive_mutex> lock(_mtx);

	TimerTaskMap::iterator iter = _taskMap.find(id);
	if (iter != _taskMap.end()) {
		iter->second->canelled = true;
		_taskQueue.erase(iter->second);
		_taskMap.erase(id);

		_taskCond.notify_all();
	}

}

void Timer::SubThreadProc() {
	while (!_shutDown) {
		std::unique_lock<std::recursive_mutex> lock(_mtx);
		while (_taskQueue.empty()) {
			_taskCond.wait(lock);
			if (_shutDown)
				return;
		}
		
		TaskQueue::iterator iter = _taskQueue.begin();
		if (iter != _taskQueue.end()) {
			
			std::shared_ptr<TimerTask> task = *iter;
			_taskQueue.erase(iter);
			
			lock.unlock();
			
			int64_t currTime = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count();
			int64_t sleeTime = task->nextRun - currTime;
			if (sleeTime <= 0 && !task->canelled) {
				if (task->callback != NULL) {
					(*task->callback)();
				}
				task->nextRun += task->interval;
			}
			
			if ((sleeTime > 0) || (task->isPeriodic && !task->canelled)) {
				lock.lock();
				_taskQueue.insert(task);
				lock.unlock();
			}
			else {
				_taskMap.erase(task->id);
			}

			if (sleeTime > 0 && !_shutDown) {
				std::chrono::milliseconds ms(sleeTime);
				lock.lock();
				_taskCond.wait_for(lock, ms);
			}
		} 
		else
		{
			lock.unlock();
		}
	}
}

}
 
