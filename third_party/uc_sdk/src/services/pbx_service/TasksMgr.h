#pragma once

#include <map>
#include "common/platform.h"
#include <boost/thread/recursive_mutex.hpp>
#include <boost/atomic/atomic.hpp>

namespace uc {

	class PbxTask : public AC_PbxCallRecord {
	public:
		static int32_t MakeTaskID() {
			static boost::atomic_int taskId(1);
			return ((int32_t)time(NULL) + taskId++);
		}

	public:
		PbxTask(int32_t taskID, bool isIncoming) {
			_taskID = taskID;

			_isIncoming = isIncoming;
			_isHold = false;
			_isMute = false;
			_startTime = 0;
		}

		virtual ~PbxTask() {}

	public:
		int32_t GetTaskID() { return _taskID; }

		int setHold(bool obj) {
			if (_isHold == obj) {
				return EC_OPERATE;
			}

			if (CALL_STATE_CONFIRMED != GetStateEx()) {
				return EC_STATE;
			}

			_isHold = obj;

			return 0;
		}

		int setMute(bool obj) {
			if (_isMute == obj) {
				return EC_OPERATE;
			}

			if (CALL_STATE_CONFIRMED != GetStateEx()) {
				return EC_STATE;
			}

			_isMute = obj;

			return 0;
		}

		void Calling() {
			_startTime = time(NULL);
		}

		bool CallDruation(){
			if (_startTime <= 0) {
				return false;
			}

			int64_t end = time(NULL);
			int druation = (int)(end - _startTime);
			if (druation <= 0) {
				return false;
			}

			SetDruation(druation);

			return true;
		}

	public:
		bool _isIncoming;

		bool _isHold;
		bool _isMute;

		int64_t _startTime;

		std::string postValue;
		std::string callUUID;

	private:
		int32_t _taskID;
	};

	class PbxTasks{
	public:
		PbxTasks() {}
		virtual ~PbxTasks() {}

	public:
		shared_ptr<PbxTask> GetTaskByID(int32_t taskID) {
			boost::unique_lock<boost::recursive_mutex> lock(_mtx);

			auto iter = _tasks.find(taskID);
			if (iter == _tasks.end()) {
				UC_LOG(ERROR) << "GetTask: taskID=" << taskID << " does not exist";
				cout << "GetTask: taskID=" << taskID << " does not exist" << endl;
				return NULL;
			}

			return iter->second;
		}

		shared_ptr<PbxTask> GetTaskByNumber(const std::string &number) {
			boost::unique_lock<boost::recursive_mutex> lock(_mtx);

			for (auto iter : _tasks) {
				if (0 == iter.second->GetNumber().compare(number)) {
					return iter.second;
				}
			}

			UC_LOG(ERROR) << "GetTask: number=" << number << " does not exist";
			cout << "GetTask: number=" << number << " does not exist" << endl;
			return NULL;
		}

		shared_ptr<PbxTask> GetTaskByCallID(int callID) {
			boost::unique_lock<boost::recursive_mutex> lock(_mtx);

			for (auto iter : _tasks) {
				if (callID == iter.second->GetCallid()) {
					return iter.second;
				}
			}

			UC_LOG(ERROR) << "GetTask: callID=" << callID << " does not exist";
			cout << "GetTask: callID=" << callID << " does not exist" << endl;
			return NULL;
		}

		bool IsExist(int32_t taskID) {
			return (NULL != GetTaskByID(taskID));
		}

		bool IsExist(const std::string &number) {
			return (NULL != GetTaskByNumber(number));
		}

		void AddTask(shared_ptr<PbxTask> task) {
			if (NULL == task) {
				return;
			}

			boost::unique_lock<boost::recursive_mutex> lock(_mtx);
			_tasks.insert(make_pair(task->GetTaskID(), task));

			UC_LOG(ERROR) << "Add Task: taskID=" << task->GetTaskID() 
				<< ", tasks count=" << _tasks.size();
			cout << "Add Task: taskID=" << task->GetTaskID() 
				<< ", tasks count=" << _tasks.size() << endl;
		}

		shared_ptr<PbxTask> RemoveTask(int32_t taskID) {
			boost::unique_lock<boost::recursive_mutex> lock(_mtx);

			auto iter = _tasks.find(taskID);
			if (iter == _tasks.end()) {
				return NULL;
			}

			UC_LOG(ERROR) << "Remove Task: taskID=" << taskID << ", tasks count=" << _tasks.size();
			cout << "Remove Task: taskID=" << taskID << ", tasks count=" << _tasks.size() << endl;
			shared_ptr<PbxTask> task = iter->second;
			_tasks.erase(iter);

			return task;
		}

		int GetTasksCount() {
			boost::unique_lock<boost::recursive_mutex> lock(_mtx);

			return _tasks.size();
		}

		void Clear() {
			boost::unique_lock<boost::recursive_mutex> lock(_mtx);

			_tasks.clear();

			lock.unlock();
		}

	private:
		std::map<int32_t, shared_ptr<PbxTask>> _tasks;
		boost::recursive_mutex _mtx;
	};

}