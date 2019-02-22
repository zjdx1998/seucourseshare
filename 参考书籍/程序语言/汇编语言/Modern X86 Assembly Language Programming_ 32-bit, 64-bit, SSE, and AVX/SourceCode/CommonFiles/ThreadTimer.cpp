#include "stdafx.h"
#include "ThreadTimer.h"
#include <stdio.h>
#include <stdexcept>

using namespace std;

double ThreadTimer::GetElapsedTime(void)
{
    __int64 elapsed_time;
    static LARGE_INTEGER performance_freq;
    static bool qpf_called = false;

    if (!qpf_called)
    {
        QueryPerformanceFrequency((LARGE_INTEGER*)&performance_freq);
        qpf_called = true;
    }

    elapsed_time = StopTime.QuadPart - StartTime.QuadPart;
    return (double)elapsed_time / performance_freq.QuadPart;
}

bool ThreadTimer::GetProcessAffinityMask(unsigned __int64* process_mask, unsigned __int64* system_mask)
{
    DWORD_PTR process_mask2;
    DWORD_PTR system_mask2;
    BOOL status;
    HANDLE h_process = GetCurrentProcess();

    status = ::GetProcessAffinityMask(h_process, &process_mask2, &system_mask2);
    *process_mask = process_mask2;
    *system_mask = system_mask2;
    return (status != 0) ? true : false;
}

bool ThreadTimer::SetProcessAffinityMask(unsigned __int64 process_mask)
{
    BOOL status;
    HANDLE h_process = GetCurrentProcess();

    status = ::SetProcessAffinityMask(h_process, (DWORD_PTR)process_mask);
    return (status != 0) ? true : false;
}

bool ThreadTimer::SetThreadAffinityMask(void)
{
    // Note: Code below may not work on computers with more than 64 processors.

    const int n = sizeof(DWORD_PTR) * 8;
    DWORD_PTR system_mask;
    DWORD_PTR process_mask;
    DWORD_PTR thread_mask = (DWORD_PTR)0x1 << (n - 1);
    HANDLE h_process = GetCurrentProcess();

    if (::GetProcessAffinityMask(h_process, &process_mask, &system_mask) == 0)
        return false;

    // Assign thread to highest available processor

    for (int i = 0; i < n; i++)
    {
        if (((system_mask & thread_mask) != 0)  && ((process_mask & thread_mask) != 0))
        {
            HANDLE h_thread = GetCurrentThread();
            DWORD_PTR status = ::SetThreadAffinityMask(h_thread, thread_mask);

            return (status != 0) ? true : false;
        }

        thread_mask >>= 1;
    }

    return false;
}

bool ThreadTimer::SetThreadAffinityMask(unsigned __int64 thread_mask)
{
    DWORD_PTR status;
    HANDLE h_thread = GetCurrentThread();

    status = ::SetThreadAffinityMask(h_thread, (DWORD_PTR)thread_mask);
    return (status != 0) ? true : false;
}

void ThreadTimer::SaveElapsedTimeMatrix(const char* fn, double* et, int nrows, int ncols)
{
    FILE* fp;

    if (fopen_s(&fp, fn, "wt") == 0)
    {
        for (int i = 0; i < nrows; i++)
        {
            for (int j = 0; j < ncols; j++)
            {
                double val = et[i * ncols + j];
                fprintf(fp, "%.8lf, ", val);
            }

            fprintf(fp, "\n");
        }

        fclose(fp);
    }
    else
    {
        char buff[512];

        sprintf_s(buff, sizeof(buff), "ThreadTimer::SaveElapsedTimeMatrix - File open error: %s\n", fn);
        throw new runtime_error(buff);
    }
}
