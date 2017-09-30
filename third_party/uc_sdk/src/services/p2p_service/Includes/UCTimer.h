#pragma once

#include "ThreadPool.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace uc {

	class ITimerSink{
	public:
		virtual void onTimerProc() = 0;
	};

	class UCTimer
	{
	public:
		UCTimer(void) : _threadPool(1) { m_lpSink = NULL; }
		virtual ~UCTimer(void) {}

	public:
		void setSink(ITimerSink *lp) { m_lpSink = lp; }
		ITimerSink* getSink() { return m_lpSink; }

		int getInterval() { return _interval; }

		void startTimer(int interval = 1){

			_interval = interval;
			_threadPool.Init();
			_threadPool.Schedule(MakeCallback(this, &UCTimer::threadProc));
		}

		void stopTimer(){

			_timer->cancel();
			_iosrv.stop();

			_threadPool.Wait();
			_threadPool.Shutdown();
		}

	protected:
		void threadProc() {
			LOGI("threadProc");

			_timer = new boost::asio::deadline_timer(_iosrv);

			size_t result = _timer->expires_from_now(boost::posix_time::seconds(_interval));
			_timer->async_wait(boost::bind(&UCTimer::timerProc, 
				boost::ref(*_timer)/*, boost::ref(_interval)*/, this));

			_iosrv.run();
		}

		static void timerProc(boost::asio::deadline_timer &timer/*, int &interval*/, UCTimer *lpTimer) {

			if (NULL != lpTimer && NULL != lpTimer->getSink())
			{
				lpTimer->getSink()->onTimerProc();
			}

			timer.expires_from_now(boost::posix_time::seconds(lpTimer->getInterval()));
			timer.async_wait(boost::bind(&UCTimer::timerProc, 
				boost::ref(timer)/*, boost::ref(interval)*/, lpTimer));
		}

	private:
		int _interval;
		ThreadPool _threadPool;
		boost::asio::deadline_timer *_timer;
		boost::asio::io_service _iosrv;

		ITimerSink *m_lpSink;
	};
}

