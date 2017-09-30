/*++

Copyright (c) 2009 G-NET

Module Name:

    MemoryLeakDetector.h

Abstract:

    This module contains the macros and helper functions used to detect 
    memory leak in cpp file.

Author:

    Yunfei Zuo (yunfei.zuo@gnetis.com) Oct/27/2009

Revision History:

    Yunfei Zuo (yunfei.zuo@gnetis.com) Created Oct/27/2009

Note:

    This head file MUST be included as the last header file that included 
    in .cpp file.

--*/

#ifndef __MEMORY_LEAK_DETECTOR_H__
#define __MEMORY_LEAK_DETECTOR_H__
#else
#error "MemoryLeadDectector.h" can only be include once as the last header file in the .cpp file.
#endif

#include <crtdbg.h>

#ifndef DISABLE_MEM_LEAK_DETECTION_SUPPORT

    #ifdef _DEBUG 
        #undef new
        #define new             new(_NORMAL_BLOCK, __FILE__, __LINE__)
        #undef malloc
        #define malloc(x)       _malloc_dbg(x, _NORMAL_BLOCK, __FILE__, __LINE__)
        #undef free
        #define free(x)         _free_dbg(x, _NORMAL_BLOCK)

        #define DBG_MEM_DETECT_ENABLE()    EnableMemoryLeakDetection(TRUE)
        #define DBG_MEM_DETECT_DISABLE()   EnableMemoryLeakDetection(FALSE)

        #define MEMORY_LEAK_DUMP_FILE  L"MemoryLeakInfo.txt"


        // Set memory leak detection state.
        inline void
        EnableMemoryLeakDetection(
            __in BOOL isEnable
            )
        /*++

        Routine Description:

            Set memory leak detection state.

        Arguments:

            __in BOOL isEnable - TRUE  means enable  memory leak detection. Report the memory leak
                                 result to local file after program exit automaticly.
                                 FALSE means disable memory leak detection.

        Return Value:

            none.
            
         --*/
        {  
            FILE * pFile = NULL;
            int dbgFlag = _CRTDBG_CHECK_DEFAULT_DF;

            //
            // Set the debug-heap flag to keep freed blocks in the
            // heap's linked list - This will allow us to catch any
            // inadvertent use of freed memory
            //
            // _CRTDBG_ALLOC_MEM_DF  - Enable debug heap allocations and use of memory block type identifiers
            // _CRTDBG_LEAK_CHECK_DF - Perform automatic leak checking at program exit
            //
            dbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

            if (isEnable)
            {
                dbgFlag |= (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
            }
            else
            {
                dbgFlag &= ~(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
            }

            _CrtSetDbgFlag(dbgFlag);

            if (isEnable)
            {
                //
                // Reassign stderr to the MemoryLeak.txt file
                //
                _wfreopen_s(&pFile, MEMORY_LEAK_DUMP_FILE, L"w", stderr);

                //
                // If memory leak detect, then report it to stderr
                //
                _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
                _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
            }
        }

    #else   // _DEBUG

        #define DBG_MEM_DETECT_ENABLE()
        #define DBG_MEM_DETECT_DISABLE()

    #endif  // _DEBUG

#else // DISABLE_MEM_LEAK_DETECTION_SUPPORT

    #define DBG_MEM_DETECT_ENABLE()
    #define DBG_MEM_DETECT_DISABLE()
    
#endif // DISABLE_MEM_LEAK_DETECTION_SUPPORT


