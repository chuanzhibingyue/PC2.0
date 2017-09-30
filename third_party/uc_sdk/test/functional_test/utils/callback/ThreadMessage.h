/*
 * ThreadMessage.h
 *
 *  Created on: May 12, 2015
 *      Author: gang wang
 */

#ifndef THREAD_MESSAGE_H_
#define THREAD_MESSAGE_H_

#include <stddef.h>

#include "Callback.h"

class MessageProc
{
public:
    virtual ~MessageProc(){}

    virtual bool onMessage(int msgID, long param1, long param2) = 0;
};

#define WM_ASYNC_CALLBACK		65555
class DefaultMessageProc : public MessageProc
{
public:
    virtual bool onMessage(int msgID, long param1, long param2) {
		if (msgID == WM_ASYNC_CALLBACK){
			Callback *func = (Callback*)param1;
			(*func)();
			delete func;
			return true;
		}
		return false;
	}
};

bool AttachMainThreadMessageProc(MessageProc *proc);
void DetachMainThreadMessageProc();

/**
 * 向主线程发消息。当AttachMainThreadMessageProc使用默认的消息处理函数DefaultMessageProc时，
 * param1必须是外部通过new创建的函数对象的类指针，DefaultMessageProc的onMessage
 * 方法在执行完该函数对象后，自动释放函数对象内存。
 */
bool PostMainThreadMessage(int msg, long param1 = NULL, long param2 = NULL);


#endif