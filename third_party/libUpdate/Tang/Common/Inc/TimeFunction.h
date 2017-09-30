#pragma once
#include <windows.h>
#include <sys/types.h>
#include <sys/timeb.h>


#define RTP_NTPTIMEOFFSET									2208988800UL
/**
* @brief ¼ÆËãÎ¢Ãî
* @param [in] void.
* @return return microseconds.
*/
static unsigned __int64 CalculateMicroseconds(unsigned __int64 performancecount,
                                       unsigned __int64 performancefrequency)
{
	if((LONGLONG)0 == performancefrequency)
	{
		LOG_ERR(L"query performance frequency is 0");
		/// add by shangrong.rao
		return 14318180;
	}
    unsigned __int64 f = performancefrequency;
    unsigned __int64 a = performancecount;
    unsigned __int64 b = a/f;
    unsigned __int64 c = a%f; // a = b*f+c => (a*1000000)/f = b*1000000+(c*1000000)/f

    return b*1000000ui64+(c*1000000ui64)/f;
}
/**
* @brief Get the microseconds since the system started.
* @param [in] void.
* @return return microseconds
*/
static UINT32 CurrentTime()
{
    static int inited = 0;
    static unsigned __int64 microseconds, initmicroseconds;
    static LARGE_INTEGER performancefrequency;

    unsigned __int64 emulate_microseconds, microdiff;
    SYSTEMTIME systemtime;
    FILETIME filetime;

    LARGE_INTEGER performancecount;

    QueryPerformanceCounter(&performancecount);

    if(!inited){
        inited = 1;
        QueryPerformanceFrequency(&performancefrequency);
		LOG_INFO(L"first time query performance frequency is:[%I64d]",performancefrequency.QuadPart);
        GetSystemTime(&systemtime);
        SystemTimeToFileTime(&systemtime,&filetime);
        microseconds = ( ((unsigned __int64)(filetime.dwHighDateTime) << 32) + (unsigned __int64)(filetime.dwLowDateTime) ) / 10ui64;
        microseconds-= 11644473600000000ui64; // EPOCH
        initmicroseconds = CalculateMicroseconds(performancecount.QuadPart, performancefrequency.QuadPart);
    }

    emulate_microseconds = CalculateMicroseconds(performancecount.QuadPart, performancefrequency.QuadPart); // from the system start

    microdiff = emulate_microseconds - initmicroseconds;

    unsigned __int64 now_microseconds = microseconds + microdiff; // now in microseconds;
    return  (UINT32)(now_microseconds*65535ui64/1000000ui64); // 1/65535 seconds;
}
