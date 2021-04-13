#ifndef RHC_STR_H
#define RHC_STR_H

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "alloc.h"

//
// Options
//

#ifndef STR_STRARRAY_SIZE
#define STR_STRARRAY_SIZE 128
#endif


//
// Types
//

typedef struct {
    char *data;
    size_t size;
} str;

typedef struct {
    str array[STR_STRARRAY_SIZE];
    size_t size;
} strarray;

typedef struct {
    str *array;
    size_t size;

    Allocator_s allocator;
} StrArray;


//
// Functions
//

static str strc(char *cstring) {
    return (str) {cstring, strlen(cstring)};
}

static bool str_empty(str s) {
    return s.data == NULL || s.size == 0;
}

// copies from into out_cpy and returns out_cpy
static str str_cpy(str *out_cpy, str from) {
    assume(!str_empty(*out_cpy), "out_cpy must not be empty");
    size_t size = out_cpy->size < from.size? out_cpy->size : from.size;
    memcpy(out_cpy->data, from.data, size);
    return *out_cpy;
}

static str str_lstrip(str s, char strip) {
    if(isspace(strip)) {
        while (!str_empty(s) && isspace(*s.data)) {
            s.data++;
            s.size--;
        }
    } else {
        while (!str_empty(s) && *s.data == strip) {
            s.data++;
            s.size--;
        }
    }
    return s;
}

static str str_rstrip(str s, char strip) {
    if(isspace(strip)) {
        while (!str_empty(s) && isspace(s.data[s.size-1])) {
            s.size--;
        }
    } else {
        while (!str_empty(s) && s.data[s.size-1] == strip) {
            s.size--;
        }
    }
    return s;
}

static str str_strip(str s, char strip) {
    return str_lstrip(str_rstrip(s, strip), strip);
}


#endif //RHC_STR_H
