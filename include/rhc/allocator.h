#ifndef RHC_ALLOCATOR_H
#define RHC_ALLOCATOR_H

//
// This module contains a second namespace apart from allocator
//      allocator methods just use 'a' as namespace as rhc_a_malloc
//          so rhc_a_new (calls malloc on the allocator) does not collide with rhc_allocator_new (constructor)
//      The postfix *_a is also just for functions taking an allocator...
//

#include "export.h"
#include "memory.h"

//
// Options
//

#ifndef RHC_ALLOCATOR_DEFAULT
#define RHC_ALLOCATOR_DEFAULT rhc_allocator_new()
#endif

//
// Interface
//

struct RhcAllocator_i;

typedef void *(*rhc_allocator_realloc_fn)(struct RhcAllocator_i allocator, void *memory, rhcsize n);

typedef struct RhcAllocator_i {
    void *impl;

    // vtable
    // if memory is NULL -> malloc
    // if rhcsize is NULL -> free
    // should be ok to pass NULL, 0, *
    rhc_allocator_realloc_fn realloc;
} RhcAllocator_i;

//
// Functions
//

static bool rhc_allocator_valid(RhcAllocator_i self) {
    return self.realloc != NULL;
}

static RhcAllocator_i rhc_allocator_new_invalid() {
    return (RhcAllocator_i) {0};
}

// interface function for rhc_allocator_new_try
static void *rhc__allocator_new_try_realloc(RhcAllocator_i allocator, void *memory, rhcsize n) {
    return rhc_realloc_try(memory, n);
}

// interface function for rhc_allocator_new_try
static void *rhc__allocator_new_realloc(RhcAllocator_i allocator, void *memory, rhcsize n) {
    return rhc_realloc(memory, n);
}

// uses realloc_try
static RhcAllocator_i rhc_allocator_new_try() {
    return (RhcAllocator_i) {.realloc = rhc__allocator_new_try_realloc};
}

// uses realloc
static RhcAllocator_i rhc_allocator_new() {
    return (RhcAllocator_i) {.realloc = rhc__allocator_new_realloc};
}

//
// FUnctions (with smaller name to not callide with rhc_allocator_new)
//

// realloc
static void *rhc_a_realloc_try(RhcAllocator_i self, void *memory, rhcsize n) {
    return self.realloc(self, memory, n);
}

// malloc
static void *rhc_a_malloc_try(RhcAllocator_i self, rhcsize n) {
    return rhc_a_realloc_try(self, NULL, n);
}

// calloc
static void *rhc_a_malloc0_try(RhcAllocator_i self, rhcsize n) {
    void *mem = rhc_a_malloc_try(self, n);
    if(!mem)
        return NULL;
    memset(mem, 0, n);
    return mem;
}

// free
static void rhc_a_free(RhcAllocator_i self, void *memory) {
    rhc_a_realloc_try(self, memory, 0);
}

// malloc + rhc_assume
static void *rhc_a_malloc(RhcAllocator_i self, rhcsize n) {
    rhc_assume(n > 0, "at least 1 byte (could also be an overflow)");
    void *mem = rhc_a_malloc_try(self, n);
    rhc_assume(mem, "rhc_a_malloc failed");
    return mem;
}

// calloc + rhc_assume
static void *rhc_a_malloc0(RhcAllocator_i self, rhcsize n) {
    rhc_assume(n > 0, "at least 1 byte (could also be an overflow)");
    void *mem = rhc_a_malloc0_try(self, n);
    rhc_assume(mem, "rhc_a_malloc0 failed");
    return mem;
}

// realloc + rhc_assume
static void *rhc_a_realloc(RhcAllocator_i self, void *memory, rhcsize n) {
    rhc_assume(n >= 0, "overflow?");
    void *mem = rhc_a_realloc_try(self, memory, n);
    rhc_assume(mem, "rhc_a_realloc failed");
    return mem;
}


//
// Macro functions (with smaller name to not callide with rhc_allocator_new)
//

// calls allocator_free and sets the pointer to NULL
#define rhc_a_clear(self, mem) \
do { \
    a_free((self), (mem)); \
    (mem) = NULL; \
} while(0)

// calls rhc_a_malloc with the rhcsize of the struct
#define rhc_a_new(self, struct_type, n_structs) \
        ((struct_type *) rhc_a_malloc ((self), (n_structs) * sizeof (struct_type)))

// calls rhc_a_malloc0 with the rhcsize of the struct
#define rhc_a_new0(self, struct_type, n_structs) \
        ((struct_type *) rhc_a_malloc0 ((self), (n_structs) * sizeof (struct_type)))

// calls rhc_a_realloc with the rhcsize of the struct
#define rhc_a_renew(self, struct_type, mem, n_structs) \
        ((struct_type *) rhc_a_realloc ((self), mem, (n_structs) * sizeof (struct_type)))

// calls rhc_a_malloc_try with the rhcsize of the struct
#define rhc_a_try_new(self, struct_type, n_structs) \
        ((struct_type *) rhc_a_malloc_try ((self), (n_structs) * sizeof (struct_type)))

// calls rhc_a_malloc0_try with the rhcsize of the struct
#define rhc_a_try_new0(self, struct_type, n_structs) \
        ((struct_type *) rhc_a_malloc0_try ((self), (n_structs) * sizeof (struct_type)))

// calls rhc_a_realloc_try with the rhcsize of the struct
#define rhc_a_try_renew(self, struct_type, mem, n_structs) \
        ((struct_type *) rhc_a_realloc_try ((self), mem, (n_structs) * sizeof (struct_type)))
        
        
//
// AllocatorArena
//

// An arena allocates everything on a fixed stack
// If realloc is called, the new object is either copied to the end,
//      or if it was the last element, its rhcsize is just updates
// the arena can be cleared with rhc_allocator_arena_clear to free all allocations
// the arena must also be killed with rhc_allocator_arena_kill (which will also free all allocations)
// Stack a version
RHC_EXPORT
RhcAllocator_i rhc_allocator_arena_new_a(rhcsize arena_size, RhcAllocator_i arena_stack_allocator);

// An arena allocates everything on a fixed stack
// If realloc is called, the new object is either copied to the end,
//      or if it was the last element, its rhcsize is just updates
// the arena can be cleared with rhc_allocator_arena_clear to free all allocations
// the arena must also be killed with rhc_allocator_arena_kill (which will also free all allocations)
static RhcAllocator_i rhc_allocator_arena_new(rhcsize arena_size) {
    return rhc_allocator_arena_new_a(arena_size, RHC_ALLOCATOR_DEFAULT);
}

RHC_EXPORT
void rhc_allocator_arena_kill(RhcAllocator_i *self_arena);

// clears / frees all allocations
RHC_EXPORT
void rhc_allocator_arena_clear(RhcAllocator_i self_arena);

// returns the rhcsize the arena was created with
RHC_EXPORT
rhcsize rhc_allocator_arena_full_size(RhcAllocator_i self_arena);

// returns the current remaining rhcsize of the arena
RHC_EXPORT
rhcsize rhc_allocator_arena_remaining_size(RhcAllocator_i self_arena);


#endif //RHC_ALLOCATOR_H
