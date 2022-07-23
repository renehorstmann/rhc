// this header file does not have an include guard!
// can be used multiple times, with different types

//
// dynamic hashmap with strings as keys
//      see hashmap.h for the base hashmap class
//

#include <string.h>
#include "memory.h"
#include "error.h"

static const char *rhc__hashmap_string_key_clone(const char *key, sAllocator_i a) {
    char *clone = rhc_a_malloc(a, strlen(key) + 1);
    rhc_assume(clone, "hashmap_string failed to clone a key");
    memcpy(clone, key, strlen(key)+1);
    return clone;
}

static void rhc__hashmap_string_key_kill(const char *key, sAllocator_i a) {
    rhc_a_free(a, (void *) key);
}

static bool rhc__hashmap_string_key_equals(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

static rhcu32 rhc__hashmap_string_key_hash(const char *key) {
    rhcu32 hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */
    return hash;
}


#define KEY const char *
#define KEY_CLONE_FN rhc__hashmap_string_key_clone
#define KEY_KILL_FN rhc__hashmap_string_key_kill
#define KEY_EQUALS_FN rhc__hashmap_string_key_equals
#define KEY_HASH_FN rhc__hashmap_string_key_hash
#include "hashmap.h"
