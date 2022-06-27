#ifndef RHC_ERROR_IMPL_H
#define RHC_ERROR_IMPL_H
#ifdef RHC_IMPL


#include "../common.h"  // for _Thread_local
#include "../log.h"
#include "../error.h"


//
// data
//

static _Thread_local const char *rhc__error;


const char *rhc_error_get() {
    return rhc__error;
}

void rhc_error_clear() {
    rhc__error = NULL;
}

void rhc_error_set(const char *static_data_error) {
    if(rhc__error) {
        rhc_log_base(RHC_LOG_WARN, NULL, 0, __func__, "overwriting error: %s", rhc__error);
    }
    rhc__error = static_data_error;
    rhc_log_base(RHC_LOG_ERROR, NULL, 0, __func__, "%s", rhc__error);
}

#endif //RHC_IMPL
#endif //RHC_ERROR_IMPL_H
