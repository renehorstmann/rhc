#ifndef RHC_ASSUME_IMPL_H
#define RHC_ASSUME_IMPL_H
#ifdef RHC_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include "../assume.h"

#ifdef PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif

#ifdef PLATFORM_ANDROID
#include <SDL2/SDL.h>
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
    char *msg = malloc(RHC_ASSUME_MAX_FORMATED_MSG_SIZE);
    vsnprintf(msg, RHC_ASSUME_MAX_FORMATED_MSG_SIZE, format, args);
    va_end(args);
#ifdef NDEBUG
    fprintf(stderr, "An assumption in the program failed: %s\n", msg);
#else
    fprintf(stderr, "Assumption failed: %s at %s:%d %s\n", expression, file, line, msg);
#endif


#ifdef PLATFORM_EMSCRIPTEN
    {
        // exit emscriptens main loop and call js error handler
        int script_size = 2*S_ASSUME_MAX_FORMATED_MSG_SIZE;
        char *script = malloc(script_size);
#ifdef NDEBUG
        snprintf(script, script_size, "set_assume(\'An assumption in the program failed: %s\');", msg);
#else
        snprintf(script, script_size, "set_assume(\'Assumption failed: %s at %s:%d %s\');", expression, file, line, msg);
#endif
        emscripten_cancel_main_loop();
        emscripten_run_script(script);
        free(script);
        exit(EXIT_FAILURE);
    }
#endif

#ifdef PLATFORM_ANDROID
    {
        // show a small toast dialog in the app and then exit
        int toast_size = 2 * S_ASSUME_MAX_FORMATED_MSG_SIZE;
        char *toast = malloc(toast_size);
#ifdef NDEBUG
        snprintf(toast, toast_size, "An assumption in the program failed: %s", msg);
#else
        snprintf(toast, toast_size, "Assumption failed: %s at %s:%d %s",
                 expression, file, line, msg);
#endif
        SDL_AndroidShowToast(toast, 1, -1, 0, 0);
        SDL_Delay(4000);
        free(toast);
        exit(EXIT_FAILURE);
    }
#endif

    free(msg);
    raise(RHC_ASSUME_SIGNAL);
}

#endif //RHC_IMPL
#endif //RHC_ASSUME_IMPL_H
