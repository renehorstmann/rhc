#include "rhc/rhc_impl.h"   // once in a project


// create a hashmap with item type = int, item key = const char *
// if you want another keytype, use hashmap.h and set the key functions (see hashmap_string.h for an example)
#define TYPE int
#include "rhc/hashmap_string.h"
// if CLASS is not defined, it will be HashMap_int in this case
// if FN_NAME is not defined, it will be hashmap_* in this case



int main() {
    HashMap_int int_map = hashmap_int_new(32);  // approx_size

    // get or create an item of key "apple"
    int *item = hashmap_int_get(&int_map, "apple");
    *item = 10;

    // banana = 3; ananas = 77
    *hashmap_int_get(&int_map, "banana") = 3;
    *hashmap_int_get(&int_map, "ananas") = 77;

    // overwrite to apple = 13
    *hashmap_int_get(&int_map, "apple") += 3;

    // removes ananas item
    hashmap_int_remove(&int_map, "ananas");

    // iterate through the whole map
    {
        HashMap_intIter_s iter = hashmap_int_iter_new(&int_map);
        HashMap_intItem_s *i;
        while((i = hashmap_int_iter_next(&iter)) != NULL) {
            printf("{%s : %i}\n", i->key, i->value);
        }
    }
    // {banana : 3}
    // {apple : 13}
}
