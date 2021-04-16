/**
 * Copyright (c) 2020 renehorstmann
 *
 * Modified version of:
 * Copyright (c) 2017 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef RHC_LOG_H
#define RHC_LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

//
// Options:
//

#ifndef RHC_LOG_DEFAULT_FILE
#define RHC_LOG_DEFAULT_FILE stdout
#endif



// use the following definition to stop using colors
// #define LOG_DO_NOT_USE_COLOR

typedef void (*log_lock_function)(void *user_data, bool lock);

enum log_level {
    LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_WTF, LOG_NUM_LEVELS
};

#define log_trace(...) rhc_log_base_(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)

#define log_debug(...) rhc_log_base_(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

#define log_info(...)  rhc_log_base_(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)

#define log_warn(...)  rhc_log_base_(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)

#define log_error(...) rhc_log_base_(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#define log_wtf(...)   rhc_log_base_(LOG_WTF, __FILE__, __LINE__, __VA_ARGS__)


void rhc_log_set_log_file(FILE *file);

void rhc_log_set_min_level(enum log_level level);

void rhc_log_set_quiet(bool set);

void rhc_log_set_locking_function(log_lock_function fun);

void rhc_log_set_locking_function_user_data(void *user_data);

void rhc_log_base_(enum log_level level, const char *file, int line, const char *format, ...);

#endif //RHC_LOG
