/*
* UCFile.h
*
*  Created on: 2015-9-7
*      Author: hualong.zhang
*/

#ifndef RESIDENT_THREAD_H_
#define RESIDENT_THREAD_H_

#include <string>
#include <list>
#include "boost/thread/mutex.hpp"
#include "boost/thread.hpp"


namespace uc {

	using namespace std;

	template <typename C, typename T>
	class ResidentThread {
	public:
		typedef void(C::*ClassFunc)(T arg);
		ResidentThread(C *ptr, ClassFunc func) : _ptr(ptr), _func(func) {}

		virtual ~ResidentThread() {}

		void Start() {
			_shutDown = false;
			_thread = boost::thread(std::bind(&ResidentThread::SubThreadProc, this));
		}

		void Stop() {
			boost::unique_lock<boost::recursive_mutex> lock(_mtx);
			_shutDown = true;
			_tasks.clear();
			_taskCond.notify_all();

			lock.unlock();
			_thread.join();
		}

		bool PushTask(const T &downloadTask) {
			boost::unique_lock<boost::recursive_mutex> lock(_mtx);
			if (_shutDown) {
				return false;
			}
			_tasks.push_back(downloadTask);
			_taskCond.notify_all();
			return true;
		}
	private:
		void SubThreadProc() {
			while (!_shutDown) {
				boost::unique_lock<boost::recursive_mutex> lock(_mtx);
				while (_tasks.empty() && (!_shutDown)) {
					_taskCond.wait(lock);
				}

				auto iter = _tasks.begin();
				if (iter != _tasks.end())
				{
					T t = *iter;
					_tasks.pop_front();
					lock.unlock();
					(_ptr->*_func)(t);
				}
			}
		}
		boost::recursive_mutex _mtx;
		boost::condition_variable_any _taskCond;
		boost::thread _thread;
		bool _shutDown;
		std::list<T> _tasks;
		C *_ptr;
		ClassFunc _func;
	};

} /* namespace uc */
#endif /* RESIDENT_THREAD_H_ */
