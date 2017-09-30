#pragma once
#include "RecorderForClient.h"
using namespace clt;
struct BackupInfoBase                 //恢复信息类的基类
{
public:
    void *(operator new)(unsigned int uiSize)	//Must overload the function. It`s static function.
    {
        return clt::Alloc(uiSize);
    }
    void  operator delete(void * p)
    {
        clt::Free(p);
    }
};