// this header file does not have an include guard!
// can be used multiple times, with different types

//
// dynamic hashmap with a trivisl key type (pod structs, int, float, ...)
//      see hashmap.h for the base hashmap class
//

#include <string.h>
#include "memory.h"
#include "error.h"

static KEY rhc__hashmap_trivial_key_clone(KEY key, sAllocator_i a) {
    return key;
}

static void rhc__hashmap_trivial_key_kill(KEY key, sAllocator_i a) {
    // noop
}

static bool rhc__hashmap_trivial_key_equals(KEY a, KEY b) {
    return memcmp(&a, &b, sizeof (KEY)) == 0;
}

static rhcu32 rhc__hashmap_trivial_key_hash(KEY key) {
    rhcu32 hash = 5381;
    const su8 *it = (su8*) &key;
    int c;
    for(int i=0; i<sizeof (KEY); i++) {
        c = it[i];
        hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

        
#define KEY_CLONE_FN rhc__hashmap_trivial_key_clone
#define KEY_KILL_FN rhc__hashmap_trivial_key_kill
#define KEY_EQUALS_FN rhc__hashmap_trivial_key_equals
#define KEY_HASH_FN rhc__hashmap_trivial_key_hash
#include "hashmap.h"
