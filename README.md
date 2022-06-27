# rhc
A C standard library addition.

## Getting started
Copy the headers into your project and include `rhc_impl.h` once in a single source file.

## Components
- [rhc.h](include/rhc/rhc.h) includes the basics of rhc:
  - [common.h](include/rhc/common.h) contains some default system libraries and a few datatypes like rhcsize
  - [error.h](include/rhc/error.h) error management
  - [assume.h](include/rhc/assume.h) `rhc_assume` is a runtime assumption (like `assert` for runtime)
  - [memory.h](include/rhc/memory.h) rhc_malloc, etc. that use `rhc_assume`
  - [log.h](include/rhc/log.h) logging
- [rhc_full.h](include/rhc/rhc_full.h) includes all of rhc (in addition to [rhc.h](include/rhc/rhc.h))
  - [allocator.h](include/rhc/allocator.h) Allocator interface and functions + arena allocator
  - [time.h](include/rhc/time.h) monotonic time as `double` in seconds and a `RhcTimer_s`
  - [terminalcolor.h](include/rhc/terminalcolor.h) Set terminal colors for stdout
  - [endian.h](include/rhc/endian.h) host to little or big endian and vice versa
  - [stream.h](include/rhc/stream.h) Interface and functions to read and write on streams
  - [file.h](include/rhc/file.h) read, write and append files, open file with `RhcStream_i` interface
  - [socket.h](include/rhc/socket.h) client and server tcp sockets with `RhcStream_i` interface
  - [str.h](include/rhc/str.h) functions, working on string views with `RhcStr_s`
  - [string.h](include/rhc/string.h) `RhcString` class, that holds a string
  - [str_parse.h](include/rhc/str_parse.h) parse/serialize values (`int, float, ...`) from and into a `RhcStr_s`
- [dynarray.h](include/rhc/dynarray.h) dynamic array like (C++'s `std::vector`)
- [hashmap_string.h](include/rhc/hashmap_string.h) hashmap with strings as key
- [hashmap.h](include/rhc/hashmap.h) hashmap base
- [rhc_impl.h](include/rhc/rhc_impl.h) include this header in a single source file

## Safety
All functions should operate on none valid input.
Types have a function called *_valid() to check if the type is in a valid context.
Some functions set the type invalid and may log and set `rhc_error` to an error.
With this style, the following [example](examples/safety.c) is possible and safe:
```c
// only in a single header file, to implement some rhc source functions
//    use rhc/rhc.h or single headers instead
#include "rhc/rhc_impl.h"

int main() {

    // should end in an invalid RhcString
    RhcString *file = rhc_file_read("not_available.txt", true);
    // produces a log_warn: file read failed: not_available.txt

    // returns an invalid str, if file is not valid
    RhcStr_s str = rhc_string_get_str(file);

    // should not break
    rhc_str_tolower(str);

    // should return 0
    rhcsize cnt = rhc_str_count(str, 'a');

    // should not write and set ok to false
    bool ok = rhc_file_write("also_not_available.txt", str, true);
    // producus a log_error: file write failed, content invalid: not_available.txt
    // sets rhc_error = file write failed, content invalid

    printf("cnt: %i, rhc_file_valid: %i, ok: %i, error_msg: %s\n",
           (int) cnt, rhc_string_valid(file), ok, rhc_error_get());
    // cnt: 0, rhc_file_valid: 0, ok: 0, error_msg: file write failed, content invalid
}

```
If you want to extend the code, for example with a function that loads an image from a `RhcStr_s`, use the valid pattern.

## Style
For a detailed style guide, see [style.md](style.md).

## Template
To change the `rhc` namespace, have a look at the python script [template.py](template/template.py).
It copies files from `in` to `out` and uses regex to replace the namespace to a user namespace.

The [some](https://github.com/renehorstmann/some) framework (C engine for desktop, mobile, web) uses `s` as `rhc` namespace replacement

# Author

René Horstmann

# License

rhc is licensed under the [MIT License](LICENSE)
