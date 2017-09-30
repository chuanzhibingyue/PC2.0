//
//  common_test.m
//  commontest
//
//  Created by jamesoy on 5/18/16.
//  Copyright © 2016 quanshi. All rights reserved.
//

#include "common_test.h"
#include "common/ThreadMessage.h"


class MyMessageProc : public uc::MessageProc
{
public:
    virtual bool onMessage(int msgID, long param1, long param2) {
        if (msgID == 111111){
        }
        return false;
    }
};

@implementation common_test

+ (void)attachToMainThread
{
    static MyMessageProc mymessageproc;
    uc::AttachMainThreadMessageProc(&mymessageproc);
}

+ (void)postMessageToMainThread
{
    uc::PostMainThreadMessage(111111, 1212, 3434);
}

@end
