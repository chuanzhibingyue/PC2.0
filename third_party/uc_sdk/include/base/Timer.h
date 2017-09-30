/*
 * timer.h
 *
 *  Created on: August 28, 2015
 *      Author: gang wang
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <map>
#include <set>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "common/Callback.h"
#include "common/platform.h"


namespace uc {	

typedef int64_t timer_id;

class UC_API Timer {
public:
    struct TimerTask
    {
        timer_id id;                        // timer id
        int64_t nextRun;					// 下一次执行的时间
        int64_t interval;					// 周期执行的间隔
        Callback *callback;                // 回调
        bool canelled;						// 是否已经被取消
        bool isPeriodic;                   // 是否是周期任务

		~TimerTask() {
			if (callback != NULL) {
				delete callback;
				callback = NULL;
			}
		}
    };
    
	static Timer& GetInstance() {return _instance;}
	
    ~Timer();
	
    /**
     * 启动Timer
     */
	void Start();
	
	/**
	 * 分配一个周期任务
	 * @param callback 定时执行的任务
	 * @param delay 第一次执行等待的时间（ms）
	 * @param interval 周期执行的间隔 （ms）
	 * @param id 定时器id
	 * @return 成功或失败
	 */
	bool Schedule(Callback *callback, uint64_t delay, uint64_t interval, timer_id *id);
	
	/**
	 * 分配一次性的执行任务
	 * @param callback 定时执行的任务
	 * @param delay 执行等待的时间（ms）
	 * @param id 定时器id
	 * @return 成功或失败
	 */
	bool Schedule(Callback *callback, uint64_t delay, timer_id *id);

	/**
	 * 销毁定时器，取消所有等待中的任务
	 */
	void Shutdown();
	
	/**
	 * 取消timer id对应的定时任务
	 */
	void Cancel(timer_id id);
	
private:
	Timer();
	void SubThreadProc();
	bool Schedule(Callback *callback, uint64_t delay, bool isPeriodic, uint64_t interval, timer_id *id);
	bool PushTask(std::shared_ptr<TimerTask> task);

private:
	struct TimerTaskComparator {
		bool operator()(const std::shared_ptr<TimerTask> &a, 
			const std::shared_ptr<TimerTask> &b) const {
			if(a->nextRun == b->nextRun) {
				return a->id < b->id;
			}
			else {
				return a->nextRun < b->nextRun;
			}
		}
	};

	static Timer _instance;
		
	typedef std::map<timer_id, std::shared_ptr<TimerTask> > TimerTaskMap;
	TimerTaskMap _taskMap;
	
	typedef std::set<std::shared_ptr<TimerTask>, TimerTaskComparator> TaskQueue;
	TaskQueue _taskQueue;
		
	std::thread _thread;
	std::recursive_mutex _mtx;
    std::condition_variable_any _taskCond;
	
	bool _shutDown;
	
	// 下次分配的定时器id
	std::recursive_mutex _idmtx;
	volatile timer_id _nextTimerId;
};
    
}

#endif /* defined(__mmflash_macosx__timer__) */
