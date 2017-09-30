#ifndef EXECUTE_CALLBACK_H_
#define EXECUTE_CALLBACK_H_

#include "common/ThreadMessage.h"
#include "common/Callback.h"

namespace uc {

/**
 * 构造callback，并切换到主进程函数执行
 */
template <typename C>
void CallOnMainThread(C *ptr,void (C::*func)()) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func));
}

template <typename C>
void CallOnMainThread(C *ptr,void (C::*func)() const) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func));
}

template <typename C, typename T1>
void CallOnMainThread(C *ptr, void (C::*func)(T1), typename CallbackTraits<T1>::FuncParamType arg1) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1));
}

template <typename C, typename T1>
void CallOnMainThread(C *ptr, void (C::*func)(T1) const, typename CallbackTraits<T1>::FuncParamType arg1) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1));
}

template <typename C, typename T1, typename T2>
void CallOnMainThread(C *ptr, void (C::*func)(T1, T2), typename CallbackTraits<T1>::FuncParamType arg1, T2 arg2) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2));
}

template <typename C, typename T1, typename T2>
void CallOnMainThread(C *ptr, void (C::*func)(T1, T2) const, typename CallbackTraits<T1>::FuncParamType arg1, 
	typename CallbackTraits<T2>::FuncParamType arg2) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2));
}

template <typename C, typename T1, typename T2, typename T3>
void CallOnMainThread(C *ptr, void (C::*func)(T1, T2, T3), typename CallbackTraits<T1>::FuncParamType arg1, 
	typename CallbackTraits<T2>::FuncParamType arg2, 
	typename CallbackTraits<T3>::FuncParamType arg3) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3));
}

template <typename C, typename T1, typename T2, typename T3>
void CallOnMainThread(C *ptr, void (C::*func)(T1, T2, T3) const, typename CallbackTraits<T1>::FuncParamType arg1, 
	typename CallbackTraits<T2>::FuncParamType arg2, 
	typename CallbackTraits<T3>::FuncParamType arg3) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3));
}

template <typename C, typename T1, typename T2, typename T3, typename T4>
void CallOnMainThread(C *ptr,void (C::*func)(T1,T2,T3,T4), 
	typename CallbackTraits<T1>::FuncParamType arg1, 
	typename CallbackTraits<T2>::FuncParamType arg2, 
	typename CallbackTraits<T3>::FuncParamType arg3, 
	typename CallbackTraits<T4>::FuncParamType arg4) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4));
}

template <typename C, typename T1, typename T2, typename T3, typename T4>
void CallOnMainThread(C *ptr,void (C::*func)(T1,T2,T3,T4) const, 
	typename CallbackTraits<T1>::FuncParamType arg1, 
	typename CallbackTraits<T2>::FuncParamType arg2, 
	typename CallbackTraits<T3>::FuncParamType arg3, 
	typename CallbackTraits<T4>::FuncParamType arg4) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5>
void CallOnMainThread(C *ptr,void (C::*func)(T1,T2,T3,T4,T5), 
	typename CallbackTraits<T1>::FuncParamType arg1, 
	typename CallbackTraits<T2>::FuncParamType arg2, 
	typename CallbackTraits<T3>::FuncParamType arg3, 
	typename CallbackTraits<T4>::FuncParamType arg4, 
	typename CallbackTraits<T5>::FuncParamType arg5) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5>
void CallOnMainThread(C *ptr,void (C::*func)(T1,T2,T3,T4,T5) const, 
	typename CallbackTraits<T1>::FuncParamType arg1, typename CallbackTraits<T2>::FuncParamType arg2, 
	typename CallbackTraits<T3>::FuncParamType arg3, typename CallbackTraits<T4>::FuncParamType arg4, 
	typename CallbackTraits<T5>::FuncParamType arg5) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6>
void CallOnMainThread(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6), 
	typename CallbackTraits<T1>::FuncParamType arg1, typename CallbackTraits<T2>::FuncParamType arg2, 
	typename CallbackTraits<T3>::FuncParamType arg3, typename CallbackTraits<T4>::FuncParamType arg4, 
	typename CallbackTraits<T5>::FuncParamType arg5, typename CallbackTraits<T6>::FuncParamType arg6) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6>
void CallOnMainThread(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6) const, 
	typename CallbackTraits<T1>::FuncParamType arg1, typename CallbackTraits<T2>::FuncParamType arg2,
	typename CallbackTraits<T3>::FuncParamType arg3, typename CallbackTraits<T4>::FuncParamType arg4,
	typename CallbackTraits<T5>::FuncParamType arg5, typename CallbackTraits<T6>::FuncParamType arg6) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6, typename T7>
void CallOnMainThread(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6,T7), 
	typename CallbackTraits<T1>::FuncParamType arg1, typename CallbackTraits<T2>::FuncParamType arg2,
	typename CallbackTraits<T3>::FuncParamType arg3, typename CallbackTraits<T4>::FuncParamType arg4,
	typename CallbackTraits<T5>::FuncParamType arg5, typename CallbackTraits<T6>::FuncParamType arg6,
	typename CallbackTraits<T7>::FuncParamType arg7) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6,arg7));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6, typename T7>
void CallOnMainThread(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6,T7) const, 
	typename CallbackTraits<T1>::FuncParamType arg1, typename CallbackTraits<T2>::FuncParamType arg2,
	typename CallbackTraits<T3>::FuncParamType arg3, typename CallbackTraits<T4>::FuncParamType arg4,
	typename CallbackTraits<T5>::FuncParamType arg5, typename CallbackTraits<T6>::FuncParamType arg6,
	typename CallbackTraits<T7>::FuncParamType arg7) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6,arg7));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6, typename T7, typename T8>
void CallOnMainThread(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6,T7, T8), 
	typename CallbackTraits<T1>::FuncParamType arg1, typename CallbackTraits<T2>::FuncParamType arg2,
	typename CallbackTraits<T3>::FuncParamType arg3, typename CallbackTraits<T4>::FuncParamType arg4,
	typename CallbackTraits<T5>::FuncParamType arg5, typename CallbackTraits<T6>::FuncParamType arg6,
	typename CallbackTraits<T7>::FuncParamType arg7, typename CallbackTraits<T8>::FuncParamType arg8) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6,arg7,arg8));
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6, typename T7, typename T8>
void CallOnMainThread(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6,T7, T8) const, 
	typename CallbackTraits<T1>::FuncParamType arg1, typename CallbackTraits<T2>::FuncParamType arg2,
	typename CallbackTraits<T3>::FuncParamType arg3, typename CallbackTraits<T4>::FuncParamType arg4,
	typename CallbackTraits<T5>::FuncParamType arg5, typename CallbackTraits<T6>::FuncParamType arg6,
	typename CallbackTraits<T7>::FuncParamType arg7, typename CallbackTraits<T8>::FuncParamType arg8) {
	PostMainThreadMessage(WM_ASYNC_CALLBACK, (long)MakeCallback(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6,arg7,arg8));
}

}

#endif