#ifndef RHC_ALLOCATOR_IMPL_H
#define RHC_ALLOCATOR_IMPL_H
#ifdef RHC_IMPL

#include "../allocator.h"

//
// AllocatorArena
//

struct Rhc__Arena {
    RhcAllocator_i a;

    rhcsize stack_size;
    rhcsize used;
    rhcu8 *last_allocation;

    // empty last array -> arena is located behind Rhc__Arena struct in memory
    rhcu8 stack[];
};

static void *rhc__allocator_arena_realloc_impl(RhcAllocator_i allocator, void *memory, rhcsize n) {
    struct Rhc__Arena *self = allocator.impl;

    // free
    if(n == 0) {
        if(!memory)
            return NULL;
        // only possible if it was the last allocation
        if(self->last_allocation == memory) {
            self->used = self->last_allocation - self->stack;
            self->last_allocation = NULL;
        }
        return NULL;
    }

    // realloc
    if(memory) {
        // actual realloc
        if(self->last_allocation == memory) {
            rhcsize new_used = self->last_allocation - self->stack + n;
            if(new_used > self->stack_size)
                return NULL;
            self->used = new_used;
            return self->last_allocation;
        }

        // fall to malloc
    }

    // malloc
    if(self->used + n > self->stack_size) {
        return NULL;
    }
    self->last_allocation = self->stack + self->used;
    self->used += n;
    return self->last_allocation;
}

RhcAllocator_i rhc_allocator_arena_new_a(rhcsize arena_size, RhcAllocator_i arena_stack_allocator) {
    struct Rhc__Arena *self = rhc_a_malloc0(arena_stack_allocator, sizeof *self + arena_size);
    self->a = arena_stack_allocator;
    self->stack_size = arena_size;

    return (RhcAllocator_i) {
            .impl = self,
            .realloc = rhc__allocator_arena_realloc_impl
    };
}

void rhc_allocator_arena_kill(RhcAllocator_i *self_arena) {
    struct Rhc__Arena *self = self_arena->impl;
    rhc_a_free(self->a, self);
    *self_arena = rhc_allocator_new_invalid();
}

// clears / frees all allocations
void rhc_allocator_arena_clear(RhcAllocator_i self_arena) {
    struct Rhc__Arena *self = self_arena.impl;
    self->used = 0;
    self->last_allocation = NULL;
}

// returns the rhcsize the arena was created with
rhcsize rhc_allocator_arena_full_size(RhcAllocator_i self_arena) {
    struct Rhc__Arena *self = self_arena.impl;
    return self->stack_size;
}

// returns the current remaining rhcsize of the arena
rhcsize rhc_allocator_arena_remaining_size(RhcAllocator_i self_arena) {
    struct Rhc__Arena *self = self_arena.impl;
    return self->stack_size-self->used;
}


#endif //RHC_IMPL
#endif //RHC_ALLOCATOR_IMPL_H
