# rhc
A C standard library addition.

## Getting started
Copy the headers into your project and include `rhc_impl.h` once in a single source file.

## Components
- [types.h](include/rhc/types.h) containing some basic types, like `Allocation_s, Str_s, String`
- [error.h](include/rhc/error.h) error management and `assume` (`assert` in runtime)
- [log.h](include/rhc/log.h) logging
- [time.h](include/rhc/time.h) monotonic time as `double` in seconds
- [allocator.h](include/rhc/allocator.h) default allocators (`malloc, ...`) for `Allocator_s`
- [file.h](include/rhc/file.h) read, write and append files
- [str.h](include/rhc/str.h) functions, working on string views with `Str_s`
- [string.h](include/rhc/string.h) `String` class, that holds a string
- [str_parse.h](include/rhc/str_parse.h) parse values (`int, float, ...`) from a `Str_s`
- [rhc.h](include/rhc/rhc.h) includes all of the above headers
- [rhc_impl.h](include/rhc/rhc_impl.h) include this header in a single source file
- [dynarray.h](include/rhc/dynarray.h) dynamic array like (C++'s `std::vector`)
- [hashmap_string.h](include/rhc/hashmap_string.h) hashmap with strings as key
- [hashmap.h](include/rhc/hashmap.h) hashmap base

## Safety
All functions should operate on not valid input.
Types have a function called *_valid() to check if the type is in a valid context.
Some functions set the type invalid and may log and set `rhc_error` to an error.
With this style, the following [example](examples/safety.c) is possible and safe:
```c
// only in a single header file, to implement some rhc source functions
//    use rhc/rhc.h or single headers instead
#include "rhc/rhc_impl.h"

int main() {

    // should end in an invalid String
    String file = file_read("not_available.txt", true);  // ascii=true
    // produces a log_warn: file read failed: not_available.txt

    // should not break
    str_tolower(file.str);

    // should return 0
    int cnt = str_count(file.str, 'a');

    // should not write and set ok to false
    bool ok = file_write("not_available.txt", file.str, true);  // ascii=true
    // producus a log_error: file write failed, content invalid: not_available.txt
    // sets rhc_error = file write failed, content invalid

    printf("cnt: %i, file_valid: %i, ok: %i, error_msg: %s\n", cnt, string_valid(file), ok, rhc_error);
    // cnt: 0, file_valid: 0, ok: 0, error_msg: file write failed, content invalid
}
```
If you want to extend the code, for example with a function that loads an image from a `Str_s`, use the valid pattern.

## Types
The [types.h](include/rhc/types.h) header defines some useful additional structs and is included in most other headers.
- `Allocator_s`
  - holds virtual functions for `malloc`, `realloc` and `free`
  - Every function in rhc, that allocates and holds memory, uses and owns an `Allocator_s`
  - If a function needs an `Allocator_s`, chances are, that their will be two versions:
    - `some_fn_*_a`: takes the `Allocator_s` as last parameter
    - `some_fn_*:` calls `*_a` with the default `Allocator_s`
  - see [allocator.h](include/rhc/allocator.h) to get an `Allocator_s`.
    - `allocator_new_default()`: the default `malloc`, `realloc`, `free` `Allocator_s`.
    - `allocator_new_raising()`: also uses the default allocators, but will raise a signal, if the memory couldn't be allocated
- `Str_s`
  - represents a view on a string, ignoring the null terminator (so may not be null terminated)
  - `char *data`: pointer to the first char
  - `size_t size`: length of the string
  - see [str.h](include/rhc/str.h) for functions working on a `Str_s`
- `String`
  - holds a string in memory, via an `Allocator_s` and a `Str_s`
  - in contrast to `Str_s`, a `String` is always null terminated
  - call `string_kill` to free the data via its `Allocator_s`
  - see [string.h](include/rhc/string.h) for methods working on a `String`

## Style
For a detailed style guide, see [style.md](style.md).
