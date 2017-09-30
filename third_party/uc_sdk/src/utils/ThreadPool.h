
#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <deque>
#include <vector>

#include "boost/function.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread.hpp"
#include "boost/smart_ptr/shared_ptr.hpp"
#include "common/Callback.h"

namespace uc {

class ThreadPool {

	class Worker {
	public:
		class WorkerFunctor{
		public:
			WorkerFunctor(Worker* worker) : _worker(worker){
			}
		    void operator()() {
		    	_worker->RunInternal();
		    }
		private:
		    Worker* _worker;
		};

		friend class WorkerFunctor;
		Worker(ThreadPool *pool) : _pool(pool), _thread(NULL) {
		}

		~Worker() {
			if (_thread != NULL) {
				delete _thread;
			}
		}

		void Run() {
			if (_pool != NULL){
				WorkerFunctor func(this);
				_thread = new boost::thread(func);
			}
		}

		void Join() {
			if (_thread->joinable()) {
				_thread->join();
			}
		}

	private:
		void RunInternal() {
			while (_pool->Execute())
				;
		}
	
		ThreadPool *_pool;
		boost::thread *_thread; 
	};

public:
	ThreadPool(int poolSize);
	~ThreadPool();

	void Init();

	/**
	 * task队列中是否有任务在等待被执行
	 */
	bool Empty() {return _tasks.empty(); }

	/**
	 * 返回线程池中线程数量
	 */
	int Size(){return _poolSize; }

	/**
	 * 往队列中压入一个task
	 */
	bool Schedule(Callback *task);

	/**
	 * 等待所有任务都已经执行结束
	 */
	void Wait();

	/**
	 * 关闭线程池：退出所有worker线程，清除所有等待任务
	 */
	void Shutdown();

	/**
	 * 清除所有等待的任务
	 */
	void Clear();

private:
	/**
	 * worker线程的执行函数
	 */
	bool Execute();

private:
	int _poolSize;

	std::vector<Worker*> _workers;
	int _busyWorkers;
	boost::recursive_mutex _workerMtx;


	std::deque<Callback *> _tasks;
	boost::recursive_mutex _mtx;
    boost::condition_variable_any _taskCond;
	boost::condition_variable_any _idleCond;

	bool _shutdown;
};

}
#endif
