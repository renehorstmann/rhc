#ifndef RHC_TIME_H
#define RHC_TIME_H

//
// functions for high precision time and a timer
//

#include "export.h"

// returns the time in seconds from the monotonic clock
RHC_EXPORT
double rhc_time_monotonic();

//
// High precision timer based on rhc_time_monotonic
//
typedef struct {
    double start_time;
} RhcTimer_s;

// creates a new timer with the current monotonic time
static RhcTimer_s rhc_timer_new() {
    return (RhcTimer_s) {rhc_time_monotonic()};
}

// returns the elapsed time in seconds
static double rhc_timer_elapsed(RhcTimer_s self) {
    return rhc_time_monotonic() - self.start_time;
}

// resets the timer and return the elapsed time in seconds
static double rhc_timer_reset(RhcTimer_s self) {
    double time = rhc_time_monotonic();
    double elapsed = time - self.start_time;
    self.start_time = time;
    return elapsed;
}


#endif //RHC_TIME_H
