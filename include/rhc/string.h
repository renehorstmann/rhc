#ifndef RHC_STRING_H
#define RHC_STRING_H

#include <string.h>
#include "types.h"
#include "alloc.h"


//
// Functions
//

static String String_new_invalid_a(Allocator_s a) {
    return (String) {NULL, .allocator = a};
}

static String String_new_invalid() {
    return (String) {NULL, .allocator = RHC_STRING_DEFAULT_ALLOCATOR};
}

static bool String_valid(String self) {
    return self.s.data != NULL;
}

// allocated start_capacity + 1 (null)
static String String_new_a(size_t start_capacity, Allocator_s a) {
    String res = {
            {
                a.alloc(a, start_capacity + 1),
                0
            },
            start_capacity,
            a
    };
    if(!res.s.data)
        return String_new_invalid_a(a);
    memset(res.s.data, 0, start_capacity + 1);
    return res;
}

// allocated start_capacity + 1 (null)
static String String_new(size_t start_capacity) {
    return String_new_a(start_capacity, RHC_STRING_DEFAULT_ALLOCATOR);
}

// clones str and appends null
static String String_new_clone_a(str to_clone, Allocator_s a) {
    String sb = String_new_a(to_clone.size, a);
    if(!String_valid(sb))
        return sb;
    memcpy(sb.s.data, to_clone.data, to_clone.size);
    return sb;
}

// clones str and appends null
static String String_new_clone(str to_clone) {
    return String_new_clone_a(to_clone, RHC_STRING_DEFAULT_ALLOCATOR);
}

// deletes string
static void String_delete(String *self) {
    self->allocator.free(self->allocator, self->s.data);
    *self = String_new_invalid_a(self->allocator);
}

// size is the sum of characters, not including termination null (as strlen)
static void String_resize(String *self, size_t size) {
    if(size > self->capacity) {
        size_t new_cap = size*2;
        void *data = self->allocator.realloc(self->allocator, self->s.data, new_cap+1);
        if(!data) {
            String_delete(self);
            return;
        }
        self->s.data = data;
        self->capacity = new_cap;
        memset(&self->s.data[self->s.size], 0, self->capacity+1-self->s.size);
    }
    self->s.size = size;
    self->s.data[self->s.size] = '\0';  //just to be sure
}

static void String_append(String *self, str append) {
    char *end = self->s.data + self->s.size;
    String_resize(self, self->s.size + append.size);
    memcpy(end, append.data, append.size);
    self->s.data[self->s.size] = '\0';  //just to be sure
}



#endif //RHC_STR_H
