#ifndef RHC_LOG_IMPL_H
#define RHC_LOG_IMPL_H
#ifdef RHC_IMPL

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../log.h"

static struct {
    FILE *log_file;
    enum log_level level;
    bool quiet;
    log_lock_function lock_function;
    void *used_data;
} rhc_log_L;


static const char *rhc_log_src_level_names_[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "WTF"
};

#ifndef LOG_DO_NOT_USE_COLOR
static const char *rhc_log_src_level_colors_[] = {
        "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif


static void rhc_log_src_lock_() {
    if (rhc_log_L.lock_function) {
        rhc_log_L.lock_function(rhc_log_L.used_data, true);
    }
}


static void rhc_log_src_unlock_() {
    if (rhc_log_L.lock_function) {
        rhc_log_L.lock_function(rhc_log_L.used_data, false);
    }
}


void rhc_log_set_log_file(FILE *file) {
    rhc_log_L.log_file = file;
}

void rhc_log_set_min_level(enum log_level level) {
    rhc_log_L.level = level;
}

void rhc_log_set_quiet(bool set) {
    rhc_log_L.quiet = set;
}

void rhc_log_set_locking_function(log_lock_function fun) {
    rhc_log_L.lock_function = fun;
}

void rhc_log_set_locking_function_user_data(void *user_data) {
    rhc_log_L.used_data = user_data;
}


void rhc_log_base_(enum log_level level, const char *file, int line, const char *format, ...) {
    if (level < rhc_log_L.level) {
        return;
    }

    /* Acquire lock_function */
    rhc_log_src_lock_();

    /* Get current time */
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);

    /* Log to RHC_DEFAULT_FILE (stdout) */
    if (!rhc_log_L.quiet) {
        va_list args;
        char buf[16];
        buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';
#ifdef LOG_DO_NOT_USE_COLOR
        fprintf(RHC_LOG_DEFAULT_FILE, "%s %-5s %s:%d: ",
                buf, rhc_log_src_level_names_[level], file, line);
#else
        fprintf(RHC_LOG_DEFAULT_FILE, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
                buf, rhc_log_src_level_colors_[level], rhc_log_src_level_names_[level], file, line);
#endif
        va_start(args, format);
        vfprintf(RHC_LOG_DEFAULT_FILE, format, args);
        va_end(args);
        fprintf(RHC_LOG_DEFAULT_FILE, "\n");
        fflush(RHC_LOG_DEFAULT_FILE);
    }

    /* Log to file */
    if (rhc_log_L.log_file) {
        va_list args;
        char buf[32];
        buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
        fprintf(rhc_log_L.log_file, "%s %-5s %s:%d: ",
                buf, rhc_log_src_level_names_[level], file, line);
        va_start(args, format);
        vfprintf(rhc_log_L.log_file, format, args);
        va_end(args);
        fprintf(rhc_log_L.log_file, "\n");
        fflush(rhc_log_L.log_file);
    }

    /* Release lock_function */
    rhc_log_src_unlock_();
}

#endif //RHC_IMPL
#endif //RHC_LOG_IMPL_H
