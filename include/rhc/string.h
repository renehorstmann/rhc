#ifndef RHC_STRING_H
#define RHC_STRING_H

//
// Dynamic string that may grow
//      Can be used as a stream (see stream.h)
// When a function returns a string, you have to kill it
//

#include "str_type.h"
#include "stream.h"

//
// Class
//

typedef struct RhcString {
    // RhcStr_s
    char *data;
    rhcsize size;

    // buf
    rhcsize capacity;
    RhcAllocator_i allocator;

    // read and write the string with a stream
    RhcStream_i stream;
    rhcsize stream_pos;
} RhcString;

//
// Functions
//

static bool rhc_string_valid(const RhcString *self) {
    return self
           && self->data && self->size>=0
           && self->capacity > 0
           && rhc_allocator_valid(self->allocator)
           && self->stream_pos >= 0 && self->stream_pos <= self->size;
}

static RhcString *rhc_string_new_invalid() {
    return NULL;
}

// returns a Str from the data
static RhcStr_s rhc_string_get_str(RhcString *self) {
    if(!rhc_string_valid(self))
        return rhc_str_new_invalid();
    return (RhcStr_s) {self->data, self->size};
}

// safe way to get the stream from the string
static RhcStream_i rhc_string_get_stream(RhcString *self) {
    if(!self)
        return rhc_stream_new_invalid();
    return self->stream;
}


// allocated start_capacity + 1 (null)
// start_capacity will be set to a minimum of 7
RhcString *rhc_string_new_a(rhcsize start_capacity, RhcAllocator_i a);

// allocated start_capacity + 1 (null)
static RhcString *rhc_string_new(rhcsize start_capacity) {
    return rhc_string_new_a(start_capacity, RHC_ALLOCATOR_DEFAULT);
}

// clones RhcStr_s and appends null
RhcString *rhc_string_new_clone_a(RhcStr_s to_clone, RhcAllocator_i a);


// clones RhcStr_s and appends null
static RhcString *rhc_string_new_clone(RhcStr_s to_clone) {
    return rhc_string_new_clone_a(to_clone, RHC_ALLOCATOR_DEFAULT);
}

// copies str s into a new string, with old -> replacement.
RhcString *rhc_string_new_replace_a(RhcStr_s to_replace, RhcStr_s old, RhcStr_s replacement, RhcAllocator_i a);


// copies str s into a new string, with old -> replacement.
static RhcString *rhc_string_new_replace(RhcStr_s to_replace, RhcStr_s old, RhcStr_s replacement) {
    return rhc_string_new_replace_a(to_replace, old, replacement, RHC_ALLOCATOR_DEFAULT);
}

// concatenates all strs
RhcString *rhc_string_new_cat_a(RhcStr_s *strs, int n, RhcAllocator_i a);

// concatenates all strs
static RhcString *rhc_string_new_cat(RhcStr_s *strs, int n) {
    return rhc_string_new_cat_a(strs, n, RHC_ALLOCATOR_DEFAULT);
}

void rhc_string_kill(RhcString **self_ptr);

// size is the sum of characters, not including termination null (as strlen)
void rhc_string_set_capacity(RhcString *self, rhcsize capacity);

// size is the sum of characters, not including termination null (as strlen)
void rhc_string_resize(RhcString *self, rhcsize size);

// appends a char
void rhc_string_push(RhcString *self, char push);

// appends a string
void rhc_string_append(RhcString *self, RhcStr_s append);

#endif //RHC_STRING_H
