#ifndef RHC_TYPES_H
#define RHC_TYPES_H

#include <stdbool.h>
#include <stddef.h>

//
// Options
//

#ifndef RHC_STRARRAY_S_SIZE
#define RHC_STRARRAY_S_SIZE 128
#endif

#ifndef RHC_STRING_DEFAULT_ALLOCATOR
#define RHC_STRING_DEFAULT_ALLOCATOR allocator_init_raising()
#endif


//
// Types
//

// virtuel allocator, see alloc.h
typedef struct Allocator_s {
    void *user_data;

    // virtual functions
    void *(*alloc)(struct Allocator_s self, size_t size);
    void *(*realloc)(struct Allocator_s self, void *memory, size_t size);
    void (*free)(struct Allocator_s self, void *memory);
} Allocator_s;

// string view, data is not null terminated
typedef struct {
    char *data;
    size_t size;
} str;

// array of str with fixed max size
typedef struct {
    str array[RHC_STRARRAY_S_SIZE];
    size_t size;
} StrArray_s;

// array of str, allocated
typedef struct {
    str *restrict array;
    size_t size;

    Allocator_s allocator;
} StrArray;

// dynamic str + always null terminated
typedef struct {
    // str
    str s;

    // buf
    size_t capacity;
    Allocator_s allocator;
} String;

#endif //RHC_TYPES_H
