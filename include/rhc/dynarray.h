// this header file does not have an include guard!
// can be used multiple times, with different types

//
// dynamic array like a std::vector
//

#include "allocator.h"
#include "log.h"


#define RHC_NAME_CONCAT(a, b) a ## b
#define RHC_NAME_CONCAT2(a, b) RHC_NAME_CONCAT(a, b)
#define RHC_TO_STRING(a) #a
#define RHC_TO_STRING2(a) RHC_TO_STRING(a)

//
// Options:
//

// array type, for example int
#ifndef TYPE
#error dynarray.h needs a type (e.g.: #define TYPE int)
#endif

// array class name, for example Foo
#ifndef CLASS
#define CLASS RHC_NAME_CONCAT2(DynArray_, TYPE)
#endif

// array function names, for example foo
#ifndef FN_NAME
#define FN_NAME RHC_NAME_CONCAT2(dynarray_, TYPE)
#endif

// remove copying functions (push)
//#define NO_COPY



// so the example would be:
// #define TYPE int
// #define CLASS Foo
// #define FN_NAME foo
// #include "rhc/dynarray.h"
//
// Foo foo = foo_new(&foo, 32);   // start_capacity
// foo_push(&foo, 7);             // push the int 7 into the array
// printf("foo.array[0]=%d ; foo.size=%d", foo.array[0], foo.size);
// foo_kill(&foo);


typedef struct CLASS {
    TYPE *array;
    rhcsize size;

    rhcsize capacity;
    RhcAllocator_i allocator;
} CLASS;


// bool foo_valid(Foo self)
static bool RHC_NAME_CONCAT2(FN_NAME, _valid)(CLASS self) {
    return self.array != NULL && rhc_allocator_valid(self.allocator);
}

// forward declaration
// Foo foo_new_invalid_a(RhcAllocator_i a)
static CLASS RHC_NAME_CONCAT2(FN_NAME, _new_invalid_a)(RhcAllocator_i a);


// Foo foo_new_a(rhcsize start_capacity, RhcAllocator_i a)
static CLASS RHC_NAME_CONCAT2(FN_NAME, _new_a)(rhcsize start_capacity, RhcAllocator_i a) {
    rhc_assume(rhc_allocator_valid(a), "a needs to be valid");

    if(start_capacity <= 0)
        return RHC_NAME_CONCAT2(FN_NAME, _new_invalid_a)(a);

    CLASS self = {
            rhc_a_new(a, TYPE, start_capacity),
            0,
            start_capacity,
            a
    };
    if (!self.array) {
        rhc_error_set("dynarray_new failed");
        rhc_log_error(RHC_TO_STRING2(FN_NAME) "_new failed: for capacity: %zu", start_capacity);
        return (CLASS) {.allocator = a};
    }
    return self;
}

// Foo foo_new(rhcsize start_capacity)
static CLASS RHC_NAME_CONCAT2(FN_NAME, _new)(rhcsize start_capacity) {
    // new_a
    return RHC_NAME_CONCAT2(FN_NAME, _new_a)(start_capacity, RHC_ALLOCATOR_DEFAULT);
}

// Foo foo_new_invalid_a(RhcAllocator_i a)
static CLASS RHC_NAME_CONCAT2(FN_NAME, _new_invalid_a)(RhcAllocator_i a) {
    return (CLASS) {.allocator = a};
}

// Foo foo_new_invalid_a()
static CLASS RHC_NAME_CONCAT2(FN_NAME, _new_invalid)() {
    // new_invalid_a
    return RHC_NAME_CONCAT2(FN_NAME, _new_invalid_a)(RHC_ALLOCATOR_DEFAULT);
}

// Foo foo_new_clone_a(int *to_clone, rhcsize n, RhcAllocator_i a)
static CLASS RHC_NAME_CONCAT2(FN_NAME, _new_clone_a)(TYPE *to_clone, rhcsize n, RhcAllocator_i a) {
    // new_a
    CLASS self = RHC_NAME_CONCAT2(FN_NAME, _new_a)(n, a);

    // valid
    if(!RHC_NAME_CONCAT2(FN_NAME, _valid)(self))
        return self;

    memcpy(self.array, to_clone, n * sizeof(TYPE));
    self.size = n;
    return self;
}

// Foo foo_new_clone(int *to_clone, rhcsize n)
static CLASS RHC_NAME_CONCAT2(FN_NAME, _new_clone)(TYPE *to_clone, rhcsize n) {
    // new_clone_a
    return RHC_NAME_CONCAT2(FN_NAME, _new_clone_a)(to_clone, n, RHC_ALLOCATOR_DEFAULT);
}

// void foo_kill(Foo *self)
static void RHC_NAME_CONCAT2(FN_NAME, _kill)(CLASS *self) {
    // valid
    if(RHC_NAME_CONCAT2(FN_NAME, _valid)(*self)) {
        rhc_a_free(self->allocator, self->array);
    }
    // new_invalid_a
    *self = RHC_NAME_CONCAT2(FN_NAME, _new_invalid_a)(self->allocator);
}

// void foo_set_capacity(Foo *self, rhcsize capacity)
static void RHC_NAME_CONCAT2(FN_NAME, _set_capacity)(CLASS *self, rhcsize capacity) {
    // !valid
    if(!RHC_NAME_CONCAT2(FN_NAME, _valid)(*self))
        return;

    void *array = rhc_a_renew(self->allocator, TYPE, self->array, capacity);
    if(!array) {
        // kill
        rhc_error_set("dynarray_set_capacity failed");
        rhc_log_error(RHC_TO_STRING2(FN_NAME) "_set_capacity failed: for capacity: %zu", capacity);
        RHC_NAME_CONCAT2(FN_NAME, _kill)(self);
        return;
    }
    self->array = array;
    self->capacity = capacity;
    if(capacity < self->size)
        self->size = capacity;
}

// void foo_resize(Foo *self, rhcsize size)
static void RHC_NAME_CONCAT2(FN_NAME, _resize)(CLASS *self, rhcsize size) {
    if(size > self->capacity) {
        // _set_capacity
        RHC_NAME_CONCAT2(FN_NAME, _set_capacity)(self, size * 2);
    }

    // valid
    if(RHC_NAME_CONCAT2(FN_NAME, _valid)(*self)) {
        self->size = size;
    }
}

// int *foo_append_array(Foo *self, int *opt_init_array, rhcsize array_size)
static TYPE *RHC_NAME_CONCAT2(FN_NAME, _append_array)(CLASS *self, TYPE *opt_init_array, rhcsize array_size) {
    // resize
    RHC_NAME_CONCAT2(FN_NAME, _resize)(self, self->size+array_size);

    TYPE *ret = &self->array[self->size-array_size];
    if(opt_init_array)
        memcpy(ret, opt_init_array, sizeof(TYPE) * array_size);
    return ret;
}

// int *foo_append(Foo *self, int *opt_init)
static TYPE *RHC_NAME_CONCAT2(FN_NAME, _append)(CLASS *self, TYPE *opt_init) {
    // append_array
    return RHC_NAME_CONCAT2(FN_NAME, _append_array)(self, opt_init, 1);
}


#ifndef NO_COPY
// void foo_push(Foo *self, int push)
static void RHC_NAME_CONCAT2(FN_NAME, _push)(CLASS *self, TYPE push) {
    // append
    RHC_NAME_CONCAT2(FN_NAME, _append)(self, &push);
}

// TYPE foo_pop(Foo *self)
static TYPE RHC_NAME_CONCAT2(FN_NAME, _pop)(CLASS *self) {
    // !valid || self->size <= 0
    if(!RHC_NAME_CONCAT2(FN_NAME, _valid)(*self) || self->size <= 0) {
        rhc_log_error(RHC_TO_STRING2(FN_NAME) "_pop failed: invalid or size = 0");
        return (TYPE) {0};
    }
    TYPE ret = self->array[self->size-1];

    // resize
    RHC_NAME_CONCAT2(FN_NAME, _resize)(self, self->size-1);

    return ret;
}

#endif



#undef TYPE
#undef CLASS
#undef FN_NAME
#undef NO_COPY
