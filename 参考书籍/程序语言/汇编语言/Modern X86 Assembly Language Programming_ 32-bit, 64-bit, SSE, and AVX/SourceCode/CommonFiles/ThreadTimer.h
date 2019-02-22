#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class ThreadTimer
{
public:
    LARGE_INTEGER StartTime;
    LARGE_INTEGER StopTime;

    ThreadTimer(bool start = false);

    double GetElapsedTime(void);
    void Start(void);
    void Stop(void);

    static bool GetProcessAffinityMask(unsigned __int64* process_mask, unsigned __int64* system_mask);
    static bool SetProcessAffinityMask(unsigned __int64 process_mask);
    static bool SetThreadAffinityMask(void);
    static bool SetThreadAffinityMask(unsigned __int64 thread_mask);

    static void SaveElapsedTimeMatrix(const char* fn, double* et, int nrows, int ncols);
};

inline ThreadTimer::ThreadTimer(bool start)
{
    StartTime.QuadPart = 0;
    StopTime.QuadPart = 0;

    if (start)
        Start();
}

inline void ThreadTimer::Start(void)
{
    QueryPerformanceCounter(&StartTime);
}

inline void ThreadTimer::Stop(void)
{
    QueryPerformanceCounter(&StopTime);
}
