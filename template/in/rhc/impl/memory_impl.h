#ifndef RHC_MEMORY_IMPL_H
#define RHC_MEMORY_IMPL_H
#ifdef RHC_IMPL

#include "../error.h"
#include "../log.h"
#include "../memory.h"


#ifdef OPTION_SDL
#include <SDL2/SDL.h>
#endif

void *rhc_realloc_try(void *memory, rhcsize n) {
    n = rhc_max(n, 0);
    if(!memory && n==0)
        return NULL;
#ifdef OPTION_SDL
    void *data = SDL_realloc(memory, n);
#else
    void *data = realloc(memory, n);
#endif
    if(n>0 && !data) {
        rhc_log_error("reallocation failed for a size of: %lli", n);
        rhc_error_set("allocation error");
        return memory;
    }
    return data;
}

#endif //RHC_IMPL
#endif //RHC_MEMORY_IMPL_H
