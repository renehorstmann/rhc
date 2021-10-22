# rhc C style guide
A style guide for modern C programming

In this file I'll show you my recommendations for a good coding style in the C programming language.

## <a name="S-contents"></a>Contents

- [tl;dr naming](#S-tldrnaming)

- [Basics](#S-basics)
    - [Where to put variable instantiations](#S-basics-where_variables)
    - [Prefer autotypes](#S-basics-autotypes)

- [Error handling](#S-err)
    - [Avoid when possible](#S-err-avoid_when_possible)
    - [Error delivery](#S-err-error_delivery)
        - [Compile Time](#S-err-compile_time)
        - [Debug Time](#S-err-debug_time)
        - [Run Time](#S-err-run_time)
            - [Signals](#S-err-signals)
            - [Parameter](#S-err-parameter)
    - [Illegal State](#S-err-illegal_state)
    - [Error Paramter Options](#S-err-error_parameter_options)

- [Naming](#S-naming)
    - [Variables](#S-naming-variables)
    - [Functions](#S-naming-functions)
    - [Macros](#S-naming-macros)
    - [Structs](#S-naming-structs)
    - [Classes](#S-naming-classes)
    - [Namespaces](#S-naming-namespaces)

- [Object orientation in C](#S-oo)
    - [When not to use](#S-oo-not)
    - [Simple machine](#S-oo-simple)
    - [Inheritance](#S-oo-inheritance)
    - [RTTI](#S-oo-rtti)
    - [Virtual Methods](#S-oo-virtual)
    - [Interfaces](#S-oo-interfaces)

## <a name="S-tldrnaming"></a>tl;dr naming
Here is the rhc naming scheme:

- variables
  - `int snake_case;`

- globals (only if really necessary)
  - `int ns_snake_case; // ns=namespace like rhc`

- constants
  - `#define NS_SCREAM_CASE 123`
  - `static const int NS_SCREAM_CASE = 123;`

- static functions in source file
  - `static void snake_case();`

- static functions in headers
  - `static void ns_snake_case();`
  - `static void module_snake_case(); // e. g. file_read in the file.h module`

- global functions
  - `void ns_snake_case();`

- macros
  - see functions or PascalCase

- pod struct (trivially copyable / autotype structs)
  - `typedef struct { int color; } PascalCase_s; // or NsPascalCase_s`

- classes
  - `typedef struct { void *memory; } PascalCase; // or NsPascalCase`

- interfaces
  - `typedef struct { void *user_data; void (*vfunc)(); } PascalCase_i; // or NsPascalCase_i`

- methods
  - `void class_name_snake_case(); // e. g. void string_resize(String *self, size_t size);` 
  - `void ns_class_name_snake_case();`

- class constructor
  - `Class class_new();`
  - `NsClass ns_class_new();`
  - `Class *class_new(); // class is allocated (for interfaces, oop, ...)`
  - `NsClass *ns_class_new();`

- class destructor
  - `void class_kill(Class *self); // also clears the class with 0 to an invalid state`
  - `void ns_class_kill(Class *self);`
  - `void class_kill(Class **self_ptr); // used for allocated classes, to also set the pointer to NULL`
  - `void ns_class_kill(Class **self_ptr);`

## <a name="S-basics"></a>Basics
### <a name="S-basics-where_variables"></a>Where to put variable instantiations
In old C compilers, variable instantiations must be at the beginning of a function.
In modern C (if you dont care for function stack sizes), you can and should create variables at the line, where they are first needed.
(If you need a variable multiple times for different use cases (e. g. error codes), put it at the start).
```c
void foo() {
    int sum = 0;

    // declare i within the for loop
    for(int i=0; i<10; i++)
        sum += i*i;

    float inv = 1.0f / sum;

    // bundle complicated assignments
    int *array;
    int size;
    bool use_array;
    {
        if(sum > 20) {
            // rhc/alloc.h contains allocators, that can raise a signal if failed (with assume from rhc/error.h)
            array = (int*) rhc_malloc(16);
            size = 4;
        } else {
            array = (int*) rhc_malloc(4);
            size = 1;
        }
        use_array = true;
    }
}
```

### <a name="S-basics-autotypes"></a>Prefer autotypes
Always prefer autotypes, e. g. use char str[64] instead of char *str_heap = malloc(64).
Its not only faster, but you also dont need to worry about freeing memory.
Structs that represents dynamic arrays can also make use of them:
```c
// An autotype struct that can safe up to 1024 indices (ints)
typedef struct {
    int data[1024];
    int size;
} Indices_s; 
```
The disadvantage is of course, that these array autotypes are limited in size,
but if the contents are small emough, always prefer them.




## <a name="S-err"></a>Error handling

### <a name="S-err-avoid_when_possible"></a> Avoid when possible
If possible, always write functions that do not produce any errors at all.
For example:
```c
int count_char(const char *string, char c) {
    if(!string)
        return 0; // simply return 0 if the string is invalid
    int cnt = 0;
    while(*string) {
        if(*string++ == c)
            cnt++;
    }
    return cnt;
}

vec3 rgb2hsv(vec3 rgb) {
    // rgb must be in range [0:1]
    // instead of reporting an error, we just clamp the values
   
    rgb = vec3_clamp(rgb, 0, 1); // see my Mathc repository
   
    // ...
}
```

### <a name="S-err-error_delivery"></a> Error delivery
If you must do some error management, determine what the error should do:
- [Create a compile time error](#S-err-compile_time)
- [Create an error in the debug session](#S-err-debug_time)
- [Create a run time error](#S-err-run_time)
    - [via a signal](#S-err-signal)
    - [via a function parameter](#S-err-parameter)


#### <a name="S-err-compile_time"></a> Compile Time
If a function/macro is completely misused (wrong types...), try to generate a compile warning/error.
For example:
```c
// array size may be < 3
void very_bad_array3_zero(float *array3) {
    memset(array3, 0, 3 * sizeof(float));
}


// array size may also be <  if the function is called
typedef float array3[3];

void bad_array3_zero(array3 arr) {
    memset(arr, 0, sizeof(array3);
}


// compiler will at least throw a warning, if the type is wrong
typedef struct {
    float a[3];
} Array3_s;

void good_array3_zero(Array3_s *arr) {
    memset(arr, 0, sizeof(Array3_s);
}

// will not compile if not used properly
Array3_s very_good_array3_zero() {
    return (Array3_s) {0};
}
```
Have a look at [Mathc](https://github.com/renehorstmann/Mathc) for a linear algebra math library,
that contains structs for vectors and matrices (`vec3`, `mat4`)

#### <a name="S-err-debug_time"></a> Debug Time
If a function is misused (should have called another way), use assertions.
For example:
```c
static int machine;
void machine_start() {
    machine = 1;
}

void machine_send(int msg) {
    assert(machine != 0 && "machine_start() called?");
    printf("%d\n", msg);
}

// dereferencing NULL causes a segmentation fault on some systems,
// but its still undefined behavior...
void machine_work(int *data, int n) {
    // would raise SIGSEGV on Linux, but better check it...
    // platform independent NULL check
    assert(data && "must not be NULL");
    for(int i=0; i<n; i++)
        printf("%d\n", data[i]);
}
```

#### <a name="S-err-run_time"></a> Run Time
There are two versions of run time erros. Predictable errors and "Should not happen" errors.
The "Should not happen" errors should let the program or module die.
In order to do this, we can raise a signal.

##### <a name="S-err-signals"></a> Signals
We can use signals to inform the user, that something unexpected happened.
For example:
```c
void runtime_assert() {
    int a = get_positive_integer();

    // if an assumption fails at runtime, a signal is raised
    assume(a>0, "get_positive failed");  // see rhc/error.h
    printf("%d\n", 1/a );
}
```
If you do not want a module to crash the whole program, you have two options:
- Sandbox the module in an own process with fork
- Recover from a function call with a signal handler and long jumps

##### <a name="S-err-parameter"></a> Parameter
If the error is common or predictable, report the error with a function parameter or return value to the user.
This seems to be the default, but consider the above options to minimize these.
For more informations on how to use them, see [section below](#S-err-error_parameter_options).


### <a name="S-err-illegal_state"></a> Illegal state
If you have multiple functions, that use the same state, make the state illegal if a function fails.
So you must not check every function for failure and just once for all functions in a sequence.
For example:
```c
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    FILE *f;
    // ...
} Reader;

bool reader_valid(Reader self) {
    return self.f != NULL;
}

int reader_next_int(Reader *self) {
    if(!reader_valid(*self))
        return 0;

    // only read if Reader is valid
    int res = 0;
    if(fscanf(self->f, "%d", &res) != 1) {
         // read failed, so make Reader invalid
         self->f = NULL;
    }
    return res;
}
// in this example, the FILE* is the state and determines if the Reader is valid
// each method/function working on Reader,
//    must check if the reader is valid 
//    and set it invalid if an error occured

int main() {
    puts("Give me 3 ints...");

    Reader r = {stdin};
    
    // multiple calls, but only one check:
    int a = reader_next_int(&r);
    int b = reader_next_int(&r);
    int c = reader_next_int(&r);

    // check for an error
    if(!reader_valid(r))
        puts("I said ints!!!");
    else
        printf("sum is %d\n", a+b+c);
}
```

With this technique, you could build an api this way:
(pseudo code, but rhc is built this way...) 

```c
#include "some_api.h"

int main() {
    Reader r = reader_new_from_file("some_file.txt");
    File f = reader_read_as_new_file(r);
    Image img = image_new_from_file(f);
    
    if(!image_valid(img)) {
        puts("error reading file!");
        exit(EXIT_FAILURE);
    }
    
    // do smth with img
    
    // cleanup
    image_kill(img);
    file_fill(f);
    reader_kill(r);
}
```

### <a name="S-err-error_parameter_options"></a> Error Parameter Options

rhc uses the global: `_Thread_local const char *rhc_error;` to represent errors.
The rhc function `error` sets and returns `rhc_error`.

So an error reporting function may be:
```c
typedef const char *err;

// bad examaple, to simple function, but you get it...
err divide(int *out, int a, int b) {
    if(b==0) {
        return rhc_error = "Division with 0";
    }
    *out = a/b;
    return NULL;
}
```

Another way would be to return an invalid type and set `rhc_error`:
```c
bool string_valid(String self) {
    return self.s.data && !str_empty(self.s);
}

String string_cat(Str_s a, Str_s b) {
    if(str_empty(a) && str_empty(b)) {
        rhc_error = "invalid members";
        return string_new_invalid();
    }
    // ...
}
```



## <a name="S-naming"></a>Naming
A consistent naming sheme is generelly usefull for all kinds of stuff, but especially in programming and its very very important in C programming.
In the C programming world, there is no specific definition of, for example, a class and its methods.
Its the programmers task to use a naming sheme, so that the reader can easily see whats going on.
With autocompletion, there is no need for small names like strtof. The programmer should always write readable code, with only common and/ or good abbreviations.


### <a name="S-naming-variables"></a>Variables
I prefer to use snake_case names for variables, if you want to use a member of a struct and you have no clue, use its lowercase name or a good abbreviation.
```c
int car_petrol;
FILE *file;
Str_s str;
IntIterator_i iter;
```

### <a name="S-naming-functions"></a>Functions
Like the variables [above](#S-naming-variables), I also prefer snake_case names.
```c
FILE *open_and_check(const char *filepath);
IntSet_s set_diff(IntSet_s a, IntSet_s b);
int max(int a, int b);
```

### <a name="S-naming-macros"></a>Macros
Lots of C programmers use SCREAM_CASE for macros. But it leads to errors if these are reset by other libraries.
If you want to use SCREAM_CASE, always use a namespace prefix like MYLIB_SCREAM_CASE (MYLIB should be replaced...).

IMHO you should avoid macros, if possible, and use functions instead.
The compiler is very good at inlining known function implementations.

For function like macros, i prefer function style naming (as with stdc):
```c
#define max(a, b) ((a) > (b) ? (a) : (b)) // small function macro
#define free0(ptr) do {free(ptr); ptr=NULL;} while(0) // procedur macro
```

Macros that generate stuff (use them as little as possible!) are in SCREAM_CASE:
```c
#define NS_GENERATE_INC(type) \
type inc_ ## type (type t) { \
    return t++; \
}
```


### <a name="S-naming-structs"></a>Structs
Structs can occour in three [code areas](#S-naming-structs-area):
+ Implementation
+ Interface header with uncommon use
+ Interface header with common use

With one of the following three [use cases](#S-naming-structs-usecases):
+ Autotype Structs
+ Structs that needs to be freed/ killed
+ Classes

#### <a name="S-naming-structs-area"></a>Code Areas
Within an implementation, or when commonly used in an interface header,
create the struct with a typedef:

```c
// Implementation & Interface header with common use

typedef struct {
    int a, b, c;
} Foo_s;

// or
typedef struct Item {
    int i;
    struct Item *next;
} Item;
```

If its a not commonly used struct in an interface header,
don't use a typedef. The user can than self decide if he want to create it.
In this way, the name of the struct is not wasted for the user (except for structs).

```c
// Interface header with uncommon use

struct Uncommen_s {
    uint8_t data[128];
    bool mode;
};

// the user could instantiate it like so:
struct Uncommen_s uc;
```


#### <a name="S-naming-structs-usecases"></a>Use Cases
As explained in Chapter [Prefer autotypes](#S-basics-autotypes), you should always prefer autotype structs.
Autotype structs should be marked, so the user can directly identify them.
There is a lot of code using `*_t` for struct members, but thats reserved for C and compiler devs!,
So Im using PascalCase_s for autotype structs. 

```c
// Autotype structs

typedef struct {
    float x, y;
} Point_s;

typedef struct {
    Point_s tl, br; // top left, bottom right
} Rect_s;

typedef struct {
    Point_s center;
    float width, height;
    float angle;
} RotatedRect_s;

typedef struct {
    Point_s data[1024];
    int size;
} PointArray_s;

typedef struct {
    int render_options;
    int state;
    bool mode3d;
} RenderStateAttributes_s;
```
Have a look at [Mathc](https://github.com/renehorstmann/Mathc) for a linear algebra math library,
that contains structs for vectors and matrices (`vec3`, `mat4`)

Structs that own data on heap, or classes, needs to be killed/ freed.
There is a fluid transition between these two, so I treat them the same.
For marking, I use PascalCase for their names.
With this convention, the user directly sees at their instantiation, that he needs to kill them somewhere.

```c
// Classes

typedef struct {
    char *str;
} String;

// Destructor:
void string_kill(String *self) {
    rhc_free(self->str);
    self->str = NULL;
}

// Method (ln = lineend)
// If the method will not change data, dont use a pointer of self (copy)
// Or use const String *self, if String is somewhat big
void string_println(String self) {
    puts(self.str);
}


typedef struct {
    int *data;
    int size;
} IntArray;

// Constructor
IntArray int_array_new(int size) {
    IntArray self;
    // calloc + assume, see rhc/alloc.h
    self.data = rhc_calloc(sizeof *self.data * size);
    self.size = size;
    return self
}

// Destructor
void int_array_kill(IntArray *self) {
    rhc_free(self->data);
    self->data = NULL;
    self->size = NULL;
}

// Method
void int_array_push(IntArray *self, int append) {
    // realloc + assume, see rhc/alloc.h
    self->data = rhc_realloc(self->data, sizeof *self.data * ++self->size);
    self->data[self->size-1] = append;
}
```

### <a name="S-naming-classes"></a>Classes
As seen in the previos example above, I prefer PascalCase for classes.
The data section of the class gets the ClassName.
The constructor is called class_name_new and the destructor class_name_kill.
All methods also use this naming sheme, like class_name_length.
With this style and an ide with autocompletion, the user gets a similar feeling to an object orientated language.


### <a name="S-naming-namespaces"></a>Namespaces
If you write a small library with a handful of good used names in the interface, you must not use a namespace.
For example a library that loads a .csv file, can use an interface header like this:

```c 
// No namespace needed (ok, csv is the namespace, but you get it...)

// csv.h
#ifndef CSV_H
#define CSV_H

/**
 * Loads a .csv file into heap memory.
 * @param out_array: Pointer to the allocated data array
 * @param file: The .csv file to load (relative or absolute path, '~' for home)
 * @returns: the number of loaded fields, or -1 on error
 */
int load_csv_file_to_heap_array(float **out_array, const char *file);

/**
 * Saves a .csv file into the given file.
 * @param file: The .csv file to save into (relative or absolute path, '~' for home)
 * @returns: the number of saved fields, or -1 on error
 */
int save_csv_file(const char *file, const float *array, int n);

#endif //CSV_H
```

When your library gets bigger and/ or types get into the interface header, that will be part of interfaces for the user, a namespace is needed.
A namespace is a simple and very short prefix for all names in your interface and your globals.
A geometry library may look like the following:

```c
// Namespace (geo)

// geo/types.h
#ifndef GEO_TYPES_H
#define GEO_TYPES_H

typedef float geo_vec[2];

// Autotype struct
typedef struct {
    float x, y;
} GeoPoint_s;

typedef struct {
    GeoPoint_s center;
    float radius;
} GeoCircle_s;


// Class
typedef struct {
    GeoPoint_s *data;
    int size;
} GeoPointArray;

void geo_point_array_kill(GeoPointArray *self) {
    free(self->data);
    self->data = NULL;
    self->size = 0;
}

#endif // GEO_TYPES_H



// geo/intersection.h
#ifndef GEO_INTERSECTION_H
#define GEO_INTERSECTION_H

#include "types.h"

/**
 * @returns: all points that lie in the circle
 */
GeoPointArray geo_points_in_circle(GeoPoint_s *array, int n, GeoCircle_s circle);

#endif // GEO_INTERSECTION_H
```
Have a look at [Mathc](https://github.com/renehorstmann/Mathc) for a linear algebra math library,
that contains structs for vectors and matrices (`vec3`, `mat4`)


## <a name="S-oo"></a>Object Orientation in C
Although the C programming language doesn't support object orientated programming nativly,
it's still possible and quite easy.

### <a name="S-oo-not"></a>When not to use
Unlike many do, you should NOT use OOP in every scenario!
In most cases its unaccesary to use all features of it and it can slow down your program.
Imagine you write a game in an OOP manner with the following hierarchy:
+ Item (base class)
    - Invisible
        - Ghost
    - Visible
        - Tree
        - TreasureChest
        - Moveable
            - Player
            - Enemy

So you could have a list of all items (unsorted) and a loop that renders each with an overloaded method render.
This is incredible slow for a normal modern CPU, because of cache misses.
A slightly better approach would be to list all enemies packed in a seperate list and render these.
A much better approuch is to pack all data that is necessary to render an enemy and loop over this list.
In the performance critical section focus on data, not code.
If you still want an OO-Hierarchy, take the focos on the data, and than let your class point to the data, instead to own it.


### <a name="S-oo-simple"></a>Simple machine
A little example of a simple "machine" class was already shown in chapter [Naming structs (use cases)](#S-naming-structs-usecases).
If you know that there will never be more than one instance of your class, go the procedure way.
Examples for this style include loggers and systeminfo machines.

```c
// max. 1 instance of Foo

//
// foo.h
//

// public data
struct FooGlobals_s {
    int cnt;
};
extern struct FooGlobals_s foo;

// constructor
void foo_init();

// destructor
void foo_kill();

// methods:
void foo_add(int add);

void foo_print();


//
// foo.c
//

// public data
sruct FooGlobals_s foo;

// private data
static struct {
  int internal_cnt;
} L;

void foo_init() {
    foo.cnt = 1;
    L.internal_cnt = -1;
}

void foo_kill() {
    // free memory, close files...
    foo.cnt = 0;
}

void foo_add(int add) {
    foo.cnt += add;
}

void foo_print() {
    printf("foo %d\n", foo.cnt+L.internal_cnt);
}

```

I like to call constructors class_name_init / _new and destructors class_name_kill.
If you stick with this, or another name, your users can easily find them for other classes as well.
Destructors should always have the function form: void(ClassName *self)
The same "machine", but with multiple possible instances looks like the following.
This may be the best style to do a class, but the data (also private data) must be declared in the header, just like C++.

```c
// multiple instances of foo possible

//
// foo.h
//

typedef struct {
    // public data
    int cnt;
  
    // private data
    struct {
        int internal_cnt;
    } L;
} Foo;

// constructor
Foo foo_new();

// destructor
void foo_kill(Foo *self);

// methods:
void foo_add(Foo *self, int add);

void foo_print(const Foo *self);


//
// foo.c
//

#include "foo.h" 

Foo foo_new() {
    Foo self;
    self.cnt = 1;
    self.L.internal_cnt = -1;
    return self;
}

void foo_kill(Foo *self) {
    // free memory, close files...
    self->cnt = 0;
}

void foo_add(Foo *self, int add) {
    self->cnt += add;
}

void foo_print(const Foo *self) {
    printf("foo %d\n", self->cnt+self->L.internal_cnt);
}

```

If you want to hide stuff from the class data in the header, use the pimpl (pointer to implementation) idiom, or return a malloced class on new:


```c
// multiple instances of foo possible, data hidden.

//
// foo.h
//

typedef struct {
    int cnt;
} Foo;
// If the class name has a _s at the end, it is trivially copyable
// If not (like in this case), a _copy method should be used

// constructor
Foo *foo_new();

// destructor
void foo_kill(Foo **self_ptr);

// copy method
Foo *foo_copy(const Foo *self);

// getter and setter

// read only
int foo_get_bar(const Foo *self);

// methods:
void foo_add(Foo *self, int add);

void foo_print(const Foo *self);


//
// foo.c
//

#include "foo.h" 

struct FooImpl {
    Foo public;
    int bar;
};


Foo *foo_new() {
    // malloc + assume, see rhc/alloc.h
    FooImpl *impl = rhc_malloc(sizeof *impl);
    Foo *self = &impl->public;
    self->cnt = 1;
    impl->bar= 123;
    return self;
}

void foo_kill(Foo **self_ptr) {
    // invalid safe
    if(!self_ptr) return;

    // free data and set the ptr to NULL
    rhc_free(*self_ptr);
    *self_ptr = NULL;
}

Foo *foo_copy(const Foo *self) {
    // malloc + assume, see rhc/alloc.h
    FooImpl *copy = rhc_malloc(sizeof *copy);
    memcpy(copy, self, sizeof *copy);
    return copy;
}

int foo_get_bar(const Foo *self) {
    FooImpl *impl = (FooImpl *) self;
    return impl->bar;
}

void foo_add(Foo *self, int add) {
    self->cnt += add;
}

void foo_print(const Foo *self) {
    const FooImpl *impl = (const FooImpl *) self;
    printf("foo %d\n", self->cnt+impl->bar);
}

```



### <a name="S-oo-inheritance"></a>Inheritance
Deriving from a class is easy in C. But its important that your users know the base class.
In the following example, the class Child derives (static) from the class Mother:
(static = no run time information and no virtual methods -> the user must call the right methods himself)

```c

// class Mother
typedef struct {
    char *data;
    int a;
} Mother;

Mother mother_new(int amount) {
    Mother self;
    // malloc + assume, see rhc/alloc.h
    self.data = rhc_malloc(amount);
    self.a = amount;
    return Mother
}

void mother_kill(Mother *self) {
    rhc_free(self->data);
    self->data = NULL;
    self->a = 0;
}

void mother_print(const Mother *self, int pos) {
    assert(pos>=0 && pos<self->a);
    printf("%c", self->data[pos]);
}

// derived class Child
typedef struct {
    // include data of mother at first place
    Mother super;

    // public data of Child
    int b;
} Child;

Child child_new(int beta) {
    Child self;
    // call super.init
    // casting to Mother works, 
    //   because the first data in Child is Mother
    self.super = mother_new(beta*2);
    self->b = beta;
}

void child_kill(Child *self) {
    mother_kill((Mother *) self);
    self->b = 0;
}

int child_length(const Child *self) {
    return self->super.a - self->b;
}


// Usage:
int main() {
    Child c = child_new(10);

    int len = child_length(&c);
    
    // Call mother method:
    mother_print((Mother *) &c, 5)
    
    // or...
    mother_print(&c.super, len);

    child_kill(&c);
}

```


### <a name="S-oo-rtti"></a>RTTI
Run time type information is needed, to determine the type of a class at runtime (dynamic_cast/ isinstance/ instanceof/ etc.).
To achieve this, the root base class should have an identification string (as char array autotype).
Or all root base classes inherit from an Object class that implements the string.
These strings contain a chained list of the class name hierarchy.
For example if class Bar and class Pub derive from class Foo, their type names would be:
+ class Foo - type "Foo"
    - class Bar : Foo - type "FooBar"
    - class Pub : Foo - type "FooPub"
+ class Car - type "Car"

```c

typedef struct {
    char type[64];
} Object;

Object object_new(const char *type) {
    Object self;
    strcpy(self.type, type);
    return self;
}

void *object_as_instance(void *object, const char *type) {
    if(strncmp(object, type, strlen(type)) == 0) 
        return object;
    return NULL;
}


// class Foo
typedef struct {
    Object super;

    int a;
} Foo;

const char *FOO_TYPE = "Foo";

Foo foo_new() {
    Foo self;
    self.super = object_new(FOO_TYPE);
    self.a = 1;
    return self;
}

// class Bar : Foo
typedef struct {
    Foo super;

    int b;
} Bar;

const char *BAR_TYPE = "FooBar";

Bar bar_new() {
    Bar self;
    self.super = foo_new();
    *((Object *) &self) = object_new(BAR_TYPE);
    self.b = 2;
    return self;
}

// class Pub : Foo
typedef struct {
    Foo super;

    int p;
} Pub;

const char *PUB_TYPE = "FooPub";

Pub pub_new() {
    Pub self;
    self.super = foo_new();
    *((Object *) &self) = object_new(PUB_TYPE);
    self.p = 3;
    return self;
}


// class Car
typedef struct {
    Object super;

    int color;
} Car;

const char *CAR_TYPE = "Car";

Car car_new() {
    Car self;
    self.super = object_new(CAR_TYPE);
    self.color = 0xff00ff;
    return self;
}


// usage
int main() {
    Bar b = bar_new();

    Foo *as_foo = object_as_instance(&b, Foo_TYPE);
    if(as_foo)
         puts("Bar is a Foo");
    
    Bar *as_bar = object_as_instance(as_foo, Bar_TYPE);
    if(as_bar)
         puts("Bar that was casted to Foo still is a Bar");

    Pub *as_pub = object_as_instance(as_foo, Pub_TYPE);
    assert(!as_pub);

    Car *as_car = object_as_instance(as_foo, Car_TYPE); 
    assert(!as_car);
}
```

### <a name="S-oo-virtual"></a>Virtual Methods
With virtual methods, the user must not know the exact type, to call the right overloaded class function (method).
As with the rest of OOP, its easy to implement in C:

```c
// Class Foo
struct Foo;

// Virtual function types
typedef void (*foo_print_function)(const struct Foo *self);
typedef int (*foo_add_function)(struct Foo *self, int add);

typedef struct Foo {
    // public data
    int f;

    // vtable (function ptr of the virtual methods)::
    foo_print_function print;
    foo_add_function add;
} Foo;

void foo_print(const Foo *self) {
    printf("Foo(%d)\n", self->f);
}

int foo_add(Foo *self, int add) {
    int f = self->f;
    self->f += add;
    return f;
}

Foo foo_new() {
    Foo self;
    self.f = 1;
    self.print = foo_print;
    self.add = foo_add;
    return self;
}


// Class Bar : Foo
typedef struct {
    Foo super;

    // public data of Bar
    float b;
} Bar;

void bar_print(const Bar *self) {
    printf("Bar(%d,%f)\n", self->super.f, self->b);
}

int bar_add(Bar *self, int add) {
    // call super.add
    int foo = foo_add((Foo *) self, add);

    self->b += (float) foo;
    return foo;
}

Bar bar_new(float init) {
    Bar self;
    // call super.new
    self.super = foo_new();

    self.b = init;

    // change overloaded vtable methods
    self.super.print = (foo_print_function) bar_print;  // optional cast...
    self.super.add = (foo_add_function) bar_add;
    return self;
}


// Usage
int main() {
    Foo foo = foo_new();

    Bar bar = bar_new(1.23f);


    Foo *foos[2] = {&foo, (Foo *) &bar}; // optional cast...

    for(int i=0; i<2; i++) {
        Foo *f = foos[i];
        f->add(f, 10);
        f->print(f);
    }

}

```




### <a name="S-oo-interfaces"></a>Interfaces
Often interfaces perform a better job, compared to inheritance, to provide an easy OOP-feel.
An interface only consists of virtual methods and so is like an abstract class without data.
In C you also must pass an void * or keep space in the printable struct for the implementation.

```c
typedef struct Printable_i {
    void *user_data
    void (*print)(struct Printable_i);
} Printable_i;


// class Foo, must be a malloc class, that is not trivially copyable (_s)
typedef struct {
    float f;

    Printable_i printable;
} Foo;

// function that takes a Printable
void bar(Printable_i p, int n) {
    for(int i=0; i<n; i++) 
        p.print(p);
}



void foo_print(Printable_i p) {
    Foo *foo = (Foo *) p.user_data;
    printf("Foo(%f)\n", foo->f);
}

// Must be an allocated constructor 
Foo *foo_new() {
    // calloc + assume, see rhc/alloc.h
    Foo *self = rhc_calloc(sizeof *self);
    self->f = 0;
    self->printable = (Printable_i) {
        self,
        foo_print
    };
    return self;
}

void foo_kill(Foo **self_ptr) {
    rhc_free(*self_ptr);
    *self_ptr = NULL;
}


// usage
int main() {
    Foo *foo = foo_new();
    foo->f = 1.23f;
    
    bar(foo->printable, 3);

    foo_kill(&foo);
}

```
