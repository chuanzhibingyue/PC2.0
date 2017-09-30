
#ifndef CALLBACK_H_
#define CALLBACK_H_

class Callback {
public:
	virtual ~Callback(){}

	virtual void operator()() = 0;
};

class Callback0 : public Callback{
public:
	typedef void (*Func)();
	Callback0(Func func) : _func(func){
	}

	virtual void operator()() {
		(*_func)();
	}
private:
	Func _func;
};

template<typename C>
class CallbackC0 : public Callback{
public:
	typedef void (C::*ClassFunc)();
	CallbackC0(C *ptr, ClassFunc func) : _ptr(ptr), _func(func){
	}

	virtual void operator()() {
		(_ptr->*_func)();
	}
private:
	C *_ptr;
	ClassFunc _func;
};

template<typename C>
class ConstCallbackC0 : public Callback{
public:
	typedef void (C::*ClassFunc)() const;
	ConstCallbackC0(C *ptr, ClassFunc func) : _ptr(ptr), _func(func){
	}

	virtual void operator()() {
		(_ptr->*_func)();
	}
private:
	C *_ptr;
	ClassFunc _func;
};

template<typename C, typename T1>
class CallbackC1 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1);
	CallbackC1(C *ptr, ClassFunc func, T1 arg1) : _ptr(ptr), _func(func) , _arg1(arg1){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1);
	}
private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
};

template<typename C, typename T1>
class ConstCallbackC1 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1) const;
	ConstCallbackC1(C *ptr, ClassFunc func, T1 arg1) : _ptr(ptr), _func(func) , _arg1(arg1){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1);
	}
private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
};

template<typename C, typename T1, typename T2>
class CallbackC2 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2);
	CallbackC2(C *ptr, ClassFunc func, T1 arg1, T2 arg2) : _ptr(ptr), 
		_func(func) , _arg1(arg1), _arg2(arg2){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2);
	}
private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
};

template<typename C, typename T1, typename T2>
class ConstCallbackC2 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2) const;
	ConstCallbackC2(C *ptr, ClassFunc func, T1 arg1, T2 arg2) : _ptr(ptr), 
		_func(func) , _arg1(arg1), _arg2(arg2){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2);
	}
private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
};

template<typename C, typename T1, typename T2, typename T3>
class CallbackC3 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2, T3);
	CallbackC3(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3) : _ptr(ptr), 
		_func(func) , _arg1(arg1), _arg2(arg2), _arg3(arg3){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2, _arg3);
	}
private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
};

template<typename C, typename T1, typename T2, typename T3>
class ConstCallbackC3 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2, T3) const;
	ConstCallbackC3(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3) : _ptr(ptr), 
		_func(func) , _arg1(arg1), _arg2(arg2), _arg3(arg3){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2, _arg3);
	}
private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
};

template<typename C, typename T1, typename T2, typename T3, typename T4>
class CallbackC4 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2, T3, T4);
	CallbackC4(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4) : _ptr(ptr), 
		_func(func) , _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2, _arg3, _arg4);
	}
private:
	C *_ptr;
	ClassFunc _func;
	T1 _arg1;
	T2 _arg2;
	T3 _arg3;
	T4 _arg4;
};

template<typename C, typename T1, typename T2, typename T3, typename T4>
class ConstCallbackC4 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2, T3, T4) const;
	ConstCallbackC4(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4) : _ptr(ptr), 
		_func(func) , _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2, _arg3, _arg4);
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
class CallbackC5 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2, T3, T4, T5);
	CallbackC5(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) 
		: _ptr(ptr), _func(func) , _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), 
		_arg5(arg5){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2, _arg3, _arg4, _arg5);
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

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
class ConstCallbackC5 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2, T3, T4, T5) const;
	ConstCallbackC5(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) 
		: _ptr(ptr), _func(func) , _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), 
		_arg5(arg5){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2, _arg3, _arg4, _arg5);
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

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, 
	typename T6>
class CallbackC6 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2, T3, T4, T5, T6);
	CallbackC6(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6) 
		: _ptr(ptr), _func(func) , _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), 
		_arg5(arg5), _arg6(arg6){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2, _arg3, _arg4, _arg5, _arg6);
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

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, 
	typename T6>
class ConstCallbackC6 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2, T3, T4, T5, T6) const;
	ConstCallbackC6(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6) 
		: _ptr(ptr), _func(func) , _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4), 
		_arg5(arg5), _arg6(arg6){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2, _arg3, _arg4, _arg5, _arg6);
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

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, 
	typename T6, typename T7>
class CallbackC7 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2, T3, T4, T5, T6, T7);
	CallbackC7(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, 
		T6 arg6, T7 arg7) : _ptr(ptr), _func(func) , _arg1(arg1), _arg2(arg2), 
		_arg3(arg3), _arg4(arg4), _arg5(arg5), _arg6(arg6), _arg7(arg7){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7);
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

template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, 
	typename T6, typename T7>
class ConstCallbackC7 : public Callback{
public:
	typedef void (C::*ClassFunc)(T1, T2, T3, T4, T5, T6, T7) const;
	ConstCallbackC7(C *ptr, ClassFunc func, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, 
		T6 arg6, T7 arg7) : _ptr(ptr), _func(func) , _arg1(arg1), _arg2(arg2), 
		_arg3(arg3), _arg4(arg4), _arg5(arg5), _arg6(arg6), _arg7(arg7){
	}

	virtual void operator()() {
		(_ptr->*_func)(_arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7);
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

template <typename C>
CallbackC0<C> *MakeCallback(C *ptr,void (C::*func)()) {
	return new CallbackC0<C>(ptr, func);
}

template <typename C>
CallbackC0<C> *MakeCallback(C *ptr,void (C::*func)() const) {
	return new ConstCallbackC0<C>(ptr, func);
}

template <typename C, typename T1>
CallbackC1<C, T1> *MakeCallback(C *ptr,void (C::*func)(T1), T1 arg1) {
	return new CallbackC1<C, T1>(ptr, func, arg1);
}

template <typename C, typename T1>
CallbackC1<C, T1> *MakeCallback(C *ptr,void (C::*func)(T1) const, T1 arg1) {
	return new ConstCallbackC1<C, T1>(ptr, func, arg1);
}

template <typename C, typename T1, typename T2>
CallbackC2<C, T1, T2> *MakeCallback(C *ptr,void (C::*func)(T1, T2), T1 arg1, T2 arg2) {
	return new CallbackC2<C, T1, T2>(ptr, func, arg1, arg2);
}

template <typename C, typename T1, typename T2>
CallbackC2<C, T1, T2> *MakeCallback(C *ptr,void (C::*func)(T1, T2) const, T1 arg1, T2 arg2) {
	return new ConstCallbackC2<C, T1, T2>(ptr, func, arg1, arg2);
}

template <typename C, typename T1, typename T2, typename T3>
CallbackC3<C, T1, T2, T3> *MakeCallback(C *ptr,void (C::*func)(T1,T2,T3), T1 arg1, T2 arg2, T3 arg3) {
	return new CallbackC3<C, T1,T2,T3>(ptr, func, arg1, arg2, arg3);
}

template <typename C, typename T1, typename T2, typename T3>
CallbackC3<C, T1, T2, T3> *MakeCallback(C *ptr,void (C::*func)(T1,T2,T3) const, T1 arg1, T2 arg2, T3 arg3) {
	return new ConstCallbackC3<C, T1,T2,T3>(ptr, func, arg1, arg2, arg3);
}

template <typename C, typename T1, typename T2, typename T3, typename T4>
CallbackC4<C, T1, T2, T3, T4> *MakeCallback(C *ptr,void (C::*func)(T1,T2,T3,T4), 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return new CallbackC4<C,T1,T2,T3,T4>(ptr, func, arg1, arg2, arg3, arg4);
}

template <typename C, typename T1, typename T2, typename T3, typename T4>
CallbackC4<C, T1, T2, T3, T4> *MakeCallback(C *ptr,void (C::*func)(T1,T2,T3,T4) const, 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return new ConstCallbackC4<C,T1,T2,T3,T4>(ptr, func, arg1, arg2, arg3, arg4);
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5>
CallbackC5<C, T1, T2, T3, T4, T5> *MakeCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5), 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) {
	return new CallbackC5<C,T1,T2,T3,T4,T5>(ptr, func, arg1, arg2, arg3, arg4, arg5);
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5>
CallbackC5<C, T1, T2, T3, T4, T5> *MakeCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5) const, 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5) {
	return new ConstCallbackC5<C,T1,T2,T3,T4,T5>(ptr, func, arg1, arg2, arg3, arg4, arg5);
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6>
CallbackC6<C, T1, T2, T3, T4, T5,T6> *MakeCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6), 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6) {
	return new CallbackC6<C,T1,T2,T3,T4,T5,T6>(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6);
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6>
CallbackC6<C, T1, T2, T3, T4, T5,T6> *MakeCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6) const, 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6) {
	return new ConstCallbackC6<C,T1,T2,T3,T4,T5,T6>(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6);
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6, typename T7>
CallbackC7<C, T1, T2, T3, T4, T5,T6,T7> *MakeCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6,T7), 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7) {
	return new CallbackC7<C,T1,T2,T3,T4,T5,T6,T7>(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6,arg7);
}

template <typename C, typename T1, typename T2, typename T3, typename T4,
	typename T5, typename T6, typename T7>
CallbackC7<C, T1, T2, T3, T4, T5,T6,T7> *MakeCallback(C *ptr,void (C::*func)(T1,T2,T3,T4,T5,T6,T7) const, 
	T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7) {
	return new ConstCallbackC7<C,T1,T2,T3,T4,T5,T6,T7>(ptr, func, arg1, arg2, arg3, arg4, arg5, arg6,arg7);
}

#endif
