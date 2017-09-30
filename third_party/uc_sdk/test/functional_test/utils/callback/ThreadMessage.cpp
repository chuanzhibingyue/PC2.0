#include "ThreadMessage.h"
#include "platform.h"

#ifdef PLATFORM_WIN
#include <windows.h>
#elif defined(PLATFORM_IOS) || defined (PLATFORM_MAC)
#include <CoreFoundation/CoreFoundation.h>
#include "boost/thread/mutex.hpp"
#endif

#include <queue>


#if defined(PLATFORM_IOS) || defined (PLATFORM_MAC) || defined (PLATFORM_ANDROID)
struct ThreadMessage
{
    ThreadMessage(int msgId = 0, long param1 = 0, long param2 = 0)
        : _msgId(msgId), _param1(param1), _param2(param2){}

    int _msgId;
    long _param1, _param2;
};


class MessageQueue {
public:
    void FireAll (MessageProc *proc) {
        boost::unique_lock<boost::recursive_mutex> lock(_mtx);
        while (!_msgs.empty()) {
            ThreadMessage msg = _msgs.front();
            _msgs.pop();
            
            lock.unlock();
            
            if (proc != NULL) {
                proc->onMessage(msg._msgId, msg._param1, msg._param2);
            }
            
            lock.lock();
            
        }
    }

    void Push(int msgId, long param1, long param2) {
        ThreadMessage msg(msgId, param1, param2);
        boost::unique_lock<boost::recursive_mutex> lock(_mtx);
        _msgs.push(msg);

    }

    void Clear() {
        boost::unique_lock<boost::recursive_mutex> lock(_mtx);
        std::queue<ThreadMessage> emptyQueue;
        std::swap(_msgs, emptyQueue);
    }

    std::queue<ThreadMessage> _msgs;
    boost::recursive_mutex _mtx;
};
#endif

#ifdef PLATFORM_WIN

bool fireCallback(int msgId, long param1, long param2);

class MessageManagerInternal {
public:
    MessageManagerInternal(){
    }
    
    bool AttachMainThreadMessageProc(MessageProc *proc) {
        WNDCLASSA wc = {0, WndProc, 0, 0, 0, 0, 0, 0, 0, "ucsdk_test_thread_message"};
        ATOM atom = ::RegisterClassA(&wc);
        if (atom == NULL) {
            return false;
        }

        if (!::IsWindow(_hwnd)) {
            _hwnd = ::CreateWindowA((const char *)atom, "ucsdk_test_thread_message_window",
                                    WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                    HWND_MESSAGE, NULL, NULL,0);
        }
        
        return true;
    }
    
    bool PostMainThreadMessage(int msg, long param1, long param2) {
        if (!::IsWindow(_hwnd) || ::PostMessageA(_hwnd, msg, param1, param2) != TRUE) {
            return false;
        }
        
        return true;
    }
    
    void DetachMainThreadMessageProc() {
        if (::IsWindow(_hwnd)) {
            ::DestroyWindow(_hwnd);
            _hwnd = NULL;
        }
        UnregisterClassA("ucsdk_test_thread_message_window", NULL);
    }
    
private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msgId, WPARAM wParam, LPARAM lParam) {
        if (fireCallback(msgId, wParam, lParam)) {
            return 0;
        }
        
        return ::DefWindowProc(hWnd, msgId, wParam, lParam);
    }
    
    HWND _hwnd;
};

#elif defined(PLATFORM_IOS) || defined (PLATFORM_MAC)

static void ThreadMessageCallBack (void *);
    
class MessageManagerInternal {
public:
	bool AttachMainThreadMessageProc(MessageProc *proc) {
		CFRunLoopSourceContext context = {0, 0, 0, 0, 0, 0, 0, 0, 0, ThreadMessageCallBack};

		/*
		_source = CFRunLoopSourceCreate(kCFAllocatorDefault, 0, &context);
		if (_source) {
			_runloop = CFRunLoopGetCurrent();
			CFRunLoopAddSource(_runloop, _source, kCFRunLoopDefaultMode);

		    return true;
		}*/

		return false;
	}

	bool PostMainThreadMessage(int msgId, long param1, long param2) {
		/*
		CFRunLoopSourceSignal(_source);
		if (CFRunLoopIsWaiting(_runloop)) {
			CFRunLoopWakeUp(_runloop);
		}*/
		return true;
	}

	void DetachMainThreadMessageProc() {
		/*
		CFRunLoopRemoveSource(_runloop, _source, kCFRunLoopDefaultMode);
		CFRelease(_source);*/
	}

private:
	CFRunLoopSourceRef _source;
	CFRunLoopRef _runloop;
};

    
#elif defined(PLATFORM_ANDROID)

const int SIGNAL_MSG = 0x10101010;
static void ThreadMessageCallBack (void *);

int LooperCallBack(int fd, int events, void* data)
{
    int sig = 0;
    read(fd, &sig, sizeof(int));
    if (sig == SIGNAL_MSG)
    {
        ThreadMessageCallBack(NULL);
    }

    return 1;
}

class MessageManagerInternal {
public:
    MessageManagerInternal() : looper(NULL) {
    }

	bool AttachMainThreadMessageProc() {
        looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
        if (looper != NULL) {
            // Acquire a reference on the given ALooper object
            ALooper_acquire(looper);

            _msgPipe[0] = 0;
            _msgPipe[1] = 0;

            if (pipe(_msgPipe) == -1) {
                ALooper_release(looper);
                return false;
            }

            if (fcntl(_msgPipe[0], F_SETFL, O_NONBLOCK)) {
                ALooper_release (looper);
                close(_msgPipe[0]);
                close(_msgPipe[1]);
                return false;
            }

            if (fcntl(_msgPipe[1], F_SETFL, O_NONBLOCK)) {
                ALooper_release(looper);
                close(_msgPipe[0]);
                close(_msgPipe[1]);
                return false;
            }

            // Adds read pipe fd to be polled by the looper.
            if (1 != ALooper_addFd(looper, _msgPipe[0], ALOOPER_POLL_CALLBACK,
                ALOOPER_EVENT_INPUT, LooperCallBack, 0)) {
                ALooper_release(looper);
                close(_msgPipe[0]);
                close(_msgPipe[1]);
                return false;
            }
        }

		return true;
	}

	bool PostMainThreadMessage(int msgId, long param1, long param2) {
        if (NULL == looper || 0 == _msgPipe[1])
            return false;

        ALooper_wake(looper);
        write(_msgPipe[1], &SIGNAL_MSG, sizeof(int));

		return true;
	}
    
	void DetachMainThreadMessageProc() {
        ALooper_removeFd(looper, _msgPipe[0]);

        if (_msgPipe[0] != 0)
            close(_msgPipe[0]);

        if (_msgPipe[1] != 0)
            close(_msgPipe[1]);

        ALooper_release(looper);

        looper = NULL;
	}

private:
    ALooper* looper;
    int _msgPipe[2];
};

#endif

class ThreadMessageManager {
public:
    ThreadMessageManager() : _proc (NULL), _attached(false) {
    }
        
    ~ThreadMessageManager(){
        DetachMainThreadMessageProc();
    }
        
    bool AttachMainThreadMessageProc(MessageProc *proc) {
        if ( _attached || proc == NULL) {
            return false;
        }

        _proc = proc;

        if (_internal.AttachMainThreadMessageProc(proc)) {
            _attached = true;
        }

        return _attached;
    }
        
    bool PostMainThreadMessage(int msg, long param1, long param2) {
        if (!_attached) {
            return false;
        }
        
#if defined(PLATFORM_IOS) || defined (PLATFORM_MAC) || defined (PLATFORM_ANDROID)  
        _mq.Push(msg, param1, param2);
#endif

        return _internal.PostMainThreadMessage(msg, param1, param2);
    }

#if defined(PLATFORM_IOS) || defined (PLATFORM_MAC) || defined (PLATFORM_ANDROID)    
    void fireCallback() {
        _mq.FireAll (_proc);
    }
#endif

    bool fireCallback(int msgId, long param1, long param2) {
        if (_proc != NULL && _proc->onMessage(msgId, param1, param2)) {
            return true;
        }
        return false;
    }

    void DetachMainThreadMessageProc() {
        if (!_attached) {
            return;
        }
        
#if defined(PLATFORM_IOS) || defined (PLATFORM_MAC) || defined (PLATFORM_ANDROID)
        _mq.Clear();
#endif
        
        _internal.DetachMainThreadMessageProc();
        
        _attached = false;
        
        if (_proc != NULL) {
            delete _proc;
            _proc = NULL;
        }
    }
        
private:
    MessageManagerInternal _internal;

#ifndef PLATFORM_WIN    
    MessageQueue _mq;
#endif

    MessageProc *_proc;
    bool _attached;
};
    
static ThreadMessageManager threadMessageManager;

#if defined(PLATFORM_IOS) || defined (PLATFORM_MAC) || defined (PLATFORM_ANDROID)
static void ThreadMessageCallBack (void *) {
    threadMessageManager.fireCallback();
}
#endif

bool fireCallback(int msgId, long param1, long param2) {
    return threadMessageManager.fireCallback(msgId, param1, param2);
}

bool AttachMainThreadMessageProc(MessageProc *proc) {
	return threadMessageManager.AttachMainThreadMessageProc(proc);
}

void DetachMainThreadMessageProc() {
	threadMessageManager.DetachMainThreadMessageProc();
}

bool PostMainThreadMessage(int msg, long param1, long param2) {
	return threadMessageManager.PostMainThreadMessage(msg, param1, param2);
}

