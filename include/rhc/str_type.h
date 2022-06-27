#ifndef RHC_STR_TYPE_H
#define RHC_STR_TYPE_H

//
// Just the struct type for Str and StrArray
//

#include "allocator.h"

typedef struct RhcStr_s {
    char *data;
    rhcsize size;
} RhcStr_s;

// array of RhcStr_s, allocated
typedef struct RhcStrArray {
    RhcStr_s *array;
    rhcsize size;

    RhcAllocator_i allocator;
} RhcStrArray;


//
// class functions
//


// returns true if the str is valid
// a valid str has a data ptr, but may have a size==0
static bool rhc_str_valid(RhcStr_s str) {
    return str.data && str.size>=0;
}

// returns true if str array is valid
static bool rhc_strarray_valid(RhcStrArray self) {
    return self.array != NULL && rhc_allocator_valid(self.allocator);
}

// kills the str array
static void rhc_strarray_kill(RhcStrArray *self) {
    if (rhc_strarray_valid(*self))
        rhc_a_free(self->allocator, self->array);
    self->array = NULL;
    self->size = 0;
}

// creates an invalid str
static RhcStr_s rhc_str_new_invalid() {
    return (RhcStr_s) {NULL, 0};
}

#endif //RHC_STR_TYPE_H
