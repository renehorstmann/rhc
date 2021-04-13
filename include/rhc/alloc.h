#ifndef RHC_ALLOC_H
#define RHC_ALLOC_H

#include <stdlib.h>
#include "assume.h"


//
// Types
//

typedef struct Allocator_s {
    void *user_data;

    // virtual functions
    void *(*alloc)(struct Allocator_s self, size_t size);
    void *(*realloc)(struct Allocator_s self, void *memory, size_t size);
    void (*free)(struct Allocator_s self, void *memory);
} Allocator_s;


//
// Functions
//

static void *allocator_default_alloc_impl_(Allocator_s self, size_t size) {
    return malloc(size);
}

static void *allocator_default_realloc_impl_(Allocator_s self, void *memory, size_t size) {
    return realloc(memory, size);
}

static void allocator_default_free_impl_(Allocator_s self, void *memory) {
    free(memory);
}

static void *allocator_raising_alloc_impl_(Allocator_s self, size_t size) {
    void *data = malloc(size);
    assume(data, "allocation failed for a size of: %zu", size);
    return data;
}

static void *allocator_raising_realloc_impl_(Allocator_s self, void *memory, size_t size) {
    void *data = realloc(self, memory, size);
    assume(data, "reallocation failed for a size of: %zu", size);
    return data;
}

static Allocator_s allocator_init_default() {
    return (Allocator_s) {NULL,
                          allocator_default_alloc_impl_,
                          allocator_default_realloc_impl_,
                          allocator_default_free_impl_};
}

static Allocator_s allocator_init_raising() {
    return (Allocator_s) {NULL,
                          allocator_raising_alloc_impl_,
                          allocator_raising_realloc_impl_,
                          allocator_default_free_impl_};
}



#endif //RHC_ALLOC_H
