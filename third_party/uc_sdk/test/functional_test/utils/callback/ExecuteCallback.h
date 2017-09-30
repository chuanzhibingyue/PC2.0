#ifndef EXECUTE_CALLBACK_H_
#define EXECUTE_CALLBACK_H_

#include "ThreadMessage.h"

/**
 * 构造callback，并切换到主进程函数执行
 */
template <typename C>
void ExecuteCallback(C *ptr,void (C::*func)()) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func));
}

template <typename C>
void ExecuteCallback(C *ptr,void (C::*func)() const) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func));
}

template <typename C, typename T1>
void ExecuteCallback(C *ptr,void (C::*func)(T1), T1 arg1) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1));
}

template <typename C, typename T1>
void ExecuteCallback(C *ptr,void (C::*func)(T1) const, T1 arg1) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1));
}

template <typename C, typename T1, typename T2>
void ExecuteCallback(C *ptr,void (C::*func)(T1, T2), T1 arg1, T2 arg2) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2));
}

template <typename C, typename T1, typename T2>
void ExecuteCallback(C *ptr,void (C::*func)(T1, T2) const, T1 arg1, T2 arg2) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2));
}

template <typename C, typename T1, typename T2, typename T3>
void ExecuteCallback(C *ptr,void (C::*func)(T1,T2,T3), T1 arg1, T2 arg2, T3 arg3) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3));
}

template <typename C, typename T1, typename T2, typename T3>
void ExecuteCallback(C *ptr,void (C::*func)(T1,T2,T3) const, T1 arg1, T2 arg2, T3 arg3) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3));
}

template <typename C, typename T1, typename T2, typename T3, typename T4>
void ExecuteCallback(C *ptr,void (C::*func)(T1,T2,T3,T4), 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4));
}

template <typename C, typename T1, typename T2, typename T3, typename T4>
void ExecuteCallback(C *ptr,void (C::*func)(T1,T2,T3,T4) const, 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5>
void ExecuteCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5), 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5>
void ExecuteCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5) const, 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6>
void ExecuteCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6), 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6>
void ExecuteCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6) const, 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6, typename T7>
void ExecuteCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6,T7), 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6,arg7));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6, typename T7>
void ExecuteCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6,T7) const, 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6,arg7));
}


#endif