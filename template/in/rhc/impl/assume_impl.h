#ifndef RHC_ASSUME_IMPL_H
#define RHC_ASSUME_IMPL_H
#ifdef RHC_IMPL

#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include "../assume.h"

#ifdef PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif

//
// options
//

#ifndef RHC_ASSUME_SIGNAL
#define RHC_ASSUME_SIGNAL SIGABRT
#endif

#ifndef RHC_ASSUME_MAX_FORMATED_MSG_SIZE
#define RHC_ASSUME_MAX_FORMATED_MSG_SIZE 4096
#endif


//
// functions
//


void rhc__rhc_assume_impl(const char *expression, const char *file, int line, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char msg[RHC_ASSUME_MAX_FORMATED_MSG_SIZE];
    vsnprintf(msg, RHC_ASSUME_MAX_FORMATED_MSG_SIZE, format, args);
    va_end(args);
#ifdef NDEBUG
    fprintf(stderr, "An assumption in the program failed: %s\n", msg);
#else
    fprintf(stderr, "Assumption failed: %s at %s:%d %s\n", expression, file, line, msg);
#endif
    raise(RHC_ASSUME_SIGNAL);

#ifdef PLATFORM_EMSCRIPTEN
    // exit emscriptens main loop and call js error handler
    emscripten_cancel_main_loop();
    EM_ASM(
            set_exit_failure_error_msg();
            );
    // emscripten does not handle signals
    exit(EXIT_FAILURE);
#endif

}

#endif //RHC_IMPL
#endif //RHC_ASSUME_IMPL_H
