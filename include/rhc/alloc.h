#ifndef RHC_ALLOC_H
#define RHC_ALLOC_H

#include "types.h"


//
// Functions
//

// malloc + rhc_error + log
void *rhc_allocator_default_alloc_impl_(Allocator_s self, size_t size);

// realloc + rhc_error + log
void *rhc_allocator_default_realloc_impl_(Allocator_s self, void *memory, size_t size);

// free
void rhc_allocator_default_free_impl_(Allocator_s self, void *memory);

// malloc + assume
void *rhc_allocator_raising_alloc_impl_(Allocator_s self, size_t size);

// realloc + assume
void *rhc_allocator_raising_realloc_impl_(Allocator_s self, void *memory, size_t size);


static Allocator_s allocator_init_default() {
    return (Allocator_s) {NULL,
                          rhc_allocator_default_alloc_impl_,
                          rhc_allocator_default_realloc_impl_,
                          rhc_allocator_default_free_impl_};
}

static Allocator_s allocator_init_raising() {
    return (Allocator_s) {NULL,
                          rhc_allocator_raising_alloc_impl_,
                          rhc_allocator_raising_realloc_impl_,
                          rhc_allocator_default_free_impl_};
}



#endif //RHC_ALLOC_H
