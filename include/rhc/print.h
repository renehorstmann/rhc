#ifndef RHC_PRINT_H
#define RHC_PRINT_H

#include <stdarg.h>
#include "str.h"

static void print_float(const float *f) {
    printf("%f", *f);
}

static int print(str format, ...) {


    va_list ap;
    va_start (ap, format);

    va_arg(ap, int);

    va_end(ap);
    return 0;
}


#endif //RHC_PRINT_H
