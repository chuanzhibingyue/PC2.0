/*
 * TaskExecutorCallback.h
 *
 *  Created on: 2015年6月12日
 *      Author: gang wang
 */

#ifndef TASK_EXECUTOR_CALLBACK_H_
#define TASK_EXECUTOR_CALLBACK_H_


#include "HTTPRequestCallback.h"
#include "utils/TaskExecutor.h"
#include "UCErrorCode.h"

namespace uc {

#define ON_REQUEST_COMPLETE(CALLBACK)						\
if (_response != NULL) {									\
	delete _response;										\
}															\
_errorCode = errorCode;										\
															\
if (response != NULL) {										\
	_response = new HTTPResponse(*response);				\
}															\
															\
Callback* cb = new CALLBACK(*this);				\
															\
_response = NULL;											\
TaskExecutor::GetInstance().Schedule(cb);					

/**
 *无输入参数的callback类
 */
template<typename C>
class HTTPRequestCallbackT : public RequestCallback,
								public Callback {
public:
	typedef void (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response);
	HTTPRequestCallbackT(C *ptr, ClassFunc func) : _ptr(ptr), _func(func), _errorCode(UC_FAILED), _response(NULL) {}

	virtual void operator()() {
		(_ptr->*_func)(_errorCode, _response);
	}

	virtual ~HTTPRequestCallbackT() {
		if (_response != NULL) {
			delete _response;
			_response = NULL;
		}
	}

public:
	virtual void OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		ON_REQUEST_COMPLETE(HTTPRequestCallbackT);
	}

private:
	C *_ptr;
	ClassFunc _func;
	int32_t _errorCode;
	HTTPResponse *_response;
};

/**
 * 允许一个参数的callback类
 */
template<typename C, typename T1>
class HTTPRequestCallbackT1 : public RequestCallback,
									public Callback{
public:
	typedef void (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1);
	HTTPRequestCallbackT1(C *ptr, ClassFunc func, T1 arg1) : _ptr(ptr),
			_func(func), _arg1(arg1), _errorCode(UC_FAILED), _response(NULL){}

	virtual ~HTTPRequestCallbackT1() {
		if (_response != NULL) {
			delete _response;
			_response = NULL;
		}
	}

	virtual void operator()() {
		(_ptr->*_func)(_errorCode, _response, _arg1);
	}

public:
	virtual void OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		ON_REQUEST_COMPLETE(HTTPRequestCallbackT1);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	int32_t _errorCode;
	HTTPResponse *_response;
};

template<typename C, typename T1, typename T2>
class HTTPRequestCallbackT2 : public RequestCallback,
								public Callback{
public:
	typedef void (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2);
	HTTPRequestCallbackT2(C *ptr, ClassFunc func, T1 arg1, T2 arg2) : _ptr(ptr),
			_func(func), _arg1(arg1), _arg2(arg2), _errorCode(UC_FAILED), _response(NULL) {}

	virtual ~HTTPRequestCallbackT2() {
		if (_response != NULL) {
			delete _response;
			_response = NULL;
		}
	}

	virtual void operator()() {
		(_ptr->*_func)(_errorCode, _response, _arg1, _arg2);
	}

public:
	virtual void OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		ON_REQUEST_COMPLETE(HTTPRequestCallbackT2);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	int32_t _errorCode;
	HTTPResponse *_response;
};

template<typename C, typename T1, typename T2, typename T3>
class HTTPRequestCallbackT3 : public RequestCallback ,
									public Callback{
public:
	typedef void (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3);
	HTTPRequestCallbackT3(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3) : _ptr(ptr),
			_func(func), _arg1(arg1), _arg2(arg2), _arg3(arg3), _errorCode(UC_FAILED), _response(NULL) {}

	virtual ~HTTPRequestCallbackT3() {
		if (_response != NULL) {
			delete _response;
			_response = NULL;
		}
	}

	virtual void operator()() {
		(_ptr->*_func)(_errorCode, _response, _arg1, _arg2, _arg3);
	}

public:
	virtual void OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		ON_REQUEST_COMPLETE(HTTPRequestCallbackT3);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
	int32_t _errorCode;
	HTTPResponse *_response;
};

template<typename C, typename T1, typename T2, typename T3, typename T4>
class HTTPRequestCallbackT4 : public RequestCallback,
									public Callback{

public:
	typedef void (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4);
	HTTPRequestCallbackT4(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4) : _ptr(ptr),
			_func(func), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), _errorCode(UC_FAILED), _response(NULL) {}

	virtual ~HTTPRequestCallbackT4() {
		if (_response != NULL) {
			delete _response;
			_response = NULL;
		}
	}

	virtual void operator()() {
		(_ptr->*_func)(_errorCode, _response, _arg1, _arg2, _arg3, _arg4);
	}

public:
	virtual void OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		ON_REQUEST_COMPLETE(HTTPRequestCallbackT4);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
	T4 _arg4;
	int32_t _errorCode;
	HTTPResponse *_response;
};

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
class HTTPRequestCallbackT5 : public RequestCallback,
									public Callback{

public:
	typedef void (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);
	HTTPRequestCallbackT5(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) : _ptr(ptr),
			_func(func), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), _arg5(arg5), _errorCode(UC_FAILED), _response(NULL) {}

	virtual ~HTTPRequestCallbackT5() {
		if (_response != NULL) {
			delete _response;
			_response = NULL;
		}
	}

	virtual void operator()() {
		(_ptr->*_func)(_errorCode, _response, _arg1, _arg2, _arg3, _arg4, _arg5);
	}

public:
	virtual void OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		ON_REQUEST_COMPLETE(HTTPRequestCallbackT5);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
	T4 _arg4;
	T5 _arg5;
	int32_t _errorCode;
	HTTPResponse *_response;
};

/**
 * 创建无参数的callback类
 * 回调函数定义例子: void OnActionComplete(int32_t errorCode, HTTPResponse *response);
 * @param ptr
 * @param func
 * @return
 */
template <typename C>
HTTPRequestCallbackT<C> *MakeRequestCallbackT(C *ptr, void (C::*func)(int32_t errorCode, HTTPResponse *response)) {
	return new HTTPRequestCallbackT<C>(ptr, func);
}

/**
 * 创建一个参数的callback类
 * 回调函数定义例子: void OnActionComplete(int32_t errorCode, HTTPResponse *response, string userData);
 * @param ptr
 * @param func
 * @param arg1
 * @return
 */
template <typename C, typename T1>
HTTPRequestCallbackT1<C, T1> *MakeRequestCallbackT(C *ptr, void (C::*func)(int32_t errorCode,
		HTTPResponse *response, T1 arg1), T1 arg1) {
	return new HTTPRequestCallbackT1<C, T1>(ptr, func, arg1);
}

template <typename C, typename T1, typename T2>
HTTPRequestCallbackT2<C, T1, T2> *MakeRequestCallbackT(C *ptr, void (C::*func)(int32_t errorCode,
		HTTPResponse *response, T1 arg1, T2 arg2), T1 arg1, T2 arg2) {
	return new HTTPRequestCallbackT2<C, T1, T2>(ptr, func, arg1, arg2);
}

template <typename C, typename T1, typename T2, typename T3>
HTTPRequestCallbackT3<C, T1, T2, T3> *MakeRequestCallbackT(C *ptr, void (C::*func)(int32_t errorCode,
		HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3), T1 arg1, T2 arg2, T3 arg3) {
	return new HTTPRequestCallbackT3<C, T1, T2, T3>(ptr, func, arg1, arg2, arg3);
}

template <typename C, typename T1, typename T2, typename T3, typename T4>
HTTPRequestCallbackT4<C, T1, T2, T3, T4> *MakeRequestCallbackT(C *ptr, void (C::*func)(int32_t errorCode,
		HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return new HTTPRequestCallbackT4<C, T1, T2, T3, T4>(ptr, func, arg1, arg2, arg3, arg4);
}

template <typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
HTTPRequestCallbackT5<C, T1, T2, T3, T4, T5> *MakeRequestCallbackT(C *ptr, void (C::*func)(int32_t errorCode,
		HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5), T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) {
	return new HTTPRequestCallbackT5<C, T1, T2, T3, T4, T5>(ptr, func, arg1, arg2, arg3, arg4, arg5);
}

} /* namespace uc */


#endif
