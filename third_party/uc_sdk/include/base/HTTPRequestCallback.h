/*
 * HTTPRequestCallback.h
 *
 *  Created on: 2015年5月26日
 *      Author: jianxue
 */

#ifndef HTTPREQUESTCALLBACK_H_
#define HTTPREQUESTCALLBACK_H_

#include "HTTPResponse.h"

namespace uc {

/**
 * http回调函数基类
 */
class RequestCallback {
public:
	virtual ~RequestCallback() {}
	/**
	 * http的回调函数
	 * @param errorCode
	 * @param response
	 * @return 返回response中code的值,如果response不是有效的则返回-1
	 */
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) = 0;
};

/**
 *无输入参数的callback类
 */
template<typename C>
class HTTPRequestCallback : public RequestCallback {
public:
	typedef int32_t (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response);
	HTTPRequestCallback(C *ptr, ClassFunc func) : _ptr(ptr), _func(func) {}

	virtual ~HTTPRequestCallback() {}

public:
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		return (_ptr->*_func)(errorCode, response);
	}

private:
	C *_ptr;
	ClassFunc _func;
};

/**
 * 允许一个参数的callback类
 */
template<typename C, typename T1>
class HTTPRequestCallback1 : public RequestCallback {
public:
	typedef int32_t (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1);
	HTTPRequestCallback1(C *ptr, ClassFunc func, T1 arg1) : _ptr(ptr), _func(func), _arg1(arg1) {}

	virtual ~HTTPRequestCallback1() {}

public:
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		return (_ptr->*_func)(errorCode, response, _arg1);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
};

template<typename C, typename T1, typename T2>
class HTTPRequestCallback2 : public RequestCallback {
public:
	typedef int32_t (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2);
	HTTPRequestCallback2(C *ptr, ClassFunc func, T1 arg1, T2 arg2) : _ptr(ptr),
			_func(func), _arg1(arg1), _arg2(arg2) {}

	virtual ~HTTPRequestCallback2() {}

public:
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		return (_ptr->*_func)(errorCode, response, _arg1, _arg2);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
};

template<typename C, typename T1, typename T2, typename T3>
class HTTPRequestCallback3 : public RequestCallback {
public:
	typedef int32_t (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3);
	HTTPRequestCallback3(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3) : _ptr(ptr),
			_func(func), _arg1(arg1), _arg2(arg2), _arg3(arg3) {}

	virtual ~HTTPRequestCallback3() {}

public:
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		return (_ptr->*_func)(errorCode, response, _arg1, _arg2, _arg3);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
};

template<typename C, typename T1, typename T2, typename T3, typename T4>
class HTTPRequestCallback4 : public RequestCallback {

public:
	typedef int32_t (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4);
	HTTPRequestCallback4(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4) : _ptr(ptr),
			_func(func), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4) {}

	virtual ~HTTPRequestCallback4() {}

public:
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		return (_ptr->*_func)(errorCode, response, _arg1, _arg2, _arg3, _arg4);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
	T4 _arg4;
};

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
class HTTPRequestCallback5 : public RequestCallback {

public:
	typedef int32_t (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);
	HTTPRequestCallback5(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) : _ptr(ptr),
			_func(func), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), _arg5(arg5) {}

	virtual ~HTTPRequestCallback5() {}

public:
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		return (_ptr->*_func)(errorCode, response, _arg1, _arg2, _arg3, _arg4, _arg5);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
	T4 _arg4;
	T5 _arg5;
};

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class HTTPRequestCallback6 : public RequestCallback {

public:
	typedef int32_t (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6);
	HTTPRequestCallback6(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6) : _ptr(ptr),
		_func(func), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), _arg5(arg5), _arg6(arg6) {}

	virtual ~HTTPRequestCallback6() {}

public:
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		return (_ptr->*_func)(errorCode, response, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
	T4 _arg4;
	T5 _arg5;
	T6 _arg6;
};

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class HTTPRequestCallback7 : public RequestCallback {

public:
	typedef int32_t (C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7);
	HTTPRequestCallback7(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7) : _ptr(ptr),
		_func(func), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), _arg5(arg5), _arg6(arg6), _arg7(arg7) {}

	virtual ~HTTPRequestCallback7() {}

public:
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		return (_ptr->*_func)(errorCode, response, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
	T4 _arg4;
	T5 _arg5;
	T6 _arg6;
	T7 _arg7;
};

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class HTTPRequestCallback8 : public RequestCallback {

public:
	typedef int32_t(C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8);
	HTTPRequestCallback8(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8) : _ptr(ptr),
		_func(func), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), _arg5(arg5), _arg6(arg6), _arg7(arg7), _arg8(arg8) {}

	virtual ~HTTPRequestCallback8() {}

public:
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		return (_ptr->*_func)(errorCode, response, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
	T4 _arg4;
	T5 _arg5;
	T6 _arg6;
	T7 _arg7;
	T8 _arg8;
};

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class HTTPRequestCallback9 : public RequestCallback {

public:
	typedef int32_t(C::*ClassFunc)(int32_t errorCode, HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9);
	HTTPRequestCallback9(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9) : _ptr(ptr),
		_func(func), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), _arg5(arg5), _arg6(arg6), _arg7(arg7), _arg8(arg8), _arg9(arg9) {}

	virtual ~HTTPRequestCallback9() {}

public:
	virtual int32_t OnRequestComplete(int32_t errorCode, HTTPResponse *response) {
		return (_ptr->*_func)(errorCode, response, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8, _arg9);
	}

private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
	T4 _arg4;
	T5 _arg5;
	T6 _arg6;
	T7 _arg7;
	T8 _arg8;
	T9 _arg9;
};

/**
 * 创建无参数的callback类
 * 回调函数定义例子: int32_t OnActionComplete(int32_t errorCode, HTTPResponse *response);
 * @param ptr
 * @param func
 * @return
 */
template <typename C>
HTTPRequestCallback<C> *MakeRequestCallback(C *ptr, int32_t (C::*func)(int32_t errorCode, HTTPResponse *response)) {
	return new HTTPRequestCallback<C>(ptr, func);
}

/**
 * 创建一个参数的callback类
 * 回调函数定义例子: int32_t OnActionComplete(int32_t errorCode, HTTPResponse *response, string userData);
 * @param ptr
 * @param func
 * @param arg1
 * @return
 */
template <typename C, typename T1>
HTTPRequestCallback1<C, T1> *MakeRequestCallback(C *ptr, int32_t (C::*func)(int32_t errorCode,
		HTTPResponse *response, T1 arg1), T1 arg1) {
	return new HTTPRequestCallback1<C, T1>(ptr, func, arg1);
}

template <typename C, typename T1, typename T2>
HTTPRequestCallback2<C, T1, T2> *MakeRequestCallback(C *ptr, int32_t(C::*func)(int32_t errorCode,
		HTTPResponse *response, T1 arg1, T2 arg2), T1 arg1, T2 arg2) {
	return new HTTPRequestCallback2<C, T1, T2>(ptr, func, arg1, arg2);
}

template <typename C, typename T1, typename T2, typename T3>
HTTPRequestCallback3<C, T1, T2, T3> *MakeRequestCallback(C *ptr, int32_t(C::*func)(int32_t errorCode,
		HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3), T1 arg1, T2 arg2, T3 arg3) {
	return new HTTPRequestCallback3<C, T1, T2, T3>(ptr, func, arg1, arg2, arg3);
}

template <typename C, typename T1, typename T2, typename T3, typename T4>
HTTPRequestCallback4<C, T1, T2, T3, T4> *MakeRequestCallback(C *ptr, int32_t(C::*func)(int32_t errorCode,
		HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return new HTTPRequestCallback4<C, T1, T2, T3, T4>(ptr, func, arg1, arg2, arg3, arg4);
}

template <typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
HTTPRequestCallback5<C, T1, T2, T3, T4, T5> *MakeRequestCallback(C *ptr, int32_t(C::*func)(int32_t errorCode,
		HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5), T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) {
	return new HTTPRequestCallback5<C, T1, T2, T3, T4, T5>(ptr, func, arg1, arg2, arg3, arg4, arg5);
}

template <typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
HTTPRequestCallback6<C, T1, T2, T3, T4, T5, T6> *MakeRequestCallback(C *ptr, int32_t(C::*func)(int32_t errorCode,
	HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6), T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6) {
		return new HTTPRequestCallback6<C, T1, T2, T3, T4, T5, T6>(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6);
}

template <typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
HTTPRequestCallback7<C, T1, T2, T3, T4, T5, T6, T7> *MakeRequestCallback(C *ptr, int32_t(C::*func)(int32_t errorCode,
	HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7), T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7) {
		return new HTTPRequestCallback7<C, T1, T2, T3, T4, T5, T6, T7>(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

template <typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
HTTPRequestCallback8<C, T1, T2, T3, T4, T5, T6, T7, T8> *MakeRequestCallback(C *ptr, int32_t(C::*func)(int32_t errorCode,
	HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8), T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8) {
	return new HTTPRequestCallback8<C, T1, T2, T3, T4, T5, T6, T7, T8>(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

template <typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
HTTPRequestCallback9<C, T1, T2, T3, T4, T5, T6, T7, T8, T9> *MakeRequestCallback(C *ptr, int32_t(C::*func)(int32_t errorCode,
	HTTPResponse *response, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9), T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9) {
	return new HTTPRequestCallback9<C, T1, T2, T3, T4, T5, T6, T7, T8, T9>(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

} /* namespace uc */

#endif /* HTTPREQUESTCALLBACK_H_ */
