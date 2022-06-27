#ifndef RHC_TIME_IMPL_H
#define RHC_TIME_IMPL_H
#ifdef RHC_IMPL

#include "../time.h"

#ifdef PLATFORM_MSVC

#include <windows.h>
double rhc_time_monotonic() {
    LARGE_INTEGER time, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&time);
    return (double) time.QuadPart / (double) freq.QuadPart;
}

#else
#include <time.h>

double rhc_time_monotonic() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (double) time.tv_sec + (double) time.tv_nsec / 1000000000.0;
}

#endif

#endif //RHC_IMPL
#endif //RHC_TIME_IMPL_H
