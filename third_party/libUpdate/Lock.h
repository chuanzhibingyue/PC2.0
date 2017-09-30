/*++

Copyright (c) 2009 G-NET

Module Name:

    Debug.h

Abstract:

    This module contains definition and prototypes
    for code debugging and memory funciton wrapper etc.

Author:

    Yunfei Zuo (yunfei.zuo@gnetis.com) Oct/27/2009

Revision History:

    Yunfei Zuo (yunfei.zuo@gnetis.com) Created Oct/27/2009

--*/

#pragma once

/**
 * This class automatically lock/unlock the internal critical section, 
 * so that we can make sure the lock is released in any case because
 * the destructor will be called when the code goes out the scope of 
 * the local variable. we use this class like this.
 */
class AutoLock
{
public:
    /**
     * Constructor. Lock the critical section.
     */
    AutoLock(LPCRITICAL_SECTION cs) : m_cs(cs)
    {
        ::EnterCriticalSection(m_cs);
    }

    /**
     * Destructor. Unlock the critical section.
     */
    ~AutoLock()
    {
        ::LeaveCriticalSection(m_cs);
    }

private:
    LPCRITICAL_SECTION m_cs;
};

