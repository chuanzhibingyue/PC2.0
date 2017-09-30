#pragma once

#include <list>
#include <map>

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace uc {
	template<class _Ty>
	class LckList : public std::list<_Ty>
	{
	public:
		LckList() {}

	public:
		boost::mutex &getLck() { return m_mutex; }

	private:
		boost::mutex m_mutex;
	};

	template<class _Kty, class _Ty>
	class LckMap : public std::map<_Kty, _Ty>
	{
	public:
		LckMap() {}

	public:
		boost::mutex &getLck() { return m_mutex; }

	private:
		boost::mutex m_mutex;
	};

#define AUTO_LOCK(al, lck_ptr) AutoLock al(lck_ptr)

	class AutoLock
	{
	public:
		AutoLock(boost::mutex &obj) : m_lck(obj){ }
		~AutoLock(){}

	private:
		boost::mutex::scoped_lock m_lck;
	};

	// 递归式的互斥量 boost::mutex::scoped_lock
	// boost::recursive_mutex提供一个递归式的互斥量。对于一个实例最多允许一个线程拥有其锁定，
	// 如果一个线程已经锁定一个boost::recursive_mutex实例，那么这个线程可以多次锁定这个实例。
	// 
	// boost::mutex io_mutex;
	// void foo( )
	// {
	// 	{
	// 		boost::mutex::scoped_lock lock( io_mutex );         /// 锁定
	// 	} /// 解锁
	// }

	// 读写锁的实现
	typedef boost::shared_lock<boost::shared_mutex> readLock;
	typedef boost::unique_lock<boost::shared_mutex> writeLock;
	// 对同一个rwmutex，线程可以同时有多个readLock，这些readLock会阻塞任意一个
	// 企图获得writeLock的线程，直到所有的readLock对象都析构。
	// 如果writeLock首先获得了rwmutex，那么它会阻塞任意一个企图在rwmutex上
	// 获得readLock或者writeLock的线程。
	// boost::shared_mutex  rwmutex;
	// void readOnly( ){
	// 	readLock  rdlock( rwmutex );
	// 	/// do something
	// }
	// 
	// void writeOnly( ){
	// 	writeLock  wtlock( rwmutex );
	// 	/// do something
	// }

	// 互斥锁的实现
	typedef boost::unique_lock<boost::mutex> exclusiveLock;

}