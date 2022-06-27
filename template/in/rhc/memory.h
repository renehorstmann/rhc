#ifndef RHC_MEMORY_H
#define RHC_MEMORY_H

//
// safe malloc and co
//

#include "common.h"
#include "assume.h"

//
// Functions
//

// realloc (see impl/memory_impl.h)
void *rhc_realloc_try(void *memory, rhcsize n);


// malloc
static void *rhc_malloc_try(rhcsize n) {
    return rhc_realloc_try(NULL, n);
}

// calloc
static void *rhc_malloc0_try(rhcsize n) {
    void *mem = rhc_malloc_try(n);
    if(!mem)
        return NULL;
    memset(mem, 0, n);
    return mem;
}

// free
static void rhc_free(void *memory) {
    rhc_realloc_try(memory, 0);
}

// malloc + rhc_assume
static void *rhc_malloc(rhcsize n) {
    rhc_assume(n > 0, "at least 1 byte (could also be an overflow)");
    void *mem = rhc_malloc_try(n);
    rhc_assume(mem, "rhc_malloc failed");
    return mem;
}

// calloc + rhc_assume
static void *rhc_malloc0(rhcsize n) {
    rhc_assume(n > 0, "at least 1 byte (could also be an overflow)");
    void *mem = rhc_malloc0_try(n);
    rhc_assume(mem, "rhc_malloc0 failed");
    return mem;
}

// realloc + rhc_assume
static void *rhc_realloc(void *memory, rhcsize n) {
    rhc_assume(n >= 0, "overflow?");
    void *mem = rhc_realloc_try(memory, n);
    rhc_assume(n==0||mem, "rhc_realloc failed");
    return mem;
}


//
// Macro functions
//

// calls rhc_free and sets the pointer to NULL
#define rhc_clear(mem) \
do { \
    rhc_free((mem)); \
    (mem) = NULL; \
} while(0)

// calls rhc_malloc with the rhcsize of the struct
#define rhc_new(struct_type, n_structs) \
        ((struct_type *) rhc_malloc ((n_structs) * sizeof (struct_type)))

// calls rhc_malloc0 with the rhcsize of the struct
#define rhc_new0(struct_type, n_structs) \
        ((struct_type *) rhc_malloc0 ((n_structs) * sizeof (struct_type)))

// calls rhc_relloc0 with the rhcsize of the struct
#define rhc_renew(struct_type, mem, n_structs) \
        ((struct_type *) rhc_realloc (mem, (n_structs) * sizeof (struct_type)))

// calls rhc_malloc_try with the rhcsize of the struct
#define rhc_new_try(struct_type, n_structs) \
        ((struct_type *) rhc_malloc_try ((n_structs) * sizeof (struct_type)))

// calls rhc_malloc0_try with the rhcsize of the struct
#define rhc_new0_try(struct_type, n_structs) \
        ((struct_type *) rhc_malloc0_try ((n_structs) * sizeof (struct_type)))

// calls rhc_realloc_try with the rhcsize of the struct
#define rhc_renew_try(struct_type, mem, n_structs) \
        ((struct_type *) rhc_realloc_try (mem, (n_structs) * sizeof (struct_type)))
        

#endif //RHC_MEMORY_H
