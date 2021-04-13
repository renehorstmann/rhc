#ifndef RHC_STRBUF_H
#define RHC_STRBUF_H

#include "str.h"

//
// Options
//

#ifndef STRBUF_DEFAULT_ALLOCATOR
#define STRBUF_DEFAULT_ALLOCATOR allocator_init_raising()
#endif


//
// Types
//

typedef struct {
    // str
    str s;

    // buf
    size_t capacity;
    Allocator_s allocator;
} strbuf;


//
// Functions
//

static strbuf strbuf_new_a(size_t start_capacity, Allocator_s a) {
    return (strbuf) {
            {
                a.alloc(a, start_capacity),
                0
            },
            start_capacity,
            a
    };
}

static strbuf strbuf_new(size_t start_capacity) {
    return strbuf_new_a(start_capacity, STRBUF_DEFAULT_ALLOCATOR);
}

static strbuf strbuf_new_clone_a(str to_clone, Allocator_s a) {
    strbuf sb = strbuf_new_a(to_clone.size, a);
    str_cpy(&sb.s, to_clone);
    return sb;
}

static strbuf strbuf_new_clone(str to_clone) {
    return strbuf_new_clone_a(to_clone, STRBUF_DEFAULT_ALLOCATOR);
}

static void strbuf_delete(strbuf *self) {
    self->allocator.free(self->allocator, self->s.data);
    memset(self, 0, sizeof(*self));
}

static void strbuf_append(strbuf *self, str append) {
    size_t size = self->s.size + append.size;
    if(size > self->capacity) {
        size_t new_cap = size*2;
        void *data = self->allocator.realloc(self->allocator, self->s.data, new_cap);
        if(!data)
            return;
        self->s.data = data;
        self->capacity = new_cap;
    }
    memcpy(self->s.data+self->s.size, append.data, append.size);
    self->s.size = size;
}



#endif //RHC_STR_H
