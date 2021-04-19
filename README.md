# rhc
A new approach as a C standard library addition.

## Getting started
Copy the headers into your project and include `rhc_impl.h` once in a single source file.

## Components
- [types.h](include/rhc/types.h) containing the basic types, like `Allocation_s, Str_s, String`
- [error.h](include/rhc/error.h) error management and `assume` (`assert` in runtime)
- [log.h](include/rhc/log.h) logging
- [time.h](include/rhc/time.h) monotonic time as `double` in seconds
- [alloc.h](include/rhc/alloc.h) default allocators (`malloc, ...`) for `Àllocation_s`
- [file.h](include/rhc/file.h) read, write and append files
- [str.h](include/rhc/str.h) functions, working on string views with `Str_s`
- [string.h](include/rhc/string.h) `String` class, that holds a string
- [str_parse.h](include/rhc/str_parse.h) parse values (`int, float, ...`) from a `Str_s`
- [rhc.h](include/rhc/rhc.h) includes all of the above headers
- [rhc_impl.h](include/rhc/rhc_impl.h) include this header in a single source file
- [dynarray.h](include/rhc/dynarray.h) dynamic array like `std::vector` in C++
- [hashmap_string.h](include/rhc/hashmap_string.h) hashmap with strings as key
- [hashmap.h](include/rhc/hashmap.h) hashmap base

## Safety
All functions should operate on not valid input.
Types have a function called *_valid() to check if the type is in a valid context.
Some functions set the type invalid and may log and set `rhc_error` to an error.
With this style, the following [example](examples/safety.c) is possible:
```c
// only in a single header file, to implement some rhc source functions
#include "rhc/rhc_impl.h"

int main() {

    // should end in an invalid String
    String file = file_read("not_available.txt", true);
    // produces a log_warn: file read failed: not_available.txt

    // should not break
    str_tolower(file.str);

    // should return 0
    int cnt = str_count(file.str, 'a');

    // should not write and set ok to false
    bool ok = file_write("not_available.txt", file.str, true);
    // producus a log_error: file write failed, content invalid: not_available.txt
    // sets rhc_error = file write failed, content invalid

    printf("cnt: %i, file_valid: %i, ok: %i, error_msg: %s\n", cnt, string_valid(file), ok, rhc_error);
    // cnt: 0, file_valid: 0, ok: 0, error_msg: file write failed, content invalid
}
```
If you want to extend the code, for example with a function that loads an image from a `Str_s`, use the valid pattern.