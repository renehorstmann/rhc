#include "rhc/rhc_impl.h"   // once in a project

#include <stdio.h>

#define TYPE int
#include "rhc/dynarray.h"
// if CLASS is not defined, it will be DynArray_int in this case
// if FN_NAME is not definied, it will be dynarray_* in this case


#define TYPE float
#define CLASS FltArr
#define FN_NAME flt_arr
#include "rhc/dynarray.h"

// it is also possible to create an dynarray of a dynarray (of a dynarray, of ...)
#define TYPE FltArr
#define CLASS FltArrArr
#define FN_NAME flt_arr_arr
#include "rhc/dynarray.h"

// an array as TYPE can not be copied, or assigned (a = b to copy)
// so set NO_COPY to not include the functions push and pop (use append and resize)
typedef float VEC4[4];
#define TYPE VEC4
#define NO_COPY
#include "rhc/dynarray.h"



int main() {
    DynArray_int int_array = dynarray_int_new(32);  // start_capacity

    // push values into the array
    dynarray_int_push(&int_array, 1);
    dynarray_int_push(&int_array, 2);
    dynarray_int_push(&int_array, 3);
    dynarray_int_push(&int_array, 4);

    printf("popped: %i\n", dynarray_int_pop(&int_array));
    // popped: 4

    for(int i=0; i<int_array.size; i++) {
        printf("int_array[%i] = %i\n", i, int_array.array[i]);
    }
    // int_array[0] = 1
    // int_array[1] = 2
    // int_array[2] = 3

    // free
    dynarray_int_kill(&int_array);


    FltArr flts = flt_arr_new(16);

    // push an array (returns a pointer to the first pushed value)
    flt_arr_append_array(&flts, (float[]) {1, 2, 3, 4}, 4);

    // clones flts to copy, can float any float* array
    FltArr cpy = flt_arr_new_clone(flts.array, flts.size);

    // removes 3, 4
    flt_arr_resize(&flts, 2);

    FltArrArr arrays = flt_arr_arr_new_clone((FltArr[]) {flts, cpy}, 2);

    for(int a=0; a<arrays.size; a++) {
        for(int i=0; i<arrays.array[a].size; i++) {
            printf("array[%i][%i] = %.1f\n", a, i, arrays.array[a].array[i]);
        }
    }
    // array[0][0] = 1.0
    // array[0][1] = 2.0
    // array[1][0] = 1.0
    // array[1][1] = 2.0
    // array[1][2] = 3.0
    // array[1][3] = 4.0


    flt_arr_kill(&flts);
    flt_arr_kill(&cpy);
    flt_arr_arr_kill(&arrays);
}
